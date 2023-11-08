//#include "stdafx.h"
#ifndef POSITIONAVERAGEACCOUNTING_HEADER
#define POSITIONAVERAGEACCOUNTING_HEADER
#include <atldbcli.h>
class CPositionAverageAccounting
{
public:
	TCHAR  m_login[20];
	TCHAR  m_symbol[32];
	int    m_Action;
	int    m_Volume; 
	double  m_subBrokerVolume;
	double  m_brokerVolume;
	double  m_extraGroupVolume;
	double  m_companyVolume;
	double  m_WAvgPrice;	
	double  m_currentPrice;
	double  m_multiplyer;
	double  m_floatingProfit;
	double  m_subBrokerfloatingProfit;
	double  m_brokerfloatingProfit;
	double  m_extraGroupfloatingProfit;
	double  m_companyfloatingProfit;
		


	DBSTATUS   s_login;
	DBSTATUS   s_symbol;
	DBSTATUS   s_Action;
	DBSTATUS   s_Volume; 
	DBSTATUS   s_subBrokerVolume;
	DBSTATUS   s_brokerVolume;
	DBSTATUS   s_extraGroupVolume;
	DBSTATUS   s_companyVolume;
	DBSTATUS   s_WAvgPrice;	
	DBSTATUS   s_currentPrice;
	DBSTATUS   s_multiplyer;
	DBSTATUS   s_floatingProfit;
	DBSTATUS   s_subBrokerfloatingProfit;
	DBSTATUS   s_brokerfloatingProfit;
	DBSTATUS   s_extraGroupfloatingProfit;
	DBSTATUS   s_companyfloatingProfit;


	DBLENGTH    l_login;
	DBLENGTH    l_symbol;
	DBLENGTH    l_Action;
	DBLENGTH    l_Volume; 
	DBLENGTH    l_subBrokerVolume;
	DBLENGTH    l_brokerVolume;
	DBLENGTH    l_extraGroupVolume;
	DBLENGTH    l_companyVolume;
	DBLENGTH    l_WAvgPrice;	
	DBLENGTH    l_currentPrice;
	DBLENGTH    l_multiplyer;
	DBLENGTH    l_floatingProfit;
	DBLENGTH    l_subBrokerfloatingProfit;
	DBLENGTH    l_brokerfloatingProfit;
	DBLENGTH    l_extraGroupfloatingProfit;
	DBLENGTH    l_companyfloatingProfit;
// Column binding map
BEGIN_COLUMN_MAP(CPositionAverageAccounting)		
	COLUMN_ENTRY_LENGTH_STATUS(1,m_login,s_login,l_login);
	COLUMN_ENTRY_LENGTH_STATUS(2,m_symbol,s_symbol,l_symbol);
	COLUMN_ENTRY_LENGTH_STATUS(3,m_Action,s_Action,l_Action);
	COLUMN_ENTRY_LENGTH_STATUS(4,m_Volume,s_Volume,l_Volume); 
	COLUMN_ENTRY_LENGTH_STATUS(5,m_subBrokerVolume,s_subBrokerVolume,l_subBrokerVolume);
	COLUMN_ENTRY_LENGTH_STATUS(6,m_brokerVolume,s_brokerVolume,l_brokerVolume);
	COLUMN_ENTRY_LENGTH_STATUS(7,m_extraGroupVolume,s_extraGroupVolume,l_extraGroupVolume);
	COLUMN_ENTRY_LENGTH_STATUS(8,m_companyVolume,s_companyVolume,l_companyVolume);
	COLUMN_ENTRY_LENGTH_STATUS(9,m_WAvgPrice,s_WAvgPrice,l_WAvgPrice);	
	COLUMN_ENTRY_LENGTH_STATUS(10,m_currentPrice,s_currentPrice,l_currentPrice);
	COLUMN_ENTRY_LENGTH_STATUS(11,m_multiplyer,s_multiplyer,l_multiplyer);
	COLUMN_ENTRY_LENGTH_STATUS(12,m_floatingProfit,s_floatingProfit,l_floatingProfit);
	COLUMN_ENTRY_LENGTH_STATUS(13,m_subBrokerfloatingProfit,s_subBrokerfloatingProfit,l_subBrokerfloatingProfit);
	COLUMN_ENTRY_LENGTH_STATUS(14,m_brokerfloatingProfit,s_brokerfloatingProfit,l_brokerfloatingProfit);
	COLUMN_ENTRY_LENGTH_STATUS(15,m_extraGroupfloatingProfit,s_extraGroupfloatingProfit,l_extraGroupfloatingProfit);
	COLUMN_ENTRY_LENGTH_STATUS(16,m_companyfloatingProfit,s_companyfloatingProfit,l_companyfloatingProfit);	
END_COLUMN_MAP()
	
};



class CBrokerageClientAndScripWise
{
public:
	TCHAR     m_login[20];
	TCHAR     m_symbol[32];	
	double    m_clientBrokTotal;
	double    m_subBrokerBrokTotal; 
	double    m_brokerBrokTotal;
	double    m_comBrokTotal;
	
		


	DBSTATUS   s_login;
	DBSTATUS   s_symbol;
	DBSTATUS   s_clientBrokTotal;
	DBSTATUS   s_subBrokerBrokTotal; 
	DBSTATUS   s_brokerBrokTotal;
	DBSTATUS   s_comBrokTotal;
	


	DBLENGTH    l_login;
	DBLENGTH    l_symbol;
	DBLENGTH    l_clientBrokTotal;
	DBLENGTH    l_subBrokerBrokTotal; 
	DBLENGTH    l_brokerBrokTotal;
	DBLENGTH    l_comBrokTotal;
	
// Column binding map
BEGIN_COLUMN_MAP(CBrokerageClientAndScripWise)		
	COLUMN_ENTRY_LENGTH_STATUS(1,m_login,s_login,l_login);
	COLUMN_ENTRY_LENGTH_STATUS(2,m_symbol,s_symbol,l_symbol);
	COLUMN_ENTRY_LENGTH_STATUS(3,m_clientBrokTotal,s_clientBrokTotal,l_clientBrokTotal);
	COLUMN_ENTRY_LENGTH_STATUS(4,m_subBrokerBrokTotal,s_subBrokerBrokTotal,l_subBrokerBrokTotal); 
	COLUMN_ENTRY_LENGTH_STATUS(5,m_brokerBrokTotal,s_brokerBrokTotal,l_brokerBrokTotal);
	COLUMN_ENTRY_LENGTH_STATUS(6, m_comBrokTotal, s_comBrokTotal, l_comBrokTotal);
END_COLUMN_MAP()
	
};
#endif