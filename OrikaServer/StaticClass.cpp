#include "StdAfx.h"
#include "StaticClass.h"
#include "DataTable\netpositionTable.h"
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
#include <iostream>
#include "server/server.h"
#include "AlertStaticClass.h"
 
using namespace rapidjson;
bool CStaticClass::terminated=false;
CManager CStaticClass::m_mtmanager;
int CStaticClass::portno;
std::string  CStaticClass::strTick="";
CMutex CStaticClass::m_mutex;
CDataSource CStaticClass::connection;
CDataSource CStaticClass::conntmp;
//CSession CStaticClass::session;
int CStaticClass::m_dealing_ID = 0;
CMutex CStaticClass::m_mutexcommoditygroup;


CMutex CStaticClass::m_SqlLock;

CString  CStaticClass::strKeyTransfer=L"";
CMutex   CStaticClass::mutex_keytranssfer; 
CMap<int, int, double, double> CStaticClass::m_ClientBalance;



CMutex CStaticClass::m_mutexLog;

CMap<CString, LPCTSTR, double, double> CStaticClass::m_LPLots;
CMap<CString, LPCTSTR, double, double> CStaticClass::m_comTotalLotsAfterMultiGroupWise;
CMap<CString, LPCTSTR, double, double> CStaticClass::m_LPLots_perfect;

CStaticClass::SymbolSourceArray CStaticClass::m_SymbolSourceArray;
CStaticClass::SymbolHighLowTimeWiseArray CStaticClass::m_SymbolHighLowTimeWiseArray;
CMap<CString, LPCTSTR, CStaticClass::st_OrderCount, CStaticClass::st_OrderCount> CStaticClass::m_OrderCountMap;
CMap<CString, LPCTSTR, CStaticClass::st_LoginSymbolWiseLastTrade, CStaticClass::st_LoginSymbolWiseLastTrade> CStaticClass::m_LoginSymbolWiseLastTrade;
CMap<CString, LPCTSTR, CStaticClass::st_BadTradeGoodTrade, CStaticClass::st_BadTradeGoodTrade> CStaticClass::m_BadTradeGoodTrade;
CMap <CString, LPCTSTR, int, int> CStaticClass::m_columnsubscription;
CMap<CString, LPCTSTR, double, double> CStaticClass::m_LpRatioCommodityGroupWiseMap;

CMap < CString, LPCTSTR, double, double> CStaticClass::m_mismatch_position;


CMap<CString, LPCTSTR, CStaticClass::st_Alert_Setting, CStaticClass::st_Alert_Setting> CStaticClass::m_AlertSettingMap;



CString CStaticClass::m_ordertypedesc[] = {L"Instant/Market Buy",L"Instant/Market Sell" ,L"Buy Limit" ,L"Sell Limit" ,L"Buy Stop" ,L"Sell Stop" ,L"" ,L"" };
CString CStaticClass::m_dealtypedesc[] = { L"DEAL_BUY",L"DEAL_SELL",L"DEAL_BALANCE",L"DEAL_CREDIT",L"DEAL_CHARGE",L"DEAL_CORRECTION",L"DEAL_BONUS",L"DEAL_COMMISSION",L"DEAL_COMMISSION_DAILY",L"DEAL_COMMISSION_MONTHLY" };


CMap<CString, LPCTSTR, CStaticClass::st_LpLotsArray, CStaticClass::st_LpLotsArray&> CStaticClass::m_LpLotsMap;

CMutex CStaticClass::m_mutex_LimitConfig;
CMutex CStaticClass::m_mutex_TotalBrokerage;

CMutex CStaticClass::m_mutexcolumnSubs;

CStaticClass::st_LpLotsArray CStaticClass::m_LpQtyArray_gold;
CStaticClass::st_LpLotsArray CStaticClass::m_LpQtyArray_silver;


CMap<CString, LPCTSTR, CStaticClass::st_LpDetailsArray, CStaticClass::st_LpDetailsArray&> CStaticClass::m_LpDetailMap;
CStaticClass::st_LpDetailsArray CStaticClass::m_LPdetailArray_gold;
CStaticClass::st_LpDetailsArray CStaticClass::m_LPdetailArray_silver;


double* CStaticClass::CompanyVolumeAfterMultiTotalArray[6000];
CMap<CString, LPCTSTR, int, int> CStaticClass::CompanyVolumeAfterMultiTotalArrayIndex;




CMutex CStaticClass::m_positiondevideRatio_lock;


CString CStaticClass::m_ClientPositionRequestType = L"FETCH_CLIENT_POSITIONS";

CString CStaticClass::m_ClientPositionRequestTimerWiseType = L"FETCH_CLIENT_POSITIONS_TIMERWISE_START";

CString CStaticClass::m_DashboardRequest = L"FETCH_DASHBOARD_DATA";

CStaticClass::Orika_ExchangeSymbolHastable CStaticClass::m_Orika_ExchangeSymbolHastable;

CMutex CStaticClass::m_mutexlogintoken;

CMap<CString, LPCTSTR, double, double> CStaticClass::m_allClosingprice;
CMap<CString, LPCTSTR, double, double> CStaticClass::m_ignoreClosingprice;

CMap<CString, LPCTSTR, CStaticClass::st_TradesHighLowPriceSymbolWise, CStaticClass::st_TradesHighLowPriceSymbolWise> CStaticClass::m_TradesHighLowPriceSymbolWise;
CMutex CStaticClass::TradesHighLowLockUpdate;
CMap<CString, LPCTSTR, CStaticClass::st_token, CStaticClass::st_token&> CStaticClass::m_tokenlist;


int CStaticClass::startComparePosition = 1;




////For FO
//CString  CStaticClass::orikaPort = L"82";
//CString  CStaticClass::SqlServerAdd = L"95.111.253.230,7082";
//CString  CStaticClass::SqlServerDatabase = L"Orika_Share";
//CString  CStaticClass::SqlServerUserID = L"sa";
//CString  CStaticClass::SqlServerPassword = L"ok@12345";
//CString  CStaticClass::MTServerAdd = L"51.68.208.248:443";
//CString  CStaticClass::MTServerUserID = L"1017";
//CString  CStaticClass::MTServerPassword = L"hello12345@";
//CString  CStaticClass::MSMQQueuName = L"orika_share";
//LPCSTR  CStaticClass::dataFeedServer="15.206.11.188:550531";
//LPCSTR  CStaticClass::GatewayServer="64.251.10.91:9980";
//int		CStaticClass::ExchangeCode=0;
//CString  CStaticClass::APIFolderPath = L"C:\\OrikaClient_Share\\API\\ClosingFile\\App_Data\\";


////For MCX
//CString  CStaticClass::orikaPort = L"81";
//CString  CStaticClass::SqlServerAdd = L"95.111.253.230,7082";
//CString  CStaticClass::SqlServerDatabase = L"Orika_MCX";
//CString  CStaticClass::SqlServerUserID = L"sa";
//CString  CStaticClass::SqlServerPassword = L"ok@12345";
//CString  CStaticClass::MTServerAdd = L"104.227.250.26:443";
//CString  CStaticClass::MTServerUserID = L"1017";
//CString  CStaticClass::MTServerPassword = L"ASDF123";
//CString  CStaticClass::MSMQQueuName = L"orika_mcx";
//LPCSTR  CStaticClass::dataFeedServer = "104.227.248.10:6060";
//LPCSTR  CStaticClass::GatewayServer="104.227.250.26:998000";
//int		CStaticClass::ExchangeCode=1102;
//CString  CStaticClass::APIFolderPath = L"C:\\OrikaClient_Share_90\\API\\ClosingFile\\App_Data\\";


////For MCX For Demo Server_85
//CString  CStaticClass::orikaPort = L"85";
//CString  CStaticClass::SqlServerAdd = L"95.111.253.230,7082";
//CString  CStaticClass::SqlServerDatabase = L"orika_demo";
//CString  CStaticClass::SqlServerUserID = L"sa";
//CString  CStaticClass::SqlServerPassword = L"ok@12345";
//CString  CStaticClass::MTServerAdd = L"38.170.242.2:443";
//CString  CStaticClass::MTServerUserID = L"1017";
//CString  CStaticClass::MTServerPassword = L"ASDF123";
//CString  CStaticClass::MSMQQueuName = L"orika_demo";
//LPCSTR   CStaticClass::dataFeedServer = "";
//LPCSTR   CStaticClass::GatewayServer = "";
//int		 CStaticClass::ExchangeCode = 1102;



////For MCX For Demo Server_90
//CString  CStaticClass::orikaPort = L"90";
//CString  CStaticClass::SqlServerAdd = L"95.111.253.230,7082";
//CString  CStaticClass::SqlServerDatabase = L"orika_demo_Test";
//CString  CStaticClass::SqlServerUserID = L"sa";
//CString  CStaticClass::SqlServerPassword = L"ok@12345";
//CString  CStaticClass::MTServerAdd = L"38.170.242.2:443";
//CString  CStaticClass::MTServerUserID = L"1017";
//CString  CStaticClass::MTServerPassword = L"ASDF123";
//CString  CStaticClass::MSMQQueuName = L"orika_demo_Test";
//LPCSTR  CStaticClass::dataFeedServer = "";
//LPCSTR  CStaticClass::GatewayServer = "";
//int		CStaticClass::ExchangeCode = 1102;
//CString  CStaticClass::APIFolderPath = L"C:\\OrikaClient_Share_90\\API\\ClosingFile\\App_Data\\";




////For FO Demo with realdata
//CString  CStaticClass::orikaPort = L"85";
//CString  CStaticClass::SqlServerAdd = L"95.111.253.230,7082";
//CString  CStaticClass::SqlServerDatabase = L"orika_demo";
//CString  CStaticClass::SqlServerUserID = L"sa";
//CString  CStaticClass::SqlServerPassword = L"ok@12345";
//CString  CStaticClass::MTServerAdd = L"51.68.208.248:443";
//CString  CStaticClass::MTServerUserID = L"1017";
//CString  CStaticClass::MTServerPassword = L"hello12345@";
//CString  CStaticClass::MSMQQueuName = L"orika_demo";
//LPCSTR   CStaticClass::dataFeedServer = "15.206.11.188:550531";
//LPCSTR   CStaticClass::GatewayServer = "64.251.10.91:9980";
//int		 CStaticClass::ExchangeCode = 0;
//CString  CStaticClass::APIFolderPath = L"C:\\OrikaClient_Share_90\\API\\ClosingFile\\App_Data\\";





////For DemoTest
//CString  CStaticClass::orikaPort = L"85";
//CString  CStaticClass::SqlServerAdd = L"95.111.253.230,7082";
//CString  CStaticClass::SqlServerDatabase = L"orika_demo";
//CString  CStaticClass::SqlServerUserID = L"sa";
//CString  CStaticClass::SqlServerPassword = L"ok@12345";
//CString  CStaticClass::MTServerAdd = L"95.111.253.230:2000";
//CString  CStaticClass::MTServerUserID = L"1000";
//CString  CStaticClass::MTServerPassword = L"ASDF123";
//CString  CStaticClass::MSMQQueuName = L"orika_demo";
//LPCSTR   CStaticClass::dataFeedServer = "13.126.83.17:60631";
//LPCSTR   CStaticClass::GatewayServer="95.111.253.230:99801";
//int		 CStaticClass::ExchangeCode = 0;
//CString  CStaticClass::APIFolderPath = L"C:\\OrikaClient_proto_95\\API\\ClosingFile\\App_Data\\";






//For Protobuffer MCX
CString  CStaticClass::orikaPort = L"86";
CString  CStaticClass::SqlServerAdd = L"95.111.253.230,7082";
CString  CStaticClass::SqlServerDatabase = L"orika_proto";
CString  CStaticClass::SqlServerUserID = L"sa";
CString  CStaticClass::SqlServerPassword = L"ok@12345";
CString  CStaticClass::MTServerAdd = L"38.170.242.2:443";
CString  CStaticClass::MTServerUserID = L"1017";
CString  CStaticClass::MTServerPassword = L"ASDF123";
CString  CStaticClass::MSMQQueuName = L"orika_proto";
LPCSTR  CStaticClass::dataFeedServer = "13.126.83.17:60631";
LPCSTR  CStaticClass::GatewayServer = "95.111.253.230:99801";
int		 CStaticClass::ExchangeCode = 0;
CString  CStaticClass::APIFolderPath = L"";
int			CStaticClass::APISERVER_PORT = 0;
//CString  CStaticClass::APIFolderPath = L"C:\\OrikaClient_proto_95\\API\\ClosingFile\\App_Data\\";




////For Protobuffer Nsefo
//CString  CStaticClass::orikaPort = L"187";
//CString  CStaticClass::SqlServerAdd = L"95.111.253.230,7082";
//CString  CStaticClass::SqlServerDatabase = L"orika_proto_share";
//CString  CStaticClass::SqlServerUserID = L"sa";
//CString  CStaticClass::SqlServerPassword = L"ok@12345";
//CString  CStaticClass::MTServerAdd = L"51.68.208.248:443";
//CString  CStaticClass::MTServerUserID = L"1017";
//CString  CStaticClass::MTServerPassword = L"hello12345@";
//CString  CStaticClass::MSMQQueuName = L"orika_proto_share";
//LPCSTR  CStaticClass::dataFeedServer = "13.126.83.17:60631";
//LPCSTR  CStaticClass::GatewayServer = "95.111.253.230:99801";
//int		 CStaticClass::ExchangeCode = 0;
//CString  CStaticClass::APIFolderPath = L"";



////For MCX Demo
//CString  CStaticClass::orikaPort = L"85";
//CString  CStaticClass::SqlServerAdd = L"95.111.253.230,7082";
//CString  CStaticClass::SqlServerDatabase = L"orika_demo";
//CString  CStaticClass::SqlServerUserID = L"sa";
//CString  CStaticClass::SqlServerPassword = L"ok@12345";
//CString  CStaticClass::MTServerAdd = L"104.227.250.26:443";
//CString  CStaticClass::MTServerUserID = L"1017";
//CString  CStaticClass::MTServerPassword = L"ASDF123";
//CString  CStaticClass::MSMQQueuName = L"orika_demo";
//LPCSTR  CStaticClass::dataFeedServer = "13.126.83.17:6063";
//LPCSTR  CStaticClass::GatewayServer="104.227.250.26:9980";
//int		CStaticClass::ExchangeCode=1101;


////For MCX Demo
//CString  CStaticClass::orikaPort = L"85";
//CString  CStaticClass::SqlServerAdd = L"95.111.253.230,7082";
//CString  CStaticClass::SqlServerDatabase = L"Orika_Demo_Test";
//CString  CStaticClass::SqlServerUserID = L"sa";
//CString  CStaticClass::SqlServerPassword = L"ok@12345";
//CString  CStaticClass::MTServerAdd = L"95.111.253.230:2000";
//CString  CStaticClass::MTServerUserID = L"1000";
//CString  CStaticClass::MTServerPassword = L"ASDF123";
//CString  CStaticClass::MSMQQueuName = L"orika_demo_test";
//LPCSTR  CStaticClass::dataFeedServer = "13.126.83.17:606311";
//LPCSTR  CStaticClass::GatewayServer="95.111.253.230:998011";
//int		CStaticClass::ExchangeCode=1101;

CStaticClass::Orika_MTclientmasterHasTable CStaticClass::m_Orika_MTclientmasterHasTable;
CStaticClass::MapsubbrokerPosition CStaticClass::m_MapsubbrokerPosition;
CStaticClass::MapsubbrokerPosition CStaticClass::m_MapsubbrokerPosition_Fix;

CCommand<CNoAccessor, CNoRowset> cmdExecute;
int CStaticClass::loginvalidate=0;


int CStaticClass::startOrderData=0;

CStaticClass::MapscripWiseNetPosition CStaticClass::m_MapscripWiseNetPosition;

CStaticClass::MapscripWiseNetPosition CStaticClass::m_MapscripWiseNetPosition_Fix;

CStaticClass::MapbrokerPosition CStaticClass::m_MapbrokerPosition;
CStaticClass::MapbrokerPosition CStaticClass::m_MapbrokerPosition_Fix;


CMap<CString, LPCTSTR, double, double> CStaticClass::m_ClientSymbolPosition_MT;


CMutex CStaticClass::m_mutex_order;
CMutex CStaticClass::m_mutex_deal;

CMutex CStaticClass::m_mutex_oSqlSession;
CMap<CString,LPCTSTR,int,int> CStaticClass::updatedData;


CMap<CString,LPCTSTR, CStaticClass::st_TotalTradedLotAndTOT, CStaticClass::st_TotalTradedLotAndTOT> CStaticClass::m_TotalLotAndTOT;


CStaticClass::requestResponseData CStaticClass::m_requestResponseData;
CStaticClass::requestResponseData CStaticClass::m_uniqueFiels;

CStaticClass::requestResponseData CStaticClass::m_tablesname;

CStaticClass::clientmasterarray CStaticClass::m_clientmasterarray;

CStaticClass::symbolmasterarray CStaticClass::m_symbolmasterarray;
CMap<CString, LPCTSTR, CString, CString> CStaticClass::m_symbolForTickData;
CMap<CString, LPCTSTR, CString, CString> CStaticClass::m_protoandjsonmessage;
CMap<CString, LPCTSTR, CStaticClass::stloginUserDetail, CStaticClass::stloginUserDetail> CStaticClass::m_loginuserlist;

CStaticClass::Orika_clientmasterHastable CStaticClass::m_Orika_clientmasterHastable;

CStaticClass::Orika_clientbrokerageHastable CStaticClass::m_Orika_clientbrokerageHastable;


CStaticClass::Orika_PLDevideRatioHastable CStaticClass::m_Orika_PLDevideRatioHastable;

CStaticClass::Orika_symbolgroupHastable CStaticClass::m_Orika_symbolgroupHastable;

CStaticClass::Orika_ClientgatewayconfigHastable CStaticClass::m_Orika_ClientgatewayconfigHastable;

CStaticClass::Orika_symbolmasterHastable CStaticClass::m_Orika_symbolmasterHastable;

CStaticClass::Orika_dealtableAccountingHastable CStaticClass::m_Orika_dealtableAccountingHastable;

CStaticClass::Orika_BalanceTableAccountingArray CStaticClass::m_Orika_BalanceTableAccountingArray;

CStaticClass::orika_positionEntryAccountingHastable CStaticClass::m_orika_positionEntryAccountingHastable;

CStaticClass::orika_PositionAverageAccountingHastable CStaticClass::m_orika_PositionAverageAccountingHastable;

CStaticClass::orika_positionEntryAccountingHastableLoginSymbolWise CStaticClass::m_orika_positionEntryAccountingHastableLoginSymbolWise_Buy;
CStaticClass::orika_positionEntryAccountingHastableLoginSymbolWise CStaticClass::m_orika_positionEntryAccountingHastableLoginSymbolWise_Sell;

CStaticClass::brokerageLoginSymbolWiseHastable CStaticClass::m_brokerageLoginSymbolWiseHastable;


CStaticClass::Orika_orderHastable CStaticClass::m_Orika_orderHastable;

CStaticClass::logindeviceArray CStaticClass::m_logindeviceArray;

CMap<int, int, int, int> CStaticClass::m_Orika_dealNO;

CMap<int, int, int, int> CStaticClass::m_OrikaOrderdealNO;

CMap<int, int, CString, CString > CStaticClass::MessageCodeList;


CMap<CString, LPCTSTR, CStaticClass::stConfig, CStaticClass::stConfig> CStaticClass::LimitConfig;

CStaticClass::TickData CStaticClass::m_TickData;
CStaticClass::TickData CStaticClass::m_updatedTickData;
CStaticClass::TickData CStaticClass::m_TickDataForContinousData;
CMutex CStaticClass::m_mutex_Tick;
CMutex CStaticClass::m_mutex_Tick_copy;
CMutex CStaticClass::m_mutex_Tick_TickSubscribe;
int CStaticClass::MSMQReaderStartStop=0;
int CStaticClass::TokenCheckingStartStop = 0;
int CStaticClass::MSMQReaderPause=0;
CLogFile CStaticClass::m_logfile;

CSqlData CStaticClass::m_sqldata;

CStaticClass::ClientList CStaticClass::m_ClientList;

CStaticClass::ClientList CStaticClass::m_ClientList_forOrder;
CStaticClass::ClientList CStaticClass::m_ClientList_forDeal;

CMutex CStaticClass::m_mutex_ClientList;


CMutex CStaticClass::m_mutex_dealingClientList;
int CStaticClass::startTickData=0;
int CStaticClass::startSendingNetpositionClientWise=0;

int CStaticClass::startCalculatingNetpositionClientWise=0;

int CStaticClass::heartBeatStart=0;
CStaticClass::ClientContext CStaticClass::m_ClientContext;


//CMutex CStaticClass::m_mutex_Thread;
CRITICAL_SECTION CStaticClass::m_cs_Thread;
CONDITION_VARIABLE CStaticClass::m_cv_Thread;


CMap<CString, LPCTSTR, CStaticClass::st_Tick, CStaticClass::st_Tick> CStaticClass::symbolLastTickArrayPrimary;

UINT64 CStaticClass::LastTickSendTime=0;




CStaticClass::st_TickBidAskLast*           CStaticClass::lastrateArray[35000];
CMap<CString,LPCTSTR,int ,int> CStaticClass::symbolLastTickArrayIndex;



double* CStaticClass::LpVolumeTotal[6000];
CMap<CString, LPCTSTR, int, int> CStaticClass::LpVolumeTotalArrayIndex;


double*           CStaticClass::clientgrosstotalArray[6000]; 	 
CMap<CString,LPCTSTR,int ,int> CStaticClass::clientgrosstotalArrayIndex;

double*           CStaticClass::clientbroktotalArray[6000]; 
CMap<CString,LPCTSTR,int ,int> CStaticClass::clientbroktotalArrayIndex;

double*           CStaticClass::clientnettotalArray[6000];
CMap<CString,LPCTSTR,int ,int> CStaticClass::clientnettotalArrayIndex;


CMap<CString,LPCTSTR,int ,int> CStaticClass::UpdatedsymbolLastTickArrayIndex;
CMap<CString,LPCTSTR,int ,int> CStaticClass::UpdatedsymbolLastTickArrayIndexForSymbolWisePosition;


CMap<CString,LPCTSTR,CStaticClass::st_netpositionClientWise,CStaticClass::st_netpositionClientWise&> CStaticClass::mapNetPositionClientWise;






CMap<CString,LPCTSTR,CStaticClass::loginarray ,CStaticClass::loginarray&> CStaticClass::mapSymbolandLogin;

int CStaticClass::lastrateArrayMaxindex=0;
int CStaticClass::clientgrosstotalArrayMaxindex=0;
int CStaticClass::clientbroktotalArrayMaxindex=0;
int CStaticClass::clientnettotalArrayMaxindex=0;
int CStaticClass::LpVolumeTotalArrayMaxIndex = 0;
int CStaticClass::CompanyVolumeAfterMultiTotalArrayMaxIndex = 0;


string CStaticClass::getcurrentTime()
{
	SYSTEMTIME	time;
	::GetLocalTime(&time);
	CString strLocalTime;
	strLocalTime.Format(L"%04d/%02d/%02d %02d:%02d:%02d:%03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
	string ssLocalTime = string(CT2CA(strLocalTime));
	return ssLocalTime;
}


CString CStaticClass::getcurrentTimeStr()
{
	SYSTEMTIME	time;
	::GetLocalTime(&time);
	CString strLocalTime;
	strLocalTime.Format(L"%02d:%02d:%02d:%03d",time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
	
	return strLocalTime;
}
void CStaticClass::set_protoandjsonmessage()
{
	m_protoandjsonmessage.SetAt(L"LOGIN_STATUS",L"loginresponse");
	m_protoandjsonmessage.SetAt(L"HB_S", L"hbs");
	m_protoandjsonmessage.SetAt(L"USER_DATA", L"userdata");
	m_protoandjsonmessage.SetAt(L"SUBBROKERDATA", L"subbrokerdata");
	m_protoandjsonmessage.SetAt(L"BROKERDATA", L"brokerdata");
	m_protoandjsonmessage.SetAt(L"CLIENTDATA", L"clientdata");
	m_protoandjsonmessage.SetAt(L"CLIENT_POSITION", L"clientposition");
	m_protoandjsonmessage.SetAt(L"DASHBOARD_DATA", L"clientposition");	
	m_protoandjsonmessage.SetAt(L"DEALING_DATA_INTERVAL", L"dealingdatainterval");
	m_protoandjsonmessage.SetAt(L"DASHBOARD", L"dashboard");
	m_protoandjsonmessage.SetAt(L"SYMBOLGROUPINFO", L"symbolgroupinfo");
	//m_protoandjsonmessage.SetAt(L"SYMBOLGROUPINFO", L"allsymbolgroupinfo");
	m_protoandjsonmessage.SetAt(L"CLIENTCREDIT", L"clientcredit");
	m_protoandjsonmessage.SetAt(L"SERVER_MEFETCH_USERS_DATASSAGE", L"servermessage");

	m_protoandjsonmessage.SetAt(L"SELECTED_THEME", L"selectedtheme");
	m_protoandjsonmessage.SetAt(L"PAGE_LIST", L"pagelist");
	m_protoandjsonmessage.SetAt(L"COLOR_THEMES", L"colorthemes");
	m_protoandjsonmessage.SetAt(L"PAGE_DETAILS", L"pagedetails");
	m_protoandjsonmessage.SetAt(L"CLIENT_MASTER_DATA", L"clientmasterdata");
	m_protoandjsonmessage.SetAt(L"SYMBOL_MASTER_DATA", L"symbolmastersata");
	m_protoandjsonmessage.SetAt(L"TICK_DATA", L"tickdata");	
	m_protoandjsonmessage.SetAt(L"COLOR_TEMPLATES", L"colortemplates");
	m_protoandjsonmessage.SetAt(L"TDH_RESPONSE_MESSAGE", L"tdhresponsemessage");
	m_protoandjsonmessage.SetAt(L"CLIENT_AND_SYMBOL", L"clientandsymbol");
	m_protoandjsonmessage.SetAt(L"CLIENT_META_DATA", L"metadata");
	m_protoandjsonmessage.SetAt(L"HIGH_LOW_MISMATCH_DATA", L"highlowmismatchdata");
	m_protoandjsonmessage.SetAt(L"ALL_GROUP_DATA", L"allgroupdata");
	m_protoandjsonmessage.SetAt(L"USERS_DATA", L"userdata");
	m_protoandjsonmessage.SetAt(L"USER_DETAILS", L"userdetails");
	m_protoandjsonmessage.SetAt(L"SERVER_MESSAGE", L"servermessage");
	m_protoandjsonmessage.SetAt(L"DEALING_DATA", L"dealingdata");
	m_protoandjsonmessage.SetAt(L"TOTAL_CLIENT_FOR_BALANCE_TRANSFER", L"totalclientforbalancetransfer");
	m_protoandjsonmessage.SetAt(L"BALANCE_TRANSFER_STATUS", L"balancetransferstatus");
	m_protoandjsonmessage.SetAt(L"BALANCE_TRANSFER_RESPONSE", L"balancetransferresponse");
	m_protoandjsonmessage.SetAt(L"POSITION_MATCH_REPORT", L"positionmatchreport");
	m_protoandjsonmessage.SetAt(L"TOTAL_NO_OF_DATA", L"totalnoofdata");
	m_protoandjsonmessage.SetAt(L"DELETE_DATA_STATUS", L"deletedatastatus");
	m_protoandjsonmessage.SetAt(L"DATA_DELETE_RESPONSE", L"datadeleteresponse");
	m_protoandjsonmessage.SetAt(L"TOTAL_DEAL_FOR_TRANSFER", L"totaldealfortransfer");
	m_protoandjsonmessage.SetAt(L"POSITIONTRANSFER_STATUS", L"positiontransferstatus");
	m_protoandjsonmessage.SetAt(L"POSITION_TRANSFER_RESPONSE", L"positiontransferresponse");
	m_protoandjsonmessage.SetAt(L"SAVE_PAGE_RES", L"savepageres");
	m_protoandjsonmessage.SetAt(L"DELETE_PAGE_RES", L"savepageres");
	m_protoandjsonmessage.SetAt(L"UPDATE_PAGE_RES", L"savepageres");	
	m_protoandjsonmessage.SetAt(L"ORDER_DATA", L"orderdata");
	m_protoandjsonmessage.SetAt(L"CLIENT_POSITION_META_DATA", L"metadata");
	m_protoandjsonmessage.SetAt(L"ORDER_ACTIVATE_RESPONSE", L"orderactivateresponse");
	m_protoandjsonmessage.SetAt(L"SYMBOLMAPPING_DATA", L"symbolmappingdata");
	m_protoandjsonmessage.SetAt(L"ALERT_MESSAGE", L"alertmessage");
	m_protoandjsonmessage.SetAt(L"SAVE_UPDATE_ALERT", L"alertsetting");	
	m_protoandjsonmessage.SetAt(L"LOGIN_DEVICE_LOG", L"logindevicelog");
	m_protoandjsonmessage.SetAt(L"ALERT_SETTING", L"alertsetting");	
	m_protoandjsonmessage.SetAt(L"ALERT_SETTING_LIST", L"alertsettinglist");
	m_protoandjsonmessage.SetAt(L"ALERT_EXECUTE_MESSAGE", L"alertexecutemessage");
	//alertsettinglist	
}


UINT64 CStaticClass::getcurrentTime_Unix()
{
	/*CString dateString = L"2023-02-27 10:15:00";
	COleDateTime dateTime;
	dateTime.ParseDateTime(dateString);

	SYSTEMTIME sysTime;
	dateTime.GetAsSystemTime(sysTime);

	UINT64 m_Time = 0;
	m_Time = SMTTime::STToTime(sysTime);*/



	SYSTEMTIME	time;
	::GetLocalTime(&time);
	UINT64 m_Time = 0; m_Time = SMTTime::STToTime(time);
	return m_Time;
}

UINT64 CStaticClass::getPreviousDateTime_Unix()
{
	/*CString dateString = L"2023-02-27 10:15:00";
	COleDateTime dateTime;
	dateTime.ParseDateTime(dateString);

	SYSTEMTIME sysTime;
	dateTime.GetAsSystemTime(sysTime);

	UINT64 m_Time = 0;
	m_Time = SMTTime::STToTime(sysTime);*/

	SYSTEMTIME	time;
	::GetLocalTime(&time);	
	time.wHour = 0;
	time.wMinute = 5;
	time.wSecond = 10;
	UINT64 m_Time = 0; m_Time = SMTTime::STToTime(time);
	return m_Time;
}


void CStaticClass::sendClientPosition_NewUpdate(SSL_session* client, int m_activeClient, CString strUserID, CString m_clientcontexKey, CString m_Requestmessage, CString m_messageType, NetPositionClientWise_All* mapNetPositionClientWise_ThreadWise, TMTArray<st_netpositionClientWise>* tmpNewposition)
{
	st_ClientContext st_Check = {};
	m_mutex_ClientList.Lock();
	m_ClientContext.Lookup(m_clientcontexKey, st_Check);
	m_mutex_ClientList.Unlock();
	CMap<CString, LPCTSTR, int, int&> m_subscribedcolumn_Local;
	int  totalColumns = st_Check.m_clientrequests_List.Total();
	for (int ci = 0; ci < totalColumns; ci++)
	{
		CString  m_ColumnsData = L"";
		m_ColumnsData = st_Check.m_clientrequests_List[ci];

		CString m_messageType_Subs = L"";
		m_messageType_Subs.Format(L":%s:", m_Requestmessage);

		if (m_ColumnsData.Find(m_messageType_Subs) >= 0)
		{
			int activate = 1;
			m_subscribedcolumn_Local.SetAt(m_ColumnsData, activate);
		}

	}
	m_mutex_ClientList.Lock();
	m_ClientContext.SetAt(m_clientcontexKey, st_Check);
	m_mutex_ClientList.Unlock();

	int jsonType = 0;
	int total_count = tmpNewposition->Total();
	CString strUpdateData = L"";
	int firstCheck = 0;
	CString str_columnJson = L"";
	CString str_FinalJsonUpdate = L"";

	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();



	string ssmessageType = string(CT2CA(m_messageType));
	const char* stmessageType = ssmessageType.c_str();
	if (total_count > 0)
	{
		writer.Key("type");
		writer.String(stmessageType);
		writer.Key("updatekey");
		writer.StartArray();
		writer.String("login");
		writer.String("symbol");
		writer.EndArray();
		writer.Key("update");		
		////(L"U1");
		writer.StartArray();
	}
	int dataSendingFlag = 0;

	for (int i = 0; i < total_count; i++)
	{
		//////(L"Data 5");
		dataSendingFlag = 1;

		CString strKey = L"";
		CStaticClass::st_netpositionClientWise st_tmpData = {};

		CStaticClass::m_mutex_Tick.Lock();
		tmpNewposition->Next(i, &st_tmpData);

		CString    m_login = st_tmpData.m_login;
		CString    m_name = st_tmpData.m_name;
		CString    m_symbol = st_tmpData.m_symbol;
		double     m_volume = *st_tmpData.m_volume;


		CString m_login_symbolKey = L"";
		m_login_symbolKey.Format(L"%s:%s", m_login, m_symbol);

		mapNetPositionClientWise_ThreadWise->Lookup(m_login_symbolKey, st_tmpData);
		double     m_previousvolume = st_tmpData.m_previousvolume;
		double     m_difference =  m_volume- st_tmpData.m_previousvolume;
		st_tmpData.m_difference = m_difference;
		mapNetPositionClientWise_ThreadWise->SetAt(m_login_symbolKey, st_tmpData);



		
		double     m_average = *st_tmpData.m_average;
		st_TickBidAskLast     m_lastrate = *st_tmpData.m_lastrate;

		double     m_clientbalance = *st_tmpData.m_clientbalance;
		double     m_clientnetamount = *st_tmpData.m_clientnetamount;

		double     m_clientgrosstotal = *st_tmpData.m_clientgrosstotal;
		double     m_clientbroktotal = *st_tmpData.m_clientbroktotal;
		double     m_clientnettotal = *st_tmpData.m_clientnettotal;
		double     m_maxallotedqty = *st_tmpData.m_maxallotedqty;
		CString    m_subbroker = st_tmpData.m_subbroker;
		CString    m_broker = st_tmpData.m_broker;

		double     m_extravolume = *st_tmpData.m_extravolume;
		double     m_freemargin = *st_tmpData.m_freemargin;
		double     m_multi = *st_tmpData.m_multi;

		CString     m_Company = st_tmpData.m_company;;

		double     m_companyvolume = *st_tmpData.m_companyvolume;
		double     m_brokervolume = *st_tmpData.m_brokervolume;
		double     m_subbrokervolume = *st_tmpData.m_subbrokervolume;
		double     m_clientbrokerage = *st_tmpData.m_clientBrokarage;
		double     m_brokerbrokerage = *st_tmpData.m_brokerBrokarage;

		double     m_subbrokerbrokerage = *st_tmpData.m_subbrokerBrokarage;
		double     m_companybrokerage = *st_tmpData.m_companyBrokarage;


		double     m_clientexposure = (*st_tmpData.m_clientexposure) / 10000000;
		double     m_Companyexposure = (*st_tmpData.m_companyexposure) / 10000000;
		double     m_brokerexposure = (*st_tmpData.m_brokerexposure) / 10000000;
		double     m_subbrokerexposure = (*st_tmpData.m_subbrokerexposure) / 10000000;
		m_clientexposure = abs(m_clientexposure);
		m_Companyexposure = abs(m_Companyexposure);
		m_brokerexposure = abs(m_brokerexposure);
		m_subbrokerexposure = abs(m_subbrokerexposure);

		double     m_clientfloatingpl = *st_tmpData.m_clientfloatingpl;
		double     m_companyfloatingpl = *st_tmpData.m_companyfloatingpl;
		double     m_brokerfloatingpl = *st_tmpData.m_brokerfloatingpl;
		double     m_subbrokerfloatingpl = *st_tmpData.m_subbrokerfloatingpl;
		double     m_clientbalancepl = *st_tmpData.m_clientbalance;
		double     m_companybalancepl = *st_tmpData.m_companybalance;
		double     m_brokerbalancepl = *st_tmpData.m_brokerbalance;
		double     m_subbrokerbalancepl = *st_tmpData.m_subbrokerbalance;
		double     m_clientplnet = *st_tmpData.m_clientnetamount;
		double     m_companyplnet = *st_tmpData.m_companyNetAmount;
		double     m_brokerplnet = *st_tmpData.m_brokerNetAmount;
		double     m_subbrokerplnet = *st_tmpData.m_subbrokerNetAmount;
		double     m_clientpatti = 100;
		double     m_companypatti = *st_tmpData.m_companyRatio;
		double     m_brokerpatti = *st_tmpData.m_brokerRatio;
		double     m_subbrokerpatti = *st_tmpData.m_subbrokerRatio;
		double     m_companybrokeragerate = *st_tmpData.m_companyBrokRate;
		double     m_brokerbrokeragerate = *st_tmpData.m_brokerBrokRate;
		double     m_subbrokerbrokeragerate = *st_tmpData.m_subBrokerBrokRate;
		double     m_clientbrokeragerate = *st_tmpData.m_clientBrokRate;

		double     m_clientgrossamount = *st_tmpData.m_ClientGrossAmount;

		double     m_brokergrossamount = ((*st_tmpData.m_ClientGrossAmount) * m_brokerpatti) / 100;
		double     m_subbrokergrossamount = ((*st_tmpData.m_ClientGrossAmount) * m_subbrokerpatti) / 100;
		double     m_companygrossamount = ((*st_tmpData.m_ClientGrossAmount) * m_companypatti) / 100;

		double     m_CreditLimit = *st_tmpData.m_creditLimit;

		/*double     m_brokergrossamount = *st_tmpData.m_BrokerGrossAmount;
		double     m_subbrokergrossamount = *st_tmpData.m_SubBrokerGrossAmount;
		double     m_companygrossamount = *st_tmpData.m_CompanyGrossAmount;*/

		CString  m_Exchange = st_tmpData.m_exchange;
		CString  m_international = st_tmpData.m_international;
		CString  m_sector = st_tmpData.m_sector;
		CString  m_industry = st_tmpData.m_industry;
		double   m_rmp = st_tmpData.m_rmp;
		double   m_QtyMulti = st_tmpData.m_QtyMulti;
		CString  m_page = st_tmpData.m_page;
		CString  m_categary = st_tmpData.m_categary;
		double   m_rm = st_tmpData.m_rm;
		double   m_qtyAfterMulti = st_tmpData.m_qtyAfterMulti;
		CString  m_buySell = st_tmpData.m_buySell;
		CString  m_debitCredit = st_tmpData.m_debitCredit;
		CString  m_currencybase = st_tmpData.m_currencybase;

		double m_lpratio = st_tmpData.m_LpRatio;
		double m_lpvolume = st_tmpData.m_LpVolume;

		CString m_commoditygroup = st_tmpData.m_commoditygroup;



		//m_login_symbolKey
		CStaticClass::st_TotalTradedLotAndTOT s_TLT = {};
		CStaticClass::m_TotalLotAndTOT.Lookup(m_login_symbolKey, s_TLT);
		double m_TotalTradedLot = s_TLT.m_TotalTradedLot;
		double m_TotalTradedTO = s_TLT.m_TotalTradedTO;
		m_TotalTradedTO = m_TotalTradedTO / 10000000;


		double m_TotalTradedLot_Buy = s_TLT.m_TotalBuyLot;
		double m_TotalTradedLot_Sell = s_TLT.m_TotalSellLot;


		CStaticClass::m_mutex_Tick.Unlock();
		writer.StartObject();

		if (jsonType == 0)
		{
			CString m_loginColumnKey = L"";
			int m_columnSubs = 0;
			CStaticClass::m_mutexcolumnSubs.Lock();

			writer.Key("login");
			string sslogin = string(CT2CA(m_login));
			const char* stlogin = sslogin.c_str();
			writer.String(stlogin);

			m_columnSubs = 0;
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"name", m_Requestmessage);
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("name");
				string ssname = string(CT2CA(m_name));
				const char* stName = ssname.c_str();
				writer.String(stName);
			}
			writer.Key("symbol");
			string sssymbol = string(CT2CA(m_symbol));
			const char* stsymbol = sssymbol.c_str();
			writer.String(stsymbol);
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"previousvolume", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("previousvolume");
				writer.Double(m_previousvolume);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"difference", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("difference");
				writer.Double(m_difference);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"volume", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("volume");
				writer.Double(m_volume);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"average", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("average");
				writer.Double(m_average);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"lastrate", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("lastrate");
				writer.Double(m_volume < 0 ? m_lastrate.m_ask : m_lastrate.m_bid);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientbalance", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientbalance");
				writer.Double(m_clientbalance);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientnetamount", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientnetamount");
				writer.Double(m_clientnetamount);
			}
			CString strLogString = L"";
			//strLogString.Format(L"%s,%s,%.2lf,%.2lf,%.2lf", m_login, m_symbol, *st_tmpData.m_ClientGrossAmount, *st_tmpData.m_clientBrokarage, *st_tmpData.m_clientnetamount);
			////(strLogString);
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientplnet", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientplnet");
				writer.Double(m_clientplnet);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companyplnet", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("companyplnet");
				writer.Double(m_companyplnet);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerplnet", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokerplnet");
				writer.Double(m_brokerplnet);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerplnet", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokerplnet");
				writer.Double(m_subbrokerplnet);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"maxallotedqty", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("maxallotedqty");
				writer.Double(m_maxallotedqty);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"totaltradedlot", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("totaltradedlot");
				writer.Double(m_TotalTradedLot);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"totaltradedto", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("totaltradedto");
				writer.Double(m_TotalTradedTO);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"company", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("company");
				string sscompany = string(CT2CA(m_Company));
				const char* stcompany = sscompany.c_str();
				writer.String(stcompany);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbroker", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbroker");
				string sssubbroker = string(CT2CA(m_subbroker));
				const char* stsubbroker = sssubbroker.c_str();
				writer.String(stsubbroker);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"broker", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("broker");
				string ssbroker = string(CT2CA(m_broker));
				const char* stbroker = ssbroker.c_str();
				writer.String(stbroker);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"extravolume", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("extravolume");
				writer.Double(m_extravolume);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"freemargin", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("freemargin");
				writer.Double(m_freemargin);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companyvolume", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("companyvolume");
				writer.Double(m_companyvolume);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokervolume", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokervolume");
				writer.Double(m_brokervolume);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokervolume", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokervolume");
				writer.Double(m_subbrokervolume);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientbrokerage", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientbrokerage");
				writer.Double(m_clientbrokerage);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerbrokerage", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokerbrokerage");
				writer.Double(m_brokerbrokerage);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerbrokerage", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokerbrokerage");
				writer.Double(m_subbrokerbrokerage);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companybrokerage", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("companybrokerage");
				writer.Double(m_companybrokerage);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientexposure", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientexposure");
				writer.Double(m_clientexposure);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"Companyexposure", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("Companyexposure");
				writer.Double(m_Companyexposure);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerexposure", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokerexposure");
				writer.Double(m_brokerexposure);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerexposure", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokerexposure");
				writer.Double(m_subbrokerexposure);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientfloatingpl", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientfloatingpl");
				writer.Double(m_clientfloatingpl);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companyfloatingpl", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("companyfloatingpl");
				writer.Double(m_companyfloatingpl);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerfloatingpl", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokerfloatingpl");
				writer.Double(m_brokerfloatingpl);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerfloatingpl", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokerfloatingpl");
				writer.Double(m_subbrokerfloatingpl);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientbalancepl", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientbalancepl");
				writer.Double(m_clientbalancepl);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companybalancepl", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("companybalancepl");
				writer.Double(m_companybalancepl);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerbalancepl", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokerbalancepl");
				writer.Double(m_brokerbalancepl);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerbalancepl", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokerbalancepl");
				writer.Double(m_subbrokerbalancepl);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientpatti", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientpatti");
				writer.Double(m_clientpatti);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companypatti", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("companypatti");
				writer.Double(m_companypatti);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerpatti", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokerpatti");
				writer.Double(m_brokerpatti);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerpatti", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokerpatti");
				writer.Double(m_subbrokerpatti);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companybrokeragerate", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("companybrokeragerate");
				writer.Double(m_companybrokeragerate);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerbrokeragerate", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokerbrokeragerate");
				writer.Double(m_brokerbrokeragerate);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerbrokeragerate", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokerbrokeragerate");
				writer.Double(m_subbrokerbrokeragerate);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientbrokeragerate", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientbrokeragerate");
				writer.Double(m_clientbrokeragerate);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientgrossamount", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientgrossamount");
				writer.Double(m_clientgrossamount);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokergrossamount", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokergrossamount");
				writer.Double(m_brokergrossamount);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokergrossamount", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokergrossamount");
				writer.Double(m_subbrokergrossamount);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companygrossamount", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("companygrossamount");
				writer.Double(m_companygrossamount);
			}

			int m_TotalNo_Of_StopBuy = 0;
			int m_TotalNo_Of_Cancelled_StopBuy = 0;
			int m_TotalNo_Of_Passed_StopBuy = 0;
			int m_TotalNo_Of_Pending_StopBuy = 0;

			int m_TotalNo_Of_StopSell = 0;
			int m_TotalNo_Of_Cancelled_StopSell = 0;
			int m_TotalNo_Of_Passed_StopSell = 0;
			int m_TotalNo_Of_Pending_StopSell = 0;

			int m_TotalNo_Of_BuyLimit = 0;
			int m_TotalNo_Of_Cancelled_BuyLimit = 0;
			int m_TotalNo_Of_Passed_BuyLimit = 0;
			int m_TotalNo_Of_Pending_BuyLimit = 0;

			int m_TotalNo_Of_SellLimit = 0;
			int m_TotalNo_Of_Cancelled_SellLimit = 0;
			int m_TotalNo_Of_Passed_SellLimit = 0;
			int m_TotalNo_Of_Pending_SellLimit = 0;



			int m_TotalNo_Of_MarketDeal = 0;
			int m_TotalNo_Of_LimitDeal = 0;
			int m_TotalNo_Of_StopLimitDeal = 0;




			CString strloginSymbolKey = L"";
			//strloginSymbolKey.Format(L"%I64u:%s:%d", m_login, m_symbol, m_stOrder_Check.m_type);
			strloginSymbolKey.Format(L"%s:%s:4", m_login, m_symbol);
			st_OrderCount m_stOrderCount = {};
			CStaticClass::m_OrderCountMap.Lookup(strloginSymbolKey, m_stOrderCount);
			m_TotalNo_Of_StopBuy = m_stOrderCount.m_TotalOrder;
			m_TotalNo_Of_Cancelled_StopBuy = m_stOrderCount.m_TotalCancelledOrder;
			m_TotalNo_Of_Passed_StopBuy = m_stOrderCount.m_TotalExecutedOrder;
			m_TotalNo_Of_Pending_StopBuy = m_TotalNo_Of_StopBuy - m_TotalNo_Of_Cancelled_StopBuy - m_TotalNo_Of_Passed_StopBuy;

			m_stOrderCount = {};
			strloginSymbolKey.Format(L"%s:%s:5", m_login, m_symbol);
			CStaticClass::m_OrderCountMap.Lookup(strloginSymbolKey, m_stOrderCount);
			m_TotalNo_Of_StopSell = m_stOrderCount.m_TotalOrder;
			m_TotalNo_Of_Cancelled_StopSell = m_stOrderCount.m_TotalCancelledOrder;
			m_TotalNo_Of_Passed_StopSell = m_stOrderCount.m_TotalExecutedOrder;
			m_TotalNo_Of_Pending_StopSell = m_TotalNo_Of_StopSell - m_TotalNo_Of_Cancelled_StopSell - m_TotalNo_Of_Passed_StopSell;

			m_stOrderCount = {};
			strloginSymbolKey.Format(L"%s:%s:2", m_login, m_symbol);
			CStaticClass::m_OrderCountMap.Lookup(strloginSymbolKey, m_stOrderCount);
			m_TotalNo_Of_BuyLimit = m_stOrderCount.m_TotalOrder;
			m_TotalNo_Of_Cancelled_BuyLimit = m_stOrderCount.m_TotalCancelledOrder;
			m_TotalNo_Of_Passed_BuyLimit = m_stOrderCount.m_TotalExecutedOrder;
			m_TotalNo_Of_Pending_BuyLimit = m_TotalNo_Of_BuyLimit - m_TotalNo_Of_Cancelled_BuyLimit - m_TotalNo_Of_Passed_BuyLimit;

			m_stOrderCount = {};
			strloginSymbolKey.Format(L"%s:%s:3", m_login, m_symbol);
			CStaticClass::m_OrderCountMap.Lookup(strloginSymbolKey, m_stOrderCount);
			m_TotalNo_Of_SellLimit = m_stOrderCount.m_TotalOrder;
			m_TotalNo_Of_Cancelled_SellLimit = m_stOrderCount.m_TotalCancelledOrder;
			m_TotalNo_Of_Passed_SellLimit = m_stOrderCount.m_TotalExecutedOrder;
			m_TotalNo_Of_Pending_SellLimit = m_TotalNo_Of_SellLimit - m_TotalNo_Of_Cancelled_SellLimit - m_TotalNo_Of_Passed_SellLimit;


			int totalMarketBuyDeal = 0;
			int totalMarketSellDeal = 0;
			m_stOrderCount = {};
			strloginSymbolKey.Format(L"%s:%s:0", m_login, m_symbol);
			CStaticClass::m_OrderCountMap.Lookup(strloginSymbolKey, m_stOrderCount);
			totalMarketBuyDeal = m_stOrderCount.m_TotalOrder;


			m_stOrderCount = {};
			strloginSymbolKey.Format(L"%s:%s:1", m_login, m_symbol);
			CStaticClass::m_OrderCountMap.Lookup(strloginSymbolKey, m_stOrderCount);
			totalMarketSellDeal = m_stOrderCount.m_TotalOrder;


			m_TotalNo_Of_MarketDeal = totalMarketBuyDeal + totalMarketSellDeal;
			m_TotalNo_Of_LimitDeal = m_TotalNo_Of_SellLimit + m_TotalNo_Of_BuyLimit;
			m_TotalNo_Of_StopLimitDeal = m_TotalNo_Of_StopBuy + m_TotalNo_Of_StopSell;

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_StopBuy", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_StopBuy");
				writer.Int(m_TotalNo_Of_StopBuy);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Cancelled_StopBuy", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Cancelled_StopBuy");
				writer.Int(m_TotalNo_Of_Cancelled_StopBuy);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Passed_StopBuy", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Passed_StopBuy");
				writer.Int(m_TotalNo_Of_Passed_StopBuy);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Pending_StopBuy", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Pending_StopBuy");
				writer.Int(m_TotalNo_Of_Pending_StopBuy);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_StopSell", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_StopSell");
				writer.Int(m_TotalNo_Of_StopSell);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Cancelled_StopSell", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Cancelled_StopSell");
				writer.Int(m_TotalNo_Of_Cancelled_StopSell);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Passed_StopSell", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Passed_StopSell");
				writer.Int(m_TotalNo_Of_Passed_StopSell);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Pending_StopSell", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Pending_StopSell");
				writer.Int(m_TotalNo_Of_Pending_StopSell);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_BuyLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_BuyLimit");
				writer.Int(m_TotalNo_Of_BuyLimit);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Cancelled_BuyLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Cancelled_BuyLimit");
				writer.Int(m_TotalNo_Of_Cancelled_BuyLimit);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Passed_BuyLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Passed_BuyLimit");
				writer.Int(m_TotalNo_Of_Passed_BuyLimit);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Pending_BuyLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Pending_BuyLimit");
				writer.Int(m_TotalNo_Of_Pending_BuyLimit);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_SellLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_SellLimit");
				writer.Int(m_TotalNo_Of_SellLimit);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Cancelled_SellLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Cancelled_SellLimit");
				writer.Int(m_TotalNo_Of_Cancelled_SellLimit);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Passed_SellLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Passed_SellLimit");
				writer.Int(m_TotalNo_Of_Passed_SellLimit);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Pending_SellLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Pending_SellLimit");
				writer.Int(m_TotalNo_Of_Pending_SellLimit);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_MarketDeal", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_MarketDeal");
				writer.Int(m_TotalNo_Of_MarketDeal);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_LimitDeal", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_LimitDeal");
				writer.Int(m_TotalNo_Of_LimitDeal);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_StopLimitDeal", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_StopLimitDeal");
				writer.Int(m_TotalNo_Of_StopLimitDeal);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalTradedLot_Buy", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalTradedLot_Buy");
				writer.Double(m_TotalTradedLot_Buy);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalTradedLot_Sell", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalTradedLot_Sell");
				writer.Double(m_TotalTradedLot_Sell);
			}

			CString  strLoginSymbolGroup = L"";
			strLoginSymbolGroup.Format(L"%s:%s", m_login, m_symbol);
			CStaticClass::st_BadTradeGoodTrade m_st_BadTradeGoodTrade = {};
			CStaticClass::m_BadTradeGoodTrade.Lookup(strLoginSymbolGroup, m_st_BadTradeGoodTrade);
			double m_badtrade = m_st_BadTradeGoodTrade.m_BadTrade;
			double m_TotalTrade = m_st_BadTradeGoodTrade.m_TotalTrade;
			double m_badtradePer = 0;
			if (m_badtrade != 0)
			{
				m_badtradePer = (m_badtrade / m_TotalTrade) * 100;
			}
			double m_badtradeIgnorePosition = m_st_BadTradeGoodTrade.m_badTradeIgnoringPosition;
			double m_badTradeIPCper = 0;
			if (m_badtradeIgnorePosition != 0)
			{
				m_badTradeIPCper = (m_badtradeIgnorePosition / m_TotalTrade) * 100;
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"badtradeper", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("badtradeper");
				writer.Double(m_badtradePer);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"badtradeipcper", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("badtradeipcper");
				writer.Double(m_badTradeIPCper);
			}

			int m_totaltradeipc = m_st_BadTradeGoodTrade.m_TradeIgnoringPosition;
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"totaltradeipc", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("totaltradeipc");
				writer.Double(m_totaltradeipc);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"totaltrade", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("totaltrade");
				writer.Double(m_TotalTrade);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"badtradeipc", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("badtradeipc");
				writer.Double(m_badtradeIgnorePosition);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"badtrade", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("badtrade");
				writer.Double(m_badtrade);
			}


			/*CString strLogVal = L"";
			strLogVal.Format(L"Login:%s Amount:%.2lf Debit/Credit:%s Volume:%.2lf GE:%.4lf", m_login, m_clientplnet, m_debitCredit, m_volume, m_clientexposure);
			//(strLogVal);*/
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"buysell", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("buysell");
				/*string ssbuySell = string(CT2CA(m_buySell));
				const char* strbuySell = ssbuySell.c_str();*/
				if (m_volume < 0)
				{
					writer.String("Sell");
				}
				else
				{
					writer.String("Buy");
				}
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"debitcredit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("debitcredit");
				/*string ssdebitCredit = string(CT2CA(m_debitCredit));
				const char* strdebitCredit = ssdebitCredit.c_str();*/
				if (m_clientplnet < 0)
				{
					writer.String("Debit");
				}
				else
				{
					writer.String("Credit");
				}

			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"exchange", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("exchange");
				string ssexchange = string(CT2CA(m_Exchange));
				const char* strexchange = ssexchange.c_str();
				writer.String(strexchange);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"international", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("international");
				string ssinternational = string(CT2CA(m_international));
				const char* strinternational = ssinternational.c_str();
				writer.String(strinternational);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"sector", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("sector");
				string sssector = string(CT2CA(m_sector));
				const char* strsector = sssector.c_str();
				writer.String(strsector);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"industry", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("industry");
				string ssindustry = string(CT2CA(m_industry));
				const char* strindustry = ssindustry.c_str();
				writer.String(strindustry);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"page", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("page");
				string sspage = string(CT2CA(m_page));
				const char* strpage = sspage.c_str();
				writer.String(strpage);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"categary", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("categary");
				string sscategary = string(CT2CA(m_categary));
				const char* strcategary = sscategary.c_str();
				writer.String(strcategary);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"rm", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("rm");
				writer.Double(m_rm);
			}


			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"lpratio", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("lpratio");
				writer.Double(m_lpratio);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"commoditygroup", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("commoditygroup");
				string sscommoditygroup = string(CT2CA(m_commoditygroup));
				const char* strcommoditygroup = sscommoditygroup.c_str();
				writer.String(strcommoditygroup);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"lpvolume", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("lpvolume");
				writer.Double(m_lpvolume);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"qtyaftermulti", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("qtyaftermulti");
				writer.Double(m_qtyAfterMulti);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"rmp", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("rmp");
				writer.Double(m_rmp);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"qtymulti", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("qtymulti");
				writer.Double(m_QtyMulti);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"currencybase", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("currencybase");
				string sscurrencybase = string(CT2CA(m_currencybase));
				const char* strcurrencybase = sscurrencybase.c_str();
				writer.String(strcurrencybase);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"creditLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("creditLimit");
				writer.Double(m_CreditLimit);
			}

			CStaticClass::stConfig m_stConfig = {};
			CString loginSymbolKey = L"";
			CStaticClass::m_mutex_LimitConfig.Lock();
			CString SymbolGroup = L"";
			if (m_symbol.GetLength() >= 5)
			{
				SymbolGroup = m_symbol.Mid(0, m_symbol.GetLength() - 3);
			}
			else
			{
				SymbolGroup = m_symbol;
			}
			int limitValue = 0;
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"BuyPositionLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				loginSymbolKey.Format(L"%s:%s:1", m_login, SymbolGroup);
				m_stConfig = {};
				CStaticClass::LimitConfig.Lookup(loginSymbolKey, m_stConfig);
				limitValue = m_stConfig.Limit;
				writer.Key("BuyPositionLimit");
				writer.Int(limitValue);
			}


			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"SellPositionLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				loginSymbolKey.Format(L"%s:%s:2", m_login, SymbolGroup);
				m_stConfig = {};
				CStaticClass::LimitConfig.Lookup(loginSymbolKey, m_stConfig);
				limitValue = m_stConfig.Limit;
				writer.Key("SellPositionLimit");
				writer.Int(limitValue);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"AllSymbolBuyPositionLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				loginSymbolKey.Format(L"%s:%s:3", m_login, L"SHARE");
				m_stConfig = {};
				CStaticClass::LimitConfig.Lookup(loginSymbolKey, m_stConfig);
				limitValue = m_stConfig.Limit;
				writer.Key("AllSymbolBuyPositionLimit");
				writer.Int(limitValue);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"AllSymbolSellPositionLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				loginSymbolKey.Format(L"%s:%s:4", m_login, L"SHARE");
				m_stConfig = {};
				CStaticClass::LimitConfig.Lookup(loginSymbolKey, m_stConfig);
				limitValue = m_stConfig.Limit;
				writer.Key("AllSymbolSellPositionLimit");
				writer.Int(limitValue);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"AllPositionLimitLoginwise", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				loginSymbolKey.Format(L"%s:%s:5", m_login, L"SHARE");
				m_stConfig = {};
				CStaticClass::LimitConfig.Lookup(loginSymbolKey, m_stConfig);
				limitValue = m_stConfig.Limit;
				writer.Key("AllPositionLimitLoginwise");
				writer.Int(limitValue);
			}
			CStaticClass::m_mutex_LimitConfig.Unlock();





			CStaticClass::m_mutexcolumnSubs.Unlock();


		}

		writer.EndObject();


		if (dataSendingFlag == 1)
		{
			string strforsend = "";
			str_FinalJsonUpdate = s.GetString();
			int dataSize = str_FinalJsonUpdate.GetLength();

			//if (dataSize >= 1500)
			if (dataSize >= 1500000)
			{
				writer.EndArray();
				writer.EndObject();

				str_FinalJsonUpdate = s.GetString();
				////(L"Test");
				
				strforsend = CT2A(str_FinalJsonUpdate.GetString());
				SendDataToClient(client, strforsend, m_clientcontexKey, m_activeClient);

				s.Clear();
				writer.Flush();

				writer.Reset(s);

				writer.StartObject();
				writer.Key("type");
				writer.String(stmessageType);
				writer.Key("updatekey");
				writer.StartArray();
				writer.String("login");
				writer.String("symbol");
				writer.EndArray();
				writer.Key("update");
				writer.StartArray();
				dataSendingFlag = 0;
			}
		}

	}

	if (dataSendingFlag == 1)
	{
		writer.EndArray();
		writer.EndObject();
		string strforsend = "";
		str_FinalJsonUpdate = s.GetString();
		strforsend = CT2A(str_FinalJsonUpdate.GetString());
		
		SendDataToClient(client, strforsend, m_clientcontexKey, m_activeClient);
		dataSendingFlag = 0;
	}
}

void CStaticClass::sendClientPosition_NewInsert(SSL_session* client, int m_activeClient, CString strUserID, CString m_clientcontexKey, CString m_Requestmessage, CString m_messageType, NetPositionClientWise_All* mapNetPositionClientWise_ThreadWise, TMTArray<st_netpositionClientWise>* tmpNewposition)
{
	int total_count = tmpNewposition->Total();
	/*if (total_count == 0)
	{
		return;
	}*/
	st_ClientContext st_Check = {};
	m_mutex_ClientList.Lock();
	m_ClientContext.Lookup(m_clientcontexKey, st_Check);	
	m_mutex_ClientList.Unlock();
	CMap<CString, LPCTSTR, int, int&> m_subscribedcolumn_Local;
	int  totalColumns = st_Check.m_clientrequests_List.Total();
	for (int ci = 0; ci < totalColumns; ci++)
	{
		CString  m_ColumnsData = L"";
		m_ColumnsData = st_Check.m_clientrequests_List[ci];

		CString m_messageType_Subs = L"";
		m_messageType_Subs.Format(L":%s:", m_Requestmessage);

		if (m_ColumnsData.Find(m_messageType_Subs) >= 0)
		{
			int activate = 1;
			m_subscribedcolumn_Local.SetAt(m_ColumnsData, activate);
		}

	}	
	m_mutex_ClientList.Lock();
	m_ClientContext.SetAt(m_clientcontexKey, st_Check);
	m_mutex_ClientList.Unlock();
		
	int jsonType = 0;	
	
	CString strUpdateData = L"";
	int firstCheck = 0;
	CString str_columnJson = L"";
	CString str_FinalJsonUpdate = L"";

	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();

	string ssmessageType = string(CT2CA(m_messageType));
	const char* stmessageType = ssmessageType.c_str();
	if (total_count > 0)
	{
		writer.Key("type");
		writer.String(stmessageType);
		writer.Key("insert");

		////(L"U1");

		writer.StartArray();
	}
		int dataSendingFlag = 0;
	
	for (int i=0;i< total_count;i++)
	{
		//////(L"Data 5");
		dataSendingFlag = 1;

		CString strKey = L"";
		CStaticClass::st_netpositionClientWise st_tmpData = {};

		CStaticClass::m_mutex_Tick.Lock();
		tmpNewposition->Next(i,&st_tmpData);

		CString    m_login = st_tmpData.m_login;
		CString    m_name = st_tmpData.m_name;
		CString    m_symbol = st_tmpData.m_symbol;
		double     m_volume = *st_tmpData.m_volume;

		
		CString m_login_symbolKey = L"";
		m_login_symbolKey.Format(L"%s:%s", m_login, m_symbol);

		mapNetPositionClientWise_ThreadWise->SetAt(m_login_symbolKey, st_tmpData);



		double     m_previousvolume = 0;
		double     m_difference = 0;
		double     m_average = *st_tmpData.m_average;
		st_TickBidAskLast     m_lastrate = *st_tmpData.m_lastrate;

		double     m_clientbalance = *st_tmpData.m_clientbalance;
		double     m_clientnetamount = *st_tmpData.m_clientnetamount;

		double     m_clientgrosstotal = *st_tmpData.m_clientgrosstotal;
		double     m_clientbroktotal = *st_tmpData.m_clientbroktotal;
		double     m_clientnettotal = *st_tmpData.m_clientnettotal;
		double     m_maxallotedqty = *st_tmpData.m_maxallotedqty;
		CString    m_subbroker = st_tmpData.m_subbroker;
		CString    m_broker = st_tmpData.m_broker;

		double     m_extravolume = *st_tmpData.m_extravolume;
		double     m_freemargin = *st_tmpData.m_freemargin;
		double     m_multi = *st_tmpData.m_multi;

		CString     m_Company = st_tmpData.m_company;;

		double     m_companyvolume = *st_tmpData.m_companyvolume;
		double     m_brokervolume = *st_tmpData.m_brokervolume;
		double     m_subbrokervolume = *st_tmpData.m_subbrokervolume;
		double     m_clientbrokerage = *st_tmpData.m_clientBrokarage;
		double     m_brokerbrokerage = *st_tmpData.m_brokerBrokarage;

		double     m_subbrokerbrokerage = *st_tmpData.m_subbrokerBrokarage;
		double     m_companybrokerage = *st_tmpData.m_companyBrokarage;


		double     m_clientexposure = (*st_tmpData.m_clientexposure) / 10000000;
		double     m_Companyexposure = (*st_tmpData.m_companyexposure) / 10000000;
		double     m_brokerexposure = (*st_tmpData.m_brokerexposure) / 10000000;
		double     m_subbrokerexposure = (*st_tmpData.m_subbrokerexposure) / 10000000;
		m_clientexposure = abs(m_clientexposure);
		m_Companyexposure = abs(m_Companyexposure);
		m_brokerexposure = abs(m_brokerexposure);
		m_subbrokerexposure = abs(m_subbrokerexposure);

		double     m_clientfloatingpl = *st_tmpData.m_clientfloatingpl;
		double     m_companyfloatingpl = *st_tmpData.m_companyfloatingpl;
		double     m_brokerfloatingpl = *st_tmpData.m_brokerfloatingpl;
		double     m_subbrokerfloatingpl = *st_tmpData.m_subbrokerfloatingpl;
		double     m_clientbalancepl = *st_tmpData.m_clientbalance;
		double     m_companybalancepl = *st_tmpData.m_companybalance;
		double     m_brokerbalancepl = *st_tmpData.m_brokerbalance;
		double     m_subbrokerbalancepl = *st_tmpData.m_subbrokerbalance;
		double     m_clientplnet = *st_tmpData.m_clientnetamount;
		double     m_companyplnet = *st_tmpData.m_companyNetAmount;
		double     m_brokerplnet = *st_tmpData.m_brokerNetAmount;
		double     m_subbrokerplnet = *st_tmpData.m_subbrokerNetAmount;
		double     m_clientpatti = 100;
		double     m_companypatti = *st_tmpData.m_companyRatio;
		double     m_brokerpatti = *st_tmpData.m_brokerRatio;
		double     m_subbrokerpatti = *st_tmpData.m_subbrokerRatio;
		double     m_companybrokeragerate = *st_tmpData.m_companyBrokRate;
		double     m_brokerbrokeragerate = *st_tmpData.m_brokerBrokRate;
		double     m_subbrokerbrokeragerate = *st_tmpData.m_subBrokerBrokRate;
		double     m_clientbrokeragerate = *st_tmpData.m_clientBrokRate;

		double     m_clientgrossamount = *st_tmpData.m_ClientGrossAmount;

		double     m_brokergrossamount = ((*st_tmpData.m_ClientGrossAmount) * m_brokerpatti) / 100;
		double     m_subbrokergrossamount = ((*st_tmpData.m_ClientGrossAmount) * m_subbrokerpatti) / 100;
		double     m_companygrossamount = ((*st_tmpData.m_ClientGrossAmount) * m_companypatti) / 100;

		double     m_CreditLimit = *st_tmpData.m_creditLimit;

		/*double     m_brokergrossamount = *st_tmpData.m_BrokerGrossAmount;
		double     m_subbrokergrossamount = *st_tmpData.m_SubBrokerGrossAmount;
		double     m_companygrossamount = *st_tmpData.m_CompanyGrossAmount;*/

		CString  m_Exchange = st_tmpData.m_exchange;
		CString  m_international = st_tmpData.m_international;
		CString  m_sector = st_tmpData.m_sector;
		CString  m_industry = st_tmpData.m_industry;
		double   m_rmp = st_tmpData.m_rmp;
		double   m_QtyMulti = st_tmpData.m_QtyMulti;
		CString  m_page = st_tmpData.m_page;
		CString  m_categary = st_tmpData.m_categary;
		double   m_rm = st_tmpData.m_rm;
		double   m_qtyAfterMulti = st_tmpData.m_qtyAfterMulti;
		CString  m_buySell = st_tmpData.m_buySell;
		CString  m_debitCredit = st_tmpData.m_debitCredit;
		CString  m_currencybase = st_tmpData.m_currencybase;

		double m_lpratio = st_tmpData.m_LpRatio;
		double m_lpvolume = st_tmpData.m_LpVolume;

		CString m_commoditygroup = st_tmpData.m_commoditygroup;



		//m_login_symbolKey
		CStaticClass::st_TotalTradedLotAndTOT s_TLT = {};
		CStaticClass::m_TotalLotAndTOT.Lookup(m_login_symbolKey, s_TLT);
		double m_TotalTradedLot = s_TLT.m_TotalTradedLot;
		double m_TotalTradedTO = s_TLT.m_TotalTradedTO;
		m_TotalTradedTO = m_TotalTradedTO / 10000000;


		double m_TotalTradedLot_Buy = s_TLT.m_TotalBuyLot;
		double m_TotalTradedLot_Sell = s_TLT.m_TotalSellLot;


		CStaticClass::m_mutex_Tick.Unlock();
		writer.StartObject();

		if (jsonType == 0)
		{
			CString m_loginColumnKey = L"";
			int m_columnSubs = 0;
			CStaticClass::m_mutexcolumnSubs.Lock();

			writer.Key("login");
			string sslogin = string(CT2CA(m_login));
			const char* stlogin = sslogin.c_str();
			writer.String(stlogin);

			m_columnSubs = 0;
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"name", m_Requestmessage);
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("name");
				string ssname = string(CT2CA(m_name));
				const char* stName = ssname.c_str();
				writer.String(stName);
			}
			writer.Key("symbol");
			string sssymbol = string(CT2CA(m_symbol));
			const char* stsymbol = sssymbol.c_str();
			writer.String(stsymbol);
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"previousvolume", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("previousvolume");
				writer.Double(m_previousvolume);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"difference", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("difference");
				writer.Double(m_difference);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"volume", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("volume");
				writer.Double(m_volume);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"average", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("average");
				writer.Double(m_average);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"lastrate", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("lastrate");
				writer.Double(m_volume < 0 ? m_lastrate.m_ask : m_lastrate.m_bid);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientbalance", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientbalance");
				writer.Double(m_clientbalance);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientnetamount", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientnetamount");
				writer.Double(m_clientnetamount);
			}
			CString strLogString = L"";
			//strLogString.Format(L"%s,%s,%.2lf,%.2lf,%.2lf", m_login, m_symbol, *st_tmpData.m_ClientGrossAmount, *st_tmpData.m_clientBrokarage, *st_tmpData.m_clientnetamount);
			////(strLogString);
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientplnet", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientplnet");
				writer.Double(m_clientplnet);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companyplnet", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("companyplnet");
				writer.Double(m_companyplnet);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerplnet", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokerplnet");
				writer.Double(m_brokerplnet);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerplnet", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokerplnet");
				writer.Double(m_subbrokerplnet);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"maxallotedqty", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("maxallotedqty");
				writer.Double(m_maxallotedqty);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"totaltradedlot", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("totaltradedlot");
				writer.Double(m_TotalTradedLot);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"totaltradedto", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("totaltradedto");
				writer.Double(m_TotalTradedTO);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"company", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("company");
				string sscompany = string(CT2CA(m_Company));
				const char* stcompany = sscompany.c_str();
				writer.String(stcompany);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbroker", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbroker");
				string sssubbroker = string(CT2CA(m_subbroker));
				const char* stsubbroker = sssubbroker.c_str();
				writer.String(stsubbroker);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"broker", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("broker");
				string ssbroker = string(CT2CA(m_broker));
				const char* stbroker = ssbroker.c_str();
				writer.String(stbroker);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"extravolume", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("extravolume");
				writer.Double(m_extravolume);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"freemargin", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("freemargin");
				writer.Double(m_freemargin);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companyvolume", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("companyvolume");
				writer.Double(m_companyvolume);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokervolume", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokervolume");
				writer.Double(m_brokervolume);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokervolume", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokervolume");
				writer.Double(m_subbrokervolume);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientbrokerage", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientbrokerage");
				writer.Double(m_clientbrokerage);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerbrokerage", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokerbrokerage");
				writer.Double(m_brokerbrokerage);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerbrokerage", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokerbrokerage");
				writer.Double(m_subbrokerbrokerage);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companybrokerage", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("companybrokerage");
				writer.Double(m_companybrokerage);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientexposure", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientexposure");
				writer.Double(m_clientexposure);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"Companyexposure", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("Companyexposure");
				writer.Double(m_Companyexposure);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerexposure", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokerexposure");
				writer.Double(m_brokerexposure);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerexposure", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokerexposure");
				writer.Double(m_subbrokerexposure);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientfloatingpl", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientfloatingpl");
				writer.Double(m_clientfloatingpl);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companyfloatingpl", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("companyfloatingpl");
				writer.Double(m_companyfloatingpl);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerfloatingpl", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokerfloatingpl");
				writer.Double(m_brokerfloatingpl);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerfloatingpl", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokerfloatingpl");
				writer.Double(m_subbrokerfloatingpl);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientbalancepl", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientbalancepl");
				writer.Double(m_clientbalancepl);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companybalancepl", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("companybalancepl");
				writer.Double(m_companybalancepl);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerbalancepl", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokerbalancepl");
				writer.Double(m_brokerbalancepl);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerbalancepl", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokerbalancepl");
				writer.Double(m_subbrokerbalancepl);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientpatti", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientpatti");
				writer.Double(m_clientpatti);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companypatti", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("companypatti");
				writer.Double(m_companypatti);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerpatti", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokerpatti");
				writer.Double(m_brokerpatti);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerpatti", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokerpatti");
				writer.Double(m_subbrokerpatti);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companybrokeragerate", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("companybrokeragerate");
				writer.Double(m_companybrokeragerate);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerbrokeragerate", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokerbrokeragerate");
				writer.Double(m_brokerbrokeragerate);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerbrokeragerate", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokerbrokeragerate");
				writer.Double(m_subbrokerbrokeragerate);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientbrokeragerate", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientbrokeragerate");
				writer.Double(m_clientbrokeragerate);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientgrossamount", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientgrossamount");
				writer.Double(m_clientgrossamount);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokergrossamount", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokergrossamount");
				writer.Double(m_brokergrossamount);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokergrossamount", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokergrossamount");
				writer.Double(m_subbrokergrossamount);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companygrossamount", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("companygrossamount");
				writer.Double(m_companygrossamount);
			}

			int m_TotalNo_Of_StopBuy = 0;
			int m_TotalNo_Of_Cancelled_StopBuy = 0;
			int m_TotalNo_Of_Passed_StopBuy = 0;
			int m_TotalNo_Of_Pending_StopBuy = 0;

			int m_TotalNo_Of_StopSell = 0;
			int m_TotalNo_Of_Cancelled_StopSell = 0;
			int m_TotalNo_Of_Passed_StopSell = 0;
			int m_TotalNo_Of_Pending_StopSell = 0;

			int m_TotalNo_Of_BuyLimit = 0;
			int m_TotalNo_Of_Cancelled_BuyLimit = 0;
			int m_TotalNo_Of_Passed_BuyLimit = 0;
			int m_TotalNo_Of_Pending_BuyLimit = 0;

			int m_TotalNo_Of_SellLimit = 0;
			int m_TotalNo_Of_Cancelled_SellLimit = 0;
			int m_TotalNo_Of_Passed_SellLimit = 0;
			int m_TotalNo_Of_Pending_SellLimit = 0;



			int m_TotalNo_Of_MarketDeal = 0;
			int m_TotalNo_Of_LimitDeal = 0;
			int m_TotalNo_Of_StopLimitDeal = 0;




			CString strloginSymbolKey = L"";
			//strloginSymbolKey.Format(L"%I64u:%s:%d", m_login, m_symbol, m_stOrder_Check.m_type);
			strloginSymbolKey.Format(L"%s:%s:4", m_login, m_symbol);
			st_OrderCount m_stOrderCount = {};
			CStaticClass::m_OrderCountMap.Lookup(strloginSymbolKey, m_stOrderCount);
			m_TotalNo_Of_StopBuy = m_stOrderCount.m_TotalOrder;
			m_TotalNo_Of_Cancelled_StopBuy = m_stOrderCount.m_TotalCancelledOrder;
			m_TotalNo_Of_Passed_StopBuy = m_stOrderCount.m_TotalExecutedOrder;
			m_TotalNo_Of_Pending_StopBuy = m_TotalNo_Of_StopBuy - m_TotalNo_Of_Cancelled_StopBuy - m_TotalNo_Of_Passed_StopBuy;

			m_stOrderCount = {};
			strloginSymbolKey.Format(L"%s:%s:5", m_login, m_symbol);
			CStaticClass::m_OrderCountMap.Lookup(strloginSymbolKey, m_stOrderCount);
			m_TotalNo_Of_StopSell = m_stOrderCount.m_TotalOrder;
			m_TotalNo_Of_Cancelled_StopSell = m_stOrderCount.m_TotalCancelledOrder;
			m_TotalNo_Of_Passed_StopSell = m_stOrderCount.m_TotalExecutedOrder;
			m_TotalNo_Of_Pending_StopSell = m_TotalNo_Of_StopSell - m_TotalNo_Of_Cancelled_StopSell - m_TotalNo_Of_Passed_StopSell;

			m_stOrderCount = {};
			strloginSymbolKey.Format(L"%s:%s:2", m_login, m_symbol);
			CStaticClass::m_OrderCountMap.Lookup(strloginSymbolKey, m_stOrderCount);
			m_TotalNo_Of_BuyLimit = m_stOrderCount.m_TotalOrder;
			m_TotalNo_Of_Cancelled_BuyLimit = m_stOrderCount.m_TotalCancelledOrder;
			m_TotalNo_Of_Passed_BuyLimit = m_stOrderCount.m_TotalExecutedOrder;
			m_TotalNo_Of_Pending_BuyLimit = m_TotalNo_Of_BuyLimit - m_TotalNo_Of_Cancelled_BuyLimit - m_TotalNo_Of_Passed_BuyLimit;

			m_stOrderCount = {};
			strloginSymbolKey.Format(L"%s:%s:3", m_login, m_symbol);
			CStaticClass::m_OrderCountMap.Lookup(strloginSymbolKey, m_stOrderCount);
			m_TotalNo_Of_SellLimit = m_stOrderCount.m_TotalOrder;
			m_TotalNo_Of_Cancelled_SellLimit = m_stOrderCount.m_TotalCancelledOrder;
			m_TotalNo_Of_Passed_SellLimit = m_stOrderCount.m_TotalExecutedOrder;
			m_TotalNo_Of_Pending_SellLimit = m_TotalNo_Of_SellLimit - m_TotalNo_Of_Cancelled_SellLimit - m_TotalNo_Of_Passed_SellLimit;


			int totalMarketBuyDeal = 0;
			int totalMarketSellDeal = 0;
			m_stOrderCount = {};
			strloginSymbolKey.Format(L"%s:%s:0", m_login, m_symbol);
			CStaticClass::m_OrderCountMap.Lookup(strloginSymbolKey, m_stOrderCount);
			totalMarketBuyDeal = m_stOrderCount.m_TotalOrder;


			m_stOrderCount = {};
			strloginSymbolKey.Format(L"%s:%s:1", m_login, m_symbol);
			CStaticClass::m_OrderCountMap.Lookup(strloginSymbolKey, m_stOrderCount);
			totalMarketSellDeal = m_stOrderCount.m_TotalOrder;


			m_TotalNo_Of_MarketDeal = totalMarketBuyDeal + totalMarketSellDeal;
			m_TotalNo_Of_LimitDeal = m_TotalNo_Of_SellLimit + m_TotalNo_Of_BuyLimit;
			m_TotalNo_Of_StopLimitDeal = m_TotalNo_Of_StopBuy + m_TotalNo_Of_StopSell;

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_StopBuy", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_StopBuy");
				writer.Int(m_TotalNo_Of_StopBuy);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Cancelled_StopBuy", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Cancelled_StopBuy");
				writer.Int(m_TotalNo_Of_Cancelled_StopBuy);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Passed_StopBuy", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Passed_StopBuy");
				writer.Int(m_TotalNo_Of_Passed_StopBuy);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Pending_StopBuy", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Pending_StopBuy");
				writer.Int(m_TotalNo_Of_Pending_StopBuy);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_StopSell", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_StopSell");
				writer.Int(m_TotalNo_Of_StopSell);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Cancelled_StopSell", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Cancelled_StopSell");
				writer.Int(m_TotalNo_Of_Cancelled_StopSell);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Passed_StopSell", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Passed_StopSell");
				writer.Int(m_TotalNo_Of_Passed_StopSell);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Pending_StopSell", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Pending_StopSell");
				writer.Int(m_TotalNo_Of_Pending_StopSell);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_BuyLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_BuyLimit");
				writer.Int(m_TotalNo_Of_BuyLimit);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Cancelled_BuyLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Cancelled_BuyLimit");
				writer.Int(m_TotalNo_Of_Cancelled_BuyLimit);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Passed_BuyLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Passed_BuyLimit");
				writer.Int(m_TotalNo_Of_Passed_BuyLimit);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Pending_BuyLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Pending_BuyLimit");
				writer.Int(m_TotalNo_Of_Pending_BuyLimit);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_SellLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_SellLimit");
				writer.Int(m_TotalNo_Of_SellLimit);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Cancelled_SellLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Cancelled_SellLimit");
				writer.Int(m_TotalNo_Of_Cancelled_SellLimit);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Passed_SellLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Passed_SellLimit");
				writer.Int(m_TotalNo_Of_Passed_SellLimit);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Pending_SellLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Pending_SellLimit");
				writer.Int(m_TotalNo_Of_Pending_SellLimit);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_MarketDeal", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_MarketDeal");
				writer.Int(m_TotalNo_Of_MarketDeal);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_LimitDeal", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_LimitDeal");
				writer.Int(m_TotalNo_Of_LimitDeal);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_StopLimitDeal", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_StopLimitDeal");
				writer.Int(m_TotalNo_Of_StopLimitDeal);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalTradedLot_Buy", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalTradedLot_Buy");
				writer.Double(m_TotalTradedLot_Buy);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalTradedLot_Sell", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalTradedLot_Sell");
				writer.Double(m_TotalTradedLot_Sell);
			}

			CString  strLoginSymbolGroup = L"";
			strLoginSymbolGroup.Format(L"%s:%s", m_login, m_symbol);
			CStaticClass::st_BadTradeGoodTrade m_st_BadTradeGoodTrade = {};
			CStaticClass::m_BadTradeGoodTrade.Lookup(strLoginSymbolGroup, m_st_BadTradeGoodTrade);
			double m_badtrade = m_st_BadTradeGoodTrade.m_BadTrade;
			double m_TotalTrade = m_st_BadTradeGoodTrade.m_TotalTrade;
			double m_badtradePer = 0;
			if (m_badtrade != 0)
			{
				m_badtradePer = (m_badtrade / m_TotalTrade) * 100;
			}
			double m_badtradeIgnorePosition = m_st_BadTradeGoodTrade.m_badTradeIgnoringPosition;
			double m_badTradeIPCper = 0;
			if (m_badtradeIgnorePosition != 0)
			{
				m_badTradeIPCper = (m_badtradeIgnorePosition / m_TotalTrade) * 100;
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"badtradeper", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("badtradeper");
				writer.Double(m_badtradePer);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"badtradeipcper", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("badtradeipcper");
				writer.Double(m_badTradeIPCper);
			}

			int m_totaltradeipc = m_st_BadTradeGoodTrade.m_TradeIgnoringPosition;
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"totaltradeipc", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("totaltradeipc");
				writer.Double(m_totaltradeipc);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"totaltrade", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("totaltrade");
				writer.Double(m_TotalTrade);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"badtradeipc", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("badtradeipc");
				writer.Double(m_badtradeIgnorePosition);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"badtrade", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("badtrade");
				writer.Double(m_badtrade);
			}


			/*CString strLogVal = L"";
			strLogVal.Format(L"Login:%s Amount:%.2lf Debit/Credit:%s Volume:%.2lf GE:%.4lf", m_login, m_clientplnet, m_debitCredit, m_volume, m_clientexposure);
			//(strLogVal);*/
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"buysell", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("buysell");
				/*string ssbuySell = string(CT2CA(m_buySell));
				const char* strbuySell = ssbuySell.c_str();*/
				if (m_volume < 0)
				{
					writer.String("Sell");
				}
				else
				{
					writer.String("Buy");
				}
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"debitcredit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("debitcredit");
				/*string ssdebitCredit = string(CT2CA(m_debitCredit));
				const char* strdebitCredit = ssdebitCredit.c_str();*/
				if (m_clientplnet < 0)
				{
					writer.String("Debit");
				}
				else
				{
					writer.String("Credit");
				}

			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"exchange", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("exchange");
				string ssexchange = string(CT2CA(m_Exchange));
				const char* strexchange = ssexchange.c_str();
				writer.String(strexchange);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"international", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("international");
				string ssinternational = string(CT2CA(m_international));
				const char* strinternational = ssinternational.c_str();
				writer.String(strinternational);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"sector", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("sector");
				string sssector = string(CT2CA(m_sector));
				const char* strsector = sssector.c_str();
				writer.String(strsector);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"industry", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("industry");
				string ssindustry = string(CT2CA(m_industry));
				const char* strindustry = ssindustry.c_str();
				writer.String(strindustry);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"page", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("page");
				string sspage = string(CT2CA(m_page));
				const char* strpage = sspage.c_str();
				writer.String(strpage);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"categary", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("categary");
				string sscategary = string(CT2CA(m_categary));
				const char* strcategary = sscategary.c_str();
				writer.String(strcategary);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"rm", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("rm");
				writer.Double(m_rm);
			}


			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"lpratio", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("lpratio");
				writer.Double(m_lpratio);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"commoditygroup", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("commoditygroup");
				string sscommoditygroup = string(CT2CA(m_commoditygroup));
				const char* strcommoditygroup = sscommoditygroup.c_str();
				writer.String(strcommoditygroup);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"lpvolume", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("lpvolume");
				writer.Double(m_lpvolume);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"qtyaftermulti", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("qtyaftermulti");
				writer.Double(m_qtyAfterMulti);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"rmp", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("rmp");
				writer.Double(m_rmp);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"qtymulti", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("qtymulti");
				writer.Double(m_QtyMulti);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"currencybase", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("currencybase");
				string sscurrencybase = string(CT2CA(m_currencybase));
				const char* strcurrencybase = sscurrencybase.c_str();
				writer.String(strcurrencybase);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"creditLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("creditLimit");
				writer.Double(m_CreditLimit);
			}

			CStaticClass::stConfig m_stConfig = {};
			CString loginSymbolKey = L"";
			CStaticClass::m_mutex_LimitConfig.Lock();
			CString SymbolGroup = L"";
			if (m_symbol.GetLength() >= 5)
			{
				SymbolGroup = m_symbol.Mid(0, m_symbol.GetLength() - 3);
			}
			else
			{
				SymbolGroup = m_symbol;
			}
			int limitValue = 0;
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"BuyPositionLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				loginSymbolKey.Format(L"%s:%s:1", m_login, SymbolGroup);
				m_stConfig = {};
				CStaticClass::LimitConfig.Lookup(loginSymbolKey, m_stConfig);
				limitValue = m_stConfig.Limit;
				writer.Key("BuyPositionLimit");
				writer.Int(limitValue);
			}


			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"SellPositionLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				loginSymbolKey.Format(L"%s:%s:2", m_login, SymbolGroup);
				m_stConfig = {};
				CStaticClass::LimitConfig.Lookup(loginSymbolKey, m_stConfig);
				limitValue = m_stConfig.Limit;
				writer.Key("SellPositionLimit");
				writer.Int(limitValue);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"AllSymbolBuyPositionLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				loginSymbolKey.Format(L"%s:%s:3", m_login, L"SHARE");
				m_stConfig = {};
				CStaticClass::LimitConfig.Lookup(loginSymbolKey, m_stConfig);
				limitValue = m_stConfig.Limit;
				writer.Key("AllSymbolBuyPositionLimit");
				writer.Int(limitValue);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"AllSymbolSellPositionLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				loginSymbolKey.Format(L"%s:%s:4", m_login, L"SHARE");
				m_stConfig = {};
				CStaticClass::LimitConfig.Lookup(loginSymbolKey, m_stConfig);
				limitValue = m_stConfig.Limit;
				writer.Key("AllSymbolSellPositionLimit");
				writer.Int(limitValue);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"AllPositionLimitLoginwise", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				loginSymbolKey.Format(L"%s:%s:5", m_login, L"SHARE");
				m_stConfig = {};
				CStaticClass::LimitConfig.Lookup(loginSymbolKey, m_stConfig);
				limitValue = m_stConfig.Limit;
				writer.Key("AllPositionLimitLoginwise");
				writer.Int(limitValue);
			}
			CStaticClass::m_mutex_LimitConfig.Unlock();





			CStaticClass::m_mutexcolumnSubs.Unlock();


		}

		writer.EndObject();


		if (dataSendingFlag == 1)
		{
			string strforsend = "";
			str_FinalJsonUpdate = s.GetString();
			int dataSize = str_FinalJsonUpdate.GetLength();

			//if (dataSize >= 1500)
			if (dataSize >= 1500000)
			{
				writer.EndArray();
				writer.EndObject();

				str_FinalJsonUpdate = s.GetString();
				////(L"Test");
				//(str_FinalJsonUpdate);
				strforsend = CT2A(str_FinalJsonUpdate.GetString());
				SendDataToClient(client, strforsend, m_clientcontexKey, m_activeClient);

				s.Clear();
				writer.Flush();

				writer.Reset(s);

				writer.StartObject();
				writer.Key("type");
				writer.String(stmessageType);
				writer.Key("insert");
				writer.StartArray();
				dataSendingFlag = 0;
			}
		}

	}

	if (dataSendingFlag == 1)
	{
		writer.EndArray();
		writer.EndObject();
		string strforsend = "";
		str_FinalJsonUpdate = s.GetString();
		
		strforsend = CT2A(str_FinalJsonUpdate.GetString());
		SendDataToClient(client, strforsend, m_clientcontexKey, m_activeClient);		
		dataSendingFlag = 0;
	}
}
void CStaticClass::sendClientPosition_Update(SSL_session* client, int m_activeClient, CString strUserID, CString m_clientcontexKey, CString m_Requestmessage, CString m_messageType, NetPositionClientWise_All* mapNetPositionClientWise_ThreadWise)
{
	st_ClientContext st_Check = {};
	m_mutex_ClientList.Lock();
	m_ClientContext.Lookup(m_clientcontexKey, st_Check);
	m_mutex_ClientList.Unlock();
	CMap<CString, LPCTSTR, int, int&> m_subscribedcolumn_Local;
	int  totalColumns = st_Check.m_clientrequests_List.Total();
	for (int ci = 0; ci < totalColumns; ci++)
	{
		CString  m_ColumnsData = L"";
		m_ColumnsData = st_Check.m_clientrequests_List[ci];

		CString m_messageType_Subs = L"";
		m_messageType_Subs.Format(L":%s:", m_Requestmessage);

		if (m_ColumnsData.Find(m_messageType_Subs) >= 0)
		{
			int activate = 1;
			m_subscribedcolumn_Local.SetAt(m_ColumnsData, activate);
		}

	}
	//Getting PreNetQty				
	CString returnval = L"";
	HRESULT hr = NULL;
	CCommand<CAccessor<CNetpositionPreQtyTable>> data_table;
	if (!SUCCEEDED(hr))
	{
		return;
	}
	CString   strCommand = L"";

	UINT64 m_preVolumeTime = 0;
	if (m_Requestmessage == L"FETCH_DASHBOARD_DATA")
	{
		m_preVolumeTime = st_Check.m_DashboardTime;
	}
	else
	{
		m_preVolumeTime = st_Check.m_netPositionTime;
	}
	if (m_preVolumeTime == 0)
	{
		m_preVolumeTime = getPreviousDateTime_Unix();
	}

	strCommand.Format(L"orika_GetClientPreviousNetQty '%I64u';", m_preVolumeTime);

	//CStaticClass::m_mutex_order.Lock();				
	//(L"L13");
	CSession m_tmpSession;
	m_tmpSession.Open(CStaticClass::connection);
	hr = data_table.Open(m_tmpSession, (LPCTSTR)strCommand);
	if (FAILED(hr))
	{
		m_tmpSession.Close();
	}
	int i = 0;
	CString m_login = L"";
	CString m_symbol = L"";
	double m_volume = 0;

	CString tmpstr = L"";
	int row_count = 0;
	//hr=data_table.MoveNext();

	struct stPrevolume
	{
		wchar_t m_login[30];
		wchar_t m_symbol[32];
		double m_volume;
	};
	CMap<CString, LPCTSTR, stPrevolume, stPrevolume&> preVolumeMap;
	st_Check.m_preQtyLoginSymbolWise.Clear();
	int m_dashboard_SplitData = 0;

	//m_ClientContext.Lookup(m_clientcontexKey, st_Check);
	while (hr = data_table.MoveNext() == S_OK)
	{
		m_login = data_table.m_login;
		m_symbol = data_table.m_symbol;
		m_volume = data_table.m_volume;

		stPrevolume m_stTempPreqty = {};
		CMTStr::Copy(m_stTempPreqty.m_login, m_login);
		CMTStr::Copy(m_stTempPreqty.m_symbol, m_symbol);
		m_stTempPreqty.m_volume = m_volume;

		CString m_login_symbolKey = L"";
		m_login_symbolKey.Format(L"%s:%s", m_login, m_symbol);
		preVolumeMap.SetAt(m_login_symbolKey, m_stTempPreqty);

		CStaticClass::stPrevolumeClientWise m_prestCS = {};
		CMTStr::Copy(m_prestCS.m_login, m_login);
		CMTStr::Copy(m_prestCS.m_symbol, m_symbol);
		m_prestCS.m_volume = m_volume;
		st_Check.m_preQtyLoginSymbolWise.Add(&m_prestCS);
	}
	//For Update Pre Qty in m_ClientContext

	m_mutex_ClientList.Lock();
	m_ClientContext.SetAt(m_clientcontexKey, st_Check);
	m_mutex_ClientList.Unlock();

	data_table.Close();
	m_tmpSession.Close();
	//CStaticClass::m_mutex_order.Unlock();
	////(L"UL13");
	//End of Getting PreNetqty

	int jsonType = 0;

	POSITION pos = mapNetPositionClientWise_ThreadWise->GetStartPosition();
	int total_count = mapNetPositionClientWise_ThreadWise->GetSize();
	CString strUpdateData = L"";
	int firstCheck = 0;
	CString str_columnJson = L"";
	CString str_FinalJsonUpdate = L"";

	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("type");


	string ssmessageType = string(CT2CA(m_messageType));
	const char* stmessageType = ssmessageType.c_str();


	writer.String(stmessageType);	
	writer.Key("updatekey");
	writer.StartArray();
	writer.String("login");
	writer.String("symbol");
	writer.EndArray();
	writer.Key("update");


	////(L"U1");

	writer.StartArray();
	int dataSendingFlag = 0;

	while (pos != NULL)
	{
		//////(L"Data 5");
		dataSendingFlag = 1;

		CString strKey = L"";
		CStaticClass::st_netpositionClientWise st_tmpData = {};

		CStaticClass::m_mutex_Tick.Lock();
		mapNetPositionClientWise_ThreadWise->GetNextAssoc(pos, strKey, st_tmpData);
		CString    m_login = st_tmpData.m_login;
		CString    m_name = st_tmpData.m_name;
		CString    m_symbol = st_tmpData.m_symbol;
		double     m_volume = *st_tmpData.m_volume;

		stPrevolume m_stTempPreqty = {};
		CString m_login_symbolKey = L"";
		m_login_symbolKey.Format(L"%s:%s", m_login, m_symbol);
		preVolumeMap.Lookup(m_login_symbolKey, m_stTempPreqty);


		double     m_previousvolume = m_stTempPreqty.m_volume;
		double     m_difference = m_volume - m_previousvolume;

		st_tmpData.m_previousvolume = m_previousvolume;
		st_tmpData.m_difference = m_difference;

		mapNetPositionClientWise_ThreadWise->SetAt(strKey, st_tmpData);

		double     m_average = *st_tmpData.m_average;
		st_TickBidAskLast     m_lastrate = *st_tmpData.m_lastrate;

		double     m_clientbalance = *st_tmpData.m_clientbalance;
		double     m_clientnetamount = *st_tmpData.m_clientnetamount;

		double     m_clientgrosstotal = *st_tmpData.m_clientgrosstotal;
		double     m_clientbroktotal = *st_tmpData.m_clientbroktotal;
		double     m_clientnettotal = *st_tmpData.m_clientnettotal;
		double     m_maxallotedqty = *st_tmpData.m_maxallotedqty;
		CString    m_subbroker = st_tmpData.m_subbroker;
		CString    m_broker = st_tmpData.m_broker;

		double     m_extravolume = *st_tmpData.m_extravolume;
		double     m_freemargin = *st_tmpData.m_freemargin;
		double     m_multi = *st_tmpData.m_multi;

		CString     m_Company = st_tmpData.m_company;;

		double     m_companyvolume = *st_tmpData.m_companyvolume;
		double     m_brokervolume = *st_tmpData.m_brokervolume;
		double     m_subbrokervolume = *st_tmpData.m_subbrokervolume;
		double     m_clientbrokerage = *st_tmpData.m_clientBrokarage;
		double     m_brokerbrokerage = *st_tmpData.m_brokerBrokarage;

		double     m_subbrokerbrokerage = *st_tmpData.m_subbrokerBrokarage;
		double     m_companybrokerage = *st_tmpData.m_companyBrokarage;


		double     m_clientexposure = (*st_tmpData.m_clientexposure) / 10000000;
		double     m_Companyexposure = (*st_tmpData.m_companyexposure) / 10000000;
		double     m_brokerexposure = (*st_tmpData.m_brokerexposure) / 10000000;
		double     m_subbrokerexposure = (*st_tmpData.m_subbrokerexposure) / 10000000;
		m_clientexposure = abs(m_clientexposure);
		m_Companyexposure = abs(m_Companyexposure);
		m_brokerexposure = abs(m_brokerexposure);
		m_subbrokerexposure = abs(m_subbrokerexposure);

		double     m_clientfloatingpl = *st_tmpData.m_clientfloatingpl;
		double     m_companyfloatingpl = *st_tmpData.m_companyfloatingpl;
		double     m_brokerfloatingpl = *st_tmpData.m_brokerfloatingpl;
		double     m_subbrokerfloatingpl = *st_tmpData.m_subbrokerfloatingpl;
		double     m_clientbalancepl = *st_tmpData.m_clientbalance;
		double     m_companybalancepl = *st_tmpData.m_companybalance;
		double     m_brokerbalancepl = *st_tmpData.m_brokerbalance;
		double     m_subbrokerbalancepl = *st_tmpData.m_subbrokerbalance;
		double     m_clientplnet = *st_tmpData.m_clientnetamount;
		double     m_companyplnet = *st_tmpData.m_companyNetAmount;
		double     m_brokerplnet = *st_tmpData.m_brokerNetAmount;
		double     m_subbrokerplnet = *st_tmpData.m_subbrokerNetAmount;
		double     m_clientpatti = 100;
		double     m_companypatti = *st_tmpData.m_companyRatio;
		double     m_brokerpatti = *st_tmpData.m_brokerRatio;
		double     m_subbrokerpatti = *st_tmpData.m_subbrokerRatio;
		double     m_companybrokeragerate = *st_tmpData.m_companyBrokRate;
		double     m_brokerbrokeragerate = *st_tmpData.m_brokerBrokRate;
		double     m_subbrokerbrokeragerate = *st_tmpData.m_subBrokerBrokRate;
		double     m_clientbrokeragerate = *st_tmpData.m_clientBrokRate;

		double     m_clientgrossamount = *st_tmpData.m_ClientGrossAmount;

		double     m_brokergrossamount = ((*st_tmpData.m_ClientGrossAmount) * m_brokerpatti) / 100;
		double     m_subbrokergrossamount = ((*st_tmpData.m_ClientGrossAmount) * m_subbrokerpatti) / 100;
		double     m_companygrossamount = ((*st_tmpData.m_ClientGrossAmount) * m_companypatti) / 100;

		double     m_CreditLimit = *st_tmpData.m_creditLimit;

		/*double     m_brokergrossamount = *st_tmpData.m_BrokerGrossAmount;
		double     m_subbrokergrossamount = *st_tmpData.m_SubBrokerGrossAmount;
		double     m_companygrossamount = *st_tmpData.m_CompanyGrossAmount;*/

		CString  m_Exchange = st_tmpData.m_exchange;
		CString  m_international = st_tmpData.m_international;
		CString  m_sector = st_tmpData.m_sector;
		CString  m_industry = st_tmpData.m_industry;
		double   m_rmp = st_tmpData.m_rmp;
		double   m_QtyMulti = st_tmpData.m_QtyMulti;
		CString  m_page = st_tmpData.m_page;
		CString  m_categary = st_tmpData.m_categary;
		double   m_rm = st_tmpData.m_rm;
		double   m_qtyAfterMulti = st_tmpData.m_qtyAfterMulti;
		CString  m_buySell = st_tmpData.m_buySell;
		CString  m_debitCredit = st_tmpData.m_debitCredit;
		CString  m_currencybase = st_tmpData.m_currencybase;

		double m_lpratio = st_tmpData.m_LpRatio;
		double m_lpvolume = st_tmpData.m_LpVolume;

		CString m_commoditygroup = st_tmpData.m_commoditygroup;



		//m_login_symbolKey
		CStaticClass::st_TotalTradedLotAndTOT s_TLT = {};
		CStaticClass::m_TotalLotAndTOT.Lookup(m_login_symbolKey, s_TLT);
		double m_TotalTradedLot = s_TLT.m_TotalTradedLot;
		double m_TotalTradedTO = s_TLT.m_TotalTradedTO;
		m_TotalTradedTO = m_TotalTradedTO / 10000000;


		double m_TotalTradedLot_Buy = s_TLT.m_TotalBuyLot;
		double m_TotalTradedLot_Sell = s_TLT.m_TotalSellLot;


		CStaticClass::m_mutex_Tick.Unlock();
		writer.StartObject();

		if (jsonType == 0)
		{
			CString m_loginColumnKey = L"";
			int m_columnSubs = 0;
			CStaticClass::m_mutexcolumnSubs.Lock();

			writer.Key("login");
			string sslogin = string(CT2CA(m_login));
			const char* stlogin = sslogin.c_str();
			writer.String(stlogin);

			m_columnSubs = 0;
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"name", m_Requestmessage);
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("name");
				string ssname = string(CT2CA(m_name));
				const char* stName = ssname.c_str();
				writer.String(stName);
			}
			writer.Key("symbol");
			string sssymbol = string(CT2CA(m_symbol));
			const char* stsymbol = sssymbol.c_str();
			writer.String(stsymbol);
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"previousvolume", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("previousvolume");
				writer.Double(m_previousvolume);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"difference", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("difference");
				writer.Double(m_difference);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"volume", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("volume");
				writer.Double(m_volume);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"average", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("average");
				writer.Double(m_average);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"lastrate", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("lastrate");
				writer.Double(m_volume < 0 ? m_lastrate.m_ask : m_lastrate.m_bid);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientbalance", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientbalance");
				writer.Double(m_clientbalance);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientnetamount", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientnetamount");
				writer.Double(m_clientnetamount);
			}
			CString strLogString = L"";
			//strLogString.Format(L"%s,%s,%.2lf,%.2lf,%.2lf", m_login, m_symbol, *st_tmpData.m_ClientGrossAmount, *st_tmpData.m_clientBrokarage, *st_tmpData.m_clientnetamount);
			////(strLogString);
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientplnet", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientplnet");
				writer.Double(m_clientplnet);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companyplnet", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("companyplnet");
				writer.Double(m_companyplnet);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerplnet", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokerplnet");
				writer.Double(m_brokerplnet);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerplnet", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokerplnet");
				writer.Double(m_subbrokerplnet);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"maxallotedqty", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("maxallotedqty");
				writer.Double(m_maxallotedqty);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"totaltradedlot", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("totaltradedlot");
				writer.Double(m_TotalTradedLot);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"totaltradedto", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("totaltradedto");
				writer.Double(m_TotalTradedTO);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"company", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("company");
				string sscompany = string(CT2CA(m_Company));
				const char* stcompany = sscompany.c_str();
				writer.String(stcompany);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbroker", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbroker");
				string sssubbroker = string(CT2CA(m_subbroker));
				const char* stsubbroker = sssubbroker.c_str();
				writer.String(stsubbroker);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"broker", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("broker");
				string ssbroker = string(CT2CA(m_broker));
				const char* stbroker = ssbroker.c_str();
				writer.String(stbroker);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"extravolume", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("extravolume");
				writer.Double(m_extravolume);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"freemargin", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("freemargin");
				writer.Double(m_freemargin);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companyvolume", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("companyvolume");
				writer.Double(m_companyvolume);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokervolume", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokervolume");
				writer.Double(m_brokervolume);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokervolume", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokervolume");
				writer.Double(m_subbrokervolume);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientbrokerage", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientbrokerage");
				writer.Double(m_clientbrokerage);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerbrokerage", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokerbrokerage");
				writer.Double(m_brokerbrokerage);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerbrokerage", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokerbrokerage");
				writer.Double(m_subbrokerbrokerage);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companybrokerage", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("companybrokerage");
				writer.Double(m_companybrokerage);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientexposure", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientexposure");
				writer.Double(m_clientexposure);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"Companyexposure", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("Companyexposure");
				writer.Double(m_Companyexposure);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerexposure", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokerexposure");
				writer.Double(m_brokerexposure);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerexposure", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokerexposure");
				writer.Double(m_subbrokerexposure);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientfloatingpl", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientfloatingpl");
				writer.Double(m_clientfloatingpl);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companyfloatingpl", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("companyfloatingpl");
				writer.Double(m_companyfloatingpl);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerfloatingpl", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokerfloatingpl");
				writer.Double(m_brokerfloatingpl);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerfloatingpl", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokerfloatingpl");
				writer.Double(m_subbrokerfloatingpl);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientbalancepl", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientbalancepl");
				writer.Double(m_clientbalancepl);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companybalancepl", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("companybalancepl");
				writer.Double(m_companybalancepl);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerbalancepl", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokerbalancepl");
				writer.Double(m_brokerbalancepl);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerbalancepl", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokerbalancepl");
				writer.Double(m_subbrokerbalancepl);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientpatti", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientpatti");
				writer.Double(m_clientpatti);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companypatti", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("companypatti");
				writer.Double(m_companypatti);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerpatti", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokerpatti");
				writer.Double(m_brokerpatti);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerpatti", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokerpatti");
				writer.Double(m_subbrokerpatti);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companybrokeragerate", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("companybrokeragerate");
				writer.Double(m_companybrokeragerate);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerbrokeragerate", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokerbrokeragerate");
				writer.Double(m_brokerbrokeragerate);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerbrokeragerate", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokerbrokeragerate");
				writer.Double(m_subbrokerbrokeragerate);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientbrokeragerate", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientbrokeragerate");
				writer.Double(m_clientbrokeragerate);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientgrossamount", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientgrossamount");
				writer.Double(m_clientgrossamount);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokergrossamount", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokergrossamount");
				writer.Double(m_brokergrossamount);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokergrossamount", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokergrossamount");
				writer.Double(m_subbrokergrossamount);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companygrossamount", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("companygrossamount");
				writer.Double(m_companygrossamount);
			}

			int m_TotalNo_Of_StopBuy = 0;
			int m_TotalNo_Of_Cancelled_StopBuy = 0;
			int m_TotalNo_Of_Passed_StopBuy = 0;
			int m_TotalNo_Of_Pending_StopBuy = 0;

			int m_TotalNo_Of_StopSell = 0;
			int m_TotalNo_Of_Cancelled_StopSell = 0;
			int m_TotalNo_Of_Passed_StopSell = 0;
			int m_TotalNo_Of_Pending_StopSell = 0;

			int m_TotalNo_Of_BuyLimit = 0;
			int m_TotalNo_Of_Cancelled_BuyLimit = 0;
			int m_TotalNo_Of_Passed_BuyLimit = 0;
			int m_TotalNo_Of_Pending_BuyLimit = 0;

			int m_TotalNo_Of_SellLimit = 0;
			int m_TotalNo_Of_Cancelled_SellLimit = 0;
			int m_TotalNo_Of_Passed_SellLimit = 0;
			int m_TotalNo_Of_Pending_SellLimit = 0;



			int m_TotalNo_Of_MarketDeal = 0;
			int m_TotalNo_Of_LimitDeal = 0;
			int m_TotalNo_Of_StopLimitDeal = 0;




			CString strloginSymbolKey = L"";
			//strloginSymbolKey.Format(L"%I64u:%s:%d", m_login, m_symbol, m_stOrder_Check.m_type);
			strloginSymbolKey.Format(L"%s:%s:4", m_login, m_symbol);
			st_OrderCount m_stOrderCount = {};
			CStaticClass::m_OrderCountMap.Lookup(strloginSymbolKey, m_stOrderCount);
			m_TotalNo_Of_StopBuy = m_stOrderCount.m_TotalOrder;
			m_TotalNo_Of_Cancelled_StopBuy = m_stOrderCount.m_TotalCancelledOrder;
			m_TotalNo_Of_Passed_StopBuy = m_stOrderCount.m_TotalExecutedOrder;
			m_TotalNo_Of_Pending_StopBuy = m_TotalNo_Of_StopBuy - m_TotalNo_Of_Cancelled_StopBuy - m_TotalNo_Of_Passed_StopBuy;

			m_stOrderCount = {};
			strloginSymbolKey.Format(L"%s:%s:5", m_login, m_symbol);
			CStaticClass::m_OrderCountMap.Lookup(strloginSymbolKey, m_stOrderCount);
			m_TotalNo_Of_StopSell = m_stOrderCount.m_TotalOrder;
			m_TotalNo_Of_Cancelled_StopSell = m_stOrderCount.m_TotalCancelledOrder;
			m_TotalNo_Of_Passed_StopSell = m_stOrderCount.m_TotalExecutedOrder;
			m_TotalNo_Of_Pending_StopSell = m_TotalNo_Of_StopSell - m_TotalNo_Of_Cancelled_StopSell - m_TotalNo_Of_Passed_StopSell;

			m_stOrderCount = {};
			strloginSymbolKey.Format(L"%s:%s:2", m_login, m_symbol);
			CStaticClass::m_OrderCountMap.Lookup(strloginSymbolKey, m_stOrderCount);
			m_TotalNo_Of_BuyLimit = m_stOrderCount.m_TotalOrder;
			m_TotalNo_Of_Cancelled_BuyLimit = m_stOrderCount.m_TotalCancelledOrder;
			m_TotalNo_Of_Passed_BuyLimit = m_stOrderCount.m_TotalExecutedOrder;
			m_TotalNo_Of_Pending_BuyLimit = m_TotalNo_Of_BuyLimit - m_TotalNo_Of_Cancelled_BuyLimit - m_TotalNo_Of_Passed_BuyLimit;

			m_stOrderCount = {};
			strloginSymbolKey.Format(L"%s:%s:3", m_login, m_symbol);
			CStaticClass::m_OrderCountMap.Lookup(strloginSymbolKey, m_stOrderCount);
			m_TotalNo_Of_SellLimit = m_stOrderCount.m_TotalOrder;
			m_TotalNo_Of_Cancelled_SellLimit = m_stOrderCount.m_TotalCancelledOrder;
			m_TotalNo_Of_Passed_SellLimit = m_stOrderCount.m_TotalExecutedOrder;
			m_TotalNo_Of_Pending_SellLimit = m_TotalNo_Of_SellLimit - m_TotalNo_Of_Cancelled_SellLimit - m_TotalNo_Of_Passed_SellLimit;


			int totalMarketBuyDeal = 0;
			int totalMarketSellDeal = 0;
			m_stOrderCount = {};
			strloginSymbolKey.Format(L"%s:%s:0", m_login, m_symbol);
			CStaticClass::m_OrderCountMap.Lookup(strloginSymbolKey, m_stOrderCount);
			totalMarketBuyDeal = m_stOrderCount.m_TotalOrder;


			m_stOrderCount = {};
			strloginSymbolKey.Format(L"%s:%s:1", m_login, m_symbol);
			CStaticClass::m_OrderCountMap.Lookup(strloginSymbolKey, m_stOrderCount);
			totalMarketSellDeal = m_stOrderCount.m_TotalOrder;


			m_TotalNo_Of_MarketDeal = totalMarketBuyDeal + totalMarketSellDeal;
			m_TotalNo_Of_LimitDeal = m_TotalNo_Of_SellLimit + m_TotalNo_Of_BuyLimit;
			m_TotalNo_Of_StopLimitDeal = m_TotalNo_Of_StopBuy + m_TotalNo_Of_StopSell;

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_StopBuy", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_StopBuy");
				writer.Int(m_TotalNo_Of_StopBuy);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Cancelled_StopBuy", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Cancelled_StopBuy");
				writer.Int(m_TotalNo_Of_Cancelled_StopBuy);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Passed_StopBuy", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Passed_StopBuy");
				writer.Int(m_TotalNo_Of_Passed_StopBuy);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Pending_StopBuy", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Pending_StopBuy");
				writer.Int(m_TotalNo_Of_Pending_StopBuy);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_StopSell", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_StopSell");
				writer.Int(m_TotalNo_Of_StopSell);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Cancelled_StopSell", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Cancelled_StopSell");
				writer.Int(m_TotalNo_Of_Cancelled_StopSell);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Passed_StopSell", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Passed_StopSell");
				writer.Int(m_TotalNo_Of_Passed_StopSell);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Pending_StopSell", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Pending_StopSell");
				writer.Int(m_TotalNo_Of_Pending_StopSell);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_BuyLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_BuyLimit");
				writer.Int(m_TotalNo_Of_BuyLimit);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Cancelled_BuyLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Cancelled_BuyLimit");
				writer.Int(m_TotalNo_Of_Cancelled_BuyLimit);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Passed_BuyLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Passed_BuyLimit");
				writer.Int(m_TotalNo_Of_Passed_BuyLimit);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Pending_BuyLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Pending_BuyLimit");
				writer.Int(m_TotalNo_Of_Pending_BuyLimit);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_SellLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_SellLimit");
				writer.Int(m_TotalNo_Of_SellLimit);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Cancelled_SellLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Cancelled_SellLimit");
				writer.Int(m_TotalNo_Of_Cancelled_SellLimit);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Passed_SellLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Passed_SellLimit");
				writer.Int(m_TotalNo_Of_Passed_SellLimit);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Pending_SellLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Pending_SellLimit");
				writer.Int(m_TotalNo_Of_Pending_SellLimit);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_MarketDeal", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_MarketDeal");
				writer.Int(m_TotalNo_Of_MarketDeal);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_LimitDeal", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_LimitDeal");
				writer.Int(m_TotalNo_Of_LimitDeal);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_StopLimitDeal", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_StopLimitDeal");
				writer.Int(m_TotalNo_Of_StopLimitDeal);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalTradedLot_Buy", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalTradedLot_Buy");
				writer.Double(m_TotalTradedLot_Buy);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalTradedLot_Sell", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalTradedLot_Sell");
				writer.Double(m_TotalTradedLot_Sell);
			}

			CString  strLoginSymbolGroup = L"";
			strLoginSymbolGroup.Format(L"%s:%s", m_login, m_symbol);
			CStaticClass::st_BadTradeGoodTrade m_st_BadTradeGoodTrade = {};
			CStaticClass::m_BadTradeGoodTrade.Lookup(strLoginSymbolGroup, m_st_BadTradeGoodTrade);
			double m_badtrade = m_st_BadTradeGoodTrade.m_BadTrade;
			double m_TotalTrade = m_st_BadTradeGoodTrade.m_TotalTrade;
			double m_badtradePer = 0;
			if (m_badtrade != 0)
			{
				m_badtradePer = (m_badtrade / m_TotalTrade) * 100;
			}
			double m_badtradeIgnorePosition = m_st_BadTradeGoodTrade.m_badTradeIgnoringPosition;
			double m_badTradeIPCper = 0;
			if (m_badtradeIgnorePosition != 0)
			{
				m_badTradeIPCper = (m_badtradeIgnorePosition / m_TotalTrade) * 100;
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"badtradeper", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("badtradeper");
				writer.Double(m_badtradePer);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"badtradeipcper", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("badtradeipcper");
				writer.Double(m_badTradeIPCper);
			}

			int m_totaltradeipc = m_st_BadTradeGoodTrade.m_TradeIgnoringPosition;
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"totaltradeipc", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("totaltradeipc");
				writer.Double(m_totaltradeipc);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"totaltrade", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("totaltrade");
				writer.Double(m_TotalTrade);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"badtradeipc", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("badtradeipc");
				writer.Double(m_badtradeIgnorePosition);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"badtrade", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("badtrade");
				writer.Double(m_badtrade);
			}


			/*CString strLogVal = L"";
			strLogVal.Format(L"Login:%s Amount:%.2lf Debit/Credit:%s Volume:%.2lf GE:%.4lf", m_login, m_clientplnet, m_debitCredit, m_volume, m_clientexposure);
			//(strLogVal);*/
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"buysell", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("buysell");
				/*string ssbuySell = string(CT2CA(m_buySell));
				const char* strbuySell = ssbuySell.c_str();*/
				if (m_volume < 0)
				{
					writer.String("Sell");
				}
				else
				{
					writer.String("Buy");
				}
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"debitcredit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("debitcredit");
				/*string ssdebitCredit = string(CT2CA(m_debitCredit));
				const char* strdebitCredit = ssdebitCredit.c_str();*/
				if (m_clientplnet < 0)
				{
					writer.String("Debit");
				}
				else
				{
					writer.String("Credit");
				}

			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"exchange", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("exchange");
				string ssexchange = string(CT2CA(m_Exchange));
				const char* strexchange = ssexchange.c_str();
				writer.String(strexchange);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"international", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("international");
				string ssinternational = string(CT2CA(m_international));
				const char* strinternational = ssinternational.c_str();
				writer.String(strinternational);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"sector", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("sector");
				string sssector = string(CT2CA(m_sector));
				const char* strsector = sssector.c_str();
				writer.String(strsector);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"industry", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("industry");
				string ssindustry = string(CT2CA(m_industry));
				const char* strindustry = ssindustry.c_str();
				writer.String(strindustry);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"page", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("page");
				string sspage = string(CT2CA(m_page));
				const char* strpage = sspage.c_str();
				writer.String(strpage);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"categary", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("categary");
				string sscategary = string(CT2CA(m_categary));
				const char* strcategary = sscategary.c_str();
				writer.String(strcategary);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"rm", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("rm");
				writer.Double(m_rm);
			}


			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"lpratio", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("lpratio");
				writer.Double(m_lpratio);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"commoditygroup", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("commoditygroup");
				string sscommoditygroup = string(CT2CA(m_commoditygroup));
				const char* strcommoditygroup = sscommoditygroup.c_str();
				writer.String(strcommoditygroup);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"lpvolume", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("lpvolume");
				writer.Double(m_lpvolume);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"qtyaftermulti", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("qtyaftermulti");
				writer.Double(m_qtyAfterMulti);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"rmp", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("rmp");
				writer.Double(m_rmp);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"qtymulti", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("qtymulti");
				writer.Double(m_QtyMulti);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"currencybase", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("currencybase");
				string sscurrencybase = string(CT2CA(m_currencybase));
				const char* strcurrencybase = sscurrencybase.c_str();
				writer.String(strcurrencybase);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"creditLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("creditLimit");
				writer.Double(m_CreditLimit);
			}

			CStaticClass::stConfig m_stConfig = {};
			CString loginSymbolKey = L"";
			CStaticClass::m_mutex_LimitConfig.Lock();
			CString SymbolGroup = L"";
			if (m_symbol.GetLength() >= 5)
			{
				SymbolGroup = m_symbol.Mid(0, m_symbol.GetLength() - 3);
			}
			else
			{
				SymbolGroup = m_symbol;
			}
			int limitValue = 0;
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"BuyPositionLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				loginSymbolKey.Format(L"%s:%s:1", m_login, SymbolGroup);
				m_stConfig = {};
				CStaticClass::LimitConfig.Lookup(loginSymbolKey, m_stConfig);
				limitValue = m_stConfig.Limit;
				writer.Key("BuyPositionLimit");
				writer.Int(limitValue);
			}


			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"SellPositionLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				loginSymbolKey.Format(L"%s:%s:2", m_login, SymbolGroup);
				m_stConfig = {};
				CStaticClass::LimitConfig.Lookup(loginSymbolKey, m_stConfig);
				limitValue = m_stConfig.Limit;
				writer.Key("SellPositionLimit");
				writer.Int(limitValue);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"AllSymbolBuyPositionLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				loginSymbolKey.Format(L"%s:%s:3", m_login, L"SHARE");
				m_stConfig = {};
				CStaticClass::LimitConfig.Lookup(loginSymbolKey, m_stConfig);
				limitValue = m_stConfig.Limit;
				writer.Key("AllSymbolBuyPositionLimit");
				writer.Int(limitValue);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"AllSymbolSellPositionLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				loginSymbolKey.Format(L"%s:%s:4", m_login, L"SHARE");
				m_stConfig = {};
				CStaticClass::LimitConfig.Lookup(loginSymbolKey, m_stConfig);
				limitValue = m_stConfig.Limit;
				writer.Key("AllSymbolSellPositionLimit");
				writer.Int(limitValue);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"AllPositionLimitLoginwise", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				loginSymbolKey.Format(L"%s:%s:5", m_login, L"SHARE");
				m_stConfig = {};
				CStaticClass::LimitConfig.Lookup(loginSymbolKey, m_stConfig);
				limitValue = m_stConfig.Limit;
				writer.Key("AllPositionLimitLoginwise");
				writer.Int(limitValue);
			}
			CStaticClass::m_mutex_LimitConfig.Unlock();





			CStaticClass::m_mutexcolumnSubs.Unlock();


		}
		
		writer.EndObject();


		if (dataSendingFlag == 1)
		{
			string strforsend = "";
			str_FinalJsonUpdate = s.GetString();

			

			int dataSize = str_FinalJsonUpdate.GetLength();

			

			//if (dataSize >= 1500)
			if (dataSize >= 1500000)
			{
				writer.EndArray();
				writer.EndObject();

				str_FinalJsonUpdate = s.GetString();
				////(L"Test");
				//(str_FinalJsonUpdate);
				strforsend = CT2A(str_FinalJsonUpdate.GetString());
				SendDataToClient(client, strforsend, m_clientcontexKey, m_activeClient);

				s.Clear();
				writer.Flush();

				writer.Reset(s);

				writer.StartObject();
				writer.Key("type");
				writer.String(stmessageType);
				writer.Key("updatekey");
				writer.StartArray();
				writer.String("login");
				writer.String("symbol");
				writer.EndArray();
				writer.Key("update");
				writer.StartArray();



				dataSendingFlag = 0;
			}
		}

	}

	if (dataSendingFlag == 1)
	{
		writer.EndArray();
		writer.EndObject();
		string strforsend = "";
		str_FinalJsonUpdate = s.GetString();
		
		strforsend = CT2A(str_FinalJsonUpdate.GetString());
		SendDataToClient(client, strforsend, m_clientcontexKey, m_activeClient);
		dataSendingFlag = 0;
	}
}

void CStaticClass::sendClientPosition_Insert(SSL_session* client, int m_activeClient,CString strUserID,CString m_clientcontexKey, CString m_Requestmessage,CString m_messageType,  NetPositionClientWise_All*   mapNetPositionClientWise_ThreadWise)
{
	
	st_ClientContext st_Check = {};
	m_mutex_ClientList.Lock();
		m_ClientContext.Lookup(m_clientcontexKey, st_Check);	
	CMap<CString, LPCTSTR, int, int&> m_subscribedcolumn_Local;
	int  totalColumns = st_Check.m_clientrequests_List.Total();
	for (int ci = 0; ci < totalColumns; ci++)
	{
		CString  m_ColumnsData = L"";
		m_ColumnsData = st_Check.m_clientrequests_List[ci];
		CString m_messageType_Subs = L"";
		m_messageType_Subs.Format(L":%s:",m_Requestmessage);
		CString m_logStr = L"";
		
		if (m_ColumnsData.Find(m_messageType_Subs) >= 0)
		{
			int activate = 1;
			m_subscribedcolumn_Local.SetAt(m_ColumnsData, activate);
		}		
	}	
	m_mutex_ClientList.Unlock();
	
	//Getting PreNetQty				
	CString returnval = L"";
	HRESULT hr = NULL;
	CCommand<CAccessor<CNetpositionPreQtyTable>> data_table;
	if (!SUCCEEDED(hr))
	{
		return;
	}
	CString   strCommand = L"";

	UINT64 m_preVolumeTime = 0;
	if (m_Requestmessage == L"FETCH_DASHBOARD_DATA")
	{
		m_preVolumeTime = st_Check.m_DashboardTime;
	}
	else
	{
		m_preVolumeTime = st_Check.m_netPositionTime;
	}
	//if (m_preVolumeTime == 0)
	//{
		m_preVolumeTime=getPreviousDateTime_Unix();
	//}

	strCommand.Format(L"orika_GetClientPreviousNetQty '%I64u';", m_preVolumeTime);

	//CStaticClass::m_mutex_order.Lock();				
	//(L"L13");
	CSession m_tmpSession;
	m_tmpSession.Open(CStaticClass::connection);
	hr = data_table.Open(m_tmpSession, (LPCTSTR)strCommand);
	if (FAILED(hr))
	{
		m_tmpSession.Close();	
	}
	int i = 0;
	CString m_login = L"";
	CString m_symbol = L"";
	double m_volume = 0;

	CString tmpstr = L"";
	int row_count = 0;
	//hr=data_table.MoveNext();

	struct stPrevolume
	{
		wchar_t m_login[30];
		wchar_t m_symbol[32];
		double m_volume;
	};
	CMap<CString, LPCTSTR, stPrevolume, stPrevolume&> preVolumeMap;
	st_Check.m_preQtyLoginSymbolWise.Clear();
	int m_dashboard_SplitData = 0;
	
	//m_ClientContext.Lookup(m_clientcontexKey, st_Check);
	while (hr = data_table.MoveNext() == S_OK)
	{
		m_login = data_table.m_login;
		m_symbol = data_table.m_symbol;
		m_volume = data_table.m_volume;

		stPrevolume m_stTempPreqty = {};
		CMTStr::Copy(m_stTempPreqty.m_login, m_login);
		CMTStr::Copy(m_stTempPreqty.m_symbol, m_symbol);
		m_stTempPreqty.m_volume = m_volume;

		CString m_login_symbolKey = L"";
		m_login_symbolKey.Format(L"%s:%s", m_login, m_symbol);
		preVolumeMap.SetAt(m_login_symbolKey, m_stTempPreqty);

		CStaticClass::stPrevolumeClientWise m_prestCS = {};
		CMTStr::Copy(m_prestCS.m_login, m_login);
		CMTStr::Copy(m_prestCS.m_symbol, m_symbol);
		m_prestCS.m_volume = m_volume;
		st_Check.m_preQtyLoginSymbolWise.Add(&m_prestCS);
	}
	//For Update Pre Qty in m_ClientContext

	m_mutex_ClientList.Lock();	
	m_ClientContext.SetAt(m_clientcontexKey, st_Check);
	m_mutex_ClientList.Unlock();

	data_table.Close();
	m_tmpSession.Close();
	//CStaticClass::m_mutex_order.Unlock();
	////(L"UL13");
	//End of Getting PreNetqty

	int jsonType = 0;

	POSITION pos = mapNetPositionClientWise_ThreadWise->GetStartPosition();
	int total_count = mapNetPositionClientWise_ThreadWise->GetSize();
	CString strUpdateData = L"";
	int firstCheck = 0;
	CString str_columnJson = L"";
	CString str_FinalJsonUpdate = L"";

	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	

	
	string ssmessageType = string(CT2CA(m_messageType));
	const char* stmessageType = ssmessageType.c_str();

	writer.Key("type");
	writer.String(stmessageType);
	writer.Key("insert");

	////(L"U1");

	writer.StartArray();
	int dataSendingFlag = 0;

	while (pos != NULL)
	{
		//////(L"Data 5");
		dataSendingFlag = 1;

		CString strKey = L"";
		CStaticClass::st_netpositionClientWise st_tmpData = {};

		CStaticClass::m_mutex_Tick.Lock();
		mapNetPositionClientWise_ThreadWise->GetNextAssoc(pos, strKey, st_tmpData);
		CString    m_login = st_tmpData.m_login;
		CString    m_name = st_tmpData.m_name;
		CString    m_symbol = st_tmpData.m_symbol;
		double     m_volume = *st_tmpData.m_volume;

		stPrevolume m_stTempPreqty = {};
		CString m_login_symbolKey = L"";
		m_login_symbolKey.Format(L"%s:%s", m_login, m_symbol);
		preVolumeMap.Lookup(m_login_symbolKey, m_stTempPreqty);


		double     m_previousvolume = m_stTempPreqty.m_volume;
		double     m_difference = m_volume - m_previousvolume;
		double     m_average = *st_tmpData.m_average;
		st_tmpData.m_previousvolume = m_previousvolume;
		st_tmpData.m_difference = m_difference;

		mapNetPositionClientWise_ThreadWise->SetAt(strKey, st_tmpData);



		st_TickBidAskLast     m_lastrate = *st_tmpData.m_lastrate;

		double     m_clientbalance = *st_tmpData.m_clientbalance;
		double     m_clientnetamount = *st_tmpData.m_clientnetamount;

		double     m_clientgrosstotal = *st_tmpData.m_clientgrosstotal;
		double     m_clientbroktotal = *st_tmpData.m_clientbroktotal;
		double     m_clientnettotal = *st_tmpData.m_clientnettotal;
		double     m_maxallotedqty = *st_tmpData.m_maxallotedqty;
		CString    m_subbroker = st_tmpData.m_subbroker;
		CString    m_broker = st_tmpData.m_broker;

		double     m_extravolume = *st_tmpData.m_extravolume;
		double     m_freemargin = *st_tmpData.m_freemargin;
		double     m_multi = *st_tmpData.m_multi;

		CString     m_Company = st_tmpData.m_company;;

		double     m_companyvolume = *st_tmpData.m_companyvolume;
		double     m_brokervolume = *st_tmpData.m_brokervolume;
		double     m_subbrokervolume = *st_tmpData.m_subbrokervolume;
		double     m_clientbrokerage = *st_tmpData.m_clientBrokarage;
		double     m_brokerbrokerage = *st_tmpData.m_brokerBrokarage;

		double     m_subbrokerbrokerage = *st_tmpData.m_subbrokerBrokarage;
		double     m_companybrokerage = *st_tmpData.m_companyBrokarage;


		double     m_clientexposure = (*st_tmpData.m_clientexposure) / 10000000;
		double     m_Companyexposure = (*st_tmpData.m_companyexposure) / 10000000;
		double     m_brokerexposure = (*st_tmpData.m_brokerexposure) / 10000000;
		double     m_subbrokerexposure = (*st_tmpData.m_subbrokerexposure) / 10000000;
		m_clientexposure = abs(m_clientexposure);
		m_Companyexposure = abs(m_Companyexposure);
		m_brokerexposure = abs(m_brokerexposure);
		m_subbrokerexposure = abs(m_subbrokerexposure);

		double     m_clientfloatingpl = *st_tmpData.m_clientfloatingpl;
		double     m_companyfloatingpl = *st_tmpData.m_companyfloatingpl;
		double     m_brokerfloatingpl = *st_tmpData.m_brokerfloatingpl;
		double     m_subbrokerfloatingpl = *st_tmpData.m_subbrokerfloatingpl;
		double     m_clientbalancepl = *st_tmpData.m_clientbalance;
		double     m_companybalancepl = *st_tmpData.m_companybalance;
		double     m_brokerbalancepl = *st_tmpData.m_brokerbalance;
		double     m_subbrokerbalancepl = *st_tmpData.m_subbrokerbalance;
		double     m_clientplnet = *st_tmpData.m_clientnetamount;
		double     m_companyplnet = *st_tmpData.m_companyNetAmount;
		double     m_brokerplnet = *st_tmpData.m_brokerNetAmount;
		double     m_subbrokerplnet = *st_tmpData.m_subbrokerNetAmount;
		double     m_clientpatti = 100;
		double     m_companypatti = *st_tmpData.m_companyRatio;
		double     m_brokerpatti = *st_tmpData.m_brokerRatio;
		double     m_subbrokerpatti = *st_tmpData.m_subbrokerRatio;
		double     m_companybrokeragerate = *st_tmpData.m_companyBrokRate;
		double     m_brokerbrokeragerate = *st_tmpData.m_brokerBrokRate;
		double     m_subbrokerbrokeragerate = *st_tmpData.m_subBrokerBrokRate;
		double     m_clientbrokeragerate = *st_tmpData.m_clientBrokRate;

		double     m_clientgrossamount = *st_tmpData.m_ClientGrossAmount;

		double     m_brokergrossamount = ((*st_tmpData.m_ClientGrossAmount) * m_brokerpatti) / 100;
		double     m_subbrokergrossamount = ((*st_tmpData.m_ClientGrossAmount) * m_subbrokerpatti) / 100;
		double     m_companygrossamount = ((*st_tmpData.m_ClientGrossAmount) * m_companypatti) / 100;

		double     m_CreditLimit = *st_tmpData.m_creditLimit;

		/*double     m_brokergrossamount = *st_tmpData.m_BrokerGrossAmount;
		double     m_subbrokergrossamount = *st_tmpData.m_SubBrokerGrossAmount;
		double     m_companygrossamount = *st_tmpData.m_CompanyGrossAmount;*/

		CString  m_Exchange = st_tmpData.m_exchange;
		CString  m_international = st_tmpData.m_international;
		CString  m_sector = st_tmpData.m_sector;
		CString  m_industry = st_tmpData.m_industry;
		double   m_rmp = st_tmpData.m_rmp;
		double   m_QtyMulti = st_tmpData.m_QtyMulti;
		CString  m_page = st_tmpData.m_page;
		CString  m_categary = st_tmpData.m_categary;
		double   m_rm = st_tmpData.m_rm;
		double   m_qtyAfterMulti = st_tmpData.m_qtyAfterMulti;
		CString  m_buySell = st_tmpData.m_buySell;
		CString  m_debitCredit = st_tmpData.m_debitCredit;
		CString  m_currencybase = st_tmpData.m_currencybase;

		double m_lpratio = st_tmpData.m_LpRatio;
		double m_lpvolume = st_tmpData.m_LpVolume;

		CString m_commoditygroup = st_tmpData.m_commoditygroup;


		//m_login_symbolKey
		CStaticClass::st_TotalTradedLotAndTOT s_TLT = {};
		CStaticClass::m_TotalLotAndTOT.Lookup(m_login_symbolKey, s_TLT);
		double m_TotalTradedLot = s_TLT.m_TotalTradedLot;
		double m_TotalTradedTO = s_TLT.m_TotalTradedTO;
		m_TotalTradedTO = m_TotalTradedTO / 10000000;


		double m_TotalTradedLot_Buy = s_TLT.m_TotalBuyLot;
		double m_TotalTradedLot_Sell = s_TLT.m_TotalSellLot;


		CStaticClass::m_mutex_Tick.Unlock();
		writer.StartObject();

		if (jsonType == 0)
		{
			CString m_loginColumnKey = L"";
			int m_columnSubs = 0;
			CStaticClass::m_mutexcolumnSubs.Lock();

			writer.Key("login");
			string sslogin = string(CT2CA(m_login));
			const char* stlogin = sslogin.c_str();
			writer.String(stlogin);

			m_columnSubs = 0;
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"name", m_Requestmessage);
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("name");
				string ssname = string(CT2CA(m_name));
				const char* stName = ssname.c_str();
				writer.String(stName);
			}
			writer.Key("symbol");
			string sssymbol = string(CT2CA(m_symbol));
			const char* stsymbol = sssymbol.c_str();
			writer.String(stsymbol);
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"previousvolume", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("previousvolume");
				writer.Double(m_previousvolume);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"difference", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("difference");
				writer.Double(m_difference);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"volume", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("volume");
				writer.Double(m_volume);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"average", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("average");
				writer.Double(m_average);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"lastrate", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("lastrate");
				writer.Double(m_volume < 0 ? m_lastrate.m_ask : m_lastrate.m_bid);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientbalance", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientbalance");
				writer.Double(m_clientbalance);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientnetamount", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientnetamount");
				writer.Double(m_clientnetamount);
			}
			CString strLogString = L"";
			//strLogString.Format(L"%s,%s,%.2lf,%.2lf,%.2lf", m_login, m_symbol, *st_tmpData.m_ClientGrossAmount, *st_tmpData.m_clientBrokarage, *st_tmpData.m_clientnetamount);
			////(strLogString);
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientplnet", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientplnet");
				writer.Double(m_clientplnet);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companyplnet", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("companyplnet");
				writer.Double(m_companyplnet);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerplnet", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokerplnet");
				writer.Double(m_brokerplnet);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerplnet", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokerplnet");
				writer.Double(m_subbrokerplnet);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"maxallotedqty", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("maxallotedqty");
				writer.Double(m_maxallotedqty);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"totaltradedlot", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("totaltradedlot");
				writer.Double(m_TotalTradedLot);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"totaltradedto", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("totaltradedto");
				writer.Double(m_TotalTradedTO);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"company", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("company");
				string sscompany = string(CT2CA(m_Company));
				const char* stcompany = sscompany.c_str();
				writer.String(stcompany);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbroker", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbroker");
				string sssubbroker = string(CT2CA(m_subbroker));
				const char* stsubbroker = sssubbroker.c_str();
				writer.String(stsubbroker);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"broker", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("broker");
				string ssbroker = string(CT2CA(m_broker));
				const char* stbroker = ssbroker.c_str();
				writer.String(stbroker);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"extravolume", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("extravolume");
				writer.Double(m_extravolume);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"freemargin", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("freemargin");
				writer.Double(m_freemargin);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companyvolume", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("companyvolume");
				writer.Double(m_companyvolume);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokervolume", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokervolume");
				writer.Double(m_brokervolume);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokervolume", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokervolume");
				writer.Double(m_subbrokervolume);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientbrokerage", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientbrokerage");
				writer.Double(m_clientbrokerage);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerbrokerage", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokerbrokerage");
				writer.Double(m_brokerbrokerage);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerbrokerage", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokerbrokerage");
				writer.Double(m_subbrokerbrokerage);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companybrokerage", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("companybrokerage");
				writer.Double(m_companybrokerage);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientexposure", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientexposure");
				writer.Double(m_clientexposure);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"Companyexposure", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("Companyexposure");
				writer.Double(m_Companyexposure);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerexposure", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokerexposure");
				writer.Double(m_brokerexposure);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerexposure", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokerexposure");
				writer.Double(m_subbrokerexposure);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientfloatingpl", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientfloatingpl");
				writer.Double(m_clientfloatingpl);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companyfloatingpl", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("companyfloatingpl");
				writer.Double(m_companyfloatingpl);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerfloatingpl", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokerfloatingpl");
				writer.Double(m_brokerfloatingpl);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerfloatingpl", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokerfloatingpl");
				writer.Double(m_subbrokerfloatingpl);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientbalancepl", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientbalancepl");
				writer.Double(m_clientbalancepl);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companybalancepl", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("companybalancepl");
				writer.Double(m_companybalancepl);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerbalancepl", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokerbalancepl");
				writer.Double(m_brokerbalancepl);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerbalancepl", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokerbalancepl");
				writer.Double(m_subbrokerbalancepl);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientpatti", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientpatti");
				writer.Double(m_clientpatti);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companypatti", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("companypatti");
				writer.Double(m_companypatti);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerpatti", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokerpatti");
				writer.Double(m_brokerpatti);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerpatti", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokerpatti");
				writer.Double(m_subbrokerpatti);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companybrokeragerate", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("companybrokeragerate");
				writer.Double(m_companybrokeragerate);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerbrokeragerate", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokerbrokeragerate");
				writer.Double(m_brokerbrokeragerate);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerbrokeragerate", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokerbrokeragerate");
				writer.Double(m_subbrokerbrokeragerate);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientbrokeragerate", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientbrokeragerate");
				writer.Double(m_clientbrokeragerate);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientgrossamount", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("clientgrossamount");
				writer.Double(m_clientgrossamount);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokergrossamount", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("brokergrossamount");
				writer.Double(m_brokergrossamount);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokergrossamount", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("subbrokergrossamount");
				writer.Double(m_subbrokergrossamount);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companygrossamount", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("companygrossamount");
				writer.Double(m_companygrossamount);
			}

			int m_TotalNo_Of_StopBuy = 0;
			int m_TotalNo_Of_Cancelled_StopBuy = 0;
			int m_TotalNo_Of_Passed_StopBuy = 0;
			int m_TotalNo_Of_Pending_StopBuy = 0;

			int m_TotalNo_Of_StopSell = 0;
			int m_TotalNo_Of_Cancelled_StopSell = 0;
			int m_TotalNo_Of_Passed_StopSell = 0;
			int m_TotalNo_Of_Pending_StopSell = 0;

			int m_TotalNo_Of_BuyLimit = 0;
			int m_TotalNo_Of_Cancelled_BuyLimit = 0;
			int m_TotalNo_Of_Passed_BuyLimit = 0;
			int m_TotalNo_Of_Pending_BuyLimit = 0;

			int m_TotalNo_Of_SellLimit = 0;
			int m_TotalNo_Of_Cancelled_SellLimit = 0;
			int m_TotalNo_Of_Passed_SellLimit = 0;
			int m_TotalNo_Of_Pending_SellLimit = 0;



			int m_TotalNo_Of_MarketDeal = 0;
			int m_TotalNo_Of_LimitDeal = 0;
			int m_TotalNo_Of_StopLimitDeal = 0;




			CString strloginSymbolKey = L"";
			//strloginSymbolKey.Format(L"%I64u:%s:%d", m_login, m_symbol, m_stOrder_Check.m_type);
			strloginSymbolKey.Format(L"%s:%s:4", m_login, m_symbol);
			st_OrderCount m_stOrderCount = {};
			CStaticClass::m_OrderCountMap.Lookup(strloginSymbolKey, m_stOrderCount);
			m_TotalNo_Of_StopBuy = m_stOrderCount.m_TotalOrder;
			m_TotalNo_Of_Cancelled_StopBuy = m_stOrderCount.m_TotalCancelledOrder;
			m_TotalNo_Of_Passed_StopBuy = m_stOrderCount.m_TotalExecutedOrder;
			m_TotalNo_Of_Pending_StopBuy = m_TotalNo_Of_StopBuy - m_TotalNo_Of_Cancelled_StopBuy - m_TotalNo_Of_Passed_StopBuy;

			m_stOrderCount = {};
			strloginSymbolKey.Format(L"%s:%s:5", m_login, m_symbol);
			CStaticClass::m_OrderCountMap.Lookup(strloginSymbolKey, m_stOrderCount);
			m_TotalNo_Of_StopSell = m_stOrderCount.m_TotalOrder;
			m_TotalNo_Of_Cancelled_StopSell = m_stOrderCount.m_TotalCancelledOrder;
			m_TotalNo_Of_Passed_StopSell = m_stOrderCount.m_TotalExecutedOrder;
			m_TotalNo_Of_Pending_StopSell = m_TotalNo_Of_StopSell - m_TotalNo_Of_Cancelled_StopSell - m_TotalNo_Of_Passed_StopSell;

			m_stOrderCount = {};
			strloginSymbolKey.Format(L"%s:%s:2", m_login, m_symbol);
			CStaticClass::m_OrderCountMap.Lookup(strloginSymbolKey, m_stOrderCount);
			m_TotalNo_Of_BuyLimit = m_stOrderCount.m_TotalOrder;
			m_TotalNo_Of_Cancelled_BuyLimit = m_stOrderCount.m_TotalCancelledOrder;
			m_TotalNo_Of_Passed_BuyLimit = m_stOrderCount.m_TotalExecutedOrder;
			m_TotalNo_Of_Pending_BuyLimit = m_TotalNo_Of_BuyLimit - m_TotalNo_Of_Cancelled_BuyLimit - m_TotalNo_Of_Passed_BuyLimit;

			m_stOrderCount = {};
			strloginSymbolKey.Format(L"%s:%s:3", m_login, m_symbol);
			CStaticClass::m_OrderCountMap.Lookup(strloginSymbolKey, m_stOrderCount);
			m_TotalNo_Of_SellLimit = m_stOrderCount.m_TotalOrder;
			m_TotalNo_Of_Cancelled_SellLimit = m_stOrderCount.m_TotalCancelledOrder;
			m_TotalNo_Of_Passed_SellLimit = m_stOrderCount.m_TotalExecutedOrder;
			m_TotalNo_Of_Pending_SellLimit = m_TotalNo_Of_SellLimit - m_TotalNo_Of_Cancelled_SellLimit - m_TotalNo_Of_Passed_SellLimit;


			int totalMarketBuyDeal = 0;
			int totalMarketSellDeal = 0;
			m_stOrderCount = {};
			strloginSymbolKey.Format(L"%s:%s:0", m_login, m_symbol);
			CStaticClass::m_OrderCountMap.Lookup(strloginSymbolKey, m_stOrderCount);
			totalMarketBuyDeal = m_stOrderCount.m_TotalOrder;


			m_stOrderCount = {};
			strloginSymbolKey.Format(L"%s:%s:1", m_login, m_symbol);
			CStaticClass::m_OrderCountMap.Lookup(strloginSymbolKey, m_stOrderCount);
			totalMarketSellDeal = m_stOrderCount.m_TotalOrder;


			m_TotalNo_Of_MarketDeal = totalMarketBuyDeal + totalMarketSellDeal;
			m_TotalNo_Of_LimitDeal = m_TotalNo_Of_SellLimit + m_TotalNo_Of_BuyLimit;
			m_TotalNo_Of_StopLimitDeal = m_TotalNo_Of_StopBuy + m_TotalNo_Of_StopSell;

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_StopBuy", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_StopBuy");
				writer.Int(m_TotalNo_Of_StopBuy);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Cancelled_StopBuy", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Cancelled_StopBuy");
				writer.Int(m_TotalNo_Of_Cancelled_StopBuy);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Passed_StopBuy", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Passed_StopBuy");
				writer.Int(m_TotalNo_Of_Passed_StopBuy);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Pending_StopBuy", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Pending_StopBuy");
				writer.Int(m_TotalNo_Of_Pending_StopBuy);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_StopSell", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_StopSell");
				writer.Int(m_TotalNo_Of_StopSell);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Cancelled_StopSell", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Cancelled_StopSell");
				writer.Int(m_TotalNo_Of_Cancelled_StopSell);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Passed_StopSell", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Passed_StopSell");
				writer.Int(m_TotalNo_Of_Passed_StopSell);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Pending_StopSell", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Pending_StopSell");
				writer.Int(m_TotalNo_Of_Pending_StopSell);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_BuyLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_BuyLimit");
				writer.Int(m_TotalNo_Of_BuyLimit);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Cancelled_BuyLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Cancelled_BuyLimit");
				writer.Int(m_TotalNo_Of_Cancelled_BuyLimit);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Passed_BuyLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Passed_BuyLimit");
				writer.Int(m_TotalNo_Of_Passed_BuyLimit);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Pending_BuyLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Pending_BuyLimit");
				writer.Int(m_TotalNo_Of_Pending_BuyLimit);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_SellLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_SellLimit");
				writer.Int(m_TotalNo_Of_SellLimit);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Cancelled_SellLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Cancelled_SellLimit");
				writer.Int(m_TotalNo_Of_Cancelled_SellLimit);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Passed_SellLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Passed_SellLimit");
				writer.Int(m_TotalNo_Of_Passed_SellLimit);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_Pending_SellLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_Pending_SellLimit");
				writer.Int(m_TotalNo_Of_Pending_SellLimit);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_MarketDeal", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_MarketDeal");
				writer.Int(m_TotalNo_Of_MarketDeal);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_LimitDeal", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_LimitDeal");
				writer.Int(m_TotalNo_Of_LimitDeal);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalNo_Of_StopLimitDeal", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalNo_Of_StopLimitDeal");
				writer.Int(m_TotalNo_Of_StopLimitDeal);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalTradedLot_Buy", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalTradedLot_Buy");
				writer.Double(m_TotalTradedLot_Buy);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"TotalTradedLot_Sell", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("TotalTradedLot_Sell");
				writer.Double(m_TotalTradedLot_Sell);
			}

			CString  strLoginSymbolGroup = L"";
			strLoginSymbolGroup.Format(L"%s:%s", m_login, m_symbol);
			CStaticClass::st_BadTradeGoodTrade m_st_BadTradeGoodTrade = {};
			CStaticClass::m_BadTradeGoodTrade.Lookup(strLoginSymbolGroup, m_st_BadTradeGoodTrade);
			double m_badtrade = m_st_BadTradeGoodTrade.m_BadTrade;
			double m_TotalTrade = m_st_BadTradeGoodTrade.m_TotalTrade;
			double m_badtradePer = 0;
			if (m_badtrade != 0)
			{
				m_badtradePer = (m_badtrade / m_TotalTrade) * 100;
			}
			double m_badtradeIgnorePosition = m_st_BadTradeGoodTrade.m_badTradeIgnoringPosition;
			double m_badTradeIPCper = 0;
			if (m_badtradeIgnorePosition != 0)
			{
				m_badTradeIPCper = (m_badtradeIgnorePosition / m_TotalTrade) * 100;
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"badtradeper", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("badtradeper");
				writer.Double(m_badtradePer);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"badtradeipcper", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("badtradeipcper");
				writer.Double(m_badTradeIPCper);
			}

			int m_totaltradeipc = m_st_BadTradeGoodTrade.m_TradeIgnoringPosition;
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"totaltradeipc", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("totaltradeipc");
				writer.Double(m_totaltradeipc);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"totaltrade", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("totaltrade");
				writer.Double(m_TotalTrade);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"badtradeipc", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("badtradeipc");
				writer.Double(m_badtradeIgnorePosition);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"badtrade", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("badtrade");
				writer.Double(m_badtrade);
			}


			/*CString strLogVal = L"";
			strLogVal.Format(L"Login:%s Amount:%.2lf Debit/Credit:%s Volume:%.2lf GE:%.4lf", m_login, m_clientplnet, m_debitCredit, m_volume, m_clientexposure);
			//(strLogVal);*/
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"buysell", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("buysell");
				/*string ssbuySell = string(CT2CA(m_buySell));
				const char* strbuySell = ssbuySell.c_str();*/
				if (m_volume < 0)
				{
					writer.String("Sell");
				}
				else
				{
					writer.String("Buy");
				}
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"debitcredit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("debitcredit");
				/*string ssdebitCredit = string(CT2CA(m_debitCredit));
				const char* strdebitCredit = ssdebitCredit.c_str();*/
				if (m_clientplnet < 0)
				{
					writer.String("Debit");
				}
				else
				{
					writer.String("Credit");
				}

			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"exchange", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("exchange");
				string ssexchange = string(CT2CA(m_Exchange));
				const char* strexchange = ssexchange.c_str();
				writer.String(strexchange);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"international", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("international");
				string ssinternational = string(CT2CA(m_international));
				const char* strinternational = ssinternational.c_str();
				writer.String(strinternational);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"sector", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("sector");
				string sssector = string(CT2CA(m_sector));
				const char* strsector = sssector.c_str();
				writer.String(strsector);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"industry", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("industry");
				string ssindustry = string(CT2CA(m_industry));
				const char* strindustry = ssindustry.c_str();
				writer.String(strindustry);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"page", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("page");
				string sspage = string(CT2CA(m_page));
				const char* strpage = sspage.c_str();
				writer.String(strpage);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"categary", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("categary");
				string sscategary = string(CT2CA(m_categary));
				const char* strcategary = sscategary.c_str();
				writer.String(strcategary);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"rm", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("rm");
				writer.Double(m_rm);
			}


			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"lpratio", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("lpratio");
				writer.Double(m_lpratio);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"commoditygroup", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("commoditygroup");
				string sscommoditygroup = string(CT2CA(m_commoditygroup));
				const char* strcommoditygroup = sscommoditygroup.c_str();
				writer.String(strcommoditygroup);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"lpvolume", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("lpvolume");
				writer.Double(m_lpvolume);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"qtyaftermulti", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("qtyaftermulti");
				writer.Double(m_qtyAfterMulti);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"rmp", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("rmp");
				writer.Double(m_rmp);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"qtymulti", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("qtymulti");
				writer.Double(m_QtyMulti);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"currencybase", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("currencybase");
				string sscurrencybase = string(CT2CA(m_currencybase));
				const char* strcurrencybase = sscurrencybase.c_str();
				writer.String(strcurrencybase);
			}
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"creditLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				writer.Key("creditLimit");
				writer.Double(m_CreditLimit);
			}

			CStaticClass::stConfig m_stConfig = {};
			CString loginSymbolKey = L"";
			CStaticClass::m_mutex_LimitConfig.Lock();
			CString SymbolGroup = L"";
			if (m_symbol.GetLength() >= 5)
			{
				SymbolGroup = m_symbol.Mid(0, m_symbol.GetLength() - 3);
			}
			else
			{
				SymbolGroup = m_symbol;
			}
			int limitValue = 0;
			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"BuyPositionLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				loginSymbolKey.Format(L"%s:%s:1", m_login, SymbolGroup);
				m_stConfig = {};
				CStaticClass::LimitConfig.Lookup(loginSymbolKey, m_stConfig);
				limitValue = m_stConfig.Limit;
				writer.Key("BuyPositionLimit");
				writer.Int(limitValue);
			}


			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"SellPositionLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				loginSymbolKey.Format(L"%s:%s:2", m_login, SymbolGroup);
				m_stConfig = {};
				CStaticClass::LimitConfig.Lookup(loginSymbolKey, m_stConfig);
				limitValue = m_stConfig.Limit;
				writer.Key("SellPositionLimit");
				writer.Int(limitValue);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"AllSymbolBuyPositionLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				loginSymbolKey.Format(L"%s:%s:3", m_login, L"SHARE");
				m_stConfig = {};
				CStaticClass::LimitConfig.Lookup(loginSymbolKey, m_stConfig);
				limitValue = m_stConfig.Limit;
				writer.Key("AllSymbolBuyPositionLimit");
				writer.Int(limitValue);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"AllSymbolSellPositionLimit", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				loginSymbolKey.Format(L"%s:%s:4", m_login, L"SHARE");
				m_stConfig = {};
				CStaticClass::LimitConfig.Lookup(loginSymbolKey, m_stConfig);
				limitValue = m_stConfig.Limit;
				writer.Key("AllSymbolSellPositionLimit");
				writer.Int(limitValue);
			}

			m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"AllPositionLimitLoginwise", m_Requestmessage);
			m_columnSubs = 0;
			m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
			if (m_columnSubs == 1)
			{
				loginSymbolKey.Format(L"%s:%s:5", m_login, L"SHARE");
				m_stConfig = {};
				CStaticClass::LimitConfig.Lookup(loginSymbolKey, m_stConfig);
				limitValue = m_stConfig.Limit;
				writer.Key("AllPositionLimitLoginwise");
				writer.Int(limitValue);
			}
			CStaticClass::m_mutex_LimitConfig.Unlock();





			CStaticClass::m_mutexcolumnSubs.Unlock();


		}
		
		writer.EndObject();


		if (dataSendingFlag == 1)
		{
			string strforsend = "";
			str_FinalJsonUpdate = s.GetString();
			int dataSize = str_FinalJsonUpdate.GetLength();

			//if (dataSize >= 1500)
			if (dataSize >= 1500000)
			{
				writer.EndArray();
				writer.EndObject();

				str_FinalJsonUpdate = s.GetString();
				////(L"Test");
				//(str_FinalJsonUpdate);
				strforsend = CT2A(str_FinalJsonUpdate.GetString());
				SendDataToClient(client, strforsend, m_clientcontexKey, m_activeClient);
				
				s.Clear();
				writer.Flush();

				writer.Reset(s);

				writer.StartObject();
				writer.Key("type");
				writer.String(stmessageType);
				writer.Key("insert");
				writer.StartArray();
				dataSendingFlag = 0;
			}
		}

	}

	if (dataSendingFlag == 1)
	{
		writer.EndArray();
		writer.EndObject();
		string strforsend = "";
		str_FinalJsonUpdate = s.GetString();
		//(str_FinalJsonUpdate);
		
		strforsend = CT2A(str_FinalJsonUpdate.GetString());
		SendDataToClient(client, strforsend, m_clientcontexKey, m_activeClient);
		dataSendingFlag = 0;
	}
}

void CStaticClass::calculateClientWiseAllPosition()
{	
#pragma region Setting_Global_Variable_To_Local_Variable
	
	CString strkey=CStaticClass::strKeyTransfer;	
	WakeAllConditionVariable(&m_cv_Thread);
	CString strUserID=strkey.Mid(0,strkey.Find(L":"));		
	//CMap<CString,LPCTSTR,CStaticClass::st_netpositionClientWise,CStaticClass::st_netpositionClientWise&>  mapNetPositionClientWise_ThreadWise;
	NetPositionClientWise_All mapNetPositionClientWise_ThreadWise;
	struct localClientTotalAmount
	{
		double m_clientgrosstotal;
		double m_clientBrokTotal;
		double m_clientnettotal;
	};	
	CMap<CString,LPCTSTR,localClientTotalAmount,localClientTotalAmount>  maplocalClientTotalAmount;
	CMap<CString,LPCTSTR, st_TickBidAskLast, st_TickBidAskLast>  maplocalClientSendedLTP;

	int thread_Status=0;
	SSL_session* client=NULL;		
		
	int m_activeClient = 0;
	m_mutex_ClientList.Lock();
	//(L"L24");
		st_ClientContext tmp_st={};
		m_ClientContext.Lookup(strkey,tmp_st);
		thread_Status=tmp_st.m_startCalculationThread ;
		client=tmp_st.m_clientConnection;
		m_activeClient = tmp_st.m_activeClient;
	m_mutex_ClientList.Unlock();
	//(L"_U24");
	
	m_mutex_Tick.Lock();
	POSITION pos=mapNetPositionClientWise.GetStartPosition();	
	int All_positiondata_count = mapNetPositionClientWise.GetCount();
	CString str_AllPositionData = L"";	
	str_AllPositionData.Format(L"All Position Data Count %d", All_positiondata_count);
	CStaticClass::m_logfile.LogEvent(str_AllPositionData);
	while (pos!=NULL )
	{
		st_netpositionClientWise m_stMain={};
		CString loginSymbolKey=L"";
		mapNetPositionClientWise.GetNextAssoc(pos,loginSymbolKey,m_stMain);
		CString strTradingLogin=loginSymbolKey.Mid(0,loginSymbolKey.Find(L":"));
		if (std::find(tmp_st.m_logins.begin(), tmp_st.m_logins.end(), strTradingLogin) != tmp_st.m_logins.end())
		{
			mapNetPositionClientWise_ThreadWise.SetAt(loginSymbolKey,m_stMain);				
		}
	}
	m_mutex_Tick.Unlock();	
#pragma endregion

	while (thread_Status==1)
	{
				
		//Sleep(500);
		#pragma region Getting_Client_Status
		thread_Status = 0;
		m_mutex_ClientList.Lock();		
			st_ClientContext st_Check={};
			m_ClientContext.Lookup(strkey,st_Check);
			thread_Status=st_Check.m_startCalculationThread;
			int subscribedRequest = st_Check.m_clientrequests_List.Total();
			CString m_strLogData = L"";
			m_strLogData.Format(L"");
		m_mutex_ClientList.Unlock();		
		#pragma endregion
		if (st_Check.m_sendDashboardData == 1)
		{	
			
			sendClientPosition_Insert(client, m_activeClient, strUserID, strkey,L"FETCH_DASHBOARD_DATA", L"DASHBOARD_DATA",  &mapNetPositionClientWise_ThreadWise);
			
			m_mutex_ClientList.Lock();
				st_Check.m_sendDashboardData = 0;			
				m_ClientContext.SetAt(strkey, st_Check);
			m_mutex_ClientList.Unlock();
		}		
		//End Of Calculating Data For Dashboard
		
		if (st_Check.m_fetch_client_position_timerwise == 1)
		{
			UINT64 CurrentTime = _time64(NULL);
			if((CurrentTime- st_Check.m_netPositionTime_TIMERWISE_LastTime)>=5)
			{
				
				sendClientPosition_Insert(client, m_activeClient, strUserID, strkey, L"FETCH_CLIENT_POSITIONS_TIMERWISE_START", L"CLIENT_POSITIONS_TIMERWISE",  &mapNetPositionClientWise_ThreadWise);
				
			}	
			m_mutex_ClientList.Lock();
				st_Check.m_fetch_client_position_timerwise = 0;
				m_ClientContext.SetAt(strkey, st_Check);
			m_mutex_ClientList.Unlock();
		}
		//End Of Calculating Data For Client Netposition

		///////////Sending Netposition Data First Time
		if (st_Check.m_ClientWiseNetPositionStart_FirstTime==1)
		{
			//int  totalColumns = st_Check.m_clientrequests_List.Total();
			
			sendClientPosition_Insert(client, m_activeClient, strUserID, strkey, L"FETCH_CLIENT_POSITIONS", L"CLIENT_POSITION",  &mapNetPositionClientWise_ThreadWise);
			
			m_mutex_ClientList.Lock();
				st_Check.m_ClientWiseNetPositionStart_FirstTime = 0;
				st_Check.m_ClientWiseNetPositionStart = 1;
				m_ClientContext.SetAt(strkey, st_Check);
			m_mutex_ClientList.Unlock();
		}
		//End Of Calculating Data For Client Netposition
		
		CStaticClass::updatedTickSymbolArray tmpArray;

		CStaticClass::updatedTickSymbolArray tmpArrayForSymbolPosition;

		CStaticClass::updatedSymbolLoginArray tmpArrayLoginSymbol;

		TMTArray<st_netpositionClientWise> tmpNewposition;
		TMTArray<st_netpositionClientWise> tmpNewposition_Updated;

		TMTArray<st_netpositionClientWise> tmpNewpositionSymbolWise;
		
		////(L"L1");
		//Sleep(500);
		m_mutex_ClientList.Lock();
		
		//(L"102");
		////(L"L1 Locked");
			st_ClientContext st_Check_Update={};
			m_ClientContext.Lookup(strkey,st_Check_Update);	
			
			
			tmpArrayForSymbolPosition.Assign(st_Check_Update.m_TickSymbolListForSymbolPosition);

			tmpArray.Assign(st_Check_Update.m_updatedTickSymbolArray);
			tmpArrayLoginSymbol.Assign(st_Check_Update.m_updatedVolumeSymbolArray); 	


			st_Check_Update.m_TickSymbolListForSymbolPosition.Clear();

			st_Check_Update.m_updatedTickSymbolArray.Clear();	
			
			st_Check_Update.m_updatedVolumeSymbolArray.Clear();
			//m_newNetPositionAdded
			tmpNewposition.Assign(st_Check_Update.m_newNetPositionAdded);
			//m_newNetPositionUpdated
			tmpNewposition_Updated.Assign(st_Check_Update.m_newNetPositionUpdated);

			int test_count = tmpNewposition_Updated.Total();
			
			int newposCount=tmpNewposition.Total();
			for (int n=0;n<newposCount;n++)
			{
				st_netpositionClientWise mtmpst={};
				mtmpst=tmpNewposition[n];
				CString symbol=L"";
				symbol=mtmpst.m_symbol;
				st_updatedTickSymbol st_tickSymbol={};
				CMTStr::Copy(st_tickSymbol.m_symbol, symbol);
				tmpArray.Add(&st_tickSymbol);
			}
			newposCount=tmpNewposition_Updated.Total();
			for (int n=0;n<newposCount;n++)
			{
				st_netpositionClientWise mtmpst={};
				mtmpst=tmpNewposition_Updated[n];
				CString symbol=L"";
				symbol=mtmpst.m_symbol;
				st_updatedTickSymbol st_tickSymbol={};
				CMTStr::Copy(st_tickSymbol.m_symbol, symbol);
				tmpArray.Add(&st_tickSymbol);

				int checkExistingSymbol=0;
				int totalTickData=tmpArrayForSymbolPosition.Total();
				for(int k=0;k<totalTickData;k++)
				{
					st_updatedTickSymbol st_tickLocalCheck={};
					st_tickLocalCheck=tmpArrayForSymbolPosition[k];
					CString tmpstrSymbol=st_tickLocalCheck.m_symbol;
					if (tmpstrSymbol==symbol)
					{
						checkExistingSymbol=1;
						break;
					}
				}
				if (checkExistingSymbol==0)
				{
					tmpArrayForSymbolPosition.Add(&st_tickSymbol);
				}
			}

			
			tmpNewpositionSymbolWise.Assign(st_Check_Update.m_newNetPositionAdded);			
			
			st_Check_Update.m_newNetPositionAdded.Clear();
			st_Check_Update.m_newNetPositionUpdated.Clear();
			m_ClientContext.SetAt(strkey,st_Check_Update);	
		m_mutex_ClientList.Unlock();
		//(L"U102");		
		//Sending Client Wise Netposition Updated Data
		if (st_Check.m_ClientWiseNetPositionStart==1)
		{
			//Sending Pre Qty
			if(st_Check.m_getPreQtyUpdate==1)	
			{	
				
				sendClientPosition_Update(client, m_activeClient, strUserID, strkey, L"FETCH_CLIENT_POSITIONS", L"CLIENT_POSITION", &mapNetPositionClientWise_ThreadWise);
				
				m_mutex_ClientList.Lock();				
					st_Check = {};
					m_ClientContext.Lookup(strkey,st_Check);
					st_Check.m_getPreQtyUpdate=0;					
					m_ClientContext.SetAt(strkey,st_Check);
				m_mutex_ClientList.Unlock();
				
				//(L"U103");				
			}
			//End Of Sending Pre Qty

			//Sending New Arrived rows for a position
			if (tmpNewposition.Total() > 0)
			{
				
				sendClientPosition_NewInsert(client, m_activeClient, strUserID, strkey, L"FETCH_CLIENT_POSITIONS", L"CLIENT_POSITION", &mapNetPositionClientWise_ThreadWise, &tmpNewposition);
				tmpNewposition.Clear();
				
			}
			//End of Sending New Arrived rows for a position


			//Sending Updated Message For New Arrival Deal			
			if (tmpNewposition_Updated.Total() > 0)
			{
				
				sendClientPosition_NewUpdate(client, m_activeClient, strUserID, strkey, L"FETCH_CLIENT_POSITIONS", L"CLIENT_POSITION", &mapNetPositionClientWise_ThreadWise, &tmpNewposition_Updated);
				tmpNewposition_Updated.Clear();
				
			}
			//End Sending Updated Message For New Arrival Deal

			
			/// Sending Update Message When Tick Has Been Changes<summary>			
			int rowsCount=tmpArray.Total(); 
			StringBuffer sN;
			Writer<StringBuffer> writerN(sN);
			writerN.StartObject();
			writerN.Key("type");
			writerN.String("CLIENT_POSITION");
			/*writerN.Key("sendingtime");
			writerN.String(getcurrentTime().c_str());*/
			writerN.Key("updatekey");
			writerN.StartArray();
			writerN.String("login");
			writerN.String("symbol");
			writerN.EndArray();
			writerN.Key("update");
			writerN.StartArray();
			int dataSendingFlag=0;
			
			int m_DataCount = 0;
			CMap<CString, LPCTSTR, int, int> m_subscribedcolumn_Local;
						

			int  totalColumns = st_Check.m_clientrequests_List.Total();
			for(int ci=0;ci< totalColumns;ci++)
			{
				CString  m_ColumnsData=L"";
				m_ColumnsData = st_Check.m_clientrequests_List[ci];												
				if (m_ColumnsData.Find(L":FETCH_CLIENT_POSITIONS:")>=0)
				{					
					m_subscribedcolumn_Local.SetAt(m_ColumnsData,1);
				}
			}

			for(int i=0;i<rowsCount;i++)
			{
				CStaticClass::st_updatedTickSymbol st_symbol={};
				st_symbol=tmpArray[i];
				CString tickUpdatedSymbol=st_symbol.m_symbol;
				loginarray m_loginarray ;
				m_mutex_Tick.Lock();
					CStaticClass::mapSymbolandLogin.Lookup(tickUpdatedSymbol,m_loginarray);					
				m_mutex_Tick.Unlock();
				int totalLoginCount=m_loginarray.Total();				
				for(int k=0;k<totalLoginCount;k++)
				{
					CString strloginForUpdate=m_loginarray[k];
					CString LoginSymbolKey=L"";
					LoginSymbolKey.Format(L"%s:%s",strloginForUpdate,tickUpdatedSymbol);
				
					CString strUpdateData=L"";
					int firstCheck=0;
					CString str_columnJson=L"";
					CString str_FinalJsonUpdate=L"";
									
					CString strKey=L"";
					st_netpositionClientWise st_tmpData={};

					CString    m_name=L"";
					CString    m_symbol=L"";
					double     m_volume=0;
					double     m_previousvolume=0; 
					double     m_difference=0;
					double     m_average=0;
					st_TickBidAskLast     m_lastrate = {};
					double     m_clientfloatingpl=0;
					double     m_clientbalance=0;
					double     m_clientnetamount=0;
					double     m_clientgrosstotal=0;
					double     m_clientbroktotal=0;
					double     m_clientnettotal=0;
					double     m_maxallotedqty=0;
					CString    m_subbroker=L"";
					CString    m_broker=L"";
					double     m_clientexposure=0;
					double     m_extravolume=0;
					double     m_freemargin=0;
					double     m_multi=0;




					CStaticClass::m_mutex_Tick.Lock();
					mapNetPositionClientWise_ThreadWise.Lookup (LoginSymbolKey,st_tmpData);				
					CString    m_login=st_tmpData.m_login;
					m_login=m_login.Trim();
					if (m_login!=L"")
					{
						m_name=st_tmpData.m_name;
						m_symbol=st_tmpData.m_symbol;
						m_volume=*st_tmpData.m_volume;
						m_previousvolume=st_tmpData.m_previousvolume; 
						m_difference=st_tmpData.m_difference;
						m_average=*st_tmpData.m_average;
						m_lastrate=*st_tmpData.m_lastrate;
						m_clientfloatingpl=*st_tmpData.m_clientfloatingpl;
						m_clientbalance=*st_tmpData.m_clientbalance;
						m_clientnetamount=*st_tmpData.m_clientnetamount;
						m_clientgrosstotal=*st_tmpData.m_clientgrosstotal;
						m_clientbroktotal=*st_tmpData.m_clientbroktotal;
						m_clientnettotal=*st_tmpData.m_clientnettotal;
						m_maxallotedqty=*st_tmpData.m_maxallotedqty;
						m_subbroker=st_tmpData.m_subbroker;
						m_broker=st_tmpData.m_broker;
						
						m_extravolume=*st_tmpData.m_extravolume;
						m_freemargin=*st_tmpData.m_freemargin;
						m_multi=*st_tmpData.m_multi;

						CString     m_Company = st_tmpData.m_company;
						CString     m_Exchange = st_tmpData.m_exchange;
						double     m_companyvolume = *st_tmpData.m_companyvolume;
						double     m_brokervolume = *st_tmpData.m_brokervolume;
						double     m_subbrokervolume = *st_tmpData.m_subbrokervolume;
						double     m_clientbrokerage = *st_tmpData.m_clientbroktotal;
						double     m_brokerbrokerage = *st_tmpData.m_brokerBrokarage;
						double     m_subbrokerbrokerage = *st_tmpData.m_subbrokerBrokarage;
						double     m_companybrokerage = *st_tmpData.m_companyBrokarage;

						m_clientexposure = (*st_tmpData.m_clientexposure) /10000000;
						double     m_Companyexposure = (*st_tmpData.m_companyexposure)/10000000;
						double     m_brokerexposure = (*st_tmpData.m_brokerexposure) /10000000;
						double     m_subbrokerexposure = (*st_tmpData.m_subbrokerexposure) /10000000;

						double     m_companyfloatingpl = *st_tmpData.m_companyfloatingpl;
						double     m_brokerfloatingpl = *st_tmpData.m_brokerfloatingpl;
						double     m_subbrokerfloatingpl = *st_tmpData.m_subbrokerfloatingpl;
						double     m_clientbalancepl = *st_tmpData.m_clientbalance;
						double     m_companybalancepl = *st_tmpData.m_companybalance;
						double     m_brokerbalancepl = *st_tmpData.m_brokerbalance;
						double     m_subbrokerbalancepl = *st_tmpData.m_subbrokerbalance;
						double     m_clientplnet = *st_tmpData.m_clientnetamount;
						double     m_companyplnet = *st_tmpData.m_companyNetAmount;
						double     m_brokerplnet = *st_tmpData.m_brokerNetAmount;
						double     m_subbrokerplnet = *st_tmpData.m_subbrokerNetAmount;
						double     m_clientpatti = 100;
						double     m_companypatti = *st_tmpData.m_companyRatio;
						double     m_brokerpatti = *st_tmpData.m_brokerRatio;
						double     m_subbrokerpatti = *st_tmpData.m_subbrokerRatio;
						double     m_companybrokeragerate = *st_tmpData.m_companyBrokRate;
						double     m_brokerbrokeragerate = *st_tmpData.m_brokerBrokRate;
						double     m_subbrokerbrokeragerate = *st_tmpData.m_subBrokerBrokRate;
						double     m_clientbrokeragerate = *st_tmpData.m_clientBrokRate;

						double     m_clientgrossamount = *st_tmpData.m_ClientGrossAmount;
						double     m_brokergrossamount = *st_tmpData.m_BrokerGrossAmount;
						double     m_subbrokergrossamount = *st_tmpData.m_SubBrokerGrossAmount;
						double     m_companygrossamount = *st_tmpData.m_CompanyGrossAmount;






						//Sending Updated Data Of Client Position When Tick Data Has Been Changed.
						if (m_volume!=0)
						{
							st_TickBidAskLast lastLTP = {};							
							maplocalClientSendedLTP.Lookup(LoginSymbolKey,lastLTP);
							if (lastLTP.m_bid!=m_lastrate.m_bid || lastLTP.m_ask != m_lastrate.m_ask)
							{																														
								SYSTEMTIME	time;
								::GetLocalTime(&time);
								CString strLocalTime;
								strLocalTime.Format(L"%04d/%02d/%02d %02d:%02d:%02d:%03d",time.wYear, time.wMonth, time.wDay,time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
								dataSendingFlag=1;
								string sslogin = string(CT2CA(m_login));
								const char* stlogin=sslogin.c_str();
								string sssymbo = string(CT2CA(m_symbol));
								const char* stsymbol=sssymbo.c_str();;
								string ssLocalTime = string(CT2CA(strLocalTime));
								const char* stlocalTime=ssLocalTime.c_str();
								writerN.StartObject();

								CString m_loginColumnKey = L"";
								int m_columnSubs = 0;
								CStaticClass::m_mutexcolumnSubs.Lock();
								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"lastrate", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("lastrate");
									writerN.Double(m_volume < 0 ? m_lastrate.m_ask : m_lastrate.m_bid);
								}
								writerN.Key("login");
								writerN.String(stlogin);
								writerN.Key("symbol");
								writerN.String(stsymbol);	

								writerN.Key("name");
								string ssName = string(CT2CA(m_name));
								const char* stName = ssName.c_str();
								writerN.String(stName);
								




								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"volume", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("volume");
									writerN.Double(m_volume);
								}


								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"previousvolume", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("previousvolume");
									writerN.Double(m_previousvolume);
								}




								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"difference", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("difference");
									writerN.Double(m_difference);
								}
								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companyvolume", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("companyvolume");
									writerN.Double(m_companyvolume);
								}
								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokervolume", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("brokervolume");
									writerN.Double(m_brokervolume);
								}
								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokervolume", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("subbrokervolume");
									writerN.Double(m_subbrokervolume);
								}
								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientbrokerage", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("clientbrokerage");
									writerN.Double(m_clientbrokerage);
								}
								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerbrokerage", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("brokerbrokerage");
									writerN.Double(m_brokerbrokerage);
								}
								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerbrokerage", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("subbrokerbrokerage");
									writerN.Double(m_subbrokerbrokerage);
								}
								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companybrokerage", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("companybrokerage");
									writerN.Double(m_companybrokerage);
								}
								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientexposure", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("clientexposure");
									writerN.Double(m_clientexposure);
								}
								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"Companyexposure", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("Companyexposure");
									writerN.Double(m_Companyexposure);
								}
								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerexposure", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("brokerexposure");
									writerN.Double(m_brokerexposure);
								}
								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerexposure", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("subbrokerexposure");
									writerN.Double(m_subbrokerexposure);
								}





								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"company", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("company");
									string sscompany = string(CT2CA(m_Company));
									const char* stcompany = sscompany.c_str();
									writerN.String(stcompany);
								}
								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbroker", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("subbroker");
									string sssubbroker = string(CT2CA(m_subbroker));
									const char* stsubbroker = sssubbroker.c_str();
									writerN.String(stsubbroker);
								}
								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"broker", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("broker");
									string ssbroker = string(CT2CA(m_broker));
									const char* stbroker = ssbroker.c_str();
									writerN.String(stbroker);
								}
								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"extravolume", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("extravolume");
									writerN.Double(m_extravolume);
								}



								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientfloatingpl", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("clientfloatingpl");
									writerN.Double(m_clientfloatingpl);
								}
								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companyfloatingpl", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("companyfloatingpl");
									writerN.Double(m_companyfloatingpl);
								}
								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerfloatingpl", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("brokerfloatingpl");
									writerN.Double(m_brokerfloatingpl);
								}
								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerfloatingpl", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("subbrokerfloatingpl");
									writerN.Double(m_subbrokerfloatingpl);
								}
								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientbalancepl", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("clientbalancepl");
									writerN.Double(m_clientbalancepl);
								}

								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientbalance", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("clientbalance");
									writerN.Double(m_clientbalancepl);
								}

								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companybalancepl", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("companybalancepl");
									writerN.Double(m_companybalancepl);
								}
								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerbalancepl", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("brokerbalancepl");
									writerN.Double(m_brokerbalancepl);
								}
								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerbalancepl", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("subbrokerbalancepl");
									writerN.Double(m_subbrokerbalancepl);
								}
								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientplnet", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("clientplnet");
									writerN.Double(m_clientplnet);
								}
								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companyplnet", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("companyplnet");
									writerN.Double(m_companyplnet);
								}
								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokerplnet", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("brokerplnet");
									writerN.Double(m_brokerplnet);
								}
								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokerplnet", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("subbrokerplnet");
									writerN.Double(m_subbrokerplnet);
								}
								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"clientgrossamount", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("clientgrossamount");
									writerN.Double(m_clientgrossamount);
								}
								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"brokergrossamount", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("brokergrossamount");
									writerN.Double(m_brokergrossamount);
								}
								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"subbrokergrossamount", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("subbrokergrossamount");
									writerN.Double(m_subbrokergrossamount);
								}
								m_loginColumnKey.Format(L"%s:%s:%s:", strUserID, L"companygrossamount", m_ClientPositionRequestType);
								m_columnSubs = 0;
								m_subscribedcolumn_Local.Lookup(m_loginColumnKey, m_columnSubs);
								if (m_columnSubs == 1)
								{
									writerN.Key("companygrossamount");
									writerN.Double(m_companygrossamount);
								}


								CStaticClass::m_mutexcolumnSubs.Unlock();
								writerN.EndObject();
								m_DataCount = m_DataCount + 1;
								if (m_DataCount == 3000)
								{
									m_DataCount = 0;
									writerN.EndArray();
									writerN.EndObject();

									string strforsend = "";
									str_FinalJsonUpdate = sN.GetString();
									strforsend = CT2A(str_FinalJsonUpdate.GetString());
									SendDataToClient(client, strforsend, strkey, m_activeClient);
									dataSendingFlag = 0;
									sN.Clear();
									writerN.Flush();
									writerN.Reset(sN);
									writerN.StartObject();
									writerN.Key("type");
									writerN.String("CLIENT_POSITION");
									/*writerN.Key("sendingtime");
									writerN.String(getcurrentTime().c_str());*/
									writerN.Key("updatekey");
									writerN.StartArray();
									writerN.String("login");
									writerN.String("symbol");
									writerN.EndArray();
									writerN.Key("update");
									writerN.StartArray();
									
								}

								maplocalClientSendedLTP.SetAt(LoginSymbolKey,m_lastrate);
							}
						}
					}
					CStaticClass::m_mutex_Tick.Unlock();				
																																				
					
				}
				
		}
		writerN.EndArray();
		writerN.EndObject();
		
		CString str_FinalJsonUpdate = L"";
		if (dataSendingFlag==1)
		{
			string strforsend="";
			str_FinalJsonUpdate=sN.GetString();
			strforsend=CT2A(str_FinalJsonUpdate.GetString());
			SendDataToClient(client, strforsend, strkey, m_activeClient);
			CString strlog = L"";
			
			dataSendingFlag=0;
		}
		
		}
		///
		/// </summary>End Of Sending Client Wise NetPosition  Updated Data
		
		//First Time Comment Change 
		if (st_Check.m_FETCH_EXISTING_COMMENT_CHANGE_DATA_FirstTime==1)
		{
			
			m_mutex_ClientList.Lock();
			////(L"104");
				st_Check = {};
				m_ClientContext.Lookup(strkey,st_Check);
				st_Check.m_FETCH_EXISTING_COMMENT_CHANGE_DATA_FirstTime=0;
				st_Check.m_FETCH_EXISTING_COMMENT_CHANGE_DATA_start=1;
				int m_activeClient = st_Check.m_activeClient;
				m_ClientContext.SetAt(strkey,st_Check);
			m_mutex_ClientList.Unlock();
			////(L"U104");

			CString str_FinalJsonUpdate=CStaticClass::m_sqldata.generateJsonCommentChangeData();
			string strforsend="";
			strforsend=CT2A(str_FinalJsonUpdate.GetString());
			SendDataToClient(client, strforsend, strkey, m_activeClient);
		}
		//End First Time Comment Change
		
		//Start Sending Updated Data for Comment Change
		if (st_Check.m_FETCH_EXISTING_COMMENT_CHANGE_DATA_start==1)
		{			
			m_mutex_ClientList.Lock();
			////(L"105");
				st_Check = {};
				m_ClientContext.Lookup(strkey,st_Check);										
					TMTArray<st_commentChange>	m_commentarray_update;
					m_commentarray_update.Assign(st_Check.m_commentArray_update );
					st_Check.m_commentArray_update.Clear();
					TMTArray<st_commentChange>	m_commentarray_insert;
					m_commentarray_insert.Assign(st_Check.m_commentArray_insert);
					st_Check.m_commentArray_insert.Clear();					

					TMTArray<INT64>	commentArray_delete;
					commentArray_delete.Assign(st_Check.m_commentArray_delete);
					st_Check.m_commentArray_delete.Clear();
					st_Check.m_brokerSendingStart_FirstTime_Call = 0;
					st_Check.m_subbrokerSendingStart_FirstTime_Call = 0;
				m_ClientContext.SetAt(strkey,st_Check);
			m_mutex_ClientList.Unlock();
			////(L"U105");

			int updateCount=m_commentarray_update.Total();
			StringBuffer s;
			Writer<StringBuffer> writer(s);
			if (updateCount>0)
			{
				
				writer.StartObject();
				writer.Key("type");
				writer.String("EXISTING_COMMENT_CHANGE_DATA");	
				writer.Key("updatekey");
				writer.StartArray();
				writer.String("deal");				
				writer.EndArray();
				writer.Key("update");
				writer.StartArray();
			}
			for (int i=0;i<updateCount;i++)
			{
				st_commentChange m_commentST={};
				m_commentST=m_commentarray_update[i];
				INT64 m_deal=m_commentST.deal  ;
				CString m_comment=m_commentST.comment ;
				CString m_commentto=m_commentST.commentTo ;
		
				string sscomment = string(CT2CA(m_comment));
				const char* stcomment=sscomment.c_str();
				string sscommentTo = string(CT2CA(m_commentto));
				const char* stcommentTo=sscommentTo.c_str();
		
				writer.StartObject();								
				writer.Key("deal");
				writer.Int64(m_deal);
				writer.Key("comment");
				writer.String(stcomment);
				writer.Key("commentto");
				writer.String(stcommentTo);		
				writer.EndObject();
			}
			if (updateCount>0)
			{
				writer.EndArray();
				writer.EndObject();				

				CString strData=L"";
				strData=s.GetString();
				string strforsend="";
				strforsend=CT2A(strData.GetString());
				SendDataToClient(client, strforsend, strkey, m_activeClient);


				s.Clear();
				s.Flush();
			}
			m_commentarray_update.Clear();

			
			
			
			int insertcount=m_commentarray_insert.Total();
			if (insertcount>0)
			{				
				writer.StartObject();
				writer.Key("type");
				writer.String("EXISTING_COMMENT_CHANGE_DATA");	
				writer.Key("insert");
				writer.StartArray();
			}
			for (int i=0;i<insertcount;i++)
			{
				st_commentChange m_commentST={};
				m_commentST=m_commentarray_insert[i];
				INT64 m_deal=m_commentST.deal  ;
				CString m_comment=m_commentST.comment ;
				CString m_commentto=m_commentST.commentTo ;
		
				string sscomment = string(CT2CA(m_comment));
				const char* stcomment=sscomment.c_str();
				string sscommentTo = string(CT2CA(m_commentto));
				const char* stcommentTo=sscommentTo.c_str();
		
				writer.StartObject();								
				writer.Key("deal");
				writer.Int64(m_deal);
				writer.Key("comment");
				writer.String(stcomment);
				writer.Key("commentto");
				writer.String(stcommentTo);		
				writer.EndObject();
			}
			if (insertcount>0)
			{
				writer.EndArray();
				writer.EndObject();				

				CString strData=L"";
				strData=s.GetString();
				string strforsend="";
				strforsend=CT2A(strData.GetString());
				SendDataToClient(client, strforsend, strkey, m_activeClient);

				s.Clear();
				s.Flush();
			}
			m_commentarray_insert.Clear();










			int deletecount=commentArray_delete.Total();
			if (deletecount>0)
			{				
				writer.StartObject();
				writer.Key("type");
				writer.String("EXISTING_COMMENT_CHANGE_DATA");	
				writer.Key("deletekey");
				writer.StartArray();
				writer.String("deal");				
				writer.EndArray();
				writer.Key("delete");
				writer.StartArray();				
			}
			for (int i=0;i<deletecount;i++)
			{
				UINT64  m_deal=0;
				
				m_deal=commentArray_delete[i];														
				writer.StartObject();
				writer.Key("deal");
				writer.Int64(m_deal);
				writer.EndObject();				
			}
			if (deletecount>0)
			{
				writer.EndArray();
				writer.EndObject();		
				

				CString strData=L"";
				strData=s.GetString();
				string strforsend="";
				strforsend=CT2A(strData.GetString());
				SendDataToClient(client, strforsend, strkey, m_activeClient);

				s.Clear();
				s.Flush();
			}
			commentArray_delete.Clear();
		}
		//End Sending Updated Data for Comment Change
			
		//sending order first time

		m_mutex_ClientList.Lock();
		int m_ordersendingStatus = st_Check.m_FETCH_ORDER_DATA_FirstTime;
		m_mutex_ClientList.Unlock();
		
		if (m_ordersendingStatus ==1)
		{
			////(L"L14");
			
			m_mutex_ClientList.Lock();
			////(L"106");
				st_Check = {};
				m_ClientContext.Lookup(strkey,st_Check);
				st_Check.m_FETCH_ORDER_DATA_FirstTime=0;
				st_Check.m_FETCH_ORDER_DATA_start=1;
				st_Check.m_brokerSendingStart_FirstTime_Call = 0;
				st_Check.m_subbrokerSendingStart_FirstTime_Call = 0;
				m_ClientContext.SetAt(strkey,st_Check);
			m_mutex_ClientList.Unlock();
			////(L"U106");
			/*CString strjson=CStaticClass::m_sqldata.getOrderData(strkey);
			string strforsend=CT2A(strjson.GetString());			
			SendDataToClient(client, strforsend, strkey, m_activeClient);*/
				
		}
		//Sending Updated Order 
		if (st_Check.m_FETCH_ORDER_DATA_start==1)
		{
			////(L"L15");
			m_mutex_ClientList.Lock();
			////(L"107");
			st_Check = {};
				m_ClientContext.Lookup(strkey,st_Check);
					st_Check.m_FETCH_ORDER_DATA_FirstTime=0;
					st_Check.m_FETCH_ORDER_DATA_start=1;

					TMTArray<st_order>	m_orderarray_update;
					m_orderarray_update.Assign(st_Check.m_orderarray_update);
					st_Check.m_orderarray_update.Clear();
					TMTArray<st_order>	m_orderarray_insert;
					m_orderarray_insert.Assign(st_Check.m_orderarray_insert);
					st_Check.m_orderarray_insert.Clear();
					TMTArray<st_order>	m_orderarray_delete;
					m_orderarray_delete.Assign(st_Check.m_orderarray_delete);
					st_Check.m_orderarray_delete.Clear();

					TMTArray<st_order_ForUpdate> orderarray_ForEdit;
					orderarray_ForEdit.Assign(st_Check.m_orderarray_ForEdit);
					st_Check.m_orderarray_ForEdit.Clear();

				m_ClientContext.SetAt(strkey,st_Check);
				////(L"U107");
			m_mutex_ClientList.Unlock();


			//Sending Updated Insert Order
			int insertRowsCount=m_orderarray_insert.Total();
			for(int i=0;i<insertRowsCount;i++)
			{
				int orderkey=0;
				CStaticClass::st_order st={};
				st=m_orderarray_insert[i];

				CString m_login=st.m_login;
				int m_time =st.m_time - 19800;

				CString m_strTime=L"";		
				CString m_tmp_date=L"";
				CMTStr256 str_time;
				SMTFormat::FormatDateTime(str_time,m_time,true,true);
				m_strTime=str_time.Str();


				int m_deal=st.m_deal ;
				int m_order=st.m_order;
				CString m_symbol=st.m_symbol;
				int m_type=st.m_type ;
				CString strtype=L"";
				if(m_type==2)
				{
					strtype=L"Buy Limit";
				}
				else if(m_type==3)
				{
					strtype=L"Sell Limit";
				}
				else if(m_type==4)
				{
					strtype=L"Buy Stop";
				}
				else if(m_type==5)
				{
					strtype=L"Sell Stop";
				}
				else if(m_type==6)
				{
					strtype=L"Buy Stop";
				}
				else if(m_type==7)
				{
					strtype=L"Sell Stop Limit";
				}
				double m_volume =st.m_volume ;
				double m_price=st.m_price ;
				CString m_comment=st.m_comment;
				CString m_status=st.m_status;
				int m_select=st.m_select ;
				CString strselect=L"";
				if (m_select==0) 
				{
					strselect="false";
				}
				else
				{
					strselect="true";
				}

				CString m_selecttype=st.m_selecttype;
				CString m_subtype=st.m_subtype;
				int m_contraorder=st.m_contraorder ;
				int m_tradeexecutetime=st.m_tradeexecutetime ;
				CString m_ourcomment=st.m_ourcomment;
				int m_orderstate=st.m_orderstate ;

				CString strtmpdata=L"";
		

				CString str_orderstate=L"";
				if (m_orderstate==1001)
				{
					str_orderstate=L"NEW";
				}
				if (m_orderstate==1002)
				{
					str_orderstate=L"UPDATE";
				}
				if (m_orderstate==1003 ||m_orderstate==1005)
				{
					str_orderstate=L"DELETE";
				}
			
				strtmpdata.Format(L"{\"login\":\"%s\",\"time\":%d,\"deal\":%d,\"order\":%d,\"symbol\":\"%s\",\"type\":\"%s\",\"volume\":%.2lf,\"price\":%.4lf,\"comment\":\"%s\",\"status\":\"%s\",\"select\":\"%s\",\"statustype\":\"%s\",\"subtype\":\"%s\",\"contraorder\":%d,\"tradeexecutetime\":%d,\"ourcomment\":\"%s\",\"orderstate\":\"%s\"}", m_login, m_time,      m_deal      ,m_order      ,m_symbol,       strtype,        m_volume,m_price,m_comment,m_status,strselect,m_selecttype,m_subtype,m_contraorder,m_tradeexecutetime,m_ourcomment,str_orderstate);
				CString str_FinalJsonUpdate=L"";
				str_FinalJsonUpdate.Format(L"{\"type\":\"ORDER_DATA\",\"insert\":[%s]}",strtmpdata);	
				string strforsend="";
				strforsend=CT2A(str_FinalJsonUpdate.GetString());
				SendDataToClient(client, strforsend, strkey, m_activeClient);
		}


			//Sending Updated Update Order
			int insertRowsCountUpdate=m_orderarray_update.Total();
			for(int i=0;i<insertRowsCountUpdate;i++)
			{
				int orderkey=0;
				CStaticClass::st_order st={};
				st=m_orderarray_update[i];

				CString m_login=st.m_login;
				int m_time =st.m_time - 19800;

				CString m_strTime=L"";		
				CString m_tmp_date=L"";
				CMTStr256 str_time;
				SMTFormat::FormatDateTime(str_time,m_time,true,true);
				m_strTime=str_time.Str();


				int m_deal=st.m_deal ;
				int m_order=st.m_order;
				CString m_symbol=st.m_symbol;
				int m_type=st.m_type ;
				CString strtype=L"";
				if(m_type==2)
				{
					strtype=L"Buy Limit";
				}
				else if(m_type==3)
				{
					strtype=L"Sell Limit";
				}
				else if(m_type==4)
				{
					strtype=L"Buy Stop";
				}
				else if(m_type==5)
				{
					strtype=L"Sell Stop";
				}
				else if(m_type==6)
				{
					strtype=L"Buy Stop";
				}
				else if(m_type==7)
				{
					strtype=L"Sell Stop Limit";
				}
				double m_volume =st.m_volume ;
				double m_price=st.m_price ;
				CString m_comment=st.m_comment;
				CString m_status=st.m_status;
				int m_select=st.m_select ;
				CString strselect=L"";
				if (m_select==0) 
				{
					strselect="false";
				}
				else
				{
					strselect="true";
				}

				CString m_selecttype=st.m_selecttype;
				CString m_subtype=st.m_subtype;
				int m_contraorder=st.m_contraorder ;
				int m_tradeexecutetime=st.m_tradeexecutetime ;
				CString m_ourcomment=st.m_ourcomment;
				int m_orderstate=st.m_orderstate ;

				CString strtmpdata=L"";
		

				CString str_orderstate=L"";
				if (m_orderstate==1001)
				{
					str_orderstate=L"NEW";
				}
				if (m_orderstate==1002)
				{
					str_orderstate=L"UPDATE";
				}
				if (m_orderstate==1003 ||m_orderstate==1005)
				{
					str_orderstate=L"DELETE";
				}
			
				strtmpdata.Format(L"{\"login\":\"%s\",\"time\":%d,\"deal\":%d,\"order\":%d,\"symbol\":\"%s\",\"type\":\"%s\",\"volume\":%.2lf,\"price\":%.4lf,\"comment\":\"%s\",\"status\":\"%s\",\"select\":\"%s\",\"statustype\":\"%s\",\"subtype\":\"%s\",\"contraorder\":%d,\"tradeexecutetime\":%d,\"ourcomment\":\"%s\",\"orderstate\":\"%s\"}", m_login, m_time,      m_deal      ,m_order      ,m_symbol,       strtype,        m_volume,m_price,m_comment,m_status,strselect,m_selecttype,m_subtype,m_contraorder,m_tradeexecutetime,m_ourcomment,str_orderstate);
				CString str_FinalJsonUpdate=L"";
				str_FinalJsonUpdate.Format(L"{\"type\":\"ORDER_DATA\",\"updatekey\":[\"order\"],\"update\":[%s]}",strtmpdata);	
				string strforsend="";
				strforsend=CT2A(str_FinalJsonUpdate.GetString());
				SendDataToClient(client, strforsend, strkey, m_activeClient);
		}
			//Sending Updated Delete Order
			int insertRowsCount_delete=m_orderarray_delete.Total();			
			for(int i=0;i<insertRowsCount_delete;i++)
			{
				int orderkey=0;
				CStaticClass::st_order st={};
				st=m_orderarray_delete[i];

				CString m_login=st.m_login;
				int m_time =st.m_time;

				CString m_strTime=L"";		
				CString m_tmp_date=L"";
				CMTStr256 str_time;
				SMTFormat::FormatDateTime(str_time,m_time,true,true);
				m_strTime=str_time.Str();				
				int m_order=st.m_order;				
				CString strtmpdata=L"";
				
					strtmpdata.Format(L"{\"order\":%d}", m_order);												
					CString str_FinalJsonUpdate=L"";
					str_FinalJsonUpdate.Format(L"{\"type\":\"ORDER_DATA\",\"deletekey\":[\"order\"],\"delete\":[%s]}",strtmpdata);	
					string strforsend="";
					strforsend=CT2A(str_FinalJsonUpdate.GetString());
					SendDataToClient(client, strforsend, strkey, m_activeClient);									
		     }
			//Sending Updated Update Order
			int tRowsCountOrderarray_ForEdit=orderarray_ForEdit.Total();
			StringBuffer s;
			Writer<StringBuffer> writer(s);
			if (tRowsCountOrderarray_ForEdit>0)
			{				
				writer.StartObject();
				writer.Key("type");
				writer.String("ORDER_DATA");	
				writer.Key("updatekey");
				writer.StartArray();
				writer.String("order");				
				writer.EndArray();
				writer.Key("update");
				writer.StartArray();
			}
			for(int i=0;i<tRowsCountOrderarray_ForEdit;i++)
			{
				int orderkey=0;
				CStaticClass::st_order_ForUpdate st_ForUpdate={};
				st_ForUpdate=orderarray_ForEdit[i];
				INT64    m_order = st_ForUpdate.m_order;
				int      m_select = st_ForUpdate.m_select;
				int      m_select_update = st_ForUpdate.m_select_update;

				CString  m_selecttype = st_ForUpdate.m_selecttype;
				int      m_selecttype_update = st_ForUpdate.m_selecttype_update;
				CString  m_subtype = st_ForUpdate.m_subtype;
				int      m_subtype_update = st_ForUpdate.m_subtype_update;
				INT64    m_contraorder = st_ForUpdate.m_contraorder;
				int      m_contraorder_updtae = st_ForUpdate.m_contraorder_updtae;
				INT64    m_tradeexecutetime = st_ForUpdate.m_tradeexecutetime;
				int		 m_tradeexecutetime_update = st_ForUpdate.m_tradeexecutetime_update;
				CString  m_ourcomment = st_ForUpdate.m_ourcomment;
				int		 m_ourcomment_update = st_ForUpdate.m_ourcomment_update;




				CStaticClass::m_mutex_order.Lock();				
					CStaticClass::st_order st = {};
					CStaticClass::m_Orika_orderHastable.Lookup(m_order, st);
				CStaticClass::m_mutex_order.Unlock();

				CString m_login = st.m_login;
			int m_time = st.m_time- 19800;

			CString m_strTime = L"";
			CString m_tmp_date = L"";
			CMTStr256 str_time;
			SMTFormat::FormatDateTime(str_time, m_time, true, true);
			m_strTime = str_time.Str();


			int m_deal = st.m_deal;
			//int m_order = st.m_order;
			CString m_symbol = st.m_symbol;
			int m_type = st.m_type;
			CString strtype = L"";
			if (m_type == 2)
			{
				strtype = L"Buy Limit";
			}
			else if (m_type == 3)
			{
				strtype = L"Sell Limit";
			}
			else if (m_type == 4)
			{
				strtype = L"Buy Stop";
			}
			else if (m_type == 5)
			{
				strtype = L"Sell Stop";
			}
			else if (m_type == 6)
			{
				strtype = L"Buy Stop";
			}
			else if (m_type == 7)
			{
				strtype = L"Sell Stop Limit";
			}
			double m_volume = st.m_volume;
			double m_price = st.m_price;
			CString m_comment = st.m_comment;
			CString m_status = st.m_status;
			//int m_select = st.m_select;
			CString strselect = L"";
			if (m_select == 0)
			{
				strselect = "false";
			}
			else
			{
				strselect = "true";
			}

			//CString m_selecttype = st.m_selecttype;
			//CString m_subtype = st.m_subtype;
			//int m_contraorder = st.m_contraorder;
			//int m_tradeexecutetime = st.m_tradeexecutetime;
			//CString m_ourcomment = st.m_ourcomment;
			int m_orderstate = st.m_orderstate;

			CString strtmpdata = L"";


			CString str_orderstate = L"";
			if (m_orderstate == 1001)
			{
				str_orderstate = L"NEW";
			}
			if (m_orderstate == 1002)
			{
				str_orderstate = L"UPDATE";
			}
			if (m_orderstate == 1003 || m_orderstate == 1005)
			{
				str_orderstate = L"DELETE";
			}




				

												
				writer.StartObject();								





				writer.Key("order");
				writer.Int64(m_order);


				writer.Key("login");
				string sslogin = string(CT2CA(m_login));
				const char* stlogin = sslogin.c_str();
				writer.String(stlogin);
				writer.Key("time");
				/*string ssTime = string(CT2CA(m_strTime));
				const char* stTime = ssTime.c_str();*/
				writer.Int(m_time);
				writer.Key("deal");
				writer.Int64(m_deal);
				
				writer.Key("symbol");
				string ssSymbol = string(CT2CA(m_symbol));
				const char* stSymbol = ssSymbol.c_str();
				writer.String(stSymbol);
				writer.Key("type");
				string sstype = string(CT2CA(strtype));
				const char* sttype = sstype.c_str();
				writer.String(sttype);
				writer.Key("volume");
				writer.Double(m_volume);
				writer.Key("price");
				writer.Double(m_price);
				writer.Key("comment");
				string sscomment = string(CT2CA(m_comment));
				const char* stcomment = sscomment.c_str();
				writer.String(stcomment);
				writer.Key("status");
				string ssstatus = string(CT2CA(m_status));
				const char* ststatus = ssstatus.c_str();
				writer.String(ststatus);												
				writer.Key("orderstate");
				string ssorderstate = string(CT2CA(str_orderstate));
				const char* storderstate = ssorderstate.c_str();
				writer.String(storderstate);




				if (m_select_update==1)
				{
					const char* strSelect="";
					if (m_select==1)
					{
						strSelect="true";
					}
					else
					{
						strSelect="false";
					}
					writer.Key("select");
					writer.String(strSelect);
				}
				/*if (m_selecttype_update==1)
				{*/
					string strstatustype = string(CT2CA(m_selecttype));
					const char* ststatustype=strstatustype.c_str();
					writer.Key("statustype");
					writer.String(ststatustype);		
				//}
				/*if (m_subtype_update==1)
				{*/
					string strsubtype = string(CT2CA(m_subtype));
					const char* stsubtype=strsubtype.c_str();
					writer.Key("subtype");
					writer.String(stsubtype);	
				//}
				/*if (m_contraorder_updtae==1)
				{*/
					writer.Key("contraorder");
					writer.Int64(m_contraorder);	
				//}
				/*if(m_tradeexecutetime_update==1)
				{*/
					writer.Key("tradeexecutetime");
					writer.Int64(m_tradeexecutetime);	
				//}
				/*if (m_ourcomment_update==1)
				{*/
					string strourcomment = string(CT2CA(m_ourcomment));
					const char* stourcomment=strourcomment.c_str();
					writer.Key("ourcomment");
					writer.String(stourcomment);					
				//}



				writer.EndObject();				
			}
			if (tRowsCountOrderarray_ForEdit>0)
			{
				writer.EndArray();
				writer.EndObject();				

				CString strData=L"";
				strData=s.GetString();
				string strforsend="";
				strforsend=CT2A(strData.GetString());
				SendDataToClient(client, strforsend, strkey, m_activeClient);
				s.Clear();
				s.Flush();
			}
		}
		
		

		
		////(L"Data Start Processing End ");
			
		//Sleep(1);
		
		
	}

	
	CStaticClass::m_mutex_ClientList.Lock();
	CStaticClass::st_ClientContext st_Check = {};
	CStaticClass::m_ClientContext.Lookup(strkey, st_Check);
	st_Check.m_thread_terminate = 1;
	CStaticClass::m_ClientContext.SetAt(strkey, st_Check);
	CStaticClass::m_mutex_ClientList.Unlock();
}


















CStaticClass::CStaticClass()
{
	
}

void CStaticClass::initializePointerArray()
{
	for (int i=0;i<35000;i++)
	{
		//lastrateArray[i]=(double *)malloc(sizeof(double)); 
		lastrateArray[i] = (CStaticClass::st_TickBidAskLast*)malloc(sizeof(CStaticClass::st_TickBidAskLast));
	}
	for (int i=0;i<6000;i++)
	{
		clientgrosstotalArray[i]=(double *)malloc(sizeof(double)); 
	}	

	for (int i = 0; i < 6000; i++)
	{
		CompanyVolumeAfterMultiTotalArray[i] = (double*)malloc(sizeof(double));
		*CompanyVolumeAfterMultiTotalArray[i] = 0;
	}
	
	for (int i = 0; i < 6000; i++)
	{
		LpVolumeTotal[i] = (double*)malloc(sizeof(double));
		*LpVolumeTotal[i] = 0;
	}

	for (int i=0;i<6000;i++)
	{
		clientbroktotalArray[i]=(double *)malloc(sizeof(double)); 
	}	
	for (int i=0;i<6000;i++)
	{
		clientnettotalArray[i]=(double *)malloc(sizeof(double)); 
	}			
}
	
CStaticClass::~CStaticClass()
{
}
void CStaticClass::initializeconnection()
{
	CoInitialize(NULL);		
	////lOCAL SERVER
	//hr=connection.OpenFromInitializationString(L"Provider=SQLNCLI11.1;Password=ok@12345;Persist Security Info=False;User ID=sa;Initial Catalog=ORIKA;Data Source=USER-PC;Use Procedure for Prepare=1;Auto Translate=True;Packet Size=4096;Workstation ID=WINDOWS-LOJSHQK;Initial File Name=\"\";Use Encryption for Data=False;Tag with column collation when possible=False;MARS Connection=False;DataTypeCompatibility=0;Trust Server Certificate=False;Application Intent=READWRITE");					
	////MCX SERVER
	//hr=connection.OpenFromInitializationString(L"Provider=SQLNCLI11.1;Password=ok@12345;Persist Security Info=False;User ID=sa;Initial Catalog=Orika;Data Source=.\\sqlexpress;Use Procedure for Prepare=1;Auto Translate=True;Packet Size=4096;Workstation ID=WINDOWS-LOJSHQK;Initial File Name=\"\";Use Encryption for Data=False;Tag with column collation when possible=False;MARS Connection=False;DataTypeCompatibility=0;Trust Server Certificate=False;Application Intent=READWRITE");					
	//Share Server
	hr=connection.OpenFromInitializationString(L"Provider=SQLNCLI11.1;Password=" + CStaticClass::SqlServerPassword + ";Persist Security Info=False;User ID=" + CStaticClass::SqlServerUserID + ";Initial Catalog=" + CStaticClass::SqlServerDatabase + ";Data Source=" + CStaticClass::SqlServerAdd + ";Use Procedure for Prepare=1;Auto Translate=True;Packet Size=4096;Workstation ID=WIN-CE63GLSHUM0;Initial File Name=\"\";Use Encryption for Data=False;Tag with column collation when possible=False;MARS Connection=False;DataTypeCompatibility=0;Trust Server Certificate=False;Application Intent=READWRITE");
	//hr=connection.OpenFromInitializationString(L"Provider=SQLNCLI11.1;Password=ok@12345;Persist Security Info=False;User ID=sa;Initial Catalog=Orika;Data Source=.\\sqlexpress;Use Procedure for Prepare=1;Auto Translate=True;Packet Size=4096;Workstation ID=WINDOWS-LOJSHQK;Initial File Name=\"\";Use Encryption for Data=False;Tag with column collation when possible=False;MARS Connection=False;DataTypeCompatibility=0;Trust Server Certificate=False;Application Intent=READWRITE");					
	/*hr=connection.OpenFromInitializationString(L"Provider=SQLOLEDB.1;Password=ok@12345;Persist Security Info=True;User ID=sa;Initial Catalog=Orika_Share_Test;Data Source=.\\sqlexpress;Use Procedure for Prepare=1;Auto Translate=True;Packet Size=4096;Workstation ID=SLSERDEMO;Use Encryption for Data=False;Tag with column collation when possible=False");					*/
	

	CStaticClass::m_requestResponseData.SetAt(L"FETCH_CLIENT_MASTER_META_DATA",L"CLIENT_MASTER_META_DATA");

	CStaticClass::m_requestResponseData.SetAt(L"FETCH_CLIENT_MASTER_DATA",L"CLIENT_MASTER_DATA");
	CStaticClass::m_requestResponseData.SetAt(L"FETCH_BROKER_WISE_NET_POSITION_META_DATA",L"BROKER_WISE_NET_POSITION_META_DATA");
	CStaticClass::m_requestResponseData.SetAt(L"FETCH_SUBBROKER_WISE_NET_POSITION_META_DATA",L"SUBBROKER_WISE_NET_POSITION_META_DATA");


	CStaticClass::m_tablesname.SetAt(L"FETCH_CLIENT_META_DATA", L"Orika_clientmaster");
	CStaticClass::m_tablesname.SetAt(L"FETCH_BROKER_META_DATA", L"orika_broker");
	CStaticClass::m_tablesname.SetAt(L"FETCH_SUBBROKER_META_DATA", L"orika_subbroker");
	

	CStaticClass::m_requestResponseData.SetAt(L"FETCH_SYMBOL_MASTER_META_DATA",L"SYMBOL_MASTER_META_DATA");
	CStaticClass::m_requestResponseData.SetAt(L"FETCH_SYMBOL_MASTER_DATA",L"SYMBOL_MASTER_DATA");
	CStaticClass::m_requestResponseData.SetAt(L"FETCH_SYMBOL_WISE_POSITOINS_META_DATA",L"SYMBOL_WISE_POSITION_META_DATA");


	CStaticClass::m_requestResponseData.SetAt(L"FETCH_CLIENT_POSITION_META_DATA",L"CLIENT_POSITION_META_DATA");
	CStaticClass::m_requestResponseData.SetAt(L"FETCH_CLIENT_META_DATA", L"CLIENT_META_DATA");

	CStaticClass::m_requestResponseData.SetAt(L"FETCH_BROKER_META_DATA", L"BROKER_META_DATA");

	CStaticClass::m_requestResponseData.SetAt(L"FETCH_SUBBROKER_META_DATA", L"SUBBROKER_META_DATA");

	CStaticClass::m_requestResponseData.SetAt(L"FETCH_BROKER_POSITION_META_DATA",L"BROKER_POSITION_META_DATA");
	CStaticClass::m_requestResponseData.SetAt(L"FETCH_SUB_BROKER_POSITION_META_DATA",L"SUB_BROKER_POSITION_META_DATA");
	CStaticClass::m_requestResponseData.SetAt(L"FETCH_BALANCE_DISTRIBUTION_META_DATA",L"BALANCE_DISTRIBUTION_META_DATA");
	CStaticClass::m_requestResponseData.SetAt(L"FETCH_VOLUME_DISTRIBUTION_META_DATA",L"VOLUME_DISTRIBUTION_META_DATA");
	CStaticClass::m_requestResponseData.SetAt(L"FETCH_EXPOSURE_DISTRIBUTION_META_DATA",L"EXPOSURE_DISTRIBUTION_META_DATA");
	CStaticClass::m_requestResponseData.SetAt(L"FETCH_LOTS_DISTRIBUTION_META_DATA",L"LOTS_DISTRIBUTION_META_DATA");
	CStaticClass::m_requestResponseData.SetAt(L"FETCH_BROKARAGE_DISTRIBUTION_META_DATA",L"BROKARAGE_DISTRIBUTION_META_DATA");

	CStaticClass::m_requestResponseData.SetAt(L"FETCH_DASHBOARD_META_DATA", L"DASHBOARD_META_DATA");

	CStaticClass::m_requestResponseData.SetAt(L"FETCH_TOTAL_AMOUNT_META_DATA",L"TOTAL_AMOUNT_META_DATA");

	//CStaticClass::m_requestResponseData.SetAt(L"FETCH_SYMBOL_WISE_POSITIONS",L"SYMBOL_WISE_POSITIONS_DATA");

	CStaticClass::m_requestResponseData.SetAt(L"FETCH_CLIENT_WISE_NET_POSITION_META_DATA",L"CLIENT_WISE_NET_POSITION_META_DATA");
	CStaticClass::m_requestResponseData.SetAt(L"FETCH_ALERT_META_DATA",L"ALERT_META_DATA");
	CStaticClass::m_requestResponseData.SetAt(L"FETCH_CLIENT_WISE_NET_POSITIONS",L"CLIENT_WISE_NET_POSITIONS");
	CStaticClass::m_requestResponseData.SetAt(L"FETCH_CLIENT_POSITIONS",L"CLIENT_POSITIONS");
	CStaticClass::m_requestResponseData.SetAt(L"FETCH_ALERT_DATA",L"ALERT_DATA");
	CStaticClass::m_requestResponseData.SetAt(L"FETCH_EXISTING_ORDER_DATA",L"EXISTING_ORDER_DATA");
	CStaticClass::m_requestResponseData.SetAt(L"FETCH_TICK_DATA",L"TICK_DATA");
	CStaticClass::m_requestResponseData.SetAt(L"FETCH_COMMENT_CHANGE_META_DATA",L"COMMENT_CHANGE_META_DATA");
	CStaticClass::m_requestResponseData.SetAt(L"FETCH_COMMENT_CHANGE_DATA",L"COMMENT_CHANGE_DATA");
	CStaticClass::m_requestResponseData.SetAt(L"FETCH_DEALING_META_DATA",L"DEALING_META_DATA");
	CStaticClass::m_requestResponseData.SetAt(L"FETCH_DEALING_DATA",L"DEALING_DATA");
	CStaticClass::m_requestResponseData.SetAt(L"FETCH_ORDER_META_DATA",L"ORDER_META_DATA");	
	CStaticClass::m_requestResponseData.SetAt(L"FETCH_ORDER_DATA",L"ORDER_DATA");	

	CStaticClass::m_requestResponseData.SetAt(L"FETCH_USER_PANEL_META_DATA",L"USER_PANEL_META_DATA");	

	CStaticClass::m_requestResponseData.SetAt(L"FETCH_USERS_DATA",L"USERS_DATA");

	CStaticClass::m_uniqueFiels.SetAt(L"FETCH_BROKER_WISE_NET_POSITION_META_DATA",L"\"brokername\",\"symbol\"");	

	CStaticClass::m_uniqueFiels.SetAt(L"FETCH_SUBBROKER_WISE_NET_POSITION_META_DATA",L"\"subbrokername\",\"symbol\"");	

	//CStaticClass::m_uniqueFiels
	CStaticClass::m_uniqueFiels.SetAt(L"FETCH_ORDER_META_DATA",L"\"order\"");	
	CStaticClass::m_uniqueFiels.SetAt(L"FETCH_USER_PANEL_META_DATA",L"\"loginUser\"");	

	CStaticClass::m_uniqueFiels.SetAt(L"FETCH_CLIENT_MASTER_META_DATA",L"\"login\",\"symbolGroup\"");	
	//CStaticClass::m_uniqueFiels.SetAt(L"FETCH_CLIENT_MASTER_DATA",L"CLIENT_MASTER_DATA");

	CStaticClass::m_uniqueFiels.SetAt(L"FETCH_SYMBOL_MASTER_META_DATA",L"\"symbol\"");
	//CStaticClass::m_uniqueFiels.SetAt(L"FETCH_SYMBOL_MASTER_DATA",L"SYMBOL_MASTER_DATA");




	CStaticClass::m_uniqueFiels.SetAt(L"FETCH_CLIENT_POSITION_META_DATA",L"\"login\",\"symbol\"");

	CStaticClass::m_uniqueFiels.SetAt(L"FETCH_CLIENT_META_DATA", L"login");

	CStaticClass::m_uniqueFiels.SetAt(L"FETCH_BROKER_META_DATA", L"broker");

	CStaticClass::m_uniqueFiels.SetAt(L"FETCH_SUBBROKER_META_DATA", L"subbroker");

	CStaticClass::m_uniqueFiels.SetAt(L"FETCH_BROKER_POSITION_META_DATA",L"\"brokername\",\"symbol\"");
	CStaticClass::m_uniqueFiels.SetAt(L"FETCH_SUB_BROKER_POSITION_META_DATA",L"\"subbrokername\",\"symbol\"");


	CStaticClass::m_uniqueFiels.SetAt(L"FETCH_BALANCE_DISTRIBUTION_META_DATA",L"\"login\",\"symbol\"");


	CStaticClass::m_uniqueFiels.SetAt(L"FETCH_VOLUME_DISTRIBUTION_META_DATA",L"\"login\",\"symbol\"");
	CStaticClass::m_uniqueFiels.SetAt(L"FETCH_EXPOSURE_DISTRIBUTION_META_DATA",L"\"login\",\"symbol\"");
	CStaticClass::m_uniqueFiels.SetAt(L"FETCH_LOTS_DISTRIBUTION_META_DATA",L"\"login\",\"symbol\"");
	CStaticClass::m_uniqueFiels.SetAt(L"FETCH_BROKARAGE_DISTRIBUTION_META_DATA",L"\"login\",\"symbol\"");




	CStaticClass::m_uniqueFiels.SetAt(L"FETCH_SYMBOL_WISE_POSITOINS_META_DATA",L"\"symbol\"");
	CStaticClass::m_uniqueFiels.SetAt(L"FETCH_CLIENT_WISE_NET_POSITION_META_DATA",L"\"login\",\"symbol\"");
	CStaticClass::m_uniqueFiels.SetAt(L"FETCH_TOTAL_AMOUNT_META_DATA",L"\"login\"");


	CStaticClass::m_uniqueFiels.SetAt(L"FETCH_ALERT_META_DATA",L"\"alertId\"");



	//CStaticClass::m_uniqueFiels.SetAt(L"FETCH_NET_POSITIONS",L"NET_POSITIONS");

	//CStaticClass::m_uniqueFiels.SetAt(L"FETCH_ALERT_DATA",L"ALERT_DATA");
	//CStaticClass::m_uniqueFiels.SetAt(L"FETCH_ORDER_DATA",L"ORDER_DATA");
	//CStaticClass::m_uniqueFiels.SetAt(L"FETCH_TICK_DATA",L"TICK_DATA");


	CStaticClass::m_uniqueFiels.SetAt(L"FETCH_COMMENT_CHANGE_META_DATA",L"\"deal\"");
	//CStaticClass::m_uniqueFiels.SetAt(L"FETCH_COMMENT_CHANGE_DATA",L"COMMENT_CHANGE_DATA");
	CStaticClass::m_uniqueFiels.SetAt(L"FETCH_DEALING_META_DATA",L"id");
	//CStaticClass::m_uniqueFiels.SetAt(L"FETCH_DEALING_DATA",L"DEALING_DATA");


}


double CStaticClass::getClientBalance(CString strLogin,CString strSymbol)
{
	CString loginSymbolKey=L"";
	loginSymbolKey.Format(L"%s:%s",strLogin,strSymbol);
	CStaticClass::st_Orika_BalanceTableAccounting  st={};
	double returnValue=0;
	/*POSITION P=m_Orika_BalanceTableAccountingArray.GetStartPosition();
	while (P!=NULL)
	{
		CString strtmpkey=L"";
		CStaticClass::st_Orika_BalanceTableAccounting  st={};
		m_Orika_BalanceTableAccountingArray.GetNextAssoc(P,strtmpkey,st);
		if (strtmpkey==loginSymbolKey)
		{
			returnValue=returnValue+st.m_clientBalance;
		}
	}	*/
	m_Orika_BalanceTableAccountingArray.Lookup(loginSymbolKey,st);
	returnValue=st.m_clientBalance;
	return returnValue;
}


void CStaticClass::loadClientwisenetpositionData()
{	
	CMap<CString,LPCTSTR,double,double> ClientBrokerageSymbolWise;
	POSITION pos=m_orika_PositionAverageAccountingHastable.GetStartPosition();
	while (pos != NULL)
	{
		st_orika_PositionAverageAccounting st = {};
		CString strKey = L"";
		m_orika_PositionAverageAccountingHastable.GetNextAssoc(pos, strKey, st);
		CString   m_login = st.m_login;
		CString   m_symbol = st.m_symbol;

		int		  m_Action = st.m_Action;
		double    m_Volume = (double)st.m_Volume / 10000;
		double    m_subBrokerVolume = st.m_subBrokerVolume;
		double    m_brokerVolume = st.m_brokerVolume;
		double    m_extraGroupVolume = st.m_extraGroupVolume;
		double    m_companyVolume = st.m_companyVolume;
		double    m_WAvgPrice = st.m_WAvgPrice;
		double    m_currentPrice = st.m_currentPrice;
		double    m_multiplyer = st.m_multiplyer;
		double    m_floatingProfit = st.m_floatingProfit;
		double    m_subBrokerfloatingProfit = st.m_subBrokerfloatingProfit;
		double    m_brokerfloatingProfit = st.m_brokerfloatingProfit;
		double    m_extraGroupfloatingProfit = st.m_extraGroupfloatingProfit;
		double    m_companyfloatingProfit = st.m_companyfloatingProfit;


		CString strSymbolGroup = L"";


		st_Orika_symbolmaster m_stSymbol = {};

		m_Orika_symbolmasterHastable.Lookup(m_symbol, m_stSymbol);
		strSymbolGroup = m_stSymbol.m_symbolGroup;
		if (m_symbol.Find(L".P.") > 0 || m_symbol.Find(L".C.") > 0)
		{
			strSymbolGroup = L"option";
		}

		CString ClientSymbolGroupKey = L"";
		ClientSymbolGroupKey.Format(L"%s:%s", m_login, strSymbolGroup);
		CString ClientKey = L"";
		ClientKey.Format(L"%s:", m_login);

		CString ClientSymbolKey = L"";
		ClientSymbolKey.Format(L"%s:%s", m_login, m_symbol);
		//updating symbol wise login List
		loginarray m_loginarray;
		mapSymbolandLogin.Lookup(m_symbol, m_loginarray);
		int rowscount = m_loginarray.Total();
		if (rowscount > 1)
		{
			int y = 0;
		}
		int checkDataExistence = 0;
		for (int i = 0; i < rowscount; i++)
		{
			CString dataval = m_loginarray[i];
			if (dataval == m_login)
			{
				checkDataExistence = 1;
				break;
			}
		}
		if (checkDataExistence == 0)
		{
			wchar_t tempdata[30];
			CMTStr::Copy(tempdata, m_login);
			m_loginarray.Add(&tempdata);
		}
		int rowsCount = m_loginarray.Total();
		mapSymbolandLogin.SetAt(m_symbol, m_loginarray);



		//End of updating symbol wise login List

		 //Updating data in mapNetPositionClientWise
		st_netpositionClientWise stnetpos = {};


		stnetpos.m_average = (double*)malloc(sizeof(double));
		stnetpos.m_lastrate = (st_TickBidAskLast*)malloc(sizeof(st_TickBidAskLast));
		stnetpos.m_maxallotedqty = (double*)malloc(sizeof(double));
		stnetpos.m_extravolume = (double*)malloc(sizeof(double));
		stnetpos.m_freemargin = (double*)malloc(sizeof(double));
		stnetpos.m_multi = (double*)malloc(sizeof(double));

		stnetpos.m_subbrokerRatio = (double*)malloc(sizeof(double));
		stnetpos.m_brokerRatio = (double*)malloc(sizeof(double));
		stnetpos.m_companyRatio = (double*)malloc(sizeof(double));

		stnetpos.m_clientexposure = (double*)malloc(sizeof(double));
		stnetpos.m_subbrokerexposure = (double*)malloc(sizeof(double));
		stnetpos.m_brokerexposure = (double*)malloc(sizeof(double));
		stnetpos.m_companyexposure = (double*)malloc(sizeof(double));

		stnetpos.m_volume = (double*)malloc(sizeof(double));
		stnetpos.m_subbrokervolume = (double*)malloc(sizeof(double));
		stnetpos.m_brokervolume = (double*)malloc(sizeof(double));
		stnetpos.m_companyvolume = (double*)malloc(sizeof(double));


		stnetpos.m_ClientGrossAmount = (double*)malloc(sizeof(double));
		stnetpos.m_SubBrokerGrossAmount = (double*)malloc(sizeof(double));
		stnetpos.m_BrokerGrossAmount = (double*)malloc(sizeof(double));
		stnetpos.m_CompanyGrossAmount = (double*)malloc(sizeof(double));

		stnetpos.m_brokerageType = (double*)malloc(sizeof(double));

		stnetpos.m_clientBrokRate = (double*)malloc(sizeof(double));
		stnetpos.m_subBrokerBrokRate = (double*)malloc(sizeof(double));
		stnetpos.m_brokerBrokRate = (double*)malloc(sizeof(double));
		stnetpos.m_companyBrokRate = (double*)malloc(sizeof(double));

		stnetpos.m_clientfloatingpl = (double*)malloc(sizeof(double));
		stnetpos.m_subbrokerfloatingpl = (double*)malloc(sizeof(double));
		stnetpos.m_brokerfloatingpl = (double*)malloc(sizeof(double));
		stnetpos.m_companyfloatingpl = (double*)malloc(sizeof(double));

		stnetpos.m_clientbalance = (double*)malloc(sizeof(double));
		stnetpos.m_subbrokerbalance = (double*)malloc(sizeof(double));
		stnetpos.m_brokerbalance = (double*)malloc(sizeof(double));
		stnetpos.m_companybalance = (double*)malloc(sizeof(double));

		stnetpos.m_clientgross = (double*)malloc(sizeof(double));
		stnetpos.m_subbrokergross = (double*)malloc(sizeof(double));
		stnetpos.m_brokergross = (double*)malloc(sizeof(double));
		stnetpos.m_companygross = (double*)malloc(sizeof(double));

		stnetpos.m_clientBrokarage = (double*)malloc(sizeof(double));
		stnetpos.m_subbrokerBrokarage = (double*)malloc(sizeof(double));
		stnetpos.m_brokerBrokarage = (double*)malloc(sizeof(double));
		stnetpos.m_companyBrokarage = (double*)malloc(sizeof(double));

		stnetpos.m_clientnetamount = (double*)malloc(sizeof(double));
		stnetpos.m_subbrokerNetAmount = (double*)malloc(sizeof(double));
		stnetpos.m_brokerNetAmount = (double*)malloc(sizeof(double));
		stnetpos.m_companyNetAmount = (double*)malloc(sizeof(double));

		stnetpos.m_creditLimit = (double*)malloc(sizeof(double));

		//stnetpos.m_LpVolumeTotal= (double*)malloc(sizeof(double));


		CMTStr::Copy(stnetpos.m_login, m_login);


		//Getting Login Name
		CString m_name = L"";
		st_Orika_clientmaster m_st_Orika_clientmaster = {};
		m_Orika_clientmasterHastable.Lookup(m_login, m_st_Orika_clientmaster);
		m_name = m_st_Orika_clientmaster.m_name;
		//End of Getting Login Name

		*stnetpos.m_creditLimit = m_st_Orika_clientmaster.m_creditLimit;
		CMTStr::Copy(stnetpos.m_name, m_name);
		CMTStr::Copy(stnetpos.m_symbol, m_symbol);
		if (st.m_Action == 1)
		{
			*stnetpos.m_volume = (double)-st.m_Volume / 10000;
		}
		else
		{
			*stnetpos.m_volume = (double)st.m_Volume / 10000;
		}

		stnetpos.m_previousvolume = 0;
		stnetpos.m_difference = 0;
		*stnetpos.m_average = st.m_WAvgPrice;

		//Getting lastrate address
		int lastRateArrayIndex = -1;
		symbolLastTickArrayIndex.Lookup(m_symbol, lastRateArrayIndex);
		//End 
		if (lastRateArrayIndex == -1)
		{
			CStaticClass::symbolLastTickArrayIndex.SetAt(m_symbol, CStaticClass::lastrateArrayMaxindex);
			lastRateArrayIndex = CStaticClass::lastrateArrayMaxindex;
			CStaticClass::lastrateArrayMaxindex = CStaticClass::lastrateArrayMaxindex + 1;
			/*CString strlog=L"";
			strlog.Format(L"Symbol:%s Max TickData Index %d",m_symbol,CStaticClass::lastrateArrayMaxindex);
			////(strlog);*/
		}
		CStaticClass::lastrateArray[lastRateArrayIndex]->m_bid = 0;
		CStaticClass::lastrateArray[lastRateArrayIndex]->m_ask = 0;
		CStaticClass::lastrateArray[lastRateArrayIndex]->m_last = 0;

		

		stnetpos.m_lastrate = lastrateArray[lastRateArrayIndex];
		
		*stnetpos.m_clientfloatingpl=0;




		*stnetpos.m_clientbalance=getClientBalance(m_login,m_symbol);
		//*stnetpos.m_clientnetamount=*stnetpos.m_clientbalance+*stnetpos.m_clientfloatingpl;



		



		//Getting ClientGrossTotal address
		int ClientGrossTotalIndex=-1;				
		clientgrosstotalArrayIndex.Lookup(m_login,ClientGrossTotalIndex);
		//End 
		if (ClientGrossTotalIndex==-1)
		{
			//clientgrosstotalArrayMaxindex
			ClientGrossTotalIndex=clientgrosstotalArrayMaxindex;
			clientgrosstotalArrayIndex.SetAt(m_login,ClientGrossTotalIndex);
			clientgrosstotalArrayMaxindex=clientgrosstotalArrayMaxindex+1;
		}	
		stnetpos.m_clientgrosstotal=clientgrosstotalArray[ClientGrossTotalIndex];			
		*clientgrosstotalArray[ClientGrossTotalIndex]=0;

		
		//Getting ClientGrossTotal address
		int clientbroktotalIndex=-1;				
		clientbroktotalArrayIndex.Lookup(m_login,clientbroktotalIndex);
		//End 
		if (clientbroktotalIndex==-1)
		{
			clientbroktotalIndex=clientbroktotalArrayMaxindex;
			clientbroktotalArrayIndex.SetAt(m_login,clientbroktotalIndex);
			clientbroktotalArrayMaxindex=clientbroktotalArrayMaxindex+1;
		}
		stnetpos.m_clientbroktotal=clientbroktotalArray[clientbroktotalIndex];
		*clientbroktotalArray[clientbroktotalIndex]=0;

		
		//Getting ClientGrossTotal address
		
		int clientnettotalIndex=-1;				
		clientnettotalArrayIndex.Lookup(m_login,clientnettotalIndex);
		
		if (clientnettotalIndex==-1)
		{	
			clientnettotalIndex=clientnettotalArrayMaxindex;
			clientnettotalArrayIndex.SetAt(m_login,clientnettotalIndex);									
			clientnettotalArrayMaxindex=clientnettotalArrayMaxindex+1;			
		}
		//End 
		stnetpos.m_clientnettotal=clientnettotalArray[clientnettotalIndex];		
		*clientnettotalArray[clientnettotalIndex]=0;		

		int max_AllotedQty=0;
		st_Orika_Clientgatewayconfig m_st={};
		m_Orika_ClientgatewayconfigHastable.Lookup(ClientSymbolGroupKey,m_st);
		*stnetpos.m_maxallotedqty=m_st.m_symbolPositionLimit;
		CMTStr::Copy(stnetpos.m_subbroker,m_st_Orika_clientmaster.m_subBroker);
		CMTStr::Copy(stnetpos.m_broker,m_st_Orika_clientmaster.m_broker);
		CMTStr::Copy(stnetpos.m_company, m_st_Orika_clientmaster.m_company);
		*stnetpos.m_multi=m_stSymbol.m_multiplayer;
		if (stnetpos.m_multi==0)
		{
			*stnetpos.m_multi=1;
		}		
				
		*stnetpos.m_extravolume=*stnetpos.m_maxallotedqty-*stnetpos.m_volume ;
		*stnetpos.m_freemargin=0;
		
		//ClientSymbolGroupKey
		st_Orika_clientbrokerage m_st_Orika_clientbrokerage={};
		CStaticClass::m_Orika_clientbrokerageHastable.Lookup(ClientSymbolGroupKey,m_st_Orika_clientbrokerage);
		//ClientKey
		CString strBrokerageLogin=m_st_Orika_clientbrokerage.m_login;
		if (strBrokerageLogin.Trim()==L"")
		{
			CStaticClass::m_Orika_clientbrokerageHastable.Lookup(ClientKey,m_st_Orika_clientbrokerage);
		}
		
		st_Orika_PLDevideRatio m_st_Orika_PLDevideRatio={};
		CStaticClass::m_Orika_PLDevideRatioHastable.Lookup(ClientSymbolGroupKey,m_st_Orika_PLDevideRatio);
		strBrokerageLogin=m_st_Orika_PLDevideRatio.m_login;
		
		if (strBrokerageLogin==L"")
		{
			CStaticClass::m_Orika_PLDevideRatioHastable.Lookup(ClientKey,m_st_Orika_PLDevideRatio);
		}


		*stnetpos.m_subbrokerRatio=m_st_Orika_PLDevideRatio.m_subBrokerPLRatio ;		
		*stnetpos.m_brokerRatio=m_st_Orika_PLDevideRatio.m_brokerPLRatio ;
		*stnetpos.m_companyRatio=m_st_Orika_PLDevideRatio.m_companyPLRatio ;
		

		


		*stnetpos.m_subbrokervolume = ((*stnetpos.m_volume) * (*stnetpos.m_subbrokerRatio)) / 100;
		*stnetpos.m_brokervolume = ((*stnetpos.m_volume) * (*stnetpos.m_brokerRatio)) / 100;
		*stnetpos.m_companyvolume = ((*stnetpos.m_volume) * (*stnetpos.m_companyRatio)) / 100;

		
		*stnetpos.m_brokerageType=m_st_Orika_clientbrokerage.m_brokageType ;
		*stnetpos.m_clientBrokRate=m_st_Orika_clientbrokerage.m_clientBrokage ;
		*stnetpos.m_subBrokerBrokRate=m_st_Orika_clientbrokerage.m_subBrokerBrokage ;
		*stnetpos.m_brokerBrokRate=m_st_Orika_clientbrokerage.m_bokerBrokage ;
		*stnetpos.m_companyBrokRate=m_st_Orika_clientbrokerage.m_companyBrokage ;

		
		*stnetpos.m_clientexposure = (*stnetpos.m_average * (*stnetpos.m_volume) * (*stnetpos.m_multi));
		*stnetpos.m_subbrokerexposure = ((*stnetpos.m_clientexposure) * (*stnetpos.m_subbrokerRatio)) / 100;
		*stnetpos.m_brokerexposure = ((*stnetpos.m_clientexposure) * (*stnetpos.m_brokerRatio)) / 100;
		*stnetpos.m_companyexposure = ((*stnetpos.m_clientexposure) * (*stnetpos.m_companyRatio)) / 100;

		//*stnetpos.m_clientfloatingpl		
		*stnetpos.m_subbrokerfloatingpl= ((*stnetpos.m_clientfloatingpl) * (*stnetpos.m_subbrokerRatio)) / 100;
		*stnetpos.m_brokerfloatingpl= ((*stnetpos.m_clientfloatingpl) * (*stnetpos.m_brokerRatio)) / 100;
		*stnetpos.m_companyfloatingpl= ((*stnetpos.m_clientfloatingpl) * (*stnetpos.m_companyRatio)) / 100;


		//*stnetpos.m_clientbalance;
		*stnetpos.m_subbrokerbalance = ((*stnetpos.m_clientbalance) * (*stnetpos.m_subbrokerRatio)) / 100;
		*stnetpos.m_brokerbalance = ((*stnetpos.m_clientbalance) * (*stnetpos.m_brokerRatio)) / 100;
		*stnetpos.m_companybalance = ((*stnetpos.m_clientbalance) * (*stnetpos.m_companyRatio)) / 100;


		*stnetpos.m_ClientGrossAmount = *stnetpos.m_clientbalance + *stnetpos.m_clientfloatingpl;
		*stnetpos.m_SubBrokerGrossAmount = ((*stnetpos.m_ClientGrossAmount) * (*stnetpos.m_subbrokerRatio)) / 100;
		*stnetpos.m_BrokerGrossAmount = ((*stnetpos.m_ClientGrossAmount) * (*stnetpos.m_brokerRatio)) / 100;
		*stnetpos.m_CompanyGrossAmount = ((*stnetpos.m_ClientGrossAmount) * (*stnetpos.m_companyRatio)) / 100;		
				
		CMTStr::Copy(stnetpos.m_commoditygroup, m_stSymbol.m_Group);
		CMTStr::Copy(stnetpos.m_exchange, m_stSymbol.m_exchange);
		CMTStr::Copy(stnetpos.m_international, m_stSymbol.m_international);
		CMTStr::Copy(stnetpos.m_sector, m_stSymbol.m_sector);
		CMTStr::Copy(stnetpos.m_industry, m_stSymbol.m_industry);
		stnetpos.m_rmp= m_stSymbol.m_rmp;
		stnetpos.m_QtyMulti= m_stSymbol.m_QtyMulti;
		CMTStr::Copy(stnetpos.m_page, m_stSymbol.m_page);
		CMTStr::Copy(stnetpos.m_categary, m_stSymbol.m_categary);
		stnetpos.m_rm= ((*stnetpos.m_companyvolume)* (*stnetpos.m_average) * (*stnetpos.m_multi)* stnetpos.m_rmp)/10000000;

		CMTStr::Copy(stnetpos.m_currencybase, m_stSymbol.m_currencybase);
		CMTStr::Copy(stnetpos.m_commoditygroup, m_stSymbol.m_Group);

		if (stnetpos.m_QtyMulti == 0)
		{
			stnetpos.m_QtyMulti = 1;
		}
		stnetpos.m_qtyAfterMulti= (*stnetpos.m_companyvolume)* stnetpos.m_QtyMulti;

		
		double comVolumeAfterMulti = stnetpos.m_qtyAfterMulti;




		CStaticClass::m_mutexcommoditygroup.Lock();
			CString strcommoditygroupkey = L"";
			CString m_commoditygroup = m_stSymbol.m_Group;
			strcommoditygroupkey.Format(L"%s:%s", m_login, m_commoditygroup);
			double m_LpRatio = 0;
			CStaticClass::m_LpRatioCommodityGroupWiseMap.Lookup(strcommoditygroupkey, m_LpRatio);
		CStaticClass::m_mutexcommoditygroup.Unlock();

		stnetpos.m_LpRatio = m_LpRatio;
		stnetpos.m_LpVolume = stnetpos.m_qtyAfterMulti * stnetpos.m_LpRatio / 100;


		//Getting LPvolumetotal address
		CString m_group = m_stSymbol.m_Group;
		int  LpVolumeTotalIndex = -1;
		LpVolumeTotalArrayIndex.Lookup(m_group, LpVolumeTotalIndex);
		if (LpVolumeTotalIndex == -1)
		{
			LpVolumeTotalIndex = LpVolumeTotalArrayMaxIndex;
			LpVolumeTotalArrayIndex.SetAt(m_group, LpVolumeTotalIndex);
			LpVolumeTotalArrayMaxIndex = LpVolumeTotalArrayMaxIndex + 1;

			
			stnetpos.m_LpVolumeTotal = LpVolumeTotal[LpVolumeTotalIndex];
			*LpVolumeTotal[LpVolumeTotalIndex] = 0;
		}
		else
		{
			stnetpos.m_LpVolumeTotal = LpVolumeTotal[LpVolumeTotalIndex];			
		}
		*LpVolumeTotal[LpVolumeTotalIndex] = *LpVolumeTotal[LpVolumeTotalIndex] + (stnetpos.m_LpVolume);
		double m_totalLPvolume = *LpVolumeTotal[LpVolumeTotalIndex];
		CStaticClass::m_LPLots.SetAt(m_group, m_totalLPvolume);



		//Getting CompanytotalvolumeafterMulti address		
		int  CompanyVolumeAfterMultiTotalIndex = -1;
		CompanyVolumeAfterMultiTotalArrayIndex.Lookup(m_group, CompanyVolumeAfterMultiTotalIndex);
		if (CompanyVolumeAfterMultiTotalIndex == -1)
		{
			CompanyVolumeAfterMultiTotalIndex = CompanyVolumeAfterMultiTotalArrayMaxIndex;
			CompanyVolumeAfterMultiTotalArrayIndex.SetAt(m_group, CompanyVolumeAfterMultiTotalIndex);
			CompanyVolumeAfterMultiTotalArrayMaxIndex = CompanyVolumeAfterMultiTotalArrayMaxIndex + 1;


			stnetpos.m_CompanyVolumeTotalAfterMulti = CompanyVolumeAfterMultiTotalArray[CompanyVolumeAfterMultiTotalIndex];
			*CompanyVolumeAfterMultiTotalArray[CompanyVolumeAfterMultiTotalIndex] = 0;
		}
		else
		{
			stnetpos.m_CompanyVolumeTotalAfterMulti = CompanyVolumeAfterMultiTotalArray[CompanyVolumeAfterMultiTotalIndex];
		}
		*CompanyVolumeAfterMultiTotalArray[CompanyVolumeAfterMultiTotalIndex] = *CompanyVolumeAfterMultiTotalArray[CompanyVolumeAfterMultiTotalIndex] + comVolumeAfterMulti;
		double m_totalCompanyVolumeAfterMulti = *CompanyVolumeAfterMultiTotalArray[CompanyVolumeAfterMultiTotalIndex];
		CStaticClass::m_comTotalLotsAfterMultiGroupWise.SetAt(m_group, m_totalCompanyVolumeAfterMulti);

		int test = 0;

		//CStaticClass::m_brokerageLoginSymbolWiseHastable

		st_orika_brokerageLoginSymbolWise m_tmpSt={};
		m_brokerageLoginSymbolWiseHastable.Lookup(ClientSymbolKey,m_tmpSt);		
				



		*stnetpos.m_clientBrokarage =m_tmpSt.m_clientBrokTotal ;
		*stnetpos.m_subbrokerBrokarage =m_tmpSt.m_subBrokerBrokTotal ;
		*stnetpos.m_brokerBrokarage=m_tmpSt.m_brokerBrokTotal ;
		*stnetpos.m_companyBrokarage=m_tmpSt.m_comBrokTotal;
		

		*stnetpos.m_clientnetamount = *stnetpos.m_ClientGrossAmount - *stnetpos.m_clientBrokarage;
		*stnetpos.m_subbrokerNetAmount=*stnetpos.m_SubBrokerGrossAmount-*stnetpos.m_subbrokerBrokarage;
		*stnetpos.m_brokerNetAmount=*stnetpos.m_BrokerGrossAmount-*stnetpos.m_brokerBrokarage;
		*stnetpos.m_companyNetAmount=*stnetpos.m_CompanyGrossAmount-*stnetpos.m_companyBrokarage;

		
		


		if (*stnetpos.m_volume < 0)
		{
			CMTStr::Copy(stnetpos.m_buySell, L"Sell");
		}
		else
		{
			CMTStr::Copy(stnetpos.m_buySell, L"Buy");
		}

		if (*stnetpos.m_clientnetamount < 0)
		{
			/*CString strLogVal = L"";
			strLogVal.Format(L"Amount:%.2lf Debit/Credit:%s", *stnetpos.m_clientnetamount, L"Debit");
			//(strLogVal);*/
			CMTStr::Copy(stnetpos.m_debitCredit, L"Debit");
		}
		else
		{
			/*CString strLogVal = L"";
			strLogVal.Format(L"Amount:%.2lf Debit/Credit:%s", *stnetpos.m_clientnetamount, L"Credit");
			//(strLogVal);*/
			CMTStr::Copy(stnetpos.m_debitCredit, L"Credit");
		}


		mapNetPositionClientWise.SetAt(ClientSymbolKey,stnetpos);
	}
	//Getting Getting Client Total GrssAmount,TotalBrokerage,TotalNetamount
	/*POSITION posG=mapNetPositionClientWise.GetStartPosition();
	while (posG != NULL) 
	{
		st_netpositionClientWise stnetposGT={};	
		CString strtempLogin=L"";
		mapNetPositionClientWise.GetNextAssoc(posG,strtempLogin,stnetposGT);
		CString strLoginFromPosition=stnetposGT.m_login; 
		CStaticClass::st_LoginWiseGrossTotal tmpGT={};
		tmpGT=GettingGrossTotalfromDatabase(strLoginFromPosition);
		*stnetposGT.m_clientgrosstotal=tmpGT.m_grossTotal; 
		*stnetposGT.m_clientbroktotal=tmpGT.m_brokerageTotal; 
		*stnetposGT.m_clientnettotal=tmpGT.m_grossTotal-tmpGT.m_brokerageTotal;
		mapNetPositionClientWise.SetAt(strtempLogin,stnetposGT);
	}*/
	//End
	double testvalue1 = 0;
	CStaticClass::m_comTotalLotsAfterMultiGroupWise.Lookup(L"SILVER", testvalue1);
	int i = 0;
	double testvalue2 = 0;
	CStaticClass::m_comTotalLotsAfterMultiGroupWise.Lookup(L"GOLD", testvalue2);
	 i = 0;

	 double testvalue3 = 0;
	 CStaticClass::m_LPLots.Lookup(L"SILVER", testvalue3);
	 i = 0;
	 double testvalue4 = 0;
	 CStaticClass::m_LPLots.Lookup(L"GOLD", testvalue4);
	 i = 0;
}
CStaticClass::st_LoginWiseGrossTotal CStaticClass::GettingGrossTotalfromDatabase(CString loginName)
{
	CStaticClass::st_LoginWiseGrossTotal tmpST={};
	HRESULT hr=NULL ;
	CCommand<CAccessor<CClientWiseGrossTotal>> data_table;								
	if(!SUCCEEDED(hr))
	{
		return tmpST;
	}
	CString   strCommand=L"";	
	strCommand.Format(L"select t1.[login],TotalBalance,TotalBrokerage from  (select [login],sum(isnull(clientBalance,0) ) as 'TotalBalance' from Orika_BalanceTableAccounting where [login]='" + loginName + "'  group by [login] )t1 left outer join  (select [login],sum(isnull(clientBrokTotal,0) )as 'TotalBrokerage' from Orika_dealtableAccounting where [login]='" + loginName + "'  group by [login] )t2 on t1.login=t2.login ");	
	
	//CStaticClass::m_mutex_order.Lock();
	//(L"L15");
	CStaticClass::m_SqlLock.Lock();
	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr=data_table.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		m_tempSession.Close();
		//CStaticClass::m_mutex_order.Unlock();
		//(L"UL15");
		CStaticClass::m_SqlLock.Unlock();
		return tmpST;
	}
	int i=0;
	CStaticClass::m_Orika_PLDevideRatioHastable.RemoveAll();
	while(hr=data_table.MoveNext()==S_OK)
	{			
		CMTStr::Copy(tmpST.m_login,data_table.m_login);
		tmpST.m_grossTotal=data_table.m_totalGrossamount ;
		tmpST.m_brokerageTotal=data_table.m_totalGrossBrokerage;
	}
	m_tempSession.Close();
	CStaticClass::m_SqlLock.Unlock();
	//CStaticClass::m_mutex_order.Unlock();
	//(L"UL15");
	return tmpST;
}

void CStaticClass::Sql_Updateorika_dealtableaccounting(st_Orika_dealtableAccounting  dt)
{
	HRESULT hr=NULL ;	
	CCommand<CNoAccessor, CNoRowset> UpdateCommand;
	if(!SUCCEEDED(hr))
	{
		return ;
	}

	CString  m_login=dt.m_login;
	int    m_deal=dt.m_deal ;
	int    m_order=dt.m_order ;
	int    m_time=dt.m_time ;
	CString  m_symbol=dt.m_symbol;
	double  m_multiplyer=dt.m_multiplyer ;
	int    m_action=dt.m_action;
	int    m_volume=dt.m_volume ;
	double  m_price=dt.m_price;


	double  m_clientBrokTotal=dt.m_clientBrokTotal ;
	double  m_subBrokerBrokTotal=dt.m_subBrokerBrokTotal; 
	double  m_brokerBrokTotal=dt.m_brokerBrokTotal ;
	double  m_extraGroupBrokTotal=dt.m_extraGroupBrokTotal;
	double  m_comBalancebrokTotal=dt.m_comBalancebrokTotal;


	double  m_brokerVolume=dt.m_brokerVolume ;
	double  m_subBrokerVolume=dt.m_subBrokerVolume ;
	double  m_extraGroupVolume=dt.m_extraGroupVolume;
	double  m_companyVolume=dt.m_companyVolume ;

	double  m_clientLots=dt.m_clientLots ;
	double  m_subBrokerLots=dt.m_subBrokerLots ;
	double  m_brokerLots=dt.m_brokerLots ;
	double  m_extraGroupLots=dt.m_extraGroupLots ;
	double  m_companyLots=dt.m_companyLots;
	int    m_duplicate=dt.m_duplicate ;
	CString m_ExternalID=dt.m_ExternalID;
	CString m_Comment=dt.m_comment;

	CString   strCommand=L"";	

	//Getting Old Position
	CString  strLoginSymbolGroup = L"";
	strLoginSymbolGroup.Format(L"%s:%s", m_login,m_symbol);
	CStaticClass::st_LoginSymbolWiseLastTrade m_st = {};
	CStaticClass::m_LoginSymbolWiseLastTrade.Lookup(strLoginSymbolGroup, m_st);
	int m_oldVolume = m_st.m_volume;
	double m_oldPrice = m_st.m_price;
	int m_oldPosition = m_st.m_position;
	//End of Getting Old Position		

	int m_newVolume = 0;
	if (m_action == 1)
	{
		m_newVolume = -m_volume;
	}
	else
	{
		m_newVolume = m_volume;
	}
	int m_newPosition = m_st.m_position+ m_newVolume;



	double m_position = m_newPosition;
	int m_priceGoUp = 0;
	if (m_oldPrice < m_price)
	{
		m_priceGoUp = 1;
	}

	int  m_priceGoDown=0; 
	if (m_oldPrice >= m_price)
	{
		m_priceGoDown = 1;
	}
	int m_positionCreated=0; 
	if (m_oldPosition == 0)
	{
		m_positionCreated = 1;
	}
	int  m_BadTrade=0; 
	int  m_BadTradeIgnorePosition = 0;
	int  m_TradeIgnorePosition = 0;
	if (m_oldPosition != 0)
	{
		m_TradeIgnorePosition = 1;
	}
	if (m_price >= m_oldPrice)
	{
		if (m_newVolume > 0 )
		{
			m_BadTrade = 1;
			if (m_oldPosition != 0)
			{
				m_BadTradeIgnorePosition = 1;
			}
		}
		
	}
	if (m_price <= m_oldPrice )
	{
		if (m_newVolume < 0 )
		{
			m_BadTrade = 1;
			if (m_oldPosition != 0)
			{
				m_BadTradeIgnorePosition = 1;
			}
		}
		

	}
	


	m_st = {};

	CMTStr::Copy(m_st.m_login,m_login);
	CMTStr::Copy(m_st.m_symbol ,m_symbol);
	m_st.m_position = m_position;
	m_st.m_volume = m_volume;
	m_st.m_price = m_price;
	CStaticClass::m_LoginSymbolWiseLastTrade.SetAt(strLoginSymbolGroup, m_st);


	//Update GoodTrade And Bad Trade
	CStaticClass::st_BadTradeGoodTrade m_st_BadTradeGoodTrade = {};
	CStaticClass::m_BadTradeGoodTrade.Lookup(strLoginSymbolGroup, m_st_BadTradeGoodTrade);
	m_st_BadTradeGoodTrade.m_TotalTrade = m_st_BadTradeGoodTrade.m_TotalTrade + 1;
	if (m_BadTrade == 1)
	{
		m_st_BadTradeGoodTrade.m_BadTrade = m_st_BadTradeGoodTrade.m_BadTrade + 1;
	}
	else
	{
		m_st_BadTradeGoodTrade.m_GoodTrade = m_st_BadTradeGoodTrade.m_GoodTrade + 1;
	}

	if (m_BadTradeIgnorePosition == 1)
	{
		m_st_BadTradeGoodTrade.m_badTradeIgnoringPosition = m_st_BadTradeGoodTrade.m_badTradeIgnoringPosition + 1;
	}	

	if (m_TradeIgnorePosition == 1)
	{
		m_st_BadTradeGoodTrade.m_TradeIgnoringPosition = m_st_BadTradeGoodTrade.m_TradeIgnoringPosition + 1;
	}
	CStaticClass::m_BadTradeGoodTrade.SetAt(strLoginSymbolGroup, m_st_BadTradeGoodTrade);
	//End of Update Good TRade and Bad trade 




	SYSTEMTIME readable_date;
	readable_date = SMTTime::TimeToST(m_time, readable_date);
	int int_dayofweek = readable_date.wDayOfWeek;
	if (int_dayofweek == 1 || int_dayofweek == 5)
	{
		m_positionCreated = 0; 
		m_BadTrade = 0; 
		m_BadTradeIgnorePosition = 0; 
		m_TradeIgnorePosition = 0;
	}
	strCommand.Format(L" Exec proc_UpdateDealTableAccounting '%s',%d,%d,%d,'%s',%.5lf,%d,%d,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%d,'%s','%s',%.5lf,%d,%d,%d,%d,%d,%d",m_login,m_deal,m_order,m_time,m_symbol,m_multiplyer,m_action,m_volume,m_price,m_clientBrokTotal,m_subBrokerBrokTotal,m_brokerBrokTotal,m_extraGroupBrokTotal,m_comBalancebrokTotal,m_brokerVolume,m_subBrokerVolume,m_extraGroupVolume,m_companyVolume,m_clientLots,m_subBrokerLots,m_brokerLots,m_extraGroupLots,m_companyLots,m_duplicate,m_ExternalID,m_Comment, m_position, m_priceGoUp, m_priceGoDown, m_positionCreated, m_BadTrade, m_BadTradeIgnorePosition, m_TradeIgnorePosition);
	//CStaticClass::m_mutex_order.Lock();
	//(L"Orderlock_67");
	CStaticClass::m_SqlLock.Lock();
	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr=UpdateCommand.Open(m_tempSession,(LPCTSTR)strCommand);
	
	UpdateCommand.Close();
	m_tempSession.Close();
	CStaticClass::m_SqlLock.Unlock();
	//CStaticClass::m_mutex_order.Unlock();
	//(L"U_Orderlock_67");
}


void CStaticClass::Sql_Insertorika_positionEntryAccounting(st_orika_positionEntryAccounting dt)
{
	HRESULT hr=NULL ;	
	CCommand<CNoAccessor, CNoRowset> UpdateCommand;
	if(!SUCCEEDED(hr))
	{
		return ;
	}

	int   m_deal=dt.m_deal;
	int m_order=dt.m_order;
	CString m_login=dt.m_login;
	CString m_symbol=dt.m_symbol;
	int   m_time=dt.m_time ;
	int   m_entryAction=dt.m_entryAction;
	int   m_entryVolume=dt.m_entryVolume;
	double m_entryPrice=dt.m_entryPrice;
	double m_currentPrice=dt.m_currentPrice;
	double m_highPrice=dt.m_highPrice;
	double m_lowPrice=dt.m_lowPrice; 	
	double m_subBrokerEntryVolume=dt.m_subBrokerEntryVolume;
	double m_brokerEntryVolume=dt.m_brokerEntryVolume;
	double m_extraGroupEntryVolume=dt.m_extraGroupEntryVolume; 
	double m_companyEntryVolume=dt.m_companyEntryVolume;
	double m_multiplyer=dt.m_multiplyer; 

	CString   strCommand=L"";	
	strCommand.Format(L"insert into orika_positionEntryAccounting (deal,[order],[login],symbol,[time],entryAction,entryVolume,entryPrice,currentPrice,highPrice,lowPrice,subBrokerEntryVolume,brokerEntryVolume,extraGroupEntryVolume,companyEntryVolume,multiplyer) values (%d,%d,'%s','%s',%d,%d,%d,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf)",m_deal,m_order ,m_login,m_symbol,m_time,m_entryAction,m_entryVolume,m_entryPrice,m_currentPrice,m_highPrice,m_lowPrice,m_subBrokerEntryVolume,m_brokerEntryVolume,m_extraGroupEntryVolume,m_companyEntryVolume,m_multiplyer);		
	//CStaticClass::m_mutex_order.Lock();
	//(L"L17");
	CStaticClass::m_SqlLock.Lock();
		CSession m_tempSession;
		m_tempSession.Open(CStaticClass::connection);
		hr=UpdateCommand.Open(m_tempSession,(LPCTSTR)strCommand);	
		UpdateCommand.Close();
		m_tempSession.Close();
	CStaticClass::m_SqlLock.Unlock();
	//(L"UL17");
}
void CStaticClass::Sql_UpdateOrika_order(st_order ot)
{
	HRESULT hr=NULL ;	
	CCommand<CNoAccessor, CNoRowset> UpdateCommand;
	if(!SUCCEEDED(hr))
	{
		return ;
	}

	CString m_login=ot.m_login;
	int m_time=ot.m_time ;
	int m_deal=ot.m_deal ;
	int m_order=ot.m_order;
	CString m_symbol=ot.m_symbol;
	int m_type=ot.m_type ;
	double m_volume=ot.m_volume ;
	double m_price=ot.m_price ;
	CString m_comment=ot.m_comment;
	CString m_status=ot.m_status;
	int m_select=ot.m_select ;
	CString m_selecttype=ot.m_selecttype;
	CString m_subtype=ot.m_subtype;
	int m_contraorder=ot.m_contraorder ;
	int m_tradeexecutetime=ot.m_tradeexecutetime ;
	CString m_ourcomment=ot.m_ourcomment;
	int m_orderstate=ot.m_orderstate ;

	CString   strCommand=L"";	
	if (m_orderstate==1001)
	{
		strCommand.Format(L"insert  orika_order([login],[time] ,deal ,[order] ,symbol ,[type] ,volume ,price ,comment ,[status],[select],selecttype ,subtype  ,contraorder ,tradeexecutetime ,ourcomment ,orderstate )values(%s,%d ,%d ,%d ,'%s' ,%d ,%.5lf ,%.5lf,'%s' ,'%s',0,'','',0,0,'',%d)",m_login,m_time ,m_deal ,m_order ,m_symbol ,m_type ,m_volume ,m_price ,m_comment ,m_status,m_orderstate);		
	}
	if (m_orderstate==1002 ||m_orderstate==1003 || m_orderstate==1005|| m_orderstate == 1009)
	{
		
			strCommand.Format(L"Exec proc_UpdateOrder '%s',%d,%d,%d,'%s',%d,%.5lf,%.5lf,'%s','%s',%d,'%s','%s',%d,%d,'%s',%d", m_login, m_time, m_deal, m_order, m_symbol, m_type, m_volume, m_price, m_comment, m_status, m_select, m_selecttype, m_subtype, m_contraorder, m_tradeexecutetime, m_ourcomment, m_orderstate);				
	}	
	
	//CStaticClass::m_mutex_order.Lock();
	//(L"Orderlock_63");
	CStaticClass::m_SqlLock.Lock();
	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr=UpdateCommand.Open(m_tempSession,(LPCTSTR)strCommand);	
	UpdateCommand.Close();
	m_tempSession.Close();
	CStaticClass::m_SqlLock.Unlock();
	//CStaticClass::m_mutex_order.Unlock();
	//(L"U_Orderlock_63");
}

void CStaticClass::Sql_UpdateOrika_BalanceTableAccounting(st_Orika_BalanceTableAccounting dt)
{
	HRESULT hr=NULL ;	
	CCommand<CNoAccessor, CNoRowset> UpdateCommand;
	if(!SUCCEEDED(hr))
	{
		return ;
	}

	CString m_login=dt.m_login;
	CString m_symbol=dt.m_symbol;
	int   m_entryDeal=dt.m_entryDeal;
	int   m_entryTime=dt.m_entryTime;
	int   m_entryOrder=dt.m_entryOrder;	
	int   m_entryAction=dt.m_entryAction;
	int   m_entryVolume=dt.m_entryVolume;
	double m_entryPrice=dt.m_entryPrice;
	int   m_exitDeal=dt.m_exitDeal;
	int   m_exitTime=dt.m_exitTime;
	int   m_exitOrder=dt.m_exitOrder;
	int   m_exitAction=dt.m_exitAction;
	int   m_exitVolume=dt.m_exitVolume;
	double m_exitPrice=dt.m_exitPrice;
	double m_plPointLoss=dt.m_plPointLoss;
	double m_plPointProfit=dt.m_plPointProfit;
	double m_multiplyer=dt.m_multiplyer;
	double m_clientBalance=dt.m_clientBalance;
	double m_subBrokerBalance=dt.m_subBrokerBalance;
	double m_brokerBalance=dt.m_brokerBalance ;
	double m_extraGroupBalance=dt.m_extraGroupBalance ; 
	double m_companyBalance=dt.m_companyBalance ;

	CString   strCommand=L"";	
	strCommand.Format(L"insert into Orika_BalanceTableAccounting([login],symbol,entryDeal,entryTime,entryOrder,entryAction,entryVolume,entryPrice,exitDeal,exitTime,exitOrder,exitAction,exitVolume,exitPrice,plPointLoss,plPointProfit,multiplyer,clientBalance,subBrokerBalance,brokerBalance,extraGroupBalance,companyBalance)values('%s','%s',%d,%d,%d,%d,%d,%.5lf,%d,%d,%d,%d,%d,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf)",m_login,m_symbol,m_entryDeal,m_entryTime,m_entryOrder,m_entryAction,m_entryVolume,m_entryPrice,m_exitDeal,m_exitTime,m_exitOrder,m_exitAction,m_exitVolume,m_exitPrice,m_plPointLoss,m_plPointProfit,m_multiplyer,m_clientBalance,m_subBrokerBalance,m_brokerBalance,m_extraGroupBalance,m_companyBalance);		
	
	//CStaticClass::m_mutex_order.Lock();
	//(L"Orderlock_64");
	CStaticClass::m_SqlLock.Lock();
	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr=UpdateCommand.Open(m_tempSession,(LPCTSTR)strCommand);
	
	UpdateCommand.Close();
	m_tempSession.Close();
	CStaticClass::m_SqlLock.Unlock();
	//CStaticClass::m_mutex_order.Unlock();
	//(L"U_Orderlock_64");
}



void CStaticClass::Sql_Updateorika_PositionAverageAccounting(st_orika_PositionAverageAccounting dt)
{
	HRESULT hr=NULL ;	
	CCommand<CNoAccessor, CNoRowset> UpdateCommand;
	if(!SUCCEEDED(hr))
	{
		return ;
	}

	CString  m_login=dt.m_login;
	CString  m_symbol=dt.m_symbol;
	int    m_Action=dt.m_Action;
	int    m_Volume=dt.m_Volume; 
	double  m_subBrokerVolume=dt.m_subBrokerVolume;
	double  m_brokerVolume=dt.m_brokerVolume;
	double  m_extraGroupVolume=dt.m_extraGroupVolume;
	double  m_companyVolume=dt.m_companyVolume;
	double  m_WAvgPrice=dt.m_WAvgPrice;	
	double  m_currentPrice=dt.m_currentPrice;
	double  m_multiplyer=dt.m_multiplyer;
	double  m_floatingProfit=dt.m_floatingProfit;
	double  m_subBrokerfloatingProfit=dt.m_subBrokerfloatingProfit;
	double  m_brokerfloatingProfit=dt.m_brokerfloatingProfit;
	double  m_extraGroupfloatingProfit=dt.m_extraGroupfloatingProfit;
	double  m_companyfloatingProfit=dt.m_companyfloatingProfit;

	CString   strCommand=L"";	
	strCommand.Format(L"if (select count(*) from orika_PositionAverageAccounting where  login='%s' and  symbol='%s')=0 begin  insert into orika_PositionAverageAccounting([login],symbol,[Action],Volume,subBrokerVolume,brokerVolume,extraGroupVolume,companyVolume,WAvgPrice,currentPrice,multiplyer,floatingProfit,subBrokerfloatingProfit,brokerfloatingProfit,extraGroupfloatingProfit,companyfloatingProfit)values('%s','%s',%d,%d,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf,%.5lf)  end else begin   update  orika_PositionAverageAccounting   set [Action]=%d,Volume=%d,subBrokerVolume=%.5lf,brokerVolume=%.5lf,extraGroupVolume=%.5lf,companyVolume=%.5lf,WAvgPrice=%.5lf where [login]='%s' and  symbol='%s'       end ",m_login,m_symbol,m_login,m_symbol,m_Action,m_Volume,m_subBrokerVolume,m_brokerVolume,m_extraGroupVolume,m_companyVolume,m_WAvgPrice,m_currentPrice,m_multiplyer,m_floatingProfit,m_subBrokerfloatingProfit,m_brokerfloatingProfit,m_extraGroupfloatingProfit,m_companyfloatingProfit,m_Action,m_Volume,m_subBrokerVolume,m_brokerVolume,m_extraGroupVolume,m_companyVolume,m_WAvgPrice ,m_login,m_symbol);		
	
	//CStaticClass::m_mutex_order.Lock();
	//(L"Orderlock_65");
	CStaticClass::m_SqlLock.Lock();
	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr=UpdateCommand.Open(m_tempSession,(LPCTSTR)strCommand);
	
	UpdateCommand.Close();
	m_tempSession.Close();
	CStaticClass::m_SqlLock.Unlock();
	//CStaticClass::m_mutex_order.Unlock();
	//(L"U_Orderlock_65");
}


void CStaticClass::Updateorika_positionEntryAccountingHashtable(st_Orika_dealtableAccounting  dt)
{		
	int			m_deal_dta=dt.m_deal;
	int         m_order_dta=dt.m_order;
	CString		m_login_dta=dt.m_login ;
	CString		m_symbol_dta=dt.m_symbol;
	int			m_time_dta =dt.m_time ;
	int			m_entryAction_dta=dt.m_action ;
	int			m_entryVolume_dta=dt.m_volume ;
	double		m_entryPrice_dta=dt.m_price ;
	double		m_currentPrice_dta=dt.m_price ;
	double       m_multiplyer_dta=dt.m_multiplyer;
	
	CStaticClass::st_Orika_symbolmaster m_st_Orika_symbolmaster={};
	CStaticClass::m_Orika_symbolmasterHastable.Lookup(m_symbol_dta,m_st_Orika_symbolmaster);
	CString m_symbolGroup=m_st_Orika_symbolmaster.m_symbolGroup;
	if (m_symbol_dta.Find(L".P.") > 0 || m_symbol_dta.Find(L".C.") > 0)
	{
		m_symbolGroup = L"option";
	}
	CString LoginSymbolKey=L"";
	LoginSymbolKey.Format(L"%s:%s",m_login_dta,m_symbol_dta);


	CString LoginSymbolgroupKey=L"";
	LoginSymbolgroupKey.Format(L"%s:%s",m_login_dta,m_symbolGroup);



	CStaticClass::st_Orika_PLDevideRatio m_st_Orika_PLDevideRatio={};
	CStaticClass::m_Orika_PLDevideRatioHastable.Lookup(LoginSymbolgroupKey,m_st_Orika_PLDevideRatio);						
	if (m_st_Orika_PLDevideRatio.m_brokerPLRatio==0)
	{
		CStaticClass::m_Orika_PLDevideRatioHastable.Lookup(m_login_dta,m_st_Orika_PLDevideRatio);
	}
	double m_brokerPLRatioRate=m_st_Orika_PLDevideRatio.m_brokerPLRatio ;
	double m_subBrokerPLRatioRate=m_st_Orika_PLDevideRatio.m_subBrokerPLRatio ;
	double m_extragroupPLRatioRate=m_st_Orika_PLDevideRatio.m_extragroupPLRatio ;
	double m_companyPLRatioRate=m_st_Orika_PLDevideRatio.m_companyPLRatio ;



	
	int			m_deal=dt.m_deal;
	CString		m_login=dt.m_login ;
	CString		m_symbol=dt.m_symbol;
	int			m_time =dt.m_time ;
	int			m_entryAction=dt.m_action ;
	int			m_entryVolume=dt.m_volume ;
	double		m_entryPrice=dt.m_price ;
	double		m_currentPrice=dt.m_price ;
	double		m_highPrice=0;
	double		m_lowPrice=0;; 	
	double		m_subBrokerEntryVolume=dt.m_subBrokerVolume ;
	double		m_brokerEntryVolume=dt.m_brokerVolume ;
	double		m_extraGroupEntryVolume=dt.m_extraGroupVolume ; 
	double		m_companyEntryVolume=dt.m_companyVolume;
			

		CString m_loginSymbolKey=L"";
		m_loginSymbolKey.Format(L"%s:%s",m_login_dta,m_symbol_dta);
		st_orika_PositionAverageAccounting m_stpac={};
		m_orika_PositionAverageAccountingHastable.Lookup(m_loginSymbolKey,m_stpac);

										
		st_orika_positionEntryAccounting m_st_orika_positionEntryAccounting_out={};
		m_st_orika_positionEntryAccounting_out.m_deal=dt.m_deal;
		m_st_orika_positionEntryAccounting_out.m_order=dt.m_order;
		CMTStr::Copy(m_st_orika_positionEntryAccounting_out.m_login,dt.m_login) ;
		CMTStr::Copy(m_st_orika_positionEntryAccounting_out.m_symbol,dt.m_symbol);
		m_st_orika_positionEntryAccounting_out.m_time =dt.m_time ;
		m_st_orika_positionEntryAccounting_out.m_entryAction=dt.m_action ;
		m_st_orika_positionEntryAccounting_out.m_entryVolume=dt.m_volume ;
		m_st_orika_positionEntryAccounting_out.m_entryPrice=dt.m_price ;
		m_st_orika_positionEntryAccounting_out.m_currentPrice=dt.m_price ;
		m_st_orika_positionEntryAccounting_out.m_highPrice=0;
		m_st_orika_positionEntryAccounting_out.m_lowPrice=0;; 	
		m_st_orika_positionEntryAccounting_out.m_subBrokerEntryVolume=dt.m_subBrokerVolume ;
		m_st_orika_positionEntryAccounting_out.m_brokerEntryVolume=dt.m_brokerVolume ;
		m_st_orika_positionEntryAccounting_out.m_extraGroupEntryVolume=dt.m_extraGroupVolume ; 
		m_st_orika_positionEntryAccounting_out.m_companyEntryVolume=dt.m_companyVolume;
		m_st_orika_positionEntryAccounting_out.m_multiplyer =dt.m_multiplyer;
		
		if (m_stpac.m_Volume ==0)
		{									
						
			Updateorika_PositionAverageAccountingHashtableNewRecord(m_st_orika_positionEntryAccounting_out,0);
			
		}
		else
		{							
				st_orika_positionEntryAccounting m_st_orika_positionEntryAccounting={};
				m_st_orika_positionEntryAccounting.m_deal=0;
				m_st_orika_positionEntryAccounting.m_order=0;
				CMTStr::Copy(m_st_orika_positionEntryAccounting.m_login,m_stpac.m_login) ;
				CMTStr::Copy(m_st_orika_positionEntryAccounting.m_symbol,m_stpac.m_symbol);
				m_st_orika_positionEntryAccounting.m_time =0;
				m_st_orika_positionEntryAccounting.m_entryAction=m_stpac.m_Action  ;
				m_st_orika_positionEntryAccounting.m_entryVolume=m_stpac.m_Volume ;
				m_st_orika_positionEntryAccounting.m_entryPrice=m_stpac.m_WAvgPrice  ;
				m_st_orika_positionEntryAccounting.m_currentPrice=m_stpac.m_WAvgPrice  ;
				m_st_orika_positionEntryAccounting.m_highPrice=0;
				m_st_orika_positionEntryAccounting.m_lowPrice=0;; 	
				m_st_orika_positionEntryAccounting.m_subBrokerEntryVolume=m_stpac.m_subBrokerVolume ;
				m_st_orika_positionEntryAccounting.m_brokerEntryVolume=m_stpac.m_brokerVolume ;
				m_st_orika_positionEntryAccounting.m_extraGroupEntryVolume=m_stpac.m_extraGroupVolume ; 
				m_st_orika_positionEntryAccounting.m_companyEntryVolume=m_stpac.m_companyVolume;
				m_st_orika_positionEntryAccounting.m_multiplyer =m_stpac.m_multiplyer;


				
				int			m_deal_in=0;
				int         m_order_in=0;
				CString		m_login_in=m_stpac.m_login ;
				CString		m_symbol_in=m_stpac.m_symbol;
				int			m_time_in =m_time ;
				int			m_entryAction_in=m_stpac.m_Action ;
				int			m_entryVolume_in=m_stpac.m_Volume ;
				double		m_entryPrice_in=m_stpac.m_WAvgPrice;
				double		m_currentPrice_in=m_currentPrice ;
				double		m_highPrice_in=0;
				double		m_lowPrice_in=0 ; 	
				double		m_subBrokerEntryVolume_in=m_stpac.m_subBrokerVolume ;
				double		m_brokerEntryVolume_in=m_stpac.m_brokerVolume ;
				double		m_extraGroupEntryVolume_in=m_stpac.m_extraGroupVolume ; 
				double		m_companyEntryVolume_in=m_stpac.m_companyVolume ;
				double		m_multiplyer_in=m_stpac.m_multiplyer ; 

				int in_volume=0;
				int out_vaolume=0;
				
				if (m_entryVolume_in>=m_entryVolume)
				{
					if (m_entryAction_in!=m_entryAction)
					{
					int m_remainingINvalume=m_entryVolume_in-m_entryVolume;										
					if (m_remainingINvalume!=0)
					{						
						int in_volume=m_entryVolume;
						int out_volume=m_entryVolume;
						m_entryVolume=m_entryVolume-m_entryVolume;
						//Calculate Balance Table						
						Updateorika_PositionAverageAccountingHashtableNewRecord(m_st_orika_positionEntryAccounting_out,0);


						m_st_orika_positionEntryAccounting.m_entryVolume=m_remainingINvalume;
						m_st_orika_positionEntryAccounting.m_subBrokerEntryVolume=m_remainingINvalume*m_subBrokerPLRatioRate/100;
						m_st_orika_positionEntryAccounting.m_brokerEntryVolume=m_remainingINvalume*m_brokerPLRatioRate/100;
						m_st_orika_positionEntryAccounting.m_extraGroupEntryVolume=m_remainingINvalume* m_extragroupPLRatioRate/100;
						m_st_orika_positionEntryAccounting.m_companyEntryVolume=m_remainingINvalume*m_companyPLRatioRate/100;


						//Updating Position Average accounting table
						
						/*st_orika_PositionAverageAccounting m_stpac_updated={};
						m_orika_PositionAverageAccountingHastable.SetAt(m_loginSymbolKey,m_stpac_updated);*/
						
						//Updateorika_PositionAverageAccountingHashtableNewRecord(m_st_orika_positionEntryAccounting,0);
																		

						//End of Updating Position Average Accounting
						
						///Updating Balance Table
						m_st_orika_positionEntryAccounting.m_entryVolume=m_entryVolume_dta;
						st_orika_positionEntryAccounting m_stout={};						
						m_stout.m_deal=m_deal_dta;
						m_stout.m_order=m_order_dta;
						CMTStr::Copy(m_stout.m_login,m_login_dta);
						CMTStr::Copy(m_stout.m_symbol,m_symbol_dta);
						m_stout.m_time=m_time_dta ;
						m_stout.m_entryAction=m_entryAction_dta;
						m_stout.m_entryVolume=m_entryVolume_dta;
						m_stout.m_entryPrice=m_entryPrice_dta;						
						m_stout.m_multiplyer=m_multiplyer_dta;
						UpdateOrika_BalanceTableAccountingHashtable(m_st_orika_positionEntryAccounting,m_stout);


						//update row in database
					}
					else
					{
						
						int in_volume=m_entryVolume;
						int out_volume=m_entryVolume;
						m_entryVolume=0;
						//Calculate Balance Table
						//m_orika_positionEntryAccountingarrayClientSymbolWise->Delete(r);
						
						st_orika_positionEntryAccounting tmpst={};						
						tmpst.m_deal =0;
						tmpst.m_order =0;
					    CMTStr::Copy(tmpst.m_login,m_stpac.m_login) ;
						CMTStr::Copy(tmpst.m_symbol,m_stpac.m_symbol);
						tmpst.m_time =m_time ;
						tmpst.m_entryAction=m_entryAction;
						tmpst.m_entryVolume=m_stpac.m_Volume ;
						tmpst.m_entryPrice=m_stpac.m_WAvgPrice;
						tmpst.m_currentPrice=m_currentPrice ;
						tmpst.m_highPrice=0;
						tmpst.m_lowPrice=0 ; 	
						tmpst.m_subBrokerEntryVolume=m_stpac.m_subBrokerVolume ;
						tmpst.m_brokerEntryVolume=m_stpac.m_brokerVolume ;
						tmpst.m_extraGroupEntryVolume=m_stpac.m_extraGroupVolume ; 
						tmpst.m_companyEntryVolume=m_stpac.m_companyVolume ;
						tmpst.m_multiplyer=m_stpac.m_multiplyer ; 

						
						Updateorika_PositionAverageAccountingHashtableNewRecord(tmpst,1);
						
												
						st_orika_positionEntryAccounting m_stout={};						
						m_stout.m_deal=m_deal_dta;
						m_stout.m_order=m_order_dta;
						CMTStr::Copy(m_stout.m_login,m_login_dta);
						CMTStr::Copy(m_stout.m_symbol,m_symbol_dta);
						m_stout.m_time=m_time_dta ;
						m_stout.m_entryAction=m_entryAction_dta;
						m_stout.m_entryVolume=m_entryVolume_dta;
						m_stout.m_entryPrice=m_entryPrice_dta;						
						m_stout.m_multiplyer=m_multiplyer_dta;
						UpdateOrika_BalanceTableAccountingHashtable(m_st_orika_positionEntryAccounting,m_stout);
					
						//Delete row from database
						
					}
				}
				else
				{
					Updateorika_PositionAverageAccountingHashtableNewRecord(m_st_orika_positionEntryAccounting_out,0);
				}
					
				}
				else
				{			
					if (m_entryAction_in!=m_entryAction)
					{
						int in_volume=m_entryVolume_in;
						int out_volume=m_entryVolume_in;
						m_entryVolume=m_entryVolume-in_volume;
						//Calculate Balance Table
										
						st_orika_positionEntryAccounting m_stout={};						
						m_stout.m_deal=m_deal_dta;
						m_stout.m_order=m_order_dta;
						CMTStr::Copy(m_stout.m_login,m_login_dta);
						CMTStr::Copy(m_stout.m_symbol,m_symbol_dta);
						m_stout.m_time=m_time_dta ;
						m_stout.m_entryAction=m_entryAction_dta;
						m_stout.m_entryVolume=in_volume;
						m_stout.m_entryPrice=m_entryPrice_dta;						
						m_stout.m_multiplyer=m_multiplyer_dta;

						UpdateOrika_BalanceTableAccountingHashtable(m_st_orika_positionEntryAccounting,m_stout);
					}	
						

						Updateorika_PositionAverageAccountingHashtableNewRecord(m_st_orika_positionEntryAccounting_out,0);
						
												
						//Delete row from database						
				}				
		}	
}


void CStaticClass::Sql_Deleteorika_positionEntryAccounting(st_orika_positionEntryAccounting dt)
{
	HRESULT hr=NULL ;	
	CCommand<CNoAccessor, CNoRowset> UpdateCommand;
	if(!SUCCEEDED(hr))
	{
		return ;
	}

	int   m_deal=dt.m_deal;
	CString m_login=dt.m_login;	
	CString   strCommand=L"";	
	strCommand.Format(L"delete from  orika_positionEntryAccounting  where deal=%d and login='%s'",m_deal,m_login);	
	
	//CStaticClass::m_mutex_order.Lock();
	//(L"L21");
	CStaticClass::m_SqlLock.Lock();
	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr=UpdateCommand.Open(m_tempSession,(LPCTSTR)strCommand);
	
	UpdateCommand.Close();
	m_tempSession.Close();
	CStaticClass::m_SqlLock.Unlock();
	//CStaticClass::m_mutex_order.Unlock();
	//(L"LU21");
}



void CStaticClass::Sql_Updateorika_positionEntryAccounting(st_orika_positionEntryAccounting dt)
{
	HRESULT hr=NULL ;	
	CCommand<CNoAccessor, CNoRowset> UpdateCommand;
	if(!SUCCEEDED(hr))
	{
		return ;
	}
	int   m_deal=dt.m_deal;
	CString m_login=dt.m_login;
	CString m_symbol=dt.m_symbol;
	int   m_time=dt.m_time ;
	int   m_entryAction=dt.m_entryAction;
	int   m_entryVolume=dt.m_entryVolume;
	double m_entryPrice=dt.m_entryPrice;
	double m_currentPrice=dt.m_currentPrice;
	double m_highPrice=dt.m_highPrice;
	double m_lowPrice=dt.m_lowPrice; 	
	double m_subBrokerEntryVolume=dt.m_subBrokerEntryVolume;
	double m_brokerEntryVolume=dt.m_brokerEntryVolume;
	double m_extraGroupEntryVolume=dt.m_extraGroupEntryVolume; 
	double m_companyEntryVolume=dt.m_companyEntryVolume;

	CString   strCommand=L"";	
	strCommand.Format(L"Update orika_positionEntryAccounting set  entryVolume='%d',subBrokerEntryVolume='%.5lf',brokerEntryVolume='%.5lf',extraGroupEntryVolume='%.5lf',companyEntryVolume='%.5lf' where deal='%d' and login='%s'",m_entryVolume,m_subBrokerEntryVolume,m_brokerEntryVolume,m_extraGroupEntryVolume,m_companyEntryVolume,m_deal,m_login);		
	
	//CStaticClass::m_mutex_order.Lock();
	//(L"L22");
	CStaticClass::m_SqlLock.Lock();
	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr=UpdateCommand.Open(m_tempSession,(LPCTSTR)strCommand);
	
	UpdateCommand.Close();
	m_tempSession.Close();
	CStaticClass::m_SqlLock.Unlock();
	//CStaticClass::m_mutex_order.Unlock();
	//(L"UL22");
}


void CStaticClass::UpdateOrika_BalanceTableAccountingHashtable(st_orika_positionEntryAccounting inDeal,st_orika_positionEntryAccounting outDeal)
{
	CString m_login=inDeal.m_login ;
	CString m_symbol=inDeal.m_symbol;
	int   m_entryDeal=inDeal.m_deal ;
	int   m_entryTime=inDeal.m_time ;
	int   m_entryOrder=inDeal.m_order;	
	int   m_entryAction=inDeal.m_entryAction  ;
	int   m_entryVolume=inDeal.m_entryVolume  ;
	double m_entryPrice=inDeal.m_entryPrice  ;
	int   m_exitDeal=outDeal.m_deal ;
	int   m_exitTime=outDeal.m_time ;
	int   m_exitOrder=outDeal.m_order ;
	int   m_exitAction=outDeal.m_entryAction  ;
	int   m_exitVolume=outDeal.m_entryVolume  ;
	double m_exitPrice=outDeal.m_entryPrice  ;

	double m_plPoint=0;
	if (m_entryAction==0)
	{
		m_plPoint=m_exitPrice-m_entryPrice;
	}
	else
	{
		m_plPoint=m_entryPrice-m_exitPrice;
	}
	double m_plPointLoss=0;
	if (m_plPoint<0)
	{
		m_plPointLoss=-m_plPoint;
	}
	double m_plPointProfit=0;
	if (m_plPoint>0)
	{
		m_plPointProfit=m_plPoint;
	}
	double m_multiplyer=inDeal.m_multiplyer;
	double m_clientBalance=m_plPoint*((double)m_entryVolume/10000)*m_multiplyer;


	CString m_symbolGroup=L"";
	CStaticClass::st_Orika_symbolmaster m_st_Orika_symbolmaster={};
	CStaticClass::m_Orika_symbolmasterHastable.Lookup(m_symbol,m_st_Orika_symbolmaster);
	m_symbolGroup=m_st_Orika_symbolmaster.m_symbolGroup;
	if (m_symbol.Find(L".P.") > 0 || m_symbol.Find(L".C.") > 0)
	{
		m_symbolGroup = L"option";
	}

	CString m_loginSymbolgroupKey=L"";
	m_loginSymbolgroupKey.Format(L"%s:%s",m_login,m_symbolGroup);

	CString m_loginKey=L"";
	m_loginKey.Format(L"%s:",m_login);


	CStaticClass::st_Orika_PLDevideRatio m_st_Orika_PLDevideRatio={};
	CStaticClass::m_Orika_PLDevideRatioHastable.Lookup(m_loginSymbolgroupKey,m_st_Orika_PLDevideRatio);
	if (m_st_Orika_PLDevideRatio.m_brokerPLRatio==0)
	{
		CStaticClass::m_Orika_PLDevideRatioHastable.Lookup(m_loginKey,m_st_Orika_PLDevideRatio);
	}


	double m_brokerPLRatioRate=m_st_Orika_PLDevideRatio.m_brokerPLRatio ;
	double m_subBrokerPLRatioRate=m_st_Orika_PLDevideRatio.m_subBrokerPLRatio ;
	double m_extragroupPLRatioRate=m_st_Orika_PLDevideRatio.m_extragroupPLRatio ;
	double m_companyPLRatioRate=m_st_Orika_PLDevideRatio.m_companyPLRatio ;


	double m_subBrokerBalance=m_clientBalance*m_subBrokerPLRatioRate/100;
	double m_brokerBalance=m_clientBalance*m_brokerPLRatioRate/100;
	double m_extraGroupBalance=m_clientBalance*m_extragroupPLRatioRate/100; 
	double m_companyBalance=m_clientBalance*m_companyPLRatioRate/100;




	st_Orika_BalanceTableAccounting m_stTemp={};
	 
	CMTStr::Copy(m_stTemp.m_login,m_login);
	CMTStr::Copy(m_stTemp.m_symbol,m_symbol);
	m_stTemp.m_entryDeal=m_entryDeal;
	m_stTemp.m_entryTime=m_entryTime;
	m_stTemp.m_entryOrder=m_entryOrder;	
	m_stTemp.m_entryAction=m_entryAction;
	m_stTemp.m_entryVolume=m_entryVolume;
	m_stTemp.m_entryPrice=m_entryPrice;
	m_stTemp.m_exitDeal=m_exitDeal;
	m_stTemp.m_exitTime=m_exitTime;
	m_stTemp.m_exitOrder=m_exitOrder;
	m_stTemp.m_exitAction=m_exitAction;
	m_stTemp.m_exitVolume=m_exitVolume;
	m_stTemp.m_exitPrice=m_exitPrice;
	m_stTemp.m_plPointLoss=m_plPointLoss;
	m_stTemp.m_plPointProfit=m_plPointProfit;
	m_stTemp.m_multiplyer=m_multiplyer;
	m_stTemp.m_clientBalance=m_clientBalance;
	m_stTemp.m_subBrokerBalance=m_subBrokerBalance;
	m_stTemp.m_brokerBalance=m_brokerBalance;
	m_stTemp.m_extraGroupBalance=m_extraGroupBalance; 
	m_stTemp.m_companyBalance=m_companyBalance;
	Sql_UpdateOrika_BalanceTableAccounting(m_stTemp);
}



void CStaticClass::Updateorika_PositionAverageAccountingHashtableNewRecord(st_orika_positionEntryAccounting m_pe,int flag)
{		
	//flag =0  Add New Record
	//flag =1  Delete New Record
	
	CString  m_login_new=m_pe.m_login;
	CString  m_symbol_new=m_pe.m_symbol;	
	int      m_Action_new=m_pe.m_entryAction;
	int      m_Volume_new=m_pe.m_entryVolume;
	double    m_Price_new=m_pe.m_entryPrice;		

	CString m_symbolGroup=L"";
	CStaticClass::st_Orika_symbolmaster m_st_Orika_symbolmaster={};
	CStaticClass::m_Orika_symbolmasterHastable.Lookup(m_symbol_new,m_st_Orika_symbolmaster);
	m_symbolGroup=m_st_Orika_symbolmaster.m_symbolGroup;

	if (m_symbol_new.Find(L".P.") > 0 || m_symbol_new.Find(L".C.") > 0)
	{
		m_symbolGroup = L"option";
	}


	CString m_loginSymbolgroupKey=L"";
	m_loginSymbolgroupKey.Format(L"%s:%s",m_login_new,m_symbolGroup);

	CString m_loginKey=L"";
	m_loginKey.Format(L"%s:",m_login_new);


	CStaticClass::st_Orika_PLDevideRatio m_st_Orika_PLDevideRatio={};
	CStaticClass::m_Orika_PLDevideRatioHastable.Lookup(m_loginSymbolgroupKey,m_st_Orika_PLDevideRatio);
	if (m_st_Orika_PLDevideRatio.m_brokerPLRatio==0)
	{
		CStaticClass::m_Orika_PLDevideRatioHastable.Lookup(m_loginKey,m_st_Orika_PLDevideRatio);
	}


	double m_brokerPLRatioRate=m_st_Orika_PLDevideRatio.m_brokerPLRatio ;
	double m_subBrokerPLRatioRate=m_st_Orika_PLDevideRatio.m_subBrokerPLRatio ;
	double m_extragroupPLRatioRate=m_st_Orika_PLDevideRatio.m_extragroupPLRatio ;
	double m_companyPLRatioRate=m_st_Orika_PLDevideRatio.m_companyPLRatio ;





	CString m_loginSymbolKey=L"";
	m_loginSymbolKey.Format(L"%s:%s",m_login_new,m_symbol_new);


	st_orika_PositionAverageAccounting m_stpac={};
	m_orika_PositionAverageAccountingHastable.Lookup(m_loginSymbolKey,m_stpac);


	CMTStr::Copy(m_stpac.m_login,m_login_new);
	CMTStr::Copy(m_stpac.m_symbol,m_symbol_new);

	int      m_Action_old=m_stpac.m_Action ;
	int      m_Volume_old=m_stpac.m_Volume ;
	double    m_WAvgPrice_old=m_stpac.m_WAvgPrice ;		




	int      m_Action_calculated=0 ;
	int      m_Volume_calculated=0;
	double     m_WAvgPrice_calculated=0;		

	

	//when add new data in position 
	if (m_Volume_old!=0)
	{
		if (flag==0 )
		{		
			if (m_Action_new==m_Action_old )
			{
				m_Volume_calculated=m_Volume_old+m_Volume_new;
				m_WAvgPrice_calculated=((m_Volume_old*m_WAvgPrice_old)+(m_Volume_new*m_Price_new))/(m_Volume_calculated);		
				m_Action_calculated=m_Action_new;
			}
			else
			{
				if (m_Volume_new<m_Volume_old)
				{
					m_Volume_calculated=m_Volume_old-m_Volume_new;
					m_WAvgPrice_calculated=m_WAvgPrice_old;		
					m_Action_calculated=m_Action_old;
				}
				else
				{
					m_Volume_calculated=m_Volume_new-m_Volume_old;
					m_WAvgPrice_calculated=m_Price_new;		
					m_Action_calculated=m_Action_new;
				}
			}
		}
		//when Delete data from position 
		if (flag==1)
		{
			m_Volume_calculated=0;
			if (m_Volume_calculated!=0)
			{
				m_WAvgPrice_calculated=0;		
			}
			m_Action_calculated=m_Action_new;
		}
	}
	else
	{
		m_Volume_calculated=m_Volume_new-m_Volume_old;
		m_WAvgPrice_calculated=m_Price_new;		
		m_Action_calculated=m_Action_new;
	}
	m_stpac.m_Action=m_Action_calculated;
	m_stpac.m_Volume=m_Volume_calculated; 
	m_stpac.m_subBrokerVolume=m_Volume_calculated*m_subBrokerPLRatioRate/100;
	m_stpac.m_brokerVolume=m_Volume_calculated*m_brokerPLRatioRate/100;
	m_stpac.m_extraGroupVolume=m_Volume_calculated*m_extragroupPLRatioRate/100;
	m_stpac.m_companyVolume=m_Volume_calculated*m_companyPLRatioRate/100;
	m_stpac.m_WAvgPrice=m_WAvgPrice_calculated;	
	m_stpac.m_multiplyer=m_pe.m_multiplyer;

	
	m_orika_PositionAverageAccountingHastable.SetAt(m_loginSymbolKey,m_stpac);	
	Sql_Updateorika_PositionAverageAccounting(m_stpac);
}



void CStaticClass::Updateorika_PositionAverageAccountingHashtable(st_orika_PositionAverageAccounting m_pac)
{		
	CString  m_login_new=m_pac.m_login;
	CString  m_symbol_new=m_pac.m_symbol;		
	CString m_loginSymbolKey=L"";
	m_loginSymbolKey.Format(L"%s:%s",m_login_new,m_symbol_new);
	CString m_loginKey=L"";
	m_loginKey.Format(L"%s:",m_login_new);	
	m_orika_PositionAverageAccountingHastable.SetAt(m_loginSymbolKey,m_pac);	
	Sql_Updateorika_PositionAverageAccounting(m_pac);
}

void CStaticClass::sendNeworderToClient(st_order st)
{
		CString strFinalJson=L"";
		CString m_login=st.m_login;
		int m_time =st.m_time;
		m_time = m_time - 19800;
		CString m_strTime=L"";		
		CString m_tmp_date=L"";
		CMTStr256 str_time;
		SMTFormat::FormatDateTime(str_time,m_time,true,true);
		m_strTime=str_time.Str();

		int m_deal=st.m_deal ;
		int m_order=st.m_order;
		CString m_symbol=st.m_symbol;
		int m_type=st.m_type ;
		CString strtype=L"";
		if(m_type==2)
		{
			strtype=L"Buy Limit";
		}
		else if(m_type==3)
		{
			strtype=L"Sell Limit";
		}
		else if(m_type==4)
		{
			strtype=L"Buy Stop";
		}
		else if(m_type==5)
		{
			strtype=L"Sell Stop";
		}
		else if(m_type==6)
		{
			strtype=L"Buy Stop";
		}
		else if(m_type==7)
		{
			strtype=L"Sell Stop Limit";
		}
		double m_volume =st.m_volume ;
		double m_price=st.m_price ;
		CString m_comment=st.m_comment;
		CString m_status=st.m_status;
		int m_select=st.m_select ;
		CString strselect=L"";
		if (m_select=0) 
		{
			strselect="true";
		}
		else
		{
			strselect="false";
		}

		CString m_selecttype=st.m_selecttype;
		CString m_subtype=st.m_subtype;
		int m_contraorder=st.m_contraorder ;
		int m_tradeexecutetime=st.m_tradeexecutetime ;
		CString m_ourcomment=st.m_ourcomment;
		int m_orderstate=st.m_orderstate ;

		CString str_orderstate=L"";
		if (m_orderstate==1001)
		{
			str_orderstate=L"NEW";
		}
		if (m_orderstate==1002)
		{
			str_orderstate=L"UPDATE";
		}
		if (m_orderstate==1003 ||m_orderstate==1005)
		{
			str_orderstate=L"DELETE";
		}


		CString strtmpdata=L"";
		
		strtmpdata.Format(L"{\"login\":\"%s\",\"time\":%d,\"deal\":%d,\"order\":%d,\"symbol\":\"%s\",\"type\":\"%s\",\"volume\":%.2lf,\"price\":%.4lf,\"comment\":\"%s\",\"status\":\"%s\",\"select\":\"%s\",\"statustype\":\"%s\",\"subtype\":\"%s\",\"contraorder\":%d,\"tradeexecutetime\":%d,\"ourcomment\":\"%s\",\"orderstate\":\"%s\"}", m_login, m_time,      m_deal      ,m_order      ,m_symbol,       strtype,        m_volume,m_price,m_comment,m_status,strselect,m_selecttype,m_subtype,m_contraorder,m_tradeexecutetime,m_ourcomment,str_orderstate);


		strFinalJson.Format(L"{\"type\": \"ORDER_DATA\",\"data\": [%s]}",strtmpdata);	


		CString StrPrintLino=L"";	
		
		CStaticClass::m_mutex_ClientList.Lock();		
		//(L"11");
		//StrPrintLino.Format(L"m_mutex_ClientList Locked(%d)",__LINE__);
		//////(StrPrintLino);


		POSITION pos = CStaticClass::m_ClientList_forOrder.GetStartPosition ();		
		while (pos != NULL) 
		{
			CString strclientkey=L"";		
			SSL_session* client=NULL;
			CStaticClass::m_ClientList_forOrder.GetNextAssoc(pos, strclientkey,client);
			///Sending order data To Client						
			string strforsend=CT2A(strFinalJson.GetString());
			
			CStaticClass::st_ClientContext m_st = {};
			CStaticClass::m_ClientContext.GetNextAssoc(pos, strclientkey, m_st);

			SendDataToClient(client, strforsend, strclientkey, m_st.m_activeClient);
			///End of Sending order data to client
		}
		CStaticClass::m_mutex_ClientList.Unlock();
		//(L"U11");
		//StrPrintLino.Format(L"m_mutex_ClientList UN Locked(%d)",__LINE__);
		//////(StrPrintLino);
}

CString  CStaticClass::CalculateLPData(CString m_login, CString m_symbolGroup, double m_LpVolume, double m_CompanyVolume)
{	
	CStaticClass::m_positiondevideRatio_lock.Lock();
	CStaticClass::m_LPLots.SetAt(m_symbolGroup, m_LpVolume);
	double previous_perfect = 0;
	CStaticClass::m_LPLots_perfect.Lookup(m_symbolGroup, previous_perfect);
	double volumediff = abs(m_LpVolume - previous_perfect);
	if (m_symbolGroup==L"GOLD")
	{								
		CStaticClass::m_LpLotsMap.Lookup(m_symbolGroup, CStaticClass::m_LpQtyArray_gold);
		int totalDataCount = CStaticClass::m_LpQtyArray_gold.Total();
		double Perfect_Total_Lots = 0;
		double Rounded_Total_Lots = 0;
		double pre_Perfect_Total_Lots = 0;
		double pre_Rounded_Total_Lots = 0;		
		for (int di = 0; di < totalDataCount; di++)
		{
			CStaticClass::st_LpLots   m_st_LpLots = {};
			m_st_LpLots = CStaticClass::m_LpQtyArray_gold[di];
						
			m_st_LpLots.m_perfect = 0;
			m_st_LpLots.m_rounded = 0;
			m_st_LpLots.m_companyVolumeAfterMultiTotal = m_CompanyVolume;
			CStaticClass::m_LpQtyArray_gold.Update(di, &m_st_LpLots);			
		}
		for (int di = 0; di < totalDataCount; di++)
		{
			CStaticClass::st_LpLots   m_st_LpLots = {};
			m_st_LpLots = CStaticClass::m_LpQtyArray_gold[di];						
			double dblstepup = 1;
			if (m_st_LpLots.m_stepup != 0)
			{
				dblstepup = m_st_LpLots.m_stepup;
			}
			if (m_LpVolume > 0)
			{
				if (m_st_LpLots.m_lots_To > 0 && m_st_LpLots.m_lots>=0)
				{
					if (m_LpVolume >= m_st_LpLots.m_lots_To)
					{
						double m_calculatedPrefect = ((m_st_LpLots.m_lots_To- m_st_LpLots.m_lots) * m_st_LpLots.m_LpPer / 100);
						m_st_LpLots.m_perfect = m_calculatedPrefect;
												
						double calculatedRounded = 0;
						/*double doubleDevideRatio = m_calculatedPrefect / dblstepup;						
						calculatedRounded = trunc(doubleDevideRatio)* dblstepup;*/

						calculatedRounded = m_calculatedPrefect;
						CStaticClass::m_LPLots_perfect.SetAt(m_symbolGroup, calculatedRounded);
						m_st_LpLots.m_rounded = calculatedRounded;
						
					}
					else
					{
						if (m_LpVolume > m_st_LpLots.m_lots)
						{
							double m_calculatedPrefect = ((m_LpVolume - m_st_LpLots.m_lots) * m_st_LpLots.m_LpPer / 100);
							m_st_LpLots.m_perfect = m_calculatedPrefect;
							

							double doubleDevideRatio = (m_LpVolume - m_st_LpLots.m_lots) / dblstepup;
							double calculatedRounded = trunc(doubleDevideRatio) * dblstepup;
							calculatedRounded = calculatedRounded * m_st_LpLots.m_LpPer / 100;
							CStaticClass::m_LPLots_perfect.SetAt(m_symbolGroup, calculatedRounded);
							m_st_LpLots.m_rounded = calculatedRounded;
						}
					}
				}
			}
			else
			{
				if (m_st_LpLots.m_lots_To < 0 && m_st_LpLots.m_lots <= 0)
				{
					if (m_LpVolume <= m_st_LpLots.m_lots_To)
					{
						double m_calculatedPrefect = ((m_st_LpLots.m_lots_To- m_st_LpLots.m_lots) * m_st_LpLots.m_LpPer / 100);
						m_st_LpLots.m_perfect = m_calculatedPrefect;

						double calculatedRounded = 0;
						/*double doubleDevideRatio = m_calculatedPrefect / dblstepup;
						calculatedRounded = trunc(doubleDevideRatio)* dblstepup;*/
						calculatedRounded = m_calculatedPrefect;

						CStaticClass::m_LPLots_perfect.SetAt(m_symbolGroup, calculatedRounded);
						m_st_LpLots.m_rounded = calculatedRounded;
					}
					else
					{
						if (m_LpVolume < m_st_LpLots.m_lots)
						{
							double m_calculatedPrefect = ((m_LpVolume -  m_st_LpLots.m_lots) * m_st_LpLots.m_LpPer / 100);
							m_st_LpLots.m_perfect = m_calculatedPrefect;

							double doubleDevideRatio = (m_LpVolume - m_st_LpLots.m_lots) / dblstepup;
							double calculatedRounded = trunc(doubleDevideRatio) * dblstepup;
							calculatedRounded = calculatedRounded * m_st_LpLots.m_LpPer / 100;
							CStaticClass::m_LPLots_perfect.SetAt(m_symbolGroup, calculatedRounded);
							m_st_LpLots.m_rounded = calculatedRounded;
						}
					}
				}
			}
			CStaticClass::m_LpQtyArray_gold.Update(di, &m_st_LpLots);
		}								
		CStaticClass::m_LpLotsMap.SetAt(m_symbolGroup, CStaticClass::m_LpQtyArray_gold);
	}


	if (m_symbolGroup == L"SILVER")
	{
		CStaticClass::m_LpLotsMap.Lookup(m_symbolGroup, CStaticClass::m_LpQtyArray_silver);
		int totalDataCount = CStaticClass::m_LpQtyArray_silver.Total();
		double Perfect_Total_Lots = 0;
		double Rounded_Total_Lots = 0;
		double pre_Perfect_Total_Lots = 0;
		double pre_Rounded_Total_Lots = 0;
		for (int di = 0; di < totalDataCount; di++)
		{
			CStaticClass::st_LpLots   m_st_LpLots = {};
			m_st_LpLots = CStaticClass::m_LpQtyArray_silver[di];

			m_st_LpLots.m_perfect = 0;
			m_st_LpLots.m_rounded = 0;
			m_st_LpLots.m_companyVolumeAfterMultiTotal = m_CompanyVolume;
			CStaticClass::m_LpQtyArray_silver.Update(di, &m_st_LpLots);
		}

		for (int di = 0; di < totalDataCount; di++)
		{
			CStaticClass::st_LpLots   m_st_LpLots = {};
			m_st_LpLots = CStaticClass::m_LpQtyArray_silver[di];
			double dblstepup = 1;
			if (m_st_LpLots.m_stepup != 0)
			{
				dblstepup = m_st_LpLots.m_stepup;
			}
			if (m_LpVolume > 0)
			{
				if (m_st_LpLots.m_lots_To > 0 && m_st_LpLots.m_lots >= 0)
				{
					if (m_LpVolume >= m_st_LpLots.m_lots_To)
					{
						double m_calculatedPrefect = ((m_st_LpLots.m_lots_To - m_st_LpLots.m_lots) * m_st_LpLots.m_LpPer / 100);
						m_st_LpLots.m_perfect = m_calculatedPrefect;
						
						double calculatedRounded = 0;
						/*double doubleDevideRatio = m_calculatedPrefect / dblstepup;
						calculatedRounded = trunc(doubleDevideRatio)* dblstepup;*/

						calculatedRounded = m_calculatedPrefect;
						CStaticClass::m_LPLots_perfect.SetAt(m_symbolGroup, calculatedRounded);
						m_st_LpLots.m_rounded = calculatedRounded;
					}
					else
					{
						if (m_LpVolume > m_st_LpLots.m_lots)
						{
							double m_calculatedPrefect = ((m_LpVolume - m_st_LpLots.m_lots) * m_st_LpLots.m_LpPer / 100);
							m_st_LpLots.m_perfect = m_calculatedPrefect;
							
							double doubleDevideRatio = (m_LpVolume - m_st_LpLots.m_lots) / dblstepup;
							double calculatedRounded = trunc(doubleDevideRatio) * dblstepup;
							calculatedRounded = calculatedRounded * m_st_LpLots.m_LpPer / 100;

							CStaticClass::m_LPLots_perfect.SetAt(m_symbolGroup, calculatedRounded);
							m_st_LpLots.m_rounded = calculatedRounded;
						}
					}
				}
			}
			else
			{
				if (m_st_LpLots.m_lots_To < 0 && m_st_LpLots.m_lots <= 0)
				{
					if (m_LpVolume <= m_st_LpLots.m_lots_To)
					{
						double m_calculatedPrefect = ((m_st_LpLots.m_lots_To - m_st_LpLots.m_lots) * m_st_LpLots.m_LpPer / 100);
						m_st_LpLots.m_perfect = m_calculatedPrefect;


						double calculatedRounded = 0;
						/*double doubleDevideRatio = m_calculatedPrefect / dblstepup;
						calculatedRounded = trunc(doubleDevideRatio)* dblstepup;*/

						calculatedRounded = m_calculatedPrefect;
						CStaticClass::m_LPLots_perfect.SetAt(m_symbolGroup, calculatedRounded);
						m_st_LpLots.m_rounded = calculatedRounded;
					}
					else
					{
						if (m_LpVolume < m_st_LpLots.m_lots)
						{
							double m_calculatedPrefect = ((m_LpVolume - m_st_LpLots.m_lots) * m_st_LpLots.m_LpPer / 100);
							m_st_LpLots.m_perfect = m_calculatedPrefect;
							
							double doubleDevideRatio = (m_LpVolume - m_st_LpLots.m_lots) / dblstepup;
							double calculatedRounded = trunc(doubleDevideRatio) * dblstepup;
							calculatedRounded = calculatedRounded * m_st_LpLots.m_LpPer / 100;

							CStaticClass::m_LPLots_perfect.SetAt(m_symbolGroup, calculatedRounded);
							m_st_LpLots.m_rounded = calculatedRounded;
						}
					}
				}
			}
			CStaticClass::m_LpQtyArray_silver.Update(di, &m_st_LpLots);
		}

		CStaticClass::m_LpLotsMap.SetAt(m_symbolGroup, CStaticClass::m_LpQtyArray_silver);
	}
	CStaticClass::m_positiondevideRatio_lock.Unlock();
	CString strData = L"";
	strData = CStaticClass::m_mtmanager.GetGATEWAY_PERFECTANDROUNDED();
	//strData = CStaticClass::m_mtmanager.GetGATEWAY_POSITIONDEVIDERATIO_UPDATE();
	return strData;
}









void CStaticClass::addNewDealToDealData(st_Orika_dealtableAccounting st)
{
	CString strlogin,tickSymbol;
	strlogin=st.m_login;
	tickSymbol=st.m_symbol;
	CString loginAndSymbolKey=L"";
	loginAndSymbolKey.Format(L"%s:%s",strlogin,tickSymbol);
	
	int TradeTime = st.m_time;
	SYSTEMTIME readable_date;
	readable_date = SMTTime::TimeToST(TradeTime, readable_date);
	int int_dayofweek = readable_date.wDayOfWeek;
	int time_hour = readable_date.wHour;


	CStaticClass::st_netpositionClientWise  st_pac={};
	////(L"m_mutex_Tick_2 Locked");
	CStaticClass::m_mutex_Tick.Lock();
	

		CStaticClass::mapNetPositionClientWise.Lookup(loginAndSymbolKey,st_pac);
		CString strCheckSymbol=st_pac.m_symbol;

		if (strCheckSymbol!=L"")
		{		
			double m_oldVolume=*st_pac.m_volume; 
			double m_oldAverage=*st_pac.m_average;
			double m_newVolume=0; 
			double m_NewTardePrice =st.m_price ;
			if (st.m_action==1)
			{
				m_newVolume=-st.m_volume;
				
			}
			else
			{
				m_newVolume=st.m_volume;				
			}
			m_newVolume=m_newVolume/10000;
			double m_NewCalculatedvolume=m_oldVolume+m_newVolume; 
			*st_pac.m_volume=m_NewCalculatedvolume;
			//Average Calculation



			CStaticClass::m_mutex_TotalBrokerage.Lock();
			//Calculating Total TO And TradedLOT
			CStaticClass::st_TotalTradedLotAndTOT m_stTO = {};
			CStaticClass::m_TotalLotAndTOT.Lookup(loginAndSymbolKey, m_stTO);
			m_stTO.m_TotalTradedLot = m_stTO.m_TotalTradedLot+abs(m_newVolume);
			m_stTO.m_TotalTradedTO= m_stTO.m_TotalTradedTO+(abs(m_newVolume)* st.m_multiplyer*st.m_price);
			if (st.m_action == 1)
			{
				m_stTO.m_TotalBuyLot= m_stTO.m_TotalBuyLot+ abs(m_newVolume);
				m_stTO.m_TotalButTO= m_stTO.m_TotalButTO+ (abs(m_newVolume) * st.m_multiplyer * st.m_price);
			}
			else
			{
				m_stTO.m_TotalSellLot= m_stTO.m_TotalSellLot +abs(m_newVolume);
				m_stTO.m_TotalSellTO= m_stTO.m_TotalSellTO+ (abs(m_newVolume) * st.m_multiplyer * st.m_price);
			}
			CStaticClass::m_TotalLotAndTOT.SetAt(loginAndSymbolKey, m_stTO);
			//End OF Calculating Total TO And TradedLOT
			CStaticClass::m_mutex_TotalBrokerage.Unlock();



			if (m_NewCalculatedvolume!=0)
			{
				if (m_oldVolume==0)
				{
					*st_pac.m_average=st.m_price; 
				}
				if ((m_oldVolume>0 && m_NewCalculatedvolume<0)||(m_oldVolume<0 && m_NewCalculatedvolume>0))
				{
					*st_pac.m_average=st.m_price; 
				}
				else
				{
					if (( m_oldVolume>0 && m_newVolume>0)||( m_oldVolume<0 && m_newVolume<0))
					{	
						double oldAvgRate=*st_pac.m_average;
						double CalculatedAvg=abs(((st.m_price*m_newVolume)+ (m_oldVolume*oldAvgRate))/m_NewCalculatedvolume);
						*st_pac.m_average=CalculatedAvg;
					}					
				}

			}
			else
			{
				*st_pac.m_average=0;
			}
			//End of Average Calculation
			//Calculating Client Balance
			double plPoint=0;
			if ( m_oldVolume>0 && m_newVolume<0)
			{
				plPoint=st.m_price-m_oldAverage;
			}
			if ( m_oldVolume<0 && m_newVolume>0)
			{
				plPoint=m_oldAverage-st.m_price;
			}
				
			double m_vaolume_Squreof=0;
			if (abs(m_oldVolume)<abs(m_newVolume))
			{
				m_vaolume_Squreof=m_oldVolume;
			}
			else
			{
				m_vaolume_Squreof=m_newVolume;
			}
			m_vaolume_Squreof=abs(m_vaolume_Squreof);
			double m_multiplyer=*st_pac.m_multi;
			double m_NewBalance=plPoint*m_vaolume_Squreof*m_multiplyer;
			double m_oldClientBalance=*st_pac.m_clientbalance;
			double m_CalculatedBalance=m_oldClientBalance+m_NewBalance;
			*st_pac.m_clientbalance=m_CalculatedBalance;			
		//End Calculating Client Balance

		//Calculating Client Brokerage
			double m_brokerageType=*st_pac.m_brokerageType;
			double m_brokeragerate=*st_pac.m_clientBrokRate;
			double m_oldBrokerage=*st_pac.m_clientBrokarage;
			double m_newbrolerage=0;
			double m_CalculatedBrokerage=0;
			
			
			m_newbrolerage=st.m_clientBrokTotal;
			m_CalculatedBrokerage=m_oldBrokerage+m_newbrolerage;

			double m_clientBrokerageDifferance=m_CalculatedBrokerage;
			*st_pac.m_clientBrokarage=m_CalculatedBrokerage;			

			*st_pac.m_subbrokerBrokarage = *st_pac.m_subbrokerBrokarage+st.m_subBrokerBrokTotal;
			*st_pac.m_brokerBrokarage = *st_pac.m_brokerBrokarage+st.m_brokerBrokTotal;
			*st_pac.m_companyBrokarage = *st_pac.m_companyBrokarage+st.m_comBalancebrokTotal;




			

			//double m_clientfloatingpl=*st_pac.m_clientfloatingpl;
			//Calculating Floating PL
			
					double previous_floatingPL=*st_pac.m_clientfloatingpl ;
					double Plpoint=0;
					double floatingPL=0;
					if (*st_pac.m_volume<0)
					{
						Plpoint=*st_pac.m_average-(st_pac.m_lastrate->m_ask); 
					}
					else
					{
						Plpoint=st_pac.m_lastrate->m_bid-(*st_pac.m_average); 
					}
					floatingPL=Plpoint*abs((*st_pac.m_volume)) *(*st_pac.m_multi) ;

					*st_pac.m_clientfloatingpl=floatingPL;

					double DiffFloatingPL=floatingPL-previous_floatingPL;
					double previous_Balance=*st_pac.m_clientbalance;
					double Balance=*st_pac.m_clientbalance;

					*st_pac.m_clientbalance=Balance;

					double clientnetamount=Balance+floatingPL;

					*st_pac.m_clientnetamount=clientnetamount- *st_pac.m_clientBrokarage;

					//Getting BrockTotal array index
					int intbroktotalIndex=-1;
					clientbroktotalArrayIndex.Lookup(strlogin,intbroktotalIndex);										 

					*clientbroktotalArray[intbroktotalIndex]=*clientbroktotalArray[intbroktotalIndex]+m_newbrolerage;		

					double clientgrosstotal=*st_pac.m_clientgrosstotal+DiffFloatingPL+m_NewBalance;				
					double clientnettotal=clientgrosstotal+*st_pac.m_clientbroktotal;


					double grossExposure= *st_pac.m_average *(*st_pac.m_volume) *(*st_pac.m_multi);
					*st_pac.m_clientexposure=grossExposure;

					int ClientGrossTotalIndex=-1;				
					clientgrosstotalArrayIndex.Lookup(strlogin,ClientGrossTotalIndex);
					//End 
					*clientgrosstotalArray[ClientGrossTotalIndex]=clientgrosstotal;		
					


					//Getting ClientGrossTotal address
					int clientnettotalIndex=-1;				
					clientnettotalArrayIndex.Lookup(strlogin,clientnettotalIndex);
					//End 
					*clientnettotalArray[clientnettotalIndex]=clientnettotal;

				
				//End of Calculating Floating PL				
			
			
			//End Of Calculating Brokerage
			double m_clientnetamount=(*st_pac.m_clientbalance+floatingPL);
			double m_netamountDifferance=m_clientnetamount-*st_pac.m_clientnetamount;

			*st_pac.m_clientnetamount=m_clientnetamount- *st_pac.m_clientBrokarage;
			double m_ClientGrossAmount=(*st_pac.m_clientbalance+floatingPL);
			
			/*double m_clientGrossAmountDiff=m_ClientGrossAmount-(*st_pac.m_ClientGrossAmount);
			*st_pac.m_clientgrosstotal=*st_pac.m_clientgrosstotal+ m_clientGrossAmountDiff;*/
			
			*st_pac.m_ClientGrossAmount=*st_pac.m_clientbalance+*st_pac.m_clientfloatingpl;


			
			



			double m_subbrokerRatio=*st_pac.m_subbrokerRatio;
			double m_brokerRatio=*st_pac.m_brokerRatio ;
			double m_companyRatio=*st_pac.m_companyRatio  ;
			if (m_companyRatio == 0)
			{
				m_companyRatio = 100;
			}
			double m_SubBrokerGrossAmount=m_ClientGrossAmount*m_subbrokerRatio/100;
			double m_BrokerGrossAmount=m_ClientGrossAmount*m_brokerRatio/100;
			double m_CompanyGrossAmount= m_ClientGrossAmount*m_companyRatio/100; 

			*st_pac.m_SubBrokerGrossAmount=m_SubBrokerGrossAmount;
			*st_pac.m_BrokerGrossAmount=m_BrokerGrossAmount;
			*st_pac.m_CompanyGrossAmount= m_CompanyGrossAmount; 

			double m_clientvolume = *st_pac.m_volume;

			double m_SubBrokerVolume = ((m_clientvolume) * m_subbrokerRatio) / 100;
			double m_BrokerVolume = ((m_clientvolume) * m_brokerRatio) / 100;
			double m_CompanyVolume = ((m_clientvolume) * m_companyRatio) / 100;

			*st_pac.m_subbrokervolume = m_SubBrokerVolume;
			*st_pac.m_brokervolume = m_BrokerVolume;
			*st_pac.m_companyvolume = m_CompanyVolume;

			CString strLogFile = L"";

			//strLogFile.Format(L"clientvolume:%.2lf  SubBrokerVolume:%.2lf    BrokerVolume:%.2lf   CompanyVolume:%.2lf", m_clientvolume, m_SubBrokerVolume, m_BrokerVolume, m_CompanyVolume);

			////(strLogFile);


			*st_pac.m_subbrokerNetAmount=(m_SubBrokerGrossAmount*m_subbrokerRatio/100)-*st_pac.m_subbrokerBrokarage;
			*st_pac.m_brokerNetAmount=(m_BrokerGrossAmount*m_brokerRatio/100)-*st_pac.m_brokerBrokarage;
			*st_pac.m_companyNetAmount=(m_CompanyGrossAmount*m_companyRatio/100)-*st_pac.m_companyBrokarage;

			st_TickBidAskLast m_lastrate=*st_pac.m_lastrate;
			double m_clientexposure=m_NewCalculatedvolume*m_multiplyer* (*st_pac.m_average);
			*st_pac.m_clientexposure=m_clientexposure;

			*st_pac.m_subbrokerexposure = ((*st_pac.m_clientexposure) * (m_subbrokerRatio)) / 100;
			*st_pac.m_brokerexposure = ((*st_pac.m_clientexposure) * (m_brokerRatio)) / 100;
			*st_pac.m_companyexposure = ((*st_pac.m_clientexposure) * (m_companyRatio)) / 100;
			/**st_pac.m_clientnettotal=*st_pac.m_clientnettotal+m_netamountDifferance;
			*st_pac.m_clientbroktotal=*st_pac.m_clientbroktotal+m_clientBrokerageDifferance; */

			
			//*stnetpos.m_clientfloatingpl		
			*st_pac.m_subbrokerfloatingpl = ((*st_pac.m_clientfloatingpl) * (m_subbrokerRatio)) / 100;
			*st_pac.m_brokerfloatingpl = ((*st_pac.m_clientfloatingpl) * (m_brokerRatio)) / 100;
			*st_pac.m_companyfloatingpl = ((*st_pac.m_clientfloatingpl) * (m_companyRatio)) / 100;


			//*stnetpos.m_clientbalance;
			*st_pac.m_subbrokerbalance = ((*st_pac.m_clientbalance) * (m_subbrokerRatio)) / 100;
			*st_pac.m_brokerbalance = ((*st_pac.m_clientbalance) * (m_brokerRatio)) / 100;
			*st_pac.m_companybalance = ((*st_pac.m_clientbalance) * (m_companyRatio)) / 100;



			*st_pac.m_subbrokerNetAmount = *st_pac.m_SubBrokerGrossAmount - *st_pac.m_subbrokerBrokarage;
			*st_pac.m_brokerNetAmount = *st_pac.m_BrokerGrossAmount - *st_pac.m_brokerBrokarage;
			*st_pac.m_companyNetAmount = *st_pac.m_CompanyGrossAmount - *st_pac.m_companyBrokarage;


			st_Orika_symbolmaster m_stSymbol = {};

			m_Orika_symbolmasterHastable.Lookup(tickSymbol, m_stSymbol);
			st_pac.m_rm = ((*st_pac.m_companyvolume) * (*st_pac.m_average) * (*st_pac.m_multi) * st_pac.m_rmp ) / 10000000;

			CMTStr::Copy(st_pac.m_currencybase, m_stSymbol.m_currencybase);
			CMTStr::Copy(st_pac.m_commoditygroup, m_stSymbol.m_Group);

			CString m_commoditygroup = m_stSymbol.m_Group;
			if (st_pac.m_QtyMulti == 0)
			{
				st_pac.m_QtyMulti = 1;
			}
			st_pac.m_qtyAfterMulti = (*st_pac.m_companyvolume) * st_pac.m_QtyMulti;

			
			st_pac.m_LpVolume = st_pac.m_qtyAfterMulti * st_pac.m_LpRatio / 100;
			

			/*if (m_commoditygroup == L"GOLD")
			{
				CString strlogstr = L"";
				double preval = *LpVolumeTotal[lpvolumetotalIndex];
				strlogstr.Format(L"Previous Value %.2lf", preval);				
			}*/

			int lpvolumetotalIndex = -1;
			LpVolumeTotalArrayIndex.Lookup(m_commoditygroup, lpvolumetotalIndex);			
			double m_actualCompanyVolume = st.m_companyVolume;
			m_actualCompanyVolume = m_actualCompanyVolume / 10000;
			if (st.m_action == 1)
			{
				m_actualCompanyVolume = -m_actualCompanyVolume;
			}
			double nowLpVolume= (m_actualCompanyVolume *st_pac.m_QtyMulti) * st_pac.m_LpRatio / 100;
			*LpVolumeTotal[lpvolumetotalIndex] = *LpVolumeTotal[lpvolumetotalIndex] + nowLpVolume;
			*st_pac.m_LpVolumeTotal = *LpVolumeTotal[lpvolumetotalIndex];
			double m_totalLPvolume = *st_pac.m_LpVolumeTotal;
			CStaticClass::m_LPLots.SetAt(m_commoditygroup, m_totalLPvolume);


			//Getting CompanytotalvolumeafterMulti address		
			int  CompanyVolumeAfterMultiTotalIndex = -1;
			clientnettotalArrayIndex.Lookup(m_commoditygroup, CompanyVolumeAfterMultiTotalIndex);										
			double comVolumeAfterMulti = (m_actualCompanyVolume * st_pac.m_QtyMulti);
			/**CompanyVolumeAfterMultiTotalArray[CompanyVolumeAfterMultiTotalIndex] = *CompanyVolumeAfterMultiTotalArray[CompanyVolumeAfterMultiTotalIndex] + comVolumeAfterMulti;

			*st_pac.m_CompanyVolumeTotalAfterMulti = *CompanyVolumeAfterMultiTotalArray[CompanyVolumeAfterMultiTotalIndex];
			double m_totalCompanyVolumeAfterMulti = *st_pac.m_CompanyVolumeTotalAfterMulti;
			CStaticClass::m_comTotalLotsAfterMultiGroupWise.SetAt(m_commoditygroup, m_totalCompanyVolumeAfterMulti);*/


			















			/*if (m_commoditygroup == L"GOLD")
			{
				CString strlogstr = L"";
				double Total = *LpVolumeTotal[lpvolumetotalIndex];
				strlogstr.Format(L"New Value %.2lf Total Value %.2lf", nowLpVolume, Total);				

			}*/
			int check = 0;





			/*if (lpvolumetotalIndex >= 0)
			{
				*LpVolumeTotal[lpvolumetotalIndex] = *st_pac.m_LpVolumeTotal + st_pac.m_LpVolume;
			}
			else
			{
				st_pac.m_LpVolumeTotal = LpVolumeTotal[LpVolumeTotalArrayMaxIndex];
				LpVolumeTotalArrayMaxIndex = LpVolumeTotalArrayMaxIndex + 1;
				LpVolumeTotalArrayIndex.SetAt(m_commoditygroup, lpvolumetotalIndex);
				*st_pac.m_LpVolumeTotal = st_pac.m_LpVolume;
			}*/

			







			CStaticClass::mapNetPositionClientWise.SetAt(loginAndSymbolKey,st_pac);

			
			CStaticClass::m_mutex_ClientList.Lock();
			//(L"L18");
			POSITION pos = CStaticClass::m_ClientContext.GetStartPosition ();		
			while (pos != NULL) 
			{
				CString strclientkey=L"";		
				CStaticClass::st_ClientContext m_st={};
				CStaticClass::m_ClientContext.GetNextAssoc(pos, strclientkey,m_st);				
				if((m_st.m_ClientWiseNetPositionStart==1) ) 
				{
					if (std::find(m_st.m_logins.begin(), m_st.m_logins.end(), strlogin) != m_st.m_logins.end())
					{
						m_st.m_newNetPositionUpdated.Add(&st_pac);							
					}
				}									
				CStaticClass::m_ClientContext.SetAt(strclientkey,m_st);
			}
			CStaticClass::m_mutex_ClientList.Unlock();
			//(L"_U18");

		}
		//Adding New Position 
		else
		{			
			CString	  strKey=L"";			
			CString   m_login=st.m_login ;
			CString   m_symbol=st.m_symbol ;
			int		  m_Action=st.m_action; 
			double    m_Volume=(double)st.m_volume /10000; 
			double    m_subBrokerVolume=st.m_subBrokerVolume;
			double    m_brokerVolume=st.m_brokerVolume;
			double    m_extraGroupVolume=st.m_extraGroupVolume;
			double    m_companyVolume=st.m_companyVolume;
			double    m_WAvgPrice=st.m_price ;	
			double    m_currentPrice=st.m_price;
			double    m_multiplyer=st.m_multiplyer ;
			double    m_floatingProfit=0;
			double    m_subBrokerfloatingProfit=0;
			double    m_brokerfloatingProfit=0;
			double    m_extraGroupfloatingProfit=0;
			double    m_companyfloatingProfit=0;
		

			CString strSymbolGroup=L"";

		


			st_Orika_symbolmaster m_stSymbol={};		

			m_Orika_symbolmasterHastable.Lookup(m_symbol,m_stSymbol);
			strSymbolGroup=m_stSymbol.m_symbolGroup;			
			if (m_symbol.Find(L".P.") > 0 || m_symbol.Find(L".C.") > 0)
			{
				strSymbolGroup = L"option";				
			}
			
			CString ClientSymbolGroupKey=L"";
			ClientSymbolGroupKey.Format(L"%s:%s",m_login,strSymbolGroup);
			CString ClientKey=L"";
			ClientKey.Format(L"%s:",m_login);

			CString ClientSymbolKey=L"";
			ClientSymbolKey.Format(L"%s:%s",m_login,m_symbol);
			//updating symbol wise login List
			 loginarray m_loginarray ;
			 mapSymbolandLogin.Lookup(m_symbol,m_loginarray);
			 int rowscount=m_loginarray.Total();
			 if (rowscount>1)
			 {
				 int y=0;
			 }
			 int checkDataExistence=0;
			 for (int i=0;i<rowscount;i++)
			 {
				 CString dataval=m_loginarray[i];			 
				 if (dataval==m_login)
				 {
					 checkDataExistence=1;
					 break;
				 }
			 }
			 if (checkDataExistence==0)
			 {
				 wchar_t tempdata[30];
				 CMTStr::Copy(tempdata,m_login);
				 m_loginarray.Add(&tempdata);
			 }
			 int rowsCount=m_loginarray.Total();
			 mapSymbolandLogin.SetAt(m_symbol,m_loginarray);



			//End of updating symbol wise login List

			 //Updating data in mapNetPositionClientWise
			 st_netpositionClientWise stnetpos={};					 
			 stnetpos.m_average = (double*)malloc(sizeof(double));
			 *stnetpos.m_average = 0;
			 stnetpos.m_lastrate = (st_TickBidAskLast*)malloc(sizeof(st_TickBidAskLast));
			 stnetpos.m_lastrate->m_bid = 0;
			 stnetpos.m_lastrate->m_ask = 0;
			 stnetpos.m_maxallotedqty = (double*)malloc(sizeof(double));
			 *stnetpos.m_maxallotedqty = 0;
			 stnetpos.m_extravolume = (double*)malloc(sizeof(double));
			 *stnetpos.m_extravolume = 0;
			 stnetpos.m_freemargin = (double*)malloc(sizeof(double));
			 *stnetpos.m_freemargin = 0;
			 stnetpos.m_multi = (double*)malloc(sizeof(double));
			 *stnetpos.m_multi = 0;
			 stnetpos.m_subbrokerRatio = (double*)malloc(sizeof(double));
			 *stnetpos.m_subbrokerRatio = 0;
			 stnetpos.m_brokerRatio = (double*)malloc(sizeof(double));
			 *stnetpos.m_brokerRatio = 0;
			 stnetpos.m_companyRatio = (double*)malloc(sizeof(double));
			 *stnetpos.m_companyRatio = 0;
			 stnetpos.m_clientexposure = (double*)malloc(sizeof(double));
			 *stnetpos.m_clientexposure = 0;
			 stnetpos.m_subbrokerexposure = (double*)malloc(sizeof(double));
			 *stnetpos.m_subbrokerexposure = 0;
			 stnetpos.m_brokerexposure = (double*)malloc(sizeof(double));
			 *stnetpos.m_brokerexposure = 0;
			 stnetpos.m_companyexposure = (double*)malloc(sizeof(double));
			 *stnetpos.m_companyexposure = 0;
			 stnetpos.m_volume = (double*)malloc(sizeof(double));
			 *stnetpos.m_volume = 0;
			 stnetpos.m_subbrokervolume = (double*)malloc(sizeof(double));
			 *stnetpos.m_subbrokervolume = 0;
			 stnetpos.m_brokervolume = (double*)malloc(sizeof(double));
			 *stnetpos.m_brokervolume = 0;
			 stnetpos.m_companyvolume = (double*)malloc(sizeof(double));
			 *stnetpos.m_companyvolume = 0;

			 stnetpos.m_ClientGrossAmount = (double*)malloc(sizeof(double));
			 *stnetpos.m_ClientGrossAmount = 0;
			 stnetpos.m_SubBrokerGrossAmount = (double*)malloc(sizeof(double));
			 *stnetpos.m_SubBrokerGrossAmount = 0;
			 stnetpos.m_BrokerGrossAmount = (double*)malloc(sizeof(double));
			 *stnetpos.m_BrokerGrossAmount = 0;
			 stnetpos.m_CompanyGrossAmount = (double*)malloc(sizeof(double));
			 *stnetpos.m_CompanyGrossAmount = 0;
			 stnetpos.m_brokerageType = (double*)malloc(sizeof(double));
			 *stnetpos.m_brokerageType = 0;
			 stnetpos.m_clientBrokRate = (double*)malloc(sizeof(double));
			 *stnetpos.m_clientBrokRate = 0;
			 stnetpos.m_subBrokerBrokRate = (double*)malloc(sizeof(double));
			 *stnetpos.m_subBrokerBrokRate = 0;
			 stnetpos.m_brokerBrokRate = (double*)malloc(sizeof(double));
			 *stnetpos.m_brokerBrokRate = 0;
			 stnetpos.m_companyBrokRate = (double*)malloc(sizeof(double));
			 *stnetpos.m_companyBrokRate = 0;

			 stnetpos.m_clientfloatingpl = (double*)malloc(sizeof(double));
			 *stnetpos.m_clientfloatingpl = 0;
			 stnetpos.m_subbrokerfloatingpl = (double*)malloc(sizeof(double));
			 *stnetpos.m_subbrokerfloatingpl = 0;
			 stnetpos.m_brokerfloatingpl = (double*)malloc(sizeof(double));
			 *stnetpos.m_brokerfloatingpl = 0;
			 stnetpos.m_companyfloatingpl = (double*)malloc(sizeof(double));
			 *stnetpos.m_companyfloatingpl = 0;
			 stnetpos.m_clientbalance = (double*)malloc(sizeof(double));
			 *stnetpos.m_clientbalance = 0;
			 stnetpos.m_subbrokerbalance = (double*)malloc(sizeof(double));
			 *stnetpos.m_subbrokerbalance = 0;
			 stnetpos.m_brokerbalance = (double*)malloc(sizeof(double));
			 *stnetpos.m_brokerbalance = 0;
			 stnetpos.m_companybalance = (double*)malloc(sizeof(double));
			 *stnetpos.m_companybalance = 0;
			 stnetpos.m_clientgross = (double*)malloc(sizeof(double));
			 *stnetpos.m_clientgross = 0;
			 stnetpos.m_subbrokergross = (double*)malloc(sizeof(double));
			 *stnetpos.m_subbrokergross = 0;
			 stnetpos.m_brokergross = (double*)malloc(sizeof(double));
			 *stnetpos.m_brokergross = 0;
			 stnetpos.m_companygross = (double*)malloc(sizeof(double));
			 *stnetpos.m_companygross = 0;

			 stnetpos.m_clientBrokarage = (double*)malloc(sizeof(double));
			 *stnetpos.m_clientBrokarage = 0;
			 stnetpos.m_subbrokerBrokarage = (double*)malloc(sizeof(double));
			 *stnetpos.m_subbrokerBrokarage = 0;
			 stnetpos.m_brokerBrokarage = (double*)malloc(sizeof(double));
			 *stnetpos.m_brokerBrokarage = 0;
			 stnetpos.m_companyBrokarage = (double*)malloc(sizeof(double));
			 *stnetpos.m_companyBrokarage = 0;

			 stnetpos.m_clientnetamount = (double*)malloc(sizeof(double));
			 *stnetpos.m_clientnetamount = 0;
			 stnetpos.m_subbrokerNetAmount = (double*)malloc(sizeof(double));
			 *stnetpos.m_subbrokerNetAmount = 0;
			 stnetpos.m_brokerNetAmount = (double*)malloc(sizeof(double));
			 *stnetpos.m_brokerNetAmount = 0;
			 stnetpos.m_companyNetAmount = (double*)malloc(sizeof(double));
			 stnetpos.m_creditLimit = (double*)malloc(sizeof(double));			 
			 *stnetpos.m_creditLimit = 0;
			 *stnetpos.m_companyNetAmount = 0;
			 stnetpos.m_LpVolumeTotal = (double*)malloc(sizeof(double));
			 *stnetpos.m_LpVolumeTotal = 0;
			 

			
			CMTStr::Copy(stnetpos.m_login,m_login);


			//Getting Login Name
			CString m_name=L"";
			st_Orika_clientmaster m_st_Orika_clientmaster={};		
			m_Orika_clientmasterHastable.Lookup(m_login,m_st_Orika_clientmaster);
			m_name=m_st_Orika_clientmaster.m_name ;
			//End of Getting Login Name


			CMTStr::Copy(stnetpos.m_name,m_name);
			CMTStr::Copy(stnetpos.m_symbol,m_symbol);
			if (st.m_action ==1)		
			{
				*stnetpos.m_volume=(double)-st.m_volume /10000 ;
			}
			else
			{
				*stnetpos.m_volume=(double)st.m_volume/10000 ;
			}

			stnetpos.m_previousvolume=0; 
			stnetpos.m_difference=0;
			*stnetpos.m_average=st.m_price  ;

			//Getting lastrate address
			int lastRateArrayIndex=-1;
			symbolLastTickArrayIndex.Lookup(m_symbol,lastRateArrayIndex);
			//End 



			if (lastRateArrayIndex==-1)
			{
				CStaticClass::symbolLastTickArrayIndex.SetAt(m_symbol,CStaticClass::lastrateArrayMaxindex);
				lastRateArrayIndex=CStaticClass::lastrateArrayMaxindex;
				CStaticClass::lastrateArrayMaxindex=CStaticClass::lastrateArrayMaxindex+1;
				CStaticClass::lastrateArray[lastRateArrayIndex]->m_bid=0;		 
				CStaticClass::lastrateArray[lastRateArrayIndex]->m_ask = 0;
				CStaticClass::lastrateArray[lastRateArrayIndex]->m_last = 0;
				/*CString strlog=L"";
				strlog.Format(L"Symbol:%s Max TickData Index %d",m_symbol,CStaticClass::lastrateArrayMaxindex);
				////(strlog);*/
			}			
			*stnetpos.m_multi=m_stSymbol.m_multiplayer;

				stnetpos.m_lastrate = lastrateArray[lastRateArrayIndex];
				stnetpos.m_lastrate->m_ask = lastrateArray[lastRateArrayIndex]->m_ask;			
				stnetpos.m_lastrate->m_bid = lastrateArray[lastRateArrayIndex]->m_bid;
			


			double previous_floatingPL=0 ;
			double Plpoint=0;
			double floatingPL=0;
			if (*stnetpos.m_volume<0)
			{
				Plpoint=*stnetpos.m_average-(stnetpos.m_lastrate->m_ask); 
			}
			else
			{
				Plpoint=stnetpos.m_lastrate->m_bid-(*stnetpos.m_average); 
			}
			floatingPL=Plpoint*abs((*stnetpos.m_volume)) *(*stnetpos.m_multi) ;

			*stnetpos.m_clientfloatingpl=floatingPL;

			

			double  check_lastRate = 0;
			
				check_lastRate = stnetpos.m_lastrate->m_ask;			
				check_lastRate = stnetpos.m_lastrate->m_bid;
			
			

			*stnetpos.m_clientbalance=getClientBalance(m_login,m_symbol);
			


			//ClientSymbolGroupKey
			st_Orika_clientbrokerage m_st_Orika_clientbrokerage={};
			CStaticClass::m_Orika_clientbrokerageHastable.Lookup(ClientSymbolGroupKey,m_st_Orika_clientbrokerage);
			//ClientKey
			CString strBrokerageLogin=m_st_Orika_clientbrokerage.m_login;
			if (strBrokerageLogin.Trim()==L"")
			{
				CStaticClass::m_Orika_clientbrokerageHastable.Lookup(ClientKey,m_st_Orika_clientbrokerage);
			}
		


			st_Orika_PLDevideRatio m_st_Orika_PLDevideRatio={};
			CStaticClass::m_Orika_PLDevideRatioHastable.Lookup(ClientSymbolGroupKey,m_st_Orika_PLDevideRatio);
			strBrokerageLogin=m_st_Orika_PLDevideRatio.m_login;
		
			if (strBrokerageLogin==L"")
			{
				CStaticClass::m_Orika_PLDevideRatioHastable.Lookup(ClientKey,m_st_Orika_PLDevideRatio);
			}


			*stnetpos.m_subbrokerRatio=m_st_Orika_PLDevideRatio.m_subBrokerPLRatio ;		
			*stnetpos.m_brokerRatio=m_st_Orika_PLDevideRatio.m_brokerPLRatio ;
			*stnetpos.m_companyRatio=m_st_Orika_PLDevideRatio.m_companyPLRatio ;
			if (*stnetpos.m_companyRatio == 0)
			{
				*stnetpos.m_companyRatio = 100;
			}

			*stnetpos.m_brokerageType=m_st_Orika_clientbrokerage.m_brokageType  ;
			*stnetpos.m_clientBrokRate=m_st_Orika_clientbrokerage.m_clientBrokage ;
			*stnetpos.m_subBrokerBrokRate=m_st_Orika_clientbrokerage.m_subBrokerBrokage ;
			*stnetpos.m_brokerBrokRate=m_st_Orika_clientbrokerage.m_bokerBrokage ;
			*stnetpos.m_companyBrokRate=m_st_Orika_clientbrokerage.m_companyBrokage ;


			//Calculating Client Brokerage
			double m_brokerageType=*stnetpos.m_brokerageType;
			double m_brokeragerate=*stnetpos.m_clientBrokRate;
			double m_oldBrokerage=*stnetpos.m_clientBrokarage;
			double m_newbrolerage=0;
			double m_CalculatedBrokerage=0;
			
			
				m_newbrolerage=st.m_clientBrokTotal;
			
			m_CalculatedBrokerage=m_oldBrokerage+m_newbrolerage;

			double m_clientBrokerageDifferance=m_newbrolerage;
			*stnetpos.m_clientBrokarage=m_CalculatedBrokerage;

			*stnetpos.m_subbrokerBrokarage = *stnetpos.m_subbrokerBrokarage+st.m_subBrokerBrokTotal;
			*stnetpos.m_brokerBrokarage = *stnetpos.m_brokerBrokarage + st.m_brokerBrokTotal;
			*stnetpos.m_companyBrokarage = *stnetpos.m_companyBrokarage + st.m_comBalancebrokTotal;



			/*double m_subBrokerBrokRate=*stnetpos.m_subBrokerBrokRate; 
			double m_brokerBrokRate=*stnetpos.m_brokerBrokRate;
			double m_companyBrokRate=*stnetpos.m_companyBrokRate; */


			*stnetpos.m_ClientGrossAmount=*stnetpos.m_clientbalance+*stnetpos.m_clientfloatingpl;
			*stnetpos.m_SubBrokerGrossAmount=((*stnetpos.m_ClientGrossAmount)*(*stnetpos.m_subbrokerRatio))/100;
			*stnetpos.m_BrokerGrossAmount=((*stnetpos.m_ClientGrossAmount)*(*stnetpos.m_brokerRatio))/100;
			*stnetpos.m_CompanyGrossAmount=((*stnetpos.m_ClientGrossAmount)*(*stnetpos.m_companyRatio))/100;
			
			//CStaticClass::m_brokerageLoginSymbolWiseHastable
			st_orika_brokerageLoginSymbolWise m_tmpSt={};
			m_brokerageLoginSymbolWiseHastable.Lookup(ClientSymbolKey,m_tmpSt);

			
						
		

			*stnetpos.m_clientnetamount = (*stnetpos.m_clientbalance + *stnetpos.m_clientfloatingpl) - *stnetpos.m_clientBrokarage;
			*stnetpos.m_subbrokerNetAmount=*stnetpos.m_SubBrokerGrossAmount-*stnetpos.m_subbrokerBrokarage;
			*stnetpos.m_brokerNetAmount=*stnetpos.m_BrokerGrossAmount-*stnetpos.m_brokerBrokarage;
			*stnetpos.m_companyNetAmount=*stnetpos.m_CompanyGrossAmount-*stnetpos.m_companyBrokarage;



			



			//Getting ClientGrossTotal address
			int ClientGrossTotalIndex=-1;				
			clientgrosstotalArrayIndex.Lookup(m_login,ClientGrossTotalIndex);
			//End 
			if (ClientGrossTotalIndex==-1)
			{
				//clientgrosstotalArrayMaxindex
				ClientGrossTotalIndex=clientgrosstotalArrayMaxindex;
				clientgrosstotalArrayIndex.SetAt(m_login,ClientGrossTotalIndex);
				clientgrosstotalArrayMaxindex=clientgrosstotalArrayMaxindex+1;
				*clientgrosstotalArray[ClientGrossTotalIndex]=0;
			}	
			stnetpos.m_clientgrosstotal=clientgrosstotalArray[ClientGrossTotalIndex];						
			*clientgrosstotalArray[ClientGrossTotalIndex]=*clientgrosstotalArray[ClientGrossTotalIndex]+*stnetpos.m_clientfloatingpl;
			
		
			//Getting ClientGrossTotal address
			int clientbroktotalIndex=-1;				
			clientbroktotalArrayIndex.Lookup(m_login,clientbroktotalIndex);
			//End 
			if (clientbroktotalIndex==-1)
			{
				clientbroktotalIndex=clientbroktotalArrayMaxindex;
				clientbroktotalArrayIndex.SetAt(m_login,clientbroktotalIndex);
				clientbroktotalArrayMaxindex=clientbroktotalArrayMaxindex+1;
				*clientbroktotalArray[clientbroktotalIndex]=0;
			}
			stnetpos.m_clientbroktotal=clientbroktotalArray[clientbroktotalIndex];
			*clientbroktotalArray[clientbroktotalIndex]=*clientbroktotalArray[clientbroktotalIndex]+m_clientBrokerageDifferance;

		
			//Getting ClientGrossTotal address
		
			int clientnettotalIndex=-1;				
			clientnettotalArrayIndex.Lookup(m_login,clientnettotalIndex);
		
			if (clientnettotalIndex==-1)
			{	
				clientnettotalIndex=clientnettotalArrayMaxindex;
				clientnettotalArrayIndex.SetAt(m_login,clientnettotalIndex);									
				*clientnettotalArray[clientnettotalIndex]=0;
				clientnettotalArrayMaxindex=clientnettotalArrayMaxindex+1;			
			}
			//End 
			stnetpos.m_clientnettotal=clientnettotalArray[clientnettotalIndex];		
			*clientnettotalArray[clientnettotalIndex]=*clientnettotalArray[clientnettotalIndex]+*stnetpos.m_clientfloatingpl-m_clientBrokerageDifferance;		

			int max_AllotedQty=0;
			st_Orika_Clientgatewayconfig m_st={};
			m_Orika_ClientgatewayconfigHastable.Lookup(ClientSymbolGroupKey,m_st);
			*stnetpos.m_maxallotedqty=m_st.m_symbolPositionLimit;
			CMTStr::Copy(stnetpos.m_subbroker,m_st_Orika_clientmaster.m_subBroker);
			CMTStr::Copy(stnetpos.m_broker,m_st_Orika_clientmaster.m_broker);
			CMTStr::Copy(stnetpos.m_company, m_st_Orika_clientmaster.m_company);
			
			if (stnetpos.m_multi==0)
			{
				*stnetpos.m_multi=1;
			}		
			
			*stnetpos.m_extravolume=*stnetpos.m_maxallotedqty-*stnetpos.m_volume ;
			*stnetpos.m_freemargin=0;
					


			*stnetpos.m_clientexposure = *stnetpos.m_average * (*stnetpos.m_volume) * (*stnetpos.m_multi);
			*stnetpos.m_subbrokerexposure = ((*stnetpos.m_clientexposure) * (*stnetpos.m_subbrokerRatio)) / 100;
			*stnetpos.m_brokerexposure = ((*stnetpos.m_clientexposure) * (*stnetpos.m_brokerRatio)) / 100;
			*stnetpos.m_companyexposure = ((*stnetpos.m_clientexposure) * (*stnetpos.m_companyRatio)) / 100;


			*stnetpos.m_subbrokervolume = ((*stnetpos.m_volume) * (*stnetpos.m_subbrokerRatio)) / 100;
			*stnetpos.m_brokervolume = ((*stnetpos.m_volume) * (*stnetpos.m_brokerRatio)) / 100;
			*stnetpos.m_companyvolume = ((*stnetpos.m_volume) * (*stnetpos.m_companyRatio)) / 100;
			


			//*stnetpos.m_clientfloatingpl		
			*stnetpos.m_subbrokerfloatingpl = ((*stnetpos.m_clientfloatingpl) * (*stnetpos.m_subbrokerRatio)) / 100;
			*stnetpos.m_brokerfloatingpl = ((*stnetpos.m_clientfloatingpl) * (*stnetpos.m_brokerRatio)) / 100;
			*stnetpos.m_companyfloatingpl = ((*stnetpos.m_clientfloatingpl) * (*stnetpos.m_companyRatio)) / 100;


			//*stnetpos.m_clientbalance;
			*stnetpos.m_subbrokerbalance = ((*stnetpos.m_clientbalance) * (*stnetpos.m_subbrokerRatio)) / 100;
			*stnetpos.m_brokerbalance = ((*stnetpos.m_clientbalance) * (*stnetpos.m_brokerRatio)) / 100;
			*stnetpos.m_companybalance = ((*stnetpos.m_clientbalance) * (*stnetpos.m_companyRatio)) / 100;

						

			*stnetpos.m_subbrokerNetAmount = *stnetpos.m_SubBrokerGrossAmount - *stnetpos.m_subbrokerBrokarage;
			*stnetpos.m_brokerNetAmount = *stnetpos.m_BrokerGrossAmount - *stnetpos.m_brokerBrokarage;
			*stnetpos.m_companyNetAmount = *stnetpos.m_CompanyGrossAmount - *stnetpos.m_companyBrokarage;
			





			CMTStr::Copy(stnetpos.m_exchange, m_stSymbol.m_exchange);
			CMTStr::Copy(stnetpos.m_international, m_stSymbol.m_international);
			CMTStr::Copy(stnetpos.m_sector, m_stSymbol.m_sector);
			CMTStr::Copy(stnetpos.m_industry, m_stSymbol.m_industry);
			stnetpos.m_rmp = m_stSymbol.m_rmp;
			stnetpos.m_QtyMulti = m_stSymbol.m_QtyMulti;
			CMTStr::Copy(stnetpos.m_page, m_stSymbol.m_page);
			CMTStr::Copy(stnetpos.m_categary, m_stSymbol.m_categary);
			stnetpos.m_rm = ((*stnetpos.m_companyvolume) * (*stnetpos.m_average) * (*stnetpos.m_multi) * stnetpos.m_rmp) / 10000000;
			

			CMTStr::Copy(stnetpos.m_currencybase, m_stSymbol.m_currencybase);
			CMTStr::Copy(stnetpos.m_commoditygroup, m_stSymbol.m_Group);
			if (stnetpos.m_QtyMulti == 0)
			{
				stnetpos.m_QtyMulti = 1;
			}
			stnetpos.m_qtyAfterMulti = (*stnetpos.m_companyvolume) * stnetpos.m_QtyMulti;
			

			CStaticClass::m_mutexcommoditygroup.Lock();
			CString strcommoditygroupkey = L"";
			CString m_commoditygroup = m_stSymbol.m_Group;
			strcommoditygroupkey.Format(L"%s:%s",m_login, m_commoditygroup);
			double m_LpRatio = 0;
			CStaticClass::m_LpRatioCommodityGroupWiseMap.Lookup(strcommoditygroupkey, m_LpRatio);
			CStaticClass::m_mutexcommoditygroup.Unlock();

			stnetpos.m_LpRatio = m_LpRatio;
			stnetpos.m_LpVolume = stnetpos.m_qtyAfterMulti * stnetpos.m_LpRatio/ 100;
			//Getting LPvolumetotal address
			CString m_group = m_stSymbol.m_Group;
			int  LpVolumeTotalIndex = -1;
			LpVolumeTotalArrayIndex.Lookup(m_group, LpVolumeTotalIndex);
			if (LpVolumeTotalIndex == -1)
			{
				LpVolumeTotalIndex = LpVolumeTotalArrayMaxIndex;
				LpVolumeTotalArrayIndex.SetAt(m_group, LpVolumeTotalIndex);
				LpVolumeTotalArrayMaxIndex = LpVolumeTotalArrayMaxIndex + 1;
				*LpVolumeTotal[LpVolumeTotalIndex] = stnetpos.m_LpVolume;
				stnetpos.m_LpVolumeTotal = LpVolumeTotal[LpVolumeTotalIndex];
			}
			else
			{
				stnetpos.m_LpVolumeTotal = LpVolumeTotal[LpVolumeTotalIndex];
				*LpVolumeTotal[LpVolumeTotalIndex] = *LpVolumeTotal[LpVolumeTotalIndex] + (stnetpos.m_LpVolume);
			}
			double m_totalLPvolume = *LpVolumeTotal[LpVolumeTotalIndex];
			CStaticClass::m_LPLots.SetAt(m_group, m_totalLPvolume);





			double comVolumeAfterMulti = stnetpos.m_qtyAfterMulti;
			//Getting CompanytotalvolumeafterMulti address		
			int  CompanyVolumeAfterMultiTotalIndex = -1;
			CompanyVolumeAfterMultiTotalArrayIndex.Lookup(m_group, CompanyVolumeAfterMultiTotalIndex);
			if (CompanyVolumeAfterMultiTotalIndex == -1)
			{
				CompanyVolumeAfterMultiTotalIndex = CompanyVolumeAfterMultiTotalArrayMaxIndex;
				CompanyVolumeAfterMultiTotalArrayIndex.SetAt(m_group, CompanyVolumeAfterMultiTotalIndex);
				CompanyVolumeAfterMultiTotalArrayMaxIndex = CompanyVolumeAfterMultiTotalArrayMaxIndex + 1;


				stnetpos.m_CompanyVolumeTotalAfterMulti = CompanyVolumeAfterMultiTotalArray[CompanyVolumeAfterMultiTotalIndex];
				*CompanyVolumeAfterMultiTotalArray[CompanyVolumeAfterMultiTotalIndex] = 0;
			}
			else
			{
				stnetpos.m_CompanyVolumeTotalAfterMulti = CompanyVolumeAfterMultiTotalArray[CompanyVolumeAfterMultiTotalIndex];
				*CompanyVolumeAfterMultiTotalArray[CompanyVolumeAfterMultiTotalIndex] = *CompanyVolumeAfterMultiTotalArray[CompanyVolumeAfterMultiTotalIndex] + comVolumeAfterMulti;
			}
			
			double m_totalCompanyVolumeAfterMulti = *CompanyVolumeAfterMultiTotalArray[CompanyVolumeAfterMultiTotalIndex];
			CStaticClass::m_comTotalLotsAfterMultiGroupWise.SetAt(m_group, m_totalCompanyVolumeAfterMulti);








			if (*stnetpos.m_volume < 0)
			{
				CMTStr::Copy(stnetpos.m_buySell, L"Sell");
			}
			else
			{
				CMTStr::Copy(stnetpos.m_buySell, L"Buy");
			}

			if (*stnetpos.m_clientnetamount < 0)
			{
				CMTStr::Copy(stnetpos.m_debitCredit, L"Debit");
			}
			else
			{
				CMTStr::Copy(stnetpos.m_debitCredit, L"Credit");
			}


			st_netpositionClientWise stnetpos_Check = {};
			mapNetPositionClientWise.Lookup(ClientSymbolKey, stnetpos_Check);
			mapNetPositionClientWise.SetAt(ClientSymbolKey,stnetpos);

			int m_DataNotExist = 0;
			CString m_loginFound = stnetpos_Check.m_login;
			m_loginFound = m_loginFound.Trim();
			if (m_loginFound != L"")
			{
				m_DataNotExist = 1;
			}
			////(L"L19");
			CStaticClass::m_mutex_ClientList.Lock();
			//(L"12");
			POSITION pos = CStaticClass::m_ClientContext.GetStartPosition ();		

			int m_totalData=CStaticClass::m_ClientContext.GetCount();



			while (pos != NULL) 
			{
				CString strclientkey=L"";		
				CStaticClass::st_ClientContext m_st={};
				CStaticClass::m_ClientContext.GetNextAssoc(pos, strclientkey,m_st);				
				if((m_st.m_ClientWiseNetPositionStart==1) ) 
				{
					//Check Problem
					if (std::find(m_st.m_logins.begin(), m_st.m_logins.end(), m_login) != m_st.m_logins.end())
					{
						/*if (m_DataNotExist == 1)
						{*/

							m_st.m_newNetPositionAdded.Add(&stnetpos);
						
					}
				}									
				CStaticClass::m_ClientContext.SetAt(strclientkey,m_st);
			}
			CStaticClass::m_mutex_ClientList.Unlock();
			//(L"U12");



		}
	CStaticClass::m_mutex_Tick.Unlock();

}



void CStaticClass::sendNewTradeToClientFromDeal(const IMTDeal* m_deal,int dealSate, CString clientkey)
{
	//(L"Enter sendNewTradeToClientFromDeal");
	CString strFinalJson = L"";

	UINT64 m_intLogin = m_deal->Login();

	CString m_login = L"";
	m_login.Format(L"%I64u", m_intLogin);
	int m_time = m_deal->Time();


	CString m_strTime = L"";
	CString m_tmp_date = L"";
	CMTStr256 str_time;
	SMTFormat::FormatDateTime(str_time, m_time, true, true);
	m_strTime = str_time.Str();

	CString m_symbol = m_deal->Symbol();

	int m_deal_No = m_deal->Deal();
	
	int m_type = m_deal->Action();
	CString strtype = L"";		
	double m_amount = m_deal->Profit();
	CString m_comment = m_deal->Comment();
	int m_order = m_deal->Order();
	double m_volume = m_deal->Volume();
	m_volume = m_volume / 10000;
	CString m_strTypeFordealing = L"";

	double m_price = m_deal->Price();
	
	


	CStaticClass::m_mutex_order.Lock();
	CStaticClass::st_order st_order = {};
	CStaticClass::m_Orika_orderHastable.Lookup(m_order, st_order);
	CStaticClass::m_mutex_order.Unlock();
	int m_order_type = st_order.m_type;

	if (m_type == 0)
	{
		m_strTypeFordealing = L"Deal Instant Buy";		
	}
	else if (m_type == 1)
	{
		m_strTypeFordealing = L"Deal Instant Sell";
	}
	if (m_order_type == 2)
	{
		m_strTypeFordealing = L"Deal Buy Limit";
	}
	else if (m_order_type == 3)
	{
		m_strTypeFordealing = L"Deal Sell Limit";
	}
	else if (m_order_type == 4)
	{
		m_strTypeFordealing = L"Deal Buy Stop";
	}
	else if (m_order_type == 5)
	{
		m_strTypeFordealing = L"Deal Sell Stop";
	}
	if (m_type == 0)
	{
		strtype = L"buy";
		
	}
	else if (m_type == 1)
	{
		//m_volume = -m_volume;
		strtype = L"sell";
		
	}
	
	//CString m_comment = m_deal->Comment();
	

	int m_dealReason = m_deal->Reason();




	CString strtmpdata = L"";
	CString m_reason = L"";
	if (dealSate == 1)
	{
		if (m_type == 2)
		{
			if (m_amount > 0)
			{
				m_strTypeFordealing = L"Balance";
				m_reason.Format(L"'%s' finished [#%d balance %.2lf [Deposit]", m_login, m_deal_No, m_amount);
			}
			else
			{
				m_strTypeFordealing = L"Balance";
				m_reason.Format(L"'%s' finished [#%d balance %.2lf [Withdrawal]", m_login, m_deal_No, m_amount);
			}
		}
		if (m_type == 3)
		{
			if (m_amount > 0)
			{
				m_strTypeFordealing = L"Credit";
				m_reason.Format(L"'%s' finished [#%d credit %.2lf [Credit In]", m_login, m_deal_No, m_amount);
			}
			else
			{
				m_strTypeFordealing = L"Credit";
				m_reason.Format(L"'%s' finished [#%d credit %.2lf [Credit Out]", m_login, m_deal_No, m_amount);
			}
		}
	}
	else if (dealSate == 2)
	{
		m_strTypeFordealing = L"Deal Updated";
		m_reason.Format(L"deal Updated ['%s' #%d %s %.2lf %s at %.4lf]", m_login, m_deal, strtype, m_volume, m_symbol, m_price);
	}
	else if (dealSate == 3)
	{
		m_strTypeFordealing = L"Deal Deleted";
		m_reason.Format(L"deal Deleted ['%s' #%d %s %.2lf %s at %.4lf]", m_login, m_deal, strtype, m_volume, m_symbol, m_price);
	}
		
	

	
	


	if (clientkey == "")
	{
		
		m_mutex_dealingClientList.Lock();
		POSITION pos = CStaticClass::m_ClientList_forDeal.GetStartPosition();

		while (pos != NULL)
		{
			//////(L"Sending Deal Data");
			CString strclientkey = L"";
			SSL_session* client = NULL;
			CStaticClass::m_ClientList_forDeal.GetNextAssoc(pos, strclientkey, client);

			CString strloginuser = client->key;
			strloginuser = strloginuser.Mid(0, strloginuser.Find(L":"));
			CStaticClass::st_ClientContext m_st = {};
			m_mutex_ClientList.Lock();
				CStaticClass::m_ClientContext.Lookup(strclientkey, m_st);
			m_mutex_ClientList.Unlock();
			if (std::find(m_st.m_logins.begin(), m_st.m_logins.end(), m_login) != m_st.m_logins.end())
			{						
				StringBuffer s;
				Writer<StringBuffer> writer(s);
				writer.StartObject();
				writer.Key("type");
				writer.String("DEALING_DATA");



				writer.Key("id");
				m_dealing_ID = m_dealing_ID + 1;
				writer.Int(m_dealing_ID);

				writer.Key("time");
				writer.Int(m_time);

				writer.Key("login");
				string strlogin = string(CT2CA(m_login));
				const char* stlogin = strlogin.c_str();
				writer.String(stlogin);
				writer.Key("symbol");
				string strsymbol = string(CT2CA(m_symbol));
				const char* stsymbol = strsymbol.c_str();
				writer.String(stsymbol);

				writer.Key("buysell");
				string stype = string(CT2CA(strtype));
				const char* m_strtype = stype.c_str();
				writer.String(m_strtype);

				writer.Key("volume");
				writer.Double(m_volume);
				writer.Key("price");
				writer.Double(m_price);
				writer.Key("reason");
				string strreason = string(CT2CA(m_reason));
				const char* streason = strreason.c_str();
				writer.String(streason);
				writer.Key("dealingtype");
				string sttype = string(CT2CA(m_strTypeFordealing));
				const char* sstype = sttype.c_str();
				writer.String(sstype);				
				writer.EndObject();
				CString strData = L"";
				strData = s.GetString();
				string strforsend = "";
				strforsend = CT2A(strData.GetString());
				s.Flush();
				s.Clear();
				writer.Flush();

				SendDataToClient(m_st.m_clientConnection, strforsend, strclientkey, m_st.m_activeClient);
			
			}

		}
		m_mutex_dealingClientList.Unlock();
		
	}
	else
	{
		
			CStaticClass::st_ClientContext m_st = {};
			CStaticClass::m_mutex_ClientList.Lock();
				CStaticClass::m_ClientContext.Lookup(clientkey, m_st);
			CStaticClass::m_mutex_ClientList.Unlock();
			if (std::find(m_st.m_logins.begin(), m_st.m_logins.end(), m_login) != m_st.m_logins.end())
			{						
				StringBuffer s;
				Writer<StringBuffer> writer(s);
				writer.StartObject();
				writer.Key("type");
				writer.String("DEALING_DATA");



				writer.Key("id");
				m_dealing_ID = m_dealing_ID + 1;
				writer.Int(m_dealing_ID);

				writer.Key("time");
				writer.Int(m_time);

				writer.Key("login");
				string strlogin = string(CT2CA(m_login));
				const char* stlogin = strlogin.c_str();
				writer.String(stlogin);
				writer.Key("symbol");
				string strsymbol = string(CT2CA(m_symbol));
				const char* stsymbol = strsymbol.c_str();
				writer.String(stsymbol);

				writer.Key("buysell");
				string stype = string(CT2CA(strtype));
				const char* m_strtype = stype.c_str();
				writer.String(m_strtype);

				writer.Key("volume");
				writer.Double(m_volume);
				writer.Key("price");
				writer.Double(m_price);
				writer.Key("reason");
				string strreason = string(CT2CA(m_reason));
				const char* streason = strreason.c_str();
				writer.String(streason);
				writer.Key("dealingtype");
				string sttype = string(CT2CA(m_strTypeFordealing));
				const char* sstype = sttype.c_str();
				writer.String(sstype);				
				writer.EndObject();
				CString strData = L"";
				strData = s.GetString();
				string strforsend = "";
				strforsend = CT2A(strData.GetString());
				s.Flush();
				s.Clear();
				writer.Flush();
				SendDataToClient(m_st.m_clientConnection, strforsend, clientkey, m_st.m_activeClient);						
			}		
	}			
}




void CStaticClass::sendorderdealingToClient(const IMTOrder* m_order, int dealSate, CString clientkey)
{
	//(L"Enter sendorderdealingToClient");
	CString strFinalJson = L"";

	UINT64 m_intLogin = m_order->Login();

	CString m_login = L"";
	m_login.Format(L"%I64u", m_intLogin);
	int m_time = m_order->TimeSetup();


	CString m_strTime = L"";
	CString m_tmp_date = L"";
	CMTStr256 str_time;
	SMTFormat::FormatDateTime(str_time, m_time, true, true);
	m_strTime = str_time.Str();

	CString m_symbol = m_order->Symbol();

	int m_orderno = m_order->Order();

	int m_type = m_order->Type();
	CString m_strTypeFordealing = L"";
	if (m_type == 0)
	{
		m_strTypeFordealing = L"Order Buy Market";
	}
	else if (m_type == 1)
	{
		m_strTypeFordealing = L"Order Sell Market";
	}
	else if (m_type == 2)
	{
		m_strTypeFordealing = L"Order Buy Limit";
	}
	else if (m_type == 3)
	{
		m_strTypeFordealing = L"Order Sell Limit";
	}
	else if (m_type == 4)
	{
		m_strTypeFordealing = L"Order Buy Stop";
	}
	else if (m_type == 5)
	{
		m_strTypeFordealing = L"Order Sell Stop";
	}
	CString strtype = L"";
	double m_volume = m_order->VolumeInitial();
	m_volume = m_volume / 10000;
	if (m_type == 0 || m_type == 2 || m_type == 4)
	{
		strtype = L"buy";
	}
	else if (m_type == 1 || m_type == 3 || m_type == 5)
	{
		strtype = L"sell";
		//m_volume = -m_volume;
	}

	double m_amount = 0;
	CString m_comment = m_order->Comment();

	


	
	double m_price = m_order->PriceOrder();
	//CString m_comment = m_deal->Comment();
	

	int m_dealReason = 0;

	CString OrderDesc = m_ordertypedesc[m_type];


	CString strtmpdata = L"";
	CString m_reason = L"";
	if (dealSate == 1)
	{
		//m_strTypeFordealing = L"Order Placed";
		m_reason.Format(L"'%s' #%d %s %.2lf %s at %.4lf", m_login, m_orderno, OrderDesc, m_volume,m_symbol, m_price);
	}
	else if (dealSate == 2)
	{
		m_strTypeFordealing = L"Order Modify";
		m_reason.Format(L"'%s' #%d %s %.2lf %s at %.4lf", m_login, m_orderno, OrderDesc, m_volume, m_symbol, m_price);
	}
	else if (dealSate == 3)
	{
		m_strTypeFordealing = L"Order Deleted";
		m_reason.Format(L"'%s' #%d %s %.2lf %s at %.4lf", m_login, m_orderno, OrderDesc, m_volume, m_symbol, m_price);
	}

	


	////(L"L20");
	CStaticClass::m_mutex_dealingClientList.Lock();
	//(L"13");

	if (clientkey == "")
	{
		int CountforTest=CStaticClass::m_ClientList_forDeal.GetCount();

		POSITION pos = CStaticClass::m_ClientList_forDeal.GetStartPosition();
		while (pos != NULL)
		{
			//////(L"Sending Deal Data");
			CString strclientkey = L"";
			SSL_session* client = NULL;
			CStaticClass::m_ClientList_forDeal.GetNextAssoc(pos, strclientkey, client);

			CString strloginuser = client->key;
			strloginuser = strloginuser.Mid(0, strloginuser.Find(L":"));
			CStaticClass::st_ClientContext m_st = {};
			CStaticClass::m_mutex_ClientList.Lock();
				CStaticClass::m_ClientContext.Lookup(strclientkey, m_st);
			CStaticClass::m_mutex_ClientList.Unlock();
			if (std::find(m_st.m_logins.begin(), m_st.m_logins.end(), m_login) != m_st.m_logins.end())
			{
				StringBuffer s;
				Writer<StringBuffer> writer(s);
				writer.StartObject();
				writer.Key("type");
				writer.String("DEALING_DATA");



				writer.Key("id");
				m_dealing_ID = m_dealing_ID + 1;
				writer.Int(m_dealing_ID);

				writer.Key("time");
				writer.Int(m_time);
				writer.Key("login");
				string strlogin = string(CT2CA(m_login));
				const char* stlogin = strlogin.c_str();
				writer.String(stlogin);
				writer.Key("symbol");
				string strsymbol = string(CT2CA(m_symbol));
				const char* stsymbol = strsymbol.c_str();
				writer.String(stsymbol);

				writer.Key("buysell");
				string stype = string(CT2CA(strtype));
				const char* m_strtype = stype.c_str();
				writer.String(m_strtype);

				writer.Key("volume");
				writer.Double(m_volume);
				writer.Key("price");
				writer.Double(m_price);
				writer.Key("reason");
				string strreason = string(CT2CA(m_reason));
				const char* streason = strreason.c_str();
				writer.String(streason);
				writer.Key("dealingtype");
				string sttype = string(CT2CA(m_strTypeFordealing));
				const char* sstype = sttype.c_str();
				writer.String(sstype);				

				writer.EndObject();
				CString strData = L"";
				strData = s.GetString();
				string strforsend = "";
				strforsend = CT2A(strData.GetString());
				s.Flush();
				s.Clear();
				writer.Flush();
				SendDataToClient(m_st.m_clientConnection, strforsend, strclientkey, m_st.m_activeClient);
		
			}

		}
	}
	else
	{
		
			CStaticClass::st_ClientContext m_st = {};
			CStaticClass::m_mutex_ClientList.Lock();
				CStaticClass::m_ClientContext.Lookup(clientkey, m_st);
			CStaticClass::m_mutex_ClientList.Unlock();
			if (std::find(m_st.m_logins.begin(), m_st.m_logins.end(), m_login) != m_st.m_logins.end())
			{
				
					StringBuffer s;
					Writer<StringBuffer> writer(s);
					writer.StartObject();
					writer.Key("type");
					writer.String("DEALING_DATA");



					writer.Key("id");
					m_dealing_ID = m_dealing_ID + 1;
					writer.Int(m_dealing_ID);

					writer.Key("time");
					writer.Int(m_time);
					writer.Key("login");
					string strlogin = string(CT2CA(m_login));
					const char* stlogin = strlogin.c_str();
					writer.String(stlogin);
					writer.Key("symbol");
					string strsymbol = string(CT2CA(m_symbol));
					const char* stsymbol = strsymbol.c_str();
					writer.String(stsymbol);

					writer.Key("buysell");
					string stype = string(CT2CA(strtype));
					const char* m_strtype = stype.c_str();
					writer.String(m_strtype);

					writer.Key("volume");
					writer.Double(m_volume);
					writer.Key("price");
					writer.Double(m_price);
					writer.Key("reason");
					string strreason = string(CT2CA(m_reason));
					const char* streason = strreason.c_str();
					writer.String(streason);
					writer.Key("dealingtype");
					string sttype = string(CT2CA(m_strTypeFordealing));
					const char* sstype = sttype.c_str();
					writer.String(sstype);					

					writer.EndObject();
					CString strData = L"";
					strData = s.GetString();
					string strforsend = "";
					strforsend = CT2A(strData.GetString());
					s.Flush();
					s.Clear();
					writer.Flush();
					SendDataToClient(m_st.m_clientConnection, strforsend, clientkey, m_st.m_activeClient);				
			}

		
	}
	CStaticClass::m_mutex_dealingClientList.Unlock();	
	//(L"Exit sendorderdealingToClient");
}



void CStaticClass::sendlogClient(const IMTOrder* m_order, int dealSate, CString clientkey)
{
	//(L"Enter sendorderdealingToClient");
	CString strFinalJson = L"";

	UINT64 m_intLogin = m_order->Login();

	CString m_login = L"";
	m_login.Format(L"%I64u", m_intLogin);
	int m_time = m_order->TimeSetup();


	CString m_strTime = L"";
	CString m_tmp_date = L"";
	CMTStr256 str_time;
	SMTFormat::FormatDateTime(str_time, m_time, true, true);
	m_strTime = str_time.Str();

	CString m_symbol = m_order->Symbol();

	int m_orderno = m_order->Order();

	int m_type = m_order->Type();
	CString m_strTypeFordealing = L"";
	if (m_type == 0)
	{
		m_strTypeFordealing = L"Order Buy Market";
	}
	else if (m_type == 1)
	{
		m_strTypeFordealing = L"Order Sell Market";
	}
	else if (m_type == 2)
	{
		m_strTypeFordealing = L"Order Buy Limit";
	}
	else if (m_type == 3)
	{
		m_strTypeFordealing = L"Order Sell Limit";
	}
	else if (m_type == 4)
	{
		m_strTypeFordealing = L"Order Buy Stop";
	}
	else if (m_type == 5)
	{
		m_strTypeFordealing = L"Order Sell Stop";
	}
	CString strtype = L"";
	double m_volume = m_order->VolumeInitial();
	m_volume = m_volume / 10000;
	if (m_type == 0 || m_type == 2 || m_type == 4)
	{
		strtype = L"buy";
	}
	else if (m_type == 1 || m_type == 3 || m_type == 5)
	{
		strtype = L"sell";
		//m_volume = -m_volume;
	}

	double m_amount = 0;
	CString m_comment = m_order->Comment();





	double m_price = m_order->PriceOrder();
	//CString m_comment = m_deal->Comment();


	int m_dealReason = 0;

	CString OrderDesc = m_ordertypedesc[m_type];


	CString strtmpdata = L"";
	CString m_reason = L"";
	if (dealSate == 1)
	{
		//m_strTypeFordealing = L"Order Placed";
		m_reason.Format(L"'%s' #%d %s %.2lf %s at %.4lf", m_login, m_orderno, OrderDesc, m_volume, m_symbol, m_price);
	}
	else if (dealSate == 2)
	{
		m_strTypeFordealing = L"Order Modify";
		m_reason.Format(L"'%s' #%d %s %.2lf %s at %.4lf", m_login, m_orderno, OrderDesc, m_volume, m_symbol, m_price);
	}
	else if (dealSate == 3)
	{
		m_strTypeFordealing = L"Order Deleted";
		m_reason.Format(L"'%s' #%d %s %.2lf %s at %.4lf", m_login, m_orderno, OrderDesc, m_volume, m_symbol, m_price);
	}




	////(L"L20");
	CStaticClass::m_mutex_dealingClientList.Lock();
	//(L"13");

	if (clientkey == "")
	{
		int CountforTest = CStaticClass::m_ClientList_forDeal.GetCount();

		POSITION pos = CStaticClass::m_ClientList_forDeal.GetStartPosition();
		while (pos != NULL)
		{
			//////(L"Sending Deal Data");
			CString strclientkey = L"";
			SSL_session* client = NULL;
			CStaticClass::m_ClientList_forDeal.GetNextAssoc(pos, strclientkey, client);

			CString strloginuser = client->key;
			strloginuser = strloginuser.Mid(0, strloginuser.Find(L":"));
			CStaticClass::st_ClientContext m_st = {};
			CStaticClass::m_mutex_ClientList.Lock();
			CStaticClass::m_ClientContext.Lookup(strclientkey, m_st);
			CStaticClass::m_mutex_ClientList.Unlock();
			if (std::find(m_st.m_logins.begin(), m_st.m_logins.end(), m_login) != m_st.m_logins.end())
			{
				StringBuffer s;
				Writer<StringBuffer> writer(s);
				writer.StartObject();
				writer.Key("type");
				writer.String("DEALING_DATA");



				writer.Key("id");
				m_dealing_ID = m_dealing_ID + 1;
				writer.Int(m_dealing_ID);

				writer.Key("time");
				writer.Int(m_time);
				writer.Key("login");
				string strlogin = string(CT2CA(m_login));
				const char* stlogin = strlogin.c_str();
				writer.String(stlogin);
				writer.Key("symbol");
				string strsymbol = string(CT2CA(m_symbol));
				const char* stsymbol = strsymbol.c_str();
				writer.String(stsymbol);

				writer.Key("buysell");
				string stype = string(CT2CA(strtype));
				const char* m_strtype = stype.c_str();
				writer.String(m_strtype);

				writer.Key("volume");
				writer.Double(m_volume);
				writer.Key("price");
				writer.Double(m_price);
				writer.Key("reason");
				string strreason = string(CT2CA(m_reason));
				const char* streason = strreason.c_str();
				writer.String(streason);
				writer.Key("dealingtype");
				string sttype = string(CT2CA(m_strTypeFordealing));
				const char* sstype = sttype.c_str();
				writer.String(sstype);

				writer.EndObject();
				CString strData = L"";
				strData = s.GetString();
				string strforsend = "";
				strforsend = CT2A(strData.GetString());
				s.Flush();
				s.Clear();
				writer.Flush();
				SendDataToClient(m_st.m_clientConnection, strforsend, strclientkey, m_st.m_activeClient);

			}

		}
	}
	else
	{

		CStaticClass::st_ClientContext m_st = {};
		CStaticClass::m_mutex_ClientList.Lock();
		CStaticClass::m_ClientContext.Lookup(clientkey, m_st);
		CStaticClass::m_mutex_ClientList.Unlock();
		if (std::find(m_st.m_logins.begin(), m_st.m_logins.end(), m_login) != m_st.m_logins.end())
		{

			StringBuffer s;
			Writer<StringBuffer> writer(s);
			writer.StartObject();
			writer.Key("type");
			writer.String("DEALING_DATA");



			writer.Key("id");
			m_dealing_ID = m_dealing_ID + 1;
			writer.Int(m_dealing_ID);

			writer.Key("time");
			writer.Int(m_time);
			writer.Key("login");
			string strlogin = string(CT2CA(m_login));
			const char* stlogin = strlogin.c_str();
			writer.String(stlogin);
			writer.Key("symbol");
			string strsymbol = string(CT2CA(m_symbol));
			const char* stsymbol = strsymbol.c_str();
			writer.String(stsymbol);

			writer.Key("buysell");
			string stype = string(CT2CA(strtype));
			const char* m_strtype = stype.c_str();
			writer.String(m_strtype);

			writer.Key("volume");
			writer.Double(m_volume);
			writer.Key("price");
			writer.Double(m_price);
			writer.Key("reason");
			string strreason = string(CT2CA(m_reason));
			const char* streason = strreason.c_str();
			writer.String(streason);
			writer.Key("dealingtype");
			string sttype = string(CT2CA(m_strTypeFordealing));
			const char* sstype = sttype.c_str();
			writer.String(sstype);

			writer.EndObject();
			CString strData = L"";
			strData = s.GetString();
			string strforsend = "";
			strforsend = CT2A(strData.GetString());
			s.Flush();
			s.Clear();
			writer.Flush();
			SendDataToClient(m_st.m_clientConnection, strforsend, clientkey, m_st.m_activeClient);
		}


	}
	CStaticClass::m_mutex_dealingClientList.Unlock();
	//(L"Exit sendorderdealingToClient");
}




void CStaticClass::sendNewTradeToClient(const IMTDeal* st)
{
	//(L"Enter sendNewTradeToClient");
		CString strFinalJson=L"";

		UINT64 m_intLogin = st->Login();

		CString m_login = L"";
		m_login.Format(L"%I64u", m_intLogin);
		int m_time =st->Time();

		m_time = m_time - 19800;
		CString m_strTime=L"";		
		CString m_tmp_date=L"";
		CMTStr256 str_time;
		SMTFormat::FormatDateTime(str_time,m_time,true,true);
		m_strTime=str_time.Str();



		int m_deal=st->Deal() ;
		int m_order=st->Order();
		CString m_symbol=st->Symbol();
		int m_type=st->Action() ;
		CString strtype=L"";
			
		double m_volume =st->Volume();
		m_volume = m_volume / 10000;



		CStaticClass::m_mutex_order.Lock();
		CStaticClass::st_order st_order = {};
		CStaticClass::m_Orika_orderHastable.Lookup(m_order, st_order);
		CStaticClass::m_mutex_order.Unlock();
		int m_order_type = st_order.m_type;
		CString m_strTypeFordealing = L"";
		if (m_type == 0)
		{
			m_strTypeFordealing = L"Deal Instant Buy";
		}
		else if (m_type == 1)
		{
			m_strTypeFordealing = L"Deal Instant Sell";
		}
		 if (m_order_type == 2)
		{
			m_strTypeFordealing = L"Deal Buy Limit";
		}
		else if (m_order_type == 3)
		{
			m_strTypeFordealing = L"Deal Sell Limit";
		}
		else if (m_order_type == 4)
		{
			m_strTypeFordealing = L"Deal Buy Stop";
		}
		else if (m_order_type == 5)
		{
			m_strTypeFordealing = L"Deal Sell Stop";
		}






		if (m_type == 0)
		{
			strtype = L"buy";
		}
		else if (m_type == 1)
		{			
			strtype = L"sell";
		}
		double m_price=st->Price() ;
		CString m_comment=st->Comment();

		CString strtmpdata=L"";	
		CString m_reason = L"";
		m_reason.Format(L"deal performed ['%s' #%d %s %.2lf %s at %.4lf]", m_login, m_deal, strtype, m_volume,m_symbol, m_price);
		


		


		////(L"L20");
			
		CStaticClass::m_mutex_dealingClientList.Lock();
		//(L"13");
		POSITION pos = CStaticClass::m_ClientList_forDeal.GetStartPosition ();		

		while (pos != NULL) 
		{
			//////(L"Sending Deal Data");
			CString strclientkey=L"";		
			SSL_session* client=NULL;
			CStaticClass::m_ClientList_forDeal.GetNextAssoc(pos, strclientkey,client);
			
			CString strloginuser= client->key;
			strloginuser=strloginuser.Mid(0,strloginuser.Find(L":"));
			CStaticClass::st_ClientContext m_st={};
			CStaticClass::m_mutex_ClientList.Lock();
				CStaticClass::m_ClientContext.Lookup(strclientkey,m_st );
			CStaticClass::m_mutex_ClientList.Unlock();
			if (std::find(m_st.m_logins.begin(), m_st.m_logins.end(), m_login) != m_st.m_logins.end())
			{					
				StringBuffer s;
				Writer<StringBuffer> writer(s);
				writer.StartObject();
				writer.Key("type");
				writer.String("DEALING_DATA");



				writer.Key("id");
				m_dealing_ID = m_dealing_ID + 1;
				writer.Int(m_dealing_ID);

				writer.Key("time");
				writer.Int(m_time);
				writer.Key("login");
				string strlogin = string(CT2CA(m_login));
				const char* stlogin = strlogin.c_str();
				writer.String(stlogin);
				writer.Key("symbol");
				string strsymbol = string(CT2CA(m_symbol));
				const char* stsymbol = strsymbol.c_str();
				writer.String(stsymbol);
				writer.Key("volume");
				writer.Double(m_volume);

				writer.Key("buysell");
				string stype = string(CT2CA(strtype));
				const char* m_strtype = stype.c_str();
				writer.String(m_strtype);

				writer.Key("price");
				writer.Double(m_price);
				writer.Key("reason");
				string strreason = string(CT2CA(m_reason));
				const char* streason = strreason.c_str();
				writer.String(streason);
				writer.Key("dealingtype");
				string sttype = string(CT2CA(m_strTypeFordealing));
				const char* sstype = sttype.c_str();
				writer.String(sstype);				

				writer.EndObject();
				CString strData = L"";
				strData = s.GetString();
				string strforsend = "";
				strforsend = CT2A(strData.GetString());
				s.Flush();
				s.Clear();
				writer.Flush();
				SendDataToClient(m_st.m_clientConnection, strforsend, strclientkey, m_st.m_activeClient);			
			}
		}
		CStaticClass::m_mutex_dealingClientList.Unlock();
		//(L"Exit sendNewTradeToClient");
}


CString CStaticClass::sysDateToStringFormat(SYSTEMTIME st)
{
	CString strDate = L"";

	strDate.Format(L"%02d-%02d %02d:%02d",
		st.wMonth,
		st.wDay,
		st.wHour,
		st.wMinute);
	return strDate;	
}
CString CStaticClass::sysOnlyDateToStringFormat(SYSTEMTIME st)
{
	CString strDate = L"";

	strDate.Format(L"%02d-%02d-%04d",
		st.wDay,
		st.wMonth, st.wYear);
	return strDate;
}

void CStaticClass::sendDataToAllClient(string msg)
{	
	////(L"Enter sendDataToAllClient");
	ClientContext m_ClientContext_Local;

		CStaticClass::m_mutex_ClientList.Lock();
		
		POSITION pos = CStaticClass::m_ClientContext.GetStartPosition();
		while (pos != NULL)
		{
			CString strclientkey = L"";
			CStaticClass::st_ClientContext m_st = {};
			CStaticClass::m_ClientContext.GetNextAssoc(pos, strclientkey, m_st);
			m_ClientContext_Local.SetAt(strclientkey, m_st);
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
			SendDataToClient(client, msg, strclientkey, m_st.m_activeClient);
			//(L"Data Has Been Sent To Client");
		}

		////(L"Exit sendDataToAllClient");
}


void CStaticClass::sendDataToAll_Other_Client(string msg,CString m_ignorekey)
{
	////(L"Enter sendDataToAllClient");
	ClientContext m_ClientContext_Local;

	CStaticClass::m_mutex_ClientList.Lock();

	POSITION pos = CStaticClass::m_ClientContext.GetStartPosition();
	while (pos != NULL)
	{
		CString strclientkey = L"";
		CStaticClass::st_ClientContext m_st = {};
		CStaticClass::m_ClientContext.GetNextAssoc(pos, strclientkey, m_st);
		if (m_ignorekey != strclientkey)
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
		SendDataToClient(client, msg, strclientkey, m_st.m_activeClient);
		//(L"Data Has Been Sent To Client");
	}

	////(L"Exit sendDataToAllClient");
}





void CStaticClass::sendTickToAllClient(CString m_symbol,string msg)
{
	////(L"Enter sendDataToAllClient");
	ClientContext m_ClientContext_Local;
	CStaticClass::m_mutex_ClientList.Lock();
	POSITION pos = CStaticClass::m_ClientContext.GetStartPosition();
	while (pos != NULL)
	{
		CString strclientkey = L"";
		CStaticClass::st_ClientContext m_st = {};
		CStaticClass::m_ClientContext.GetNextAssoc(pos, strclientkey, m_st);
		if (m_st.m_TicksSendingStart == 1)
		{
			/*if (m_st.m_Subscribed_Symbol.size()==0)
			{
				m_ClientContext_Local.SetAt(strclientkey, m_st);
			}*/

			for (int i = 0; i < m_st.m_Subscribed_Symbol.size(); i++)
			{
				if (m_st.m_Subscribed_Symbol[i] == m_symbol)
				{
					m_ClientContext_Local.SetAt(strclientkey, m_st);
					break; 
				}
			}			
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
		SendDataToClient(client, msg, strclientkey, m_st.m_activeClient);
		//(L"Data Has Been Sent To Client");
	}

	////(L"Exit sendDataToAllClient");
}

void CStaticClass::sendDataToAllClient_LPData(string msg)
{
	////(L"Enter sendDataToAllClient");
	ClientContext m_ClientContext_Local;
	CStaticClass::m_mutex_ClientList.Lock();
	POSITION pos = CStaticClass::m_ClientContext.GetStartPosition();
	while (pos != NULL)
	{
		CString strclientkey = L"";
		CStaticClass::st_ClientContext m_st = {};
		CStaticClass::m_ClientContext.GetNextAssoc(pos, strclientkey, m_st);
		if (m_st.m_start_gateway_positiondevideratio == 1 && m_st.m_ClientType==0)
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
		////(L"Going To Send Data To Client");
		SendDataToClient(client, msg, strclientkey, m_st.m_activeClient);
		////(L"Data Has Been Sent To Client");
	}
	////(L"Exit sendDataToAllClient");
}



void CStaticClass::sendDataToAllAPIClient_LPData(string msg)
{
	//(L"Enter sendDataToAllClient");
	ClientContext m_ClientContext_Local;
	CStaticClass::m_mutex_ClientList.Lock();
	POSITION pos = CStaticClass::m_ClientContext.GetStartPosition();
	while (pos != NULL)
	{
		CString strclientkey = L"";
		CStaticClass::st_ClientContext m_st = {};
		CStaticClass::m_ClientContext.GetNextAssoc(pos, strclientkey, m_st);
		if ((std::find(m_st.m_SubscribedRequest.begin(), m_st.m_SubscribedRequest.end(), 1001) != m_st.m_SubscribedRequest.end()) && m_st.m_ClientType == 1)
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
		////(L"Going To Send Data To Client");
		SendDataToClient(client, msg, strclientkey, m_st.m_activeClient);
		////(L"Data Has Been Sent To Client");
	}
	//(L"Exit sendDataToAllClient");
}




void CStaticClass::sendHeartBeattoAllClient()
{
	while(CStaticClass::heartBeatStart==1)
	{
		
		//////(L"L21");
		CStaticClass::m_mutex_ClientList.Lock();
		////(L"14");
		
		POSITION pos = CStaticClass::m_ClientContext.GetStartPosition ();
		
		while (pos != NULL) 
		{
			CString strclientkey=L"";		
			CStaticClass::st_ClientContext m_st={};
			CStaticClass::m_ClientContext.GetNextAssoc(pos, strclientkey,m_st);
			
				SSL_session* client=NULL;			
				client=m_st.m_clientConnection;
				UINT64 CurrentTime=_time64(NULL);
				if (CurrentTime-m_st.m_lastDataSended>5 && m_st.m_heartBeatStart==1)
				{
					
					string strforsend="{\"type\":\"HEART_BEAT\",\"data\":\"ok\"}";
					////(L"HB Sending Start");
					CStaticClass::m_ClientContext.GetNextAssoc(pos, strclientkey,m_st);
					SendDataToClient(client, strforsend, strclientkey, m_st.m_activeClient);
					////(L"HB Sending End");
					m_st.m_lastDataSended=CurrentTime;
					CStaticClass::m_ClientContext.SetAt(strclientkey,m_st);
				}

			
		}
		
		CStaticClass::m_mutex_ClientList.Unlock();
		////(L"U14");
		Sleep(20);
		//////(L"_U21");
	}
	
}

void CStaticClass::sendUpdatedTick()
{
	while (CStaticClass::CStaticClass::startTickData == 1)
	{
		CStaticClass::TickData m_localdata;
		CStaticClass::m_mutex_Tick_TickSubscribe.Lock();
		POSITION pos1 = CStaticClass::m_updatedTickData.GetStartPosition();
		while (pos1 != NULL)
		{
			CString  iKey = L"";
			CStaticClass::st_Tick m_st_Tick = {};
			CStaticClass::m_updatedTickData.GetNextAssoc(pos1, iKey, m_st_Tick);
			m_localdata.SetAt(iKey, m_st_Tick);
		}
		CStaticClass::m_updatedTickData.RemoveAll();
		CStaticClass::m_mutex_Tick_TickSubscribe.Unlock();
		
		POSITION pos = m_localdata.GetStartPosition();
		int row_count = 0;
		CString tmpstr = L"";
		CString strFinalJson = L"";
		while (pos != NULL)
		{
			CString  iKey = L"";
			CStaticClass::st_Tick m_st_Tick = {};
			m_localdata.GetNextAssoc(pos, iKey, m_st_Tick);
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
			
			strFinalJson.Format(L"{\"type\": \"TICK_DATA\",\"updatekey\":[\"symbol\"],\"update\": [%s]}", strtmpdata);
			string strforsend = CT2A(strFinalJson.GetString());
			sendTickToAllClient(m_symbol,strforsend);
		}
		
		
		
		Sleep(20);	
	}

}
void CStaticClass::comparePosition()
{
	
	while (CStaticClass::startComparePosition == 1)
	{
		m_mtmanager.getPositionFromMT();		

		CMap<CString, LPCTSTR, CStaticClass::st_netpositionClientWise, CStaticClass::st_netpositionClientWise&>  mapNetPositionClientWise_Temp;
		m_mutex_Tick.Lock();
		POSITION pos = mapNetPositionClientWise.GetStartPosition();
		while (pos != NULL)
		{
			st_netpositionClientWise m_stMain = {};
			CString loginSymbolKey = L"";
			mapNetPositionClientWise.GetNextAssoc(pos, loginSymbolKey, m_stMain);						
			mapNetPositionClientWise_Temp.SetAt(loginSymbolKey, m_stMain);
		}
		m_mutex_Tick.Unlock();
		POSITION pos_P = CStaticClass::m_ClientSymbolPosition_MT.GetStartPosition();
		while (pos_P != NULL)
		{
			CString login_symbol = L"";
			double m_volume = 0;
			CStaticClass::m_ClientSymbolPosition_MT.GetNextAssoc(pos_P, login_symbol, m_volume);
			m_volume = m_volume / 10000;
			st_netpositionClientWise m_stMain = {};
			mapNetPositionClientWise_Temp.Lookup(login_symbol, m_stMain);
			double m_volume_Orika = *m_stMain.m_volume;
			if (m_volume_Orika!= m_volume)
			{
				m_mismatch_position.SetAt(login_symbol, m_volume_Orika);
			}
		}
		for (int i = 0; i <= 5; i++)
		{
			Re_comparePosition();
			Sleep(1000);
		}

		POSITION pos_P_final = CStaticClass::m_mismatch_position.GetStartPosition();
		while (pos_P_final != NULL)
		{
			CString login_symbol = L"";
			double m_volume = 0;
			CStaticClass::m_mismatch_position.GetNextAssoc(pos_P_final, login_symbol, m_volume);
		}
		Sleep(300000);
	}

}

void CStaticClass::Re_comparePosition()
{	
	POSITION pos_P = CStaticClass::m_mismatch_position.GetStartPosition();
	while (pos_P != NULL)
	{
		CString login_symbol = L"";
		double m_volume = 0;
		CStaticClass::m_mismatch_position.GetNextAssoc(pos_P, login_symbol, m_volume);
		CString m_login = login_symbol.Mid(0, login_symbol.Find(L":") - 1);
		CString m_symbol= login_symbol.Mid(login_symbol.Find(L":") + 1,(login_symbol.GetLength()-(login_symbol.Find(L":") + 1)));
		UINT64 m_ulogin = _wtoi64(m_symbol);
		double m_mt_position=m_mtmanager.getPositionFromMTClientWise(m_ulogin, m_symbol);
		m_mt_position = m_mt_position / 10000;
		m_mutex_Tick.Lock();				
			st_netpositionClientWise m_stMain = {};			
			mapNetPositionClientWise.Lookup (login_symbol, m_stMain);
			double m_orika_position = *m_stMain.m_volume;
		m_mutex_Tick.Unlock();
		if (m_orika_position == m_mt_position)
		{
			mapNetPositionClientWise.RemoveKey(login_symbol);
		}
	}	
}




void CStaticClass::GettingTickDataFromManager()
{
	while (startCalculatingNetpositionClientWise == 1)
	{
		CMap<CString, LPCTSTR, CStaticClass::st_Tick, CStaticClass::st_Tick> symbolLastTickArray_Temp;

		CStaticClass::m_mutex_Tick_copy.Lock();
		POSITION posTick = CStaticClass::symbolLastTickArrayPrimary.GetStartPosition();
		while (posTick != NULL)
		{
			CStaticClass::st_Tick m_st_Tick_tmp = {};
			CString strSymbolTmp = L"";
			CStaticClass::symbolLastTickArrayPrimary.GetNextAssoc(posTick, strSymbolTmp, m_st_Tick_tmp);
			symbolLastTickArray_Temp.SetAt(strSymbolTmp, m_st_Tick_tmp);
		}
		CStaticClass::symbolLastTickArrayPrimary.RemoveAll();
		CStaticClass::m_mutex_Tick_copy.Unlock();


		POSITION posTick_process = symbolLastTickArray_Temp.GetStartPosition();
		while (posTick_process != NULL)
		{
			CStaticClass::st_TickBidAskLast old_tick = {};
			CStaticClass::st_Tick m_st_Tick_pro = {};
			CString m_symbol = L"";
			symbolLastTickArray_Temp.GetNextAssoc(posTick_process, m_symbol, m_st_Tick_pro);

			CStaticClass::m_mutex_Tick.Lock();
			int tickAddressIndex = -1;
			CStaticClass::symbolLastTickArrayIndex.Lookup(m_symbol, tickAddressIndex);
			if (tickAddressIndex == -1)
			{
				CStaticClass::symbolLastTickArrayIndex.SetAt(m_symbol, CStaticClass::lastrateArrayMaxindex);
				tickAddressIndex = CStaticClass::lastrateArrayMaxindex;
				CStaticClass::lastrateArrayMaxindex = CStaticClass::lastrateArrayMaxindex + 1;
			}
			old_tick = *CStaticClass::lastrateArray[tickAddressIndex];

			CStaticClass::lastrateArray[tickAddressIndex]->m_bid = m_st_Tick_pro.m_bid;
			CStaticClass::lastrateArray[tickAddressIndex]->m_ask = m_st_Tick_pro.m_ask;

			CStaticClass::UpdatedsymbolLastTickArrayIndex.SetAt(m_symbol, tickAddressIndex);
			CStaticClass::m_mutex_Tick.Unlock();
			if (old_tick.m_bid != CStaticClass::lastrateArray[tickAddressIndex]->m_bid || old_tick.m_ask != CStaticClass::lastrateArray[tickAddressIndex]->m_ask)
			{
				CStaticClass::st_updatedTickSymbol m_tmpst = {};
				CMTStr::Copy(m_tmpst.m_symbol, m_symbol);
				CStaticClass::m_mutex_ClientList.Lock();
				//(L"T17");
				POSITION posClient = CStaticClass::m_ClientContext.GetStartPosition();
				while (posClient != NULL)
				{
					CString strclientkey = L"";
					CStaticClass::st_ClientContext m_stClient = {};
					CStaticClass::m_ClientContext.GetNextAssoc(posClient, strclientkey, m_stClient);

					int indexKey = -1;
					int getIndexValue = m_stClient.m_TickSymbolListForSymbolPosition.Total();
					for (int r = 0; r < getIndexValue; r++)
					{
						CStaticClass::st_updatedTickSymbol mTempTickST = {};
						mTempTickST = m_stClient.m_TickSymbolListForSymbolPosition[r];
						CString strExistingSymbol = mTempTickST.m_symbol;
						if (strExistingSymbol == m_symbol)
						{
							indexKey = r;
							break;
						}
					}

					if (indexKey < 0)
					{
						m_stClient.m_TickSymbolListForSymbolPosition.Add(&m_tmpst);
						m_stClient.m_updatedTickSymbolArray.Add(&m_tmpst);
					}
					else
					{
						m_stClient.m_TickSymbolListForSymbolPosition.Update(indexKey, &m_tmpst);
						m_stClient.m_updatedTickSymbolArray.Update(indexKey, &m_tmpst);
					}

					CStaticClass::m_ClientContext.SetAt(strclientkey, m_stClient);
				}
				CStaticClass::m_mutex_ClientList.Unlock();
				//(L"UT17");
			}
		}
		Sleep(1);
	}
}

//Data Calculatision On Tick Wise
void CStaticClass::DataCalNetPositionclientwiseFromTickData()
{
	while (startCalculatingNetpositionClientWise==1)
	{
		
		CStaticClass::m_mutex_Tick.Lock();
		POSITION posTick = CStaticClass::UpdatedsymbolLastTickArrayIndex.GetStartPosition ();
		int row_count=0;
		CString tmpstr=L"";
		CString strFinalJson=L"";
		while (posTick != NULL) 
		{
			CString  tickSymbol=L"";
			int arraykeyINdex=-1;
			CStaticClass::UpdatedsymbolLastTickArrayIndex.GetNextAssoc(posTick, tickSymbol,arraykeyINdex);
			loginarray m_loginarray ;
			CStaticClass::mapSymbolandLogin.Lookup(tickSymbol,m_loginarray);
			int rowsCount=m_loginarray.Total();
		
			for (int i=0;i<rowsCount;i++)
			{
				CString strlogin=m_loginarray[i];
				CString loginAndSymbolKey=L"";
				loginAndSymbolKey.Format(L"%s:%s",strlogin,tickSymbol);

				

				//////(loginAndSymbolKey);

				CStaticClass::st_netpositionClientWise  st_pac={};
				CStaticClass::mapNetPositionClientWise.Lookup(loginAndSymbolKey,st_pac);

				//Calculating Floating PL			

				double  previous_floatingPL=0;
				double  floatingPL=0;
				double  DiffFloatingPL=0;
				double  previous_Balance=0;
				double  Balance=0;
				double  clientnetamount=0;

				double  grossExposure=0;

				double  clientgrosstotal=0;
				double  clientbroktotal=0;
				double  clientnettotal=0;


				

				if (*st_pac.m_volume!=0)
				{
					previous_floatingPL=*st_pac.m_clientfloatingpl ;
					double Plpoint=0;
					
					if (*st_pac.m_volume<0)
					{
						Plpoint=*st_pac.m_average-(st_pac.m_lastrate->m_ask); 
					}
					else
					{
						Plpoint=st_pac.m_lastrate->m_bid-(*st_pac.m_average); 
					}
					floatingPL=Plpoint*abs((*st_pac.m_volume)) *(*st_pac.m_multi) ;

					*st_pac.m_clientfloatingpl=floatingPL;

					DiffFloatingPL=floatingPL-previous_floatingPL;
					previous_Balance=*st_pac.m_clientbalance;
					Balance=*st_pac.m_clientbalance;

					*st_pac.m_clientbalance=Balance;

					clientnetamount=Balance+floatingPL;

					*st_pac.m_clientnetamount=clientnetamount- *st_pac.m_clientBrokarage ;

					

					clientgrosstotal=*st_pac.m_clientgrosstotal+DiffFloatingPL;				
					clientnettotal=*st_pac.m_clientnettotal+DiffFloatingPL;

					
					
					

					grossExposure=*st_pac.m_average*(*st_pac.m_volume) *(*st_pac.m_multi);
					*st_pac.m_clientexposure=grossExposure;

					int ClientGrossTotalIndex=-1;				
					clientgrosstotalArrayIndex.Lookup(strlogin,ClientGrossTotalIndex);
					//End 
					*clientgrosstotalArray[ClientGrossTotalIndex]=clientgrosstotal;		

					
					

					//Getting ClientGrossTotal address
					int clientnettotalIndex=-1;				
					clientnettotalArrayIndex.Lookup(strlogin,clientnettotalIndex);
					//End 
					*clientnettotalArray[clientnettotalIndex]=clientnettotal;


					st_ClientTotalBalance st_balance={};
					st_balance.grossTotal=clientgrosstotal;
					st_balance.netTotal=clientnettotal;


					
		
					*st_pac.m_ClientGrossAmount=*st_pac.m_clientbalance+*st_pac.m_clientfloatingpl;
					*st_pac.m_SubBrokerGrossAmount=((*st_pac.m_ClientGrossAmount)*(*st_pac.m_subbrokerRatio))/100;
					*st_pac.m_BrokerGrossAmount=((*st_pac.m_ClientGrossAmount)*(*st_pac.m_brokerRatio))/100;
					*st_pac.m_CompanyGrossAmount=((*st_pac.m_ClientGrossAmount)*(*st_pac.m_companyRatio))/100;																	
					


					/**st_pac.m_clientexposure = *st_pac.m_average * (*st_pac.m_volume) * (*st_pac.m_multi);
					*st_pac.m_subbrokerexposure = ((*st_pac.m_clientexposure) * (*st_pac.m_subbrokerRatio)) / 100;
					*st_pac.m_brokerexposure = ((*st_pac.m_clientexposure) * (*st_pac.m_brokerRatio)) / 100;
					*st_pac.m_companyexposure = ((*st_pac.m_clientexposure) * (*st_pac.m_companyRatio)) / 100;*/

					//*st_pac.m_clientfloatingpl		
					*st_pac.m_subbrokerfloatingpl = ((*st_pac.m_clientfloatingpl) * (*st_pac.m_subbrokerRatio)) / 100;
					*st_pac.m_brokerfloatingpl = ((*st_pac.m_clientfloatingpl) * (*st_pac.m_brokerRatio)) / 100;
					*st_pac.m_companyfloatingpl = ((*st_pac.m_clientfloatingpl) * (*st_pac.m_companyRatio)) / 100;


					//*st_pac.m_clientbalance;
					*st_pac.m_subbrokerbalance = ((*st_pac.m_clientbalance) * (*st_pac.m_subbrokerRatio)) / 100;
					*st_pac.m_brokerbalance = ((*st_pac.m_clientbalance) * (*st_pac.m_brokerRatio)) / 100;
					*st_pac.m_companybalance = ((*st_pac.m_clientbalance) * (*st_pac.m_companyRatio)) / 100;
					

					//CStaticClass::m_brokerageLoginSymbolWiseHastable										

					*st_pac.m_subbrokerNetAmount = *st_pac.m_SubBrokerGrossAmount - *st_pac.m_subbrokerBrokarage;
					*st_pac.m_brokerNetAmount = *st_pac.m_BrokerGrossAmount - *st_pac.m_brokerBrokarage;
					*st_pac.m_companyNetAmount = *st_pac.m_CompanyGrossAmount - *st_pac.m_companyBrokarage;

					//CStaticClass::mapBalanceClientWiseForSending.SetAt(strlogin,st_balance);
					CStaticClass::mapNetPositionClientWise.SetAt(loginAndSymbolKey,st_pac);			
					//CStaticClass::mapNetPositionClientWiseForSending.SetAt(loginAndSymbolKey,st_pac);					
				}
				//End of Calculating Floating PL					
			}
		}			
		CStaticClass::UpdatedsymbolLastTickArrayIndex.RemoveAll();
		CStaticClass::m_mutex_Tick.Unlock();
		//////(L"End Calculation");
		Sleep(1);
	}
}

void CStaticClass::UpdateOrderStatus(CString m_Symbol,double low,double heigh)
{
	CString strLog = L"";
	strLog.Format(L"Going To Update Order Status For Symbol %s Current Heigh:%.2lf Low:%.2lf", m_Symbol, heigh,low);
	//(strLog);
	POSITION pos = CStaticClass::m_Orika_orderHastable.GetStartPosition();
	StringBuffer s;
	Writer<StringBuffer> writer(s);	
	writer.StartObject();
	writer.Key("type");
	writer.String("ORDER_DATA");
	writer.Key("updatekey");
	writer.StartArray();
	writer.String("order");
	writer.EndArray();
	writer.Key("update");
	writer.StartArray();	
	while (pos != NULL)
	{
		CStaticClass::st_order st = {};
		int m_order = 0;
		CStaticClass::m_Orika_orderHastable.GetNextAssoc(pos,m_order, st);
		CString m_orderSymbol = st.m_symbol;
		int orderState = st.m_orderstate;
		int orderType = st.m_type;
		double orderPrice = st.m_price;
		int m_orderno = st.m_order;
		CString m_OrderStatus = L"";
		
		if (m_Symbol == m_orderSymbol && (orderState == 1001 || orderState == 1002) && low != 0 && (orderType == 2 || orderType == 5 || orderType == 6))
		{
			if (orderPrice >= low)
			{
				m_OrderStatus = L"PBNPS";
				//CStaticClass m_staticClass;
				CString strCurrentDate = getcurrentTimeStr();
				m_OrderStatus.Format(L"%s %.2lf %s", L"PBNPS", low, strCurrentDate);
				CMTStr::Copy(st.m_status , m_OrderStatus);		


				strLog.Format(L"Order %d Found For Change Status PBNPS Order Price:%.4lf low:%.4lf", m_orderno, orderPrice, low);
				//(strLog);
				CStaticClass::m_mtmanager.UpdateOrderINMT(m_order, L"ExternalID", m_OrderStatus);
				strLog.Format(L"Status Update");
				//(strLog);
			}
			CStaticClass::m_Orika_orderHastable.SetAt(m_order, st);
			writer.StartObject();
			writer.Key("order");
			writer.Int64(m_order);			
			string strstatustype = string(CT2CA(m_OrderStatus));
			const char* ststatustype = strstatustype.c_str();
			writer.Key("statustype");
			writer.String(ststatustype);			
			writer.EndObject();
			//(L"Test02");
		}
		if (m_Symbol == m_orderSymbol && (orderState == 1001 || orderState == 1002) && heigh != 0 && (orderType == 3 || orderType == 4 || orderType == 7))
		{
			//(L"Test03");
			if (orderPrice <= heigh)
			{
				m_OrderStatus = L"PBNPS";
				CStaticClass m_staticClass;
				CString strCurrentDate = m_staticClass.getcurrentTimeStr();
				m_OrderStatus.Format(L"%s %.2lf %s", L"PBNPS", heigh, strCurrentDate);
				CMTStr::Copy(st.m_status, m_OrderStatus);

				strLog.Format(L"Order %d Found For Change Status PBNPS Order Price:%.4lf Heigh:%.4lf", m_orderno, orderPrice, heigh);
				//(strLog);
				CStaticClass::m_mtmanager.UpdateOrderINMT(m_order,L"ExternalID", m_OrderStatus);
				strLog.Format(L"Status Update");
				//(strLog);
			}
			CStaticClass::m_Orika_orderHastable.SetAt(m_order, st);


			CString m_login = st.m_login;
			int m_time = st.m_time- 19800;

			CString m_strTime = L"";
			CString m_tmp_date = L"";
			CMTStr256 str_time;
			SMTFormat::FormatDateTime(str_time, m_time, true, true);
			m_strTime = str_time.Str();


			int m_deal = st.m_deal;
			int m_order = st.m_order;
			CString m_symbol = st.m_symbol;
			int m_type = st.m_type;
			CString strtype = L"";
			if (m_type == 2)
			{
				strtype = L"Buy Limit";
			}
			else if (m_type == 3)
			{
				strtype = L"Sell Limit";
			}
			else if (m_type == 4)
			{
				strtype = L"Buy Stop";
			}
			else if (m_type == 5)
			{
				strtype = L"Sell Stop";
			}
			else if (m_type == 6)
			{
				strtype = L"Buy Stop";
			}
			else if (m_type == 7)
			{
				strtype = L"Sell Stop Limit";
			}
			double m_volume = st.m_volume;
			double m_price = st.m_price;
			CString m_comment = st.m_comment;
			CString m_status = st.m_status;
			int m_select = st.m_select;
			CString strselect = L"";
			if (m_select == 0)
			{
				strselect = "false";
			}
			else
			{
				strselect = "true";
			}

			CString m_selecttype = st.m_selecttype;
			CString m_subtype = st.m_subtype;
			int m_contraorder = st.m_contraorder;
			int m_tradeexecutetime = st.m_tradeexecutetime;
			CString m_ourcomment = st.m_ourcomment;
			int m_orderstate = st.m_orderstate;

			CString strtmpdata = L"";


			CString str_orderstate = L"";
			if (m_orderstate == 1001)
			{
				str_orderstate = L"NEW";
			}
			if (m_orderstate == 1002)
			{
				str_orderstate = L"UPDATE";
			}
			if (m_orderstate == 1003 || m_orderstate == 1005)
			{
				str_orderstate = L"DELETE";
			}

			writer.StartObject();
			/*writer.Key("order");
			writer.Int64(m_order);
			string strstatustype = string(CT2CA(m_OrderStatus));
			const char* ststatustype = strstatustype.c_str();
			writer.Key("statustype");
			writer.String(ststatustype);*/			
				writer.StartObject();
				writer.Key("login");
				string sslogin = string(CT2CA(m_login));
				const char* stlogin = sslogin.c_str();
				writer.String(stlogin);
				writer.Key("time");
				/*string ssTime = string(CT2CA(m_strTime));
				const char* stTime = ssTime.c_str();*/
				writer.Int(m_time);
				writer.Key("deal");
				writer.Int64(m_deal);
				writer.Key("order");
				writer.Int64(m_order);
				writer.Key("symbol");
				string ssSymbol = string(CT2CA(m_symbol));
				const char* stSymbol = ssSymbol.c_str();
				writer.String(stSymbol);
				writer.Key("type");
				string sstype = string(CT2CA(strtype));
				const char* sttype = sstype.c_str();
				writer.String(sttype);
				writer.Key("volume");
				writer.Double(m_volume);
				writer.Key("price");
				writer.Double(m_price);
				writer.Key("comment");
				string sscomment = string(CT2CA(m_comment));
				const char* stcomment = sscomment.c_str();
				writer.String(stcomment);
				writer.Key("status");
				string ssstatus = string(CT2CA(m_status));
				const char* ststatus = ssstatus.c_str();
				writer.String(ststatus);
				writer.Key("select");
				string ssselect = string(CT2CA(strselect));
				const char* stselect = ssselect.c_str();
				writer.String(stselect);
				writer.Key("statustype");
				string ssselecttype = string(CT2CA(m_selecttype));
				const char* stselecttype = ssselecttype.c_str();
				writer.String(stselecttype);				
				writer.Key("subtype");
				string sssubtype = string(CT2CA(m_subtype));
				const char* stsubtype = sssubtype.c_str();
				writer.String(stsubtype);
				writer.Key("contraorder");
				writer.Int(m_contraorder);
				writer.Key("tradeexecutetime");
				writer.Int(m_tradeexecutetime);
				writer.Key("ourcomment");
				string ssourcomment = string(CT2CA(m_ourcomment));
				const char* stourcomment = ssourcomment.c_str();
				writer.String(stourcomment);
				writer.Key("orderstate");
				string ssorderstate = string(CT2CA(str_orderstate));
				const char* storderstate = ssorderstate.c_str();
				writer.String(storderstate);
				writer.EndObject();			
			writer.EndObject();			
		}
	}	
	writer.EndArray();
	writer.EndObject();
	CString strData = L"";
	strData = s.GetString();
	string strforsend = "";
	strforsend = CT2A(strData.GetString());
	//(L"Test05");
	sendDataToAllClient(strforsend);
	//(L"Test06");
	s.Clear();
	s.Flush();
}