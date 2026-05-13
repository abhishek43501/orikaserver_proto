#ifndef _CStaticClass_
#define _CStaticClass_
#include <atldbcli.h>
#include "Manager.h"
#include "SQLDATA\SqlData.h"
#include "LogWriter\LogFile.h"
#include "server\openssl_iocp.h"
#define SUBSCRIBE_POSITION_FOR_TRANSFER    101

class CStaticClass 
  {
public:
	static int      m_dealing_ID;
	static CString  SqlServerAdd;
	static CString  SqlServerDatabase;
	static CString  SqlServerUserID;
	static CString  SqlServerPassword;
	static CString  MTServerAdd;
	static CString  MTServerUserID;
	static CString  MTServerPassword;
	static CString  MSMQQueuName;
	static CString  orikaPort;
	static LPCSTR  dataFeedServer;
	static LPCSTR  GatewayServer;
	static int ExchangeCode;
	static CString  APIFolderPath;
	static int		APISERVER_PORT;
	CStaticClass();
	~CStaticClass();
	static int loginvalidate;
	static CManager m_mtmanager;
	static std::string  strTick;
	static int portno;
	static bool terminated ;
	static CMutex m_mutex;


	static CMutex m_SqlLock;
	//Alert Setting Variables
	struct st_Alert_Condisition
	{
		wchar_t  m_compareCondition[200];
		wchar_t  m_conditionType[200];
		wchar_t  m_conditionValue[30];
		wchar_t  m_orcondisition[500];
	};






	struct st_actions
	{
		wchar_t   m_actionName[100];
		wchar_t   m_action[200];
		wchar_t   m_actionSendBy[200];
		wchar_t   m_actionSendTo[200];
		wchar_t   m_actionTrigger[200];
	};
	struct st_Alert_Setting
	{
		TMTArray < st_Alert_Condisition> m_condisition;
		wchar_t  m_alertName[50];
		wchar_t  m_triggerType[50];
		wchar_t  m_startTime[30];
		wchar_t  m_expiryTime[30];
		TMTArray < int> m_daysOfMonth;
		TMTArray < wchar_t[20]> m_daysOfWeek;
		TMTArray < wchar_t[20]> m_selectedMonths;
		int		m_repetitions;
		int     m_days;
		int     m_hours;
		int     m_minutes;
		TMTArray < st_actions> m_actions;
	};
	static CMap<CString, LPCTSTR, st_Alert_Setting, st_Alert_Setting> m_AlertSettingMap;
	//End of Alert Setting Variables

	static CMutex m_mutexcommoditygroup;


	static CMutex m_mutexcolumnSubs;

	static CMutex m_positiondevideRatio_lock;

	
	static CMap<CString, LPCTSTR, double, double> m_ClientSymbolPosition_MT;
	static CMap<CString, LPCTSTR, double, double> m_ClientSymbolPosition_orika;

	static int startOrderData;

	static int startTickData;
	static int heartBeatStart;

	static CMutex m_mutex_Tick;
	static CMutex m_mutex_Tick_TickSubscribe;
	static CMutex m_mutex_Tick_copy;


	static CMutex m_mutex_ClientList;
	
	static CMutex m_mutex_dealingClientList;

	static CMutex m_mutex_order;
	static CMutex m_mutex_deal;

	static CMutex m_mutex_oSqlSession;


	static CMutex m_mutex_TotalBrokerage;


	static CMutex m_mutex_LimitConfig;


	static CDataSource connection;
	static CDataSource conntmp;
	//static CSession session;

	static CSqlData m_sqldata;
	static CLogFile m_logfile;

	
	
	void initializeconnection();
	struct st_Tick
	{
		wchar_t  m_symbol[32];
		double   m_bid;
		double   m_ask;
		double   m_last;
		INT64    m_datetime;
	};


	struct st_TotalTradedLotAndTOT
	{		
		double m_TotalTradedLot;
		double m_TotalTradedTO;		
		double m_TotalBuyLot;
		double m_TotalSellLot;
		double m_TotalButTO;
		double m_TotalSellTO;
	};
	static  CMap<CString, LPCTSTR, st_TotalTradedLotAndTOT, st_TotalTradedLotAndTOT> m_TotalLotAndTOT;



	





	struct st_TradesHighLowPriceSymbolWise
	{
		wchar_t  m_symbol[32];
		double m_High;
		double m_Low;		
	};
	static  CMap<CString, LPCTSTR, st_TradesHighLowPriceSymbolWise, st_TradesHighLowPriceSymbolWise> m_TradesHighLowPriceSymbolWise;
	static CMutex TradesHighLowLockUpdate;


	static  CMap<CString, LPCTSTR, double, double> m_LpRatioCommodityGroupWiseMap;


	struct st_LoginSymbolWiseLastTrade
	{
		wchar_t m_login[20];
		wchar_t m_symbol[32];
		int     m_volume;
		double  m_price;
		int     m_position;		
	};
	static  CMap<CString, LPCTSTR, st_LoginSymbolWiseLastTrade, st_LoginSymbolWiseLastTrade> m_LoginSymbolWiseLastTrade;

	
	struct st_BadTradeGoodTrade
	{
		wchar_t m_login[20];
		wchar_t m_symbol[32];
		int     m_TotalTrade;
		int		m_BadTrade;
		int     m_GoodTrade;
		int		m_badTradeIgnoringPosition;
		int		m_TradeIgnoringPosition;
	};
	static  CMap<CString, LPCTSTR, st_BadTradeGoodTrade, st_BadTradeGoodTrade> m_BadTradeGoodTrade;

	struct st_TickBidAskLast
	{		
		double   m_bid;
		double   m_ask;
		double   m_last;	
	};

	static int lastrateArrayMaxindex;
	static int clientgrosstotalArrayMaxindex;
	static int clientbroktotalArrayMaxindex;
	static int clientnettotalArrayMaxindex;

	static  st_TickBidAskLast*           lastrateArray[35000];
	static  CMap<CString,LPCTSTR,int ,int> symbolLastTickArrayIndex;
	static  CMap<CString,LPCTSTR,int ,int> UpdatedsymbolLastTickArrayIndex;
	static  CMap<CString,LPCTSTR,int ,int> UpdatedsymbolLastTickArrayIndexForSymbolWisePosition;


	static CMap<CString, LPCTSTR, st_Tick, st_Tick> symbolLastTickArrayPrimary;
	static UINT64 LastTickSendTime;
	

	static  double*   clientgrosstotalArray[6000]; 	 
	static  CMap<CString,LPCTSTR,int,int> clientgrosstotalArrayIndex;


	static int LpVolumeTotalArrayMaxIndex ;

	static double* LpVolumeTotal[6000];
	static CMap<CString, LPCTSTR, int, int> LpVolumeTotalArrayIndex;



	static  CMap<int, int, CString, CString > MessageCodeList;


	
	static CString m_ClientPositionRequestType;



	static CString m_DashboardRequest;


	static  CString m_ClientPositionRequestTimerWiseType;

	static  double*    clientbroktotalArray[6000]; 
	static  CMap<CString,LPCTSTR,int,int> clientbroktotalArrayIndex;

	double getClientBalance(CString strLogin,CString strSymbol);



	static  double*     clientnettotalArray[6000];
	static  CMap<CString,LPCTSTR,int,int> clientnettotalArrayIndex;
	

	static CMap<CString, LPCTSTR, double, double> m_comTotalLotsAfterMultiGroupWise;
	static  double* CompanyVolumeAfterMultiTotalArray[6000];
	static  CMap<CString, LPCTSTR, int, int> CompanyVolumeAfterMultiTotalArrayIndex;
	static int CompanyVolumeAfterMultiTotalArrayMaxIndex;


	struct st_order
	{
		wchar_t m_login[20];
		int m_time ;
		int m_deal ;
		int m_order;
		wchar_t m_symbol[32];
		int m_type ;
		double m_volume ;
		double m_price ;
		wchar_t m_comment[50];
		wchar_t m_status[30];
		int m_select ;
		wchar_t m_selecttype[15];
		wchar_t m_subtype[15];
		int m_contraorder ;
		int m_tradeexecutetime ;
		wchar_t m_ourcomment[50];
		int m_orderstate ;
	};

	typedef CMap<int,int,st_order,st_order> Orika_orderHastable;
	static Orika_orderHastable m_Orika_orderHastable;

	static CMap<int,int,int,int> m_Orika_dealNO;

	static CMap<int, int, int, int> m_OrikaOrderdealNO;

	struct st_LoginWiseGrossTotal
	{
		wchar_t m_login[20];
		double m_grossTotal;
		double m_brokerageTotal;
	};


	struct st_logindevice
	{
		int     m_time;
		wchar_t m_ip[20];
		wchar_t m_login[20];
		wchar_t m_deviceID[300];		
	};
	typedef TMTArray <st_logindevice> logindeviceArray;
	static  logindeviceArray m_logindeviceArray;


	struct st_order_ForUpdate
	{				
		INT64  m_order;					
		int m_select ;
		int m_select_update ;
		wchar_t m_selecttype[15];
		int m_selecttype_update;
		wchar_t m_subtype[15];
		int m_subtype_update;
		INT64  m_contraorder ;
		int m_contraorder_updtae;
		INT64  m_tradeexecutetime ;
		int m_tradeexecutetime_update;
		wchar_t m_ourcomment[50];		
		int m_ourcomment_update;
	};



	struct  st_Orika_Dealing
	{
	wchar_t  m_logTime[30];
	wchar_t  m_time[30];
	wchar_t  m_login[30];
	double deal;
	double order;
	wchar_t  m_orderType[30];
	wchar_t  m_symbol[32];
	wchar_t  m_type[30];
	double volume;
	double price;
	wchar_t  m_comment[30];
	wchar_t  m_entryType[30];
	wchar_t  m_currentStatus[30];
	};





	void initializePointerArray();
	static int startSendingNetpositionClientWise;
	static int startCalculatingNetpositionClientWise;

	//void sendClientWiseNetPositionToAllClient();
	struct st_netpositionClientWise
	{		
		wchar_t    m_login[20];
		wchar_t    m_name[40];
		wchar_t    m_symbol[40];
		
		wchar_t    m_subbroker[20];
		wchar_t    m_broker[20];

		wchar_t    m_company[30];

		double	   m_previousvolume; 
		double	   m_difference;



		double*    m_average;
		st_TickBidAskLast*    m_lastrate;
		double*    m_maxallotedqty;			
		double*    m_extravolume;
		double*    m_freemargin;
		double*    m_multi;
		

		double*	m_subbrokerRatio;		
		double*	m_brokerRatio;
		double*	m_companyRatio;


		double* m_clientexposure;
		double* m_subbrokerexposure;
		double* m_brokerexposure;
		double* m_companyexposure;

		double* m_volume;
		double* m_subbrokervolume;
		double* m_brokervolume;
		double* m_companyvolume;


		double*	m_ClientGrossAmount;
		double*	m_SubBrokerGrossAmount;
		double*	m_BrokerGrossAmount;
		double*	m_CompanyGrossAmount;

		double*	m_brokerageType;

		double*	m_clientBrokRate;
		double*	m_subBrokerBrokRate;
		double*	m_brokerBrokRate;
		double*	m_companyBrokRate;		


		double* m_clientfloatingpl;
		double* m_subbrokerfloatingpl;
		double* m_brokerfloatingpl;
		double* m_companyfloatingpl;

		double* m_clientbalance;	
		double* m_subbrokerbalance;
		double* m_brokerbalance;
		double* m_companybalance;


		double* m_clientgross;
		double* m_subbrokergross;
		double* m_brokergross;
		double* m_companygross;

		
		


		double* m_clientBrokarage;
		double* m_subbrokerBrokarage;
		double* m_brokerBrokarage;
		double* m_companyBrokarage;


		double* m_clientnetamount;
		double* m_subbrokerNetAmount;
		double* m_brokerNetAmount;
		double* m_companyNetAmount;
						

		double* m_clientgrosstotal;
				
		double* m_clientbroktotal;
		

		double* m_clientnettotal;
		
		double* m_creditLimit;


		

		wchar_t    m_exchange[32];
		wchar_t    m_international[32];
		wchar_t    m_sector[32];
		wchar_t    m_industry[32];
		double     m_rmp;
		
		wchar_t    m_page[32];
		wchar_t    m_categary[32];

		double    m_rm;


		double     m_QtyMulti;
		double    m_qtyAfterMulti;

		double	 m_LpRatio;
		double	 m_LpVolume;
		

		double* m_LpVolumeTotal;
		double* m_CompanyVolumeTotalAfterMulti;
		
		wchar_t   m_buySell[10];
		wchar_t   m_debitCredit[10];

		wchar_t  m_currencybase[32];
		wchar_t  m_commoditygroup[32];
	};

	void loadClientwisenetpositionData();
	void DataCalNetPositionclientwiseFromTickData();
	void sendlogClient(const IMTOrder* m_order, int dealSate, CString clientkey);
	static  CMap<CString,LPCTSTR,st_netpositionClientWise,st_netpositionClientWise&> mapNetPositionClientWise;


	struct st_ClientTotalBalance
	{
		double grossTotal;
		double netTotal;
	};

	
	typedef TMTArray <wchar_t[30]> loginarray;
	static  CMap<CString,LPCTSTR,loginarray,loginarray&> mapSymbolandLogin;


	
	typedef CMap <CString, LPCTSTR,CString,CString > SymbolSourceArray;
	static SymbolSourceArray m_SymbolSourceArray;

	CString getcurrentTimeStr();
	UINT64 getcurrentTime_Unix();

	void set_protoandjsonmessage();

	struct st_symbolHighLowTimeWise
	{
		wchar_t   m_Symbol[32];
		double    m_Heigh;
		double    m_Low;
		double    m_PreviousHeigh;
		double    m_PreviousLow;
		INT64     m_LowTime;
		INT64     m_HeighTime;
	};
	typedef CMap <CString, LPCTSTR,st_symbolHighLowTimeWise, st_symbolHighLowTimeWise> SymbolHighLowTimeWiseArray;
	static SymbolHighLowTimeWiseArray m_SymbolHighLowTimeWiseArray;


	st_LoginWiseGrossTotal GettingGrossTotalfromDatabase(CString loginName);


	static  CMap<CString,LPCTSTR,int,int> updatedData;



	struct st_requesttype
	{
		wchar_t   m_requesttype[100];		
	};


	


	static CString  strKeyTransfer;

	static CMutex   mutex_keytranssfer;

	static CMutex m_mutexlogintoken;


	static CMutex m_mutexLog;



	typedef vector<CString> LoginVector;

	typedef vector<CString> TabVector;


	struct st_updatedTickSymbol
	{
		wchar_t   m_symbol[32];
		/*double    m_bid;
		double    m_ask;
		double    m_ltp;*/
	};

	typedef TMTArray<st_updatedTickSymbol> updatedTickSymbolArray;

	
	

	struct st_updatedSymbolLogin
	{
		wchar_t   m_login[32];
		wchar_t   m_symbol[32];
	};

	typedef TMTArray<st_updatedSymbolLogin> updatedSymbolLoginArray;



	static CMutex               m_mutex_
		;
	static CRITICAL_SECTION     m_cs_Thread;
	static CONDITION_VARIABLE   m_cv_Thread;

	int m_forlogprint;
struct st_Orika_dealtableAccounting
{	
	TCHAR  m_login[20];
	int    m_deal ;
	int    m_order ;
	int    m_time ;
	TCHAR  m_symbol[32];
	double  m_multiplyer ;
	int    m_action;
	int    m_volume ;
	double  m_price;

	double  m_clientBrokTotal ;
	double  m_subBrokerBrokTotal; 
	double  m_brokerBrokTotal ;
	double  m_extraGroupBrokTotal;
	double  m_comBalancebrokTotal;

	double  m_brokerVolume ;
	double  m_subBrokerVolume ;
	double  m_extraGroupVolume;
	double  m_companyVolume ;

	double  m_clientLots ;
	double  m_subBrokerLots ;
	double  m_brokerLots ;
	double  m_extraGroupLots ;
	double  m_companyLots;
	int    m_duplicate ;
	TCHAR  m_ExternalID[20];
	TCHAR  m_comment[50];
};
typedef CMap<CString ,LPCTSTR,st_Orika_dealtableAccounting,st_Orika_dealtableAccounting> Orika_dealtableAccountingHastable;
static Orika_dealtableAccountingHastable m_Orika_dealtableAccountingHastable;

static CMap<int, int, double, double> m_ClientBalance;
	struct st_commentChange
	{
		INT64		deal;
		wchar_t		comment[30];
		wchar_t		commentTo[30];
	};
	

	struct stPrevolumeForBroker
	{
		wchar_t m_broker[30];
		wchar_t m_symbol[32];
		double m_volume;
	};

	struct stPrevolumeClientWise
	{
		wchar_t m_login[30];
		wchar_t m_symbol[32];
		double m_volume;
	};

	CString sysOnlyDateToStringFormat(SYSTEMTIME st);

	
	struct st_ClientContext
	{
		wchar_t									m_userlogin[30];
		wchar_t									m_LastRequestType[200];
		int										m_alertYN;		
		int										m_activeClient;
		//For Start and Stop Calculation Thread
		int										m_startCalculationThread;
		int										m_TicksSendingStart;		
		int										m_heartBeatStart;
		//For Sending Updated Data
		int										m_DealsSendingStart;
		int										m_OrdersSendingStart;				
		int										m_SymbolPositionSendingStart;
		int										m_ClientWiseNetPositionStart_FirstTime;
		int										m_brokerSendingStart;
		int										m_subbrokerSendingStart;
		int										m_ClientWiseNetPositionStart;
		int										m_BALANCE_DISTRIBUTION_start;
		int										m_BROKERAGE_DISTRIBUTION_start;
		int										m_EXPOSURE_DISTRIBUTION_start;
		int										m_LOTS_DISTRIBUTION_start;
		int										m_VOLUME_DISTRIBUTION_start;
		int										m_TOTAL_PL_GE_start;
		int										m_FETCH_ORDER_DATA_start;
		int										m_FETCH_DEALING_DATA_start;
		int										m_FETCH_EXISTING_COMMENT_CHANGE_DATA_start;
		//For Sending First Time Data
		int										m_DealsSendingStart_FirstTime;
		int										m_OrdersSendingStart_FirstTime;				
		int										m_SymbolPositionSendingStart_FirstTime;
		int										m_brokerSendingStart_FirstTime;
		int										m_brokerSendingStart_FirstTime_Call;
		int										m_subbrokerSendingStart_FirstTime;
		int										m_subbrokerSendingStart_FirstTime_Call;
		
		int										m_BALANCE_DISTRIBUTION_start_FirstTime;
		int										m_BROKERAGE_DISTRIBUTION_start_FirstTime;
		int										m_EXPOSURE_DISTRIBUTION_start_FirstTime;
		int										m_LOTS_DISTRIBUTION_start_FirstTime;
		int										m_VOLUME_DISTRIBUTION_start_FirstTime;
		int										m_TOTAL_PL_GE_start_FirstTime;
		int										m_FETCH_ORDER_DATA_FirstTime;
		int										m_FETCH_EXISTING_COMMENT_CHANGE_DATA_FirstTime;
		int										m_getPreQtyUpdate;
		int										m_firstTimeDataSended;
		int										m_refreshClientPosition;
		int										m_sendDashboardData;
		int										m_calculatePreNetPosition;
		INT64									m_netPositionTime;
		INT64									m_BrokerPositionTime;
		INT64									m_SubBrokerPositionTime;
		INT64									m_DashboardTime;

		int										m_start_gateway_positiondevideratio;

		TMTArray<wchar_t[250]>				    m_clientrequests_List;


		int										m_fetch_client_position_timerwise;

		INT64									m_lastDealSended;
		INT64									m_lastOrderSended;
		INT64									m_ConnectedTime;
		INT64									m_lastDataSended;
		vector<CString>							m_logins;
		TabVector								m_activetabs;
		SSL_session*							m_clientConnection;
		int										m_loginValidate;
		updatedTickSymbolArray					m_updatedTickSymbolArray;	

		updatedTickSymbolArray				    m_TickSymbolListForSymbolPosition;

		updatedSymbolLoginArray					m_updatedVolumeSymbolArray;		
		TMTArray<st_order>						m_orderarray_update;
		TMTArray<st_order>						m_orderarray_insert;
		TMTArray<st_order>						m_orderarray_delete;

		TMTArray<stPrevolumeForBroker>			m_brokerPreqtyArray;
		TMTArray<stPrevolumeForBroker>			m_SubBrokerPreqtyArray;

		TMTArray<st_order_ForUpdate>			m_orderarray_ForEdit;

		TMTArray<st_Orika_dealtableAccounting>  m_dealarray;
		TMTArray<st_netpositionClientWise>		m_newNetPositionAdded;
		TMTArray<st_netpositionClientWise>		m_newNetPositionUpdated;
		TMTArray<st_order>						m_orderarray_dealing_update;
		TMTArray<st_order>						m_orderarray_dealing_insert;
		TMTArray<st_commentChange>				m_commentArray_insert;
		TMTArray<st_commentChange>				m_commentArray_update;
		TMTArray<INT64>							m_commentArray_delete;
		TMTArray<stPrevolumeClientWise>			m_preQtyLoginSymbolWise;
		INT64									m_netPositionTime_TIMERWISE;
		INT64									m_netPositionTime_TIMERWISE_LastTime;
		int                                     m_ClientType;		  
		vector<int>                             m_SubscribedRequest;
		int										m_primary_connection;
		wchar_t									m_logintoken[250];
		vector<CString>							m_Subscribed_Symbol;
		HANDLE									m_localThred;
		int										m_thread_terminate;
		wchar_t									m_ipaddress[30];
		wchar_t									m_fingerprint[100];
		TMTArray<wchar_t[250]>					m_subscriptionId;
		
	};
	typedef CMap<CString ,LPCTSTR,st_ClientContext,st_ClientContext&> ClientContext;
	static ClientContext m_ClientContext;

    struct st_token
	{
		TMTArray<wchar_t[250]>				    m_clientkey;
		int										m_status;
		UINT64									m_tokenactivetime;
		UINT64									m_lastdisconnecttime;
		wchar_t									m_login[32];
		wchar_t									m_hdno[250];
		int										m_totalconnection;
		wchar_t									m_ipaddress[16];
	};

	static CMap<CString, LPCTSTR, st_token, st_token&> m_tokenlist;

	static CMap < CString, LPCTSTR, double, double> m_mismatch_position;

	static int startComparePosition;
	void calculateClientWiseAllPosition();	  	
	typedef CMap<CString ,LPCTSTR,st_Tick,st_Tick> TickData;
	static TickData m_TickData;
	static TickData m_updatedTickData;
	static TickData  m_TickDataForContinousData;
	void	sendUpdatedTick();
	void    comparePosition();
	void    Re_comparePosition();
	UINT64  getPreviousDateTime_Unix();
	HRESULT hr;
	CCommand<CNoAccessor, CNoRowset> cmdExecute;
	struct stclientmaster
	{
		wchar_t  m_login[20];
		wchar_t  m_symbolGroup[20];
		wchar_t  m_name[20];
		wchar_t  m_broker[20];
		wchar_t  m_subBroker[20];
		wchar_t  m_extraGroup[20];
		double m_subBrokerPLRatio;
		double m_brokerPLRatio;
		double m_companyPLRatio;
		wchar_t  m_brokageType[9];
		double m_clientBrokage;
		double m_subBrokerBrokage;
		double m_bokerBrokage;
		double m_companyBrokage;
		double m_symbolWiseBuyLimit;
		double m_symbolWiseSellLimit;
		int m_symbolWisePendingOrderEnableDisable;
		int m_symbolPositionLimit;
		double m_symbolPendingOrderDiffFromBidAsk;
		double m_lossLimit;
		double m_creditLimit;
		int m_comment;
		double m_qtyLimitMultiplayer;
		int m_ignoreTrader;
		wchar_t  m_colour[20];
	};

	typedef TMTArray<stclientmaster> clientmasterarray;
	static clientmasterarray m_clientmasterarray;



	struct stsymbolmaster
	{
		wchar_t  m_symbol[32];
		double m_closingPrice;
		double m_multiplayer;
		double m_symbolMargin ;
		double m_maxlimitOnsameprice; 
		double m_symbolPendingOrderDiffFromBidAsk ; 
		wchar_t m_ltpMapping[20];
		double m_upperCircuit;
		double m_lowerCircuit;
		wchar_t m_exchangeSymboll[20];
		wchar_t  m_nextSymbol[32];
		int m_nselotsize;
		int m_mtlotsize;
		double m_maxAllowedVolume;
		double m_allowedLimitPerClient;
		double m_symbolPositionLimit;
		wchar_t m_symbolGroup[20];
		double m_binsize;
		double m_pocketnumber;
	};
	typedef TMTArray<stsymbolmaster> symbolmasterarray;
	static symbolmasterarray m_symbolmasterarray;


	static CMap<CString, LPCTSTR, CString, CString> m_symbolForTickData;

	static CMap<CString, LPCTSTR, CString, CString> m_protoandjsonmessage;

	struct stloginUserDetail
	{
		wchar_t  m_loginuser[32];
		wchar_t  m_name[32];
	};
	static CMap<CString, LPCTSTR, stloginUserDetail, stloginUserDetail> m_loginuserlist;

	typedef CMap<CString, LPCTSTR, CStaticClass::st_netpositionClientWise, CStaticClass::st_netpositionClientWise&>  NetPositionClientWise_All;

	void sendClientPosition_Insert(SSL_session* client, int m_activeClient,CString strUserID, CString m_clientcontexKey, CString m_Requestmessage,CString m_messageType,  NetPositionClientWise_All* mapNetPositionClientWise_ThreadWise);


	void sendClientPosition_Update(SSL_session* client, int m_activeClient, CString strUserID, CString m_clientcontexKey, CString m_Requestmessage, CString m_messageType, NetPositionClientWise_All* mapNetPositionClientWise_ThreadWise);


	void sendClientPosition_NewInsert(SSL_session* client, int m_activeClient, CString strUserID, CString m_clientcontexKey, CString m_Requestmessage, CString m_messageType, NetPositionClientWise_All* mapNetPositionClientWise_ThreadWise, TMTArray<st_netpositionClientWise>* tmpNewposition);
	void sendClientPosition_NewUpdate(SSL_session* client, int m_activeClient, CString strUserID, CString m_clientcontexKey, CString m_Requestmessage, CString m_messageType, NetPositionClientWise_All* mapNetPositionClientWise_ThreadWise, TMTArray<st_netpositionClientWise>* tmpNewposition);

	struct st_scripWiseNetPosition
	{
		wchar_t m_symbol[32];
		double  m_clientQty;
		double  m_clientAvg;
		double  m_clientGrossExposure;
		double  m_PL;
		double  m_companyQty;
		double  m_companyAvg;
		double  m_lastRate;
		double  m_companyBrokarage;
		double  m_companyPL;
		double  m_companyExposure;
		double  m_multiplayer;
	};
	typedef CMap<CString ,LPCTSTR,st_scripWiseNetPosition ,st_scripWiseNetPosition> MapscripWiseNetPosition;
	static MapscripWiseNetPosition m_MapscripWiseNetPosition;

	static  MapscripWiseNetPosition m_MapscripWiseNetPosition_Fix;


	struct st_brokerPosition
	{
		wchar_t m_brokername[30];
		wchar_t m_name[50];
		wchar_t m_symbol[32];
		double  m_volume;
		double  m_preVolume;
		double  m_volDiff;
		double  m_avg;
		double  m_last;
		double  m_clientgrossamount;
		double  m_clientnetamount;
		double  m_brokergrossamount;
		double  m_brokernetamount;
	};
	typedef CMap<CString ,LPCTSTR,st_brokerPosition,st_brokerPosition> MapbrokerPosition;
	static MapbrokerPosition m_MapbrokerPosition;
	static MapbrokerPosition m_MapbrokerPosition_Fix;


	
	


	struct st_subbrokerPosition
	{
		wchar_t m_subbrokername[30];
		wchar_t m_name[50];
		wchar_t m_symbol[32];
		double  m_volume;
		double  m_preVolume;
		double  m_volDiff;
		double  m_avg;
		double  m_last;
		double  m_clientgrossamount;
		double  m_clientnetamount;
		double  m_brokergrossamount;
		double  m_brokernetamount;
	};
	typedef CMap<CString ,LPCTSTR,st_subbrokerPosition,st_subbrokerPosition> MapsubbrokerPosition;
	static MapsubbrokerPosition m_MapsubbrokerPosition;
	static MapsubbrokerPosition m_MapsubbrokerPosition_Fix;


	
	
	
	
	typedef CMap<CString ,LPCTSTR,CString ,CString > requestResponseData;

	static requestResponseData m_requestResponseData;

	static requestResponseData  m_tablesname;

	typedef CMap<CString ,LPCTSTR, SSL_session* , SSL_session* > ClientList;
	static  ClientList m_ClientList;

	static  ClientList m_ClientList_forOrder;
	static  ClientList m_ClientList_forDeal;

	static int TokenCheckingStartStop;

	static CString m_ordertypedesc[8];
	static CString m_dealtypedesc[10];


	void sendNewTradeToClientFromDeal(const IMTDeal* m_deal, int dealSate, CString clientkey);

	static requestResponseData m_uniqueFiels;

	static int MSMQReaderStartStop;
	static int MSMQReaderPause;


	struct st_Orika_clientmaster
	{
		wchar_t  m_login[20];
		wchar_t  m_name[50];
		wchar_t  m_broker[20];
		wchar_t  m_subBroker[20];
		wchar_t  m_extraGroup[20];
		double m_lossLimit ;
		double m_creditLimit ;
		int   m_comment ;
		double m_qtyLimitMultiplayer; 
		int   m_ignoreTrader ;
		wchar_t  m_colour[20];
		wchar_t  m_company[30];
		double   m_lpRatio;
	};

	struct st_LpLots
	{
		double m_lots;
		double m_lots_To;
		double m_LpPer ;
		double m_stepup;
		double m_perfect;
		double m_rounded;
		double   m_perfectTotal;
		double   m_roundedTotal;
		double   m_LPVolume;
		double   m_companyVolumeAfterMultiTotal;
	};

	typedef TMTArray<st_LpLots> st_LpLotsArray;

	static CMap<CString,LPCTSTR, st_LpLotsArray, st_LpLotsArray&> m_LpLotsMap;

	static st_LpLotsArray m_LpQtyArray_gold;
	static st_LpLotsArray m_LpQtyArray_silver;



	static CMap<CString, LPCTSTR, double, double> m_LPLots;
	static CMap<CString, LPCTSTR, double, double> m_LPLots_perfect;


	struct st_LpDetails
	{
		wchar_t  m_lpname[50];
		wchar_t  m_lpsymbol[32]; 
		int		 m_lppriority;
		double   m_lpmaxlots; 
		wchar_t  m_lpsendingtype[20]; 
		double   m_lpratio;		
	};
	typedef TMTArray<st_LpDetails> st_LpDetailsArray;

	static CMap<CString, LPCTSTR, st_LpDetailsArray, st_LpDetailsArray&> m_LpDetailMap;
	static st_LpDetailsArray m_LPdetailArray_gold;
	static st_LpDetailsArray m_LPdetailArray_silver;

	struct st_Orika_MTclientmaster
	{
		wchar_t  m_login[20];
		wchar_t  m_name[50];
		wchar_t  m_comment[200];
		wchar_t  m_address[200];
		wchar_t  m_leadsource[200];
		wchar_t  m_leadcampaign[200];
		int		 m_language;
		wchar_t  m_group[200];		
	};
	typedef CMap<int, int, st_Orika_MTclientmaster, st_Orika_MTclientmaster> Orika_MTclientmasterHasTable;
	static Orika_MTclientmasterHasTable m_Orika_MTclientmasterHasTable;


	void sendDataToAllClient_LPData(string msg);
	void sendDataToAllAPIClient_LPData(string msg);

	CString  CalculateLPData(CString m_login,CString m_symbolGroup,double m_LpVolume, double m_CompanyVolume);


	typedef CMap<CString ,LPCTSTR,st_Orika_clientmaster ,st_Orika_clientmaster> Orika_clientmasterHastable;
	static Orika_clientmasterHastable m_Orika_clientmasterHastable;


	struct st_Orika_clientbrokerage
	{
		wchar_t m_login[20];
		wchar_t m_symbolGroup[20];
		int     m_brokageType;
		double   m_clientBrokage;
		double   m_subBrokerBrokage; 
		double   m_bokerBrokage;
		double   m_extraGroupBrokage;
		double   m_companyBrokage;
	};
	typedef CMap<CString ,LPCTSTR,st_Orika_clientbrokerage ,st_Orika_clientbrokerage> Orika_clientbrokerageHastable;
	static Orika_clientbrokerageHastable m_Orika_clientbrokerageHastable;



	struct st_Orika_PLDevideRatio
	{
		wchar_t m_login[20];
		wchar_t m_symbolGroup[20];
		double m_brokerPLRatio;
		double m_subBrokerPLRatio;
		double m_extragroupPLRatio;
		double m_companyPLRatio;
	};
	typedef CMap<CString ,LPCTSTR,st_Orika_PLDevideRatio ,st_Orika_PLDevideRatio> Orika_PLDevideRatioHastable;
	static Orika_PLDevideRatioHastable m_Orika_PLDevideRatioHastable;

	
	void sendHeartBeattoAllClient();

	void GettingTickDataFromManager();


	void UpdateOrderStatus(CString m_Symbol, double low, double heigh);

	string CStaticClass::getcurrentTime();

	struct st_Orika_symbolgroup
	{
		wchar_t   m_symbolGroup[20];
		int		  m_symbolWiseBuyLimit;
		int       m_symbolWiseSellLimit;
		int       m_symbolWisePendingOrderEnableDisable;
		int       m_symbolPositionLimit;
		double     m_symbolPendingOrderDiffFromBidAsk;
		double     m_symbolMargin;
		double     m_expectedchange;
		int       m_binsize;
		int       m_pocketnumber;
		double     m_maxlimitOnsameprice;
	};
	typedef CMap<CString ,LPCTSTR,st_Orika_symbolgroup,st_Orika_symbolgroup> Orika_symbolgroupHastable;
	static Orika_symbolgroupHastable m_Orika_symbolgroupHastable;


	struct st_Orika_Clientgatewayconfig
	{
		wchar_t  m_login[20];
		wchar_t  m_symbolGroup[20];
		int   m_symbolWiseBuyLimit;
		int   m_symbolWiseSellLimit;
		int   m_symbolWisePendingOrderEnableDisable;
		int   m_symbolPositionLimit;
		int   m_qtyLimitMultiplayer;
	};
	typedef CMap<CString ,LPCTSTR,st_Orika_Clientgatewayconfig,st_Orika_Clientgatewayconfig> Orika_ClientgatewayconfigHastable;
	static Orika_ClientgatewayconfigHastable m_Orika_ClientgatewayconfigHastable;
	
	

	struct stConfig
	{
		INT64   Login;
		CString Symbol;
		int     SaudaType;
		int     Limit;
		int		SymbolGroupCheck;
	};
	static CMap<CString, LPCTSTR, stConfig, stConfig> LimitConfig;




	struct st_Orika_symbolmaster
	{
		wchar_t  m_symbol[32];
		double    m_closingPrice ;
		double    m_multiplayer ;
		double    m_symbolMargin ;
		double    m_upperCircuit ;
		double    m_lowerCircuit ;
		wchar_t  m_ltpMapping[32];
		wchar_t  m_nextSymbol[32];
		wchar_t  m_exchangeSymbol[32];
		int      m_nselotsize ;
		int      m_mtlotsize  ;
		wchar_t  m_symbolGroup[20];
		wchar_t  m_ltpSymbol[32];

		wchar_t  m_exchange[32];
		wchar_t  m_international[32];
		wchar_t  m_sector[32];
		wchar_t  m_industry[32];
		double   m_rmp;
		double   m_QtyMulti;
		wchar_t  m_page[32];
		wchar_t  m_categary[32];
		wchar_t  m_currencybase[32];
		wchar_t  m_Group[32];
	};
	typedef CMap<CString ,LPCTSTR,st_Orika_symbolmaster,st_Orika_symbolmaster> Orika_symbolmasterHastable;
	static Orika_symbolmasterHastable m_Orika_symbolmasterHastable;
	


	struct st_Orika_ExchangeSymbol
	{
		wchar_t  m_exchangeSymbol[32];
		wchar_t  m_MTsymbol[32];
		wchar_t  m_NextMTsymbol[32];		
	};
	typedef CMap<CString, LPCTSTR, st_Orika_ExchangeSymbol, st_Orika_ExchangeSymbol> Orika_ExchangeSymbolHastable;
	static Orika_ExchangeSymbolHastable m_Orika_ExchangeSymbolHastable;



	static CMap<CString, LPCTSTR, double, double> m_allClosingprice;
	static CMap<CString, LPCTSTR, double, double> m_ignoreClosingprice;




struct st_orika_positionEntryAccounting
{
	int   m_deal;
	int   m_order;
	TCHAR m_login[20];
	TCHAR m_symbol[32];
	int   m_time ;
	int   m_entryAction;
	int   m_entryVolume;
	double m_entryPrice;
	double m_currentPrice;
	double m_highPrice;
	double m_lowPrice; 	
	double m_subBrokerEntryVolume;
	double m_brokerEntryVolume;
	double m_extraGroupEntryVolume; 
	double m_companyEntryVolume;
	double m_multiplyer;
};

typedef CMap<CString ,LPCTSTR,st_orika_positionEntryAccounting,st_orika_positionEntryAccounting> orika_positionEntryAccountingHastable;
static orika_positionEntryAccountingHastable m_orika_positionEntryAccountingHastable;


typedef TMTArray<st_orika_positionEntryAccounting> orika_positionEntryAccountingarrayClientSymbolWise;


//typedef CMap<CString ,LPCTSTR,orika_positionEntryAccountingarrayClientSymbolWise*,orika_positionEntryAccountingarrayClientSymbolWise*> orika_positionEntryAccountingHastableLoginSymbolWise;
typedef CMap<CString ,LPCTSTR,orika_positionEntryAccountingarrayClientSymbolWise,orika_positionEntryAccountingarrayClientSymbolWise&> orika_positionEntryAccountingHastableLoginSymbolWise;
static orika_positionEntryAccountingHastableLoginSymbolWise m_orika_positionEntryAccountingHastableLoginSymbolWise_Buy;
static orika_positionEntryAccountingHastableLoginSymbolWise m_orika_positionEntryAccountingHastableLoginSymbolWise_Sell;




struct  st_Orika_BalanceTableAccounting
{
	TCHAR m_login[20];
	TCHAR m_symbol[32];
	int   m_entryDeal;
	int   m_entryTime;
	int   m_entryOrder;	
	int   m_entryAction;
	int   m_entryVolume;
	double m_entryPrice;
	int   m_exitDeal;
	int   m_exitTime;
	int   m_exitOrder;
	int   m_exitAction;
	int   m_exitVolume;
	double m_exitPrice;
	double m_plPointLoss;
	double m_plPointProfit;
	double m_multiplyer;
	double m_clientBalance;
	double m_subBrokerBalance;
	double m_brokerBalance;
	double m_extraGroupBalance; 
	double m_companyBalance;
};

typedef CMap <CString,LPCTSTR ,st_Orika_BalanceTableAccounting,st_Orika_BalanceTableAccounting> Orika_BalanceTableAccountingArray;
static Orika_BalanceTableAccountingArray m_Orika_BalanceTableAccountingArray;




static  CMap <CString, LPCTSTR, int, int> m_columnsubscription;





struct st_orika_PositionAverageAccounting
{
	TCHAR  m_login[20];
	TCHAR  m_symbol[32];
	int    m_Action;
	int    m_Volume; 
	double  m_subBrokerVolume;
	double  m_brokerVolume;
	double  m_extraGroupVolume;
	double  m_companyVolume;
	double  m_WAvgPrice;	
	double  m_currentPrice;
	double  m_multiplyer;
	double  m_floatingProfit;
	double  m_subBrokerfloatingProfit;
	double  m_brokerfloatingProfit;
	double  m_extraGroupfloatingProfit;
	double  m_companyfloatingProfit;
};
typedef CMap<CString ,LPCTSTR,st_orika_PositionAverageAccounting,st_orika_PositionAverageAccounting> orika_PositionAverageAccountingHastable;
static orika_PositionAverageAccountingHastable m_orika_PositionAverageAccountingHastable;


struct st_orika_brokerageLoginSymbolWise
{
	TCHAR  m_login[20];
	TCHAR  m_symbol[32];
	double    m_clientBrokTotal;
	double    m_subBrokerBrokTotal; 
	double    m_brokerBrokTotal;
	double    m_comBrokTotal;
};
typedef CMap<CString ,LPCTSTR,st_orika_brokerageLoginSymbolWise,st_orika_brokerageLoginSymbolWise> brokerageLoginSymbolWiseHastable;
static brokerageLoginSymbolWiseHastable m_brokerageLoginSymbolWiseHastable;


struct st_OrderCount
{
	TCHAR  m_login[20];
	TCHAR  m_symbol[32];
	int		m_Type;
	int m_TotalOrder;
	int m_TotalCancelledOrder;
	int m_TotalExecutedOrder;
};
static CMap<CString, LPCTSTR, st_OrderCount, st_OrderCount> m_OrderCountMap;


void sendTickToAllClient(CString m_symbol, string msg);
void sendDataToAll_Other_Client(string msg, CString m_ignorekey);
void Sql_Updateorika_dealtableaccounting(st_Orika_dealtableAccounting  dt);
void Sql_Insertorika_positionEntryAccounting(st_orika_positionEntryAccounting dt);
void Sql_Updateorika_positionEntryAccounting(st_orika_positionEntryAccounting dt);
void Sql_Deleteorika_positionEntryAccounting(st_orika_positionEntryAccounting dt);
void Sql_UpdateOrika_BalanceTableAccounting(st_Orika_BalanceTableAccounting dt);
void Sql_Updateorika_PositionAverageAccounting(st_orika_PositionAverageAccounting dt);
void Updateorika_positionEntryAccountingHashtable(st_Orika_dealtableAccounting  dt);
void UpdateOrika_BalanceTableAccountingHashtable(st_orika_positionEntryAccounting inDeal,st_orika_positionEntryAccounting outDeal);
void Updateorika_PositionAverageAccountingHashtable(st_orika_PositionAverageAccounting m_pac);
void Updateorika_PositionAverageAccountingHashtableNewRecord(st_orika_positionEntryAccounting m_pe,int flag);
void Sql_UpdateOrika_order(st_order ot);
void sendNeworderToClient(st_order st);
void sendNewTradeToClient(const IMTDeal* st);
void addNewDealToDealData(st_Orika_dealtableAccounting st);
void sendorderdealingToClient(const IMTOrder* m_order, int dealSate,CString clientkey);
void sendDataToAllClient(string msg);
CString sysDateToStringFormat(SYSTEMTIME st);



};
#endif