//#include "stdafx.h"
#ifndef		CTABLEGROUPDATA_HEADER
#define		CTABLEGROUPDATA_HEADER
#include <atldbcli.h>
class CTableGroupData
{
public:
	TCHAR		m_group[20];  
	TCHAR		m_subgroup[20];  
	TCHAR		m_login[20];  


	DBSTATUS    s_group;    
	DBSTATUS    s_subgroup;    
	DBSTATUS    s_login;    

	DBLENGTH    l_group;
	DBLENGTH    l_subgroup;
	DBLENGTH    l_login;
    
// Column binding map
BEGIN_COLUMN_MAP(CTableGroupData)		
	COLUMN_ENTRY_LENGTH_STATUS(1,m_group,s_group,l_group);
	COLUMN_ENTRY_LENGTH_STATUS(2,m_subgroup,s_subgroup,l_subgroup);
	COLUMN_ENTRY_LENGTH_STATUS(3,m_login,s_login,l_login);
	
END_COLUMN_MAP()
	
};
#endif