#pragma once
#include <process.h>
#include "windows.h"
//+------------------------------------------------------------------+
//| Thread encapsulating class                                       |
//+------------------------------------------------------------------+
class CThread
  {
private:
   HANDLE            m_thread;
public:
                     CThread(void);
                    ~CThread(void);
   //--- control
   bool              Start(unsigned (__stdcall *thread_func)(void*),void *thread_param,const UINT stack_size);
   bool              Shutdown(const UINT timeout=INFINITE);
   void              Terminate(void);
   bool              IsBusy(void);
   HANDLE            Handle(void) const { return(m_thread); }
  };
//+------------------------------------------------------------------+
//| Constructor                                                      |
//+------------------------------------------------------------------+
inline CThread::CThread(void) : m_thread(NULL)
  {
  }
//+------------------------------------------------------------------+
//| Destructor                                                       |
//+------------------------------------------------------------------+
inline CThread::~CThread(void)
  {
//--- wait till it's over
   Shutdown();
//--- close the handle
   if(m_thread!=NULL) { CloseHandle(m_thread); m_thread=NULL; }
  }
//+------------------------------------------------------------------+
//| Start the thread                                                 |
//+------------------------------------------------------------------+
inline bool CThread::Start(unsigned (__stdcall *thread_func)(void*),void *thread_param,const UINT stack_size)
  {
   DWORD id;
//--- check if the processing thread is started
   if(m_thread!=NULL)
     {
      GetExitCodeThread(m_thread,&id);
      //--- the thread is active......
      if(id==STILL_ACTIVE) 
		  return(false);
      //--- clear what is left
      CloseHandle(m_thread); m_thread=NULL;
     }
//--- start the processing thread
   if((m_thread=(HANDLE)_beginthreadex(NULL,stack_size,thread_func,(void*)thread_param,STACK_SIZE_PARAM_IS_A_RESERVATION,(UINT*)&id))==NULL)
      return(false);
//--- everything is ok
   return(true);
  }
//+------------------------------------------------------------------+
//| Thread stop                                                      |
//+------------------------------------------------------------------+
inline bool CThread::Shutdown(const UINT timeout/*=INIFINITE*/)
  {
//--- checking
   if(m_thread==NULL) return(true);
//--- wait till it's over
   return(WaitForSingleObject(m_thread,timeout)==WAIT_OBJECT_0);
  }
//+------------------------------------------------------------------+
//| Kill the thread                                                  |
//+------------------------------------------------------------------+
inline void CThread::Terminate(void)
  {
//--- perhaps there is nothing
   if(m_thread==NULL) return;
//--- forced killing of the thread
   TerminateThread(m_thread,0);
//--- close the handle
   CloseHandle(m_thread);
   m_thread=NULL;
  }
//+------------------------------------------------------------------+
//| Check if the thread is busy                                      |
//+------------------------------------------------------------------+
inline bool CThread::IsBusy(void)
  {
   DWORD res=0;
//--- is there a thread?
   if(!m_thread) 
	   return(false);
//--- request the thread state
   GetExitCodeThread(m_thread,&res);
//--- is the thread active?
   if(res==STILL_ACTIVE) return(true);
//--- close the handle
   CloseHandle(m_thread);
   m_thread=NULL;
//--- the thread is over
   return(false);
  }
//+------------------------------------------------------------------+
