//#include "stdafx.h"
#ifndef		CTableTabAndColumn_HEADER
#define		CTableTabAndColumn_HEADER
#include <atldbcli.h>
class CTableTabAndColumn
{
public:
	TCHAR  m_TabName[30];
	TCHAR  m_Key[40];
	TCHAR  m_displayValue[60];
		
	
	
	DBSTATUS   s_TabName;
	DBSTATUS   s_Key;
	DBSTATUS   s_displayValue;


	DBLENGTH    l_TabName;
	DBLENGTH    l_Key;
	DBLENGTH    l_displayValue;
// Column binding map
BEGIN_COLUMN_MAP(CTableTabAndColumn)		
	COLUMN_ENTRY_LENGTH_STATUS(1,m_TabName,s_TabName,l_TabName);
	COLUMN_ENTRY_LENGTH_STATUS(2,m_Key,s_Key,l_Key);
	COLUMN_ENTRY_LENGTH_STATUS(3,m_displayValue,s_displayValue,l_displayValue);	
END_COLUMN_MAP()
	
};



class CTableDashboardMetadata
{
public:
	TCHAR m_key[100];
	TCHAR m_headerName[100];
	TCHAR m_field[100];
	int   m_pinned;
	TCHAR m_colId[100];
	int   m_decimallocator;
	TCHAR m_datatype[100];
	int   m_collapsed;
	int   m_suppressMenu;
	int   m_editable;
	int   m_floatingFilter;
	int   m_resizable;
	int   m_sortable;
	int   m_readOnly;
	int   m_TemplateID;



	DBSTATUS s_key;
	DBSTATUS s_headerName;
	DBSTATUS s_field;
	DBSTATUS s_pinned;
	DBSTATUS s_colId;
	DBSTATUS s_decimallocator;
	DBSTATUS s_datatype;
	DBSTATUS s_collapsed;
	DBSTATUS s_suppressMenu;
	DBSTATUS s_editable;
	DBSTATUS s_floatingFilter;
	DBSTATUS s_resizable;
	DBSTATUS s_sortable;
	DBSTATUS s_readOnly;
	DBSTATUS s_TemplateID;


	DBLENGTH l_key;
	DBLENGTH l_headerName;
	DBLENGTH l_field;
	DBLENGTH l_pinned;
	DBLENGTH l_colId;
	DBLENGTH l_decimallocator;
	DBLENGTH l_datatype;
	DBLENGTH l_collapsed;
	DBLENGTH l_suppressMenu;
	DBLENGTH l_editable;
	DBLENGTH l_floatingFilter;
	DBLENGTH l_resizable;
	DBLENGTH l_sortable;
	DBLENGTH l_readOnly;
	DBLENGTH l_TemplateID;
	// Column binding map
	BEGIN_COLUMN_MAP(CTableDashboardMetadata)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_key, s_key, l_key);
		COLUMN_ENTRY_LENGTH_STATUS(2, m_headerName, s_headerName, l_headerName);
		COLUMN_ENTRY_LENGTH_STATUS(3, m_field, s_field, l_field);
		COLUMN_ENTRY_LENGTH_STATUS(4, m_pinned,s_pinned,l_pinned);
		COLUMN_ENTRY_LENGTH_STATUS(5, m_colId,s_colId,l_colId);
		COLUMN_ENTRY_LENGTH_STATUS(6, m_decimallocator,s_decimallocator,l_decimallocator);
		COLUMN_ENTRY_LENGTH_STATUS(7, m_datatype,s_datatype,l_datatype);
		COLUMN_ENTRY_LENGTH_STATUS(8, m_collapsed,s_collapsed,l_collapsed);
		COLUMN_ENTRY_LENGTH_STATUS(9, m_suppressMenu,s_suppressMenu,l_suppressMenu);
		COLUMN_ENTRY_LENGTH_STATUS(10, m_editable,s_editable,l_editable);
		COLUMN_ENTRY_LENGTH_STATUS(11, m_floatingFilter,s_floatingFilter,l_floatingFilter);
		COLUMN_ENTRY_LENGTH_STATUS(12, m_resizable,s_resizable,l_resizable);
		COLUMN_ENTRY_LENGTH_STATUS(13, m_sortable,s_sortable,l_sortable);
		COLUMN_ENTRY_LENGTH_STATUS(14, m_readOnly,s_readOnly,l_readOnly);
		COLUMN_ENTRY_LENGTH_STATUS(15, m_TemplateID,s_TemplateID,l_TemplateID);
	END_COLUMN_MAP()

};




