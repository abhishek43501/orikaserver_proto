#include "StdAfx.h"
#include "DataBuffer.h"




//Abhishek
DataBuffer::DataBuffer(unsigned int uMaxSize)
{
    uSize = 0;
	Allocate(uMaxSize);
}

DataBuffer::DataBuffer()
{
    uSize = 0;
    pBuffer = NULL;
}

void DataBuffer::Allocate( unsigned int uMaxSize )
{    	
	this->uMaxSize = uMaxSize;	
    pBuffer = new char[uMaxSize];
	
}

DataBuffer::~DataBuffer(void)
{
    delete[] pBuffer;
}
bool DataBuffer::Append( char* _pBuffer, unsigned int _uSize )
{
    if(getRemainingSize() < _uSize)
        return false;

    //Copy the data :
    ::memcpy(pBuffer+uSize,_pBuffer,_uSize);

    //Update the size of the buffer.
    uSize+=_uSize;

    return true;
}



void DataBuffer::Clear()         
   { 	    
	   
	   if(pBuffer) 
	   { 
		   delete[] pBuffer; 
		   pBuffer=NULL; 
	   }
	   char* new_char=new char[10000000];
	   pBuffer=new_char;
	   this->uMaxSize = 10000000;
	   uSize=0;
	   
	   
   }


bool DataBuffer::Append( unsigned char* _pBuffer, unsigned int _uSize )
{
    return Append((char*) _pBuffer, _uSize);
}

void DataBuffer::Pop( int _uSize )
{
    ::memmove(pBuffer,pBuffer+_uSize,(uSize-_uSize));
    uSize-=_uSize;
}

unsigned int DataBuffer::GetDataSize()
{
    return uSize;
}

unsigned int DataBuffer::GetMaxDataSize()
{
    return uMaxSize;
}

char* DataBuffer::GetBuffer()
{
    return pBuffer;
}

unsigned int DataBuffer::getRemainingSize()
{
    return uMaxSize - uSize;
}

void DataBuffer::GrowSize( unsigned int growBy )
{
    uSize += growBy;
}

char DataBuffer::getAt( int offset )
{
    return pBuffer[offset];
}




