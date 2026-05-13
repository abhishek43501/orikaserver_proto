#include "StdAfx.h"
#include "Manager.h"
#include "StaticClass.h"
#include "MSMQ\MSMQApiWrapper.h"
#include "AlertStaticClass.h"
#include "OrikaServerDlg.h"
#include "dealerSink.h"
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
#include <iostream>
#include "TableClosingFile.h"
#include "afxdb.h"
#include "DataTable\orika_clientmaster.h"



using namespace rapidjson;
int CManager::dealSendToMSMQStart=0;
int CManager::previousRecordCount = 0;
int CManager::previousRecordCount_login = 0;
CManager::CManager() : m_manager(NULL),m_admin(NULL),m_login(0)
  {
   m_server[0]  =L'\0';
   m_password[0]=L'\0';
  }

CManager::~CManager(void)
  {

   Shutdown();
  }

CString CManager::ClosingFileTransfer(CString StrFilePath, CString closingDate)
{
	CString ReturnVal = L"";
	if (StrFilePath.IsEmpty())
	{
		return L"";
	}
	CString FileExtnsion = L"";
	int pos = StrFilePath.ReverseFind('.');
	if (pos != -1)
	{
		FileExtnsion = StrFilePath.Mid(pos, StrFilePath.GetLength());
	}
	CString cstring = L"";
	if (FileExtnsion.CompareNoCase(L".xlsx") == 0 || FileExtnsion.CompareNoCase(L".xls") == 0)
	{
		CString strFileFullPath = L"";
		strFileFullPath = CStaticClass::APIFolderPath + StrFilePath;
		ReturnVal = ExcelClosingfileTransfer(strFileFullPath);
	
	}
	if (FileExtnsion.CompareNoCase(L".csv") == 0)
	{
		CString strFileFullPath = L"";
		strFileFullPath = CStaticClass::APIFolderPath + StrFilePath;
		ReturnVal = CSVClosingfileTransfer(strFileFullPath, closingDate);
	}
	return ReturnVal;
}

CString  CManager::symbolMapping(CString StrFilePath)
{	
	CString ReturnVal = L"";
	if (StrFilePath.IsEmpty())
	{		
		return L"";
	}
	CString FileExtnsion = L"";
	int pos = StrFilePath.ReverseFind('.');
	if (pos != -1)
	{
		FileExtnsion = StrFilePath.Mid(pos, StrFilePath.GetLength());
	}
	CString cstring = L"";
	
		CString strFileFullPath = L"";
		strFileFullPath = CStaticClass::APIFolderPath + StrFilePath;
		ReturnVal=SymbolMappingfileTransfer(strFileFullPath, FileExtnsion);
		
	return ReturnVal;
}
CString CManager::ExcelClosingfileTransfer(CString StrFilePath)
{
	CDataSource Connection;
	HRESULT hr, hr1;
	CSession session, session1;
	CString Cmd = L"";
	CoInitialize(NULL);

	CString strReturnVal = L"";

	CCommand<CAccessor<ExcelDataTable>> table;
	CString connectionString = L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" + StrFilePath + ";Extended Properties=\"Excel 12.0;HDR=NO;\"";

	hr = Connection.OpenFromInitializationString(connectionString);

	

	if (SUCCEEDED(hr))
	{
		hr = session.Open(Connection);
	}

	if (SUCCEEDED(hr))
	{		
		CString Insert_Command = L"";
		Cmd = "SELECT F2,F18,IIF(isnull(F9),'0',F9),IIF(ISNULL(F10),'',F10),IIF(ISNULL(F15),0,F15),IIF(ISNULL(F16),0,F16) FROM [Sheet1$];";
		hr = table.Open(session, LPCTSTR(Cmd));
		if (SUCCEEDED(hr))
		{
			StringBuffer s;
			Writer<StringBuffer> writer(s);
			writer.StartObject();
			writer.Key("type");
			writer.String("CLOSINGPRICE_LIST");
			writer.Key("data");
			writer.StartArray();
			while (hr = table.MoveNext() == S_OK)
			{
				
				CString NseContract = table.v_nsecontract;
				double  rate = table.v_last_tick;
				CString optiontype = table.v_OptionType;
				CString	strikeprice = table.v_StrikePrice;
				double  high = table.v_high;
				double  low = table.v_low;


				CString temp_symbol = L"";
				if (!NseContract.IsEmpty() && rate != 0)
				{
					if (optiontype != L"")
					{
						CString temp_contract = NseContract;
						CString temp_indextype = temp_contract.Mid(0, temp_contract.Find(L" ", 0));
						temp_contract = temp_contract.Mid(temp_contract.Find(L" ", 0) + 2, temp_contract.GetLength() - temp_contract.Find(L" ", 0) + 2);
						 temp_symbol = temp_contract.Mid(0, temp_contract.Find(L" ", 0));
						temp_contract = temp_contract.Mid(temp_contract.Find(L" ", 0) + 2, temp_contract.GetLength() - temp_contract.Find(L" ", 0) + 2);
						CString temp_Expiray = temp_contract.Mid(0, temp_contract.Find(L" ", 0));
						NseContract = temp_indextype + temp_symbol + temp_Expiray;
					}
					else
					{
						CString temp_contract = NseContract;
						temp_symbol = temp_contract.Mid(temp_contract.Find(L" ", 0) + 2, temp_contract.GetLength() - temp_contract.Find(L" ", 0) + 2);
						temp_symbol = temp_symbol.Mid(0, temp_symbol.Find(L" ", 0));
					}
					NseContract.Remove(' ');
					CString tmp_Insert_Command = L"";
					tmp_Insert_Command.Format(L" EXEC [Update_closing] '%s','%f','%s','%s','%f','%f';", NseContract, rate, strikeprice, optiontype,high,low);
					Insert_Command = Insert_Command + tmp_Insert_Command;
				}
				writer.StartObject();
				CString m_MtSymbol = L"";


				CStaticClass::m_mutex_Tick.Lock();
					CStaticClass::st_Orika_ExchangeSymbol m_tempSt = {};
					CStaticClass::m_Orika_ExchangeSymbolHastable.Lookup(temp_symbol, m_tempSt);
					m_MtSymbol = m_tempSt.m_MTsymbol;
				CStaticClass::m_mutex_Tick.Unlock();





				writer.Key("mtsymbol");
				string ssMTsymbol = string(CT2CA(m_MtSymbol));
				const char* stMTsymbol = ssMTsymbol.c_str();
				writer.String(stMTsymbol);




				writer.Key("exsymbol");
				string ssexsymbol = string(CT2CA(temp_symbol));
				const char* stexsymbol = ssexsymbol.c_str();
				writer.String(stexsymbol);
				writer.Key("closingprice");
				writer.Double(rate);
				writer.Key("high");
				writer.Double(high);
				writer.Key("low");
				writer.Double(low);
				writer.EndObject();
			}

			writer.EndArray();
			writer.EndObject();



			CString str_FinalJsonUpdate = L"";

			str_FinalJsonUpdate = s.GetString();

			s.Clear();
			writer.Flush();
			strReturnVal = str_FinalJsonUpdate;
		}		
		if (Insert_Command.Trim() != L"")
		{
			CStaticClass::m_sqldata.executeCommand(Insert_Command);
		}
	}
	

	table.Close();
	session.Close();
	CoUninitialize();
	Connection.Close();
	return strReturnVal;
}




CString CManager::SymbolMappingfileTransfer(CString StrFilePath,CString FileType)
{
	struct m_symbolStructure
	{
		wchar_t  m_MTSymbol[100];
		wchar_t  m_NSESymbol[100];
		wchar_t  m_MTNextSymbol[100];
		double m_NSELotSize;
		double m_MTLotSize;
	};
	CMap<CString, LPCTSTR, m_symbolStructure, m_symbolStructure&> m_TempSymbolMap;
	CString ReturnVal = L"";


	if (FileType.CompareNoCase(L".xlsx") == 0 || FileType.CompareNoCase(L".xls") == 0)
	{
		CDataSource Connection;
		HRESULT hr, hr1;
		CSession session, session1;
		CString Cmd = L"";
		CoInitialize(NULL);
		CCommand<CAccessor<SymbolMappingDataTable>> table;
		CString connectionString = L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" + StrFilePath + ";Extended Properties=\"Excel 12.0;HDR=NO;\"";
		hr = Connection.OpenFromInitializationString(connectionString);
		//now opening the session 
		if (SUCCEEDED(hr))
		{
			hr = session.Open(Connection);
		}
		if (SUCCEEDED(hr))
		{
			CString Insert_Command = L"";
			Cmd = "SELECT F1,F2,F3,IIF(ISNULL(F4),1,F4),IIF(ISNULL(F5),1,F5) FROM [Sheet1$];";
			//Cmd = "SELECT F1 FROM [Sheet1$];";
			hr = table.Open(session, LPCTSTR(Cmd));
			if (SUCCEEDED(hr))
			{
				while (hr = table.MoveNext() == S_OK)
				{
					CString  m_MTSymbol = L"";
					m_MTSymbol = table.m_MTSymbol;
					CString  m_NSESymbol = L"";
					m_NSESymbol = table.m_NSESymbol;
					CString  m_MTNextSymbol = L"";
					m_MTNextSymbol = table.m_MTNextSymbol;
					double   m_NSELotSize = table.m_NSELotSize;
					double   m_MTLotSize = table.m_MTLotSize;
					m_symbolStructure m_tmp = {};
					CMTStr::Copy(m_tmp.m_MTSymbol, m_MTSymbol);
					CMTStr::Copy(m_tmp.m_NSESymbol, m_NSESymbol);
					CMTStr::Copy(m_tmp.m_MTNextSymbol, m_MTNextSymbol);
					m_tmp.m_NSELotSize = m_NSELotSize;
					m_tmp.m_MTLotSize = m_MTLotSize;
					m_TempSymbolMap.SetAt(m_MTSymbol, m_tmp);
				}
			}
		}
		table.Close();
		session.Close();
		CoUninitialize();
		Connection.Close();
	}
	if (FileType.CompareNoCase(L".csv") == 0|| FileType.CompareNoCase(L".Csv") == 0 || FileType.CompareNoCase(L".CSV") == 0)
	{
		CString strval = L"";
		CStdioFile	myFile;
		if (myFile.Open(StrFilePath, CFile::modeRead))
		{
			myFile.Seek(0, CFile::begin);
			CString Insert_Command = L"";
			int data_count = 0;			
			while (myFile.ReadString(strval))
			{				
				CString m_MTSymbol = strval.Mid(0, strval.Find(','));
				strval = strval.Mid(strval.Find(',') + 1, (strval.GetLength() - strval.Find(',') - 1));
				CString m_NSESymbol = strval.Mid(0, strval.Find(','));
				strval = strval.Mid(strval.Find(',') + 1, (strval.GetLength() - strval.Find(',') - 1));
				CString m_MTNextSymbol = strval.Mid(0, strval.Find(','));
				strval = strval.Mid(strval.Find(',') + 1, (strval.GetLength() - strval.Find(',') - 1));
				CString strNSELotSize = strval.Mid(0, strval.Find(','));				
				strval = strval.Mid(strval.Find(',') + 1, (strval.GetLength() - strval.Find(',') - 1));								
				CString strMTLotSize = strval;

				double   m_NSELotSize = _wtof(strNSELotSize);
				double   m_MTLotSize = _wtof(strMTLotSize);
				m_symbolStructure m_tmp = {};
				CMTStr::Copy(m_tmp.m_MTSymbol, m_MTSymbol);
				CMTStr::Copy(m_tmp.m_NSESymbol, m_NSESymbol);
				CMTStr::Copy(m_tmp.m_MTNextSymbol, m_MTNextSymbol);
				m_tmp.m_NSELotSize = m_NSELotSize;
				m_tmp.m_MTLotSize = m_MTLotSize;
				m_TempSymbolMap.SetAt(m_MTSymbol, m_tmp);

			}
		 }
		 myFile.Close();		
	}
	POSITION pos = m_TempSymbolMap.GetStartPosition();
	while (pos != NULL)
	{		
		CString strSymbol = L"";
		m_symbolStructure m_tmp = {};
		m_TempSymbolMap.GetNextAssoc(pos,  strSymbol, m_tmp);
		CString  m_MTSymbol = L"";
		m_MTSymbol = m_tmp.m_MTSymbol;
		CString  m_NSESymbol = L"";
		m_NSESymbol = m_tmp.m_NSESymbol;
		CString  m_MTNextSymbol = L"";
		m_MTNextSymbol = m_tmp.m_MTNextSymbol;
		double   m_NSELotSize = m_tmp.m_NSELotSize;
		double   m_MTLotSize = m_tmp.m_MTLotSize;



		CStaticClass::m_mutex_Tick.Lock();
			CStaticClass::st_Orika_symbolmaster m_st_Orika_symbolmaster = {};		
			CStaticClass::m_Orika_symbolmasterHastable.Lookup(strSymbol, m_st_Orika_symbolmaster);				
			CMTStr::Copy(m_st_Orika_symbolmaster.m_nextSymbol, m_MTNextSymbol);
			CMTStr::Copy(m_st_Orika_symbolmaster.m_exchangeSymbol, m_NSESymbol);
			m_st_Orika_symbolmaster.m_nselotsize = m_NSELotSize;
			m_st_Orika_symbolmaster.m_mtlotsize = m_MTLotSize;				
			CStaticClass::m_Orika_symbolmasterHastable.SetAt(strSymbol, m_st_Orika_symbolmaster);




			CStaticClass::st_Orika_ExchangeSymbol m_tempSt = {};
			CString StrExchangeSymbol = m_NSESymbol;
			CMTStr::Copy(m_tempSt.m_exchangeSymbol, StrExchangeSymbol);
			CMTStr::Copy(m_tempSt.m_MTsymbol, strSymbol);
			CMTStr::Copy(m_tempSt.m_NextMTsymbol, m_MTNextSymbol);
			CStaticClass::m_Orika_ExchangeSymbolHastable.SetAt(StrExchangeSymbol, m_tempSt);

		CStaticClass::m_mutex_Tick.Unlock();
		CString strCommand = L"";
		strCommand.Format(L"update Orika_symbolmaster  set nextSymbol='%s',exchangeSymbol='%s',nselotsize='%.0lf',mtlotsize='%.0lf' where symbol='%s'", m_MTNextSymbol, m_NSESymbol, m_NSELotSize, m_MTLotSize, strSymbol);
		CStaticClass::m_sqldata.executeCommand(strCommand);


	}
	ReturnVal = L"{\"type\":\"SYMBOL_MAPPING_SUCCESS\",\"message\" : \"All Symbol Has Been Updated\"}";
	return ReturnVal;
}

CString CManager::GetsymbolMappingData()
{
	
	CStaticClass::m_mutex_Tick.Lock();

	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("type");
	writer.String("SYMBOLMAPPING_DATA");
	
	writer.Key("symboldata");
	writer.StartArray();				
	POSITION pos = CStaticClass::m_Orika_symbolmasterHastable.GetStartPosition();
	while (pos != NULL)
	{
		CString strSymbol = L"";
		CStaticClass::st_Orika_symbolmaster m_tmp = {};
		CStaticClass::m_Orika_symbolmasterHastable.GetNextAssoc(pos, strSymbol, m_tmp);
				
		CString  m_MTSymbol = L"";
		m_MTSymbol = strSymbol;
		CString  m_NSESymbol = L"";
		m_NSESymbol = m_tmp.m_exchangeSymbol;
		CString  m_MTNextSymbol = L"";
		m_MTNextSymbol = m_tmp.m_nextSymbol;
		double   m_NSELotSize = m_tmp.m_nselotsize;
		double   m_MTLotSize = m_tmp.m_mtlotsize;
		writer.StartObject();
		writer.Key("mtsymbol");
		string ssSymbol = string(CT2CA(m_MTSymbol));
		const char* stSymbol = ssSymbol.c_str();
		writer.String(stSymbol);

		writer.Key("exchangesymbol");
		string exchangesymbol = string(CT2CA(m_NSESymbol));
		const char* stexchangesymbol = exchangesymbol.c_str();
		writer.String(stexchangesymbol);

		writer.Key("nextmtsymbol");
		string nextmtsymbol = string(CT2CA(m_MTNextSymbol));
		const char* stnextmtsymbol = nextmtsymbol.c_str();
		writer.String(stnextmtsymbol);

		writer.Key("exchangelotsize");		
		writer.Double(m_NSELotSize);

		writer.Key("mtlotsize");
		writer.Double(m_MTLotSize);


		writer.EndObject();
	}	
	writer.EndArray();
	writer.EndObject();
	CStaticClass::m_mutex_Tick.Unlock();


	CString str_FinalJsonUpdate = L"";
	
	str_FinalJsonUpdate = s.GetString();

	s.Clear();
	writer.Flush();

	return str_FinalJsonUpdate;
}





class CMyRecordset : public CRecordset
{
public:
	CString m_pageandsheetID, m_pageID, m_sheet;
	CMyRecordset(CDatabase* pDatabase)
		: CRecordset(pDatabase)
	{
		m_pageandsheetID = L""; 
		m_pageID = L""; 
		m_sheet = L"";
	}	
	// Step 3: Override the DoFieldExchange function
	virtual void DoFieldExchange(CFieldExchange* pFX) 
	{		
		RFX_Text(pFX, L"pageandsheetID", m_pageandsheetID);
		RFX_Text(pFX, L"pageID", m_pageID);
		RFX_Text(pFX, L"sheet", m_sheet);
	}

	
};







//FETCH_PAGE_DETAILS
CString CManager::CreateFETCH_PAGE_DETAILS(CString loginUser,CString pageId)
{
	CString  strReturnVal = L"";
	HRESULT hrr = NULL;
	CSession m_tempsession;	
	CSession m_tempsession_sheet;

	CString m_widgetConfigs1=L"";
	CString   strCommand = L"";

	//CTableorika_page_widgetConfig
	CCommand<CAccessor<CTableorika_page_widgetConfig>> data_table_wid;
	if (!SUCCEEDED(hrr))
	{
		return L"";
	}
	strCommand.Format(L"select pageId,widgetConfig from orika_pageIDandwidgetconfigs where pageId='%s';", pageId);
	//(L"Orderlock_70");
	m_tempsession.Open(CStaticClass::connection);	
	hrr = data_table_wid.Open(m_tempsession, (LPCTSTR)strCommand);
	if (FAILED(hrr))
	{
		m_tempsession.Close();
		return L"";
	}
	while (hrr = data_table_wid.MoveNext() == S_OK)
	{		
		CString m_subwidgetconfig = data_table_wid.m_widgetConfig1;
		CString m_wConfigID = m_subwidgetconfig.Mid(0, m_subwidgetconfig.Find(L":") - 1);
		m_wConfigID.Replace(L"\"", L"");

		CString m_wConfigJason = m_subwidgetconfig.Mid(m_subwidgetconfig.Find(L":")+1, (m_subwidgetconfig.GetLength()- m_subwidgetconfig.Find(L":")-1));


		CString pageandconfigID = pageId+m_wConfigID;

		rapidjson::Document  m_subdocument;
		string m_strsubjason = CT2A(m_wConfigJason.GetString());
		const char* strwidgetConfigs = m_strsubjason.c_str();
		if (!m_subdocument.Parse<0>(strwidgetConfigs).HasParseError())
		{

			CString strCommand_sheet = L"";
			strCommand_sheet.Format(L"select pageandsheetID,pageID,sheet from  orika_pageIDandwidgetconfigs_and_sheet_mapping where pageandsheetID='%s'  order by isnull(sl_no,0)  asc;", pageandconfigID);
			////(L"Orderlock_70");
			//m_tempsession_sheet.Open(CStaticClass::connection);
			//CCommand<CAccessor<CTableorika_page_widgetConfig_and_Sheet>> data_table_wid_Sheet;
			//hrr = data_table_wid_Sheet.Open(m_tempsession_sheet, (LPCTSTR)strCommand_sheet);
			//if (FAILED(hrr))
			//{
			//	m_tempsession_sheet.Close();
			//	return L"";
			//}	
			CDatabase db;
			//"DRIVER={SQL Server};SERVER=YourServerName;DATABASE=YourDatabaseName;UID=YourUsername;PWD=YourPassword;"
			//if (!db.OpenEx(L"Provider=SQLNCLI11.1;Password=" + CStaticClass::SqlServerPassword + ";Persist Security Info=False;User ID=" + CStaticClass::SqlServerUserID + ";Initial Catalog=" + CStaticClass::SqlServerDatabase + ";Data Source=" + CStaticClass::SqlServerAdd + ";Use Procedure for Prepare=1;Auto Translate=True;Packet Size=4096;Workstation ID=WIN-CE63GLSHUM0;Initial File Name=\"\";Use Encryption for Data=False;Tag with column collation when possible=False;MARS Connection=False;DataTypeCompatibility=0;Trust Server Certificate=False;Application Intent=READWRITE"))
			if (!db.OpenEx(L"DRIVER={SQL Server};SERVER=" + CStaticClass::SqlServerAdd + ";DATABASE=" + CStaticClass::SqlServerDatabase + ";UID=" + CStaticClass::SqlServerUserID + ";PWD=" + CStaticClass::SqlServerPassword + ";"))
			{
				// Handle connection error
				//return;
			}

			CMyRecordset  rs(&db);			
			int sheetExist = 0;
			rapidjson::Document::AllocatorType& allocator = m_subdocument.GetAllocator();
			rapidjson::Value newArray(rapidjson::kArrayType);
			if (rs.Open(CRecordset::forwardOnly, strCommand_sheet))
			{
				
				while (!rs.IsEOF())
				{					
					CString m_stetandconfigid = L"";
					m_stetandconfigid = rs.m_pageandsheetID;
					CString m_Sheet = L"";
					m_Sheet = rs.m_sheet;

					string strSheet = CT2A(m_Sheet);


					rapidjson::Value sheetobject(rapidjson::kObjectType);

					rapidjson::Document m_tempdocument;
					m_tempdocument.Parse(strSheet.c_str());


					if (!m_tempdocument.HasParseError())
					{
						const rapidjson::Value& m_tempobject = m_tempdocument;
						rapidjson::Value sheetobject(m_tempobject, allocator);

						rapidjson::Document::AllocatorType& allocator_t = m_subdocument.GetAllocator();

						newArray.PushBack(sheetobject, allocator_t);
						allocator_t.Free(NULL);
					}
					sheetExist = 1;

					rs.MoveNext();
				}
			}
			db.Close();
			if (sheetExist == 1)
			{
				m_subdocument.AddMember("sheets", newArray, allocator);
			}
			allocator.Free(NULL);		
		}
		
		StringBuffer m_tempbuffersheet;
		Writer<StringBuffer>m_tempwritersheet(m_tempbuffersheet);
		m_subdocument.Accept(m_tempwritersheet);
		CString m_temp_conf(m_tempbuffersheet.GetString());

		m_temp_conf = L"\"" + m_wConfigID+L"\":" + m_temp_conf;
		
		if (m_widgetConfigs1 == L"")
		{
			m_widgetConfigs1 = m_temp_conf;
		}
		else
		{
			m_widgetConfigs1 = m_widgetConfigs1 + L"," + m_temp_conf;
		}
	}
	m_tempsession.Close();


	m_widgetConfigs1 = L"{" + m_widgetConfigs1 + L"}";


	CCommand<CAccessor<CTableorika_page_details>> data_table;
	if (!SUCCEEDED(hrr))
	{
		return L"";
	}
	
	strCommand.Format(L"exec fetch_page_Details '%s';",  pageId);
	
	//(L"Orderlock_70");
	
	m_tempsession.Open(CStaticClass::connection);
	hrr = data_table.Open(m_tempsession, (LPCTSTR)strCommand);
	if (FAILED(hrr))
	{
		m_tempsession.Close();
		return L"";
	}
	int i = 0;

	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("type");
	writer.String("PAGE_DETAILS");
		
	while (hrr = data_table.MoveNext() == S_OK)
	{
		writer.Key("page");
		writer.StartObject();
		writer.Key("pageId");
		CString m_pegeID = data_table.m_pageId;
		string strpegeID = CT2A(m_pegeID);
		const char* cspegeID = strpegeID.c_str();
		writer.String(cspegeID);

		writer.Key("name");
		CString m_name = data_table.m_name;
		string strname = CT2A(m_name);
		const char* csname = strname.c_str();
		writer.String(csname);

		writer.Key("shortName");
		CString m_shortName = data_table.m_shortName;
		string strshortName = CT2A(m_shortName);
		const char* csshortName = strshortName.c_str();
		writer.String(csshortName);



		writer.Key("layout");
		CString m_layout1 = data_table.m_layout1;
		


		

		CString m_layout= m_layout1;
		string strlayout = CT2A(m_layout);
		const char* cslayout = strlayout.c_str();
		writer.String(cslayout);


		writer.Key("widgetConfigs");
		 			


		CString m_widgetConfigs = m_widgetConfigs1;
		string strwidgetConfigs = CT2A(m_widgetConfigs);
		const char* cswidgetConfigs = strwidgetConfigs.c_str();
		writer.String(cswidgetConfigs);


		writer.EndObject();
	}
	
	m_tempsession.Close();
	
	//(L"U_Orderlock_70");
	writer.EndObject();
	CString str_FinalJsonUpdate = L"";
	strReturnVal = str_FinalJsonUpdate = s.GetString();
	s.Clear();
	writer.Flush();
	return strReturnVal;
}
CString CManager::createFETCH_COLOR_TEMPLATE(CString loginUser)
{
	CString  strReturnVal = L"";
	HRESULT hrr = NULL;
	CCommand<CAccessor<CTableorika_COLOR_TEMPLATE_ClientWise>> data_table;
	if (!SUCCEEDED(hrr))
	{
		return L"";
	}
	CString   strCommand = L"";
	strCommand.Format(L"select loginUser,colortemplate from orika_COLOR_TEMPLATE_ClientWise where loginUser='%s';", loginUser);
	 
	//(L"Orderlock_70");
	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hrr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
	if (FAILED(hrr))
	{
		m_tempSession.Close();
		
		return L"";
	}
	int i = 0;

	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("type");
	writer.String("COLOR_TEMPLATES");
	writer.Key("loginUser");
	string strloginUser = CT2A(loginUser);
	const char* csLoginUser = strloginUser.c_str();
	writer.String(csLoginUser);
	CString str_template = L"";
	writer.Key("templates");
	writer.StartArray();
	while (hrr = data_table.MoveNext() == S_OK)
	{		
		str_template = data_table.m_template;
		string strtemplate = CT2A(str_template);
		const char* cstemplate = strtemplate.c_str();
		writer.RawValue(cstemplate, strlen(cstemplate), kObjectType);
	}
	writer.EndArray();
	m_tempSession.Close();	 
	//(L"U_Orderlock_70");
	writer.EndObject();


	CString str_FinalJsonUpdate = L"";
	strReturnVal = str_FinalJsonUpdate = s.GetString();
	s.Clear();
	writer.Flush();
	return strReturnVal;
}


CString CManager::FETCH_COLOR_THEME(CString id)
{
	CString  strReturnVal = L"";
	HRESULT hrr = NULL;
	CCommand<CAccessor<CTableColorTheme>> data_table;
	if (!SUCCEEDED(hrr))
	{
		return L"";
	}
	CString   strCommand = L"";
	strCommand.Format(L"select loginUser,COLOR_THEME_ID,COLOR_THEME_name,[color],[fontcolor],[default] from orika_COLOR_THEME where COLOR_THEME_ID='%s';", id);
	CSession m_tempSession;

	//(L"Orderlock_70");
	m_tempSession.Open(CStaticClass::connection);
	hrr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
	if (FAILED(hrr))
	{
		m_tempSession.Close();

		return L"";
	}
	int i = 0;

	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	
	while (hrr = data_table.MoveNext() == S_OK)
	{	
		writer.Key("type");
		writer.String("COLOR_THEME");
		writer.Key("loginUser");
		CString m_loginUser = data_table.m_loginUser;
		string strloginUser = CT2A(m_loginUser);
		const char* csLoginUser = strloginUser.c_str();
		writer.String(csLoginUser);

		writer.Key("id");
		CString m_ID = data_table.m_COLOR_THEME_ID;
		string strID = CT2A(m_ID);
		const char* csID = strID.c_str();
		writer.String(csID);

		writer.Key("name");
		CString m_name = data_table.m_COLOR_THEME_name;
		string strname = CT2A(m_name);
		const char* csname = strname.c_str();
		writer.String(csname);

		writer.Key("color");
		CString m_color = data_table.m_color;
		string strcolor = CT2A(m_color);
		const char* csscolor = strcolor.c_str();
		writer.RawValue(csscolor, strlen(csscolor), kObjectType);


		writer.Key("fontColor");
		CString  m_fontColor = data_table.m_fontcolor;
		string strfontColor = CT2A(m_fontColor);
		const char* cssfontColor = strfontColor.c_str();		
		writer.RawValue(cssfontColor, strlen(cssfontColor), kObjectType);



		writer.Key("default");
		int  m_default = data_table.m_default;
		bool m_b_default = false;
		if (m_default == 1)
		{
			m_b_default = true;
		}
		writer.Bool(m_b_default);


		
	}	
	m_tempSession.Close();

	//(L"U_Orderlock_70");
	writer.EndObject();


	CString str_FinalJsonUpdate = L"";
	strReturnVal = str_FinalJsonUpdate = s.GetString();
	s.Clear();
	writer.Flush();
	return strReturnVal;
}


