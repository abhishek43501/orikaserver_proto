#include "stdafx.h"
#include <Windows.h>
#include "FrameAndDeframeMessage.h"
#include <sstream>
#include "base64/base64.h"
#include "sha1/sha1.h"
#include "WebsocketDataMessage.h"
#include <vector> // Include this header for std::vector
#include "StaticClass.h"
FrameAndDeframeMessage::FrameAndDeframeMessage()
{}
FrameAndDeframeMessage::~FrameAndDeframeMessage(void)
{}
char FrameAndDeframeMessage::getAt(char* Buffer,int offset)
{
	return Buffer[offset];
}
void FrameAndDeframeMessage::deframeIncomingMessage(char* incomingBuffer,int bufferSize, SSL_session* psession, DataBuffer& buffer)
{
	if (bufferSize == 47)
	{
		memcpy(psession->pending_socket_buffer_Rev + psession->pendingdataSize, incomingBuffer, bufferSize);		
		BYTE payloadFlags = getAt(incomingBuffer, 1);
		payloadFlags = getAt(incomingBuffer+1, 2);
		payloadFlags = getAt(incomingBuffer+2, 3);
		payloadFlags = getAt(incomingBuffer+3, 4);
		payloadFlags = getAt(incomingBuffer+4, 5);
		payloadFlags = getAt(incomingBuffer+5, 6);
		payloadFlags = getAt(incomingBuffer+6, 7);
		payloadFlags = getAt(incomingBuffer+7, 8);	
	}

	if (psession->datapending == 1)
	{		
		memcpy(psession->pending_socket_buffer_Rev+ psession->pendingdataSize, incomingBuffer, bufferSize);

		memcpy(incomingBuffer, psession->pending_socket_buffer_Rev, bufferSize+ psession->pendingdataSize);
		bufferSize = bufferSize + psession->pendingdataSize;
	}
	string  returnval="";
	//In the other cases, we should expect a data message : 
	int nMinExpectedSize = 6;
	if (bufferSize < nMinExpectedSize)
		return ;



	bool isFin = (getAt(incomingBuffer,0) & 0x80) != 0;
	bool isContinuation = (getAt(incomingBuffer, 0) & 0x0F) == 0x0;
	uint8_t opcode = getAt(incomingBuffer + 1, 1) & 0x0F;




	BYTE payloadFlags = getAt(incomingBuffer, 0);
	if (payloadFlags != 129 && payloadFlags != 130)
	{
		return ;
	}		
	BYTE basicSize = getAt(incomingBuffer, 1) & 0x7F;
	unsigned __int64 payloadSize;
	int masksOffset;

	if (basicSize <= 125)
	{
		payloadSize = basicSize;
		masksOffset = 2;
	}
	else if (basicSize == 126)
	{
		nMinExpectedSize += 2;
		if (bufferSize < nMinExpectedSize)
			return ;
		payloadSize = ntohs(*(u_short*)(incomingBuffer + 2));
		masksOffset = 4;
	}
	else if (basicSize == 127)
	{
		nMinExpectedSize += 8;
		if (bufferSize < nMinExpectedSize)
			return ;
		payloadSize = ntohl(*(u_long*)(incomingBuffer + 2));
		payloadSize = *(u_long*)(incomingBuffer + 2);
		masksOffset = 10;
	}
	else
		return ;

	nMinExpectedSize += payloadSize;
	if (bufferSize < nMinExpectedSize)
	{
		//psession->pending_socket_buffer_Rev = new char[bufferSize];
		psession->datapending = 1;
		psession->pendingdataSize = bufferSize;
		memcpy(psession->pending_socket_buffer_Rev, incomingBuffer, bufferSize);		 
		return ;
	}
	else
	{
		if (psession->datapending!=0)
		{
			for (int i = 0; i < 900000; i++)
			{
				psession->pending_socket_buffer_Rev[i] ='\0';
			}
			psession->datapending = 0;
			psession->pendingdataSize = 0;
		}
		
	}

	BYTE masks[4];
	memcpy(masks, incomingBuffer + masksOffset, 4);

	char* payload = new char[payloadSize + 1];
	memcpy(payload, incomingBuffer + masksOffset + 4, payloadSize);
	for (unsigned __int64 i = 0; i < payloadSize; i++) {
		payload[i] = (payload[i] ^ masks[i % 4]);
	}
	payload[payloadSize] = '\0';
	//WebsocketDataMessage* pMessage = new WebsocketDataMessage(payload);
	//
	//string rMessage = pMessage->raw;
	////returnval = rMessage.c_str();
	//returnval = rMessage;	
	buffer.Append(payload, payloadSize);
	delete[] payload;
	//return returnval;
}

