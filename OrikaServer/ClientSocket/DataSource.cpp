
#include "Stdafx.h"
#include "DataSource.h"
#define STACK_SIZE_COMMON     262144
MTTickArray       CDataSourceConnection::m_ticks_buffer;
CDataSourceConnection::CDataSourceConnection() : m_workflag(0),m_initalized(0),m_state(STATE_DISCONNECTED),m_unisocket(NULL)
  {
//--- null the strings
   m_address[0] ='\0';
   m_login[0]   ='\0';
   m_password[0]='\0';
	bid=0;
	ask=0;
	ltp=0;
	////For Debugging
   //_snprintf_s(m_address,_countof(m_address)-1,_TRUNCATE,"35.154.123.222:5051");
	//_snprintf_s(m_address,_countof(m_address)-1,_TRUNCATE,"13.126.83.17:6060");
    _snprintf_s(m_address, _countof(m_address) - 1, _TRUNCATE, "13.126.83.17:6062");
   _snprintf_s(m_login,_countof(m_login)-1,_TRUNCATE,"1000");
   _snprintf_s(m_password,_countof(m_password)-1,_TRUNCATE,"hello");
  }

CDataSourceConnection::~CDataSourceConnection()
  {
   
  }
bool CDataSourceConnection::Start()
  {	  
    StateSet(STATE_CONNECTSTART);
//--- set the thread operation flag
   InterlockedExchange(&m_workflag,1);
//--- start the thread of external connection data processing
   if(!m_thread.Start(ProcessThreadWrapper,this,STACK_SIZE_COMMON))
    {    
        return(false);
    }
//--- everything is ok
   return(true);
  }

bool CDataSourceConnection::Check()
  {
//--- if not initialized, quit
   if(!InterlockedExchangeAdd(&m_initalized,0))
      return(true);
//--- check the state
   if(StateGet()==STATE_DISCONNECTED)
      return(false);
//--- check the socket
   return(true);
  }
//+------------------------------------------------------------------+
//| Getting state                                                    |
//+------------------------------------------------------------------+
void CDataSourceConnection::StateSet(int stateval)
{
    int currentval = StateGet();
    InterlockedExchangeAdd(&m_state, stateval- currentval);
}
LONG CDataSourceConnection::StateGet()
  {
   return(InterlockedExchangeAdd(&m_state,0));
  }
//+------------------------------------------------------------------+
//| Starting the thread handling data of the external connection     |
//+------------------------------------------------------------------+
UINT __stdcall CDataSourceConnection::ProcessThreadWrapper(LPVOID param)
  {
//--- checking
   CDataSourceConnection *pThis=reinterpret_cast<CDataSourceConnection*>(param);
   if(pThis!=NULL) pThis->ProcessThread();
//--- ok
   return(0);
  }
//+------------------------------------------------------------------+
//| Processing data of the external connection                       |
//+------------------------------------------------------------------+
void CDataSourceConnection::ProcessThread(void)
  {	
   while(InterlockedExchangeAdd(&m_workflag,0)>0)
     {		
      if(StateGet()==STATE_CONNECTSTART)
	  {		 
          if (ProcessConnect())
          {
              StateSet(STATE_CONNECTED);
          }
	  }
      //--- connected, receive data	  
      if(StateGet()==STATE_CONNECTED)
	  {		 
         ProcessData();
	  }
      //--- sleep
      Sleep(1);
     }
  }
//+------------------------------------------------------------------+
//| Connection                                                       |
//+------------------------------------------------------------------+
bool CDataSourceConnection::ProcessConnect()
  {	
	bool res=false;
   if(!m_unisocket && (m_unisocket=new CUniFeederSocket(*this))==NULL)
      return(false);
   return  m_unisocket->Connect(CStaticClass::dataFeedServer, "", "", "");
  }


bool CDataSourceConnection::ProcessData()
  {	  
   MTAPIRES res=MT_RET_OK;   
//--- receive ticks from data source
   if(!m_unisocket || !m_unisocket->ReadTicks(m_ticks_buffer))
      res=MT_RET_ERROR; 
   return(res==MT_RET_OK);    
  }
//+------------------------------------------------------------------+
