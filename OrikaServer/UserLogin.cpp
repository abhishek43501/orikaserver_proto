
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
	// I10: previous code did `HRESULT hr=NULL; if(!SUCCEEDED(hr)) return false;`
	// which was a dead guard (SUCCEEDED(0) is true). Initialise hr to E_FAIL
	// and remove the dead branch. MoveNext loop's precedence was also wrong
	// (assigned bool to hr instead of HRESULT); now parenthesised correctly.
	// SQL access wrapped in try/catch because ATL CSession::Open throws on
	// failure when the CDataSource is not open (audit I9 chain).
	HRESULT hr = E_FAIL;
	CString strCommand;
	strCommand.Format(L"select userlogin,HDno from orika_userLoginHDkeyMapping where userlogin='%s' and (HDno='%s' or HDno='*' );", loginuser, hdno);

	int m_sl_validated = 0;
	CSession m_tempSession;
	try
	{
		m_tempSession.Open(CStaticClass::connection);
		CCommand<CAccessor<CTableHDno>> data_table;
		hr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
		if (FAILED(hr))
		{
			m_tempSession.Close();
			return false;
		}
		while ((hr = data_table.MoveNext()) == S_OK)
		{
			m_sl_validated = 1;
		}
		m_tempSession.Close();
	}
	catch (CException* e)
	{
		TCHAR errMsg[512] = {0};
		e->GetErrorMessage(errMsg, _countof(errMsg));
		CString line;
		line.Format(L"validateSerialno: SQL exception: %s", errMsg);
		CStaticClass::m_logfile.LogEvent(line);
		e->Delete();
		return false;
	}
	catch (...)
	{
		CStaticClass::m_logfile.LogEvent(L"validateSerialno: unknown SQL exception (likely CDataSource not open)");
		return false;
	}

	return (m_sl_validated == 1);
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



					// I10: see validateSerialno for rationale - dead SUCCEEDED guard
					// removed, hr initialised to E_FAIL, MoveNext precedence fixed,
					// SQL access wrapped in try/catch.
					HRESULT hr = E_FAIL;
					CString strCommand;
					strCommand.Format(L"select pwd,name from orika_userlogin where UserLogin='%s';", struser);

					CString m_password;
					CString m_name;
					CSession m_tempSession;
					try
					{
						m_tempSession.Open(CStaticClass::connection);
						CCommand<CAccessor<CTableLoginPassword>> data_table;
						hr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
						if (FAILED(hr))
						{
							m_tempSession.Close();
							return 1;
						}
						while ((hr = data_table.MoveNext()) == S_OK)
						{
							m_password = data_table.m_password;
							m_name = data_table.m_name;
						}
						m_tempSession.Close();
					}
					catch (CException* e)
					{
						TCHAR errMsg[512] = {0};
						e->GetErrorMessage(errMsg, _countof(errMsg));
						CString line;
						line.Format(L"userValideate(login): SQL exception: %s", errMsg);
						CStaticClass::m_logfile.LogEvent(line);
						e->Delete();
						return 1;
					}
					catch (...)
					{
						CStaticClass::m_logfile.LogEvent(L"userValideate(login): unknown SQL exception");
						return 1;
					}

					if (m_password == strpwd)
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
				// I10: see validateSerialno for rationale - dead SUCCEEDED guard
				// removed, hr initialised to E_FAIL, MoveNext precedence fixed,
				// SQL access wrapped in try/catch.
				HRESULT hr = E_FAIL;
				CString strCommand;
				strCommand.Format(L"select pwd,name from orika_userlogin where UserLogin='%s' and apilogin=1;", struser);

				CString m_password;
				CString m_name;
				CSession m_tempSession;
				try
				{
					m_tempSession.Open(CStaticClass::connection);
					CCommand<CAccessor<CTableLoginPassword>> data_table;
					hr = data_table.Open(m_tempSession, (LPCTSTR)strCommand);
					if (FAILED(hr))
					{
						m_tempSession.Close();
						return 1;
					}
					while ((hr = data_table.MoveNext()) == S_OK)
					{
						m_password = data_table.m_password;
						m_name = data_table.m_name;
					}
					m_tempSession.Close();
				}
				catch (CException* e)
				{
					TCHAR errMsg[512] = {0};
					e->GetErrorMessage(errMsg, _countof(errMsg));
					CString line;
					line.Format(L"userValideate(api): SQL exception: %s", errMsg);
					CStaticClass::m_logfile.LogEvent(line);
					e->Delete();
					return 1;
				}
				catch (...)
				{
					CStaticClass::m_logfile.LogEvent(L"userValideate(api): unknown SQL exception");
					return 1;
				}

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



