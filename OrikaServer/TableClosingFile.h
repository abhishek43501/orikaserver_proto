#pragma once
//#include "stdafx.h"
#ifndef EXCEL_TABLE_HEADER
#define EXCEL_TABLE_HEADER
#include <atldbcli.h>
class ExcelDataTable
{

public:
	TCHAR   v_nsecontract[100];
	DOUBLE  v_last_tick;
	TCHAR	v_StrikePrice[30];
	TCHAR   v_OptionType[10];
	DOUBLE  v_high;
	DOUBLE  v_low;


	DBSTATUS	V_s_nsecontract;
	DBSTATUS	V_s_last_tick;
	DBSTATUS	s_StrikePrice;
	DBSTATUS	s_OptionType;
	DBSTATUS	s_high;
	DBSTATUS	s_low;



	DBLENGTH 	l_V_nsecontract;
	DBLENGTH	l_V_last_tick;
	DBLENGTH	l_StrikePrice;
	DBLENGTH	l_OptionType;
	DBLENGTH	l_high;
	DBLENGTH	l_low;


	// Column binding map
	BEGIN_COLUMN_MAP(ExcelDataTable)
		COLUMN_ENTRY_LENGTH_STATUS(1, v_nsecontract, V_s_nsecontract, l_V_nsecontract);
		COLUMN_ENTRY_LENGTH_STATUS(2, v_last_tick, V_s_last_tick, l_V_last_tick);
		COLUMN_ENTRY_LENGTH_STATUS(3, v_StrikePrice, s_StrikePrice, l_StrikePrice);
		COLUMN_ENTRY_LENGTH_STATUS(4, v_OptionType, s_OptionType, l_OptionType);
		COLUMN_ENTRY_LENGTH_STATUS(5, v_high, s_high, l_high);
		COLUMN_ENTRY_LENGTH_STATUS(6, v_low, s_low, l_low);
	END_COLUMN_MAP()

};



class SymbolMappingDataTable
{

public:
	TCHAR  m_MTSymbol[100];
	TCHAR  m_NSESymbol[100];
	TCHAR  m_MTNextSymbol[100];
	DOUBLE m_NSELotSize;
	DOUBLE m_MTLotSize;
	


	DBSTATUS  s_MTSymbol;
	DBSTATUS  s_NSESymbol;
	DBSTATUS  s_MTNextSymbol;
	DBSTATUS  s_NSELotSize;
	DBSTATUS  s_MTLotSize;




	DBLENGTH  l_MTSymbol;
	DBLENGTH  l_NSESymbol;
	DBLENGTH  l_MTNextSymbol;
	DBLENGTH  l_NSELotSize;
	DBLENGTH  l_MTLotSize;



	// Column binding map
	BEGIN_COLUMN_MAP(SymbolMappingDataTable)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_MTSymbol, s_MTSymbol, l_MTSymbol);
		COLUMN_ENTRY_LENGTH_STATUS(2, m_NSESymbol, s_NSESymbol, l_NSESymbol);
		COLUMN_ENTRY_LENGTH_STATUS(3, m_MTNextSymbol, s_MTNextSymbol, l_MTNextSymbol);
		COLUMN_ENTRY_LENGTH_STATUS(4, m_NSELotSize, s_NSELotSize, l_NSELotSize);
		COLUMN_ENTRY_LENGTH_STATUS(5, m_MTLotSize, s_MTLotSize, l_MTLotSize);
	END_COLUMN_MAP()

};



class Orika_Closingprice
{

public:
	TCHAR  m_MTSymbol[100];
	DOUBLE  m_closing;
	DOUBLE  m_heigh;
	DOUBLE m_low;	

	DBSTATUS  s_MTSymbol;
	DBSTATUS  s_closing;
	DBSTATUS  s_heigh;
	DBSTATUS  s_low;

	DBLENGTH  l_MTSymbol;
	DBLENGTH  l_closing;
	DBLENGTH  l_heigh;
	DBLENGTH  l_low;



