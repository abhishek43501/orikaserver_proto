                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         // servers.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <atlbase.h>
#include <stdio.h>
#include "openssl_iocp.h"
#include "WebsocketHandshakeMessage.h"
#include "FrameAndDeframeMessage.h"
#include <sstream>
#include "server.h"
#include "DataBuffer.h"
#include "StaticClass.h"
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
#include <iostream>
#include "istreamwrapper.h"

#include "UserLogin.h"
#include "DataTable\orika_clientmaster.h"
#include "DataBuffer.h"
#include "..\\MSMQ\MSMQApiWrapper.h"
//#include "LZ\lzstring.h"
#include "..\ClientSocket\ClientSocket.h"
#include "..\DataTable\Orika_Clientgatewayconfig.h"
//#include <lzo1z.h>
//#include <lzoconf.h>
#include "ProtoMessageConverter.h"


using namespace rapidjson;
using namespace std;
int handsake = 0;

void sendDataToAllClient(CString msg);
void sendDataToAll_Other_Client(CString  msg, CString m_ignoreClient);
UINT threadForLoginWiseCalculate(LPVOID pParam);
UINT threadForLoginWiseCalculate(LPVOID pParam)
{
	CStaticClass m_staticclass;
	m_staticclass.calculateClientWiseAllPosition();
	return 0;
}


void app_on_session_connect(SSL_session *psession)
{
	//printf("Connected from %s\n", psession->addresses_sz[REMOTE]);
}

void app_on_session_send(SSL_session *psession)
{
}
void app_on_session_recv(SSL_session *psession)
{
	DataBuffer DataForReceive;
	DataForReceive.Allocate(1500000);	
	//printf("Received %d bytes from %s:", psession->ssl_buffer_size[RECV], psession->addresses_sz[REMOTE]);
	char buffer[BUFFER_SIZE+1] = {0};
	strncpy_s(buffer, psession->ssl_buffer_Rev, psession->ssl_buffer_size[RECV]);
	buffer[psession->ssl_buffer_size[RECV]] = 0;
	//printf("%s\n", buffer);		
	if (psession->handsake  == 0)
	{		
		char* sendData;
		WebsocketHandshakeMessage* wshs = new WebsocketHandshakeMessage(buffer, strlen(buffer));		
		if (wshs->Parse() == true)
		{
			psession->handsake = 1;
			string r_str = wshs->Serialize();
			delete wshs;
			int test = 0;
			sendData = const_cast<char*>(r_str.c_str());
			int datasize = strlen(sendData);
			session_send_data(psession, sendData, strlen(sendData), L"", 1);
		}
		
		//delete wshs;
	}
	else
	{
		FrameAndDeframeMessage FDObj;
		string strReceivedMSG = "";		
				
		FDObj.deframeIncomingMessage(psession->ssl_buffer_Rev, psession->ssl_buffer_size[RECV], psession, DataForReceive);
								
		CString  strkey= psession->key ; 
		CString  strlogin = psession->login; 
		CString  strHDSLno = psession->HDSLno;		
		MessageReceived(psession, DataForReceive.GetBuffer(), DataForReceive.GetDataSize(), strkey, strlogin, strHDSLno);
		strHDSLno = psession->HDSLno;

	}	
	DataForReceive.Delete();
	
}

