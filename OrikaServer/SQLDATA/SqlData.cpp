#include "stdafx.h"
#include "SqlData.h"
#include "StaticClass.h"
#include "document.h"
using namespace rapidjson;
#include "..\DataTable\ClientMasterTable.h"
#include "..\DataTable\SymbolMasterTable.h"
#include "..\DataTable\MetaDataTable.h"
#include "..\DataTable\netpositionTable.h"
#include "..\DataTable\Orika_clientmaster.h"
#include "..\DataTable\Orika_clientbrokerage.h"
#include "..\DataTable\Orika_PLDevideRatio.h"
#include "..\DataTable\Orika_symbolgroup.h"
#include "..\DataTable\Orika_Clientgatewayconfig.h"
#include "..\DataTable\Orika_symbolmaster.h"
#include "..\DataTable\OrderTable.h"
#include "..\DataTable\TableTabAndColumn.h"
#include "..\DataTable\Orika_positionAverageAccounting.h"
#include "..\DataTable\Orika_LoginTable.h"
#include "..\DataTable\TableGroupData.h"
#include "..\DataTable\TableLoginPassword.h"
#include "..\Orika_BalanceTableAccounting.h"
#include "..\DataTable\TableUpdateColumnMetaData.h"

#include "document.h"
#include "stringbuffer.h"
#include "writer.h"
#include <iostream>
#include "StaticClass.h"
using namespace rapidjson;

CSqlData::columnArray CSqlData::m_columnArray;

CSqlData::CSqlData()
{

}
CSqlData::~CSqlData()
{

}


CString CSqlData::getOrderData(CString loginuser)   
{	
	CString returnval=L"";
	CString tmpstr=L"";
	int row_count=0;
	
	CStaticClass::m_mutex_order.Lock();
	//(L"Mutex_order Locked _3");

	CString StrPrintLino=L"";
	StrPrintLino.Format(L"m_mutex_order Locked(%d)",__LINE__);
	////(StrPrintLino);


	CStaticClass::st_ClientContext m_st = {};
	CStaticClass::m_ClientContext.Lookup(loginuser, m_st);

	/*for (size_t i = 0; i < m_st.m_logins.size(); ++i) 
	{
		CString m_mappedLogin = m_st.m_logins[i];
		CStaticClass::m_logfile.LogEvent(m_mappedLogin);		
	}*/


	POSITION pos = CStaticClass::m_Orika_orderHastable.GetStartPosition ();
	StringBuffer s;
	Writer<StringBuffer> writer(s);
	//returnval.Format(L"{\"type\":\"ORDER_DATA\",\"insert\":[%s]}",tmpstr);	
	writer.StartObject();
	writer.Key("type");
	writer.String("ORDER_DATA");
	writer.Key("insert");
	writer.StartArray();
	while (pos != NULL)
	{
		int orderkey = 0;
		CStaticClass::st_order st = {};
		CStaticClass::m_Orika_orderHastable.GetNextAssoc(pos, orderkey, st);

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
		if (m_orderstate == 1001 || m_orderstate == 1002)
		{
			
			if (std::find(m_st.m_logins.begin(), m_st.m_logins.end(), m_login) != m_st.m_logins.end())
			{
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
				//strtmpdata.Format(L"{\"login\":\"%s\",\"time\":\"%s\",\"deal\":%d,\"order\":%d,\"symbol\":\"%s\",\"type\":\"%s\",\"volume\":%.2lf,\"price\":%.4lf,
				//\"comment\":\"%s\",\"status\":\"%s\",\"select\":\"%s\",\
					// "statustype\":\"%s\",\"subtype\":\"%s\",
					// //\"contraorder\":%d,\"tradeexecutetime\":%d,\"ourcomment\":\"%s\",\"orderstate\":\"%s\"}",
					//,m_contraorder,m_tradeexecutetime,m_ourcomment,str_orderstate);				
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
			}
		}
	}
	CStaticClass::m_mutex_order.Unlock();
	//(L"UNm_mutex_order Locked _3");	
	//returnval.Format(L"{\"type\":\"ORDER_DATA\",\"insert\":[%s]}",tmpstr);	
	writer.EndArray();
	writer.EndObject();
	returnval = s.GetString();
	return returnval;
}

void CSqlData::loadClientBrokerageFromDealTableAccounting()
{
	HRESULT hr=NULL ;
	CCommand<CAccessor<CBrokerageClientAndScripWise>> data_table;								
	if(!SUCCEEDED(hr))
	{
		return ;
	}
	CString   strCommand=L"";	
	strCommand.Format(L"select [login],symbol,sum(clientBrokTotal) as 'clientBrokTotal',sum(subBrokerBrokTotal) as 'subBrokerBrokTotal',sum(brokerBrokTotal) as 'brokerBrokTotal',sum(comBalancebrokTotal) as 'CompanyBrokTotal' from orika_dealtableaccounting  group by [login],symbol");
	//CStaticClass::m_mutex_order.Lock();
	//(L"Orderlock_22");
	CString StrPrintLino=L"";
	StrPrintLino.Format(L"m_mutex_order Locked(%d)",__LINE__);
	////(StrPrintLino);
	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr=data_table.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		m_tempSession.Close();		
		return ;
	}
	int i=0;
		
	CStaticClass::m_brokerageLoginSymbolWiseHastable.RemoveAll();
	while(hr=data_table.MoveNext()==S_OK)
	{		
		CStaticClass::st_orika_brokerageLoginSymbolWise  st_pac={};
		//CMTStr::Copy
		CMTStr::Copy(st_pac.m_login,data_table.m_login);
		CMTStr::Copy(st_pac.m_symbol,data_table.m_symbol);
		st_pac.m_clientBrokTotal =data_table.m_clientBrokTotal ;
		st_pac.m_subBrokerBrokTotal =data_table.m_subBrokerBrokTotal ; 
		st_pac.m_brokerBrokTotal =data_table.m_brokerBrokTotal ;
		st_pac.m_comBrokTotal = data_table.m_comBrokTotal;
		
		CString str_login=data_table.m_login;
		CString str_symbol=data_table.m_symbol;
		CString strloginSymbolKey=L"";
		strloginSymbolKey.Format(L"%s:%s",str_login,str_symbol);
		CStaticClass::m_mutex_TotalBrokerage.Lock();
			CStaticClass::m_brokerageLoginSymbolWiseHastable.SetAt(strloginSymbolKey,st_pac);
		CStaticClass::m_mutex_TotalBrokerage.Unlock();
	}
	m_tempSession.Close();
	
	
}



void CSqlData::loadTradeLotAndTOT()
{
	HRESULT hr = NULL;
	CCommand<CAccessor<CTableTradeQtyTOT>> data_table;
	if (!SUCCEEDED(hr))
	{
		return;
	}
	CString   strCommand = L"";
	strCommand.Format(L"exec proc_loadTradeLotAndTOT;");
	
	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
	if (FAILED(hr))
	{
		m_tempSession.Close();		
		return;
	}
	CStaticClass::m_TotalLotAndTOT.RemoveAll();
	while (hr = data_table.MoveNext() == S_OK)
	{
		CStaticClass::st_TotalTradedLotAndTOT  st_pac = {};
		//CMTStr::Copy		
		CString str_login = data_table.m_login ;
		CString str_symbol = data_table.m_symbol;
		CString strloginSymbolKey = L"";
		strloginSymbolKey.Format(L"%s:%s", str_login, str_symbol);
		
		st_pac.m_TotalTradedLot= data_table.m_TotalTradedLot;
		st_pac.m_TotalTradedTO= data_table.m_TotalTradedTO;
		st_pac.m_TotalBuyLot= data_table.m_TotalBuyLot;
		st_pac.m_TotalSellLot= data_table.m_TotalSellLot;
		st_pac.m_TotalButTO= data_table.m_TotalButTO;
		st_pac.m_TotalSellTO= data_table.m_TotalSellTO;
		
		CStaticClass::m_TotalLotAndTOT.SetAt(strloginSymbolKey, st_pac);
	}
	m_tempSession.Close();
	
	//(L"U_Orderlock_23");
}



void CSqlData::loadOrderCount()
{
	HRESULT hr = NULL;
	CCommand<CAccessor<CTableOrderCount>> data_table;
	if (!SUCCEEDED(hr))
	{
		return;
	}
	CString   strCommand = L"";
	strCommand.Format(L"exec proc_OrderCount;");
	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
	if (FAILED(hr))
	{
		m_tempSession.Close();
		

		return;
	}

	
	while (hr = data_table.MoveNext() == S_OK)
	{
		CStaticClass::st_OrderCount  st_pac = {};
		//CMTStr::Copy		
		CString str_login = data_table.m_login;
		CString str_symbol = data_table.m_symbol;
		int m_type = data_table.m_Type;
		CString strloginSymbolKey = L"";
		strloginSymbolKey.Format(L"%s:%s:%d", str_login, str_symbol, m_type);

		CMTStr::Copy(st_pac.m_login, str_login);
		CMTStr::Copy(st_pac.m_symbol, str_symbol);
		st_pac.m_Type= data_table.m_Type;
		st_pac.m_TotalOrder = data_table.m_TotalOrder;
		st_pac.m_TotalCancelledOrder = data_table.m_TotalCancelledOrder;
		st_pac.m_TotalExecutedOrder = data_table.m_TotalExecutedOrder;

		CStaticClass::m_OrderCountMap.SetAt(strloginSymbolKey, st_pac);
	}
	/*CStaticClass::st_OrderCount  st_pac_Test = {};
	CStaticClass::m_OrderCountMap.Lookup(L"200231:GOLDOCT:5", st_pac_Test);*/
	m_tempSession.Close();
	
	//(L"U_Orderlock_24");
}


void CSqlData::loadColumnSubscription()
{
	HRESULT hr = NULL;
	CCommand<CAccessor<CTableColumnsubscription>> data_table;
	if (!SUCCEEDED(hr))
	{
		return;
	}
	CString   strCommand = L"";
	strCommand.Format(L"select userlogin,ColumnKey,subscribe,RequestKey from loginColumnMapping;");
	
	//(L"Orderlock_25");
	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
	if (FAILED(hr))
	{
		m_tempSession.Close();		
		return;
	}
	while (hr = data_table.MoveNext() == S_OK)
	{		
		CString m_userlogin = data_table.m_userlogin;
		CString m_ColumnKey = data_table.m_ColumnKey;
		CString m_RequestKey = data_table.m_RequestKey;
		int m_subscribe = data_table.m_subscribe;
		CString m_userloginColumnKey = L"";
		m_userloginColumnKey.Format(L"%s:%s|%s", m_userlogin, m_RequestKey, m_ColumnKey);
		CStaticClass::m_columnsubscription.SetAt(m_userloginColumnKey, m_subscribe);
	}	
	m_tempSession.Close();
	
	//(L"Orderlock_25");
}


void CSqlData::checkColumnSubscription(TMTArray<wchar_t[200]> &m_mappedcolumns, CMap<CString, LPCTSTR, int, int> &m_subscribedcolumn)
{	
	CStaticClass::m_mutexcolumnSubs.Lock();
	int totalrequest = m_mappedcolumns.Total();	
	for (int cr = 0; cr < totalrequest; cr++)
	{		
		CString m_columndata_defined = L""; 
		m_columndata_defined = m_mappedcolumns[cr];
		m_subscribedcolumn.SetAt(m_columndata_defined, 1);
	}
	CStaticClass::m_mutexcolumnSubs.Unlock();
}








void CSqlData::loadPositionAverageAccounting()
{
	HRESULT hr=NULL ;
	CCommand<CAccessor<CPositionAverageAccounting>> data_table;								
	if(!SUCCEEDED(hr))
	{
		return ;
	}
	CString   strCommand=L"";	
	strCommand.Format(L"select [login],symbol,[Action],Volume,subBrokerVolume,brokerVolume,extraGroupVolume,companyVolume,WAvgPrice,currentPrice,multiplyer,floatingProfit,subBrokerfloatingProfit,brokerfloatingProfit,extraGroupfloatingProfit,companyfloatingProfit from orika_PositionAverageAccounting");		

	
	//(L"Orderlock_26");

	CString StrPrintLino=L"";
	StrPrintLino.Format(L"m_mutex_order Locked(%d)",__LINE__);
	////(StrPrintLino);


	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr=data_table.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		m_tempSession.Close();		
		
		return ;
	}
	int i=0;
	
	
	CStaticClass::m_orika_PositionAverageAccountingHastable.RemoveAll();
	while(hr=data_table.MoveNext()==S_OK)
	{		
		CStaticClass::st_orika_PositionAverageAccounting  st_pac={};
		
		CMTStr::Copy(st_pac.m_login,data_table.m_login);
		CMTStr::Copy(st_pac.m_symbol,data_table.m_symbol);
		st_pac.m_Action=data_table.m_Action;
		st_pac.m_Volume=data_table.m_Volume; 
		st_pac.m_subBrokerVolume=data_table.m_subBrokerVolume;
		st_pac.m_brokerVolume=data_table.m_brokerVolume;
		st_pac.m_extraGroupVolume=data_table.m_extraGroupVolume;
		st_pac.m_companyVolume=data_table.m_companyVolume;
		st_pac.m_WAvgPrice=data_table.m_WAvgPrice;	
		st_pac.m_currentPrice=data_table.m_currentPrice;
		st_pac.m_multiplyer=data_table.m_multiplyer;
		st_pac.m_floatingProfit=data_table.m_floatingProfit;
		st_pac.m_subBrokerfloatingProfit=data_table.m_subBrokerfloatingProfit;
		st_pac.m_brokerfloatingProfit=data_table.m_brokerfloatingProfit;
		st_pac.m_extraGroupfloatingProfit=data_table.m_extraGroupfloatingProfit;
		st_pac.m_companyfloatingProfit=data_table.m_companyfloatingProfit;
		CString str_login=data_table.m_login;
		CString str_symbol=data_table.m_symbol;
		CString strloginSymbolKey=L"";
		strloginSymbolKey.Format(L"%s:%s",str_login,str_symbol);
		CStaticClass::m_orika_PositionAverageAccountingHastable.SetAt(strloginSymbolKey,st_pac);
	}
	m_tempSession.Close();	
	//(L"U_Orderlock_26");
}



void CSqlData::loadBalanceTableAccounting()
{
	HRESULT hr=NULL ;
	CCommand<CAccessor<CBalanceTableAccounting>> data_table;								
	if(!SUCCEEDED(hr))
	{
		return ;
	}
	CString   strCommand=L"";	
	strCommand.Format(L"select [login],symbol,entryDeal,entryTime,entryOrder,entryAction,entryVolume,entryPrice,exitDeal,exitTime,exitOrder,exitAction,exitVolume,exitPrice,plPointLoss,plPointProfit,multiplyer,clientBalance,subBrokerBalance,brokerBalance,extraGroupBalance,companyBalance from Orika_BalanceTableAccounting");		
	
	//(L"Orderlock_27");

	CString StrPrintLino=L"";
	StrPrintLino.Format(L"m_mutex_order Locked(%d)",__LINE__);
	////(StrPrintLino);



	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr=data_table.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		m_tempSession.Close();		
		return ;
	}
	int i=0;
	
	//m_Orika_BalanceTableAccountingArray

	//m_Orika_BalanceTableAccountingArray

	CStaticClass::m_Orika_BalanceTableAccountingArray.RemoveAll();
	while(hr=data_table.MoveNext()==S_OK)
	{		
		CStaticClass::st_Orika_BalanceTableAccounting  st_pac={};
		//CMTStr::Copy

		CMTStr::Copy(st_pac.m_login,data_table.m_login);
		CMTStr::Copy(st_pac.m_symbol,data_table.m_symbol);
		st_pac.m_entryDeal=data_table.m_entryDeal;
		st_pac.m_entryTime=data_table.m_entryTime;
		st_pac.m_entryOrder=data_table.m_entryOrder;
		st_pac.m_entryAction=data_table.m_entryAction;
		st_pac.m_entryVolume=data_table.m_entryVolume;
		st_pac.m_entryPrice=data_table.m_entryPrice;
		st_pac.m_exitDeal=data_table.m_exitDeal;
		st_pac.m_exitTime=data_table.m_exitTime;
		st_pac.m_exitOrder=data_table.m_exitOrder;
		st_pac.m_exitAction=data_table.m_exitAction;
		st_pac.m_exitVolume=data_table.m_exitVolume;
		st_pac.m_exitPrice=data_table.m_exitPrice;
		st_pac.m_plPointLoss=data_table.m_plPointLoss;
		st_pac.m_plPointProfit=data_table.m_plPointProfit;
		st_pac.m_multiplyer=data_table.m_multiplyer;
		st_pac.m_clientBalance=data_table.m_clientBalance;
		st_pac.m_subBrokerBalance=data_table.m_subBrokerBalance;
		st_pac.m_brokerBalance=data_table.m_brokerBalance;
		st_pac.m_extraGroupBalance=data_table.m_extraGroupBalance;
		st_pac.m_companyBalance=data_table.m_companyBalance;






		CString str_login=data_table.m_login;
		CString str_symbol=data_table.m_symbol;
		CString strloginSymbolKey=L"";
		strloginSymbolKey.Format(L"%s:%s",str_login,str_symbol);


		//Adding From Previous Balance
		CStaticClass::st_Orika_BalanceTableAccounting  st_pac_temp={};
		CStaticClass::m_Orika_BalanceTableAccountingArray.Lookup(strloginSymbolKey,st_pac_temp);
		st_pac.m_clientBalance=data_table.m_clientBalance+st_pac_temp.m_clientBalance;
		st_pac.m_subBrokerBalance=data_table.m_subBrokerBalance+st_pac_temp.m_subBrokerBalance;
		st_pac.m_brokerBalance=data_table.m_brokerBalance+st_pac_temp.m_brokerBalance;
		st_pac.m_extraGroupBalance=data_table.m_extraGroupBalance+st_pac_temp.m_extraGroupBalance;
		st_pac.m_companyBalance=data_table.m_companyBalance+st_pac_temp.m_companyBalance;
		//End 

		CStaticClass::m_Orika_BalanceTableAccountingArray.SetAt(strloginSymbolKey,st_pac);
	}
	m_tempSession.Close();
	
	//(L"U_Orderlock_27");
}

void CSqlData::loadDealTableAccounting()
{	
	HRESULT hr = NULL;
	CCommand<CAccessor<CDealTable>> data_table;
	if (!SUCCEEDED(hr))
	{
		return;
	}
	CString   strCommand = L"";
	strCommand.Format(L"select [login],deal,[order],[time],symbol,[action],volume,price,comment from Orika_dealtableAccounting;");

	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
	if (FAILED(hr))
	{
		m_tempSession.Close();
		return;
	}
	int i = 0;
	
	while (hr = data_table.MoveNext() == S_OK)
	{
		CString m_login = data_table.m_login;
		int m_deal= data_table.m_deal;
		int m_order= data_table.m_order ;
		int m_time= data_table.m_time;
		CString m_symbol= data_table.m_symbol;
		int m_action= data_table.m_action;
		double m_volume= data_table.m_volume;
		double m_price= data_table.m_price;
		CString m_comment= data_table.m_comment;


		CStaticClass::st_Orika_dealtableAccounting m_st_Orika_dealtableAccounting = {};
		CMTStr::Copy(m_st_Orika_dealtableAccounting.m_login, m_login);
		m_st_Orika_dealtableAccounting.m_deal = m_deal;
		m_st_Orika_dealtableAccounting.m_order = m_order;
		m_st_Orika_dealtableAccounting.m_time = m_time;
		CMTStr::Copy(m_st_Orika_dealtableAccounting.m_symbol, m_symbol);		
		m_st_Orika_dealtableAccounting.m_action = m_action;
		m_st_Orika_dealtableAccounting.m_volume = m_volume;
		m_st_Orika_dealtableAccounting.m_price = m_price;
		CMTStr::Copy(m_st_Orika_dealtableAccounting.m_comment, m_comment);

		CString strLoginDealKey = L"";
		strLoginDealKey.Format(L"%s:%I64u", m_login, m_deal);
		CStaticClass::m_mutex_deal.Lock();
		CStaticClass::m_Orika_dealtableAccountingHastable.SetAt(strLoginDealKey, m_st_Orika_dealtableAccounting);
		CStaticClass::m_mutex_deal.Unlock();
	}
	m_tempSession.Close();

}

void CSqlData::loadDealNo()
{
	HRESULT hr = NULL;
	CCommand<CAccessor<CDealNOTable>> data_table;
	if (!SUCCEEDED(hr))
	{
		return;
	}
	CString   strCommand = L"";
	strCommand.Format(L"select deal,[order] from Orika_dealtableaccounting;");
	
	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
	if (FAILED(hr))
	{
		m_tempSession.Close();		
		return;
	}
	int i = 0;
	CStaticClass::m_Orika_dealNO.RemoveAll();	
	while (hr = data_table.MoveNext() == S_OK)
	{				
		int m_deal = data_table.m_deal;		
		int m_orderno = data_table.m_order;
		CStaticClass::m_Orika_dealNO.SetAt(m_deal, m_deal);
		CStaticClass::m_OrikaOrderdealNO.SetAt(m_orderno, m_deal);
	}
	m_tempSession.Close();	
	//(L"U_Orderlock_28");
}


void CSqlData::loadOrderHashTable()
{
	HRESULT hr=NULL ;
	CCommand<CAccessor<COrderTable>> data_table;								
	if(!SUCCEEDED(hr))
	{
		return ;
	}
	CString   strCommand=L"";	
	strCommand.Format(L"select [login],[time],deal,[order],symbol,[type],volume,price,comment,[status],[select],selecttype,subtype,contraorder,tradeexecutetime,ourcomment,orderstate from Orika_order where orderstate in (1001,1002);");					

	CSession m_tempSession;

	m_tempSession.Open(CStaticClass::connection);
	hr=data_table.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		m_tempSession.Close();		
		return ;
	}
	int i=0;
	CStaticClass::m_Orika_orderHastable.RemoveAll();
	while(hr=data_table.MoveNext()==S_OK)
	{		
		CStaticClass::st_order m_st_st_order={};		
		CString strlogin=data_table.m_login;
		int m_order=data_table.m_order;
		CString strsymbol=data_table.m_symbol;
		CString strcomment=data_table.m_comment;
		CString strstatus=data_table.m_status;
		CString strselecttype=data_table.m_selecttype;
		CString strsubtype=data_table.m_subtype;
		CString strourcomment=data_table.m_ourcomment;

		
		CMTStr::Copy(m_st_st_order.m_login,strlogin);
		m_st_st_order.m_time=data_table.m_time ;
		m_st_st_order.m_deal=data_table.m_deal ;
		m_st_st_order.m_order=data_table.m_order;
		CMTStr::Copy(m_st_st_order.m_symbol,strsymbol);
		m_st_st_order.m_type=data_table.m_type ;
		m_st_st_order.m_volume=data_table.m_volume ;
		m_st_st_order.m_price=data_table.m_price ;
		CMTStr::Copy(m_st_st_order.m_comment,strcomment);
		CMTStr::Copy(m_st_st_order.m_status,strstatus);
		m_st_st_order.m_select=data_table.m_select ;
		CMTStr::Copy(m_st_st_order.m_selecttype,strselecttype);
		CMTStr::Copy(m_st_st_order.m_subtype,strsubtype);
		m_st_st_order.m_contraorder=data_table.m_contraorder ;
		m_st_st_order.m_tradeexecutetime=data_table.m_tradeexecutetime ;
		CMTStr::Copy(m_st_st_order.m_ourcomment,strourcomment);
		m_st_st_order.m_orderstate=data_table.m_orderstate ;

		CStaticClass::m_Orika_orderHastable.SetAt(m_order,m_st_st_order);
	}
	m_tempSession.Close();
	//(L"U_Orderlock_29");
}







//CString CSqlData::getClientposition(CString loginuser)
//{	
//	CString returnval=L"";		
//	CString tmpstr=L"";
//	int row_count=0;
//	
//	/*CStaticClass::m_mutex_order.Lock()
//
//	if (std::find(m_st.m_logins.begin(), m_st.m_logins.end(), m_login) != m_st.m_logins.end())
//	{											
//		strtmpdata.Format(L"{\"login\":\"%s\",\"name\":\"%s\",\"symbol\":\"%s\",\"volume\":%.2lf,\"previousVolume\":%.2lf,\"difference\":%.2lf,\"average\":%.4lf,\"lastRate\":%.4lf,\"clientnetAmount\":\"%.0lf\",\"clientnettotal\":\"%.0lf\",\"maxallotedlots\":\"%.0lf\",\"subbroker\":\"%s\",\"broker\":\"%s\",\"clientExposure\":\"%.0lf\",\"extravolume\":\"0\",\"freemargin\":\"0\"}",m_login,m_name,m_symbol,m_volume,m_previousVolume,m_difference,m_average,m_lastRate,m_clientnetAmount,m_clientnettotal,m_maxallotedlots,m_subbroker,m_broker,m_clientExposure,m_extravolume,m_freemargin);									
//		if (row_count!=0)
//		{
//			tmpstr=tmpstr+L","+strtmpdata;
//		}
//		else
//		{
//			tmpstr=tmpstr+strtmpdata;
//		}
//		row_count=row_count+1;
//	}
//	CStaticClass::m_mutex_order.Unlock();
//*/
//
//	returnval.Format(L"{\"type\": \"CLIENT_POSITIONS\",\"data\": [%s]}",tmpstr);	
//	return returnval;
//}





CString CSqlData::getBrokerposition(CString loginuser)
{	
	CString returnval=L"";
	HRESULT hr=NULL ;
	CCommand<CAccessor<CBrokerpositionTable>> data_table;								
	if(!SUCCEEDED(hr))
	{
		return L"";
	}
	CString   strCommand=L"";
	strCommand.Format(L"orika_GetBrokerPositionData'1605632400';");	

	//(L"Orderlock_30");


	

	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr=data_table.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		m_tempSession.Close();		
		return L"";
	}
	int i=0;
	

	CString m_broker=L"";
	CString m_name=L"";
	CString m_symbol=L"";
	double m_volume=0;
	double m_previousVolume=0;
	double m_difference=0;
	double m_average=0;
	double m_lastRate=0;
	double m_clientnetamount=0;
	double m_clientnetTotal=0;
	double m_brokergrossAmount=0;
	double m_brokernetAmount=0;


	CString tmpstr=L"";
	int row_count=0;
	while(hr=data_table.MoveNext()==S_OK)
	{		
		m_broker=data_table.m_broker;
		m_name=data_table.m_name;
		m_symbol=data_table.m_symbol;
		m_volume=data_table.m_volume;
		m_previousVolume=data_table.m_prevVol;
		m_difference=data_table.m_volDiff;
		m_average=data_table.m_avg;
		m_lastRate=data_table.m_lastRate;
		m_clientnetamount=data_table.m_clientnetamount;
		m_clientnetTotal=data_table.m_clientnetTotal;
		m_brokergrossAmount=data_table.m_brokergrossAmount;
		m_brokernetAmount=m_brokernetAmount;


		
		CString strtmpdata=L"";

													
			strtmpdata.Format(L"{\"broker\":\"%s\",\"name\":\"%s\",\"symbol\":\"%s\",\"volume\":%.2lf,\"previousVolume\":%.2lf,\"difference\":%.2lf,\"average\":%.4lf,\"lastRate\":%.4lf,\"clientnetAmount\":\"%.0lf\",\"clientnettotal\":\"%.0lf\",\"brokergrossAmount\":\"%.0lf\",\"brokernetAmount\":\"%s\"}",m_broker,m_name,m_symbol,m_volume,m_previousVolume,m_difference,m_average,m_lastRate,m_clientnetamount,m_clientnetTotal,m_brokergrossAmount,m_brokernetAmount);									

			if (row_count!=0)
			{
				tmpstr=tmpstr+L","+strtmpdata;
			}
			else
			{
				tmpstr=tmpstr+strtmpdata;
			}
			row_count=row_count+1;
		
	}
	data_table.Close();
	m_tempSession.Close();
	
	//(L"U_Orderlock_30");

	returnval.Format(L"{\"type\": \"BROKER_POSITION_DATA\",\"data\": [%s]}",tmpstr);	
	return returnval;
}



CString CSqlData::getSubBrokerposition(CString loginuser)
{	
	CString returnval=L"";
	HRESULT hr=NULL ;
	CCommand<CAccessor<CBrokerpositionTable>> data_table;								
	if(!SUCCEEDED(hr))
	{
		return L"";
	}
	CString   strCommand=L"";
	strCommand.Format(L"orika_GetSubBrokerPositionData '1605632400';");	
	
	CSession m_tempSession;

	m_tempSession.Open(CStaticClass::connection);
	hr=data_table.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		m_tempSession.Close();		
		return L"";
	}
	int i=0;
	

	CString m_broker=L"";
	CString m_name=L"";
	CString m_symbol=L"";
	double m_volume=0;
	double m_previousVolume=0;
	double m_difference=0;
	double m_average=0;
	double m_lastRate=0;
	double m_clientnetamount=0;
	double m_clientnetTotal=0;
	double m_brokergrossAmount=0;
	double m_brokernetAmount=0;


	CString tmpstr=L"";
	int row_count=0;
	while(hr=data_table.MoveNext()==S_OK)
	{		
		m_broker=data_table.m_broker;
		m_name=data_table.m_name;
		m_symbol=data_table.m_symbol;
		m_volume=data_table.m_volume;
		m_previousVolume=data_table.m_prevVol;
		m_difference=data_table.m_volDiff;
		m_average=data_table.m_avg;
		m_lastRate=data_table.m_lastRate;
		m_clientnetamount=data_table.m_clientnetamount;
		m_clientnetTotal=data_table.m_clientnetTotal;
		m_brokergrossAmount=data_table.m_brokergrossAmount;
		m_brokernetAmount=m_brokernetAmount;


		
		CString strtmpdata=L"";

													
			strtmpdata.Format(L"{\"broker\":\"%s\",\"name\":\"%s\",\"symbol\":\"%s\",\"volume\":%.2lf,\"previousVolume\":%.2lf,\"difference\":%.2lf,\"average\":%.4lf,\"lastRate\":%.4lf,\"clientnetAmount\":\"%.0lf\",\"clientnettotal\":\"%.0lf\",\"brokergrossAmount\":\"%.0lf\",\"brokernetAmount\":\"%s\"}",m_broker,m_name,m_symbol,m_volume,m_previousVolume,m_difference,m_average,m_lastRate,m_clientnetamount,m_clientnetTotal,m_brokergrossAmount,m_brokernetAmount);									

			if (row_count!=0)
			{
				tmpstr=tmpstr+L","+strtmpdata;
			}
			else
			{
				tmpstr=tmpstr+strtmpdata;
			}
			row_count=row_count+1;
		
	}
	data_table.Close();
	m_tempSession.Close();	

	returnval.Format(L"{\"type\": \"BROKER_POSITION_DATA\",\"data\": [%s]}",tmpstr);	
	return returnval;
}




CString CSqlData::getBalanceDistribution(CString loginuser)
{	
	CString returnval=L"";
	HRESULT hr=NULL ;
	CCommand<CAccessor<CBalanceDistributionTable>> data_table;								
	if(!SUCCEEDED(hr))
	{
		return L"";
	}
	CString   strCommand=L"";
	strCommand.Format(L"exec orika_GetBalanceDistributionData;");	
	
	CSession m_tempSession;

	m_tempSession.Open(CStaticClass::connection);
	hr=data_table.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		m_tempSession.Close();		
		return L"";
	}
	int i=0;
	

	CString m_login=L"";
	CString m_subBroker=L"";
	CString m_broker=L"";
	CString m_symbol=L"";
	double m_subbrokerRatio=0;
	double m_brokerratio=0;
	double m_companyRatio=0;
	double m_clientNetAmount=0;
	double m_subbrokerNetAmount=0;
	double m_clientGrossAmount=0;
	double m_companyNetTotal=0;


	CString tmpstr=L"";
	int row_count=0;
	while(hr=data_table.MoveNext()==S_OK)
	{		
		m_login=data_table.m_login;
		m_subBroker=data_table.m_subBroker;
		m_broker=data_table.m_broker;
		m_symbol=data_table.m_symbol;
		m_subbrokerRatio=data_table.m_subbrokerRatio;
		m_brokerratio=data_table.m_brokerratio;
		m_companyRatio=data_table.m_companyRatio;
		m_clientNetAmount=data_table.m_clientNetAmount;
		m_subbrokerNetAmount=data_table.m_subbrokerNetAmount;
		m_clientGrossAmount=data_table.m_clientGrossAmount;
		m_companyNetTotal=data_table.m_companyNetTotal;


		
		CString strtmpdata=L"";

													
			strtmpdata.Format(L"{\"login\":\"%s\",\"subBrokerName\":\"%s\",\"brokerName\":\"%s\",\"symbol\":\"%s\",\"subbrokerRatio\":%.2lf,\"brokerRatio\":%.2lf,\"companyRatio\":%.4lf,\"clientNetAmount\":%.4lf,\"subbrokerNetAmount\":\"%.0lf\",\"clientGrossAmount\":\"%.0lf\",\"companyNetTotal\":\"%.0lf\"}",m_login,m_subBroker,m_broker,m_symbol,m_subbrokerRatio,m_brokerratio,m_companyRatio,m_clientNetAmount,m_subbrokerNetAmount,m_clientGrossAmount,m_companyNetTotal);									

			if (row_count!=0)
			{
				tmpstr=tmpstr+L","+strtmpdata;
			}
			else
			{
				tmpstr=tmpstr+strtmpdata;
			}
			row_count=row_count+1;
		
	}
	data_table.Close();
	m_tempSession.Close();			
	returnval.Format(L"{\"type\": \"BALANCE_DISTRIBUTION_DATA\",\"data\": [%s]}",tmpstr);	
	return returnval;
}




CString CSqlData::getExposureDistribution(CString loginuser)
{	
	CString returnval=L"";
	HRESULT hr=NULL ;
	CCommand<CAccessor<CGrossExposureDistributionTable>> data_table;								
	if(!SUCCEEDED(hr))
	{
		return L"";
	}
	CString   strCommand=L"";
	strCommand.Format(L"exec CGrossExposureDistributionTable;");	
	
	CSession m_tempSession;

	m_tempSession.Open(CStaticClass::connection);
	hr=data_table.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		m_tempSession.Close();		
		return L"";
	}
	int i=0;
	

	CString m_login=L"";
	CString m_subBroker=L"";
	CString m_broker=L"";
	CString m_symbol=L"";
	double m_subbrokerRatio=0;
	double m_brokerratio=0;
	double m_companyRatio=0;
	double m_volume=0;
	double m_clientExposure=0;
	double m_subbrokerExposure=0;
	double m_brokerExposure=0;
	double m_companyExposure=0;


	

	CString tmpstr=L"";
	int row_count=0;
	while(hr=data_table.MoveNext()==S_OK)
	{		
		m_login=data_table.m_login;
		m_subBroker=data_table.m_subBroker;
		m_broker=data_table.m_broker;
		m_symbol=data_table.m_symbol;
		m_subbrokerRatio=data_table.m_subbrokerRatio;
		m_brokerratio=data_table.m_brokerratio;
		m_companyRatio=data_table.m_companyRatio;
		m_volume=data_table.m_volume;
		m_clientExposure=data_table.m_clientExposure;
		m_subbrokerExposure=data_table.m_subbrokerExposure;
		m_brokerExposure=data_table.m_brokerExposure;
		m_companyExposure=data_table.m_companyExposure;


		
		CString strtmpdata=L"";

													
		strtmpdata.Format(L"{\"login\":\"%s\",\"subBrokerName\":\"%s\",\"brokerName\":\"%s\",\"symbol\":\"%s\",\"subbrokerRatio\":%.2lf,\"brokerRatio\":%.2lf,\"companyRatio\":%.4lf,\"volume\":%.4lf,\"clientExposure\":\"%.0lf\",\"subBrokerExposure\":\"%.0lf\",\"brokerExposure\":\"%.0lf\",\"companyExposure\":\"%.0lf\"}",m_login,m_subBroker,m_broker,m_symbol,m_subbrokerRatio,m_brokerratio,m_companyRatio,m_volume,m_clientExposure,m_subbrokerExposure,m_brokerExposure,m_companyExposure);									

		if (row_count!=0)
		{
			tmpstr=tmpstr+L","+strtmpdata;
		}
		else
		{
			tmpstr=tmpstr+strtmpdata;
		}
		row_count=row_count+1;
		
	}
	data_table.Close();
	m_tempSession.Close();	
	//(L"U_Orderlock_33");
	returnval.Format(L"{\"type\": \"EXPOSURE_DISTRIBUTION_DATA\",\"data\": [%s]}",tmpstr);	
	return returnval;
}


