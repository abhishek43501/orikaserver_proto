
#pragma once
#include "windows.h"
class CSync
  {
private:
   CRITICAL_SECTION  m_cs;            // description of a critical section

public:

   CSync(void)
     {
      ZeroMemory(&m_cs,sizeof(m_cs));
      InitializeCriticalSection(&m_cs);
     }

   ~CSync(void)
     {
      DeleteCriticalSection(&m_cs);
      ZeroMemory(&m_cs,sizeof(m_cs));
     }

   inline void       Lock(const int line)
     {
      //--- normal blocking
      EnterCriticalSection(&m_cs);
     }
   inline void       Unlock(void)
     {
      //--- unblock
      LeaveCriticalSection(&m_cs);
     }
  };
//+------------------------------------------------------------------+