	// Column binding map
	BEGIN_COLUMN_MAP(Orika_Closingprice)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_MTSymbol, s_MTSymbol, l_MTSymbol);
		COLUMN_ENTRY_LENGTH_STATUS(2, m_closing, s_closing, l_closing);
		COLUMN_ENTRY_LENGTH_STATUS(3, m_heigh, s_heigh, l_heigh);
		COLUMN_ENTRY_LENGTH_STATUS(4, m_low, s_low, l_low);
	END_COLUMN_MAP()

};


class Orika_HighLowMismatchDataTable
{
public:	
	int		m_dealdate;
	int		m_deal;
	int		m_order;
	int		m_login;
	TCHAR   m_symbol[32];
	double 	m_rate;
	TCHAR	m_comment[100];

	DBSTATUS	s_dealdate;
	DBSTATUS	s_deal;
	DBSTATUS	s_order;
	DBSTATUS	s_login;
	DBSTATUS	s_symbol;
	DBSTATUS 	s_rate;
	DBSTATUS	s_comment;

	DBLENGTH	l_dealdate;
	DBLENGTH	l_deal;
	DBLENGTH	l_order;
	DBLENGTH	l_login;
	DBLENGTH	l_symbol;
	DBLENGTH 	l_rate;
	DBLENGTH	l_comment;
	// Column binding map
	BEGIN_COLUMN_MAP(Orika_HighLowMismatchDataTable)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_dealdate, s_dealdate, l_dealdate);
		COLUMN_ENTRY_LENGTH_STATUS(2, m_deal, s_deal, l_deal);
		COLUMN_ENTRY_LENGTH_STATUS(3, m_order, s_order, l_order);
		COLUMN_ENTRY_LENGTH_STATUS(4, m_login, s_login, l_login);
		COLUMN_ENTRY_LENGTH_STATUS(5, m_symbol, s_symbol, l_symbol);
		COLUMN_ENTRY_LENGTH_STATUS(6, m_rate, s_rate, l_rate);
		COLUMN_ENTRY_LENGTH_STATUS(7, m_comment, s_comment, l_comment);
	END_COLUMN_MAP()

};



class CTableDashboard
{
public:
	TCHAR m_loginuser[20];
	TCHAR m_field[30];
	TCHAR m_type[30];
	TCHAR m_gridProperties[8000];
	TCHAR m_data[8000];

	DBSTATUS s_loginuser;
	DBSTATUS s_field;
	DBSTATUS s_type;
	DBSTATUS s_gridProperties;
	DBSTATUS s_data;

	DBLENGTH l_loginuser;
	DBLENGTH l_field;
	DBLENGTH l_type;
	DBLENGTH l_gridProperties;
	DBLENGTH l_data;
	// Column binding map
	BEGIN_COLUMN_MAP(CTableDashboard)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_loginuser, s_loginuser, l_loginuser);
	COLUMN_ENTRY_LENGTH_STATUS(2, m_field, s_field, l_field);
	COLUMN_ENTRY_LENGTH_STATUS(3, m_type, s_type, l_type);
	COLUMN_ENTRY_LENGTH_STATUS(4, m_gridProperties, s_gridProperties, l_gridProperties);
	COLUMN_ENTRY_LENGTH_STATUS(5, m_data, s_data, l_data);
	END_COLUMN_MAP()

};



class CTableCanvas
{
public:
	TCHAR m_loginuser[20];
	TCHAR m_tabtype[100];
	TCHAR m_data[20000];

	DBSTATUS s_loginuser;
	DBSTATUS s_tabtype;
	DBSTATUS s_data;

	DBLENGTH l_loginuser;
	DBLENGTH l_tabtype;
	DBLENGTH l_data;
	// Column binding map
	BEGIN_COLUMN_MAP(CTableCanvas)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_loginuser, s_loginuser, l_loginuser);
		COLUMN_ENTRY_LENGTH_STATUS(2, m_tabtype, s_tabtype, l_tabtype);
		COLUMN_ENTRY_LENGTH_STATUS(3, m_data, s_data, l_data);	
	END_COLUMN_MAP()

};



