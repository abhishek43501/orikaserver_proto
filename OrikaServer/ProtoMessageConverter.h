#include "..\server\DataBuffer.h"
class ProtoMessageConverter
{
public:
	string convertprotoToString(const char* protomessge,int datasize);

	string createloginresponse(string token);
	void JasonToProto(string jason, DataBuffer& buffer);
	
};

