#include "stdafx.h"
#include "openssl_iocp.h"
#include <string>
#include <process.h>
#include "..\server\FrameAndDeframeMessage.h"
#include "..\\StaticClass.h"
#include <openssl/crypto.h>
#include <openssl/opensslv.h>
#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "mswsock.lib")

#if _MSC_VER >= 1900
FILE _iob[] = { *stdin, *stdout, *stderr };
extern "C" FILE * __cdecl __iob_func(void) { return _iob; }
#pragma comment (lib, "legacy_stdio_definitions.lib")
#endif


#define CERTF  "Certificate.pem"
#define KEYF   "key.pem"

void session_on_completed_packets(DWORD dwNumberOfBytesTransferred, ULONG_PTR lpCompletionKey, LPOVERLAPPED pOverlapped);

struct iocp_info
{
	HANDLE h_iocp;
	size_t threads_count;
	HANDLE* h_threads;
}iocp;
CMap<CString, LPCTSTR, int, int> m_deletedClientlist;

void iocp_on_completed_packets(DWORD dwNumberOfBytesTransferred, ULONG_PTR lpCompletionKey, LPOVERLAPPED pOverlapped)
{
	session_on_completed_packets(dwNumberOfBytesTransferred, lpCompletionKey, pOverlapped);
}

unsigned __stdcall iocp_proc(void* p)
{
	iocp_info* iocp = (iocp_info*)p;
	while (true)
	{
		DWORD dwNumberOfBytesTransferred = 0;
		ULONG_PTR lpCompletionKey = 0;
		LPOVERLAPPED pOverlapped = NULL;
		GetQueuedCompletionStatus(iocp->h_iocp, &dwNumberOfBytesTransferred, &lpCompletionKey, &pOverlapped, INFINITE);
		if (NULL == lpCompletionKey && NULL == pOverlapped)
		{
			PostQueuedCompletionStatus(iocp->h_iocp, 0, 0, 0);
			CStaticClass::m_logfile.LogEvent(L"Error in IOCP");
			break;

		}
		iocp_on_completed_packets(dwNumberOfBytesTransferred, lpCompletionKey, pOverlapped);
	}
	CStaticClass::m_logfile.LogEvent(L"IOCP loop Terminited");
	return 0;
}

void iocp_start(iocp_info* iocp)
{
	iocp->h_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	SYSTEM_INFO info = { 0 };
	GetNativeSystemInfo(&info);
	iocp->threads_count = info.dwNumberOfProcessors * 2;
	//iocp->threads_count = 2;
	iocp->h_threads = (HANDLE*)malloc(sizeof(HANDLE) * iocp->threads_count);
	for (size_t n = 0; n < iocp->threads_count; ++n)
		iocp->h_threads[n] = (HANDLE)_beginthreadex(0, 0, iocp_proc, iocp, 0, 0);
}

void iocp_stop(iocp_info* iocp)
{
	PostQueuedCompletionStatus(iocp->h_iocp, 0, 0, 0);
	WaitForMultipleObjects(iocp->threads_count, iocp->h_threads, TRUE, INFINITE);
	for (size_t n = 0; n < iocp->threads_count; ++n)
		CloseHandle(iocp->h_threads[n]);
	CloseHandle(iocp->h_iocp);
}

void iocp_associate_handle(HANDLE h)
{
	CreateIoCompletionPort(h, iocp.h_iocp, 0, 0);
}

struct CRYPTO_dynlock_value {
	ssl_lock lock;
};

int number_of_locks = 0;
ssl_lock* ssl_locks = nullptr;
SSL_CTX* ssl_ctx = nullptr;
ssl_lock lock_connect_ex;
LPFN_CONNECTEX pfn_ConnectEx = nullptr;

void ssl_lock_callback(int mode, int n, const char* file, int line)
{
	if (mode & CRYPTO_LOCK)
		EnterCriticalSection(&ssl_locks[n]);
	else
		LeaveCriticalSection(&ssl_locks[n]);
}

CRYPTO_dynlock_value* ssl_lock_dyn_create_callback(const char* file, int line)
{
	CRYPTO_dynlock_value* l = (CRYPTO_dynlock_value*)malloc(sizeof(CRYPTO_dynlock_value));
	InitializeCriticalSection(&l->lock);
	return l;
}

void ssl_lock_dyn_callback(int mode, CRYPTO_dynlock_value* l, const char* file, int line)
{
	if (mode & CRYPTO_LOCK)
		EnterCriticalSection(&l->lock);
	else
		LeaveCriticalSection(&l->lock);
}

void ssl_lock_dyn_destroy_callback(CRYPTO_dynlock_value* l, const char* file, int line)
{
	DeleteCriticalSection(&l->lock);
	free(l);
}

