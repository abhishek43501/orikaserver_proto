//#include "stdafx.h"
#ifndef COrika_clientmaster_HEADER
#define COrika_clientmaster_HEADER
#include <atldbcli.h>
class COrika_clientmaster
{
public:
	TCHAR m_login[20];
	TCHAR m_name[50];
	TCHAR m_broker[20];
	TCHAR m_subBroker[20];
	TCHAR m_extraGroup[20];
	double m_lossLimit;
	double m_creditLimit ;
	int   m_comment ;
	double m_qtyLimitMultiplayer; 
	int   m_ignoreTrader ;
	TCHAR m_colour[20];
	TCHAR m_company[30];
	double m_lpRatio;
	double m_maxbrokerage;
		

	DBSTATUS s_login;
	DBSTATUS s_name;
	DBSTATUS s_broker;
	DBSTATUS s_subBroker;
	DBSTATUS s_extraGroup;
	DBSTATUS s_lossLimit;
	DBSTATUS s_creditLimit;
	DBSTATUS s_comment;
	DBSTATUS s_qtyLimitMultiplayer;
	DBSTATUS s_ignoreTrader;
	DBSTATUS s_colour;
	DBSTATUS s_company;
	DBSTATUS s_lpRatio;
	DBSTATUS s_maxbrokerage;

	DBLENGTH l_login;
	DBLENGTH l_name;
	DBLENGTH l_broker;
	DBLENGTH l_subBroker;
	DBLENGTH l_extraGroup;
	DBLENGTH l_lossLimit;
	DBLENGTH l_creditLimit;
	DBLENGTH l_comment;
	DBLENGTH l_qtyLimitMultiplayer;
	DBLENGTH l_ignoreTrader;
	DBLENGTH l_colour;
	DBLENGTH l_company;
	DBLENGTH l_lpRatio;
	DBLENGTH l_maxbrokerage;
// Column binding map
BEGIN_COLUMN_MAP(COrika_clientmaster)
	COLUMN_ENTRY_LENGTH_STATUS(1,m_login,s_login,l_login);
	COLUMN_ENTRY_LENGTH_STATUS(2,m_name,s_name,l_name);
	COLUMN_ENTRY_LENGTH_STATUS(3,m_broker,s_broker,l_broker);
	COLUMN_ENTRY_LENGTH_STATUS(4,m_subBroker,s_subBroker,l_subBroker);	
	COLUMN_ENTRY_LENGTH_STATUS(5,m_extraGroup,s_extraGroup,l_extraGroup);
	COLUMN_ENTRY_LENGTH_STATUS(6,m_lossLimit,s_lossLimit,l_lossLimit);
	COLUMN_ENTRY_LENGTH_STATUS(7,m_creditLimit,s_creditLimit,l_creditLimit);
	COLUMN_ENTRY_LENGTH_STATUS(8,m_comment,s_comment,l_comment);
	COLUMN_ENTRY_LENGTH_STATUS(9,m_qtyLimitMultiplayer,s_qtyLimitMultiplayer,l_qtyLimitMultiplayer);
	COLUMN_ENTRY_LENGTH_STATUS(10,m_ignoreTrader,s_ignoreTrader,l_ignoreTrader);
	COLUMN_ENTRY_LENGTH_STATUS(11,m_colour,s_colour,l_colour);
	COLUMN_ENTRY_LENGTH_STATUS(12, m_company, s_company, l_company);
	COLUMN_ENTRY_LENGTH_STATUS(13, m_lpRatio, s_lpRatio, l_lpRatio);
	COLUMN_ENTRY_LENGTH_STATUS(14, m_maxbrokerage, s_maxbrokerage, l_maxbrokerage);
END_COLUMN_MAP() 
	
};




class COrika_broker
{
public:
	TCHAR m_broker[20];	
	double m_maxbrokerage;


	DBSTATUS s_broker;	
	DBSTATUS s_maxbrokerage;

	DBLENGTH l_broker;	
	DBLENGTH l_maxbrokerage;
	// Column binding map
	BEGIN_COLUMN_MAP(COrika_broker)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_broker, s_broker, l_broker);		
		COLUMN_ENTRY_LENGTH_STATUS(2, m_maxbrokerage, s_maxbrokerage, l_maxbrokerage);
	END_COLUMN_MAP()
};

class COrika_subbroker
{
public:
	TCHAR m_subbroker[20];
	double m_maxbrokerage;


	DBSTATUS s_subbroker;
	DBSTATUS s_maxbrokerage;

	DBLENGTH l_subbroker;
	DBLENGTH l_maxbrokerage;
	// Column binding map
	BEGIN_COLUMN_MAP(COrika_subbroker)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_subbroker, s_subbroker, l_subbroker);
		COLUMN_ENTRY_LENGTH_STATUS(2, m_maxbrokerage, s_maxbrokerage, l_maxbrokerage);
	END_COLUMN_MAP()
};



class CloginListTable
{
public:
	TCHAR m_login[20];			
	DBSTATUS s_login;
	DBLENGTH l_login;
// Column binding map
BEGIN_COLUMN_MAP(CloginListTable)
	COLUMN_ENTRY_LENGTH_STATUS(1,m_login,s_login,l_login);	
END_COLUMN_MAP() 
	
};
#endif