void app_on_session_close(CString strKey)
{
	////(L"Going to Delete Data From Client Context");	

	CStaticClass::m_mutex_ClientList.Lock();
	//(L"17");	
	CStaticClass::st_ClientContext m_tmp_st = {};
	CStaticClass::m_ClientContext.Lookup(strKey, m_tmp_st);
	CString logintoken = m_tmp_st.m_logintoken;
	CStaticClass::m_sqldata.removelogintoken(logintoken,strKey);	
	CStaticClass::m_mutex_ClientList.Unlock();
	//(L"Going to Delete Client From List");
	//Sleep(5000);
	
		
	//(L"Client Session Deleted");
	CStaticClass::m_mutex_ClientList.Lock();
		CStaticClass::st_ClientContext m_st = {};	
		int totalclient = CStaticClass::m_ClientContext.GetCount();
		CStaticClass::m_ClientContext.RemoveKey(strKey);
		int remainingclient = CStaticClass::m_ClientContext.GetCount();
		//Wait for Complete All sending Data
		//No need to Unlock Session because session object is being deleted.				
	CStaticClass::m_mutex_ClientList.Unlock();
	if (strKey != L"")
	{
		CString strjason = L"";
		strjason.Format(L"{\"type\": \"USERS_DATA\",\"action\": \"delete\",\"uniquekey\":\"sessionid\",\"sessionid\":\"%s\"}", strKey);
		//sendDataToAll_Other_Client(strjason, strKey);
	}
	CString strlogW = L"";
	strlogW.Format(L"TotalClient %d", totalclient);
	CStaticClass::m_logfile.LogEvent(strlogW);
	strlogW.Format(L"Client Removed From Client Context %s ", strKey);
	CStaticClass::m_logfile.LogEvent(strlogW);
	strlogW.Format(L"Remaining Client %d", remainingclient);
	CStaticClass::m_logfile.LogEvent(strlogW);
	
}
// create a listening socket and associate it with iocp
// I13: returns INVALID_SOCKET on any failure (WSASocket / bind / listen).
// Previously the return value of those calls was discarded and a broken
// socket was returned to stratServer.
SOCKET create_listen_socket(int port)
{
	sockaddr_storage addr = {0};
	sockaddr_in *paddrin = (sockaddr_in*)&addr;
	addr.ss_family = AF_INET;
	paddrin->sin_port = htons(port);
	paddrin->sin_addr.s_addr = htonl(INADDR_ANY);

	SOCKET s = WSASocket(addr.ss_family, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (s == INVALID_SOCKET)
	{
		CString line;
		line.Format(L"create_listen_socket: WSASocket failed (port=%d, WSAGetLastError=%d)", port, WSAGetLastError());
		CStaticClass::m_logfile.LogEvent(line);
		return INVALID_SOCKET;
	}
	iocp_associate_handle((HANDLE)s);
	if (bind(s, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		CString line;
		line.Format(L"create_listen_socket: bind failed (port=%d, WSAGetLastError=%d) - port already in use?", port, WSAGetLastError());
		CStaticClass::m_logfile.LogEvent(line);
		closesocket(s);
		return INVALID_SOCKET;
	}
	if (listen(s, SOMAXCONN) == SOCKET_ERROR)
	{
		CString line;
		line.Format(L"create_listen_socket: listen failed (port=%d, WSAGetLastError=%d)", port, WSAGetLastError());
		CStaticClass::m_logfile.LogEvent(line);
		closesocket(s);
		return INVALID_SOCKET;
	}
	return s;
}

// I13: now returns bool. False means the WebSocket listener did NOT start
// (port misconfigured, port already in use, ssl_init / cert load failure
// trickled into a NULL session, etc.). Caller (OnBnClickedStart) should
// react accordingly.
bool stratServer()
{
	//lzo_init();
	ssl_init();
	// I3: ssl_set_ctx_cert_and_key loads Certificate.pem / key.pem from disk
	// (and has done so since pre-S6); it never used its cert/pkey arguments.
	// The former set_cert() wrapper parsed the now-empty embedded blobs only
	// to throw the results away. Call directly with null args.
	ssl_set_ctx_cert_and_key(nullptr, nullptr);

	// I13: validate the configured port. _wtoi returns 0 on any unparseable
	// input (empty string, non-numeric); bind to port 0 would succeed and
	// pick an ephemeral port - the server would silently run on the wrong
	// port and no client could ever connect.
	int port = _wtoi(CStaticClass::orikaPort);
	if (port <= 0 || port > 65535)
	{
		CString line;
		line.Format(L"stratServer: configured orikaPort '%s' is invalid (parsed as %d) - listener NOT started",
			(LPCWSTR)CStaticClass::orikaPort, port);
		CStaticClass::m_logfile.LogEvent(line);
		return false;
	}

	SOCKET s = create_listen_socket(port);
	if (s == INVALID_SOCKET)
	{
		// create_listen_socket already logged the specific WSA error.
		return false;
	}

	// A4: session_new does malloc(sizeof(SSL_session)) which is ~6.3 MB after
	// the SSL_session struct grew (audit f8e5007). Under memory pressure or
	// if SSL_new fails (e.g., ssl_ctx is NULL because ssl_init bailed), the
	// allocation/init can fail and return nullptr. Previously the next line
	// dereferenced unconditionally - silent process death with no log entry.
	SSL_session *psession = session_new(0);
	if (psession == nullptr)
	{
		CStaticClass::m_logfile.LogEvent(L"stratServer: session_new returned NULL - server listener NOT started");
		closesocket(s);
		return false;
	}

	psession->s_listening = s;
	session_accept(psession);

	return true;
}
std::string JsonAsString(const Value& json);
void updateFilterState(const Value& jsonObject, int m_viewno, CString m_metadatatype, CString m_userlogin)
{
	CString m_colID = L"";
	CString  m_filterjson = L"";
	CString strSqlCommand = L"";
	strSqlCommand.Format(L"delete from orika_ToggleViewfilterState where viewSerialNo='%d' and metadatatype='%s' and userlogin='%s' ", m_viewno, m_metadatatype, m_userlogin);
	CStaticClass::m_sqldata.executeCommand(strSqlCommand);
	for (Value::ConstMemberIterator itr = jsonObject.MemberBegin();
		itr != jsonObject.MemberEnd(); ++itr)
	{
		m_colID = itr->name.GetString();		
		const Value& objectVal = itr->value;
		std::string strjson = JsonAsString(objectVal);
		m_filterjson = strjson.c_str();	
		strSqlCommand.Format(L"exec Update_ToggleViewfilterState   '%d','%s','%s','%s','%s'", m_viewno, m_metadatatype, m_userlogin, m_colID, m_filterjson);
		CStaticClass::m_sqldata.executeCommand(strSqlCommand);
	}	
}
void updatesorting(const Value& jsonObject, int m_viewno, CString m_metadatatype, CString m_userlogin)
{
	CString m_colID = L"";
	CString  m_short = L"";
	CString strSqlCommand = L"";

	strSqlCommand.Format(L"delete from orika_ToggleViewshortingState where viewSerialNo='%d' and metadatatype='%s' and userlogin='%s' ", m_viewno, m_metadatatype, m_userlogin);
	CStaticClass::m_sqldata.executeCommand(strSqlCommand);

	if (jsonObject.IsArray())
	{
		for (SizeType i = 0; i < jsonObject.Size(); i++)
		{
			if (jsonObject[i].IsObject())
			{
				const Value& objvar = jsonObject[i];
				if (objvar.HasMember("colId"))
				{
					const Value& colIDvar = objvar["colId"];
					m_colID = colIDvar.GetString();
				}
				if (objvar.HasMember("sort"))
				{
					const Value& sortvar = objvar["sort"];
					m_short = sortvar.GetString();
				}

				strSqlCommand.Format(L"exec update_ToggleViewshortingState '%d','%s','%s','%s','%s'", m_viewno, m_metadatatype, m_userlogin, m_colID, m_short);
				CStaticClass::m_sqldata.executeCommand(strSqlCommand);
			}
		}
	}
}
std::string JsonAsString(const Value& json)
{
	std::string result;		

	StringBuffer strbuf;
	strbuf.Clear();

	Writer<StringBuffer> writer(strbuf);
	json.Accept(writer);

	result = strbuf.GetString();	
	strbuf.Clear();
	strbuf.Flush();
	return result;
}
bool Matchvalue(CString strval);
bool Matchvalue(CString strval)
{
	bool returnval = false;


	return returnval;
}


void MessageReceived(SSL_session* psession, char* c_message, int datasize,CString  key, CString login, CString HDSLno)
{	
	if (c_message == "")
	{
		return;
	}
	
	std::string message = "";	
	ProtoMessageConverter obj_converter;		
	message = obj_converter.convertprotoToString(c_message, datasize);
	
	CString strOLogFile(message.c_str());
		

	CStaticClass::m_logfile.LogEvent(strOLogFile);

	rapidjson::Document sub_document;
	if (!sub_document.Parse<0>(message.c_str()).HasParseError())
	{
		for (Value::ConstMemberIterator it = sub_document.MemberBegin(); it != sub_document.MemberEnd(); ++it)
		{
			// Get the member name (key)
			CString key = L"";
			key = it->name.GetString();
			if (key != "type")
			{
				// Get the member's value
				if (it->value.IsObject())
				{
					rapidjson::StringBuffer buffer;
					rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
					it->value.Accept(writer);
					message=buffer.GetString();
				}
			}
		}
	}

	
	int checkLoginValidate = 0;
	int m_clientType = 0;
	
	
	CString strLogFile(message.c_str());
	

	if (strLogFile == L"{}")
	{
		return;
	}
	
	SSL_session*  client = psession;
	int m_ActiveClient = 0;
		
	CString strloginuser= login + ":" + HDSLno;
	CString strKey = strloginuser;
	//(L"Going to lock lock1");
	
	CStaticClass::m_mutex_ClientList.Lock();
	//(L"lock1 Locked");
	CStaticClass::st_ClientContext m_tmp_st = {};
	CStaticClass::m_ClientContext.Lookup(strKey, m_tmp_st);
	checkLoginValidate = m_tmp_st.m_loginValidate;
	m_ActiveClient = m_tmp_st.m_activeClient;
	m_clientType = m_tmp_st.m_ClientType;
	CStaticClass::m_mutex_ClientList.Unlock();
	
	

	strloginuser = strloginuser.Mid(0, strloginuser.Find(L":"));
	std::string str = message;

	rapidjson::Document d;
	const char* strforjson = str.c_str();

	CString struser = L"";
	CString strpwd = L"";
	CString strtype = L"";
	CString _action = L"";
	CString tmpjson(str.c_str());
	////(tmpjson);
	if (!d.Parse<0>(strforjson).HasParseError())
	{
		if (d.HasMember("action") == true)
		{
			const Value& Keyuser = d["action"];
			_action = Keyuser.GetString();
		}

		if (d.HasMember("type") == true)
		{
			const Value& Keyuser = d["type"];
			strtype = Keyuser.GetString();			
			if (strtype == "login")
			{
				CUserLogin m_CUserLogin;
				int returnVal = 1;
				returnVal = m_CUserLogin.userValideate(strforjson, L"");
				if (returnVal == 0 || returnVal == 4)
				{											
					
					CStaticClass::m_mutex_ClientList.Lock();
					
					const Value& Keylogin = d["login"];
					CString strLogin = L"";
					strLogin = Keylogin.GetString();

					CString strslno = L"";
					if (d.HasMember("serialNo"))
					{
						const Value& slno = d["serialNo"];
						if (slno.IsString())
						{
							strslno = slno.GetString();
						}
					}
					CString strTabData = L"";
					strTabData = CStaticClass::m_sqldata.generateJsonForUserTabs(strLogin);

					CT2CA forconvert_strData(strTabData);
					std::string LoginData(forconvert_strData);
					
					CStaticClass::loginvalidate = 1;
					///Storing Login Details					
					CStaticClass::st_ClientContext m_st = {};
					
					m_st.m_FETCH_ORDER_DATA_FirstTime = 0;
					CMTStr::Copy(m_st.m_userlogin, strLogin);
					m_st.m_loginValidate = 1;
					int				m_alertYN = 0;
					int				m_TicksSendingStart = 0;
					int				m_DealsSendingStart = 0;
					int				m_OrdersSendingStart = 0;
					INT64			m_lastDealSended = 0;
					INT64			m_lastOrderSended = 0;
					UINT64 CurrentTime = _time64(NULL);
					Sleep(100);
					m_st.m_ConnectedTime = CurrentTime;
					m_st.m_lastDataSended = CurrentTime;



					
					CString strTime = L"";
					//strTime.Format(L"%I64u", CurrentTime);
					CStaticClass m_staticClass;
					strTime = m_staticClass.getcurrentTimeStr();

					CString strKeyCraete = strLogin + ":" + strslno + "-" + strTime;
					strKey = strKeyCraete;

					CString strLog = L"";
					

					CMTStr::Copy(psession->key, strKeyCraete);
					CMTStr::Copy(psession->login, strLogin);
					CString strSlWithTime = L"";
					strSlWithTime= strslno + "-" + strTime;
					CMTStr::Copy(psession->HDSLno, strSlWithTime);
					m_st.m_clientConnection = psession;
					CStaticClass::m_sqldata.GetLoginsClient(strLogin,m_st.m_logins);					
					const char* ipaddress = psession->addresses_sz[REMOTE];
					CString strIP(ipaddress);
					CString strIPWithPort = strIP;
					CMTStr::Copy(m_st.m_ipaddress, strIP);
					m_st.m_FETCH_ORDER_DATA_FirstTime = 0;
					strIP = strIP.Mid(0, strIP.Find(L":"));
					CMTStr::Copy(m_st.m_fingerprint, strSlWithTime);
					CString strtoken = CStaticClass::m_sqldata.generatelogintoken(strLogin, strKeyCraete, CurrentTime, strIP);
					m_st.m_primary_connection = 1;

					CMTStr::Copy(m_st.m_logintoken, strtoken);
					CStaticClass::m_sqldata.updatelogintoken(strtoken, 1, CurrentTime, 0, strLogin, strKeyCraete, strslno, strIP);

					string sstoken = string(CT2CA(strtoken));

					string  strjson = "{\"type\":\"LOGIN_STATUS\",\"status\":\"success\",\"logintoken\":\"" + sstoken + "\"," + LoginData + "}";					
					if (returnVal == 0)
					{
						HRESULT hr = NULL;
						CCommand<CAccessor<CloginListTable>> data_table;
						if (!SUCCEEDED(hr))
						{
							CStaticClass::m_mutex_ClientList.Unlock();
							////(L"U123");
							return;
						}
						//4
						CString   strCommand = L"";
						strCommand.Format(L"exec GetUserLoginList '%s';", strLogin);

						
						////(L"L10");
						CSession m_temoSession;
						m_temoSession.Open(CStaticClass::connection);
						hr = data_table.Open(m_temoSession, (LPCTSTR)strCommand);
						if (FAILED(hr))
						{
							CStaticClass::m_mutex_ClientList.Unlock();
							
							////(L"UL10");
							return;
						}
						int i = 0;
						while (hr = data_table.MoveNext() == S_OK)
						{
							CString strlogin = data_table.m_login;
							m_st.m_logins.push_back(strlogin);
						}
						m_temoSession.Close();
						
						////(L"UL10");
					}
					m_st.m_clientConnection = client;

					m_st.m_startCalculationThread = 1;
					m_st.m_activeClient = 1;
					m_st.m_ClientType = 1;
					CStaticClass::m_ClientContext.SetAt(strKeyCraete, m_st);
					///End of storing Login Details					
					CStaticClass::m_mutex_ClientList.Unlock();
					//(L"123 UnLocked");
					SendDataToClient(client, strjson, strKey, m_st.m_activeClient);
					//CString strLog = L"";
					//strLog.Format(L"Going to register a new client with session ID %s", strKeyCraete);
					//(strLog);
					
					//(L"U321");

					//Start A thread for New Client
					EnterCriticalSection(&CStaticClass::m_cs_Thread);
					CStaticClass::strKeyTransfer = strKey;

					m_st.m_localThred=AfxBeginThread(threadForLoginWiseCalculate, NULL);

					SleepConditionVariableCS(&CStaticClass::m_cv_Thread, &CStaticClass::m_cs_Thread, INFINITE);
					LeaveCriticalSection(&CStaticClass::m_cs_Thread);

					CStaticClass::m_mutex_ClientList.Lock();
					CStaticClass::m_ClientContext.SetAt(strKeyCraete, m_st);
					///End of storing Login Details	
					CString m_strloginuser = L"";
					CStaticClass::stloginUserDetail m_stuser = {};
					CStaticClass::m_loginuserlist.Lookup(strloginuser, m_stuser);
					
					CString strname = m_stuser.m_name;					
					CStaticClass::m_mutex_ClientList.Unlock();
					//(L"UN321");


					CString m_silgleData = L"";
					m_silgleData.Format(L"{\"loginuser\":\"%s\",\"name\":\"%s\",\"fingerprint\":\"%s\",\"status\":\"online\",\"ipaddress\":\"%s\",\"sessionid\":\"%s\"}", strloginuser, strname, strSlWithTime, strIPWithPort, strtoken);
					CString m_rval = L"";
					m_rval.Format(L"{\"type\": \"USERS_DATA\",\"action\":\"addsession\",\"users\": [%s]}", m_silgleData);
					string strforsend = CT2A(m_rval.GetString());					
					//SendDataToClient(client, strforsend, strKey, m_ActiveClient);
				}
				else if (returnVal == 1)
				{
					string strforsend = "{\"type\":\"LOGIN_STATUS\",\"status\":\"invalid credentials\"}";					
					SendDataToClient_UnRegistered(psession, strforsend);
				}
				else if (returnVal == 2)
				{
					string strforsend="{\"type\":\"LOGIN_STATUS\",\"status\":\"invalid SerialNo\"}";					
					SendDataToClient_UnRegistered(psession, strforsend);
				}
				else if (returnVal == 3)
				{
					string strforsend = "{\"type\":\"LOGIN_STATUS\",\"status\":\"Invalid ApiLogin\"}";					
					SendDataToClient_UnRegistered(psession, strforsend);
				}
			}

			if (strtype == "loginbytoken")
			{
				//(L"Going to lock client lock loginbytoken");
				CStaticClass::m_mutex_ClientList.Lock();
				//(L"Client lock locked");
					CStaticClass::st_ClientContext m_check_allreadyRegister = {};
					CStaticClass::m_ClientContext.Lookup(strKey, m_check_allreadyRegister);
					CString m_registeredKey = L"";
					m_registeredKey = m_check_allreadyRegister.m_userlogin;
				CStaticClass::m_mutex_ClientList.Unlock();
				//(L"Exit from client lock");
				if (m_registeredKey.Trim() != L"")
				{
					return;
				}

				CString m_logintoken = L"";
				const Value& vallogintoken = d["logintoken"];
				m_logintoken = vallogintoken.GetString();
				CUserLogin m_CUserLogin;
				int returnVal = 1;
				
				const char* ipaddress = psession->addresses_sz[REMOTE];
				CString strIP(ipaddress);
				strIP = strIP.Mid(0, strIP.Find(L":"));
				CSqlData::st_logintokendetail m_temp = {};
				CStaticClass::m_sqldata.validatelogintoken(m_logintoken, strIP, m_temp);
				if (m_temp.returnval == 0 )
				{
					UINT64 CurrentTime = _time64(NULL);
					//Sleep(100);
					CString strTime = L"";
					//strTime.Format(L"%I64u", CurrentTime);					
					CStaticClass m_staticClass;
					strTime = m_staticClass.getcurrentTimeStr();

					CString strLogin = L"";
					strLogin = m_temp.m_login;
					CString strslno = L"";
					strslno = m_temp.m_hdno;
					CString strKeyCraete = strLogin + ":" + strslno + "-" + strTime;
					strKey = strKeyCraete;
					CString strLog = L"";
					//strLog.Format(L"New Key Created loginbytoken Validated %s", strKey);
					//(strLog);
					//CStaticClass::m_mutex_ClientList.Lock();
					//////(L"122");
					//CStaticClass::m_ClientContext.RemoveKey(strKey);
					//CStaticClass::m_mutex_ClientList.Unlock();
					//(L"U122");


					CStaticClass::m_mutex_ClientList.Lock();
					//(L"123");
					
					

					CString strTabData = L"";
					strTabData = CStaticClass::m_sqldata.generateJsonForUserTabs(strLogin);
					

					CT2CA forconvert_strData(strTabData);
					std::string LoginData(forconvert_strData);


					CStaticClass::loginvalidate = 1;
					///Storing Login Details					
					CStaticClass::st_ClientContext m_st = {};					
					
					m_st.m_FETCH_ORDER_DATA_FirstTime = 0;
					CMTStr::Copy(m_st.m_userlogin, strLogin);
					
					int				m_alertYN = 0;
					int				m_TicksSendingStart = 0;
					int				m_DealsSendingStart = 0;
					int				m_OrdersSendingStart = 0;
					INT64			m_lastDealSended = 0;
					INT64			m_lastOrderSended = 0;
					
					m_st.m_ConnectedTime = CurrentTime;
					m_st.m_lastDataSended = CurrentTime;




					

					
					CMTStr::Copy(psession->key, strKeyCraete);
					CMTStr::Copy(psession->login, strLogin);
					CString strSlWithTime = L"";
					strSlWithTime = strslno+ "-" + strTime;
					CMTStr::Copy(psession->HDSLno, strSlWithTime);
					m_st.m_clientConnection = psession;
					m_st.m_loginValidate = 1;
					CStaticClass::m_sqldata.GetLoginsClient(strLogin,m_st.m_logins);					
					//const char* ipaddress = psession->addresses_sz[REMOTE];

					CString strtoken = L"";
					//m_st.m_primary_connection = 1;
					CMTStr::Copy(m_st.m_logintoken, strtoken);
					//(L"123_1");
					CStaticClass::m_sqldata.updatelogintoken(strtoken, 1, CurrentTime, 0, strLogin, strKeyCraete, strslno, strIP);
					//(L"123_2");
					string sstoken = string(CT2CA(strtoken));
					string  strjson = "{\"type\":\"LOGIN_STATUS\",\"status\":\"success\"," + LoginData + "}";									
						HRESULT hr = NULL;
						CCommand<CAccessor<CloginListTable>> data_table;
						if (!SUCCEEDED(hr))
						{
							CStaticClass::m_mutex_ClientList.Unlock();
							////(L"U123");
							return;
						}
						//2
						CString   strCommand = L"";
						strCommand.Format(L"exec GetUserLoginList '%s';", strLogin);

						//CStaticClass::m_mutex_order.Lock();
						////(L"L10");
						CSession m_sqlsession;
						m_sqlsession.Open(CStaticClass::connection);
						hr = data_table.Open(m_sqlsession, (LPCTSTR)strCommand);
						if (FAILED(hr))
						{
							CStaticClass::m_mutex_ClientList.Unlock();
							//CStaticClass::m_mutex_order.Unlock();
							////(L"UL10");
							return;
						}
						int i = 0;
						while (hr = data_table.MoveNext() == S_OK)
						{
							CString strlogin = data_table.m_login;
							m_st.m_logins.push_back(strlogin);
						}
						m_sqlsession.Close();
						//CStaticClass::m_mutex_order.Unlock();
						//(L"UL10");
					
					m_st.m_clientConnection = client;

					m_st.m_startCalculationThread = 1;
					m_st.m_activeClient = 1;
					m_st.m_ClientType = 1;

					CMTStr::Copy(psession->HDSLno, strSlWithTime);
					m_st.m_clientConnection = psession;
					
					const char* ipaddress = psession->addresses_sz[REMOTE];
					CString strIP(ipaddress);
					CMTStr::Copy(m_st.m_ipaddress, strIP);
					m_st.m_FETCH_ORDER_DATA_FirstTime = 0;
					strIP = strIP.Mid(0, strIP.Find(L":"));	
					CMTStr::Copy(m_st.m_fingerprint, strSlWithTime);
					CStaticClass::m_ClientContext.SetAt(strKeyCraete, m_st);
					///End of storing Login Details					
					CStaticClass::m_mutex_ClientList.Unlock();
					SendDataToClient(client, strjson, strKey, m_st.m_activeClient);

					//(L"U123");

					//Start A thread for New Client
					EnterCriticalSection(&CStaticClass::m_cs_Thread);
					CStaticClass::strKeyTransfer = strKey;
					//(L"Going to enter in critical section");
					m_st.m_localThred=AfxBeginThread(threadForLoginWiseCalculate, NULL);
					//(L"Entered in critical section");
					SleepConditionVariableCS(&CStaticClass::m_cv_Thread, &CStaticClass::m_cs_Thread, INFINITE);
					LeaveCriticalSection(&CStaticClass::m_cs_Thread);
					//(L"U123_21");
					CStaticClass::m_mutex_ClientList.Lock();
					CStaticClass::m_ClientContext.SetAt(strKeyCraete, m_st);
					///End of storing Login Details					
					CStaticClass::m_mutex_ClientList.Unlock();
					//(L"U123_1");
				}
				else if (m_temp.returnval == 1)
				{
					string strforsend = "{\"type\":\"LOGIN_STATUS\",\"status\":\"invalid login token\"}";					
					SendDataToClient_UnRegistered(psession, strforsend);					
					/*session_close(psession);
					app_on_session_close(psession);*/
					send_close_message_to_client(psession);
					return;
				}
				else if (m_temp.returnval == 2)
				{
					string strforsend = "{\"type\":\"LOGIN_STATUS\",\"status\":\"This token is not valid for this ip\"}";					
					SendDataToClient_UnRegistered(psession, strforsend);					
					/*session_close(psession);
					app_on_session_close(psession);*/
					send_close_message_to_client(psession);
					return;
				}				
			}
			if (strtype == "LOGIN_BY_KEY")
			{
				//(L"Going to lock client lock loginbytoken");
				CStaticClass::m_mutex_ClientList.Lock();
				//(L"Client lock locked");
				CStaticClass::st_ClientContext m_check_allreadyRegister = {};
				CStaticClass::m_ClientContext.Lookup(strKey, m_check_allreadyRegister);
				CString m_registeredKey = L"";
				m_registeredKey = m_check_allreadyRegister.m_userlogin;
				CStaticClass::m_mutex_ClientList.Unlock();
				//(L"Exit from client lock");
				if (m_registeredKey.Trim() != L"")
				{
					return;
				}

				CString m_authKey = L"";
				if (d.HasMember("authKey"))
				{
					const Value& authKey = d["authKey"];
					m_authKey = authKey.GetString();
				}
				if (d.HasMember("token"))
				{
					const Value& authKey = d["token"];
					m_authKey = authKey.GetString();
				}
				CUserLogin m_CUserLogin;
				int returnVal = 1;

				const char* ipaddress = psession->addresses_sz[REMOTE];
				CString strIP(ipaddress);
				strIP = strIP.Mid(0, strIP.Find(L":"));
				CSqlData::st_logintokendetail m_temp = {};
				CStaticClass::m_sqldata.validateauthKey(m_authKey, m_temp);
				if (m_temp.returnval == 0)
				{
					UINT64 CurrentTime = _time64(NULL);
					//Sleep(100);
					CString strTime = L"";
					//strTime.Format(L"%I64u", CurrentTime);					
					CStaticClass m_staticClass;
					strTime = m_staticClass.getcurrentTimeStr();

					CString strLogin = L"";
					strLogin = m_temp.m_login;
					CString strslno = L"";
					strslno = m_temp.m_hdno;
					CString strKeyCraete = strLogin + ":" + strslno + "-" + strTime;
					strKey = strKeyCraete;
					CString strLog = L"";
					//strLog.Format(L"New Key Created loginbytoken Validated %s", strKey);
					//(strLog);
					//CStaticClass::m_mutex_ClientList.Lock();
					//////(L"122");
					//CStaticClass::m_ClientContext.RemoveKey(strKey);
					//CStaticClass::m_mutex_ClientList.Unlock();
					//(L"U122");


					CStaticClass::m_mutex_ClientList.Lock();
					//(L"123");



					CString strTabData = L"";
					strTabData = CStaticClass::m_sqldata.generateJsonForUserTabs(strLogin);


					CT2CA forconvert_strData(strTabData);
					std::string LoginData(forconvert_strData);


					CStaticClass::loginvalidate = 1;
					///Storing Login Details					
					CStaticClass::st_ClientContext m_st = {};

					m_st.m_FETCH_ORDER_DATA_FirstTime = 0;
					CMTStr::Copy(m_st.m_userlogin, strLogin);

					int				m_alertYN = 0;
					int				m_TicksSendingStart = 0;
					int				m_DealsSendingStart = 0;
					int				m_OrdersSendingStart = 0;
					INT64			m_lastDealSended = 0;
					INT64			m_lastOrderSended = 0;

					m_st.m_ConnectedTime = CurrentTime;
					m_st.m_lastDataSended = CurrentTime;







					CMTStr::Copy(psession->key, strKeyCraete);
					CMTStr::Copy(psession->login, strLogin);
					CString strSlWithTime = L"";
					strSlWithTime = strslno + "-" + strTime;
					CMTStr::Copy(psession->HDSLno, strSlWithTime);
					m_st.m_clientConnection = psession;
					m_st.m_loginValidate = 1;
					CStaticClass::m_sqldata.GetLoginsClient(strLogin, m_st.m_logins);
					//const char* ipaddress = psession->addresses_sz[REMOTE];

					CString strtoken = L"";
					//m_st.m_primary_connection = 1;
					CMTStr::Copy(m_st.m_logintoken, strtoken);
					//(L"123_1");
					CStaticClass::m_sqldata.updatelogintoken(strtoken, 1, CurrentTime, 0, strLogin, strKeyCraete, strslno, strIP);
					//(L"123_2");
					string sstoken = string(CT2CA(strtoken));
					string  strjson = "{\"type\":\"LOGIN_STATUS\",\"status\":\"success\"," + LoginData + "}";

					HRESULT hr = NULL;
					CCommand<CAccessor<CloginListTable>> data_table;
					if (!SUCCEEDED(hr))
					{
						CStaticClass::m_mutex_ClientList.Unlock();
						////(L"U123");
						return;
					}
					//3
					CString   strCommand = L"";
					strCommand.Format(L"exec GetUserLoginList '%s';", strLogin);

					//CStaticClass::m_mutex_order.Lock();
					////(L"L10");
					CSession m_sqlsession;
					m_sqlsession.Open(CStaticClass::connection);
					hr = data_table.Open(m_sqlsession, (LPCTSTR)strCommand);
					if (FAILED(hr))
					{
						CStaticClass::m_mutex_ClientList.Unlock();
						//CStaticClass::m_mutex_order.Unlock();
						////(L"UL10");
						return;
					}
					int i = 0;
					while (hr = data_table.MoveNext() == S_OK)
					{
						CString strlogin = data_table.m_login;
						m_st.m_logins.push_back(strlogin);
					}
					m_sqlsession.Close();
					//CStaticClass::m_mutex_order.Unlock();
					//(L"UL10");

					m_st.m_clientConnection = client;

					m_st.m_startCalculationThread = 1;
					m_st.m_activeClient = 1;
					m_st.m_ClientType = 1;

					CMTStr::Copy(psession->HDSLno, strSlWithTime);
					m_st.m_clientConnection = psession;

					const char* ipaddress = psession->addresses_sz[REMOTE];
					CString strIP(ipaddress);
					CMTStr::Copy(m_st.m_ipaddress, strIP);
					m_st.m_FETCH_ORDER_DATA_FirstTime = 0;
					strIP = strIP.Mid(0, strIP.Find(L":"));
					CMTStr::Copy(m_st.m_fingerprint, strSlWithTime);
					CStaticClass::m_ClientContext.SetAt(strKeyCraete, m_st);
					///End of storing Login Details					
					CStaticClass::m_mutex_ClientList.Unlock();
					SendDataToClient(client, strjson, strKey, m_st.m_activeClient);

					//(L"U123");

					//Start A thread for New Client
					EnterCriticalSection(&CStaticClass::m_cs_Thread);
					CStaticClass::strKeyTransfer = strKey;
					//(L"Going to enter in critical section");
					m_st.m_localThred = AfxBeginThread(threadForLoginWiseCalculate, NULL);
					//(L"Entered in critical section");
					SleepConditionVariableCS(&CStaticClass::m_cv_Thread, &CStaticClass::m_cs_Thread, INFINITE);
					LeaveCriticalSection(&CStaticClass::m_cs_Thread);
					//(L"U123_21");
					CStaticClass::m_mutex_ClientList.Lock();
					CStaticClass::m_ClientContext.SetAt(strKeyCraete, m_st);
					///End of storing Login Details					
					CStaticClass::m_mutex_ClientList.Unlock();
					//(L"U123_1");
				}
				else if (m_temp.returnval == 1)
				{
					string strforsend = "{\"type\":\"LOGIN_STATUS\",\"status\":\"invalid login token\"}";
					SendDataToClient_UnRegistered(psession, strforsend);
					/*session_close(psession);
					app_on_session_close(psession);*/
					send_close_message_to_client(psession);
					return;
				}
			}
			else if (strtype == L"FETCH_CLIENT_POSITIONS_TIMERWISE_START")
			{

			/*TMTArray<wchar_t[250]> m_ColumnsData;
			CStaticClass::m_sqldata.getMappingSymbol(m_ColumnsData, strloginuser, strtype);*/
			
				CStaticClass::st_ClientContext m_st = {};
				
				CStaticClass::m_mutex_ClientList.Lock();
				//(L"P122");
				CStaticClass::m_ClientContext.Lookup(strKey, m_st);
				m_st.m_fetch_client_position_timerwise = 1;
				//m_st.m_clientrequests_List.Assign(m_ColumnsData);

				if (d.HasMember("time"))
				{
					const Value& Keytime = d["time"];
					if (Keytime.IsNumber())
					{
						m_st.m_calculatePreNetPosition = 1;

						if (_action == L"refresh")
						{
							m_st.m_refreshClientPosition = 1;
						}
						UINT64 positionTime = 0;
						positionTime = Keytime.GetInt64();
						m_st.m_netPositionTime_TIMERWISE = positionTime;
					}
				}



				CStaticClass::m_ClientContext.SetAt(strKey, m_st);
				CStaticClass::m_mutex_ClientList.Unlock();
				//(L"UP122");
			}
			else if (strtype == L"FETCH_CLIENT_POSITIONS_TIMERWISE_STOP")
			{
				CStaticClass::st_ClientContext m_st = {};

				CStaticClass::m_mutex_ClientList.Lock();
				CStaticClass::m_ClientContext.Lookup(strKey, m_st);
				m_st.m_fetch_client_position_timerwise = 0;
				CStaticClass::m_ClientContext.SetAt(strKey, m_st);
				CStaticClass::m_mutex_ClientList.Unlock();
			}


			else if (strtype == L"FETCH_SUBBROKERDATA")
			{	
				CString strsubbroker = L"";
				if (d.HasMember("subbroker"))
				{
					const Value& Keytime = d["subbroker"];
					if (Keytime.IsString())
					{
						strsubbroker = Keytime.GetString();
					}
				}
				CString strjson = CStaticClass::m_sqldata.GetOrika_subbrokerdata(strsubbroker);
				string strforsend = CT2A(strjson.GetString());
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
			}
			else if (strtype == L"FETCH_BROKERDATA")
			{
				CString strbroker = L"";
				if (d.HasMember("broker"))
				{
					const Value& Keytime = d["broker"];
					if (Keytime.IsString())
					{
						strbroker = Keytime.GetString();
					}
				}
				CString strjson = CStaticClass::m_sqldata.GetOrika_brokerdata(strbroker);
				string strforsend = CT2A(strjson.GetString());
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
			}
			else if (strtype == L"FETCH_CLIENTDATA")
			{
			CString strlogin = L"";
			if (d.HasMember("login"))
			{
				const Value& Keytime = d["login"];
				if (Keytime.IsString())
				{
					strlogin = Keytime.GetString();
				}
			}
			CString strjson = CStaticClass::m_sqldata.GetOrika_clientmaster(strlogin);
			string strforsend = CT2A(strjson.GetString());
			SendDataToClient(client, strforsend, strKey, m_ActiveClient);

			}

			else if (strtype == L"FETCH_DASHBOARD_DATA")
			{			
		
			/*TMTArray<wchar_t[250]> m_ColumnsData;
			CStaticClass::m_sqldata.getMappingSymbol(m_ColumnsData, strloginuser, strtype);*/
			
				CStaticClass::st_ClientContext m_st = {};
				CStaticClass::m_mutex_ClientList.Lock();
				CStaticClass::m_ClientContext.Lookup(strKey, m_st);
				m_st.m_sendDashboardData = 1;
				//m_st.m_clientrequests_List.Assign(m_ColumnsData);
				if (d.HasMember("time"))
				{
					const Value& Keytime = d["time"];
					if (Keytime.IsNumber())
					{
						UINT64 positionTime = 0;
						positionTime = Keytime.GetInt64();
						m_st.m_DashboardTime = positionTime;
					}
				}
				CStaticClass::m_ClientContext.SetAt(strKey, m_st);
				CStaticClass::m_mutex_ClientList.Unlock();				
			}
 			else if ((strtype == L"SUBSCRIBE_POSITION_FOR_TRANSFER" ) && checkLoginValidate == 1 && m_clientType==1)
			{
				CStaticClass::st_ClientContext m_st = {};
				//(L"P123");
				CStaticClass::m_mutex_ClientList.Lock();				
					CStaticClass::m_ClientContext.Lookup(strKey, m_st);
					m_st.m_SubscribedRequest.push_back(SUBSCRIBE_POSITION_FOR_TRANSFER);
				CStaticClass::m_mutex_ClientList.Unlock();
				//(L"UP123");
			}			
			else if ((strtype == L"FETCH_DEALING_DATA_INTERVAL") && checkLoginValidate == 1 && m_clientType == 1)
			{

				CString  strclientkey = L"";
				strclientkey = client->key;
				UINT64 m_datefrom = 0;
				UINT64 m_dateto = 0;
				CString m_subscriptionId = L"";
				if (d.HasMember("subscriptionId"))
				{
					const Value& valsubscriptionId = d["subscriptionId"];
					if (valsubscriptionId.IsString())
					{
						m_subscriptionId = valsubscriptionId.GetString();
					}
				}

				if (d.HasMember("startTime"))
				{
					const Value& valstartTime = d["startTime"];
					if (valstartTime.IsNumber())
					{
						m_datefrom = valstartTime.GetInt64();
					}
				}
				if (d.HasMember("endTime"))
				{
					const Value& valendTime = d["endTime"];
					if (valendTime.IsNumber())
					{
						m_dateto = valendTime.GetInt64();
					}
				}
				CString strVal = L"";
				strVal = CStaticClass::m_mtmanager.FetchHistoricalDealingFromMemory(m_datefrom, m_dateto, strclientkey, m_subscriptionId);

				string strforsend = CT2A(strVal.GetString());
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);

			}
			else if ((strtype == L"FETCH_CLIENT_POSITIONS" || strtype == L"FETCH_CLIENT_POSITIONS_PRENETQTY" || strtype == L"FETCH_ORDER_DATA" || strtype == L"FETCH_DEALING_DATA" || strtype == L"FETCH_LOGIN_DEVICE_LOG" || strtype == L"EXISTING_COMMENT_CHANGE_DATA") && checkLoginValidate == 1)
			{									
				CStaticClass::st_ClientContext m_st = {};						
				CStaticClass::m_mutex_ClientList.Lock();						
				CStaticClass::m_ClientContext.Lookup(strKey, m_st);
				CStaticClass::m_mutex_ClientList.Unlock();												
								
						CString strTabName = L"";
						strTabName = strtype;
						//m_st.m_logins.push_back(strTabName);

						////Udating Subscribed Column
						////(L"P124");
						//CStaticClass::m_mutex_ClientList.Lock();
						////(L"P124_Locked");
						///*CStaticClass::st_ClientContext m_st = {};
						//CStaticClass::m_ClientContext.Lookup(strKey, m_st);*/

						//TMTArray<wchar_t[250]> m_ColumnsData;
						//m_ColumnsData.Assign(m_st.m_clientrequests_List);
						///*CStaticClass::m_sqldata.getMappingSymbol(m_ColumnsData, strloginuser, strTabName);
						//m_st.m_clientrequests_List.Assign(m_ColumnsData);*/

						//int checkData = m_ColumnsData.Total();
						//for (int r = 0; r < checkData; r++)
						//{
						//	CString str = m_ColumnsData[r];
						//	int i = 0;
						//}
						//CStaticClass::m_mutex_ClientList.Unlock();
						////(L"UP124");
						////End of Udating Subscribed Column

						if (strTabName == "FETCH_CLIENT_POSITIONS")
						{
							CString strjson = L"";
							m_st.m_ClientWiseNetPositionStart_FirstTime = 1;									
							UINT64 positionTime = 0;
							if (d.HasMember("time"))
							{
								const Value& Keytime = d["time"];
								if (Keytime.IsNumber())
								{
									positionTime = Keytime.GetInt64();
								}
							}
									m_st.m_calculatePreNetPosition = 1;
										
									m_st.m_firstTimeDataSended = 0;

									if (_action == L"refresh")
									{
										m_st.m_refreshClientPosition = 1;
										
									}
									
																			
									m_st.m_netPositionTime = positionTime;
								
							
							if (m_st.m_netPositionTime == 0)
							{
								CStaticClass msc;
								m_st.m_netPositionTime = msc.getcurrentTime_Unix();
							}
						}
						//m_getPreQtyUpdate
						if (strTabName == "FETCH_CLIENT_POSITIONS_PRENETQTY")
						{
							CString strjson = L"";
							if (d.HasMember("time"))
							{
								const Value& Keytime = d["time"];
								if (Keytime.IsNumber())
								{

									UINT64 positionTime = 0;
									positionTime = Keytime.GetInt64();
									m_st.m_getPreQtyUpdate = 1;
									m_st.m_netPositionTime = positionTime;
								}
							}
						}								
						if (strTabName == L"FETCH_ORDER_DATA")
						{													
							CStaticClass::m_mutex_ClientList.Lock();
							m_st.m_firstTimeDataSended = 0;
							m_st.m_FETCH_ORDER_DATA_FirstTime = 1;
							CStaticClass::m_ClientContext.SetAt(strKey, m_st);
							CStaticClass::m_mutex_ClientList.Unlock();
							

							CString strjson = CStaticClass::m_sqldata.getOrderData(strKey);
							string strforsend = CT2A(strjson.GetString());
							SendDataToClient(client, strforsend, strKey, m_st.m_activeClient);


						}								
						
						if (strTabName == L"FETCH_DEALING_DATA")
						{							
							CString  strclientkey = L"";
							strclientkey = client->key;
							UINT64 m_datefrom = 0;
							UINT64 m_dateto = 0;
							CString m_subscriptionId = L"";
							if (d.HasMember("subscriptionId"))
							{
								const Value& valsubscriptionId = d["subscriptionId"];
								if (valsubscriptionId.IsString())
								{
									m_subscriptionId = valsubscriptionId.GetString();
								}
							}

							if (d.HasMember("startTime"))
							{
								const Value& valstartTime = d["startTime"];
								if (valstartTime.IsNumber())
								{
									m_datefrom = valstartTime.GetInt64();
								}
							}
							if (d.HasMember("endTime"))
							{
								const Value& valendTime = d["endTime"];
								if (valendTime.IsString())
								{
									m_dateto = valendTime.GetInt64();
								}
							}
							m_dateto = 0;
														

							//CStaticClass::m_mtmanager.FetchHistoricalDealingData(m_datefrom, m_dateto, strclientkey);
							//m_subscriptionId
							
								m_st.m_FETCH_DEALING_DATA_start = 1;
								CStaticClass::m_mutex_dealingClientList.Lock();
								CStaticClass::m_ClientList_forDeal.SetAt(strclientkey, psession);
								CStaticClass::m_ClientList_forOrder.SetAt(strclientkey, psession);
								CStaticClass::m_mutex_dealingClientList.Unlock();
							
						}


						if (strTabName == L"FETCH_LOGIN_DEVICE_LOG")
						{
							CString  strclientkey = L"";
							strclientkey = client->key;
							UINT64 m_datefrom = 0;
							UINT64 m_dateto = 0;
							CString m_subscriptionId = L"";							
							if (d.HasMember("startTime"))
							{
								const Value& valstartTime = d["startTime"];
								if (valstartTime.IsNumber())
								{
									m_datefrom = valstartTime.GetInt64();
								}
							}
							if (d.HasMember("endTime"))
							{
								const Value& valendTime = d["endTime"];
								if (valendTime.IsNumber())
								{
									m_dateto = valendTime.GetInt64();
								}
							}
							UINT64 m_datetosplit = 0;
							while (m_datefrom <= m_dateto)
							{
								CString m_logdata = L"";
								m_datetosplit= m_datefrom + (5 * 3600);
								m_logdata = CStaticClass::m_mtmanager.FetchLogFromMemory(m_datefrom, m_datetosplit, L"");

								string strforsend = CT2A(m_logdata.GetString());
								SendDataToClient(client, strforsend, strKey, m_ActiveClient);
								m_datefrom = m_datetosplit+1;
								if (m_datefrom > m_dateto)
								{
									m_datefrom = m_dateto+1;
								}
							}

						}

						//FETCH_DEALING_DATA								
						if (strTabName == "EXISTING_COMMENT_CHANGE_DATA")
						{
							/*m_st.m_FETCH_EXISTING_COMMENT_CHANGE_DATA_FirstTime = 1;
							m_st.m_firstTimeDataSended = 0;*/
									
						}
								
					
				
				CStaticClass::m_mutex_ClientList.Lock();
				CStaticClass::m_ClientContext.SetAt(strKey, m_st);
				CStaticClass::m_mutex_ClientList.Unlock();
				////(L"U124");
				////(L"Unlocklock3");

					

				
			}

			else if (strtype == L"UNSUBSCRIBE"  && checkLoginValidate == 1)
			{
			CStaticClass::st_ClientContext m_st = {};
			CStaticClass::m_mutex_ClientList.Lock();
			CStaticClass::m_ClientContext.Lookup(strKey, m_st);
			CStaticClass::m_mutex_ClientList.Unlock();

			CString requestType = L"";
			if (d.HasMember("requestType"))
			{
				const Value& KeyrequestType = d["requestType"];
				if (KeyrequestType.IsString())
				{
					requestType = KeyrequestType.GetString();
				}
			}
			

			if (requestType == "FETCH_CLIENT_POSITIONS")
			{
				CString strjson = L"";
				m_st.m_ClientWiseNetPositionStart_FirstTime = 0;
				m_st.m_calculatePreNetPosition = 0;
				m_st.m_refreshClientPosition = 0;
				m_st.m_ClientWiseNetPositionStart = 0;
				string strforsend = "{\"type\":\"SERVER_MESSAGE\",\"responseMessage\":\"FETCH_CLIENT_POSITIONS has been unsubscribed successfully.\"}";
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
			}			
			if (requestType == L"FETCH_ORDER_DATA")
			{
				CStaticClass::m_mutex_ClientList.Lock();
					m_st.m_FETCH_ORDER_DATA_FirstTime = 0;
					m_st.m_firstTimeDataSended = 0;
				CStaticClass::m_mutex_ClientList.Unlock();
			}
			//FETCH_DEALING_DATA
			if (requestType == L"FETCH_DEALING_DATA")
			{
				m_st.m_FETCH_DEALING_DATA_start = 0;
				CString  strclientkey = L"";
				strclientkey = client->key;
				CStaticClass::m_mutex_dealingClientList.Lock();
					CStaticClass::m_ClientList_forDeal.RemoveKey(strclientkey);
					CStaticClass::m_ClientList_forOrder.RemoveKey(strclientkey);
				CStaticClass::m_mutex_dealingClientList.Unlock();
			}
			//FETCH_DEALING_DATA								
			if (requestType == "EXISTING_COMMENT_CHANGE_DATA")
			{
				m_st.m_FETCH_EXISTING_COMMENT_CHANGE_DATA_FirstTime = 0;
				m_st.m_firstTimeDataSended = 0;

			}

			if (requestType == "FETCH_TICK_DATA")
			{
				m_st.m_TicksSendingStart = 0;				
			}

			CStaticClass::m_mutex_ClientList.Lock();
			CStaticClass::m_ClientContext.SetAt(strKey, m_st);
			CStaticClass::m_mutex_ClientList.Unlock();
			////(L"U124");
			////(L"Unlocklock3");




			}
			else if (strtype == "CLOSING_UPDATE_UPLOAD" && checkLoginValidate == 1)
			{
				CString FilePath = L"";
				CString closingDate = L"";
				if (d.HasMember("filename"))
				{
					const Value& valPath = d["filename"];																
					if (valPath.IsString())
					{
						FilePath = valPath.GetString();						
					}																													
				}
				if (d.HasMember("closingdate"))
				{
					const Value& valcldate = d["closingdate"];					
					if (valcldate.IsString())
					{
						closingDate = valcldate.GetString();						
					}
				}
				CString strReturnJson = L"";
				strReturnJson = CStaticClass::m_mtmanager.ClosingFileTransfer(FilePath, closingDate);
				string strforsend = CT2A(strReturnJson.GetString());
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
			}
			else if (strtype == "SYMBOL_FILE_TRANSFER" && checkLoginValidate == 1)
			{
				CString returnJson = L"";
				if (d.HasMember("filename"))
				{
					const Value& valPath = d["filename"];
					CString FilePath = L"";
					
						
							if (valPath.IsString())
							{
								FilePath = valPath.GetString();
								returnJson=CStaticClass::m_mtmanager.symbolMapping(FilePath);
								string strforsend = CT2A(returnJson.GetString());
								SendDataToClient(client, strforsend, strKey, m_ActiveClient);								
							}											
				}
			}
			else if (strtype == "IGNORE_CLOSINGANDSAVE" && checkLoginValidate == 1)
			{
				CStaticClass::m_ignoreClosingprice.RemoveAll();
				CString returnJson = L"";
				if (d.HasMember("data"))
				{
					const Value& valdataarray = d["data"];					
					if (valdataarray.IsArray())
					{
						for (SizeType i = 0; i < valdataarray.Size(); i++)
						{							
							if (valdataarray[i].IsObject())
							{
								const Value& IgnoreClosingObjects = valdataarray[i];
								CString strIgnoreSymbol = L"";
								if (IgnoreClosingObjects.HasMember("mtsymbol"))
								{
									const Value& ignoreSymbol = IgnoreClosingObjects["mtsymbol"];									
									strIgnoreSymbol = ignoreSymbol.GetString();																		
								}
								double dbl_closing = 0;
								if (IgnoreClosingObjects.HasMember("closingprice"))
								{
									const Value& val_closing = IgnoreClosingObjects["closingprice"];									
									if (val_closing.IsDouble() )
									{
										dbl_closing = val_closing.GetDouble();
									}
									if (val_closing.IsInt())
									{
										dbl_closing = val_closing.GetInt();
									}									
									
								}
								if (IgnoreClosingObjects.HasMember("select"))
								{
									const Value& val_select = IgnoreClosingObjects["select"];
									if (val_select.IsBool())
									{
										bool m_select = val_select.GetBool();
										if (m_select == true)
										{
											CStaticClass::m_ignoreClosingprice.SetAt(strIgnoreSymbol, dbl_closing);
										}
									}
								}
								if (IgnoreClosingObjects.HasMember("ignore"))
								{
									const Value& val_select = IgnoreClosingObjects["ignore"];
									if (val_select.IsBool())
									{
										bool m_select = val_select.GetBool();
										if (m_select == true)
										{
											CStaticClass::m_ignoreClosingprice.SetAt(strIgnoreSymbol, dbl_closing);
										}
									}
								}

								double dbl_high = 0;
								if (IgnoreClosingObjects.HasMember("high"))
								{
									const Value& val_high = IgnoreClosingObjects["high"];									
									if (val_high.IsDouble())
									{
										dbl_high = val_high.GetDouble();
									}
									if (val_high.IsInt())
									{
										dbl_high = val_high.GetInt();
									}
								}
								double dbl_low = 0;
								if (IgnoreClosingObjects.HasMember("low"))
								{
									const Value& val_low = IgnoreClosingObjects["low"];									
									if (val_low.IsDouble())
									{
										dbl_low = val_low.GetDouble();
									}
									if (val_low.IsInt())
									{
										dbl_low = val_low.GetInt();
									}
								}								
							}
						}
						CStaticClass::m_mtmanager.SaveClosingRate();
						CString strReturnJson = L"{\"type\":\"CLOSING_UPDATE_SUCCESS\",\"message\" : \"ClosingPrice Updated\"}";;
						string strforsend = CT2A(strReturnJson.GetString());
						SendDataToClient(client, strforsend, strKey, m_ActiveClient);
					}
				}
			}			
			else if (strtype == "UPDATE_CLOSINGPRICE" && checkLoginValidate == 1)
			{
				CString Insert_Command = L"";
				CStaticClass::m_ignoreClosingprice.RemoveAll();
				CString returnJson = L"";
				if (d.HasMember("data"))
				{
					const Value& valdataarray = d["data"];
					if (valdataarray.IsArray())
					{
						for (SizeType i = 0; i < valdataarray.Size(); i++)
						{
							if (valdataarray[i].IsObject())
							{
								CString strSymbol = L"";
								double dblclosingprice = 0;
								double dblhigh = 0;
								double dbllow = 0;
							
								const Value& ClosingObjects = valdataarray[i];
								if (ClosingObjects.HasMember("mtsymbol"))
								{
									const Value& Symbol = ClosingObjects["mtsymbol"];
									if (Symbol.IsString())
									{
										strSymbol = Symbol.GetString();
									}
								
								}
								if (ClosingObjects.HasMember("closingprice"))
								{
									const Value& valclosingprice = ClosingObjects["closingprice"];								
									if (valclosingprice.IsDouble() || valclosingprice.IsNumber())
									{
										dblclosingprice = valclosingprice.GetDouble();
									}

								}
								if (ClosingObjects.HasMember("high"))
								{
									const Value& valhigh = ClosingObjects["high"];
									if (valhigh.IsDouble() || valhigh.IsNumber())
									{
										dblhigh= valhigh.GetDouble();
									}

								}
								if (ClosingObjects.HasMember("low"))
								{
									const Value& vallow = ClosingObjects["low"];
									if (vallow.IsDouble() || vallow.IsNumber())
									{
										dbllow = vallow.GetDouble();
									}

								}


								CString tmp_Insert_Command = L"";
								tmp_Insert_Command.Format(L" EXEC [Update_closing] '%s','%f','%f','%f';", strSymbol, dblclosingprice, dblhigh, dbllow);
								Insert_Command = Insert_Command + tmp_Insert_Command;
							}
						}
						if (Insert_Command.Trim() != L"")
						{
							CStaticClass::m_sqldata.executeCommand(Insert_Command);
						}
						CString strReturnJson = L"{\"type\":\"CLOSING_UPDATE_SUCCESS\",\"message\" : \"ClosingPrice Updated\"}";;
						string strforsend = CT2A(strReturnJson.GetString());
						SendDataToClient(client, strforsend, strKey, m_ActiveClient);
					}
				}
			}
			else if (strtype == "FETCH_HIGH_LOW_MISMATCH_DATA" && checkLoginValidate == 1)
			{
				if (d.HasMember("date"))
				{
					const Value& valcldate = d["date"];
					CString dealDate = L"";
					if (valcldate.IsString())
					{
						dealDate = valcldate.GetString();
					}					
					string strforsend = CStaticClass::m_mtmanager.gethighLowMismatchDeals(dealDate);
					SendDataToClient(client, strforsend, strKey, m_ActiveClient);
				}
				
			}			
			else if (strtype == "GET_DASHBOARD" && checkLoginValidate == 1)
			{
				CString returnJson = L"";
				returnJson = CStaticClass::m_mtmanager.GetDashboardData(login);
				string strforsend = CT2A(returnJson.GetString());
				 //strforsend = "{\"type\":\"DASHBOARD\",\"gridcolumndata\":[{    \"field\": \"Grid1\",    \"type\": \"grid\",    \"gridProperties\": {\"hide\": false,\"gridLayoutLocation\": {\"i\": \"LoginDashboard\",\"x\": 0,\"y\": 0,\"w\": 4,\"h\": 2}    },    \"data\": [{\"field\": \"login\",\"width\": 100,\"order\": 1,\"pinned\": \"left\",\"rowGroup\": true,\"hide\": true,\"collapsed\": false},{\"field\": \"clientfloatingpl\",\"width\": 200,\"hide\": false,\"order\": 2,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"clientgrossamount\",\"width\": 200,\"hide\": false,\"order\": 3,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"companyplnet\",\"width\": 144,\"hide\": false,\"order\": 4,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"volume\",\"width\": 158,\"hide\": false,\"order\": 5,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"companyvolume\",\"width\": 154,\"hide\": false,\"order\": 6,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"clientexposure\",\"width\": 200,\"hide\": false,\"order\": 7,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"Companyexposure\",\"width\": 200,\"hide\": false,\"order\": 8,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false}    ]  },  {    \"field\": \"Grid2\",    \"type\": \"grid\",    \"gridProperties\": {\"hide\": false,\"gridLayoutLocation\": {\"i\": \"Grid2\",\"x\": 6,\"y\": 2,\"w\": 6,\"h\": 1}    },    \"data\": [{\"field\": \"sector\",\"width\": 200,\"order\": 1,\"pinned\": \"left\",\"rowGroup\": true,\"hide\": true,\"collapsed\": false},{\"field\": \"clientfloatingpl\",\"width\": 200,\"hide\": false,\"order\": 2,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"clientgrossamount\",\"width\": 200,\"hide\": false,\"order\": 3,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"companyplnet\",\"width\": 144,\"hide\": false,\"order\": 4,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"volume\",\"width\": 158,\"hide\": false,\"order\": 5,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"companyvolume\",\"width\": 154,\"hide\": false,\"order\": 6,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"clientexposure\",\"width\": 200,\"hide\": false,\"order\": 7,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"Companyexposure\",\"width\": 200,\"hide\": false,\"order\": 8,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false}    ]  },  {    \"field\": \"Grid3\",    \"type\": \"grid\",    \"gridProperties\": {\"hide\": false,\"gridLayoutLocation\": {\"i\": \"Grid3\",\"x\": 4,\"y\": 0,\"w\": 4,\"h\": 2}    },    \"data\": [{\"field\": \"subbroker\",\"width\": 115,\"order\": 1,\"pinned\": \"left\",\"rowGroup\": true,\"hide\": true,\"collapsed\": false},{\"field\": \"clientfloatingpl\",\"width\": 200,\"hide\": false,\"order\": 2,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"clientgrossamount\",\"width\": 200,\"hide\": false,\"order\": 3,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"companyplnet\",\"width\": 144,\"hide\": false,\"order\": 4,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"volume\",\"width\": 158,\"hide\": false,\"order\": 5,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"companyvolume\",\"width\": 154,\"hide\": false,\"order\": 6,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"clientexposure\",\"width\": 200,\"hide\": false,\"order\": 7,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"Companyexposure\",\"width\": 200,\"hide\": false,\"order\": 8,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false}    ]  },  {    \"field\": \"Grid4\",    \"type\": \"grid\",    \"gridProperties\": {\"hide\": false,\"gridLayoutLocation\": {\"i\": \"Grid4\",\"x\": 8,\"y\": 0,\"w\": 4,\"h\": 2}    },    \"data\": [{\"field\": \"symbol\",\"width\": 100,\"order\": 1,\"pinned\": \"left\",\"rowGroup\": true,\"hide\": true,\"collapsed\": false},{\"field\": \"clientfloatingpl\",\"width\": 200,\"hide\": false,\"order\": 2,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"clientgrossamount\",\"width\": 200,\"hide\": false,\"order\": 3,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"companyplnet\",\"width\": 144,\"hide\": false,\"order\": 4,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"volume\",\"width\": 158,\"hide\": false,\"order\": 5,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"companyvolume\",\"width\": 154,\"hide\": false,\"order\": 6,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"clientexposure\",\"width\": 200,\"hide\": false,\"order\": 7,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"Companyexposure\",\"width\": 200,\"hide\": false,\"order\": 8,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false}]},{\"field\": \"Grid5\",\"type\": \"grid\",\"gridProperties\": {\"hide\": false,\"gridLayoutLocation\": {\"i\": \"Grid5\",\"x\": 0,\"y\": 2,\"w\": 6,\"h\": 1}},\"data\": [{\"field\": \"broker\",\"width\": 100,\"order\": 1,\"pinned\": \"left\",\"rowGroup\": true,\"hide\": true,\"collapsed\": false},{\"field\": \"clientfloatingpl\",\"width\": 200,\"hide\": false,\"order\": 2,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"clientgrossamount\",\"width\": 200,\"hide\": false,\"order\": 3,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"companyplnet\",\"width\": 144,\"hide\": false,\"order\": 4,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"volume\",\"width\": 158,\"hide\": false,\"order\": 5,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"companyvolume\",\"width\": 154,\"hide\": false,\"order\": 6,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"clientexposure\",\"width\": 200,\"hide\": false,\"order\": 7,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"Companyexposure\",\"width\": 200,\"hide\": false,\"order\": 8,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false}]},{\"field\": \"Grid6\",    \"type\": \"grid\",    \"gridProperties\": {\"hide\": false,\"gridLayoutLocation\": {\"i\": \"Grid6\",\"x\": 0,\"y\": 3,\"w\": 6,\"h\": 1}    },    \"data\": [{\"field\": \"company\",\"minWidth\": 100,\"width\": 100,\"order\": 1,\"pinned\": \"left\",\"rowGroup\": true,\"hide\": true,\"collapsed\": false},{\"field\": \"clientfloatingpl\",\"width\": 200,\"hide\": false,\"order\": 2,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"clientgrossamount\",\"width\": 200,\"hide\": false,\"order\": 3,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"companyplnet\",\"width\": 144,\"hide\": false,\"order\": 4,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"volume\",\"width\": 158,\"hide\": false,\"order\": 5,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"companyvolume\",\"width\": 154,\"hide\": false,\"order\": 6,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"clientexposure\",\"width\": 200,\"hide\": false,\"order\": 7,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"Companyexposure\",\"width\": 200,\"hide\": false,\"order\": 8,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false}    ]  },  {    \"field\": \"Grid7\",    \"type\": \"grid\",    \"gridProperties\": {\"hide\": false,\"gridLayoutLocation\": {\"i\": \"Grid7\",\"x\": 6,\"y\": 3,\"w\": 6,\"h\": 1}    },    \"data\": [{\"field\": \"exchange\",\"minWidth\": 100,\"width\": 100,\"order\": 1,\"pinned\": \"left\",\"rowGroup\": true,\"hide\": true,\"collapsed\": false},{\"field\": \"clientfloatingpl\",\"width\": 200,\"hide\": false,\"order\": 2,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"clientgrossamount\",\"width\": 200,\"hide\": false,\"order\": 3,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"companyplnet\",\"width\": 144,\"hide\": false,\"order\": 4,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"volume\",\"width\": 158,\"hide\": false,\"order\": 5,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"companyvolume\",\"width\": 154,\"hide\": false,\"order\": 6,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"clientexposure\",\"width\": 200,\"hide\": false,\"order\": 7,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"Companyexposure\",\"width\": 200,\"hide\": false,\"order\": 8,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false}    ]  },  {    \"field\": \"Grid8\",    \"type\": \"grid\",    \"gridProperties\": {\"hide\": false,\"gridLayoutLocation\": {\"i\": \"Grid8\",\"x\": 0,\"y\": 4,\"w\": 6,\"h\": 1}    },   \"data\": [{\"field\": \"international\",\"minWidth\": 100,\"width\": 100,\"order\": 1,\"pinned\": \"left\",\"rowGroup\": true,\"hide\": true,\"collapsed\": false},{\"field\": \"clientfloatingpl\",\"width\": 200,\"hide\": false,\"order\": 2,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"clientgrossamount\",\"width\": 200,\"hide\": false,\"order\": 3,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"companyplnet\",\"width\": 144,\"hide\": false,\"order\": 4,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"volume\",\"width\": 158,\"hide\": false,\"order\": 5,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"companyvolume\",\"width\": 154,\"hide\": false,\"order\": 6,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"clientexposure\",\"width\": 200,\"hide\": false,\"order\": 7,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"Companyexposure\",\"width\": 200,\"hide\": false,\"order\": 8,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false}]},{\"field\": \"Grid9\",\"type\": \"grid\",\"gridProperties\": {\"hide\": false,      \"gridLayoutLocation\": {\"i\": \"Grid9\",\"x\": 6,\"y\": 4,\"w\": 6,\"h\": 1      }    },    \"data\": [      {\"field\": \"categary\",\"minWidth\": 100,\"width\": 100,\"order\": 1,\"pinned\": \"left\",\"rowGroup\": true,\"hide\": true,\"collapsed\": false      },     {\"field\": \"clientfloatingpl\",\"width\": 200,\"hide\": false,\"order\": 2,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false     },     {\"field\": \"clientgrossamount\",\"width\": 200,        \"hide\": false,       \"order\": 3,       \"pinned\": \"left\",       \"rowGroup\": true,       \"collapsed\": false     },     {       \"field\": \"companyplnet\",       \"width\": 144,       \"hide\": false,       \"order\": 4,       \"pinned\": \"left\",       \"rowGroup\": true,       \"collapsed\": false     },     {       \"field\": \"volume\",       \"width\": 158,       \"hide\": false,       \"order\": 5,       \"pinned\": \"left\",       \"rowGroup\": true,       \"collapsed\": false     },     {       \"field\": \"companyvolume\",       \"width\": 154,       \"hide\": false,\"order\": 6,        \"pinned\": \"left\",       \"rowGroup\": true,       \"collapsed\": false     },     {       \"field\": \"clientexposure\",       \"width\": 200,       \"hide\": false,       \"order\": 7,       \"pinned\": \"left\",       \"rowGroup\": true,       \"collapsed\": false     },     {      \"field\": \"Companyexposure\",       \"width\": 200,       \"hide\": false,       \"order\": 8,       \"pinned\": \"left\",       \"rowGroup\": true,       \"collapsed\": false     }   ] }]}";
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
			}
			else if (strtype == "GET_CANVAS" && checkLoginValidate == 1)
			{
				CString returnJson = L"";
				CString m_tabtype = L"";
				if (d.HasMember("tabtype"))
				{
					Value& valtabtype = d["tabtype"];

					if (valtabtype.IsString())
					{
						m_tabtype = valtabtype.GetString();
					}
				}
				returnJson = CStaticClass::m_mtmanager.GetCanvasData(login, m_tabtype);
				string strforsend = CT2A(returnJson.GetString());
				//strforsend = "{\"type\":\"DASHBOARD\",\"gridcolumndata\":[{    \"field\": \"Grid1\",    \"type\": \"grid\",    \"gridProperties\": {\"hide\": false,\"gridLayoutLocation\": {\"i\": \"LoginDashboard\",\"x\": 0,\"y\": 0,\"w\": 4,\"h\": 2}    },    \"data\": [{\"field\": \"login\",\"width\": 100,\"order\": 1,\"pinned\": \"left\",\"rowGroup\": true,\"hide\": true,\"collapsed\": false},{\"field\": \"clientfloatingpl\",\"width\": 200,\"hide\": false,\"order\": 2,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"clientgrossamount\",\"width\": 200,\"hide\": false,\"order\": 3,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"companyplnet\",\"width\": 144,\"hide\": false,\"order\": 4,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"volume\",\"width\": 158,\"hide\": false,\"order\": 5,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"companyvolume\",\"width\": 154,\"hide\": false,\"order\": 6,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"clientexposure\",\"width\": 200,\"hide\": false,\"order\": 7,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"Companyexposure\",\"width\": 200,\"hide\": false,\"order\": 8,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false}    ]  },  {    \"field\": \"Grid2\",    \"type\": \"grid\",    \"gridProperties\": {\"hide\": false,\"gridLayoutLocation\": {\"i\": \"Grid2\",\"x\": 6,\"y\": 2,\"w\": 6,\"h\": 1}    },    \"data\": [{\"field\": \"sector\",\"width\": 200,\"order\": 1,\"pinned\": \"left\",\"rowGroup\": true,\"hide\": true,\"collapsed\": false},{\"field\": \"clientfloatingpl\",\"width\": 200,\"hide\": false,\"order\": 2,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"clientgrossamount\",\"width\": 200,\"hide\": false,\"order\": 3,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"companyplnet\",\"width\": 144,\"hide\": false,\"order\": 4,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"volume\",\"width\": 158,\"hide\": false,\"order\": 5,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"companyvolume\",\"width\": 154,\"hide\": false,\"order\": 6,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"clientexposure\",\"width\": 200,\"hide\": false,\"order\": 7,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"Companyexposure\",\"width\": 200,\"hide\": false,\"order\": 8,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false}    ]  },  {    \"field\": \"Grid3\",    \"type\": \"grid\",    \"gridProperties\": {\"hide\": false,\"gridLayoutLocation\": {\"i\": \"Grid3\",\"x\": 4,\"y\": 0,\"w\": 4,\"h\": 2}    },    \"data\": [{\"field\": \"subbroker\",\"width\": 115,\"order\": 1,\"pinned\": \"left\",\"rowGroup\": true,\"hide\": true,\"collapsed\": false},{\"field\": \"clientfloatingpl\",\"width\": 200,\"hide\": false,\"order\": 2,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"clientgrossamount\",\"width\": 200,\"hide\": false,\"order\": 3,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"companyplnet\",\"width\": 144,\"hide\": false,\"order\": 4,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"volume\",\"width\": 158,\"hide\": false,\"order\": 5,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"companyvolume\",\"width\": 154,\"hide\": false,\"order\": 6,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"clientexposure\",\"width\": 200,\"hide\": false,\"order\": 7,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"Companyexposure\",\"width\": 200,\"hide\": false,\"order\": 8,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false}    ]  },  {    \"field\": \"Grid4\",    \"type\": \"grid\",    \"gridProperties\": {\"hide\": false,\"gridLayoutLocation\": {\"i\": \"Grid4\",\"x\": 8,\"y\": 0,\"w\": 4,\"h\": 2}    },    \"data\": [{\"field\": \"symbol\",\"width\": 100,\"order\": 1,\"pinned\": \"left\",\"rowGroup\": true,\"hide\": true,\"collapsed\": false},{\"field\": \"clientfloatingpl\",\"width\": 200,\"hide\": false,\"order\": 2,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"clientgrossamount\",\"width\": 200,\"hide\": false,\"order\": 3,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"companyplnet\",\"width\": 144,\"hide\": false,\"order\": 4,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"volume\",\"width\": 158,\"hide\": false,\"order\": 5,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"companyvolume\",\"width\": 154,\"hide\": false,\"order\": 6,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"clientexposure\",\"width\": 200,\"hide\": false,\"order\": 7,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"Companyexposure\",\"width\": 200,\"hide\": false,\"order\": 8,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false}]},{\"field\": \"Grid5\",\"type\": \"grid\",\"gridProperties\": {\"hide\": false,\"gridLayoutLocation\": {\"i\": \"Grid5\",\"x\": 0,\"y\": 2,\"w\": 6,\"h\": 1}},\"data\": [{\"field\": \"broker\",\"width\": 100,\"order\": 1,\"pinned\": \"left\",\"rowGroup\": true,\"hide\": true,\"collapsed\": false},{\"field\": \"clientfloatingpl\",\"width\": 200,\"hide\": false,\"order\": 2,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"clientgrossamount\",\"width\": 200,\"hide\": false,\"order\": 3,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"companyplnet\",\"width\": 144,\"hide\": false,\"order\": 4,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"volume\",\"width\": 158,\"hide\": false,\"order\": 5,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"companyvolume\",\"width\": 154,\"hide\": false,\"order\": 6,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"clientexposure\",\"width\": 200,\"hide\": false,\"order\": 7,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"Companyexposure\",\"width\": 200,\"hide\": false,\"order\": 8,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false}]},{\"field\": \"Grid6\",    \"type\": \"grid\",    \"gridProperties\": {\"hide\": false,\"gridLayoutLocation\": {\"i\": \"Grid6\",\"x\": 0,\"y\": 3,\"w\": 6,\"h\": 1}    },    \"data\": [{\"field\": \"company\",\"minWidth\": 100,\"width\": 100,\"order\": 1,\"pinned\": \"left\",\"rowGroup\": true,\"hide\": true,\"collapsed\": false},{\"field\": \"clientfloatingpl\",\"width\": 200,\"hide\": false,\"order\": 2,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"clientgrossamount\",\"width\": 200,\"hide\": false,\"order\": 3,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"companyplnet\",\"width\": 144,\"hide\": false,\"order\": 4,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"volume\",\"width\": 158,\"hide\": false,\"order\": 5,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"companyvolume\",\"width\": 154,\"hide\": false,\"order\": 6,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"clientexposure\",\"width\": 200,\"hide\": false,\"order\": 7,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"Companyexposure\",\"width\": 200,\"hide\": false,\"order\": 8,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false}    ]  },  {    \"field\": \"Grid7\",    \"type\": \"grid\",    \"gridProperties\": {\"hide\": false,\"gridLayoutLocation\": {\"i\": \"Grid7\",\"x\": 6,\"y\": 3,\"w\": 6,\"h\": 1}    },    \"data\": [{\"field\": \"exchange\",\"minWidth\": 100,\"width\": 100,\"order\": 1,\"pinned\": \"left\",\"rowGroup\": true,\"hide\": true,\"collapsed\": false},{\"field\": \"clientfloatingpl\",\"width\": 200,\"hide\": false,\"order\": 2,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"clientgrossamount\",\"width\": 200,\"hide\": false,\"order\": 3,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"companyplnet\",\"width\": 144,\"hide\": false,\"order\": 4,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"volume\",\"width\": 158,\"hide\": false,\"order\": 5,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"companyvolume\",\"width\": 154,\"hide\": false,\"order\": 6,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"clientexposure\",\"width\": 200,\"hide\": false,\"order\": 7,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"Companyexposure\",\"width\": 200,\"hide\": false,\"order\": 8,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false}    ]  },  {    \"field\": \"Grid8\",    \"type\": \"grid\",    \"gridProperties\": {\"hide\": false,\"gridLayoutLocation\": {\"i\": \"Grid8\",\"x\": 0,\"y\": 4,\"w\": 6,\"h\": 1}    },   \"data\": [{\"field\": \"international\",\"minWidth\": 100,\"width\": 100,\"order\": 1,\"pinned\": \"left\",\"rowGroup\": true,\"hide\": true,\"collapsed\": false},{\"field\": \"clientfloatingpl\",\"width\": 200,\"hide\": false,\"order\": 2,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"clientgrossamount\",\"width\": 200,\"hide\": false,\"order\": 3,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"companyplnet\",\"width\": 144,\"hide\": false,\"order\": 4,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"volume\",\"width\": 158,\"hide\": false,\"order\": 5,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"companyvolume\",\"width\": 154,\"hide\": false,\"order\": 6,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"clientexposure\",\"width\": 200,\"hide\": false,\"order\": 7,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false},{\"field\": \"Companyexposure\",\"width\": 200,\"hide\": false,\"order\": 8,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false}]},{\"field\": \"Grid9\",\"type\": \"grid\",\"gridProperties\": {\"hide\": false,      \"gridLayoutLocation\": {\"i\": \"Grid9\",\"x\": 6,\"y\": 4,\"w\": 6,\"h\": 1      }    },    \"data\": [      {\"field\": \"categary\",\"minWidth\": 100,\"width\": 100,\"order\": 1,\"pinned\": \"left\",\"rowGroup\": true,\"hide\": true,\"collapsed\": false      },     {\"field\": \"clientfloatingpl\",\"width\": 200,\"hide\": false,\"order\": 2,\"pinned\": \"left\",\"rowGroup\": true,\"collapsed\": false     },     {\"field\": \"clientgrossamount\",\"width\": 200,        \"hide\": false,       \"order\": 3,       \"pinned\": \"left\",       \"rowGroup\": true,       \"collapsed\": false     },     {       \"field\": \"companyplnet\",       \"width\": 144,       \"hide\": false,       \"order\": 4,       \"pinned\": \"left\",       \"rowGroup\": true,       \"collapsed\": false     },     {       \"field\": \"volume\",       \"width\": 158,       \"hide\": false,       \"order\": 5,       \"pinned\": \"left\",       \"rowGroup\": true,       \"collapsed\": false     },     {       \"field\": \"companyvolume\",       \"width\": 154,       \"hide\": false,\"order\": 6,        \"pinned\": \"left\",       \"rowGroup\": true,       \"collapsed\": false     },     {       \"field\": \"clientexposure\",       \"width\": 200,       \"hide\": false,       \"order\": 7,       \"pinned\": \"left\",       \"rowGroup\": true,       \"collapsed\": false     },     {      \"field\": \"Companyexposure\",       \"width\": 200,       \"hide\": false,       \"order\": 8,       \"pinned\": \"left\",       \"rowGroup\": true,       \"collapsed\": false     }   ] }]}";
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
			}
			else if (strtype == "UPDATE_GATEWAY_POSITIONDEVIDERATIO" && checkLoginValidate == 1)
			{
				if (d.HasMember("data"))
				{
					Value& valdata = d["data"];
					if(valdata.IsArray())
					{
						for (SizeType di = 0; di < valdata.Size(); di++)
						{
							Value& valdatavalue = valdata[di];
							if (valdatavalue.HasMember("symbolgroup") && valdatavalue.HasMember("ratiodefinition") && valdatavalue.HasMember("LpEntries"))
							{
								Value& valSymbolGroup = valdatavalue["symbolgroup"];
								CString strSymbolGroup = L"";
								strSymbolGroup = valSymbolGroup.GetString();
								CString strSqlCommand = L"";
								strSqlCommand.Format(L"delete from Orika_GATEWAY_POSITIONDEVIDERATIO where [symbolgroup]='%s';delete from Orika_LpPriotity where symbolgroup='%s';", strSymbolGroup, strSymbolGroup);
								//strSqlCommand.Format(L"delete from Orika_LpPriotity where symbolgroup='%s';", strSymbolGroup, strSymbolGroup);
								CStaticClass::m_sqldata.executeCommand(strSqlCommand);

								Value& valIpEntries = valdatavalue["LpEntries"];
								if (valIpEntries.IsArray())
								{
									for (SizeType LPi = 0; LPi < valIpEntries.Size(); LPi++)
									{
										Value& LPData = valIpEntries[LPi];
										if (LPData.IsObject())
										{
											
											CString  m_lpname=L"";
											CString  m_lpsymbol=L"";
											int  m_lppriority=0;
											double  m_lpmaxlots=0; 
											CString  m_lpsendingtype=L"";
											double  m_lpratio=0;
											if (LPData.HasMember("LpName"))
											{
												Value& valIpName = LPData["LpName"];
												if (valIpName.IsString())
												{
													m_lpname = valIpName.GetString();
												}
											}
											if (LPData.HasMember("LpSymbol"))
											{
												Value& valIpSymbol = LPData["LpSymbol"];
												if (valIpSymbol.IsString())
												{
													m_lpsymbol = valIpSymbol.GetString();
												}
												
											}
											if (LPData.HasMember("LpPriority"))
											{
												Value& valIpPriority = LPData["LpPriority"];
												if (valIpPriority.IsNumber())
												{
													m_lppriority = valIpPriority.GetInt();
												}
											}
											if (LPData.HasMember("LpMaxLots"))
											{
												Value& valIpMaxLots = LPData["LpMaxLots"];
												if (valIpMaxLots.IsDouble())
												{
													m_lpmaxlots = valIpMaxLots.GetDouble();
												}
											}
											if (LPData.HasMember("LpSendingType"))
											{
												Value& valIpSendingType = LPData["LpSendingType"];
												if (valIpSendingType.IsString())
												{
													m_lpsendingtype = valIpSendingType.GetString();
												}

											}
											if (LPData.HasMember("LpRatio"))
											{
												Value& valIpRatio = LPData["LpRatio"];
												if (valIpRatio.IsDouble())
												{
													m_lpratio = valIpRatio.GetDouble();
												}
											}
											strSqlCommand.Format(L"  insert into Orika_LpPriotity(symbolgroup,lpname,lpsymbol,lppriority,lpmaxlots,lpsendingtype,lpratio) values('%s','%s','%s','%d','%.2lf','%s','%.2lf');", strSymbolGroup, m_lpname, m_lpsymbol, m_lppriority, m_lpmaxlots, m_lpsendingtype, m_lpratio);
											CStaticClass::m_sqldata.executeCommand(strSqlCommand);
										}
									}
								}





								Value& valratiodefinition = valdatavalue["ratiodefinition"];
								if (valratiodefinition.IsArray())
								{
									for (SizeType i = 0; i < valratiodefinition.Size(); i++)
									{
										Value& valratiodefinitiondata = valratiodefinition[i];
										if (valratiodefinitiondata.IsObject())
										{
											double dblLot = 0;
											if (valratiodefinitiondata.HasMember("lots"))
											{

												Value& valLot = valratiodefinitiondata["lots"];
												if (valLot.IsNumber()|| valLot.IsDouble())
												{
													dblLot = valLot.GetDouble();
												}
												if (valLot.IsString() )
												{
													CString strLot = L""; 
													strLot=valLot.GetString();
													dblLot = _wtof(strLot);
												}
											}
											double dblRatio = 0;
											if (valratiodefinitiondata.HasMember("ratio"))
											{

												Value& valratio = valratiodefinitiondata["ratio"];
												if (valratio.IsNumber() || valratio.IsDouble())
												{
													dblRatio = valratio.GetDouble();
												}
												if (valratio.IsString())
												{
													CString strRatio = L""; 
													strRatio=valratio.GetString();
													dblRatio = _wtof(strRatio);
												}
											}
											//stepup
											double dblstepup = 0;
											if (valratiodefinitiondata.HasMember("stepup"))
											{

												Value& valstepup = valratiodefinitiondata["stepup"];
												if (valstepup.IsNumber() || valstepup.IsDouble())
												{
													dblstepup = valstepup.GetDouble();
												}
												if (valstepup.IsString())
												{
													CString strlstepup = L"";
													strlstepup = valstepup.GetString();
													dblstepup = _wtof(strlstepup);
												}
											}
											strSqlCommand.Format(L" insert into Orika_GATEWAY_POSITIONDEVIDERATIO(symbolgroup,lots,[ratio],stepup) values('%s','%.2lf','%.2lf','%.2lf');", strSymbolGroup,  dblLot, dblRatio, dblstepup);
											CStaticClass::m_sqldata.executeCommand(strSqlCommand);
										}
									}
								}
							}
						}
						CStaticClass::m_sqldata.Load_PerfectAndRoundedInDataBase();

						//CStaticClass::m_positiondevideRatio_lock.Lock();							
						POSITION pos2 = CStaticClass::m_LPLots.GetStartPosition();
						while (pos2 != NULL)
						{
							CString strSymbolGroup = L"";
							double lpvolume = 0;							
							CStaticClass::m_LPLots.GetNextAssoc(pos2, strSymbolGroup, lpvolume);
							CStaticClass m_staticClass;

							double m_totalComVolumeAfterMulti = 0;
							int  CompanyVolumeAfterMultiTotalIndex = -1;
							CStaticClass::clientnettotalArrayIndex.Lookup(strSymbolGroup, CompanyVolumeAfterMultiTotalIndex);
							if (CompanyVolumeAfterMultiTotalIndex >= 0)
							{
								m_totalComVolumeAfterMulti = *CStaticClass::CompanyVolumeAfterMultiTotalArray[CompanyVolumeAfterMultiTotalIndex];
							}
							m_staticClass.CalculateLPData(L"", strSymbolGroup, lpvolume, m_totalComVolumeAfterMulti);
						}	
						//CStaticClass::m_positiondevideRatio_lock.Unlock();


						CString returnJson = L"";
						returnJson = CStaticClass::m_mtmanager.GetGATEWAY_POSITIONDEVIDERATIO_UPDATE();
						string strforsend = CT2A(returnJson.GetString());
						SendDataToClient(client, strforsend, strKey, m_ActiveClient);
				}
				}
			}
			else if (strtype == "FETCH_GATEWAY_POSITIONDEVIDERATIO" && checkLoginValidate == 1)
			{
				CString returnJson = L"";
				//returnJson = CStaticClass::m_mtmanager.GetGATEWAY_POSITIONDEVIDERATIO();
				returnJson = CStaticClass::m_mtmanager.GetGATEWAY_POSITIONDEVIDERATIO_UPDATE();
				string strforsend = CT2A(returnJson.GetString());			
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
			}
			else if (strtype == "SAVE_DASHBOARD" && checkLoginValidate == 1)
			{		
				if (d.HasMember("gridcolumndata"))
				{
					CString strSqlCommand = L"";
					strSqlCommand.Format(L"delete from orika_Dashboard where [loginuser]='%s';", login);
					CStaticClass::m_sqldata.executeCommand(strSqlCommand);
					Value& valgridcolumndata = d["gridcolumndata"];
					if (valgridcolumndata.IsArray())
					{
						for (SizeType i = 0; i < valgridcolumndata.Size(); i++)
						{
							Value& gridcolumndatadata = valgridcolumndata[i];
							if (gridcolumndatadata.IsObject())
							{
								CString strField = L"";
								if (gridcolumndatadata.HasMember("field"))
								{
									
									Value& valField = gridcolumndatadata["field"];
									if (valField.IsString())
									{
										strField = valField.GetString();
									}
								}
								CString strType = L"";
								if (gridcolumndatadata.HasMember("type"))
								{
									
									Value& valType = gridcolumndatadata["type"];
									if (valType.IsString())
									{
										strType = valType.GetString();
									}
								}
								CString strgridProperties = L"";
								if (gridcolumndatadata.HasMember("gridProperties"))
								{
									
									Value& valgridProperties = gridcolumndatadata["gridProperties"];

									std::string strjson = JsonAsString(valgridProperties);
									strgridProperties = strjson.c_str();																											
								}
								CString strdata = L"";
								if (gridcolumndatadata.HasMember("data"))
								{
									
									Value& valdata = gridcolumndatadata["data"];
									std::string strjson = JsonAsString(valdata);
									strdata = strjson.c_str();
								}
								
								
								strSqlCommand.Format(L"insert into orika_Dashboard(loginuser,field,[type],gridProperties,data) values('%s','%s','%s','%s','%s');", login, strField, strType, strgridProperties, strdata);
								CStaticClass::m_sqldata.executeCommand(strSqlCommand);
							}
						}
					}
				}
				string strforsend = "{\"type\":\"SERVER_MESSAGE\",\"responseMessage\":\"Dashboard Data Has Been Updated.\"}";
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
			}
			else if (strtype == "FETCH_SYMBOLGROUPINFO" && checkLoginValidate == 1)
			{
				CString strSymbolGroup = L"";
				if (d.HasMember("symbolgroup"))
				{
					Value& valsymbolgroup = d["symbolgroup"];					
					if (valsymbolgroup.IsString())
					{
						strSymbolGroup = valsymbolgroup.GetString();
					}
				}
				CStaticClass::st_Orika_symbolgroup m_st_Orika_symbolgroup = {};
				CStaticClass::m_Orika_symbolgroupHastable.Lookup(strSymbolGroup, m_st_Orika_symbolgroup);
				double m_symbolgroupmargin = m_st_Orika_symbolgroup.m_symbolMargin;
				StringBuffer s;
				Writer<StringBuffer> writer(s);
				writer.StartObject();
				writer.Key("type");
				writer.String("SYMBOLGROUPINFO");								
				writer.Key("symbolgroup");
				string ssSymbolgroup = string(CT2CA(strSymbolGroup));
				const char* stSymbolgroup = ssSymbolgroup.c_str();
				writer.String(stSymbolgroup);

				writer.Key("symbolgroupmargin");
				writer.Double(m_symbolgroupmargin);

				writer.EndObject();
				CString str_FinalJsonUpdate = L"";

				str_FinalJsonUpdate = s.GetString();
				s.Clear();
				writer.Flush();
				string strforsend = CT2A(str_FinalJsonUpdate.GetString());
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
			}
			else if (strtype == "FETCH_ALL_SYMBOLGROUPINFO" && checkLoginValidate == 1)
			{							
				StringBuffer s;
				Writer<StringBuffer> writer(s);
				writer.StartObject();
				writer.Key("type");
				writer.String("SYMBOLGROUPINFO");
				writer.Key("symbolgroups");
				writer.StartArray();
				POSITION pos = CStaticClass::m_Orika_symbolgroupHastable.GetStartPosition();
				while (pos != NULL)
				{
					CString strclientkey = L"";
					CStaticClass::st_Orika_symbolgroup m_st_Orika_symbolgroup = {};
					CStaticClass::m_Orika_symbolgroupHastable.GetNextAssoc(pos, strclientkey, m_st_Orika_symbolgroup);
					writer.StartObject();
						CString strSymbolGroup = m_st_Orika_symbolgroup.m_symbolGroup;
						double m_symbolgroupmargin = m_st_Orika_symbolgroup.m_symbolMargin;
						string ssSymbolgroup = string(CT2CA(strSymbolGroup));
						const char* stSymbolgroup = ssSymbolgroup.c_str();
						writer.Key("symbolgroup");
						writer.String(stSymbolgroup);
						writer.Key("symbolgroupmargin");
						writer.Double(m_symbolgroupmargin);
					writer.EndObject();
				}
				writer.EndArray();
				writer.EndObject();
				CString str_FinalJsonUpdate = L"";

				str_FinalJsonUpdate = s.GetString();
				s.Clear();
				writer.Flush();
				string strforsend = CT2A(str_FinalJsonUpdate.GetString());
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
			}
			else if (strtype == "UPDATE_SYMBOLMARGIN" && checkLoginValidate == 1)
			{		
				if (d.HasMember("symbolgroupmargins"))
				{					
					Value& symbolgroupmargins = d["symbolgroupmargins"];
					if (symbolgroupmargins.IsArray())
					{
						CString strUpdateMarginCommand = L"";
						for (SizeType i = 0; i < symbolgroupmargins.Size(); i++)
						{
							Value& symbolgroupmargin = symbolgroupmargins[i];
							if (symbolgroupmargin.IsObject())
							{
								CString strsymbolgroup = L"";
								double m_symbolgroupmargin = 0;
								if (symbolgroupmargin.HasMember("symbolgroup"))
								{
									Value& valsymbolgroup = symbolgroupmargin["symbolgroup"];
									if (valsymbolgroup.IsString())
									{
										strsymbolgroup = valsymbolgroup.GetString();
									}
								}

								if (symbolgroupmargin.HasMember("symbolgroupmargin"))
								{
									Value& valsymbolgroupmargin = symbolgroupmargin["symbolgroupmargin"];
									if (valsymbolgroupmargin.IsDouble())
									{
										m_symbolgroupmargin = valsymbolgroupmargin.GetDouble();
									}
									if (valsymbolgroupmargin.IsInt())
									{
										m_symbolgroupmargin = valsymbolgroupmargin.GetInt();
									}
								}
								CString strtempCommand = L""; 
								strtempCommand.Format(L"update Orika_symbolgroup set symbolMargin='%.2lf' where symbolGroup='%s';", m_symbolgroupmargin, strsymbolgroup);
								strUpdateMarginCommand = strUpdateMarginCommand + strtempCommand;

								CStaticClass::st_Orika_symbolgroup m_st_Orika_symbolgroup = {};								
								CStaticClass::m_Orika_symbolgroupHastable.Lookup(strsymbolgroup, m_st_Orika_symbolgroup);
								m_st_Orika_symbolgroup.m_symbolMargin = m_symbolgroupmargin;
								CStaticClass::m_Orika_symbolgroupHastable.SetAt(strsymbolgroup, m_st_Orika_symbolgroup);

							}							
						}	
						CStaticClass::m_sqldata.executeCommand(strUpdateMarginCommand);
						string strforsend = "{\"type\":\"SERVER_MESSAGE\",\"responseMessage\":\"Symbol Margin Has Been Updated.\"}";
						SendDataToClient(client, strforsend, strKey, m_ActiveClient);
					}
				}							
			}
			else if (strtype == "FETCH_CLIENTCREDIT" && checkLoginValidate == 1)
			{
				StringBuffer s;
				Writer<StringBuffer> writer(s);
				writer.StartObject();
				writer.Key("type");
				writer.String("CLIENTCREDIT");
				writer.Key("clientcredits");
				writer.StartArray();
				
				int totalData = CStaticClass::m_clientmasterarray.Total();
				for (int i = 0; i < totalData; i++)
				{
					CStaticClass::stclientmaster m_stclientmaster = {};
					m_stclientmaster = CStaticClass::m_clientmasterarray[i];
																			
					writer.StartObject();
					CString m_login = m_stclientmaster.m_login;
					double m_clientcredit = m_stclientmaster.m_creditLimit;
					string sslogin = string(CT2CA(m_login));
					const char* stlogin = sslogin.c_str();
					writer.Key("login");
					writer.String(stlogin);
					writer.Key("credit");
					writer.Double(m_clientcredit);
					writer.EndObject();
				}
				writer.EndArray();
				writer.EndObject();
				CString str_FinalJsonUpdate = L"";

				str_FinalJsonUpdate = s.GetString();
				s.Clear();
				writer.Flush();
				string strforsend = CT2A(str_FinalJsonUpdate.GetString());
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
			}
			else if (strtype == "UPDATE_CLIENTCREDIT" && checkLoginValidate == 1)
			{
			if (d.HasMember("clientcredits"))
			{
				Value& objclientcredits = d["clientcredits"];
				if (objclientcredits.IsArray())
				{
					CString strUpdateClientCreditCommand = L"";
					for (SizeType i = 0; i < objclientcredits.Size(); i++)
					{
						Value& clientcredit = objclientcredits[i];
						if (clientcredit.IsObject())
						{
							CString strclient = L"";
							double m_clientcredit = 0;
							if (clientcredit.HasMember("login"))
							{
								Value& valclient = clientcredit["login"];
								if (valclient.IsString())
								{
									strclient = valclient.GetString();
								}
							}

							if (clientcredit.HasMember("credit"))
							{
								Value& valcredit= clientcredit["credit"];
								if (clientcredit.IsDouble())
								{
									m_clientcredit = clientcredit.GetDouble();
								}
								if (clientcredit.IsInt())
								{
									m_clientcredit = clientcredit.GetInt();
								}
							}
							CString strtempCommand = L"";
							strtempCommand.Format(L"update Orika_clientmaster  set creditLimit='%.2lf' where [login]='%s';", m_clientcredit, strclient);
							strUpdateClientCreditCommand = strUpdateClientCreditCommand + strtempCommand;							
							//Update Clientmaster hash Table

							int totalData = CStaticClass::m_clientmasterarray.Total();
							for (int i = 0; i < totalData; i++)
							{
								CStaticClass::stclientmaster m_stclientmaster = {};
								m_stclientmaster=CStaticClass::m_clientmasterarray[i];
								CString m_login = m_stclientmaster.m_login;
								if (m_login == strclient)
								{
									m_stclientmaster.m_creditLimit = m_clientcredit;
									CStaticClass::m_clientmasterarray.Update(i, &m_stclientmaster);
									break;
								}
							}														
							//End  Update Clientmaster Hash Table
						}
					}
					if (strUpdateClientCreditCommand != L"")
					{
						CStaticClass::m_sqldata.executeCommand(strUpdateClientCreditCommand);
					}
					string strforsend = "{\"type\":\"SERVER_MESSAGE\",\"responseMessage\":\"Symbol Margin Has Been Updated.\"}";
					SendDataToClient(client, strforsend, strKey, m_ActiveClient);
				}
			}
			}
			//FETCH_REJECTLIMIT_SETTING
			else if (strtype == "FETCH_REJECTLIMIT_SETTING" && checkLoginValidate == 1)
			{
					CString m_requestedlogin = L"";
					if (d.HasMember("login"))
					{
						const Value& Keytime = d["login"];
						if (Keytime.IsString())
						{
							m_requestedlogin = Keytime.GetString();
						}
					}				
					StringBuffer s;
					Writer<StringBuffer> writer(s);
					writer.StartObject();
					writer.Key("type");
					writer.String("REJECTLIMIT_SETTING");
					writer.Key("limits");
					writer.StartArray();
	
					POSITION pos = CStaticClass::m_Orika_ClientgatewayconfigHastable.GetStartPosition();
					while (pos != NULL)
					{
						CString strclientkey = L"";														
						CStaticClass::st_Orika_Clientgatewayconfig m_st_Orika_Clientgatewayconfig = {};
						CStaticClass::m_Orika_ClientgatewayconfigHastable.GetNextAssoc(pos, strclientkey, m_st_Orika_Clientgatewayconfig); 
						
						CString m_login = L""; 
						m_login = m_st_Orika_Clientgatewayconfig.m_login;
						if ((m_login==m_requestedlogin) || (m_requestedlogin == L""))
						{
							writer.StartObject();
							string sslogin = string(CT2CA(m_login));
							const char* stlogin = sslogin.c_str();
							writer.Key("login");
							writer.String(stlogin);
							writer.Key("symbolgroup");
							CString m_symbolgroup = m_st_Orika_Clientgatewayconfig.m_symbolGroup;
							string sssymbolgroup = string(CT2CA(m_symbolgroup));
							const char* stsymbolgroup = sssymbolgroup.c_str();
							writer.String(stsymbolgroup);
							writer.Key("symbolwisebuylimit");
							writer.Double(m_st_Orika_Clientgatewayconfig.m_symbolWiseBuyLimit);
							writer.Key("symbolwiseselllimit");
							writer.Double(m_st_Orika_Clientgatewayconfig.m_symbolWiseSellLimit);
							writer.Key("symbolwisependingorderenabledisable");
							writer.Double(m_st_Orika_Clientgatewayconfig.m_symbolWisePendingOrderEnableDisable);
							writer.Key("symbolPositionLimit");
							writer.Double(m_st_Orika_Clientgatewayconfig.m_symbolPositionLimit);
							writer.Key("qtyLimitMultiplayer");
							writer.Double(m_st_Orika_Clientgatewayconfig.m_qtyLimitMultiplayer);
							writer.EndObject();
						}
					}					
					writer.EndArray();
					writer.EndObject();
					CString str_FinalJsonUpdate = L"";
					str_FinalJsonUpdate = s.GetString();
					s.Clear();
					writer.Flush();
					string strforsend = CT2A(str_FinalJsonUpdate.GetString());
					SendDataToClient(client, strforsend, strKey, m_ActiveClient);
				}
			else if (strtype == "UPDATE_REJECTLIMIT_SETTING" && checkLoginValidate == 1)
			{
			if (d.HasMember("clientlimits"))
			{
				Value& objclientlimits = d["clientlimits"];
				if (objclientlimits.IsArray())
				{
					CString strUpdateobjclientlimitsCommand = L"";
					for (SizeType i = 0; i < objclientlimits.Size(); i++)
					{
						Value& clientlimit = objclientlimits[i];
						if (clientlimit.IsObject())
						{
							CString strclient = L"";
							CString strsymbolgroup = L"";
							double m_symbolwiseselllimit = 0;
							double m_symbolwisebuylimit = 0;
							double m_symbolwisependingorderenabledisable = 0;
							double m_symbolPositionLimit = 0;
							double m_qtyLimitMultiplayer = 0;
							if (clientlimit.HasMember("login"))
							{
								Value& valclient = clientlimit["login"];
								if (valclient.IsString())
								{
									strclient = valclient.GetString();
								}
							}
							if (clientlimit.HasMember("symbolgroup"))
							{
								Value& valsymbolgroup = clientlimit["symbolgroup"];
								if (valsymbolgroup.IsString())
								{
									strsymbolgroup = valsymbolgroup.GetString();
								}
							}
							if (clientlimit.HasMember("symbolwisebuylimit"))
							{
								Value& val = clientlimit["symbolwisebuylimit"];
								if (val.IsDouble())
								{
									m_symbolwisebuylimit = val.GetDouble();
								}
								if (val.IsInt())
								{
									m_symbolwisebuylimit = val.GetInt();
								}
							}
							//symbolwiseselllimit
							if (clientlimit.HasMember("symbolwiseselllimit"))
							{
								Value& val= clientlimit["symbolwiseselllimit"];
								if (val.IsDouble())
								{
									m_symbolwiseselllimit = val.GetDouble();
								}
								if (val.IsInt())
								{
									m_symbolwiseselllimit = val.GetInt();
								}
							}
							//symbolwisependingorderenabledisable
							if (clientlimit.HasMember("symbolwisependingorderenabledisable"))
							{
								Value& val = clientlimit["symbolwisependingorderenabledisable"];
								if (val.IsDouble())
								{
									m_symbolwisependingorderenabledisable = val.GetDouble();
								}
								if (val.IsInt())
								{
									m_symbolwisependingorderenabledisable = val.GetInt();
								}
							}
							//symbolPositionLimit
							if (clientlimit.HasMember("symbolPositionLimit"))
							{
								Value& val = clientlimit["symbolPositionLimit"];
								if (val.IsDouble())
								{
									m_symbolPositionLimit = val.GetDouble();
								}
								if (val.IsInt())
								{
									m_symbolPositionLimit = val.GetInt();
								}
							}
							//m_qtyLimitMultiplayer
							if (clientlimit.HasMember("qtyLimitMultiplayer"))
							{
								Value& val = clientlimit["qtyLimitMultiplayer"];
								if (val.IsDouble())
								{
									m_qtyLimitMultiplayer = val.GetDouble();
								}
								if (val.IsInt())
								{
									m_qtyLimitMultiplayer = val.GetInt();
								}
							}
							CString strtempCommand = L"";
							//strtempCommand.Format(L"update Orika_clientmaster  set creditLimit='%.2lf' where [login]='%s';", m_clientcredit, strclient);
							strUpdateobjclientlimitsCommand = strUpdateobjclientlimitsCommand + strtempCommand;
							//Update hash Table

							CStaticClass::st_Orika_Clientgatewayconfig m_st_Orika_Clientgatewayconfig = {};
							CString strKey = L"";							
							strKey.Format(L"%s:%s", strclient, strsymbolgroup);

							CMTStr::Copy(m_st_Orika_Clientgatewayconfig.m_login, strclient);
							CMTStr::Copy(m_st_Orika_Clientgatewayconfig.m_symbolGroup, strsymbolgroup);
							m_st_Orika_Clientgatewayconfig.m_symbolWiseBuyLimit = m_symbolwiseselllimit;
							m_st_Orika_Clientgatewayconfig.m_symbolWiseSellLimit = m_symbolwisebuylimit;
							m_st_Orika_Clientgatewayconfig.m_symbolWisePendingOrderEnableDisable = m_symbolwisependingorderenabledisable;
							m_st_Orika_Clientgatewayconfig.m_symbolPositionLimit = m_symbolPositionLimit;
							m_st_Orika_Clientgatewayconfig.m_qtyLimitMultiplayer = m_qtyLimitMultiplayer;
							CStaticClass::m_Orika_ClientgatewayconfigHastable.SetAt(strKey, m_st_Orika_Clientgatewayconfig);
							//End  Update Hash Table
						}
					}
					if (strUpdateobjclientlimitsCommand != L"")
					{
						//CStaticClass::m_sqldata.executeCommand(strUpdateClientCreditCommand);
					}
					string strforsend = "{\"type\":\"SERVER_MESSAGE\",\"responseMessage\":\"Symbol Margin Has Been Updated.\"}";
					SendDataToClient(client, strforsend, strKey, m_ActiveClient);
				}
			}
			}
				else if (strtype == "SET_SELECTED_THEME" && checkLoginValidate == 1)
				{
					if (d.HasMember("theme"))
					{
						CString m_theme = L"";
						Value& vartheme = d["theme"];
						if (vartheme.IsString())
						{
							m_theme = vartheme.GetString();
						}
						int hr = 0;
						CString StrSaveCommand = L"";
						StrSaveCommand.Format(L"delete from orika_theme where loginuser='%s'; insert into orika_theme(loginuser,theme) values('%s','%s');", login, login, m_theme);
						hr = CStaticClass::m_sqldata.executeCommandwiterrorcode(StrSaveCommand);
						string strforsend = "{\"type\":\"SERVER_MESSAGE\",\"responseMessage\":\"Theme Data Has Been Updated.\"}";
						SendDataToClient(client, strforsend, strKey, m_ActiveClient);
					}
				}
				else if (strtype == "FETCH_SELECTED_THEME" && checkLoginValidate == 1)
				{
					CString m_loginUser = L"";
					if (d.HasMember("loginUser"))
					{					
						Value& varloginUser = d["loginUser"];
						if (varloginUser.IsString())
						{
							m_loginUser = varloginUser.GetString();
						}					
					}
					if (d.HasMember("loginuser"))
					{
						Value& varloginUser = d["loginuser"];
						if (varloginUser.IsString())
						{
							m_loginUser = varloginUser.GetString();
						}
					}

					CString returnJson = L"";
					returnJson = CStaticClass::m_mtmanager.CreateFETCH_Theme(m_loginUser);
					string strforsend = CT2A(returnJson.GetString());
					SendDataToClient(client, strforsend, strKey, m_ActiveClient);

			}
			else if (strtype == "SAVE_PAGE" && checkLoginValidate == 1)
			{
				if (d.HasMember("page"))
				{
					Value& varpage = d["page"];
					if (varpage.IsObject())
					{
						if (varpage.HasMember("pageId")  && varpage.HasMember("name") && varpage.HasMember("shortName") )
						{
							CString m_pageId = L"";
							CString m_loginUser = L"";
							CString m_name = L"";
							CString m_shortName = L"";
							CString m_layout = L"";
							CString m_widgetConfigs = L"";
							Value& varpageId = varpage["pageId"];
							if (varpageId.IsString())
							{
								m_pageId = varpageId.GetString();
							}							
							m_loginUser = login;							
							Value& varname = varpage["name"];
							if (varname.IsString())
							{
								m_name = varname.GetString();
							}
							Value& varshortName = varpage["shortName"];
							if (varshortName.IsString())
							{
								m_shortName = varshortName.GetString();
							}
							/*Value& varlayout = varpage["layout"];
							if (varlayout.IsString())
							{
								m_layout = varlayout.GetString();
							}*/
							/*Value& varwidgetConfigs = varpage["widgetConfigs"];
							if (varwidgetConfigs.IsString())
							{
								m_widgetConfigs = varwidgetConfigs.GetString();
							}*/							

							int hr = 0;
							CString StrSaveCommand = L"";
							//StrSaveCommand.Format(L"exec Insert_Orika_page_details '%s','%s','%s','%s','%s' ; delete from orika_pageIDandwidgetconfigs where pageId='%s';delete from orika_pageIDandwidgetconfigs_and_sheet_mapping where pageID='%s'", m_pageId, m_loginUser, m_name, m_shortName, m_layout, m_pageId, m_pageId);
							StrSaveCommand.Format(L"exec Insert_Orika_page_details '%s','%s','%s','%s','%s' ; ", m_pageId, m_loginUser, m_name, m_shortName, m_layout);
							hr=CStaticClass::m_sqldata.executeCommandwiterrorcode(StrSaveCommand);
							


							CString Page_String = L"";
							string strjson = JsonAsString(varpage);

							Page_String = strjson.c_str();
							CString FilePath = CStaticClass::APIFolderPath + m_pageId + L".json";
							CStdioFile	myFile;
							int fileopen = 0;
							if (myFile.Open(FilePath, CFile::modeReadWrite| CFile::modeCreate))
							{
								myFile.WriteString(Page_String);
								fileopen = 1;
							}
							if (fileopen == 1)
							{
								myFile.Close();
							}

							StringBuffer s;
							Writer<StringBuffer> writer(s);
							writer.StartObject();
							writer.Key("type");
							writer.String("SAVE_PAGE_RES");
							writer.Key("success");
							if (hr == 0)
							{
								writer.Bool(true);
							}
							else
							{
								writer.Bool(false);
							}
							writer.Key("pageId");
							string strpageId = CT2A(m_pageId);
							const char* cspageId = strpageId.c_str();
							writer.String(cspageId);

							writer.Key("message");
							if (hr == 0)
							{
								writer.String("Page saved successfully");
							}
							else
							{
								writer.String("Error while saving the page");
							}							
							writer.EndObject();


							CString strFinalJson = L"";
							strFinalJson = s.GetString();
							string strforsend = CT2A(strFinalJson.GetString());
							SendDataToClient(client, strforsend, strKey, m_ActiveClient);
							s.Clear();
							writer.Flush();

						}						 							 							      							 							    
					}
				}
			}
			else if (strtype == "UPDATE_PAGE" && checkLoginValidate == 1)
			{
			if (d.HasMember("page"))
			{
				Value& varpage = d["page"];
				if (varpage.IsObject())
				{
					if (varpage.HasMember("pageId") && varpage.HasMember("name") && varpage.HasMember("shortName") )
					{
						CString m_pageId = L"";
						CString m_loginUser = L"";
						CString m_name = L"";
						CString m_shortName = L"";
						CString m_layout = L"";
						CString m_widgetConfigs = L"";
						Value& varpageId = varpage["pageId"];
						if (varpageId.IsString())
						{
							m_pageId = varpageId.GetString();
						}
						//(m_pageId);
						m_loginUser = login;

						Value& varname = varpage["name"];
						if (varname.IsString())
						{
							m_name = varname.GetString();
						}
						Value& varshortName = varpage["shortName"];
						if (varshortName.IsString())
						{
							m_shortName = varshortName.GetString();
						}
						Value& varlayout = varpage["layout"];
						if (varlayout.IsString())
						{
							m_layout = varlayout.GetString();
						}
						Value& varwidgetConfigs = varpage["widgetConfigs"];
						if (varwidgetConfigs.IsString())
						{
							m_widgetConfigs = varwidgetConfigs.GetString();
						}

						int hr = 0;
						CString StrSaveCommand = L"";
						StrSaveCommand.Format(L"delete from orika_pageIDandwidgetconfigs where pageId='%s'; update orika_page_details set name='%s',shortName='%s',layout='%s' where pageId='%s';delete from orika_pageIDandwidgetconfigs_and_sheet_mapping where pageID='%s'", m_pageId,  m_name, m_shortName, m_layout,m_pageId,m_pageId);
						hr = CStaticClass::m_sqldata.executeCommandwiterrorcode(StrSaveCommand);


						

						CString Page_String = L"";
						string strjson = JsonAsString(varpage);

						Page_String = strjson.c_str();
						CString FilePath = CStaticClass::APIFolderPath+m_pageId + L".json";
						CStdioFile	myFile;
						//(FilePath);
						//(Page_String);
						if (myFile.Open(FilePath, CFile::modeCreate| CFile::modeWrite))
						{
							myFile.WriteString(Page_String);
						}
						myFile.Close();



						//End Updating widgetConfig in Database
						StringBuffer s;
						Writer<StringBuffer> writer(s);
						writer.StartObject();
						writer.Key("type");
						writer.String("UPDATE_PAGE_RES");
						writer.Key("success");
						if (hr == 0)
						{
							writer.Bool(true);
						}
						else
						{
							writer.Bool(false);
						}
						writer.Key("pageId");
						string strpageId = CT2A(m_pageId);
						const char* cspageId = strpageId.c_str();
						writer.String(cspageId);

						writer.Key("message");
						if (hr == 0)
						{
							writer.String("Page Updated");
						}
						else
						{
							writer.String("Error while updating the page");
						}
						writer.EndObject();


						CString strFinalJson = L"";
						strFinalJson = s.GetString();
						string strforsend = CT2A(strFinalJson.GetString());
						SendDataToClient(client, strforsend, strKey, m_ActiveClient);
						s.Clear();
						writer.Flush();

					}
				}
			}
			}
			else if (strtype == "DELETE_PAGE" && checkLoginValidate == 1)
			{
				if (d.HasMember("pageId"))
				{
						Value& varpageId = d["pageId"];				
						CString m_pageId = L"";					
						if (varpageId.IsString())
						{
							m_pageId = varpageId.GetString();
						}	
						if (m_pageId.Trim() != L"")
						{
							int hr = 0;
							CString StrSaveCommand = L"";
							StrSaveCommand.Format(L"delete orika_page_details where pageId='%s'", m_pageId);
							hr = CStaticClass::m_sqldata.executeCommandwiterrorcode(StrSaveCommand);

							StringBuffer s;
							Writer<StringBuffer> writer(s);
							writer.StartObject();
							writer.Key("type");
							writer.String("DELETE_PAGE_RES");
							writer.Key("success");
							if (hr == 0)
							{
								writer.Bool(true);
							}
							else
							{
								writer.Bool(false);
							}
							writer.Key("pageId");
							string strpageId = CT2A(m_pageId);
							const char* cspageId = strpageId.c_str();
							writer.String(cspageId);

							writer.Key("message");
							if (hr == 0)
							{
								writer.String("Page Deleted");
							}
							else
							{
								writer.String("Error while deleting the page");
							}
							writer.EndObject();


							CString strFinalJson = L"";
							strFinalJson = s.GetString();
							string strforsend = CT2A(strFinalJson.GetString());
							SendDataToClient(client, strforsend, strKey, m_ActiveClient);
							s.Clear();
							writer.Flush();

						}
				
				}
			}
			else if (strtype == "FETCH_PAGE_LIST" && checkLoginValidate == 1)
			{
				CString m_loginUser = L"";
				if (d.HasMember("loginUser"))
				{
					Value& varloginUser = d["loginUser"];
					if (varloginUser.IsString())
					{
						m_loginUser = varloginUser.GetString();
						CString returnJson = L"";
						returnJson = CStaticClass::m_mtmanager.CreateFETCH_PAGE_LIST(m_loginUser);
						string strforsend = CT2A(returnJson.GetString());
						SendDataToClient(client, strforsend, strKey, m_ActiveClient);						
					}
				}
			}
			else if (strtype == "UPDATE_PAGE_SEQUENCE" && checkLoginValidate == 1)
			{
				CString Updatecommand = L"";
				if (d.HasMember("data"))
				{
					Value& vardata = d["data"];
					if (vardata.IsArray())
					{
						for (SizeType i = 0; i < vardata.Size(); i++)
						{
							CString m_tmpUpdateCommand = L"";
							Value& pageidsequence = vardata[i];
							if (pageidsequence.IsObject())
							{
								CString m_pageid = L"";
								int m_sequence = 0;
								if (pageidsequence.HasMember("pageId"))
								{
									Value& varpageid = pageidsequence["pageId"];
									if (varpageid.IsString())
									{
										m_pageid = varpageid.GetString();
									}
								}
								if (pageidsequence.HasMember("pageid"))
								{
									Value& varpageid = pageidsequence["pageid"];
									if (varpageid.IsString())
									{
										m_pageid = varpageid.GetString();
									}
								}
								if (pageidsequence.HasMember("sequence"))
								{
									Value& varsequence = pageidsequence["sequence"];
									if (varsequence.IsNumber())
									{
										m_sequence = varsequence.GetInt();
									}
								}

								m_tmpUpdateCommand.Format(L"update [orika_page_details] set sequence='%d' where pageId='%s';", m_sequence, m_pageid);

								Updatecommand = Updatecommand + m_tmpUpdateCommand;
							}
						}


						if (Updatecommand.Trim() != L"")
						{							
							CStaticClass::m_sqldata.executeCommand(Updatecommand);
							string strforsend = "{\"type\":\"SERVER_MESSAGE\",\"responseMessage\":\"Data Has Been Updated.\"}";
							SendDataToClient(client, strforsend, strKey, m_ActiveClient);
						}
					}
				}
			}


			else if (strtype == "FETCH_COLOR_THEMES" && checkLoginValidate == 1)
			{
				CString m_loginUser = L"";
				if (d.HasMember("loginUser"))
				{
					Value& varloginUser = d["loginUser"];
					if (varloginUser.IsString())
					{
						m_loginUser = varloginUser.GetString();
						CString returnJson = L"";
						returnJson = CStaticClass::m_mtmanager.FETCH_COLOR_THEMES(m_loginUser);
						string strforsend = CT2A(returnJson.GetString());
						SendDataToClient(client, strforsend, strKey, m_ActiveClient);
					}
				}
			}
			//FETCH_COLOR_THEME
			else if (strtype == "FETCH_COLOR_THEME" && checkLoginValidate == 1)
			{
				CString m_id = L"";
				if (d.HasMember("id"))
				{
					Value& varid = d["id"];
					if (varid.IsString())
					{
						m_id = varid.GetString();
						CString returnJson = L"";
						returnJson = CStaticClass::m_mtmanager.FETCH_COLOR_THEME(m_id);
						string strforsend = CT2A(returnJson.GetString());
						SendDataToClient(client, strforsend, strKey, m_ActiveClient);
					}
				}
			}
			else if (strtype == "SAVE_COLOR_THEME" && checkLoginValidate == 1)
			{
			CString m_loginUser = L"";
			CString m_id = L"";
			CString m_name = L"";
			CString m_color = L"";
			CString m_fontColor = L"";
			int  m_default = 0;
			if (d.HasMember("loginUser"))
			{
				Value& varloginUser = d["loginUser"];
				if (varloginUser.IsString())
				{
					m_loginUser = varloginUser.GetString();
				}
			}
			
			if (d.HasMember("theme"))
			{
				Value& vartheme = d["theme"];
				if (vartheme.IsObject() && vartheme.HasMember("id") && vartheme.HasMember("name") && vartheme.HasMember("color") && vartheme.HasMember("fontColor"))
				{
					Value& varm_id = vartheme["id"];
					if (varm_id.IsString())
					{
						m_id = varm_id.GetString();
					}

					Value& varname = vartheme["name"];
					if (varname.IsString())
					{
						m_name = varname.GetString();
					}
					
					Value& varcolor = vartheme["color"];										
					StringBuffer m_tempbuffer;
					Writer<StringBuffer>m_tempwriter(m_tempbuffer);
					varcolor.Accept(m_tempwriter);
					CString m_t_color(m_tempbuffer.GetString());
					m_color = m_t_color;
					
					m_tempbuffer.Clear();
					m_tempwriter.Flush();;

					m_tempwriter.Reset(m_tempbuffer);
					Value& varfontColor = vartheme["fontColor"];
					varfontColor.Accept(m_tempwriter);
					CString m_t_fontColor(m_tempbuffer.GetString());
					m_fontColor = m_t_fontColor;
					m_tempbuffer.Clear();
					m_tempwriter.Flush();;


					
					Value& vardefault= vartheme["default"];
					if (vardefault.IsBool())
					{
						bool m_b_default = vardefault.GetBool();
						if (m_b_default == true)
						{
							m_default = 1;
						}
					}
				}				
				if (m_id.Trim() != L"")
				{					
					CString Insertcommand = L"";
					Insertcommand.Format(L"delete from orika_COLOR_THEME where loginUser='%s' and COLOR_THEME_ID='%s';insert into orika_COLOR_THEME(loginUser,COLOR_THEME_ID,COLOR_THEME_name,[color],[fontcolor],[default])values('%s','%s','%s','%s','%s','%d')", m_loginUser,m_id, m_loginUser, m_id, m_name, m_color, m_fontColor, m_default);
					CStaticClass::m_sqldata.executeCommand(Insertcommand);
					string strforsend = "{\"type\":\"SERVER_MESSAGE\",\"responseMessage\":\"Data Has Been Updated.\"}";
					SendDataToClient(client, strforsend, strKey, m_ActiveClient);
				}
			}
			}


			else if (strtype == "SAVE_COLOR_TEMPLATE" && checkLoginValidate == 1)
			{
				CString m_loginUser = L"";
				CString m_id = L"";
				if (d.HasMember("loginUser"))
				{
					Value& varloginUser = d["loginUser"];
					if (varloginUser.IsString())
					{
						m_loginUser = varloginUser.GetString();						
					}
				}
				/*if (d.HasMember("id"))
				{
					Value& varm_id = d["id"];
					if (varm_id.IsString())
					{
						m_id = varm_id.GetString();
					}
				}*/
				if (d.HasMember("template"))
				{
					Value& vartemplate = d["template"];
					if (vartemplate.IsObject()&& vartemplate.HasMember("id"))
					{
						Value& varm_id = vartemplate["id"];
						if (varm_id.IsString())
						{
							m_id = varm_id.GetString();
						}
					}
					if (m_id.Trim() != L"")
					{
						std::string sttemplate = JsonAsString(vartemplate);
						CString strtemplate(sttemplate.c_str());
						CString Insertcommand = L"";
						Insertcommand.Format(L"delete from orika_COLOR_TEMPLATE_ClientWise where loginUser='%s' and id='%s';insert into orika_COLOR_TEMPLATE_ClientWise(loginUser,id,colortemplate) values('%s','%s','%s')", m_loginUser, m_id, m_loginUser, m_id, strtemplate);
						CStaticClass::m_sqldata.executeCommand(Insertcommand);
						string strforsend = "{\"type\":\"SERVER_MESSAGE\",\"responseMessage\":\"Data Has Been Updated.\"}";
						SendDataToClient(client, strforsend, strKey, m_ActiveClient);
					}
				}
			}
			else if (strtype == "DELETE_TEMPLATE" && checkLoginValidate == 1)
			{
				CString m_loginUser = L"";
				CString m_id = L"";
				if (d.HasMember("loginUser"))
				{
					Value& varloginUser = d["loginUser"];
					if (varloginUser.IsString())
					{
						m_loginUser = varloginUser.GetString();
					}
				}
				if (d.HasMember("id"))
				{
					Value& varm_id = d["id"];
					if (varm_id.IsString())
					{
						m_id = varm_id.GetString();
					}
				}			
				CString Insertcommand = L"";
				Insertcommand.Format(L"delete from orika_COLOR_TEMPLATE_ClientWise where loginUser='%s' and id='%s';", m_loginUser, m_id);
				CStaticClass::m_sqldata.executeCommand(Insertcommand);
				string strforsend = "{\"type\":\"SERVER_MESSAGE\",\"responseMessage\":\"Data Has Been Deleted.\"}";
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);


			}
			else if (strtype == "DELETE_COLOR_THEME" && checkLoginValidate == 1)
			{
			CString m_loginUser = L"";
			CString m_id = L"";
			if (d.HasMember("loginUser"))
			{
				Value& varloginUser = d["loginUser"];
				if (varloginUser.IsString())
				{
					m_loginUser = varloginUser.GetString();
				}
			}
			if (d.HasMember("id"))
			{
				Value& varm_id = d["id"];
				if (varm_id.IsString())
				{
					m_id = varm_id.GetString();
				}
			}
			CString Insertcommand = L"";
			Insertcommand.Format(L"delete from orika_COLOR_THEME where loginUser='%s' and [COLOR_THEME_ID]='%s';", m_loginUser, m_id);
			CStaticClass::m_sqldata.executeCommand(Insertcommand);
			string strforsend = "{\"type\":\"SERVER_MESSAGE\",\"responseMessage\":\"Data Has Been Deleted.\"}";
			SendDataToClient(client, strforsend, strKey, m_ActiveClient);


			}
			else if (strtype == "FETCH_COLOR_TEMPLATES" && checkLoginValidate == 1)
			{
				CString m_loginUser = L"";
				if (d.HasMember("loginUser"))
				{
					Value& varloginUser = d["loginUser"];
					if (varloginUser.IsString())
					{
						m_loginUser = varloginUser.GetString();
						CString returnJson = L"";
						returnJson = CStaticClass::m_mtmanager.createFETCH_COLOR_TEMPLATE(m_loginUser);
						string strforsend = CT2A(returnJson.GetString());
						SendDataToClient(client, strforsend, strKey, m_ActiveClient);
					}
				}
				
			}

			else if (strtype == "FETCH_PAGE_DETAILS" && checkLoginValidate == 1)
			{
			CString m_loginUser = L"";
			CString m_pageId = L"";
			//pageId
			if (d.HasMember("loginUser") && d.HasMember("pageId"))
			{
				Value& varloginUser = d["loginUser"];
				if (varloginUser.IsString())
				{
					m_loginUser = varloginUser.GetString();					
				}
				Value& varpageId = d["pageId"];
				if (varpageId.IsString())
				{
					m_pageId = varpageId.GetString();
				}

				CString m_login_and_Page = L"";
				m_login_and_Page.Format(L"Login:%s  and PageID:%s", m_loginUser, m_pageId);

				//(m_login_and_Page);

				CString StrFilePath = CStaticClass::APIFolderPath + m_pageId + L".json";

				//(StrFilePath);


				CStdioFile	myFile;
				CString strval = L"";
				CString m_FnalStng = L"";
				int fileOpen_Flag = 0;
				if (myFile.Open(StrFilePath, CFile::modeRead))
				{
					fileOpen_Flag = 1;
					myFile.Seek(0, CFile::begin);
					CString Insert_Command = L"";
					int data_count = 0;
					
					while (myFile.ReadString(strval))
					{
						m_FnalStng = m_FnalStng + strval;
					}
				}
				if (fileOpen_Flag == 1)
				{
					myFile.Close();
				}
				CString returnJson = L"";
				if (m_FnalStng == L"")
				{
					m_FnalStng = L"{}";
				}
				returnJson.Format(L"{\"type\":\"PAGE_DETAILS\",\"page\":\% s\}", m_FnalStng);
				//returnJson = CStaticClass::m_mtmanager.CreateFETCH_PAGE_DETAILS(m_loginUser, m_pageId);
				int datalength = returnJson.GetLength();
				string strforsend = CT2A(returnJson.GetString());
				//(returnJson);
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
			}
			}
			else if (strtype == "UPDATE_DATA_BY_TABLENAME" && checkLoginValidate == 1)
			{
				CString m_tablename = L"";
				CString strUniqueKeyCommand = L"";
				CString m_updatedvalue = L"";
				CString m_columnforupdate = L"";
				if (d.HasMember("tablename"))
				{
					Value& tablename = d["tablename"];					
					m_tablename = tablename.GetString();
				}
				CString m_columnname = L"";
				double m_dblcolumnvalue = 0;
				int m_intcolumnvalue = 0;
				CString m_columnvalue = L"";
				if (d.HasMember("uniqueKey"))
				{
					Value& uniqueKey = d["uniqueKey"];
					if (uniqueKey.IsArray())
					{					
						for (SizeType i = 0; i < uniqueKey.Size(); i++)
						{
							Value& uniquekeydata = uniqueKey[i];
							if (uniquekeydata.IsObject())
							{
								
								m_columnvalue = L"";
								if (uniquekeydata.HasMember("columnname"))
								{
									Value& valcolumnname = uniquekeydata["columnname"];																		
									if (valcolumnname.IsString())
									{
										m_columnname = valcolumnname.GetString();
									}
								}

								if (uniquekeydata.HasMember("columnvalue"))
								{
									Value& valcolumnvalue = uniquekeydata["columnvalue"];
									if (valcolumnvalue.IsDouble())
									{
										m_dblcolumnvalue = valcolumnvalue.GetDouble();
										m_columnvalue.Format(L"%.4lf", m_dblcolumnvalue);
									}
									if (valcolumnvalue.IsInt())
									{
										m_intcolumnvalue = valcolumnvalue.GetInt();
										m_columnvalue.Format(L"%d", m_intcolumnvalue);
									}
									if (valcolumnvalue.IsString())
									{
										m_columnvalue = valcolumnvalue.GetString();
									}
								}								
								CString m_tempstr = L"";
								m_tempstr.Format(L"[%s]='%s'", m_columnname, m_columnvalue);
								if (strUniqueKeyCommand.Trim() == L"")
								{
									strUniqueKeyCommand = strUniqueKeyCommand + m_tempstr;
								}
								else
								{
									strUniqueKeyCommand = strUniqueKeyCommand+L" and " + m_tempstr;
								}
							}
						}					
					}
					if (d.HasMember("columnforupdate"))
					{
						Value& columnforupdate = d["columnforupdate"];
						if (columnforupdate.IsString())
						{
							m_columnforupdate = columnforupdate.GetString();
						}
					}
					
					if (d.HasMember("updatedvalue"))
					{
						double m_dblupdatedvalue = 0;
						int m_intupdatedvalue = 0;
						bool m_bullupdatedvalue = false;
						Value& updatedvalue = d["updatedvalue"];
						if (updatedvalue.IsDouble())
						{
							m_dblupdatedvalue = updatedvalue.GetDouble();
							m_updatedvalue.Format(L"%.4lf", m_dblupdatedvalue);
						}
						if (updatedvalue.IsBool())
						{
							m_bullupdatedvalue = updatedvalue.GetBool();
							if (m_bullupdatedvalue == true)
							{
								m_updatedvalue=L"1";
							}
							else
							{
								m_updatedvalue = L"0";
							}
						}
						if (updatedvalue.IsInt())
						{
							m_intupdatedvalue = updatedvalue.GetInt();
							m_updatedvalue.Format(L"%d", m_intupdatedvalue);
						}
						if (updatedvalue.IsString())
						{
							m_updatedvalue = updatedvalue.GetString();
						}



						if (m_tablename == L"orika_order")
						{
							m_intcolumnvalue = _wtoi64(m_columnvalue);
							CStaticClass::st_order_ForUpdate m_st_updateOrder = {};
							m_st_updateOrder.m_order = m_intcolumnvalue;
							m_st_updateOrder.m_select_update = 1;
							m_st_updateOrder.m_select= _wtoi(m_updatedvalue);
							////(L"lock8");

							CStaticClass::m_mutex_order.Lock();
							CStaticClass::st_order st = {};
							CStaticClass::m_Orika_orderHastable.Lookup(m_intcolumnvalue, st);
							st.m_select = _wtoi(m_updatedvalue);
							CStaticClass::m_Orika_orderHastable.SetAt(m_intcolumnvalue, st);
							CStaticClass::m_mutex_order.Unlock();






							CStaticClass::m_mutex_ClientList.Lock();
							//(L"22");
							POSITION pos = CStaticClass::m_ClientContext.GetStartPosition();
							while (pos != NULL)
							{
								CString strclientkey = L"";
								CStaticClass::st_ClientContext m_st = {};
								CStaticClass::m_ClientContext.GetNextAssoc(pos, strclientkey, m_st);
								if (m_st.m_FETCH_ORDER_DATA_start == 1)
								{
									m_st.m_orderarray_ForEdit.Add(&m_st_updateOrder);
									CStaticClass::m_ClientContext.SetAt(strclientkey, m_st);
								}
							}
							CStaticClass::m_mutex_ClientList.Unlock();
						}

					}
					else
					{
						if (m_tablename == L"orika_order")
						{
							
							m_updatedvalue=L"0";
							m_intcolumnvalue = _wtoi64(m_columnvalue);
							CStaticClass::st_order_ForUpdate m_st_updateOrder = {};
							m_st_updateOrder.m_order = m_intcolumnvalue;
							m_st_updateOrder.m_select_update = 1;
							m_st_updateOrder.m_select = _wtoi(m_updatedvalue);
							////(L"lock8");

							CStaticClass::m_mutex_order.Lock();
							CStaticClass::st_order st = {};
							CStaticClass::m_Orika_orderHastable.Lookup(m_intcolumnvalue, st);
							st.m_select = _wtoi(m_updatedvalue);
							CStaticClass::m_Orika_orderHastable.SetAt(m_intcolumnvalue, st);
							CStaticClass::m_mutex_order.Unlock();






							CStaticClass::m_mutex_ClientList.Lock();
							//(L"22");
							POSITION pos = CStaticClass::m_ClientContext.GetStartPosition();
							while (pos != NULL)
							{
								CString strclientkey = L"";
								CStaticClass::st_ClientContext m_st = {};
								CStaticClass::m_ClientContext.GetNextAssoc(pos, strclientkey, m_st);
								if (m_st.m_FETCH_ORDER_DATA_start == 1)
								{
									m_st.m_orderarray_ForEdit.Add(&m_st_updateOrder);
									CStaticClass::m_ClientContext.SetAt(strclientkey, m_st);
								}
							}
							CStaticClass::m_mutex_ClientList.Unlock();
						}
					}
				}
				
				/*m_tablename = L"";
				strUniqueKeyCommand = L"";
				m_updatedvalue = L"";*/

				CString strUpdateMarginCommand = L"";
				strUpdateMarginCommand.Format(L"update %s set [%s]='%s' where %s", m_tablename,m_columnforupdate, m_updatedvalue, strUniqueKeyCommand);
				CStaticClass::m_sqldata.executeCommand(strUpdateMarginCommand);


				string strforsend = "{\"type\":\"SERVER_MESSAGE\",\"responseMessage\":\"Data Has Been Updated.\"}";
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
			}

			else if (strtype == "SAVE_CANVAS" && checkLoginValidate == 1)
			{
				/*if (d.HasMember("loginuser"))
				{
					Value& valloginuser = d["loginuser"];
					CString m_loginuser = L"";
					if (valloginuser.IsString())
					{
						m_loginuser = valloginuser.GetString();
					}
				}*/
				CString m_tabtype = L"";
				if (d.HasMember("tabtype"))
				{								
					Value& valtabtype = d["tabtype"];
					
					if (valtabtype.IsString())
					{
						m_tabtype = valtabtype.GetString();
					}								
				}
				CString m_data = L"";
				if (d.HasMember("data"))
				{
					Value& valdata = d["data"];
					
					if (valdata.IsString())
					{
						m_data= valdata.GetString();
					}
				}
				CString strSqlCommand = L"";
				strSqlCommand.Format(L"delete from orika_canvas where loginuser='%s' and tabtype='%s' ;insert into orika_canvas(loginuser,tabtype,[data]) values('%s','%s','%s');", login, m_tabtype, login, m_tabtype, m_data);
				CStaticClass::m_sqldata.executeCommand(strSqlCommand);
				string strforsend = "{\"type\":\"SERVER_MESSAGE\",\"responseMessage\":\"Canvas Data Has Been Updated.\"}";
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
			}
			else if (strtype == "GET_ALL_ALERT" && checkLoginValidate == 1)
			{
				CString m_alertName = L"";
				if (d.HasMember("alertName"))
				{
					Value& val_alertName = d["alertName"];
					if (val_alertName.IsString())
					{
						m_alertName = val_alertName.GetString();
					}
				}
				CString returnJson=CStaticClass::m_sqldata.getAlertSetting(m_alertName);
				string strforsend = CT2A(returnJson.GetString());
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
			}
			else if (strtype == "SAVE_UPDATE_ALERT" && checkLoginValidate == 1)
			{
				CString m_alertName = L"";
				CString m_triggerType = L"";
				CString 	m_startTime = L"";
				CString 	m_expiryTime = L"";
				int m_repetitions=0;
				int m_hours = 0;
				int m_minutes = 0;
				int m_days = 0;
				CString 	m_daysOfMonth = L"";
				CString 	m_daysOfWeek = L"";
				CString 	m_selectedMonths = L"";
				if (d.HasMember("alertName"))
				{
					Value& val_alertName = d["alertName"];
					if (val_alertName.IsString())
					{
						m_alertName = val_alertName.GetString();
					}					
				}
				/*if (d.HasMember("triggerType"))
				{
					Value& val_triggerType = d["triggerType"];
					if (val_triggerType.IsString())
					{
						m_triggerType = val_triggerType.GetString();
					}
				}

				if (d.HasMember("startTime"))
				{
					Value& val_startTime = d["startTime"];
					if (val_startTime.IsString())
					{
						m_startTime = val_startTime.GetString();
					}
				}
				if (d.HasMember("expiryTime"))
				{
					Value& val_expiryTime = d["expiryTime"];
					if (val_expiryTime.IsString())
					{
						m_expiryTime = val_expiryTime.GetString();
					}
				}

				if (d.HasMember("repetitions"))
				{
					Value& val_repetitions = d["repetitions"];
					if (val_repetitions.IsNumber())
					{
						m_repetitions = val_repetitions.GetInt();
					}
				}
				if (d.HasMember("hours"))
				{
					Value& val_hours = d["hours"];
					if (val_hours.IsNumber())
					{
						m_hours = val_hours.GetInt();
					}
				}

				if (d.HasMember("minutes"))
				{
					Value& val_minutes = d["minutes"];
					if (val_minutes.IsNumber())
					{
						m_minutes = val_minutes.GetInt();
					}
				}
				if (d.HasMember("days"))
				{
					Value& val_days = d["days"];
					if (val_days.IsNumber())
					{
						m_days = val_days.GetInt();
					}
				}
				if (d.HasMember("daysOfMonth"))
				{
					Value& val_dayofMonth = d["daysOfMonth"];

					rapidjson::StringBuffer buffer;
					rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
					val_dayofMonth.Accept(writer);

					string str_dayofMonth = buffer.GetString();

					CString mt_dayofMonth(str_dayofMonth.c_str());
					m_daysOfMonth = mt_dayofMonth;
				}
				if (d.HasMember("daysOfWeek"))
				{
					Value& val_daysOfWeek = d["daysOfWeek"];
					
					rapidjson::StringBuffer buffer;
					rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
					val_daysOfWeek.Accept(writer);

					string str_daysOfWeek = buffer.GetString();

					CString mt_daysOfWeek(str_daysOfWeek.c_str());
					m_daysOfWeek = mt_daysOfWeek;
					
					
				}
				if (d.HasMember("selectedMonths"))
				{
					Value& val_selectedMonths = d["selectedMonths"];
					rapidjson::StringBuffer buffer;
					rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
					val_selectedMonths.Accept(writer);

					string str_selectedMonths = buffer.GetString();

					CString mt_selectedMonths(str_selectedMonths.c_str());
					m_selectedMonths = mt_selectedMonths;
				}*/

				d.RemoveMember("type");

				Document::AllocatorType& allocator = d.GetAllocator();
				if (d.HasMember("hours")==false)
				{
					d.AddMember("hours", 0, allocator);
				}

				if (d.HasMember("minutes") == false)
				{
					d.AddMember("minutes", 0, allocator);
				}
				if (d.HasMember("days") == false)
				{
					d.AddMember("days", 0, allocator);  
				}

				StringBuffer buffer;
				Writer<StringBuffer> writer(buffer);
				d.Accept(writer);
				string m_str_jasonString = buffer.GetString();
				CString m_jasonString(m_str_jasonString.c_str());

				CString strFinalCommand = L"";
				strFinalCommand.Format(L"delete from orika_Event_Setting where alertName='%s';insert into orika_Event_Setting (alertName,alertsetting_jason)values('%s','%s')",m_alertName, m_alertName, m_jasonString);
				int m_status = CStaticClass::m_sqldata.executeCommandwiterrorcode(strFinalCommand);
				string strforsend = "";
				if (m_status == 0)
				{
					CStaticClass::m_loginuserlist.RemoveKey(strloginuser);
					strforsend = "{\"type\":\"SERVER_MESSAGE\",\"responseMessage\":\"Event Has Been Updated Successfully.\"}";
				}
				else
				{
					strforsend = "{\"type\":\"SERVER_MESSAGE\",\"responseMessage\":\"Error while updating data\"}";
				}
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
				
				
				
				CString returnJson = CStaticClass::m_sqldata.getAlertSetting(m_alertName);
				

				sendDataToAllClient(returnJson);
				//SendDataToClient(client, strforsend, strKey, m_ActiveClient);

				





			}

			else if (strtype == "DELETE_ALERT" && checkLoginValidate == 1)
			{
				CString m_alertName = L"";
				CString m_triggerType = L"";
				CString 	m_startTime = L"";
				CString 	m_expiryTime = L"";
				int m_repetitions = 0;
				int m_hours = 0;
				int m_minutes = 0;
				int m_days = 0;
				CString 	m_daysOfMonth = L"";
				CString 	m_daysOfWeek = L"";
				CString 	m_selectedMonths = L"";
				if (d.HasMember("alertName"))
				{
					Value& val_alertName = d["alertName"];
					if (val_alertName.IsString())
					{
						m_alertName = val_alertName.GetString();
					}
				}				
				

				StringBuffer buffer;
				Writer<StringBuffer> writer(buffer);
				d.Accept(writer);
				string m_str_jasonString = buffer.GetString();
				CString m_jasonString(m_str_jasonString.c_str());

				CString strFinalCommand = L"";
				strFinalCommand.Format(L"delete from orika_Event_Setting where alertName='%s';", m_alertName);
				int m_status = CStaticClass::m_sqldata.executeCommandwiterrorcode(strFinalCommand);
				string strforsend = "";
				if (m_status == 0)
				{
					CStaticClass::m_loginuserlist.RemoveKey(strloginuser);
					strforsend = "{\"type\":\"SERVER_MESSAGE\",\"responseMessage\":\"Event Has Been Deleted Successfully.\"}";
				}
				else
				{
					strforsend = "{\"type\":\"SERVER_MESSAGE\",\"responseMessage\":\"Error while deleting data\"}";
				}
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);




				}



			else if (strtype == "GET_SYMBOLMAPPING" && checkLoginValidate == 1)
			{
				CString returnJson = L"";
				returnJson = CStaticClass::m_mtmanager.GetsymbolMappingData();
				string strforsend = CT2A(returnJson.GetString());
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
			}
			else if (strtype == "FETCH_CLOSINGPRICE" && checkLoginValidate == 1)
			{
				CString returnJson = L"";
				returnJson = CStaticClass::m_mtmanager.getUpdatedClosingRate();
				string strforsend = CT2A(returnJson.GetString());
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
			}
			else if (strtype == "FETCH_CLIENT_PREPOSITION" && checkLoginValidate == 1)
			{
				if (d.HasMember("time"))
				{
					const Value& Keytime = d["time"];
					if (Keytime.IsNumber())
					{
						CStaticClass::st_ClientContext m_st = {};
						int positionTime = 0;
						positionTime = Keytime.GetInt();
						////(L"lock4");
						CStaticClass::m_mutex_ClientList.Lock();
						//(L"125");
						CStaticClass::m_ClientContext.Lookup(strKey, m_st);
						m_st.m_calculatePreNetPosition = 1;
						if (_action == L"refresh")
						{
							m_st.m_refreshClientPosition = 1;
						}
						m_st.m_netPositionTime = positionTime;
						CStaticClass::m_ClientContext.SetAt(strKey, m_st);
						CStaticClass::m_mutex_ClientList.Unlock();
						//(L"U125");
					}
				}
			}
			else if (strtype == "FETCH_CLIENT_MASTER_DATA" && checkLoginValidate == 1)
			{
				CString strjson = CStaticClass::m_sqldata.generateClientmasterJson();
				string strforsend = CT2A(strjson.GetString());
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
			}
			else if ((strtype == "COLUMN_TEMPLATE_SAVE" || strtype == "COLUMN_TEMPLATE_UPDATE") && checkLoginValidate == 1)
			{			
				CString m_name = L"";
				int m_id = 0;
				CString m_userlogin = L"";
				if (d.HasMember("name") )
				{
					Value& valname = d["name"];
					if (valname.IsString())
					{
						m_name= valname.GetString();
					}
				}
				if (d.HasMember("id"))
				{
					Value& valid = d["id"];
					if (valid.IsInt())
					{
						m_id = valid.GetInt();
					}
				}
				if (d.HasMember("userlogin"))
				{
					Value& valuserlogin = d["userlogin"];
					if (valuserlogin.IsString())
					{
						m_userlogin = valuserlogin.GetString();
					}
				}
				CString m_rows = L"";
				if (d.HasMember("colors"))
				{
					Value& valcolor = d["colors"];
					if (valcolor.IsArray())
					{
						for (SizeType i = 0; i < valcolor.Size(); i++)
						{
							int m_sequence = 0;
							CString m_color = L"";
							CString m_fontcolor = L"";
							CString m_value = L"";
							CString m_row = L"";
							if (valcolor[i].IsObject())
							{
								Value& colorObjects = valcolor[i];
								if (colorObjects.HasMember("sequence"))
								{
									Value& valSequence = colorObjects["sequence"];
									if (valSequence.IsInt())
									{
										m_sequence = valSequence.GetInt();
									}
								}
								if (colorObjects.HasMember("color"))
								{
									Value& valcolor = colorObjects["color"];
									if (valcolor.IsString())
									{
										m_color = valcolor.GetString();
									}
								}

								if (colorObjects.HasMember("fontColor"))
								{
									Value& valfontColor = colorObjects["fontColor"];
									if (valfontColor.IsString())
									{
										m_fontcolor = valfontColor.GetString();
									}
								}
								//Field Has Been Changed Value To startPoint
								if (colorObjects.HasMember("startPoint"))
								{
									Value& valvalue = colorObjects["startPoint"];
									if (valvalue.IsString())
									{
										m_value = valvalue.GetString();
									}
								}

								CString m_colorDark = L"";
								CString m_fontColorDark = L"";
								
								if (colorObjects.HasMember("colorDark"))
								{
									Value& valvalue = colorObjects["colorDark"];
									if (valvalue.IsString())
									{
										m_colorDark = valvalue.GetString();
									}
								}


								if (colorObjects.HasMember("fontColorDark"))
								{
									Value& valvalue = colorObjects["fontColorDark"];
									if (valvalue.IsString())
									{
										m_fontColorDark = valvalue.GetString();
									}
								}

								m_row.Format( L"(%d,''%s'',''%s'',''%s'',''%s'',''%s'')", m_sequence, m_color, m_value, m_fontcolor, m_colorDark, m_fontColorDark);
								if (m_rows.GetLength()>0)
								{ 
									m_rows = m_rows+L"," + m_row;
								}
								else
								{
									m_rows = m_rows + m_row;
								}
							}
						}
					}
				}
				CString strjson = L"";
				if (strtype == "COLUMN_TEMPLATE_SAVE")
				{
					strjson = CStaticClass::m_sqldata.columnTemplateSave(m_name, m_userlogin, m_rows);
				}
				if (strtype == "COLUMN_TEMPLATE_UPDATE")
				{
					strjson = CStaticClass::m_sqldata.columnTemplateUpdate(m_id,m_name, m_userlogin, m_rows);					
				}
				string strforsend = CT2A(strjson.GetString());
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
			}			
			else if (strtype == "COLUMN_TEMPLATE_DELETE" && checkLoginValidate == 1)
			{
				CString m_name = L"";
				int m_id = 0;
				CString m_userlogin = L"";				
				if (d.HasMember("id"))
				{
					Value& valid = d["id"];
					if (valid.IsInt())
					{
						m_id = valid.GetInt();
					}
					CString	strjson = CStaticClass::m_sqldata.columnTemplateDelete(m_id);				
					string strforsend = CT2A(strjson.GetString());
					SendDataToClient(client, strforsend, strKey, m_ActiveClient);
				}
			}
			else if (strtype == "FETCH_COLUMN_TEMPLATE" && checkLoginValidate == 1)
			{				
				CString m_userlogin = L"";
				if (d.HasMember("userlogin"))
				{
					Value& valuserlogin = d["userlogin"];
					if (valuserlogin.IsString())
					{
						m_userlogin = valuserlogin.GetString();
					}
					CString	strjson = CStaticClass::m_sqldata.getColumnTempletUserLoginWise(m_userlogin);
					string strforsend = CT2A(strjson.GetString());
					SendDataToClient(client, strforsend, strKey, m_ActiveClient);
				}
			}
			else if (strtype == "FETCH_CLIENT_WISE_PREVIOUS_NET_VOLUME" && checkLoginValidate == 1)
			{
				int requestTime = 0;
				//previoustime
				if (d.HasMember("previoustime"))
				{
					Value& valrequestTime = d["previoustime"];
					if (valrequestTime.IsInt())
					{
						requestTime = valrequestTime.GetInt();
					}
				}
				CString strjson = CStaticClass::m_sqldata.getClientwisePreQty(strKey, requestTime);
				string strforsend = CT2A(strjson.GetString());
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
				//////(L" Netposition data has been send");
			}
			
			else if (strtype == "DELETE_COMMENT_CHANGE_DATA" && checkLoginValidate == 1)
			{
				TMTArray<INT64> tmpdealarray;
				CString strdeals = L"";
				if (d.HasMember("deals"))
				{
					Value& deals = d["deals"];
					if (deals.IsArray())
					{
						for (SizeType i = 0; i < deals.Size(); i++)
						{

							if (deals[i].IsInt())
							{
								CString strtmpdeal = L"";
								Value& valint = deals[i];
								INT64  intdeal = valint.GetInt();
								tmpdealarray.Add(&intdeal);
								strtmpdeal.Format(L"%d", intdeal);
								if (i == 0)
								{
									strdeals = strtmpdeal;
								}
								else
								{
									strdeals = strdeals + L"," + strtmpdeal;
								}
							}
						}
					}
				}

				CString strjson = CStaticClass::m_sqldata.deleteDataCommentChange(strdeals);
				string strforsend = CT2A(strjson.GetString());
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
				if (strjson.Find(L"Data Has Been Deleted") >= 0)
				{
					////(L"lock5");
					CStaticClass::m_mutex_ClientList.Lock();
					//(L"126");
					POSITION pos = CStaticClass::m_ClientContext.GetStartPosition();
					while (pos != NULL)
					{
						CString strclientkey = L"";
						CStaticClass::st_ClientContext m_st = {};
						CStaticClass::m_ClientContext.GetNextAssoc(pos, strclientkey, m_st);
						if (m_st.m_FETCH_EXISTING_COMMENT_CHANGE_DATA_start == 1)
						{
							m_st.m_commentArray_delete.Assign(tmpdealarray);
							CStaticClass::m_ClientContext.SetAt(strclientkey, m_st);
						}
					}
					CStaticClass::m_mutex_ClientList.Unlock();
					//(L"U126");
					tmpdealarray.Clear();
				}
			}
			else if (strtype == "COMMENT_DEALWISE_REQUEST" && checkLoginValidate == 1)
			{
				CString strdeals = L"";
				if (d.HasMember("deals"))
				{
					Value& deals = d["deals"];
					if (deals.IsArray())
					{
						for (SizeType i = 0; i < deals.Size(); i++)
						{

							if (deals[i].IsInt())
							{
								CString strtmpdeal = L"";
								Value& valint = deals[i];
								int intdeal = valint.GetInt();
								strtmpdeal.Format(L"%d", intdeal);
								if (i == 0)
								{
									strdeals = strtmpdeal;
								}
								else
								{
									strdeals = strdeals + L"," + strtmpdeal;
								}
							}
						}
					}
				}



				CString strjson = CStaticClass::m_sqldata.getCommentDealWise(strdeals);
				string strforsend = CT2A(strjson.GetString());
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
			}
			else if (strtype == "FETCH_CLIENT_POSITIONS" && checkLoginValidate == 1)
			{
				CString strjson = CStaticClass::m_sqldata.getClientwisenetpositionData(strKey);
				string strforsend = CT2A(strjson.GetString());
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
				CStaticClass::st_ClientContext m_st = {};
				////(L"lock6");
				CStaticClass::m_mutex_ClientList.Lock();
				////(L"127");
				CStaticClass::m_ClientContext.Lookup(strKey, m_st);
				m_st.m_ClientWiseNetPositionStart = 1;
				if (_action == L"refresh")
				{
					m_st.m_refreshClientPosition = 1;
				}				
				CStaticClass::m_ClientContext.SetAt(strKey, m_st);
				CStaticClass::m_mutex_ClientList.Unlock();
				////(L"U127");

			}
			
			else if (strtype == "FETCH_SYMBOL_MASTER_DATA" && checkLoginValidate == 1)
			{
				//////(L"Enter in handle_11");
				CString strjson = CStaticClass::m_sqldata.generateSymbolMasterJson();
				string strforsend = CT2A(strjson.GetString());
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
				//////(L"Enter in handle_12");
			}								
			else if (strtype == "FETCH_USERS_DATA" && checkLoginValidate == 1)
			{
				//////(L"Enter in handle_11");
				CString strjson = CStaticClass::m_sqldata.generateJsonLoginData();
				string strforsend = CT2A(strjson.GetString());
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
				//////(L"Enter in handle_12");
			}
			else if (strtype == "FETCH_USER_DETAILS" && checkLoginValidate == 1)
			{
				//////(L"Enter in handle_11");
				CString strloginuser = L"";
				if (d.HasMember("loginuser"))
				{
					const Value& loginuser = d["loginuser"];					
					strloginuser = loginuser.GetString();
				}
				if (d.HasMember("loginUser"))
				{
					const Value& loginuser = d["loginUser"];
					strloginuser = loginuser.GetString();
				}
				CString strjson = CStaticClass::m_sqldata.generateJsonLoginData_Details(strloginuser);
				string strforsend = CT2A(strjson.GetString());
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
				//////(L"Enter in handle_12");
			}
			else if (strtype == "DELETE_USER_DETAILS" && checkLoginValidate == 1)
			{
				CString strloginuser = L"";
				
				if (d.HasMember("data"))
				{
					const Value& vardata = d["data"];
					if (vardata.IsObject())
					{
						if (vardata.HasMember("loginuser"))
						{
							const Value& loginuser = vardata["loginuser"];
							strloginuser = loginuser.GetString();
						}
						if (vardata.HasMember("loginUser"))
						{
							const Value& loginuser = vardata["loginUser"];
							strloginuser = loginuser.GetString();
						}
					}
				}


				if (d.HasMember("loginuser"))
				{
					const Value& loginuser = d["loginuser"];
					strloginuser = loginuser.GetString();
				}
				if (d.HasMember("loginUser"))
				{
					const Value& loginuser = d["loginUser"];
					strloginuser = loginuser.GetString();
				}

				CString strFinalCommand = L"";
				strFinalCommand.Format(L"delete from Orika_userlogin where userlogin='%s';delete from orika_userLoginHDkeyMapping where userlogin='%s';delete from orika_userLoginAndPermission where loginuser='%s';delete from orika_userLoginAndGroupMapping where loginuser='%s';delete from orika_userTableColumnMapping where loginuser='%s'", strloginuser, strloginuser, strloginuser, strloginuser, strloginuser);
				int m_status=CStaticClass::m_sqldata.executeCommandwiterrorcode(strFinalCommand);
				string strforsend = "";
				if (m_status == 0)
				{
					CStaticClass::m_loginuserlist.RemoveKey(strloginuser);
					strforsend = "{\"type\":\"SERVER_MESSAGE\",\"responseMessage\":\"Data Has Been Deleted.\"}";
				}
				else
				{
					strforsend = "{\"type\":\"SERVER_MESSAGE\",\"responseMessage\":\"Error while Deleting data.\"}";
				}
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);

			}
			else if (strtype == "SAVE_USER_DETAILS" && checkLoginValidate == 1)
			{				
				CMap<CString, LPCTSTR, int, int> m_entireGroupMap;
				CMap<CString, LPCTSTR, int, int> m_notentireGroupMap;
				CMap<CString, LPCTSTR, int, int> m_groups;
				CString strloginuser = L"";
				CString strpassword = L"";
				CString strname = L"";
				if (d.HasMember("data"))
				{
					const Value& vardata = d["data"];
					if (vardata.IsObject())
					{
						if (vardata.HasMember("loginuser"))
						{
							const Value& loginuser = vardata["loginuser"];
							strloginuser = loginuser.GetString();
						}
						if (vardata.HasMember("password"))
						{
							const Value& varpassword = vardata["password"];
							strpassword = varpassword.GetString();
						}
						if (vardata.HasMember("name"))
						{
							const Value& varname = vardata["name"];
							strname = varname.GetString();
						}
						CString m_strgroupCommand = L"";
						CString m_strpermission = L"";
						CString m_strfprintcommand = L"";
						if (vardata.HasMember("groups"))
						{
							const Value& vargroups = vardata["groups"];
							if (vargroups.IsArray())
							{
								for (SizeType i = 0; i < vargroups.Size(); i++)
								{
									const Value& vargroup = vargroups[i];
									if (vargroup.IsObject())
									{
										if (vargroup.HasMember("group") && vargroup.HasMember("checked"))
										{
											CString strgroup = L"";
											bool m_checked = false;
											int m_int_checked = 0;
											const Value& vargroupvalue = vargroup["group"];
											if (vargroupvalue.IsString())
											{
												strgroup = vargroupvalue.GetString();
											}
											CString m_OGroup = strgroup;
											CString m_parent_Group = L"";
											CString m_parent_subGroup = L"";
											while (m_OGroup.Find(L"\\") > 0)
											{
												m_parent_subGroup = m_OGroup.Mid(0, m_OGroup.Find(L"\\"));
												if (m_parent_Group == L"")
												{
													m_parent_Group = m_parent_Group + m_parent_subGroup;
												}
												else
												{
													m_parent_Group = m_parent_Group+L"\\" + m_parent_subGroup;
												}

												CString m_parent_Group_select = m_parent_Group + L"\\*";
												m_entireGroupMap.SetAt(m_parent_Group_select, 1);
												m_groups.SetAt(m_parent_Group_select, 1);
												m_OGroup= m_OGroup.Mid(m_OGroup.Find(L"\\")+1, m_OGroup.GetLength()- m_OGroup.Find(L"\\")-1);
											}
											m_parent_Group = m_parent_Group + L"\\*";
											m_entireGroupMap.SetAt(m_parent_Group,1);
											m_groups.SetAt(m_parent_Group, 1);
											const Value& varchecked = vargroup["checked"];
											if (varchecked.IsBool())
											{
												m_checked = varchecked.GetBool();
												if (m_checked == true)
												{
													m_int_checked = 1;
												}
												else
												{
													POSITION m_temp_pos = m_groups.GetStartPosition();
													while (m_temp_pos != NULL)
													{
														CString m_tempstar = L"";
														int m_check = 0;
														m_groups.GetNextAssoc(m_temp_pos, m_tempstar, m_check);
														m_entireGroupMap.RemoveKey(m_tempstar);
														m_notentireGroupMap.SetAt(m_tempstar, 1);
													}
												}
											}
											m_groups.RemoveAll();
											CString strtempcommand = L"";
											strtempcommand.Format(L"insert into orika_userLoginAndGroupMapping(loginuser,[group],[select]) values('%s','%s','%d');", strloginuser, strgroup, m_int_checked);
											m_strgroupCommand = m_strgroupCommand + strtempcommand;
										}
										else
										{
											if (vargroup.HasMember("group"))
											{
												CString strgroup = L"";
												bool m_checked = false;
												int m_int_checked = 0;
												const Value& vargroupvalue = vargroup["group"];
												if (vargroupvalue.IsString())
												{
													strgroup = vargroupvalue.GetString();
												}
												CString m_OGroup = strgroup;
												CString m_parent_Group = L"";
												CString m_parent_subGroup = L"";
												while (m_OGroup.Find(L"\\") > 0)
												{
													m_parent_subGroup = m_OGroup.Mid(0, m_OGroup.Find(L"\\"));
													if (m_parent_Group == L"")
													{
														m_parent_Group = m_parent_Group + m_parent_subGroup;
													}
													else
													{
														m_parent_Group = m_parent_Group + L"\\" + m_parent_subGroup;
													}
													CString m_parent_Group_select = m_parent_Group + L"\\*";
													m_entireGroupMap.SetAt(m_parent_Group_select, 1);
													m_groups.SetAt(m_parent_Group_select, 1);
													m_OGroup = m_OGroup.Mid(m_OGroup.Find(L"\\")+1, m_OGroup.GetLength() - m_OGroup.Find(L"\\")-1);
												}
												m_parent_Group = m_parent_Group + L"\\*";
												m_groups.SetAt(m_parent_Group, 1);

												POSITION m_temp_pos = m_groups.GetStartPosition();
												while (m_temp_pos != NULL)
												{
													CString m_tempstar = L"";
													int m_check = 0;
													m_groups.GetNextAssoc(m_temp_pos, m_tempstar, m_check);
													m_entireGroupMap.RemoveKey(m_tempstar);
													m_notentireGroupMap.SetAt(m_tempstar, 1);
												}												
											}
										}

										POSITION m_mapposition = m_entireGroupMap.GetStartPosition();
										while (m_mapposition != NULL)
										{
											CString str_group = L"";
											int val = 0;
											m_entireGroupMap.GetNextAssoc(m_mapposition, str_group, val);
											CString strtempcommand = L"";
											strtempcommand.Format(L"delete from orika_userLoginAndGroupMapping where loginuser='%s' and [group]='%s';  insert into orika_userLoginAndGroupMapping(loginuser,[group],[select]) values('%s','%s','1');", strloginuser, str_group, strloginuser, str_group);
											m_strgroupCommand = m_strgroupCommand + strtempcommand;
										}
										POSITION m_mapposition_not = m_notentireGroupMap.GetStartPosition();
										while (m_mapposition_not != NULL)
										{
											CString str_group = L"";
											int val = 0;
											m_notentireGroupMap.GetNextAssoc(m_mapposition_not, str_group, val);
											CString strtempcommand = L"";
											strtempcommand.Format(L"delete from orika_userLoginAndGroupMapping where loginuser='%s' and [group]='%s';", strloginuser, str_group);
											m_strgroupCommand = m_strgroupCommand + strtempcommand;
										}

									}
								}
							}
						}
						if (vardata.HasMember("tablecolumns"))
						{
							const Value& vargroups = vardata["tablecolumns"];
							if (vargroups.IsArray())
							{
								for (SizeType i = 0; i < vargroups.Size(); i++)
								{
									const Value& vargroup = vargroups[i];
									if (vargroup.IsObject())
									{
										if (vargroup.HasMember("group") && vargroup.HasMember("checked"))
										{
											CString strgroup = L"";
											bool m_checked = false;
											int m_int_checked = 0;
											const Value& vargroupvalue = vargroup["group"];
											if (vargroupvalue.IsString())
											{
												strgroup = vargroupvalue.GetString();
												strgroup.Replace(L"_",L" ");
											}
											const Value& varchecked = vargroup["checked"];
											if (varchecked.IsBool())
											{
												m_checked = varchecked.GetBool();
												if (m_checked == true)
												{
													m_int_checked = 1;
												}
											}
											CString strtempcommand = L"";
											strtempcommand.Format(L"insert into orika_userTableColumnMapping(loginuser,[tablecolumn],[select]) values('%s','%s','%d');", strloginuser, strgroup, m_int_checked);
											m_strgroupCommand = m_strgroupCommand + strtempcommand;
										}
									}
								}
							}
						}



						if (vardata.HasMember("permission"))
						{
							const Value& varpermission = vardata["permission"];
							if (varpermission.IsArray())
							{
								for (SizeType i = 0; i < varpermission.Size(); i++)
								{
									const Value& vargroup = varpermission[i];
									if (vargroup.IsObject())
									{
										if (vargroup.HasMember("group") && vargroup.HasMember("checked"))
										{
											CString strgroup = L"";
											bool m_checked = false;
											int m_int_checked = 0;
											const Value& vargroupvalue = vargroup["group"];
											if (vargroupvalue.IsString())
											{
												strgroup = vargroupvalue.GetString();
											}
											const Value& varchecked = vargroup["checked"];
											if (varchecked.IsBool())
											{
												m_checked = varchecked.GetBool();
												if (m_checked == true)
												{
													m_int_checked = 1;
												}
											}
											CString strtempcommand = L"";
											strtempcommand.Format(L"insert into orika_userLoginAndPermission(loginuser,[accessControlName],[select]) values('%s','%s','%d');", strloginuser, strgroup, m_int_checked);
											m_strpermission = m_strpermission + strtempcommand;
										}
									}
								}
							}
						}
						if (vardata.HasMember("fingerPrint"))
						{
							const Value& varfingerPrints = vardata["fingerPrint"];
							if (varfingerPrints.IsArray())
							{
								CString m_fprints = L"";
								for (SizeType i = 0; i < varfingerPrints.Size(); i++)
								{
									const Value& varfingerPrint = varfingerPrints[i];
									if (varfingerPrint.IsString())
									{
										CString m_fprint = L"";
										m_fprint = varfingerPrint.GetString();										
										CString strtempcommand = L"";
										strtempcommand.Format(L"insert into orika_userLoginHDkeyMapping(userlogin,hdno)values('%s','%s');", strloginuser, m_fprint);
										m_strfprintcommand = m_strfprintcommand + strtempcommand;
									}
								}
							}
						}
						CString strFinalCommand = L"";
						strFinalCommand.Format(L"exec orika_saveloginuser '%s','%s','%s';delete from orika_userLoginHDkeyMapping where userlogin='%s';delete from orika_userLoginAndPermission where loginuser='%s';delete from orika_userLoginAndGroupMapping where loginuser='%s' %s %s %s", strloginuser, strpassword,strname, strloginuser, strloginuser, strloginuser, m_strfprintcommand, m_strpermission, m_strgroupCommand);
						int m_statuscode = 0;
						m_statuscode=CStaticClass::m_sqldata.executeCommandwiterrorcode(strFinalCommand);
						if (m_statuscode == 0)
						{
							CStaticClass::stloginUserDetail m_st = {};
							CMTStr::Copy(m_st.m_loginuser, strloginuser);
							CMTStr::Copy(m_st.m_name, strname);
							CStaticClass::m_loginuserlist.SetAt(strloginuser, m_st);
							CString m_silgleData = L"";
							m_silgleData.Format(L"{\"loginuser\":\"%s\",\"name\":\"%s\",\"fingerprint\":\"\",\"status\":\"\",\"ipaddress\":\"\",\"sessionid\":\"\"}", strloginuser, strname);
							CString m_rval = L"";
							m_rval.Format(L"{\"type\": \"USERS_DATA\",\"action\":\"add\",\"users\": [%s]}", m_silgleData);
							string strforsend = CT2A(m_rval.GetString());							
							SendDataToClient(client, strforsend, strKey, m_ActiveClient);
						}
						else
						{
							string strforsend = "{\"type\":\"SERVER_MESSAGE\",\"responseMessage\":\"Error while saving data.\"}";
							SendDataToClient(client, strforsend, strKey, m_ActiveClient);
						}
					}
				}				
			}
			else if (strtype == "FETCH_ALL_GROUP_DATA" && checkLoginValidate == 1)
			{
				//////(L"Enter in handle_11");
				CString strloginuser = L"";
				if (d.HasMember("loginuser"))
				{
					const Value& loginuser = d["loginuser"];
					strloginuser = loginuser.GetString();
				}
				if (d.HasMember("loginUser"))
				{
					const Value& loginuser = d["loginUser"];
					strloginuser = loginuser.GetString();
				}
				CString strjson = CStaticClass::m_sqldata.generateGroupJason(strloginuser);
				string strforsend = CT2A(strjson.GetString());
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
				//////(L"Enter in handle_12");
			}

			//generateJsonForFetchUserData

			else if (strtype == "FETCH_USER_DATA" && checkLoginValidate == 1)
			{
				if (d.HasMember("id"))
				{
					const Value& loginuser = d["id"];
					CString strloginuser = L"";
					strloginuser = loginuser.GetString();
					CString strjson = CStaticClass::m_sqldata.generateJsonForFetchUserData(strloginuser);
					string strforsend = CT2A(strjson.GetString());
					SendDataToClient(client, strforsend, strKey, m_ActiveClient);
				}
			}
			else if (strtype == "SAVE_USER_DATA" && checkLoginValidate == 1)
			{
				CString strjson = CStaticClass::m_sqldata.UpdateLoginUserCredentials(strforjson);
				string strforsend = CT2A(strjson.GetString());
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
			}
			else if (strtype == "START_GATEWAY_POSITIONDEVIDERATIO" && checkLoginValidate == 1)
			{
				CStaticClass::st_ClientContext m_st = {};				
				CStaticClass::m_mutex_ClientList.Lock();				
					CStaticClass::m_ClientContext.Lookup(strKey, m_st);
					m_st.m_start_gateway_positiondevideratio = 1;
					CStaticClass::m_ClientContext.SetAt(strKey, m_st);
					CStaticClass::m_mutex_ClientList.Unlock();
					CStaticClass m_staticclass;
					CString strjson = CStaticClass::m_mtmanager.GetGATEWAY_POSITIONDEVIDERATIO_UPDATE();
					string strforsend = CT2A(strjson.GetString());
					SendDataToClient(client, strforsend, strKey, m_ActiveClient);
				
			}
			else if (strtype == "STOP_GATEWAY_POSITIONDEVIDERATIO" && checkLoginValidate == 1)
			{
				CStaticClass::st_ClientContext m_st = {};
					CStaticClass::m_mutex_ClientList.Lock();
					CStaticClass::m_ClientContext.Lookup(strKey, m_st);
					m_st.m_start_gateway_positiondevideratio = 0;
					CStaticClass::m_ClientContext.SetAt(strKey, m_st);
				CStaticClass::m_mutex_ClientList.Unlock();
			}
			else if (strtype == "ACTIVE_COLUMNS_CHANGED" && checkLoginValidate == 1)
			{

				//(L"Enter in ACTIVE_COLUMNS_CHANGED");
				if (d.HasMember("loginUser") && d.HasMember("columns"))
				{
					CString strjson = L"";
					const Value& loginuser = d["loginUser"];
					CString m_loginUser = L"";
					if (loginuser.IsString())
					{
						m_loginUser = loginuser.GetString();
					}
					const Value& columnsArray = d["columns"];



					CString m_requesttype = L"";
					if (d.HasMember("requestType"))
					{
						const Value& requesttype = d["requestType"];
						m_requesttype = requesttype.GetString();

					}
					
					if (d.HasMember("requesttype"))
					{
						const Value& requesttype = d["requesttype"];
						m_requesttype = requesttype.GetString();						
					}
					if (m_requesttype != L"")
					{
						//CStaticClass::st_ClientContext m_st = {};
						//CStaticClass::m_mutex_ClientList.Lock();
						//CStaticClass::m_ClientContext.Lookup(strKey, m_st);
						


						//CStaticClass::m_ClientContext.SetAt(strKey, m_st);
						//CStaticClass::m_mutex_ClientList.Unlock();
					}

					if (columnsArray.IsArray())
					{
						//CStaticClass::m_sqldata.deleteColumnSubscriptionData(m_loginUser, m_requesttype);
						CStaticClass::st_ClientContext m_st = {};
						CStaticClass::m_mutex_ClientList.Lock();
						CStaticClass::m_ClientContext.Lookup(strKey, m_st);

						//First Remove Same RequestType For Update
						int data_count = m_st.m_clientrequests_List.Total();
						for (int i = data_count-1; i>=0; i--)
						{
							wchar_t  strloginCompany[250];
							CMTStr::Copy(strloginCompany, m_st.m_clientrequests_List[i]);
							CString strtemp = strloginCompany;
							CString CustomType = L"";
							CustomType.Format(L":%s:", m_requesttype);
							if (strtemp.Find(CustomType) >= 0)
							{
								m_st.m_clientrequests_List.Delete(i);
							}
						}
						int allow_columns_count = columnsArray.Size();
						for (SizeType i = 0; i < columnsArray.Size(); i++)
						{
							const Value& loginData = columnsArray[i];
							if (loginData.IsString())
							{
								CString m_column = L"";
								m_column=loginData.GetString();								
								CString userLoginColumnKey = L"";
								userLoginColumnKey.Format(L"%s:%s:%s:", m_loginUser, m_column, m_requesttype);
								wchar_t  strloginCompany[250];
								CMTStr::Copy(strloginCompany, userLoginColumnKey);								
								//Update From The Request Active column Change
								m_st.m_clientrequests_List.Add(&strloginCompany);
								//CString m_logStr = L"";
								//m_logStr.Format(L"Column Added in column List %s", userLoginColumnKey);
								//(m_logStr);
							}
						}
						CStaticClass::m_ClientContext.SetAt(strKey, m_st);
						
						CStaticClass::st_ClientContext m_st_test = {};

						CStaticClass::m_ClientContext.Lookup(strKey, m_st_test);
						int test_count = m_st.m_clientrequests_List.Total();
						for (int i = 0; i < test_count; i++)
						{
							CString  m_ColumnsData = L"";
							m_ColumnsData = m_st_test.m_clientrequests_List[i];																					
						}						
						CStaticClass::m_mutex_ClientList.Unlock();
					}										
					string strforsend = "{\"type\":\"SERVER_MESSAGE\",\"responseMessage\":\"Column Subscription Has Been Updated.\"}";
					SendDataToClient(client, strforsend, strKey, m_ActiveClient);
				}
				//(L"Exit From ACTIVE_COLUMNS_CHANGED");
			}
			else if (strtype == "ACTIVE_COLUMNS_DELETE" && checkLoginValidate == 1)
			{
			
			if (d.HasMember("loginUser") && d.HasMember("columns"))
			{
				CString strjson = L"";
				const Value& loginuser = d["loginUser"];
				CString m_loginUser = L"";
				if (loginuser.IsString())
				{
					m_loginUser = loginuser.GetString();
				}
				const Value& columnsArray = d["columns"];



				CString m_requesttype = L"";
				if (d.HasMember("requestType"))
				{
					const Value& requesttype = d["requestType"];
					m_requesttype = requesttype.GetString();

				}

				if (d.HasMember("requesttype"))
				{
					const Value& requesttype = d["requesttype"];
					m_requesttype = requesttype.GetString();
				}				
				if (columnsArray.IsArray())
				{
					//CStaticClass::m_sqldata.deleteColumnSubscriptionData(m_loginUser, m_requesttype);
					CStaticClass::st_ClientContext m_st = {};
					CStaticClass::m_mutex_ClientList.Lock();
					CStaticClass::m_ClientContext.Lookup(strKey, m_st);
					
					for (SizeType i = 0; i < columnsArray.Size(); i++)
					{
						const Value& loginData = columnsArray[i];
						if (loginData.IsString())
						{
							CString m_column = L"";
							m_column = loginData.GetString();
							CString userLoginColumnKey = L"";
							userLoginColumnKey.Format(L"%s:%s:%s:", m_loginUser, m_column, m_requesttype);
																					
							int data_count = m_st.m_clientrequests_List.Total();
							for (int i = data_count - 1; i >= 0; i--)
							{
								wchar_t  strloginCompany[250];
								CMTStr::Copy(strloginCompany, m_st.m_clientrequests_List[i]);
								CString strtemp = strloginCompany;
								CString CustomType = L"";
								CustomType.Format(L":%s:", m_requesttype);
								if (strtemp.Find(userLoginColumnKey) >= 0)
								{
									m_st.m_clientrequests_List.Delete(i);
								}
							}

						}
					}
					CStaticClass::m_ClientContext.SetAt(strKey, m_st);

					CStaticClass::m_mutex_ClientList.Unlock();
				}
				string strforsend = "{\"type\":\"SERVER_MESSAGE\",\"responseMessage\":\"Subscribed column has been deleted from Subscription list.\"}";
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
			}
			
			}
			else if (strtype == "TDH_EXECUTION" && checkLoginValidate == 1)
			{
				CString m_buySymbol=L"";
				int m_buyLogin=0;
				double m_buyRate=0;
				double m_buyLot=0;
				CString m_sellSymbol=L"";
				int m_sellLogin=0;
				double m_sellRate=0;
				double m_sellLot=0;
				CString m_executionType=L"";
				if (d.HasMember("buySymbol"))
				{
					const Value& m_buySymbol_Value = d["buySymbol"];
					if (m_buySymbol_Value.IsString())
					{
						m_buySymbol = m_buySymbol_Value.GetString();
					}
				}

				struct     TDHLogin
				{
					int m_login;
					double m_lot;
				};
				CManager::TDHLoginArray m_buyLoginArray;
				CManager::TDHLoginArray m_sellLoginArray;
				if (d.HasMember("buyLogin"))
				{
					CManager::TDHLogin m_st = {};
					const Value& m_buyLogin_Value = d["buyLogin"];
					if (m_buyLogin_Value.IsArray())
					{						
						for (SizeType i = 0; i < m_buyLogin_Value.Size(); i++)
						{
							const Value& loginData = m_buyLogin_Value[i];
							if (loginData.IsObject())
							{
								if (loginData.HasMember("login") && loginData.HasMember("lot"))
								{
									int Clientlogin = 0;
									double clientlot = 0;
									const Value& ValLogin = loginData["login"];
									if (ValLogin.IsNumber())
									{
										Clientlogin = ValLogin.GetInt();
									}

									const Value& ValLot = loginData["lot"];
									if (ValLot.IsNumber())
									{
										clientlot = ValLot.GetDouble();
									}
									m_st.m_login = Clientlogin;
									m_st.m_lot = clientlot;
									m_buyLoginArray.Add(&m_st);
								}

							}
						}												
					}
				}
				if (d.HasMember("buyRate"))
				{
					const Value& m_buyRate_Value = d["buyRate"];
					if (m_buyRate_Value.IsDouble()|| m_buyRate_Value.IsInt())
					{
						m_buyRate = m_buyRate_Value.GetDouble();
					}
				}
				
				if (d.HasMember("sellSymbol"))
				{
					const Value& m_sellSymbol_Value = d["sellSymbol"];
					if (m_sellSymbol_Value.IsString())
					{
						m_sellSymbol = m_sellSymbol_Value.GetString();
					}
				}

				if (d.HasMember("sellLogin"))
				{
					const Value& m_sellLogin_Value = d["sellLogin"];
					CManager::TDHLogin m_st = {};					
					if (m_sellLogin_Value.IsArray())
					{
						for (SizeType i = 0; i < m_sellLogin_Value.Size(); i++)
						{
							const Value& loginData = m_sellLogin_Value[i];
							if (loginData.IsObject())
							{
								if (loginData.HasMember("login") && loginData.HasMember("lot"))
								{
									int Clientlogin = 0;
									double clientlot = 0;
									const Value& ValLogin = loginData["login"];
									if (ValLogin.IsNumber())
									{
										Clientlogin = ValLogin.GetInt();
									}

									const Value& ValLot = loginData["lot"];
									if (ValLot.IsNumber())
									{
										clientlot = ValLot.GetDouble();
									}
									m_st.m_login = Clientlogin;
									m_st.m_lot = clientlot;
									m_sellLoginArray.Add(&m_st);
								}

							}
						}
					}
				}
				if (d.HasMember("sellRate"))
				{
					const Value& m_sellRate_Value = d["sellRate"];
					if (m_sellRate_Value.IsDouble()|| m_sellRate_Value.IsInt())
					{
						m_sellRate = m_sellRate_Value.GetDouble();
					}
				}
				
				if (d.HasMember("executionType"))
				{
					const Value& m_executionType_Value = d["executionType"];
					if (m_executionType_Value.IsString())
					{
						m_executionType = m_executionType_Value.GetString();
					}
				}
												
				CString strReturnMessage=L"";
				strReturnMessage=CStaticClass::m_mtmanager.TDHExecution(m_buySymbol, m_buyLoginArray, m_buyRate, m_sellSymbol, m_sellLoginArray, m_sellRate, m_executionType);
				string strforsend = CT2A(strReturnMessage.GetString());
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
			}
			else if (strtype == "DELETE_USER_DATA" && checkLoginValidate == 1)
			{
				if (d.HasMember("loginUser"))
				{
					CString m_loginUser = L"";
					const Value& loginUserVal = d["loginUser"];
					m_loginUser = loginUserVal.GetString();
					CString strSqlCommand = L"";
					strSqlCommand.Format(L"exec userloginDelete '%s';", m_loginUser);
					CStaticClass::m_sqldata.executeCommand(strSqlCommand);
					CString strFinalJson = L"";


					strFinalJson.Format(L"{\"type\":\"USERS_DATA\",\"deletekey\":[\"loginUser\"],\"delete\":[{\"loginUser\":\"%s\"}]}", m_loginUser);
					//Send Message To All Client
					//Error 
					sendDataToAllClient(strFinalJson);
					//ENd of Sending Message To all Client

					strFinalJson.Format(L"{\"type\":\"SERVER_MESSAGE\",\"responseMessage\":\"LoginUser:%s has been deleted.\"}", m_loginUser);
					string strforsend = CT2A(strFinalJson.GetString());
					SendDataToClient(client, strforsend, strKey, m_ActiveClient);
				}
			}

			else if (strtype == "ALERT_MESSAGE" && checkLoginValidate == 1)
			{
				if (d.HasMember("RCODE") )
				{
					CString m_alert_message = L"";
					CString strFinalJson = L"";
					const Value& rcodeVal = d["RCODE"];
					if (rcodeVal.IsString())
					{
						m_alert_message = rcodeVal.GetString();
						strFinalJson.Format(L"{\"type\":\"SERVER_MESSAGE\",\"responseMessage\":\"Alert From Alert Engine:%s\"}", m_alert_message);						
						sendDataToAllClient(strFinalJson);
						
					}
				}
			}



			else if (strtype == "SET_AUTH_KEY" && checkLoginValidate == 1)
			{
				if (d.HasMember("loginUser") && d.HasMember("spreadSheetId") && d.HasMember("authKey"))
				{
					CString m_loginUser = L"";
					const Value& loginUserVal = d["loginUser"];
					if (loginUserVal.IsString())
					{
						m_loginUser = loginUserVal.GetString();
					}
					CString m_spreadSheetId = L"";
					const Value& spreadSheetId = d["spreadSheetId"];
					if (spreadSheetId.IsString())
					{
						m_spreadSheetId = spreadSheetId.GetString();
					}
					CString m_authKey = L"";
					const Value& authKey = d["authKey"];
					if (authKey.IsString())
					{
						m_authKey = authKey.GetString();
					}
					CString strSqlCommand = L"";
					strSqlCommand.Format(L"delete from authKey where loginUser='%s' and spreadSheetId='%s' ;insert into authKey(loginUser,spreadSheetId,authKey)values('%s','%s','%s');", m_loginUser, m_spreadSheetId, m_loginUser, m_spreadSheetId, m_authKey);
					CStaticClass::m_sqldata.executeCommand(strSqlCommand);
					CString strFinalJson = L"";
					strFinalJson.Format(L"{\"type\":\"SERVER_MESSAGE\",\"responseMessage\":\"AuthKey for LoginUser:%s has been updated.\"}", m_loginUser);
					string strforsend = CT2A(strFinalJson.GetString());
					SendDataToClient(client, strforsend, strKey, m_ActiveClient);
				}
			}
			else if (strtype == "LOGOUT" && checkLoginValidate == 1)
			{
				if (d.HasMember("login") && d.HasMember("serialNo"))
				{
					string strforsend = "{\"type\": \"LOGOUT_STATUS\", \"status\": \"logout\"}";
					SendDataToClient(client, strforsend, strKey, m_ActiveClient);
					send_close_message_to_client(psession);
				}

			}
			else if (strtype == "LOGOUT_USER" && checkLoginValidate == 1)
			{
				
					string strforsend = "{\"type\": \"LOGOUT_STATUS\", \"status\": \"logout\"}";
					SendDataToClient(client, strforsend, strKey, m_ActiveClient);
					send_close_message_to_client(psession);
				

			}
			else if (strtype == "SUBSCRIBE_TICKER" && checkLoginValidate == 1)
			{
				if (d.HasMember("tickers"))
				{
					Value& valticker = d["tickers"];
					if (valticker.IsArray())
					{
						CStaticClass::m_mutex_ClientList.Lock();
						CStaticClass::st_ClientContext m_st = {};
						CStaticClass::m_ClientContext.Lookup(strKey, m_st);
						for (SizeType i = 0; i < valticker.Size(); i++)
						{
							Value& commentData = valticker[i];
							if (commentData.IsString())
							{
								CString strSymbolforsubscribe = L""; 
								strSymbolforsubscribe = commentData.GetString();																
								m_st.m_Subscribed_Symbol.push_back(strSymbolforsubscribe);																
							}
						}
						CStaticClass::m_ClientContext.SetAt(strKey, m_st);
						CStaticClass::m_mutex_ClientList.Unlock();





							
						for (SizeType i = 0; i < valticker.Size(); i++)
						{
							Value& commentData = valticker[i];
							if (commentData.IsString())
							{
								CString strSymbolforsubscribe = L"";
								strSymbolforsubscribe = commentData.GetString();
								int row_count = 0;
								CString tmpstr = L"";
								CString strFinalJson = L"";
								CStaticClass::m_mutex_Tick_TickSubscribe.Lock();
									CStaticClass::st_Tick m_st_Tick = {};
									CStaticClass::m_TickData.Lookup(strSymbolforsubscribe, m_st_Tick);
								CStaticClass::m_mutex_Tick_TickSubscribe.Unlock();
								CString  m_symbol = m_st_Tick.m_symbol;
								double   m_bid = m_st_Tick.m_bid;
								double   m_ask = m_st_Tick.m_ask;
								double   m_last = m_st_Tick.m_last;
								INT64    m_datetime = m_st_Tick.m_datetime;

								CString m_strTime = L"";
								CString m_tmp_date = L"";
								CMTStr256 str_time;
								SMTFormat::FormatDateTime(str_time, m_datetime, true, true);
								m_strTime = str_time.Str();
								CString strtmpdata = L"";
								strtmpdata.Format(L"{\"symbol\": \"%s\",\"bid\": %.5lf,\"ask\": %.5lf,\"last\": %.5lf,\"ticktime\": \"%s\",\"open\": %.5lf,\"high\": %.5lf,\"low\": %.5lf,\"close\": %.5lf}", m_symbol, m_bid, m_ask, m_last, m_strTime, 0.00, 0.00, 0.00, 0.00);
								strFinalJson.Format(L"{\"type\": \"TICK_DATA\",\"insert\": [%s]}", strtmpdata);
								string strforsend = CT2A(strFinalJson.GetString());
								SendDataToClient(client, strforsend, strKey, m_ActiveClient);
							}														
						}
						






					}
				}

			}
			else if (strtype == "UNSUBSCRIBE_TICKER" && checkLoginValidate == 1)
			{
				if (d.HasMember("tickers"))
				{
					Value& valticker = d["tickers"];
					if (valticker.IsArray())
					{
						CStaticClass::m_mutex_ClientList.Lock();
						CStaticClass::st_ClientContext m_st = {};
						CStaticClass::m_ClientContext.Lookup(strKey, m_st);
						for (SizeType i = 0; i < valticker.Size(); i++)
						{
							Value& commentData = valticker[i];
							if (commentData.IsString())
							{
								CString strUnSymbolforsubscribe = L"";
								strUnSymbolforsubscribe = commentData.GetString();
								for (int i = 0; i < m_st.m_Subscribed_Symbol.size(); i++)
								{
									if (m_st.m_Subscribed_Symbol[i] == strUnSymbolforsubscribe)
									{
										m_st.m_Subscribed_Symbol.erase(m_st.m_Subscribed_Symbol.begin() + i);										
										break; 
									}
								}
							}
						}
						CStaticClass::m_ClientContext.SetAt(strKey, m_st);
						CStaticClass::m_mutex_ClientList.Unlock();
					}
				}
			}
			else if ((strtype == "COMMENT_CHANGE_SAVE_DATA" || strtype == "COMMENT_CHANGE_UPDATE_DATA") && checkLoginValidate == 1)
			{
				CString strjson = CStaticClass::m_sqldata.CommentChangeSaveData(strforjson);
				string strforsend = CT2A(strjson.GetString());
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
				/*if (strjson.Find(L"Data Has Been Updated")>=0)
				{
					rapidjson::Document d_comment;
					if(!d_comment.Parse<0>(jsonString).HasParseError())
					{
						if (d_comment.HasMember("deals")==true)
						{
							Value &deals=d_comment["deals"];
							Value &reqiusetType=d_comment["type"];
							CString strreqiusetType=L"";strreqiusetType=reqiusetType.GetString();
							if (deals.IsArray())
							{
								for (SizeType i = 0; i < deals.Size(); i++)
								{
									Value  &commentData=deals[i];
									if (commentData.IsObject())
									{
										if (commentData.HasMember("deal")&&commentData.HasMember("comment")&&commentData.HasMember("commentto"))
										{
											Value &valdealno=commentData["deal"];
											Value &valcomment=commentData["comment"];
											Value &valcommentto=commentData["commentto"];
											if (valdealno.IsInt()&& valcomment.IsString()  && valcommentto.IsString())
											{
												CString tmpcommand=L"";
												INT64  intdeal=valdealno.GetInt64();
												CString strcomment=L"";
												strcomment=valcomment.GetString();
												CString strcommentto=L"";
												strcommentto=valcommentto.GetString();
												CStaticClass::st_commentChange st={};
												st.deal=intdeal;
												CMTStr::Copy(st.comment,strcomment);
												CMTStr::Copy(st.commentTo,strcommentto);
												CStaticClass::m_mutex_ClientList.Lock();
												POSITION pos = CStaticClass::m_ClientContext.GetStartPosition ();
												while (pos != NULL)
												{
													CString strclientkey=L"";
													CStaticClass::st_ClientContext m_st={};
													CStaticClass::m_ClientContext.GetNextAssoc(pos, strclientkey,m_st);
													if (m_st.m_FETCH_EXISTING_COMMENT_CHANGE_DATA_start==1)
													{
														if (strreqiusetType==L"COMMENT_CHANGE_SAVE_RESPONSE")
														{
															m_st.m_commentArray_insert.Add(&st);
														}
														if (strreqiusetType==L"COMMENT_CHANGE_UPDATE_RESPONSE")
														{
															m_st.m_commentArray_update.Add(&st);
														}
														CStaticClass::m_ClientContext.SetAt(strclientkey,m_st);
													}
												}
												CStaticClass::m_mutex_ClientList.Unlock();
											}
										}
									}
								}
							}
						}
					}


				}*/
			}




			else if (strtype == "COMMENT_CHANGE_REQUEST" && checkLoginValidate == 1)
			{
				CString strjson = CStaticClass::m_sqldata.CommentChangeToMT5(strforjson);
				string strforsend = CT2A(strjson.GetString());
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
			}
			else if (strtype == "HB_R" && checkLoginValidate == 1)
			{			
				string strforsend = "{\"type\":\"HB_S\",\"RCODE\":\"OK\"}";
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
			}
			else if (strtype.Find(L"META_DATA") >= 0 && checkLoginValidate == 1)
			{	
				//(L"Enter in Fetch Meta Data");
				CString strjson = L"";
				//FETCH_CLIENTMETA_DATA
				if (strtype == "FETCH_CLIENT_META_DATA"|| strtype == "FETCH_BROKER_META_DATA"|| strtype == "FETCH_SUBBROKER_META_DATA" || strtype == "FETCH_DEALING_META_DATA")
				{
					strjson = CStaticClass::m_sqldata.generateMetadataGlobal(strtype);
				}
				else if (strtype == "FETCH_ALL_TAB_COLUMN_META_DATA")
				{

					strjson = CStaticClass::m_sqldata.generateTabAndColumnMetadata();
				}
				else if (strtype == "FETCH_DASHBOARD_META_DATA")
				{
					strjson = CStaticClass::m_sqldata.generateDashBoardMetadata(login);
				}
				else
				{
					
					CString strLoginUser=client->key;
					strLoginUser = strLoginUser.Mid(0, strLoginUser.Find(L":"));
					strjson = CStaticClass::m_sqldata.generateMasterMetadata(strtype, strLoginUser);
				}
				string strforsend = CT2A(strjson.GetString());				
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);	


				//(L"End Of Fetching Meta Data");
			}
			/*else if (strtype.Find(L"DASHBOARD_META_DATA") >= 0 && checkLoginValidate == 1)
			{
				CString strjson = L"";			 
				strjson = CStaticClass::m_sqldata.generateTabAndColumnMetadata();						
				string strforsend = CT2A(strjson.GetString());
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
			}*/
			else if (strtype.Find(L"START_HEART_BEAT") >= 0 && checkLoginValidate == 1)
			{
				//CStaticClass::st_ClientContext m_st = {};
				//////(L"lock7");
				//CStaticClass::m_mutex_ClientList.Lock();
				//////(L"128");
				//CStaticClass::m_ClientContext.Lookup(strKey, m_st);
				//m_st.m_heartBeatStart = 1;
				//CStaticClass::m_ClientContext.SetAt(strKey, m_st);
				//CStaticClass::m_mutex_ClientList.Unlock();
				////(L"U128");
			}
			else if (strtype.Find(L"FETCH_TICK_DATA") >= 0 && checkLoginValidate == 1)
			{
				//////(L"Enter in handle_19");

				CStaticClass::st_ClientContext m_st={};
				CStaticClass::m_mutex_ClientList.Lock();
				CStaticClass::m_ClientContext.Lookup(strKey,m_st);
				m_st.m_TicksSendingStart=1;
				CStaticClass::m_ClientContext.SetAt(strKey,m_st);
				CStaticClass::m_mutex_ClientList.Unlock();
				

				CStaticClass::m_mutex_Tick_TickSubscribe.Lock();
				CStaticClass::TickData m_localdata;				
				POSITION pos1 = CStaticClass::m_TickData.GetStartPosition();
				while (pos1 != NULL)
				{
					CString  iKey = L"";
					CStaticClass::st_Tick m_st_Tick = {};
					CStaticClass::m_TickData.GetNextAssoc(pos1, iKey, m_st_Tick);
					m_localdata.SetAt(iKey, m_st_Tick);
				}				
				CStaticClass::m_mutex_Tick_TickSubscribe.Unlock();
				
				POSITION pos = m_localdata.GetStartPosition ();
				int row_count=0;
				CString tmpstr=L"";
				CString strFinalJson=L"";
				while (pos != NULL)
				{
					CString  iKey=L"";
					CStaticClass::st_Tick m_st_Tick={};
					m_localdata.GetNextAssoc(pos, iKey,m_st_Tick);
					CString  m_symbol=m_st_Tick.m_symbol ;
					double   m_bid=m_st_Tick.m_bid ;
					double   m_ask=m_st_Tick.m_ask ;
					double   m_last=m_st_Tick.m_last ;
					INT64    m_datetime=m_st_Tick.m_datetime ;

					CString m_strTime=L"";
					CString m_tmp_date=L"";
					CMTStr256 str_time;
					SMTFormat::FormatDateTime(str_time,m_datetime,true,true);
					m_strTime=str_time.Str();


					CString strtmpdata = L"";
					for (int i = 0; i < m_st.m_Subscribed_Symbol.size(); i++)
					{
						if (m_st.m_Subscribed_Symbol[i] == m_symbol)
						{
							strtmpdata.Format(L"{\"symbol\": \"%s\",\"bid\": %.5lf,\"ask\": %.5lf,\"last\": %.5lf,\"ticktime\": \"%s\",\"open\": %.5lf,\"high\": %.5lf,\"low\": %.5lf,\"close\": %.5lf}", m_symbol, m_bid, m_ask, m_last, m_strTime, 0.00, 0.00, 0.00, 0.00);

							if (row_count != 0)
							{
								tmpstr = tmpstr + L"," + strtmpdata;
							}
							else
							{
								tmpstr = tmpstr + strtmpdata;
							}
							row_count = row_count + 1;

							break;
						}
					}

									
					
				}
				if (tmpstr.Trim() != L"")
				{
					strFinalJson.Format(L"{\"type\": \"TICK_DATA\",\"insert\": [%s]}", tmpstr);
					string strforsend = CT2A(strFinalJson.GetString());
					SendDataToClient(client, strforsend, strKey, m_ActiveClient);
				}				
			}
			else if (strtype == "ORDER_ACTIVATE_REQUEST" && checkLoginValidate == 1)
			{
				CString tmpstr = L"";
				CString strFinalJson = L"";
				CString strOrderStatusJason = L"";
				int i = 0;

					CStaticClass::m_mutex_order.Lock();
					POSITION p = CStaticClass::m_Orika_orderHastable.GetStartPosition();
					while (p != NULL)
					{
						CStaticClass::st_order st = {};
						int m_orderno = 0;
						CStaticClass::m_Orika_orderHastable.GetNextAssoc(p, m_orderno, st);
						if (st.m_select == 1)
						{
							int orderActivateStatus = 0;
						orderActivateStatus = CStaticClass::m_mtmanager.Activate_Order(m_orderno);
						CString orderStatus = L"";
						if (orderActivateStatus == 1)
						{
							orderStatus = L"activated";
							tmpstr.Format(L"{\"order\":%d,\"mtMessage\":\"%s\"}", m_orderno, orderStatus);
							if (i != 0)
							{
								strOrderStatusJason = strOrderStatusJason + L"," + tmpstr;
							}
							else
							{
								strOrderStatusJason = tmpstr;
							}




							UINT64    m_order = m_orderno;
							UINT      m_type = 0;
							UINT64    m_Ordertime = 0;
							CString   m_Symbol = L"";
							double    m_price_order = 0;
							UINT64    m_time_exp = 0;
							UINT64    m_order_done_time = 0;
							UINT      m_state = 0;
							CString   m_External_ID = L"";
							UINT      m_contract_Size = 0;
							CString   m_comment = L"";
							UINT64    m_volume = 0;
							UINT64    m_login = 0;

							CString str_msmq = L"";
							str_msmq.Format(L"{\"MessageType\":1003,\"Data\":{\"login\":%I64u,\"order\":%I64u,\"type\":%u,\"Ordertime\":%I64u,\"Symbol\":\"%s\",\"price_order\":%.4lf,\"time_exp\":%I64u,\"order_done_time\":%I64u,\"state\":%u,\"External_ID\":\"%s\",\"contract_Size\":%u,\"comment\":\"%s\",\"volume\":%I64u}}", m_login, m_order, m_type, m_Ordertime, m_Symbol, m_price_order, m_time_exp, m_order_done_time, m_state, m_External_ID, m_contract_Size, m_comment, m_volume);
							CMSMQApiWrapper  m_CMSMQApiWrapper;
							CString szLabel, szMessage, szOutput;
							szLabel = L"OrderDelete";
							_bstr_t temp_bst = str_msmq;
							char* strforwrite;
							strforwrite = temp_bst;
							m_CMSMQApiWrapper.SendStringMessage(CStaticClass::MSMQQueuName, szLabel, strforwrite);
						}
						CStaticClass::m_mutex_order.Unlock();
					}
					
					}
					if (strOrderStatusJason.Trim() != L"")
					{
						strFinalJson.Format(L"{\"type\":\"ORDER_ACTIVATE_RESPONSE\",\"responseMessage\":[%s]}", strOrderStatusJason);
						string strforsend = CT2A(strFinalJson.GetString());
						SendDataToClient(client, strforsend, strKey, m_ActiveClient);
					}
				
			}
			else if (strtype == "ORDER_UPDATE_REQUEST" && checkLoginValidate == 1)
			{
				rapidjson::Document orderObject;
				const char* strforjson = str.c_str();
				CString struser = L"";
				CString strpwd = L"";
				CString strkey = L"";
				if (!orderObject.Parse<0>(strforjson).HasParseError())
				{
					if (orderObject.HasMember("key") == true)
					{
						const Value& Key = d["key"];
						strkey = Key.GetString();
						//{"type":"ORDER_UPDATE_REQUEST","key":"select","select":[{"order":1024619,"dataType":"boolean","value":true}]}
						if (strkey == L"select")
						{
							if (orderObject.HasMember("select") == true)
							{
								const Value& obj_select = orderObject["select"];

								if (obj_select.IsArray())
								{
									for (SizeType i = 0; i < obj_select.Size(); i++)
									{
										const  Value& obj_selectItem = obj_select[i];
										if (obj_selectItem.IsObject())
										{
											if (obj_selectItem.HasMember("order") && obj_selectItem.HasMember("value") && obj_selectItem["order"].IsInt() && obj_selectItem["value"].IsBool())
											{
												const Value& val_order = obj_selectItem["order"];
												INT64 m_order = val_order.GetInt64();
												CStaticClass::st_order_ForUpdate m_st_updateOrder = {};
												m_st_updateOrder.m_order = m_order;
												if (obj_selectItem["value"].GetBool() == true)
												{
													m_st_updateOrder.m_select = 1;
												}
												m_st_updateOrder.m_select_update = 1;
												////(L"lock8");
												CStaticClass::m_mutex_ClientList.Lock();
												//(L"22");
												POSITION pos = CStaticClass::m_ClientContext.GetStartPosition();
												while (pos != NULL)
												{
													CString strclientkey = L"";
													CStaticClass::st_ClientContext m_st = {};
													CStaticClass::m_ClientContext.GetNextAssoc(pos, strclientkey, m_st);
													if (m_st.m_FETCH_ORDER_DATA_start == 1)
													{
														m_st.m_orderarray_ForEdit.Add(&m_st_updateOrder);
														CStaticClass::m_ClientContext.SetAt(strclientkey, m_st);
													}
												}
												CStaticClass::m_mutex_ClientList.Unlock();
												//(L"U22");
											}
										}
									}
								}
							}
						}
						//{"type":"ORDER_UPDATE_REQUEST","key":"","statustype":[{"order":1024628,"dataType":"string","value":"ORDER"}]}
						if (strkey == L"statustype")
						{
							if (orderObject.HasMember("statustype") == true)
							{
								const Value& obj_statustypeMain = orderObject["statustype"];
								if (obj_statustypeMain.IsArray())
								{
									for (SizeType i = 0; i < obj_statustypeMain.Size(); i++)
									{
										const Value& obj_statustype = obj_statustypeMain[i];
										if (obj_statustype.IsObject())
										{
											if (obj_statustype.HasMember("order") && obj_statustype.HasMember("value") && obj_statustype["order"].IsInt() && obj_statustype["value"].IsString())
											{
												const Value& val_order = obj_statustype["order"];
												INT64 m_order = val_order.GetInt64();

												const Value& val_Value = obj_statustype["value"];
												CString strValue = L"";
												strValue = val_Value.GetString();
												CStaticClass::st_order_ForUpdate m_st_updateOrder = {};
												m_st_updateOrder.m_order = m_order;
												CMTStr::Copy(m_st_updateOrder.m_selecttype, strValue);
												m_st_updateOrder.m_selecttype_update = 1;
												////(L"lock9");
												CStaticClass::m_mutex_ClientList.Lock();
												//(L"23");
												POSITION pos = CStaticClass::m_ClientContext.GetStartPosition();
												while (pos != NULL)
												{
													CString strclientkey = L"";
													CStaticClass::st_ClientContext m_st = {};
													CStaticClass::m_ClientContext.GetNextAssoc(pos, strclientkey, m_st);
													if (m_st.m_FETCH_ORDER_DATA_start == 1)
													{
														m_st.m_orderarray_ForEdit.Add(&m_st_updateOrder);
														CStaticClass::m_ClientContext.SetAt(strclientkey, m_st);
													}
												}
												CStaticClass::m_mutex_ClientList.Unlock();
												//(L"U23");
											}
										}
									}
								}
							}
						}
						//{"type":"ORDER_UPDATE_REQUEST","key":"","subtype":[{"order":1024628,"dataType":"string","value":"PBDU"}]}
						if (strkey == L"subtype")
						{
							if (orderObject.HasMember("subtype") == true)
							{
								const Value& obj_statustypeMain = orderObject["subtype"];
								if (obj_statustypeMain.IsArray())
								{
									for (SizeType i = 0; i < obj_statustypeMain.Size(); i++)
									{
										const Value& obj_statustype = obj_statustypeMain[i];
										if (obj_statustype.IsObject())
										{
											if (obj_statustype.HasMember("order") && obj_statustype.HasMember("value") && obj_statustype["order"].IsInt() && obj_statustype["value"].IsString())
											{
												const Value& val_order = obj_statustype["order"];
												INT64 m_order = val_order.GetInt64();

												const Value& val_Value = obj_statustype["value"];
												CString strValue = L"";
												strValue = val_Value.GetString();
												CStaticClass::st_order_ForUpdate m_st_updateOrder = {};
												m_st_updateOrder.m_order = m_order;
												CMTStr::Copy(m_st_updateOrder.m_subtype, strValue);
												m_st_updateOrder.m_subtype_update = 1;
												////(L"lock10");
												CStaticClass::m_mutex_ClientList.Lock();
												//(L"24");
												POSITION pos = CStaticClass::m_ClientContext.GetStartPosition();
												while (pos != NULL)
												{
													CString strclientkey = L"";
													CStaticClass::st_ClientContext m_st = {};
													CStaticClass::m_ClientContext.GetNextAssoc(pos, strclientkey, m_st);
													if (m_st.m_FETCH_ORDER_DATA_start == 1)
													{
														m_st.m_orderarray_ForEdit.Add(&m_st_updateOrder);
														CStaticClass::m_ClientContext.SetAt(strclientkey, m_st);
													}
												}
												CStaticClass::m_mutex_ClientList.Unlock();
												//(L"U24");
											}
										}
									}
								}
							}
						}
						//{"type":"ORDER_UPDATE_REQUEST","key":"","contraorder":[{"order":1024628,"dataType":"int","value":"10"}]}
						if (strkey == L"contraorder")
						{
							if (orderObject.HasMember("contraorder") == true)
							{
								const Value& obj_statustypeMain = orderObject["contraorder"];
								if (obj_statustypeMain.IsArray())
								{
									for (SizeType i = 0; i < obj_statustypeMain.Size(); i++)
									{
										const Value& obj_statustype = obj_statustypeMain[i];
										if (obj_statustype.IsObject())
										{
											if (obj_statustype.HasMember("order") && obj_statustype.HasMember("value") && obj_statustype["order"].IsInt() && obj_statustype["value"].IsString())
											{
												const Value& val_order = obj_statustype["order"];
												INT64 m_order = val_order.GetInt64();

												const Value& val_Value = obj_statustype["value"];
												CString strValue = L"";
												strValue = val_Value.GetString();
												CStaticClass::st_order_ForUpdate m_st_updateOrder = {};
												m_st_updateOrder.m_order = m_order;
												m_st_updateOrder.m_contraorder = _wtoi64(strValue);
												m_st_updateOrder.m_contraorder_updtae = 1;
												////(L"lock11");
												CStaticClass::m_mutex_ClientList.Lock();
												//(L"25");
												POSITION pos = CStaticClass::m_ClientContext.GetStartPosition();
												while (pos != NULL)
												{
													CString strclientkey = L"";
													CStaticClass::st_ClientContext m_st = {};
													CStaticClass::m_ClientContext.GetNextAssoc(pos, strclientkey, m_st);
													if (m_st.m_FETCH_ORDER_DATA_start == 1)
													{
														m_st.m_orderarray_ForEdit.Add(&m_st_updateOrder);
														CStaticClass::m_ClientContext.SetAt(strclientkey, m_st);
													}
												}
												CStaticClass::m_mutex_ClientList.Unlock();
												//(L"U25");
											}
										}
									}
								}
							}
						}
						//{"type":"ORDER_UPDATE_REQUEST","key":"","tradeexecutetime":[{"order":1024628,"dataType":"int","value":1569954600}]}
						if (strkey == L"tradeexecutetime")
						{
							if (orderObject.HasMember("tradeexecutetime") == true)
							{
								const Value& obj_statustypeMain = orderObject["tradeexecutetime"];
								if (obj_statustypeMain.IsArray())
								{
									for (SizeType i = 0; i < obj_statustypeMain.Size(); i++)
									{
										const Value& obj_statustype = obj_statustypeMain[i];
										if (obj_statustype.IsObject())
										{
											if (obj_statustype.HasMember("order") && obj_statustype.HasMember("value") && obj_statustype["order"].IsInt() && obj_statustype["value"].IsInt())
											{
												const Value& val_order = obj_statustype["order"];
												INT64 m_order = val_order.GetInt64();

												const Value& val_Value = obj_statustype["value"];
												INT64  intValue = 0;
												intValue = val_Value.GetInt64();
												CStaticClass::st_order_ForUpdate m_st_updateOrder = {};
												m_st_updateOrder.m_order = m_order;
												m_st_updateOrder.m_tradeexecutetime = intValue;
												m_st_updateOrder.m_tradeexecutetime_update = 1;
												////(L"lock12");
												CStaticClass::m_mutex_ClientList.Lock();
												//(L"26");
												POSITION pos = CStaticClass::m_ClientContext.GetStartPosition();
												while (pos != NULL)
												{
													CString strclientkey = L"";
													CStaticClass::st_ClientContext m_st = {};
													CStaticClass::m_ClientContext.GetNextAssoc(pos, strclientkey, m_st);
													if (m_st.m_FETCH_ORDER_DATA_start == 1)
													{
														m_st.m_orderarray_ForEdit.Add(&m_st_updateOrder);
														CStaticClass::m_ClientContext.SetAt(strclientkey, m_st);
													}
												}
												CStaticClass::m_mutex_ClientList.Unlock();
												//(L"U26");
											}
										}
									}
								}
							}
						}
						//{"type":"ORDER_UPDATE_REQUEST","key":"","ourcomment":[{"order":1024628,"dataType":"string","value":"200"}]}
						if (strkey == L"ourcomment")
						{
							if (orderObject.HasMember("ourcomment") == true)
							{
								const Value& obj_statustypeMain = orderObject["ourcomment"];
								if (obj_statustypeMain.IsArray())
								{
									for (SizeType i = 0; i < obj_statustypeMain.Size(); i++)
									{
										const Value& obj_statustype = obj_statustypeMain[i];
										if (obj_statustype.IsObject())
										{
											if (obj_statustype.HasMember("order") && obj_statustype.HasMember("value") && obj_statustype["order"].IsInt() && obj_statustype["value"].IsString())
											{
												const Value& val_order = obj_statustype["order"];
												INT64 m_order = val_order.GetInt64();

												const Value& val_Value = obj_statustype["value"];
												CString   strValue = L"";
												strValue = val_Value.GetString();
												CStaticClass::st_order_ForUpdate m_st_updateOrder = {};
												m_st_updateOrder.m_order = m_order;
												CMTStr::Copy(m_st_updateOrder.m_ourcomment, strValue);
												m_st_updateOrder.m_ourcomment_update = 1;
												////(L"lock13");
												CStaticClass::m_mutex_ClientList.Lock();
												//(L"27");
												POSITION pos = CStaticClass::m_ClientContext.GetStartPosition();
												while (pos != NULL)
												{
													CString strclientkey = L"";
													CStaticClass::st_ClientContext m_st = {};
													CStaticClass::m_ClientContext.GetNextAssoc(pos, strclientkey, m_st);
													if (m_st.m_FETCH_ORDER_DATA_start == 1)
													{
														m_st.m_orderarray_ForEdit.Add(&m_st_updateOrder);
														CStaticClass::m_ClientContext.SetAt(strclientkey, m_st);
													}
												}
												CStaticClass::m_mutex_ClientList.Unlock();
												//(L"U27");
											}
										}
									}
								}
							}
						}
					}
				}
				CStaticClass::m_sqldata.orderUpdate(strforjson);
			}
			else if (strtype == "FETCH_LOGIN_AND_SYMBOL" && checkLoginValidate == 1)
			{
				CStaticClass::m_mutex_Tick.Lock();
				StringBuffer s;
				Writer<StringBuffer> writer(s);
				writer.StartObject();
				writer.Key("type");
				writer.String("CLIENT_AND_SYMBOL");
				writer.Key("logins");
				writer.StartArray();
				//	CStaticClass::m_Orika_clientmasterHastable
					POSITION pos = CStaticClass::m_Orika_clientmasterHastable.GetStartPosition();
					while (pos != NULL)
					{
						writer.StartObject();
						CString strclientkey = L"";
						CStaticClass::st_Orika_clientmaster m_st = {};
						CStaticClass::m_Orika_clientmasterHastable.GetNextAssoc(pos, strclientkey, m_st);
						CString m_login = m_st.m_login;
						string strLogin = CT2A(m_login);
						const char* login = strLogin.c_str();
						writer.Key("login");
						writer.String(login);
						writer.Key("name");
						CString m_name = m_st.m_name;
						string strname = CT2A(m_name);
						const char* name = strname.c_str();
						writer.String(name);
						writer.EndObject();
					}
					
				writer.EndArray();
				writer.Key("symbols");
				writer.StartArray();
				//CStaticClass::m_Orika_symbolmasterHastable

				CSqlData::columnArray  tmparray; 
				CStaticClass::m_sqldata.getStringColumnList(L"exec TDHSYMBOL",&tmparray);
				int totalDtataCount = tmparray.Total();
				for (int i = 0; i < totalDtataCount; i++)
				{
					CString m_symbol;
					m_symbol = tmparray[i];
					string strSymbol = CT2A(m_symbol);
					const char* symbol = strSymbol.c_str();
					writer.String(symbol);
				}
				writer.EndArray();
				writer.EndObject();


				CString strFinalJson = L"";
				strFinalJson = s.GetString();
				string strforsend = CT2A(strFinalJson.GetString());
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
				s.Clear();
				writer.Flush();
				CStaticClass::m_mutex_Tick.Unlock();
			}

			else if (strtype == "FETCH_ALL_LOGIN_AND_SYMBOL" && checkLoginValidate == 1)
			{
				CStaticClass::m_mutex_Tick.Lock();
				StringBuffer s;
				Writer<StringBuffer> writer(s);
				writer.StartObject();
				writer.Key("type");
				writer.String("CLIENT_AND_SYMBOL");
				writer.Key("logins");
				writer.StartArray();
				//	CStaticClass::m_Orika_clientmasterHastable
				POSITION pos = CStaticClass::m_Orika_clientmasterHastable.GetStartPosition();
				while (pos != NULL)
				{
					writer.StartObject();
					CString strclientkey = L"";
					CStaticClass::st_Orika_clientmaster m_st = {};
					CStaticClass::m_Orika_clientmasterHastable.GetNextAssoc(pos, strclientkey, m_st);
					CString m_login = m_st.m_login;
					string strLogin = CT2A(m_login);
					const char* login = strLogin.c_str();
					writer.Key("login");
					writer.String(login);
					writer.Key("name");
					CString m_name = m_st.m_name;
					string strname = CT2A(m_name);
					const char* name = strname.c_str();
					writer.String(name);
					writer.EndObject();
				}

				writer.EndArray();
				writer.Key("symbols");
				writer.StartArray();
				//CStaticClass::m_Orika_symbolmasterHastable

				CSqlData::columnArray  tmparray;
				CStaticClass::m_sqldata.getStringColumnList(L"exec allsymbol", &tmparray);
				int totalDtataCount = tmparray.Total();
				for (int i = 0; i < totalDtataCount; i++)
				{
					CString m_symbol;
					m_symbol = tmparray[i];
					string strSymbol = CT2A(m_symbol);
					const char* symbol = strSymbol.c_str();
					writer.String(symbol);
				}
				writer.EndArray();
				writer.EndObject();


				CString strFinalJson = L"";
				strFinalJson = s.GetString();
				string strforsend = CT2A(strFinalJson.GetString());
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
				s.Clear();
				writer.Flush();
				CStaticClass::m_mutex_Tick.Unlock();
			}
			else if (strtype == "FETCH_ALL_LOGIN_AND_SYMBOL_TDH" && checkLoginValidate == 1)
			{
				CStaticClass::m_mutex_Tick.Lock();
				StringBuffer s;
				Writer<StringBuffer> writer(s);
				writer.StartObject();
				writer.Key("type");
				writer.String("CLIENT_AND_SYMBOL");
				writer.Key("logins");
				writer.StartArray();
				//	CStaticClass::m_Orika_clientmasterHastable
				POSITION pos = CStaticClass::m_Orika_clientmasterHastable.GetStartPosition();
				while (pos != NULL)
				{
					writer.StartObject();
					CString strclientkey = L"";
					CStaticClass::st_Orika_clientmaster m_st = {};
					CStaticClass::m_Orika_clientmasterHastable.GetNextAssoc(pos, strclientkey, m_st);
					CString m_login = m_st.m_login;
					string strLogin = CT2A(m_login);
					const char* login = strLogin.c_str();
					writer.Key("login");
					writer.String(login);
					writer.Key("name");
					CString m_name = m_st.m_name;
					string strname = CT2A(m_name);
					const char* name = strname.c_str();
					writer.String(name);
					writer.EndObject();
				}

				writer.EndArray();
				writer.Key("symbols");
				writer.StartArray();
				//CStaticClass::m_Orika_symbolmasterHastable

				CSqlData::columnArray  tmparray;
				CStaticClass::m_sqldata.getStringColumnList(L"exec TDHSYMBOL", &tmparray);
				int totalDtataCount = tmparray.Total();
				for (int i = 0; i < totalDtataCount; i++)
				{
					CString m_symbol;
					m_symbol = tmparray[i];
					string strSymbol = CT2A(m_symbol);
					const char* symbol = strSymbol.c_str();
					writer.String(symbol);
				}
				writer.EndArray();
				writer.EndObject();


				CString strFinalJson = L"";
				strFinalJson = s.GetString();
				string strforsend = CT2A(strFinalJson.GetString());
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
				s.Clear();
				writer.Flush();
				CStaticClass::m_mutex_Tick.Unlock();
				}

			else if (strtype == "VERIFY_POSITION" && checkLoginValidate == 1)
			{
				CString FilePath = L"";
				CString ReturnVal = L"";
				if (d.HasMember("filename"))
				{
					const Value& valPath = d["filename"];
					if (valPath.IsString())
					{
						FilePath = valPath.GetString();
					}
				}
				//PositionFileReadAndCheck
				CString strFileFullPath = L"";
				strFileFullPath = CStaticClass::APIFolderPath + FilePath;
				if (FilePath.Trim() != L"")
				{
					ReturnVal = CStaticClass::m_sqldata.PositionFileReadAndCheck(strFileFullPath);

					string strforsend = CT2A(ReturnVal.GetString());
					SendDataToClient(client, strforsend, strKey, m_ActiveClient);
				}
			}

			else if (strtype == "MTPOSITION_FILE" && checkLoginValidate == 1)
			{
				CString FilePath = L"";
				CString ReturnVal = L"";
				if (d.HasMember("filename"))
				{
					const Value& valPath = d["filename"];
					if (valPath.IsString())
					{
						FilePath = valPath.GetString();
					}
				}
				//PositionFileReadAndCheck
				CString strFileFullPath = L"";
				strFileFullPath = CStaticClass::APIFolderPath + FilePath;
				if (FilePath.Trim() != L"")
				{
					ReturnVal = CStaticClass::m_sqldata.PositionFileReadAndCheck(strFileFullPath);

					string strforsend = CT2A(ReturnVal.GetString());
					SendDataToClient(client, strforsend, strKey, m_ActiveClient);
				}
			}
			else if (strtype == "DELETE_DATA_REQUEST" && checkLoginValidate == 1)
			{
				UINT64 m_time=0;
				CStaticClass m_staticClass;
				m_time = m_staticClass.getcurrentTime_Unix();
				SYSTEMTIME readable_date;
				readable_date = SMTTime::TimeToST(m_time, readable_date);
				int int_dayofweek = readable_date.wDayOfWeek;
				if (int_dayofweek == 0 || int_dayofweek == 6)
				{
					CString FilePath = L"";
					CString ReturnVal = L"";
					if (d.HasMember("groups"))
					{
						const Value& valgroupPath = d["groups"];
						std::string strjson = JsonAsString(valgroupPath);
						CString str_group = L"";
						str_group = strjson.c_str();
						CStaticClass::m_mtmanager.DeleteMTData(str_group);
					}
					StringBuffer s;
					Writer<StringBuffer> writer(s);
					writer.StartObject();
					writer.Key("type");
					writer.String("DATA_DELETE_RESPONSE");
					writer.Key("message");
					writer.String("All Data Has Benn Deteted");
					writer.EndObject();

					CString str_FinalJsonUpdate = L"";
					str_FinalJsonUpdate = s.GetString();
					string strforsend = CT2A(str_FinalJsonUpdate.GetString());
					s.Clear();
					writer.Flush();
					SendDataToClient(client, strforsend, strKey, m_ActiveClient);
				}
			}			
			else if (strtype == "BALANCE_TRANSFER_REQUEST" && checkLoginValidate == 1)
			{
				UINT64 m_time = 0;
				CStaticClass m_staticClass;
				m_time = m_staticClass.getcurrentTime_Unix();
				SYSTEMTIME readable_date;
				readable_date = SMTTime::TimeToST(m_time, readable_date);
				int int_dayofweek = readable_date.wDayOfWeek;
				if (int_dayofweek == 0 || int_dayofweek == 6)
				{
					std::string strjson = "";
					CString FilePath = L"";
					CString ReturnVal = L"";
					int TotalClientCount = 0;
					if (d.HasMember("groups"))
					{
						const Value& valgroupPath = d["groups"];
						strjson = JsonAsString(valgroupPath);
						TotalClientCount = CStaticClass::m_mtmanager.BalanceTransferdataCount(strjson);
					}
					StringBuffer s;
					Writer<StringBuffer> writer(s);
					writer.StartObject();
					writer.Key("type");
					writer.String("TOTAL_CLIENT_FOR_BALANCE_TRANSFER");
					writer.Key("TOTAL_CLIENT");
					writer.Int(TotalClientCount);
					writer.EndObject();
					CString str_FinalJsonUpdate = L"";
					str_FinalJsonUpdate = s.GetString();
					string strforsend = CT2A(str_FinalJsonUpdate.GetString());
					s.Clear();
					writer.Flush();
					SendDataToClient(client, strforsend, strKey, m_ActiveClient);
					CStaticClass::m_mtmanager.BalanceTransfer(TotalClientCount, strjson);

					POSITION pos = CStaticClass::m_ClientBalance.GetStartPosition();
					int pendingBalance = TotalClientCount;
					int TransferedBalance = 0;
					while (pos != NULL)
					{
						double dbcredit = 0;
						int MTlogins = 0;
						CStaticClass::m_ClientBalance.GetNextAssoc(pos, MTlogins, dbcredit);
						if (dbcredit > 0)
						{
							CStaticClass::m_mtmanager.DealerBalance(MTlogins, dbcredit, 3, L"Credit", true);
						}
						TransferedBalance = TransferedBalance + 1;
						pendingBalance = pendingBalance - 1;
						StringBuffer s;
						Writer<StringBuffer> writer(s);
						writer.StartObject();
						writer.Key("type");
						writer.String("BALANCE_TRANSFER_STATUS");
						writer.Key("TRANSFERED");
						writer.Int(TransferedBalance);
						writer.Key("PENDING");
						writer.Int(pendingBalance);
						writer.EndObject();
						CString str_FinalJsonUpdate = L"";
						str_FinalJsonUpdate = s.GetString();
						string strforsend = CT2A(str_FinalJsonUpdate.GetString());
						s.Clear();
						writer.Flush();
						SendDataToClient(client, strforsend, strKey, m_ActiveClient);

					}

					StringBuffer s1;
					Writer<StringBuffer> writer1(s1);
					writer1.StartObject();
					writer1.Key("type");
					writer1.String("BALANCE_TRANSFER_RESPONSE");
					writer1.Key("message");
					writer1.String("Balance of all clients has been transfered.");
					writer1.EndObject();

					str_FinalJsonUpdate = L"";
					str_FinalJsonUpdate = s1.GetString();
					strforsend = CT2A(str_FinalJsonUpdate.GetString());
					s1.Clear();
					writer1.Flush();
					SendDataToClient(client, strforsend, strKey, m_ActiveClient);
				}
			}
			else if (strtype == "TRADE_DATE_CHANGE" && checkLoginValidate == 1)
			{
				CString strFinalJson = L"";
				strFinalJson.Format(L"{\"type\":\"SERVER_MESSAGE\",\"responseMessage\":\"Date has been sucessfully changed\"}");
				string strforsend = CT2A(strFinalJson.GetString());
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
			}
			else if (strtype == "DEAL_TRANSFER_REQUEST" && checkLoginValidate == 1)
			{			
				CString FilePath = L"";
				CString ReturnVal = L"";
				if (d.HasMember("filename"))
				{
					const Value& valPath = d["filename"];
					if (valPath.IsString())
					{
						FilePath = valPath.GetString();
					}
				}
				string strjson = "";
				if (d.HasMember("groups"))
				{
					const Value& valgroupPath = d["groups"];
					strjson = JsonAsString(valgroupPath);					
				}
				CString strFileFullPath = L"";
				strFileFullPath = CStaticClass::APIFolderPath + FilePath;

				int total_position = 0;
				if (FilePath.Trim() != L"")
				{
					total_position = CStaticClass::m_sqldata.positionCount(strjson, strFileFullPath);

				}				
				StringBuffer s;
				Writer<StringBuffer> writer(s);
				writer.StartObject();
				writer.Key("type");
				writer.String("TOTAL_DEAL_FOR_TRANSFER");
				writer.Key("TOTAL_DEAL");
				writer.Int(total_position);
				writer.EndObject();
				CString str_FinalJsonUpdate = L"";
				str_FinalJsonUpdate = s.GetString();
				string strforsend = CT2A(str_FinalJsonUpdate.GetString());
				s.Clear();
				writer.Flush();
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);


				CStaticClass::m_mtmanager.PositionTransferToMT(strjson, strFileFullPath, total_position);


				StringBuffer s1;
				Writer<StringBuffer> writer1(s1);
				writer1.StartObject();
				writer1.Key("type");
				writer1.String("POSITION_TRANSFER_RESPONSE");
				writer1.Key("message");
				writer1.String("Position of all clients has been transfered.");
				writer1.EndObject();
				str_FinalJsonUpdate = L"";
				str_FinalJsonUpdate = s1.GetString();
				strforsend = CT2A(str_FinalJsonUpdate.GetString());
				s1.Clear();
				writer1.Flush();
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
			}
			else if (strtype == "SAVE_VIEW_STATE" && checkLoginValidate == 1)
			{
				CString strSqlCommand = L"";
				CString m_viewName = L"";
				int m_viewno = 0;
				CString m_userlogin = login;
				CString m_metadatatype = L"";
				CString m_filterState = L"";
				CString m_sortingState = L"";
				int m_isDarkMode = 0;
				int m_isFilterOn = 0;
				int m_isColorOn = 0;
				int m_isPivot = 0;
				if (d.HasMember("viewName") == true)
				{
					const Value& viewnameValue = d["viewName"];
					if (viewnameValue.IsString() == true)
					{
						m_viewName = viewnameValue.GetString();
					}
				}
				if (d.HasMember("viewNo") == true)
				{
					const Value& viewnoValue = d["viewNo"];
					if (viewnoValue.IsInt() == true)
					{
						m_viewno = viewnoValue.GetInt();
					}
				}				
				if (d.HasMember("metadatatype") == true)
				{
					const Value& metadatatypeValue = d["metadatatype"];
					if (metadatatypeValue.IsString() == true)
					{
						m_metadatatype = metadatatypeValue.GetString();
					}
				}

				if (d.HasMember("isDarkMode") == true)
				{
					const Value& isDarkModeValue = d["isDarkMode"];
					boolean m_booldarkmode = false;
					if (isDarkModeValue.IsBool() == true)
					{
						m_booldarkmode = isDarkModeValue.GetBool();
					}
					m_isDarkMode = m_booldarkmode == true ? 1 : 0;
				}
				if (d.HasMember("isFilterOn") == true)
				{
					const Value& isFilterOnValue = d["isFilterOn"];
					boolean m_filterOn = false;
					if (isFilterOnValue.IsBool() == true)
					{
						m_filterOn = isFilterOnValue.GetBool();
					}
					m_isFilterOn = m_filterOn == true ? 1 : 0;
				}
				if (d.HasMember("isColorOn") == true)
				{
					const Value& isColorOnValue = d["isColorOn"];
					boolean m_colorOn = false; 
					if (isColorOnValue.IsBool() == true)
					{
						m_colorOn = isColorOnValue.GetBool();
					}
					m_isColorOn = m_colorOn == true ? 1 : 0;
				}
				if (d.HasMember("isPivot") == true)
				{
					const Value& isPivotValue = d["isPivot"];
					boolean m_PivotOn = false;
					if (isPivotValue.IsBool() == true)
					{
						m_PivotOn = isPivotValue.GetBool();
					}
					m_isPivot = m_PivotOn == true ? 1 : 0;
				}
				if (d.HasMember("filterState") == true)
				{
					const Value& filterstateValue = d["filterState"];					
					updateFilterState(filterstateValue, m_viewno, m_metadatatype, m_userlogin);
				}
				if (d.HasMember("sortingState") == true)
				{
					const Value& sortingStateValue = d["sortingState"];
					updatesorting(sortingStateValue, m_viewno, m_metadatatype, m_userlogin);
				}
				
				strSqlCommand.Format(L"exec Update_ToggleView_Details '%s' ,'%s' ,'%s' ,'%d' ,'%d' ,'%d' ,'%d','%d' ; delete from orika_ToggleView where userlogin='%s' and metadatatype='%s' and viewSerialNo='%d';", m_viewName, m_metadatatype, m_userlogin, m_isDarkMode, m_isFilterOn, m_isColorOn, m_viewno, m_isPivot, m_userlogin, m_metadatatype, m_viewno);
				CStaticClass::m_sqldata.executeCommand(strSqlCommand);
				if (d.HasMember("columns") == true)
				{
					const Value& columnsValue = d["columns"];
					if (columnsValue.IsArray())
					{
						for (SizeType i = 0; i < columnsValue.Size(); i++)
						{
							CString  m_strcolId = L"";
							int      m_hide = 0;
							CString  m_aggFunc = L"";
							int      m_width=0;
							int      m_template = 0;
							int      m_pivotIndex = 0;
							CString       m_pinned = L"-1";
							int      m_rowGroupIndex = 0;

							if (columnsValue[i].IsObject())
							{
								const Value& columnVal = columnsValue[i];
								if (columnVal.HasMember("colId") == true)
								{
									const Value& colIdVal = columnVal["colId"];
									if (colIdVal.IsString() == true)
									{
										m_strcolId = colIdVal.GetString();
									}
								}
								if (columnVal.HasMember("hide") == true)
								{
									const Value& hideVal = columnVal["hide"];
									if (hideVal.IsBool())
									{
										m_hide = hideVal.GetBool();
									}
								}
								if (columnVal.HasMember("aggFunc") == true)
								{
									const Value& aggFuncval = columnVal["aggFunc"];
									if (aggFuncval.IsString())
									{
										m_aggFunc = aggFuncval.GetString();
									}
								}
								if (columnVal.HasMember("width") == true)
								{
									const Value& widthval = columnVal["width"];
									if (widthval.IsInt())
									{
										m_width = widthval.GetInt();
									}
								}

								if (columnVal.HasMember("template") == true)
								{
									const Value& objtemplate = columnVal["template"];
									if (objtemplate.IsInt())
									{
										m_template = objtemplate.GetInt();
									}
								}

								if (columnVal.HasMember("pivotIndex") == true)
								{
									const Value& pivotIndexval = columnVal["pivotIndex"];
									if (pivotIndexval.IsInt())
									{
										m_pivotIndex = pivotIndexval.GetInt();
									}
									else
									{
										m_pivotIndex = -1;
									}
								}
								if (columnVal.HasMember("pinned") == true)
								{
									const Value& pinnedval = columnVal["pinned"];
									if (pinnedval.IsString())
									{
										m_pinned = pinnedval.GetString();
									}
									else
									{
										m_pinned = L"-1";
									}
								}
								if (columnVal.HasMember("rowGroupIndex") == true)
								{
									const Value& rowGroupIndexval = columnVal["rowGroupIndex"];
									if (rowGroupIndexval.IsInt())
									{
										m_rowGroupIndex = rowGroupIndexval.GetInt();
									}
									else
									{
										m_rowGroupIndex = -1;
									}
								}
								//Insert data In Sql								
								strSqlCommand.Format(L"exec update_ToggleView '%s'	,'%s','%s','%s','%d','%s','%d','%d','%d','%d','%s',%d", m_userlogin,m_metadatatype, m_viewName, m_strcolId, m_rowGroupIndex, m_aggFunc, m_viewno, m_hide, m_width, m_pivotIndex, m_pinned,m_template);
								CStaticClass::m_sqldata.executeCommand(strSqlCommand);
								//End of Inert Data in Sql

							}
						}
					}
				}	
				CString strFinalJson = L"";
				strFinalJson.Format(L"{\"type\":\"SERVER_MESSAGE\",\"responseMessage\":\"View %s (%d) Has been updated.\"}", m_viewName, m_viewno);
				string strforsend = CT2A(strFinalJson.GetString());
				SendDataToClient(client, strforsend, strKey, m_ActiveClient);
				
			}
			else if (strtype == "UPDATE_REQUEST" && checkLoginValidate == 1)
			{				
				CString 		m_columnkey=L"";
				CString 		m_tabname = L"";
				CString 		m_value = L"";
				
				if (d.HasMember("columnkey") == true)
				{
					const Value& Keyuser = d["columnkey"];
					m_columnkey = Keyuser.GetString();
				}
				if (d.HasMember("tabname") == true)
				{
					const Value& Keyuser = d["tabname"];
					m_tabname = Keyuser.GetString();
				}
				if (d.HasMember("value") == true)
				{
					const Value& Keyuser = d["value"];
					m_value = Keyuser.GetString();
				}
				CString LoginValue = L"";
				CString SymbolValue = L"";
				if (d.HasMember("updateuniquekeys") == true)
				{
					const Value& updateuniquekeys = d["updateuniquekeys"];
					if (updateuniquekeys.IsArray())
					{
						for (SizeType i = 0; i < updateuniquekeys.Size(); i++)
						{
							if (updateuniquekeys[i].IsObject())
							{
								const Value& updateuniquekey = updateuniquekeys[i];
								if (updateuniquekey.HasMember("updateuniquekey") == true)
								{
									const Value& c_updateuniquekey = updateuniquekey["updateuniquekey"];									
									CString  m_loginKey = L"";
									m_loginKey = c_updateuniquekey.GetString();
									if (m_loginKey == L"login")
									{
										if (updateuniquekey.HasMember("updatekeyvalue") == true)
										{
											const Value& c_updatekeyvalue = updateuniquekey["updatekeyvalue"];
											LoginValue = c_updatekeyvalue.GetString();
										}
									}
									if (m_loginKey == L"symbol")
									{
										if (updateuniquekey.HasMember("updatekeyvalue") == true)
										{
											const Value& c_updatekeyvalue = updateuniquekey["updatekeyvalue"];
											SymbolValue = c_updatekeyvalue.GetString();
										}
									}									
								}
								
							}
						}
					}
				}

				if (m_columnkey == L"lpratio")
				{
					//SymbolValue
					CString strSqlCommand = L"";

					CStaticClass::st_Orika_symbolmaster m_stSymbol = {};

					CStaticClass::m_Orika_symbolmasterHastable.Lookup(SymbolValue, m_stSymbol);
					CString commodityGroup = m_stSymbol.m_Group;

					strSqlCommand.Format(L"exec proc_update_client_LpRatio '%s','%s','%s'", LoginValue, commodityGroup, m_value);
					CStaticClass::m_sqldata.executeCommand(strSqlCommand);
					CStaticClass::st_Orika_clientmaster m_st_Orika_clientmaster = {};					
					m_st_Orika_clientmaster.m_lpRatio = _wtof(m_value);
					//locking required
					CStaticClass::m_Orika_clientmasterHastable.SetAt(LoginValue, m_st_Orika_clientmaster);
				}


				if (m_columnkey == L"AllPositionLimitLoginwise" || m_columnkey == L"AllSymbolBuyPositionLimit" || m_columnkey == L"AllSymbolSellPositionLimit" || m_columnkey == L"BuyPositionLimit" || m_columnkey == L"SellPositionLimit")
				{
					CClientSocket* m_unisocket;										
					m_unisocket = new CClientSocket();
					m_unisocket->Connect(CStaticClass::GatewayServer, "", "", "");
					CString strData = L"";
					int m_OrderType = 0;
					if (SymbolValue.GetLength() > 4)
					{
						SymbolValue = SymbolValue.Mid(0, SymbolValue.GetLength() - 3);
					}
					if (m_columnkey == L"BuyPositionLimit")
					{
						m_OrderType = 1;
					}
					if (m_columnkey == L"SellPositionLimit")
					{
						m_OrderType = 2;
					}
					if (m_columnkey == L"AllSymbolBuyPositionLimit")
					{
						SymbolValue = L"SHARE";
						m_OrderType = 3;
					}
					if (m_columnkey == L"AllSymbolSellPositionLimit")
					{
						SymbolValue = L"SHARE";
						m_OrderType = 4;
					}
					if (m_columnkey == L"AllPositionLimitLoginwise")
					{
						SymbolValue = L"SHARE";
						m_OrderType = 5;
					}
					
					strData.Format(L"%s|%s|%d|%s", LoginValue, SymbolValue, m_OrderType, m_value);
					
					
					CStringA strA(strData); // a helper string
					LPCSTR m_DataForSend = strA;
					
					
					CStaticClass::stConfig m_stConfig = {};
					m_stConfig.Login = _wtoi64(LoginValue);
					m_stConfig.Symbol = SymbolValue;
					m_stConfig.SaudaType = m_OrderType;
					m_stConfig.Limit = _wtoi(m_value);

					CString strSymbolGroupCheck = L"0";
					m_stConfig.SymbolGroupCheck = _wtoi(strSymbolGroupCheck);

					CString loginSymbolKey = L"";
					
					loginSymbolKey.Format(L"%s:%s:%d", LoginValue, SymbolValue, m_OrderType);
					
					CStaticClass::m_mutex_LimitConfig.Lock();
					
						CStaticClass::LimitConfig.SetAt(loginSymbolKey, m_stConfig);
					
					
					CStaticClass::m_mutex_LimitConfig.Unlock();



					m_unisocket->SendString(m_DataForSend);
					m_unisocket->Close();
				}
				else
				{
					CStaticClass::m_sqldata.processUpdateRequest(m_columnkey, m_tabname, m_value, LoginValue, SymbolValue);
				}
			}

		}





		std::string olddata = "";
		
	}
	else
	{
		string strforsend = "{\"type\":\"LOGIN_STATUS\",\"status\":\"invalid input\"}";
		SendDataToClient(client, strforsend, strKey, m_ActiveClient);
		send_close_message_to_client(psession);
	}

	//////(L"Finish handle");
}




