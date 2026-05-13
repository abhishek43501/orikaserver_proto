#ifndef CSqlData_header
#define CSqlData_header
class CSqlData
{	
public:
	  CSqlData();
	  ~CSqlData();

	  void getClientmasterData();
	  CString generateClientmasterJson();
	  CString generateMasterMetadata(CString metadataRequest,CString strLoginUser);
	  CString CSqlData::gettingFilterStateJson(CString metadataRequest, CString strLoginUser, int viewSlNo);
	  CString CSqlData::gettingShortingColJson(CString metadataRequest, CString strLoginUser, int viewSlNo);
	  CString generateTabAndColumnMetadata();
	  CString generateMetadataGlobal(CString metadatatype);
	  CString GetOrika_brokerdata(CString m_broker);
	  CString GetOrika_subbrokerdata(CString m_subbroker);
	  CString generateJsonLoginData_Details(CString loginuser);

	  void Loadloginuser();

	  void getSymbolMasterData();

	  void getSymbolMasterDataForTickSubscribe();
	  CString generateSymbolMasterJson();
	

	  CString CSqlData::getAlertSetting(CString alertName);
	  void GetLoginsClient(CString strLoginuser, vector<CString>&	LA);

	  void loadEventSetting();

	  void  LoadLpRatioCommodityGroupwise();

	  typedef TMTArray<wchar_t [40]> columnArray;

	  static columnArray m_columnArray;

	  void loadMessageCodeDesc();

	  void getStringColumnList(CString m_commandtext, CSqlData::columnArray* m_colDataArray);

	  CString getClientwisenetpositionData(CString loginuser);

	  CString getSymbolWisePositionData(CString loginuser);
	  CString getSubBrokerWisePositionData(CString loginuser);

	  CString getBrokerWisePositionData(CString loginuser);
	  struct stViewDetails
	  {
		  int m_isDarkMode ;
		  int	m_isFilterOn ;
		  int	m_isColorOn;
		  int	m_isPivot;		  
	  };
	  stViewDetails getToggleViewDetails(CString metadataRequest, CString  strLoginUser, int viewSerialNo);
	  CString getBalanceDistripution(CString loginuser);


	  CString getCommentDealWise(CString deals);

	  int checkDealExist(INT64 deals);

	  int executeCommandwiterrorcode(CString strCommand);
	  void executeCommand(CString strCommand);

	  void    deleteColumnSubscriptionData(CString m_loginUser,CString m_requestType);
	  CString SaveColumnSubscriptionData(CString m_loginUser, CString m_columnKey, CString m_requestKey);
	  void getMappingSymbol(TMTArray<wchar_t[250]>& m_columns, CString m_loginuser, CString m_requestType);
	  

	  void checkColumnSubscription(TMTArray<wchar_t[200]> &m_mappedcolumns,  CMap<CString, LPCTSTR, int, int>& m_subscribedcolumn);


	  void loadColumnSubscription();
	  CString deleteDataCommentChange(CString deals);
	  CString columnTemplateSave(CString name, CString userLogin, CString colorData);
	  CString columnTemplateUpdate(int id, CString name, CString userLogin, CString colorData);
	  CString columnTemplateDelete(int id);
	  CString getColumnTempletUserLoginWise(CString userLogin);

	 /* CString get
	  (CString loginuser);*/

	  CString generateDashBoardMetadata(CString loginUser);
	  CString PositionFileReadAndCheck(CString filePath);

	  void loadDataOrika_dealsHighLow();
	  CString getBrokerposition(CString loginuser);
	  CString getSubBrokerposition(CString loginuser);
	  CString getBalanceDistribution(CString loginuser);
	  CString getVolumeDistribution(CString loginuser);
	  CString getLotDistribution(CString loginuser);
	  CString getExposureDistribution(CString loginuser);
	  CString getBrokerageDistribution(CString loginuser);
	  CString getClientwisePreQty(CString loginuser,int time);

	  void loadDataOrika_clientmaster();
	  CString GetOrika_clientmaster(CString m_login);
	  void loadDataOrika_clientbrokerage();
	  void loadOrika_PLDevideRatio();
	  void loadOrika_symbolgroup();
	  void loadOrika_Clientgatewayconfig();
	  void loadOrika_symbolmaster();

	  void loadOrderHashTable();
	  void loadDealNo();
	  void loadPositionAverageAccounting();

	  void loadClientBrokerageFromDealTableAccounting();
	  void loadTradeLotAndTOT();
	  void loadOrderCount();

	  void loadOrika_LoginSymbolWiseLastTrade();
	  void loadOrika_LoadGoodTradeBadTrade();

	  void loadBalanceTableAccounting();

	  struct st_logintokendetail
	  {
		  wchar_t m_login[40];
		  wchar_t m_hdno[250];
		  wchar_t m_ipaddress[16];
		  int returnval;
	  };




	  void convertToDealFormat(CString str_data);
	  CString generateJsonForUserTabs(CString  userlogin);
	  CString generatelogintoken(CString  userlogin,CString key,UINT64 logintime, CString ipAddress);
	  void updatelogintoken(CString	m_logintoken, int    m_status, UINT64 m_tokenactivetime, UINT64 m_lastdisconnecttime, CString	 m_login, CString	m_clientkey,CString m_hdno, CString m_ipaddress);
	  void removelogintoken(CString	m_logintoken,  CString	m_clientkey);
	  void orderUpdate(const char* strforjson);
	  void validatelogintoken(CString	m_logintoken, CString ipaddress, CSqlData::st_logintokendetail& m_temp);
	  void Update_PerfectAndRoundedInDataBase(CString SymbolGroup, double lots, double perfect, double rounded);
	  void Load_PerfectAndRoundedInDataBase();



	  struct st_cTableUpdateColumnMetaData
	  {
		  wchar_t m_ColumnKey[40];
		  wchar_t m_TabName[40];
		  wchar_t m_SqlTable[50];
		  wchar_t m_SqlTableColumn[50];
		  wchar_t m_MTModuleName[50];
		  wchar_t m_MtColumnName[50];
		  int m_DataIndexForUpdate;
		  wchar_t m_DataSeprator[1];
	  };
	  void processUpdateRequest(CString m_columnkey, CString m_tabname, CString m_value,CString m_login, CString m_symbol);
	  st_cTableUpdateColumnMetaData GetSqlTable(CString m_selectCondisiton);

	  CString generateJsonLoginData();

	  void loadDealTableAccounting();

	  int positionCount(string strgroupjason, CString filePath);

	  CString generateGroupJason(CString loginuser);


	  CString generateJsonCommentChangeData();

	  CString generateJsonGroupData();
	  CString getOrderData(CString loginuser)  ;

	  CString generateJsonForFetchUserData(CString  login);
	  CString UpdateLoginUserCredentials(const char* strforjson);		 
	  CString CommentChangeSaveData(const char* strforjson);	
	  CString CommentChangeToMT5(const char* strforjson);	
	  void validateauthKey(CString	authKey, CSqlData::st_logintokendetail& m_temp);
};
#endif