CString CSqlData::getBrokerageDistribution(CString loginuser)
{	
	CString returnval=L"";
	HRESULT hr=NULL ;
	CCommand<CAccessor<CBrokerageDistributionTable>> data_table;								
	if(!SUCCEEDED(hr))
	{
		return L"";
	}
	CString   strCommand=L"";
	strCommand.Format(L"exec BrokerageDistributionTable;");	
	
	
	CSession m_tempSession;

	m_tempSession.Open(CStaticClass::connection);
	hr=data_table.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		m_tempSession.Close();		
		return L"";
	}
	int i=0;
	

	CString m_login=L"";
	CString m_subBroker=L"";
	CString m_broker=L"";
	CString m_symbol=L"";
	CString m_brokarageType=L"";
	double m_clientBrokarageRate=0;
	double m_subbrokerBrokarageRate=0;
	double m_brokerBrokarageRate=0;
	double m_companyBrokarageRate=0;
	double m_clientBrokarage=0;
	double m_subbrokerBrokarage=0;
	double m_brokerBrokarage=0;
	double m_companyBrokarage=0;


	

	CString tmpstr=L"";
	int row_count=0;
	while(hr=data_table.MoveNext()==S_OK)
	{		
		m_login=data_table.m_login;
		m_subBroker=data_table.m_subBroker;
		m_broker=data_table.m_broker;
		m_symbol=data_table.m_symbol;
		m_brokarageType=data_table.m_brokarageType;
		m_clientBrokarageRate=data_table.m_clientBrokarageRate;
		m_subbrokerBrokarageRate=data_table.m_subbrokerBrokarageRate;
		m_brokerBrokarageRate=data_table.m_brokerBrokarageRate;
		m_companyBrokarageRate=data_table.m_companyBrokarageRate;
		m_clientBrokarage=data_table.m_clientBrokarage;
		m_subbrokerBrokarage=data_table.m_subbrokerBrokarage;
		m_brokerBrokarage=data_table.m_brokerBrokarage;
		m_companyBrokarage=data_table.m_companyBrokarage;


		
		CString strtmpdata=L"";

													
		strtmpdata.Format(L"{\"login\":\"%s\",\"subBrokerName\":\"%s\",\"brokerName\":\"%s\",\"symbol\":\"%s\",\"clientBrokageType\":\"%s\",\"clientBrokRate\":%.2lf,\"subBrokerBrokRate\":%.2lf,\"brokerBrokRate\":%.4lf,\"companyBrokRate\":%.4lf,\"clientBrokTotal\":\"%.0lf\",\"subbrokerBrokarage\":\"%.0lf\",\"brokerBrokarage\":\"%.0lf\",\"companyBrokarage\":\"%.0lf\"}",m_login,m_subBroker,m_broker,m_symbol,m_brokarageType,m_clientBrokarageRate,m_subbrokerBrokarageRate,m_brokerBrokarageRate,m_companyBrokarageRate,m_clientBrokarage,m_subbrokerBrokarage,m_brokerBrokarage,m_companyBrokarage);									

		if (row_count!=0)
		{
			tmpstr=tmpstr+L","+strtmpdata;
		}
		else
		{
			tmpstr=tmpstr+strtmpdata;
		}
		row_count=row_count+1;
		
	}
	data_table.Close();
	m_tempSession.Close();	
	//(L"U_Orderlock_34");
	returnval.Format(L"{\"type\": \"BROKARAGE_DISTRIBUTION_DATA\",\"data\": [%s]}",tmpstr);	
	return returnval;
}



CString CSqlData::getVolumeDistribution(CString loginuser)
{	
	CString returnval=L"";
	HRESULT hr=NULL ;
	CCommand<CAccessor<CBalanceDistributionTable>> data_table;								
	if(!SUCCEEDED(hr))
	{
		return L"";
	}
	CString   strCommand=L"";
	strCommand.Format(L"exec orika_GetVolumeDistributionData;");	
	
	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr=data_table.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		m_tempSession.Close();		
		return L"";
	}
	int i=0;
	

	CString m_login=L"";
	CString m_subBroker=L"";
	CString m_broker=L"";
	CString m_symbol=L"";
	double m_subbrokerRatio=0;
	double m_brokerratio=0;
	double m_companyRatio=0;
	double m_volume=0;
	double m_subbrokerVolume=0;
	double m_brokerVolume=0;
	double m_companyVolume=0;


	

	CString tmpstr=L"";
	int row_count=0;
	while(hr=data_table.MoveNext()==S_OK)
	{		
		m_login=data_table.m_login;
		m_subBroker=data_table.m_subBroker;
		m_broker=data_table.m_broker;
		m_symbol=data_table.m_symbol;
		m_subbrokerRatio=data_table.m_subbrokerRatio;
		m_brokerratio=data_table.m_brokerratio;
		m_companyRatio=data_table.m_companyRatio;
		m_volume=data_table.m_clientNetAmount;
		m_subbrokerVolume=data_table.m_subbrokerNetAmount;
		m_brokerVolume=data_table.m_clientGrossAmount;
		m_companyVolume=data_table.m_companyNetTotal;

		
		CString strtmpdata=L"";													
		strtmpdata.Format(L"{\"login\":\"%s\",\"subBrokerName\":\"%s\",\"brokerName\":\"%s\",\"symbol\":\"%s\",\"subbrokerRatio\":%.2lf,\"brokerRatio\":%.2lf,\"companyRatio\":%.4lf,\"volume\":%.4lf,\"subBrokerVolume\":\"%.0lf\",\"brokerVolume\":\"%.0lf\",\"companyVolume\":\"%.0lf\"}",m_login,m_subBroker,m_broker,m_symbol,m_subbrokerRatio,m_brokerratio,m_companyRatio,m_volume,m_subbrokerVolume,m_brokerVolume,m_companyVolume);									
		if (row_count!=0)
		{
			tmpstr=tmpstr+L","+strtmpdata;
		}
		else
		{
			tmpstr=tmpstr+strtmpdata;
		}
		row_count=row_count+1;
		
	}
	data_table.Close();
	m_tempSession.Close();	
	//(L"U_Orderlock_35");

	returnval.Format(L"{\"type\": \"VOLUME_DISTRIBUTION_DATA\",\"data\": [%s]}",tmpstr);	
	return returnval;
}

CString CSqlData::getLotDistribution(CString loginuser)
{	
	CString returnval=L"";
	HRESULT hr=NULL ;
	CCommand<CAccessor<CBalanceDistributionTable>> data_table;								
	if(!SUCCEEDED(hr))
	{
		return L"";
	}
	CString   strCommand=L"";
	strCommand.Format(L"exec orika_GetLotDistributionData;");	
	
	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr=data_table.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		m_tempSession.Close();		
		return L"";
	}
	int i=0;
	

	CString m_login=L"";
	CString m_subBroker=L"";
	CString m_broker=L"";
	CString m_symbol=L"";
	double m_subbrokerRatio=0;
	double m_brokerratio=0;
	double m_companyRatio=0;
	double m_volume=0;
	double m_subbrokerVolume=0;
	double m_brokerVolume=0;
	double m_companyVolume=0;


	

	CString tmpstr=L"";
	int row_count=0;
	while(hr=data_table.MoveNext()==S_OK)
	{		
		m_login=data_table.m_login;
		m_subBroker=data_table.m_subBroker;
		m_broker=data_table.m_broker;
		m_symbol=data_table.m_symbol;
		m_subbrokerRatio=data_table.m_subbrokerRatio;
		m_brokerratio=data_table.m_brokerratio;
		m_companyRatio=data_table.m_companyRatio;
		m_volume=data_table.m_clientNetAmount;
		m_subbrokerVolume=data_table.m_subbrokerNetAmount;
		m_brokerVolume=data_table.m_clientGrossAmount;
		m_companyVolume=data_table.m_companyNetTotal;


		
		CString strtmpdata=L"";

													
			strtmpdata.Format(L"{\"login\":\"%s\",\"subBrokerName\":\"%s\",\"brokerName\":\"%s\",\"symbol\":\"%s\",\"subbrokerRatio\":%.2lf,\"brokerRatio\":%.2lf,\"companyRatio\":%.4lf,\"clientLots\":%.4lf,\"subBrokerLots\":\"%.0lf\",\"brokerLots\":\"%.0lf\",\"companyLots\":\"%.0lf\"}",m_login,m_subBroker,m_broker,m_symbol,m_subbrokerRatio,m_brokerratio,m_companyRatio,m_volume,m_subbrokerVolume,m_brokerVolume,m_companyVolume);									

			if (row_count!=0)
			{
				tmpstr=tmpstr+L","+strtmpdata;
			}
			else
			{
				tmpstr=tmpstr+strtmpdata;
			}
			row_count=row_count+1;
		
	}
	data_table.Close();
	m_tempSession.Close();	
	//(L"U_Orderlock_36");

	returnval.Format(L"{\"type\": \"LOTS_DISTRIBUTION_DATA\",\"data\": [%s]}",tmpstr);	
	return returnval;
}


CString CSqlData::columnTemplateSave(CString name, CString userLogin, CString colorData)
{
	//CTelplateIDTable
	CString returnval = L"";
	CString dealfound = L"";
	HRESULT hr = NULL;
	CCommand<CAccessor<CTelplateIDTable>> data_table;
	if (!SUCCEEDED(hr))
	{
		return L"";
	}
	CString   strCommand = L"";
	strCommand.Format(L"exec insert_orika_columnTemplate '%s','%s','%s';", name, userLogin, colorData);
	
	CString StrPrintLino = L"";	
	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
	if (FAILED(hr))
	{
		m_tempSession.Close();		
		return L"";
	}
	int m_returnID = 0;
	while (hr = data_table.MoveNext() == S_OK)
	{
		m_returnID = data_table.m_templateID;
	}


	data_table.Close();
	m_tempSession.Close();	
	//(L"U_Orderlock_37");
	CString tmpstr = L"";
	CString m_status = L"SUCCESS";
	if (m_returnID == 0)
	{
		m_status = L"FAIL";
	}
	returnval.Format(L"{\"type\":\"COLUMN_TEMPLATE_CREATE_STATUS\",\"id\":%d,\"status\":\"%s\"}", m_returnID, m_status);
	return returnval;
}
CString CSqlData::columnTemplateUpdate(int id,CString name, CString userLogin, CString colorData)
{
	CString returnval = L"";
	CString dealfound = L"";
	HRESULT hr = NULL;
	CCommand<CNoAccessor, CNoRowset> UpdateCommand;
	if (!SUCCEEDED(hr))
	{
		return L"";
	}
	CString   strCommand = L"";
	strCommand.Format(L"exec Update_orika_columnTemplate '%d','%s','%s','%s';", id, name, userLogin, colorData);	
	//(L"Orderlock_38");

	CString StrPrintLino = L"";
	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr = UpdateCommand.Open(m_tempSession, (LPCTSTR)strCommand);
	UpdateCommand.Close();
	m_tempSession.Close();	
	//(L"U_Orderlock_38");
	CString tmpstr = L"";
	CString m_status = L"SUCCESS";
	returnval.Format(L"{\"type\":\"COLUMN_TEMPLATE_UPDATE_STATUS\",\"id\":%d,\"status\":\"%s\"}", id, m_status);
	return returnval;
}

CString CSqlData::columnTemplateDelete(int id)
{
	CString returnval = L"";
	CString dealfound = L"";
	HRESULT hr = NULL;
	CCommand<CNoAccessor, CNoRowset> UpdateCommand;
	if (!SUCCEEDED(hr))
	{
		return L"";
	}
	CString   strCommand = L"";
	strCommand.Format(L"exec Delete_orika_columnTemplate '%d';", id);
	
	CSession m_tempSession;
	CString StrPrintLino = L"";
	m_tempSession.Open(CStaticClass::connection);
	hr = UpdateCommand.Open(m_tempSession, (LPCTSTR)strCommand);
	UpdateCommand.Close();
	m_tempSession.Close();	
	//(L"U_Orderlock_39");
	CString tmpstr = L"";
	CString m_status = L"SUCCESS";
	returnval.Format(L"{\"type\":\"COLUMN_TEMPLATE_DELETE_STATUS\",\"id\":%d,\"status\":\"%s\"}", id, m_status);
	return returnval;
}

CString CSqlData::getColumnTempletUserLoginWise(CString userLogin)
{
	CString returnval = L"";
	CString rval = L"";
	HRESULT hr = NULL;
	CCommand<CAccessor<CTableColumnTemplate>> data_table;
	if (!SUCCEEDED(hr))
	{
		return L"";
	}
	
	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	CString m_commandtext = L"";
	m_commandtext.Format(L"Fetch_orika_columnTemplate '%s'", userLogin);
	hr = data_table.Open(m_tempSession, (LPCTSTR)m_commandtext);
	if (FAILED(hr))
	{
		m_tempSession.Close();		
		return L"";
	}
	
	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("type");
	writer.String("COLUMN_TEMPLATE");
	writer.Key("templets");
	writer.StartArray();
	int data_count = 0;
	int m_OldtempletID = 0;
	while (hr = data_table.MoveNext() == S_OK)
	{								
			CString  m_template_name=L"";
			CString  m_userlogin = L"";
			int m_templetID;
			int m_sequence;
			CString  m_color = L"";
			CString  m_value = L"";
			CString  m_fontcolor = L"";
			CString  m_colorDark = L"";
			CString  m_fontColorDark = L"";


			m_template_name = data_table.m_template_name;
			m_userlogin = data_table.m_userlogin;
			m_templetID = data_table.m_templetID;
			m_sequence = data_table.m_sequence;
			m_color =  data_table.m_color;
			m_value = data_table.m_value;
			m_fontcolor = data_table.m_fontColor;
			m_colorDark = data_table.m_colorDark;
			m_fontColorDark = data_table.m_fontColorDark;

			string  strtemplate_name = CT2A(m_template_name);
			const char* template_name = strtemplate_name.c_str();
			string  struserlogin = CT2A(m_userlogin);
			const char* userlogin = struserlogin.c_str();
			string strcolor = CT2A(m_color);
			const char* color = strcolor.c_str();
			string strvalue = CT2A(m_value);
			const char* value = strvalue.c_str();

			string strfontcolor = CT2A(m_fontcolor);
			const char* fontcolor = strfontcolor.c_str();


			string strcolorDark = CT2A(m_colorDark);
			const char* colorDark = strcolorDark.c_str();

			string strfontColorDark = CT2A(m_fontColorDark);
			const char* fontColorDark = strfontColorDark.c_str();


			if (m_OldtempletID != m_templetID)
			{
				if (data_count != 0)
				{
					writer.EndArray();
					writer.EndObject();
				}

				writer.StartObject();
				writer.Key("name");
				writer.String(template_name);
				writer.Key("id");
				writer.Int(m_templetID);
				writer.Key("userlogin");
				writer.String(userlogin);
				writer.Key("colors");

				writer.StartArray();				
			}
			writer.StartObject();
			writer.Key("sequence");
			writer.Int(m_sequence);
			writer.Key("color");
			writer.String(color);
			writer.Key("startPoint");
			writer.String(value);
			writer.Key("fontColor");
			writer.String(fontcolor);


			writer.Key("colorDark");
			writer.String(colorDark);
			writer.Key("fontColorDark");
			writer.String(fontColorDark);

			writer.EndObject();

			
			m_OldtempletID = m_templetID;
			data_count = data_count + 1;
	}
	if (data_count != 0)
	{
		writer.EndArray();
		writer.EndObject();
	}

	data_table.Close();
	m_tempSession.Close();	
	//(L"U_Orderlock_40");
	writer.EndArray();
	writer.EndObject();
	
	returnval = s.GetString();	
	s.Clear();
	writer.Flush();
	return returnval;
}





CString CSqlData::deleteDataCommentChange(CString deals)
{
	CString returnval=L"";

	CString dealfound=L"";


	HRESULT hr=NULL ;
	CCommand<CNoAccessor, CNoRowset> UpdateCommand;							
	if(!SUCCEEDED(hr))
	{
		return L"";
	}
	CString   strCommand=L"";
	strCommand.Format(L"delete  from orika_CommentChangeDeals  where deal in (%s);",deals);	
	
	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr=UpdateCommand.Open(m_tempSession,(LPCTSTR)strCommand);
	
	UpdateCommand.Close();
	m_tempSession.Close();	
	//(L"U_Orderlock_41");
	CString tmpstr=L"";	
	returnval.Format(L"{\"type\":\"DELETE_COMMENT_CHANGE_STATUS\",\"deals\":[%s],\"status\":\"Data Has Been Deleted\"}",deals);	
	return returnval;
}

void CSqlData::deleteColumnSubscriptionData(CString m_loginUser,  CString m_requestType)
{	
	CString dealfound = L"";
	HRESULT hr = NULL;
	CCommand<CNoAccessor, CNoRowset> UpdateCommand;
	if (!SUCCEEDED(hr))
	{
		return ;
	}
	CString   strCommand = L"";
	strCommand.Format(L"delete from loginColumnMapping where  userlogin='%s' and RequestKey='%s';", m_loginUser, m_requestType);
	CSession m_tempSession;
	//(L"Orderlock_42");
	
	m_tempSession.Open(CStaticClass::connection);
	hr = UpdateCommand.Open(m_tempSession, (LPCTSTR)strCommand);
	UpdateCommand.Close();
	m_tempSession.Close();	
	//(L"U_Orderlock_42");
	CStaticClass::m_mutexcolumnSubs.Lock();
	POSITION pos = CStaticClass::m_columnsubscription.GetStartPosition();

	CString KeyForDelete = L"";
	KeyForDelete.Format(L"%s:%s", m_loginUser, m_requestType);

	while (pos != NULL)
	{
		CString loginUserColumnKey = L"";
		int subs = 0;
		CStaticClass::m_columnsubscription.GetNextAssoc(pos, loginUserColumnKey,subs);

		CStaticClass::m_logfile.LogEvent(L"loginUserColumnKey");
		CString m_getUserlogin = loginUserColumnKey.Mid(0, loginUserColumnKey.Find(L"|"));		
		CStaticClass::m_logfile.LogEvent(L"End loginUserColumnKey");
		if (m_getUserlogin == KeyForDelete)
		{
			CStaticClass::m_columnsubscription.RemoveKey(loginUserColumnKey);
		}
	}	
	CStaticClass::m_mutexcolumnSubs.Unlock();

}

CString CSqlData::SaveColumnSubscriptionData(CString m_loginUser,CString m_columnKey, CString m_requestKey)
{
	CString returnval = L"";
	CString dealfound = L"";
	HRESULT hr = NULL;
	CCommand<CNoAccessor, CNoRowset> UpdateCommand;
	if (!SUCCEEDED(hr))
	{
		return L"";
	}
	CString   strCommand = L"";
	strCommand.Format(L"insert into loginColumnMapping(userlogin,ColumnKey,subscribe,RequestKey)values('%s','%s',1,'%s') ;", m_loginUser, m_columnKey, m_requestKey);
	
	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr = UpdateCommand.Open(m_tempSession, (LPCTSTR)strCommand);

	UpdateCommand.Close();
	m_tempSession.Close();
	
	//(L"Orderlock_43");
	CString userLoginColumnKey = L"";
	userLoginColumnKey.Format(L"%s:%s|%s", m_loginUser, m_requestKey, m_columnKey);
	CStaticClass::m_mutexcolumnSubs.Lock();
		CStaticClass::m_columnsubscription.SetAt(userLoginColumnKey,1);
	CStaticClass::m_mutexcolumnSubs.Unlock();
	CString tmpstr = L"";
	//returnval.Format(L"");
	return returnval;
}





int CSqlData::checkDealExist(INT64 deals)
{
	int returnVal=0;
	CString returnval=L"";

	CString dealfound=L"";


	HRESULT hr=NULL ;
	CCommand<CAccessor<CComment>> data_table;								
	if(!SUCCEEDED(hr))
	{
		return 0;
	}
	CString   strCommand=L"";
	strCommand.Format(L"select deal,isnull(comment,'') from orika_CommentChangeDeals  where deal in (%I64u);",deals);	
	
	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr=data_table.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		m_tempSession.Close();		
		return 0;
	}
	int i=0;
	
	int row_count=0;
	CString tmpstr=L"";
	while(hr=data_table.MoveNext()==S_OK)
	{		
		
		returnVal=1;
	}
	data_table.Close();
	m_tempSession.Close();	
	//(L"U_Orderlock_43");
	return returnVal;
}










CString CSqlData::getCommentDealWise(CString deals)
{
	CString returnval=L"";
	CString dealfound=L"";
	HRESULT hr=NULL ;
	CCommand<CAccessor<CComment>> data_table;								
	if(!SUCCEEDED(hr))
	{
		return L"";
	}
	CString   strCommand=L"";
	strCommand.Format(L"select deal,isnull(comment,'') from Orika_dealtableAccounting  where deal in (%s);",deals);	
	
	CSession m_tempSession;
	
	m_tempSession.Open(CStaticClass::connection);
	hr=data_table.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		m_tempSession.Close();		
		return L"";
	}
	int i=0;	
	int row_count=0;
	CString tmpstr=L"";
	while(hr=data_table.MoveNext()==S_OK)
	{				
		int deal=data_table.m_deal;		
		CString comment=data_table.m_comment;

		CString strtmpdata=L"";
		
		strtmpdata.Format(L"{\"deal\":%d,\"comment\":\"%s\"}",deal,comment);				
		
		CString tmpdeal=L"";
		tmpdeal.Format(L"[%d]",deal);
		dealfound=dealfound+tmpdeal;


		if (row_count!=0)
		{
			tmpstr=tmpstr+L","+strtmpdata;
		}
		else
		{
			tmpstr=strtmpdata;
		}
		row_count=row_count+1;
	}
	data_table.Close();
	m_tempSession.Close();	
	//(L"U_Orderlock_44");
	//dealfound
	while(deals!=L"")
	{
		CStaticClass::m_logfile.LogEvent(L"Test_11");
		CString tmpdeals=L"";
		if (deals.Find(L",")>=0)
		{
			tmpdeals=deals.Mid(0,deals.Find(L","));
			tmpdeals=L"["+tmpdeals+L"]";

			deals=deals.Mid(deals.Find(L",")+1,deals.GetLength()-deals.Find(L","));
			if (dealfound.Find(tmpdeals)<0)
			{
				CString strtmpdata=L"";		
				strtmpdata.Format(L"{\"deal\":%s,\"status\":\"invalid\"}",tmpdeals);				
					if (tmpstr!=L"")
					{
						tmpstr=tmpstr+L","+strtmpdata;
					}
					else
					{
						tmpstr=strtmpdata;
					}
			}
		}
		else
		{
			tmpdeals.Format(L"[%s]",deals);
			if (dealfound.Find(tmpdeals)<0)
			{
				CString strtmpdata=L"";		
				strtmpdata.Format(L"{\"deal\":%s,\"status\":\"invalid\"}",tmpdeals);				
					if (tmpstr!=L"")
					{
						tmpstr=tmpstr+L","+strtmpdata;
					}
					else
					{
						tmpstr=strtmpdata;
					}
			}
			deals=L"";
		}
		CStaticClass::m_logfile.LogEvent(L"End Test_11");
	}
	returnval.Format(L"{\"type\":\"COMMENT_DEALWISE\",\"deals\":[%s]}",tmpstr);	
	return returnval;
}

CString CSqlData::getClientwisenetpositionData(CString loginuser)
{		
	
	CString returnval=L"";
	CStaticClass::m_mutex_ClientList.Lock();
	//(L"115");
	CStaticClass::m_mutex_Tick.Lock();
	
	POSITION pos = CStaticClass::mapNetPositionClientWise.GetStartPosition ();					
	CString strUpdateData=L"";
	int firstCheck=0;
	CString str_columnJson=L"";
	CString str_FinalJsonUpdate=L"";
	while (pos != NULL) 
	{
		//////(L"Data 5");
		CString strKey=L"";
		CStaticClass::st_netpositionClientWise st_tmpData={};
		CStaticClass::mapNetPositionClientWise.GetNextAssoc(pos,strKey,st_tmpData);
		
		CString    m_login=st_tmpData.m_login;
		CString    m_name=st_tmpData.m_name;
		CString    m_symbol=st_tmpData.m_symbol;
		double     m_volume=*st_tmpData.m_volume;
		double     m_previousvolume=st_tmpData.m_previousvolume; 
		double     m_difference=st_tmpData.m_difference;
		double     m_average=*st_tmpData.m_average;
		CStaticClass::st_TickBidAskLast     m_lastrate=*st_tmpData.m_lastrate;
		double     m_clientfloatingpl=*st_tmpData.m_clientfloatingpl;
		double     m_clientbalance=*st_tmpData.m_clientbalance;
		double     m_clientnetamount=*st_tmpData.m_clientnetamount;
		double     m_clientgrosstotal=*st_tmpData.m_clientgrosstotal;
		double     m_clientbroktotal=*st_tmpData.m_clientbroktotal;
		double     m_clientnettotal=*st_tmpData.m_clientnettotal;
		double     m_maxallotedqty=*st_tmpData.m_maxallotedqty;
		CString    m_subbroker=st_tmpData.m_subbroker;
		CString    m_broker=st_tmpData.m_broker;
		double     m_clientexposure=*st_tmpData.m_clientexposure;
		double     m_extravolume=*st_tmpData.m_extravolume;
		double     m_freemargin=*st_tmpData.m_freemargin;
		double     m_multi=*st_tmpData.m_multi;
		

		m_clientexposure = abs(m_clientexposure);
		


		CStaticClass::st_ClientContext m_st={};
		CStaticClass::m_ClientContext.Lookup(loginuser,m_st );						
		if (std::find(m_st.m_logins.begin(), m_st.m_logins.end(), m_login) != m_st.m_logins.end())
		{
			CString strUpdateData=L"";
			
			//strUpdateData.Format(L"{\"login\":\"%s\",\"name\":\"%s\",\"symbol\":\"%s\",\"previousvolume\":%.2f,\"difference\":%.2f,\"volume\":%.2f,\"average\":%.4f,\"lastrate\":%.4f,\"clientfloatingpl\":%.2f,\"clientbalance\":%.2f,\"clientnetamount\":%.2f,\"clientgrosstotal\":%.0f,\"clientbroktotal\":%.2f,\"clientnettotal\":%.0f,\"maxallotedqty\":%.2f,\"subbroker\":\"%s\",\"broker\":\"%s\",\"clientexposure\":%.0f,\"extravolume\":%.0f,\"freemargin\":%.0f}",m_login,m_name,m_symbol,m_previousvolume,m_difference,m_volume,m_average,m_lastrate,m_clientfloatingpl,m_clientbalance,m_clientnetamount,m_clientgrosstotal,m_clientbroktotal,m_clientnettotal,m_maxallotedqty,m_subbroker,m_broker,m_clientexposure,m_extravolume,m_freemargin);
			strUpdateData.Format(L"{\"login\":\"%s\",\"name\":\"%s\",\"symbol\":\"%s\",\"previousvolume\":%.2f,\"difference\":%.2f,\"volume\":%.2f,\"average\":%.4f,\"lastrate\":%.4f,\"clientfloatingpl\":%.2f,\"clientbalance\":%.2f,\"clientnetamount\":%.2f,\"maxallotedqty\":%.2f,\"subbroker\":\"%s\",\"broker\":\"%s\",\"clientexposure\":%.0f,\"extravolume\":%.0f,\"freemargin\":%.0f}",m_login,m_name,m_symbol,m_previousvolume,m_difference,m_volume,m_average,m_lastrate,m_clientfloatingpl,m_clientbalance,m_clientnetamount,m_maxallotedqty,m_subbroker,m_broker,m_clientexposure,m_extravolume,m_freemargin);
			if (firstCheck==0)
			{
				firstCheck=1;
				str_columnJson=strUpdateData;
			}
			else
			{
				str_columnJson=str_columnJson+L","+strUpdateData;
			}			
		}
			
	}
	str_FinalJsonUpdate.Format(L"{\"type\":\"CLIENT_POSITION\",\"insert\":[%s]}",str_columnJson);
	
	returnval=str_FinalJsonUpdate;
	CStaticClass::m_mutex_ClientList.Unlock();
	//(L"U115");
	CStaticClass::m_mutex_Tick.Unlock();
	return returnval;
}


CString CSqlData::getSymbolWisePositionData(CString loginuser)
{		
		CString returnval=L"";
		CString str_columnJson=L"";
		int firstCheck=0;
		CString str_FinalJsonUpdate=L"";
		POSITION possend=CStaticClass::m_MapscripWiseNetPosition_Fix.GetStartPosition();
		while (possend!=NULL )
		{
			CStaticClass::st_scripWiseNetPosition tmpsnp={};
			CString strsymbolkey=L"";
			CStaticClass::m_MapscripWiseNetPosition_Fix.GetNextAssoc(possend,strsymbolkey,tmpsnp);
			CString n_symbol=tmpsnp.m_symbol ;			
			double n_clientQty=tmpsnp.m_clientQty;
			double n_clientAvg=tmpsnp.m_clientAvg;
			double n_clientGrossExposure=tmpsnp.m_clientGrossExposure;
			double n_PL=tmpsnp.m_PL;
			double n_companyQty=tmpsnp.m_companyQty;
			double n_companyAvg=tmpsnp.m_companyAvg;
			double n_lastRate=tmpsnp.m_lastRate  ;
			double n_companyBrokarage=tmpsnp.m_companyBrokarage;
			double n_companyPL=tmpsnp.m_companyPL;
			double n_companyExposure=tmpsnp.m_companyExposure;
			double n_multiplayer=tmpsnp.m_multiplayer;


			CString strUpdateData=L"";
			strUpdateData.Format(L"{\"symbol\":\"%s\",\"clientQty\":%.2f,\"clientAvg\":%.4f,\"clientGrossExposure\":%.0f,\"PL\":%.2f,\"companyQty\":%.2f,\"companyAvg\":%.2f,\"lastRate\":%.0f,\"companyBrokarage\":%.0f,\"companyPL\":%.0f,\"companyExposure\":%.0f,\"multiplayer\":%.0f}",n_symbol,n_clientQty,n_clientAvg,n_clientGrossExposure,n_PL,n_companyQty,n_companyAvg,n_lastRate,n_companyBrokarage,n_companyPL,n_companyExposure,n_multiplayer);
			if (firstCheck==0)
			{
				firstCheck=1;
				str_columnJson=strUpdateData;
			}
			else
			{
				str_columnJson=str_columnJson+L","+strUpdateData;
			}
		}
	str_FinalJsonUpdate.Format(L"{\"type\":\"SYMBOLWISE_POSITION\",\"insert\":[%s]}",str_columnJson);
	
	returnval=str_FinalJsonUpdate;
	
	return returnval;
}
CString CSqlData::getSubBrokerWisePositionData(CString loginuser)
{		
		CString returnval=L"";
		CString str_columnJson=L"";
		int firstCheck=0;
		CString str_FinalJsonUpdate=L"";
		POSITION possend=CStaticClass::m_MapsubbrokerPosition_Fix.GetStartPosition();
		while (possend!=NULL )
		{
			CStaticClass::st_subbrokerPosition tmpsnp={};
			CString strsymbolkey=L"";
			CStaticClass::m_MapsubbrokerPosition_Fix.GetNextAssoc(possend,strsymbolkey,tmpsnp);
			
			CString m_brokername=tmpsnp.m_subbrokername;
			CString m_name=tmpsnp.m_name;
			CString m_symbol=tmpsnp.m_symbol;
			double  m_volume=tmpsnp.m_volume;
			double  m_preVolume=tmpsnp.m_preVolume;
			double  m_volDiff=tmpsnp.m_volDiff;
			double  m_avg=tmpsnp.m_avg;
			double  m_last=tmpsnp.m_last;
			double  m_clientgrossamount=tmpsnp.m_clientgrossamount;
			double  m_clientnetamount=tmpsnp.m_clientnetamount;
			double  m_brokergrossamount=tmpsnp.m_brokergrossamount;
			double  m_brokernetamount=tmpsnp.m_brokernetamount;


			CString strUpdateData=L"";
			strUpdateData.Format(L"{\"subbrokername\":\"%s\",\"name\":\"%s\",\"symbol\":\"%s\",\"volume\":%.2f,\"preVolume\":%.2f,\"volDiff\":%.2f,\"avg\":%.4f,\"last\":%4f,\"clientgrossamount\":%0f,\"clientnetamount\":%.0f,\"brokergrossamount\":%.0f,\"brokernetamount\":%.0f}",m_brokername,m_name,m_symbol,m_volume,m_preVolume,m_volDiff,m_avg,m_last,m_clientgrossamount,m_clientnetamount,m_brokergrossamount,m_brokernetamount);
			if (firstCheck==0)
			{
				firstCheck=1;
				str_columnJson=strUpdateData;
			}
			else
			{
				str_columnJson=str_columnJson+L","+strUpdateData;
			}
		}
	str_FinalJsonUpdate.Format(L"{\"type\":\"SUBBROKER_POSITION\",\"insert\":[%s]}",str_columnJson);
	
	returnval=str_FinalJsonUpdate;
	
	return returnval;
}
CString CSqlData::getBrokerWisePositionData(CString loginuser)
{		
		CString returnval=L"";
		CString str_columnJson=L"";
		int firstCheck=0;
		CString str_FinalJsonUpdate=L"";
		POSITION possend=CStaticClass::m_MapbrokerPosition_Fix.GetStartPosition();
		while (possend!=NULL )
		{
			CStaticClass::st_brokerPosition tmpsnp={};
			CString strsymbolkey=L"";
			CStaticClass::m_MapbrokerPosition_Fix.GetNextAssoc(possend,strsymbolkey,tmpsnp);
			
			CString m_brokername=tmpsnp.m_brokername;
			CString m_name=tmpsnp.m_name;
			CString m_symbol=tmpsnp.m_symbol;
			double  m_volume=tmpsnp.m_volume;
			double  m_preVolume=tmpsnp.m_preVolume;
			double  m_volDiff=tmpsnp.m_volDiff;
			double  m_avg=tmpsnp.m_avg;
			double  m_last=tmpsnp.m_last;
			double  m_clientgrossamount=tmpsnp.m_clientgrossamount;
			double  m_clientnetamount=tmpsnp.m_clientnetamount;
			double  m_brokergrossamount=tmpsnp.m_brokergrossamount;
			double  m_brokernetamount=tmpsnp.m_brokernetamount;


			CString strUpdateData=L"";
			strUpdateData.Format(L"{\"brokername\":\"%s\",\"name\":\"%s\",\"symbol\":\"%s\",\"volume\":%.2f,\"preVolume\":%.2f,\"volDiff\":%.2f,\"avg\":%.4f,\"last\":%4f,\"clientgrossamount\":%0f,\"clientnetamount\":%.0f,\"brokergrossamount\":%.0f,\"brokernetamount\":%.0f}",m_brokername,m_name,m_symbol,m_volume,m_preVolume,m_volDiff,m_avg,m_last,m_clientgrossamount,m_clientnetamount,m_brokergrossamount,m_brokernetamount);
			if (firstCheck==0)
			{
				firstCheck=1;
				str_columnJson=strUpdateData;
			}
			else
			{
				str_columnJson=str_columnJson+L","+strUpdateData;
			}
		}
	str_FinalJsonUpdate.Format(L"{\"type\":\"BROKER_POSITION\",\"insert\":[%s]}",str_columnJson);
	
	returnval=str_FinalJsonUpdate;
	
	return returnval;
}