void ssl_init()
{
	WSADATA wsaData = { 0 };
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	iocp_start(&iocp);

	number_of_locks = CRYPTO_num_locks();
	if (number_of_locks > 0)
	{
		ssl_locks = (ssl_lock*)malloc(number_of_locks * sizeof(ssl_lock));
		for (int n = 0; n < number_of_locks; ++n)
			InitializeCriticalSection(&ssl_locks[n]);
	}

#ifdef _DEBUG
	/*CRYPTO_malloc_debug_init();
	CRYPTO_dbg_set_options(V_CRYPTO_MDEBUG_ALL);
	CRYPTO_mem_ctrl(CRYPTO_MEM_CHECK_ON);*/
	
#endif

	CRYPTO_set_locking_callback(&ssl_lock_callback);
	CRYPTO_set_dynlock_create_callback(&ssl_lock_dyn_create_callback);
	CRYPTO_set_dynlock_lock_callback(&ssl_lock_dyn_callback);
	CRYPTO_set_dynlock_destroy_callback(&ssl_lock_dyn_destroy_callback);

	SSL_load_error_strings();
	SSL_library_init();

	//const SSL_METHOD* meth = SSLv23_method();


	const SSL_METHOD* meth = TLS_server_method();
	ssl_ctx = SSL_CTX_new(meth);

	if (!SSL_CTX_load_verify_locations(ssl_ctx, "gd_bundle.crt", NULL)) 
	{
		BIO* bio = BIO_new_file("error_log.txt", "w");
		ERR_print_errors(bio);
		BIO_free(bio);
	}
	//SSL_CTX_set_verify(ssl_ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);
	 
	SSL_CTX_set_verify(ssl_ctx, SSL_VERIFY_NONE, nullptr);
	//SSL_set_verify()
	//SSL_CTX_load_verify_locations(ssl_ctx, "C:\\SllCertificate\\21-04-2025\\certificate.crt", NULL);

	InitializeCriticalSection(&lock_connect_ex);
}

void ssl_set_ctx_cert_and_key(X509* cert, EVP_PKEY* pkey)
{
	/*SSL_CTX_use_certificate(ssl_ctx, cert);
	SSL_CTX_use_PrivateKey(ssl_ctx, pkey);*/
	if (SSL_CTX_use_certificate_file(ssl_ctx, CERTF, SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stderr);
		exit(3);
	}
	if (SSL_CTX_use_PrivateKey_file(ssl_ctx, KEYF, SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stderr);
		exit(4);
	}

	if (!SSL_CTX_check_private_key(ssl_ctx)) {
		//printf(stderr, "La clave privada no coincide con la clave publica del certificado\n");
		exit(5);
	}
	int i = 0;
}

void ssl_deinit()
{
	SSL_CTX_free(ssl_ctx);
	ssl_ctx = nullptr;

	CRYPTO_set_locking_callback(NULL);
	CRYPTO_set_dynlock_create_callback(NULL);
	CRYPTO_set_dynlock_lock_callback(NULL);
	CRYPTO_set_dynlock_destroy_callback(NULL);

	EVP_cleanup();
	CRYPTO_cleanup_all_ex_data();
	//ERR_remove_state(0);
	ERR_free_strings();

	if (nullptr != ssl_locks)
	{
		for (int n = 0; n < number_of_locks; ++n)
			DeleteCriticalSection(&ssl_locks[n]);

		free(ssl_locks);
		ssl_locks = nullptr;
		number_of_locks = 0;
	}

	iocp_stop(&iocp);
	DeleteCriticalSection(&lock_connect_ex);
	WSACleanup();
}

static bool ssl_is_fatal_error(int ssl_error)
{
	switch (ssl_error)
	{
	case SSL_ERROR_NONE:
	case SSL_ERROR_WANT_READ:
	case SSL_ERROR_WANT_WRITE:
	case SSL_ERROR_WANT_CONNECT:
	case SSL_ERROR_WANT_ACCEPT:
		return false;
	}
	return true;
}

static int ssl_get_error(SSL* ssl, int result)
{
	int error = SSL_get_error(ssl, result);
	if (SSL_ERROR_NONE != error)
	{
		char message[512] = { 0 };
		int error_log = error;
		while (SSL_ERROR_NONE != error_log)
		{
			ERR_error_string_n(error_log, message, _countof(message));
			if (ssl_is_fatal_error(error_log))
			{
				// print error message to console or logs
			}
			error_log = ERR_get_error();
		}
	}
	return error;
}

static void sockaddr_to_string(const sockaddr_storage* paddr, char* pszaddr, DWORD dwSize = ADDR_SZ_SIZE)
{
	WSAAddressToStringA((sockaddr*)paddr, sizeof(sockaddr_storage), NULL, pszaddr, &dwSize);
}
//Error Session is Comming NULL Some Time
SSL_session* session_init(SSL_session* psession, void* pdata)
{
	if (psession == nullptr)
	{
		return psession;
	}
	memset(psession, 0, sizeof(SSL_session));
	psession->pdata = pdata;
	// S3: owner ref. session_delete drops this. In-flight async ops AddRef
	// before posting and Release in the completion handler. Last ref frees.
	psession->refcount = 1;
	InitializeCriticalSection(&psession->lock);

	psession->overlapped[CONNECT].psession = psession->overlapped[RECV].psession = psession->overlapped[SEND].psession = psession;
	psession->wsabuf[RECV].buf = psession->socket_buffer_Rev;
	psession->wsabuf[RECV].len = BUFFER_SIZE;
	psession->wsabuf[SEND].buf = psession->socket_buffer_Send;
	psession->wsabuf[SEND].len = 0;

	psession->handsake = 0;

	CMTStr::Copy(psession->key, L"");
	CMTStr::Copy(psession->login, L"");
	CMTStr::Copy(psession->HDSLno, L"");

	psession->ssl = SSL_new(ssl_ctx);
	psession->bio[SEND] = BIO_new(BIO_s_mem());
	psession->bio[RECV] = BIO_new(BIO_s_mem());
	SSL_set_bio(psession->ssl, psession->bio[RECV], psession->bio[SEND]);

	return psession;
}

