#include "IOCPServer.h"

#include <process.h>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")

namespace iocp {

namespace {

constexpr DWORD kAcceptAddrLen = sizeof(SOCKADDR_IN) + 16;

bool LoadAcceptExFns(SOCKET s,
                     LPFN_ACCEPTEX* outAcceptEx,
                     LPFN_GETACCEPTEXSOCKADDRS* outGetSockaddrs) {
    GUID guidAcceptEx = WSAID_ACCEPTEX;
    GUID guidGetSock  = WSAID_GETACCEPTEXSOCKADDRS;
    DWORD bytes = 0;

    if (WSAIoctl(s, SIO_GET_EXTENSION_FUNCTION_POINTER,
                 &guidAcceptEx, sizeof(guidAcceptEx),
                 outAcceptEx, sizeof(*outAcceptEx),
                 &bytes, NULL, NULL) == SOCKET_ERROR) {
        return false;
    }
    if (WSAIoctl(s, SIO_GET_EXTENSION_FUNCTION_POINTER,
                 &guidGetSock, sizeof(guidGetSock),
                 outGetSockaddrs, sizeof(*outGetSockaddrs),
                 &bytes, NULL, NULL) == SOCKET_ERROR) {
        return false;
    }
    return true;
}

} // namespace

IOCPServer::IOCPServer()
    : m_listenSocket(INVALID_SOCKET)
    , m_iocp(NULL)
    , m_acceptThread(NULL)
    , m_running(false)
    , m_port(0)
    , m_pendingAccepts(16)
    , m_lpfnAcceptEx(nullptr)
    , m_lpfnGetAcceptExSockaddrs(nullptr)
    , m_nextClientId(1) {

    WSADATA wsa = {};
    WSAStartup(MAKEWORD(2, 2), &wsa);
}

IOCPServer::~IOCPServer() {
    Stop();
    WSACleanup();
}

bool IOCPServer::Start(unsigned short port, int workerThreadCount, int pendingAccepts) {
    if (m_running.load()) return false;

    m_port           = port;
    m_pendingAccepts = pendingAccepts > 0 ? pendingAccepts : 16;

    if (workerThreadCount <= 0) {
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        workerThreadCount = si.dwNumberOfProcessors * 2;
        if (workerThreadCount < 2) workerThreadCount = 2;
    }

    m_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (!m_iocp) {
        ReportError("CreateIoCompletionPort failed", GetLastError());
        return false;
    }

    m_listenSocket = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (m_listenSocket == INVALID_SOCKET) {
        ReportError("WSASocket(listen) failed", WSAGetLastError());
        return false;
    }

    BOOL reuse = TRUE;
    setsockopt(m_listenSocket, SOL_SOCKET, SO_REUSEADDR,
               reinterpret_cast<const char*>(&reuse), sizeof(reuse));

    SOCKADDR_IN addr = {};
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port        = htons(port);

    if (bind(m_listenSocket, reinterpret_cast<SOCKADDR*>(&addr), sizeof(addr)) == SOCKET_ERROR) {
        ReportError("bind failed", WSAGetLastError());
        closesocket(m_listenSocket);
        m_listenSocket = INVALID_SOCKET;
        return false;
    }

    if (listen(m_listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        ReportError("listen failed", WSAGetLastError());
        closesocket(m_listenSocket);
        m_listenSocket = INVALID_SOCKET;
        return false;
    }

    if (!LoadAcceptExFns(m_listenSocket, &m_lpfnAcceptEx, &m_lpfnGetAcceptExSockaddrs)) {
        ReportError("Failed to load AcceptEx functions", WSAGetLastError());
        closesocket(m_listenSocket);
        m_listenSocket = INVALID_SOCKET;
        return false;
    }

    if (!CreateIoCompletionPort(reinterpret_cast<HANDLE>(m_listenSocket), m_iocp, 0, 0)) {
        ReportError("Associate listen socket with IOCP failed", GetLastError());
        closesocket(m_listenSocket);
        m_listenSocket = INVALID_SOCKET;
        return false;
    }

    m_running = true;

    for (int i = 0; i < workerThreadCount; ++i) {
        unsigned tid = 0;
        HANDLE h = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, &IOCPServer::WorkerThreadProc, this, 0, &tid));
        if (h) m_workerThreads.push_back(h);
    }

