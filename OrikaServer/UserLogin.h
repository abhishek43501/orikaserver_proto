
#pragma once

#include "resource.h"		// main symbols


class CUserLogin
{
public:
	CUserLogin();
	~CUserLogin();
	struct loginatemptLoginwise
	{
		wchar_t  m_login[20];
		UINT64   m_lastloginTime;
		int      m_loginfalureCount;
		UINT64   m_lastfalureTime;
	};

	struct loginatemptIp
	{
		wchar_t  m_IpAddress[20];
		UINT64   m_lastloginTime;
		int      m_loginfalureCount;
		UINT64   m_lastfalureTime;
	};
	static CMap<CString ,LPCTSTR,loginatemptLoginwise,loginatemptLoginwise> m_loginatemptLoginwiseMap;
	static CMap<CString ,LPCTSTR,loginatemptIp,loginatemptIp> m_loginatemptIp;
	int userValideate(const char* jsonString,CString ipAddress );
	bool validateSerialno(CString loginuser,CString hdno);
};

 