void sendDataToAllClient(CString  msg)
{

	string str = string(CT2CA(msg));
	ProtoMessageConverter obj_converter;
	DataBuffer protodata;
	protodata.Allocate(1500000);
	obj_converter.JasonToProto(str, protodata);

	DataBuffer DataForSend;
	DataForSend.Allocate(1500000);


	FrameAndDeframeMessage FDObj;
	FDObj.frameOutgoingMessage(protodata.GetBuffer(), DataForSend, protodata.GetDataSize());




	CStaticClass::m_mutex_ClientList.Lock();
	//(L"119");
	POSITION pos = CStaticClass::m_ClientContext.GetStartPosition();
	while (pos != NULL)
	{
		CString strclientkey = L"";
		CStaticClass::st_ClientContext m_st = {};
		CStaticClass::m_ClientContext.GetNextAssoc(pos, strclientkey, m_st);
		//string strforsend = CT2A(msg.GetString());
		session_send_data_AllClient(m_st.m_clientConnection, DataForSend.GetBuffer(), DataForSend.GetDataSize(), strclientkey, m_st.m_activeClient);		
	}
	CStaticClass::m_mutex_ClientList.Unlock();
	protodata.Delete();
	//(L"U119");
}

void sendDataToAll_Other_Client(CString  msg,CString m_ignoreClient)
{
	string str = string(CT2CA(msg));
	ProtoMessageConverter obj_converter;
	DataBuffer protodata;
	protodata.Allocate(1500000);
	obj_converter.JasonToProto(str, protodata);



	////(L"Enter sendDataToAllClient");
	CStaticClass::ClientContext m_ClientContext_Local;

	CStaticClass::m_mutex_ClientList.Lock();

	POSITION pos = CStaticClass::m_ClientContext.GetStartPosition();
	while (pos != NULL)
	{
		CString strclientkey = L"";
		CStaticClass::st_ClientContext m_st = {};
		CStaticClass::m_ClientContext.GetNextAssoc(pos, strclientkey, m_st);
		if (m_ignoreClient != strclientkey)
		{
			m_ClientContext_Local.SetAt(strclientkey, m_st);
		}
	}

	CStaticClass::m_mutex_ClientList.Unlock();

	POSITION pos_tmp = m_ClientContext_Local.GetStartPosition();
	while (pos_tmp != NULL)
	{
		CString strclientkey = L"";
		CStaticClass::st_ClientContext m_st = {};
		m_ClientContext_Local.GetNextAssoc(pos_tmp, strclientkey, m_st);
		SSL_session* client = NULL;
		client = m_st.m_clientConnection;
		//(L"Going To Send Data To Client");		
		SendDataToClient(client, protodata.GetBuffer(), strclientkey, m_st.m_activeClient);
		//(L"Data Has Been Sent To Client");
	}
	protodata.Delete();
	////(L"Exit sendDataToAllClient");
}
void SendDataToClient(SSL_session* sslsession,string msg,CString strkey, int ActiveClient)
{		

	//CString m_strLog(msg.c_str());
	//( m_strLog);
	if (sslsession ==  nullptr)
	{				
		return;
	}

	ProtoMessageConverter obj_converter;
	
	DataBuffer protodata;
	protodata.Allocate(1500000);
	obj_converter.JasonToProto(msg, protodata);			

	if (sslsession!=NULL )
	{ 
	DataBuffer DataForSend;
	DataForSend.Allocate(1500000);	
		

	//LZString m_obj;
	string utf8_CompressedData = "";

		
	string utf8_UncompressedData = "";	
	
	
	//char* chrData = const_cast<char*>(strforsend.data());

	
	
	/*int Datalen = strlen(chrData);
	QString  strUnCompressData = QString::fromUtf8((const char*)chrData, Datalen);	
	QString strcompressed = m_obj.compressToUTF16(strUnCompressData);
	utf8_CompressedData = strcompressed.toUtf8().constData();		
	char* chrDataCompressed = const_cast<char*>(utf8_CompressedData.c_str());*/


	FrameAndDeframeMessage FDObj;
	
	FDObj.frameOutgoingMessage(protodata.GetBuffer(), DataForSend, protodata.GetDataSize());		
	
	session_send_data(sslsession, DataForSend.GetBuffer(), DataForSend.GetDataSize(), strkey, ActiveClient);		
	
	
	DataForSend.Delete();
	}	
	protodata.Delete();
}