CString CSqlData::getBalanceDistripution(CString loginuser)
{		
		CString returnval=L"";
		CString str_columnJson=L"";
		int firstCheck=0;
		CString str_FinalJsonUpdate=L"";
		POSITION possend=CStaticClass::m_MapbrokerPosition_Fix.GetStartPosition();
		while (possend!=NULL )
		{
			CStaticClass::st_brokerPosition tmpsnp={};
			CString strsymbolkey=L"";
			CStaticClass::m_MapbrokerPosition_Fix.GetNextAssoc(possend,strsymbolkey,tmpsnp);
			
			CString m_brokername=tmpsnp.m_brokername;
			CString m_name=tmpsnp.m_name;
			CString m_symbol=tmpsnp.m_symbol;
			double  m_volume=tmpsnp.m_volume;
			double  m_preVolume=tmpsnp.m_preVolume;
			double  m_volDiff=tmpsnp.m_volDiff;
			double  m_avg=tmpsnp.m_avg;
			double  m_last=tmpsnp.m_last;
			double  m_clientgrossamount=tmpsnp.m_clientgrossamount;
			double  m_clientnetamount=tmpsnp.m_clientnetamount;
			double  m_brokergrossamount=tmpsnp.m_brokergrossamount;
			double  m_brokernetamount=tmpsnp.m_brokernetamount;


			CString strUpdateData=L"";
			strUpdateData.Format(L"{\"brokername\":\"%s\",\"name\":\"%s\",\"symbol\":\"%s\",\"volume\":%.2f,\"preVolume\":%.2f,\"volDiff\":%.2f,\"avg\":%.4f,\"last\":%4f,\"clientgrossamount\":%0f,\"clientnetamount\":%.0f,\"brokergrossamount\":%.0f,\"brokernetamount\":%.0f}",m_brokername,m_name,m_symbol,m_volume,m_preVolume,m_volDiff,m_avg,m_last,m_clientgrossamount,m_clientnetamount,m_brokergrossamount,m_brokernetamount);
			if (firstCheck==0)
			{
				firstCheck=1;
				str_columnJson=strUpdateData;
			}
			else
			{
				str_columnJson=str_columnJson+L","+strUpdateData;
			}
		}
	str_FinalJsonUpdate.Format(L"{\"type\":\"BROKER_POSITION\",\"insert\":[%s]}",str_columnJson);
	
	returnval=str_FinalJsonUpdate;
	
	return returnval;
}




CString CSqlData::getClientwisePreQty(CString loginuser,int time)
{		
	CString returnval=L"";
	HRESULT hr=NULL ;
	CCommand<CAccessor<CNetpositionPreQtyTable>> data_table;								
	if(!SUCCEEDED(hr))
	{
		return L"";
	}
	CString   strCommand=L"";
	strCommand.Format(L"orika_GetClientPreviousNetQty '%d';",time);	
	
	CSession m_tempSession;

	m_tempSession.Open(CStaticClass::connection);
	hr=data_table.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		m_tempSession.Close();		
		return L"";
	}
	int i=0;
	

	CString m_login=L"";	
	CString m_symbol=L"";
	double m_volume=0;
	
	CString tmpstr=L"";
	int row_count=0;
	//hr=data_table.MoveNext();
	while(hr=data_table.MoveNext()==S_OK)
	{		
		m_login=data_table.m_login;	
	    m_symbol=data_table.m_symbol;
	    m_volume=data_table.m_volume;	   
		CString strtmpdata=L"";
	
		CStaticClass::st_ClientContext m_st={};
		CStaticClass::m_ClientContext.Lookup(loginuser,m_st );				
		if (std::find(m_st.m_logins.begin(), m_st.m_logins.end(), m_login) != m_st.m_logins.end())
		{
			strtmpdata.Format(L"{\"login\":\"%s\",\"symbol\":\"%s\",\"preVolume\":%.2lf}",m_login,m_symbol,m_volume);								
			if (row_count!=0)
			{
				tmpstr=tmpstr+L","+strtmpdata;
			}
			else
			{
				tmpstr=tmpstr+strtmpdata;
			}
			row_count=row_count+1;
		}
	}
	data_table.Close();
	m_tempSession.Close();	

	//(L"U_Orderlock_45");
	returnval.Format(L"{\"type\": \"CLIENT_WISE_PREVIOUS_NET_VOLUME\",\"data\": [%s]}",tmpstr);	
	return returnval;
}

CSqlData::stViewDetails CSqlData::getToggleViewDetails(CString metadataRequest, CString  strLoginUser, int viewSerialNo)
{
	stViewDetails st = {};
	HRESULT hr = NULL;
	CCommand<CAccessor<CToggleView_Details_Table>> m_CToggleViewDetails_table;
	CString   strCommandChield = L"";
	strCommandChield.Format(L"exec proc_ToggleView_Details '%s','%s','%d';", metadataRequest, strLoginUser, viewSerialNo);
	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr = m_CToggleViewDetails_table.Open(m_tempSession, (LPCTSTR)strCommandChield);

	int m_isDarkMode = 0;
	int	m_isFilterOn = 0;
	int	m_isColorOn = 0;
	int m_isPivot = 0;
	

	
	while (hr = m_CToggleViewDetails_table.MoveNext() == S_OK)
	{
		m_isDarkMode = m_CToggleViewDetails_table.m_isDarkMode;
		m_isFilterOn = m_CToggleViewDetails_table.m_isFilterOn;
		m_isColorOn = m_CToggleViewDetails_table.m_isColorOn;
		m_isPivot = m_CToggleViewDetails_table.m_isPivot;		
	}
	m_CToggleViewDetails_table.Close();

	
	st.m_isDarkMode = m_isDarkMode;
	st.m_isFilterOn = m_isFilterOn;
	st.m_isColorOn = m_isColorOn;
	st.m_isPivot = m_isPivot;	
	return st;
}

void CSqlData::getClientmasterData()
{	
	HRESULT hr=NULL ;
	CCommand<CAccessor<CClientmastertable>> data_table;								
	if(!SUCCEEDED(hr))
	{
		return ;
	}
	CString   strCommand=L"";	
	strCommand.Format(L"GetClientMasterData;");	
	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr=data_table.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		m_tempSession.Close();		
		return ;
	}
	int i=0;
	CStaticClass::m_clientmasterarray.Clear();
	while(hr=data_table.MoveNext()==S_OK)
	{		
		CStaticClass::stclientmaster m_stclientmaster={};		
		
		CMTStr::Copy(m_stclientmaster.m_login,data_table.m_login);
		CMTStr::Copy(m_stclientmaster.m_symbolGroup,data_table.m_symbolGroup);
		CMTStr::Copy(m_stclientmaster.m_name,data_table.m_name);
		CMTStr::Copy(m_stclientmaster.m_broker,data_table.m_broker);
		CMTStr::Copy(m_stclientmaster.m_subBroker,data_table.m_subBroker);
		CMTStr::Copy(m_stclientmaster.m_extraGroup,data_table.m_extraGroup);
		m_stclientmaster.m_subBrokerPLRatio=data_table.m_subBrokerPLRatio ;
		m_stclientmaster.m_brokerPLRatio =data_table.m_brokerPLRatio;
		m_stclientmaster.m_companyPLRatio =data_table.m_companyPLRatio;
		CMTStr::Copy(m_stclientmaster.m_brokageType ,data_table.m_brokageType);
		m_stclientmaster.m_clientBrokage =data_table.m_clientBrokage;
		m_stclientmaster.m_subBrokerBrokage  =data_table.m_subBrokerBrokage;
		m_stclientmaster.m_bokerBrokage =data_table.m_bokerBrokage;
		m_stclientmaster.m_companyBrokage=data_table.m_companyBrokage ;
		m_stclientmaster.m_symbolWiseBuyLimit=data_table.m_symbolWiseBuyLimit ;
		m_stclientmaster.m_symbolWiseSellLimit=data_table.m_symbolWiseSellLimit ;
		m_stclientmaster.m_symbolWisePendingOrderEnableDisable=data_table.m_symbolWisePendingOrderEnableDisable ;
		m_stclientmaster.m_symbolPositionLimit=data_table.m_symbolPositionLimit ;
		m_stclientmaster.m_symbolPendingOrderDiffFromBidAsk=data_table.m_symbolPendingOrderDiffFromBidAsk ;
		m_stclientmaster.m_lossLimit=data_table.m_lossLimit ;
		m_stclientmaster.m_creditLimit=data_table.m_creditLimit ;
		m_stclientmaster.m_comment=data_table.m_comment ;
		m_stclientmaster.m_qtyLimitMultiplayer=data_table.m_qtyLimitMultiplayer ;
		m_stclientmaster.m_ignoreTrader=data_table.m_ignoreTrader ;
		CMTStr::Copy(m_stclientmaster.m_colour,data_table.m_colour);
		CStaticClass::m_clientmasterarray.Add(&m_stclientmaster);
	}
	m_tempSession.Close();	
	//(L"U_Orderlock_46");
}
CString CSqlData::gettingFilterStateJson(CString metadataRequest,CString strLoginUser,int viewSlNo)
{
	HRESULT hr = NULL;
	CString  strCommandChield = L"";
	CCommand<CAccessor<CFilterState_Table>> m_CFilterState_Table;
	strCommandChield.Format(L"exec proc_ToggleViewfilterState '%s','%s','%d';", metadataRequest, strLoginUser, viewSlNo);
	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr = m_CFilterState_Table.Open(m_tempSession, (LPCTSTR)strCommandChield);
	CString strFilterState = L"";
	while (hr = m_CFilterState_Table.MoveNext() == S_OK)
	{
		CString strsubFilterState = L"";
		CString strcolID = m_CFilterState_Table.m_colID;
		CString filterJson = m_CFilterState_Table.m_filterjson;
		strsubFilterState.Format(L"\"%s\":%s", strcolID, filterJson);
		strFilterState = strFilterState + strsubFilterState + L",";
	}
	m_CFilterState_Table.Close();
	m_tempSession.Close();
	CString strFilterStateFinal = L"";
	
	if (strFilterState.GetLength() > 0)
	{
		strFilterStateFinal = strFilterState.Mid(0, strFilterState.GetLength() - 1);
	}
	
	return strFilterStateFinal;
}

CString CSqlData::gettingShortingColJson(CString metadataRequest, CString strLoginUser, int viewSlNo)
{
	HRESULT hr = NULL;
	CString  strCommandChield = L"";
	CCommand<CAccessor<CToggleViewshortingState_table>> m_CToggleViewshortingState;
	strCommandChield.Format(L"exec proc_ToggleViewshortingState '%s','%s','%d';", metadataRequest, strLoginUser, viewSlNo);
	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr = m_CToggleViewshortingState.Open(m_tempSession, (LPCTSTR)strCommandChield);

	CString strFinalShortingState = L"";
	CString strShortingState = L"";
	while (hr = m_CToggleViewshortingState.MoveNext() == S_OK)
	{
		CString strsub = L"";
		CString strcolID = m_CToggleViewshortingState.m_colID;
		CString sort = m_CToggleViewshortingState.m_sort;
		strsub.Format(L"{\"colId\":\"%s\",\"sort\":\"%s\"}", strcolID, sort);
		strShortingState = strShortingState + strsub + L",";
	}
	m_CToggleViewshortingState.Close();
	m_tempSession.Close();
	
	if (strShortingState.GetLength() > 0)
	{
		strFinalShortingState = strShortingState.Mid(0, strShortingState.GetLength() - 1);
	}
	
	return strFinalShortingState;
}
CString CSqlData::generateMasterMetadata(CString metadataRequest,CString strLoginUser)
{
	CString rval=L"";
	HRESULT hr=NULL ;
	CCommand<CAccessor<CMetadataTable>> data_table;								
	if(!SUCCEEDED(hr))
	{
		return L"" ;
	}
	CString   strCommand=L"";	
	strCommand.Format(L"exec GetMetaData '%s','%s'",strLoginUser,metadataRequest);		
	CSession m_tempSession;
	//(L"Orderlock_47");
	m_tempSession.Open(CStaticClass::connection);
	hr=data_table.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		return L"";
		m_tempSession.Close();		
	}
	CString tmpstr=L"";
	CString  m_key=L"";
    CString m_displayValue=L"";
	CString m_dataType=L"";
	CString m_GroupingColumns = L"";
    int m_size=0 ;
    int m_editable=0 ;
    int m_hidden =0;
	int m_GroupingEnabled = 0;
	int m_PivotEnabled = 0;
	int m_decimalLocator=0;
    int m_sequence=0 ;
	CString m_metadatatype=L"";
	int row_count=0;

	int m_calculateTotal=0;
	CString str_calculateTotal=L"";

	int m_iscurrency=0;
	int m_shouldanimate=0;

	CString m_inputcontrol=L"";

	int m_EnableRowGroup;
	int m_EnablePivot;
	int m_EnableValue;
	int m_template;


	while(hr=data_table.MoveNext()==S_OK)
	{			
		m_key=data_table.m_key;
		m_displayValue=data_table.m_displayValue;
		m_dataType=data_table.m_dataType;
		m_size=data_table.m_size ;
		m_decimalLocator=data_table.m_decimalLocator;
		m_editable=data_table.m_editable;
		m_hidden =data_table.m_hidden;
		m_sequence=data_table.m_sequence;
		m_metadatatype=data_table.m_metadatatype;
		m_calculateTotal=data_table.m_calculateTotal;
		m_iscurrency=data_table.m_iscurrency;
		m_GroupingEnabled = data_table.m_GroupingEnabled;
		m_PivotEnabled = data_table.m_PivotEnabled;
		m_template = data_table.m_template;
		CString striscurrency=L"";
		if (m_calculateTotal==1)
		{
			str_calculateTotal="true";
		}
		else
		{
			str_calculateTotal="false";
		}



		if (m_iscurrency==1)
		{
			striscurrency="true";
		}
		else
		{
			striscurrency="false";
		}
		m_shouldanimate=data_table.m_shouldanimate;
		CString strshouldanimate=L"";
		if (m_shouldanimate==1)
		{
			strshouldanimate="true";
		}
		else
		{
			strshouldanimate="false";
		}
		m_inputcontrol=data_table.m_inputcontrol;

		CString str_inputcontrol=L"";
		if (m_inputcontrol!="")
		{
			str_inputcontrol.Format(L"\"inputcontrol\":\"%s\",",m_inputcontrol);
		}

		CString strhidden=L"";
		
		CString strGroupingEnabled = L"";
		CString streditable = L"";
		if (m_editable==1)
		{
			streditable=L"true";
		}
		else
		{
			streditable=L"false";
		}


		if (m_hidden ==1)
		{
			strhidden=L"true";
		}
		else
		{
			strhidden=L"false";
		}
		if (m_GroupingEnabled == 1)
		{
			strGroupingEnabled = L"true";
		}
		else
		{
			strGroupingEnabled = L"false";
		}
		CString strPivotEnabled = L"";
		if (m_PivotEnabled == 1)
		{
			strPivotEnabled = L"true";
		}
		else
		{
			strPivotEnabled = L"false";
		}		
		m_GroupingColumns = data_table.m_GroupingColumns;
		CString m_PendingGroup = L"";
		CString m_json = L"";


		

		CCommand<CAccessor<CChieldMetadataTable>> m_Chielddata_table;
		CString   strCommandChield = L"";
		strCommandChield.Format(L"select childrenColumnKey,displayValue,dataType,size,enablePivot,enableRowGroup,enableValue,shouldAnimate,isCurrency,inputcontrol,sequence,editable,decimalLocator,colorType from  Orika_ChieldColumn where metadatatype='%s' and parentcolumnkey='%s'  order by sequence asc", metadataRequest, m_key);		
		hr = m_Chielddata_table.Open(m_tempSession, (LPCTSTR)strCommandChield);
		while (hr = m_Chielddata_table.MoveNext() == S_OK)
		{
			CString m_Clild = m_Chielddata_table.m_childrenColumnKey;
			CString m_headerName = m_Chielddata_table.m_displayValue;
			CString m_field = m_Clild;
			CString m_clieldDataType = m_Chielddata_table.m_dataType;
			int m_minWidth = 100;
			CString m_filter = L"agTextColumnFilter";

			int m_decimalLocator= m_Chielddata_table.m_decimalLocator;


			int m_colortype = m_Chielddata_table.m_colortype;

			m_inputcontrol = m_Chielddata_table.m_inputcontrol;

			CString str_inputcontrol = L"";
			if (m_inputcontrol != "")
			{
				str_inputcontrol.Format(L"\"inputcontrol\":\"%s\",", m_inputcontrol);
			}


			m_EnableRowGroup = m_Chielddata_table.m_enableRowGroup;
			m_EnablePivot = m_Chielddata_table.m_enablePivot;
			m_EnableValue = m_Chielddata_table.m_enableValue;

			CString  m_strm_EnableRowGroup = L"";
			CString  m_strm_EnablePivot = L"";
			CString  m_strm_EnableValue = L"";
			if (m_EnableRowGroup == 0)
			{
				m_strm_EnableRowGroup = L",\"enableRowGroup\":false";
			}
			if (m_EnableRowGroup == 1)
			{
				m_strm_EnableRowGroup = L",\"enableRowGroup\":true";
			}
			if (m_EnablePivot == 0)
			{
				m_strm_EnablePivot = L",\"enablePivot\":false";
			}
			if (m_EnablePivot == 1)
			{
				m_strm_EnablePivot = L",\"enablePivot\":true";
			}
			if (m_EnableValue == 0)
			{
				m_strm_EnableValue = L",\"enableValue\":false";
			}
			if (m_EnableValue == 1)
			{
				m_strm_EnableValue = L",\"enableValue\":true";
			}





			m_editable = m_Chielddata_table.m_editable;
			CString streditable = L"";
			if (m_editable == 1)
			{
				streditable = L"true";
			}
			else
			{
				streditable = L"false";
			}
			CString m_tmpJson = L"";
			m_tmpJson.Format(L"{\"headerName\": \"%s\",\"field\": \"%s\",\"minWidth\": %d,\"filter\": \"agTextColumnFilter\"%s%s%s,\"decimalLocator\":%d,\"dataType\":\"%s\",%s\"editable\":%s,\"colorType\":%d,\"template\":%d}", m_headerName, m_field, m_minWidth, m_strm_EnableRowGroup, m_strm_EnablePivot, m_strm_EnableValue, m_decimalLocator, m_clieldDataType, str_inputcontrol, streditable, m_colortype,m_template);
			if (m_json.GetLength() > 0)
			{
				m_json = m_json+L"," + m_tmpJson;
			}
			else
			{
				m_json =m_tmpJson;
			}
			
		}
		m_Chielddata_table.Close();
		m_json = L"[" + m_json + L"]";
		CString m_Groupingarray = m_json;

		CString strtmpdata=L"";
		if (m_size!=0)
		{
			strtmpdata.Format(L"{\"key\": \"%s\",\"template\":%d,\"displayValue\": \"%s\",\"dataType\": \"%s\",\"size\": %d,\"editable\": %s,\"hidden\": %s,%s\"sequence\": %d,\"iscurrency\":%s,\"shouldanimate\":%s,\"calculateTotal\":%s,\"GroupingEnabled\":%s,\"PivotEnabled\":%s,\"GroupingColumns\":%s}",m_key,m_template,m_displayValue,m_dataType,m_size,streditable,strhidden,str_inputcontrol,m_sequence,striscurrency,strshouldanimate,str_calculateTotal, strGroupingEnabled,strPivotEnabled, m_Groupingarray);
		}
		else
		{
			strtmpdata.Format(L"{\"key\": \"%s\",\"template\":%d,\"displayValue\": \"%s\",\"dataType\": \"%s\",\"decimalLocator\": %d,\"editable\": %s,\"hidden\": %s,%s\"sequence\": %d,\"iscurrency\":%s,\"shouldanimate\":%s,\"calculateTotal\":%s,\"GroupingEnabled\":%s,\"PivotEnabled\":%s,\"GroupingColumns\":%s}",m_key, m_template,m_displayValue,m_dataType,m_decimalLocator,streditable,strhidden,str_inputcontrol,m_sequence,striscurrency,strshouldanimate,str_calculateTotal, strGroupingEnabled,strPivotEnabled, m_Groupingarray);
		}
		if (row_count!=0)
		{
			tmpstr=tmpstr+L","+strtmpdata;
		}
		else
		{
			tmpstr=tmpstr+strtmpdata;
		}
		row_count=row_count+1;

	}
	
	CString responseKey=L"";
	CStaticClass::m_requestResponseData.Lookup(metadataRequest,responseKey);

	CString strUnique=L"";
	CStaticClass::m_uniqueFiels.Lookup(metadataRequest,strUnique);

	//Getting ToggleView Details

	CString strFilterStateFinal = L""; 
	CString strFinalShortingState = L"";

	
	CString   strCommandChield = L"";
	CString m_toggleViewName = L"";
	
	

	//End oF getting Toggle View Details

	//Getting toggle View
	CCommand<CAccessor<CToggleView>> m_CToggleView_table;
	
	strCommandChield.Format(L"exec proc_ToggleView '%s','%s';", metadataRequest, strLoginUser);
	
	hr = m_CToggleView_table.Open(m_tempSession, (LPCTSTR)strCommandChield);
	CString m_oldToggleViewName = L"";

	CString m_substringJson = L"";
	CString m_columnArray = L"";
	CString m_toggleArray = L"";
	int m_Viewno = 0;
	
	 m_toggleViewName = L"";
	while (hr = m_CToggleView_table.MoveNext() == S_OK)
	{
		CString m_metadatatype    = m_CToggleView_table.m_metadatatype;
		m_toggleViewName  = m_CToggleView_table.m_toggleViewName;
		CString m_colId			  = m_CToggleView_table.m_colId;
		int     m_rowGroupIndex   = m_CToggleView_table.m_rowGroupIndex;
		CString m_aggFunc         = m_CToggleView_table.m_aggFunc;
		
		CString strColumArraySubstring = L"";
		

		int m_pivotIndex = m_CToggleView_table.m_povitIndex;
		CString 	m_pinned = m_CToggleView_table.m_pinned;
		
		

		int m_hide = m_CToggleView_table.m_hide;
		CString mstrhide = (m_hide == 1)?L"true":L"false";

		CString strAggDetail = L"";
		if (m_aggFunc == L"")
		{
			strAggDetail = L"\"aggFunc\":null";
		}
		else
		{
			strAggDetail.Format(L"\"aggFunc\":\"%s\"", m_aggFunc);
		}
		int m_width = m_CToggleView_table.m_width;

		int m_template = m_CToggleView_table.m_template;
		if (m_oldToggleViewName != m_toggleViewName && m_oldToggleViewName != L"")
		{
			CSqlData::stViewDetails st_vd = {};
			st_vd = CSqlData::getToggleViewDetails(metadataRequest, strLoginUser, m_Viewno);

			int m_isDarkMode = st_vd.m_isDarkMode;
			int	m_isFilterOn = st_vd.m_isFilterOn;
			int	m_isColorOn = st_vd.m_isColorOn;
			int m_isPivot = st_vd.m_isPivot;
			

			CString m_strisDarkMode = L"";
			CString m_strisFilterOn = L"";
			CString m_strisColorOn = L"";

			CString m_strisPivot = L"";

			m_strisDarkMode = (m_isDarkMode == 0) ? "false" : "true";
			m_strisColorOn = (m_isColorOn == 0) ? "false" : "true";
			m_strisFilterOn = (m_isFilterOn == 0) ? "false" : "true";
			m_strisPivot = (m_isPivot == 0) ? "false" : "true";


			strFilterStateFinal=gettingFilterStateJson(m_metadatatype, strLoginUser, m_Viewno);
			strFinalShortingState=gettingShortingColJson(m_metadatatype, strLoginUser, m_Viewno);
			
		    m_substringJson.Format(L"{\"viewName\":\"%s\",\"viewNo\":%d,\"metadatatype\":\"%s\",\"columns\": [%s],\"filterState\":{%s},\"sortingState\":[%s],\"isDarkMode\":%s,\"isColorOn\":%s,\"isFilterOn\":%s,\"isPivot\":%s}", m_oldToggleViewName, m_Viewno, m_metadatatype, m_columnArray, strFilterStateFinal, strFinalShortingState, m_strisDarkMode, m_strisColorOn, m_strisFilterOn, m_strisPivot);
			m_columnArray = L"";
			if (m_toggleArray.Trim() == L"")
			{
				m_toggleArray = m_substringJson;
			}
			else
			{
				m_toggleArray = m_toggleArray + L"," + m_substringJson;
			}
			m_columnArray = L"";
		}
		m_Viewno = m_CToggleView_table.m_viewSerialNo;
		
		CString m_str_pivotIndex = L""; 
		if (m_pivotIndex == -1)
		{
			m_str_pivotIndex = L"null";
		} 
		else
		{
			m_str_pivotIndex.Format(L"%d", m_pivotIndex);
		}
		
		CString  m_str_rowGroupIndex = L"";
		if (m_rowGroupIndex == -1)
		{
			m_str_rowGroupIndex = L"null";
		}
		else
		{
			m_str_rowGroupIndex.Format(L"%d", m_rowGroupIndex);
		}
		CString m_str_pinned = L"";
		if (m_pinned == L"-1")
		{
			m_str_pinned = L"null";
		}
		else 
		{
			m_str_pinned.Format(L"\"%s\"", m_pinned);
		}
		
		strColumArraySubstring.Format(L"{\"colId\":\"%s\",\"hide\":%s,%s,\"width\":%d,\"pivotIndex\":%s,\"pinned\":%s,\"rowGroupIndex\":%s,\"template\":%d}", m_colId, mstrhide, strAggDetail, m_width, m_str_pivotIndex, m_str_pinned, m_str_rowGroupIndex,m_template );
		

		if (m_columnArray.Trim() == L"")
		{
			m_columnArray = strColumArraySubstring;
		}
		else
		{
			m_columnArray = m_columnArray+L","+ strColumArraySubstring;
		}

		
		m_oldToggleViewName = m_toggleViewName;
	}
	

	//Getting toggle View
	

	
	strFilterStateFinal = gettingFilterStateJson(m_metadatatype, strLoginUser, m_Viewno);
	strFinalShortingState = gettingShortingColJson(m_metadatatype, strLoginUser, m_Viewno);


	CSqlData::stViewDetails st_vd = {};
	st_vd = CSqlData::getToggleViewDetails(metadataRequest, strLoginUser, m_Viewno);

	int m_isDarkMode = st_vd.m_isDarkMode;
	int	m_isFilterOn = st_vd.m_isFilterOn;
	int	m_isColorOn = st_vd.m_isColorOn;
	int m_isPivot = st_vd.m_isPivot;

	CString m_strisDarkMode = L"";
	CString m_strisFilterOn = L"";
	CString m_strisColorOn = L"";

	CString m_strisPivot = L"";

	m_strisDarkMode = (m_isDarkMode == 0) ? "false" : "true";
	m_strisColorOn = (m_isColorOn == 0) ? "false" : "true";
	m_strisFilterOn = (m_isFilterOn == 0) ? "false" : "true";
	m_strisPivot = (m_isPivot == 0) ? "false" : "true";



	m_substringJson.Format(L"{\"viewName\":\"%s\",\"viewNo\":%d,\"metadatatype\":\"%s\",\"columns\": [%s],\"filterState\":{%s},\"sortingState\":[%s],\"isDarkMode\":%s,\"isColorOn\":%s,\"isFilterOn\":%s,\"isPivot\":%s}", m_oldToggleViewName, m_Viewno, m_metadatatype, m_columnArray, strFilterStateFinal, strFinalShortingState, m_strisDarkMode,m_strisColorOn,m_strisFilterOn, m_strisPivot);
	m_columnArray = L"";
	if (m_toggleArray.Trim() == L"")
	{
		m_toggleArray = m_substringJson;
	}
	else
	{
		m_toggleArray = m_toggleArray + L"," + m_substringJson;
	}
	m_CToggleView_table.Close();
	//End of Getting toggle View

	m_tempSession.Close();	
	//(L"U_Orderlock_47");
	rval.Format(L"{\"type\": \"%s\",\"data\": [%s],\"uniqueKey\":[%s],\"toggleView\":[%s]}",responseKey,tmpstr,strUnique, m_toggleArray);
	return rval;
}



CString CSqlData::PositionFileReadAndCheck(CString filePath)
{
	CString returnVal = L"";
	CString strval = L"";
	CStdioFile	myFile;
	if (myFile.Open(filePath, CFile::modeRead))
	{
		myFile.Seek(0, CFile::begin);				
		StringBuffer s;
		Writer<StringBuffer> writer(s);
		writer.StartObject();
		writer.Key("type");
		writer.String("POSITION_MATCH_REPORT");
		writer.Key("message");
		writer.StartArray();
		CString m_Login = L""; 
		CString m_Position = L""; 
		CString m_Symbol = L""; 
		CString m_Type = L""; 
		CString m_Volume = L""; 
		CString m_Price = L""; 
		CString m_Current_Price = L""; 
		CString m_Reason = L"";  
		CString m_Swap = L"";
		int checkStatus = 0;
		CStaticClass::m_logfile.LogEvent(L"Test_16");
		while (myFile.ReadString(strval))
		{
			m_Login = strval.Mid(0, strval.Find(';'));						
			strval = strval.Mid(strval.Find(';') + 1, (strval.GetLength() - strval.Find(';') - 1));
			m_Position = strval.Mid(0, strval.Find(';'));
			strval = strval.Mid(strval.Find(';') + 1, (strval.GetLength() - strval.Find(';') - 1));
			m_Symbol = strval.Mid(0, strval.Find(';'));
			strval = strval.Mid(strval.Find(';') + 1, (strval.GetLength() - strval.Find(';') - 1));
			m_Type= strval.Mid(0, strval.Find(';'));
			strval = strval.Mid(strval.Find(';') + 1, (strval.GetLength() - strval.Find(';') - 1));
			m_Volume= strval.Mid(0, strval.Find(';'));
			strval = strval.Mid(strval.Find(';') + 1, (strval.GetLength() - strval.Find(';') - 1));
			m_Price= strval.Mid(0, strval.Find(';'));
			strval = strval.Mid(strval.Find(';') + 1, (strval.GetLength() - strval.Find(';') - 1));
			m_Current_Price= strval.Mid(0, strval.Find(';'));
			strval = strval.Mid(strval.Find(';') + 1, (strval.GetLength() - strval.Find(';') - 1));
			m_Reason= strval.Mid(0, strval.Find(';'));
			strval = strval.Mid(strval.Find(';') + 1, (strval.GetLength() - strval.Find(';') - 1));
			m_Swap= strval;

			CString m_loginSymbolKey = L"";
			m_loginSymbolKey.Format(L"%s:%s", m_Login, m_Symbol);

			CStaticClass::m_mutex_Tick.Lock();
			CStaticClass::st_netpositionClientWise m_stMain = {};				
				CStaticClass::mapNetPositionClientWise.Lookup(m_loginSymbolKey, m_stMain);
			CStaticClass::m_mutex_Tick.Unlock();
			CString getLogin = L"";
			getLogin = m_stMain.m_login;
			double databaseVolume = 0;
			if (getLogin != L"")
			{
				databaseVolume = *m_stMain.m_volume;
			}
			double fileVolume = _wtof(m_Volume);
			if (m_Type == L"sell")
			{
				fileVolume = -fileVolume;
			}
			CString mismatchData = L"";
			if (databaseVolume!= fileVolume && _wtoi64(m_Login)!=0)
			{
				checkStatus = 1;
				mismatchData.Format(L"%s:%s FV:%.2lf  DV:%2lf", m_Login, m_Symbol, fileVolume, databaseVolume);
				string ssmismatchData = string(CT2CA(mismatchData));
				const char* stmismatchData = ssmismatchData.c_str();
				writer.String(stmismatchData);
			}
		}
		CStaticClass::m_logfile.LogEvent(L" End Test_16");
		writer.EndArray();
		writer.Key("status");
		if (checkStatus == 1)
		{
			writer.String("mismatch");
		}
		if (checkStatus == 0)
		{
			writer.String("match");
		}
		writer.EndObject();
		CString str_FinalJsonUpdate = L"";

		str_FinalJsonUpdate = s.GetString();

		s.Clear();
		writer.Flush();

		return str_FinalJsonUpdate;
	}
}



int CSqlData::positionCount(string strgroupjason,CString filePath)
{
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

	int total_position = 0;
	CStdioFile	myFile;
	int fileopenCheck = 0;
	if (myFile.Open(filePath, CFile::modeRead))
	{
		fileopenCheck = 1;
		myFile.Seek(0, CFile::begin);
		CString strval = L"";
		CString m_Login = L"";
		while (myFile.ReadString(strval))
		{
			//strval = strval.Mid(strval.Find(';')+1,strval.GetLength()- strval.Find(';'));
			CStaticClass::m_logfile.LogEvent(L"Test_17");
			m_Login = strval.Mid(0, strval.Find(';'));
			CStaticClass::m_logfile.LogEvent(L"Test_18");
			CStaticClass::st_Orika_MTclientmaster m_st_Orika_MTclientmaster = {};
			INT64 INT_LOGIN = _wtoi64(m_Login);
			CStaticClass::m_Orika_MTclientmasterHasTable.Lookup(INT_LOGIN, m_st_Orika_MTclientmaster);
			CString m_logingroup = m_st_Orika_MTclientmaster.m_group;

			int groupChecked = 0;
			m_groupmap.Lookup(m_logingroup, groupChecked);
			if (groupChecked == 1)
			{
				total_position = total_position + 1;
			}
		}						
	}
	if (fileopenCheck == 1)
	{
		myFile.Close();
	}
	return total_position;
}