//class CTableorika_page_details
//{
//public:
//	TCHAR m_pageId[150];
//	TCHAR m_loginUser[32];
//	TCHAR m_name[100];
//	TCHAR m_shortName[32];
//	TCHAR m_layout1[1000];
//	TCHAR m_layout2[1000];
//	TCHAR m_layout3[1000];
//	TCHAR m_layout4[1000];
//	TCHAR m_layout5[1000];
//	TCHAR m_layout6[1000];
//	TCHAR m_layout7[1000];
//	//TCHAR m_layout8[1000];
//	//TCHAR m_layout9[1000];
//	//TCHAR m_layout10[1000];
//
//	TCHAR m_widgetConfigs1[1000];
//	TCHAR m_widgetConfigs2[1000];
//	TCHAR m_widgetConfigs3[1000];
//	TCHAR m_widgetConfigs4[1000];
//	TCHAR m_widgetConfigs5[1000];
//	TCHAR m_widgetConfigs6[1000];
//	TCHAR m_widgetConfigs7[1000];
//	//TCHAR m_widgetConfigs8[1000];
//	//TCHAR m_widgetConfigs9[1000];
//	//TCHAR m_widgetConfigs10[1000];
//
//
//	DBSTATUS s_pageId;
//	DBSTATUS s_loginUser;
//	DBSTATUS s_name;
//	DBSTATUS s_shortName;
//	DBSTATUS s_layout1;
//	DBSTATUS s_layout2;
//	DBSTATUS s_layout3;
//	DBSTATUS s_layout4;
//	DBSTATUS s_layout5;
//	DBSTATUS s_layout6;
//	DBSTATUS s_layout7;
//	//DBSTATUS s_layout8;
//	//DBSTATUS s_layout9;
//	//DBSTATUS s_layout10;
//
//	DBSTATUS s_widgetConfigs1;
//	DBSTATUS s_widgetConfigs2;
//	DBSTATUS s_widgetConfigs3;
//	DBSTATUS s_widgetConfigs4;
//	DBSTATUS s_widgetConfigs5;
//	DBSTATUS s_widgetConfigs6;
//	DBSTATUS s_widgetConfigs7;
//	//DBSTATUS s_widgetConfigs8;
//	//DBSTATUS s_widgetConfigs9;
//	//DBSTATUS s_widgetConfigs10;
//
//
//	DBLENGTH l_pageId;
//	DBLENGTH l_loginUser;
//	DBLENGTH l_name;
//	DBLENGTH l_shortName;
//
//	DBLENGTH l_layout1;
//	DBLENGTH l_layout2;
//	DBLENGTH l_layout3;
//	DBLENGTH l_layout4;
//	DBLENGTH l_layout5;
//	DBLENGTH l_layout6;
//	DBLENGTH l_layout7;
//	//DBLENGTH l_layout8;
//	//DBLENGTH l_layout9;
//	//DBLENGTH l_layout10;
//
//	DBLENGTH l_widgetConfigs1;
//	DBLENGTH l_widgetConfigs2;
//	DBLENGTH l_widgetConfigs3;
//	DBLENGTH l_widgetConfigs4;
//	DBLENGTH l_widgetConfigs5;
//	DBLENGTH l_widgetConfigs6;
//	DBLENGTH l_widgetConfigs7;
//	//DBLENGTH l_widgetConfigs8;
//	//DBLENGTH l_widgetConfigs9;
//	//DBLENGTH l_widgetConfigs10;
//	
//	// Column binding map
//	BEGIN_COLUMN_MAP(CTableorika_page_details)
//		COLUMN_ENTRY_LENGTH_STATUS(1, m_pageId, s_pageId, l_pageId);
//		COLUMN_ENTRY_LENGTH_STATUS(2, m_loginUser, s_loginUser, l_loginUser);
//		COLUMN_ENTRY_LENGTH_STATUS(3, m_name, s_name, l_name);
//		COLUMN_ENTRY_LENGTH_STATUS(4, m_shortName, s_shortName, l_shortName);
//		COLUMN_ENTRY_LENGTH_STATUS(5, m_layout1, s_layout1, l_layout1);
//		COLUMN_ENTRY_LENGTH_STATUS(6, m_layout2, s_layout2, l_layout2);
//		COLUMN_ENTRY_LENGTH_STATUS(7, m_layout3, s_layout3, l_layout3);
//		COLUMN_ENTRY_LENGTH_STATUS(8, m_layout4, s_layout4, l_layout4);
//		COLUMN_ENTRY_LENGTH_STATUS(9, m_layout5, s_layout5, l_layout5);
//		COLUMN_ENTRY_LENGTH_STATUS(10, m_layout6, s_layout6, l_layout6);
//		COLUMN_ENTRY_LENGTH_STATUS(11, m_layout7, s_layout7, l_layout7);
//		//COLUMN_ENTRY_LENGTH_STATUS(12, m_layout8, s_layout8, l_layout8);
//		//COLUMN_ENTRY_LENGTH_STATUS(13, m_layout9, s_layout9, l_layout9);
//		//COLUMN_ENTRY_LENGTH_STATUS(14, m_layout10, s_layout10, l_layout10);		
//		COLUMN_ENTRY_LENGTH_STATUS(12, m_widgetConfigs1, s_widgetConfigs1, l_widgetConfigs1);
//		COLUMN_ENTRY_LENGTH_STATUS(13, m_widgetConfigs2, s_widgetConfigs2, l_widgetConfigs2);
//		COLUMN_ENTRY_LENGTH_STATUS(14, m_widgetConfigs3, s_widgetConfigs3, l_widgetConfigs3);
//		COLUMN_ENTRY_LENGTH_STATUS(15, m_widgetConfigs4, s_widgetConfigs4, l_widgetConfigs4);
//		COLUMN_ENTRY_LENGTH_STATUS(16, m_widgetConfigs5, s_widgetConfigs5, l_widgetConfigs5);
//		COLUMN_ENTRY_LENGTH_STATUS(17, m_widgetConfigs6, s_widgetConfigs6, l_widgetConfigs6);
//		COLUMN_ENTRY_LENGTH_STATUS(18, m_widgetConfigs7, s_widgetConfigs7, l_widgetConfigs7);
//		//COLUMN_ENTRY_LENGTH_STATUS(22, m_widgetConfigs8, s_widgetConfigs8, l_widgetConfigs8);
//		//COLUMN_ENTRY_LENGTH_STATUS(23, m_widgetConfigs9, s_widgetConfigs9, l_widgetConfigs9);
//		//COLUMN_ENTRY_LENGTH_STATUS(24, m_widgetConfigs10, s_widgetConfigs10, l_widgetConfigs10);
//	END_COLUMN_MAP()
//
//};


