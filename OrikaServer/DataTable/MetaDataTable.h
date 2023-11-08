//#include "stdafx.h"
#ifndef		CMetadataTable_HEADER
#define		CMetadataTable_HEADER
#include <atldbcli.h>
class CMetadataTable
{
public:
	TCHAR m_key[50];
    TCHAR m_displayValue[50];
	TCHAR m_dataType[30];
    int m_size ;
	int m_decimalLocator;
    int m_editable ;
    int m_hidden ;
    int m_sequence ;
	TCHAR m_metadatatype[50];
	TCHAR m_inputcontrol[30];
	int m_iscurrency;
	int m_shouldanimate;
	int m_calculateTotal;
	int m_GroupingEnabled;
	int m_PivotEnabled;
	TCHAR m_GroupingColumns[255];
	int m_EnableRowGroup; 
	int m_EnablePivot; 
	int m_EnableValue;
	int   m_template;
	
	
	DBSTATUS  s_key;
    DBSTATUS  s_displayValue;
	DBSTATUS  s_dataType;
	DBSTATUS  s_size ;
	DBSTATUS  s_decimalLocator;
    DBSTATUS  s_editable ;
    DBSTATUS  s_hidden ;
    DBSTATUS  s_sequence ;
	DBSTATUS  s_metadatatype;
	DBSTATUS  s_inputcontrol;
	DBSTATUS  s_iscurrency;
	DBSTATUS  s_shouldanimate;
	DBSTATUS  s_calculateTotal;
	DBSTATUS  s_GroupingEnabled;
	DBSTATUS  s_PivotEnabled;
	DBSTATUS  s_GroupingColumns;
	DBSTATUS  s_EnableRowGroup;
	DBSTATUS  s_EnablePivot;
	DBSTATUS  s_EnableValue;
	DBSTATUS  s_template;

	DBLENGTH   l_key;
    DBLENGTH   l_displayValue;
	DBLENGTH   l_dataType;
    DBLENGTH   l_size ;
	DBLENGTH   l_decimalLocator;
    DBLENGTH   l_editable ;
    DBLENGTH   l_hidden ;
    DBLENGTH   l_sequence ;
	DBLENGTH   l_metadatatype;
	DBLENGTH   l_inputcontrol;
	DBLENGTH   l_iscurrency;
	DBLENGTH   l_shouldanimate;
	DBLENGTH   l_calculateTotal;
	DBLENGTH   l_GroupingEnabled;
	DBLENGTH   l_PivotEnabled;
	DBLENGTH   l_GroupingColumns;
	DBLENGTH   l_EnableRowGroup;
	DBLENGTH   l_EnablePivot;
	DBLENGTH   l_EnableValue;
	DBLENGTH	l_template;
// Column binding map
BEGIN_COLUMN_MAP(CMetadataTable)		
	COLUMN_ENTRY_LENGTH_STATUS(1,m_key,s_key,l_key);
	COLUMN_ENTRY_LENGTH_STATUS(2,m_displayValue,s_displayValue,l_displayValue);
	COLUMN_ENTRY_LENGTH_STATUS(3,m_dataType,s_dataType,l_dataType);
	COLUMN_ENTRY_LENGTH_STATUS(4,m_size ,s_size,l_size);
	COLUMN_ENTRY_LENGTH_STATUS(5,m_decimalLocator,s_decimalLocator,l_decimalLocator);
	COLUMN_ENTRY_LENGTH_STATUS(6,m_editable ,s_editable,l_editable);
	COLUMN_ENTRY_LENGTH_STATUS(7,m_hidden ,s_hidden,l_hidden);
	COLUMN_ENTRY_LENGTH_STATUS(8,m_sequence,s_sequence,l_sequence);
	COLUMN_ENTRY_LENGTH_STATUS(9,m_metadatatype,s_metadatatype,l_metadatatype);
	COLUMN_ENTRY_LENGTH_STATUS(10,m_inputcontrol,s_inputcontrol,l_inputcontrol);	