CString CManager::FETCH_COLOR_THEMES(CString loginUser)
{
	CString  strReturnVal = L"";
	HRESULT hrr = NULL;
	CCommand<CAccessor<CTableColorTheme>> data_table;
	if (!SUCCEEDED(hrr))
	{
		return L"";
	}
	CString   strCommand = L"";
	strCommand.Format(L"get_color_Themes '%s';", loginUser);
	CSession m_tempSession;

	//(L"Orderlock_70");
	m_tempSession.Open(CStaticClass::connection);
	hrr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
	if (FAILED(hrr))
	{
		m_tempSession.Close();

		return L"";
	}
	int i = 0;

	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("type");
	writer.String("COLOR_THEMES");
	writer.Key("loginUser");
	string strloginUser = CT2A(loginUser);
	const char* csLoginUser = strloginUser.c_str();
	writer.String(csLoginUser);
	writer.Key("themes");
	writer.StartArray();
	while (hrr = data_table.MoveNext() == S_OK)
	{
		writer.StartObject();
		writer.Key("id");
		CString m_ID = data_table.m_COLOR_THEME_ID;
		string strID = CT2A(m_ID);
		const char* csID = strID.c_str();
		writer.String(csID);

		writer.Key("name");
		CString m_name = data_table.m_COLOR_THEME_name;
		string strname = CT2A(m_name);
		const char* csname = strname.c_str();
		writer.String(csname);

		writer.Key("color");
		CString m_color = data_table.m_color;
		string strcolor = CT2A(m_color);
		const char* csscolor = strcolor.c_str();
		writer.RawValue(csscolor, strlen(csscolor), kObjectType);


		writer.Key("fontColor");
		CString  m_fontColor = data_table.m_fontcolor;
		string strfontColor = CT2A(m_fontColor);
		const char* cssfontColor = strfontColor.c_str();
		writer.RawValue(cssfontColor, strlen(cssfontColor), kObjectType);


		writer.Key("default");
		int  m_default = data_table.m_default;
		bool m_b_default = false;
		if (m_default == 1)
		{
			m_b_default = true;
		}
		writer.Bool(m_b_default);


		writer.EndObject();
	}
	writer.EndArray();
	m_tempSession.Close();

	//(L"U_Orderlock_70");
	writer.EndObject();


	CString str_FinalJsonUpdate = L"";
	strReturnVal = str_FinalJsonUpdate = s.GetString();
	s.Clear();
	writer.Flush();
	return strReturnVal;
}
CString CManager::CreateFETCH_PAGE_LIST(CString loginUser)
{
	CString  strReturnVal = L"";
	HRESULT hrr = NULL;
	CCommand<CAccessor<CTableorika_page_list>> data_table;
	if (!SUCCEEDED(hrr))
	{
		return L"";
	}
	CString   strCommand = L"";
	strCommand.Format(L"get_page_details '%s';", loginUser);
	CSession m_tempSession;
	 
	//(L"Orderlock_70");
	m_tempSession.Open(CStaticClass::connection);
	hrr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
	if (FAILED(hrr))
	{
		m_tempSession.Close();
		 
		return L"";
	}
	int i = 0;

	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("type");
	writer.String("PAGE_LIST");
	writer.Key("loginUser");
	string strloginUser = CT2A(loginUser);
	const char* csLoginUser = strloginUser.c_str();
	writer.String(csLoginUser);
	writer.Key("pages");
	writer.StartArray();
	while (hrr = data_table.MoveNext() == S_OK)
	{
		writer.StartObject();
		writer.Key("pageId");
		CString m_pegeID = data_table.m_pageId;
		string strpegeID = CT2A(m_pegeID);
		const char* cspegeID = strpegeID.c_str();
		writer.String(cspegeID);

		writer.Key("name");
		CString m_name = data_table.m_name;
		string strname = CT2A(m_name);
		const char* csname = strname.c_str();
		writer.String(csname);

		writer.Key("shortName");
		CString m_shortName = data_table.m_shortName;
		string strshortName = CT2A(m_shortName);
		const char* csshortName = strshortName.c_str();
		writer.String(csshortName);


		writer.Key("sequence");
		int  m_sequence = data_table.m_sequence;		
		writer.Int(m_sequence);

		writer.EndObject();
	}
	writer.EndArray();
	m_tempSession.Close();
	 
	//(L"U_Orderlock_70");
	writer.EndObject();


	CString str_FinalJsonUpdate = L"";
	strReturnVal = str_FinalJsonUpdate = s.GetString();
	s.Clear();
	writer.Flush();
	return strReturnVal;
}
CString CManager::CreateFETCH_Theme(CString loginUser)
{
	CString  strReturnVal = L"";
	HRESULT hrr = NULL;
	CCommand<CAccessor<CTableorika_Theme>> data_table;
	if (!SUCCEEDED(hrr))
	{
		return L"";
	}
	CString   strCommand = L"";
	strCommand.Format(L"select loginuser,theme from   orika_theme where loginuser='%s';", loginUser);
	 
	CSession m_tempSession;
	//(L"Orderlock_70");
	m_tempSession.Open(CStaticClass::connection);
	hrr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
	if (FAILED(hrr))
	{
		m_tempSession.Close();		
		return L"";
	}
	int i = 0;

	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("type");
	writer.String("SELECTED_THEME");
	CString m_theme = L"";
	while (hrr = data_table.MoveNext() == S_OK)
	{		
		m_theme = data_table.m_theme;			
	}
	
	m_tempSession.Close();
	 
	//(L"U_Orderlock_70");

	writer.Key("theme");
	string strm_theme = CT2A(m_theme);
	const char* csm_theme = strm_theme.c_str();
	writer.String(csm_theme);
	writer.EndObject();


	CString str_FinalJsonUpdate = L"";
	strReturnVal = str_FinalJsonUpdate = s.GetString();
	s.Clear();
	writer.Flush();
	return strReturnVal;
}


CString CManager::GetDashboardData(CString loginUser)
{
	CString  strReturnVal = L"";
	HRESULT hr = NULL;
	CCommand<CAccessor<CTableDashboard>> data_table;
	if (!SUCCEEDED(hr))
	{
		return L"";
	}
	CString   strCommand = L"";
	strCommand.Format(L"exec get_Dashboard_Data '%s';", loginUser);
	 
	//(L"Orderlock_69");
	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
	if (FAILED(hr))
	{
		m_tempSession.Close();
		 
		return L"";
	}
	int i = 0;

	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("type");
	writer.String("DASHBOARD");

	writer.Key("gridcolumndata");
	writer.StartArray();


	while (hr = data_table.MoveNext() == S_OK)
	{
		CString  m_loginuser= data_table.m_loginuser;
		CString  m_field= data_table.m_field;
		CString  m_type= data_table.m_type;
		CString  m_gridProperties= data_table.m_gridProperties;
		CString  m_data= data_table.m_data;
		writer.StartObject();
		writer.Key("field");
		string ssfield = string(CT2CA(m_field));
		const char* stfield = ssfield.c_str();
		writer.String(stfield);
		writer.Key("type");
		string sstype = string(CT2CA(m_type));
		const char* sttype = sstype.c_str();
		writer.String(sttype);
		writer.Key("gridProperties");
		string ssgridProperties = string(CT2CA(m_gridProperties));
		const char* stgridProperties = ssgridProperties.c_str();
		writer.RawValue(stgridProperties, m_gridProperties.GetLength(), rapidjson::Type::kObjectType);
		writer.Key("data");
		string ssdata = string(CT2CA(m_data));
		const char* stdata = ssdata.c_str();
		//writer.String(stdata);		
		writer.RawValue(stdata, m_data.GetLength(), rapidjson::Type::kArrayType);
		writer.EndObject();
	}
	writer.EndArray();
	writer.EndObject();
	m_tempSession.Close();
	 
	//(L"U_Orderlock_69");
	CString str_FinalJsonUpdate = L"";
	strReturnVal = str_FinalJsonUpdate = s.GetString();
	s.Clear();
	writer.Flush();
	return strReturnVal;
}

CString CManager::GetCanvasData(CString loginUser,CString tabtype)
{
	CString  strReturnVal = L"";
	HRESULT hr = NULL;
	CCommand<CAccessor<CTableCanvas>> data_table;
	if (!SUCCEEDED(hr))
	{
		return L"";
	}
	CString   strCommand = L"";
	strCommand.Format(L"select * from orika_canvas where loginuser='%s' and tabtype='%s';", loginUser, tabtype);
	 
	//(L"Orderlock_70");
	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
	if (FAILED(hr))
	{
		m_tempSession.Close();
		 
		return L"";
	}
	int i = 0;

	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("type");
	writer.String("CANVAS");

	


	while (hr = data_table.MoveNext() == S_OK)
	{
		CString  m_loginuser = data_table.m_loginuser;
		CString  m_tabtype = data_table.m_tabtype;		
		CString  m_data = data_table.m_data;
		writer.Key("tabtype");
		string sstabtype = string(CT2CA(m_tabtype));
		const char* strtabtype = sstabtype.c_str();				
		writer.String(strtabtype);

		string ssdata = string(CT2CA(m_data));
		const char* stdata = ssdata.c_str();		
		writer.Key("data");
		writer.RawValue(stdata, m_data.GetLength(), rapidjson::Type::kArrayType);


		//string ssgridProperties = string(CT2CA(m_gridProperties));
		//const char* stgridProperties = ssgridProperties.c_str();
		//writer.RawValue(stgridProperties, m_gridProperties.GetLength(), rapidjson::Type::kObjectType);
		//writer.Key("data");
		//string ssdata = string(CT2CA(m_data));
		//const char* stdata = ssdata.c_str();
		////writer.String(stdata);		
		//writer.RawValue(stdata, m_data.GetLength(), rapidjson::Type::kArrayType);
		//writer.EndObject();
	}
	
	m_tempSession.Close();
	 
	//(L"U_Orderlock_70");
	writer.EndObject();


	CString str_FinalJsonUpdate = L"";
	strReturnVal = str_FinalJsonUpdate = s.GetString();
	s.Clear();
	writer.Flush();
	return strReturnVal;
}





//CString CManager::GetGATEWAY_POSITIONDEVIDERATIO()
//{
//	CString  strReturnVal = L"";
//	HRESULT hr = NULL;
//	CString   strCommand = L"";
//	m_IpPriotityArray.Clear();
//	CCommand<CAccessor<CTableOrika_PositionTransferPriotity>> data_table_LP;
//	if (!SUCCEEDED(hr))
//	{
//		return L"";
//	}	
//	strCommand.Format(L"select symbolgroup,lpname ,lpsymbol ,lppriority ,lpmaxlots ,lpsendingtype,lpratio from Orika_LpPriotity order by symbolgroup asc");
//	CStaticClass::m_mutex_order.Lock();
//	
//
//	CString  m_symbolgroup=L"";
//	CString  m_lpname = L"";
//	CString  m_lpsymbol = L"";
//	int    m_lppriority=0;
//	double m_lpmaxlots=0;
//	CString  m_lpsendingtype = L"";
//	double m_lpratio=0;
//
//
//	////(StrPrintLino);
//	CStaticClass::session.Open(CStaticClass::connection);
//	hr = data_table_LP.Open(CStaticClass::session, (LPCTSTR)strCommand);
//	if (FAILED(hr))
//	{
//		CStaticClass::session.Close();
//		CStaticClass::m_mutex_order.Unlock();
//		return L"";
//	}
//	CString m_oldsymbolgroup = L"";	
//	int dataCount = 0;
//	while (hr = data_table_LP.MoveNext() == S_OK)
//	{		
//		m_symbolgroup = data_table_LP.m_symbolgroup;
//		m_lpname = data_table_LP.m_lpname;
//		m_lpsymbol = data_table_LP.m_lpsymbol;
//		m_lppriority = data_table_LP.m_lppriority;
//		m_lpmaxlots = data_table_LP.m_lpmaxlots;
//		m_lpsendingtype = data_table_LP.m_lpsendingtype;
//		m_lpratio = data_table_LP.m_lpratio;
//		
//		st_IpPriotity m_st_IpPriotity = {};
//		CMTStr::Copy(m_st_IpPriotity.m_symbolgroup, m_symbolgroup);
//		CMTStr::Copy(m_st_IpPriotity.m_lpname, m_lpname);
//		CMTStr::Copy(m_st_IpPriotity.m_lpsymbol, m_lpsymbol);
//		m_st_IpPriotity.m_lppriority = m_lppriority;
//		m_st_IpPriotity.m_lpmaxlots = m_lpmaxlots;
//		CMTStr::Copy(m_st_IpPriotity.m_lpsendingtype, m_lpsendingtype);
//		m_st_IpPriotity.m_lpratio = m_lpratio;
//		m_IpPriotityArray.Add(&m_st_IpPriotity);
//		
//	
//	}
//	
//	data_table_LP.Close();
//	CStaticClass::session.Close();
//
//
//	CCommand<CAccessor<CTableGATEWAY_POSITIONDEVIDERATIO>> data_table;
//	if (!SUCCEEDED(hr))
//	{
//		return L"";
//	}
//	
//	strCommand.Format(L"select symbolgroup,lots,ratio,stepup,isnull(perfect,0)as'perfect',isnull(rounded,0)as 'rounded' from Orika_GATEWAY_POSITIONDEVIDERATIO order by symbolgroup");
//	
//	CString StrPrintLino = L"";
//	StrPrintLino.Format(L"m_mutex_order Locked(%d)", __LINE__);
//	////(StrPrintLino);
//	CStaticClass::session.Open(CStaticClass::connection);
//	hr = data_table.Open(CStaticClass::session, (LPCTSTR)strCommand);
//	if (FAILED(hr))
//	{
//		CStaticClass::session.Close();
//		CStaticClass::m_mutex_order.Unlock();
//		return L"";
//	}
//	int i = 0;
//	StringBuffer s;
//	Writer<StringBuffer> writer(s);
//	writer.StartObject();
//	writer.Key("type");
//	writer.String("GATEWAY_POSITIONDEVIDERATIO");	
//	writer.Key("data");
//	writer.StartArray();
//	CString m_oldSymbolGroup = L"";
//	int endobjectFlag = 0;
//	double m_total = 0;
//	while (hr = data_table.MoveNext() == S_OK)
//	{
//		CString m_symbolgroup = data_table.m_symbolgroup;
//		double m_lots = data_table.m_lots;
//		double m_ratio = data_table.m_ratio;
//		double m_stepup = data_table.m_stepup;
//		double m_perfect = data_table.m_perfect;
//		double m_rounded = data_table.m_rounded;
//		if (m_oldSymbolGroup.MakeUpper() != m_symbolgroup.MakeUpper())
//		{
//			endobjectFlag = 1;
//			if (m_oldSymbolGroup.Trim().GetLength() != 0)
//			{
//				writer.EndArray();
//				
//				writer.EndObject();
//			}
//			writer.StartObject();
//			writer.Key("symbolgroup");
//			string sssymbolgroup = string(CT2CA(m_symbolgroup));
//			const char* stsymbolgroup = sssymbolgroup.c_str();
//			writer.String(stsymbolgroup);
//			
//			writer.Key("total");	
//			writer.Double(m_total);
//			m_total = 0;
//
//
//			writer.Key("LpEntries");
//			writer.StartArray();
//				
//				
//				int totalArrayData = m_IpPriotityArray.Total();
//				for (int td = 0; td < totalArrayData; td++)
//				{
//					st_IpPriotity m_st_IpPriotity = {};
//					m_st_IpPriotity = m_IpPriotityArray[td];
//					CString t_symbolgroup = m_st_IpPriotity.m_symbolgroup;
//					CString t_lpname = m_st_IpPriotity.m_lpname;
//					CString t_lpsymbol = m_st_IpPriotity.m_lpsymbol;
//					int t_lppriority = m_st_IpPriotity.m_lppriority;
//					double t_lpmaxlots = m_st_IpPriotity.m_lpmaxlots;
//					CString t_lpsendingtype = m_st_IpPriotity.m_lpsendingtype;
//					double t_lpratio = m_st_IpPriotity.m_lpratio;
//					if (t_symbolgroup.MakeUpper() == m_symbolgroup.MakeUpper())
//					{
//						writer.StartObject();
//						writer.Key("LpName");
//						string sslpname = string(CT2CA(t_lpname));
//						const char* stlpname = sslpname.c_str();
//						writer.String(stlpname);
//						writer.Key("LpSymbol");
//						string sslpsymbol = string(CT2CA(t_lpsymbol));
//						const char* stlpsymbol = sslpsymbol.c_str();
//						writer.String(stlpsymbol);
//						writer.Key("LpPriority");
//						writer.Int(t_lppriority);
//						writer.Key("LpMaxLots");
//						writer.Double(t_lpmaxlots);
//						writer.Key("LpSendingType");
//						string ssIpSendingType = string(CT2CA(t_lpsendingtype));
//						const char* stlpsendingtype = ssIpSendingType.c_str();
//						writer.String(stlpsendingtype);
//						writer.Key("LpRatio");
//						writer.Double(t_lpratio);
//						writer.EndObject();
//					}
//				}
//				
//			writer.EndArray();
//
//
//			
//			writer.Key("ratiodefinition");
//			writer.StartArray();
//			writer.StartObject();
//			writer.Key("lots");
//			writer.Double(m_lots);
//			writer.Key("ratio");
//			writer.Double(m_ratio);
//			writer.Key("stepup");
//			writer.Double(m_stepup);
//			writer.Key("perfect");
//			writer.Double(m_perfect);
//			m_total = m_total + m_perfect;
//			writer.Key("rounded");
//			writer.Double(m_rounded);
//			writer.EndObject();			
//		}
//		else
//		{
//			writer.StartObject();
//			writer.Key("lots");
//			writer.Double(m_lots);
//			writer.Key("ratio");
//			writer.Double(m_ratio);
//			writer.Key("stepup");
//			writer.Double(m_stepup);
//			writer.Key("perfect");
//			writer.Double(m_perfect);
//			writer.Key("rounded");
//			writer.Double(m_rounded);
//			m_total = m_total + m_perfect;
//			writer.EndObject();
//		}				
//		m_oldSymbolGroup = m_symbolgroup;		
//	}
//	if (endobjectFlag == 1)
//	{
//		writer.EndArray();
//		writer.EndObject();
//	}
//	writer.EndArray();
//	writer.EndObject();
//	CStaticClass::session.Close();
//	CStaticClass::m_mutex_order.Unlock();
//
//	CString str_FinalJsonUpdate = L"";
//	strReturnVal = str_FinalJsonUpdate = s.GetString();
//	s.Clear();
//	writer.Flush();
//	return strReturnVal;
//}


CString CManager::GetGATEWAY_PERFECTANDROUNDED_API()
{
	//CStaticClass::m_LpLotsMap
	CStaticClass::m_positiondevideRatio_lock.Lock();
	CString  strReturnVal = L"";

	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("type");
	writer.String("GATEWAY_POSITIONDEVIDERATIO_PERFECT_ROUNDED_UPDATE_API");

	writer.Key("data");
	writer.StartArray();
	
	int endobjectFlag = 0;


	double m_Total_Perfect = 0;

	double m_Total_Rounded = 0;
	//CStaticClass::m_LpLotsMap

	POSITION pos = CStaticClass::m_LpLotsMap.GetStartPosition();
	while (pos != NULL)
	{
		CString m_symbolgroup = L"";
		CStaticClass::st_LpLotsArray m_LpLotArray;
		CStaticClass::m_LpLotsMap.GetNextAssoc(pos, m_symbolgroup, m_LpLotArray);

		writer.StartObject();
		writer.Key("symbolgroup");
		string sssymbolgroup = string(CT2CA(m_symbolgroup));
		const char* stsymbolgroup = sssymbolgroup.c_str();
		writer.String(stsymbolgroup);				

		
		
		int dataCount = m_LpLotArray.Total();
		int Count0 = 0;
		for (int dc = 0; dc < dataCount; dc++)
		{
			CStaticClass::st_LpLots   m_st_LpLots = {};
			m_st_LpLots = m_LpLotArray[dc];			
			double m_perfect = m_st_LpLots.m_perfect;
			double m_rounded = m_st_LpLots.m_rounded;			
			m_Total_Perfect = m_Total_Perfect + m_perfect;
			m_Total_Rounded = m_Total_Rounded + m_rounded;						
		}		
		writer.Key("perfecttotal");
		writer.Double(m_Total_Perfect);
		writer.Key("roundedtotal");
		writer.Double(m_Total_Rounded);
		m_Total_Perfect = 0;
		m_Total_Rounded = 0;
		writer.EndObject();
	}
	writer.EndArray();
	writer.EndObject();
	CSession m_tempSession;
	m_tempSession.Close();
	 

	CString str_FinalJsonUpdate = L"";
	strReturnVal = str_FinalJsonUpdate = s.GetString();
	s.Clear();
	writer.Flush();
	CStaticClass::m_positiondevideRatio_lock.Unlock();
	return strReturnVal;
}





CString CManager::GetGATEWAY_PERFECTANDROUNDED()
{
	//CStaticClass::m_LpLotsMap
	CStaticClass::m_positiondevideRatio_lock.Lock();
	CString  strReturnVal = L"";

	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("type");
	writer.String("GATEWAY_POSITIONDEVIDERATIO_PERFECT_ROUNDED_UPDATE");

	writer.Key("data");
	writer.StartArray();
	CString m_oldSymbolGroup = L"";
	int endobjectFlag = 0;


	double m_Total_Perfect = 0;

	double m_Total_Rounded = 0;
	//CStaticClass::m_LpLotsMap

	POSITION pos = CStaticClass::m_LpLotsMap.GetStartPosition();
	while (pos != NULL)
	{
		CString m_symbolgroup = L"";
		CStaticClass::st_LpLotsArray m_LpLotArray;
		CStaticClass::m_LpLotsMap.GetNextAssoc(pos, m_symbolgroup, m_LpLotArray);

		writer.StartObject();
		writer.Key("symbolgroup");
		string sssymbolgroup = string(CT2CA(m_symbolgroup));
		const char* stsymbolgroup = sssymbolgroup.c_str();
		writer.String(stsymbolgroup);

		double m_totalCompanyVolumeAfterMulti = 0;
		CStaticClass::m_comTotalLotsAfterMultiGroupWise.Lookup(m_symbolgroup, m_totalCompanyVolumeAfterMulti);
		writer.Key("companytotalvolumeaftermulti");
		writer.Double(m_totalCompanyVolumeAfterMulti);


		double m_totalLPvolume = 0;
		CStaticClass::m_LPLots.Lookup(m_symbolgroup, m_totalLPvolume);
		writer.Key("lpvolumetotal");
		writer.Double(m_totalLPvolume);



		writer.Key("lastupdatedtime");
		CStaticClass m_staticClass;
		CString strCurrentDate = m_staticClass.getcurrentTimeStr();
		string ssCurrentDate = string(CT2CA(strCurrentDate));
		const char* ststrCurrentDate = ssCurrentDate.c_str();
		writer.String(ststrCurrentDate);

		writer.Key("LpEntries");
		writer.StartArray();

		CStaticClass::st_LpDetailsArray m_tempArray;
		CStaticClass::m_LpDetailMap.Lookup(m_symbolgroup, m_tempArray);
		int totalArrayData = m_tempArray.Total();
		for (int td = 0; td < totalArrayData; td++)
		{
			CStaticClass::st_LpDetails m_st_IpPriotity = {};
			m_st_IpPriotity = m_tempArray[td];
			CString t_symbolgroup = m_symbolgroup;
			CString t_lpname = m_st_IpPriotity.m_lpname;
			CString t_lpsymbol = m_st_IpPriotity.m_lpsymbol;
			int t_lppriority = m_st_IpPriotity.m_lppriority;
			double t_lpmaxlots = m_st_IpPriotity.m_lpmaxlots;
			CString t_lpsendingtype = m_st_IpPriotity.m_lpsendingtype;
			double t_lpratio = m_st_IpPriotity.m_lpratio;


			writer.StartObject();
			writer.Key("LpName");
			string sslpname = string(CT2CA(t_lpname));
			const char* stlpname = sslpname.c_str();
			writer.String(stlpname);
			writer.Key("LpSymbol");
			string sslpsymbol = string(CT2CA(t_lpsymbol));
			const char* stlpsymbol = sslpsymbol.c_str();
			writer.String(stlpsymbol);
			writer.Key("LpPriority");
			writer.Int(t_lppriority);
			writer.Key("LpMaxLots");
			writer.Double(t_lpmaxlots);
			writer.Key("LpSendingType");
			string ssIpSendingType = string(CT2CA(t_lpsendingtype));
			const char* stlpsendingtype = ssIpSendingType.c_str();
			writer.String(stlpsendingtype);
			writer.Key("LpRatio");
			writer.Double(t_lpratio);
			writer.EndObject();

		}

		writer.EndArray();



		writer.Key("ratiodefinition");
		writer.StartArray();
		int dataCount = m_LpLotArray.Total();
		int Count0 = 0;
		for (int dc = 0; dc < dataCount; dc++)
		{
			CStaticClass::st_LpLots   m_st_LpLots = {};
			m_st_LpLots = m_LpLotArray[dc];

			double m_lots = m_st_LpLots.m_lots;
			double m_lots_to = m_st_LpLots.m_lots_To;
			double m_ratio = m_st_LpLots.m_LpPer;
			double m_stepup = m_st_LpLots.m_stepup;
			double m_perfect = m_st_LpLots.m_perfect;
			double m_rounded = m_st_LpLots.m_rounded;
			
				writer.StartObject();
				writer.Key("lots");
				writer.Double(m_lots_to);
				writer.Key("ratio");
				writer.Double(m_ratio);
				writer.Key("stepup");
				writer.Double(m_stepup);
				writer.Key("perfect");
				writer.Double(m_perfect);
				writer.Key("rounded");
				writer.Double(m_rounded);
				m_Total_Perfect = m_Total_Perfect + m_perfect;
				m_Total_Rounded = m_Total_Rounded + m_rounded;
				writer.EndObject();
			
			m_oldSymbolGroup = m_symbolgroup;
		}
		writer.EndArray();

		writer.Key("perfecttotal");
		writer.Double(m_Total_Perfect);
		writer.Key("roundedtotal");
		writer.Double(m_Total_Rounded);
		m_Total_Perfect = 0;
		m_Total_Rounded = 0;
		writer.EndObject();

	}
	writer.EndArray();
	writer.EndObject();
	
	CString str_FinalJsonUpdate = L"";
	strReturnVal = str_FinalJsonUpdate = s.GetString();
	s.Clear();
	writer.Flush();
	CStaticClass::m_positiondevideRatio_lock.Unlock();
	return strReturnVal;
}

