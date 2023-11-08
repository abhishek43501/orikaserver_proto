#pragma once
//#include "Symbols.h"
#include "FrameAndDeframeMessage.h"
#include "DataBuffer.h"
#include <iostream>
#include "openssl_iocp.h"

//#include <iterator> 
#include <map> 

using namespace std;


class  FrameAndDeframeMessage {
public:
	FrameAndDeframeMessage();	
	~FrameAndDeframeMessage(void);
	void deframeIncomingMessage(char* incomingBuffer, int bufferSize, SSL_session* psession, DataBuffer& buffer);
	void deframeLargeIncomingMessage(char* incomingBuffer, int bufferSize, SSL_session* psession, DataBuffer& buffer);
	void frameOutgoingMessage(char* inputdata, DataBuffer& buffer, int nWrittenBytes);
	char getAt(char* Buffer, int offset);
};


