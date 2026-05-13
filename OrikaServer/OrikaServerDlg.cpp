// OrikaServerDlg.cpp : implementation file
#include "stdafx.h"
#include "OrikaServer.h"
#include "OrikaServerDlg.h"
#include "afxdialogex.h"
#include "StaticClass.h"
#include "MSMQ\MSMQApiWrapper.h"
#include "server/server.h"
#include "ClientSocket/DataSource.h"
#include "..\ClientSocket\ClientSocket.h"
#include "SocketServer.h"
#include "SocketServer/IOCPServer.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// IOCP-based multi-client socket server (see SocketServer/ folder).
static iocp::IOCPServer g_iocpServer;

static void StartIOCPSocketServer(unsigned short port)
{
	g_iocpServer.SetOnConnect([](iocp::ClientContext* c) {
		CString log;
		log.Format(L"[IOCP] Client %llu connected (port=%u)",
			c->clientId, ntohs(c->remoteAddr.sin_port));
		CStaticClass::m_logfile.LogEvent(log);
	});

	g_iocpServer.SetOnDisconnect([](unsigned long long id) {
		CString log;
		log.Format(L"[IOCP] Client %llu disconnected", id);
		CStaticClass::m_logfile.LogEvent(log);
	});

	g_iocpServer.SetOnData([](iocp::ClientContext* c, const char* data, int len) {
		// Echo by default. Replace with the real request handler when ready.
		g_iocpServer.Send(c->clientId, data, len);
	});

	g_iocpServer.SetOnError([](const std::string& msg, int code) {
		CString log;
		log.Format(L"[IOCP] %S (code=%d)", msg.c_str(), code);
		CStaticClass::m_logfile.LogEvent(log);
	});

	if (!g_iocpServer.Start(port)) {
		CString err;
		err.Format(L"[IOCP] Failed to start on port %u", port);
		CStaticClass::m_logfile.LogEvent(err);
		return;
	}

	CString ok;
	ok.Format(L"[IOCP] Server listening on port %u", port);
	CStaticClass::m_logfile.LogEvent(ok);
}

void COrikaServerDlg::checkandremovetoken()
{
	while (CStaticClass::TokenCheckingStartStop == 1)
	{
		CStaticClass::m_mutexlogintoken.Lock();				
		POSITION pos = CStaticClass::m_tokenlist.GetStartPosition();
		while (pos != NULL)
		{
			CString logintoken = L"";
			CStaticClass::st_token m_token = {};
			CStaticClass::m_tokenlist.GetNextAssoc(pos, logintoken, m_token);
			UINT64 CurrentTime = _time64(NULL);
			if (m_token.m_totalconnection<1 && (CurrentTime- m_token.m_lastdisconnecttime)>300)
			{
				CStaticClass::m_tokenlist.RemoveKey(logintoken);
			}
		}		
		CStaticClass::m_mutexlogintoken.Unlock();
		Sleep(300);
	}
}
void COrikaServerDlg::data_read_from_MSMQ()
{
	CMSMQApiWrapper  m_CMSMQApiWrapper;
	CString szLabel,szMessage,szOutput;	
	int count=0;	
	while(CStaticClass::MSMQReaderStartStop==1)
	{		
		if (CStaticClass::MSMQReaderPause==0)
		{						
			szMessage="";
			szOutput="";					
			m_CMSMQApiWrapper.ReadStringMessage(CStaticClass::MSMQQueuName,szLabel,szMessage);
			szOutput.Format(L"%s %s",szLabel,szMessage);			

			if (szMessage.GetLength()>0)
			{
				
				CStaticClass::m_sqldata.convertToDealFormat(szMessage);
				
			}			
			CString MQStatus=L"";			
		}	
		Sleep(1);		 

	}	
}


UINT Read_Data_From_MSMQ(LPVOID pParam);
UINT Read_Data_From_MSMQ(LPVOID pParam)
{
	COrikaServerDlg *Dlg= (COrikaServerDlg*)pParam;  
	Dlg->data_read_from_MSMQ();
	return 0;
}

UINT CheckToken(LPVOID pParam);
UINT CheckToken(LPVOID pParam)
{
	COrikaServerDlg* Dlg = (COrikaServerDlg*)pParam;
	Dlg->checkandremovetoken();
	return 0;
}

//UINT SendNetpositionClientWise(LPVOID pParam);
//UINT SendNetpositionClientWise(LPVOID pParam)
//{
//	CStaticClass m_staticclass;
//	m_staticclass.sendClientWiseNetPositionToAllClient();
//	return 0;
//}
UINT StartTickReceivingFromExternalServer(LPVOID pParam);
UINT StartTickReceivingFromExternalServer(LPVOID pParam)
{
	CDataSourceConnection m_CDataSourceConnection;
	m_CDataSourceConnection.Start();
	return 0;
}


