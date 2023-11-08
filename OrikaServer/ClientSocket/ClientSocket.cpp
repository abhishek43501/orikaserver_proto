
#include "StdAfx.h"
#include "ClientSocket.h"


CClientSocket::CClientSocket()
   : m_socket(INVALID_SOCKET),m_pingtime(0),m_closetime(0),m_data(NULL),
     m_data_max(0),m_data_readed(0),m_data_total(0)
  {
//--- null the string
  data_read_flag=0;
  header_size=0;
  tick_size=0;
   m_string[0]=L'\0';
  }

//+------------------------------------------------------------------+
//| Destructor                                                       |
//+------------------------------------------------------------------+
CClientSocket::~CClientSocket()
  {
//--- close the socket
   Close();
//--- release the memory
   if(m_data)
     {
      delete[] m_data;
      m_data=NULL;
     }
//--- null positions
   m_data_max=m_data_readed=m_data_total=0;
  }
//+------------------------------------------------------------------+
//| Checking the connection                                          |
//+------------------------------------------------------------------+
bool CClientSocket::Check()
  {
//--- socket not closed yet?
   if(m_socket==INVALID_SOCKET)
      return(false);
//--- check the time
   if(_time64(NULL)>=m_closetime)
     {
      //--- close the socket
      Close();
      return(false);
     }
//--- everything is ok
   return(true);
  }
//+------------------------------------------------------------------+
//| Check connection to the server and connect, if necessary         |
//+------------------------------------------------------------------+
bool CClientSocket::Connect(LPCSTR server,LPCSTR login,LPCSTR password,LPCSTR symbols)
  {
//--- checking
   if(!server || !login || !password)
      return(false);
//--- if already connected, fine, quit
   if(m_socket!=INVALID_SOCKET)
      return(true);
//--- return the connection result
   //return(ConnectDDE(server) && Login(login,password,symbols));
   return(ConnectDDE(server) );
  }
//+------------------------------------------------------------------+
//| Closing the socket                                               |
//+------------------------------------------------------------------+
void CClientSocket::Close(void)
  {
//--- if the socket is open, close it
   if(m_socket!=INVALID_SOCKET)
     {
      shutdown(m_socket,2);
      closesocket(m_socket);
      m_socket=INVALID_SOCKET;
     }
  }
//+------------------------------------------------------------------+
//| Receipt of ticks                                                 |
//+------------------------------------------------------------------+
bool CClientSocket::ReadTicks(MTTickArray &ticks)
  {
   MTTick tick={0};
   
//--- read data from socket
   if(!DataCheck())
      return(false);
   
if (data_read_flag==0)
{
	header_size=16;
}
else
{
	header_size=tick_size;
}
 
while(DataGetString(m_string,_countof(m_string)-1,header_size))
{  
	if (header_size==16)
	{
		
		
	}
	else
	{		
		ZeroMemory(&tick,sizeof(MTTick));
		  //--- read one tick and store it in the buffer
        ReadTick(m_string, tick);
			 
	}	
	
	//Sleep(1);
} 


   return(true);
  }
//+------------------------------------------------------------------+
//| Connection to UniDDE server                                      |
//+------------------------------------------------------------------+
bool CClientSocket::ConnectDDE(LPCSTR server)
  {
   struct sockaddr_in srv={0};
   struct hostent    *hp=NULL;
   char              *cp=NULL,ip[128]={0};
   unsigned int       addr=0;
   int                port=0;
//--- checking
   if(!server || !server[0])
      return(false);
//--- unparse the server and port
   strcpy_s(ip,_countof(ip)-1,server);
   if((cp=strstr(ip,":"))!=NULL)
     {
      port=atoi(cp+1);
      *cp=0;
     }
//--- close the old connection
   Close();
//--- refresh the time
   UpdateTime();
//--- create a socket
   if((m_socket=socket(AF_INET,SOCK_STREAM,0))==INVALID_SOCKET)
     {      
      return(false);
     }
//--- get a host
   if((addr=inet_addr(ip))==INADDR_NONE)
     {
      if((hp=gethostbyname(ip))==NULL)
        {
         Close();         
         return(false);
        }
      srv.sin_addr.s_addr=*((unsigned long*)hp->h_addr);
     }
   else
      srv.sin_addr.s_addr=addr;
//--- fill out parameters
   srv.sin_family=AF_INET;
   srv.sin_port  =htons(port);
//--- connect
   if(connect(m_socket,(struct sockaddr*)&srv,sizeof(srv))!=0)
     {
      Close();      
      return(false);
     }
  
   //SendString("1007\n");
//--- refresh the time
   UpdateTime();
//--- everything is ok 
   return(true);
  }
