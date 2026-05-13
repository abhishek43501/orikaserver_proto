#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

namespace iocp {

enum IoOperation {
    IO_ACCEPT = 0,
    IO_RECV   = 1,
    IO_SEND   = 2
};

#ifndef IOCP_BUFFER_SIZE
#define IOCP_BUFFER_SIZE 8192
#endif

struct PerIoContext {
    OVERLAPPED  overlapped;
    WSABUF      wsaBuf;
    char        buffer[IOCP_BUFFER_SIZE];
    DWORD       bytesTransferred;
    IoOperation operation;
    SOCKET      acceptSocket;
};

struct ClientContext {
    SOCKET          socket;
    SOCKADDR_IN     remoteAddr;
    PerIoContext    recvCtx;
    PerIoContext    sendCtx;
    CRITICAL_SECTION lock;
    LONG            refCount;
    LONG            closing;
    unsigned long long clientId;
};

} // namespace iocp
