// Element.h: interface for the Element class.
//
//////////////////////////////////////////////////////////////////////

#ifndef ELEMENT_H
#define ELEMENT_H

#include "KVData.h"

namespace Exist
{

namespace SearchType
{
	enum SearchType
	{
		stIndex = 0,
		stKey,
	};

}
/*
	元素类
		不管是容器还是数值，都有自己的元素，数值是只有1个元素的容器
		不早不晚，及时且正确时机的去进行网络IO操作
		1.可以准确的判断，用户是要读还是写
			对于KVData及其子类来说，在取得一个接口对象时，无法判断用户是要用这个接口来读还是写
			所以需要此类来
		2.也可以用于对一个元素反复IO
			对于一个KVData,如果取出后保存在c/c++基础/自定义类型中，则无法使用这个对象来操作数据
			所以需要此类来操作
 */
template<class T>
class Element
{
public:
	Element()
	{
		m_searchType = SearchType::stKey;
		m_index = NULL;
		m_keySize = 0;
		m_key[m_keySize] = 0;
	}
	
	Element( KVData<T, Element<T> > *owner, int indexType, const void *index, int size )
		:m_data((*(T*)IOBus::s_defaultObj))
	{
		Init( owner, indexType, index, size );
	}

	Element( const Element &value )
		:m_data((*(T*)IOBus::s_defaultObj))
	{
		Copy( value );
	}

	void Init( KVData<T, Element<T> > *owner, int searchType, const void *index, int size )
	{
		m_owner = owner;
/*
		m_searchType = (SearchType::SearchType)searchType;
		m_keySize = size;
		if ( SearchType::stKey == m_searchType )
		{
			if ( m_keySize > MAX_KEY_SIZE ) m_keySize = MAX_KEY_SIZE;
			memcpy(m_key, index, m_keySize);
			m_key[m_keySize] = 0;
			m_hash = IOBus::Hash(m_key, m_keySize);//计算hash
		}
		else 
		{
			memcpy(&m_index, index, m_keySize);
		}
*/
	}

	void Copy( const Element &value )
	{
		m_owner = value.m_owner;
		m_searchType = value.m_searchType;
		m_keySize = value.m_keySize;
		m_index = value.m_index;
		memcpy(m_key, value.m_key, m_keySize);
		m_key[m_keySize] = 0;
		m_data = value.m_data;
	}
	
	virtual ~Element()
	{
		IOBus::ReleaseExistObj( this );
	}

	Element& operator=(const Element& right)
	{
		Copy(right);
		return *this;
	}

	T& operator=(const T &right)
	{
		/*
			暂不支持将一个容器赋给一个容器的元素
			Exist::Map<int, Exist<int, int> > a("嵌套map");
			Exist<int, int> b("map");
			a[1] = b;//不支持这种用法
		*/
		mdk::mdk_assert( !DataType::IsContainer(m_owner->Type()) );

		m_data = right;
		if ( SearchType::stKey == m_searchType ) 
		{
			m_owner->WriteData(&m_data, sizeof(T), UpdateType::utCopy);
		}
		else 
		{
			m_owner->WriteData( m_index, &m_data, sizeof(T) );
		}
		return m_data;
	}

	T& operator+=(const T &right)
	{
		m_data = right;
		if ( SearchType::stKey == m_searchType ) 
		{
			m_owner->WriteData(&m_data, sizeof(T),UpdateType::utAddSelf);
		}
		else 
		{
			m_owner->WriteData( m_index, &m_data, sizeof(T) );
		}
		return m_data;
	}

	T& operator-=(const T &right)
	{
		m_data = right;
		if ( SearchType::stKey == m_searchType ) 
		{
			m_owner->WriteData(&m_data, sizeof(T), UpdateType::utSubtractSelf);
		}
		else 
		{
			m_owner->WriteData( m_index, &m_data, sizeof(T) );
		}
		return m_data;
	}

	T& operator*=(const T &right)
	{
		m_data = right;
		if ( SearchType::stKey == m_searchType ) 
		{
			m_owner->WriteData(&m_data, sizeof(T), UpdateType::utMultiplySelf);
		}
		else 
		{
			m_owner->WriteData( m_index, &m_data, sizeof(T) );
		}
		return m_data;
	}

	T& operator/=(const T &right)
	{
		//VC++6.0不支持int 与0.0比较，故而先创建T类型对象初始化为0.0，然后T T比较
		T zore = 0.0;
		mdk::mdk_assert( zore != right ); 

		m_data = right;
		if ( SearchType::stKey == m_searchType ) 
		{
			m_owner->WriteData(&m_data, sizeof(T), UpdateType::utDivideSelf);
		}
		else 
		{
			m_owner->WriteData( m_index, &m_data, sizeof(T) );
		}
		return m_data;
	}
	
	operator T()
	{
		//数值类型元素，直接到Exist查询数据值
		if ( DataType::IsValue(m_owner->Type()) )
		{
			int size = sizeof(T);
			if ( m_owner->QueryData(size) )
			{
				m_owner->ReadData(&m_data, size);
			}

			return m_data;
		}

/*
		//容器类型元素，请期待
		//对于容器类型的元素，就是需要先取得自己在Exist上的key
		if ( SearchType::stKey == m_searchType ) //对于Key检索方式，元素key就是元素在Exist上的真实key，所以直接copy即可
		{
			((IOBus *)&m_data)->Init(m_key, m_keySize, m_owner->IsProtracted());
		}
		else//对于下标检索方式，需要到Exist上查询自己的key
		{
			char key[MAX_KEY_SIZE+1];
			int size = MAX_KEY_SIZE;
//			m_owner->ReadData(m_index, key, size);
			((IOBus *)&m_data)->Init(key, size, m_owner->IsProtracted());
		}
*/

		return m_data;
	}

protected:
	KVData<T, Element<T> >				*m_owner;
	SearchType::SearchType				m_searchType;	//容器类元素检索方式0：下标检索，非0：key检索
	int									m_index;
	int									m_hash;
	char								m_key[MAX_KEY_SIZE+1];
	int									m_keySize;
	T									m_data;
};

}

#endif // ifndef ELEMENT_H