SSL_session* session_new(void* pdata)
{
	SSL_session* psession = (SSL_session*)malloc(sizeof(SSL_session));
	return session_init(psession, pdata);
}

// S3: actual destruction. Only called by session_release when refcount reaches 0.
// Caller is expected to have called session_close beforehand to release SSL/socket.
static void session_destroy_internal(SSL_session* psession)
{
	DeleteCriticalSection(&psession->lock);
	free(psession);
}

void session_addref(SSL_session* psession)
{
	if (psession)
		InterlockedIncrement(&psession->refcount);
}

void session_release(SSL_session* psession)
{
	if (!psession)
		return;
	if (0 == InterlockedDecrement(&psession->refcount))
		session_destroy_internal(psession);
}

void session_delete(SSL_session* psession)
{
	// S3: drop the owner reference. If any async ops are still in flight,
	// they hold their own refs and the last one to Release will free the
	// struct. Previously this unconditionally free()d the session, which
	// could leave IOCP workers operating on freed memory.
	session_release(psession);
}

void send_close_message_to_client(SSL_session* psession)
{
	BYTE closeMessage[] = { 0x88, 0x00 };
	const char* closeMessageStr = reinterpret_cast<const char*>(closeMessage);
	//clientSocket->ShutDown();
	//clientSocket->Close();
	session_send_data(psession, closeMessageStr, sizeof(closeMessage), L"", 1);
}

void session_lock(SSL_session* psession)
{
	EnterCriticalSection(&psession->lock);
}

void session_unlock(SSL_session* psession)
{
	LeaveCriticalSection(&psession->lock);
}

bool session_is_data_pending(SSL_session* psession)
{
	if (psession->bytes_transferred[RECV] > 0)
		return true;

	if (SENDING == (psession->status & SENDING))
		return true;

	if (nullptr != psession->ssl)
	{
		if (BIO_pending(psession->bio[SEND]) > 0 || BIO_pending(psession->bio[RECV]) > 0)
			return true;
	}

	return false;
}

void session_close(SSL_session* psession)
{
	psession->status |= CLOSING;
	if (INVALID_SOCKET != psession->s && !session_is_data_pending(psession))
	{
		shutdown(psession->s, SD_BOTH);
		closesocket(psession->s);
		psession->s = INVALID_SOCKET;
	}

	if (INVALID_SOCKET == psession->s && 0 == (psession->status & OPERATING))
	{
		if (nullptr != psession->ssl)
		{
			SSL_shutdown(psession->ssl);
			SSL_free(psession->ssl);
			psession->ssl = nullptr;
			psession->bio[SEND] = psession->bio[RECV] = nullptr;
		}
		psession->status = CLOSED;
	}
}

void session_connect(SSL_session* psession, const sockaddr_storage* remote_addr)
{
	memcpy_s(&psession->addresses[REMOTE], sizeof(sockaddr_storage), remote_addr, sizeof(sockaddr_storage));
	sockaddr_to_string(&psession->addresses[REMOTE], psession->addresses_sz[REMOTE]);

	psession->s = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	setsockopt(psession->s, SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, NULL, 0);
	iocp_associate_handle((HANDLE)psession->s);

	addrinfo hints = { 0 };
	hints.ai_family = remote_addr->ss_family;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	addrinfo* paddrinfo = nullptr;
	getaddrinfo(0, "", &hints, &paddrinfo);
	if (NULL != paddrinfo)
	{
		bind(psession->s, paddrinfo->ai_addr, paddrinfo->ai_addrlen);
		freeaddrinfo(paddrinfo);
		paddrinfo = nullptr;
	}

	EnterCriticalSection(&lock_connect_ex);
	if (nullptr == pfn_ConnectEx)
	{
		DWORD dwRetBytes = 0;
		GUID guid = WSAID_CONNECTEX;
		WSAIoctl(psession->s, SIO_GET_EXTENSION_FUNCTION_POINTER, (void*)&guid, sizeof(guid), (void*)&pfn_ConnectEx, sizeof(pfn_ConnectEx), &dwRetBytes, NULL, NULL);
	}
	LeaveCriticalSection(&lock_connect_ex);

	psession->status |= CONNECTING;
	(*pfn_ConnectEx)(psession->s, (sockaddr*)remote_addr, sizeof(sockaddr_storage), 0, 0, 0, &psession->overlapped[CONNECT].overlapped);
	psession->overlapped[CONNECT].result = WSAGetLastError();
	if (0 != psession->overlapped[CONNECT].result && WSA_IO_PENDING != psession->overlapped[CONNECT].result)
	{
		psession->status &= ~CONNECTING;
		session_close(psession);
	}
}