class CTableorika_page_details
{
public:
	TCHAR m_pageId[150];
	TCHAR m_loginUser[32];
	TCHAR m_name[100];
	TCHAR m_shortName[32];
	TCHAR m_layout1[15000];	
	
	
	DBSTATUS s_pageId;
	DBSTATUS s_loginUser;
	DBSTATUS s_name;
	DBSTATUS s_shortName;
	DBSTATUS s_layout1;	
	
	


	DBLENGTH l_pageId;
	DBLENGTH l_loginUser;
	DBLENGTH l_name;
	DBLENGTH l_shortName;
	DBLENGTH l_layout1;	
	
	

	// Column binding map
	BEGIN_COLUMN_MAP(CTableorika_page_details)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_pageId, s_pageId, l_pageId);
		COLUMN_ENTRY_LENGTH_STATUS(2, m_loginUser, s_loginUser, l_loginUser);
		COLUMN_ENTRY_LENGTH_STATUS(3, m_name, s_name, l_name);
		COLUMN_ENTRY_LENGTH_STATUS(4, m_shortName, s_shortName, l_shortName);
		COLUMN_ENTRY_LENGTH_STATUS(5, m_layout1, s_layout1, l_layout1);				
	END_COLUMN_MAP()

};


class CTableorika_page_widgetConfig
{
public:
	TCHAR m_pageId[150];	
	TCHAR m_widgetConfig1[20000];

	DBSTATUS s_pageId;	
	DBSTATUS s_widgetConfig1;

	DBLENGTH l_pageId;	
	DBLENGTH l_widgetConfig1;

	// Column binding map
	BEGIN_COLUMN_MAP(CTableorika_page_widgetConfig)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_pageId, s_pageId, l_pageId);	
		COLUMN_ENTRY_LENGTH_STATUS(2, m_widgetConfig1, s_widgetConfig1, l_widgetConfig1);
	END_COLUMN_MAP()
};


