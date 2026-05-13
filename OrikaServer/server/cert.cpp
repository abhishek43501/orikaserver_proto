#include "stdafx.h"
#include "cert.h"


// S6: embedded TLS key material removed. The runtime loads Certificate.pem
// and key.pem from disk via ssl_set_ctx_cert_and_key() in openssl_iocp.cpp,
// so these symbols are kept only as empty stubs to preserve the legacy
// set_cert() call path in Server.cpp.
const char * ca_cert_key_pem = "";
const char * server_cert_key_pem = "";