	COLUMN_ENTRY_LENGTH_STATUS(11,m_iscurrency,s_iscurrency,l_iscurrency);
	COLUMN_ENTRY_LENGTH_STATUS(12,m_shouldanimate,s_shouldanimate,l_shouldanimate);	
	//calculateTotal
	COLUMN_ENTRY_LENGTH_STATUS(13,m_calculateTotal,s_calculateTotal,l_calculateTotal);	
	COLUMN_ENTRY_LENGTH_STATUS(14, m_GroupingEnabled, s_GroupingEnabled, l_GroupingEnabled);
	COLUMN_ENTRY_LENGTH_STATUS(15, m_PivotEnabled, s_PivotEnabled, l_PivotEnabled);
	COLUMN_ENTRY_LENGTH_STATUS(16, m_GroupingColumns, s_GroupingColumns, l_GroupingColumns);


	COLUMN_ENTRY_LENGTH_STATUS(17, m_EnableRowGroup, s_EnableRowGroup, l_EnableRowGroup);
	COLUMN_ENTRY_LENGTH_STATUS(18, m_EnablePivot, s_EnablePivot, l_EnablePivot);
	COLUMN_ENTRY_LENGTH_STATUS(19, m_EnableValue, s_EnableValue, l_EnableValue);
	COLUMN_ENTRY_LENGTH_STATUS(20, m_template, s_template, l_template);

END_COLUMN_MAP()
	
};