void session_accept(SSL_session* psession)
{
	psession->s = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	iocp_associate_handle((HANDLE)psession->s);

	psession->status |= ACCEPTING;
	session_addref(psession); // S3: ref for the pending AcceptEx; released at end of session_on_completed_packets
	BOOL accepted = AcceptEx(psession->s_listening, psession->s, psession->addresses, 0, sizeof(sockaddr_storage), sizeof(sockaddr_storage), 0, &psession->overlapped[CONNECT].overlapped);
	psession->overlapped[CONNECT].result = WSAGetLastError();
	if (WSA_IO_PENDING != psession->overlapped[CONNECT].result)
	{
		psession->status = NONE;
		session_release(psession); // S3: post failed, no completion will fire; unwind the AddRef
		session_close(psession);
	}
}

void session_send(SSL_session* psession)
{
	if ((SENDING != (psession->status & SENDING)) && (psession->wsabuf[SEND].len > 0))
	{
		psession->status |= SENDING;
		psession->bytes_transferred[SEND] = 0;
		session_addref(psession); // S3: ref for the pending WSASend; released at end of session_on_completed_packets
		WSASend(psession->s, &psession->wsabuf[SEND], 1, &psession->bytes_transferred[SEND], 0, &psession->overlapped[SEND].overlapped, 0);
		psession->overlapped[SEND].result = WSAGetLastError();
		if (0 != psession->overlapped[SEND].result && WSA_IO_PENDING != psession->overlapped[SEND].result)
		{
			try
			{
				psession->status &= ~SENDING;
				session_release(psession); // S3: post failed, unwind the AddRef
				session_close(psession);
			}
			catch (CException* e)
			{
				// handle the exception
				e->ReportError();
				e->Delete();
			}
		}
	}
}

void session_recv(SSL_session* psession)
{
	if ((CLOSING != (psession->status & CLOSING)) && (RECEIVING != (psession->status & RECEIVING)) && (psession->bytes_transferred[RECV] == 0))
	{
		psession->status |= RECEIVING;
		DWORD size = 0;
		session_addref(psession); // S3: ref for the pending WSARecv; released at end of session_on_completed_packets
		WSARecv(psession->s, &psession->wsabuf[RECV], 1, &size, &psession->wsa_flags[RECV], &psession->overlapped[RECV].overlapped, 0);
		psession->overlapped[RECV].result = WSAGetLastError();
		if (0 != psession->overlapped[RECV].result && WSA_IO_PENDING != psession->overlapped[RECV].result)
		{
			psession->status &= ~RECEIVING;
			session_release(psession); // S3: post failed, unwind the AddRef
			session_close(psession);
		}
	}
	////(L"11");
}

bool session_process(SSL_session* psession)
{
	////(L"101");
	bool fatal_error_occurred = false;
	if (nullptr != psession->ssl)
	{
		if (psession->bytes_transferred[RECV] > 0)
		{

			int bytes = BIO_write(psession->bio[RECV], psession->socket_buffer_Rev, psession->bytes_transferred[RECV]);
			int ssl_error = ssl_get_error(psession->ssl, bytes);
			if (bytes == psession->bytes_transferred[RECV])
			{
				psession->bytes_transferred[RECV] = 0;
			}
			else if (!BIO_should_retry(psession->bio[RECV]))
			{
				fatal_error_occurred = true;
			}
		}

		if (psession->ssl_buffer_size[RECV] == 0)
		{
			int totalbytes = 0;
			int bytes = 0;
			do
			{
				////(L"102");
				bytes = SSL_read(psession->ssl, psession->ssl_buffer_Rev, BUFFER_SIZE);
				int ssl_error = ssl_get_error(psession->ssl, bytes);
				if (bytes > 0)
				{
					totalbytes = totalbytes + bytes;
				}

				if ((HANDSHAKING == (psession->status & HANDSHAKING)) && SSL_is_init_finished(psession->ssl))
				{
					psession->status &= ~HANDSHAKING;
					psession->status |= CONNECTED;

					app_on_session_connect(psession);

				}
				////(L"103");
				if (bytes > 0)
				{
					psession->ssl_buffer_size[RECV] = bytes;
					////(L"Going To receive Data");
					app_on_session_recv(psession);
					////(L"Data received");
					psession->ssl_buffer_size[RECV] = 0;
				}
				else if (ssl_is_fatal_error(ssl_error))
				{
					fatal_error_occurred = true;
				}
				////(L"104");
			} while (bytes > 0);

		}

		if (psession->ssl_buffer_size[SEND] > 0)
		{
			////(L"105");
			int bytes = SSL_write(psession->ssl, psession->ssl_buffer_Send, psession->ssl_buffer_size[SEND]);
			int ssl_error = ssl_get_error(psession->ssl, bytes);
			if (bytes == psession->ssl_buffer_size[SEND])
			{
				psession->ssl_buffer_size[SEND] = 0;
			}
			else if (ssl_is_fatal_error(ssl_error))
			{
				fatal_error_occurred = true;
			}
		}

		if (psession->wsabuf[SEND].len == 0 && (0 != psession->s_listening || BIO_pending(psession->bio[SEND])))
		{

			int bytes = BIO_read(psession->bio[SEND], psession->socket_buffer_Send, BUFFER_SIZE);
			int ssl_error = ssl_get_error(psession->ssl, bytes);
			if (bytes > 0)
			{
				psession->wsabuf[SEND].len = bytes;
			}
			else if (ssl_is_fatal_error(ssl_error))
			{
				fatal_error_occurred = true;
			}
			////(L"106");
		}

		if (fatal_error_occurred)
			session_close(psession);
	}
	////(L"10");
	session_send(psession);
	session_recv(psession);

	return !fatal_error_occurred;
}

