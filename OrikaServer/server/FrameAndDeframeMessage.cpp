#include "stdafx.h"
#include <Windows.h>
#include "FrameAndDeframeMessage.h"
#include <sstream>
#include "base64/base64.h"
#include "sha1/sha1.h"
#include "WebsocketDataMessage.h"
#include <vector> // Include this header for std::vector
#include "StaticClass.h"

// S4: hard cap on WebSocket payload to bound allocation and bufferSize math.
// 1 MB is well above any legitimate JSON message in this protocol; raise only
// after auditing every consumer for int-overflow safety.
static const unsigned __int64 MAX_WEBSOCKET_PAYLOAD_SIZE = 1ULL * 1024ULL * 1024ULL;

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
	DataBuffer m_databuffer;
	m_databuffer.Allocate(100000);
	m_databuffer.Append(psession->unprocessBuffer, psession->unprocessBuffer_size);
	m_databuffer.Append(incomingBuffer, bufferSize);
	psession->unprocessBuffer_size = 0;
	int payloadSize = 0;
	int masksOffset = 0;
	int extra_headerSize = 0;
	char frameHeader = m_databuffer.GetBuffer()[0];
	string  returnval = "";
	string preMessage_t_s = "";
	//In the other cases, we should expect a data message : 
	int nMinExpectedSize = 6;
	if (m_databuffer.GetDataSize() < nMinExpectedSize)
	{
		m_databuffer.Clear();
		return ;
	}

	// S4: RFC 6455 requires MASK bit on every client->server frame. The code
	// below XORs payload bytes with mask[i%4] unconditionally; if MASK is 0,
	// mask[] is unset noise and the payload is corrupted. Reject early.
	if (psession->frame_buffer_size == 0 && (getAt(m_databuffer.GetBuffer(), 1) & 0x80) == 0)
	{
		m_databuffer.Clear();
		return ;
	}

	bool isFin = (frameHeader & 0x80) != 0;
	bool isContinuation = (getAt(m_databuffer.GetBuffer(), 0) & 0x0F) == 0x0;
	BYTE payloadFlags = getAt(m_databuffer.GetBuffer(), 0);
	if (psession->frame_buffer_size == 0)
	{
		BYTE basicSize = getAt(m_databuffer.GetBuffer(), 1) & 0x7F;
		if (basicSize <= 125)
		{
			payloadSize = basicSize;
			masksOffset = 2;
			extra_headerSize = 4;
		}

		else if (basicSize == 126)
		{
			nMinExpectedSize += 2;
			if (m_databuffer.GetDataSize() < nMinExpectedSize)
			{
				m_databuffer.Clear();
				return ;
			}

			payloadSize = ntohs(*(u_short*)(m_databuffer.GetBuffer() + 2));
			masksOffset = 4;
			extra_headerSize = 4;
		}
		else if (basicSize == 127)
		{
			nMinExpectedSize += 8;
			if (m_databuffer.GetDataSize() < nMinExpectedSize)
			{
				m_databuffer.Clear();
				return ;
			}
			// S4: 8-byte length, big-endian. The previous code read only 4
			// bytes from offset 6, dropping the high 32 bits and corrupting
			// any payload above ~4 GB.
			unsigned __int64 lenHi = (unsigned __int64)ntohl(*(u_long*)(m_databuffer.GetBuffer() + 2));
			unsigned __int64 lenLo = (unsigned __int64)ntohl(*(u_long*)(m_databuffer.GetBuffer() + 6));
			unsigned __int64 fullLen = (lenHi << 32) | lenLo;
			// S4: reject oversized payloads before they can drive the int
			// payloadSize / nMinExpectedSize math to overflow.
			if (fullLen > MAX_WEBSOCKET_PAYLOAD_SIZE)
			{
				m_databuffer.Clear();
				return ;
			}
			payloadSize = (int)fullLen;
			masksOffset = 10;
			extra_headerSize = 4;
		}
		else
		{
			m_databuffer.Clear();
			return ;
		}
		psession->fin = isFin;
		memcpy(psession->frame_buffer, m_databuffer.GetBuffer() + masksOffset + extra_headerSize, m_databuffer.GetDataSize() - masksOffset - extra_headerSize);
		psession->frame_buffer_size = (m_databuffer.GetDataSize() - masksOffset - extra_headerSize);
		psession->remaining_frame_buffer_size = payloadSize - psession->frame_buffer_size;
		memcpy(psession->masks, m_databuffer.GetBuffer() + masksOffset, 4);
	}
	else
	{
		if (psession->remaining_frame_buffer_size > 0)
		{
			if (psession->remaining_frame_buffer_size >= m_databuffer.GetDataSize())
			{
				memcpy(psession->frame_buffer + psession->frame_buffer_size, m_databuffer.GetBuffer(), m_databuffer.GetDataSize());
				psession->frame_buffer_size = psession->frame_buffer_size + m_databuffer.GetDataSize();
				psession->remaining_frame_buffer_size = psession->remaining_frame_buffer_size - m_databuffer.GetDataSize();
			}
			else
			{
				memcpy(psession->frame_buffer + psession->frame_buffer_size, m_databuffer.GetBuffer(), psession->remaining_frame_buffer_size);
				psession->frame_buffer_size = psession->frame_buffer_size + psession->remaining_frame_buffer_size;
				memcpy(psession->unprocessBuffer, m_databuffer.GetBuffer() + psession->remaining_frame_buffer_size, m_databuffer.GetDataSize() - psession->remaining_frame_buffer_size);
				psession->unprocessBuffer_size = m_databuffer.GetDataSize() - psession->remaining_frame_buffer_size;
				psession->remaining_frame_buffer_size = 0;
			}
		}
	}
	if (psession->remaining_frame_buffer_size == 0)
	{
		char* payload_t = new char[psession->frame_buffer_size + 1];
		for (unsigned __int64 i = 0; i < psession->frame_buffer_size; i++)
		{
			payload_t[i] = (psession->frame_buffer[i] ^ psession->masks[i % 4]);
		}
		payload_t[psession->frame_buffer_size] = '\0';


		memcpy(psession->LastFrame + psession->LastFrame_size, payload_t, psession->frame_buffer_size);
		psession->LastFrame_size = psession->LastFrame_size + psession->frame_buffer_size;
		psession->frame_buffer_size = 0;

		// S5: payload_t was allocated with new char[...] and never freed.
		// Each WebSocket frame leaked frame_buffer_size + 1 bytes; under load
		// this is unbounded steady-state growth.
		delete[] payload_t;

		if (psession->fin == false)
		{
			m_databuffer.Clear();
			return ;
		}		
		/*WebsocketDataMessage* preMessage_t = new WebsocketDataMessage(psession->LastFrame);
		preMessage_t_s = preMessage_t->raw;*/
		buffer.Append(psession->LastFrame, psession->LastFrame_size);
		for (int li = 0; li < psession->LastFrame_size; li++)
		{
			psession->LastFrame[li] = '\0';
		}
		psession->LastFrame_size = 0;

	}
	else
	{
		m_databuffer.Clear();
		return ;
	}
	m_databuffer.Clear();
	/*string finalmsg = preMessage_t_s;
	return finalmsg;*/
}

void FrameAndDeframeMessage::deframeLargeIncomingMessage(char* incomingBuffer, int bufferSize, SSL_session* psession, DataBuffer& buffer)
{
	string return_data = "";	
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
		//(stlog);
		buffer.Append(inputdata, nWrittenBytes);
		nWrittenBytes = expectedSize;
		return ;		
}