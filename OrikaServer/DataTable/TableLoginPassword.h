//#include "stdafx.h"
#ifndef		CTableLoginPassword_HEADER
#define		CTableLoginPassword_HEADER
#include <atldbcli.h>
class CTableLoginPassword
{
public:
	TCHAR  m_password[40];			
	TCHAR  m_name[200];	
	DBSTATUS   s_password;	
	DBSTATUS   s_name;	
	DBLENGTH    l_password;
	DBLENGTH    l_name;
	
// Column binding map
BEGIN_COLUMN_MAP(CTableLoginPassword)		
	COLUMN_ENTRY_LENGTH_STATUS(1,m_password,s_password,l_password);	
	COLUMN_ENTRY_LENGTH_STATUS(2,m_name,s_name,l_name);	
END_COLUMN_MAP()
	
};



class CTableHDno
{
public:
	TCHAR  m_userlogin[20];			
	TCHAR  m_HDno[300];	
	DBSTATUS   s_userlogin;	
	DBSTATUS   s_HDno;	
	DBLENGTH    l_userlogin;
	DBLENGTH    l_HDno;
	
// Column binding map
BEGIN_COLUMN_MAP(CTableHDno)		
	COLUMN_ENTRY_LENGTH_STATUS(1,m_userlogin,s_userlogin,l_userlogin);	
	COLUMN_ENTRY_LENGTH_STATUS(2,m_HDno,s_HDno,l_HDno);	
END_COLUMN_MAP()
	
};
#endif