CString CSqlData::generateDashBoardMetadata(CString loginUser)
{

	CString rval = L"";
	HRESULT hr = NULL;
	CCommand<CAccessor<CTableDashboardMetadata>> data_table;
	CString   strCommand = L"";
	strCommand.Format(L"exec getDashboardMetadata '%s';", loginUser);
	CSession m_tempsession;
	//(L"Orderlock_48");
	m_tempsession.Open(CStaticClass::connection);
	hr = data_table.Open(m_tempsession, (LPCTSTR)strCommand);
	if (FAILED(hr))
	{
		m_tempsession.Close();		
		return L"";

	}	
	int data_count = 0;
	int newtab = 1;
	int tabcount = 0;
	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("type");
	writer.String("DASHBOARD_META_DATA");
	writer.Key("data");
	writer.StartArray();
	while (hr = data_table.MoveNext() == S_OK)
	{
		CString m_key= data_table.m_key;
		CString m_headerName= data_table.m_headerName;
		CString m_field= data_table.m_field;
		int   m_pinned= data_table.m_pinned;
		CString m_colId= data_table.m_colId;
		int   m_decimallocator= data_table.m_decimallocator;
		CString m_datatype= data_table.m_datatype;
		int   m_collapsed= data_table.m_collapsed;
		int   m_suppressMenu= data_table.m_suppressMenu;
		int   m_editable= data_table.m_editable;
		int   m_floatingFilter= data_table.m_floatingFilter;
		int   m_resizable= data_table.m_resizable;
		int   m_sortable= data_table.m_sortable;
		int   m_readOnly= data_table.m_readOnly;
		int   m_TemplateID= data_table.m_TemplateID;
		writer.StartObject();

			writer.Key("key");
			string ssKey = string(CT2CA(m_key));
			const char* stKey = ssKey.c_str();
			writer.String(stKey);
			writer.Key("headerName");
			string ssheaderName = string(CT2CA(m_headerName));
			const char* stheaderName = ssheaderName.c_str();
			writer.String(stheaderName);
			writer.Key("field");
			string ssfield = string(CT2CA(m_field));
			const char* stfield = ssfield.c_str();
			writer.String(stfield);
			writer.Key("pinned");
			writer.Bool(m_pinned==0?false:true);
			writer.Key("colId");
			string sscolId = string(CT2CA(m_colId));
			const char* stcolId = sscolId.c_str();
			writer.String(stcolId);
			writer.Key("decimalLocator");
			writer.Int(m_decimallocator);
			writer.Key("dataType");
			string ssdatatype = string(CT2CA(m_datatype));
			const char* stdatatype = ssdatatype.c_str();
			writer.String(stdatatype);
			writer.Key("collapsed");
			writer.Bool(m_collapsed == 0 ? false : true);
			writer.Key("menuTabs");			
			writer.StartArray();
				writer.String("generalMenuTab");
			writer.EndArray();			
			writer.Key("suppressMenu");
			writer.Bool(m_suppressMenu == 0 ? false : true);
			writer.Key("editable");
			writer.Bool(m_editable == 0 ? false : true);
			writer.Key("floatingFilter");
			writer.Bool(m_floatingFilter == 0 ? false : true);
			writer.Key("resizable");
			writer.Bool(m_resizable == 0 ? false : true);
			writer.Key("sortable");
			writer.Bool(m_sortable == 0 ? false : true);
			writer.Key("readOnly");
			writer.Bool(m_readOnly == 0 ? false : true);
			writer.Key("template");
			writer.Int(m_TemplateID);

		writer.EndObject();
	}
	writer.EndArray();
	writer.EndObject();
	m_tempsession.Close();
	
	//(L"U_Orderlock_48");
	CString str_FinalJsonUpdate = L"";
	rval = str_FinalJsonUpdate = s.GetString();
	s.Clear();
	writer.Flush();
	return rval;
}
CString CSqlData::generateTabAndColumnMetadata()
{

	CString rval = L"";
	HRESULT hr = NULL;
	CCommand<CAccessor<CTableTabAndColumn>> data_table;
	CString   strCommand = L"select TabName,[Key],displayValue from metadata where tab='1' order by tabName asc;";
	
	//(L"Orderlock_49");
	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
	if (FAILED(hr))
	{
		m_tempSession.Close();
		
		return L"";

	}
	CString   m_TabName;
	CString   m_Key;
	CString   m_displayValue;
	CString   m_TabNameNewValue;

	CString   m_tabWiseColumn = L"";

	CString		m_column = L"";
	CString		m_allColumn = L"";

	int data_count = 0;
	int newtab = 1;
	int tabcount = 0;
	while (hr = data_table.MoveNext() == S_OK)
	{
		m_TabName = data_table.m_TabName;;
		m_Key = data_table.m_Key;
		m_displayValue = data_table.m_displayValue;



		if (m_TabName != m_TabNameNewValue)
		{
			if (data_count != 0)
			{
				CString tmpm_allColumn = m_allColumn;
				m_allColumn.Format(L"[%s]", tmpm_allColumn);
				if (tabcount == 0)
				{
					m_tabWiseColumn.Format(L"{\"TabName\":\"%s\",\"columns\":%s}", m_TabNameNewValue, m_allColumn);
				}
				else
				{
					CString strtmp = m_tabWiseColumn;
					m_tabWiseColumn.Format(L"%s,{\"TabName\":\"%s\",\"columns\":%s}", strtmp, m_TabNameNewValue, m_allColumn);
				}
				m_allColumn = L"";
				newtab = 1;
				tabcount = 1;
			}

		}
		m_column.Format(L"{\"key\":\"%s\",\"displayValue\":\"%s\"}", m_Key, m_displayValue);
		if (m_allColumn != L"")
		{
			m_allColumn = m_allColumn + L"," + m_column;
		}
		else
		{
			m_allColumn = m_column;
		}


		m_TabNameNewValue = m_TabName;
		data_count = 1;
		newtab = 0;
	}
	m_tempSession.Close();
	
	//(L"U_Orderlock_49");
	if (data_count != 0)
	{
		CString tmpstr = m_allColumn;
		m_allColumn.Format(L"[%s]", tmpstr);
		CString m_tabWiseColumntmp = m_tabWiseColumn;
		m_tabWiseColumn.Format(L"%s,{\"TabName\":\"%s\",\"columns\":%s}", m_tabWiseColumntmp, m_TabName, m_allColumn);
	}
	rval.Format(L"{\"type\": \"ALL_TAB_COLUMN_META_DATA\",\"data\": [%s],\"uniqueKey\":[\"brokerName\",\"symbol\"]}", m_tabWiseColumn);
	return rval;
}




CString CSqlData::generateMetadataGlobal(CString metadatatype)
{	
	CString rval=L"";
	HRESULT hr=NULL ;
	CCommand<CAccessor<CTableMetadataGlobal>> data_table;	
	CString   strCommand = L"select metadatatype,field ,headerName,minWidth,filter,enableRowGroup,enablePivot,enableValue,decimalLocator,dataType,inputcontrol,editable,colorType,sequence from GlobalMetadata where metadatatype='" + metadatatype + "'  order by sequence asc ;";	
	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr=data_table.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		m_tempSession.Close();
		
		return L"";
		
	}


	CString 	m_metadatatype;
	CString		m_field;
	CString		m_headerName;
	int			m_minWidth;
	CString 	m_filter;
	int			m_enableRowGroup;
	int			m_enablePivot;
	int			m_enableValue;
	int			m_decimalLocator;
	CString 	m_dataType;
	CString 	m_inputcontrol;
	int			m_editable;
	CString 	m_colorType;
	int			m_sequence;

	CString m_strMetadataType = L"";
	CStaticClass::m_requestResponseData.Lookup(metadatatype, m_strMetadataType);

	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("type");	
	string ssstrMetadataType = string(CT2CA(m_strMetadataType));
	const char* ststrMetadataType = ssstrMetadataType.c_str();
	writer.String(ststrMetadataType);
	writer.Key("uniqueKey");
	CString strUniqueKey = L"";
	CStaticClass::m_uniqueFiels.Lookup(metadatatype, strUniqueKey);
	string ssstrUniqueKey = string(CT2CA(strUniqueKey));
	const char* ststrUniqueKey = ssstrUniqueKey.c_str();
	writer.StartArray();
	writer.String(ststrUniqueKey);
	writer.EndArray();
	CString m_tableName = L"";
	CStaticClass::m_tablesname.Lookup(metadatatype, m_tableName);
	writer.Key("tablename");
	string sstablename = string(CT2CA(m_tableName));
	const char* sttableName = sstablename.c_str();
	writer.String(sttableName);

	writer.Key("data");
	writer.StartArray();
	

	while(hr=data_table.MoveNext()==S_OK)
	{	
		writer.StartObject();

		m_field = data_table.m_field;
		writer.Key("field");
		string ssfield = string(CT2CA(m_field));
		const char* stfield = ssfield.c_str();
		writer.String(stfield);

		writer.Key("colid");
		writer.String(stfield);

		m_headerName= data_table.m_headerName;		
		writer.Key("headername");
		string ssheaderName = string(CT2CA(m_headerName));
		const char* stheaderName = ssheaderName.c_str();
		writer.String(stheaderName);
		m_minWidth= data_table.m_minWidth;
		writer.Key("minwidth");
		writer.Double(m_minWidth);
		m_filter= data_table.m_filter;
		writer.Key("filter");
		string ssfilter = string(CT2CA(m_filter));
		const char* stfilter = ssfilter.c_str();
		writer.String(stfilter);
		m_enableRowGroup= data_table.m_enableRowGroup;
		writer.Key("enablerowgroup");
		writer.Bool(m_enableRowGroup == 0 ? false : true);
		m_enablePivot= data_table.m_enablePivot;
		writer.Key("enablepivot");
		writer.Bool(m_enablePivot == 0 ? false : true);
		m_enableValue= data_table.m_enableValue;
		writer.Key("enablevalue");
		writer.Bool(m_enableValue == 0 ? false : true);
		m_decimalLocator= data_table.m_decimalLocator;
		writer.Key("decimallocator");
		writer.Int(m_decimalLocator);
		m_dataType= data_table.m_dataType;
		writer.Key("datatype");
		string ssdataType = string(CT2CA(m_dataType));
		const char* stdataType = ssdataType.c_str();
		writer.String(stdataType);
		m_inputcontrol= data_table.m_inputcontrol;
		writer.Key("inputcontrol");
		string ssinputcontrol = string(CT2CA(m_inputcontrol));
		const char* stinputcontrol = ssinputcontrol.c_str();
		writer.String(stinputcontrol);
		m_editable= data_table.m_editable;
		writer.Key("editable");
		writer.Bool(m_editable == 0 ? false : true);
		m_colorType= data_table.m_colorType;
		writer.Key("colortype");
		string sscolorType = string(CT2CA(m_colorType));
		const char* stcolorType = sscolorType.c_str();
		writer.String(stcolorType);
		m_sequence= data_table.m_sequence;
		writer.Key("sequence");
		writer.Int(m_sequence);
		writer.EndObject();
	}
	writer.EndArray();
	writer.EndObject();
	m_tempSession.Close();	
	rval = s.GetString();	
	return rval;
}




CString CSqlData::generateClientmasterJson()
{
	CString returnval=L"";
	CString tmpstr=L"";

	CString  m_login;
	CString  m_symbolGroup;
	CString  m_name;
	CString  m_broker;
	CString  m_subBroker;
	CString  m_extraGroup;
	double m_subBrokerPLRatio;
	double m_brokerPLRatio;
	double m_companyPLRatio;
	CString  m_brokageType;
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
	CString m_colour;


	int rcount=CStaticClass::m_clientmasterarray.Total();
	for (int r=0;r<rcount;r++)
	{		
		CStaticClass::stclientmaster m_st={};
		m_st=CStaticClass::m_clientmasterarray[r];
		m_login=m_st.m_login ;		
		m_symbolGroup=m_st.m_symbolGroup;
		m_name=m_st.m_name;
		m_broker=m_st.m_broker;
		m_subBroker=m_st.m_subBroker;
		m_extraGroup=m_st.m_extraGroup;
		m_subBrokerPLRatio=m_st.m_subBrokerPLRatio;
		m_brokerPLRatio=m_st.m_brokerPLRatio;
		m_companyPLRatio=m_st.m_companyPLRatio;
		m_brokageType=m_st.m_brokageType;
		m_clientBrokage=m_st.m_clientBrokage;
		m_subBrokerBrokage=m_st.m_subBrokerBrokage;
		m_bokerBrokage=m_st.m_bokerBrokage;
		m_companyBrokage=m_st.m_companyBrokage;
		m_symbolWiseBuyLimit=m_st.m_symbolWiseBuyLimit;
		m_symbolWiseSellLimit=m_st.m_symbolWiseSellLimit;
		m_symbolWisePendingOrderEnableDisable=m_st.m_symbolWisePendingOrderEnableDisable;
		m_symbolPositionLimit=m_st.m_symbolPositionLimit;
		m_symbolPendingOrderDiffFromBidAsk=m_st.m_symbolPendingOrderDiffFromBidAsk;
		m_lossLimit=m_st.m_lossLimit;
		m_creditLimit=m_st.m_creditLimit;
		m_comment=m_st.m_comment;
		m_qtyLimitMultiplayer=m_st.m_qtyLimitMultiplayer;
		m_ignoreTrader=m_st.m_ignoreTrader;
		m_colour=m_st.m_colour;

		//Creating Json Data		
		CString strtmpdata=L"";
		strtmpdata.Format(L"{\"login\":\"%s\",\"symbolGroup\":\"%s\",\"name\":\"%s\",\"broker\":\"%s\",\"subBroker\":\"%s\",\"extraGroup\":\"%s\",\"subBrokerPLRatio\":%.2lf,\"brokerPLRatio\":%.2lf,\"companyPLRatio\":%.2lf,\"clientBrokerageType\":\"%s\",\"clientBrokRate\":%.5lf,\"subBrokerBrokRate\":%.5lf,\"brokerBrokRate\":%.5lf,\"companyBrokRate\":%.5lf,\"symbolWiseBuyLimit\":%.2lf,\"symbolWiseSellLimit\":%.2lf,\"symbolWisePendingOrderEnableDisable\":%d,\"symbolPositionLimit\":%d,\"symbolPendingOrderDiffFromBidAsk\":%.4lf,\"lossLimit\":%.2lf,\"creditLimit\":%.2lf,\"comment\":%d,\"qtyLimitMultiplayer\":%.2lf,\"ignoreTrader\":%d,\"colour\":\"%s\"}",m_login,m_symbolGroup,m_name,m_broker,m_subBroker,m_extraGroup,m_subBrokerPLRatio,m_brokerPLRatio,m_companyPLRatio,m_brokageType,m_clientBrokage,m_subBrokerBrokage,m_bokerBrokage,m_companyBrokage,m_symbolWiseBuyLimit,m_symbolWiseSellLimit,m_symbolWisePendingOrderEnableDisable,m_symbolPositionLimit,m_symbolPendingOrderDiffFromBidAsk,m_lossLimit,m_creditLimit,m_comment,m_qtyLimitMultiplayer,m_ignoreTrader,m_colour);				
		if (r!=0)
		{
			tmpstr=tmpstr+L","+strtmpdata;
		}
		else
		{
			tmpstr=tmpstr+strtmpdata;
		}
	}
	returnval.Format(L"{\"type\": \"CLIENT_MASTER_DATA\",\"data\": [%s]}",tmpstr);
	int len=returnval.GetLength();
	return returnval;
}


void CSqlData::getSymbolMasterData()
{	
	HRESULT hr=NULL ;
	CCommand<CAccessor<CSymbolmastertable>> data_table;								
	if(!SUCCEEDED(hr))
	{
		return ;
	}
	CString   strCommand=L"";	
	strCommand.Format(L"GetSymbolMaster;");		
	CSession m_tempSession;
	//(L"Orderlock_50");
	m_tempSession.Open(CStaticClass::connection);
	hr=data_table.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		m_tempSession.Close();
		
		return ;
	}
	int i=0;
	CStaticClass::m_symbolmasterarray.Clear();
	while(hr=data_table.MoveNext()==S_OK)
	{		
		CStaticClass::stsymbolmaster m_stsymbolmaster={};		
		
		
		CMTStr::Copy(m_stsymbolmaster.m_symbol,data_table.m_symbol);
		m_stsymbolmaster.m_closingPrice=data_table.m_closingPrice;
		m_stsymbolmaster.m_multiplayer=data_table.m_multiplayer;
		m_stsymbolmaster.m_symbolMargin=data_table.m_symbolMargin;
		m_stsymbolmaster.m_maxlimitOnsameprice =data_table.m_maxlimitOnsameprice;
		m_stsymbolmaster.m_symbolPendingOrderDiffFromBidAsk =data_table.m_symbolPendingOrderDiffFromBidAsk ;
		CMTStr::Copy(m_stsymbolmaster.m_ltpMapping,data_table.m_ltpMapping);
		m_stsymbolmaster.m_upperCircuit=data_table.m_upperCircuit;
		m_stsymbolmaster.m_lowerCircuit=data_table.m_lowerCircuit;
		CMTStr::Copy(m_stsymbolmaster.m_exchangeSymboll,data_table.m_exchangeSymboll);
		CMTStr::Copy(m_stsymbolmaster.m_nextSymbol,data_table.m_nextSymbol);
		m_stsymbolmaster.m_nselotsize=data_table.m_nselotsize;
		m_stsymbolmaster.m_mtlotsize=data_table.m_mtlotsize;
		m_stsymbolmaster.m_maxAllowedVolume=data_table.m_maxAllowedVolume;
		m_stsymbolmaster.m_allowedLimitPerClient=data_table.m_allowedLimitPerClient;
		m_stsymbolmaster.m_symbolPositionLimit=data_table.m_symbolPositionLimit;
		CMTStr::Copy(m_stsymbolmaster.m_symbolGroup ,data_table.m_symbolGroup );
		m_stsymbolmaster.m_binsize=data_table.m_binsize;
		m_stsymbolmaster.m_pocketnumber=data_table.m_pocketnumber;
		CStaticClass::m_symbolmasterarray.Add(&m_stsymbolmaster);
	}
	m_tempSession.Close();	
	//(L"U_Orderlock_50");
}




void CSqlData::getSymbolMasterDataForTickSubscribe()
{
	HRESULT hr = NULL;
	CCommand<CAccessor<CSymbolmastertableForTickSubscribe>> data_table;
	if (!SUCCEEDED(hr))
	{
		return;
	}
	CString   strCommand = L"";
	strCommand.Format(L"select distinct(isnull(symbol,''))as 'Symbol' from Orika_dealtableAccounting;");
	CSession m_tempSession;
	//(L"Orderlock_51");
	m_tempSession.Open(CStaticClass::connection);
	hr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
	if (FAILED(hr))
	{
		m_tempSession.Close();
		
		return;
	}
	int i = 0;
	
	while (hr = data_table.MoveNext() == S_OK)
	{		
		CString strKey = data_table.m_symbol;
		CString strKeyData = data_table.m_symbol;		
		CStaticClass::m_symbolForTickData.SetAt(strKey, strKeyData);
	}
	m_tempSession.Close();
	
	//(L"U_Orderlock_51");
}
void CSqlData::validateauthKey(CString	authKey, CSqlData::st_logintokendetail& m_temp)
{
	HRESULT hr = NULL;
	CCommand<CAccessor<CTableauthKey>> data_table;
	if (!SUCCEEDED(hr))
	{
		m_temp.returnval = 1;
		return;
	}
	CSession m_tempSession;
	//(L"Orderlock_8");
	m_tempSession.Open(CStaticClass::connection);
	CString command = L"";
	command.Format(L"select loginUser,spreadSheetId,authKey from authKey where authKey='%s'", authKey);
	hr = data_table.Open(m_tempSession, (LPCTSTR)command);
	if (FAILED(hr))
	{
		m_tempSession.Close();
		m_temp.returnval = 1;
		return;
	}
	CString  m_login = L"";

	while (hr = data_table.MoveNext() == S_OK)
	{
		m_login = data_table.m_loginUser;
	}
	data_table.Close();
	m_tempSession.Close();




	if (m_login == L"")
	{
		m_temp.returnval = 1;
	}
	else
	{
		m_temp.returnval = 0;
		CMTStr::Copy(m_temp.m_login, m_login);
		CMTStr::Copy(m_temp.m_hdno, authKey);
	}

}

CString CSqlData::generateSymbolMasterJson()
{
	CString returnval=L"";
	CString tmpstr=L"";
	
	CString m_symbol;
	double m_closingPrice;
	double m_multiplayer;
	double m_symbolMargin ;
	double m_maxlimitOnsameprice; 
	double m_symbolPendingOrderDiffFromBidAsk ; 
	CString m_ltpMapping;
	double m_upperCircuit;
	double m_lowerCircuit;
	CString m_exchangeSymbol;
	CString m_nextSymbol;
	int m_nselotsize;
	int m_mtlotsize;
	double m_maxAllowedVolume;
	double m_allowedLimitPerClient;
	double m_symbolPositionLimit;
	CString m_symbolGroup;
	double m_binsize;
	double m_pocketnumber;

	
	
	int rcount=CStaticClass::m_symbolmasterarray.Total();
	for (int r=0;r<rcount;r++)
	{		
		CStaticClass::stsymbolmaster  m_st={};
		m_st=CStaticClass::m_symbolmasterarray[r];
		m_symbol=m_st.m_symbol;
		m_closingPrice=m_st.m_closingPrice;
		m_multiplayer=m_st.m_multiplayer;
		m_symbolMargin=m_st.m_symbolMargin ;
		m_maxlimitOnsameprice=m_st.m_maxlimitOnsameprice; 
		m_symbolPendingOrderDiffFromBidAsk=m_st.m_symbolPendingOrderDiffFromBidAsk ; 
		m_ltpMapping=m_st.m_ltpMapping;
		m_upperCircuit=m_st.m_upperCircuit;
		m_lowerCircuit=m_st.m_lowerCircuit;
		m_exchangeSymbol=m_st.m_exchangeSymboll;
		m_nextSymbol=m_st.m_nextSymbol;
		m_nselotsize=m_st.m_nselotsize;
		m_mtlotsize=m_st.m_mtlotsize;
		m_maxAllowedVolume=m_st.m_maxAllowedVolume;
		m_allowedLimitPerClient=m_st.m_allowedLimitPerClient;
		m_symbolPositionLimit=m_st.m_symbolPositionLimit;
		m_symbolGroup=m_st.m_symbolGroup;
		m_binsize=m_st.m_binsize;
		m_pocketnumber=m_st.m_pocketnumber;


		//Creating Json Data		
		CString strtmpdata=L"";
		strtmpdata.Format(L"{\"symbol\":\"%s\",\"closingPrice\":%.4lf,\"multiplayer\":%.2lf,\"symbolMargin\":%.2lf,\"maxlimitOnsameprice\":%.2lf,\"symbolPendingOrderDiffFromBidAsk\":%.2lf,\"ltpMapping\":\"%s\",\"upperCircuit\":%.2lf,\"lowerCircuit\":%.2lf,\"exchangeSymbol\":\"%s\",\"nextSymbol\":\"%s\",\"nselotsize\":%d,\"mtlotsize\":%d,\"maxAllowedVolume\":%.2lf,\"allowedLimitPerClient\":%.2lf,\"symbolPositionLimit\":%.2lf,\"symbolGroup\":\"%s\",\"binsize\":%.2lf,\"pocketnumber\":%.2lf}",m_symbol,m_closingPrice,m_multiplayer,m_symbolMargin ,m_maxlimitOnsameprice ,m_symbolPendingOrderDiffFromBidAsk ,m_ltpMapping,m_upperCircuit,m_lowerCircuit,m_exchangeSymbol,m_nextSymbol,m_nselotsize,m_mtlotsize,m_maxAllowedVolume,m_allowedLimitPerClient,m_symbolPositionLimit,m_symbolGroup,m_binsize,m_pocketnumber);				
		if (r!=0)
		{
			tmpstr=tmpstr+L","+strtmpdata;
		}
		else
		{
			tmpstr=tmpstr+strtmpdata;
		}
	}
	returnval.Format(L"{\"type\": \"SYMBOL_MASTER_DATA\",\"data\": [%s]}",tmpstr);
	return returnval;
}

void CSqlData::loadDataOrika_clientmaster()
{
	HRESULT hr=NULL ;
	CCommand<CAccessor<COrika_clientmaster>> data_table;								
	if(!SUCCEEDED(hr))
	{
		return ;
	}
	CString   strCommand=L"";	
	strCommand.Format(L"select [login],name,[broker],subBroker,extraGroup,lossLimit,creditLimit,comment,qtyLimitMultiplayer,ignoreTrader,colour,company,isnull(lpRatio,0)as 'lpRatio',isnull(maxbrokerage,0)as 'maxbrokerage' from Orika_clientmaster;");		
	CSession m_tempSession;
	//(L"Orderlock_52");
	m_tempSession.Open(CStaticClass::connection);
	hr=data_table.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		m_tempSession.Close();
		
		return ;
	}
	int i=0;
	CStaticClass::m_Orika_clientmasterHastable.RemoveAll();
	while(hr=data_table.MoveNext()==S_OK)
	{		
		CStaticClass::st_Orika_clientmaster m_st_Orika_clientmaster={};		
		CString strlogin=data_table.m_login;
		
		CMTStr::Copy(m_st_Orika_clientmaster.m_login,strlogin);
		CMTStr::Copy(m_st_Orika_clientmaster.m_name,data_table.m_name);
		CMTStr::Copy(m_st_Orika_clientmaster.m_broker,data_table.m_broker);
		CMTStr::Copy(m_st_Orika_clientmaster.m_subBroker,data_table.m_subBroker);
		CMTStr::Copy(m_st_Orika_clientmaster.m_extraGroup,data_table.m_extraGroup);
		m_st_Orika_clientmaster.m_lossLimit=data_table.m_lossLimit  ;
		m_st_Orika_clientmaster.m_creditLimit=data_table.m_creditLimit  ;
		m_st_Orika_clientmaster.m_comment=data_table.m_comment  ;
		m_st_Orika_clientmaster.m_qtyLimitMultiplayer=data_table.m_qtyLimitMultiplayer ; 
		m_st_Orika_clientmaster.m_ignoreTrader =data_table.m_ignoreTrader ;
		CMTStr::Copy(m_st_Orika_clientmaster.m_colour,data_table.m_colour) ;
		CMTStr::Copy(m_st_Orika_clientmaster.m_company, data_table.m_company);
		m_st_Orika_clientmaster.m_lpRatio = data_table.m_lpRatio;
		CStaticClass::m_Orika_clientmasterHastable.SetAt(strlogin,m_st_Orika_clientmaster);
	}
	m_tempSession.Close();	
	//(L"U_Orderlock_52");
}

CString CSqlData::GetOrika_subbrokerdata(CString m_subbroker)
{
	HRESULT hr = NULL;
	CCommand<CAccessor<COrika_subbroker>> data_table;
	if (!SUCCEEDED(hr))
	{
		return L"";
	}
	CString   strCommand = L"";

	CString strsubBroker = L"";
	if (m_subbroker.Trim() != L"")
	{
		strsubBroker.Format(L" where [subbroker]='%s'", m_subbroker);
	}


	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("type");
	writer.String("SUBBROKERDATA");
	writer.Key("subbrokers");
	writer.StartArray();


	strCommand.Format(L"select [subbroker],maxbrokerage from orika_subbroker " + strsubBroker + ";");
	CSession m_tempSession;
	//(L"Orderlock_52");
	m_tempSession.Open(CStaticClass::connection);
	hr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
	if (FAILED(hr))
	{
		m_tempSession.Close();		
		return  L"";
	}
	int i = 0;
	CStaticClass::m_Orika_clientmasterHastable.RemoveAll();
	while (hr = data_table.MoveNext() == S_OK)
	{
		CString strsubbroker = data_table.m_subbroker;
		writer.StartObject();
		string sssubbroker = string(CT2CA(strsubbroker));
		const char* stsubbroker = sssubbroker.c_str();
		writer.Key("subbroker");
		writer.String(stsubbroker);
		writer.Key("maxbrokerage");
		writer.Double(data_table.m_maxbrokerage);
		writer.EndObject();
	}
	m_tempSession.Close();	
	//(L"U_Orderlock_52");
	CString strReturnJason = L"";
	writer.EndArray();
	writer.EndObject();
	strReturnJason = s.GetString();
	return strReturnJason;
}

CString CSqlData::GetOrika_brokerdata(CString m_broker)
{
	HRESULT hr = NULL;
	CCommand<CAccessor<COrika_broker>> data_table;
	if (!SUCCEEDED(hr))
	{
		return L"";
	}
	CString   strCommand = L"";

	CString strBroker = L"";
	if (m_broker.Trim() != L"")
	{
		strBroker.Format(L" where [broker]='%s'", m_broker);
	}


	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("type");
	writer.String("BROKERDATA");
	writer.Key("brokers");
	writer.StartArray();


	strCommand.Format(L"select [broker],maxbrokerage from orika_broker " + strBroker + ";");
	CSession m_tempSession;
	//(L"Orderlock_52");
	m_tempSession.Open(CStaticClass::connection);
	hr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
	if (FAILED(hr))
	{
		m_tempSession.Close();
		
		return  L"";
	}
	int i = 0;
	CStaticClass::m_Orika_clientmasterHastable.RemoveAll();
	while (hr = data_table.MoveNext() == S_OK)
	{		
		CString strbroker = data_table.m_broker;
		writer.StartObject();		
		string ssbroker = string(CT2CA(strbroker));
		const char* stbroker = ssbroker.c_str();
		writer.Key("broker");
		writer.String(stbroker);		
		writer.Key("maxbrokerage");
		writer.Double(data_table.m_maxbrokerage);		
		writer.EndObject();
	}
	m_tempSession.Close();	
	//(L"U_Orderlock_52");
	CString strReturnJason = L"";
	writer.EndArray();
	writer.EndObject();
	strReturnJason = s.GetString();
	return strReturnJason;
}



CString CSqlData::GetOrika_clientmaster(CString m_login)
{
	HRESULT hr = NULL;
	CCommand<CAccessor<COrika_clientmaster>> data_table;
	if (!SUCCEEDED(hr))
	{
		return L"";
	}
	CString   strCommand = L"";

	CString strLogin = L"";
	if (m_login.Trim() != L"")
	{
		strLogin.Format(L" where [login]='%s'", m_login);
	}


	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("type");
	writer.String("CLIENTDATA");	
	writer.Key("clients");
	writer.StartArray();


	strCommand.Format(L"select [login],name,[broker],subBroker,extraGroup,lossLimit,creditLimit,comment,qtyLimitMultiplayer,ignoreTrader,colour,company,isnull(lpRatio,0)as 'lpRatio',isnull(maxbrokerage,0)as 'maxbrokerage' from Orika_clientmaster " + strLogin + ";");
	CSession m_tempSession;
	//(L"Orderlock_52");
	m_tempSession.Open(CStaticClass::connection);
	hr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
	if (FAILED(hr))
	{
		m_tempSession.Close();		
		return  L"";
	}
	int i = 0;
	CStaticClass::m_Orika_clientmasterHastable.RemoveAll();
	while (hr = data_table.MoveNext() == S_OK)
	{
		CStaticClass::st_Orika_clientmaster m_st_Orika_clientmaster = {};
		CString strlogin = data_table.m_login;
		writer.StartObject();
		CMTStr::Copy(m_st_Orika_clientmaster.m_login, strlogin);
		string ssLogin = string(CT2CA(strlogin));
		const char* stlogin = ssLogin.c_str();
		writer.Key("login");
		writer.String(stlogin);
		CMTStr::Copy(m_st_Orika_clientmaster.m_name, data_table.m_name);
		CString m_strname = data_table.m_name;
		string sName = string(CT2CA(m_strname));
		const char* stName = sName.c_str();
		writer.Key("name");
		writer.String(stName);
		CMTStr::Copy(m_st_Orika_clientmaster.m_broker, data_table.m_broker);
		CString m_strbroker = data_table.m_broker;
		string sBroker = string(CT2CA(m_strbroker));
		const char* stBroker = sBroker.c_str();
		writer.Key("broker");
		writer.String(stBroker);
		CMTStr::Copy(m_st_Orika_clientmaster.m_subBroker, data_table.m_subBroker);
		CString m_strsubbroker = data_table.m_subBroker;
		string sSubBroker = string(CT2CA(m_strsubbroker));
		const char* stSubBroker = sSubBroker.c_str();
		writer.Key("subbroker");
		writer.String(stSubBroker);
		CMTStr::Copy(m_st_Orika_clientmaster.m_extraGroup, data_table.m_extraGroup);
		CString m_strExtraGroup = data_table.m_extraGroup;
		string sExtraGroup = string(CT2CA(m_strExtraGroup));
		const char* stm_strExtraGroup = sExtraGroup.c_str();
		writer.Key("extragroup");
		writer.String(stm_strExtraGroup);
		m_st_Orika_clientmaster.m_lossLimit = data_table.m_lossLimit;
		writer.Key("losslimit");
		writer.Double(data_table.m_lossLimit);
		m_st_Orika_clientmaster.m_creditLimit = data_table.m_creditLimit;
		writer.Key("creditlimit");
		writer.Double(data_table.m_creditLimit);
		m_st_Orika_clientmaster.m_comment = data_table.m_comment;
		writer.Key("comment");
		writer.Double(data_table.m_comment);
		m_st_Orika_clientmaster.m_qtyLimitMultiplayer = data_table.m_qtyLimitMultiplayer;
		writer.Key("qtylimitmultiplayer");
		writer.Double(data_table.m_qtyLimitMultiplayer);
		m_st_Orika_clientmaster.m_ignoreTrader = data_table.m_ignoreTrader;
		writer.Key("ignoretrader");
		writer.Double(data_table.m_ignoreTrader);
		CMTStr::Copy(m_st_Orika_clientmaster.m_colour, data_table.m_colour);
		CString m_strColour = data_table.m_colour;
		string sColour = string(CT2CA(m_strColour));
		const char* stm_strColour = sColour.c_str();
		writer.Key("colour");
		writer.String(stm_strColour);
		CMTStr::Copy(m_st_Orika_clientmaster.m_company, data_table.m_company);
		CString m_strCompany = data_table.m_company;
		string sCompany = string(CT2CA(m_strCompany));
		const char* stm_strCompany = sCompany.c_str();
		writer.Key("company");
		writer.String(stm_strCompany);
		m_st_Orika_clientmaster.m_lpRatio = data_table.m_lpRatio;
		writer.Key("lpratio");
		writer.Double(data_table.m_lpRatio);
		writer.Key("maxbrokerage");
		writer.Double(data_table.m_maxbrokerage);
		CStaticClass::m_Orika_clientmasterHastable.SetAt(strlogin, m_st_Orika_clientmaster);				
		writer.EndObject();
	}
	m_tempSession.Close();
	
	//(L"U_Orderlock_52");
	CString strReturnJason = L"";
	writer.EndArray();
	writer.EndObject();
	strReturnJason = s.GetString();
	return strReturnJason;
}




void CSqlData::Update_PerfectAndRoundedInDataBase(CString SymbolGroup, double lots, double perfect, double rounded)
{
	CString returnval = L"";
	CString dealfound = L"";
	HRESULT hr = NULL;
	CCommand<CNoAccessor, CNoRowset> UpdateCommand;
	if (!SUCCEEDED(hr))
	{
		return ;
	}
	CString   strCommand = L"";
	strCommand.Format(L"update Orika_GATEWAY_POSITIONDEVIDERATIO set perfect='%.2lf',rounded='%.2lf' where symbolgroup='%s' and lots='%.2lf'", perfect, rounded, SymbolGroup, lots);
	CSession m_tempSession;

	//(L"Orderlock_53");
	m_tempSession.Open(CStaticClass::connection);
	hr = UpdateCommand.Open(m_tempSession, (LPCTSTR)strCommand);

	UpdateCommand.Close();
	m_tempSession.Close();
	
	//(L"U_Orderlock_53");
}