CString CManager::GetGATEWAY_POSITIONDEVIDERATIO_UPDATE()
{
	//CStaticClass::m_LpLotsMap
	CStaticClass::m_positiondevideRatio_lock.Lock();
	CString  strReturnVal = L"";	
	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("type");
	writer.String("GATEWAY_POSITIONDEVIDERATIO_UPDATE");
	writer.Key("data");
	writer.StartArray();
	CString m_oldSymbolGroup = L"";
	int endobjectFlag = 0;


	double m_Total_Perfect = 0;

	double m_Total_Rounded = 0;
	//CStaticClass::m_LpLotsMap

	POSITION pos = CStaticClass::m_LpLotsMap.GetStartPosition();
	while (pos != NULL)
	{
		CString m_symbolgroup = L"";
		CStaticClass::st_LpLotsArray m_LpLotArray;				
		CStaticClass::m_LpLotsMap.GetNextAssoc(pos, m_symbolgroup, m_LpLotArray);

		writer.StartObject();
		writer.Key("symbolgroup");
		string sssymbolgroup = string(CT2CA(m_symbolgroup));
		const char* stsymbolgroup = sssymbolgroup.c_str();
		writer.String(stsymbolgroup);



		double m_totalCompanyVolumeAfterMulti = 0;
		CStaticClass::m_comTotalLotsAfterMultiGroupWise.Lookup(m_symbolgroup, m_totalCompanyVolumeAfterMulti);
		writer.Key("companytotalvolumeaftermulti");
		writer.Double(m_totalCompanyVolumeAfterMulti);


		double m_totalLPvolume = 0;
		CStaticClass::m_LPLots.Lookup(m_symbolgroup, m_totalLPvolume);
		writer.Key("lpvolumetotal");
		writer.Double(m_totalLPvolume);

		writer.Key("lastupdatedtime");
		CStaticClass m_staticClass;
		CString strCurrentDate = m_staticClass.getcurrentTimeStr();
		string ssCurrentDate = string(CT2CA(strCurrentDate));
		const char* ststrCurrentDate = ssCurrentDate.c_str();
		writer.String(ststrCurrentDate);







		writer.Key("LpEntries");
		writer.StartArray();
				
		CStaticClass::st_LpDetailsArray m_tempArray;
		CStaticClass::m_LpDetailMap.Lookup(m_symbolgroup, m_tempArray);
		int totalArrayData = m_tempArray.Total();
		for (int td = 0; td < totalArrayData; td++)
		{
			CStaticClass::st_LpDetails m_st_IpPriotity = {};
			m_st_IpPriotity = m_tempArray[td];
			CString t_symbolgroup = m_symbolgroup;
			CString t_lpname = m_st_IpPriotity.m_lpname;
			CString t_lpsymbol = m_st_IpPriotity.m_lpsymbol;
			int t_lppriority = m_st_IpPriotity.m_lppriority;
			double t_lpmaxlots = m_st_IpPriotity.m_lpmaxlots;
			CString t_lpsendingtype = m_st_IpPriotity.m_lpsendingtype;
			double t_lpratio = m_st_IpPriotity.m_lpratio;
			
				writer.StartObject();
				writer.Key("LpName");
				string sslpname = string(CT2CA(t_lpname));
				const char* stlpname = sslpname.c_str();
				writer.String(stlpname);
				writer.Key("LpSymbol");
				string sslpsymbol = string(CT2CA(t_lpsymbol));
				const char* stlpsymbol = sslpsymbol.c_str();
				writer.String(stlpsymbol);
				writer.Key("LpPriority");
				writer.Int(t_lppriority);
				writer.Key("LpMaxLots");
				writer.Double(t_lpmaxlots);
				writer.Key("LpSendingType");
				string ssIpSendingType = string(CT2CA(t_lpsendingtype));
				const char* stlpsendingtype = ssIpSendingType.c_str();
				writer.String(stlpsendingtype);
				writer.Key("LpRatio");
				writer.Double(t_lpratio);
				writer.EndObject();
			
		}				
		writer.EndArray();
		writer.Key("ratiodefinition");
		writer.StartArray();
		int dataCount = m_LpLotArray.Total();

		int Count0 = 0;

		for (int dc=0;dc<dataCount;dc++)
		{
			CStaticClass::st_LpLots   m_st_LpLots = {};
			m_st_LpLots = m_LpLotArray[dc];

			double m_lots = m_st_LpLots.m_lots;
			double m_lots_to = m_st_LpLots.m_lots_To;
			double m_ratio = m_st_LpLots.m_LpPer;
			double m_stepup = m_st_LpLots.m_stepup;
			double m_perfect = m_st_LpLots.m_perfect;
			double m_rounded = m_st_LpLots.m_rounded;						
			
				writer.StartObject();
				writer.Key("lots");
				writer.Double(m_lots_to);
				writer.Key("ratio");
				writer.Double(m_ratio);
				writer.Key("stepup");
				writer.Double(m_stepup);
				writer.Key("perfect");
				writer.Double(m_perfect);
				writer.Key("rounded");
				writer.Double(m_rounded);
				m_Total_Perfect = m_Total_Perfect + m_perfect;
				m_Total_Rounded = m_Total_Rounded + m_rounded;
				writer.EndObject();
			
			
			m_oldSymbolGroup = m_symbolgroup;
		}
		writer.EndArray();

		writer.Key("perfecttotal");
		writer.Double(m_Total_Perfect);
		writer.Key("roundedtotal");
		writer.Double(m_Total_Rounded);
		m_Total_Perfect = 0;
		m_Total_Rounded = 0;
		writer.EndObject();

	}
	writer.EndArray();
	writer.EndObject();		

	CString str_FinalJsonUpdate = L"";
	strReturnVal = str_FinalJsonUpdate = s.GetString();
	s.Clear();
	writer.Flush();
	CStaticClass::m_positiondevideRatio_lock.Unlock();
	return strReturnVal;
}



void CManager::deletePage(CString pageId)
{	
	CString strCommand = L"";
	strCommand.Format(L"delete from orika_page_details where pageId='%s'", pageId);
	CStaticClass::m_sqldata.executeCommand(strCommand);	
}



void CManager::SaveClosingRate()
{
	UINT64 m_time = 0;
	CStaticClass m_staticClass;
	m_time = m_staticClass.getcurrentTime_Unix();

	POSITION pos = CStaticClass::m_ignoreClosingprice.GetStartPosition();
	while (pos != NULL)
	{		
		CString strSymbol = L"";
		double clrate = 0;
		CStaticClass::m_ignoreClosingprice.GetNextAssoc(pos, strSymbol, clrate);
		
		if (clrate != 0)
		{
			CString strCommand = L"";
			strCommand.Format(L"exec update_closing '%s' ,'%.4lf'", strSymbol, clrate);
			CStaticClass::m_sqldata.executeCommand(strCommand);

			SYSTEMTIME readable_date;
			readable_date = SMTTime::TimeToST(m_time, readable_date);
			int int_dayofweek = readable_date.wDayOfWeek;
			if (int_dayofweek == 0 || int_dayofweek == 6)
			{				
				MTTick   newtick = {};
				CMTStr::Copy(newtick.symbol, strSymbol);
				newtick.ask = clrate;
				newtick.bid = clrate;
				newtick.last = clrate;

				CString strsymbol = strSymbol.Trim();
				m_manager->TickAdd(strsymbol, newtick);
			}
		}
	}

}


CString CManager::getUpdatedClosingRate()
{
	CString  strReturnVal = L"";
	HRESULT hr = NULL;
	CCommand<CAccessor<Orika_Closingprice>> data_table;
	if (!SUCCEEDED(hr))
	{
		return L"";
	}
	CString   strCommand = L"";
	strCommand.Format(L"select symbol,clrate,Hrate,Lrate from orika_closingprice;");
	 
	//(L"Orderlock_71");
	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
	if (FAILED(hr))
	{
		m_tempSession.Close();
		 
		return L"";
	}
	int i = 0;
	
	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("type");
	writer.String("CLOSINGPRICE");

	writer.Key("data");
	writer.StartArray();


	while (hr = data_table.MoveNext() == S_OK)
	{
		
		CString m_symbol = data_table.m_MTSymbol;
		double m_clrate = data_table.m_closing;
		double m_heigh = data_table.m_heigh;
		double m_low = data_table.m_low;
		writer.StartObject();
			writer.Key("symbol");
			string ssSymbol = string(CT2CA(m_symbol));
			const char* stSymbol = ssSymbol.c_str();
			writer.String(stSymbol);
			writer.Key("closingPrice");
			writer.Double(m_clrate);
			writer.Key("high");
			writer.Double(m_heigh);
			writer.Key("low");
			writer.Double(m_low);
		writer.EndObject();

	}
	writer.EndArray();
	writer.EndObject();
	m_tempSession.Close();
	 
	//(L"U_Orderlock_71");
	CString str_FinalJsonUpdate = L"";
	strReturnVal=str_FinalJsonUpdate = s.GetString();
	s.Clear();
	writer.Flush();
	return strReturnVal;
}



string CManager::gethighLowMismatchDeals(CString dealDate)
{
	string  strReturnVal = "";
	HRESULT hr = NULL;
	CCommand<CAccessor<Orika_HighLowMismatchDataTable>> data_table;
	if (!SUCCEEDED(hr))
	{
		return "";
	}
	CString   strCommand = L"";
	strCommand.Format(L"select dealdate,deal,[order],[login],symbol,rate,comment from orika_HeighLowMismatchTrades where replace(convert(varchar(11),dateadd(s,[dealdate],'01-01-1970'),106 ),' ','-')='%s';", dealDate);
	 
	CSession m_tempSession;
	//(L"Orderlock_72");
	m_tempSession.Open(CStaticClass::connection);
	hr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
	if (FAILED(hr))
	{
		m_tempSession.Close();
		 
		return "";
	}
	int i = 0;

	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("type");
	writer.String("HIGH_LOW_MISMATCH_DATA");

	writer.Key("data");
	writer.StartArray();
	
	while (hr = data_table.MoveNext() == S_OK)
	{

		int		m_dealdate= data_table.m_dealdate;
		int		m_deal= data_table.m_deal;
		int		m_order= data_table.m_order;
		int		m_login= data_table.m_login;
		CString   m_symbol= data_table.m_symbol;
		double 	m_rate= data_table.m_rate;
		CString	m_comment= data_table.m_comment;
		
		writer.StartObject();
		writer.Key("dealdate");
		writer.Int(m_dealdate);
		writer.Key("deal");
		writer.Int(m_deal);
		writer.Key("order");
		writer.Int(m_order);
		writer.Key("login");
		writer.Int(m_login);
		writer.Key("symbol");
		string ssSymbol = string(CT2CA(m_symbol));
		const char* stSymbol = ssSymbol.c_str();
		writer.String(stSymbol);
		writer.Key("rate");
		writer.Double(m_rate);		
		writer.Key("comment");
		string ssComment = string(CT2CA(m_comment));
		const char* stComment = ssComment.c_str();
		writer.String(stComment);
		
		writer.EndObject();

	}
	writer.EndArray();
	writer.EndObject();
	m_tempSession.Close();
	 
	//(L"U_Orderlock_72");
	CString str_FinalJsonUpdate = L"";
	str_FinalJsonUpdate = s.GetString();
	s.Clear();
	writer.Flush();
	string strforsend = CT2A(str_FinalJsonUpdate.GetString());
	return strforsend;
}




CString CManager::CSVClosingfileTransfer(CString StrFilePath, CString closingDate)
{
	CStaticClass::m_allClosingprice.RemoveAll();
	CString returnVal = L"";
	CString strval = L"";
	CStdioFile	myFile;
	if (myFile.Open(StrFilePath, CFile::modeRead))
	{
		myFile.Seek(0, CFile::begin);
		CString Insert_Command = L"";
		int data_count = 0;
		StringBuffer s;
		Writer<StringBuffer> writer(s);
		writer.StartObject();
		writer.Key("type");
		writer.String("CLOSINGPRICE_LIST");
		writer.Key("data");
		writer.StartArray();
		while (myFile.ReadString(strval))
		{
			strval = strval.Mid(strval.Find(',') + 1, (strval.GetLength() - strval.Find(',') - 1));
			CString NSEIndexType = strval.Mid(0, strval.Find(','));
			strval = strval.Mid(strval.Find(',') + 1, (strval.GetLength() - strval.Find(',') - 1));
			CString NseSymbol = strval.Mid(0, strval.Find(','));
			strval = strval.Mid(strval.Find(',') + 1, (strval.GetLength() - strval.Find(',') - 1));
			CString NseExp = strval.Mid(0, strval.Find(','));
			strval = strval.Mid(strval.Find(',') + 1, (strval.GetLength() - strval.Find(',') - 1));
			NseExp = NseExp.Mid(0, 2) + NseExp.Mid(3, 3) + NseExp.Mid(7, 4);
			CString NSEContract = NSEIndexType + NseSymbol + NseExp;
			strval = strval.Mid(strval.Find(',') + 1, (strval.GetLength() - strval.Find(',') - 1));
			strval = strval.Mid(strval.Find(',') + 1, (strval.GetLength() - strval.Find(',') - 1));
			strval = strval.Mid(strval.Find(',') + 1, (strval.GetLength() - strval.Find(',') - 1));
			strval = strval.Mid(strval.Find(',') + 1, (strval.GetLength() - strval.Find(',') - 1));
			strval = strval.Mid(strval.Find(',') + 1, (strval.GetLength() - strval.Find(',') - 1));
			strval = strval.Mid(strval.Find(',') + 1, (strval.GetLength() - strval.Find(',') - 1));
			strval = strval.Mid(strval.Find(',') + 1, (strval.GetLength() - strval.Find(',') - 1));
			strval = strval.Mid(strval.Find(',') + 1, (strval.GetLength() - strval.Find(',') - 1));
			strval = strval.Mid(strval.Find(',') + 1, (strval.GetLength() - strval.Find(',') - 1));
			strval = strval.Mid(strval.Find(',') + 1, (strval.GetLength() - strval.Find(',') - 1));
			strval = strval.Mid(strval.Find(',') + 1, (strval.GetLength() - strval.Find(',') - 1));
			CString strHeigh = strval.Mid(0, strval.Find(','));
			double Heigh = _wtof(strHeigh);
			strval = strval.Mid(strval.Find(',') + 1, (strval.GetLength() - strval.Find(',') - 1));
			CString strLow = strval.Mid(0, strval.Find(','));
			double Low = _wtof(strLow);
			strval = strval.Mid(strval.Find(',') + 1, (strval.GetLength() - strval.Find(',') - 1));
			strval = strval.Mid(strval.Find(',') + 1, (strval.GetLength() - strval.Find(',') - 1));
			CString Last_Rate = strval.Mid(0, strval.Find(','));
			double rate = _wtof(Last_Rate);
			if (rate != 0 && NSEIndexType.Mid(0,1)==L"F")
			{
				CStaticClass::m_mutex_Tick.Lock();
					CStaticClass::st_Orika_ExchangeSymbol m_tempSt = {};
					CStaticClass::m_Orika_ExchangeSymbolHastable.Lookup(NseSymbol, m_tempSt);				
					CString mt_currentSymbol = m_tempSt.m_MTsymbol;
					CString mt_nextSymbol = m_tempSt.m_NextMTsymbol;
				CStaticClass::m_mutex_Tick.Unlock();

				
				/*SYSTEMTIME	m_closingPriceDate;
				m_closingPriceDate.wDay = 1;
				m_closingPriceDate.wMonth = 1;
				m_closingPriceDate.wYear = 2022;*/
				CString m_MTClosingSymbol = L"";
				if (NseExp.Mid(2, 3).MakeUpper() == closingDate.Mid(3, 3).MakeUpper()   )
				{
					m_MTClosingSymbol = mt_currentSymbol;
				}
				else
				{
					m_MTClosingSymbol = mt_nextSymbol;
				}
				if (m_MTClosingSymbol != L"")
				{
					CStaticClass::m_allClosingprice.SetAt(m_MTClosingSymbol, rate);
					writer.StartObject();
					CString m_MtSymbol = L"";
					writer.Key("mtsymbol");
					string ssMTsymbol = string(CT2CA(m_MTClosingSymbol));
					const char* stMTsymbol = ssMTsymbol.c_str();
					writer.String(stMTsymbol);

					writer.Key("exsymbol");
					string ssexsymbol = string(CT2CA(NseSymbol));
					const char* stexsymbol = ssexsymbol.c_str();
					writer.String(stexsymbol);
					writer.Key("closingprice");
					writer.Double(rate);


					/*CString SymbolDateKey = L"";
					SymbolDateKey.Format(L"%s:%s", m_MTClosingSymbol, closingDate);


					CStaticClass::TradesHighLowLockUpdate.Lock();
					CStaticClass::st_TradesHighLowPriceSymbolWise m_st_TradesHighLowPriceSymbolWise = {};
					CStaticClass::m_TradesHighLowPriceSymbolWise.Lookup(SymbolDateKey, m_st_TradesHighLowPriceSymbolWise);
					if (m_st_TradesHighLowPriceSymbolWise.m_High != 0 && m_st_TradesHighLowPriceSymbolWise.m_Low != 0)
					{
						if (m_st_TradesHighLowPriceSymbolWise.m_High <= Heigh && m_st_TradesHighLowPriceSymbolWise.m_Low >= Low)
						{
							writer.Key("hlmismatch");
							writer.Bool(false);
						}
						else
						{
							writer.Key("hlmismatch");
							writer.Bool(true);
							CString strCommandUpdateMismatchTrade = L"";
							strCommandUpdateMismatchTrade.Format(L"exec Insert_HeighLowMismatchTrades '%s',%.4lf,%.4lf,'%s'", m_MTClosingSymbol, Heigh, Low, closingDate);
							CStaticClass::m_sqldata.executeCommand(strCommandUpdateMismatchTrade);
						}
					}
					else
					{
						writer.Key("hlmismatch");
						writer.Bool(false);
					}
					CStaticClass::TradesHighLowLockUpdate.Unlock();*/


					

					writer.EndObject();
				}
			}

		}
		myFile.Close();		

		writer.EndArray();
		writer.EndObject();
		CString str_FinalJsonUpdate = L"";
		str_FinalJsonUpdate = s.GetString();
		s.Clear();
		writer.Flush();
		returnVal = str_FinalJsonUpdate;
	}
	return returnVal;
}



bool CManager::LoadConfigFile(CString StrFilePath)
{
	CString strval = L"";
	CStdioFile	myFile;
	if (myFile.Open(StrFilePath, CFile::modeRead))
	{
		myFile.Seek(0, CFile::begin);		
		while (myFile.ReadString(strval))
		{
			CString str_key = strval.Mid(0, strval.Find(':'));
			strval = strval.Mid(strval.Find(':') + 1, (strval.GetLength() - strval.Find(':') - 1));
			if (str_key == L"orikaPort")
			{
				CStaticClass::orikaPort = strval;
			}
			if (str_key == L"SqlServerAdd")
			{
				CStaticClass::SqlServerAdd = strval;
			}
			if (str_key == L"SqlServerDatabase")
			{
				CStaticClass::SqlServerDatabase = strval;
			}
			if (str_key == L"SqlServerUserID")
			{
				CStaticClass::SqlServerUserID = strval;
			}
			if (str_key == L"SqlServerPassword")
			{
				CStaticClass::SqlServerPassword = strval;
			}
			if (str_key == L"MTServerAdd")
			{
				CStaticClass::MTServerAdd = strval;
			}
			if (str_key == L"MTServerUserID")
			{
				CStaticClass::MTServerUserID = strval;
			}
			if (str_key == L"MTServerPassword")
			{
				CStaticClass::MTServerPassword = strval;
			}
			if (str_key == L"MSMQQueuName")
			{
				CStaticClass::MSMQQueuName = strval;
			}
			if (str_key == L"dataFeedServer")
			{
				CStringA cstrA(strval);				
				CStaticClass::dataFeedServer = cstrA;
			}
			if (str_key == L"GatewayServer")
			{
				CStringA cstrA(strval);
				CStaticClass::GatewayServer = cstrA;
			}
			if (str_key == L"ExchangeCode")
			{
				CStaticClass::ExchangeCode = _wtoi(strval);
			}
			if (str_key == L"APIFolderPath")
			{
				CStaticClass::APIFolderPath = strval;
			}
			if (str_key == L"APISERVER_PORT")
			{
				CStaticClass::APISERVER_PORT = _wtoi(strval);
			}
						
		}
		myFile.Close();
		return true;
	}
	return false;
}




void CManager::SendOrderCountUpdate(CString m_login, CString  m_symbol)
{
	/*StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("type");
	writer.String("CLIENT_POSITION");
	writer.Key("updatekey");
	writer.StartArray();
	writer.String("login");
	writer.String("symbol");
	writer.EndArray();
	writer.Key("update");
	writer.StartArray();
	writer.StartObject();

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
	strloginSymbolKey.Format(L"%s:%s:4", m_login, m_symbol);
	CStaticClass::st_OrderCount m_stOrderCount = {};
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

	
	writer.Key("login");
	string ssLogin = string(CT2CA(m_login));
	const char* stLogin = ssLogin.c_str();
	writer.String(stLogin);
	
	writer.Key("symbol");
	string ssSymbol = string(CT2CA(m_symbol));
	const char* stSymbol = ssSymbol.c_str();
	writer.String(stSymbol);



	writer.Key("TotalNo_Of_StopBuy");
	writer.Int(m_TotalNo_Of_StopBuy);
	writer.Key("TotalNo_Of_Cancelled_StopBuy");
	writer.Int(m_TotalNo_Of_Cancelled_StopBuy);
	writer.Key("TotalNo_Of_Passed_StopBuy");
	writer.Int(m_TotalNo_Of_Passed_StopBuy);
	writer.Key("TotalNo_Of_Pending_StopBuy");
	writer.Int(m_TotalNo_Of_Pending_StopBuy);


	writer.Key("TotalNo_Of_StopSell");
	writer.Int(m_TotalNo_Of_StopSell);
	writer.Key("TotalNo_Of_Cancelled_StopSell");
	writer.Int(m_TotalNo_Of_Cancelled_StopSell);
	writer.Key("TotalNo_Of_Passed_StopSell");
	writer.Int(m_TotalNo_Of_Passed_StopSell);
	writer.Key("TotalNo_Of_Pending_StopSell");
	writer.Int(m_TotalNo_Of_Pending_StopSell);

	writer.Key("TotalNo_Of_BuyLimit");
	writer.Int(m_TotalNo_Of_BuyLimit);
	writer.Key("TotalNo_Of_Cancelled_BuyLimit");
	writer.Int(m_TotalNo_Of_Cancelled_BuyLimit);
	writer.Key("TotalNo_Of_Passed_BuyLimit");
	writer.Int(m_TotalNo_Of_Passed_BuyLimit);
	writer.Key("TotalNo_Of_Pending_BuyLimit");
	writer.Int(m_TotalNo_Of_Pending_BuyLimit);

	writer.Key("TotalNo_Of_SellLimit");
	writer.Int(m_TotalNo_Of_SellLimit);
	writer.Key("TotalNo_Of_Cancelled_SellLimit");
	writer.Int(m_TotalNo_Of_Cancelled_SellLimit);
	writer.Key("TotalNo_Of_Passed_SellLimit");
	writer.Int(m_TotalNo_Of_Passed_SellLimit);
	writer.Key("TotalNo_Of_Pending_SellLimit");
	writer.Int(m_TotalNo_Of_Pending_SellLimit);


	writer.Key("TotalNo_Of_MarketDeal");
	writer.Int(m_TotalNo_Of_MarketDeal);
	writer.Key("TotalNo_Of_LimitDeal");
	writer.Int(m_TotalNo_Of_LimitDeal);
	writer.Key("TotalNo_Of_StopLimitDeal");
	writer.Int(m_TotalNo_Of_StopLimitDeal);
	


	


	writer.EndObject();			
	writer.EndArray();
	writer.EndObject();
	CString str_FinalJsonUpdate = L"";
	string strforsend = "";
	str_FinalJsonUpdate = s.GetString();
	strforsend = CT2A(str_FinalJsonUpdate.GetString());
	CStaticClass objCStaticClass;
	
	objCStaticClass.sendDataToAllClient(strforsend);*/
}
bool CManager::Initialize()
  {
   MTAPIRES res    =MT_RET_OK_NONE;
   UINT     version=0;
   CString  message;
//--- check
   //if(parent==NULL)return(false);
//--- parent window
//--- initialize factory
   // I11: every early-return below now calls m_factory.Shutdown() (which is
   // guarded internally by `if(m_hmodule)` and is safe to call even on failed
   // Initialize). Previously three paths leaked the loaded MT5 DLL handle.
   // The Format() messages are also now logged - previously they were
   // computed and thrown away.
   if((res=m_factory.Initialize(L"MT5\\Manager\\API\\"))!=MT_RET_OK)
     {
      message.Format(L"CManager::Initialize: m_factory.Initialize failed (%u)",res);
      CStaticClass::m_logfile.LogEvent(message);
      m_factory.Shutdown();
      return(false);
     }


//--- check Manager API version
   if((res=m_factory.Version(version))!=MT_RET_OK)
     {
      message.Format(L"CManager::Initialize: m_factory.Version failed (%u)",res);
      CStaticClass::m_logfile.LogEvent(message);
      m_factory.Shutdown();
      return(false);
     }
   if(version!=MTManagerAPIVersion)
     {
      message.Format(L"CManager::Initialize: wrong MT5 Manager API version (got %u, expected %u)",version,MTManagerAPIVersion);
      CStaticClass::m_logfile.LogEvent(message);
      m_factory.Shutdown();
      return(false);
     }
//--- create manager interface
   if((res=m_factory.CreateManager(MTManagerAPIVersion,&m_manager))!=MT_RET_OK)
     {
      message.Format(L"CManager::Initialize: CreateManager failed (%u)",res);
      CStaticClass::m_logfile.LogEvent(message);
      m_factory.Shutdown();
      return(false);
     }

   if((res=m_factory.CreateAdmin(MTManagerAPIVersion,&m_admin))!=MT_RET_OK)
     {
      message.Format(L"CManager::Initialize: CreateAdmin failed (%u)",res);
      CStaticClass::m_logfile.LogEvent(message);
      // Release the manager we just created before shutting down the factory.
      if (m_manager) { m_manager->Release(); m_manager = NULL; }
      m_factory.Shutdown();
      return(false);
     }
//--- done


   return(true);
  }

void CManager::Shutdown()
  {
   if(m_manager)
   {	
	m_manager->TickUnsubscribe(this);
	m_manager->DealUnsubscribe(this);
	m_manager->OrderUnsubscribe(this);
	m_manager->UserUnsubscribe(this);
	m_manager->SymbolUnsubscribe(this);
	m_manager->GroupUnsubscribe(this);

	m_manager->Release();
	m_manager=NULL;
   }
	m_factory.Shutdown();
  }



void CManager::DeviceLogTransfer(INT64 fromTime, INT64 toTime, CString strDealSearch)
{
	MTLogRecord* logRecords;
	UINT Total_Record = 0;
	/*int search_count=CManager::m_SearchString.Total();
	for(int s=0;s<search_count;s++)
	{*/
	//CString SearchStr=CManager::m_SearchString[s];
	m_manager->LoggerServerRequest(2, 0, fromTime, toTime, strDealSearch, logRecords, Total_Record);
	for (int i = previousRecordCount; i < Total_Record; i++)
	{
		CString strLogFile = L"";
		CString strMessage = L"";
		CString Ipaddress = L"";
		strMessage = logRecords[i].message;
		UINT64 strLogTime = logRecords[i].datetime_msc;
		Ipaddress = logRecords[i].source;
		


		if (strLogTime != 0)
		{
			CMTStr256 strtmp;
			CString strTime = L"";
			strTime = SMTFormat::FormatTimeMsc(strtmp, strLogTime, true);
			// '1017': deal performed ['200011' #4035895 buy 22 GOLDAUG at 48046.00]
			//

			if (strMessage.Find(L"login (Client") >= 0 || strMessage.Find(L"login as investor (Client") >= 0)
			{
				strLogFile.Format(L"%s %s", strTime, strMessage);
				CString str_login = strMessage.Mid(1, strMessage.Find(L"':") - 1);
				CString strRem = strMessage.Mid(strMessage.Find(L"cid: ") + 5, strMessage.GetLength() - strMessage.Find(L"cid: ") - 5);
				CString str_CID = strRem.Mid(0, strRem.Find(L", point"));


				//(Ipaddress+L","+ str_login+L","+ str_CID);

				CStaticClass::st_logindevice m_st_logindevice = {};

				
				
				m_st_logindevice.m_time = logRecords[i].datetime;
				CMTStr::Copy(m_st_logindevice.m_ip, Ipaddress);
				CMTStr::Copy(m_st_logindevice.m_login, str_login);
				CMTStr::Copy(m_st_logindevice.m_deviceID, str_CID);
				CStaticClass::m_mutexLog.Lock();
				CStaticClass::m_logindeviceArray.Add(&m_st_logindevice);
				CStaticClass::m_mutexLog.Unlock();

				CString ExHD = L"";
				//CManager::HdDataLoginWise.Lookup(str_login, ExHD);

				if (ExHD.Find(str_CID) >= 0)
				{
				}
				else
				{
					if (ExHD.Trim() != "")
					{
						ExHD = ExHD + " ," + str_CID;
					}
					else
					{
						ExHD = str_CID;
					}
					//CManager::HdDataLoginWise.SetAt(str_login, ExHD);
				}

				//Adding Hd wise Data
				CString strEXlogin = L"";
				//CManager::LoginHdDataWise.Lookup(str_CID, strEXlogin);

				if (strEXlogin.Find(str_login) >= 0)
				{
				}
				else
				{
					if (strEXlogin.Trim() != "")
					{
						strEXlogin = strEXlogin + " ," + str_login;
					}
					else
					{
						strEXlogin = str_login;
					}
					//CManager::LoginHdDataWise.SetAt(str_CID, strEXlogin);
				}
			}
		}
	}
	previousRecordCount = Total_Record;

	//}
	m_manager->Free(logRecords);
}











void CManager::SymbolSubscribeForTick(CString symbol)
{
	m_manager->SelectedAdd(symbol);
}

