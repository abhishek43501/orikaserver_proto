#pragma once

#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#include <windows.h>

#include "openssl/ssl.h"
#include "openssl/err.h"
#include "openssl/x509v3.h"

typedef CRITICAL_SECTION	ssl_lock;

enum OVERLAPPED_TYPE{
	RECV = 0,
	SEND,
	CONNECT
};

enum ADDRESS_TYPE{
	LOCAL = 0,
	REMOTE
};

enum SOCKET_STATUS{
	NONE		= 0x0,
	ACCEPTING	= 0x1,
	CONNECTING	= 0x2,
	HANDSHAKING	= 0x4,
	CONNECTED	= 0x8,
	RECEIVING	= 0x10,
	SENDING		= 0x20,
	CLOSING		= 0x40,
	CLOSED		= 0x80,
	OPERATING	= ACCEPTING | CONNECTING | HANDSHAKING | RECEIVING | SENDING
};

#define ADDR_SZ_SIZE	46
#define BUFFER_SIZE		    900000
#define BUFFER_SIZE_REV		900000

struct SSL_session;

struct session_overlapped
{
	OVERLAPPED overlapped;
	DWORD result;
	SSL_session *psession;
};

struct SSL_session
{
	SOCKET s; // handle to socket
	SOCKET s_listening; // handle to external listening socket
	sockaddr_storage addresses[2]; // local and remote address
	char addresses_sz[2][ADDR_SZ_SIZE]; // addresses in string format
	char socket_buffer_Rev[BUFFER_SIZE_REV]; // memory used for read/write from/to socket
	char socket_buffer_Send[BUFFER_SIZE]; // memory used for read/write from/to socket
	//char ssl_buffer[2][BUFFER_SIZE]; // memory used for read/write from/to ssl memory bio
	char ssl_buffer_Rev[BUFFER_SIZE_REV]; // memory used for read/write from/to ssl memory bio
	char ssl_buffer_Send[BUFFER_SIZE]; // memory used for read/write from/to ssl memory bio
	DWORD ssl_buffer_size[2]; // indicates the bytes of valid data in ssl_buffer
	unsigned int status; // stores current socket status, bit-masked value of one or more of SOCKET_STATUS
	session_overlapped overlapped[3]; // structure for overlapped operations
	WSABUF wsabuf[2]; // structure used for pass buffer to overlapped operations
	DWORD bytes_transferred[2]; // store the bytes of buffer that received/sent from/to the socket
	DWORD wsa_flags[2]; // store the flags send/receive from overlapped operations, not used
	SSL *ssl; // SSL structure used by OpenSSL
	BIO *bio[2]; // memory BIO used by OpenSSL
	ssl_lock lock; // synchronization object for multiple-thread data access
	char pending_socket_buffer_Rev[BUFFER_SIZE_REV]; // memory used for read/write from/to socket
	int datapending;
	int pendingdataSize;
	int handsake;	
	wchar_t  key[255]; 
	wchar_t  login[15]; 
	wchar_t HDSLno[239];
	void *pdata; // user supplied contextual data, not used by openssl processing
	char frame_buffer[BUFFER_SIZE]; // memory used store pending frame
	int  frame_buffer_size;
	int  remaining_frame_buffer_size;
	BYTE masks[4];
	char LastFrame[BUFFER_SIZE]; // memory used store pending frame
	int LastFrame_size;
	bool fin;
	int continousframe;
	char unprocessBuffer[BUFFER_SIZE];
	int unprocessBuffer_size;
	volatile LONG refcount; // S3: refcount-based lifetime; struct is freed when this reaches 0
};

void iocp_associate_handle(HANDLE h);

void ssl_init();
void ssl_set_ctx_cert_and_key(X509 *cert, EVP_PKEY *pkey);
void ssl_deinit();
void ssl_print_cert_info(X509 *cert);

SSL_session* session_new(void *pdata);
void session_delete(SSL_session *psession);
void send_close_message_to_client(SSL_session* psession);
void session_lock(SSL_session *psession);
void session_unlock(SSL_session *psession);
void session_addref(SSL_session *psession);
void session_release(SSL_session *psession);
void session_connect(SSL_session *psession, const sockaddr_storage *remote_addr);
void session_accept(SSL_session *psession);
int session_send_data(SSL_session *psession, const char *data, int len, CString strkey,int ActiveClient);
int session_send_data_AllClient(SSL_session* psession, const char* data, int len, CString strkey, int ActiveClient);
void session_close(SSL_session *psession);
bool session_process(SSL_session *psession);

// callback functions which are called under the protection of the psession->lock.
extern void app_on_session_connect(SSL_session *psession);
extern void app_on_session_send(SSL_session *psession);
extern void app_on_session_recv(SSL_session *psession);

// callback function which is called when psession is no longer in use and be safely
// deleted by calling session_delete().
//extern void app_on_session_close(SSL_session *psession);
extern void app_on_session_close(CString strKey);
int session_send_dataToUnregisteredClient(SSL_session* psession, const char* data, int len);