bool CClientSocket::get_data(char *buf,const int maxlen,int r_byte)
{
    char *start=m_data+m_data_readed,*end=m_data+m_data_total;
   int   len=0;
   
//--- checking parameters
   if(!buf || maxlen<1 || !m_data || (m_data_total-m_data_readed)<r_byte)
      return(false);
//--- start parsing
  
	   while(len<=r_byte)
		 {       			
				  *buf++=*start;
				  start++;			 			 
					len++;
		 }
   
	*buf=0;
     m_data_readed=m_data_readed+len-1;
   return(true);
   
  }
bool CClientSocket::Login(LPCSTR login,LPCSTR password,LPCSTR symbols)
  {
   char  tmp[MAX_PATH]={0};
   int   count=0;
   bool  found=false;
//--- checking
   if(!login || !password || !symbols)
      return(false);
//--- form a command
   _snprintf_s(tmp,_countof(tmp)-1,_TRUNCATE,"%s\r\n",login);
//--- send a command
   if(!SendString(tmp))
     {      
      return(false);
     }
//--- wait for a login line
   for(count=0;count<LOGIN_COUNT_MAX;count++)
     {
      //--- read the next string and check inclusion of the sub-string "Login: "
      if(ReadStringCheck(tmp,sizeof(tmp)-1,"Login: ",found) && found)
         break;
     }
//--- if we couldn't find the string, error
   if(!found)
     {      
      return(false);
     }
//--- form a command
   _snprintf_s(tmp,_countof(tmp)-1,_TRUNCATE,"%s\r\n",password);
//--- send a command
   if(!SendString(tmp))
     {      
      return(false);
     }
//--- wait for a password line
   for(count=0;count<LOGIN_COUNT_MAX;count++)
     {
      //--- read the next string and check inclusion of the sub-string "Password: "
      if(ReadStringCheck(tmp,sizeof(tmp)-1,"Password: ",found) && found)
         break;
     }
//--- if we couldn't find the string, error
   if(!found)
     {      
      return(false);
     }
//--- read the reply
   for(count=0;count<LOGIN_COUNT_MAX;count++)
     {
      //--- read the next string and check inclusion of the sub-string "Password: "
      if(ReadStringCheck(tmp,sizeof(tmp)-1,"Access granted",found))
         break;
     }
//--- if we couldn't find the string, error
   if(!found)
     {      
      return(false);
     }
//--- login is confirmed
   m_pingtime=_time64(NULL);
//--- reset positions
   m_data_total=m_data_readed=0;
//--- send symbols
   if(symbols && symbols[0])
     {
      if(!SendString("> Symbols:") || !SendString(symbols) || !SendString("\r\n"))
        {         
         return(false);
        }
     }
//--- everything is ok
   return(true);
  }