//class CMetadataTable
//{
//public:
//	TCHAR m_key[50];
//	TCHAR m_displayValue[50];
//	TCHAR m_dataType[30];
//	int m_size;
//	int m_decimalLocator;
//	int m_editable;
//	int m_hidden;
//	int m_sequence;
//	TCHAR m_metadatatype[50];
//	TCHAR m_inputcontrol[30];
//	int m_iscurrency;
//	int m_shouldanimate;
//	int m_calculateTotal;
//	int m_GroupingEnabled;
//	int m_PivotEnabled;
//	TCHAR m_GroupingColumns[255];
//	int m_EnableRowGroup;
//	int m_EnablePivot;
//	int m_EnableValue;
//
//
//	DBSTATUS  s_key;
//	DBSTATUS  s_displayValue;
//	DBSTATUS  s_dataType;
//	DBSTATUS  s_size;
//	DBSTATUS  s_decimalLocator;
//	DBSTATUS  s_editable;
//	DBSTATUS  s_hidden;
//	DBSTATUS  s_sequence;
//	DBSTATUS  s_metadatatype;
//	DBSTATUS  s_inputcontrol;
//	DBSTATUS  s_iscurrency;
//	DBSTATUS  s_shouldanimate;
//	DBSTATUS  s_calculateTotal;
//	DBSTATUS  s_GroupingEnabled;
//	DBSTATUS  s_PivotEnabled;
//	DBSTATUS  s_GroupingColumns;
//	DBSTATUS  s_EnableRowGroup;
//	DBSTATUS  s_EnablePivot;
//	DBSTATUS  s_EnableValue;
//
//	DBLENGTH   l_key;
//	DBLENGTH   l_displayValue;
//	DBLENGTH   l_dataType;
//	DBLENGTH   l_size;
//	DBLENGTH   l_decimalLocator;
//	DBLENGTH   l_editable;
//	DBLENGTH   l_hidden;
//	DBLENGTH   l_sequence;
//	DBLENGTH   l_metadatatype;
//	DBLENGTH   l_inputcontrol;
//	DBLENGTH   l_iscurrency;
//	DBLENGTH   l_shouldanimate;
//	DBLENGTH   l_calculateTotal;
//	DBLENGTH   l_GroupingEnabled;
//	DBLENGTH   l_PivotEnabled;
//	DBLENGTH   l_GroupingColumns;
//	DBLENGTH   l_EnableRowGroup;
//	DBLENGTH   l_EnablePivot;
//	DBLENGTH   l_EnableValue;
//	// Column binding map
//	BEGIN_COLUMN_MAP(CMetadataTable)
//		COLUMN_ENTRY_LENGTH_STATUS(1, m_key, s_key, l_key);
//	COLUMN_ENTRY_LENGTH_STATUS(2, m_displayValue, s_displayValue, l_displayValue);
//	COLUMN_ENTRY_LENGTH_STATUS(3, m_dataType, s_dataType, l_dataType);
//	COLUMN_ENTRY_LENGTH_STATUS(4, m_size, s_size, l_size);
//	COLUMN_ENTRY_LENGTH_STATUS(5, m_decimalLocator, s_decimalLocator, l_decimalLocator);
//	COLUMN_ENTRY_LENGTH_STATUS(6, m_editable, s_editable, l_editable);
//	COLUMN_ENTRY_LENGTH_STATUS(7, m_hidden, s_hidden, l_hidden);
//	COLUMN_ENTRY_LENGTH_STATUS(8, m_sequence, s_sequence, l_sequence);
//	COLUMN_ENTRY_LENGTH_STATUS(9, m_metadatatype, s_metadatatype, l_metadatatype);
//	COLUMN_ENTRY_LENGTH_STATUS(10, m_inputcontrol, s_inputcontrol, l_inputcontrol);
//
//	COLUMN_ENTRY_LENGTH_STATUS(11, m_iscurrency, s_iscurrency, l_iscurrency);
//	COLUMN_ENTRY_LENGTH_STATUS(12, m_shouldanimate, s_shouldanimate, l_shouldanimate);
//	//calculateTotal
//	COLUMN_ENTRY_LENGTH_STATUS(13, m_calculateTotal, s_calculateTotal, l_calculateTotal);
//	COLUMN_ENTRY_LENGTH_STATUS(14, m_GroupingEnabled, s_GroupingEnabled, l_GroupingEnabled);
//	COLUMN_ENTRY_LENGTH_STATUS(15, m_PivotEnabled, s_PivotEnabled, l_PivotEnabled);
//	COLUMN_ENTRY_LENGTH_STATUS(16, m_GroupingColumns, s_GroupingColumns, l_GroupingColumns);
//
//
//	COLUMN_ENTRY_LENGTH_STATUS(17, m_EnableRowGroup, s_EnableRowGroup, l_EnableRowGroup);
//	COLUMN_ENTRY_LENGTH_STATUS(18, m_EnablePivot, s_EnablePivot, l_EnablePivot);
//	COLUMN_ENTRY_LENGTH_STATUS(19, m_EnableValue, s_EnableValue, l_EnableValue);
//
//	END_COLUMN_MAP()
//
//};
//
//


class CChieldMetadataTable
{
public:
	TCHAR m_childrenColumnKey[50];
	TCHAR m_displayValue[50];
	TCHAR m_dataType[30];
	int m_size;
	int m_enablePivot;
	int m_enableRowGroup;
	int m_enableValue;
	int m_shouldAnimate;
	int m_isCurrency;
	TCHAR m_inputcontrol[30];
	int m_sequence;
	int m_editable;
	int m_decimalLocator;
	int m_colortype;


	DBSTATUS  s_childrenColumnKey;
	DBSTATUS  s_displayValue;
	DBSTATUS  s_dataType;
	DBSTATUS  s_size;
	DBSTATUS  s_enablePivot;
	DBSTATUS  s_enableRowGroup;
	DBSTATUS  s_enableValue;
	DBSTATUS  s_shouldAnimate;
	DBSTATUS  s_isCurrency;
	DBSTATUS  s_inputcontrol;
	DBSTATUS  s_sequence;
	DBSTATUS  s_editable;
	DBSTATUS  s_decimalLocator;
	DBSTATUS  s_colortype;

