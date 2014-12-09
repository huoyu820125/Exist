// KVData.h: interface for the KVData class.
//
//////////////////////////////////////////////////////////////////////

#ifndef KVDATA_H
#define KVDATA_H

#include "IOBus.h"
	
namespace Exist
{
/*
 	key-value数据类
		非用户接口类，对用户透明
		所有数据接口的模版的基类
		不管是数值，还是容器，在Exist上都必须对应到一个key－value数据
 */
template<class T, class Element>
class KVData : public IOBus
{
public:
	KVData()
		:IOBus()
	{
	}
	
	KVData( const char *name, bool protracted )
		:IOBus(name, protracted)
	{
	}
	
	KVData( const KVData &value )
		:IOBus(value)	
	{
	}
	
	KVData& operator=( const KVData &right )
	{
		this->Copy(right);
		return *this;
	}

	virtual ~KVData()
	{
	}

};

}

#endif // ifndef KVDATA_H
