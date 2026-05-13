#include "stdafx.h"
#include "socketserver.h"
#include <string>
#include <process.h>
#include "StaticClass.h"
#include "AlertStaticClass.h"
#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "mswsock.lib")
int SocketServer::m_Serverislive = 0;
int SocketServer::m_recvThreadSrynk = 0;

CMap<int, int, SOCKET, SOCKET> SocketServer::m_socket_detail_Map;

SOCKET hClinetHandle;
SOCKET hServerHandle;


SOCKET hClinetHandle_r;
SOCKET hServerHandle_r;

UINT Request_receiver_tHandler(LPVOID pParam);
UINT Sending_Handler(LPVOID pParam);

void SocketServer::serverPrepare()
{	
	AfxSocketInit();
	CSocket Listen;
	int iPort;
	CString strPort;
	iPort = CStaticClass::APISERVER_PORT;
	strPort.Format(_T("%d"), iPort);
	Listen.Create(iPort);
	hServerHandle = Listen.m_hSocket;
	if (Listen.Listen())
	{		
		int i = 0;
		while (m_Serverislive==1)
		{
			
			CSocket* pCSock = new CSocket();
			if (Listen.Accept(*pCSock))
			{
				CString  m_Ipadd = L"";
				UINT m_portno = 0;
				pCSock->GetPeerName(m_Ipadd, m_portno);
				CString strlog = L"";
				strlog.Format(L"New Client Address is %s:%d", m_Ipadd, m_portno);
				CStaticClass::m_logfile.LogEvent(strlog);				

				//m_receiveThread_Lock.Lock();

					m_recvThreadSrynk = 1;
					hClinetHandle_r = pCSock->Detach();				
					AfxBeginThread(Request_receiver_tHandler, 0);
					AfxBeginThread(Sending_Handler, 0);
					while (m_recvThreadSrynk == 1)
					{
					}

				//m_receiveThread_Lock.Unlock();

				strlog.Format(L"New Client Address is %s:%d Is attached to seprate server", m_Ipadd, m_portno);
				CStaticClass::m_logfile.LogEvent(strlog);
				Sleep(10);
				//LeaveCriticalSection(&m_socket_lock);								
				
				
			}
			delete pCSock;
			
		}
	}
	else
	{
		CString strMsg;
		strMsg.Format(L"Unable to open requested port %s to this server. May be nother server running on this port. Please specify another port and start server again.", strPort);						
		AfxMessageBox(strMsg);		
	}
}

UINT Sending_Handler(LPVOID pParam)
{
	while (true)
	{
		AlertStaticClass::m_alert_dealing_Lock.Lock();
			TMTArray<AlertStaticClass::dealingData> m_tempArray;
			m_tempArray.Assign(AlertStaticClass::m_Alertdealing_Array);
			AlertStaticClass::m_Alertdealing_Array.Clear();
		AlertStaticClass::m_alert_dealing_Lock.Unlock();

		int total_Data_count = m_tempArray.Total();
		for (int i = 0; i < total_Data_count; i++)
		{
			AlertStaticClass::dealingData m_st = {};
			m_st = m_tempArray[i];
			//char n_ch[DEALING_DATA_SIZE+ HEADER_SIZE];
			char n_ch[DEALING_DATA_SIZE+ HEADER_SIZE];
			AlertStaticClass::st_header m_stheader = {};
			m_stheader.Action = 1;
			m_stheader.Data_Size = DEALING_DATA_SIZE;
			m_stheader.Data_Type = 0;
			m_stheader.reserved = 0;

			memcpy(n_ch, &m_stheader, sizeof(m_stheader));
			memcpy(n_ch+ HEADER_SIZE, &m_st, sizeof(m_st));						
			POSITION pos = SocketServer::m_socket_detail_Map.GetStartPosition();
			while (pos != NULL)
			{
				int m_socketno = 0;
				SOCKET m_mainSocket;
				SocketServer::m_socket_detail_Map.GetNextAssoc(pos, m_socketno, m_mainSocket);
				///Sending order data To Client										
				//m_socket->Send(n_ch, DEALING_DATA_SIZE+ HEADER_SIZE+5);
				CSocket m_socket;
				m_socket.Attach(m_mainSocket);				
				m_socket.Send(n_ch, sizeof(n_ch));
				m_socket.Detach();
			}
		}
	}
	return 0;
}
UINT Request_receiver_tHandler(LPVOID pParam)
{
	//m_CLogFile.LogEvent(L"Receiving Start");
	int i_userid;
	AfxSocketInit();	
	if (hClinetHandle_r != INVALID_SOCKET)	
	{		
		CSocket* pSocket=new CSocket();
		//EnterCriticalSection(&m_Re_socket_lock);		
		pSocket->Attach(hClinetHandle_r);

		/*SocketServer::m_socket_details m_st = {};
		m_st.pSocket.Attach(pSocket);
		m_st.m_dealing = 1;*/

		int m_socketno = *pSocket;
		SocketServer::m_socket_detail_Map.SetAt(m_socketno, hClinetHandle_r);

		SocketServer::m_recvThreadSrynk = 0;
		//LeaveCriticalSection(&m_Re_socket_lock);
		CSocket t_socket;
		CString  m_Ipadd = L"";
		UINT m_portno = 0;
		pSocket->GetPeerName(m_Ipadd, m_portno);		
		//receive Use Id and Get UserPortfolio
		CString m_token = L"";

		CString New_data = L"";
		char	cbuf[25000];
		for (int i = 0; i < 25000; i++)
			cbuf[i] = '\0';
		//Wait for Receiving Menucode Status		
		CString recv_data = L"";
		int incrementval = 0;
		while (true)
		{
			int datasize = 0;
			if ((datasize=pSocket->Receive(cbuf + incrementval, sizeof(cbuf)-1)) > 0)
			{				
			}
			else
			{
				break;
			}
		}
		
		SocketServer::m_socket_detail_Map.RemoveKey(m_socketno);
		delete pSocket;
	}
	return 0;
}





