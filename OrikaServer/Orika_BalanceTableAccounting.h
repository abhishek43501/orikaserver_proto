//#include "stdafx.h"
#ifndef BALANCETABLEACCOUNTING_HEADER
#define BALANCETABLEACCOUNTING_HEADER
#include <atldbcli.h>
class CBalanceTableAccounting
{
public:
	TCHAR m_login[20];
	TCHAR m_symbol[32];
	int   m_entryDeal;
	int   m_entryTime;
	int   m_entryOrder;	
	int   m_entryAction;
	int   m_entryVolume;
	double m_entryPrice;
	int   m_exitDeal;
	int   m_exitTime;
	int   m_exitOrder;
	int   m_exitAction;
	int   m_exitVolume;
	double m_exitPrice;
	double m_plPointLoss;
	double m_plPointProfit;
	double m_multiplyer;
	double m_clientBalance;
	double m_subBrokerBalance;
	double m_brokerBalance;
	double m_extraGroupBalance; 
	double m_companyBalance;
			

	DBSTATUS s_login;
	DBSTATUS s_symbol;
	DBSTATUS s_entryDeal;
	DBSTATUS s_entryTime;
	DBSTATUS s_entryOrder;	
	DBSTATUS s_entryAction;
	DBSTATUS s_entryVolume;
	DBSTATUS s_entryPrice;
	DBSTATUS s_exitDeal;
	DBSTATUS s_exitTime;
	DBSTATUS s_exitOrder;
	DBSTATUS s_exitAction;
	DBSTATUS s_exitVolume;
	DBSTATUS s_exitPrice;
	DBSTATUS s_plPointLoss;
	DBSTATUS s_plPointProfit;
	DBSTATUS s_multiplyer;
	DBSTATUS s_clientBalance;
	DBSTATUS s_subBrokerBalance;
	DBSTATUS s_brokerBalance;
	DBSTATUS s_extraGroupBalance; 
	DBSTATUS s_companyBalance;


	DBLENGTH l_login;
	DBLENGTH l_symbol;
	DBLENGTH l_entryDeal;
	DBLENGTH l_entryTime;
	DBLENGTH l_entryOrder;	
	DBLENGTH l_entryAction;
	DBLENGTH l_entryVolume;
	DBLENGTH l_entryPrice;
	DBLENGTH l_exitDeal;
	DBLENGTH l_exitTime;
	DBLENGTH l_exitOrder;
	DBLENGTH l_exitAction;
	DBLENGTH l_exitVolume;
	DBLENGTH l_exitPrice;
	DBLENGTH l_plPointLoss;
	DBLENGTH l_plPointProfit;
	DBLENGTH l_multiplyer;
	DBLENGTH l_clientBalance;
	DBLENGTH l_subBrokerBalance;
	DBLENGTH l_brokerBalance;
	DBLENGTH l_extraGroupBalance; 
	DBLENGTH l_companyBalance;
// Column binding map
BEGIN_COLUMN_MAP(CBalanceTableAccounting)			
	COLUMN_ENTRY_LENGTH_STATUS(1,m_login,s_login,l_login);
	COLUMN_ENTRY_LENGTH_STATUS(2,m_symbol,s_symbol,l_symbol);
	COLUMN_ENTRY_LENGTH_STATUS(3,m_entryDeal,s_entryDeal,l_entryDeal);
	COLUMN_ENTRY_LENGTH_STATUS(4,m_entryTime,s_entryTime,l_entryTime);
	COLUMN_ENTRY_LENGTH_STATUS(5,m_entryOrder,s_entryOrder,l_entryOrder);	
	COLUMN_ENTRY_LENGTH_STATUS(6,m_entryAction,s_entryAction,l_entryAction);
	COLUMN_ENTRY_LENGTH_STATUS(7,m_entryVolume,s_entryVolume,l_entryVolume);
	COLUMN_ENTRY_LENGTH_STATUS(8,m_entryPrice,s_entryPrice,l_entryPrice);
	COLUMN_ENTRY_LENGTH_STATUS(9,m_exitDeal,s_exitDeal,l_exitDeal);
	COLUMN_ENTRY_LENGTH_STATUS(10,m_exitTime,s_exitTime,l_exitTime);
	COLUMN_ENTRY_LENGTH_STATUS(11,m_exitOrder,s_exitOrder,l_exitOrder);
	COLUMN_ENTRY_LENGTH_STATUS(12,m_exitAction,s_exitAction,l_exitAction);
	COLUMN_ENTRY_LENGTH_STATUS(13,m_exitVolume,s_exitVolume,l_exitVolume);
	COLUMN_ENTRY_LENGTH_STATUS(14,m_exitPrice,s_exitPrice,l_exitPrice);
	COLUMN_ENTRY_LENGTH_STATUS(15,m_plPointLoss,s_plPointLoss,l_plPointLoss);
	COLUMN_ENTRY_LENGTH_STATUS(16,m_plPointProfit,s_plPointProfit,l_plPointProfit);
	COLUMN_ENTRY_LENGTH_STATUS(17,m_multiplyer,s_multiplyer,l_multiplyer);
	COLUMN_ENTRY_LENGTH_STATUS(18,m_clientBalance,s_clientBalance,l_clientBalance);
	COLUMN_ENTRY_LENGTH_STATUS(19,m_subBrokerBalance,s_subBrokerBalance,l_subBrokerBalance);
	COLUMN_ENTRY_LENGTH_STATUS(20,m_brokerBalance,s_brokerBalance,l_brokerBalance);
	COLUMN_ENTRY_LENGTH_STATUS(21,m_extraGroupBalance,s_extraGroupBalance,l_extraGroupBalance); 
	COLUMN_ENTRY_LENGTH_STATUS(22,m_companyBalance,s_companyBalance,l_companyBalance);
END_COLUMN_MAP()
	
};
#endif