void session_on_connect(SSL_session* psession)
{
	psession->status &= ~CONNECTING;
	if (0 == psession->overlapped[CONNECT].result)
	{
		int size = sizeof(psession->addresses[LOCAL]);
		getsockname(psession->s, (sockaddr*)&psession->addresses[LOCAL], &size);
		sockaddr_to_string(&psession->addresses[LOCAL], psession->addresses_sz[LOCAL]);

		psession->status |= HANDSHAKING;
		SSL_set_connect_state(psession->ssl);

		session_process(psession);
	}
	else
	{
		app_on_session_connect(psession);
		session_close(psession);
	}
}

void session_on_accept(SSL_session* psession)
{
	if (0 == psession->overlapped[CONNECT].result)
	{
		SSL_session* psession_new = session_new(psession->pdata);

		if (psession_new == nullptr)
		{
			return;
		}
		psession_new->s_listening = psession->s_listening;
		session_accept(psession_new);

		psession->status &= ~ACCEPTING;
		psession->status |= HANDSHAKING;

		int naddrslen[2] = { 0, 0 };
		sockaddr* paddrs[2] = { 0, 0 };
		GetAcceptExSockaddrs(psession->addresses, 0, sizeof(sockaddr_storage), sizeof(sockaddr_storage), &paddrs[LOCAL], &naddrslen[LOCAL], &paddrs[REMOTE], &naddrslen[REMOTE]);
		memcpy_s(&psession->addresses[LOCAL], sizeof(sockaddr_storage), paddrs[LOCAL], naddrslen[LOCAL]);
		memcpy_s(&psession->addresses[REMOTE], sizeof(sockaddr_storage), paddrs[REMOTE], naddrslen[REMOTE]);
		setsockopt(psession->s, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)&psession->s_listening, sizeof(SOCKET));
		sockaddr_to_string(&psession->addresses[LOCAL], psession->addresses_sz[LOCAL]);
		sockaddr_to_string(&psession->addresses[REMOTE], psession->addresses_sz[REMOTE]);

		SSL_set_accept_state(psession->ssl);
		session_process(psession);
	}
}

void session_on_recv(SSL_session* psession)
{
	psession->status &= ~RECEIVING;
	if ((psession->bytes_transferred[RECV] > 0))
	{

		session_process(psession);

	}
	else if (psession->overlapped[RECV].result != 0)
	{

		session_close(psession);

	}
}

void session_on_send(SSL_session* psession)
{
	psession->status &= ~SENDING;
	psession->wsabuf[SEND].len = 0;
	app_on_session_send(psession);
	if (0 == psession->overlapped[SEND].result)
	{
		session_process(psession);
	}
	else
	{
		session_close(psession);
	}
}