	DBLENGTH  l_childrenColumnKey;
	DBLENGTH  l_displayValue;
	DBLENGTH  l_dataType;
	DBLENGTH  l_size;
	DBLENGTH  l_enablePivot;
	DBLENGTH  l_enableRowGroup;
	DBLENGTH  l_enableValue;
	DBLENGTH  l_shouldAnimate;
	DBLENGTH  l_isCurrency;
	DBLENGTH  l_inputcontrol;
	DBLENGTH  l_sequence;
	DBLENGTH  l_editable;
	DBLENGTH  l_decimalLocator;
	DBLENGTH  l_colortype;
	// Column binding map
	BEGIN_COLUMN_MAP(CChieldMetadataTable)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_childrenColumnKey, s_childrenColumnKey, l_childrenColumnKey);
		COLUMN_ENTRY_LENGTH_STATUS(2, m_displayValue, s_displayValue, l_displayValue);
		COLUMN_ENTRY_LENGTH_STATUS(3, m_dataType, s_dataType, l_dataType);
		COLUMN_ENTRY_LENGTH_STATUS(4, m_size, s_size, l_size);
		COLUMN_ENTRY_LENGTH_STATUS(5, m_enablePivot, s_enablePivot, l_enablePivot);
		COLUMN_ENTRY_LENGTH_STATUS(6, m_enableRowGroup, s_enableRowGroup, l_enableRowGroup);
		COLUMN_ENTRY_LENGTH_STATUS(7, m_enableValue, s_enableValue, l_enableValue);
		COLUMN_ENTRY_LENGTH_STATUS(8, m_shouldAnimate, s_shouldAnimate, l_shouldAnimate);
		COLUMN_ENTRY_LENGTH_STATUS(9, m_isCurrency, s_isCurrency, l_isCurrency);
		COLUMN_ENTRY_LENGTH_STATUS(10, m_inputcontrol, s_inputcontrol, l_inputcontrol);
		COLUMN_ENTRY_LENGTH_STATUS(11, m_sequence, s_sequence, l_sequence);
		COLUMN_ENTRY_LENGTH_STATUS(12, m_editable, s_editable, l_editable);
		COLUMN_ENTRY_LENGTH_STATUS(13, m_decimalLocator, s_decimalLocator, l_decimalLocator);
		COLUMN_ENTRY_LENGTH_STATUS(14, m_colortype, s_colortype, l_colortype);
	END_COLUMN_MAP()

};




class CToggleView
{
public:
	TCHAR m_userlogin[15];
	TCHAR m_metadatatype[100];
	TCHAR m_toggleViewName[100];
	TCHAR m_colId[50];
	int   m_rowGroupIndex;
	TCHAR m_aggFunc[30];
	int   m_hide; 
	int   m_width; 
	int   m_povitIndex; 
	TCHAR    m_pinned[15];
	int   m_viewSerialNo;
	int   m_template;


	DBSTATUS s_userlogin;
	DBSTATUS s_metadatatype;
	DBSTATUS s_toggleViewName;
	DBSTATUS s_colId;
	DBSTATUS s_rowGroupIndex;
	DBSTATUS s_aggFunc;
	DBSTATUS s_hide;
	DBSTATUS s_width;
	DBSTATUS s_povitIndex;
	DBSTATUS s_pinned;
	DBSTATUS  s_viewSerialNo;
	DBSTATUS  s_template;