int CManager::login(CString server,CString login,CString password)
{
	if( m_manager->Connect(server,_wtoi64(login),password,L"",IMTManagerAPI::PUMP_MODE_FULL,5000000))
     {      
      return 0;	  
     }	

	if( m_admin->Connect(server,_wtoi64(login),password,L"",IMTManagerAPI::PUMP_MODE_FULL,5000000))
     {      
      return 0;	  
     }

	 if(m_manager->TickSubscribe(this)!=MT_RET_OK)
	 {
		return(false);
	 }	
	 //m_manager->SelectedDeleteAll();

	 POSITION pos = CStaticClass::m_symbolForTickData.GetStartPosition();
	 while (pos != NULL)
	 {
		 CString strKey = L"";
		 CString strSymbol = L"";
		 CStaticClass::m_symbolForTickData.GetNextAssoc(pos, strKey, strSymbol);
		 if (strSymbol != L"")
		 {
			 m_manager->SelectedAdd(strSymbol);
		 }
	 }	 

	
	 m_manager->DealSubscribe(this);
	 m_manager->OrderSubscribe(this);
	 m_manager->UserSubscribe(this);
	 m_manager->SymbolSubscribe(this);
	 m_manager->GroupSubscribe(this);
	 return 1;
}
 

 
 void CManager::loadLastTickFromMT()
 {
	 int totalSymbolCount=CStaticClass::m_symbolmasterarray.Total();
	 for (int i=0;i<totalSymbolCount;i++)
	 {		
		CStaticClass::stsymbolmaster m_stsymbolmaster={};	
		m_stsymbolmaster=CStaticClass::m_symbolmasterarray[i];
		CString  m_symbol=m_stsymbolmaster.m_symbol ;		

		

		MTTickShort tick ={};
		m_manager->TickLast(m_symbol,tick);


		CStaticClass::st_Tick m_st_Tick={};
		CMTStr::Copy(m_st_Tick.m_symbol,m_symbol);
		m_st_Tick.m_bid=tick.bid ;
		m_st_Tick.m_ask=tick.ask ;
		m_st_Tick.m_last=tick.last ;
		m_st_Tick.m_datetime=tick.datetime;
		CStaticClass::m_mutex_Tick.Lock();
		CStaticClass::st_Orika_symbolmaster m_tmpst={};
		CStaticClass::m_Orika_symbolmasterHastable.Lookup(m_symbol,m_tmpst);
		CString m_symbolforcheck=m_tmpst.m_symbol; 

		
		if (m_symbolforcheck!=L"")
		{
			//Checking Old Data 
			int tickAddressIndex=-1; 
			CStaticClass::symbolLastTickArrayIndex.Lookup(m_symbol,tickAddressIndex);	
			if (tickAddressIndex==-1)
			{
				CStaticClass::symbolLastTickArrayIndex.SetAt(m_symbol,CStaticClass::lastrateArrayMaxindex);
				tickAddressIndex=CStaticClass::lastrateArrayMaxindex;
				CStaticClass::lastrateArrayMaxindex=CStaticClass::lastrateArrayMaxindex+1;
				
			}
			
			CStaticClass::lastrateArray[tickAddressIndex]->m_ask=m_st_Tick.m_ask;
			CStaticClass::lastrateArray[tickAddressIndex]->m_bid = m_st_Tick.m_bid;
			CStaticClass::lastrateArray[tickAddressIndex]->m_last = m_st_Tick.m_last;
			 CStaticClass::UpdatedsymbolLastTickArrayIndex.SetAt(m_symbol,tickAddressIndex);
			
		}
		CStaticClass::m_mutex_Tick.Unlock();	
	 }

 }
 void  CManager::UpdatePendingOrder()
 {

 }

 void  CManager::UpdatePendingSaudaToMSMQ()
 {	 
		 INT64       server_time = m_manager->TimeServer();
		 INT64       to = server_time;
		 INT64       from = server_time - (60 * 60 * 24 * 365);
		 INT64		INT_LOGIN = 0;
		 UINT totalNoOfPosition = 0;
		 int r_count = 0;
		 IMTConGroup* group;
		 MTAPIRES res;
		 group = m_manager->GroupCreate();

		 for (UINT i = 0; m_manager->GroupNext(i, group) == MT_RET_OK; i++)
		 {
			 UINT64* logins = NULL;
			 UINT    logins_total = 0;
			 if (res = m_manager->UserLogins(group->Group(), logins, logins_total) == MT_RET_OK)
			 {				 
				 IMTOrder* order = m_manager->OrderCreate();
				 IMTOrderArray* orders = m_manager->OrderCreateArray();
				 IMTDealArray* deals = m_manager->DealCreateArray();
				 IMTDeal* deal = m_manager->DealCreate();				 
				 if (logins && logins_total)
					 for (UINT j = 0; j < logins_total; j++)
					 {
						 INT_LOGIN = logins[j];
						 
						 if (res = m_manager->OrderRequestOpen(logins[j], orders) == MT_RET_OK)
						 {
							 r_count = orders->Total();
							 for (UINT p = 0; p < orders->Total(); p++)
							 {
								 order = orders->Next(p);	
								 CStaticClass::st_order m_st_st_order = {};
								 UINT64 intlogin = order->Login();
								 CString strlogin = L"";
								 strlogin.Format(L"%I64u", intlogin);
								 int m_order = order->Order();
								 CString strsymbol = order->Symbol();
								 CString strcomment = order->Comment();
								 CString strstatus = L"";
								 CString strselecttype = L"";
								 CString strsubtype = L"";
								 CString strourcomment = L"";


								 CMTStr::Copy(m_st_st_order.m_login, strlogin);
								 m_st_st_order.m_time = order->TimeSetup();
								 m_st_st_order.m_deal = 0;
								 m_st_st_order.m_order = m_order;
								 CMTStr::Copy(m_st_st_order.m_symbol, strsymbol);
								 m_st_st_order.m_type = order->Type();
								 m_st_st_order.m_volume = order->VolumeInitial();

								 double m_realvolume = m_st_st_order.m_volume / 10000;
								 m_st_st_order.m_volume = m_realvolume;

								 m_st_st_order.m_price = order->PriceOrder();
								 CMTStr::Copy(m_st_st_order.m_comment, strcomment);
								 CMTStr::Copy(m_st_st_order.m_status, strstatus);
								 m_st_st_order.m_select = 0;
								 CMTStr::Copy(m_st_st_order.m_selecttype, strselecttype);
								 CMTStr::Copy(m_st_st_order.m_subtype, strsubtype);
								 m_st_st_order.m_contraorder = 0;
								 m_st_st_order.m_tradeexecutetime = 0;
								 CMTStr::Copy(m_st_st_order.m_ourcomment, strourcomment);
								 m_st_st_order.m_orderstate = 1001;



								 CStaticClass::st_order m_stOrder_Check = {};
								 CStaticClass::m_Orika_orderHastable.Lookup(m_order, m_stOrder_Check);
								 if (m_stOrder_Check.m_order == 0)
								 {									 
									 CStaticClass::m_Orika_orderHastable.SetAt(m_order, m_st_st_order);									 
								 }

							 }
						 }




						 //Getting Order History
						 if (res = m_manager->HistoryRequest(logins[j], from, to, orders) == MT_RET_OK)
						 {
							 ////(L"Going to add History Order in msmq");
							 r_count = orders->Total();
							 for (UINT p = 0; p < orders->Total(); p++)
							 {
								 order = orders->Next(p);
								 
								 if (order->Type() != 0 && order->Type() != 1)
								 {
									 CStaticClass::st_order m_st_st_order = {};
									 UINT64 intlogin = order->Login();
									 CString strlogin = L"";
									 strlogin.Format(L"%I64u", intlogin);
									 int m_order = order->Order();
									 CString strsymbol = order->Symbol();
									 CString strcomment = order->Comment();
									 CString strstatus = L"";
									 CString strselecttype = L"";
									 CString strsubtype = L"";
									 CString strourcomment = L"";


									 CMTStr::Copy(m_st_st_order.m_login, strlogin);
									 m_st_st_order.m_time = order->TimeSetup();
									 m_st_st_order.m_deal = 0;
									 m_st_st_order.m_order = m_order;
									 CMTStr::Copy(m_st_st_order.m_symbol, strsymbol);
									 m_st_st_order.m_type = order->Type();
									 m_st_st_order.m_volume = order->VolumeInitial();

									 double m_realvolume = m_st_st_order.m_volume / 10000;
									 m_st_st_order.m_volume = m_realvolume;

									 m_st_st_order.m_price = order->PriceOrder();
									 CMTStr::Copy(m_st_st_order.m_comment, strcomment);
									 CMTStr::Copy(m_st_st_order.m_status, strstatus);
									 m_st_st_order.m_select = 0;
									 CMTStr::Copy(m_st_st_order.m_selecttype, strselecttype);
									 CMTStr::Copy(m_st_st_order.m_subtype, strsubtype);
									 m_st_st_order.m_contraorder = 0;
									 m_st_st_order.m_tradeexecutetime = 0;
									 CMTStr::Copy(m_st_st_order.m_ourcomment, strourcomment);
									 m_st_st_order.m_orderstate = 1003;



									 CStaticClass::st_order m_stOrder_Check = {};
									 CStaticClass::m_Orika_orderHastable.Lookup(m_order, m_stOrder_Check);
									 if (m_stOrder_Check.m_order == 0)
									 {
										 CStaticClass m_staticclass;
										 CStaticClass::m_Orika_orderHastable.SetAt(m_order, m_st_st_order);
										 m_staticclass.Sql_UpdateOrika_order(m_st_st_order);
									 }
								 }								 

							 }
							 
						 }

						 //End of Getting Order History
						 



						 if (res = m_manager->DealRequest(logins[j], from, to, deals) == MT_RET_OK)
						 {
							 r_count = deals->Total();

							 for (UINT p = 0; p < deals->Total(); p++)
							 {
								 deal = deals->Next(p);	
								 int dealno = deal->Deal();
								 int dealnocheck = 0;
								 
								 CStaticClass::m_Orika_dealNO.Lookup(dealno, dealnocheck);
								 if (dealnocheck == 0)
								 {
									 if (deal->Action() == 0 || deal->Action() == 1)
									 {
										 UINT64    m_login = deal->Login();
										 UINT64    m_deal = deal->Deal();

										 int intdeal = m_deal;

										 CStaticClass::m_Orika_dealNO.SetAt(intdeal, intdeal);

										 UINT64    m_order = deal->Order();
										 UINT      m_type = deal->Action();
										 UINT64    m_dealTime = deal->Time();
										 CString   m_Symbol = deal->Symbol();
										 double    m_price = deal->Price();
										 CString   m_External_ID = deal->ExternalID();
										 UINT      m_contract_Size = deal->ContractSize();
										 CString   m_comment = deal->Comment();
										 UINT64    m_volume = deal->Volume();

										 CString str_msmq = L"";
										 //str_msmq.Format(L"{\"MessageType\":1004,\"Data\":{\"login\":%I64u,\"deal\":%I64u,\"order\":%I64u,\"type\":%u,
										 //\"dealtime\":%I64u,\"Symbol\":\"%s\",
										 //\"price\":%.4lf,\"External_ID\":\"%s\",\"contract_Size\":%u,\"comment\":\"%s\",\"volume\":%I64u}}", 
										 //m_login, m_deal, m_order, m_type, m_dealTime, m_Symbol, m_price, m_External_ID, m_contract_Size, m_comment, m_volume);

										 /*string sslogin = string(CT2CA(strLogin));
										 const char* stlogin = sslogin.c_str();
										 writer.String(stlogin);*/


										 StringBuffer s;
										 Writer<StringBuffer> writer(s);
										 writer.StartObject();
										 writer.Key("MessageType");
										 writer.Int(1004);
										 writer.Key("Data");																				 
										 writer.StartObject();

										 
										 writer.Key("login");										 
										 writer.Int64(m_login);
										 writer.Key("deal");
										 writer.Int64(m_deal);
										 writer.Key("order");
										 writer.Int64(m_order);
										 writer.Key("type");
										 writer.Int(m_type);
										 writer.Key("dealtime");
										 writer.Int(m_dealTime);
										 writer.Key("Symbol");
										 string ssSymbol = string(CT2CA(m_Symbol));
										const char* stSymbol = ssSymbol.c_str();
										writer.String(stSymbol);
										writer.Key("price");
										writer.Double(m_price);
										writer.Key("External_ID");
										string ssExternalID = string(CT2CA(m_External_ID));
										const char* stExternalID = ssExternalID.c_str();
										writer.String(stExternalID);
										writer.Key("contract_Size");
										writer.Int(m_contract_Size);
										writer.Key("comment");
										string sscomment = string(CT2CA(m_comment));
										const char* stcomment = sscomment.c_str();
										writer.String(stcomment);
										writer.Key("volume");
										writer.Int64(m_volume);
										 writer.EndObject();
										 writer.EndObject();
										 
										 str_msmq = s.GetString();
										 



										 CMSMQApiWrapper  m_CMSMQApiWrapper;
										 CString szLabel, szMessage, szOutput;
										 szLabel = L"DealAdd";
										 _bstr_t temp_bst = str_msmq;
										 char* strforwrite;
										 strforwrite = temp_bst;
										 m_CMSMQApiWrapper.SendStringMessage(CStaticClass::MSMQQueuName, szLabel, strforwrite);
										 s.Clear();
										 writer.Flush();
									 }
								 }
							 }
						 }						 						 
					 }				 
				 orders->Release();
				 deals->Release();
			 }
		 }
		 group->Release();
	 

		 int m_totalOrder = CStaticClass::m_Orika_orderHastable.GetCount();
		 //CString m_strmessage = L"";
		 //m_strmessage.Format(L"Total Order Loaded %d", m_totalOrder);
		 //(m_strmessage);

	 	 
	 int totalData = m_PendingDealStringArray.Total();
	 for (int i = 0; i < totalData; i++)
	 {
		 CString  strData=L"";
		 dealandMSMQString mst = {};
		 mst =m_PendingDealStringArray[i];
		 strData = mst.msmqstring;
		 int dealnocheck = 0;
		 int intdealno = mst.dealno;
		 CStaticClass::m_Orika_dealNO.Lookup(intdealno, dealnocheck);
		 if (dealnocheck == 0)
		 {
			 CMSMQApiWrapper  m_CMSMQApiWrapper;
			 CString szLabel, szMessage, szOutput;
			 szLabel = L"DealAdd";
			 _bstr_t temp_bst = strData;
			 char* strforwrite;
			 strforwrite = temp_bst;
			 m_CMSMQApiWrapper.SendStringMessage(CStaticClass::MSMQQueuName, szLabel, strforwrite);
		 }
	 }
	 dealSendToMSMQStart = 1;
	 m_PendingDealStringArray.Clear();
 }







 void  CManager::FetchHistoricalDealingData(UINT64 datefrom,UINT64 dateto,CString clientkey)
 {	 
	 INT64       to = dateto;
	 INT64       from = datefrom;
	 INT64		INT_LOGIN = 0;
	 UINT totalNoOfPosition = 0;
	 int r_count = 0;
	 IMTConGroup* group;
	 MTAPIRES res;
	 group = m_manager->GroupCreate();
	 for (UINT i = 0; m_manager->GroupNext(i, group) == MT_RET_OK; i++)
	 {
		 UINT64* logins = NULL;
		 UINT    logins_total = 0;
		 if (res = m_manager->UserLogins(group->Group(), logins, logins_total) == MT_RET_OK)
		 {
			 IMTOrder* order = m_manager->OrderCreate();
			 IMTOrderArray* orders = m_manager->OrderCreateArray();
			 IMTDealArray* deals = m_manager->DealCreateArray();
			 IMTDeal* deal = m_manager->DealCreate();
			 if (logins && logins_total)
				 for (UINT j = 0; j < logins_total; j++)
				 {
					 INT_LOGIN = logins[j];

					 if (res = m_manager->OrderRequestOpen(logins[j], orders) == MT_RET_OK)
					 {
						 r_count = orders->Total();
						 for (UINT p = 0; p < orders->Total(); p++)
						 {
							 order = orders->Next(p);							 
							 CStaticClass m_tempstaticclass;
							 m_tempstaticclass.sendorderdealingToClient(order, 1, clientkey);
						 }
					 }
					 //Getting Order History
					 if (res = m_manager->HistoryRequest(logins[j], from, to, orders) == MT_RET_OK)
					 {
						 ////(L"Going to add History Order in msmq");
						 r_count = orders->Total();
						 for (UINT p = 0; p < orders->Total(); p++)
						 {
							 order = orders->Next(p);							 
							 CStaticClass m_tempstaticclass;
							 m_tempstaticclass.sendorderdealingToClient(order, 1, clientkey);
						 }
						 
					 }					 

					 if (res = m_manager->DealRequest(logins[j], from, to, deals) == MT_RET_OK)
					 {
						 r_count = deals->Total();
						 for (UINT p = 0; p < deals->Total(); p++)
						 {
							 deal = deals->Next(p);			
							 CStaticClass m_tempstaticclass;
							 m_tempstaticclass.sendNewTradeToClientFromDeal(deal,1, clientkey);
						 }
					 }
				 }
			 orders->Release();
			 deals->Release();
		 }
	 }
	 group->Release();	 
 }



 CString  CManager::FetchLogFromMemory(UINT64 datefrom, UINT64 dateto, CString clientkey)
 {
	 CString m_returnval = L"";
	 StringBuffer s;
	 Writer<StringBuffer> writer(s);
	 writer.StartObject();
	 writer.Key("type");
	 writer.String("LOGIN_DEVICE_LOG");	 
	 writer.Key("logdata");
	 writer.StartArray();
	 CStaticClass::m_mutexLog.Lock();
	 int total_Data = CStaticClass::m_logindeviceArray.Total();
	 CStaticClass::m_mutexLog.Unlock();
	 CString str_log = L"";


	 CStaticClass m_obj;
	 SYSTEMTIME m_dateUnix_From;
	 m_dateUnix_From = SMTTime::TimeToST(datefrom, m_dateUnix_From);
	 CString str_Datefrom = m_obj.sysDateToStringFormat(m_dateUnix_From);

	 SYSTEMTIME m_dateUnix_to;
	 m_dateUnix_to = SMTTime::TimeToST(dateto, m_dateUnix_to);
	 CString str_DateTo = m_obj.sysDateToStringFormat(m_dateUnix_to);


	// str_log.Format(L"DateFrom:%s DateTo:%s ", str_Datefrom, str_DateTo);
	 //(str_log);
	 for (int i = 0; i < total_Data; i++)
	 {
		 CStaticClass::st_logindevice m_st_logindevice = {};
		 CStaticClass::m_mutexLog.Lock();
		 m_st_logindevice = CStaticClass::m_logindeviceArray[i];
		 CStaticClass::m_mutexLog.Unlock();
		 int m_time = m_st_logindevice.m_time;
		 
		 if (m_time >= datefrom && m_time <= dateto)
		 {
			 writer.StartObject();

			 CString m_ip = m_st_logindevice.m_ip;
			 CString m_login = m_st_logindevice.m_login;
			 CString m_deviceID = m_st_logindevice.m_deviceID;
			 writer.Key("time");
			 writer.Int(m_time);
			 writer.Key("ip");
			 string m_sip = string(CT2CA(m_ip));
			 const char* m_ssip = m_sip.c_str();
			 writer.String(m_ssip);

			 writer.Key("login");
			 string m_slogin = string(CT2CA(m_login));
			 const char* m_sslogin = m_slogin.c_str();
			 writer.String(m_sslogin);


			 writer.Key("deviceid");
			 string m_sdeviceID = string(CT2CA(m_deviceID));
			 const char* m_ssdeviceID = m_sdeviceID.c_str();
			 writer.String(m_ssdeviceID);

			 writer.EndObject();
		 }
	 }	
	 writer.EndArray();
	 writer.EndObject();
	 CString strData = L"";
	 strData = s.GetString();
	 m_returnval = strData;
	 s.Flush();
	 s.Clear();
	 writer.Flush();
	 //(m_returnval);
	 return m_returnval;
 }


 CString  CManager::FetchHistoricalDealingFromMemory(UINT64 datefrom, UINT64 dateto,CString clientkey, CString subscriptionId)
 {
	 CString m_returnval = L"";
	 StringBuffer s;
	 Writer<StringBuffer> writer(s);
	 writer.StartObject();
	 writer.Key("type");
	 writer.String("DEALING_DATA_INTERVAL");
	 writer.Key("subscriptionId");
	 string m_stsubscriptionId = string(CT2CA(subscriptionId));
	 const char* m_sssubscriptionId = m_stsubscriptionId.c_str();
	 writer.String(m_sssubscriptionId);
	 writer.Key("dealingData");
	 writer.StartArray();

	 CStaticClass::m_mutex_ClientList.Lock();
	 CStaticClass::st_ClientContext m_st = {};
	 CStaticClass::m_ClientContext.Lookup(clientkey, m_st);
	 CStaticClass::m_mutex_ClientList.Unlock();

	 CStaticClass::m_mutex_order.Lock();
	 POSITION pos = CStaticClass::m_Orika_orderHastable.GetStartPosition();
	 int p_time = 0;
	 int p_CheckTime = 0;
	 while (pos != NULL)
	 {
		 int m_orderno = 0;
		 CStaticClass::st_order st = {};
		 CStaticClass::m_Orika_orderHastable.GetNextAssoc(pos, m_orderno, st);
		 CString m_strlogin = L"";
		 CString strFinalJson = L"";
		 m_strlogin = st.m_login;

		 int m_time = st.m_time-19800;;
		 if (m_time >= datefrom && m_time <= dateto)
		 {
			/* if (m_time < p_CheckTime)
			 {
				 m_time = p_time;
			 }
			 else
			 {
				 p_time = m_time;
				 p_CheckTime = m_time+(5*60);
			 }*/


			 CString m_strTime = L"";
			 CString m_tmp_date = L"";
			 CMTStr256 str_time;
			 SMTFormat::FormatDateTime(str_time, m_time, true, true);
			 m_strTime = str_time.Str();

			 CString m_symbol = st.m_symbol;

			 int m_orderno = st.m_order;

			 int m_type = st.m_type;
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
			 double m_volume = st.m_volume;
			 m_volume = m_volume ;
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
			 CString m_comment = st.m_comment;





			 double m_price = st.m_price;
			 //CString m_comment = m_deal->Comment();


			 int m_dealReason = 0;

			 CString OrderDesc = CStaticClass::m_ordertypedesc[m_type];


			 CString strtmpdata = L"";
			 CString m_reason = L"";

			 //m_strTypeFordealing = L"Order Placed";
			 m_reason.Format(L"'%s' #%d %s %.2lf %s at %.4lf", m_strlogin, m_orderno, OrderDesc, m_volume, m_symbol, m_price);

			 ////(L"L20");
			 
			 if (std::find(m_st.m_logins.begin(), m_st.m_logins.end(), m_strlogin) != m_st.m_logins.end())
			 {

				 writer.StartObject();
				 writer.Key("type");
				 writer.String("DEALING_DATA");

				 writer.Key("id");
				 CStaticClass::m_dealing_ID = CStaticClass::m_dealing_ID + 1;
				 writer.Int(CStaticClass::m_dealing_ID);

				 writer.Key("time");
				 writer.Int(m_time);
				 writer.Key("login");
				 string strlogin = string(CT2CA(m_strlogin));
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

				 CString m_subscriptionId = subscriptionId;
				 writer.Key("subscriptionId");
				 string stsubscriptionId = string(CT2CA(m_subscriptionId));
				 const char* sssubscriptionId = stsubscriptionId.c_str();
				 writer.String(sssubscriptionId);
				 writer.EndObject();
			 }
		 }
	 }
	 CStaticClass::m_mutex_order.Unlock();
	 CStaticClass::m_mutex_deal.Lock();
	 POSITION pos1 = CStaticClass::m_Orika_dealtableAccountingHastable.GetStartPosition();
	 
	 while (pos1 != NULL)
	 {
		 CString strLoginDealKey = L"";
		 CStaticClass::st_Orika_dealtableAccounting m_st_Orika_dealtableAccounting = {};
		 CStaticClass::m_Orika_dealtableAccountingHastable.GetNextAssoc(pos1, strLoginDealKey, m_st_Orika_dealtableAccounting);
		 

			CString m_login = m_st_Orika_dealtableAccounting.m_login;
	
			int m_time = m_st_Orika_dealtableAccounting.m_time;
			if (m_time >= datefrom && m_time <= dateto)
			{
				
				if (std::find(m_st.m_logins.begin(), m_st.m_logins.end(), m_login) != m_st.m_logins.end())
				{
					CString m_strTime = L"";
					CString m_tmp_date = L"";
					CMTStr256 str_time;
					SMTFormat::FormatDateTime(str_time, m_time, true, true);
					m_strTime = str_time.Str();

					CString m_symbol = m_st_Orika_dealtableAccounting.m_symbol;

					int m_deal_No = m_st_Orika_dealtableAccounting.m_deal;

					int m_type = m_st_Orika_dealtableAccounting.m_action;
					CString strtype = L"";

					CString m_comment = m_st_Orika_dealtableAccounting.m_comment;
					int m_order = m_st_Orika_dealtableAccounting.m_order;
					double m_volume = m_st_Orika_dealtableAccounting.m_volume;
					m_volume = m_volume / 10000;
					CString m_strTypeFordealing = L"";

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
					double m_price = m_st_Orika_dealtableAccounting.m_price;
					//CString m_comment = m_deal->Comment();



					CString strtmpdata = L"";
					CString m_reason = L"";
					m_reason.Format(L"deal performed ['%s' #%d %s %.2lf %s at %.4lf]", m_login, m_deal_No, strtype, m_volume, m_symbol, m_price);


					writer.StartObject();
					writer.Key("type");
					writer.String("DEALING_DATA");



					writer.Key("id");
					CStaticClass::m_dealing_ID = CStaticClass::m_dealing_ID + 1;
					writer.Int(CStaticClass::m_dealing_ID);

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
				}
			}
	 }
	 CStaticClass::m_mutex_deal.Unlock();


			 writer.EndArray();
			 writer.EndObject();
			 CString strData = L"";
			 strData = s.GetString();
			 m_returnval = strData;
			 s.Flush();
			 s.Clear();
			 writer.Flush();
			 return m_returnval;
 }

 void  CManager::OnDealDelete(const IMTDeal* deal)
 {	 
	 CStaticClass m_staticclass;
	 m_staticclass.sendNewTradeToClientFromDeal(deal, 3, L"");
 }
 void  CManager::OnDealAdd(const IMTDeal* deal)
 {
	 ////(L"Going to add deal in msmq");
		 if (deal->Action() == 0 || deal->Action() == 1)
		 {
			 CStaticClass m_staticclass;
			 m_staticclass.sendNewTradeToClient(deal);
			 UINT64    m_login = deal->Login();
			 UINT64    m_deal = deal->Deal();
			 UINT64    m_order = deal->Order();
			 UINT      m_type = deal->Action();
			 UINT64    m_dealTime = deal->Time();
			 CString   m_Symbol = deal->Symbol();
			 double    m_price = deal->Price();
			 CString   m_External_ID = deal->ExternalID();
			 UINT      m_contract_Size = deal->ContractSize();
			 CString   m_comment = deal->Comment();
			 UINT64    m_volume = deal->Volume();

			 UINT m_Reason= deal->Reason();



			 AlertStaticClass::m_alert_dealing_Lock.Lock();
				 AlertStaticClass::dealingData m_stdeal = {};
				 CString m_str_login = L"";
				 m_str_login.Format(L"%I64u", m_login);
				 CMTStr::Copy(m_stdeal.m_login, m_str_login);
				 CMTStr::Copy(m_stdeal.m_symbol, m_Symbol);
				 double m_dclVolume = m_volume / 10000;
				 m_stdeal.m_volume = m_dclVolume;
				 m_stdeal.m_rate = m_price;
				 m_stdeal.m_action = m_type;
				 AlertStaticClass::m_Alertdealing_Array.Add(&m_stdeal);
			 AlertStaticClass::m_alert_dealing_Lock.Unlock();




			 CString str_msmq = L"";
			 //str_msmq.Format(L"{\"MessageType\":1004,\"Data\":{\"login\":%I64u,\"deal\":%I64u,\"order\":%I64u,\"type\":%u,\"dealtime\":%I64u,\"Symbol\":\"%s\",\"price\":%.4lf,\"External_ID\":\"%s\",\"contract_Size\":%u,\"comment\":\"%s\",\"volume\":%I64u}}", m_login, m_deal, m_order, m_type, m_dealTime, m_Symbol, m_price, m_External_ID, m_contract_Size, m_comment, m_volume);
			 CStaticClass::m_mutex_order.Lock();
			 ////(L"L113");
				CStaticClass::m_OrikaOrderdealNO.SetAt(m_order, m_deal);
			 
			 ////(L"UL113");
			 CStaticClass::st_order m_stOrder_Check = {};
			 CStaticClass::m_Orika_orderHastable.Lookup(m_order, m_stOrder_Check);
			 CStaticClass::m_mutex_order.Unlock();
			 CStaticClass::st_OrderCount m_st = {};
			 CString strloginSymbolKey = L"";
			 CString strLogin = m_stOrder_Check.m_login;
			 if (strLogin != L"")
			 {
				 strloginSymbolKey.Format(L"%I64u:%s:%d", m_login, m_Symbol, m_stOrder_Check.m_type);
			 }
			 else
			 {
				 strloginSymbolKey.Format(L"%I64u:%s:%d", m_login, m_Symbol, m_type);
			 }
			 CStaticClass::m_OrderCountMap.Lookup(strloginSymbolKey, m_st);

			 if (strLogin != L"")
			 {
				 m_st.m_TotalExecutedOrder = m_st.m_TotalExecutedOrder + 1;
			 }
			 else
			 {
				 m_st.m_TotalOrder  = m_st.m_TotalOrder + 1;
			 }
			 //m_st.m_TotalOrder = m_st.m_TotalOrder + 1;
			 //m_st.m_TotalCancelledOrder = m_st.m_TotalCancelledOrder -1;			 
			 CStaticClass::m_OrderCountMap.SetAt(strloginSymbolKey, m_st);
			 CString mstr_login = L"";
			 mstr_login.Format(L"%I64u", m_login);
			 SendOrderCountUpdate(mstr_login, m_Symbol);




			 StringBuffer s;
			 Writer<StringBuffer> writer(s);
			 writer.StartObject();
			 writer.Key("MessageType");
			 writer.Int(1004);
			 writer.Key("Data");
			 writer.StartObject();


			 writer.Key("login");
			 writer.Int64(m_login);
			 writer.Key("deal");
			 writer.Int64(m_deal);
			 writer.Key("order");
			 writer.Int64(m_order);
			 writer.Key("type");
			 writer.Int(m_type);
			 writer.Key("dealtime");
			 writer.Int(m_dealTime);
			 writer.Key("Symbol");
			 string ssSymbol = string(CT2CA(m_Symbol));
			 const char* stSymbol = ssSymbol.c_str();
			 writer.String(stSymbol);
			 writer.Key("price");
			 writer.Double(m_price);
			 writer.Key("External_ID");
			 string ssExternalID = string(CT2CA(m_External_ID));
			 const char* stExternalID = ssExternalID.c_str();
			 writer.String(stExternalID);
			 writer.Key("contract_Size");
			 writer.Int(m_contract_Size);
			 writer.Key("comment");
			 string sscomment = string(CT2CA(m_comment));
			 const char* stcomment = sscomment.c_str();
			 writer.String(stcomment);
			 writer.Key("volume");
			 writer.Int64(m_volume);
			 writer.EndObject();
			 writer.EndObject();

			 str_msmq = s.GetString();



			 if (dealSendToMSMQStart==1)
			 {
				 CMSMQApiWrapper  m_CMSMQApiWrapper;
				 CString szLabel, szMessage, szOutput;
				 szLabel = L"DealAdd";
				 _bstr_t temp_bst = str_msmq;
				 char* strforwrite;
				 strforwrite = temp_bst;
				 m_CMSMQApiWrapper.SendStringMessage(CStaticClass::MSMQQueuName, szLabel, strforwrite);
			 }
			 else
			 {
				 dealandMSMQString mst = {};
				 wchar_t strDAta[256];
				 CMTStr::Copy(mst.msmqstring, str_msmq);
				 mst.dealno = m_deal;
				 m_PendingDealStringArray.Add(&mst);
			 }
		 }
		 else
		 {
			 CStaticClass m_staticclass;
			 m_staticclass.sendNewTradeToClientFromDeal(deal,1,L"");
		 }
		 ////(L"Deal added in msmq");
 }
 void  CManager::OnOrderAdd(const IMTOrder* order)
 {
	 ////(L"Going to add Order in msmq");
	 if (order->Type() != 0 && order->Type() != 1)
	 {
		 CStaticClass m_staticclass;
		 m_staticclass.sendorderdealingToClient(order, 1, L"");
		 UINT64    m_login = order->Login();
		 UINT64    m_order = order->Order();
		 UINT      m_type = order->Type();
		 UINT64    m_Ordertime = order->TimeSetup();
		 CString   m_Symbol = order->Symbol();
		 double    m_price_order = order->PriceOrder();
		 UINT64    m_time_exp = order->TimeExpiration();
		 UINT64    m_order_done_time = order->TimeDone();
		 UINT      m_state = order->State();
		 CString   m_External_ID = order->ExternalID();
		 UINT      m_contract_Size = order->ContractSize();
		 CString   m_comment = order->Comment();
		 UINT64    m_volume = order->VolumeInitial();
		 

		 CStaticClass::st_OrderCount m_st = {};		 		 		 
		 CString strloginSymbolKey = L"";
		 strloginSymbolKey.Format(L"%I64u:%s:%d", m_login, m_Symbol, m_type);
		 CStaticClass::m_OrderCountMap.Lookup(strloginSymbolKey, m_st);
		 m_st.m_TotalOrder = m_st.m_TotalOrder + 1;
		 CStaticClass::m_OrderCountMap.SetAt(strloginSymbolKey, m_st);

		 CString mstr_login = L"";
		 mstr_login.Format(L"%I64u", m_login);
		 SendOrderCountUpdate(mstr_login, m_Symbol);
		 //m_st.m_TotalCancelledOrder = m_st.m_TotalCancelledOrder + 1;
		 //m_st.m_TotalExecutedOrder = m_st.m_TotalExecutedOrder + 1;

		 CString str_msmq = L"";
		 str_msmq.Format(L"{\"MessageType\":1001,\"Data\":{\"login\":%I64u,\"order\":%I64u,\"type\":%u,\"Ordertime\":%I64u,\"Symbol\":\"%s\",\"price_order\":%.4lf,\"time_exp\":%I64u,\"order_done_time\":%I64u,\"state\":%u,\"External_ID\":\"%s\",\"contract_Size\":%u,\"comment\":\"%s\",\"volume\":%I64u}}", m_login, m_order, m_type, m_Ordertime, m_Symbol, m_price_order, m_time_exp, m_order_done_time, m_state, m_External_ID, m_contract_Size, m_comment, m_volume);
		 CMSMQApiWrapper  m_CMSMQApiWrapper;
		 CString szLabel, szMessage, szOutput;
		 szLabel = L"OrderAdd";
		 _bstr_t temp_bst = str_msmq;
		 char* strforwrite;
		 strforwrite = temp_bst;
		 m_CMSMQApiWrapper.SendStringMessage(CStaticClass::MSMQQueuName, szLabel, strforwrite);
	 }
	 ////(L"Order Added in msmq");
 }
 void  CManager::OnOrderUpdate(const IMTOrder* order)
 {
	 ////(L"Going to Update Order in msmq");
	 if (order->Type() != 0 && order->Type() != 1)
	 {
		 CStaticClass m_staticclass;
		 m_staticclass.sendorderdealingToClient(order, 2, L"");
		 UINT64    m_login = order->Login();
		 UINT64    m_order = order->Order();
		 UINT      m_type = order->Type();
		 UINT64    m_Ordertime = order->TimeSetup();
		 CString   m_Symbol = order->Symbol();
		 double    m_price_order = order->PriceOrder();
		 UINT64    m_time_exp = order->TimeExpiration();
		 UINT64    m_order_done_time = order->TimeDone();
		 UINT      m_state = order->State();
		 CString   m_External_ID = order->ExternalID();
		 UINT      m_contract_Size = order->ContractSize();
		 CString   m_comment = order->Comment();
		 UINT64    m_volume = order->VolumeInitial();

		 CString str_msmq = L"";
		 str_msmq.Format(L"{\"MessageType\":1002,\"Data\":{\"login\":%I64u,\"order\":%I64u,\"type\":%u,\"Ordertime\":%I64u,\"Symbol\":\"%s\",\"price_order\":%.4lf,\"time_exp\":%I64u,\"order_done_time\":%I64u,\"state\":%u,\"External_ID\":\"%s\",\"contract_Size\":%u,\"comment\":\"%s\",\"volume\":%I64u}}", m_login, m_order, m_type, m_Ordertime, m_Symbol, m_price_order, m_time_exp, m_order_done_time, m_state, m_External_ID, m_contract_Size, m_comment, m_volume);
		 CMSMQApiWrapper  m_CMSMQApiWrapper;
		 CString szLabel, szMessage, szOutput;
		 szLabel = L"OrderUpdate";
		 _bstr_t temp_bst = str_msmq;
		 char* strforwrite;
		 strforwrite = temp_bst;
		 m_CMSMQApiWrapper.SendStringMessage(CStaticClass::MSMQQueuName, szLabel, strforwrite);
	 }
	 ////(L"Order Updated in msmq");
 }
 void  CManager::OnOrderDelete(const IMTOrder* order)
 {
	 ////(L"Going to Delete Order in msmq");
	 if (order->Type() != 0 && order->Type() != 1)
	 {
		 CStaticClass m_staticclass;
		 m_staticclass.sendorderdealingToClient(order, 3, L"");
		 UINT64    m_login = order->Login();
		 UINT64    m_order = order->Order();
		 UINT      m_type = order->Type();
		 UINT64    m_Ordertime = order->TimeSetup();
		 CString   m_Symbol = order->Symbol();
		 double    m_price_order = order->PriceOrder();
		 UINT64    m_time_exp = order->TimeExpiration();
		 UINT64    m_order_done_time = order->TimeDone();
		 UINT      m_state = order->State();
		 CString   m_External_ID = order->ExternalID();
		 UINT      m_contract_Size = order->ContractSize();
		 CString   m_comment = order->Comment();
		 UINT64    m_volume = order->VolumeInitial();




		 CStaticClass::st_OrderCount m_st = {};
		 CString strloginSymbolKey = L"";
		 strloginSymbolKey.Format(L"%I64u:%s:%d", m_login, m_Symbol, m_type);
		 CStaticClass::m_OrderCountMap.Lookup(strloginSymbolKey, m_st);
		 //m_st.m_TotalOrder = m_st.m_TotalOrder + 1;


		
		 int m_testDeal = 0;
		 CStaticClass::m_OrikaOrderdealNO.Lookup(m_order, m_testDeal);


		 if (m_testDeal == 0)
		 {
			 m_st.m_TotalCancelledOrder = m_st.m_TotalCancelledOrder + 1;
		 }
		
		 CStaticClass::m_OrderCountMap.SetAt(strloginSymbolKey, m_st);
		 CString mstr_login = L"";
		 mstr_login.Format(L"%I64u", m_login);
		 SendOrderCountUpdate(mstr_login, m_Symbol);

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
	 ////(L"Order Deleted from msmq");
 }
 

 void CManager::OnDealUpdate(const IMTDeal* deal)
 {
	 CStaticClass m_staticclass;
	 m_staticclass.sendNewTradeToClientFromDeal(deal, 2, L"");
	 if (deal->Action() == 0 || deal->Action() == 1)
	 {
		 UINT64    m_login = deal->Login();
		 UINT64    m_deal = deal->Deal();
		 UINT64    m_order = deal->Order();
		 UINT      m_type = deal->Action();
		 UINT64    m_dealTime = deal->Time();
		 CString   m_Symbol = deal->Symbol();
		 double    m_price = deal->Price();
		 CString   m_External_ID = deal->ExternalID();
		 UINT      m_contract_Size = deal->ContractSize();
		 CString   m_comment = deal->Comment();
		 UINT64    m_volume = deal->Volume();

		 CString str_msmq = L"";
		 str_msmq.Format(L"{\"MessageType\":1006,\"Data\":{\"login\":%I64u,\"deal\":%I64u,\"order\":%I64u,\"type\":%u,\"dealtime\":%I64u,\"Symbol\":\"%s\",\"price\":%.4lf,\"External_ID\":\"%s\",\"contract_Size\":%u,\"comment\":\"%s\",\"volume\":%I64u}}", m_login, m_deal, m_order, m_type, m_dealTime, m_Symbol, m_price, m_External_ID, m_contract_Size, m_comment, m_volume);
		 CMSMQApiWrapper  m_CMSMQApiWrapper;
		 CString szLabel, szMessage, szOutput;
		 szLabel = L"DealUpdate";
		 _bstr_t temp_bst = str_msmq;
		 char* strforwrite;
		 strforwrite = temp_bst;
		 m_CMSMQApiWrapper.SendStringMessage(CStaticClass::MSMQQueuName, szLabel, strforwrite);
	 }
	 ////(L"Deal Updated in msmq");
 }
 void CManager::getPositionFromMT()
 {	 
	 CStaticClass::m_ClientSymbolPosition_MT.RemoveAll();
	 INT64		INT_LOGIN = 0;
	 UINT totalNoOfPosition = 0;
	 int r_count = 0;
	 IMTConGroup* group;
	 MTAPIRES res;
	 IMTPositionArray* positions = m_manager->PositionCreateArray();
	 m_manager->PositionGetByGroup(L"*", positions);
	 r_count = positions->Total();
	 for (UINT p = 0; p < positions->Total(); p++)
	 {
		 IMTPosition* position = m_manager->PositionCreate();
		 position = positions->Next(p);
		 CString strPosition = L"";
		 INT64 clientLogin = position->Login();
		 CString m_symbol = position->Symbol();
		 INT64 volume = position->Volume();
		 int action = position->Action();
		 CString strAction = L"";
		 if (action == 1)
		 {
			 volume = -volume;
		 }

		 CString m_loginSymbol = L"";
		 m_loginSymbol.Format(L"%I64u:%s", clientLogin, m_symbol);
		 CStaticClass::m_ClientSymbolPosition_MT.SetAt(m_loginSymbol, volume);
		 position->Release();
	 }
	 positions->Release();



	 /*group = m_manager->GroupCreate();
	 
		 for (UINT i = 0; m_manager->GroupNext(i, group) == MT_RET_OK; i++)
		 {
			 UINT64* logins = NULL;
			 UINT    logins_total = 0;
			 if (res = m_manager->UserLogins(group->Group(), logins, logins_total) == MT_RET_OK)
			 {
				 IMTPositionArray* positions = m_manager->PositionCreateArray();
				 IMTPosition* position = m_manager->PositionCreate();				 				 
				 if (logins && logins_total)
					 for (UINT j = 0; j < logins_total; j++)
					 {						 
						 INT_LOGIN = logins[j];						 
						 if (res = m_manager->PositionGet(logins[j], positions) == MT_RET_OK)
						 {
							 r_count = positions->Total();
							 for (UINT p = 0; p < positions->Total(); p++)
							 {
								 position = positions->Next(p);
								 CString strPosition = L"";
								 INT64 clientLogin = position->Login();
								 CString m_symbol = position->Symbol();
								 INT64 volume = position->Volume();
								 int action = position->Action();
								 CString strAction = L"";
								 if (action == 1)
								 {
									 volume = -volume;
								 }	

								 CString m_loginSymbol = L"";
								 m_loginSymbol.Format(L"%I64u:%s", clientLogin, m_symbol);
								 CStaticClass::m_ClientSymbolPosition_MT.SetAt(m_loginSymbol, volume);
							 }
						 }
					 }
				 positions->Release();				 
			 }		 
	 }
	 group->Release();*/
 }


 double CManager::getPositionFromMTClientWise(UINT64 m_login, CString m_symbol)
 {	 	 
	 UINT totalNoOfPosition = 0;
	 int r_count = 0;
	 IMTConGroup* group;
	 MTAPIRES res;
	 IMTPosition* position = m_manager->PositionCreate();
	 m_manager->PositionGet(m_login, m_symbol, position);
	 double m_volume = 0; 
	 m_volume = position->Volume();
	 position->Release();
	 return m_volume;
 }


 void  CManager::OnHistoryAdd(const IMTOrder* order)
 {
	 ////(L"Going to update History Order in msmq");
	 if (order->Type() != 0 && order->Type() != 1)
	 {
		 UINT64    m_login = order->Login();
		 UINT64    m_order = order->Order();
		 UINT      m_type = order->Type();
		 UINT64    m_Ordertime = order->TimeSetup();
		 CString   m_Symbol = order->Symbol();
		 double    m_price_order = order->PriceOrder();
		 UINT64    m_time_exp = order->TimeExpiration();
		 UINT64    m_order_done_time = order->TimeDone();
		 UINT      m_state = order->State();
		 CString   m_External_ID = order->ExternalID();
		 UINT      m_contract_Size = order->ContractSize();
		 CString   m_comment = order->Comment();
		 UINT64    m_volume = order->VolumeInitial();
		 CString str_msmq = L"";
		 str_msmq.Format(L"{\"MessageType\":1005,\"Data\":{\"login\":%I64u,\"order\":%I64u,\"type\":%u,\"Ordertime\":%I64u,\"Symbol\":\"%s\",\"price_order\":%.4lf,\"time_exp\":%I64u,\"order_done_time\":%I64u,\"state\":%u,\"External_ID\":\"%s\",\"contract_Size\":%u,\"comment\":\"%s\",\"volume\":%I64u}}", m_login, m_order, m_type, m_Ordertime, m_Symbol, m_price_order, m_time_exp, m_order_done_time, m_state, m_External_ID, m_contract_Size, m_comment, m_volume);
		 CMSMQApiWrapper  m_CMSMQApiWrapper;
		 CString szLabel, szMessage, szOutput;
		 szLabel = L"ORDER_HISTORY";
		 _bstr_t temp_bst = str_msmq;
		 char* strforwrite;
		 strforwrite = temp_bst;
		 m_CMSMQApiWrapper.SendStringMessage(CStaticClass::MSMQQueuName, szLabel, strforwrite);
	 }
	 ////(L"updated History Order in msmq");
 }

 inline void  CManager::OnGroupAdd(const IMTConGroup* config)
 {
	 CString str_group_path = config->Group();
	 CString m_OGroup = str_group_path;
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
		 m_OGroup = m_OGroup.Mid(m_OGroup.Find(L"\\") + 1, m_OGroup.GetLength() - m_OGroup.Find(L"\\") - 1);
	 }
	 m_parent_Group = m_parent_Group + L"\\*";	 

	 CString strCommand = L"";
	 strCommand.Format(L"exec updatelogin_group_Access '%s','%s';", m_parent_Group, str_group_path);
	 CStaticClass::m_sqldata.executeCommand(strCommand);
 }

