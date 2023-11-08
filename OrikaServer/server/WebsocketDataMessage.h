#pragma once
#include "WebsocketMessage.h"
#include <string.h>
#include <ctype.h>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

class  WebsocketDataMessage : public WebsocketMessage
{
public:
	WebsocketDataMessage(const char* buffer);
	WebsocketDataMessage(int serviceId);
	~WebsocketDataMessage(void);
	
	int GetServiceId();
	void SetServiceId(int serviceId);
	void SetArguments(string arg1, string arg2 = "");
	string GetArg1();
	string GetArg2();
    bool Encode();
	bool Decode();
	string GetEncodedData();
	string raw;
private:
	int serviceId;
	string arg1;
	string arg2;

	

};

