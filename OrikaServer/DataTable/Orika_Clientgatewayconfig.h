//#include "stdafx.h"
#ifndef COrika_Clientgatewayconfig_HEADER
#define COrika_Clientgatewayconfig_HEADER
#include <atldbcli.h>
class COrika_Clientgatewayconfig
{
public:
	TCHAR m_login[20];
	TCHAR m_symbolGroup[20];
	int   m_symbolWiseBuyLimit;
	int   m_symbolWiseSellLimit;
	int   m_symbolWisePendingOrderEnableDisable;
	int   m_symbolPositionLimit;
	int   m_qtyLimitMultiplayer;
		
 	DBSTATUS s_login;
	DBSTATUS s_symbolGroup;
	DBSTATUS s_symbolWiseBuyLimit;
	DBSTATUS s_symbolWiseSellLimit;
	DBSTATUS s_symbolWisePendingOrderEnableDisable;
	DBSTATUS s_symbolPositionLimit;
	DBSTATUS s_qtyLimitMultiplayer;

	DBLENGTH l_login;
	DBLENGTH l_symbolGroup;
	DBLENGTH l_symbolWiseBuyLimit;
	DBLENGTH l_symbolWiseSellLimit;
	DBLENGTH l_symbolWisePendingOrderEnableDisable;
	DBLENGTH l_symbolPositionLimit;
	DBLENGTH l_qtyLimitMultiplayer;
// Column binding map
BEGIN_COLUMN_MAP(COrika_Clientgatewayconfig)			
	COLUMN_ENTRY_LENGTH_STATUS(1,m_login,s_login,l_login);
	COLUMN_ENTRY_LENGTH_STATUS(2,m_symbolGroup,s_symbolGroup,l_symbolGroup);
	COLUMN_ENTRY_LENGTH_STATUS(3,m_symbolWiseBuyLimit,s_symbolWiseBuyLimit,l_symbolWiseBuyLimit);
	COLUMN_ENTRY_LENGTH_STATUS(4,m_symbolWiseSellLimit,s_symbolWiseSellLimit,l_symbolWiseSellLimit);
	COLUMN_ENTRY_LENGTH_STATUS(5,m_symbolWisePendingOrderEnableDisable,s_symbolWisePendingOrderEnableDisable,l_symbolWisePendingOrderEnableDisable);
	COLUMN_ENTRY_LENGTH_STATUS(6,m_symbolPositionLimit,s_symbolPositionLimit,l_symbolPositionLimit);
	COLUMN_ENTRY_LENGTH_STATUS(7,m_qtyLimitMultiplayer,s_qtyLimitMultiplayer,l_qtyLimitMultiplayer);
END_COLUMN_MAP() 
	
};


class CLpRatioCommoditygroupwise
{
public:
	TCHAR m_login[32];
	TCHAR m_commoditygroup[32];
	double m_lpratio;

	DBSTATUS s_login;
	DBSTATUS s_commoditygroup;
	DBSTATUS s_lpratio;

	DBLENGTH l_login;
	DBLENGTH l_commoditygroup;
	DBLENGTH l_lpratio;
	// Column binding map
	BEGIN_COLUMN_MAP(CLpRatioCommoditygroupwise)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_login, s_login, l_login);
		COLUMN_ENTRY_LENGTH_STATUS(2, m_commoditygroup, s_commoditygroup, l_commoditygroup);
		COLUMN_ENTRY_LENGTH_STATUS(3, m_lpratio, s_lpratio, l_lpratio);
	END_COLUMN_MAP()

};


#endif