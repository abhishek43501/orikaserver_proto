//#include "stdafx.h"
#ifndef COrika_symbolgroup_HEADER
#define COrika_symbolgroup_HEADER
#include <atldbcli.h>
class COrika_symbolgroup
{
public:
	TCHAR m_symbolGroup[20];
	int   m_symbolWiseBuyLimit;
	int   m_symbolWiseSellLimit;
	int   m_symbolWisePendingOrderEnableDisable;
	int   m_symbolPositionLimit;
	double m_symbolPendingOrderDiffFromBidAsk;
	double m_symbolMargin;
	double m_expectedchange;
	int   m_binsize;
	int   m_pocketnumber;
	double m_maxlimitOnsameprice;
		
	DBSTATUS  s_symbolGroup;
	DBSTATUS  s_symbolWiseBuyLimit;
	DBSTATUS  s_symbolWiseSellLimit;
	DBSTATUS  s_symbolWisePendingOrderEnableDisable;
	DBSTATUS  s_symbolPositionLimit;
	DBSTATUS  s_symbolPendingOrderDiffFromBidAsk;
	DBSTATUS  s_symbolMargin;
	DBSTATUS  s_expectedchange;
	DBSTATUS  s_binsize;
	DBSTATUS  s_pocketnumber;
	DBSTATUS  s_maxlimitOnsameprice;

	DBLENGTH  l_symbolGroup;
	DBLENGTH  l_symbolWiseBuyLimit;
	DBLENGTH  l_symbolWiseSellLimit;
	DBLENGTH  l_symbolWisePendingOrderEnableDisable;
	DBLENGTH  l_symbolPositionLimit;
	DBLENGTH  l_symbolPendingOrderDiffFromBidAsk;
	DBLENGTH  l_symbolMargin;
	DBLENGTH  l_expectedchange;
	DBLENGTH  l_binsize;
	DBLENGTH  l_pocketnumber;
	DBLENGTH  l_maxlimitOnsameprice;
// Column binding map
BEGIN_COLUMN_MAP(COrika_symbolgroup)		
	COLUMN_ENTRY_LENGTH_STATUS(1,m_symbolGroup,s_symbolGroup,l_symbolGroup);
	COLUMN_ENTRY_LENGTH_STATUS(2,m_symbolWiseBuyLimit,s_symbolWiseBuyLimit,l_symbolWiseBuyLimit);
	COLUMN_ENTRY_LENGTH_STATUS(3,m_symbolWiseSellLimit,s_symbolWiseSellLimit,l_symbolWiseSellLimit);
	COLUMN_ENTRY_LENGTH_STATUS(4,m_symbolWisePendingOrderEnableDisable,s_symbolWisePendingOrderEnableDisable,l_symbolWisePendingOrderEnableDisable);
	COLUMN_ENTRY_LENGTH_STATUS(5,m_symbolPositionLimit,s_symbolPositionLimit,l_symbolPositionLimit);
	COLUMN_ENTRY_LENGTH_STATUS(6,m_symbolPendingOrderDiffFromBidAsk,s_symbolPendingOrderDiffFromBidAsk,l_symbolPendingOrderDiffFromBidAsk);
	COLUMN_ENTRY_LENGTH_STATUS(7,m_symbolMargin,s_symbolMargin,l_symbolMargin);
	COLUMN_ENTRY_LENGTH_STATUS(8,m_expectedchange,s_expectedchange,l_expectedchange);
	COLUMN_ENTRY_LENGTH_STATUS(9,m_binsize,s_binsize,l_binsize);
	COLUMN_ENTRY_LENGTH_STATUS(10,m_pocketnumber,s_pocketnumber,l_pocketnumber);
	COLUMN_ENTRY_LENGTH_STATUS(11,m_maxlimitOnsameprice,s_maxlimitOnsameprice,l_maxlimitOnsameprice);
END_COLUMN_MAP() 
	
};
#endif