inline void CManager::OnTick(LPCWSTR symbol, const MTTickShort& tick)
{	
	int m_TickFlag = tick.flags;
	CString strSymbol = L"";
	CStaticClass::m_symbolForTickData.Lookup(symbol, strSymbol);
	if (strSymbol == L"")
	{		
		return;
	}		
	CString  m_symbol=symbol;		
	CStaticClass::st_Tick m_st_Tick={};
	CMTStr::Copy(m_st_Tick.m_symbol,symbol);
	m_st_Tick.m_bid=tick.bid ;
	m_st_Tick.m_ask=tick.ask ;
	m_st_Tick.m_last=tick.last ;
	m_st_Tick.m_datetime=tick.datetime;
	CStaticClass::m_mutex_Tick_copy.Lock();
	CStaticClass::symbolLastTickArrayPrimary.SetAt(m_symbol, m_st_Tick);
	CStaticClass::m_mutex_Tick_copy.Unlock();
		
	CStaticClass::m_mutex_Tick_TickSubscribe.Lock();
		CStaticClass::m_TickData.SetAt(m_symbol, m_st_Tick);				
		CStaticClass::m_updatedTickData.SetAt(m_symbol, m_st_Tick);
	CStaticClass::m_mutex_Tick_TickSubscribe.Unlock();
}

void CManager::UpdateClientMasterFromMuser(const IMTUser* m_user)
{	
	UINT64 INT_LOGIN = m_user->Login();	
	CStaticClass:: st_Orika_MTclientmaster m_st_Orika_MTclientmaster = {};
	CStaticClass::m_Orika_MTclientmasterHasTable.Lookup(INT_LOGIN, m_st_Orika_MTclientmaster);
	CString m_nameTmp=m_st_Orika_MTclientmaster.m_name;
	CString m_commentTmp =m_st_Orika_MTclientmaster.m_comment;
	CString m_addressTmp =m_st_Orika_MTclientmaster.m_address;
	CString m_leadsourceTmp =m_st_Orika_MTclientmaster.m_leadsource;
	CString m_leadcampaignTmp =m_st_Orika_MTclientmaster.m_leadcampaign;
	int     m_languageTmp = m_st_Orika_MTclientmaster.m_language;
	CString strGroup = m_st_Orika_MTclientmaster.m_group;
	if (m_nameTmp != m_user->Name() || m_commentTmp != m_user->Comment() || m_addressTmp != m_user->Address() || m_leadsourceTmp != m_user->LeadSource() || m_leadcampaignTmp != m_user->LeadCampaign() || m_languageTmp != m_user->Language()|| strGroup!= m_user->Group())
	{
		if (m_user->Group() != L"")
		{
			UpdateClientMasterFromMuser_Manual(m_user);
			updateLoginToClientList();
		}
	}
}

void CManager::updateLoginToClientList()
{
	CStaticClass::m_logfile.LogEvent(L"updateLoginToClientList");
	
	CStaticClass::m_mutex_ClientList.Lock();
	//(L"12");
	POSITION pos = CStaticClass::m_ClientContext.GetStartPosition();

	int m_totalData = CStaticClass::m_ClientContext.GetCount();

	
	while (pos != NULL)
	{
		CString strclientkey = L"";
		CStaticClass::st_ClientContext m_st = {};
		CStaticClass::m_ClientContext.GetNextAssoc(pos, strclientkey, m_st);
	


		CString m_login = m_st.m_userlogin;
		CString   strCommand = L"";
		strCommand.Format(L"exec GetUserLoginList '%s';", m_login);

		//CStaticClass::m_mutex_order.Lock();
		
		
		HRESULT hr = NULL;
		CCommand<CAccessor<CloginListTable>> data_table;
		if (!SUCCEEDED(hr))
		{
			return;
		}				
		CSession m_sqlsession;
		hr=m_sqlsession.Open(CStaticClass::connection);
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
		CStaticClass::m_ClientContext.SetAt(strclientkey, m_st);
	}
	CStaticClass::m_mutex_ClientList.Unlock();




	CStaticClass::m_logfile.LogEvent(L"End  updateLoginToClientList");

	
}



