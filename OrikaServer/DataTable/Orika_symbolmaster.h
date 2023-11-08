//#include "stdafx.h"
#ifndef COrika_symbolmaster_HEADER
#define COrika_symbolmaster_HEADER
#include <atldbcli.h>
class COrika_symbolmaster
{
public:
	TCHAR m_symbol[32];
	double m_closingPrice ;
	double m_multiplayer ;
	double m_symbolMargin ;
	double m_upperCircuit ;
	double m_lowerCircuit ;
	TCHAR m_ltpMapping[30];
	TCHAR m_nextSymbol[32];
	TCHAR m_exchangeSymbol[32];
	int   m_nselotsize ;
	int   m_mtlotsize  ;
	TCHAR m_symbolGroup[20];
	TCHAR m_ltpSymbol[32];

	TCHAR m_exchange[32];
	TCHAR m_international[32];
	TCHAR m_sector[32];
	TCHAR m_industry[32]; 
	double m_rmp;
	double m_QtyMulti;
	TCHAR m_page[32];
	TCHAR m_categary[32];
	TCHAR m_currencybase[32];
	TCHAR m_Group[32];



		
 	DBSTATUS  s_symbol;
	DBSTATUS  s_closingPrice ;
	DBSTATUS  s_multiplayer ;
	DBSTATUS  s_symbolMargin ;
	DBSTATUS  s_upperCircuit ;
	DBSTATUS  s_lowerCircuit ;
	DBSTATUS  s_ltpMapping;
	DBSTATUS  s_nextSymbol;
	DBSTATUS  s_exchangeSymbol;
	DBSTATUS  s_nselotsize ;
	DBSTATUS  s_mtlotsize  ;
	DBSTATUS  s_symbolGroup;
	DBSTATUS  s_ltpSymbol;	
	DBSTATUS s_exchange;
	DBSTATUS s_international;
	DBSTATUS s_sector;
	DBSTATUS s_industry;
	DBSTATUS s_rmp;
	DBSTATUS s_QtyMulti;
	DBSTATUS s_page;
	DBSTATUS s_categary;
	DBSTATUS s_currencybase;
	DBSTATUS s_Group;



	DBLENGTH   l_symbol;
	DBLENGTH   l_closingPrice ;
	DBLENGTH   l_multiplayer ;
	DBLENGTH   l_symbolMargin ;
	DBLENGTH   l_upperCircuit ;
	DBLENGTH   l_lowerCircuit ;
	DBLENGTH   l_ltpMapping;
	DBLENGTH   l_nextSymbol;
	DBLENGTH   l_exchangeSymbol;
	DBLENGTH   l_nselotsize ;
	DBLENGTH   l_mtlotsize  ;
	DBLENGTH   l_symbolGroup;
	DBLENGTH   l_ltpSymbol;
	DBLENGTH   l_exchange;
	DBLENGTH   l_international;
	DBLENGTH   l_sector;
	DBLENGTH   l_industry;
	DBLENGTH   l_rmp;
	DBLENGTH   l_QtyMulti;
	DBLENGTH   l_page;
	DBLENGTH   l_categary;
	DBLENGTH   l_currencybase;
	DBSTATUS   l_Group;
// Column binding map
BEGIN_COLUMN_MAP(COrika_symbolmaster)				
	COLUMN_ENTRY_LENGTH_STATUS(1,m_symbol,s_symbol,l_symbol);
	COLUMN_ENTRY_LENGTH_STATUS(2,m_closingPrice,s_closingPrice,l_closingPrice);
	COLUMN_ENTRY_LENGTH_STATUS(3,m_multiplayer,s_multiplayer,l_multiplayer);
	COLUMN_ENTRY_LENGTH_STATUS(4,m_symbolMargin,s_symbolMargin,l_symbolMargin);
	COLUMN_ENTRY_LENGTH_STATUS(5,m_upperCircuit,s_upperCircuit,l_upperCircuit);
	COLUMN_ENTRY_LENGTH_STATUS(6,m_lowerCircuit,s_lowerCircuit,l_lowerCircuit);
	COLUMN_ENTRY_LENGTH_STATUS(7,m_ltpMapping,s_ltpMapping,l_ltpMapping);
	COLUMN_ENTRY_LENGTH_STATUS(8,m_nextSymbol,s_nextSymbol,l_nextSymbol);
	COLUMN_ENTRY_LENGTH_STATUS(9,m_exchangeSymbol,s_exchangeSymbol,l_exchangeSymbol);
	COLUMN_ENTRY_LENGTH_STATUS(10,m_nselotsize,s_nselotsize,l_nselotsize);
	COLUMN_ENTRY_LENGTH_STATUS(11,m_mtlotsize,s_mtlotsize,l_mtlotsize);
	COLUMN_ENTRY_LENGTH_STATUS(12,m_symbolGroup,s_symbolGroup,l_symbolGroup);
	COLUMN_ENTRY_LENGTH_STATUS(13,m_ltpSymbol,s_ltpSymbol,l_ltpSymbol);

	COLUMN_ENTRY_LENGTH_STATUS(14,m_exchange,s_exchange, l_exchange);
	COLUMN_ENTRY_LENGTH_STATUS(15,m_international,s_international, l_international);
	COLUMN_ENTRY_LENGTH_STATUS(16,m_sector,s_sector, l_sector);
	COLUMN_ENTRY_LENGTH_STATUS(17,m_industry,s_industry,l_industry);
	COLUMN_ENTRY_LENGTH_STATUS(18,m_rmp,s_rmp, l_rmp);
	COLUMN_ENTRY_LENGTH_STATUS(19,m_QtyMulti,s_QtyMulti, l_QtyMulti);
	COLUMN_ENTRY_LENGTH_STATUS(20,m_page,s_page, l_page);
	COLUMN_ENTRY_LENGTH_STATUS(21, m_categary,s_categary,l_categary);
	COLUMN_ENTRY_LENGTH_STATUS(22, m_currencybase, s_currencybase, l_currencybase);
	COLUMN_ENTRY_LENGTH_STATUS(23, m_Group, s_Group, l_Group);
END_COLUMN_MAP() 	
};
#endif