void session_on_completed_packets(DWORD dwNumberOfBytesTransferred, ULONG_PTR lpCompletionKey, LPOVERLAPPED pOverlapped)
{
	session_overlapped* p = (session_overlapped*)pOverlapped;
	////(L"pT");
	session_lock(p->psession);
	////(L"pTL");
	DWORD dwOverlappeddNumberOfBytesTransferred = 0, dwOverlappedFlags = 0;
	BOOL succeeded = WSAGetOverlappedResult(p->psession->s, pOverlapped, &dwOverlappeddNumberOfBytesTransferred, TRUE, &dwOverlappedFlags);
	p->result = WSAGetLastError();

	if (pOverlapped == &p->psession->overlapped[CONNECT].overlapped)
	{
		////(L"1");
		if (0 == p->psession->s_listening)
			session_on_connect(p->psession);
		else
			session_on_accept(p->psession);
	}
	else if (pOverlapped == &p->psession->overlapped[RECV].overlapped)
	{
		////(L"2");
		p->psession->bytes_transferred[RECV] = dwNumberOfBytesTransferred;

		session_on_recv(p->psession);
	}
	else if (pOverlapped == &p->psession->overlapped[SEND].overlapped)
	{
		////(L"3");
		p->psession->bytes_transferred[SEND] = dwNumberOfBytesTransferred;
		session_on_send(p->psession);
	}

	CString strLogString = L"";
	int int_SocketStatus = p->psession->status;
	//strLogString.Format(L"Socket Status %d", int_SocketStatus);
	////(strLogString);
	bool close_session = (CLOSED == p->psession->status);
	////(L"UnLocked Event 02");
	////(L"Going ro UnLock UpT");
	session_unlock(p->psession);
	////(L"UpT");
	if (close_session)
	{
		CString m_key = L"";
		m_key = p->psession->key;

		int deleteclientFlag = 0;
		m_deletedClientlist.Lookup(m_key, deleteclientFlag);
		if (deleteclientFlag == 1)
		{
			// S3: release this completion's ref before bailing out.
			// Another worker is already handling the close-and-delete.
			session_release(p->psession);
			return;
		}
		deleteclientFlag = 1;
		m_deletedClientlist.SetAt(m_key, deleteclientFlag);
		CString m_log = L"";
		/*m_log.Format(L"Going to delete client For Key %s", m_key);
		CStaticClass::m_logfile.LogEvent(m_log);*/


		CStaticClass::m_mutex_dealingClientList.Lock();
		CStaticClass::m_ClientList_forDeal.RemoveKey(m_key);
		CStaticClass::m_ClientList_forOrder.RemoveKey(m_key);
		CStaticClass::m_mutex_dealingClientList.Unlock();

		CStaticClass::m_mutex_ClientList.Lock();

		CStaticClass::st_ClientContext st_Check = {};
		CStaticClass::m_ClientContext.Lookup(m_key, st_Check);
		st_Check.m_startCalculationThread = 0;
		CStaticClass::m_ClientContext.SetAt(m_key, st_Check);
		CStaticClass::m_mutex_ClientList.Unlock();
		int m_thread_terminate = 0;
		/*if (WaitForSingleObject(st_Check.m_localThred, INFINITE) == WAIT_OBJECT_0)
		{
		}*/

		if (st_Check.m_activeClient != 0)
		{
			m_log.Format(L"Going to trminate thread %s", m_key);
			CStaticClass::m_logfile.LogEvent(m_log);
			while (m_thread_terminate == 0)
			{
				CStaticClass::m_mutex_ClientList.Lock();
				/*m_log.Format(L"Going to Lock Client Lock %s", m_key);
				CStaticClass::m_logfile.LogEvent(m_log);*/
				CStaticClass::m_ClientContext.Lookup(m_key, st_Check);
				m_thread_terminate = st_Check.m_thread_terminate;
				CStaticClass::m_mutex_ClientList.Unlock();
				/*m_log.Format(L"UnLocked Client Lock %s", m_key);
				CStaticClass::m_logfile.LogEvent(m_log);*/
				Sleep(100);
			}

		}
		m_log.Format(L"Client thread trminated %s", m_key);
		CStaticClass::m_logfile.LogEvent(m_log);


		CStaticClass::m_mutex_ClientList.Lock();
		CStaticClass::m_ClientContext.RemoveKey(m_key);
		CStaticClass::m_mutex_ClientList.Unlock();


		session_lock(p->psession);
		session_close(p->psession);
		session_unlock(p->psession);


		/*m_log.Format(L"Call app_on_session_close %s", m_key);
		CStaticClass::m_logfile.LogEvent(m_log);*/
		app_on_session_close(m_key);
		/*m_log.Format(L"Call app_on_session_close Finished %s", m_key);
		CStaticClass::m_logfile.LogEvent(m_log);*/

		session_delete(p->psession);
		m_log.Format(L"Client deleted %s", m_key);
		CStaticClass::m_logfile.LogEvent(m_log);
	}

	// S3: release the ref taken when this completion's op was posted (in
	// session_accept / session_send / session_recv). If the close branch
	// above already released the owner ref AND this is the last in-flight
	// op, this Release will free the session. After this point p->psession
	// must NOT be touched.
	session_release(p->psession);
}