void CManager::UpdateClientMasterFromMuser_Manual(const IMTUser* m_user)
{
	UINT64 INT_LOGIN = m_user->Login();
	CStaticClass::st_Orika_MTclientmaster m_st_Orika_MTclientmaster = {};
	CStaticClass::m_Orika_MTclientmasterHasTable.Lookup(INT_LOGIN, m_st_Orika_MTclientmaster);
	CString m_nameTmp = m_st_Orika_MTclientmaster.m_name;
	CString m_commentTmp = m_st_Orika_MTclientmaster.m_comment;
	CString m_addressTmp = m_st_Orika_MTclientmaster.m_address;
	CString m_leadsourceTmp = m_st_Orika_MTclientmaster.m_leadsource;
	CString m_leadcampaignTmp = m_st_Orika_MTclientmaster.m_leadcampaign;
	int     m_languageTmp = m_st_Orika_MTclientmaster.m_language;
	CString strGroup = m_st_Orika_MTclientmaster.m_group;
	
		CStaticClass::m_mutex_Tick.Lock();
		strGroup = m_user->Group();
		CString m_ogroup = strGroup;

		CString login = L"";
		login.Format(L"%I64u", INT_LOGIN);


		CString broker = L"";
		CString subbroker = L"";
		CString extragroup = L"";
		CString Exchange = L"";

		CString company = L"";
		if (strGroup.Find(L"\\") >= 0)
		{
			company = strGroup.Mid(0, strGroup.Find(L"\\"));
		}

		strGroup = strGroup.Mid(strGroup.Find(L"\\") + 1, strGroup.GetLength() - strGroup.Find(L"\\") - 1);
		if (strGroup.Find(L"\\") >= 0)
		{
			broker = strGroup.Mid(0, strGroup.Find(L"\\"));
			strGroup = strGroup.Mid(strGroup.Find(L"\\") + 1, strGroup.GetLength() - strGroup.Find(L"\\") - 1);
		}
		else
		{
			broker = strGroup;
			strGroup = L"";
		}
		if (strGroup.Find(L"\\") >= 0)
		{
			subbroker = strGroup.Mid(0, strGroup.Find(L"\\"));
			strGroup = strGroup.Mid(strGroup.Find(L"\\") + 1, strGroup.GetLength() - strGroup.Find(L"\\") - 1);
		}
		else
		{
			subbroker = strGroup;
			strGroup = L"";
		}

		if (strGroup.Find(L"\\") >= 0)
		{
			extragroup = strGroup.Mid(0, strGroup.Find(L"\\"));
			strGroup = strGroup.Mid(strGroup.Find(L"\\") + 1, strGroup.GetLength() - strGroup.Find(L"\\") - 1);
		}
		else
		{
			extragroup = strGroup;
			strGroup = L"";
		}



		CString name = m_user->Name();
		//CString broker=broker;
		CString subBroker = subbroker;
		CString extraGroup = extragroup;
		double lossLimit = 0;
		CString StrCreditLimit = m_user->Comment();
		double creditLimit = _wtof(StrCreditLimit);


		int comment = 0;
		int m_language = m_user->Language();
		if (m_language == 25)
		{
			comment = 1;
		}
		double qtyLimitMultiplayer = 0;
		int ignoreTrader = 0;
		CString colour = L"";
		CString strBrokerage = m_user->Address();
		CString strbrokerageRatio = m_user->LeadSource();
		CString strPlRatio = m_user->LeadCampaign();
		CString strGroup_User = m_user->Group();


		CMTStr::Copy(m_st_Orika_MTclientmaster.m_name, name);
		CMTStr::Copy(m_st_Orika_MTclientmaster.m_comment, StrCreditLimit);
		CMTStr::Copy(m_st_Orika_MTclientmaster.m_address, strBrokerage);
		CMTStr::Copy(m_st_Orika_MTclientmaster.m_leadsource, strbrokerageRatio);
		CMTStr::Copy(m_st_Orika_MTclientmaster.m_leadcampaign, strPlRatio);
		m_st_Orika_MTclientmaster.m_language = m_language;
		CMTStr::Copy(m_st_Orika_MTclientmaster.m_group, strGroup_User);

		CStaticClass::m_Orika_MTclientmasterHasTable.SetAt(INT_LOGIN, m_st_Orika_MTclientmaster);

		CString strCommand = L"";
		strCommand.Format(L"exec updateClient '%s','%s','%s','%s','%s','%.0f','%.0f','%d','%.2f','%d','%s','%s','%s','%s'", login, name, broker, subBroker, extraGroup, lossLimit, creditLimit, comment, qtyLimitMultiplayer, ignoreTrader, colour, Exchange, company, m_ogroup);
		CStaticClass::m_sqldata.executeCommand(strCommand);

		double subBrokerBrokerageRatio = 0;
		double BrokerBrokerageRatio = 0;
		double CompanyBrokerageRatio = 0;

		CString  str_subBrokerBrokerageRatio = L"";
		CString  str_BrokerBrokerageRatio = L"";
		CString  str_CompanyBrokerageRatio = L"";
		if (strbrokerageRatio.Find(L",") >= 0)
		{
			str_subBrokerBrokerageRatio = strbrokerageRatio.Mid(0, strbrokerageRatio.Find(L","));
			strbrokerageRatio = strbrokerageRatio.Mid(strbrokerageRatio.Find(L",") + 1, strbrokerageRatio.GetLength() - strbrokerageRatio.Find(L",") - 1);
		}
		else
		{
			str_subBrokerBrokerageRatio = strbrokerageRatio;
			strbrokerageRatio = L"";
		}
		subBrokerBrokerageRatio = _wtof(str_subBrokerBrokerageRatio);

		if (strbrokerageRatio.Find(L",") >= 0)
		{
			str_BrokerBrokerageRatio = strbrokerageRatio.Mid(0, strbrokerageRatio.Find(L","));
			strbrokerageRatio = strbrokerageRatio.Mid(strbrokerageRatio.Find(L",") + 1, strbrokerageRatio.GetLength() - strbrokerageRatio.Find(L",") - 1);
		}
		else
		{
			str_BrokerBrokerageRatio = strbrokerageRatio;
			strbrokerageRatio = L"";
		}
		BrokerBrokerageRatio = _wtof(str_BrokerBrokerageRatio);

		if (strbrokerageRatio.Find(L",") >= 0)
		{
			str_CompanyBrokerageRatio = strbrokerageRatio.Mid(0, strbrokerageRatio.Find(L","));
			strbrokerageRatio = strbrokerageRatio.Mid(strbrokerageRatio.Find(L",") + 1, strbrokerageRatio.GetLength() - strbrokerageRatio.Find(L",") - 1);
		}
		else
		{
			str_CompanyBrokerageRatio = strbrokerageRatio;
			strbrokerageRatio = L"";
		}
		CompanyBrokerageRatio = _wtof(str_CompanyBrokerageRatio);





		double subBrokerPlratio = 0;
		double BrokerPlratio = 0;
		double CompanyPlratio = 0;

		CString str_subBrokerPlratio = L"";
		CString str_BrokerPlratio = L"";
		CString str_CompanyPlratio = L"";

		if (strPlRatio.Find(L",") >= 0)
		{
			str_subBrokerPlratio = strPlRatio.Mid(0, strPlRatio.Find(L","));
			strPlRatio = strPlRatio.Mid(strPlRatio.Find(L",") + 1, strPlRatio.GetLength() - strPlRatio.Find(L",") - 1);
		}
		else
		{
			str_subBrokerPlratio = strPlRatio;
			strPlRatio = L"";
		}
		subBrokerPlratio = _wtof(str_subBrokerPlratio);

		if (strPlRatio.Find(L",") >= 0)
		{
			str_BrokerPlratio = strPlRatio.Mid(0, strPlRatio.Find(L","));
			strPlRatio = strPlRatio.Mid(strPlRatio.Find(L",") + 1, strPlRatio.GetLength() - strPlRatio.Find(L",") - 1);
		}
		else
		{
			str_BrokerPlratio = strPlRatio;
			strPlRatio = L"";
		}
		BrokerPlratio = _wtof(str_BrokerPlratio);

		if (strPlRatio.Find(L",") >= 0)
		{
			str_CompanyPlratio = strPlRatio.Mid(0, strPlRatio.Find(L","));
			strPlRatio = strPlRatio.Mid(strPlRatio.Find(L",") + 1, strPlRatio.GetLength() - strPlRatio.Find(L",") - 1);
		}
		else
		{
			str_CompanyPlratio = strPlRatio;
			strPlRatio = L"";
		}
		CompanyPlratio = _wtof(str_CompanyPlratio);



		//Start Updating Orika_clientbrokerage
		CString strBrok = L"";
		CString strSymbolGroup = L"";
		CString strbrokerage = L"";
		CString strbrokerageType = L"";

		int brokageType = 1;
		double clientBrokage = 0;
		double subBrokerBrokage = 0;
		double bokerBrokage = 0;
		double companyBrokage = 0;


		while (strBrokerage.Find(L",") >= 0)
		{
			strBrok = strBrokerage.Mid(0, strBrokerage.Find(L","));
			strSymbolGroup = strBrok.Mid(0, strBrok.Find(L":"));
			if (strSymbolGroup.Trim() == L"0")
			{
				strSymbolGroup = L"";
			}
			if (strSymbolGroup.Trim() == L"1")
			{
				strSymbolGroup = L"option";
			}
			strBrok = strBrok.Mid(strBrok.Find(L":") + 1, strBrok.GetLength() - strBrok.Find(L":") - 1);
			strbrokerageType = strBrok.Mid(0, strBrok.Find(L":"));
			brokageType = _wtoi(strbrokerageType);

			strbrokerage = strBrok.Mid(strBrok.Find(L":") + 1, strBrok.GetLength() - 1);
			clientBrokage = _wtof(strbrokerage);
			subBrokerBrokage = clientBrokage * subBrokerBrokerageRatio / 100;
			bokerBrokage = clientBrokage * BrokerBrokerageRatio / 100;
			companyBrokage = clientBrokage * CompanyBrokerageRatio / 100;
			if (companyBrokage == 0)
			{
				companyBrokage = clientBrokage - (subBrokerBrokage + bokerBrokage);
			}



			strCommand.Format(L"exec updateClientbrokerage '%s','%s','%d','%.6f','%.6f','%.6f','%.6f'", login, strSymbolGroup, brokageType, clientBrokage, subBrokerBrokage, bokerBrokage, companyBrokage);
			CStaticClass::m_sqldata.executeCommand(strCommand);

			//Updating Brokerage in Memory
			CStaticClass::st_Orika_clientbrokerage m_st_Orika_clientbrokerage = {};
			CString strKey = L"";
			CString strlogin = login;
			CString strsymbol = strSymbolGroup;
			strKey.Format(L"%s:%s", strlogin, strsymbol);
			CMTStr::Copy(m_st_Orika_clientbrokerage.m_login, login);
			CMTStr::Copy(m_st_Orika_clientbrokerage.m_symbolGroup, strSymbolGroup);
			m_st_Orika_clientbrokerage.m_brokageType = brokageType;
			m_st_Orika_clientbrokerage.m_clientBrokage = clientBrokage;
			m_st_Orika_clientbrokerage.m_subBrokerBrokage = subBrokerBrokage;
			m_st_Orika_clientbrokerage.m_bokerBrokage = bokerBrokage;
			m_st_Orika_clientbrokerage.m_companyBrokage = companyBrokage;
			CStaticClass::m_Orika_clientbrokerageHastable.SetAt(strKey, m_st_Orika_clientbrokerage);
			//End of Updating brokerage in Memory





			strBrokerage = strBrokerage.Mid(strBrokerage.Find(L",") + 1, strBrokerage.GetLength() - strBrokerage.Find(L",") - 1);


			//Updating Realtime client
			CStaticClass::stclientmaster m_stclientmaster = {};
			CMTStr::Copy(m_stclientmaster.m_login, login);
			CMTStr::Copy(m_stclientmaster.m_symbolGroup, strSymbolGroup);
			CMTStr::Copy(m_stclientmaster.m_name, name);
			CMTStr::Copy(m_stclientmaster.m_broker, broker);
			CMTStr::Copy(m_stclientmaster.m_subBroker, subBroker);
			CMTStr::Copy(m_stclientmaster.m_extraGroup, extraGroup);
			m_stclientmaster.m_subBrokerPLRatio = subBrokerPlratio;
			m_stclientmaster.m_brokerPLRatio = BrokerPlratio;
			m_stclientmaster.m_companyPLRatio = CompanyPlratio;
			CString strBrokerageType = L"";
			if (brokageType == 0)
			{
				strBrokerageType = L"turnover";
			}
			if (brokageType == 1)
			{
				strBrokerageType = L"lotwise";
			}
			CMTStr::Copy(m_stclientmaster.m_brokageType, strBrokerageType);
			m_stclientmaster.m_clientBrokage = clientBrokage;
			m_stclientmaster.m_subBrokerBrokage = subBrokerBrokage;
			m_stclientmaster.m_bokerBrokage = bokerBrokage;
			m_stclientmaster.m_companyBrokage = companyBrokage;
			//m_stclientmaster.m_symbolWiseBuyLimit = data_table.m_symbolWiseBuyLimit;
			//m_stclientmaster.m_symbolWiseSellLimit = data_table.m_symbolWiseSellLimit;
			//m_stclientmaster.m_symbolWisePendingOrderEnableDisable = data_table.m_symbolWisePendingOrderEnableDisable;
			//m_stclientmaster.m_symbolPositionLimit = data_table.m_symbolPositionLimit;
			//m_stclientmaster.m_symbolPendingOrderDiffFromBidAsk = data_table.m_symbolPendingOrderDiffFromBidAsk;	
			m_stclientmaster.m_lossLimit = lossLimit;
			m_stclientmaster.m_creditLimit = creditLimit;
			m_stclientmaster.m_comment = comment;
			m_stclientmaster.m_qtyLimitMultiplayer = qtyLimitMultiplayer;
			m_stclientmaster.m_ignoreTrader = ignoreTrader;
			CMTStr::Copy(m_stclientmaster.m_colour, colour);
			int ClientDataIndex = -1;
			int m_totalData = CStaticClass::m_clientmasterarray.Total();
			for (int i = 0; i < m_totalData; i++)
			{
				CStaticClass::stclientmaster m_tmpData = {};
				m_tmpData = CStaticClass::m_clientmasterarray[i];
				CString strTmpLogin = m_tmpData.m_login;
				CString strTmpSymbolGroup = m_tmpData.m_symbolGroup;
				if (strTmpLogin == login && strTmpSymbolGroup == strSymbolGroup)
				{
					ClientDataIndex = i;
				}
			}
			if (ClientDataIndex == -1)
			{
				CStaticClass::m_clientmasterarray.Add(&m_stclientmaster);
			}
			else
			{
				CStaticClass::m_clientmasterarray.Update(ClientDataIndex, &m_stclientmaster);
			}
			//End Of Updating RealTime Client


		}
		strBrok = strBrokerage;
		strSymbolGroup = strBrok.Mid(0, strBrok.Find(L":"));
		if (strSymbolGroup.Trim() == L"0")
		{
			strSymbolGroup = L"";
		}
		if (strSymbolGroup.Trim() == L"1")
		{
			strSymbolGroup = L"option";
		}
		strBrok = strBrok.Mid(strBrok.Find(L":") + 1, strBrok.GetLength() - strBrok.Find(L":") - 1);
		strbrokerageType = strBrok.Mid(0, strBrok.Find(L":"));
		brokageType = _wtoi(strbrokerageType);

		strbrokerage = strBrok.Mid(strBrok.Find(L":") + 1, strBrok.GetLength() - 1);
		clientBrokage = _wtof(strbrokerage);
		subBrokerBrokage = clientBrokage * subBrokerBrokerageRatio / 100;
		bokerBrokage = clientBrokage * BrokerBrokerageRatio / 100;
		companyBrokage = clientBrokage * CompanyBrokerageRatio / 100;



		strCommand.Format(L"exec updateClientbrokerage '%s','%s','%d','%.6f','%.6f','%.6f','%.6f'", login, strSymbolGroup, brokageType, clientBrokage, subBrokerBrokage, bokerBrokage, companyBrokage);
		CStaticClass::m_sqldata.executeCommand(strCommand);

		//Updating Brokerage in Memory
		CStaticClass::st_Orika_clientbrokerage m_st_Orika_clientbrokerage = {};
		CString strKey = L"";
		CString strlogin = login;
		CString strsymbol = strSymbolGroup;
		strKey.Format(L"%s:%s", strlogin, strsymbol);
		CMTStr::Copy(m_st_Orika_clientbrokerage.m_login, login);
		CMTStr::Copy(m_st_Orika_clientbrokerage.m_symbolGroup, strSymbolGroup);
		m_st_Orika_clientbrokerage.m_brokageType = brokageType;
		m_st_Orika_clientbrokerage.m_clientBrokage = clientBrokage;
		m_st_Orika_clientbrokerage.m_subBrokerBrokage = subBrokerBrokage;
		m_st_Orika_clientbrokerage.m_bokerBrokage = bokerBrokage;
		m_st_Orika_clientbrokerage.m_companyBrokage = companyBrokage;
		CStaticClass::m_Orika_clientbrokerageHastable.SetAt(strKey, m_st_Orika_clientbrokerage);
		CompanyPlratio = CompanyPlratio == 0 ? 100 : CompanyPlratio;
		//End of Updating brokerage in Memory

		//End Updating Orika_clientbrokerage
		//Updating Client PlRatio
		//strPlRatio				
		strCommand.Format(L"exec UpdatePLDevideRatio '%s','','%.6f','%.6f','%.6f'", login, BrokerPlratio, subBrokerPlratio, CompanyPlratio);
		CStaticClass::m_sqldata.executeCommand(strCommand);
		//Updating PL Ratio in Memory
		CStaticClass::st_Orika_PLDevideRatio m_st_Orika_PLDevideRatio = {};

		strlogin = login;
		strsymbol = L"";
		strKey.Format(L"%s:%s", strlogin, strsymbol);
		CMTStr::Copy(m_st_Orika_PLDevideRatio.m_login, strlogin);
		CMTStr::Copy(m_st_Orika_PLDevideRatio.m_symbolGroup, strsymbol);

		m_st_Orika_PLDevideRatio.m_brokerPLRatio = BrokerPlratio;
		m_st_Orika_PLDevideRatio.m_subBrokerPLRatio = subBrokerPlratio;
		m_st_Orika_PLDevideRatio.m_companyPLRatio = CompanyPlratio;

		CStaticClass::m_Orika_PLDevideRatioHastable.SetAt(strKey, m_st_Orika_PLDevideRatio);
		//End of Updating PL Ratio in Memory


		//End Of Updating Plratio


		CStaticClass::st_Orika_clientmaster m_st_Orika_clientmaster = {};


		CMTStr::Copy(m_st_Orika_clientmaster.m_login, login);
		CMTStr::Copy(m_st_Orika_clientmaster.m_name, name);
		CMTStr::Copy(m_st_Orika_clientmaster.m_broker, broker);
		CMTStr::Copy(m_st_Orika_clientmaster.m_subBroker, subBroker);
		CMTStr::Copy(m_st_Orika_clientmaster.m_extraGroup, extraGroup);
		m_st_Orika_clientmaster.m_lossLimit = lossLimit;
		m_st_Orika_clientmaster.m_creditLimit = creditLimit;
		m_st_Orika_clientmaster.m_comment = comment;
		m_st_Orika_clientmaster.m_qtyLimitMultiplayer = qtyLimitMultiplayer;
		m_st_Orika_clientmaster.m_ignoreTrader = ignoreTrader;
		CMTStr::Copy(m_st_Orika_clientmaster.m_colour, colour);
		CMTStr::Copy(m_st_Orika_clientmaster.m_company, company);
		CStaticClass::m_Orika_clientmasterHastable.SetAt(login, m_st_Orika_clientmaster);


		//Updating Netposition Hash Table		
		StringBuffer s;
		Writer<StringBuffer> writer(s);
		writer.StartObject();
		writer.Key("type");
		writer.String("CLIENT_POSITION");
		writer.Key("updatekey");
		writer.StartArray();
		writer.String("login");
		writer.String("symbol");
		writer.EndArray();
		writer.Key("update");
		writer.StartArray();
		POSITION pos = CStaticClass::m_Orika_symbolmasterHastable.GetStartPosition();
		while (pos != NULL)
		{
			CString strKey = L"";
			CStaticClass::st_Orika_symbolmaster m_st_Orika_symbolmaster = {};
			CStaticClass::m_Orika_symbolmasterHastable.GetNextAssoc(pos, strKey, m_st_Orika_symbolmaster);
			CString m_SymbolGroup = m_st_Orika_symbolmaster.m_symbolGroup;
			CString ClientSymbolKey = L"";
			ClientSymbolKey.Format(L"%I64u:%s", INT_LOGIN, strKey);
			CStaticClass::st_netpositionClientWise stnetpos = {};
			CStaticClass::mapNetPositionClientWise.Lookup(ClientSymbolKey, stnetpos);
			CString m_GetLoginValue = stnetpos.m_login;
			if (m_GetLoginValue.Trim() != L"")
			{
				//ClientSymbolGroupKey
				CString ClientSymbolGroupKey = L"";
				ClientSymbolGroupKey.Format(L"%I64u:%s", INT_LOGIN, m_SymbolGroup);
				CStaticClass::st_Orika_clientbrokerage m_st_Orika_clientbrokerage = {};
				CStaticClass::m_Orika_clientbrokerageHastable.Lookup(ClientSymbolGroupKey, m_st_Orika_clientbrokerage);
				//ClientKey
				CString strBrokerageLogin = m_st_Orika_clientbrokerage.m_login;


				CString ClientKey = L"";
				ClientKey.Format(L"%I64u:", INT_LOGIN);

				if (strBrokerageLogin.Trim() == L"")
				{
					CStaticClass::m_Orika_clientbrokerageHastable.Lookup(ClientKey, m_st_Orika_clientbrokerage);
				}

				CStaticClass::st_Orika_PLDevideRatio m_st_Orika_PLDevideRatio = {};
				CStaticClass::m_Orika_PLDevideRatioHastable.Lookup(ClientSymbolGroupKey, m_st_Orika_PLDevideRatio);
				strBrokerageLogin = m_st_Orika_PLDevideRatio.m_login;

				if (strBrokerageLogin == L"")
				{
					CStaticClass::m_Orika_PLDevideRatioHastable.Lookup(ClientKey, m_st_Orika_PLDevideRatio);
				}

				CMTStr::Copy(stnetpos.m_name, name);
				CMTStr::Copy(stnetpos.m_broker, broker);
				CMTStr::Copy(stnetpos.m_subbroker, subbroker);

				CMTStr::Copy(stnetpos.m_company, company);

				*stnetpos.m_subbrokerRatio = m_st_Orika_PLDevideRatio.m_subBrokerPLRatio;
				*stnetpos.m_brokerRatio = m_st_Orika_PLDevideRatio.m_brokerPLRatio;
				*stnetpos.m_companyRatio = m_st_Orika_PLDevideRatio.m_companyPLRatio;





				*stnetpos.m_subbrokervolume = ((*stnetpos.m_volume) * (*stnetpos.m_subbrokerRatio)) / 100;
				*stnetpos.m_brokervolume = ((*stnetpos.m_volume) * (*stnetpos.m_brokerRatio)) / 100;
				*stnetpos.m_companyvolume = ((*stnetpos.m_volume) * (*stnetpos.m_companyRatio)) / 100;


				*stnetpos.m_brokerageType = m_st_Orika_clientbrokerage.m_brokageType;
				*stnetpos.m_clientBrokRate = m_st_Orika_clientbrokerage.m_clientBrokage;
				*stnetpos.m_subBrokerBrokRate = m_st_Orika_clientbrokerage.m_subBrokerBrokage;
				*stnetpos.m_brokerBrokRate = m_st_Orika_clientbrokerage.m_bokerBrokage;
				*stnetpos.m_companyBrokRate = m_st_Orika_clientbrokerage.m_companyBrokage;



				//Recalculating Brokerage
				CStaticClass::m_mutex_TotalBrokerage.Lock();
				CStaticClass::st_TotalTradedLotAndTOT m_stTO = {};
				CStaticClass::m_TotalLotAndTOT.Lookup(ClientSymbolKey, m_stTO);
				double m_TotalTradedLot = m_stTO.m_TotalTradedLot;
				double m_TotalTradedTO = m_stTO.m_TotalTradedTO;
				//Calculating Client Brokerage

				double m_brokerageType = *stnetpos.m_brokerageType;
				double m_brokeragerate = *stnetpos.m_clientBrokRate;
				double m_subBrokerBrokRate = *stnetpos.m_subBrokerBrokRate;
				double m_brokerBrokRate = *stnetpos.m_brokerBrokRate;
				double m_companyBrokRate = *stnetpos.m_companyBrokRate;
				double m_newbrolerage = 0;
				double m_subBrokerBrokTotal = 0;
				double m_brokerBrokTotal = 0;
				double m_extraGroupBrokTotal = 0;
				double m_comBalancebrokTotal = 0;
				if (m_brokerageType == 1)
				{
					m_newbrolerage = (m_TotalTradedLot * m_brokeragerate);
					m_subBrokerBrokTotal = (m_TotalTradedLot * m_subBrokerBrokRate);
					m_brokerBrokTotal = (m_TotalTradedLot * m_brokerBrokRate);
					//m_extraGroupBrokTotal = (m_TotalTradedLot * m_m_extraGroupBrokageRate) / 100;
					m_comBalancebrokTotal = (m_TotalTradedLot * m_companyBrokRate);
				}
				else
				{
					m_newbrolerage = (m_TotalTradedTO * m_brokeragerate) / 100;
					m_subBrokerBrokTotal = (m_TotalTradedTO * m_subBrokerBrokRate) / 100;
					m_brokerBrokTotal = (m_TotalTradedTO * m_brokerBrokRate) / 100;
					//m_extraGroupBrokTotal = (m_TotalTradedTO * m_m_extraGroupBrokageRate) / 100;
					m_comBalancebrokTotal = (m_TotalTradedTO * m_companyBrokRate) / 100;
				}
				m_newbrolerage = abs(m_newbrolerage);


				*stnetpos.m_clientBrokarage = m_newbrolerage;
				*stnetpos.m_subbrokerBrokarage = m_subBrokerBrokTotal;
				*stnetpos.m_brokerBrokarage = m_brokerBrokTotal;
				*stnetpos.m_companyBrokarage = m_comBalancebrokTotal;



				CStaticClass::st_orika_brokerageLoginSymbolWise  st_pac = {};

				st_pac.m_clientBrokTotal = m_newbrolerage;
				st_pac.m_subBrokerBrokTotal = m_subBrokerBrokTotal;
				st_pac.m_brokerBrokTotal = m_brokerBrokTotal;
				st_pac.m_comBrokTotal = m_comBalancebrokTotal;
				CStaticClass::m_brokerageLoginSymbolWiseHastable.SetAt(ClientSymbolGroupKey, st_pac);


				CStaticClass::m_mutex_TotalBrokerage.Unlock();
				//End Of Recalculating Brokerage



				*stnetpos.m_clientexposure = (*stnetpos.m_average * (*stnetpos.m_volume) * (*stnetpos.m_multi));
				*stnetpos.m_subbrokerexposure = ((*stnetpos.m_clientexposure) * (*stnetpos.m_subbrokerRatio)) / 100;
				*stnetpos.m_brokerexposure = ((*stnetpos.m_clientexposure) * (*stnetpos.m_brokerRatio)) / 100;
				*stnetpos.m_companyexposure = ((*stnetpos.m_clientexposure) * (*stnetpos.m_companyRatio)) / 100;

				//*stnetpos.m_clientfloatingpl		
				*stnetpos.m_subbrokerfloatingpl = ((*stnetpos.m_clientfloatingpl) * (*stnetpos.m_subbrokerRatio)) / 100;
				*stnetpos.m_brokerfloatingpl = ((*stnetpos.m_clientfloatingpl) * (*stnetpos.m_brokerRatio)) / 100;
				*stnetpos.m_companyfloatingpl = ((*stnetpos.m_clientfloatingpl) * (*stnetpos.m_companyRatio)) / 100;


				//*stnetpos.m_clientbalance;
				*stnetpos.m_subbrokerbalance = ((*stnetpos.m_clientbalance) * (*stnetpos.m_subbrokerRatio)) / 100;
				*stnetpos.m_brokerbalance = ((*stnetpos.m_clientbalance) * (*stnetpos.m_brokerRatio)) / 100;
				*stnetpos.m_companybalance = ((*stnetpos.m_clientbalance) * (*stnetpos.m_companyRatio)) / 100;


				*stnetpos.m_ClientGrossAmount = *stnetpos.m_clientbalance + *stnetpos.m_clientfloatingpl;
				*stnetpos.m_SubBrokerGrossAmount = ((*stnetpos.m_ClientGrossAmount) * (*stnetpos.m_subbrokerRatio)) / 100;
				*stnetpos.m_BrokerGrossAmount = ((*stnetpos.m_ClientGrossAmount) * (*stnetpos.m_brokerRatio)) / 100;
				*stnetpos.m_CompanyGrossAmount = ((*stnetpos.m_ClientGrossAmount) * (*stnetpos.m_companyRatio)) / 100;


				*stnetpos.m_clientnetamount = *stnetpos.m_ClientGrossAmount - *stnetpos.m_clientBrokarage;
				*stnetpos.m_subbrokerNetAmount = *stnetpos.m_SubBrokerGrossAmount - *stnetpos.m_subbrokerBrokarage;
				*stnetpos.m_brokerNetAmount = *stnetpos.m_BrokerGrossAmount - *stnetpos.m_brokerBrokarage;
				*stnetpos.m_companyNetAmount = *stnetpos.m_CompanyGrossAmount - *stnetpos.m_companyBrokarage;


				double m_clientplnet = *stnetpos.m_clientnetamount;
				double m_subbrokerplnet = *stnetpos.m_subbrokerNetAmount;
				double m_brokerplnet = *stnetpos.m_brokerNetAmount;
				double m_companyplnet = *stnetpos.m_companyNetAmount;



				CStaticClass::mapNetPositionClientWise.SetAt(ClientSymbolKey, stnetpos);
				CString strUpdateData = L"";
				int firstCheck = 0;
				CString str_columnJson = L"";





				writer.StartObject();


				CString strLogin = stnetpos.m_login;
				CString strSymbol = stnetpos.m_symbol;
				writer.Key("login");
				string sslogin = string(CT2CA(strLogin));
				const char* stlogin = sslogin.c_str();
				writer.String(stlogin);
				writer.Key("symbol");
				string sssymbol = string(CT2CA(strSymbol));
				const char* stsymbol = sssymbol.c_str();
				writer.String(stsymbol);




				writer.Key("name");
				string ssname = string(CT2CA(name));
				const char* stName = ssname.c_str();
				writer.String(stName);



				writer.Key("company");
				string sscompany = string(CT2CA(company));
				const char* stcompany = sscompany.c_str();
				writer.String(stcompany);


				writer.Key("subbroker");
				string sssubbroker = string(CT2CA(subbroker));
				const char* stsubbroker = sssubbroker.c_str();
				writer.String(stsubbroker);
				writer.Key("broker");
				string ssbroker = string(CT2CA(broker));
				const char* stbroker = ssbroker.c_str();
				writer.String(stbroker);
				writer.Key("creditLimit");
				double creditLimit = _wtof(StrCreditLimit);
				writer.Double(creditLimit);



				writer.Key("clientplnet");
				writer.Double(m_clientplnet);
				writer.Key("companyplnet");
				writer.Double(m_companyplnet);
				writer.Key("brokerplnet");
				writer.Double(m_brokerplnet);
				writer.Key("subbrokerplnet");
				writer.Double(m_subbrokerplnet);


				writer.Key("clientbrokerage");
				writer.Double(m_newbrolerage);
				writer.Key("brokerbrokerage");
				writer.Double(m_brokerBrokTotal);
				writer.Key("subbrokerbrokerage");
				writer.Double(m_subBrokerBrokTotal);
				writer.Key("companybrokerage");
				writer.Double(m_comBalancebrokTotal);


				writer.EndObject();

			}
		}
		writer.EndArray();
		writer.EndObject();


		CString str_FinalJsonUpdate = L"";
		string strforsend = "";
		str_FinalJsonUpdate = s.GetString();
		strforsend = CT2A(str_FinalJsonUpdate.GetString());
		CStaticClass objCStaticClass;
		objCStaticClass.sendDataToAllClient(strforsend);
		//End of Updating Hash table of netposition
		CStaticClass::m_mutex_Tick.Unlock();
	
}

void CManager::LoadMTClientMaster()
{
	INT64		INT_LOGIN = 0;
	UINT totalNoOfPosition = 0;
	int r_count = 0;
	IMTConGroup* group;
	MTAPIRES res;
	group = m_manager->GroupCreate();
	for (UINT i = 0; m_manager->GroupNext(i, group) == MT_RET_OK; i++)
	{
		CString strGroup = group->Group();
		CString strGroup_Test = group->Group();
		
		UINT64* logins = NULL;
		UINT    logins_total = 0;
		if (res = m_manager->UserLogins(group->Group(), logins, logins_total) == MT_RET_OK)
		{
			if (logins && logins_total)
				for (UINT j = 0; j < logins_total; j++)
				{
					INT_LOGIN = logins[j];
					IMTUser* m_user = m_manager->UserCreate();
					m_manager->UserGet(INT_LOGIN, m_user);					
					CString m_name = m_user->Name();										
					CString m_comment = m_user->Comment();					
					CString m_address = m_user->Address();
					CString m_leadsource = m_user->LeadSource();
					CString m_leadcampaign = m_user->LeadCampaign();
					UINT m_language = m_user->Language();
					
					CString strGroup = m_user->Group();
					
					CStaticClass::st_Orika_MTclientmaster m_st_Orika_MTclientmaster = {};
					CMTStr::Copy(m_st_Orika_MTclientmaster.m_name, m_name);
					CMTStr::Copy(m_st_Orika_MTclientmaster.m_comment, m_comment);
					CMTStr::Copy(m_st_Orika_MTclientmaster.m_address, m_address);
					CMTStr::Copy(m_st_Orika_MTclientmaster.m_leadsource, m_leadsource);
					CMTStr::Copy(m_st_Orika_MTclientmaster.m_leadcampaign, m_leadcampaign);
					m_st_Orika_MTclientmaster.m_language=m_language;
					CMTStr::Copy(m_st_Orika_MTclientmaster.m_group, strGroup);
					
					CStaticClass::m_Orika_MTclientmasterHasTable.SetAt(INT_LOGIN, m_st_Orika_MTclientmaster);
					m_user->Release();
				}
		}
	}
	group->Release();
}