void CSqlData::Load_PerfectAndRoundedInDataBase()
{	
	HRESULT hr = NULL;
	CCommand<CAccessor<COrika_LpRatioData>> data_table;
	if (!SUCCEEDED(hr))
	{
		return;
	}
	CString   strCommand = L"";
	strCommand.Format(L"select symbolgroup,isnull(lots,0)as 'lots',isnull(ratio,0)as 'ratio',isnull(stepup,'0') as 'stepup',isnull(perfect,0)as 'perfect',isnull(rounded,0)as 'rounded' from Orika_GATEWAY_POSITIONDEVIDERATIO WHERE symbolgroup='GOLD' order by lots;");
	CSession m_tempSession;
	//(L"Orderlock_54");
	m_tempSession.Open(CStaticClass::connection);
	hr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
	if (FAILED(hr))
	{
		m_tempSession.Close();
		
		return;
	}		
	CStaticClass::m_LpQtyArray_gold.Clear();
	CString m_symbolGroup = L"GOLD";
	double m_previous_row_Lot = 0;
	int rowCount = 0;
	while (hr = data_table.MoveNext() == S_OK)
	{		
		CStaticClass::st_LpLots   m_st_LpLots = {};		
		m_st_LpLots.m_LpPer= data_table.m_ratio;
		m_st_LpLots.m_stepup= data_table.m_stepup;
		m_st_LpLots.m_perfect = data_table.m_perfect;
		m_st_LpLots.m_rounded= data_table.m_rounded;			
		m_st_LpLots.m_lots = data_table.m_lots;		
		CStaticClass::m_LpQtyArray_gold.Add(&m_st_LpLots);				
		m_previous_row_Lot = m_st_LpLots.m_lots_To;
		rowCount = rowCount + 1;
	}	
	CStaticClass::st_LpLotsArray m_tempArray;
	m_tempArray.Assign(CStaticClass::m_LpQtyArray_gold);
	int dataCount = CStaticClass::m_LpQtyArray_gold.Total();
	for (int dc = 0; dc < dataCount; dc++)
	{
		CStaticClass::st_LpLots   m_tempst = {};
		m_tempst = CStaticClass::m_LpQtyArray_gold[dc];

		if (m_tempst.m_lots <= 0)
		{
			if (m_tempst.m_lots != 0)
			{
				CStaticClass::st_LpLots   m_tempst2 = {};
				m_tempst2 = CStaticClass::m_LpQtyArray_gold[dc + 1];
				m_tempst.m_lots_To = m_tempst.m_lots;
				m_tempst.m_lots = m_tempst2.m_lots;				
				CStaticClass::m_LpQtyArray_gold.Update(dc, &m_tempst);
			}

		}
		else
		{
			CStaticClass::st_LpLots   m_tempst2 = {};
			m_tempst2 = CStaticClass::m_LpQtyArray_gold[dc - 1];			
			m_tempst.m_lots_To = m_tempst.m_lots;
			m_tempst.m_lots = m_tempst2.m_lots_To;
			CStaticClass::m_LpQtyArray_gold.Update(dc, &m_tempst);
		}

	}
	//For Testing Slab
	int totalDataCount = CStaticClass::m_LpQtyArray_gold.Total();
	for (int td = 0; td < totalDataCount; td++)
	{
		CStaticClass::st_LpLots   m_TestSt = {};
		m_TestSt = CStaticClass::m_LpQtyArray_gold[td];
		int test = 0;
	}

	

	
	CStaticClass::m_LpLotsMap.SetAt(m_symbolGroup, CStaticClass::m_LpQtyArray_gold);
	data_table.Close();
	m_tempSession.Close();

	strCommand.Format(L"select symbolgroup,isnull(lots,0)as 'lots',isnull(ratio,0)as 'ratio',isnull(stepup,'0') as 'stepup',isnull(perfect,0)as 'perfect',isnull(rounded,0)as 'rounded' from Orika_GATEWAY_POSITIONDEVIDERATIO WHERE symbolgroup='SILVER' order by lots;");	
	////(StrPrintLino);
	m_tempSession.Open(CStaticClass::connection);
	hr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
	if (FAILED(hr))
	{
		m_tempSession.Close();
		
		return;
	}
	
	CStaticClass::m_LpQtyArray_silver.Clear();
	m_symbolGroup = L"SILVER";
	m_previous_row_Lot = 0;
	rowCount = 0;
	while (hr = data_table.MoveNext() == S_OK)
	{
		CStaticClass::st_LpLots   m_st_LpLots = {};
		m_st_LpLots.m_LpPer = data_table.m_ratio;
		m_st_LpLots.m_stepup = data_table.m_stepup;
		m_st_LpLots.m_perfect = data_table.m_perfect;
		m_st_LpLots.m_rounded = data_table.m_rounded;
		m_st_LpLots.m_lots = data_table.m_lots;
		CStaticClass::m_LpQtyArray_silver.Add(&m_st_LpLots);
		m_previous_row_Lot = m_st_LpLots.m_lots_To;
		rowCount = rowCount + 1;
	}
	 m_tempArray.Clear();
	m_tempArray.Assign(CStaticClass::m_LpQtyArray_silver);
	dataCount = CStaticClass::m_LpQtyArray_silver.Total();
	for (int dc = 0; dc < dataCount; dc++)
	{
		CStaticClass::st_LpLots   m_tempst = {};
		m_tempst = CStaticClass::m_LpQtyArray_silver[dc];

		if (m_tempst.m_lots <= 0)
		{
			if (m_tempst.m_lots != 0)
			{
				CStaticClass::st_LpLots   m_tempst2 = {};
				m_tempst2 = CStaticClass::m_LpQtyArray_silver[dc + 1];
				m_tempst.m_lots_To = m_tempst.m_lots;
				m_tempst.m_lots = m_tempst2.m_lots;
				CStaticClass::m_LpQtyArray_silver.Update(dc, &m_tempst);
			}

		}
		else
		{
			CStaticClass::st_LpLots   m_tempst2 = {};
			m_tempst2 = CStaticClass::m_LpQtyArray_silver[dc - 1];
			m_tempst.m_lots_To = m_tempst.m_lots;
			m_tempst.m_lots = m_tempst2.m_lots_To;
			CStaticClass::m_LpQtyArray_silver.Update(dc, &m_tempst);
		}

	}
	CStaticClass::m_LpLotsMap.SetAt(m_symbolGroup, CStaticClass::m_LpQtyArray_silver);
	data_table.Close();
	m_tempSession.Close();





	//COrika_LpPriotity
	CCommand<CAccessor<COrika_LpPriotity>> data_table_LP;
	strCommand.Format(L"select symbolgroup,lpname,lpsymbol,lppriority,lpmaxlots,lpsendingtype,lpratio from Orika_LpPriotity where symbolgroup='GOLD';");
	////(StrPrintLino);
	m_tempSession.Open(CStaticClass::connection);
	hr = data_table_LP.Open(m_tempSession, (LPCTSTR)strCommand);
	if (FAILED(hr))
	{
		m_tempSession.Close();
		
		return;
	}

	CStaticClass::m_LPdetailArray_gold.Clear();
	m_symbolGroup = L"GOLD";
	while (hr = data_table_LP.MoveNext() == S_OK)
	{
		CStaticClass::st_LpDetails   m_st_LpLots = {};		
		CMTStr::Copy(m_st_LpLots.m_lpname, data_table_LP.m_lpname);
		CMTStr::Copy(m_st_LpLots.m_lpsymbol, data_table_LP.m_lpsymbol);
		m_st_LpLots.m_lppriority= data_table_LP.m_lppriority;
		m_st_LpLots.m_lpmaxlots= data_table_LP.m_lpmaxlots;
		CMTStr::Copy(m_st_LpLots.m_lpsendingtype, data_table_LP.m_lpsendingtype);
		m_st_LpLots.m_lpratio= data_table_LP.m_lpratio;		
		CStaticClass::m_LPdetailArray_gold.Add(&m_st_LpLots);
	}
	CStaticClass::m_LpDetailMap.SetAt(m_symbolGroup, CStaticClass::m_LPdetailArray_gold);
	data_table_LP.Close();
	m_tempSession.Close();


	strCommand.Format(L"select symbolgroup,lpname,lpsymbol,lppriority,lpmaxlots,lpsendingtype,lpratio from Orika_LpPriotity where symbolgroup='SILVER';");
	////(StrPrintLino);
	m_tempSession.Open(CStaticClass::connection);
	hr = data_table_LP.Open(m_tempSession, (LPCTSTR)strCommand);
	if (FAILED(hr))
	{
		m_tempSession.Close();
		
		return;
	}

	m_symbolGroup = L"SILVER";
	CStaticClass::m_LPdetailArray_silver.Clear();
	while (hr = data_table_LP.MoveNext() == S_OK)
	{
		CStaticClass::st_LpDetails   m_st_LpLots = {};
		CMTStr::Copy(m_st_LpLots.m_lpname, data_table_LP.m_lpname);
		CMTStr::Copy(m_st_LpLots.m_lpsymbol, data_table_LP.m_lpsymbol);
		m_st_LpLots.m_lppriority = data_table_LP.m_lppriority;
		m_st_LpLots.m_lpmaxlots = data_table_LP.m_lpmaxlots;
		CMTStr::Copy(m_st_LpLots.m_lpsendingtype, data_table_LP.m_lpsendingtype);
		m_st_LpLots.m_lpratio = data_table_LP.m_lpratio;
		CStaticClass::m_LPdetailArray_silver.Add(&m_st_LpLots);
	}
	CStaticClass::m_LpDetailMap.SetAt(m_symbolGroup, CStaticClass::m_LPdetailArray_silver);
	data_table_LP.Close();
	m_tempSession.Close();




	
	//(L"U_Orderlock_54");
}





void CSqlData::loadDataOrika_clientbrokerage()
{
	HRESULT hr=NULL ;
	CCommand<CAccessor<COrika_clientbrokerage>> data_table;								
	if(!SUCCEEDED(hr))
	{
		return ;
	}
	CString   strCommand=L"";	
	strCommand.Format(L"select [login],symbolGroup,brokageType,clientBrokage,subBrokerBrokage,bokerBrokage,companyBrokage from Orika_clientbrokerage;");	
	CSession m_tempSession;
	//(L"Orderlock_55");
	m_tempSession.Open(CStaticClass::connection);
	hr=data_table.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		m_tempSession.Close();		
		return ;
	}
	int i=0;
	CStaticClass::m_Orika_clientbrokerageHastable.RemoveAll();
	while(hr=data_table.MoveNext()==S_OK)
	{		
		CStaticClass::st_Orika_clientbrokerage m_st_Orika_clientbrokerage={};		
		CString strKey=L"";
		CString strlogin=data_table.m_login;
		
		CString strsymbol=data_table.m_symbolGroup;
		strKey.Format(L"%s:%s",strlogin,strsymbol);		
		CMTStr::Copy(m_st_Orika_clientbrokerage.m_login,data_table.m_login);
		CMTStr::Copy(m_st_Orika_clientbrokerage.m_symbolGroup,data_table.m_symbolGroup);
		m_st_Orika_clientbrokerage.m_brokageType=data_table.m_brokageType;
		m_st_Orika_clientbrokerage.m_clientBrokage=data_table.m_clientBrokage;
		m_st_Orika_clientbrokerage.m_subBrokerBrokage=data_table.m_subBrokerBrokage; 
		m_st_Orika_clientbrokerage.m_bokerBrokage=data_table.m_bokerBrokage;
		m_st_Orika_clientbrokerage.m_companyBrokage=data_table.m_companyBrokage;
		CStaticClass::m_Orika_clientbrokerageHastable.SetAt(strKey,m_st_Orika_clientbrokerage);
	}
	m_tempSession.Close();	
	//(L"U_Orderlock_55");
}



void CSqlData::loadDataOrika_dealsHighLow()
{
	HRESULT hr=NULL ;
	CCommand<CAccessor<COrika_dealsHighLow>> data_table;
	if(!SUCCEEDED(hr))
	{
		return ;
	}
	CString   strCommand=L"";	
	strCommand.Format(L"select symbol+':'+convert(varchar(10),dateadd(s,[time],'01-01-1970'),105) as 'Symbol',min(price)as'LowPrice',max(price)as'HighPrice' from Orika_dealtableAccounting group by symbol+':'+convert(varchar(10),dateadd(s,[time],'01-01-1970'),105);");	
	CSession m_tempSession;
	//(L"Orderlock_56");
	m_tempSession.Open(CStaticClass::connection);
	hr=data_table.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		m_tempSession.Close();
		
		return ;
	}
	int i=0;
	CStaticClass::m_TradesHighLowPriceSymbolWise.RemoveAll();
	while(hr=data_table.MoveNext()==S_OK)
	{		
		CStaticClass::st_TradesHighLowPriceSymbolWise m_st_TradesHighLowPriceSymbolWise ={};
		CString strKey=L"";
		CString strSymbol=data_table.m_symbol;		
		strKey = strSymbol;
		CMTStr::Copy(m_st_TradesHighLowPriceSymbolWise.m_symbol, strSymbol);
		m_st_TradesHighLowPriceSymbolWise.m_High = data_table.m_high;
		m_st_TradesHighLowPriceSymbolWise.m_Low= data_table.m_low;
		CStaticClass::TradesHighLowLockUpdate.Lock();
			CStaticClass::m_TradesHighLowPriceSymbolWise.SetAt(strKey, m_st_TradesHighLowPriceSymbolWise);
		CStaticClass::TradesHighLowLockUpdate.Unlock();
	}
	m_tempSession.Close();
	
	//(L"U_Orderlock_56");
}




























void CSqlData::loadOrika_PLDevideRatio()
{
	HRESULT hr=NULL ;
	CCommand<CAccessor<COrika_PLDevideRatio>> data_table;								
	if(!SUCCEEDED(hr))
	{
		return ;
	}
	CString   strCommand=L"";	
	strCommand.Format(L"select [login],symbolGroup,brokerPLRatio,subBrokerPLRatio,companyPLRatio from Orika_PLDevideRatio;");	
	CSession m_tempSession;
	//(L"Orderlock_57");
	m_tempSession.Open(CStaticClass::connection);
	hr=data_table.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		m_tempSession.Close();
		
		return ;
	}
	int i=0;
	CStaticClass::m_Orika_PLDevideRatioHastable.RemoveAll();
	while(hr=data_table.MoveNext()==S_OK)
	{		
		CStaticClass::st_Orika_PLDevideRatio m_st_Orika_PLDevideRatio={};		
		CString strKey=L"";
		CString strlogin=data_table.m_login;
		CString strsymbol=data_table.m_symbolGroup;
		strKey.Format(L"%s:%s",strlogin,strsymbol);		
		CMTStr::Copy(m_st_Orika_PLDevideRatio.m_login,data_table.m_login);
		CMTStr::Copy(m_st_Orika_PLDevideRatio.m_symbolGroup,data_table.m_symbolGroup);
		
		m_st_Orika_PLDevideRatio.m_brokerPLRatio=data_table.m_brokerPLRatio;
		m_st_Orika_PLDevideRatio.m_subBrokerPLRatio=data_table.m_subBrokerPLRatio;
		m_st_Orika_PLDevideRatio.m_companyPLRatio=data_table.m_companyPLRatio;
				
		CStaticClass::m_Orika_PLDevideRatioHastable.SetAt(strKey,m_st_Orika_PLDevideRatio);
	}
	m_tempSession.Close();
	
	//(L"Orderlock_57");
}






void CSqlData::loadOrika_symbolgroup()
{
	HRESULT hr=NULL ;
	CCommand<CAccessor<COrika_symbolgroup>> data_table;								
	if(!SUCCEEDED(hr))
	{
		return ;
	}
	CString   strCommand=L"";	
	strCommand.Format(L"select symbolGroup,symbolWiseBuyLimit,symbolWiseSellLimit,symbolWisePendingOrderEnableDisable,symbolPositionLimit,symbolPendingOrderDiffFromBidAsk,symbolMargin,expectedChange,binsize,pocketnumber,maxlimitOnsameprice  from Orika_symbolgroup;");		

	CSession m_tempSession;
	//(L"Orderlock_58");

	m_tempSession.Open(CStaticClass::connection);
	hr=data_table.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		m_tempSession.Close();
		
		return ;
	}
	int i=0;
	CStaticClass::m_Orika_symbolgroupHastable.RemoveAll();
	while(hr=data_table.MoveNext()==S_OK)
	{		
		CStaticClass::st_Orika_symbolgroup m_st_Orika_symbolgroup={};		
		CString strsymbolGroup=data_table.m_symbolGroup;		
		CMTStr::Copy(m_st_Orika_symbolgroup.m_symbolGroup,strsymbolGroup);		
		m_st_Orika_symbolgroup.m_symbolWiseBuyLimit=data_table.m_symbolWiseBuyLimit;
		m_st_Orika_symbolgroup.m_symbolWiseSellLimit=data_table.m_symbolWiseSellLimit;
		m_st_Orika_symbolgroup.m_symbolWisePendingOrderEnableDisable=data_table.m_symbolWisePendingOrderEnableDisable;
		m_st_Orika_symbolgroup.m_symbolPositionLimit=data_table.m_symbolPositionLimit;
		m_st_Orika_symbolgroup.m_symbolPendingOrderDiffFromBidAsk=data_table.m_symbolPendingOrderDiffFromBidAsk;
		m_st_Orika_symbolgroup.m_symbolMargin=data_table.m_symbolMargin;
		m_st_Orika_symbolgroup.m_expectedchange=data_table.m_expectedchange;
		m_st_Orika_symbolgroup.m_binsize=data_table.m_binsize;
		m_st_Orika_symbolgroup.m_pocketnumber=data_table.m_pocketnumber;
		m_st_Orika_symbolgroup.m_maxlimitOnsameprice=data_table.m_maxlimitOnsameprice;				
		CStaticClass::m_Orika_symbolgroupHastable.SetAt(strsymbolGroup,m_st_Orika_symbolgroup);
	}
	m_tempSession.Close();	
	//(L"U_Orderlock_58");
}




void CSqlData::loadOrika_LoadGoodTradeBadTrade()
{
	HRESULT hr = NULL;
	CCommand<CAccessor<CTableBadTradeGoodTrade>> data_table;
	if (!SUCCEEDED(hr))
	{
		return;
	}
	CString   strCommand = L"";
	strCommand.Format(L"select t1.[login],t1.symbol,isnull(t1.TotalTrade,0)as 'TotalTrade',isnull(t2.BadTrade,0)as 'BadTrade',(isnull(t1.TotalTrade,0)-isnull(t2.BadTrade,0))as 'GoodTrade',isnull(t3.BadTradeIPC,0)as 'BadTradeIPC',isnull(t4.TradeIPC,0)as'TradeIPC' from  (select [login],symbol,count(*) as 'TotalTrade' from orika_dealtableaccounting where (DATEPART(WEEKDAY, dateadd(S, [Time], '19700101 00:00:00:000'))>1 and DATEPART(WEEKDAY, dateadd(S, [Time], '19700101 00:00:00:000'))<7) group by [login],symbol)t1 left outer join  (select [login],symbol,count(*) as 'BadTrade' from orika_dealtableaccounting where Badtrade=1 group by [login],symbol)t2 on t1.[login]+t1.symbol=t2.[login]+t2.symbol  left outer join (select [login],symbol,count(*) as 'BadTradeIPC' from orika_dealtableaccounting where badtradeignorePosition=1 group by [login],symbol)t3 on t1.[login]+t1.symbol=t3.[login]+t3.symbol left outer join (select [login],symbol,count(*) as 'TradeIPC' from orika_dealtableaccounting where IgnoreTradePosition=1 group by [login],symbol)t4  on t1.[login]+t1.symbol=t4.[login]+t4.symbol;");

	CSession m_tempSession;
	//(L"Orderlock_59")


	m_tempSession.Open(CStaticClass::connection);
	hr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
	if (FAILED(hr))
	{
		m_tempSession.Close();		
		return;
	}
	int i = 0;
	while (hr = data_table.MoveNext() == S_OK)
	{
		CStaticClass::st_BadTradeGoodTrade m_st_BadTradeGoodTrade = {};
		CString m_symbol = data_table.m_symbol;
		CString m_login = data_table.m_login;

		CMTStr::Copy(m_st_BadTradeGoodTrade.m_login, m_login);
		CMTStr::Copy(m_st_BadTradeGoodTrade.m_symbol, m_symbol);
		m_st_BadTradeGoodTrade.m_TotalTrade  = data_table.m_TotalTrade;
		m_st_BadTradeGoodTrade.m_BadTrade = data_table.m_BadTrade;
		m_st_BadTradeGoodTrade.m_GoodTrade = data_table.m_GoodTrade;
		m_st_BadTradeGoodTrade.m_badTradeIgnoringPosition = data_table.m_badTradeIgnoringPosition;
		m_st_BadTradeGoodTrade.m_TradeIgnoringPosition = data_table.m_TradeIgnoringPosition;

		CString strloginSymbol = L"";
		strloginSymbol.Format(L"%s:%s", m_login, m_symbol);
		CStaticClass::m_BadTradeGoodTrade.SetAt(strloginSymbol, m_st_BadTradeGoodTrade);
	}
	m_tempSession.Close();
	
	//(L"U_Orderlock_59");
}



void CSqlData::loadOrika_LoginSymbolWiseLastTrade()
{
	HRESULT hr = NULL;
	CCommand<CAccessor<CTableClientSymbolWiseLastTrade>> data_table;
	if (!SUCCEEDED(hr))
	{
		return;
	}
	CString   strCommand = L"";
	strCommand.Format(L"select t1.Login,t1.symbol,orika_dealtableaccounting.volume,orika_dealtableaccounting.price,isnull(orika_dealtableaccounting.position,0)as 'Position' from (select [Login],symbol,max(deal) as 'Deal' from orika_dealtableaccounting group by [Login],symbol)t1 left outer join orika_dealtableaccounting on t1.deal=orika_dealtableaccounting.deal order by t1.Login,t1.symbol asc;");

	CSession m_tempSession;
	

	m_tempSession.Open(CStaticClass::connection);
	hr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
	if (FAILED(hr))
	{
		m_tempSession.Close();
		
		return;
	}
	int i = 0;	
	while (hr = data_table.MoveNext() == S_OK)
	{
		CStaticClass::st_LoginSymbolWiseLastTrade m_st_LoginSymbolWiseLastTrade = {};
		CString m_symbol = data_table.m_symbol;
		CString m_login = data_table.m_login;
		
		CMTStr::Copy(m_st_LoginSymbolWiseLastTrade.m_login, m_login);
		CMTStr::Copy(m_st_LoginSymbolWiseLastTrade.m_symbol, m_symbol);
		m_st_LoginSymbolWiseLastTrade.m_volume = data_table.m_volume;
		m_st_LoginSymbolWiseLastTrade.m_price = data_table.m_Price;
		m_st_LoginSymbolWiseLastTrade.m_position = data_table.m_Position;

		CString strloginSymbol = L"";
		strloginSymbol.Format(L"%s:%s", m_login, m_symbol);
		CStaticClass::m_LoginSymbolWiseLastTrade.SetAt(strloginSymbol, m_st_LoginSymbolWiseLastTrade);
	}
	m_tempSession.Close();
	
	//(L"U_Orderlock_60");
}




void CSqlData::loadOrika_Clientgatewayconfig()
{
	HRESULT hr=NULL ;
	CCommand<CAccessor<COrika_Clientgatewayconfig>> data_table;								
	if(!SUCCEEDED(hr))
	{
		return ;
	}
	CString   strCommand=L"";	
	strCommand.Format(L"select [login],symbolGroup,symbolWiseBuyLimit,symbolWiseSellLimit,symbolWisePendingOrderEnableDisable,symbolPositionLimit,qtyLimitMultiplayer from  Orika_Clientgatewayconfig;");		
	
	CSession m_tempSession;
	//(L"Orderlock_61");

	m_tempSession.Open(CStaticClass::connection);
	hr=data_table.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		m_tempSession.Close();
		
		return ;
	}
	int i=0;
	CStaticClass::m_Orika_ClientgatewayconfigHastable.RemoveAll();
	while(hr=data_table.MoveNext()==S_OK)
	{		
		CStaticClass::st_Orika_Clientgatewayconfig m_st_Orika_Clientgatewayconfig={};				
		CString strKey=L"";
		CString strlogin=data_table.m_login;
		CString strsymbolgroup=data_table.m_symbolGroup;
		strKey.Format(L"%s:%s",strlogin,strsymbolgroup);								
		
		CMTStr::Copy(m_st_Orika_Clientgatewayconfig.m_login,data_table.m_login);
		CMTStr::Copy(m_st_Orika_Clientgatewayconfig.m_symbolGroup,data_table.m_symbolGroup);
		m_st_Orika_Clientgatewayconfig.m_symbolWiseBuyLimit=data_table.m_symbolWiseBuyLimit;
		m_st_Orika_Clientgatewayconfig.m_symbolWiseSellLimit=data_table.m_symbolWiseSellLimit;
		m_st_Orika_Clientgatewayconfig.m_symbolWisePendingOrderEnableDisable=data_table.m_symbolWisePendingOrderEnableDisable;
		m_st_Orika_Clientgatewayconfig.m_symbolPositionLimit=data_table.m_symbolPositionLimit;
		m_st_Orika_Clientgatewayconfig.m_qtyLimitMultiplayer=data_table.m_qtyLimitMultiplayer;
		

		CStaticClass::m_Orika_ClientgatewayconfigHastable.SetAt(strKey,m_st_Orika_Clientgatewayconfig);
	}
	m_tempSession.Close();
	
	//(L"U_Orderlock_61");
}






void CSqlData::LoadLpRatioCommodityGroupwise()
{
	HRESULT hr = NULL;
	CCommand<CAccessor<CLpRatioCommoditygroupwise>> data_table;
	if (!SUCCEEDED(hr))
	{
		return;
	}
	CString   strCommand = L"";
	strCommand.Format(L"select [login],commoditygroup,lpratio from Orika_clientLpRatio;");

	CStaticClass::m_mutexcommoditygroup.Lock();
	CString StrPrintLino = L"";
	StrPrintLino.Format(L"m_mutex_order Locked(%d)", __LINE__);
	////(StrPrintLino);
	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
	if (FAILED(hr))
	{
		m_tempSession.Close();
		CStaticClass::m_mutexcommoditygroup.Unlock();
		return;
	}
	int i = 0;
	CStaticClass::m_LpRatioCommodityGroupWiseMap.RemoveAll();
	while (hr = data_table.MoveNext() == S_OK)
	{
		CString strKey = L"";
		double m_LpRatio = data_table.m_lpratio;
		CString m_login = data_table.m_login;
		CString m_commoditygroup = data_table.m_commoditygroup;
		strKey.Format(L"%s:%s", m_login, m_commoditygroup);

		CStaticClass::m_LpRatioCommodityGroupWiseMap.SetAt(strKey, m_LpRatio);
	}
	m_tempSession.Close();
	CStaticClass::m_mutexcommoditygroup.Unlock();
}







void CSqlData::loadOrika_symbolmaster()
{
	HRESULT hr=NULL ;
	CCommand<CAccessor<COrika_symbolmaster>> data_table;								
	if(!SUCCEEDED(hr))
	{
		return ;
	}
	CString   strCommand=L"";	
	strCommand.Format(L"select symbol,closingPrice,multiplayer,symbolMargin,upperCircuit,lowerCircuit,ltpMapping,nextSymbol,exchangeSymbol,nselotsize,mtlotsize,symbolGroup,ltpSymbol,exchange,international,sector,industry,rmp,QtyMulti,[page],categary,CurrencyBase,[group] from Orika_symbolmaster;");
	CSession m_tempSession;


	//(L"Orderlock_62");



	m_tempSession.Open(CStaticClass::connection);
	hr=data_table.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		m_tempSession.Close();
		
		return ;
	}
	int i=0;
	CStaticClass::m_Orika_symbolmasterHastable.RemoveAll();
	while(hr=data_table.MoveNext()==S_OK)
	{		
		CStaticClass::st_Orika_symbolmaster m_st_Orika_symbolmaster={};						
		CString strSymbol=data_table.m_symbol;

		
														
		CMTStr::Copy(m_st_Orika_symbolmaster.m_symbol,strSymbol);
		m_st_Orika_symbolmaster.m_closingPrice=data_table.m_closingPrice ;
		m_st_Orika_symbolmaster.m_multiplayer=data_table.m_multiplayer ;
		m_st_Orika_symbolmaster.m_symbolMargin=data_table.m_symbolMargin ;
		m_st_Orika_symbolmaster.m_upperCircuit=data_table.m_upperCircuit ;
		m_st_Orika_symbolmaster.m_lowerCircuit=data_table.m_lowerCircuit ;
		CMTStr::Copy(m_st_Orika_symbolmaster.m_ltpMapping,data_table.m_ltpMapping);
		CMTStr::Copy(m_st_Orika_symbolmaster.m_nextSymbol,data_table.m_nextSymbol);
		CMTStr::Copy(m_st_Orika_symbolmaster.m_exchangeSymbol,data_table.m_exchangeSymbol);
		m_st_Orika_symbolmaster.m_nselotsize=data_table.m_nselotsize ;
		m_st_Orika_symbolmaster.m_mtlotsize=data_table.m_mtlotsize  ;
		CMTStr::Copy(m_st_Orika_symbolmaster.m_symbolGroup,data_table.m_symbolGroup);
		CMTStr::Copy(m_st_Orika_symbolmaster.m_ltpSymbol,data_table.m_ltpSymbol);
		
		
		CMTStr::Copy(m_st_Orika_symbolmaster.m_exchange, data_table.m_exchange);
		CMTStr::Copy(m_st_Orika_symbolmaster.m_international, data_table.m_international);
		CMTStr::Copy(m_st_Orika_symbolmaster.m_sector, data_table.m_sector);
		CMTStr::Copy(m_st_Orika_symbolmaster.m_industry, data_table.m_industry);
		m_st_Orika_symbolmaster.m_rmp= data_table.m_rmp;
		m_st_Orika_symbolmaster.m_QtyMulti= data_table.m_QtyMulti;
		CMTStr::Copy(m_st_Orika_symbolmaster.m_page, data_table.m_page);
		CMTStr::Copy(m_st_Orika_symbolmaster.m_categary, data_table.m_categary);
		CMTStr::Copy(m_st_Orika_symbolmaster.m_currencybase, data_table.m_currencybase);

		CMTStr::Copy(m_st_Orika_symbolmaster.m_Group, data_table.m_Group);

		CStaticClass::m_Orika_symbolmasterHastable.SetAt(strSymbol,m_st_Orika_symbolmaster);

		CStaticClass::st_Orika_ExchangeSymbol m_tempSt = {};
		CString StrExchangeSymbol = data_table.m_exchangeSymbol;
		CMTStr::Copy(m_tempSt.m_exchangeSymbol, data_table.m_exchangeSymbol);
		CMTStr::Copy(m_tempSt.m_MTsymbol, strSymbol);
		CMTStr::Copy(m_tempSt.m_NextMTsymbol, data_table.m_nextSymbol);
		CStaticClass::m_Orika_ExchangeSymbolHastable.SetAt(StrExchangeSymbol,m_tempSt);
	}
	m_tempSession.Close();
	
	//(L"U_Orderlock_62");
}


void CSqlData::getMappingSymbol(TMTArray<wchar_t[250]> &m_columns,CString m_loginuser,CString m_requestType)
{
	CString rval = L"";
	HRESULT hr = NULL;
	CCommand<CAccessor<CTableColumns>> data_table;
	if (!SUCCEEDED(hr))
	{
		return;
	}
	CSession m_tempSession;
	//(L"Enter In Symbol Mapping");
	m_tempSession.Open(CStaticClass::connection);
	CString strcommand = L"select [ColumnKey]  from loginColumnMapping where isnull(requestkey,'')='" + m_requestType + "' and [userlogin]='" + m_loginuser + "'  and subscribe=1";
	//(L"Enter In Symbol Mapping_2");
	hr = data_table.Open(m_tempSession, (LPCTSTR)strcommand);
	//(L"Enter In Symbol Mapping_3");
	if (FAILED(hr))
	{
		m_tempSession.Close();
		
		return;
	}
	CString  m_columnData = L"";
	

	while (hr = data_table.MoveNext() == S_OK)
	{		
		CString m_coulumns = data_table.m_column;
		CString m_loginColumnRequest = L"";
		m_loginColumnRequest.Format(L"%s:%s:%s:", m_loginuser, m_coulumns, m_requestType);
		
		wchar_t m_tmpdata[250];
		CMTStr::Copy(m_tmpdata, m_loginColumnRequest);
		m_columns.Add(&m_tmpdata);
		
	}
	data_table.Close();
	m_tempSession.Close();	
	//(L"Exit From Symbol Mapping");
	return;
}





