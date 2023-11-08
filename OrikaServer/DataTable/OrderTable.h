//#include "stdafx.h"
#ifndef COrderTable_HEADER
#define COrderTable_HEADER
#include <atldbcli.h>
class COrderTable
{
public:
		TCHAR m_login[20];
		int m_time ;
		int m_deal ;
		int m_order;
		TCHAR m_symbol[32];
		int m_type ;
		double m_volume ;
		double m_price ;
		TCHAR m_comment[50];
		TCHAR m_status[30];
		int m_select ;
		TCHAR m_selecttype[15];
		TCHAR m_subtype[15];
		int m_contraorder ;
		int m_tradeexecutetime ;
		TCHAR m_ourcomment[50];
		int m_orderstate ;
		
		 

		DBSTATUS s_login;
		DBSTATUS s_time ;
		DBSTATUS s_deal ;
		DBSTATUS s_order;
		DBSTATUS s_symbol;
		DBSTATUS s_type ;
		DBSTATUS s_volume ;
		DBSTATUS s_price ;
		DBSTATUS s_comment;
		DBSTATUS s_status;
		DBSTATUS s_select ;
		DBSTATUS s_selecttype;
		DBSTATUS s_subtype;
		DBSTATUS s_contraorder ;
		DBSTATUS s_tradeexecutetime ;
		DBSTATUS s_ourcomment;
		DBSTATUS s_orderstate ;

		 
		DBLENGTH l_login;
		DBLENGTH l_time ;
		DBLENGTH l_deal ;
		DBLENGTH l_order;
		DBLENGTH l_symbol;
		DBLENGTH l_type ;
		DBLENGTH l_volume ;
		DBLENGTH l_price ;
		DBLENGTH l_comment;
		DBLENGTH l_status;
		DBLENGTH l_select ;
		DBLENGTH l_selecttype;
		DBLENGTH l_subtype;
		DBLENGTH l_contraorder ;
		DBLENGTH l_tradeexecutetime ;
		DBLENGTH l_ourcomment;
		DBLENGTH l_orderstate ;
// Column binding map
BEGIN_COLUMN_MAP(COrderTable)		
	COLUMN_ENTRY_LENGTH_STATUS(1,m_login,s_login,l_login);
	COLUMN_ENTRY_LENGTH_STATUS(2,m_time,s_time,l_time) ;
	COLUMN_ENTRY_LENGTH_STATUS(3,m_deal,s_deal,l_deal) ;
	COLUMN_ENTRY_LENGTH_STATUS(4,m_order,s_order,l_order);
	COLUMN_ENTRY_LENGTH_STATUS(5,m_symbol,s_symbol,l_symbol);
	COLUMN_ENTRY_LENGTH_STATUS(6,m_type,s_type,l_type) ;
	COLUMN_ENTRY_LENGTH_STATUS(7,m_volume,s_volume,l_volume) ;
	COLUMN_ENTRY_LENGTH_STATUS(8,m_price,s_price,l_price) ;
	COLUMN_ENTRY_LENGTH_STATUS(9,m_comment,s_comment,l_comment);
	COLUMN_ENTRY_LENGTH_STATUS(10,m_status,s_status,l_status);
	COLUMN_ENTRY_LENGTH_STATUS(11,m_select,s_select,l_select) ;
	COLUMN_ENTRY_LENGTH_STATUS(12,m_selecttype,s_selecttype,l_selecttype);
	COLUMN_ENTRY_LENGTH_STATUS(13,m_subtype,s_subtype,l_subtype);
	COLUMN_ENTRY_LENGTH_STATUS(14,m_contraorder,s_contraorder,l_contraorder) ;
	COLUMN_ENTRY_LENGTH_STATUS(15,m_tradeexecutetime,s_tradeexecutetime,l_tradeexecutetime) ;
	COLUMN_ENTRY_LENGTH_STATUS(16,m_ourcomment,s_ourcomment,l_ourcomment);
	COLUMN_ENTRY_LENGTH_STATUS(17,m_orderstate,s_ourcomment,l_ourcomment) ;
END_COLUMN_MAP()
	
};





class CDealNOTable
{
public:
	
	int m_deal;
	int m_order;
	



	
	DBSTATUS s_deal;
	DBSTATUS s_order;
	


	
	DBLENGTH l_deal;
	DBLENGTH l_order;
	
	// Column binding map
	BEGIN_COLUMN_MAP(CDealNOTable)


	COLUMN_ENTRY_LENGTH_STATUS(1, m_deal, s_deal, l_deal);
	COLUMN_ENTRY_LENGTH_STATUS(2, m_order, s_order, l_order);

	END_COLUMN_MAP()

};


class CDealTable
{
public:
	TCHAR m_login[20];
	int m_deal;
	int m_order;
	int m_time;		
	TCHAR m_symbol[32];
	int m_action;
	double m_volume;
	double m_price;
	TCHAR m_comment[50];
	



	DBSTATUS s_login;
	DBSTATUS s_deal;
	DBSTATUS s_order;
	DBSTATUS s_time;
	DBSTATUS s_symbol;
	DBSTATUS s_action;
	DBSTATUS s_volume;
	DBSTATUS s_price;
	DBSTATUS s_comment;


	DBLENGTH l_login;
	DBLENGTH l_deal;
	DBLENGTH l_order;
	DBLENGTH l_time;
	DBLENGTH l_symbol;
	DBLENGTH l_action;
	DBLENGTH l_volume;
	DBLENGTH l_price;
	DBLENGTH l_comment;
	// Column binding map
	BEGIN_COLUMN_MAP(CDealTable)


		COLUMN_ENTRY_LENGTH_STATUS(1, m_login, s_login, l_login);
	COLUMN_ENTRY_LENGTH_STATUS(2, m_deal, s_deal, l_deal);
	COLUMN_ENTRY_LENGTH_STATUS(3, m_order, s_order, l_order);
	COLUMN_ENTRY_LENGTH_STATUS(4, m_time, s_time, l_time);
	COLUMN_ENTRY_LENGTH_STATUS(5, m_symbol, s_symbol, l_symbol);
	COLUMN_ENTRY_LENGTH_STATUS(6, m_action, s_action, l_action);
	COLUMN_ENTRY_LENGTH_STATUS(7, m_volume, s_volume, l_volume);
	COLUMN_ENTRY_LENGTH_STATUS(8, m_price,s_price, l_price);
	COLUMN_ENTRY_LENGTH_STATUS(9, m_comment, s_comment, l_comment);


	END_COLUMN_MAP()

};



#endif