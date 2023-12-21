
#pragma once
#define API_REQUEST (WM_USER+1)
#define API_DISCONNECT 0xffff
#ifndef _ATA_CMANAGER_
#define _ATA_CMANAGER_



class CManager :public IMTTickSink ,public IMTDealSink,public IMTOrderSink,public IMTUserSink,public IMTConSymbolSink,public IMTConGroupSink
  {
public:
  struct Tick_Data
  {
	   wchar_t           symbol[32];
	   INT64             datetime;                        	   
	   double            last;                            
  };



				   
private:
   CMTManagerAPIFactory		m_factory;     // Manager API factory   
   MTAPISTR					m_server;         // server address   
   UINT64					m_login;          // dealer login
   MTAPISTR					m_password;       // dealer password   
public:					
	IMTManagerAPI    *m_manager;        // Manager interface
	IMTAdminAPI      *m_admin;
	//CMutex           m_MutexForMTop;
                     CManager();
                    ~CManager(void);

					struct st_IpPriotity
					{
						wchar_t		m_symbolgroup[32];
						wchar_t 	m_lpname[32];
						wchar_t 	m_lpsymbol[32];
						int			m_lppriority;
						double		m_lpmaxlots;
						wchar_t		m_lpsendingtype[20];
						double 		m_lpratio;
					};

					 TMTArray<st_IpPriotity> m_IpPriotityArray;
					

	static int dealSendToMSMQStart;
	CString ClosingFileTransfer(CString StrFilePath, CString closingDate);
	CString ExcelClosingfileTransfer(CString StrFilePath);
	CString  CSVClosingfileTransfer(CString StrFilePath,CString closingDate);

	//CString GetGATEWAY_POSITIONDEVIDERATIO();


	void UpdateClientMasterFromMuser_Manual(const IMTUser* m_user);
	void UpdateClientMaster_Manual();

	void LoadConfigFile(CString StrFilePath);

	int CheckGroup(CString strgroupjson, CString strgroup);
	void DeleteMTData(CString strgroupjson);
	int BalanceTransferdataCount(string strgroupjason);
	void BalanceTransfer(int TotalClientCount, string strgroupjason);
	bool DealerBalance(const UINT64 login, const double amount, const UINT type, const LPCWSTR comment, bool deposit);
	void PositionTransferToMT(string strgroupjson, CString filepath,int totalposition);


	CString GetsymbolMappingData();

   //--- initialization 

	void  UpdatePendingSaudaToMSMQ();
	void  UpdatePendingOrder();
	struct dealandMSMQString
	{
		int dealno;
		wchar_t msmqstring[256];
	};
	TMTArray<dealandMSMQString>m_PendingDealStringArray;


	void			 SendOrderCountUpdate(CString m_login, CString  m_symbol);
   bool              Initialize();    
   void              Shutdown(void);
   int				 login(CString server,CString login,CString password);   
   void				 SymbolSubscribeForTick(CString symbol);
     
   virtual void      OnTick(LPCWSTR symbol, const MTTickShort& tick);
   virtual void		 OnDealAdd(const IMTDeal* deal);

   
   virtual void		 OnDealDelete(const IMTDeal* deal);

   CString			 CreateFETCH_PAGE_LIST(CString loginUser);

   void				 FetchHistoricalDealingData(UINT64 datefrom, UINT64 dateto,CString clientkey);
   CString			 FetchHistoricalDealingFromMemory(UINT64 datefrom, UINT64 dateto,CString clientkey,CString subscriptionId);

   CString			FETCH_COLOR_THEMES(CString loginUser);

   CString			FETCH_COLOR_THEME(CString id);

   CString			createFETCH_COLOR_TEMPLATE(CString loginUser);

   CString			CreateFETCH_PAGE_DETAILS(CString loginUser, CString pageId);
   void				deletePage(CString pageId);

   CString			CreateFETCH_Theme(CString loginUser);

   CString			GetGATEWAY_PERFECTANDROUNDED();
   CString			GetGATEWAY_PERFECTANDROUNDED_API();
   CString			GetGATEWAY_POSITIONDEVIDERATIO_UPDATE();

   CString			GetCanvasData(CString loginUser, CString tabtype);

   void				UpdateOrderINMT(int orderno, CString field, CString txtData);
   void				UpdateOrderHistoryINMT(int orderno, CString field, CString txtData);

   virtual void		 OnOrderAdd(const IMTOrder* order);

   void				 UpdateClientMasterFromMuser(const IMTUser* m_user);

   virtual void		 OnUserAdd(const IMTUser* user);
   virtual void		 OnUserUpdate(const IMTUser* user);

   virtual void		 OnOrderUpdate(const IMTOrder* order);
   virtual void		 OnOrderDelete(const IMTOrder* order);
   virtual void		 OnDealUpdate(const IMTDeal* deal);
   virtual void      OnHistoryAdd(const IMTOrder* order);
   virtual void      OnSymbolUpdate(const IMTConSymbol* config);
   virtual void      OnGroupAdd(const IMTConGroup* config);
   void				UpdateSymbolSource();


   void  SaveClosingRate();
   CString getUpdatedClosingRate();
   string gethighLowMismatchDeals(CString dealDate);



   CString			symbolMapping(CString StrFilePath);
   CString          SymbolMappingfileTransfer(CString StrFilePath,  CString FileType);

   void				 UpdateClientMaster();
   void				 UpdateSymbolMaster();
   void				 UpdateSymbolMasterFromSymbol(const IMTConSymbol* symbol);
   void				 loadLastTickFromMT();
   int				 Change_Comment(CString OldComment,CString NewComment,UINT64 Dealno);
   int				 Activate_Order(UINT64 OrderNo);

   CString			GetDashboardData(CString loginUser);
   
   struct     TDHLogin
   {
	   int m_login;
	   double m_lot;
   };
   typedef TMTArray<TDHLogin> TDHLoginArray;
   CString  TDHTradeExecution(TDHLoginArray& m_BuyLoginAndLot, TDHLoginArray& m_SellLoginAndLot, CString m_buySymbol, double buySymbolBidRate, CString TDHBuyComment, CString m_sellSymbol, double sellSymbolBidRate, CString TDHSellComment);

   CString			TDHExecution(CString m_buySymbol, TDHLoginArray& m_BuyLoginAndLot, double m_buyRate, CString m_sellSymbol, TDHLoginArray& m_SellLoginAndLot, double m_sellRate, CString m_executionType);
   int				trade_transfer(UINT64 login, LPCWSTR symbol, UINT  action, UINT ordertype, double m_volume, double price, LPCWSTR comment, UINT64 expiration);
   int               updateMTUserData(CString m_ColumnKey, CString m_value, CString m_DataSeprator, int m_DataIndexForUpdate,  CString m_login, CString m_symbol);

   void				LoadMTClientMaster();


  };
#endif