void CSqlData::convertToDealFormat(CString str_data)
{		
	CString  m_login=L"";
	int		 m_deal=0 ;
	int      m_order=0 ;
	int      m_time=0 ;
	CString  m_symbol=L"";
	int		 m_state = 0;
	double    m_multiplyer=0 ;
	int      m_action=0;
	int      m_volume=0 ;
	double    m_price=0;

	double    m_clientBrokTotal =0;
	double    m_subBrokerBrokTotal=0; 
	double    m_brokerBrokTotal =0;
	double    m_extraGroupBrokTotal=0;
	double    m_comBalancebrokTotal=0;

	double    m_brokerVolume=0 ;
	double    m_subBrokerVolume=0 ;
	double    m_extraGroupVolume=0;
	double    m_companyVolume=0 ;

	double   m_LPvolume = 0;

	double    m_clientLots=0 ;
	double    m_subBrokerLots=0 ;
	double    m_brokerLots=0 ;
	double    m_extraGroupLots=0 ;
	double    m_companyLots=0;
	int      m_duplicate=0 ;
	CString  m_ExternalID=L"";
	CString  m_comment=L"";


	_bstr_t str_bstr_mesage=str_data;
	const char* json= str_bstr_mesage;       
	Document  document;  	
	if (!document.Parse<0>(json).HasParseError())
	{			
		Value& msg_message= document["MessageType"];		
		assert(msg_message.IsNumber()) ;

		int m_messageType=msg_message.GetInt();
		Value& msg_data= document["Data"];
		assert(msg_data.IsObject()) ;
		//Updating Trades
		if (m_messageType==1004||m_messageType==1006)
		{
			Value& msg_login= msg_data["login"];		
			assert(msg_login.IsInt64()) ;
			UINT64  reallogin=msg_login.GetInt64();
			 

			Value& msg_deal= msg_data["deal"];		
			assert(msg_deal.IsInt64()) ;
			m_deal=msg_deal.GetInt64();

			/*if (m_deal == 5500213)
			{
				int i = 0;
			}*/			
			Value& msg_order= msg_data["order"];		
			assert(msg_order.IsInt64()) ;
			m_order=msg_order.GetInt64();			
			

			Value& msg_type= msg_data["type"];		
			assert(msg_type.IsNumber()) ;
			m_action=msg_type.GetInt();			


			Value& msg_dealtime= msg_data["dealtime"];		
			assert(msg_dealtime.IsInt64()) ;
			m_time=msg_dealtime.GetInt64();						
			 


			Value& msg_Symbol= msg_data["Symbol"];		
			assert(msg_Symbol.IsString()) ;
			m_symbol=msg_Symbol.GetString();	
			 

			CString strKey = L"";
			CString strSymbol = L"";
			CStaticClass::m_symbolForTickData.Lookup(m_symbol, strSymbol);
			if (strSymbol == L"")
			{
				CStaticClass::m_mtmanager.SymbolSubscribeForTick(m_symbol);
				CString strlog = L"";
				strlog.Format(L"Symbol %s Subscribed For TickData", m_symbol);
				//(strlog);
				strSymbol = m_symbol;
				CStaticClass::m_symbolForTickData.SetAt(m_symbol, strSymbol);
			}


			Value& msg_price= msg_data["price"];		
			assert(msg_price.IsDouble()) ;
			m_price=msg_price.GetDouble();						
			

			Value& msg_External_ID= msg_data["External_ID"];		
			assert(msg_External_ID.IsString()) ;
			m_ExternalID=msg_External_ID.GetString();				 
			


			Value& msg_contract_Size= msg_data["contract_Size"];		
			assert(msg_contract_Size.IsNumber()) ;
			m_multiplyer=msg_contract_Size.GetInt();
			

			 Value& msg_comment= msg_data["comment"];		
			assert(msg_comment.IsString()) ;
			m_comment=msg_comment.GetString();	
			 

			Value& msg_volume= msg_data["volume"];		
			assert(msg_volume.IsInt64()) ;
			m_volume=msg_volume.GetInt64();
						
			//Calculating data
			//Getting Symbol Group Details
			CString m_symbolGroup=L"";
			CStaticClass::st_Orika_symbolmaster m_st_Orika_symbolmaster={};
			CStaticClass::m_Orika_symbolmasterHastable.Lookup(m_symbol,m_st_Orika_symbolmaster);
			m_symbolGroup=m_st_Orika_symbolmaster.m_symbolGroup;

			if (m_symbol.Find(L".P.") > 0 || m_symbol.Find(L".C.") > 0)
			{
				m_symbolGroup = L"option";
			}

			int m_nselotsize=m_st_Orika_symbolmaster.m_nselotsize;
			if (m_nselotsize==0)
			{
				m_nselotsize=1;
			}


			//End Getting Symbol Group Details

			

			////Going to Check and Update NPBPS
			//CString strLog = L"";
			//strLog.Format(L"Going to Check and Update NPBPS Status for Deal No %d", m_deal);
			////(strLog);
			//CStaticClass::m_mutex_order.Lock();

			//CStaticClass::st_symbolHighLowTimeWise m_stHL = {};
			//CStaticClass::m_SymbolHighLowTimeWiseArray.Lookup(m_symbol, m_stHL);
			//
			//double m_heigh = m_stHL.m_Heigh/100;
			//double m_low = m_stHL.m_Low/100;

			//strLog.Format(L"%s for Heigh:%.4lf and Low:%.4lf OrderPrice:%.4lf and OrderNo:%d", m_symbol, m_heigh, m_low, m_price, m_order);
			////(strLog);
			//if (m_price >= m_low && m_price <= m_heigh)
			//{
			//}
			//else
			//{
			//	if (m_heigh != 0 && m_low != 0)
			//	{ 
			//		CStaticClass::st_order  st_order = {};
			//		CStaticClass::m_Orika_orderHastable.Lookup(m_order, st_order);
			//		CMTStr::Copy(st_order.m_status, L"NPBPS");
			//		st_order.m_orderstate = 1009;
			//		CStaticClass::m_Orika_orderHastable.SetAt(m_order, st_order);
			//		strLog.Format(L"Going to  Update NPBPS Status in History DataBase");
			//		//(strLog);
			//		CStaticClass::m_mtmanager.UpdateOrderHistoryINMT(m_order, L"ExternalID", L"NPBPS");
			//		strLog.Format(L"Updated NPBPS Status in History DataBase");
			//		//(strLog);
			//		CStaticClass::m_mutex_ClientList.Lock();

			//		POSITION pos = CStaticClass::m_ClientContext.GetStartPosition();
			//		while (pos != NULL)
			//		{
			//			CString strclientkey = L"";
			//			CStaticClass::st_ClientContext m_st = {};
			//			CStaticClass::m_ClientContext.GetNextAssoc(pos, strclientkey, m_st);
			//			if ((m_st.m_FETCH_ORDER_DATA_start == 1))
			//			{
			//				m_st.m_orderarray_update.Add(&st_order);
			//			}
			//			CStaticClass::m_ClientContext.SetAt(strclientkey, m_st);
			//		}
			//		CStaticClass::m_mutex_ClientList.Unlock();
			//	}
			//}

			//CStaticClass::m_mutex_order.Unlock();
			////End of Checking NPBPS

			//strLog.Format(L"Updated NPBPS _2");
			////(strLog);



			CString realLoginKey=L"";
			realLoginKey.Format(L"%I64u",reallogin);
			//Getting login details Details
			CStaticClass::st_Orika_clientmaster  m_st_Orika_clientmaster={};

			
			CStaticClass::m_Orika_clientmasterHastable.Lookup(realLoginKey,m_st_Orika_clientmaster); 

			int m_commentYN=m_st_Orika_clientmaster.m_comment;

			if (m_commentYN==1)
			{
				m_login.Format(L"%I64u-%s",reallogin,m_comment);
			}
			else
			{
				m_login.Format(L"%I64u",reallogin);
			}



			CString m_loginSymbolKey=L"";
			m_loginSymbolKey.Format(L"%s:%s",m_login,m_symbol);

			CString m_loginSymbolgroupKey=L"";
			m_loginSymbolgroupKey.Format(L"%s:%s",m_login,m_symbolGroup);




			CString loginKey=L"";
			loginKey.Format(L"%s:",m_login);

			//End calculating Data

			CStaticClass::st_Orika_clientbrokerage m_st_Orika_clientbrokerage={};
			CStaticClass::m_Orika_clientbrokerageHastable.Lookup(m_loginSymbolgroupKey,m_st_Orika_clientbrokerage);
			if (m_st_Orika_clientbrokerage.m_clientBrokage==0)
			{
				CStaticClass::m_Orika_clientbrokerageHastable.Lookup(loginKey,m_st_Orika_clientbrokerage);
			}

			int     m_brokageType=m_st_Orika_clientbrokerage.m_brokageType;
			double   m_clientBrokageRate=m_st_Orika_clientbrokerage.m_clientBrokage;

			if (m_clientBrokageRate>0)
			{
				int i=0;
			}

			double   m_subBrokerBrokageRate=m_st_Orika_clientbrokerage.m_subBrokerBrokage; 
			double   m_bokerBrokageRate=m_st_Orika_clientbrokerage.m_bokerBrokage;
			double   m_m_extraGroupBrokageRate=m_st_Orika_clientbrokerage.m_extraGroupBrokage; 
			double   m_companyBrokageRate=m_st_Orika_clientbrokerage.m_companyBrokage;


			double real_volume=double(m_volume)/10000;


			//Ignoring Calculation of Saturday Brokergae
			SYSTEMTIME readable_date;
			readable_date=SMTTime::TimeToST(m_time,readable_date);
			int int_dayofweek=readable_date.wDayOfWeek;
			int time_hour = readable_date.wHour;

			if (int_dayofweek == 0 || (int_dayofweek == 6 && time_hour > 10))
			{
			}
			else			
			{
				if (m_brokageType==0)
				{
				
					m_clientBrokTotal=(real_volume*m_price*m_multiplyer)*m_clientBrokageRate/100 ;
					m_subBrokerBrokTotal=(real_volume*m_price*m_multiplyer)*m_subBrokerBrokageRate/100 ; 
					m_brokerBrokTotal =(real_volume*m_price*m_multiplyer)*m_bokerBrokageRate/100 ;
					m_extraGroupBrokTotal=(real_volume*m_price*m_multiplyer)*m_m_extraGroupBrokageRate/100 ;
					m_comBalancebrokTotal=(real_volume*m_price*m_multiplyer)*m_companyBrokageRate/100 ;
				}
				if (m_brokageType==1)
				{
					double volume_lot=real_volume/m_nselotsize;
					m_clientBrokTotal=volume_lot*m_clientBrokageRate;
					m_subBrokerBrokTotal=volume_lot*m_subBrokerBrokageRate; 
					m_brokerBrokTotal =volume_lot*m_bokerBrokageRate;
					m_extraGroupBrokTotal=volume_lot*m_m_extraGroupBrokageRate;
					m_comBalancebrokTotal=volume_lot*m_companyBrokageRate;
				}
			}


			//Updating Total Brokerage
			CStaticClass::st_orika_brokerageLoginSymbolWise  st_pac = {};
			CStaticClass::m_mutex_TotalBrokerage.Lock();
				CStaticClass::m_brokerageLoginSymbolWiseHastable.Lookup(m_loginSymbolgroupKey, st_pac);			
				st_pac.m_clientBrokTotal = st_pac.m_clientBrokTotal+ m_clientBrokTotal;
				st_pac.m_subBrokerBrokTotal = st_pac.m_subBrokerBrokTotal+ m_subBrokerBrokTotal;
				st_pac.m_brokerBrokTotal = st_pac.m_brokerBrokTotal+ m_brokerBrokTotal;
				st_pac.m_comBrokTotal = st_pac.m_comBrokTotal+ m_comBalancebrokTotal;
				CStaticClass::m_brokerageLoginSymbolWiseHastable.SetAt(m_loginSymbolgroupKey, st_pac);
			CStaticClass::m_mutex_TotalBrokerage.Unlock();
			//Updating Total Brokerage

			CStaticClass::st_Orika_PLDevideRatio m_st_Orika_PLDevideRatio={};
			CStaticClass::m_Orika_PLDevideRatioHastable.Lookup(m_loginSymbolgroupKey,m_st_Orika_PLDevideRatio);
			if (m_st_Orika_PLDevideRatio.m_brokerPLRatio==0)
			{
				CStaticClass::m_Orika_PLDevideRatioHastable.Lookup(loginKey,m_st_Orika_PLDevideRatio);
			}


			double m_brokerPLRatioRate=m_st_Orika_PLDevideRatio.m_brokerPLRatio ;
			double m_subBrokerPLRatioRate=m_st_Orika_PLDevideRatio.m_subBrokerPLRatio ;
			double m_extragroupPLRatioRate=m_st_Orika_PLDevideRatio.m_extragroupPLRatio ;
			double m_companyPLRatioRate=m_st_Orika_PLDevideRatio.m_companyPLRatio ;



			m_brokerVolume=m_volume*m_brokerPLRatioRate/100 ;
			m_subBrokerVolume=m_volume*m_subBrokerPLRatioRate/100 ;
			m_extraGroupVolume=m_volume*m_extragroupPLRatioRate/100;
			m_companyVolume=m_volume*m_companyPLRatioRate/100 ;


			
				CStaticClass::m_Orika_clientmasterHastable.Lookup(m_login , m_st_Orika_clientmaster);
				double m_LPration = 0;
				m_LPration = m_st_Orika_clientmaster.m_lpRatio;
			



			m_LPvolume = m_companyVolume * m_LPration / 100;


			m_clientLots=m_volume/m_nselotsize ;
			m_subBrokerLots=m_brokerVolume/m_nselotsize;
			m_brokerLots=m_subBrokerVolume/m_nselotsize ;
			m_extraGroupLots =m_extraGroupVolume/m_nselotsize;
			m_companyLots=m_companyVolume/m_nselotsize;						
			m_duplicate=0; ;

			CStaticClass::st_Orika_dealtableAccounting m_st_Orika_dealtableAccounting={};
			
			CMTStr::Copy(m_st_Orika_dealtableAccounting.m_login,m_login);
			m_st_Orika_dealtableAccounting.m_deal=m_deal ;
			m_st_Orika_dealtableAccounting.m_order=m_order ;
			m_st_Orika_dealtableAccounting.m_time=m_time ;
			CMTStr::Copy(m_st_Orika_dealtableAccounting.m_symbol,m_symbol);
			m_st_Orika_dealtableAccounting.m_multiplyer=m_multiplyer ;
			m_st_Orika_dealtableAccounting.m_action=m_action;
			m_st_Orika_dealtableAccounting.m_volume=m_volume ;
			m_st_Orika_dealtableAccounting.m_price=m_price;

			m_st_Orika_dealtableAccounting.m_clientBrokTotal=m_clientBrokTotal ;
			m_st_Orika_dealtableAccounting.m_subBrokerBrokTotal=m_subBrokerBrokTotal; 
			m_st_Orika_dealtableAccounting.m_brokerBrokTotal=m_brokerBrokTotal ;
			m_st_Orika_dealtableAccounting.m_extraGroupBrokTotal=m_extraGroupBrokTotal;
			m_st_Orika_dealtableAccounting.m_comBalancebrokTotal=m_comBalancebrokTotal;

			m_st_Orika_dealtableAccounting.m_brokerVolume=m_brokerVolume  ;
			m_st_Orika_dealtableAccounting.m_subBrokerVolume=m_subBrokerVolume ;
			m_st_Orika_dealtableAccounting.m_extraGroupVolume=m_extraGroupVolume;
			m_st_Orika_dealtableAccounting.m_companyVolume=m_companyVolume ;

			m_st_Orika_dealtableAccounting.m_clientLots=m_clientLots ;
			m_st_Orika_dealtableAccounting.m_subBrokerLots=m_subBrokerLots ;
			m_st_Orika_dealtableAccounting.m_brokerLots=m_brokerLots ;
			m_st_Orika_dealtableAccounting.m_extraGroupLots=m_extraGroupLots ;
			m_st_Orika_dealtableAccounting.m_companyLots=m_companyLots;
			m_st_Orika_dealtableAccounting.m_duplicate=m_duplicate ;
			CMTStr::Copy(m_st_Orika_dealtableAccounting.m_ExternalID,m_ExternalID);

			CMTStr::Copy(m_st_Orika_dealtableAccounting.m_comment,m_comment);

			CStaticClass m_staticclass;
			CStaticClass::st_TradesHighLowPriceSymbolWise m_st_TradesHighLowPriceSymbolWise = {};
			CString strSymbolDateKey = L"";			
			SYSTEMTIME readable_DealDate;
			readable_DealDate = SMTTime::TimeToST(m_time, readable_DealDate);
		    CString str_TradeDate = m_staticclass.sysOnlyDateToStringFormat(readable_DealDate);
			strSymbolDateKey.Format(L"%s:%s",m_symbol, str_TradeDate);
			CStaticClass::TradesHighLowLockUpdate.Lock();
				CStaticClass::m_TradesHighLowPriceSymbolWise.Lookup(strSymbolDateKey, m_st_TradesHighLowPriceSymbolWise);
				if (m_st_TradesHighLowPriceSymbolWise.m_High >= m_price && m_st_TradesHighLowPriceSymbolWise.m_Low <= m_price)
				{
				}
				else
				{
					if (m_price > m_st_TradesHighLowPriceSymbolWise.m_High)
					{
						m_st_TradesHighLowPriceSymbolWise.m_High = m_price;
					}
					if (m_price < m_st_TradesHighLowPriceSymbolWise.m_Low)
					{
						m_st_TradesHighLowPriceSymbolWise.m_Low = m_price;
					}
					CStaticClass::m_TradesHighLowPriceSymbolWise.SetAt(strSymbolDateKey, m_st_TradesHighLowPriceSymbolWise);
				}
			CStaticClass::TradesHighLowLockUpdate.Unlock();



			
			CString strLoginDealKey=L"";
			strLoginDealKey.Format(L"%s:%I64u",m_login,m_deal);
			CStaticClass::m_mutex_deal.Lock();
				CStaticClass::m_Orika_dealtableAccountingHastable.SetAt(strLoginDealKey,m_st_Orika_dealtableAccounting);
			CStaticClass::m_mutex_deal.Unlock();
			m_staticclass.Sql_Updateorika_dealtableaccounting(m_st_Orika_dealtableAccounting);
			m_staticclass.Updateorika_positionEntryAccountingHashtable(m_st_Orika_dealtableAccounting);
			
			

			CStaticClass::m_mutex_ClientList.Lock();
			//(L"116");
				POSITION pos = CStaticClass::m_ClientContext.GetStartPosition();		
				while (pos != NULL) 
				{
					CString strclientkey=L"";		
					CStaticClass::st_ClientContext m_st={};
					CStaticClass::m_ClientContext.GetNextAssoc(pos, strclientkey,m_st);
					if(m_st.m_FETCH_DEALING_DATA_start==1) 
					{
						m_st.m_dealarray.Add(&m_st_Orika_dealtableAccounting);
						CStaticClass::st_updatedSymbolLogin m_stUpdated={};
						CMTStr::Copy(m_stUpdated.m_login,m_st_Orika_dealtableAccounting.m_login );
						CMTStr::Copy(m_stUpdated.m_symbol ,m_st_Orika_dealtableAccounting.m_symbol );
						m_st.m_updatedVolumeSymbolArray.Add(&m_stUpdated);
					}
										
					CStaticClass::m_ClientContext.SetAt(strclientkey,m_st);
				}
			CStaticClass::m_mutex_ClientList.Unlock();
			//(L"U116");



			if (m_commentYN==1)
			{
				CString strreallogin=L"";
				strreallogin.Format(L"%I64u",reallogin);
				CMTStr::Copy(m_st_Orika_dealtableAccounting.m_login,strreallogin);
				strLoginDealKey.Format(L"%I64u:%I64u",reallogin,m_deal);
				m_st_Orika_dealtableAccounting.m_duplicate=1;
				CStaticClass::m_mutex_deal.Lock();
					CStaticClass::m_Orika_dealtableAccountingHastable.SetAt(strLoginDealKey,m_st_Orika_dealtableAccounting);
				CStaticClass::m_mutex_deal.Unlock();
				m_staticclass.Sql_Updateorika_dealtableaccounting(m_st_Orika_dealtableAccounting);
				m_staticclass.Updateorika_positionEntryAccountingHashtable(m_st_Orika_dealtableAccounting);
			}
			//Adding new deal for Position and average Update
			m_staticclass.addNewDealToDealData(m_st_Orika_dealtableAccounting);		
			CString strReturnVal = L"";
			CString m_group = m_st_Orika_symbolmaster.m_Group;

			double m_TotalLpVolume = 0;
			int  LpVolumeTotalIndex = -1;
			CStaticClass::LpVolumeTotalArrayIndex.Lookup(m_group, LpVolumeTotalIndex);
			if (LpVolumeTotalIndex >= 0)
			{
				m_TotalLpVolume=*CStaticClass::LpVolumeTotal[LpVolumeTotalIndex];
			}					


			double m_totalComVolumeAfterMulti = 0;
			int  CompanyVolumeAfterMultiTotalIndex = -1;
			CStaticClass::clientnettotalArrayIndex.Lookup(m_group, CompanyVolumeAfterMultiTotalIndex);
			if (CompanyVolumeAfterMultiTotalIndex >= 0)
			{
				m_totalComVolumeAfterMulti=*CStaticClass::CompanyVolumeAfterMultiTotalArray[CompanyVolumeAfterMultiTotalIndex];
			}
			CString m_symbolCheck = m_symbol;
			if (m_group==L"GOLD"|| m_group == L"SILVER")
			{
				strReturnVal = m_staticclass.CalculateLPData(m_login, m_group, m_TotalLpVolume, m_totalComVolumeAfterMulti);
				string StringstrReturnVal = string(CT2CA(strReturnVal));
				m_staticclass.sendDataToAllClient_LPData(StringstrReturnVal);

				CString strApiJason = L"";
				strApiJason = CStaticClass::m_mtmanager.GetGATEWAY_PERFECTANDROUNDED_API();				
				string StringReturnValApi= string(CT2CA(strApiJason));
				m_staticclass.sendDataToAllAPIClient_LPData(StringReturnValApi);
			}
			
		}	


		///Updating Orders


		if (m_messageType==1001||m_messageType==1002 ||m_messageType==1003 ||m_messageType==1005)
		{
			m_deal=0;
			Value& msg_login= msg_data["login"];		
			assert(msg_login.IsInt64()) ;
			UINT64  reallogin=msg_login.GetInt64();			 						


			Value& msg_order= msg_data["order"];		
			assert(msg_order.IsInt64()) ;
			m_order=msg_order.GetInt64();			
			

			Value& msg_type= msg_data["type"];		
			assert(msg_type.IsNumber()) ;
			m_action=msg_type.GetInt();			


			Value& msg_dealtime= msg_data["Ordertime"];		
			assert(msg_dealtime.IsInt64()) ;
			m_time=msg_dealtime.GetInt64();						
			 


			Value& msg_Symbol= msg_data["Symbol"];		
			assert(msg_Symbol.IsString()) ;
			m_symbol=msg_Symbol.GetString();	
			 



			Value& msg_State = msg_data["state"];
			assert(msg_State.IsNumber());
			m_state = msg_State.GetInt();
			


			Value& msg_price= msg_data["price_order"];		
			assert(msg_price.IsDouble()) ;
			m_price=msg_price.GetDouble();						
			

			Value& msg_External_ID= msg_data["External_ID"];		
			assert(msg_External_ID.IsString()) ;
			m_ExternalID=msg_External_ID.GetString();				 
			


			Value& msg_contract_Size= msg_data["contract_Size"];		
			assert(msg_contract_Size.IsNumber()) ;
			m_multiplyer=msg_contract_Size.GetInt();
			

			 Value& msg_comment= msg_data["comment"];		
			assert(msg_comment.IsString()) ;
			m_comment=msg_comment.GetString();	
			 

			Value& msg_volume= msg_data["volume"];		
			assert(msg_volume.IsInt64()) ;
			m_volume=msg_volume.GetInt64();
			double real_volume = double(m_volume) / 10000;

			CStaticClass::st_order st={};
			CString strlogin=L"";
			strlogin.Format(L"%I64u",reallogin);
			CMTStr::Copy(st.m_login,strlogin);
			st.m_time=m_time  ;
			st.m_deal=m_deal ;
			st.m_order=m_order;
			CMTStr::Copy(st.m_symbol,m_symbol);
			st.m_type=m_action  ;
			st.m_volume= real_volume;
			st.m_price=m_price ;
			CMTStr::Copy(st.m_comment,m_comment);
			CMTStr::Copy(st.m_status,m_ExternalID);
			st.m_select=0  ;
			CMTStr::Copy(st.m_selecttype,L"");
			CMTStr::Copy(st.m_subtype,L"");
			st.m_contraorder=0 ;
			st.m_tradeexecutetime=0 ;
			CMTStr::Copy(st.m_ourcomment,L"");
		    st.m_orderstate=m_messageType ;


			CStaticClass m_staticclass;

			/*CStaticClass::m_mutex_order.Lock();

			CString StrPrintLino=L"";
			StrPrintLino.Format(L"m_mutex_order Locked(%d)",__LINE__);
			////(StrPrintLino);



			m_staticclass.sendNeworderToClient(st);
			CStaticClass::m_mutex_order.Unlock();*/

			if (m_messageType==1001 || m_messageType==1002)
			{			
				CStaticClass::m_mutex_order.Lock();
				//(L"L116");
				
				CStaticClass::m_Orika_orderHastable.SetAt(m_order,st);
				CStaticClass::m_mutex_order.Unlock();

				//(L"UL116");
				CStaticClass::m_mutex_ClientList.Lock();
				//(L"109");
				POSITION pos = CStaticClass::m_ClientContext.GetStartPosition ();		
				while (pos != NULL) 
				{
					CString strclientkey=L"";		
					CStaticClass::st_ClientContext m_st={};
					CStaticClass::m_ClientContext.GetNextAssoc(pos, strclientkey,m_st);
					//if((m_st.m_FETCH_ORDER_DATA_start==1||m_st.m_FETCH_DEALING_DATA_start==1) && m_messageType==1001) 
					if((m_st.m_FETCH_ORDER_DATA_start==1) && m_messageType==1001) 
					{
						m_st.m_orderarray_insert.Add(&st);
					}
					if((m_st.m_FETCH_DEALING_DATA_start==1) && m_messageType==1001) 
					{
						m_st.m_orderarray_dealing_insert.Add(&st);
					}
					//if((m_st.m_FETCH_ORDER_DATA_start==1 ||m_st.m_FETCH_DEALING_DATA_start ==1) && m_messageType==1002) 
					if((m_st.m_FETCH_ORDER_DATA_start==1 ) && m_messageType==1002) 
					{
						m_st.m_orderarray_update.Add(&st);
					}	
					if((m_st.m_FETCH_DEALING_DATA_start ==1 ) && m_messageType==1002) 
					{
						m_st.m_orderarray_dealing_update.Add(&st);
					}
					CStaticClass::m_ClientContext.SetAt(strclientkey,m_st);
				}
				CStaticClass::m_mutex_ClientList.Unlock();
				//(L"U109");
				
			}
			if (m_messageType==1003 )
			{
				CString strLog = L"";
				/*strLog.Format(L"Going to Check PBDU Status For Order No:%d its status is %s", m_order, m_ExternalID);
				//(strLog);
				//(L"lock_1");*/
				CStaticClass::m_mutex_order.Lock();	
				//(L"Orderlock_1");
				int CheckOrderNo = 0;
				int CheckDealNo = 0;
				CStaticClass::m_OrikaOrderdealNO.Lookup(CheckOrderNo, CheckDealNo);

				/*strLog.Format(L"Deal for Order %d is %d", m_order, CheckDealNo);
				//(strLog);*/
				CString m_ExtarnalIDForCheck = L"";
				CString m_UpdateRateAndTime = L"";
				CStaticClass::m_logfile.LogEvent(L"Test_18");
				if (m_ExternalID.GetLength() >= 5)
				{
					m_ExtarnalIDForCheck = m_ExternalID.Mid(0, 5);
					m_UpdateRateAndTime = m_ExternalID.Mid(5, m_ExternalID.GetLength() - 5);
				}
				CStaticClass::m_logfile.LogEvent(L"End Test_18");
				if (m_ExtarnalIDForCheck == L"PBNPS" && CheckDealNo == 0 && m_state==2)
				{
					strLog.Format(L"Going to Update PBDU Status For Order No:%d", m_order);
					//(strLog);
					CStaticClass::st_order  st_order = {};
					CStaticClass::m_Orika_orderHastable.Lookup(m_order, st_order);

					CString m_updateString = L"PBDU "+ m_UpdateRateAndTime;

					CMTStr::Copy(st_order.m_status, m_updateString);
					st_order.m_orderstate = 1009;
					CStaticClass::m_Orika_orderHastable.SetAt(m_order, st_order);
					st.m_orderstate = 1009;
					CMTStr::Copy(st.m_status, m_updateString);
					CStaticClass::m_mtmanager.UpdateOrderHistoryINMT(m_order, L"ExternalID", m_updateString);

					strLog.Format(L"PBDU Status Has Been Updated For Order No:%d", m_order);
					//(strLog);

					CStaticClass::m_mutex_ClientList.Lock();

					POSITION pos = CStaticClass::m_ClientContext.GetStartPosition();
					while (pos != NULL)
					{
						CString strclientkey = L"";
						CStaticClass::st_ClientContext m_st = {};
						CStaticClass::m_ClientContext.GetNextAssoc(pos, strclientkey, m_st);
						if ((m_st.m_FETCH_ORDER_DATA_start == 1))
						{
							m_st.m_orderarray_update.Add(&st);
						}
						CStaticClass::m_ClientContext.SetAt(strclientkey, m_st);
					}
					CStaticClass::m_mutex_ClientList.Unlock();
				}
				else
				{
					CStaticClass::m_Orika_orderHastable.RemoveKey(m_order);
					CStaticClass::m_mutex_ClientList.Lock();

					POSITION pos = CStaticClass::m_ClientContext.GetStartPosition();
					while (pos != NULL)
					{
						CString strclientkey = L"";
						CStaticClass::st_ClientContext m_st = {};
						CStaticClass::m_ClientContext.GetNextAssoc(pos, strclientkey, m_st);
						if ((m_st.m_FETCH_ORDER_DATA_start == 1))
						{
							m_st.m_orderarray_delete.Add(&st);
						}
						CStaticClass::m_ClientContext.SetAt(strclientkey, m_st);
					}
					CStaticClass::m_mutex_ClientList.Unlock();
				}
				CStaticClass::m_mutex_order.Unlock();
				//(L"U_Orderlock_1");
				

				
				
			}
			
			m_staticclass.Sql_UpdateOrika_order(st);
		}

	}
}






CString CSqlData::generateJsonCommentChangeData()
{
	CString m_allData=L"";
	{
	CString rval=L"";
	HRESULT hr=NULL ;
	CCommand<CAccessor<CCommentChangeData>> data_table;								
	if(!SUCCEEDED(hr))
	{
		return L"" ;
	}
	CString   strCommand=L"select deal,comment,commentTo from orika_CommentChangeDeals where changedStatus='1';";	
	
	CSession m_tempSession;
	


	m_tempSession.Open(CStaticClass::connection);
	hr=data_table.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		m_tempSession.Close();
		
		return L"";
	}
	int  m_deal;
	CString  m_comment;
	CString  m_commentto;	
	int ccount=0;
	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key("type");
	writer.String("EXISTING_COMMENT_CHANGE_DATA");	
	writer.Key("insert");
	writer.StartArray();
	while(hr=data_table.MoveNext()==S_OK)
	{			
		m_deal=data_table.m_deal ;
		m_comment=data_table.m_comment;
		m_commentto=data_table.m_commentTo ;
		
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
	m_tempSession.Close();
	
	//(L"U_Orderlock_3");
	writer.EndArray();
	writer.EndObject();
	rval=s.GetString();
	return rval;
}
}

void CSqlData::Loadloginuser()
{
	CString rval = L"";
	HRESULT hr = NULL;
	CCommand<CAccessor<CLoginTable>> data_table;
	if (!SUCCEEDED(hr))
	{
		return ;
	}
	CString   strCommand = L"select Orika_userlogin.userlogin as 'loginUser', isnull(Name,'') as 'Name', orika_viewLoginWiseGroup.groups as 'group', isnull(orika_viewLoginWiseSeialNumber.serialNumbers,'') as 'serialNo', '' as 'loginStatus' from Orika_userlogin  left outer join orika_viewLoginWiseSeialNumber on orika_viewLoginWiseSeialNumber.userlogin=Orika_userlogin.userlogin left outer join orika_viewLoginWiseGroup on orika_viewLoginWiseGroup.userlogin=Orika_userlogin.userlogin;";

	CSession m_tempSession;	

	m_tempSession.Open(CStaticClass::connection);
	hr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
	if (FAILED(hr))
	{
		m_tempSession.Close();		
		return;
	}
	CString  m_loginUser;
	CString  m_Name;
	CString  m_group;
	CString  m_serialNo;
	CString  m_loginStatus;

	CString  m_silgleData = L"";
	CString  m_allData = L"";
	int ccount = 0;
	while (hr = data_table.MoveNext() == S_OK)
	{
		m_loginUser = data_table.m_loginUser;
		m_Name = data_table.m_Name;
		m_group = data_table.m_group;
		m_serialNo = data_table.m_serialNo;
		m_loginStatus = data_table.m_loginStatus;
		CStaticClass::stloginUserDetail m_st = {};
		CMTStr::Copy(m_st.m_loginuser, m_loginUser);
		CMTStr::Copy(m_st.m_name, m_Name);
		CStaticClass::m_loginuserlist.SetAt(m_loginUser, m_st);
	}
	m_tempSession.Close();
	
	//(L"U_Orderlock_4");	
}

CString CSqlData::generateJsonLoginData()
{
	CString	m_rval=L"";
	CString  m_loginUser;
	CString  m_Name;
	CString  m_group;
	CString  m_fingerprint;
	CString  m_loginStatus;
	CString m_ipaddress = L"";
	CString  m_silgleData = L"";
	CString  m_allData = L"";
	int ccount = 0;

	POSITION pos_01 = CStaticClass::m_loginuserlist.GetStartPosition();
	while (pos_01 != NULL)
	{
		CString m_strloginuser = L"";
		CStaticClass::stloginUserDetail m_stuser = {};
		CStaticClass::m_loginuserlist.GetNextAssoc(pos_01, m_strloginuser, m_stuser);
		m_loginUser = m_strloginuser;
		m_Name = m_stuser.m_name;
		m_silgleData.Format(L"{\"loginuser\":\"%s\",\"name\":\"%s\",\"fingerprint\":\"\",\"status\":\"\",\"ipaddress\":\"\",\"sessionid\":\"\"}", m_loginUser, m_Name);
		if (ccount == 0)
		{
			m_allData = m_silgleData;
		}
		else
		{
			m_allData = m_allData + L"," + m_silgleData;
		}
		ccount = 1;
	}
	CString m_SessionId = L"";
	CStaticClass::m_mutex_ClientList.Lock();
	POSITION pos = CStaticClass::m_ClientContext.GetStartPosition();
	while (pos != NULL)
	{
		CString strclientkey = L"";
		CStaticClass::st_ClientContext m_st = {};
		CStaticClass::m_ClientContext.GetNextAssoc(pos, strclientkey, m_st);
		m_loginUser = m_st.m_userlogin;	
		CStaticClass::stloginUserDetail m_stuser = {};
		CStaticClass::m_loginuserlist.Lookup(m_loginUser, m_stuser);
		m_Name = m_stuser.m_name;
		m_ipaddress = m_st.m_ipaddress;
		//m_ipaddress = m_ipaddress.Mid(0, m_ipaddress.Find(L":"));
		m_fingerprint = m_st.m_fingerprint;		
		m_loginStatus = L"Online";
		m_SessionId = strclientkey;
		//int m_threadid = m_st.m_clientConnection->s;
		/*if (m_Name.Trim() == L"")
		{
			CStaticClass::m_ClientContext.RemoveKey(strclientkey);
		}*/
		if (m_Name.Trim() != L"")
		{
			m_silgleData.Format(L"{\"loginuser\":\"%s\",\"name\":\"%s\",\"fingerprint\":\"%s\",\"status\":\"%s\",\"ipaddress\":\"%s\",\"sessionid\":\"%s\"}", m_loginUser, m_Name, m_fingerprint, m_loginStatus, m_ipaddress, m_SessionId);
			if (ccount == 0)
			{
				m_allData = m_silgleData;
			}
			else
			{
				m_allData = m_allData + L"," + m_silgleData;
			}
			ccount = 1;
		}
	}

	/*size_t size = sizeof(CStaticClass::m_ClientContext);
	CString str_logfile = L"";

	str_logfile.Format(L"Size of m_ClientContex: %u bytes", size);

	*/
	CStaticClass::m_mutex_ClientList.Unlock();
		
	m_rval.Format(L"{\"type\": \"USERS_DATA\",\"users\": [%s]}", m_allData);
	return m_rval;
}



CString CSqlData::generateGroupJason(CString loginuser)
{
	CString rval = L"";
	HRESULT hr = NULL;

	struct st_group
	{
		wchar_t m_group[200];
		int		m_select;
	};
	TMTArray<st_group> m_grouplist;		
	//CGroupTable
	CSession m_tempSession;
	CString   strSelectGroupCommand = L"";

	CCommand<CAccessor<CGroupTable>> m_groupTable;
	if (!SUCCEEDED(hr))
	{
		return L"";
	}
	strSelectGroupCommand.Format(L"getloginusergroups '%s'", loginuser);
	m_tempSession.Open(CStaticClass::connection);
	hr = m_groupTable.Open(m_tempSession, (LPCTSTR)strSelectGroupCommand);
	if (FAILED(hr))
	{
		m_tempSession.Close();
		return L"";
	}
	CString  m_clientgroup;
	int		 m_select;
	while (hr = m_groupTable.MoveNext() == S_OK)
	{
		m_clientgroup = m_groupTable.m_group;
		m_select = m_groupTable.m_select;
		st_group m_st_group = {};
		CMTStr::Copy(m_st_group.m_group, m_clientgroup);
		m_st_group.m_select = m_select;
		m_grouplist.Add(&m_st_group);
	}
	m_tempSession.Close();
	
	StringBuffer s;
	Writer<StringBuffer> writer(s);	
	writer.StartObject();
	writer.Key("type");
	writer.String("ALL_GROUP_DATA");
	writer.Key("groups");	
	writer.StartArray();
	int datacount = m_grouplist.Total();
	for (int i = 0; i < datacount; i++)
	{		
		st_group m_tmp = {};
		m_tmp = m_grouplist[i];
		CString m_selectGroup = m_tmp.m_group;				
		string ssselectedgroup = string(CT2CA(m_selectGroup));
		const char* stelectedgroup = ssselectedgroup.c_str();
		writer.String(stelectedgroup);		
	}
	writer.EndArray();
			
	writer.EndObject();	
	rval = s.GetString();
	s.Clear();
	writer.Flush();
	return rval;
}

