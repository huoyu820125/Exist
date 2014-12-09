// Value.h: interface for the Value class.
//
//////////////////////////////////////////////////////////////////////

#ifndef VALUE_H
#define VALUE_H

#include "KVData.h"
#include "Element.h"

namespace Exist
{
/*
	数值基础类
		非用户接口类，对用户透明
		将来实现Int8, Int16, int32, int64==基础数据类型访问接口时，都从此类派生
 */
template<class T>
class Value : public KVData<T, Element<T> >
{
protected:
	Value()
		:KVData<T, Element<T> >()
	{
		this->m_type = DataType::uninit;
		this->m_size = sizeof(T);
		this->m_elementType = DataType::uninit;
	}
	
	Value( const char *name, bool protracted )
		:KVData<T, Element<T> >(name, protracted)
	{
		this->m_type = DataType::uninit;
		this->m_size = sizeof(T);
		this->m_elementType = DataType::uninit;
		m_data.Init(this, 1, this->Key(), this->KeySize() );
	}

public:
	Value( const Value &value )
		:KVData<T, Element<T> >(value)
	{
		this->m_size = sizeof(T);
		this->m_elementType = DataType::uninit;
		m_data.Init(this, 1, this->Key(), this->KeySize() );
	}
	
	Value& operator=( const Value &right )
	{
		this->Copy(right);
		m_data.Init(this, 1, this->Key(), this->KeySize() );
		return *this;
	}

	virtual ~Value()
	{
	}

	operator T()
	{
		return m_data;
	}


/*
	派生类重载运算符
	T& operator=(const T right)
	{
		m_data = right;
		return m_data;
	}
	T& operator+=(const T right)
	{
		m_data += right;
		return m_data;
	}
	T& operator-=(const T right)
	{
		m_data -= right;
		return m_data;
	}
	T& operator*=(const T right)
	{
		m_data *= right;
		return m_data;
	}
	T& operator/=(const T right)
	{
		m_data /= right;
		return m_data;
	}
*/

protected:
	Element<T> m_data;//数值是只有1个元素的数据
};

}

#endif // ifndef VALUE_H