UINT DataCalNetPositionclientwiseFromTickDataThread(LPVOID pParam);
UINT DataCalNetPositionclientwiseFromTickDataThread(LPVOID pParam)
{
	CStaticClass m_staticclass;
	m_staticclass.DataCalNetPositionclientwiseFromTickData();
	return 0;
}


UINT SendTickData(LPVOID pParam)
{	
	CStaticClass m_staticclass;
	m_staticclass.sendUpdatedTick();
	return 0;
}

UINT StartLogWriteThread(void*);
UINT StartLogWriteThread(void* pParam)
{
	COrikaServerDlg* obj = (COrikaServerDlg*)pParam;
	obj->ThreadProc_Log();
	return 0;
}


UINT compareposition_and_update(LPVOID pParam)
{
	CStaticClass m_staticclass;
	m_staticclass.sendUpdatedTick();
	return 0;
}




UINT CopyAllPendingTickDataFromManager(LPVOID pParam);
UINT CopyAllPendingTickDataFromManager(LPVOID pParam)
{
	CStaticClass m_staticclass;
	m_staticclass.GettingTickDataFromManager();
	return 0;
}


UINT SendHeartbeat(LPVOID pParam);
UINT SendHeartbeat(LPVOID pParam)
{
	CStaticClass m_staticclass;
	m_staticclass.sendHeartBeattoAllClient();
	return 0;
}


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// COrikaServerDlg dialog



COrikaServerDlg::COrikaServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COrikaServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COrikaServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX,BTN_START,m_btnstart);
	DDX_Control(pDX,BTN_STOP,m_btnstop);
	DDX_Control(pDX,BTN_EXIT,m_btnexit);
	DDX_Control(pDX,TXT_PORT,m_txtport);
}

BEGIN_MESSAGE_MAP(COrikaServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BTN_START, &COrikaServerDlg::OnBnClickedStart)
	ON_WM_CLOSE()	
	ON_BN_CLICKED(BTN_EXIT, &COrikaServerDlg::OnBnClickedExit)
	ON_BN_CLICKED(BTN_STOP, &COrikaServerDlg::OnBnClickedStop)
	ON_BN_CLICKED(BTN_IMPORT_MASTERDATA, &COrikaServerDlg::OnBnClickedImportMasterdata)
END_MESSAGE_MAP()