int session_send_data(SSL_session* psession, const char* data, int len, CString strkey, int ActiveClient)
{
	int bytes_sent = 0;
	UINT64 LoopEnterTime = _time64(NULL);
	while (0 == bytes_sent)
	{
		UINT64 CurrentTime = _time64(NULL);
		if (CurrentTime - LoopEnterTime > 5)
		{
			//(L"Going To Terminate While Loop and delete Client in client list Because Sending Time is more than 5 sec");
			CStaticClass::m_mutex_ClientList.Lock();
			CStaticClass::st_ClientContext m_tmp_st = {};
			CStaticClass::m_ClientContext.Lookup(strkey, m_tmp_st);
			m_tmp_st.m_startCalculationThread = 0;
			CStaticClass::m_ClientContext.SetAt(strkey, m_tmp_st);
			CStaticClass::m_mutex_ClientList.Unlock();
			Sleep(5000);

			CStaticClass::m_mutex_ClientList.Lock();
			CStaticClass::m_ClientContext.RemoveKey(strkey);
			CStaticClass::m_mutex_ClientList.Unlock();
			CStaticClass::m_logfile.LogEvent(L"While Loop Terminated and  Client has been deleted in client list Because Sending Time is more than 5 sec");
			return 0;
		}

		if (strkey != L"")
		{

			if (ActiveClient == 0)
			{
				//(L"Client is not Active");
				return 0;
			}

			if (psession == nullptr)
			{
				//(L"Client  Session is null");
				return 0;
			}
			if (psession->ssl == nullptr || psession->ssl == NULL)
			{
				CStaticClass::m_logfile.LogEvent(L"Going To Terminate While Loop and delete Client in client list");
				CStaticClass::m_mutex_ClientList.Lock();
				CStaticClass::st_ClientContext m_tmp_st = {};
				CStaticClass::m_ClientContext.Lookup(strkey, m_tmp_st);
				m_tmp_st.m_startCalculationThread = 0;
				CStaticClass::m_ClientContext.SetAt(strkey, m_tmp_st);
				CStaticClass::m_mutex_ClientList.Unlock();

				//Sleep(5000);				

				CStaticClass::m_mutex_ClientList.Lock();
				CStaticClass::m_ClientContext.RemoveKey(strkey);
				CStaticClass::m_mutex_ClientList.Unlock();
				CStaticClass::m_logfile.LogEvent(L"While Loop Terminated and  Client has been deleted in client list");
				return 0;
			}

		}

		////(L"p"+ strkey);
		
		session_lock(psession);
		
		////(L"p1"+ strkey);
		if (psession->ssl_buffer_size[SEND] == 0)
		{
			bytes_sent = min(BUFFER_SIZE, len);
			memcpy_s(psession->ssl_buffer_Send, BUFFER_SIZE, data, bytes_sent);
			psession->ssl_buffer_size[SEND] = bytes_sent;
			////(L"p");
			session_process(psession);
			////(L"Up");

		}
		session_unlock(psession);
		
		////(L"Up"+ strkey);
		if (0 == bytes_sent)
			Sleep(0);
	}

	return bytes_sent;
}



int session_send_data_AllClient(SSL_session* psession, const char* data, int len, CString strkey, int ActiveClient)
{
	int bytes_sent = 0;
	UINT64 LoopEnterTime = _time64(NULL);
	while (0 == bytes_sent)
	{
		UINT64 CurrentTime = _time64(NULL);
		if (CurrentTime - LoopEnterTime > 5)
		{
			//(L"Going To Terminate While Loop and delete Client in client list Because Sending Time is more than 5 sec");
			//CStaticClass::m_mutex_ClientList.Lock();
			CStaticClass::st_ClientContext m_tmp_st = {};
			CStaticClass::m_ClientContext.Lookup(strkey, m_tmp_st);
			m_tmp_st.m_startCalculationThread = 0;
			CStaticClass::m_ClientContext.SetAt(strkey, m_tmp_st);
			//CStaticClass::m_mutex_ClientList.Unlock();
			Sleep(5000);

			//CStaticClass::m_mutex_ClientList.Lock();
			CStaticClass::m_ClientContext.RemoveKey(strkey);
			//CStaticClass::m_mutex_ClientList.Unlock();
			//(L"While Loop Terminated and  Client has been deleted in client list Because Sending Time is more than 5 sec");
			return 0;
		}

		if (strkey != L"")
		{

			if (ActiveClient == 0)
			{
				//(L"Client is not Active");
				return 0;
			}

			if (psession == nullptr)
			{
				//(L"Client  Session is null");
				return 0;
			}
			if (psession->ssl == nullptr || psession->ssl == NULL)
			{
				//(L"Going To Terminate While Loop and delete Client in client list");
				//CStaticClass::m_mutex_ClientList.Lock();

				CStaticClass::st_ClientContext m_tmp_st = {};
				CStaticClass::m_ClientContext.Lookup(strkey, m_tmp_st);
				m_tmp_st.m_startCalculationThread = 0;
				CStaticClass::m_ClientContext.SetAt(strkey, m_tmp_st);
				//CStaticClass::m_mutex_ClientList.Unlock();

				Sleep(5000);

				//CStaticClass::m_mutex_ClientList.Lock();
				CStaticClass::m_ClientContext.RemoveKey(strkey);
				//CStaticClass::m_mutex_ClientList.Unlock();
				//(L"While Loop Terminated and  Client has been deleted in client list");
				return 0;
			}

		}
		////(L"p"+ strkey);
		session_lock(psession);
		////(L"p1"+ strkey);
		if (psession->ssl_buffer_size[SEND] == 0)
		{
			bytes_sent = min(BUFFER_SIZE, len);
			memcpy_s(psession->ssl_buffer_Send, BUFFER_SIZE, data, bytes_sent);
			psession->ssl_buffer_size[SEND] = bytes_sent;
			////(L"p");
			session_process(psession);
			////(L"Up");

		}
		session_unlock(psession);
		////(L"Up"+ strkey);
		if (0 == bytes_sent)
			Sleep(0);
	}

	return bytes_sent;
}
int session_send_dataToUnregisteredClient(SSL_session* psession, const char* data, int len)
{

	int bytes_sent = 0;
	while (0 == bytes_sent)
	{
		session_lock(psession);
		if (psession->ssl_buffer_size[SEND] == 0)
		{
			bytes_sent = min(BUFFER_SIZE, len);
			memcpy_s(psession->ssl_buffer_Send, BUFFER_SIZE, data, bytes_sent);
			psession->ssl_buffer_size[SEND] = bytes_sent;
			session_process(psession);
		}
		session_unlock(psession);
		if (0 == bytes_sent)
			Sleep(0);
	}

	return bytes_sent;
}