//+------------------------------------------------------------------+ 
//| Reading a tick Structure                                     | 
//+------------------------------------------------------------------+
bool CClientSocket::ReadTick(char* str,MTTick &tick)
{				
   char symbols[MAX_PATH]={0},*cp=NULL,*np=NULL;
   int  params=1;
	//--- checking
   if(!str)
   return(false);  
    st_tick* m_st_tick = reinterpret_cast<st_tick*>(str);
	data_read_flag=0;
	header_size=16;
	if (m_st_tick->Exchange_code ==1102)
	{
		CString  strsymbol=L"";
		strsymbol.Format(L"%d",m_st_tick->Instrument_Code);
		strsymbol=strsymbol.Trim();
		CMTStr::Copy(tick.symbol,strsymbol);

		/*if (m_st_tick->Instrument_Code==110)
		{
			CString str=L"";
			str.Format(L" Pre Conver into tick Symbol %d Bid:-%d ASK:-%d ",m_st_tick->Instrument_Code,m_st_tick->Buy_Price ,m_st_tick->Sell_Price);
			ExtLogger.Out(MTLogOK,str);
		}*/

		double ask=0;
		double bid=0;
		double tmp=m_st_tick->Sell_Price;
		ask=tmp/1000000;
		tick.ask=tmp/1000000;
		tmp=m_st_tick->Buy_Price;
		bid=tmp/1000000;		
		tick.bid=tmp/1000000;
		tmp=m_st_tick->LTP;
		tick.last=tmp/1000000;
		
	}
	else
	{
		CString  strsymbol=L"";
		strsymbol.Format(L"%d",m_st_tick->Instrument_Code);
		strsymbol=strsymbol.Trim();
		CMTStr::Copy(tick.symbol,strsymbol);
		double tmp=m_st_tick->Sell_Price;
		tick.ask=tmp/100;
		tmp=m_st_tick->Buy_Price;
		tick.bid=tmp/100;
		tmp=m_st_tick->LTP;
		tick.last=tmp/100;	

	}		
	tick.datetime_msc=m_st_tick->LUT;	
	tick.volume=m_st_tick->LTQ ;	   
    //CStaticClass::m_logfile.LogEvent(L"L3");
    CStaticClass::m_mutex_order.Lock();
    
       
        
        CString m_symbol = L"";
        CString m_RealSymbol = L"";
        CString m_source = L"";

        CString m_SymbolInTickData = L"";
        m_SymbolInTickData.Format(L"%d", m_st_tick->Instrument_Code);
        CStaticClass::m_SymbolSourceArray.Lookup(m_SymbolInTickData, m_RealSymbol);
        
        while (m_RealSymbol.GetLength() > 0)
        {
            if (m_RealSymbol.Find(L":") > 0)
            {
                m_symbol = m_RealSymbol.Mid(0, m_RealSymbol.Find(L":") );
                m_RealSymbol= m_RealSymbol.Mid(m_RealSymbol.Find(L":") + 1, m_RealSymbol.GetLength()- m_RealSymbol.Find(L":") + 1);
            }
            else
            {
                m_symbol = m_RealSymbol;
                m_RealSymbol = L"";
            }
            
            CStaticClass::st_symbolHighLowTimeWise m_stHL = {};
            CStaticClass::m_SymbolHighLowTimeWiseArray.Lookup(m_symbol, m_stHL);
            if (m_stHL.m_Heigh != m_st_tick->High)
            {
                if (m_stHL.m_Heigh != 0)
                {
                    double m_heighFromExternalFeed = m_st_tick->High;
                    double realH = m_heighFromExternalFeed / 100;
                    //CStaticClass::m_logfile.LogEvent(L"Going to update Status S1");
                    m_StaticClass.UpdateOrderStatus(m_symbol,0, realH);
                    //CStaticClass::m_logfile.LogEvent(L"Going to update Status S1 Updated");
                }

                CMTStr::Copy(m_stHL.m_Symbol, m_symbol);
                m_stHL.m_PreviousHeigh = m_stHL.m_Heigh;
                m_stHL.m_Heigh = m_st_tick->High;
                UINT64 CurrentTime = _time64(NULL);
                m_stHL.m_HeighTime = CurrentTime;

                CStaticClass::m_SymbolHighLowTimeWiseArray.SetAt(m_symbol, m_stHL);
            }
            if (m_stHL.m_Low != m_st_tick->Low)
            {
                if (m_stHL.m_Low != 0)
                {
                    double lowFromExternalFeed = m_st_tick->Low;
                    double realL = lowFromExternalFeed / 100;
                    //CStaticClass::m_logfile.LogEvent(L"Going to update Status S2");
                    m_StaticClass.UpdateOrderStatus(m_symbol, realL,0);
                    //CStaticClass::m_logfile.LogEvent(L"Going to update Status S2 Updated" );
                }
                CMTStr::Copy(m_stHL.m_Symbol, m_symbol);
                m_stHL.m_PreviousLow = m_stHL.m_Low;
                m_stHL.m_Low = m_st_tick->Low;
                UINT64 CurrentTime = _time64(NULL);
                m_stHL.m_LowTime = CurrentTime;
                CStaticClass::m_SymbolHighLowTimeWiseArray.SetAt(m_symbol, m_stHL);
            }
        }
    
    CStaticClass::m_mutex_order.Unlock();
    //CStaticClass::m_logfile.LogEvent(L"UL3");
//--- checking obtained data
   if(tick.bid<=0 || tick.ask<=0 || tick.bid>tick.ask)
     {      
      return(false);
     }   
//--- everything is ok
   return(true);
  }
