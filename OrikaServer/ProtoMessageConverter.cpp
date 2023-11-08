#include "stdafx.h"
#include "ProtoMessageConverter.h"
#include "..\ProtoFile\login.pb.h"
#include "..\ProtoFile\clientmessage.pb.h"
#include "..\ProtoFile\loginresponse.pb.h"
#include "..\ProtoFile\tabcolumn.pb.h"
#include "..\ProtoFile\tabdata.pb.h"
#include "..\ProtoFile\tab.pb.h"
#include <google/protobuf/stubs/status_macros.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <google/protobuf/util/json_util.h>
#include "StaticClass.h"
#include <openssl/bio.h>
#include <openssl/evp.h>
#include "document.h"
#include <lzo1z.h>
#include <lzoconf.h>
#include "LZ\lzstring.h"
using namespace rapidjson;
string ProtoMessageConverter::convertprotoToString(const char* protomessge, int datasize)
{
	string rval = "";
	Oreka::ClientMessage* m_clientmessage = new Oreka::ClientMessage();
	m_clientmessage->ParseFromArray(protomessge, datasize);
	google::protobuf::util::MessageToJsonString(*m_clientmessage, &rval);
	return rval;
}


std::string base64_encode(const std::string& input) {
	const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	std::string encoded;
	int i = 0;
	int j = 0;
	int in_len = input.length();
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--) {
		char_array_3[i++] = input[j++];
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (i = 0; i < 4; i++) {
				encoded += base64_chars[char_array_4[i]];
			}
			i = 0;
		}
	}

	if (i > 0) {
		for (int k = i; k < 3; k++) {
			char_array_3[k] = 0;
		}

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

		for (int k = 0; k < i + 1; k++) {
			encoded += base64_chars[char_array_4[k]];
		}

		while (i++ < 3) {
			encoded += '=';
		}
	}

	return encoded;
}


string ProtoMessageConverter::createloginresponse(string token)
{
	string strJsonResult = "";
	return strJsonResult;

}
void  ProtoMessageConverter::JasonToProto(string jason, DataBuffer& buffer)
{
	CString strType = L"";
	rapidjson::Document sub_document;
	if (!sub_document.Parse<0>(jason.c_str()).HasParseError())
	{
		if (sub_document.HasMember("type") == true)
		{
			const Value& Keyuser = sub_document["type"];
			strType = Keyuser.GetString();
		}
	}
	CString strRealJason(jason.c_str());
	//CStaticClass::m_logfile.LogEvent(L"Request: " + strRealJason);
	CString strprototype = L"";
	CStaticClass::m_protoandjsonmessage.Lookup(strType, strprototype);
	CString finaljason=L"";
	finaljason.Format(L"{\"type\":\"%s\",\"%s\":%s}", strType, strprototype, strRealJason);
	// Create a new protobuf object]
	Oreka::ClientMessage m_clientmessage;
	// Parse the JSON string and populate the protobuf object
	google::protobuf::util::JsonParseOptions jsonOptions;
	jsonOptions.ignore_unknown_fields = true;

	string strforconvert = CT2A(finaljason.GetString());

	auto status = google::protobuf::util::JsonStringToMessage(strforconvert, &m_clientmessage, jsonOptions);
	std::string serializeData;
	m_clientmessage.SerializeToString(&serializeData);
	//CString msg(serializeData.c_str());

	

	int serializedSize = m_clientmessage.ByteSizeLong();
	// Create a buffer to store the serialized data
	char* charBuffer = new char[serializedSize];
	// Serialize the message into the charBuffer
	m_clientmessage.SerializeToArray(charBuffer, serializedSize);
	buffer.Append(charBuffer, serializedSize);


	//////Extra Code for comment



	//int dataSize = serializeData.size();

	//CString stlog = L"";
	//stlog.Format(L"Binary Data Size:%d ", dataSize);
	//CStaticClass::m_logfile.LogEvent(stlog);


	// string base64data = base64_encode(serializeData);
	// CString strBase64(base64data.c_str());
	// CStaticClass::m_logfile.LogEvent(L"Base64Data:"+ strBase64);



	//Oreka::ClientMessage m_clientmessage_test;
	//m_clientmessage_test.ParseFromArray(charBuffer, dataSize);

	//std::string strJsonResult = "";
	//google::protobuf::util::MessageToJsonString(m_clientmessage_test, &strJsonResult);
	//CString logstr(strJsonResult.c_str());
	//dataSize = logstr.GetLength();
	//stlog.Format(L"Jason Format Data Size:%d ", dataSize);
	//CStaticClass::m_logfile.LogEvent(L"Response: " + logstr);
	//stlog.Format(L"Jason Data Size:%d ", logstr.GetLength());
	//CStaticClass::m_logfile.LogEvent(stlog);




	//LZString m_obj;
	//string utf8_CompressedData = "";


	//string utf8_UncompressedData = "";

	//char* chrData = const_cast<char*>(strJsonResult.c_str());


	//int Datalen = strlen(chrData);
	//CString strLog = L"";
	////strLog.Format(L"Uncompressed Data Size %d", Datalen);
	////CStaticClass::m_logfile.LogEvent(strLog);
	//QString  strUnCompressData = QString::fromUtf8((const char*)chrData, Datalen);
	//QString strcompressed = m_obj.compressToUTF16(strUnCompressData);
	//utf8_CompressedData = strcompressed.toUtf8().constData();

	//char* chrDataCompressed = const_cast<char*>(utf8_CompressedData.c_str());

	//int ConpressedDataSize = strlen(chrDataCompressed);
	//strLog.Format(L"Compressed Data Size of Jason %d", ConpressedDataSize);
	//CStaticClass::m_logfile.LogEvent(strLog);

	
}