BOOL COrikaServerDlg::OnInitDialog()
{	
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CString exePath;
	TCHAR buffer[MAX_PATH];
	DWORD length = GetModuleFileName(nullptr, buffer, MAX_PATH);
	if (length != 0)
	{
		// Remove the executable file name to get the folder path
		PathRemoveFileSpec(buffer);
		exePath = buffer;
	}
	CStaticClass::m_mtmanager.LoadConfigFile(exePath + L"\\oreka.config");
	m_txtport.SetWindowText(CStaticClass::orikaPort);
	if (CStaticClass::APIFolderPath == L"")
	{
		CStaticClass::APIFolderPath = exePath + L"\\Page\\";
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COrikaServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void COrikaServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR COrikaServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void COrikaServerDlg::OnBnClickedStart()
{		
	InitializeCriticalSection(&CStaticClass::m_cs_Thread);
	InitializeConditionVariable(&CStaticClass::m_cv_Thread);

	m_btnstart.EnableWindow(false);
	m_btnstop.EnableWindow(true);
	m_btnexit.EnableWindow(false);	
	m_txtport.EnableWindow(false);	
	CString strportno= L"";
	m_txtport.GetWindowText(strportno);
	CStaticClass::portno=_wtoi(strportno);
		
	CStaticClass m_staticclass;
	m_staticclass.initializeconnection();
	m_staticclass.initializePointerArray();
	CStaticClass::m_mtmanager.Initialize();


	
	CStaticClass::m_sqldata.getSymbolMasterDataForTickSubscribe();

	////Share SERVER
	CStaticClass::m_mtmanager.login(CStaticClass::MTServerAdd, CStaticClass::MTServerUserID, CStaticClass::MTServerPassword);
	
	CStaticClass::m_mtmanager.LoadMTClientMaster();
	
	CStaticClass::m_sqldata.getClientmasterData();
	//CStaticClass::m_sqldata.getSymbolMasterDataForTickSubscribe();
	CStaticClass::m_sqldata.getSymbolMasterData();
	CStaticClass::m_sqldata.loadDataOrika_clientmaster();
	CStaticClass::m_sqldata.loadDataOrika_clientbrokerage();
	CStaticClass::m_sqldata.loadOrika_PLDevideRatio();
	CStaticClass::m_sqldata.loadOrika_symbolgroup();
	CStaticClass::m_sqldata.loadOrika_Clientgatewayconfig();
	CStaticClass::m_sqldata.loadOrika_symbolmaster();
	CStaticClass::m_sqldata.loadOrderHashTable();
	CStaticClass::m_sqldata.loadTradeLotAndTOT();
	CStaticClass::m_sqldata.loadOrika_LoginSymbolWiseLastTrade();
	CStaticClass::m_sqldata.loadOrika_LoadGoodTradeBadTrade();
	CStaticClass::m_sqldata.Load_PerfectAndRoundedInDataBase();
	CStaticClass::m_sqldata.loadBalanceTableAccounting();
	CStaticClass::m_sqldata.loadPositionAverageAccounting();

	CStaticClass::m_sqldata.Loadloginuser();
	CStaticClass::m_sqldata.LoadLpRatioCommodityGroupwise();
	CStaticClass::m_sqldata.loadClientBrokerageFromDealTableAccounting();	
	m_staticclass.loadClientwisenetpositionData();
	m_staticclass.set_protoandjsonmessage();
	CStaticClass::m_sqldata.loadDealNo();
	CStaticClass::m_sqldata.loadDealTableAccounting();
	CStaticClass::m_sqldata.loadMessageCodeDesc();
		
	CStaticClass::m_sqldata.loadDataOrika_dealsHighLow();


	

	//Loading Pending Data
		CStaticClass::m_mtmanager.UpdatePendingSaudaToMSMQ();
	//End of Loading Pending Data
	CStaticClass::m_sqldata.loadOrderCount();
	CStaticClass::m_sqldata.loadColumnSubscription();
	
	////Start Thread for Getting Data Feed For Check Order Status 
	//CDataSourceConnection m_CDataSourceConnection;
	//m_CDataSourceConnection.Start();
	////End

	////Create For Socket Connection For getting Limit Config From Order and position check Gateway
	//CClientSocket* m_unisocket;
	//m_unisocket = new CClientSocket();
	//m_unisocket->Connect(CStaticClass::GatewayServer, "", "", "");
	//m_unisocket->SendString("!sharedata!");
	//Sleep(500);
	//int ReceivingTimeour = 10;
	//char receivingChar[5000];
	//UINT64 ReceivingStart = _time64(NULL);
	//int pendingData = 0;
	//while ((_time64(NULL) - ReceivingStart) < ReceivingTimeour)
	//{		
	//	m_unisocket->ReadString(receivingChar+ pendingData, 5000);
	//	CString strAllData = CString(receivingChar);
	//	for (int i = 0; i < 5000; i++)
	//	{
	//		receivingChar[i] = ' ';
	//	}
	//	if (strAllData.GetLength() > 0)
	//	{	
	//		while (strAllData.Find(L"|") > 0)
	//		{
	//			pendingData = 0;
	//			CString strData = strAllData.Mid(0, strAllData.Find(L"|")); ;
	//			strAllData= strAllData.Mid(strAllData.Find(L"|") + 1, (strAllData.GetLength() - strAllData.Find(L"|")));
	//			CString m_login = strData.Mid(0, strData.Find(L":"));
	//			strData = strData.Mid(strData.Find(L":") + 1, (strData.GetLength() - strData.Find(L":")));
	//			CString m_Symbol = strData.Mid(0, strData.Find(L":"));
	//			strData = strData.Mid(strData.Find(L":") + 1, (strData.GetLength() - strData.Find(L":")));
	//			int m_SaudaType = _wtoi(strData.Mid(0, strData.Find(L":")));
	//			strData = strData.Mid(strData.Find(L":") + 1, (strData.GetLength() - strData.Find(L":")));
	//			int m_alloatedLimit = _wtoi(strData);

	//			CStaticClass::stConfig m_stConfig = {};
	//			m_stConfig.Login = _wtoi64(m_login);
	//			m_stConfig.Symbol = m_Symbol;
	//			m_stConfig.SaudaType = m_SaudaType;
	//			m_stConfig.Limit = m_alloatedLimit;

	//			CString strSymbolGroupCheck = L"0";
	//			m_stConfig.SymbolGroupCheck = _wtoi(strSymbolGroupCheck);

	//			CString loginSymbolKey = L"";
	//			CString strlogPrint = L"";
	//			loginSymbolKey.Format(L"%s:%s:%d", m_login, m_Symbol, m_SaudaType);
	//			strlogPrint.Format(L"%s,%s,%d,%d", m_login, m_Symbol, m_SaudaType, m_alloatedLimit);
	//			CStaticClass::m_mutex_LimitConfig.Lock();
	//				CStaticClass::LimitConfig.SetAt(loginSymbolKey, m_stConfig);
	//			CStaticClass::m_mutex_LimitConfig.Unlock();
	//		}
	//		if (strAllData.GetLength() > 0)
	//		{
	//			pendingData = strAllData.GetLength();
	//			for (int k = 0; k < strAllData.GetLength(); k++)
	//			{
	//				receivingChar[k] = strAllData.GetAt(k);
	//			}
	//		}
	//	}
	//}
	//m_unisocket->Close();


	AfxBeginThread(StartTickReceivingFromExternalServer, this);

	stratServer();

	
	CStaticClass::startTickData=1;	
	AfxBeginThread(SendTickData,this);
	////(L"Server Started");
	/*CStaticClass::heartBeatStart=1;
	AfxBeginThread(SendHeartbeat,this);*/
	CStaticClass::startSendingNetpositionClientWise=1;
	//AfxBeginThread(SendNetpositionClientWise,this);
	CStaticClass::startCalculatingNetpositionClientWise=1;
	AfxBeginThread(DataCalNetPositionclientwiseFromTickDataThread,this);
	AfxBeginThread(CopyAllPendingTickDataFromManager, this);

	SocketServer::m_Serverislive = 1;
	StartIOCPSocketServer(static_cast<unsigned short>(CStaticClass::APISERVER_PORT));

	Sleep(2000);
	CStaticClass::m_mtmanager.loadLastTickFromMT();
	CStaticClass::MSMQReaderStartStop=1;
	CStaticClass::m_mtmanager.UpdateSymbolSource();
	AfxBeginThread(Read_Data_From_MSMQ,this);
	CStaticClass::TokenCheckingStartStop = 1;
	AfxBeginThread(CheckToken, this);	
	AfxBeginThread(StartLogWriteThread, this);
	//AfxBeginThread(SymbolwisePositionSendingThread,this);	
}


void COrikaServerDlg::OnCancel()
{
	CStaticClass::MSMQReaderStartStop=0;
	//m_WebsocketServer.StopServer();
}


void COrikaServerDlg::OnOK()
{
	
}


void COrikaServerDlg::OnClose()
{
	
}

void COrikaServerDlg::OnBnClickedExit()
{
	EndDialog( 0 );
}


void COrikaServerDlg::OnBnClickedStop()
{
	CStaticClass::startComparePosition = 0;
	CStaticClass::startTickData=0;
	CStaticClass::MSMQReaderStartStop=0;
	SocketServer::m_Serverislive = 0;
	g_iocpServer.Stop();
	m_btnstart.EnableWindow(true);
	m_btnstop.EnableWindow(false);
	m_btnexit.EnableWindow(true);
	m_txtport.EnableWindow(true);
}


void COrikaServerDlg::OnBnClickedImportMasterdata()
{
	if (::MessageBox(NULL,L"Do you want to import master data?",L"Import Master Data",MB_YESNOCANCEL )==IDYES )
	{
		CStaticClass::m_mtmanager.UpdateClientMaster_Manual();
		CStaticClass::m_mtmanager.UpdateSymbolMaster();		
		AfxMessageBox(L"All Master Data Has Been Updated");
	}
}

void COrikaServerDlg::ThreadProc_Log()
{
	//SMTTime::STToTime(s_date);
	CString str_interval = L"";
str_interval=L"10000";
CString m_searchString = L"";
	wchar_t       tmp[256];
	//SYSTEMTIME    from,to;
	SYSTEMTIME    from;
	CMTStr256     str, stype;
	
	//m_To.GetTime(&to);	   
	UINT64 INT_from, INT_to, Pre_to;
	INT_from = 1714248000;
	Pre_to = INT_from;

	
	

	while (true)
	{
		SYSTEMTIME curr_To;
		GetLocalTime(&curr_To);
		INT_to = SMTTime::STToTime(curr_To);

		//m_manager.LogTransfer(Pre_to+1,INT_to,L"");

		
		CStaticClass::m_mtmanager.DeviceLogTransfer(Pre_to + 1, INT_to, L"");
		Pre_to = INT_to;

		/*int search_count=CManager::m_SearchString.Total();
		for(int s=0;s<search_count;s++)
		{
			CString SearchStr=CManager::m_SearchString[s];
			if (SearchStr.Find(L"deal")>=0)
			{
				m_manager.LogTransfer(Pre_to+1,INT_to,SearchStr);
			}
			if (SearchStr.Find(L"login")>=0)
			{
				m_manager.LogTransferLogin(Pre_to+1,INT_to,SearchStr);
			}
			Pre_to=INT_to;
		}*/

		Sleep(10000);
	}
}