CString CSqlData::generateJsonLoginData_Details(CString loginuser)
{	
	CString rval = L"";
	HRESULT hr = NULL;
	
	struct st_group
	{
		wchar_t m_group[200];		
		int		m_select;
	};
	TMTArray<st_group> m_grouplist;

	TMTArray<st_group> m_tablecolumnlist;


	struct st_permission
	{
		wchar_t m_accessControlName[200];
		int		m_select;
	};
	TMTArray<st_permission> m_permissionlist;
	//CGroupTable
	CSession m_tempSession;
	CString   strSelectGroupCommand = L"";

	CCommand<CAccessor<CGroupTable>> m_groupTable;
	if (!SUCCEEDED(hr))
	{
		return L"";
	}
	strSelectGroupCommand.Format(L"getloginusergroups '%s'", loginuser);
	m_tempSession.Open(CStaticClass::connection);
	hr = m_groupTable.Open(m_tempSession, (LPCTSTR)strSelectGroupCommand);
	if (FAILED(hr))
	{
		m_tempSession.Close();		
		return L"";
	}
	CString  m_clientgroup;
	int		 m_select;
	
	while (hr = m_groupTable.MoveNext() == S_OK)
	{
		m_clientgroup = m_groupTable.m_group;
		m_select = m_groupTable.m_select;
		st_group m_st_group = {};
		CMTStr::Copy(m_st_group.m_group, m_clientgroup);
		m_st_group.m_select = m_select;
		m_grouplist.Add(&m_st_group);
	}
	m_tempSession.Close();



	CCommand<CAccessor<CGroupTable>> m_tableColumn;
	strSelectGroupCommand.Format(L"getloginuserTableColumns '%s'", loginuser);
	m_tempSession.Open(CStaticClass::connection);
	hr = m_tableColumn.Open(m_tempSession, (LPCTSTR)strSelectGroupCommand);
	if (FAILED(hr))
	{
		m_tempSession.Close();
		return L"";
	}
	 m_clientgroup=L"";
	 m_select=0;

	while (hr = m_tableColumn.MoveNext() == S_OK)
	{
		m_clientgroup = m_tableColumn.m_group;
		m_select = m_tableColumn.m_select;
		st_group m_st_group = {};
		CMTStr::Copy(m_st_group.m_group, m_clientgroup);
		m_st_group.m_select = m_select;
		m_tablecolumnlist.Add(&m_st_group);
	}
	m_tempSession.Close();










	CCommand<CAccessor<CGroupTable>> m_permissionTable;
	if (!SUCCEEDED(hr))
	{
		return L"";
	}
	strSelectGroupCommand.Format(L"getloginuserpermission '%s'", loginuser);
	m_tempSession.Open(CStaticClass::connection);
	hr = m_permissionTable.Open(m_tempSession, (LPCTSTR)strSelectGroupCommand);
	if (FAILED(hr))
	{
		m_tempSession.Close();		
		return L"";
	}
	CString  m_accessControlName;
	while (hr = m_permissionTable.MoveNext() == S_OK)
	{
		m_accessControlName = m_permissionTable.m_group;
		m_select = m_permissionTable.m_select;
		st_permission m_st_permission = {};
		CMTStr::Copy(m_st_permission.m_accessControlName, m_accessControlName);
		m_st_permission.m_select = m_select;
		m_permissionlist.Add(&m_st_permission);
	}
	m_tempSession.Close();




	CCommand<CAccessor<CLoginTable>> data_table;								
	if(!SUCCEEDED(hr))
	{
		return L"" ;
	}
	CString   strSelectCommand = L"select Orika_userlogin.userlogin as 'loginUser', isnull(Name,'') as 'Name', '' as 'group', isnull(orika_viewLoginWiseSeialNumber.serialNumbers,'') as 'serialNo', '' as 'loginStatus' from Orika_userlogin  left outer join orika_viewLoginWiseSeialNumber on orika_viewLoginWiseSeialNumber.userlogin=Orika_userlogin.userlogin left outer join orika_viewLoginWiseGroup on orika_viewLoginWiseGroup.userlogin=Orika_userlogin.userlogin where Orika_userlogin.userlogin='" + loginuser + "';";
	

	m_tempSession.Open(CStaticClass::connection);
	hr=data_table.Open(m_tempSession,(LPCTSTR)strSelectCommand);
	if(FAILED(hr))
	{
		m_tempSession.Close();
		
		return L"";
	}
	CString  m_loginUser;
	CString  m_Name;
	CString  m_group;
	CString  m_serialNo;
	CString  m_loginStatus;

	CString  m_silgleData=L"";
	CString  m_allData=L"";
	int ccount=0;
	StringBuffer s;
	Writer<StringBuffer> writer(s);
	//returnval.Format(L"{\"type\":\"ORDER_DATA\",\"insert\":[%s]}",tmpstr);	

	while(hr=data_table.MoveNext()==S_OK)
	{			
		m_loginUser=data_table.m_loginUser;
		m_Name=data_table.m_Name;
		m_group=data_table.m_group;
		m_serialNo=data_table.m_serialNo;
		m_loginStatus=data_table.m_loginStatus;		
	}
	m_tempSession.Close();	
	writer.StartObject();
	writer.Key("type");
	writer.String("USER_DETAILS");
	writer.Key("data");
	writer.StartObject();
	writer.Key("loginuser");
	string ssloginUser = string(CT2CA(m_loginUser));
	const char* stloginUser = ssloginUser.c_str();
	writer.String(stloginUser);
	writer.Key("password");	
	writer.String("");
	writer.Key("name");
	string ssname = string(CT2CA(m_Name));
	const char* stname = ssname.c_str();
	writer.String(stname);
	writer.Key("groups");
	writer.StartArray();
	int datacount = m_grouplist.Total();
	for (int i = 0; i < datacount; i++)
	{
		writer.StartObject();
		st_group m_tmp = {};
		m_tmp = m_grouplist[i];
		CString m_selectGroup = m_tmp.m_group;
		int m_selectcheck = m_tmp.m_select;
		writer.Key("group");
		string ssselectedgroup = string(CT2CA(m_selectGroup));
		const char* stelectedgroup = ssselectedgroup.c_str();
		writer.String(stelectedgroup);
		writer.Key("checked");
		if (m_selectcheck == 1)
		{
			writer.Bool(true);
		}
		else
		{
			writer.Bool(false);
		}
		writer.EndObject();
	}
	writer.EndArray();



	writer.Key("tablecolumns");
	writer.StartArray();
	datacount = m_tablecolumnlist.Total();
	for (int i = 0; i < datacount; i++)
	{
		writer.StartObject();
		st_group m_tmp = {};
		m_tmp = m_tablecolumnlist[i];
		CString m_selectGroup = m_tmp.m_group;
		m_selectGroup.Replace(L" ", L"_");
		int m_selectcheck = m_tmp.m_select;
		writer.Key("group");
		string ssselectedgroup = string(CT2CA(m_selectGroup));
		const char* stelectedgroup = ssselectedgroup.c_str();
		writer.String(stelectedgroup);
		writer.Key("checked");
		if (m_selectcheck == 1)
		{
			writer.Bool(true);
		}
		else
		{
			writer.Bool(false);
		}
		writer.EndObject();
	}
	writer.EndArray();




	writer.Key("permission");
	writer.StartArray();
	datacount = m_permissionlist.Total();
	for (int i = 0; i < datacount; i++)
	{
		writer.StartObject();
		st_permission m_tmp = {};
		m_tmp = m_permissionlist[i];
		CString m_accessControlName = m_tmp.m_accessControlName;
		int m_selectcheck = m_tmp.m_select;
		writer.Key("group");
		string ssaccessControlName = string(CT2CA(m_accessControlName));
		const char* staccessControlName = ssaccessControlName.c_str();
		writer.String(staccessControlName);
		writer.Key("checked");
		if (m_selectcheck == 1)
		{
			writer.Bool(true);
		}
		else
		{
			writer.Bool(false);
		}
		writer.EndObject();
	}
	writer.EndArray();




	writer.Key("fingerPrint");
	writer.StartArray();
	while (m_serialNo.Find(L";") >= 0)
	{
		CStaticClass::m_logfile.LogEvent(L"Fingure Print Validate");
			CString m_serial = m_serialNo.Mid(0, m_serialNo.Find(L";"));
			m_serialNo = m_serialNo.Mid(m_serialNo.Find(L";") + 1, (m_serialNo.GetLength() - m_serialNo.Find(L";") - 1));
		CStaticClass::m_logfile.LogEvent(L"End Fingure Print Validate");
		string ssserial = string(CT2CA(m_serial));
		const char* stserial = ssserial.c_str();
		writer.String(stserial);
	}
	string ssserial_Final = string(CT2CA(m_serialNo));
	const char* stserial_Final = ssserial_Final.c_str();
	writer.String(stserial_Final);
	writer.EndArray();
	writer.EndObject();
	writer.EndObject();	
	rval = s.GetString();
	s.Clear();
	writer.Flush();
	return rval;
}


CString CSqlData::generateJsonGroupData()
{
	CString rval=L"";
	HRESULT hr=NULL ;
	CCommand<CAccessor<CTableGroupData>> data_table;								
	if(!SUCCEEDED(hr))
	{
		return L"" ;
	}
	CString   strCommand=L"select distinct[broker] as 'Group',subbroker as 'SubGroup',[login] as 'Login' from Orika_clientmaster  where isnull([broker],'')<>'' order by [broker],subbroker,[login] asc;";	
	
	CSession m_tempSession;
	//(L"Orderlock_5");


	/*CString StrPrintLino=L"";
	StrPrintLino.Format(L"m_mutex_order Locked(%d)",__LINE__);
	////(StrPrintLino);*/




	m_tempSession.Open(CStaticClass::connection);
	hr=data_table.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		m_tempSession.Close();		
		return L"";
	}	
	CString  m_group=L"";
	CString  m_oldGroup=L"";
	CString  m_groupAllData=L"";
	CString  m_groupsilgleData=L"";

	CString m_subgroup=L"";
	CString m_allLogin=L"";
	CString m_singleLogin=L"";
	CString m_SubgroupAllData=L"";

	CString m_login=L"";
	CString m_subGroup=L"";
	CString m_old_subgroup=L"";
	CString subGroupJson=L"";
	int ccount=0;
	int checkfirstSubgroup=1;
	int firstloginCheck=1;
	int firstgroupCheck=1;
	while(hr=data_table.MoveNext()==S_OK)
	{		
		
		m_group=data_table.m_group;
		if (m_group!=m_oldGroup)
		{
			if (checkfirstSubgroup!=1)
			{

				subGroupJson.Format(L"{\"subgroup\":\"%s\",\"login\":[%s]}",m_old_subgroup,m_allLogin);
				if (checkfirstSubgroup==1)
				{				
					m_SubgroupAllData=subGroupJson;				
				}
				else
				{
					m_SubgroupAllData=m_SubgroupAllData+","+subGroupJson;
					
				}

				CStaticClass::m_logfile.LogEvent(L"m_SubgroupAllData.Mid");
				if (m_SubgroupAllData.Mid(0,1)==",")
				{
					m_SubgroupAllData=m_SubgroupAllData.Mid(1,m_SubgroupAllData.GetLength()-1);
				}
				CStaticClass::m_logfile.LogEvent(L"End m_SubgroupAllData.Mid");
				m_groupsilgleData.Format(L"{\"group\":\"%s\", \"subgroups\":[%s]}",m_oldGroup,m_SubgroupAllData);
				if (firstgroupCheck==1)
				{
					m_groupAllData=m_groupsilgleData;
				}
				else
				{
					m_groupAllData=m_groupAllData+L","+m_groupsilgleData;
				}
			
				firstgroupCheck=0;
			}
			//checkfirstSubgroup=1;
			firstloginCheck=1;
			m_SubgroupAllData=L"";
		}
		
		m_oldGroup=m_group;
		
		m_subGroup=data_table.m_subgroup;
		m_login=data_table.m_login ;		
		m_singleLogin.Format(L"\"%s\"",m_login);
		if (firstloginCheck==1)
		{
			m_allLogin=m_singleLogin;
		}
		else
		{
			if (m_subGroup==m_old_subgroup)
			{
				m_allLogin=m_allLogin+L","+m_singleLogin;
			}
		}





		
		
		if (m_subGroup!=m_old_subgroup && firstloginCheck!=1)
		{
			
				subGroupJson.Format(L"{\"subgroup\":\"%s\", \"login\":[%s]}",m_old_subgroup,m_allLogin);
				if (checkfirstSubgroup==1)
				{				
					m_SubgroupAllData=subGroupJson;				
				}
				else
				{
					m_SubgroupAllData=m_SubgroupAllData+","+subGroupJson;
					
				}
				m_allLogin=L"";
				m_allLogin.Format(L"\"%s\"",m_login);	
				firstloginCheck=1;				
				checkfirstSubgroup=0;									
		}
		m_old_subgroup=m_subGroup;
		firstloginCheck=0;
		

	}

	if (firstloginCheck!=1)
	{			
		subGroupJson.Format(L"{\"subgroup\":\"%s\", \"login\":[%s]}",m_old_subgroup,m_allLogin);
		if (checkfirstSubgroup==1)
		{				
			m_SubgroupAllData=subGroupJson;				
		}
		else
		{
			m_SubgroupAllData=m_SubgroupAllData+","+subGroupJson;
					
		}
		CStaticClass::m_logfile.LogEvent(L"m_SubgroupAllData.Mid_1");
		if (m_SubgroupAllData.Mid(0,1)==",")
		{
			m_SubgroupAllData=m_SubgroupAllData.Mid(1,m_SubgroupAllData.GetLength()-1);
		}
		CStaticClass::m_logfile.LogEvent(L"End m_SubgroupAllData.Mid_1");
		m_groupsilgleData.Format(L"{\"group\":\"%s\", \"subgroups\":[%s]}",m_oldGroup,m_SubgroupAllData);
		if (firstgroupCheck==1)
		{
			m_groupAllData=m_groupsilgleData;
		}
		else
		{
			m_groupAllData=m_groupAllData+L","+m_groupsilgleData;
		}
		
	}




	m_tempSession.Close();
	
	//(L"U_Orderlock_5");
	rval.Format(L"{\"type\": \"ALL_GROUP_DATA\",\"data\": [%s]}",m_groupAllData);
	return rval;
}



CString CSqlData::generateJsonForFetchUserData(CString  userlogin)
{
	CString rval=L"";
	HRESULT hr=NULL ;
	CCommand<CAccessor<CTableGroupData>> data_table;								
	if(!SUCCEEDED(hr))
	{
		return L"" ;
	}
	CString   strCommand=L"";	
	strCommand.Format(L"select [broker],[subbroker],[login] from orika_userLoginAndbrokerClientMapping where userlogin='%s' order by [broker],subbroker,[login] asc;",userlogin);	
	
	CSession m_tempSession;
	//(L"Orderlock_6");



	

	m_tempSession.Open(CStaticClass::connection);
	hr=data_table.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		m_tempSession.Close();
		
		return L"";
	}	
	CString  m_group=L"";
	CString  m_oldGroup=L"";
	CString  m_groupAllData=L"";
	CString  m_groupsilgleData=L"";

	CString m_subgroup=L"";
	CString m_allLogin=L"";
	CString m_singleLogin=L"";
	CString m_SubgroupAllData=L"";

	CString m_login=L"";
	CString m_subGroup=L"";
	CString m_old_subgroup=L"";
	CString subGroupJson=L"";
	int ccount=0;
	int checkfirstSubgroup=1;
	int firstloginCheck=1;
	int firstgroupCheck=1;
	while(hr=data_table.MoveNext()==S_OK)
	{				
		m_group=data_table.m_group;
		if (m_group!=m_oldGroup)
		{
			if (checkfirstSubgroup!=1)
			{
				if (m_allLogin==L"\"\"")
				{
					m_allLogin="";
				}
				subGroupJson.Format(L"{\"subgroup\":\"%s\", \"login\":[%s]}",m_old_subgroup,m_allLogin);
				if (checkfirstSubgroup==1)
				{				
					m_SubgroupAllData=subGroupJson;				
				}
				else
				{
					m_SubgroupAllData=m_SubgroupAllData+","+subGroupJson;
					
				}
				
			}
			else
			{
				if (firstgroupCheck!=1)
				{
					if (m_old_subgroup!=L"")
					{
						subGroupJson.Format(L"{\"subgroup\":\"%s\"}",m_old_subgroup);
						if (checkfirstSubgroup==1)
						{				
							m_SubgroupAllData=subGroupJson;				
						}
						else
						{
							m_SubgroupAllData=m_SubgroupAllData+","+subGroupJson;
					
						}
					}
				}
			}
				if (firstgroupCheck!=1)
				{
					m_groupsilgleData.Format(L"{\"group\":\"%s\", \"subgroups\":[%s]}",m_oldGroup,m_SubgroupAllData);
					
				}
				if (firstgroupCheck==1)
				{
					m_groupAllData=m_groupsilgleData;
					firstgroupCheck=0;
				}
				else
				{
					if (m_groupAllData==L"")
					{
						m_groupAllData=m_groupsilgleData;
					}
					else
					{
						m_groupAllData=m_groupAllData+L","+m_groupsilgleData;
					}
				}
			
			
			checkfirstSubgroup=1;
			firstloginCheck=1;
			m_SubgroupAllData=L"";
			//subGroupJson="";
		}
		
		m_oldGroup=m_group;
		
		m_subGroup=data_table.m_subgroup;
		m_login=data_table.m_login ;	
		if (m_login!=L"")
		{
			m_singleLogin.Format(L"\"%s\"",m_login);
		}
		else
		{
			m_singleLogin.Format(L"%s ",m_login);
		}
		if (firstloginCheck==1)
		{
			m_allLogin=m_singleLogin;
		}
		else
		{
			if (m_subGroup==m_old_subgroup)
			{
				m_allLogin=m_allLogin+L","+m_singleLogin;
			}
		}





		
		
		if (m_subGroup!=m_old_subgroup )
		{
			if (firstloginCheck!=1)
			{
				if (m_allLogin==L"\"\"")
				{
					m_allLogin="";
				}
				subGroupJson.Format(L"{\"subgroup\":\"%s\", \"login\":[%s]}",m_old_subgroup,m_allLogin);
				if (checkfirstSubgroup==1)
				{				
					m_SubgroupAllData=subGroupJson;				
				}
				else
				{
					m_SubgroupAllData=m_SubgroupAllData+","+subGroupJson;
					
				}
				m_allLogin=L"";
				m_allLogin.Format(L"\"%s\"",m_login);	
				firstloginCheck=1;				
				checkfirstSubgroup=0;									
			}
			/*else
			{
				if (m_allLogin==L"")
				{
					subGroupJson.Format(L"{\"subgroup\":\"%s\"}",m_old_subgroup);
				}
				if (checkfirstSubgroup==1)
				{				
					m_SubgroupAllData=subGroupJson;				
				}
				else
				{
					m_SubgroupAllData=m_SubgroupAllData+","+subGroupJson;
					
				}
				m_allLogin=L"";
				m_allLogin.Format(L"\"%s\"",m_login);	
				firstloginCheck=1;				
				checkfirstSubgroup=0;	
			}*/
		}
		m_old_subgroup=m_subGroup;
		firstloginCheck=0;
		

	}

	data_table.Close();

	if (firstloginCheck!=1)
	{		
		if (m_allLogin==L"\"\"")
		{
			m_allLogin="";
		}
		subGroupJson.Format(L"{\"subgroup\":\"%s\", \"login\":[%s]}",m_old_subgroup,m_allLogin);
		if (checkfirstSubgroup==1)
		{				
			m_SubgroupAllData=subGroupJson;				
		}
		else
		{
			m_SubgroupAllData=m_SubgroupAllData+","+subGroupJson;
					
		}

		if (m_allLogin=="\"\"")
		{
			m_allLogin="";
		}

		if (m_old_subgroup!="" )
		{
			m_groupsilgleData.Format(L"{\"group\":\"%s\", \"subgroups\":[%s]}",m_oldGroup,m_SubgroupAllData);
		}
		else
		{
			m_groupsilgleData.Format(L"{\"group\":\"%s\", \"subgroups\":[]}",m_oldGroup);
		}
		if (firstgroupCheck==1)
		{
			m_groupAllData=m_groupsilgleData;
		}
		else
		{
			if (m_groupAllData!=L"")
			{
				m_groupAllData=m_groupAllData+L","+m_groupsilgleData;
			}
			else
			{
				m_groupAllData=m_groupsilgleData;
			}
		}
		
	}
	//CStaticClass::session.Close();

	///Generating Tab And column Data




	
	CCommand<CAccessor<CTableTabAndColumn>> data_tableTab;								
	if(!SUCCEEDED(hr))
	{
		return L"" ;
	}
	strCommand=L"";	
	strCommand.Format(L"select tabName,[key],[name] from orika_userLoginAndTabMapping where userlogin='%s' order by tabName asc;",userlogin);
	
	
	
	hr=data_tableTab.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		return L"";
	}
	CString   m_TabName;
	CString   m_Key;
	CString   m_displayValue;
	CString   m_TabNameNewValue;

	CString   m_tabWiseColumn=L"";

	CString		m_column=L"";
	CString		m_allColumn=L"";

	int data_count=0;
	int newtab=1;
	int tabcount=0;
	while(hr=data_tableTab.MoveNext()==S_OK)
	{			
		m_TabName=data_tableTab.m_TabName;;
		m_Key=data_tableTab.m_Key ;
		m_displayValue=data_tableTab.m_displayValue ;


		
		if (m_TabName!=m_TabNameNewValue)
		{			
			if (data_count!=0)
			{
				CString tmpm_allColumn=m_allColumn;
				m_allColumn.Format(L"[%s]",tmpm_allColumn);
				if (tabcount==0)
				{
					m_tabWiseColumn.Format(L"{\"TabName\":\"%s\",\"columns\":%s}",m_TabNameNewValue,m_allColumn);
				}
				else
				{
					CString strtmp=m_tabWiseColumn;
					if (strtmp!="")
					{
						m_tabWiseColumn.Format(L"%s,{\"TabName\":\"%s\",\"columns\":%s}",strtmp,m_TabNameNewValue,m_allColumn);
					}
					else
					{
						m_tabWiseColumn.Format(L"{\"TabName\":\"%s\",\"columns\":%s}",m_TabNameNewValue,m_allColumn);
					}
				}
				m_allColumn=L"";
				newtab=1;
				tabcount=1;
			}			
		}
			m_column.Format(L"{\"key\":\"%s\",\"displayValue\":\"%s\"}",m_Key,m_displayValue);
			if (m_allColumn!=L"")
			{
				m_allColumn=m_allColumn+L","+m_column;
			}
			else
			{
				m_allColumn=m_column;
			}


		m_TabNameNewValue=m_TabName;
		data_count=1;
		newtab=0;
	}
	data_tableTab.Close();


	if (data_count!=0)
	{
		CString tmpstr=m_allColumn;
		m_allColumn.Format(L"[%s]",tmpstr);	
		CString m_tabWiseColumntmp=m_tabWiseColumn;
		if (m_tabWiseColumntmp!="")
		{
			m_tabWiseColumn.Format(L"%s,{\"TabName\":\"%s\",\"columns\":%s}",m_tabWiseColumntmp,m_TabName,m_allColumn);		
		}
		else
		{
			m_tabWiseColumn.Format(L"{\"TabName\":\"%s\",\"columns\":%s}",m_TabName,m_allColumn);		
		}
	}






	int m_userDataexist=0;
//Getting Login Password
	CString   m_password;
	CString   m_name;

	CCommand<CAccessor<CTableLoginPassword>> data_tablePassword;								
	if(!SUCCEEDED(hr))
	{
		return L"" ;
	}
	strCommand=L"";	
	strCommand.Format(L"select pwd,name from orika_userlogin where UserLogin='%s';",userlogin);
	
	
	
	hr=data_tablePassword.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		return L"";
	}	
	while(hr=data_tablePassword.MoveNext()==S_OK)
	{			
		m_password=data_tablePassword.m_password ;
		m_name=data_tablePassword.m_name;
		m_userDataexist=1;
	}
	data_tablePassword.Close();



	///Getting HD Serial No
	CString HDno=L"";
	CString AllHDNo=L"";
	CCommand<CAccessor<CTableHDno>> data_hdno;								
	if(!SUCCEEDED(hr))
	{
		return L"" ;
	}
	strCommand=L"";	
	strCommand.Format(L"select userlogin,HDno from orika_userLoginHDkeyMapping where userlogin='%s';",userlogin);
	
	
	
	hr=data_hdno.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		return L"";
	}	
	while(hr=data_hdno.MoveNext()==S_OK)
	{	
		if (HDno==L"")	
		{
			HDno=data_hdno.m_HDno;
			AllHDNo.Format(L"\"%s\"",HDno);
		}
		else
		{
			HDno=data_hdno.m_HDno;
			CString tmpAllHDNo=AllHDNo;
			AllHDNo.Format(L"%s,\"%s\"",tmpAllHDNo,HDno);			
		}		
	}
	data_tablePassword.Close();



	m_tempSession.Close();




	
	//(L"U_Orderlock_6");
	
	
		rval.Format(L"{ \"type\" : \"USER_DATA\", \"data\" : { \"groups\" : [%s], \"tabs\" : [%s],\"password\" : \"%s\",\"name\":\"%s\",\"id\":\"%s\",\"serialNumbers\":[%s]  }}",m_groupAllData,m_tabWiseColumn,m_password,m_name,userlogin,AllHDNo);
	



	return rval;
}


CString CSqlData::generateJsonForUserTabs(CString  userlogin)
{
	CString rval=L"";
	HRESULT hr=NULL ;
	
	

	


	if(!SUCCEEDED(hr))
	{
		return L"" ;
	}	
	CString strCommand=L"";
	
	CCommand<CAccessor<CTableTabAndColumn>> data_tableTab;								
	if(!SUCCEEDED(hr))
	{
		return L"" ;
	}
	
	//(L"generateJsonForUserTabs_1");

	strCommand=L"";	
	strCommand.Format(L"select tabName,[key],[name] from orika_userLoginAndTabMapping where userlogin='%s' order by tabName asc;",userlogin);		
	
	CDataSource m_sqlconnection;
	hr= m_sqlconnection.OpenFromInitializationString(L"Provider=SQLNCLI11.1;Password=" + CStaticClass::SqlServerPassword + ";Persist Security Info=False;User ID=" + CStaticClass::SqlServerUserID + ";Initial Catalog=" + CStaticClass::SqlServerDatabase + ";Data Source=" + CStaticClass::SqlServerAdd + ";Use Procedure for Prepare=1;Auto Translate=True;Packet Size=4096;Workstation ID=WIN-CE63GLSHUM0;Initial File Name=\"\";Use Encryption for Data=False;Tag with column collation when possible=False;MARS Connection=False;DataTypeCompatibility=0;Trust Server Certificate=False;Application Intent=READWRITE");
	CSession m_sqlsession;	
	
	m_sqlsession.Open(m_sqlconnection);
	hr=data_tableTab.Open(m_sqlsession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		m_sqlsession.Close();		
		return L"";
	}
	//(L"generateJsonForUserTabs_2");
	CString   m_TabName;
	CString   m_Key;
	CString   m_displayValue;
	CString   m_TabNameNewValue;

	CString   m_tabWiseColumn=L"";

	CString		m_column=L"";
	CString		m_allColumn=L"";

	int data_count=0;
	int newtab=1;
	int tabcount=0;
	while(hr=data_tableTab.MoveNext()==S_OK)
	{			
		m_TabName=data_tableTab.m_TabName;;
		m_Key=data_tableTab.m_Key ;
		m_displayValue=data_tableTab.m_displayValue ;


		
		if (m_TabName!=m_TabNameNewValue)
		{			
			if (data_count!=0)
			{
				CString tmpm_allColumn=m_allColumn;
				m_allColumn.Format(L"[%s]",tmpm_allColumn);
				if (tabcount==0)
				{
					m_tabWiseColumn.Format(L"{\"TabName\":\"%s\",\"columns\":%s}",m_TabNameNewValue,m_allColumn);
				}
				else
				{
					CString strtmp=m_tabWiseColumn;
					if (strtmp!="")
					{
						m_tabWiseColumn.Format(L"%s,{\"TabName\":\"%s\",\"columns\":%s}",strtmp,m_TabNameNewValue,m_allColumn);
					}
					else
					{
						m_tabWiseColumn.Format(L"{\"TabName\":\"%s\",\"columns\":%s}",m_TabNameNewValue,m_allColumn);
					}
				}
				m_allColumn=L"";
				newtab=1;
				tabcount=1;
			}
			
		}
			m_column.Format(L"{\"key\":\"%s\",\"displayValue\":\"%s\"}",m_Key,m_displayValue);
			if (m_allColumn!=L"")
			{
				m_allColumn=m_allColumn+L","+m_column;
			}
			else
			{
				m_allColumn=m_column;
			}


		m_TabNameNewValue=m_TabName;
		data_count=1;
		newtab=0;
	}
	
	//(L"generateJsonForUserTabs_3");

	if (data_count!=0)
	{
		CString tmpstr=m_allColumn;
		m_allColumn.Format(L"[%s]",tmpstr);	
		CString m_tabWiseColumntmp=m_tabWiseColumn;
		if (m_tabWiseColumntmp!="")
		{
			m_tabWiseColumn.Format(L"%s,{\"TabName\":\"%s\",\"columns\":%s}",m_tabWiseColumntmp,m_TabName,m_allColumn);		
		}
		else
		{
			m_tabWiseColumn.Format(L"{\"TabName\":\"%s\",\"columns\":%s}",m_TabName,m_allColumn);		
		}
	}


	data_tableTab.Close();



	int m_userDataexist=0;
	//Getting Login Password
	CString   m_password;
	CString   m_name;

	CCommand<CAccessor<CTableLoginPassword>> data_tablePassword;								
	if(!SUCCEEDED(hr))
	{
		m_sqlsession.Close();
		
		return L"" ;
	}
	strCommand=L"";	
	strCommand.Format(L"select pwd,name from orika_userlogin where UserLogin='%s';",userlogin);
	
	
	
	hr=data_tablePassword.Open(m_sqlsession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		m_sqlsession.Close();
		
		return L"";
	}	
	while(hr=data_tablePassword.MoveNext()==S_OK)
	{			
		m_password=data_tablePassword.m_password ;
		m_name=data_tablePassword.m_name;
		m_userDataexist=1;
	}	
	data_tablePassword.Close();
	m_sqlsession.Close();
	
	//(L"generateJsonForUserTabs_4");
	rval.Format(L"\"data\" : { \"tabs\" : [%s],\"name\":\"%s\",\"id\":\"%s\"}",m_tabWiseColumn,m_name,userlogin);	
	return rval;
}

void CSqlData::loadMessageCodeDesc()
{
	CString rval = L"";
	HRESULT hr = NULL;
	CCommand<CAccessor<CTableColDesc>> data_table;
	if (!SUCCEEDED(hr))
	{
		return;
	}

	CSession m_tempSession;
	//(L"Orderlock_8");
	m_tempSession.Open(CStaticClass::connection);
	hr = data_table.Open(m_tempSession, (LPCTSTR)L"select  MessageCode,MessageDesc from orika_MessageCode");
	if (FAILED(hr))
	{
		m_tempSession.Close();
		
		return;
	}
	CString  m_columnData = L"";


	while (hr = data_table.MoveNext() == S_OK)
	{
		int m_messagecode = data_table.m_messagecode;
		CString m_desc = data_table.m_desc;
		CStaticClass::MessageCodeList.SetAt(m_messagecode, m_desc);
	}
	data_table.Close();
	m_tempSession.Close();
	
	//(L"U_Orderlock_8");
	return;
}

void CSqlData::getStringColumnList(CString m_commandtext, CSqlData::columnArray* m_colDataArray)
{		
	CString rval = L"";
	HRESULT hr = NULL;
	CCommand<CAccessor<CTableStringCol>> data_table;
	if (!SUCCEEDED(hr))
	{
		return;
	}



	CSession m_tempSession;
	//(L"Orderlock_9");
	m_tempSession.Open(CStaticClass::connection);
	hr = data_table.Open(m_tempSession, (LPCTSTR)m_commandtext);
	if (FAILED(hr))
	{
		m_tempSession.Close();
		
		return ;
	}
	CString  m_columnData = L"";
	

	while (hr = data_table.MoveNext() == S_OK)
	{			
		m_columnData =data_table.m_colData;
		wchar_t   m_strdata[40];
		CMTStr::Copy(m_strdata, m_columnData);
		m_colDataArray->Add(&m_strdata);
	}
	data_table.Close();
	m_tempSession.Close();
	
	//(L"U_Orderlock_9");
	return ;
}


CSqlData::st_cTableUpdateColumnMetaData CSqlData::GetSqlTable(CString m_selectCondisiton)
{
	CSqlData::st_cTableUpdateColumnMetaData m_st = {};
	CString rval = L"";
	HRESULT hr = NULL;
	CCommand<CAccessor<CTableUpdateColumnMetaData>> data_table;
	if (!SUCCEEDED(hr))
	{
		return m_st;
	}
	CString   strCommand = L"";
	strCommand.Format(L"select ColumnKey,TabName,SqlTable,SqlTableColumn,MTModuleName,MtColumnName,DataIndexForUpdate,DataSeprator from   Orika_UpdateColumnMetaData %s;", m_selectCondisiton);
	CSession m_tempSession;
	//(L"Orderlock_10");
	m_tempSession.Open(CStaticClass::connection);
	hr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
	if (FAILED(hr))
	{
		m_tempSession.Close();		
		return m_st;
	}
	CString  m_ColumnKey = L"";
	CString  m_TabName = L"";
	CString  m_SqlTable = L"";
	CString  m_SqlTableColumn = L"";
	CString  m_MTModuleName = L"";
	CString  m_MtColumnName = L"";
	int m_DataIndexForUpdate = 0;
	CString m_DataSeprator = L"";

	while (hr = data_table.MoveNext() == S_OK)
	{
		m_ColumnKey = data_table.m_ColumnKey;
		m_TabName = data_table.m_TabName;
		m_SqlTable = data_table.m_SqlTable;
		m_SqlTableColumn = data_table.m_SqlTableColumn;
		m_MTModuleName = data_table.m_MTModuleName;
		m_MtColumnName = data_table.m_MtColumnName;
		m_DataIndexForUpdate = data_table.m_DataIndexForUpdate;
		m_DataSeprator = data_table.m_DataSeprator;

		
		CMTStr::Copy(data_table.m_ColumnKey, m_ColumnKey);
		CMTStr::Copy(data_table.m_TabName, m_TabName);
		CMTStr::Copy(data_table.m_SqlTable, m_SqlTable);
		CMTStr::Copy(data_table.m_SqlTableColumn, m_SqlTableColumn);
		CMTStr::Copy(data_table.m_MTModuleName, m_MTModuleName);
		CMTStr::Copy(data_table.m_MtColumnName, m_MtColumnName);
		data_table.m_DataIndexForUpdate= m_DataIndexForUpdate;
		CMTStr::Copy(data_table.m_DataSeprator, m_DataSeprator);
	}
	data_table.Close();
	m_tempSession.Close();
	
	//(L"U_Orderlock_10");
	return m_st;
}
void CSqlData::processUpdateRequest(CString m_columnkey,CString m_tabname,CString m_value, CString m_login, CString m_symbol)
{
	CSqlData::st_cTableUpdateColumnMetaData m_st = {};
	CString m_wherecon = L"";
	m_wherecon.Format(L" where ColumnKey='%s' and TabName='%s'", m_columnkey, m_tabname);
	m_st=GetSqlTable(m_wherecon);
	
	CString  m_TabName = L"";
	CString  m_SqlTable = L"";
	CString  m_SqlTableColumn = L"";
	CString  m_MTModuleName = L"";
	CString  m_MtColumnName = L"";
	int m_DataIndexForUpdate = 0;
	CString m_DataSeprator = L"";
	
	m_TabName = m_st.m_TabName;
	m_SqlTable = m_st.m_SqlTable;
	m_SqlTableColumn = m_st.m_SqlTableColumn;
	m_MTModuleName = m_st.m_MTModuleName;
	m_MtColumnName = m_st.m_MtColumnName;
	m_DataIndexForUpdate = m_st.m_DataIndexForUpdate;
	m_DataSeprator = m_st.m_DataSeprator;

	CStaticClass::m_mtmanager.updateMTUserData(m_columnkey, m_value, m_DataSeprator, m_DataIndexForUpdate, m_login, m_symbol);
}