void CManager::UpdateClientMaster()
{	
	INT64		INT_LOGIN=0;
	UINT totalNoOfPosition=0;
	int r_count=0;
	IMTConGroup *group;
	MTAPIRES res;
	group=m_manager->GroupCreate();	 	
    for(UINT i=0;m_manager->GroupNext(i,group)==MT_RET_OK;i++)
    {
		CString strGroup = group->Group();
		CString strGroup_Test = group->Group();
		CString broker = L"";
		CString subbroker = L"";
		CString extragroup = L"";
		CString Exchange = L"";

		CString company = L"";
		if (strGroup.Find(L"\\") >= 0)
		{
			company = strGroup.Mid(0, strGroup.Find(L"\\"));
		}
		strGroup = strGroup.Mid(strGroup.Find(L"\\") + 1, strGroup.GetLength() - strGroup.Find(L"\\") - 1);

		if (strGroup.Find(L"\\") >= 0)
		{
			broker = strGroup.Mid(0, strGroup.Find(L"\\"));
			strGroup = strGroup.Mid(strGroup.Find(L"\\") + 1, strGroup.GetLength() - strGroup.Find(L"\\") - 1);
		}
		else
		{
			broker = strGroup;
			strGroup = L"";
		}

		if (strGroup.Find(L"\\") >= 0)
		{
			subbroker = strGroup.Mid(0, strGroup.Find(L"\\"));
			strGroup = strGroup.Mid(strGroup.Find(L"\\") + 1, strGroup.GetLength() - strGroup.Find(L"\\") - 1);
		}
		else
		{
			subbroker = strGroup;
			strGroup = L"";
		}

		if (strGroup.Find(L"\\") >= 0)
		{
			extragroup = strGroup.Mid(0, strGroup.Find(L"\\"));
			strGroup = strGroup.Mid(strGroup.Find(L"\\") + 1, strGroup.GetLength() - strGroup.Find(L"\\") - 1);
		}
		else
		{
			extragroup = strGroup;
			strGroup = L"";
		}

		//extragroup = strGroup;
		strGroup = L"";

		//extragroup.Replace(L"\\", L" ");

		//subbroker = subbroker + L" " + extragroup;

		UINT64* logins = NULL;
		UINT    logins_total = 0;
		if(res=m_manager->UserLogins(group->Group(),logins,logins_total)==MT_RET_OK)
		{			  						  
			if(logins && logins_total)				 
			for(UINT j=0;j<logins_total;j++)
			{		
				INT_LOGIN=logins[j];
			    IMTUser *m_user=m_manager->UserCreate();
				m_manager->UserGet(INT_LOGIN,m_user);
								
				UpdateClientMasterFromMuser(m_user);								
			}
		}
	}
	group->Release();
}


void CManager::UpdateClientMaster_Manual()
{
	INT64		INT_LOGIN = 0;
	UINT totalNoOfPosition = 0;
	int r_count = 0;
	IMTConGroup* group;
	MTAPIRES res;
	group = m_manager->GroupCreate();
	for (UINT i = 0; m_manager->GroupNext(i, group) == MT_RET_OK; i++)
	{
		CString strGroup = group->Group();
		CString strGroup_Test = group->Group();
		CString broker = L"";
		CString subbroker = L"";
		CString extragroup = L"";
		CString Exchange = L"";

		CString company = L"";
		if (strGroup.Find(L"\\") >= 0)
		{
			company = strGroup.Mid(0, strGroup.Find(L"\\"));
		}
		strGroup = strGroup.Mid(strGroup.Find(L"\\") + 1, strGroup.GetLength() - strGroup.Find(L"\\") - 1);

		if (strGroup.Find(L"\\") >= 0)
		{
			broker = strGroup.Mid(0, strGroup.Find(L"\\"));
			strGroup = strGroup.Mid(strGroup.Find(L"\\") + 1, strGroup.GetLength() - strGroup.Find(L"\\") - 1);
		}
		else
		{
			broker = strGroup;
			strGroup = L"";
		}

		if (strGroup.Find(L"\\") >= 0)
		{
			subbroker = strGroup.Mid(0, strGroup.Find(L"\\"));
			strGroup = strGroup.Mid(strGroup.Find(L"\\") + 1, strGroup.GetLength() - strGroup.Find(L"\\") - 1);
		}
		else
		{
			subbroker = strGroup;
			strGroup = L"";
		}

		if (strGroup.Find(L"\\") >= 0)
		{
			extragroup = strGroup.Mid(0, strGroup.Find(L"\\"));
			strGroup = strGroup.Mid(strGroup.Find(L"\\") + 1, strGroup.GetLength() - strGroup.Find(L"\\") - 1);
		}
		else
		{
			extragroup = strGroup;
			strGroup = L"";
		}

		//extragroup = strGroup;
		strGroup = L"";

		//extragroup.Replace(L"\\", L" ");

		//subbroker = subbroker + L" " + extragroup;

		UINT64* logins = NULL;
		UINT    logins_total = 0;
		if (res = m_manager->UserLogins(group->Group(), logins, logins_total) == MT_RET_OK)
		{
			if (logins && logins_total)
				for (UINT j = 0; j < logins_total; j++)
				{
					INT_LOGIN = logins[j];
					IMTUser* m_user = m_manager->UserCreate();
					m_manager->UserGet(INT_LOGIN, m_user);

					UpdateClientMasterFromMuser_Manual(m_user);
				}
		}
	}
	group->Release();
}


void  CManager::OnUserAdd(const IMTUser* user)
{
	UpdateClientMasterFromMuser(user);
}

void  CManager::OnUserUpdate(const IMTUser* user)
{
	UpdateClientMasterFromMuser(user);
}


void  CManager::OnSymbolUpdate(const IMTConSymbol* config)
{
	UpdateSymbolMasterFromSymbol(config);

	CStaticClass::m_mutex_order.Lock();	
	//(L"L114");
	CString m_OSymbol = config->Symbol();
	CString m_OSource = config->Source();	
	CString m_preSymbol = L"";
	CStaticClass::m_SymbolSourceArray.Lookup(m_OSource, m_preSymbol);
	if (m_preSymbol.GetLength() > 0)
	{
		m_preSymbol = m_preSymbol+L":" + m_OSymbol;
	}
	else
	{
		m_preSymbol = m_OSymbol;
	}	
	CStaticClass::m_SymbolSourceArray.SetAt(m_OSource, m_preSymbol);
	CStaticClass::m_mutex_order.Unlock();
	//(L"UL114");
}


void CManager::UpdateSymbolMasterFromSymbol(const IMTConSymbol* symbol)
{
	CString tmpgroup = symbol->Path();
	CString symbolName = symbol->Symbol();
	CString group = symbolName.Mid(0, symbolName.GetLength() - 3);

	if (tmpgroup.Find(L"Options") > 0)
	{
		group = L"option";
	}

	//Inserting Data In Symbol Group
	CString symbolGroup = group;
	int symbolWiseBuyLimit = 0;
	int symbolWiseSellLimit = 0;
	int symbolWisePendingOrderEnableDisable = 0;
	int symbolPositionLimit = 0;
	double  symbolPendingOrderDiffFromBidAsk = 0;
	double symbolMargin = symbol->MarginInitial();
	double expectedChange = 0;
	int binsize = 0;
	int pocketnumber = 0;
	double maxlimitOnsameprice = 0;
	CString strCommand = L"";
	/*if (group==L"option")
	{*/
	strCommand.Format(L"exec updatesymbolgroup '%s','%d','%d','%d','%d','%.2f','%.2f','%.2f','%d','%d','%.2f';", symbolGroup, symbolWiseBuyLimit, symbolWiseSellLimit, symbolWisePendingOrderEnableDisable, symbolPositionLimit, symbolPendingOrderDiffFromBidAsk, symbolMargin, expectedChange, binsize, pocketnumber, maxlimitOnsameprice);
	CStaticClass::m_sqldata.executeCommand(strCommand);
	//}
	//End of inserting data of symbol group

	//Inserting Data In Symbol		
		//symbol varchar(20),
	double closingPrice = 0;
	double multiplayer = symbol->ContractSize();
	double upperCircuit = 0;
	double lowerCircuit = 0;
	CString ltpMapping = L"";
	CString nextSymbol = L"";
	CString exchangeSymbol = L"";
	int nselotsize = 1;
	int mtlotsize = symbol->ContractSize();



	//CString m_exchange = symbol->Page();
	CString m_exchange = symbol->Exchange();
	CString m_international = symbol->International();
	int m_intindustry = symbol->Industry();
	CString m_industry = L"";
	m_industry.Format(L"%d", m_intindustry);
	CString m_strQtyMulti = symbol->ISIN();	
	CString m_strrmp = symbol->CFI();

	int m_intsector = symbol->Sector();
	CString m_sector = L"";
	m_sector.Format(L"%d", m_intsector);
	double  m_rmp = _wtof(m_strrmp);


	double m_QtyMulti = _wtof(m_strQtyMulti);

	CString m_page = symbol->Page();
	CString m_categary = symbol->Category();

	CString m_currencybase = symbol->CurrencyBase();


	//CString symbolGroup;
	CString ltpSymbol = L"";
	//CString group;
	/*if (group==L"option")
	{*/
	strCommand.Format(L"exec UpdateSymbolmaster '%s','%.4f','%.4f','%.4f','%.4f','%.4f','%s','%s','%s','%d','%d','%s','%s','%s', '%s', '%s', '%s', '%s', '%.4lf', '%.4lf', '%s', '%s','%s';", symbolName, closingPrice, multiplayer, symbolMargin, upperCircuit, lowerCircuit, ltpMapping, nextSymbol, exchangeSymbol, nselotsize, mtlotsize, symbolGroup, ltpSymbol, group, m_exchange, m_international, m_sector, m_industry, m_rmp, m_QtyMulti, m_page, m_categary, m_currencybase);
	CStaticClass::m_sqldata.executeCommand(strCommand);


	CStaticClass::m_mutex_Tick.Lock();
	CStaticClass::m_mutex_ClientList.Lock();
	//(L"_117");
	//Updating Netposition Hash Table		



	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("type");
	writer.String("CLIENT_POSITION");
	writer.Key("updatekey");
	writer.StartArray();
	writer.String("login");
	writer.String("symbol");
	writer.EndArray();
	writer.Key("update");
	writer.StartArray();
	POSITION pos = CStaticClass::m_Orika_clientmasterHastable.GetStartPosition();
	
	while (pos != NULL)
	{
		CString strclientkey = L"";
		CStaticClass::st_Orika_clientmaster m_st = {};
		CStaticClass::m_Orika_clientmasterHastable.GetNextAssoc(pos, strclientkey, m_st);
		CString ClientSymbolKey = L"";
		CString strLogin = L"";
		strLogin=m_st.m_login;
		ClientSymbolKey.Format(L"%s:%s", strLogin, symbolName);
		CStaticClass::st_netpositionClientWise stnetpos = {};
		CStaticClass::mapNetPositionClientWise.Lookup(ClientSymbolKey, stnetpos);
		
		CString m_GetLoginValue = stnetpos.m_login;
		if (m_GetLoginValue.Trim() != L"")
		{	

			CMTStr::Copy(stnetpos.m_exchange, m_exchange);
			CMTStr::Copy(stnetpos.m_international, m_international);
			CMTStr::Copy(stnetpos.m_sector, m_sector);
			CMTStr::Copy(stnetpos.m_industry, m_industry);
			stnetpos.m_rmp = m_rmp;
			stnetpos.m_QtyMulti = m_QtyMulti;

			

			CMTStr::Copy(stnetpos.m_page, m_page);
			CMTStr::Copy(stnetpos.m_categary, m_categary);
			stnetpos.m_rm = ((*stnetpos.m_companyvolume) * (*stnetpos.m_average) * (*stnetpos.m_multi) * stnetpos.m_rmp ) / 10000000;
			

			double m_rm = stnetpos.m_rm;
			CMTStr::Copy(stnetpos.m_currencybase, m_currencybase);

			if (stnetpos.m_QtyMulti == 0)
			{
				stnetpos.m_QtyMulti = 1;
			}
			stnetpos.m_qtyAfterMulti = (*stnetpos.m_volume) * stnetpos.m_QtyMulti;

			double m_qtyAfterMulti = stnetpos.m_qtyAfterMulti;
			CStaticClass::mapNetPositionClientWise.SetAt(ClientSymbolKey, stnetpos);
			CString strUpdateData = L"";
			int firstCheck = 0;
			CString str_columnJson = L"";
			



			
			writer.StartObject();


			CString strLogin = stnetpos.m_login;
			CString strSymbol = stnetpos.m_symbol;
			writer.Key("login");
			string sslogin = string(CT2CA(strLogin));
			const char* stlogin = sslogin.c_str();
			writer.String(stlogin);
			writer.Key("symbol");
			string sssymbol = string(CT2CA(strSymbol));
			const char* stsymbol = sssymbol.c_str();
			writer.String(stsymbol);

			writer.Key("exchange");
			string ssexchange = string(CT2CA(m_exchange));
			const char* strexchange = ssexchange.c_str();
			writer.String(strexchange);

			writer.Key("international");
			string ssinternational = string(CT2CA(m_international));
			const char* strinternational = ssinternational.c_str();
			writer.String(strinternational);


			writer.Key("sector");
			string sssector = string(CT2CA(m_sector));
			const char* strsector = sssector.c_str();
			writer.String(strsector);


			writer.Key("industry");
			string ssindustry = string(CT2CA(m_industry));
			const char* strindustry = ssindustry.c_str();
			writer.String(strindustry);



			writer.Key("page");
			string sspage = string(CT2CA(m_page));
			const char* strpage = sspage.c_str();
			writer.String(strpage);


			writer.Key("categary");
			string sscategary = string(CT2CA(m_categary));
			const char* strcategary = sscategary.c_str();
			writer.String(strcategary);


			writer.Key("rm");
			writer.Double(m_rm);
			writer.Key("qtyaftermulti");
			writer.Double(m_qtyAfterMulti);
			writer.Key("rmp");
			writer.Double(m_rmp);
			writer.Key("qtymulti");
			writer.Double(m_QtyMulti);


			writer.Key("currencybase");
			string sscurrencybase = string(CT2CA(m_currencybase));
			const char* strcurrencybase = sscurrencybase.c_str();
			writer.String(strcurrencybase);


			

			writer.EndObject();
			
		}
	}
	writer.EndArray();
	writer.EndObject();
	CString str_FinalJsonUpdate = L"";
	string strforsend = "";
	str_FinalJsonUpdate = s.GetString();
	strforsend = CT2A(str_FinalJsonUpdate.GetString());
	CStaticClass objCStaticClass;
	


	//End of Updating Hash table of netposition
	CStaticClass::m_mutex_Tick.Unlock();
	CStaticClass::m_mutex_ClientList.Unlock();
	//(L"U117");
	objCStaticClass.sendDataToAllClient(strforsend);
	





	




	//}
//End Of Inserting Data In Symbol
}


int CManager::CheckGroup(CString strgroupjson, CString strgroup)
{	
	CMap<CString, LPCTSTR, int, int> grupsMap;
	rapidjson::Document d;
	std::string str = CT2A(strgroupjson.GetString());
	const char* strforjson = str.c_str();
	if (!d.Parse<0>(strforjson).HasParseError())
	{
		if (d.IsArray() == true)
		{
			for (SizeType i = 0; i < d.Size(); i++)
			{
				const Value& subdata = d[i];
				CString group = L"";
				if (subdata.IsString())
				{
					group = subdata.GetString();
					grupsMap.SetAt(group, 1);
				}
			}
		}
	}
	int getGroupValue = 0;
	
	grupsMap.Lookup(L"*", getGroupValue);
	if (getGroupValue == 1)
	{
		return 1;
	}
	grupsMap.Lookup(strgroup, getGroupValue);
	if (getGroupValue == 1)
	{
		return 1;
	}
	//tmpgroup = tmpgroup.Mid(0, tmpgroup.Find(L"\\"));
	CString tempgroup = strgroup;
	CString tmpGroupWithStar = L"";
	while(tempgroup.Find(L"\\") > 0)
	{
		tempgroup = tempgroup.Mid(0, tempgroup.Find(L"\\"));
		tmpGroupWithStar = tempgroup + L"\\*";
		grupsMap.Lookup(strgroup, getGroupValue);
		if (getGroupValue == 1)
		{
			return 1;
		}
	}
	return 0;
}
void CManager::DeleteMTData(CString strgroupjson)
{		
	string ssgroupjason = string(CT2CA(strgroupjson));
	const char* stgroupjason = ssgroupjason.c_str();	
	CMap<CString, LPCTSTR, int, int> m_groupmap;
	rapidjson::Document d;	
	if (!d.Parse<0>(stgroupjason).HasParseError())
	{	
			
			if (d.IsArray() == true)
			{
				for (SizeType i = 0; i < d.Size(); i++)
				{
					const Value& valgroup = d[i];
					CString group = L"";
					if (valgroup.IsString())
					{
						group = valgroup.GetString();
						m_groupmap.SetAt(group, 1);
					}
				}
			}
		
	}






	INT64			server_time = m_manager->TimeServer();
	INT64			to = server_time;
	INT64			from = server_time - (60 * 60 * 24 * 365);
	INT64			INT_LOGIN = 0;
	UINT			totalNoOfPosition = 0;
	int				r_count = 0;
	IMTConGroup*	group;
	MTAPIRES		res;
	int total_order = 0;
	int total_deal = 0;
	int total_position = 0;
	int total_data = 0;
	group = m_manager->GroupCreate();
	for (UINT i = 0; m_manager->GroupNext(i, group) == MT_RET_OK; i++)
	{
		UINT64* logins = NULL;
		UINT    logins_total = 0;
		if (res = m_manager->UserLogins(group->Group(), logins, logins_total) == MT_RET_OK)
		{
			CString tmpgroup = group->Group();			
			int groupChecked = 0;
			m_groupmap.Lookup(tmpgroup, groupChecked);
			if (groupChecked == 1)
			{
			
				IMTPositionArray* positions = m_manager->PositionCreateArray();
				IMTPosition* position = m_manager->PositionCreate();
				IMTOrder* order = m_manager->OrderCreate();
				IMTOrderArray* orders = m_manager->OrderCreateArray();
				IMTDealArray* deals = m_manager->DealCreateArray();
				IMTDeal* deal = m_manager->DealCreate();
				if (logins && logins_total)
					for (UINT j = 0; j < logins_total; j++)
					{
						INT_LOGIN = logins[j];
						if (res = m_admin->HistoryRequest(logins[j], from, to, orders) == MT_RET_OK)
						{
							r_count = orders->Total();
							total_order = total_order + r_count;
							
						}
						if (res = m_admin->OrderRequestOpen(logins[j], orders) == MT_RET_OK)
						{
							r_count = orders->Total();
							total_order = total_order + r_count;
							
						}

						if (res = m_admin->DealRequest(logins[j], from, to, deals) == MT_RET_OK)
						{
							r_count = deals->Total();
							total_deal = total_deal + r_count;
							
						}
						if (res = m_manager->PositionGet(logins[j], positions) == MT_RET_OK)
						{
							r_count = positions->Total();
							total_position = total_position + r_count;
							
						}						
					}
				positions->Release();
				orders->Release();
				deals->Release();
			}
		}
	}

	group->Release();
	total_data = total_order + total_deal + total_position;
	
	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("type");
	writer.String("TOTAL_NO_OF_DATA");
	writer.Key("TOTAL_DATA");
	writer.Int(total_data);
	writer.Key("TOTAL_ORDER");
	writer.Int(total_order);
	writer.Key("TOTAL_DEAL");
	writer.Int(total_deal);
	writer.Key("TOTAL_POSITION");
	writer.Int(total_position);

	writer.EndObject();
	CString str_FinalJsonUpdate = L"";
	string strforsend = "";
	str_FinalJsonUpdate = s.GetString();
	strforsend = CT2A(str_FinalJsonUpdate.GetString());
	CStaticClass objCStaticClass;
	objCStaticClass.sendDataToAllClient(strforsend);











	int deletedData = 0;
	int pendingData = 0;

	group = m_manager->GroupCreate();
	for (UINT i = 0; m_manager->GroupNext(i, group) == MT_RET_OK; i++)
	{
		UINT64* logins = NULL;
		UINT    logins_total = 0;
		if (res = m_manager->UserLogins(group->Group(), logins, logins_total) == MT_RET_OK)
		{
			CString tmpgroup = group->Group();			
			int groupChecked = 0;
			m_groupmap.Lookup(tmpgroup, groupChecked);
			if (groupChecked == 1)
			{			
				IMTPositionArray* positions = m_manager->PositionCreateArray();
				IMTPosition* position = m_manager->PositionCreate();
				IMTOrder* order = m_manager->OrderCreate();
				IMTOrderArray* orders = m_manager->OrderCreateArray();
				IMTDealArray* deals = m_manager->DealCreateArray();
				IMTDeal* deal = m_manager->DealCreate();
				if (logins && logins_total)
					for (UINT j = 0; j < logins_total; j++)
					{
						INT_LOGIN = logins[j];
						if (res = m_admin->HistoryRequest(logins[j], from, to, orders) == MT_RET_OK)
						{
							r_count = orders->Total();
							
							for (UINT p = 0; p < orders->Total(); p++)
							{
								order = orders->Next(p);
								res = m_admin->OrderDelete(order->Order());

								deletedData = deletedData + 1;
								pendingData = total_data - deletedData;
								StringBuffer s;
								Writer<StringBuffer> writer(s);
								writer.StartObject();
								writer.Key("type");
								writer.String("DELETE_DATA_STATUS");
								writer.Key("DELETED_DATA");
								writer.Int(deletedData);
								writer.Key("PENDING_DATA");
								writer.Int(pendingData);
								writer.Key("STATUS");
								writer.String("Order Deleting");
								writer.EndObject();
								CString str_FinalJsonUpdate = L"";
								string strforsend = "";
								str_FinalJsonUpdate = s.GetString();
								strforsend = CT2A(str_FinalJsonUpdate.GetString());
								CStaticClass objCStaticClass;
								objCStaticClass.sendDataToAllClient(strforsend);
							}
						}
						if (res = m_admin->OrderRequestOpen(logins[j], orders) == MT_RET_OK)
						{
							r_count = orders->Total();
							total_order = total_order + r_count;
							for (UINT p = 0; p < orders->Total(); p++)
							{
								order = orders->Next(p);
								res = m_admin->OrderDelete(order->Order());
								deletedData = deletedData + 1;
								pendingData = total_data - deletedData;
								StringBuffer s;
								Writer<StringBuffer> writer(s);
								writer.StartObject();
								writer.Key("type");
								writer.String("DELETE_DATA_STATUS");
								writer.Key("DELETED_DATA");
								writer.Int(deletedData);
								writer.Key("PENDING_DATA");
								writer.Int(pendingData);
								writer.Key("STATUS");
								writer.String("Order Deleting");
								writer.EndObject();
								CString str_FinalJsonUpdate = L"";
								string strforsend = "";
								str_FinalJsonUpdate = s.GetString();
								strforsend = CT2A(str_FinalJsonUpdate.GetString());
								CStaticClass objCStaticClass;
								objCStaticClass.sendDataToAllClient(strforsend);
							}
						}

						if (res = m_admin->DealRequest(logins[j], from, to, deals) == MT_RET_OK)
						{
							r_count = deals->Total();
							total_deal = total_deal + r_count;
							for (UINT p = 0; p < deals->Total(); p++)
							{
								deal = deals->Next(p);
								res = m_admin->DealDelete(deal->Deal());
								deletedData = deletedData + 1;
								pendingData = total_data - deletedData;
								StringBuffer s;
								Writer<StringBuffer> writer(s);
								writer.StartObject();
								writer.Key("type");
								writer.String("DELETE_DATA_STATUS");
								writer.Key("DELETED_DATA");
								writer.Int(deletedData);
								writer.Key("PENDING_DATA");
								writer.Int(pendingData);
								writer.Key("STATUS");
								writer.String("Deal Deleting");
								writer.EndObject();
								CString str_FinalJsonUpdate = L"";
								string strforsend = "";
								str_FinalJsonUpdate = s.GetString();
								strforsend = CT2A(str_FinalJsonUpdate.GetString());
								CStaticClass objCStaticClass;
								objCStaticClass.sendDataToAllClient(strforsend);
							}
						}
						if (res = m_manager->PositionGet(logins[j], positions) == MT_RET_OK)
						{							
							r_count = positions->Total();
							total_position = total_position + r_count;
							for (UINT p = 0; p < positions->Total(); p++)
							{
								position = positions->Next(p);
								res = m_admin->PositionDelete(position);
								deletedData = deletedData + 1;
								pendingData = total_data - deletedData;
								StringBuffer s;
								Writer<StringBuffer> writer(s);
								writer.StartObject();
								writer.Key("type");
								writer.String("DELETE_DATA_STATUS");
								writer.Key("DELETED_DATA");
								writer.Int(deletedData);
								writer.Key("PENDING_DATA");
								writer.Int(pendingData);
								writer.Key("STATUS");
								writer.String("Position Deleting");
								writer.EndObject();
								CString str_FinalJsonUpdate = L"";
								string strforsend = "";
								str_FinalJsonUpdate = s.GetString();
								strforsend = CT2A(str_FinalJsonUpdate.GetString());
								CStaticClass objCStaticClass;
								objCStaticClass.sendDataToAllClient(strforsend);
							}
							double bal1 = 0;
							double bal2 = 0;
							double bal3 = 0;
							double bal4 = 0;							
							res = m_admin->UserBalanceCheck(INT_LOGIN, 1, bal1, bal2, bal3, bal4);
							int check = 0;
						}
						

					}
				positions->Release();
				orders->Release();
				deals->Release();
			}
		}
	}

	group->Release();
	
//}
//End Of Inserting Data In Symbol
}
void CManager::PositionTransferToMT(string strgroupjson,CString filepath, int totalposition)
{	
	const char* stgroupjason = strgroupjson.c_str();
	CMap<CString, LPCTSTR, int, int> m_groupmap;
	rapidjson::Document d;
	if (!d.Parse<0>(stgroupjason).HasParseError())
	{					
			if (d.IsArray() == true)
			{
				for (SizeType i = 0; i < d.Size(); i++)
				{
					const Value& valgroup = d[i];
					CString group = L"";
					if (valgroup.IsString())
					{
						group = valgroup.GetString();
						m_groupmap.SetAt(group, 1);
					}
				}
			}
		
	}

	int transeferedData = 0;
	int pendingData = 0;
	CStdioFile	myFile;
	if (myFile.Open(filepath, CFile::modeRead))
	{
		myFile.Seek(0, CFile::begin);
		CString strval = L"";
		CString m_Login = L"";
		while (myFile.ReadString(strval))
		{
			//Time;Login;Position;Symbol;Type;Volume;Price;Stop Loss;Take Profit;Current Price			
			m_Login = strval.Mid(0, strval.Find(';'));
			CStaticClass::st_Orika_MTclientmaster m_st_Orika_MTclientmaster = {};
			INT64 INT_LOGIN = _wtoi64(m_Login);
			CStaticClass::m_Orika_MTclientmasterHasTable.Lookup(INT_LOGIN, m_st_Orika_MTclientmaster);
			CString m_logingroup = m_st_Orika_MTclientmaster.m_group;

			int groupChecked = 0;
			m_groupmap.Lookup(m_logingroup, groupChecked);
			if (groupChecked == 1)
			{								
				strval = strval.Mid(strval.Find(';') + 1, (strval.GetLength() - strval.Find(';') - 1));
				strval = strval.Mid(strval.Find(';') + 1, (strval.GetLength() - strval.Find(';') - 1));
				CString m_strSymbol = strval.Mid(0, strval.Find(';'));
				
				UINT int_actioan = 0;
				int_actioan = 200;				

				strval = strval.Mid(strval.Find(';') + 1, (strval.GetLength() - strval.Find(';') - 1));
				CString m_strType = strval.Mid(0, strval.Find(';'));

				UINT int_type = 0;
				if (m_strType == L"sell")
				{
					int_type = 1;
				}
				
				strval = strval.Mid(strval.Find(';') + 1, (strval.GetLength() - strval.Find(';') - 1));

				CString strVolume = strval.Mid(0, strval.Find(';'));
				double  dbVolume = 0;
				if (strVolume.Find(L"K") >= 0)
				{
					strVolume = strVolume.Mid(0, strVolume.Find(L"K"));
					dbVolume = _wtof(strVolume);
					dbVolume = dbVolume * 1000 * 10000;
				}
				else if (strVolume.Find(L"k") >= 0)
				{
					strVolume = strVolume.Mid(0, strVolume.Find(L"k"));
					dbVolume = _wtof(strVolume);
					dbVolume = dbVolume * 1000 * 10000;
				}
				else
				{
					dbVolume = _wtof(strVolume);
					dbVolume = dbVolume * 10000;
				}
				

				LPTSTR endPtr;


				strval = strval.Mid(strval.Find(';') + 1, (strval.GetLength() - strval.Find(';') - 1));
				strval = strval.Mid(strval.Find(';') + 1, (strval.GetLength() - strval.Find(';') - 1));
				//strval = strval.Mid(strval.Find(',') + 1, (strval.GetLength() - strval.Find(',') - 1));
				//strval = strval.Mid(strval.Find(',') + 1, (strval.GetLength() - strval.Find(',') - 1));

				double  dbPrice = _wtof(strval);
				//dbPrice
				//CBalanceExampleDlg::t_price=dbPrice;
				
				if (dbVolume != 0)
				{
					trade_transfer(INT_LOGIN, m_strSymbol, int_actioan, int_type, dbVolume, dbPrice, L"", 0);
				}







				transeferedData = transeferedData + 1;
				pendingData = totalposition - transeferedData;
				StringBuffer s;
				Writer<StringBuffer> writer(s);
				writer.StartObject();
				writer.Key("type");
				writer.String("POSITIONTRANSFER_STATUS");
				writer.Key("TRANSFERED_DATA");
				writer.Int(transeferedData);
				writer.Key("PENDING_DATA");
				writer.Int(pendingData);
				writer.Key("STATUS");
				writer.String("Position Transfer");
				writer.EndObject();
				CString str_FinalJsonUpdate = L"";
				string strforsend = "";
				str_FinalJsonUpdate = s.GetString();
				strforsend = CT2A(str_FinalJsonUpdate.GetString());
				CStaticClass objCStaticClass;
				objCStaticClass.sendDataToAllClient(strforsend);
			}
		}
	}		
}








