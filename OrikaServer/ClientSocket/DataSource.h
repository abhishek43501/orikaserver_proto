#ifndef _CDataSource_
#define _CDataSource_
#pragma once
#include "UniFeederSocket.h"
#include "Thread.h"
//+------------------------------------------------------------------+
//| Class of external connection data processing                     |
//+------------------------------------------------------------------+
class CDataSourceConnection
  {
public:
	int bid,ask,ltp;		
   enum
     {
      STATE_DISCONNECTED=0x00,        // there is no connection
      STATE_CONNECTSTART=0x01,        // starting connection
      STATE_CONNECTED   =0x02,        // connection has been established
     };

private:
   //--- constants
   enum constants
     {
      THREAD_TIMEOUT   =60,           // timeout of thread completion, s
      READ_BUFFER_MAX  =1024*1024,    // maximal buffer size
      READ_BUFFER_STEP =16*1024,      // step of the buffer reallocation
      BUFFER_SIZE_CLIENT      =128*1024,     // size of the parsing buffer
      LOGIN_COUNT_MAX  =3             // maximal number of authorization attempts
     };

private:
    CThread           m_thread;
   volatile long     m_workflag;
   //--- sign of initialization (receipt of connection settings)
   volatile long     m_initalized;
   //--- state of the external connection
   volatile long     m_state;
   //--- access to Universal DDE Connector  
   CUniFeederSocket* m_unisocket;      // feeder socket
   char              m_address[128];   // server address
   char              m_login[64];      // login
   char              m_password[64];   // password   
public:		   	
                    CDataSourceConnection();
                    ~CDataSourceConnection();
   //--- connection check
   bool              Check();
   bool              Start();
   //--- state of the external connection
   LONG              StateGet();  
   void              StateSet(int stateval);

   static MTTickArray       m_ticks_buffer;
      
private:
   //--- external connection data processing thread
   static UINT __stdcall ProcessThreadWrapper(LPVOID param);
   void              ProcessThread();
   //--- external connection
   bool              ProcessConnect();
   //--- processing of external connection data
   bool              ProcessData();
  };
//+------------------------------------------------------------------+
#endif