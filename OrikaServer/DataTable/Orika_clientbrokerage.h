//#include "stdafx.h"
#ifndef COrika_clientbrokerage_HEADER
#define COrika_clientbrokerage_HEADER
#include <atldbcli.h>
class COrika_clientbrokerage
{
public:
	TCHAR m_login[20];
	TCHAR m_symbolGroup[20];
	int   m_brokageType;
	double m_clientBrokage;
	double m_subBrokerBrokage; 
	double m_bokerBrokage;
	double m_companyBrokage;
		

	DBSTATUS s_login;
	DBSTATUS s_symbolGroup;
	DBSTATUS s_brokageType;
	DBSTATUS s_clientBrokage;
	DBSTATUS s_subBrokerBrokage;
	DBSTATUS s_bokerBrokage;
	DBSTATUS s_companyBrokage;


	DBLENGTH l_login;
	DBLENGTH l_symbolGroup;
	DBLENGTH l_brokageType;
	DBLENGTH l_clientBrokage;
	DBLENGTH l_subBrokerBrokage;
	DBLENGTH l_bokerBrokage;
	DBLENGTH l_companyBrokage;
// Column binding map
BEGIN_COLUMN_MAP(COrika_clientbrokerage)	
	COLUMN_ENTRY_LENGTH_STATUS(1,m_login,s_login,l_login);
	COLUMN_ENTRY_LENGTH_STATUS(2,m_symbolGroup,s_symbolGroup,l_symbolGroup);
	COLUMN_ENTRY_LENGTH_STATUS(3,m_brokageType,s_brokageType,l_brokageType);
	COLUMN_ENTRY_LENGTH_STATUS(4,m_clientBrokage,s_clientBrokage,l_clientBrokage);
	COLUMN_ENTRY_LENGTH_STATUS(5,m_subBrokerBrokage,s_subBrokerBrokage,l_subBrokerBrokage);
	COLUMN_ENTRY_LENGTH_STATUS(6,m_bokerBrokage,s_bokerBrokage,l_bokerBrokage);
	COLUMN_ENTRY_LENGTH_STATUS(7,m_companyBrokage,s_companyBrokage,l_companyBrokage);
END_COLUMN_MAP() 
	
};



class COrika_LpRatioData
{
public:
	TCHAR m_symbolgroup[32];
	double m_lots;
	double m_ratio;
	double m_stepup;
	double m_perfect;
	double m_rounded;

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
	BEGIN_COLUMN_MAP(COrika_LpRatioData)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_symbolgroup, s_symbolgroup, l_symbolgroup);
		COLUMN_ENTRY_LENGTH_STATUS(2, m_lots, s_lots, l_lots);
		COLUMN_ENTRY_LENGTH_STATUS(3, m_ratio, s_ratio, l_ratio);
		COLUMN_ENTRY_LENGTH_STATUS(4, m_stepup, s_stepup, l_stepup);
		COLUMN_ENTRY_LENGTH_STATUS(5, m_perfect, s_perfect, l_perfect);
		COLUMN_ENTRY_LENGTH_STATUS(6, m_rounded, s_rounded, l_rounded);	
	END_COLUMN_MAP()

};







class COrika_dealsHighLow
{
public:
	TCHAR m_symbol[32];		
	double m_high;
	double m_low;	

	DBSTATUS s_symbol;
	DBSTATUS s_high;
	DBSTATUS s_low;

	DBLENGTH l_symbol;
	DBLENGTH l_high;
	DBLENGTH l_low;
	// Column binding map
	BEGIN_COLUMN_MAP(COrika_dealsHighLow)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_symbol, s_symbol, l_symbol);
		COLUMN_ENTRY_LENGTH_STATUS(2, m_high, s_high, s_low);
		COLUMN_ENTRY_LENGTH_STATUS(3, m_low, l_high, l_low);
	END_COLUMN_MAP()
};



class COrika_LpPriotity
{
public:
	TCHAR   m_symbolgroup[32]; 
	TCHAR   m_lpname[50]; 
	TCHAR   m_lpsymbol[32]; 
	double  m_lppriority; 
	double  m_lpmaxlots; 
	TCHAR   m_lpsendingtype[32]; 
	double  m_lpratio;

	DBSTATUS s_symbolgroup;
	DBSTATUS s_lpname;
	DBSTATUS s_lpsymbol;
	DBSTATUS s_lppriority;
	DBSTATUS s_lpmaxlots;
	DBSTATUS s_lpsendingtype;
	DBSTATUS s_lpratio;


	DBLENGTH l_symbolgroup;
	DBLENGTH l_lpname;
	DBLENGTH l_lpsymbol;
	DBLENGTH l_lppriority;
	DBLENGTH l_lpmaxlots;
	DBLENGTH l_lpsendingtype;
	DBLENGTH l_lpratio;
	// Column binding map
	BEGIN_COLUMN_MAP(COrika_LpPriotity)
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