void CManager::UpdateSymbolMaster()
{	
	INT64			INT_LOGIN=0;
	UINT			totalNoOfPosition=0;
	int				r_count=0;
	IMTConSymbol   *symbol;
	MTAPIRES res;	
	symbol=m_manager->SymbolCreate();	 	
    for(UINT i=0;m_manager->SymbolNext(i,symbol)==MT_RET_OK;i++)
    {
		UpdateSymbolMasterFromSymbol(symbol);
    }          
	symbol->Release();	
}

void CManager::UpdateSymbolSource()
{
	INT64			INT_LOGIN = 0;
	UINT			totalNoOfPosition = 0;
	int				r_count = 0;
	IMTConSymbol* symbol;
	MTAPIRES res;
	symbol = m_manager->SymbolCreate();
	for (UINT i = 0; m_manager->SymbolNext(i, symbol) == MT_RET_OK; i++)
	{
		CStaticClass::m_mutex_order.Lock();
		//(L"Orderlock_68");
			CString m_symbol = symbol->Symbol();
			CString m_source = symbol->Source();			
			CString m_preSymbol = L"";
			CStaticClass::m_SymbolSourceArray.Lookup(m_source, m_preSymbol);
			if (m_preSymbol.GetLength() > 0)
			{
				m_preSymbol = m_preSymbol + L":" + m_symbol;
			}
			else
			{
				m_preSymbol = m_symbol;
			}
			CStaticClass::m_SymbolSourceArray.SetAt(m_source, m_preSymbol);
		CStaticClass::m_mutex_order.Unlock();
		//(L"U_Orderlock_68");
	}
	symbol->Release();
}

int CManager::Change_Comment(CString OldComment,CString NewComment,UINT64 Dealno)
{		
	MTAPIRES res;	
	IMTDeal* deal=NULL;	

	if(!(deal=m_manager->DealCreate()))
     {
      m_manager->LoggerOut(MTLogErr,L"DealCreateArray fail");
      //AfxMessageBox(L"DealCreateArray fail");
      return(false);
     }

	res=m_admin->DealRequest(Dealno,deal);
	res=deal->Comment(NewComment);
	res=m_admin->DealUpdate(deal);

	if (res!=MT_RET_OK)
	{
		return 0;
	}
	return 1;
}


int CManager::trade_transfer(UINT64 login, LPCWSTR symbol, UINT  action, UINT ordertype, double m_volume, double price, LPCWSTR comment, UINT64 expiration)
{	
	//m_MutexForMTop.Lock();
	MTAPIRES              res = MT_RET_OK_NONE;
	
	IMTRequest* request = m_manager->RequestCreate();
	IMTRequest* result = m_manager->RequestCreate();
	IMTConfirm* confirm = m_manager->DealerConfirmCreate();
	CDealerSink sink;
	UINT64 volume = m_volume*10000;


	UINT        id = 0;

	if (request && result && confirm && sink.Initialize(result, confirm))
	{
		//--- buy 1.00 EURUSD
		request->Clear();
		UINT64 int64Login = login;
		request->Login(int64Login);
		request->Action(IMTRequest::TA_DEALER_POS_EXECUTE);
		UINT intaction = 0;
		if (ordertype == 0)
		{
			request->Type(IMTOrder::OP_BUY);
		}
		if (ordertype == 1)
		{
			request->Type(IMTOrder::OP_SELL);
		}
		request->Volume(volume);
		CString str_symbol = symbol;
		str_symbol = str_symbol.Trim();
		request->Symbol(str_symbol);

		request->PriceOrder(price);
		request->Comment(comment);
		res = m_manager->DealerSend(request, &sink, id);
		if (res == MT_RET_OK)
		{
			res = sink.Wait(10000);
		}

		if (request)
		{
			request->Release();
			request = NULL;
		}
		if (result)
		{
			result->Release();
			result = NULL;
		}
		if (confirm)
		{
			confirm->Release();
			confirm = NULL;
		}
				
	}
	//m_MutexForMTop.Unlock();
	return res;
}

CString  CManager::TDHTradeExecution(TDHLoginArray& m_BuyLoginAndLot, TDHLoginArray& m_SellLoginAndLot, CString m_buySymbol, double buySymbolBidRate, CString TDHBuyComment, CString m_sellSymbol, double sellSymbolBidRate, CString TDHSellComment)
{
	CString strMessage = L"";
	CString strReturnVal = L"";
	CString strBuyMessage = L"";
	CString strSellMessage = L"";
	CString  messageType = L"success";
	//Execute Trade 
	int trdeRequestStatus = 0;
	int totalBuyLogin = m_BuyLoginAndLot.Total();
	int totalsellLogin = m_SellLoginAndLot.Total();
	TDHLogin m_stLogin = {};
	int m_marginOK = 1;
	//Check Margin
	CMap<int, int, double, double> ClientMarginRequired;
	for (int i = 0; i < totalBuyLogin; i++)
	{
		m_stLogin = m_BuyLoginAndLot[i];
		int tmplogin = m_stLogin.m_login;
		double tmplot = m_stLogin.m_lot;
		//trdeRequestStatus = trade_transfer(tmplogin, m_buySymbol, 0, 0, tmplot, buySymbolBidRate, TDHBuyComment, 0);

		IMTConSymbol* symbol;
		MTAPIRES res;
		symbol = m_manager->SymbolCreate();
		m_manager->SymbolGet(m_buySymbol, symbol);
		double margin = symbol->MarginInitial();
		symbol->Release();
		double totalMargin = 0;
		ClientMarginRequired.Lookup(tmplogin, totalMargin);		
		double calculatedMarin = tmplot* margin;
		totalMargin = totalMargin + calculatedMarin;
		ClientMarginRequired.SetAt(tmplogin, totalMargin);
		
	}

	for (int i = 0; i < totalsellLogin; i++)
	{
		m_stLogin = m_SellLoginAndLot[i];
		int tmplogin = m_stLogin.m_login;
		double tmplot = m_stLogin.m_lot;
		//trdeRequestStatus = trade_transfer(tmplogin, m_sellSymbol, 0, 1, tmplot, sellSymbolBidRate, TDHSellComment, 0);
		IMTConSymbol* symbol;
		MTAPIRES res;
		symbol = m_manager->SymbolCreate();
		m_manager->SymbolGet(m_sellSymbol, symbol);
		double margin = symbol->MarginInitial();
		symbol->Release();
		double totalMargin = 0;
		ClientMarginRequired.Lookup(tmplogin, totalMargin);
		double calculatedMarin = tmplot * margin;
		totalMargin = totalMargin + calculatedMarin;
		ClientMarginRequired.SetAt(tmplogin, totalMargin);
	}


	POSITION pos = ClientMarginRequired.GetStartPosition();
	while (pos != NULL)
	{
		int login = 0;
		double TotalRequiredMargin= 0;
		ClientMarginRequired.GetNextAssoc(pos, login, TotalRequiredMargin);
		MTAPIRES res;
		UINT64 m_IntUser = login;
		/*IMTUser* m_user = m_manager->UserCreate();			
		m_manager->UserGet(m_IntUser, m_user);		
		m_user->Release();*/

		IMTAccount* m_account = m_manager->UserCreateAccount();

		
		m_manager->UserAccountGet(m_IntUser, m_account);
		double ClientBalance = m_account->MarginFree();
		m_account->Release();
		if (ClientBalance < TotalRequiredMargin)
		{
			m_marginOK = 0;
			strMessage.Format(L"%d Margin Required %.0f and available margin is %.0f", login, TotalRequiredMargin, ClientBalance);
			messageType = L"error";
		}
	}

	//End of Checking Margin
	if (m_marginOK == 1)
	{
		for (int i = 0; i < totalBuyLogin; i++)
		{
			m_stLogin = m_BuyLoginAndLot[i];
			int tmplogin = m_stLogin.m_login;
			double tmplot = m_stLogin.m_lot;
			tmplot = tmplot ;
			trdeRequestStatus = trade_transfer(tmplogin, m_buySymbol, 0, 0, tmplot, buySymbolBidRate, TDHBuyComment, 0);
			CString tmpMessage = L"";


			CString m_desc = L"";
			CStaticClass::MessageCodeList.Lookup(trdeRequestStatus, m_desc);

			if (trdeRequestStatus == 10009)
			{
				tmpMessage.Format(L"%.2lf Lots of Buy %s bought in %d  @%.4lf ", tmplot, m_buySymbol, tmplogin, buySymbolBidRate);
			}
			else
			{
				tmpMessage.Format(L"Error %d:\"%s\"   for   %.2lf Lots of Buy %s bought in %d  @%.4lf ", trdeRequestStatus, m_desc, tmplot, m_buySymbol, tmplogin, buySymbolBidRate);
				messageType = L"error";
			}
			strBuyMessage = strBuyMessage + L"\r\n" + tmpMessage;
		}
	strBuyMessage = strBuyMessage.Mid(2, strBuyMessage.GetLength() - 2);

	for (int i = 0; i < totalsellLogin; i++)
	{
		m_stLogin = m_SellLoginAndLot[i];
		int tmplogin = m_stLogin.m_login;
		double tmplot = m_stLogin.m_lot;
		tmplot = tmplot ;
		trdeRequestStatus = trade_transfer(tmplogin, m_sellSymbol, 0, 1, tmplot, sellSymbolBidRate, TDHSellComment, 0);
		CString tmpMessage = L"";
		CString m_desc = L"";
		CStaticClass::MessageCodeList.Lookup(trdeRequestStatus, m_desc);
		if (trdeRequestStatus == 10009)
		{
			tmpMessage.Format(L"%.2lf Lots of Sold %s sold in %d @%.4lf", tmplot, m_sellSymbol, tmplogin, sellSymbolBidRate);
		}
		else
		{
			tmpMessage.Format(L"Error %d:\"%s\"   for   %.2lf Lots of Sold %s bought in %d @%.4lf", trdeRequestStatus, m_desc, tmplot, m_sellSymbol, tmplogin, sellSymbolBidRate);
			messageType = L"error";
		}

		strSellMessage = strSellMessage + L"\r\n" + tmpMessage;
	}
	strSellMessage = strSellMessage.Mid(2, strSellMessage.GetLength() - 2);
	strMessage.Format(L"%s \r\n %s", strBuyMessage, strSellMessage);
}
	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("type");
	writer.String("TDH_RESPONSE_MESSAGE");
	writer.Key("message");
	string str_message = "";
	str_message = CT2A(strMessage.GetString());
	const char* charmessage = str_message.c_str();
	writer.String(charmessage);


	writer.Key("messagetype");
	string str_messageType = CT2A(messageType.GetString());
	const char* charmessageType = str_messageType.c_str();
	writer.String(charmessageType);


	writer.EndObject();
	strReturnVal = s.GetString();
	return strReturnVal;
}


CString CManager::TDHExecution(CString m_buySymbol, TDHLoginArray& m_BuyLoginAndLot, double m_buyRate, CString m_sellSymbol, TDHLoginArray& m_SellLoginAndLot, double m_sellRate, CString m_executionType)
{



	CString strReturnVal = L"";	
	CString TDHBuyComment = L"";
	CString TDHSellComment = L"";
	CString strMessage = L"";
	SYSTEMTIME	time;
	::GetLocalTime(&time);
	CStaticClass m_staticclass;
	CString TDHExecutisionTime = m_staticclass.sysDateToStringFormat(time);

	CString m_AllSellLogins = L"";
	CString m_AllBuyLogins = L"";
	int totalsellLogin = m_SellLoginAndLot.Total();
	TDHLogin m_stLogin = {};
	double m_totalBuyLot = 0;
	double m_totalSellLot = 0;
	for (int i = 0; i < totalsellLogin; i++)
	{
		m_stLogin = m_SellLoginAndLot[i];
		int tmplogin= m_stLogin.m_login;
		CString strTmpLogin = L"";
		strTmpLogin.Format(L"%d", tmplogin);
		m_AllSellLogins = m_AllSellLogins + L"," + strTmpLogin;

		double tmpqty = m_stLogin.m_lot;
		m_totalBuyLot = m_totalBuyLot+ tmpqty;

	}	
	m_AllSellLogins = m_AllSellLogins.Mid(1, m_AllSellLogins.GetLength()-1);



	int totalBuyLogin = m_BuyLoginAndLot.Total();
	m_stLogin = {};
	for (int i = 0; i < totalBuyLogin; i++)
	{
		m_stLogin = m_BuyLoginAndLot[i];
		int tmplogin = m_stLogin.m_login;
		CString strTmpLogin = L"";
		strTmpLogin.Format(L"%d", tmplogin);
		m_AllBuyLogins = m_AllBuyLogins + L"," + strTmpLogin;


		double tmpqty = m_stLogin.m_lot;
		m_totalSellLot = m_totalSellLot + tmpqty;
	}
	m_AllBuyLogins = m_AllBuyLogins.Mid(1, m_AllBuyLogins.GetLength() - 1);

	TDHBuyComment.Format(L"TDH-%s-%d", TDHExecutisionTime, m_AllSellLogins);
	TDHSellComment.Format(L"TDH-%s-%d", TDHExecutisionTime, m_AllBuyLogins);

	CStaticClass::st_Orika_symbolmaster m_st_Orika_symbolmaster = {};
	CStaticClass::m_Orika_symbolmasterHastable.Lookup(m_buySymbol, m_st_Orika_symbolmaster);
	double buyMulti = m_st_Orika_symbolmaster.m_multiplayer;
	CString m_buySymbolGroup = m_st_Orika_symbolmaster.m_symbolGroup;
	m_st_Orika_symbolmaster = {};
	CStaticClass::m_Orika_symbolmasterHastable.Lookup(m_sellSymbol, m_st_Orika_symbolmaster);
	double sellMulti = m_st_Orika_symbolmaster.m_multiplayer;
	CString m_sellSymbolGroup = m_st_Orika_symbolmaster.m_symbolGroup;


	if (m_executionType == L"market")
	{
		double buySymbolBidRate = 0;
		double sellSymbolBidRate = 0;
		MTTickShort tick = {};
		m_manager->TickLast(m_buySymbol, tick);
		buySymbolBidRate = tick.bid;
		m_manager->TickLast(m_sellSymbol, tick);
		sellSymbolBidRate = tick.bid;
		double buyOrSellDifference = ((buyMulti * m_totalBuyLot * buySymbolBidRate) - (sellMulti * m_totalSellLot * sellSymbolBidRate));
		if (buyOrSellDifference < 0)
		{
			buyOrSellDifference = -buyOrSellDifference;
		}
		if (buySymbolBidRate != sellSymbolBidRate)
		{			
			strMessage = L"Price of both symbols are not matching Please try again.";
			strReturnVal.Format(L"{ \"type\" : \"TDH_RESPONSE_MESSAGE\",\"message\":\"%s\",\"messagetype\":\"error\" }", strMessage);
			return strReturnVal;
		}
		/*else if (buyOrSellDifference>200000)
		{
			strMessage = L"Value of buy and sell lots are not same, Please check buy and sell lots.";
			strReturnVal.Format(L"{ \"type\" : \"TDH_RESPONSE_MESSAGE\",\"message\":\"%s\" }", strMessage);
			return strReturnVal;
		}*/
		else if (buySymbolBidRate ==0 ||  sellSymbolBidRate==0)
		{
			strMessage.Format(L"Buy Symbol BidRate:%.2lf and Sell Symbol BidRate:%.2lf", buySymbolBidRate, sellSymbolBidRate);
			strReturnVal.Format(L"{ \"type\" : \"TDH_RESPONSE_MESSAGE\",\"message\":\"%s\" ,\"messagetype\":\"error\"}", strMessage);
			return strReturnVal;
		}
		else
		{
			strReturnVal=TDHTradeExecution(m_BuyLoginAndLot, m_SellLoginAndLot, m_buySymbol, buySymbolBidRate, TDHBuyComment, m_sellSymbol, sellSymbolBidRate, TDHSellComment);
			return strReturnVal;
		}
	}
	if (m_executionType == L"rate")
	{
		double buySymbolBidRate = m_buyRate;
		double sellSymbolBidRate = m_sellRate;		

		
		

		m_stLogin = {};
		double buyCalculatision = 0;
		double sellCalculatision = 0;
		for (int i = 0; i < totalBuyLogin; i++)
		{
			m_stLogin = m_BuyLoginAndLot[i];
			int tmplogin = m_stLogin.m_login;
			CString strTmpLogin = L"";
			strTmpLogin.Format(L"%d", tmplogin);
			

			double tmpqty = m_stLogin.m_lot;
			

			CString strBuyKey = L"";
			strBuyKey.Format(L"%d:%s", tmplogin, m_buySymbolGroup);
			

			//Getting Buy Patti
			CStaticClass::st_Orika_PLDevideRatio m_st_Orika_PLDevideRatio = {};
			CStaticClass::m_Orika_PLDevideRatioHastable.Lookup(strBuyKey, m_st_Orika_PLDevideRatio);
			double buyPatti = m_st_Orika_PLDevideRatio.m_companyPLRatio;
			if (buyPatti == 0)
			{
				m_buySymbolGroup = L"";
				strBuyKey.Format(L"%d:%s", tmplogin, m_buySymbolGroup);
				CStaticClass::m_Orika_PLDevideRatioHastable.Lookup(strBuyKey, m_st_Orika_PLDevideRatio);
				buyPatti = m_st_Orika_PLDevideRatio.m_companyPLRatio;
			}
			//End Getting Buy Patti
			if (buyPatti == 0)
			{
				buyPatti = 100;
			}			
			buyCalculatision = buyCalculatision+ (buyPatti * buyMulti * tmpqty * buySymbolBidRate);
		}

		
		for (int i = 0; i < totalsellLogin; i++)
		{
			m_stLogin = m_SellLoginAndLot[i];
			int tmplogin = m_stLogin.m_login;
			CString strTmpLogin = L"";
			strTmpLogin.Format(L"%d", tmplogin);
			

			double tmpqty = m_stLogin.m_lot;


			//Getting Sell Patti
			CString strSellKey = L"";
			strSellKey.Format(L"%d:%s", tmplogin, m_sellSymbolGroup);
			CStaticClass::st_Orika_PLDevideRatio m_st_Orika_PLDevideRatio = {};
			CStaticClass::m_Orika_PLDevideRatioHastable.Lookup(strSellKey, m_st_Orika_PLDevideRatio);
			double sellPatti = m_st_Orika_PLDevideRatio.m_companyPLRatio;
			if (sellPatti == 0)
			{
				m_sellSymbolGroup = L"";
				strSellKey.Format(L"%d:%s", tmplogin, m_sellSymbolGroup);
				CStaticClass::m_Orika_PLDevideRatioHastable.Lookup(strSellKey, m_st_Orika_PLDevideRatio);
				sellPatti = m_st_Orika_PLDevideRatio.m_companyPLRatio;
			}
			if (sellPatti == 0)
			{
				sellPatti = 100;
			}
			//End Getting Sell Patti
			sellCalculatision = sellCalculatision+( sellPatti * sellMulti * tmpqty * sellSymbolBidRate);
		}





		if (buyCalculatision != sellCalculatision)
		{
			strMessage = L"Company Patti in both client are not same Please use Market Execution";
			strReturnVal.Format(L"{ \"type\" : \"TDH_RESPONSE_MESSAGE\",\"message\":\"%s\",\"messagetype\":\"error\" }", strMessage);
			return strReturnVal;
		}
		else
		{
			strReturnVal=TDHTradeExecution(m_BuyLoginAndLot, m_SellLoginAndLot, m_buySymbol, buySymbolBidRate, TDHBuyComment, m_sellSymbol, sellSymbolBidRate, TDHSellComment);
			return strReturnVal;
		}
	}
	strMessage=L"Invalid Request Type";
	strReturnVal.Format(L"{ \"type\" : \"TDH_RESPONSE_MESSAGE\",\"message\":\"%s\" ,\"messagetype\":\"error\"}", strMessage);
	return strReturnVal;
}
void CManager::UpdateOrderINMT(int orderno, CString field, CString txtData)
{
	MTAPIRES res;
	UINT64 intorderno = orderno;
	IMTOrder* or ;	
	or = m_manager->OrderCreate();	
	res = m_manager->OrderGet(intorderno, or );
	CString strLogPrint = L"";	
	if (field == L"ExternalID")
	{		
		or ->ExternalID(txtData);				
	}
	if (res != 13)
	{
		res = m_manager->OrderUpdate(or );
	}
	strLogPrint.Format(L"Order HasBeen Updated With Return Code %d", res);
	//(strLogPrint);
	or ->Release();
}
void CManager::UpdateOrderHistoryINMT(int orderno, CString field, CString txtData)
{
	MTAPIRES res;
	UINT64 intorderno = orderno;

	UINT64* ordernos= &intorderno;
	
	IMTOrder* or ;
	or = m_manager->OrderCreate();	
	IMTOrderArray* orders = m_manager->OrderCreateArray();
	res = m_manager->HistoryRequestByTickets(ordernos,1, orders);
	for (int i = 0; i < orders->Total(); i++)
	{
		or = orders->Next(i);
	}	
	CString strCheckexID = or ->ExternalID();
	if (field == L"ExternalID")
	{
		or ->ExternalID(txtData);
	}
	res=m_manager->OrderUpdate(or);
	or ->Release();	
}
int CManager::Activate_Order(UINT64 OrderNo)
{		
	//m_MutexForMTop.Lock();
	MTAPIRES res;	
	UINT64 intorderno=OrderNo;	
	IMTOrder* or;
	or=m_manager->OrderCreate();
	res=m_manager->OrderGet(intorderno,or);	
	//--- create request
   IMTRequest *request=m_manager->RequestCreate();
   IMTRequest *result =m_manager->RequestCreate();
   IMTConfirm *confirm=m_manager->DealerConfirmCreate();
   CDealerSink sink;   
   UINT        id=0;  
   if(request && result && confirm && sink.Initialize(result,confirm))
   {
		request->Clear();
		request->Order(or->Order());
		request->Login(or->Login());
		request->Action(IMTRequest::TA_DEALER_ORD_ACTIVATE);
		
		request->Type(or->Type());
		request->Volume(or->VolumeInitial());
		request->Symbol(or->Symbol());
		request->PriceOrder(or->PriceOrder());
		res=m_manager->DealerSend(request,&sink,id);
				//---
		if(res==MT_RET_OK)
		{
			res=sink.Wait(1000);
		}						
   }
	
	or->Release();
	if (res== 10009)
	{
		//m_MutexForMTop.Unlock();
		return 1;
	}
	//m_MutexForMTop.Unlock();
	return 0;
}




int CManager::updateMTUserData(CString m_ColumnKey, CString m_value, CString m_DataSeprator, int m_DataIndexForUpdate, CString m_login, CString m_symbol)
{
	MTAPIRES res;
	IMTUser* m_user = m_manager->UserCreate();
	UINT64 m_IntUser = _wtoi64(m_login);
	m_manager->UserGet(m_IntUser, m_user);
	if (m_ColumnKey == L"name")
	{
		m_user->Name(m_value);
		
	}
	if (m_ColumnKey == L"companyRatio")
	{
		m_user->Name(m_value);
		//m_admin->UserUpdate(m_user);
	}
	if (m_ColumnKey == L"brokerRatio")
	{
		m_user->Name(m_value);
		//m_admin->UserUpdate(m_user);
	}
	if (m_ColumnKey == L"subbrokerRatio")
	{
		m_user->Name(m_value);
		//m_admin->UserUpdate(m_user);
	}
	if (m_ColumnKey == L"creditLimit")
	{
		m_user->Comment(m_value);
		//m_admin->UserUpdate(m_user);
	}
	
	//m_manager->UserUpdate(m_user);
	m_admin->UserUpdate(m_user);
	m_user->Release();
	return 0;
}


int CManager::BalanceTransferdataCount(string strgroupjason)
{	
	IMTConGroup* group;
	MTAPIRES		res;	
	int total_Client = 0;
	const char* stgroupjason;
	stgroupjason = strgroupjason.c_str();
	CMap<CString, LPCTSTR, int, int> m_groupmap;
	rapidjson::Document d;
	if (!d.Parse<0>(stgroupjason).HasParseError())
	{					
			if (d.IsArray() == true)
			{
				for (SizeType i = 0; i < d.Size(); i++)
				{
					const Value& valgroup = d[i];
					CString group = L"";
					if (valgroup.IsString())
					{
						group = valgroup.GetString();
						m_groupmap.SetAt(group, 1);
					}
				}
			}
		
	}
	group = m_manager->GroupCreate();
	for (UINT i = 0; m_manager->GroupNext(i, group) == MT_RET_OK; i++)
	{
		UINT64* logins = NULL;
		UINT    logins_total = 0;


		CString tmpgroup = group->Group();		
		int groupChecked = 0;
		m_groupmap.Lookup(tmpgroup, groupChecked);
		if (groupChecked == 1)
		{
			if (res = m_manager->UserLogins(group->Group(), logins, logins_total) == MT_RET_OK)
			{
				total_Client = total_Client + logins_total;
			}
		}
	}
	group->Release();
	return total_Client;
}


void CManager::BalanceTransfer(int TotalClientCount, string strgroupjason)
{
	CStaticClass::m_ClientBalance.RemoveAll();
	INT64		INT_LOGIN = 0;
	UINT totalNoOfPosition = 0;
	int r_count = 0;
	IMTConGroup* group;
	MTAPIRES res;


	const char* stgroupjason;
	stgroupjason = strgroupjason.c_str();
	CMap<CString, LPCTSTR, int, int> m_groupmap;
	rapidjson::Document d;
	if (!d.Parse<0>(stgroupjason).HasParseError())
	{				
			if (d.IsArray() == true)
			{
				for (SizeType i = 0; i < d.Size(); i++)
				{
					const Value& valgroup = d[i];
					CString group = L"";
					if (valgroup.IsString())
					{
						group = valgroup.GetString();
						m_groupmap.SetAt(group, 1);
					}
				}
			}		
	}
	group = m_manager->GroupCreate();
	for (UINT i = 0; m_manager->GroupNext(i, group) == MT_RET_OK; i++)
	{
		UINT64* logins = NULL;
		UINT    logins_total = 0;

		CString tmpgroup = group->Group();
		int groupChecked = 0;
		m_groupmap.Lookup(tmpgroup, groupChecked);
		if (groupChecked == 1)
		{
			if (res = m_manager->UserLogins(group->Group(), logins, logins_total) == MT_RET_OK)
			{
				if (logins && logins_total)
					for (UINT j = 0; j < logins_total; j++)
					{
						INT_LOGIN = logins[j];

						IMTUser* m_user = m_manager->UserCreate();
						m_manager->UserGet(INT_LOGIN, m_user);
						CString strComment = m_user->Comment();
						m_user->Release();
						double  dbcredit = _wtof(strComment);
						CStaticClass::m_ClientBalance.SetAt(INT_LOGIN, dbcredit);
						if (dbcredit > 0)
						{
							DealerBalance(INT_LOGIN, dbcredit, 3, L"Credit", true);
						}
					}
				//account->Release();
			}
		}
	}
	group->Release();
}


bool CManager::DealerBalance(const UINT64 login, const double amount, const UINT type, const LPCWSTR comment, bool deposit)
{
	UINT64 deal_id = 0;
	//--- dealer operation
	MTAPIRES res = m_manager->DealerBalance(login, deposit ? amount : -amount, type, comment, deal_id);
	if (res != MT_RET_REQUEST_DONE)
	{
		m_manager->LoggerOut(MTLogErr, L"DealerBalance failed (%u)", res);
		return(false);
	}
	//---
	return(true);
}