//+------------------------------------------------------------------+
//| Obtain the size of data in the socket                            |
//+------------------------------------------------------------------+
int CClientSocket::IsReadable(void)
  {
   ULONG size=0;
//--- checking the socket validity
   if(m_socket!=INVALID_SOCKET)
      if(ioctlsocket(m_socket,FIONREAD,&size)!=0)
        {
         Close();
         size=0;
        }
//--- return the size
	  if(size>0)
	  //ExtLogger.Out(MTLogOK,L"Pending Data On Socket Socket %d bytes",size);
   return(size);
  }
//+------------------------------------------------------------------+
//| Read data from the socket                                        |
//+------------------------------------------------------------------+

void CClientSocket::CharToByte(char* chars, byte* bytes, unsigned int count)
{
    for(unsigned int i = 0; i < count; i++)
    	bytes[i] = (byte)chars[i];
}
int old_data_size=0;
bool CClientSocket::DataUpdate(void)
  {
   int   len=0,res=0;
   char *cp=NULL;
//--- check if there are data in the socket
 
   if((len=IsReadable())<1)
   {
	   
      return(true);
   }  
     //ExtLogger.Out(MTLogErr,L"CHECKDATA FROM SOCKET");
//--- shift unprocessed data
   if(m_data!=NULL && m_data_readed>0)
     {
      if((m_data_total-m_data_readed)>0)
         memmove(m_data,m_data+m_data_readed,m_data_total-m_data_readed);
      //--- refresh pointers
      m_data_total       -=m_data_readed;
      m_data[m_data_total]=0;
      m_data_readed       =0;
     }
//--- checking the buffer size
 
   if((m_data_total+len)>m_data_max || !m_data)
     {
      //--- checking the maximal size
      if((m_data_total+len)>READ_BUFFER_MAX)
        {         
         return(false);
        }
      //--- select data
      if((cp=new char[m_data_total+len+READ_BUFFER_STEP+16])==NULL)
        {         
         return(false);
        }
      //--- copy old data
      if(m_data!=NULL)
        {
         if(m_data_total>0)
            memcpy(cp,m_data,m_data_total);
         delete[] m_data; m_data=NULL;
        }
      //--- copy the pointer to m_data
      m_data    =cp;
      m_data_max=m_data_total+len+READ_BUFFER_STEP;
     }
  
//--- read data from the socket

   if((res=recv(m_socket,m_data+m_data_total,len,0))<1)  
   {
	   //ExtLogger.Out(MTLogErr,L"Problem In Data Receiving");
	    return(false);
   }         
//--- form the result
   m_data_total       +=res;
   m_data[m_data_total]=0;
//--- refresh the time
   UpdateTime();
//--- everything is ok
   return(true);
  }
