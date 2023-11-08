void MessageReceived(SSL_session* psession, char* message,int datasize, CString  key, CString login, CString HDSLno);
void stratServer();
void SendDataToClient(SSL_session* sslsession, string msg, CString strkey, int ActiveClient);
void SendDataToClient_AllClient(SSL_session* sslsession, string msg, CString strkey, int ActiveClient);
void SendDataToClient_UnRegistered(SSL_session* sslsession, string msg);