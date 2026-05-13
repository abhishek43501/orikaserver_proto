#pragma once

#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <vector>
#include <unordered_map>
#include <functional>
#include <mutex>
#include <atomic>
#include <string>

#include "ClientContext.h"

namespace iocp {

class IOCPServer {
public:
    using OnConnectFn    = std::function<void(ClientContext*)>;
    using OnDisconnectFn = std::function<void(unsigned long long clientId)>;
    using OnDataFn       = std::function<void(ClientContext*, const char* data, int length)>;
    using OnErrorFn      = std::function<void(const std::string& message, int code)>;

    IOCPServer();
    ~IOCPServer();

    IOCPServer(const IOCPServer&)            = delete;
    IOCPServer& operator=(const IOCPServer&) = delete;

    bool Start(unsigned short port, int workerThreadCount = 0, int pendingAccepts = 16);
    void Stop();

    bool Send(unsigned long long clientId, const char* data, int length);
    int  Broadcast(const char* data, int length);
    bool Disconnect(unsigned long long clientId);
    int  GetClientCount();

    void SetOnConnect(OnConnectFn cb)       { m_onConnect    = std::move(cb); }
    void SetOnDisconnect(OnDisconnectFn cb) { m_onDisconnect = std::move(cb); }
    void SetOnData(OnDataFn cb)             { m_onData       = std::move(cb); }
    void SetOnError(OnErrorFn cb)           { m_onError      = std::move(cb); }

private:
    static unsigned __stdcall WorkerThreadProc(void* param);
    static unsigned __stdcall AcceptThreadProc(void* param);

    void   WorkerLoop();
    void   AcceptLoop();
    bool   PostAccept(PerIoContext* ioCtx);
    bool   PostRecv(ClientContext* client);
    void   HandleAccept(PerIoContext* ioCtx, DWORD bytesTransferred);
    void   HandleRecv(ClientContext* client, DWORD bytesTransferred);
    void   HandleSend(ClientContext* client, DWORD bytesTransferred);
    void   CloseClient(ClientContext* client);
    void   ReleaseClient(ClientContext* client);
    void   AddRefClient(ClientContext* client);
    void   ReportError(const std::string& msg, int code);

    SOCKET                              m_listenSocket;
    HANDLE                              m_iocp;
    std::vector<HANDLE>                 m_workerThreads;
    HANDLE                              m_acceptThread;
    std::atomic<bool>                   m_running;
    unsigned short                      m_port;
    int                                 m_pendingAccepts;

    LPFN_ACCEPTEX                       m_lpfnAcceptEx;
    LPFN_GETACCEPTEXSOCKADDRS           m_lpfnGetAcceptExSockaddrs;

    std::vector<PerIoContext*>          m_acceptCtxPool;

    std::mutex                                              m_clientsMtx;
    std::unordered_map<unsigned long long, ClientContext*>  m_clients;
    std::atomic<unsigned long long>                         m_nextClientId;

    OnConnectFn     m_onConnect;
    OnDisconnectFn  m_onDisconnect;
    OnDataFn        m_onData;
    OnErrorFn       m_onError;
};

} // namespace iocp