//+------------------------------------------------------------------+
//| Parse the line obtained from the socket                          |
//+------------------------------------------------------------------+
bool CClientSocket::DataGetString(char *buf,const int maxlen,int r_byte)
  {
   char *start=m_data+m_data_readed,*end=m_data+m_data_total;
   int   len=0;   
//--- checking parameters
   if(!buf || maxlen<1 || !m_data || (m_data_total-m_data_readed)<r_byte)
      return(false);
//--- start parsing
  
	   while(len<=r_byte)
		{       			
			*buf++=*start;
			start++;			 			 
			len++;
		}   
	*buf=0;
     m_data_readed=m_data_readed+len-1;
	 data_read_flag=1;
   return(true);
  }
//+------------------------------------------------------------------+
//| Data check                                                       |
//+------------------------------------------------------------------+
bool CClientSocket::DataCheck(void)
  {
//--- check connection
   if(m_socket==INVALID_SOCKET)
      return(false);
//--- send ping to DDE Connector server if necessary
   if(!SendPing())
     {      
      return(false);
     }
//--- get all data from the socket to buffer m_data
   if(!DataUpdate())
     {      
      return(false);
     }
//--- successful
   return(true);
  }
//+------------------------------------------------------------------+
//| Read data from the socket and compare to the preset substring    |
//+------------------------------------------------------------------+
bool CClientSocket::ReadStringCheck(char *buf,const int maxlen,LPCSTR str,bool &found)
  {
   bool res=false;
//--- checking
   if(!buf || !str)
      return(false);
//--- reset the flag that we've found the line
   found=false;
//--- reading data
   if(res=ReadString(buf,maxlen))

	   //For Checking Pur
		   str="multi";
		   //End
      found=strstr(buf,str)!=NULL;
//--- return the result
   return(res);
  }
//+------------------------------------------------------------------+
//| Read the string from the socket                                  |
//+------------------------------------------------------------------+
bool CClientSocket::ReadString(char *buf,const int maxlen)
  {
   int count=0,len=0;
   char* ptr=buf;
   
//--- checking
   if(!buf || m_socket==INVALID_SOCKET)
      return(false);
//--- read data
		
		int recvcheck=0;
	if(recvcheck=recv(m_socket,buf, maxlen,0)!=1)
	{		
		//--- checking errors of the socket
		if(WSAGetLastError()!=WSAEWOULDBLOCK || count>10)
		{
			Close();
			return(false);
		}
		//--- read data
		
		count++;
		Sleep(1);
		
	}		 
   return(true);
  }
//+------------------------------------------------------------------+
//| Sending ping to DDE Connector                                    |
//+------------------------------------------------------------------+
bool CClientSocket::SendPing(void)
  {
   UINT64 ctm=_time64(NULL);
//--- every minute send a ping signal to DDE Connector
   if((ctm-m_pingtime)>60)
     {
      //--- send the ping
      if(SendString("> Ping\r\n")==false)
        {         
         return(false);
        }
      //--- save the time of the last ping
      m_pingtime=ctm;
     }
//--- everything is ok
   return(true);
  }
//+------------------------------------------------------------------+
//| Send the string                                                  |
//+------------------------------------------------------------------+
bool CClientSocket::SendString(LPCSTR buf)
  {
   int count=0,res,len,size;
//--- checking
   if(buf==NULL || m_socket==INVALID_SOCKET) 
	   return(false);
//--- send data
   len=size=(int)strlen(buf);
   while(len>0)
     {
      if((res=send(m_socket,buf,len,0))<1)
        {
         //--- checking for the socket error
         if(WSAGetLastError()!=WSAEWOULDBLOCK || count>10) 
		 { 
			 Close(); 
			 return(false); 
		 }
         //--- write the rest of data
         count++; Sleep(50); continue;
        }
      //--- change positions
      buf+=res; len-=res;
     }
//--- raise the time
   UpdateTime();   
//--- everything is ok
   return(true);
  }
//+------------------------------------------------------------------+