void SendDataToClient_AllClient(SSL_session* sslsession, string msg, CString strkey, int ActiveClient)
{
	if (sslsession == nullptr)
	{
		return;
	}	
	if (sslsession != NULL)
	{
		DataBuffer DataForSend;
		DataForSend.Allocate(1500000);		
		string utf8_CompressedData = "";
		string utf8_UncompressedData = "";

		char* chrDataForSend = const_cast<char*>(msg.c_str());




		//int Datalen = strlen(chrData);
		//QString  strUnCompressData = QString::fromUtf8((const char*)chrData, Datalen);
		//QString strcompressed = m_obj.compressToUTF16(strUnCompressData);
		//utf8_CompressedData = strcompressed.toUtf8().constData();
		//char* chrDataCompressed = const_cast<char*>(utf8_CompressedData.c_str());

		//int check_data_length_befoeFrame = protodata.GetDataSize();

		FrameAndDeframeMessage FDObj;
		FDObj.frameOutgoingMessage(chrDataForSend, DataForSend, msg.length());

		//int check_data_length_AfterFrame = DataForSend.GetDataSize();
		//FDObj.frameOutgoingMessage(chrData, DataForSend, strlen(chrData));

		session_send_data_AllClient(sslsession, DataForSend.GetBuffer(), DataForSend.GetDataSize(), strkey, ActiveClient);


	}
	
}

