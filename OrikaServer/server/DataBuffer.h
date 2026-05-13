
#ifndef DataBuffer__INCLUDED
#define DataBuffer__INCLUDED

#pragma once




class  DataBuffer
{
public:
    DataBuffer(unsigned int uMaxSize);
    DataBuffer();
    ~DataBuffer(void);

    void Allocate(unsigned int uMaxSize);
    bool Append(char* _pBuffer, unsigned int _uSize);
	void Clear();   
    void Delete();
    bool Append(unsigned char* _pBuffer, unsigned int _uSize);
    void Pop(int _uSize);
    unsigned int GetDataSize();
    unsigned int GetMaxDataSize();
    char* GetBuffer();
    void GrowSize(unsigned int growBy);
    unsigned int getRemainingSize();
    char getAt(int offset);
private:
    char* pBuffer;
    unsigned int uSize;
    unsigned int uMaxSize;
};



#endif // DataBuffer__INCLUDED