	DBLENGTH l_userlogin;
	DBLENGTH l_metadatatype;
	DBLENGTH l_toggleViewName;
	DBLENGTH l_colId;
	DBLENGTH l_rowGroupIndex;
	DBLENGTH l_aggFunc;
	DBLENGTH l_hide;
	DBLENGTH l_width;
	DBLENGTH l_povitIndex;
	DBLENGTH l_pinned;
	DBLENGTH l_viewSerialNo;
	DBLENGTH l_template;
	// Column binding map
	BEGIN_COLUMN_MAP(CToggleView)		
		COLUMN_ENTRY_LENGTH_STATUS(1, m_userlogin, s_userlogin, l_userlogin);
		COLUMN_ENTRY_LENGTH_STATUS(2, m_metadatatype,s_metadatatype,l_metadatatype);
		COLUMN_ENTRY_LENGTH_STATUS(3, m_toggleViewName,s_toggleViewName,l_toggleViewName);
		COLUMN_ENTRY_LENGTH_STATUS(4, m_colId,s_colId,l_colId);
		COLUMN_ENTRY_LENGTH_STATUS(5, m_rowGroupIndex,s_rowGroupIndex,l_rowGroupIndex);
		COLUMN_ENTRY_LENGTH_STATUS(6, m_aggFunc,s_aggFunc,l_aggFunc);

		COLUMN_ENTRY_LENGTH_STATUS(7, m_hide,s_hide,l_hide);
		COLUMN_ENTRY_LENGTH_STATUS(8, m_width,s_width,l_width);
		COLUMN_ENTRY_LENGTH_STATUS(9, m_povitIndex,s_povitIndex,l_povitIndex);
		COLUMN_ENTRY_LENGTH_STATUS(10, m_pinned,s_pinned,l_pinned);
		COLUMN_ENTRY_LENGTH_STATUS(11, m_viewSerialNo, s_viewSerialNo, l_viewSerialNo);
		COLUMN_ENTRY_LENGTH_STATUS(12, m_template, s_template, l_template);
	END_COLUMN_MAP()

};







class CToggleView_Details_Table
{
public:
	int m_isDarkMode;
	int m_isFilterOn;
	int m_isColorOn;
	int m_isPivot;
	

	DBSTATUS s_isDarkMode;
	DBSTATUS s_isFilterOn;
	DBSTATUS s_isColorOn;
	DBSTATUS  s_isPivot;
	

	DBLENGTH l_isDarkMode;
	DBLENGTH l_isFilterOn;
	DBLENGTH l_isColorOn;
	DBLENGTH l_isPivot;
	
	// Column binding map
	BEGIN_COLUMN_MAP(CToggleView_Details_Table)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_isDarkMode, s_isDarkMode, l_isDarkMode);
		COLUMN_ENTRY_LENGTH_STATUS(2, m_isFilterOn, s_isFilterOn, l_isFilterOn);
		COLUMN_ENTRY_LENGTH_STATUS(3, m_isColorOn, s_isColorOn, l_isColorOn);	
		COLUMN_ENTRY_LENGTH_STATUS(4, m_isPivot, s_isPivot, l_isPivot);	
	END_COLUMN_MAP()

};




class CFilterState_Table
{
public:
	TCHAR m_colID[30];
	TCHAR m_filterjson[255];
	


	DBSTATUS s_colID;
	DBSTATUS s_filterjson;

	DBLENGTH l_colID;
	DBLENGTH l_filterjson;
	// Column binding map
	BEGIN_COLUMN_MAP(CFilterState_Table)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_colID, s_colID, l_colID);
		COLUMN_ENTRY_LENGTH_STATUS(2, m_filterjson, s_filterjson, l_filterjson);
	END_COLUMN_MAP()

};


class CToggleViewshortingState_table
{
public:
	TCHAR m_colID[30];
	TCHAR m_sort[50];

	DBSTATUS s_colID;
	DBSTATUS s_sort;

	DBLENGTH l_colID;
	DBLENGTH l_sort;
	// Column binding map
	BEGIN_COLUMN_MAP(CToggleViewshortingState_table)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_colID, s_colID, l_colID);
		COLUMN_ENTRY_LENGTH_STATUS(2, m_sort, s_sort, l_sort);
	END_COLUMN_MAP()

};
#endif