class CTableMetadataGlobal
{
public:
	TCHAR	m_metadatatype[150];//metadatatype,
	TCHAR	m_field[150];//field,
	TCHAR	m_headerName[150];//headerName,
	int		m_minWidth;//minWidth,
	TCHAR 	m_filter[100];//filter,
	int		m_enableRowGroup;//enableRowGroup,
	int		m_enablePivot;//enablePivot,
	int		m_enableValue;//enableValue,
	int		m_decimalLocator;//decimalLocator,
	TCHAR 	m_dataType[30];//dataType,
	TCHAR 	m_inputcontrol[30];//inputcontrol,
	int	    m_editable;//editable,
	TCHAR 	m_colorType[30];//colorType,
	int		m_sequence;//sequence


	DBSTATUS s_metadatatype;
	DBSTATUS s_field;
	DBSTATUS s_headerName;
	DBSTATUS s_minWidth;
	DBSTATUS s_filter;
	DBSTATUS s_enableRowGroup;
	DBSTATUS s_enablePivot;
	DBSTATUS s_enableValue;
	DBSTATUS s_decimalLocator;
	DBSTATUS s_dataType;
	DBSTATUS s_inputcontrol;
	DBSTATUS s_editable;
	DBSTATUS s_colorType;
	DBSTATUS s_sequence;


	DBLENGTH l_metadatatype;
	DBLENGTH l_field;
	DBLENGTH l_headerName;
	DBLENGTH l_minWidth;
	DBLENGTH l_filter;
	DBLENGTH l_enableRowGroup;
	DBLENGTH l_enablePivot;
	DBLENGTH l_enableValue;
	DBLENGTH l_decimalLocator;
	DBLENGTH l_dataType;
	DBLENGTH l_inputcontrol;
	DBLENGTH l_editable;
	DBLENGTH l_colorType;
	DBLENGTH l_sequence;
	// Column binding map
	BEGIN_COLUMN_MAP(CTableMetadataGlobal)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_metadatatype, s_metadatatype, l_metadatatype);
		COLUMN_ENTRY_LENGTH_STATUS(2, m_field, s_field, l_field);
		COLUMN_ENTRY_LENGTH_STATUS(3, m_headerName, s_headerName, l_headerName);				
		COLUMN_ENTRY_LENGTH_STATUS(4, m_minWidth,s_minWidth,l_minWidth);
		COLUMN_ENTRY_LENGTH_STATUS(5, m_filter,s_filter,l_filter);
		COLUMN_ENTRY_LENGTH_STATUS(6, m_enableRowGroup,s_enableRowGroup,l_enableRowGroup);
		COLUMN_ENTRY_LENGTH_STATUS(7, m_enablePivot,s_enablePivot,l_enablePivot);
		COLUMN_ENTRY_LENGTH_STATUS(8, m_enableValue,s_enableValue,l_enableValue);
		COLUMN_ENTRY_LENGTH_STATUS(9, m_decimalLocator,s_decimalLocator,l_decimalLocator);
		COLUMN_ENTRY_LENGTH_STATUS(10, m_dataType,s_dataType,l_dataType);
		COLUMN_ENTRY_LENGTH_STATUS(11, m_inputcontrol,s_inputcontrol,l_inputcontrol);
		COLUMN_ENTRY_LENGTH_STATUS(12, m_editable,s_editable,l_editable);
		COLUMN_ENTRY_LENGTH_STATUS(13, m_colorType,s_colorType,l_colorType);
		COLUMN_ENTRY_LENGTH_STATUS(14, m_sequence,s_sequence,l_sequence);
	END_COLUMN_MAP()
};


#endif