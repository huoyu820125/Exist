// Stream.h: interface for the Stream class.
//
//////////////////////////////////////////////////////////////////////

#ifndef STREAM_H
#define STREAM_H

#include "./frame/KVData.h"
#include "./frame/Element.h"
#include <string>

namespace Exist
{

class Stream : public KVData<unsigned char*, Element<unsigned char*> >
{
protected:
	Stream();

public:
	Stream( const char *name );
	Stream( const Stream &value );
	Stream& operator=( const Stream &right );
	virtual ~Stream();

public:
	/*
		不会new，如果pData够大，保存数据到pData，
		否则返回false,size返回需要的长度
	*/
	bool GetData(unsigned char *pData, int &size);
	unsigned char* GetData(int &size);//会new合适大小的内存到返回，用完记得释放
	bool SetData(const std::string data, int size);
};

}

namespace SSD
{

class Stream : public Exist::KVData<unsigned char*, Exist::Element<unsigned char*> >
{
protected:
	Stream();

public:
	Stream( const char *name );
	Stream( const Stream &value );
	Stream& operator=( const Stream &right );
	virtual ~Stream();

public:
	/*
		不会new，如果pData够大，保存数据到pData，
		否则返回false,size返回需要的长度
	*/
	bool GetData(unsigned char *pData, int &size);
	unsigned char* GetData(int &size);//会new合适大小的内存到返回，用完记得释放
	bool SetData(const std::string data, int size);
};

}
#endif // ifndef INT8_H