class CTableorika_page_widgetConfig_and_Sheet
{
public:
	TCHAR m_pageandsheetID[300];
	TCHAR m_pageId[150];
	TCHAR m_sheet[20000];

	DBSTATUS s_pageandsheetID;
	DBSTATUS s_pageId;
	DBSTATUS s_sheet;

	DBLENGTH l_pageandsheetID;
	DBLENGTH l_pageId;
	DBLENGTH l_sheet;

	// Column binding map
	BEGIN_COLUMN_MAP(CTableorika_page_widgetConfig_and_Sheet)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_pageandsheetID, s_pageandsheetID, l_pageandsheetID);
		COLUMN_ENTRY_LENGTH_STATUS(2, m_pageId, s_pageId, l_pageId);
		COLUMN_ENTRY_LENGTH_STATUS(3, m_sheet, s_sheet, l_sheet);
	END_COLUMN_MAP()
};



class CTableorika_page_list
{
public:
	TCHAR m_pageId[150];
	TCHAR m_loginUser[32];
	TCHAR m_name[100];
	TCHAR m_shortName[32];
	int   m_sequence;
	


	DBSTATUS s_pageId;
	DBSTATUS s_loginUser;
	DBSTATUS s_name;
	DBSTATUS s_shortName;
	DBSTATUS s_sequence;

	


	DBLENGTH l_pageId;
	DBLENGTH l_loginUser;
	DBLENGTH l_name;
	DBLENGTH l_shortName;
	DBLENGTH l_sequence;
	

	// Column binding map
	BEGIN_COLUMN_MAP(CTableorika_page_list)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_pageId, s_pageId, l_pageId);
		COLUMN_ENTRY_LENGTH_STATUS(2, m_loginUser, s_loginUser, l_loginUser);
		COLUMN_ENTRY_LENGTH_STATUS(3, m_name, s_name, l_name);
		COLUMN_ENTRY_LENGTH_STATUS(4, m_shortName, s_shortName, l_shortName);	
		COLUMN_ENTRY_LENGTH_STATUS(5, m_sequence, s_sequence, l_sequence);
	END_COLUMN_MAP()

};



class CTableColorTheme
{
public:
	TCHAR m_loginUser[30];
	TCHAR m_COLOR_THEME_ID[100];
	TCHAR m_COLOR_THEME_name[100];
	TCHAR m_color[500];
	TCHAR m_fontcolor[500];
	int   m_default;



	DBSTATUS s_loginUser;
	DBSTATUS s_COLOR_THEME_ID;
	DBSTATUS s_COLOR_THEME_name;
	DBSTATUS s_color;
	DBSTATUS s_fontcolor;
	DBSTATUS s_default;



	DBLENGTH l_loginUser;
	DBSTATUS l_COLOR_THEME_ID;
	DBSTATUS l_COLOR_THEME_name;
	DBSTATUS l_color;
	DBSTATUS l_fontcolor;
	DBSTATUS l_default;


	// Column binding map
	BEGIN_COLUMN_MAP(CTableColorTheme)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_loginUser, s_loginUser, l_loginUser);
		COLUMN_ENTRY_LENGTH_STATUS(2, m_COLOR_THEME_ID, s_COLOR_THEME_ID, l_COLOR_THEME_ID);
		COLUMN_ENTRY_LENGTH_STATUS(3, m_COLOR_THEME_name, s_COLOR_THEME_name, l_COLOR_THEME_name);
		COLUMN_ENTRY_LENGTH_STATUS(4, m_color, s_color, l_color);
		COLUMN_ENTRY_LENGTH_STATUS(5, m_fontcolor, s_fontcolor, l_fontcolor);
		COLUMN_ENTRY_LENGTH_STATUS(6, m_default, s_default, l_default);
	END_COLUMN_MAP()

};


class CTableorika_COLOR_TEMPLATE_ClientWise
{
public:	
	TCHAR m_loginUser[32];	
	TCHAR m_template[1000];

	DBSTATUS s_oginUser;
	DBSTATUS s_template;
	

	DBLENGTH l_oginUser;
	DBLENGTH l_template;
	
