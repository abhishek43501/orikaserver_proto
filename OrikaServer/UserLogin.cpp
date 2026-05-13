
// OrikaServer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "UserLogin.h"
#include "document.h"
#include "StaticClass.h"
#include "DataTable\TableLoginPassword.h"
using namespace rapidjson;
CUserLogin::CUserLogin()
{

}
CUserLogin::~CUserLogin()
{

}

bool CUserLogin::validateSerialno(CString loginuser,CString hdno)
{
	CString rval=L"";
	HRESULT hr=NULL ;
	CCommand<CAccessor<CTableHDno>> data_table;								
	if(!SUCCEEDED(hr))
	{
		return false ;
	}
	CString   strCommand=L"";	
	strCommand=L"";	
	strCommand.Format(L"select userlogin,HDno from orika_userLoginHDkeyMapping where userlogin='%s' and (HDno='%s' or HDno='*' );",loginuser,hdno);  
	
	CSession m_tempSession;
	////CStaticClass::m_logfile.LogEvent(L"L12");
	m_tempSession.Open(CStaticClass::connection);
	hr=data_table.Open(m_tempSession,(LPCTSTR)strCommand);
	if(FAILED(hr))
	{				
		return false;
	}
	CString m_password=L"" ;
	CString m_name=L"" ;
	int m_sl_validated = 0;
	while(hr=data_table.MoveNext()==S_OK)
	{			
		m_sl_validated = 1;
		////CStaticClass::m_logfile.LogEvent(L"UL12");			
	}

	m_tempSession.Close();
	if (m_sl_validated == 1)
	{
		return true;		
	}
	////CStaticClass::m_logfile.LogEvent(L"UL12");
	
	return false;
}

int CUserLogin::userValideate(const char* jsonString,CString ipAddress)
{
	rapidjson::Document d;	
	CString struser=L"";
	CString strpwd=L"";
	CString strtype=L"";
	CString strserialno=L"";
	if(!d.Parse<0>(jsonString).HasParseError())
	{		 
		if (d.HasMember("type")==true)
		{	
			const Value& Keyuser=d["type"];
			strtype=Keyuser.GetString();
			if (strtype=="login")
			{
				if (d.HasMember("login")==true && d.HasMember("pwd")==true && d.HasMember("serialNo")==true )
				{
					const Value& Keyuser=d["login"];
					struser=Keyuser.GetString();
					const Value& Keypwd=d["pwd"];
					strpwd=Keypwd.GetString();
					const Value& serialno=d["serialNo"];
					strserialno=serialno.GetString();


					if (validateSerialno(struser,strserialno)==false)
					{
						return 2;
					}



					CString rval=L"";
					HRESULT hr=NULL ;
					CCommand<CAccessor<CTableLoginPassword>> data_table;								
					if(!SUCCEEDED(hr))
					{
						return 1 ;
					}
					CString   strCommand=L"";	
					strCommand=L"";	
					strCommand.Format(L"select pwd,name from orika_userlogin where UserLogin='%s';",struser);
					
					CSession m_tempSession;
					////CStaticClass::m_logfile.LogEvent(L"L11");
					m_tempSession.Open(CStaticClass::connection);
					hr=data_table.Open(m_tempSession,(LPCTSTR)strCommand);
					if(FAILED(hr))
					{
						
						////CStaticClass::m_logfile.LogEvent(L"UL11");
						return 1;
					}
					CString m_password=L"" ;
					CString m_name=L"" ;
					while(hr=data_table.MoveNext()==S_OK)
					{			
						m_password=data_table.m_password ;
						m_name=data_table.m_name;
					}
					m_tempSession.Close();
					
					////CStaticClass::m_logfile.LogEvent(L"UL11");

					if (m_password==strpwd)
					{
						return 0;
					}
					else
					{
						return 1;
					}

					
			}
			else if (d.HasMember("login") == true && d.HasMember("pwd") == true && d.HasMember("api") == true)
			{
				const Value& Keyuser = d["login"];
				struser = Keyuser.GetString();
				const Value& Keypwd = d["pwd"];
				strpwd = Keypwd.GetString();
				const Value& api = d["api"];
				int intApi = 0;
				if (api.IsNumber())
				{
					intApi = api.GetInt();
				}				 
				if (intApi==0)
				{
					return 3;
				}
				CString rval = L"";
				HRESULT hr = NULL;
				CCommand<CAccessor<CTableLoginPassword>> data_table;
				if (!SUCCEEDED(hr))
				{
					return 1;
				}
				CString   strCommand = L"";
				strCommand = L"";
				strCommand.Format(L"select pwd,name from orika_userlogin where UserLogin='%s' and apilogin=1;", struser);

				CSession m_tempSession;
				////CStaticClass::m_logfile.LogEvent(L"L11");
				m_tempSession.Open(CStaticClass::connection);
				hr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
				if (FAILED(hr))
				{										
					return 1;
				}
				CString m_password = L"";
				CString m_name = L"";
				while (hr = data_table.MoveNext() == S_OK)
				{
					m_password = data_table.m_password;
					m_name = data_table.m_name;
				}
				m_tempSession.Close();
				
				////CStaticClass::m_logfile.LogEvent(L"UL11");

				if (m_password == strpwd)
				{
					return 4;
				}
				else
				{
					return 1;
				}


			}
			else
			{
				return 1;
			}
	}	
	else
	{
		return 1;
	}
	}
	else
	{
		return 1;
	}
	}
	else
	{
		return 1;
	}
}



