#ifndef _CUniFeederSocket_
#define _CUniFeederSocket_
#pragma once
#include "..//StaticClass.h"
typedef TMTArray<MTTick> MTTickArray;
class CDataSourceConnection;
//+------------------------------------------------------------------+
//| Class of socket of connection to UniDDE Connector                |
//+------------------------------------------------------------------+
class CUniFeederSocket
  {
   //--- constants
   enum constants
     {
      THREAD_TIMEOUT   =60,           // timeout of thread completion, s
      READ_BUFFER_MAX  =1024*1024,    // maximal buffer size
      READ_BUFFER_STEP =16*1024,      // step of the buffer reallocation
      BUFFER_SIZE_SOCKET      =128*1024,     // size of the parsing buffer
      LOGIN_COUNT_MAX  =3             // maximal number of authorization attempts
     };

private:
   //--- reference to parent
	CDataSourceConnection &m_parent;
   //--- access to Universal DDE Connector
   SOCKET            m_socket;         // socket
   //--- connection timeouts
   INT64             m_pingtime;       // server ping time
   INT64             m_closetime;      // time of closing by tomeout
   
   //--- buffer for the read string
   char              m_string[BUFFER_SIZE_SOCKET];
   //--- data for working with the socket
   char             *m_data;           // buffer of the socket data
   byte* m_bytes;
   UINT              m_data_max;       // maximal length
   UINT              m_data_readed;    // amount of read data
   UINT              m_data_total;     // toral amount of data

public:
	CStaticClass m_StaticClass;


	struct THeaderRecord
    {
        char MsgType;
        short  MsgLen;
    };

 struct TDataStartRecord //MsgType = 'B' or 'I'
    {
        THeaderRecord Header;
        int Time;
    };

  

 struct st_tick
	{
		int Exchange_code;
		int Segment;
		int Instrument_Code;
		int Buy_Price;
		int Buy_Qty;
		int Sell_Price;
		int Sell_Qty;
		int LTP;
		int LTQ;	
		int LTT; //Last Trade Time from api
		int ATP;
		int TTQ;
		int Open_Interest;
		int Open;
		int High;
		int Low;
		int Close;
		int RESERVED;
		INT64  LUT;//Last update time add manually from server	    		
	};

	struct st_Data_Details
	{
		int Data_Type;
		int Un_com_Size;
		int Com_size;
		int reservevalue1;
	};




   


  int data_read_flag;
  int header_size;
  int tick_size;

  
void CUniFeederSocket::ConvertToHeaderData(char* st);
bool CUniFeederSocket::get_data(char *buf,const int maxlen,int r_byte);


   //--- constructor/destructor
                     CUniFeederSocket(CDataSourceConnection &parent);
					 
                    ~CUniFeederSocket();
   //--- connection/disconnection of Universal DDE Connector
   bool              Connect(LPCSTR server,LPCSTR login,LPCSTR password,LPCSTR symbols);
   void              Close(void);
   //--- receipt of a ticks
   bool              ReadTicks(MTTickArray &ticks);
   //--- check of the connection
   bool              Check();
   bool              SendString(LPCSTR buf);
private:
   //--- connection methods
   bool              ConnectDDE(LPCSTR server);
   bool              Login(LPCSTR login,LPCSTR password,LPCSTR symbols);
   //--- getting a tick from a string
   bool              ReadTick(char* str,MTTick &tick);
   
   //--- processing of a data buffer
   bool              DataUpdate(void);
   bool              DataGetString(char *buf,const int maxlen,int r_byte);
   bool              DataCheck(void);
   //--- function of reading/writing to socket
   int               IsReadable(void);
   bool              ReadStringCheck(char *buf,const int maxlen,LPCSTR str,bool &found);
   bool              ReadString(char *buf,const int maxlen);
   bool              SendPing(void);
   
   //--- refreshing the socket closing time
   void              UpdateTime(void) { m_closetime=_time64(NULL)+THREAD_TIMEOUT; }
  };
//+------------------------------------------------------------------+
#endif