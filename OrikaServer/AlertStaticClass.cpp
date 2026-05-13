#include "stdafx.h"
#include "AlertStaticClass.h"

TMTArray<AlertStaticClass::dealingData> AlertStaticClass::m_Alertdealing_Array;
CMutex AlertStaticClass::m_alert_dealing_Lock;