    for (int i = 0; i < m_pendingAccepts; ++i) {
        PerIoContext* ctx = new PerIoContext();
        ZeroMemory(ctx, sizeof(*ctx));
        ctx->operation = IO_ACCEPT;
        ctx->wsaBuf.buf = ctx->buffer;
        ctx->wsaBuf.len = IOCP_BUFFER_SIZE;
        m_acceptCtxPool.push_back(ctx);

        if (!PostAccept(ctx)) {
            ReportError("PostAccept (initial) failed", WSAGetLastError());
        }
    }

    unsigned tid = 0;
    m_acceptThread = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, &IOCPServer::AcceptThreadProc, this, 0, &tid));

    return true;
}

void IOCPServer::Stop() {
    if (!m_running.exchange(false)) return;

    if (m_listenSocket != INVALID_SOCKET) {
        closesocket(m_listenSocket);
        m_listenSocket = INVALID_SOCKET;
    }

    {
        std::lock_guard<std::mutex> lk(m_clientsMtx);
        for (auto& kv : m_clients) {
            ClientContext* c = kv.second;
            if (c && c->socket != INVALID_SOCKET) {
                shutdown(c->socket, SD_BOTH);
                closesocket(c->socket);
                c->socket = INVALID_SOCKET;
            }
        }
    }

    if (m_iocp) {
        for (size_t i = 0; i < m_workerThreads.size(); ++i) {
            PostQueuedCompletionStatus(m_iocp, 0, 0, NULL);
        }
    }

    for (HANDLE h : m_workerThreads) {
        WaitForSingleObject(h, 5000);
        CloseHandle(h);
    }
    m_workerThreads.clear();

    if (m_acceptThread) {
        WaitForSingleObject(m_acceptThread, 1000);
        CloseHandle(m_acceptThread);
        m_acceptThread = NULL;
    }

    if (m_iocp) {
        CloseHandle(m_iocp);
        m_iocp = NULL;
    }

    {
        std::lock_guard<std::mutex> lk(m_clientsMtx);
        for (auto& kv : m_clients) {
            ClientContext* c = kv.second;
            if (c) {
                DeleteCriticalSection(&c->lock);
                delete c;
            }
        }
        m_clients.clear();
    }

    for (PerIoContext* ctx : m_acceptCtxPool) {
        if (ctx->acceptSocket != INVALID_SOCKET && ctx->acceptSocket != 0) {
            closesocket(ctx->acceptSocket);
        }
        delete ctx;
    }
    m_acceptCtxPool.clear();
}

unsigned __stdcall IOCPServer::WorkerThreadProc(void* param) {
    static_cast<IOCPServer*>(param)->WorkerLoop();
    return 0;
}

unsigned __stdcall IOCPServer::AcceptThreadProc(void* param) {
    static_cast<IOCPServer*>(param)->AcceptLoop();
    return 0;
}

void IOCPServer::AcceptLoop() {
    while (m_running.load()) {
        Sleep(100);
    }
}

bool IOCPServer::PostAccept(PerIoContext* ioCtx) {
    SOCKET s = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (s == INVALID_SOCKET) return false;

    ioCtx->acceptSocket = s;
    ZeroMemory(&ioCtx->overlapped, sizeof(OVERLAPPED));
    ioCtx->operation = IO_ACCEPT;

    DWORD bytes = 0;
    BOOL ok = m_lpfnAcceptEx(
        m_listenSocket,
        s,
        ioCtx->buffer,
        0,
        kAcceptAddrLen,
        kAcceptAddrLen,
        &bytes,
        &ioCtx->overlapped);

    if (!ok && WSAGetLastError() != ERROR_IO_PENDING) {
        closesocket(s);
        ioCtx->acceptSocket = INVALID_SOCKET;
        return false;
    }
    return true;
}

