//#include "stdafx.h"
#ifndef		CLoginTable_HEADER
#define		CLoginTable_HEADER
#include <atldbcli.h>
class CLoginTable
{
public:
	TCHAR m_loginUser[15];
	TCHAR m_Name[40];
	TCHAR m_group[15];
	TCHAR m_serialNo[200];
	TCHAR m_loginStatus[40];
		
	
	
	DBSTATUS s_loginUser;
	DBSTATUS s_Name;
	DBSTATUS s_group;
	DBSTATUS s_serialNo;
	DBSTATUS s_loginStatus;

	
	DBLENGTH l_loginUser;
	DBLENGTH l_Name;
	DBLENGTH l_group;
	DBLENGTH l_serialNo;
	DBLENGTH l_loginStatus;
// Column binding map
BEGIN_COLUMN_MAP(CLoginTable)			
	COLUMN_ENTRY_LENGTH_STATUS(1,m_loginUser,s_loginUser,l_loginUser);
	COLUMN_ENTRY_LENGTH_STATUS(2,m_Name,s_Name,l_Name);
	COLUMN_ENTRY_LENGTH_STATUS(3,m_group,s_group,l_group);
	COLUMN_ENTRY_LENGTH_STATUS(4,m_serialNo,s_serialNo,l_serialNo);
	COLUMN_ENTRY_LENGTH_STATUS(5,m_loginStatus,s_loginStatus,l_loginStatus);
END_COLUMN_MAP()
	
};




class CCommentChangeData
{
public:
	int	m_deal;
	TCHAR m_comment[50];
	TCHAR m_commentTo[50];
		
	
	
	DBSTATUS s_deal;
	DBSTATUS s_comment;
	DBSTATUS s_commentTo;

	
	DBLENGTH  l_deal;
	DBLENGTH  l_comment;
	DBLENGTH  l_commentTo;
// Column binding map
BEGIN_COLUMN_MAP(CCommentChangeData)			
	COLUMN_ENTRY_LENGTH_STATUS(1,m_deal,s_deal,l_deal);
	COLUMN_ENTRY_LENGTH_STATUS(2,m_comment,s_comment,l_comment);
	COLUMN_ENTRY_LENGTH_STATUS(3,m_commentTo,s_commentTo,l_commentTo);
	
END_COLUMN_MAP()
	
};

class CGroupTable
{
public:
	TCHAR m_group[200];
	int	  m_select;	
	
	DBSTATUS s_group;
	DBSTATUS s_select;
	
	DBLENGTH  l_group;
	DBLENGTH  l_select;
	// Column binding map
	BEGIN_COLUMN_MAP(CGroupTable)
		COLUMN_ENTRY_LENGTH_STATUS(1, m_group, s_group, l_group);
		COLUMN_ENTRY_LENGTH_STATUS(2, m_select, s_select, l_select);
	END_COLUMN_MAP()

};

#endif