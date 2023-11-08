//#include "stdafx.h"
#ifndef		SYMBOLMASTERTABLE_HEADER
#define		SYMBOLMASTERTABLE_HEADER
#include <atldbcli.h>
class CSymbolmastertable
{
public:
	TCHAR m_symbol[32];
	double m_closingPrice;
	double m_multiplayer;
	double m_symbolMargin ;
	double m_maxlimitOnsameprice; 
	double m_symbolPendingOrderDiffFromBidAsk ; 
	TCHAR m_ltpMapping[20];
	double m_upperCircuit;
	double m_lowerCircuit;
	TCHAR m_exchangeSymboll[20];
	TCHAR m_nextSymbol[32];
	int m_nselotsize;
	int m_mtlotsize;
	double m_maxAllowedVolume;
	double m_allowedLimitPerClient;
	double m_symbolPositionLimit;
	TCHAR m_symbolGroup[20];
	double m_binsize;
	double m_pocketnumber;
		
	
	
	DBSTATUS s_Symbol;
	DBSTATUS s_closingprice;
	DBSTATUS s_Multiplayer;
	DBSTATUS s_SymbolMargin;
	DBSTATUS s_maxlimitonsameprice; 
	DBSTATUS s_pendingorderratediff; 
	DBSTATUS s_ltpmapping;
	DBSTATUS s_uppercircuit;
	DBSTATUS s_lowercircuit;
	DBSTATUS s_exchangesymbol;
	DBSTATUS s_nextsymbol;
	DBSTATUS s_nselotsize;
	DBSTATUS s_mtlotsize;
	DBSTATUS s_MaxAllowedVolume;
	DBSTATUS s_AllowedLimitPerClient;
	DBSTATUS s_positionlimit;
	DBSTATUS s_symbolgroup;
	DBSTATUS s_binsize;
	DBSTATUS s_pocketnumber;


	DBLENGTH l_Symbol;
	DBLENGTH l_closingprice;
	DBLENGTH l_Multiplayer;
	DBLENGTH l_SymbolMargin;
	DBLENGTH l_maxlimitonsameprice; 
	DBLENGTH l_pendingorderratediff; 
	DBLENGTH l_ltpmapping;
	DBLENGTH l_uppercircuit;
	DBLENGTH l_lowercircuit;
	DBLENGTH l_exchangesymbol;
	DBLENGTH l_nextsymbol;
	DBLENGTH l_nselotsize;
	DBLENGTH l_mtlotsize;
	DBLENGTH l_MaxAllowedVolume;
	DBLENGTH l_AllowedLimitPerClient;
	DBLENGTH l_positionlimit;
	DBLENGTH l_symbolgroup;
	DBLENGTH l_binsize;
	DBLENGTH l_pocketnumber;
// Column binding map
BEGIN_COLUMN_MAP(CSymbolmastertable)		
	COLUMN_ENTRY_LENGTH_STATUS(1,m_symbol,s_Symbol,l_Symbol);
	COLUMN_ENTRY_LENGTH_STATUS(2,m_closingPrice,s_closingprice,l_closingprice);
	COLUMN_ENTRY_LENGTH_STATUS(3,m_multiplayer,s_Multiplayer,l_Multiplayer);
	COLUMN_ENTRY_LENGTH_STATUS(4,m_symbolMargin ,s_SymbolMargin,l_SymbolMargin);
	COLUMN_ENTRY_LENGTH_STATUS(5,m_maxlimitOnsameprice ,s_maxlimitonsameprice,l_maxlimitonsameprice);
	COLUMN_ENTRY_LENGTH_STATUS(6,m_symbolPendingOrderDiffFromBidAsk ,s_pendingorderratediff,l_pendingorderratediff);
	COLUMN_ENTRY_LENGTH_STATUS(7,m_ltpMapping,s_ltpmapping,l_ltpmapping);
	COLUMN_ENTRY_LENGTH_STATUS(8,m_upperCircuit,s_uppercircuit,l_uppercircuit);
	COLUMN_ENTRY_LENGTH_STATUS(9,m_lowerCircuit,s_lowercircuit,l_lowercircuit);
	COLUMN_ENTRY_LENGTH_STATUS(10,m_exchangeSymboll,s_exchangesymbol,l_exchangesymbol);
	COLUMN_ENTRY_LENGTH_STATUS(11,m_nextSymbol,s_nextsymbol,l_nextsymbol);
	COLUMN_ENTRY_LENGTH_STATUS(12,m_nselotsize,s_nselotsize,l_nselotsize);
	COLUMN_ENTRY_LENGTH_STATUS(13,m_mtlotsize,s_mtlotsize,l_mtlotsize);
	COLUMN_ENTRY_LENGTH_STATUS(14,m_maxAllowedVolume,s_MaxAllowedVolume,l_MaxAllowedVolume);
	COLUMN_ENTRY_LENGTH_STATUS(15,m_allowedLimitPerClient,s_AllowedLimitPerClient,l_AllowedLimitPerClient);
	COLUMN_ENTRY_LENGTH_STATUS(16,m_symbolPositionLimit,s_positionlimit,l_positionlimit);
	COLUMN_ENTRY_LENGTH_STATUS(17,m_symbolGroup,s_symbolgroup,l_symbolgroup);
	COLUMN_ENTRY_LENGTH_STATUS(18,m_binsize,s_binsize,l_binsize);
	COLUMN_ENTRY_LENGTH_STATUS(19,m_pocketnumber,s_pocketnumber,l_pocketnumber);
END_COLUMN_MAP()
	
};




class CSymbolmastertableForTickSubscribe
{
public:
	TCHAR m_symbol[32];	
	DBSTATUS s_Symbol;	
	DBLENGTH l_Symbol;	
	// Column binding map
	BEGIN_COLUMN_MAP(CSymbolmastertableForTickSubscribe)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_symbol, s_Symbol, l_Symbol);	
	END_COLUMN_MAP()

};
#endif