	// Column binding map
	BEGIN_COLUMN_MAP(CTableorika_COLOR_TEMPLATE_ClientWise)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_loginUser, s_oginUser , l_oginUser);
		COLUMN_ENTRY_LENGTH_STATUS(2, m_template , s_template , l_template);
	END_COLUMN_MAP()

};


class CTableorika_Theme
{
public:
	TCHAR m_loginUser[32];
	TCHAR m_theme[1000];

	DBSTATUS s_oginUser;
	DBSTATUS s_theme;


	DBLENGTH l_oginUser;
	DBLENGTH l_theme;

	// Column binding map
	BEGIN_COLUMN_MAP(CTableorika_Theme)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_loginUser, s_oginUser, l_oginUser);
		COLUMN_ENTRY_LENGTH_STATUS(2, m_theme, s_theme, l_theme);
	END_COLUMN_MAP()

};




class CTableGATEWAY_POSITIONDEVIDERATIO
{
public:
	TCHAR m_symbolgroup[32];
	double  m_lots;
	double  m_ratio;
	double  m_stepup;
	double  m_perfect;
	double  m_rounded;

	DBSTATUS s_symbolgroup;
	DBSTATUS s_lots;
	DBSTATUS s_ratio;
	DBSTATUS s_stepup;
	DBSTATUS s_perfect;
	DBSTATUS s_rounded;

	DBLENGTH l_symbolgroup;
	DBLENGTH l_lots;
	DBLENGTH l_ratio;
	DBLENGTH l_stepup;
	DBLENGTH l_perfect;
	DBLENGTH l_rounded;
	// Column binding map
	BEGIN_COLUMN_MAP(CTableGATEWAY_POSITIONDEVIDERATIO)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_symbolgroup, s_symbolgroup, l_symbolgroup);
	COLUMN_ENTRY_LENGTH_STATUS(2, m_lots, s_lots, l_lots);
	COLUMN_ENTRY_LENGTH_STATUS(3, m_ratio, s_ratio, l_ratio);
	COLUMN_ENTRY_LENGTH_STATUS(4, m_stepup, s_stepup, l_stepup);
	COLUMN_ENTRY_LENGTH_STATUS(5, m_perfect, s_perfect, l_perfect);
	COLUMN_ENTRY_LENGTH_STATUS(6, m_rounded, s_rounded, l_rounded);
	END_COLUMN_MAP()

};




class CTableOrika_PositionTransferPriotity
{
public:
	TCHAR  m_symbolgroup[32];
	TCHAR  m_lpname[32];
	TCHAR  m_lpsymbol[32];
	int    m_lppriority;
	double m_lpmaxlots;
	TCHAR  m_lpsendingtype[32];
	double m_lpratio;


	TCHAR	  s_symbolgroup;
	DBSTATUS  s_lpname;
	DBSTATUS  s_lpsymbol;
	DBSTATUS  s_lppriority;
	DBSTATUS  s_lpmaxlots;
	DBSTATUS  s_lpsendingtype;
	DBSTATUS  s_lpratio;

	DBLENGTH  l_symbolgroup;
	DBLENGTH  l_lpname;
	DBLENGTH  l_lpsymbol;
	DBLENGTH  l_lppriority;
	DBLENGTH  l_lpmaxlots;
	DBLENGTH  l_lpsendingtype;
	DBLENGTH  l_lpratio;
	// Column binding map
	BEGIN_COLUMN_MAP(CTableOrika_PositionTransferPriotity)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_symbolgroup, s_symbolgroup, l_symbolgroup);
		COLUMN_ENTRY_LENGTH_STATUS(2, m_lpname, s_lpname, l_lpname);
		COLUMN_ENTRY_LENGTH_STATUS(3, m_lpsymbol, s_lpsymbol, l_lpsymbol);
		COLUMN_ENTRY_LENGTH_STATUS(4, m_lppriority, s_lppriority, l_lppriority);
		COLUMN_ENTRY_LENGTH_STATUS(5, m_lpmaxlots, s_lpmaxlots, l_lpmaxlots);
		COLUMN_ENTRY_LENGTH_STATUS(6, m_lpsendingtype, s_lpsendingtype, l_lpsendingtype);
		COLUMN_ENTRY_LENGTH_STATUS(7, m_lpratio, s_lpratio, l_lpratio);
	END_COLUMN_MAP()

};





#endif