#pragma once

#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#include <windows.h>

class SocketServer
{
public:
	static int m_Serverislive;

	static int m_recvThreadSrynk;

	typedef CRITICAL_SECTION	socket_lock;
	struct Socket_iocp_info
	{
		HANDLE h_iocp;
		size_t threads_count;
		HANDLE* h_threads;
	}iocp;

	enum Socket_OVERLAPPED_TYPE {
		RECV = 0,
		SEND,
		CONNECT
	};

	



	enum Socket_ADDRESS_TYPE {
		LOCAL = 0,
		REMOTE
	};

	enum Socket_SOCKET_STATUS {
		NONE = 0x0,
		ACCEPTING = 0x1,
		CONNECTING = 0x2,
		HANDSHAKING = 0x4,
		CONNECTED = 0x8,
		RECEIVING = 0x10,
		SENDING = 0x20,
		CLOSING = 0x40,
		CLOSED = 0x80,
		OPERATING = ACCEPTING | CONNECTING | HANDSHAKING | RECEIVING | SENDING
	};

	SOCKET hClinetHandle;
	SOCKET hServerHandle;

	void serverPrepare();
	//CMutex m_receiveThread_Lock;
	struct m_socket_details
	{
		CSocket pSocket;
		int     m_Netpos;
		int     m_dealing;
	};
	static CMap<int, int, SOCKET, SOCKET> m_socket_detail_Map;
    
};