void CSqlData::orderUpdate(const char* strforjson)
{
	CString strreturnValue=L"";	

	rapidjson::Document d;	
	if(!d.Parse<0>(strforjson).HasParseError())
	{
	}
	else
	{
		return ;
	}

	CString strCommand=L"";
	//Update Select To Order
	if (d.HasMember("select"))
	{
		const Value& data=d["select"];
		if (data.IsArray())
		{
			for(SizeType i=0;i<data.Size();i++)
			{
				const Value& subdata=data[i];
				if (subdata.IsObject())
				{
					if (subdata.HasMember("order")&&subdata.HasMember("dataType")&&subdata.HasMember("value"))
					{
						const Value& order=subdata["order"];
						const Value& dataType=subdata["dataType"];
						const Value& value=subdata["value"];
						if (order.IsInt() && dataType.IsString() && value.IsBool())
						{
							int intOrder=0;
							intOrder=order.GetInt();
						
							CString strDataType=L"";
							strDataType=dataType.GetString();
						
							boolean  boolValue=false ;
							boolValue=value.GetBool();
							int datavalue=0;
							if (boolValue==true )
							{
								datavalue=1;								
							}
							CString strTmp=L"";
							strTmp.Format(L"update orika_order set [select]='%d' where [order]='%d';",datavalue,intOrder);
							strCommand=strCommand+strTmp;

							CStaticClass::m_mutex_order.Lock();	
							//(L"Orderlock_11");

							int orderkey=intOrder;
							CStaticClass::st_order st={};
							CStaticClass::m_Orika_orderHastable.Lookup( orderkey,st);
							st.m_select=datavalue;
							CStaticClass::m_Orika_orderHastable.SetAt( orderkey,st);
							CStaticClass::m_mutex_order.Unlock();
							//(L"U_Orderlock_11");
							
						}
					}
				}
			}			
		}
	}
	//End Update Select To Order

	if (d.HasMember("statustype"))
	{
		const Value& data=d["statustype"];
		if (data.IsArray())
		{
			for(SizeType i=0;i<data.Size();i++)
			{
				const Value& subdata=data[i];
				if (subdata.IsObject())
				{
					if (subdata.HasMember("order")&&subdata.HasMember("dataType")&&subdata.HasMember("value"))
					{
						const Value& order=subdata["order"];
						const Value& dataType=subdata["dataType"];
						const Value& value=subdata["value"];
						if (order.IsInt() && dataType.IsString() && value.IsString())
						{
							int intOrder=0;
							intOrder=order.GetInt();
						
							CString strDataType=L"";
							strDataType=dataType.GetString();
						
							CString  strValue=L"";
							strValue=value.GetString();
							
							CString strTmp=L"";
							strTmp.Format(L"update orika_order set [selecttype]='%s' where [order]='%d';",strValue,intOrder);
							strCommand=strCommand+strTmp;



							CStaticClass::m_mutex_order.Lock();		
							//(L"Orderlock_12");


							CString StrPrintLino=L"";
							StrPrintLino.Format(L"m_mutex_order Locked(%d)",__LINE__);
							////(StrPrintLino);




							int orderkey=intOrder;
							CStaticClass::st_order st={};
							CStaticClass::m_Orika_orderHastable.Lookup( orderkey,st);							
							CMTStr::Copy(st.m_selecttype ,strValue);
							CStaticClass::m_Orika_orderHastable.SetAt( orderkey,st);
							CStaticClass::m_mutex_order.Unlock();
							//(L"U_Orderlock_12");

						}
					}
				}
			}			
		}
	}


	if (d.HasMember("subtype"))
	{
		const Value& data=d["subtype"];
		if (data.IsArray())
		{
			for(SizeType i=0;i<data.Size();i++)
			{
				const Value& subdata=data[i];
				if (subdata.IsObject())
				{
					if (subdata.HasMember("order")&&subdata.HasMember("dataType")&&subdata.HasMember("value"))
					{
						const Value& order=subdata["order"];
						const Value& dataType=subdata["dataType"];
						const Value& value=subdata["value"];
						if (order.IsInt() && dataType.IsString() && value.IsString())
						{
							int intOrder=0;
							intOrder=order.GetInt();
						
							CString strDataType=L"";
							strDataType=dataType.GetString();
						
							CString  strValue=L"";
							strValue=value.GetString();
							
							CString strTmp=L"";
							strTmp.Format(L"update orika_order set [subtype]='%s' where [order]='%d';",strValue,intOrder);
							strCommand=strCommand+strTmp;



							CStaticClass::m_mutex_order.Lock();		
							//(L"Orderlock_13");

							CString StrPrintLino=L"";
							StrPrintLino.Format(L"m_mutex_order Locked(%d)",__LINE__);
							////(StrPrintLino);





							int orderkey=intOrder;
							CStaticClass::st_order st={};
							CStaticClass::m_Orika_orderHastable.Lookup( orderkey,st);							
							CMTStr::Copy(st.m_subtype ,strValue);
							CStaticClass::m_Orika_orderHastable.SetAt( orderkey,st);
							CStaticClass::m_mutex_order.Unlock();
							//(L"U_Orderlock_13");


						}
					}
				}
			}			
		}
	}


	if (d.HasMember("contraorder"))
	{
		const Value& data=d["contraorder"];
		if (data.IsArray())
		{
			for(SizeType i=0;i<data.Size();i++)
			{
				const Value& subdata=data[i];
				if (subdata.IsObject())
				{
					if (subdata.HasMember("order")&&subdata.HasMember("dataType")&&subdata.HasMember("value"))
					{
						const Value& order=subdata["order"];
						const Value& dataType=subdata["dataType"];
						const Value& value=subdata["value"];
						if (order.IsInt() && dataType.IsString() && value.IsString())
						{
							int intOrder=0;
							intOrder=order.GetInt();
						
							CString strDataType=L"";
							strDataType=dataType.GetString();
						
							CString  strValue=L"";
							strValue=value.GetString();
							
							CString strTmp=L"";
							strTmp.Format(L"update orika_order set [contraorder]='%s' where [order]='%d';",strValue,intOrder);
							strCommand=strCommand+strTmp;

							CStaticClass::m_mutex_order.Lock();	
							//(L"Orderlock_14");


							CString StrPrintLino=L"";
							StrPrintLino.Format(L"m_mutex_order Locked(%d)",__LINE__);
							////(StrPrintLino);



							int orderkey=intOrder;
							CStaticClass::st_order st={};
							CStaticClass::m_Orika_orderHastable.Lookup( orderkey,st);							
							int intContractorder=_wtoi(strValue);
							st.m_contraorder =intContractorder;
							CStaticClass::m_Orika_orderHastable.SetAt( orderkey,st);
							CStaticClass::m_mutex_order.Unlock();
							//(L"U_Orderlock_14");

						}
					}
				}
			}			
		}
	}




	if (d.HasMember("tradeexecutetime"))
	{
		const Value& data=d["tradeexecutetime"];
		if (data.IsArray())
		{
			for(SizeType i=0;i<data.Size();i++)
			{
				const Value& subdata=data[i];
				if (subdata.IsObject())
				{
					if (subdata.HasMember("order")&&subdata.HasMember("dataType")&&subdata.HasMember("value"))
					{
						const Value& order=subdata["order"];
						const Value& dataType=subdata["dataType"];
						const Value& value=subdata["value"];
						if (order.IsInt() && dataType.IsString() && value.IsInt())
						{
							int intOrder=0;
							intOrder=order.GetInt();
						
							CString strDataType=L"";
							strDataType=dataType.GetString();
						
							int  intValue=0;
							intValue=value.GetInt();
							
							CString strTmp=L"";
							strTmp.Format(L"update orika_order set [tradeexecutetime]=%d where [order]='%d';",intValue,intOrder);
							strCommand=strCommand+strTmp;


							/*int intTradeTime=0;
							if (strValue.GetLength()==24)
							{
								SYSTEMTIME curr_from;							
								curr_from.wDay=_wtoi(strValue.Mid(8,2));
								curr_from.wMonth=_wtoi(strValue.Mid(5,2));
								curr_from.wYear=_wtoi(strValue.Mid(0,4));
								curr_from.wHour=_wtoi(strValue.Mid(11,2)); 
								curr_from.wMinute=_wtoi(strValue.Mid(14,2)); 
								curr_from.wSecond =_wtoi(strValue.Mid(17,2)); 
								curr_from.wSecond =_wtoi(strValue.Mid(20,3)); 

								intTradeTime=SMTTime::STToTime(curr_from);
							}*/

							CStaticClass::m_mutex_order.Lock();		
							//(L"Orderlock_15");


							CString StrPrintLino=L"";
							StrPrintLino.Format(L"m_mutex_order Locked(%d)",__LINE__);
							////(StrPrintLino);





							int orderkey=intOrder;
							CStaticClass::st_order st={};
							CStaticClass::m_Orika_orderHastable.Lookup( orderkey,st);							
							st.m_tradeexecutetime=intValue;
							CStaticClass::m_Orika_orderHastable.SetAt( orderkey,st);
							CStaticClass::m_mutex_order.Unlock();
							//(L"U_Orderlock_15");


						}
					}
				}
			}			
		}
	}


	if (d.HasMember("ourcomment"))
	{
		const Value& data=d["ourcomment"];
		if (data.IsArray())
		{
			for(SizeType i=0;i<data.Size();i++)
			{
				const Value& subdata=data[i];
				if (subdata.IsObject())
				{
					if (subdata.HasMember("order")&&subdata.HasMember("dataType")&&subdata.HasMember("value"))
					{
						const Value& order=subdata["order"];
						const Value& dataType=subdata["dataType"];
						const Value& value=subdata["value"];
						if (order.IsInt() && dataType.IsString() && value.IsString())
						{
							int intOrder=0;
							intOrder=order.GetInt();
						
							CString strDataType=L"";
							strDataType=dataType.GetString();
						
							CString  strValue=L"";
							strValue=value.GetString();
							
							CString strTmp=L"";
							strTmp.Format(L"update orika_order set [ourcomment]='%s' where [order]='%d';",strValue,intOrder);
							strCommand=strCommand+strTmp;


							CStaticClass::m_mutex_order.Lock();	
							//(L"Orderlock_16");

							CString StrPrintLino=L"";
							StrPrintLino.Format(L"m_mutex_order Locked(%d)",__LINE__);
							////(StrPrintLino);




							int orderkey=intOrder;
							CStaticClass::st_order st={};
							CStaticClass::m_Orika_orderHastable.Lookup( orderkey,st);							
							CMTStr::Copy(st.m_ourcomment,strValue);
							CStaticClass::m_Orika_orderHastable.SetAt( orderkey,st);
							CStaticClass::m_mutex_order.Unlock();
							//(L"U_Orderlock_16");
						}
					}
				}
			}			
		}
	}

	//Updating Command To Sql Server
	HRESULT hr=NULL ;	
	CCommand<CNoAccessor, CNoRowset> UpdateCommand;
	if(!SUCCEEDED(hr))
	{
		return ;
	}
	CSession m_tempSession;
	
	m_tempSession.Open(CStaticClass::connection);
	hr=UpdateCommand.Open(m_tempSession,(LPCTSTR)strCommand);
	
	UpdateCommand.Close();
	m_tempSession.Close();	
	//(L"U_Orderlock_17");
	//End of Updating Command

}


int CSqlData::executeCommandwiterrorcode(CString strCommand)
{
	HRESULT hr = NULL;
	CCommand<CNoAccessor, CNoRowset> UpdateCommand;
	if (!SUCCEEDED(hr))
	{
		return 3;
	}
	CSession m_tempSession;
	//(L"Orderlock_18");


	



	m_tempSession.Open(CStaticClass::connection);
	hr = UpdateCommand.Open(m_tempSession, (LPCTSTR)strCommand);
	UpdateCommand.Close();
	m_tempSession.Close();
	
	if (hr == S_OK)
	{
		return 0;
	}
	else
	{
		return 1;
	}
	//(L"U_Orderlock_18");
}



void CSqlData::executeCommand(CString strCommand)
{
	HRESULT hr=NULL ;	
	CCommand<CNoAccessor, CNoRowset> UpdateCommand;
	if(!SUCCEEDED(hr))
	{
		return ;
	}
	CSession m_tempSession;
	


		m_tempSession.Open(CStaticClass::connection);
		hr=UpdateCommand.Open(m_tempSession,(LPCTSTR)strCommand);
		UpdateCommand.Close();
		m_tempSession.Close();
	
	//(L"U_Orderlock_18");
}

CString CSqlData::CommentChangeToMT5(const char* strforjson)
{
	CString strreturnValue=L"";	
	TMTArray<INT64> changedDealList;
	rapidjson::Document d;	
	if(!d.Parse<0>(strforjson).HasParseError())
	{
	}
	else
	{
		return L"";
	}


	CString totalUpdateCommand=L"";
	CString tmpjsonGlobal=L"";
	if (d.HasMember("deals"))
			{
				Value &deals=d["deals"];
				if (deals.IsArray())
				{
					for (SizeType i = 0; i < deals.Size(); i++)
					{
						Value  &valdeal=deals[i];
						if (valdeal.IsObject())
						{
							if (valdeal.HasMember("deal")&&valdeal.HasMember("commentfrom")&&valdeal.HasMember("commentto"))
							{
								Value &valdealno=valdeal["deal"];
								Value &valcomment=valdeal["commentfrom"];
								Value &valcommentto=valdeal["commentto"];

								if (valdealno.IsInt()&& valcomment.IsString()  && valcommentto.IsString())
								{
									CString tmpcommand=L"";

									INT64  intdeal=valdealno.GetInt64();
									
									CString strcomment=L"";strcomment=valcomment.GetString();
									CString strcommentto=L"";strcommentto=valcommentto.GetString();

									CStaticClass::m_mtmanager.Change_Comment(strcomment,strcommentto,intdeal);
									tmpcommand.Format(L"update orika_CommentChangeDeals set changedStatus='0'  where deal='%d'; update  Orika_dealtableAccounting set comment='%s' where deal='%d' ;",intdeal,strcommentto,intdeal);
									
									totalUpdateCommand=totalUpdateCommand+tmpcommand;
									
									CString strUpdateStatus=L"updated";
									CString tmpjson=L"";
									tmpjson.Format(L"{\"deal\":%I64u,\"status\":\"%s\"}",intdeal,strUpdateStatus);

									if (strUpdateStatus==L"updated")
									{
										changedDealList.Add(&intdeal);
									}


									if (tmpjsonGlobal==L"")
									{
										tmpjsonGlobal=tmpjson;
									}
									else
									{
										tmpjsonGlobal=tmpjsonGlobal+L","+tmpjson;
									}
									
								}
								else
								{
									strreturnValue.Format(L"{\"type\":\"COMMENT_CHANGE_STATUS\",\"status\":\"Invalid DataType  in Json\"}");
									return strreturnValue;
								}
							}
							else
							{
								strreturnValue.Format(L"{\"type\":\"COMMENT_CHANGE_STATUS\",\"status\":\"Invalid Json\"}");
								return strreturnValue;
							}
						}
						else
						{
							strreturnValue.Format(L"{\"type\":\"COMMENT_CHANGE_STATUS\",\"status\":\"Invalid Json\"}");
							return strreturnValue;
						}
					}
				}
				else
				{
					strreturnValue.Format(L"{\"type\":\"COMMENT_CHANGE_STATUS\",\"status\":\"Invalid Json\"}");
					return strreturnValue;
				}
										
				HRESULT hr=NULL ;	
				CCommand<CNoAccessor, CNoRowset> UpdateCommand;
				if(!SUCCEEDED(hr))
				{
					return L"";
				}
				CSession m_tempSession;
				




				m_tempSession.Open(CStaticClass::connection);
				hr=UpdateCommand.Open(m_tempSession,(LPCTSTR)totalUpdateCommand);
				strreturnValue.Format(L"{\"type\":\"COMMENT_CHANGE_STATUS\",\"deals\":[%s]}",tmpjsonGlobal);
				UpdateCommand.Close();
				m_tempSession.Close();
				
				

				CStaticClass::m_mutex_ClientList.Lock();
				//(L"112");
				POSITION pos = CStaticClass::m_ClientContext.GetStartPosition ();		
				while (pos != NULL) 
				{
					CString strclientkey=L"";		
					CStaticClass::st_ClientContext m_st={};												
					CStaticClass::m_ClientContext.GetNextAssoc(pos, strclientkey,m_st);																				
						if (m_st.m_FETCH_EXISTING_COMMENT_CHANGE_DATA_start==1)
						{
							m_st.m_commentArray_delete.Assign(changedDealList);										
							CStaticClass::m_ClientContext.SetAt(strclientkey,m_st);
						}
				}
				CStaticClass::m_mutex_ClientList.Unlock();
				//(L"U112");
				changedDealList.Clear();


			}
	else
	{
		strreturnValue.Format(L"{\"type\":\"COMMENT_CHANGE_STATUS\",\"status\":\"Invalid Json\"}");
		return strreturnValue;
	}
			

	return strreturnValue;
}
CString CSqlData::CommentChangeSaveData(const char* strforjson)
{
	CString strreturnValue=L"";	

	rapidjson::Document d;	
	if(!d.Parse<0>(strforjson).HasParseError())
	{
	}
	else
	{
		return L"";
	}
	CString strtype=L"";
	if(d.HasMember("type"))
	{
		Value &objtype=d["type"];
		if (objtype.IsString())
		{
			strtype=objtype.GetString();
		}
	}

	CString totalUpdateCommand=L"";
	if (d.HasMember("deals"))
			{
				Value &deals=d["deals"];
				

				rapidjson::StringBuffer  buffer;
			    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
				deals.Accept(writer);
				CString jString=L"";
				jString=buffer.GetString();
				
				if (deals.IsArray())
				{
					for (SizeType i = 0; i < deals.Size(); i++)
					{
						Value  &valdeal=deals[i];
						if (valdeal.IsObject())
						{
							if (valdeal.HasMember("deal")&&valdeal.HasMember("comment")&&valdeal.HasMember("commentto"))
							{
								Value &valdealno=valdeal["deal"];
								Value &valcomment=valdeal["comment"];
								Value &valcommentto=valdeal["commentto"];

								if (valdealno.IsInt()&& valcomment.IsString()  && valcommentto.IsString())
								{
									CString tmpcommand=L"";

									INT64 intdeal=valdealno.GetInt64();
									
									CString strcomment=L"";strcomment=valcomment.GetString();
									CString strcommentto=L"";strcommentto=valcommentto.GetString();
									int UpdateInsertFlag=0;
									tmpcommand.Format(L"delete from orika_CommentChangeDeals where deal='%I64u';insert into orika_CommentChangeDeals(deal,comment,commentTo,changedStatus)values('%I64u','%s','%s','1');",intdeal,intdeal,strcomment,strcommentto);
									
									totalUpdateCommand=totalUpdateCommand+tmpcommand;

									UpdateInsertFlag=checkDealExist(intdeal);						
											
											
									CStaticClass::st_commentChange st={};
									st.deal=intdeal;
									CMTStr::Copy(st.comment,strcomment);
									CMTStr::Copy(st.commentTo,strcommentto);
									CStaticClass::m_mutex_ClientList.Lock();
									//(L"114");
									POSITION pos = CStaticClass::m_ClientContext.GetStartPosition ();		
									while (pos != NULL) 
									{
										CString strclientkey=L"";		
										CStaticClass::st_ClientContext m_st={};												
										CStaticClass::m_ClientContext.GetNextAssoc(pos, strclientkey,m_st);		
										if (m_st.m_FETCH_EXISTING_COMMENT_CHANGE_DATA_start==1)
										{
											if (UpdateInsertFlag==0)
											{
												m_st.m_commentArray_insert.Add(&st);					
											}
											if (UpdateInsertFlag==1)
											{
												m_st.m_commentArray_update.Add(&st);					
											}
											CStaticClass::m_ClientContext.SetAt(strclientkey,m_st);
										}
									}
									CStaticClass::m_mutex_ClientList.Unlock();
									//(L"U114");


									

									
								}
								else
								{
									strreturnValue.Format(L"{\"type\":\"COMMENT_CHANGE_SAVE_RESPONSE\",\"status\":\"Invalid DataType  in Json\"}");
									return strreturnValue;
								}
							}
							else
							{
								strreturnValue.Format(L"{\"type\":\"COMMENT_CHANGE_SAVE_RESPONSE\",\"status\":\"Invalid Json\"}");
								return strreturnValue;
							}
						}
						else
						{
							strreturnValue.Format(L"{\"type\":\"COMMENT_CHANGE_SAVE_RESPONSE\",\"status\":\"Invalid Json\"}");
							return strreturnValue;
						}
					}
				}
				else
				{
					strreturnValue.Format(L"{\"type\":\"COMMENT_CHANGE_SAVE_RESPONSE\",\"status\":\"Invalid Json\"}");
					return strreturnValue;
				}
										
				HRESULT hr=NULL ;	
				CCommand<CNoAccessor, CNoRowset> UpdateCommand;
				if(!SUCCEEDED(hr))
				{
					return L"";
				}
				
				CSession m_tempSession;
				
				m_tempSession.Open(CStaticClass::connection);
				hr=UpdateCommand.Open(m_tempSession,(LPCTSTR)totalUpdateCommand);
				if (strtype==L"COMMENT_CHANGE_SAVE_DATA")
				//if (UpdateInsertFlag==0)
				{
					strreturnValue.Format(L"{\"type\":\"COMMENT_CHANGE_SAVE_RESPONSE\",\"deals\":%s,\"status\":\"Data Has Been Updated\"}",jString);
				}
				if (strtype==L"COMMENT_CHANGE_UPDATE_DATA")
				//if (UpdateInsertFlag==1)
				{
					strreturnValue.Format(L"{\"type\":\"COMMENT_CHANGE_UPDATE_RESPONSE\",\"deals\":%s,\"status\":\"Data Has Been Updated\"}",jString);
				}
				UpdateCommand.Close();
				m_tempSession.Close();
				
				//(L"U_Orderlock_20");
				////(L"m_mutex_order UNLocked _1");
			}
	else
	{
		strreturnValue.Format(L"{\"type\":\"COMMENT_CHANGE_SAVE_RESPONSE\",\"status\":\"Invalid Json\"}");
		return strreturnValue;
	}
			

			return strreturnValue;

	}







CString CSqlData::UpdateLoginUserCredentials(const char* strforjson)
{
	CString strreturnValue=L"";	

	rapidjson::Document d;	
	if(!d.Parse<0>(strforjson).HasParseError())
	{
	}
	else
	{
		return L"";
	}



	if (d.HasMember("data"))
	{
		const Value& data=d["data"];
		if (data.IsObject())
		{
			if (data.HasMember("group") &&data.HasMember("tabs") && data.HasMember("password") && data.HasMember("name") && data.HasMember("id"))
			{
				const Value& id=data["id"];
				const Value& name=data["name"];
				const Value& password=data["password"];
				CString strid=L"";
				strid=id.GetString();
				CString strname=L"";
				strname=name.GetString();
				CString strpassword=L"";
				strpassword=password.GetString();
				const Value& group=data["group"];
				const Value& tab=data["tabs"];	

				CString allInsertCommand_group=L"";

				if (data.HasMember("serialNumbers"))
				{
					const Value& serialNos=data["serialNumbers"];
					for(SizeType i=0;i<serialNos.Size();i++)
					{
						const Value& serialNo=serialNos[i];
						CString strserialNo=L"";
						strserialNo=serialNo.GetString();
						CString tmpCommand=L"" ;
						tmpCommand.Format(L"insert into orika_userLoginHDkeyMapping(userlogin,HDno)values('%s','%s'); ",strid,strserialNo);
						allInsertCommand_group=allInsertCommand_group+tmpCommand;
					}
				}



				if (strid==L"")		
				{
					strreturnValue="{ \"type\" : \"RESPONSE_MESSAGE\",\"message\":\"Login can not be blank\" }";
					return strreturnValue;
				}
			
				if (strpassword==L"")		
				{
					strreturnValue="{ \"type\" : \"RESPONSE_MESSAGE\",\"message\":\"Password can not be blank\" }";
					return strreturnValue;
				}

	//CString loginUser,CString name,CString password,const Value& group,const Value& tab

	CString errorMessage=L"";
	
	
	
	
		if (group.IsArray())
		{
			for(SizeType i=0;i<group.Size();i++)
			{
				const Value& brokerdata=group[i];
				if (brokerdata.HasMember("group") && brokerdata.HasMember("subgroups"))
				{
					const Value& broker=brokerdata["group"];					
					CString strbroker=L"";
					strbroker=broker.GetString();
					const Value& subbrokerdata=brokerdata["subgroups"];
					if (subbrokerdata.IsArray())
					{						
						if (subbrokerdata.Size()>0)
						{
						for(SizeType j=0;j<subbrokerdata.Size();j++)
						{

							const Value& objsubbroker=subbrokerdata[j];

							if (objsubbroker.HasMember("subgroup") && objsubbroker.HasMember("login"))
							{

								const Value& subbroker=objsubbroker["subgroup"];								
								CString strsubbroker=L"";
								strsubbroker=subbroker.GetString();
								const Value& logins=objsubbroker["login"];
								if (logins.IsArray())
								{
									if(logins.Size()>0)
									{
										for(SizeType k=0;k<logins.Size();k++)
										{
											const Value& login=logins[k];										

											if (login.IsObject() )
											{
												if (login.HasMember("login")&& login.HasMember("checked") )
												{
													const Value& login_selected=login["login"];
													CString strlogin=L"";
													strlogin=login_selected.GetString();

													const Value& login_selectedYN=login["checked"];
													if (login_selectedYN.IsBool())
													{
														bool bool_login_selectedYN=login_selectedYN.GetBool();													
														if (bool_login_selectedYN==true )
														{
															CString tmpCommand=L"" ;
															tmpCommand.Format(L"insert into orika_userLoginAndbrokerClientMapping(userlogin,[broker],[subbroker],[login]) values('%s','%s','%s','%s'); ",strid,strbroker,strsubbroker,strlogin);
															allInsertCommand_group=allInsertCommand_group+tmpCommand;
														}
													}
												}
											}
										}
									}
									else
									{
											CString tmpCommand=L"" ;
											tmpCommand.Format(L"insert into orika_userLoginAndbrokerClientMapping(userlogin,[broker],[subbroker],[login]) values('%s','%s','%s',''); ",strid,strbroker,strsubbroker);
											allInsertCommand_group=allInsertCommand_group+tmpCommand;
									}
								}
							}
							else
							{
								if (objsubbroker.HasMember("subgroup") )
								{
									const Value& subbroker=objsubbroker["subgroup"];								
									CString strsubbroker=L"";
									strsubbroker=subbroker.GetString();
									CString tmpCommand=L"" ;
									tmpCommand.Format(L"insert into orika_userLoginAndbrokerClientMapping(userlogin,[broker],[subbroker],[login]) values('%s','%s','%s',''); ",strid,strbroker,strsubbroker);
									allInsertCommand_group=allInsertCommand_group+tmpCommand;
								}
							}
						}
					}
						else
						{
							CString tmpCommand=L"" ;
							tmpCommand.Format(L"insert into orika_userLoginAndbrokerClientMapping(userlogin,[broker],[subbroker],[login]) values('%s','%s','',''); ",strid,strbroker);
							allInsertCommand_group=allInsertCommand_group+tmpCommand;
						}

						
					}
					else
					{
						errorMessage=L"Invalid Json String for Groups.";
						strreturnValue.Format(L"{ \"type\" : \"RESPONSE_MESSAGE\",\"message\":\"%s\" }",errorMessage);
						return strreturnValue;
					}
				}
				else
				{
						errorMessage=L"Invalid Json String for Groups.";
						strreturnValue.Format(L"{ \"type\" : \"RESPONSE_MESSAGE\",\"message\":\"%s\" }",errorMessage);
						return strreturnValue;
				}
			}
		}
		else
		{
			CString tmpCommand=L"" ;
			CString allgroup=L"";
			allgroup=group.GetString();
			if (allgroup==L"*")
			{
				tmpCommand.Format(L"insert into orika_userLoginAndbrokerClientMapping(userlogin,[broker],[subbroker],[login]) values('%s','*','',''); ",strid);
				allInsertCommand_group=allInsertCommand_group+tmpCommand;
			}
		}
	
	
	
	///Parsing tabData
	//tabjson.

	CString insertcommand_tab=L"";
	
		if (tab.IsArray())
		{
			for(SizeType i=0;i<tab.Size();i++)
			{
				const Value& tabs=tab[i] ;
				if (tabs.HasMember("TabName") && tabs.HasMember("columns"))
				{
					const Value& TabName=tabs["TabName"];
					CString strtabName=L"";
					strtabName=TabName.GetString();
					const Value& columns=tabs["columns"];
					if (columns.IsArray())
					{
						for(SizeType j=0;j<columns.Size();j++)
						{
							const Value& columnsdata=columns[j];
							if (columnsdata.HasMember("key") && columnsdata.HasMember("displayValue"))
							{
								const Value& key=columnsdata["key"];	
								CString strkey=L"";
								strkey=key.GetString();


								const Value& displayValue=columnsdata["displayValue"];	
								CString strdisplayValue=L"";
								strdisplayValue=displayValue.GetString();								

								CString tmpCommand=L"";
								tmpCommand.Format(L"insert into orika_userLoginAndTabMapping(userlogin,tabName,[key],[name])values('%s','%s','%s','%s')",strid,strtabName,strkey,strdisplayValue );
								insertcommand_tab=insertcommand_tab+tmpCommand;
							}
							else
							{
								errorMessage=L"Invalid Json String for Tabs.";
							}
						}
					}
					else
					{
						errorMessage=L"Invalid Json String for Tabs.";	
					}
				}
				else
				{
					if (tabs.HasMember("TabName"))
					{
						const Value& TabName=tabs["TabName"];
						CString strtabName=L"";
						strtabName=TabName.GetString();
						if (strtabName==L"*")
						{
							CString tmpCommand=L"";
							tmpCommand.Format(L"insert into orika_userLoginAndTabMapping(userlogin,tabName,[key],[name])values('%s','*','*','*')",strid);
							insertcommand_tab=insertcommand_tab+tmpCommand;
						}
					}
					
				}
			}
		}
	


	if (errorMessage!=L"")
	{
		
	}
	else
	{		

		CString totalUpdateCommand=L"";
		totalUpdateCommand.Format(L"delete from orika_userLoginHDkeyMapping where userlogin='%s'; delete from orika_userLoginAndbrokerClientMapping where userlogin='%s'; delete from orika_userLoginAndTabMapping where userlogin='%s' ;exec  updateUserID '%s','%s','%s';    %s %s",strid,strid,strid,strid,strpassword,strname,allInsertCommand_group,insertcommand_tab);
		
		HRESULT hr=NULL ;	
		CCommand<CNoAccessor, CNoRowset> UpdateCommand;
		if(!SUCCEEDED(hr))
		{
			return L"";
		}
		CSession m_tempSession;
		
		


		m_tempSession.Open(CStaticClass::connection);
		hr=UpdateCommand.Open(m_tempSession,(LPCTSTR)totalUpdateCommand);
	
		UpdateCommand.Close();
		m_tempSession.Close();
		CStaticClass::m_mutex_order.Unlock();
		//(L"U_Orderlock_21");

	}
	strreturnValue="{ \"type\" : \"RESPONSE_MESSAGE\",\"message\":\"Login Data Has Been Updated\" }";
	}
		}								
	}
	return strreturnValue;
}



void CSqlData::validatelogintoken(CString	m_logintoken,CString ipaddress, CSqlData::st_logintokendetail& m_temp)
{	
	CStaticClass::m_mutexlogintoken.Lock();
	CStaticClass::st_token m_token = {};
	CStaticClass::m_tokenlist.Lookup(m_logintoken, m_token);
	CString m_defineIP = L"";
	m_defineIP = m_token.m_ipaddress;
	
	if (m_token.m_status==0)
	{ 
		m_temp.returnval = 1;				
	}
	else if (m_defineIP != ipaddress)
	{
		m_temp.returnval = 2;
	}
	else if (m_token.m_status == 1)
	{
		m_temp.returnval = 0;		
		CMTStr::Copy(m_temp.m_login, m_token.m_login);
		CMTStr::Copy(m_temp.m_hdno, m_token.m_hdno);
	}
	
	CStaticClass::m_mutexlogintoken.Unlock();
}
CString CSqlData::generatelogintoken(CString  userlogin, CString key, UINT64 logintime, CString ipAddress)
{
	CString returnval = L"";	
	returnval.Format(L"%s!~o!%s!~o!%s!~o!%I64u", key, userlogin, ipAddress, logintime);
	return returnval;
}

void CSqlData::updatelogintoken(CString	m_logintoken, int    m_status, UINT64 m_tokenactivetime, UINT64 m_lastdisconnecttime, CString	 m_login, CString	m_clientkey, CString m_hdno,CString m_ipaddress)
{
	CStaticClass::m_mutexlogintoken.Lock();
		CStaticClass::st_token m_token = {};
		CStaticClass::m_tokenlist.Lookup(m_logintoken, m_token);
		wchar_t	m_temptoken[250];
		CMTStr::Copy(m_temptoken, m_clientkey);
		m_token.m_clientkey.Add(&m_temptoken);
		m_token.m_status=1;
		if (m_token.m_tokenactivetime == 0)
		{
			m_token.m_tokenactivetime = m_tokenactivetime;
		}
		CMTStr::Copy(m_token.m_login, m_login);	
		CMTStr::Copy(m_token.m_hdno, m_hdno);
		CMTStr::Copy(m_token.m_ipaddress, m_ipaddress);
		m_token.m_totalconnection= m_token.m_totalconnection+1;
		CStaticClass::m_tokenlist.SetAt(m_logintoken, m_token);
	CStaticClass::m_mutexlogintoken.Unlock();	
}
void CSqlData::removelogintoken(CString	m_logintoken, CString	m_clientkey)
{
	//loginToken Remove Code
	/*CStaticClass::m_mutexlogintoken.Lock();
	CStaticClass::st_token m_token = {};
	CStaticClass::m_tokenlist.Lookup(m_logintoken, m_token);
		
	int totalConnection = m_token.m_clientkey.Total();
	for (int i = 0; i < totalConnection; i++)
	{
		wchar_t	m_temptoken[250];		
		CMTStr::Copy(m_temptoken,m_token.m_clientkey[i]);
		CString m_strdata = m_temptoken;
		if (m_strdata == m_clientkey)
		{
			m_token.m_clientkey.Delete(i);
			break;
		}
	}	
	m_token.m_status = 1;
	
	UINT64 CurrentTime = _time64(NULL);
		m_token.m_lastdisconnecttime = CurrentTime;		
	
	m_token.m_totalconnection = m_token.m_totalconnection - 1;
	CStaticClass::m_tokenlist.SetAt(m_logintoken, m_token);
	CStaticClass::m_mutexlogintoken.Unlock();*/
}
void CSqlData::GetLoginsClient(CString strLoginuser, vector<CString>&	LA)
{
	HRESULT hr=NULL ;
	CCommand<CAccessor<CloginListTable>> data_table;								
	if(!SUCCEEDED(hr))
	{
		return ;
	}
	//1
	CString   strCommand=L"";	
	strCommand.Format(L"exec GetUserLoginList '%s';",strLoginuser);		
	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);	
	hr=data_table.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{
		return ;
	}
	int i=0;	
	while(hr=data_table.MoveNext()==S_OK)
	{				
		CString strlogin=data_table.m_login;				
		LA.push_back(strlogin);
	}
	m_tempSession.Close();
	
}



CString CSqlData::getAlertSetting(CString alertName)
{
	CString returnval = L"";
	HRESULT hr = NULL;
	CCommand<CAccessor<CalertSettingTable>> data_table;
	if (!SUCCEEDED(hr))
	{
		return L"";
	}
	CString str_where = L"";

	if (alertName.Trim() != "")
	{
		str_where.Format(L" where alertName='%s'", alertName);
	}

	CString   strCommand = L"";
	strCommand.Format(L"select alertName,alertsetting_jason from orika_Event_Setting  %s", str_where);

	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
	if (FAILED(hr))
	{
		m_tempSession.Close();
		return L"";
	}
	int i = 0;


	CString m_alertName = L"";
	CString m_alertSetting = L"";
	


	CString tmpstr = L"";
	int row_count = 0;

	StringBuffer s;
	Writer<StringBuffer> writer(s);
	//returnval.Format(L"{\"type\":\"ORDER_DATA\",\"insert\":[%s]}",tmpstr);	
	writer.StartObject();
	writer.Key("type");
	writer.String("ALERT_SETTING_LIST");
	writer.Key("alrets");
	writer.StartArray();


	

	while (hr = data_table.MoveNext() == S_OK)
	{
		m_alertName = data_table.m_alertName;
		m_alertSetting = data_table.m_alertSetting;							
		string m_str_alertSetting = CT2A(m_alertSetting);		
		Document objDoc1;
		objDoc1.Parse(m_str_alertSetting.c_str());
		if (objDoc1.HasParseError()) {						
		}
		objDoc1.Accept(writer);

	}
	data_table.Close();
	m_tempSession.Close();
	writer.EndArray();
	writer.EndObject();
	returnval = s.GetString();
	//(L"U_Orderlock_35");

	
	return returnval;
}



void CSqlData::loadEventSetting()
{
	CString returnval = L"";
	HRESULT hr = NULL;
	CCommand<CAccessor<CalertSettingTable>> data_table;
	if (!SUCCEEDED(hr))
	{
		return ;
	}
	CString str_where = L"";
	
	CString   strCommand = L"";
	strCommand.Format(L"select alertName,alertsetting_jason from orika_Event_Setting  %s", str_where);

	CSession m_tempSession;
	m_tempSession.Open(CStaticClass::connection);
	hr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
	if (FAILED(hr))
	{
		m_tempSession.Close();
		return ;
	}
	int i = 0;

	CString m_alertName = L"";
	CString m_alertSetting = L"";
	CString tmpstr = L"";
	int row_count = 0;	
	while (hr = data_table.MoveNext() == S_OK)
	{
		m_alertName = data_table.m_alertName;
		m_alertSetting = data_table.m_alertSetting;
		string m_str_alertSetting = CT2A(m_alertSetting);
		Document objDoc1;
		objDoc1.Parse(m_str_alertSetting.c_str());
		if (objDoc1.HasParseError()) 
		{
			CStaticClass::st_Alert_Setting m_temp = {};
		}
		
	}			
}