void IOCPServer::WorkerLoop() {
    while (true) {
        DWORD        bytesTransferred = 0;
        ULONG_PTR    completionKey    = 0;
        OVERLAPPED*  pOverlapped      = NULL;

        BOOL ok = GetQueuedCompletionStatus(m_iocp, &bytesTransferred,
                                            &completionKey, &pOverlapped, INFINITE);

        if (!m_running.load() && pOverlapped == NULL) {
            break;
        }

        if (pOverlapped == NULL) {
            if (!m_running.load()) break;
            continue;
        }

        PerIoContext* ioCtx = reinterpret_cast<PerIoContext*>(pOverlapped);

        if (ioCtx->operation == IO_ACCEPT) {
            if (!ok) {
                if (m_running.load()) {
                    if (ioCtx->acceptSocket != INVALID_SOCKET) {
                        closesocket(ioCtx->acceptSocket);
                        ioCtx->acceptSocket = INVALID_SOCKET;
                    }
                    PostAccept(ioCtx);
                }
                continue;
            }
            HandleAccept(ioCtx, bytesTransferred);
            continue;
        }

        ClientContext* client = reinterpret_cast<ClientContext*>(completionKey);
        if (!client) continue;

        if (!ok || (bytesTransferred == 0 && ioCtx->operation == IO_RECV)) {
            CloseClient(client);
            ReleaseClient(client);
            continue;
        }

        switch (ioCtx->operation) {
            case IO_RECV:
                HandleRecv(client, bytesTransferred);
                break;
            case IO_SEND:
                HandleSend(client, bytesTransferred);
                break;
            default:
                break;
        }

        ReleaseClient(client);
    }
}

void IOCPServer::HandleAccept(PerIoContext* ioCtx, DWORD /*bytesTransferred*/) {
    SOCKET clientSocket = ioCtx->acceptSocket;

    setsockopt(clientSocket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
               reinterpret_cast<const char*>(&m_listenSocket), sizeof(m_listenSocket));

    SOCKADDR* localAddr  = NULL;
    SOCKADDR* remoteAddr = NULL;
    int       localLen   = 0;
    int       remoteLen  = 0;

    m_lpfnGetAcceptExSockaddrs(
        ioCtx->buffer, 0,
        kAcceptAddrLen, kAcceptAddrLen,
        &localAddr, &localLen,
        &remoteAddr, &remoteLen);

    ClientContext* client = new ClientContext();
    ZeroMemory(client, sizeof(*client));
    client->socket   = clientSocket;
    client->refCount = 1;
    client->closing  = 0;
    client->clientId = m_nextClientId.fetch_add(1);
    if (remoteAddr && remoteLen >= static_cast<int>(sizeof(SOCKADDR_IN))) {
        memcpy(&client->remoteAddr, remoteAddr, sizeof(SOCKADDR_IN));
    }
    InitializeCriticalSection(&client->lock);

    client->recvCtx.operation = IO_RECV;
    client->recvCtx.wsaBuf.buf = client->recvCtx.buffer;
    client->recvCtx.wsaBuf.len = IOCP_BUFFER_SIZE;

    client->sendCtx.operation = IO_SEND;
    client->sendCtx.wsaBuf.buf = client->sendCtx.buffer;
    client->sendCtx.wsaBuf.len = IOCP_BUFFER_SIZE;

    if (!CreateIoCompletionPort(reinterpret_cast<HANDLE>(clientSocket), m_iocp,
                                reinterpret_cast<ULONG_PTR>(client), 0)) {
        ReportError("Associate client socket with IOCP failed", GetLastError());
        closesocket(clientSocket);
        DeleteCriticalSection(&client->lock);
        delete client;
        ioCtx->acceptSocket = INVALID_SOCKET;
        if (m_running.load()) PostAccept(ioCtx);
        return;
    }

    {
        std::lock_guard<std::mutex> lk(m_clientsMtx);
        m_clients[client->clientId] = client;
    }

    if (m_onConnect) m_onConnect(client);

    if (!PostRecv(client)) {
        CloseClient(client);
        ReleaseClient(client);
    }

    ioCtx->acceptSocket = INVALID_SOCKET;
    if (m_running.load()) {
        PostAccept(ioCtx);
    }
}

bool IOCPServer::PostRecv(ClientContext* client) {
    if (!client || client->socket == INVALID_SOCKET) return false;

    AddRefClient(client);

    ZeroMemory(&client->recvCtx.overlapped, sizeof(OVERLAPPED));
    client->recvCtx.operation  = IO_RECV;
    client->recvCtx.wsaBuf.buf = client->recvCtx.buffer;
    client->recvCtx.wsaBuf.len = IOCP_BUFFER_SIZE;

    DWORD bytes = 0;
    DWORD flags = 0;
    int rc = WSARecv(client->socket,
                     &client->recvCtx.wsaBuf, 1,
                     &bytes, &flags,
                     &client->recvCtx.overlapped, NULL);

    if (rc == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING) {
        ReleaseClient(client);
        return false;
    }
    return true;
}

