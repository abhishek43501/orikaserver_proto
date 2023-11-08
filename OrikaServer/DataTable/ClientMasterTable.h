//#include "stdafx.h"
#ifndef CLIENTMASTERTABLE_HEADER
#define CLIENTMASTERTABLE_HEADER
#include <atldbcli.h>
class CClientmastertable
{
public:
	TCHAR m_login[20];
	TCHAR m_symbolGroup[20];
	TCHAR m_name[20];
	TCHAR m_broker[20];
	TCHAR m_subBroker[20];
	TCHAR m_extraGroup[20];
	double m_subBrokerPLRatio;
	double m_brokerPLRatio;
	double m_companyPLRatio;
	TCHAR m_brokageType[9];
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
	TCHAR m_colour[20];
		

















	
	
	DBSTATUS 	s_Login;
	DBSTATUS 	s_SymbolGroup;
	DBSTATUS 	s_name;
	DBSTATUS 	s_Broker;
	DBSTATUS 	s_Subbroker;
	DBSTATUS 	s_ExtraGroup;
	DBSTATUS 	s_SubbrokerPlration;
	DBSTATUS 	s_BrokerPlRatio;
	DBSTATUS 	s_CompanyplRatio;
	DBSTATUS 	s_brokeragetype;
	DBSTATUS 	s_clientbrokerage;
	DBSTATUS 	s_subbrokerbrokerage;
	DBSTATUS 	s_brokerbrokerage;
	DBSTATUS 	s_companybrokerage;
	DBSTATUS 	s_buylimit;
	DBSTATUS 	s_selllimit;
	DBSTATUS 	s_pendingorderenable;
	DBSTATUS 	s_positionlimit;
	DBSTATUS 	s_pendingorderratediff;
	DBSTATUS 	s_losslimit;
	DBSTATUS 	s_creditlimit;
	DBSTATUS 	s_Commentyn;
	DBSTATUS 	s_QtyLimitMultiplayer;
	DBSTATUS 	s_ignoretrader;
	DBSTATUS 	s_Colour;


	DBLENGTH 	l_Login;
	DBLENGTH 	l_SymbolGroup;
	DBLENGTH 	l_name;
	DBLENGTH 	l_Broker;
	DBLENGTH 	l_Subbroker;
	DBLENGTH 	l_ExtraGroup;
	DBLENGTH 	l_SubbrokerPlration;
	DBLENGTH 	l_BrokerPlRatio;
	DBLENGTH 	l_CompanyplRatio;
	DBLENGTH 	l_brokeragetype;
	DBLENGTH 	l_clientbrokerage;
	DBLENGTH 	l_subbrokerbrokerage;
	DBLENGTH 	l_brokerbrokerage;
	DBLENGTH 	l_companybrokerage;
	DBLENGTH 	l_buylimit;
	DBLENGTH 	l_selllimit;
	DBLENGTH 	l_pendingorderenable;
	DBLENGTH 	l_positionlimit;
	DBLENGTH 	l_pendingorderratediff;
	DBLENGTH 	l_losslimit;
	DBLENGTH 	l_creditlimit;
	DBLENGTH 	l_Commentyn;
	DBLENGTH 	l_QtyLimitMultiplayer;
	DBLENGTH 	l_ignoretrader;
	DBLENGTH 	l_Colour;				
// Column binding map
BEGIN_COLUMN_MAP(CClientmastertable)	
	COLUMN_ENTRY_LENGTH_STATUS(1,m_login,s_Login,l_Login);
	COLUMN_ENTRY_LENGTH_STATUS(2,m_symbolGroup,s_SymbolGroup,l_SymbolGroup);
	COLUMN_ENTRY_LENGTH_STATUS(3,m_name,s_name,l_name);
	COLUMN_ENTRY_LENGTH_STATUS(4,m_broker,s_Broker,l_Broker);
	COLUMN_ENTRY_LENGTH_STATUS(5,m_subBroker,s_Subbroker,l_Subbroker);
	COLUMN_ENTRY_LENGTH_STATUS(6,m_extraGroup,s_ExtraGroup,l_ExtraGroup);
	COLUMN_ENTRY_LENGTH_STATUS(7,m_subBrokerPLRatio,m_subBrokerPLRatio,l_SubbrokerPlration);
	COLUMN_ENTRY_LENGTH_STATUS(8,m_brokerPLRatio,s_BrokerPlRatio,l_BrokerPlRatio);
	COLUMN_ENTRY_LENGTH_STATUS(9,m_companyPLRatio,s_CompanyplRatio,l_CompanyplRatio);
	COLUMN_ENTRY_LENGTH_STATUS(10,m_brokageType,s_brokeragetype,l_brokeragetype);
	COLUMN_ENTRY_LENGTH_STATUS(11,m_clientBrokage,s_clientbrokerage,l_clientbrokerage);
	COLUMN_ENTRY_LENGTH_STATUS(12,m_subBrokerBrokage,s_subbrokerbrokerage,l_subbrokerbrokerage);
	COLUMN_ENTRY_LENGTH_STATUS(13,m_bokerBrokage,s_brokerbrokerage,l_brokerbrokerage);
	COLUMN_ENTRY_LENGTH_STATUS(14,m_companyBrokage,s_companybrokerage,l_companybrokerage);
	COLUMN_ENTRY_LENGTH_STATUS(15,m_symbolWiseBuyLimit,s_buylimit,l_buylimit);
	COLUMN_ENTRY_LENGTH_STATUS(16,m_symbolWiseSellLimit,s_selllimit,l_selllimit);
	COLUMN_ENTRY_LENGTH_STATUS(17,m_symbolWisePendingOrderEnableDisable,s_pendingorderenable,l_pendingorderenable);
	COLUMN_ENTRY_LENGTH_STATUS(18,m_symbolPositionLimit,s_positionlimit,l_positionlimit);
	COLUMN_ENTRY_LENGTH_STATUS(19,m_symbolPendingOrderDiffFromBidAsk,s_pendingorderratediff,l_pendingorderratediff);
	COLUMN_ENTRY_LENGTH_STATUS(20,m_lossLimit,s_losslimit,l_losslimit);
	COLUMN_ENTRY_LENGTH_STATUS(21,m_creditLimit,s_creditlimit,l_creditlimit);
	COLUMN_ENTRY_LENGTH_STATUS(22,m_comment,s_Commentyn,l_Commentyn);
	COLUMN_ENTRY_LENGTH_STATUS(23,m_qtyLimitMultiplayer,s_QtyLimitMultiplayer,l_QtyLimitMultiplayer);
	COLUMN_ENTRY_LENGTH_STATUS(24,m_ignoreTrader,s_ignoretrader,l_ignoretrader);
	COLUMN_ENTRY_LENGTH_STATUS(25,m_colour,s_Colour,l_Colour);
END_COLUMN_MAP()
	
};
#endif