std::string ssl_get_cert_issuer_info_by_id(X509_NAME* issuer, int id)
{
	std::string issuer_info;
	int index = X509_NAME_get_index_by_NID(issuer, id, -1);
	X509_NAME_ENTRY* entry = X509_NAME_get_entry(issuer, index);
	if (entry > 0)
	{
		ASN1_STRING* asn1_data = X509_NAME_ENTRY_get_data(entry);
		if (asn1_data > 0)
		{
			//unsigned char* info = ASN1_STRING_data(asn1_data);
			const unsigned char* info = ASN1_STRING_get0_data(asn1_data);

			if (info > 0)
				issuer_info = (char*)info;
		}
	}
	return issuer_info;
}

void ssl_print_cert_info(X509* cert)
{
	//printf("=======================\n");
	X509_NAME* name_subject = X509_get_subject_name(cert);
	if (name_subject > 0)
	{
		BIO* bio = BIO_new(BIO_s_mem());
		X509_NAME_print_ex(bio, name_subject, 0, XN_FLAG_RFC2253);
		char* subject = nullptr;
		long length = BIO_get_mem_data(bio, &subject);
		if (nullptr != subject && length > 0)
		{
			std::string str;
			str.resize(length);
			std::copy(subject, subject + length, str.begin());
			//printf("CERT subject: %s\n", str.c_str());
		}
		BIO_free(bio);
	}

	X509_NAME* name_issuer = X509_get_issuer_name(cert);
	if (name_issuer > 0)
	{
		//printf("CERT cn: %s\n", ssl_get_cert_issuer_info_by_id(name_issuer, NID_commonName).c_str());
		//printf("CERT c: %s\n", ssl_get_cert_issuer_info_by_id(name_issuer, NID_countryName).c_str());
		//printf("CERT o: %s\n", ssl_get_cert_issuer_info_by_id(name_issuer, NID_organizationName).c_str());
	}

	int criticality = -1, ext_index = -1;
	ASN1_BIT_STRING* key_usage = (ASN1_BIT_STRING*)X509_get_ext_d2i(cert, NID_key_usage, &criticality, &ext_index);
	if (key_usage > 0)
	{
		const char* usages[] = { "digitalSignature",
								"nonRepudiation",
								"keyEncipherment",
								"dataEncipherment",
								"keyAgreement",
								"keyCertSign",
								"cRLSign",
								"encipherOnly",
								"decipherOnly" };

		//printf("CERT key_usage:");
		for (int index = 0; index < 8; index++)
		{
			if (ASN1_BIT_STRING_get_bit(key_usage, index))
			{
			}
			////printf(" %s;", usages[index]);
		}
		////printf("\n");
	}

	const char* kuValue = NULL;
	STACK_OF(ASN1_OBJECT)* ext_key_usage = (STACK_OF(ASN1_OBJECT)*)X509_get_ext_d2i(cert, NID_ext_key_usage, NULL, NULL);
	if (ext_key_usage > 0)
	{
		//printf("CERT ext_key_usage:");
		while (sk_ASN1_OBJECT_num(ext_key_usage) > 0)
		{
			int usage_id = OBJ_obj2nid(sk_ASN1_OBJECT_pop(ext_key_usage));
			const char* usage_value = OBJ_nid2sn(usage_id);
			//printf(" %d:%s;", usage_id, usage_value);
		}
		//printf("\n");
	}

	STACK_OF(GENERAL_NAME)* alt_name = (STACK_OF(GENERAL_NAME)*) X509_get_ext_d2i(cert, NID_subject_alt_name, NULL, NULL);
	while (sk_GENERAL_NAME_num(alt_name) > 0)
	{
		GENERAL_NAME* name = sk_GENERAL_NAME_pop(alt_name);
		switch (name->type)
		{
		case GEN_DNS:
			//printf("CERT GEN_DNS: %s\n", ASN1_STRING_data(name->d.dNSName));
			break;
		case GEN_URI:
			//printf("CERT GEN_URI: %s\n", ASN1_STRING_data(name->d.uniformResourceIdentifier));
			break;
		case GEN_EMAIL:
			//printf("CERT GEN_EMAIL: %s\n", (char*)ASN1_STRING_data(name->d.rfc822Name));
			break;
		}
	}
	//printf("=======================\n");
}