void IOCPServer::HandleRecv(ClientContext* client, DWORD bytesTransferred) {
    if (m_onData) {
        m_onData(client, client->recvCtx.buffer, static_cast<int>(bytesTransferred));
    }
    if (!PostRecv(client)) {
        CloseClient(client);
    }
}

void IOCPServer::HandleSend(ClientContext* /*client*/, DWORD /*bytesTransferred*/) {
    // Send completion. With current single-buffer scheme, nothing else to do here.
}

bool IOCPServer::Send(unsigned long long clientId, const char* data, int length) {
    if (!data || length <= 0 || length > IOCP_BUFFER_SIZE) return false;

    ClientContext* client = nullptr;
    {
        std::lock_guard<std::mutex> lk(m_clientsMtx);
        auto it = m_clients.find(clientId);
        if (it == m_clients.end()) return false;
        client = it->second;
        AddRefClient(client);
    }

    if (!client || client->socket == INVALID_SOCKET) {
        ReleaseClient(client);
        return false;
    }

    EnterCriticalSection(&client->lock);
    memcpy(client->sendCtx.buffer, data, length);
    client->sendCtx.wsaBuf.buf = client->sendCtx.buffer;
    client->sendCtx.wsaBuf.len = length;
    ZeroMemory(&client->sendCtx.overlapped, sizeof(OVERLAPPED));
    client->sendCtx.operation  = IO_SEND;

    DWORD bytes = 0;
    int rc = WSASend(client->socket,
                     &client->sendCtx.wsaBuf, 1,
                     &bytes, 0,
                     &client->sendCtx.overlapped, NULL);
    LeaveCriticalSection(&client->lock);

    if (rc == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING) {
        ReleaseClient(client);
        return false;
    }
    return true;
}

int IOCPServer::Broadcast(const char* data, int length) {
    if (!data || length <= 0) return 0;

    std::vector<unsigned long long> ids;
    {
        std::lock_guard<std::mutex> lk(m_clientsMtx);
        ids.reserve(m_clients.size());
        for (auto& kv : m_clients) ids.push_back(kv.first);
    }

    int sent = 0;
    for (auto id : ids) {
        if (Send(id, data, length)) ++sent;
    }
    return sent;
}

bool IOCPServer::Disconnect(unsigned long long clientId) {
    ClientContext* client = nullptr;
    {
        std::lock_guard<std::mutex> lk(m_clientsMtx);
        auto it = m_clients.find(clientId);
        if (it == m_clients.end()) return false;
        client = it->second;
        AddRefClient(client);
    }
    CloseClient(client);
    ReleaseClient(client);
    return true;
}

int IOCPServer::GetClientCount() {
    std::lock_guard<std::mutex> lk(m_clientsMtx);
    return static_cast<int>(m_clients.size());
}

void IOCPServer::CloseClient(ClientContext* client) {
    if (!client) return;
    if (InterlockedExchange(&client->closing, 1) != 0) return;

    if (client->socket != INVALID_SOCKET) {
        shutdown(client->socket, SD_BOTH);
        closesocket(client->socket);
        client->socket = INVALID_SOCKET;
    }

    unsigned long long id = client->clientId;
    {
        std::lock_guard<std::mutex> lk(m_clientsMtx);
        m_clients.erase(id);
    }

    if (m_onDisconnect) m_onDisconnect(id);
}

void IOCPServer::AddRefClient(ClientContext* client) {
    if (client) InterlockedIncrement(&client->refCount);
}

void IOCPServer::ReleaseClient(ClientContext* client) {
    if (!client) return;
    if (InterlockedDecrement(&client->refCount) == 0) {
        DeleteCriticalSection(&client->lock);
        delete client;
    }
}

void IOCPServer::ReportError(const std::string& msg, int code) {
    if (m_onError) m_onError(msg, code);
}

} // namespace iocp