void FrameAndDeframeMessage::deframeLargeIncomingMessage(char* incomingBuffer, int bufferSize, SSL_session* psession, DataBuffer& buffer)
{
	string return_data = "";
	// check at least 6 bytes are set (first 2 bytes and 4 bytes for the mask key)
	if (bufferSize < 6)
		return ;
	// fetch first 2 bytes of header
	unsigned char octet0 = getAt(incomingBuffer, 0);
	unsigned char octet1 = getAt(incomingBuffer, 1);

	unsigned char fin = octet0 & 0x80;
	unsigned char opcode = octet0 & 0x0f;



	int nMinExpectedSize = 6;
	if (bufferSize < nMinExpectedSize)
		return ;
	//unsigned char mask = octet1 & WS_MASK;
	//if (octet1 < 128)
	//	return ""; // close socket, as no mask bit was sent from the client

						  		
	BYTE basicSize = getAt(incomingBuffer, 1) & 0x7F;
	unsigned __int64 payloadSize;
	int masksOffset;
	payloadSize = 0;
	int dataposition = 0;
	int seek = 0;
	if (basicSize <= 125)
	{
		//payloadSize = basicSize;
		masksOffset = 0;
		seek = 6;
		dataposition = 0;
		payloadSize = bufferSize;
			
		/*payloadSize = getAt(incomingBuffer+1, 1);
		int test = 0;*/
	}
	else if (basicSize == 126)
	{
		nMinExpectedSize += 2;
		if (bufferSize < nMinExpectedSize)
			return ;
		payloadSize = ntohs(*(u_short*)(incomingBuffer + 2));
		masksOffset = 4;
	}
	else if (basicSize == 127)
	{
		nMinExpectedSize += 8;
		if (bufferSize < nMinExpectedSize)
			return ;
		//payloadSize = ntohl(*(u_long*)(incomingBuffer + 2));
		payloadSize = ntohl(*(reinterpret_cast<const uint64_t*>(incomingBuffer + 2)));
		masksOffset = 10;
		if (payloadSize == 0)
		{
			payloadSize = bufferSize - 14;
		}
		dataposition = 4;
		memcpy(psession->masks, incomingBuffer + masksOffset, 4);

	}
	else
		return ;
	
	// decode payload data
	//masksOffset = 10;	

	char* payload = new char[payloadSize + 1];

	memcpy(payload, incomingBuffer + masksOffset + dataposition, payloadSize);
	char* payload_test = new char[payloadSize + 1];
	for (unsigned __int64 i = 0; i < payloadSize; i++) {
		payload_test[i] = (payload[i] ^ psession->masks[i % 4]);
	}

	payload_test[payloadSize] = '\0';
	

	// check if the frame is marked as the final frame in the message
	if (fin != 0) {
		// check if this is the first frame in the message
		if (opcode != 0x0) {
			memcpy(psession->pending_frame_buffer_Rev + psession->pending_frame_buffer_size, payload, payloadSize);
			psession->pending_frame_buffer_size = psession->pending_frame_buffer_size + payloadSize;
		}
		else {
					
			// push frame payload data onto message buffer			
			memcpy(psession->pending_frame_buffer_Rev + psession->pending_frame_buffer_size, payload, payloadSize);
			// increase message payload data length
			psession->pending_frame_buffer_size = psession->pending_frame_buffer_size + payloadSize;
			/*WebsocketDataMessage* pMessage = new WebsocketDataMessage(psession->pending_frame_buffer_Rev);

			string rMessage = pMessage->raw;*/
			// process the message
			//bool result = wsProcessClientMessage(clientID, client->MessageOpcode, client->MessageBuffer, client->MessageBufferLength);

			// check if the client wasn't removed, then reset message buffer and message opcode
			psession->pending_frame_buffer_size = 0;
			for (int i = 0; i < 900000; i++)
			{
				psession->pending_frame_buffer_Rev [i] = '\0';
			}						
		}
	}
	else
	{
		memcpy(psession->pending_frame_buffer_Rev + psession->pending_frame_buffer_size, payload_test, payloadSize);
		psession->pending_frame_buffer_size = psession->pending_frame_buffer_size + payloadSize;
		int i = 0;
	}	
	buffer.Append(payload, payloadSize);
	delete[] payload;
}

void FrameAndDeframeMessage::frameOutgoingMessage(char* inputdata, DataBuffer& buffer, int nWrittenBytes)
{
	CString stlog = L"";
	/*stlog.Format(L"Payload Size:%d ", nWrittenBytes);
	CStaticClass::m_logfile.LogEvent(stlog);*/

		WebsocketDataMessage& dataMessage = WebsocketDataMessage(inputdata);
		string strData = dataMessage.GetEncodedData();

		//unsigned __int64 payloadSize = strData.size();
		unsigned __int64 payloadSize = nWrittenBytes;
		

		int expectedSize = payloadSize + 1; //flags byte.
		if (payloadSize <= 125 && payloadSize <= 65535)
			expectedSize += 1;
		else if (payloadSize > 125 && payloadSize <= 65535)
			expectedSize += 3;
		else
			expectedSize += 9;

		if (expectedSize > buffer.getRemainingSize())
			return ;

		//create the flags byte
		char payloadFlags = 130;
		buffer.Append(&payloadFlags, 1);

		//create the length byte
		if (payloadSize <= 125)
		{
			char basicSize = payloadSize;
			buffer.Append(&basicSize, 1);
		}
		else if (payloadSize > 125 & payloadSize <= 65535)
		{
			char basicSize = 126;
			buffer.Append(&basicSize, 1);

			char len[2];
			len[0] = (payloadSize >> 8) & 255;
			len[1] = (payloadSize) & 255;
			buffer.Append(len, 2);
		}
		else
		{
			char basicSize = 127;
			buffer.Append(&basicSize, 1);

			char len[8];
			len[0] = (payloadSize >> 56) & 255;
			len[1] = (payloadSize >> 48) & 255;
			len[2] = (payloadSize >> 40) & 255;
			len[3] = (payloadSize >> 32) & 255;
			len[4] = (payloadSize >> 24) & 255;
			len[5] = (payloadSize >> 16) & 255;
			len[6] = (payloadSize >> 8) & 255;
			len[7] = (payloadSize) & 255;
			buffer.Append(len, 8);
		}
		//int dataSize = strData.size();		
		//stlog.Format(L"Data Size for Update:%d ", dataSize);
		//CStaticClass::m_logfile.LogEvent(stlog);
		buffer.Append(inputdata, nWrittenBytes);
		nWrittenBytes = expectedSize;
		return ;		
}