void SendDataToClient_UnRegistered(SSL_session* sslsession, string msg)
{
	ProtoMessageConverter obj_converter;
	string strforsend = "";
	DataBuffer protodata;
	protodata.Allocate(1500000);
	obj_converter.JasonToProto(msg, protodata);

	if (sslsession == nullptr)
	{
		return;
	}
	if (sslsession != NULL)
	{
		DataBuffer DataForSend;
		DataForSend.Allocate(1500000);



		//LZString m_obj;
		string utf8_CompressedData = "";

		std::string test = strforsend;

		string utf8_UncompressedData = "";

		//char* chrData = const_cast<char*>(strforsend.c_str());




		/*int Datalen = strlen(chrData);
		QString  strUnCompressData = QString::fromUtf8((const char*)chrData, Datalen);		
		QString strcompressed = m_obj.compressToUTF16(strUnCompressData);
		utf8_CompressedData = strcompressed.toUtf8().constData();		
		char* chrDataCompressed = const_cast<char*>(utf8_CompressedData.c_str());*/
		FrameAndDeframeMessage FDObj;
		//int dataSize = strforsend.length();				
		//char* chrDataForFraming = const_cast<char*>(strforsend.c_str());
		FDObj.frameOutgoingMessage(protodata.GetBuffer(), DataForSend, protodata.GetDataSize());

		session_send_dataToUnregisteredClient(sslsession, DataForSend.GetBuffer(), DataForSend.GetDataSize());

		DataForSend.Delete();
	}
	protodata.Delete();
}
