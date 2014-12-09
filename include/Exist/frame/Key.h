#ifndef KEY_H
#define KEY_H

#include "IOBus.h"

namespace Exist
{

template<class K>
class Key
{
public:
	Key( K k )
		:m_key(k)
	{
	}
	
	Key( const Key& right )
		:m_key((*(K*)IOBus::s_defaultObj))
	{
		Copy( right );
	}
	
	K& operator=(const Key& right)
	{
		Copy( right );
	}
	
	void Copy( const Key &right )
	{
		m_key = right.m_key;
	}
	
	virtual ~Key()
	{
	}

	K& operator=(const K& right)
	{
		m_key = right;
	}
	
	operator K()
	{
		return m_key;
	}
		
	K m_key;
};

}

#endif //ifndef KEY_H