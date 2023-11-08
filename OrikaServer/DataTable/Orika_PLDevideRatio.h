//#include "stdafx.h"
#ifndef COrika_PLDevideRatio_HEADER
#define COrika_PLDevideRatio_HEADER
#include <atldbcli.h>
class COrika_PLDevideRatio
{
public:
	TCHAR m_login[20];
	TCHAR m_symbolGroup[20];
	double m_brokerPLRatio;
	double m_subBrokerPLRatio;
	double m_companyPLRatio;
		
	DBSTATUS s_login;
	DBSTATUS s_symbolGroup;
	DBSTATUS s_brokerPLRatio;
	DBSTATUS s_subBrokerPLRatio;
	DBSTATUS s_companyPLRatio;

	DBLENGTH l_login;
	DBLENGTH l_symbolGroup;
	DBLENGTH l_brokerPLRatio;
	DBLENGTH l_subBrokerPLRatio;
	DBLENGTH l_companyPLRatio;
// Column binding map
BEGIN_COLUMN_MAP(COrika_PLDevideRatio)	
	COLUMN_ENTRY_LENGTH_STATUS(1,m_login,s_login,l_login);
	COLUMN_ENTRY_LENGTH_STATUS(2,m_symbolGroup,s_symbolGroup,l_symbolGroup);
	COLUMN_ENTRY_LENGTH_STATUS(3,m_brokerPLRatio,s_brokerPLRatio,l_brokerPLRatio);
	COLUMN_ENTRY_LENGTH_STATUS(4,m_subBrokerPLRatio,s_subBrokerPLRatio,l_subBrokerPLRatio);
	COLUMN_ENTRY_LENGTH_STATUS(5,m_companyPLRatio,s_companyPLRatio,l_companyPLRatio);		
END_COLUMN_MAP() 
	
};
#endif