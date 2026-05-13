#pragma once
#define DEALING_DATA_SIZE 152
#define HEADER_SIZE 16
class AlertStaticClass
{
public:
	struct dealingData
	{
		wchar_t   m_login[32];
		wchar_t   m_symbol[32];
		int       m_action;
		int       m_reserve;
		double    m_volume;
		double    m_rate;				
	};	
	static TMTArray<dealingData> m_Alertdealing_Array;
	static CMutex m_alert_dealing_Lock;
	struct st_header
	{
		UINT Action;
		UINT Data_Size;
		UINT Data_Type;
		UINT reserved;
	};
};

