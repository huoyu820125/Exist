// Int8.cpp: implementation of the Int8 class.
//
//////////////////////////////////////////////////////////////////////

#include "../../include/Exist/Int8.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace Exist
{

Int8::Int8()
:Value<mdk::int8>()
{
	m_type = DataType::int8;
	IOBus::CreateExistObj( this, m_type );
}

Int8::Int8( const char *name )
:Value<mdk::int8>(name, false)
{
	m_type = DataType::int8;
	IOBus::CreateExistObj( this, m_type );
}

Int8::Int8( const Int8 &value )
:Value<mdk::int8>(value)
{
	m_type = DataType::int8;
	IOBus::CreateExistObj( this, m_type );
}

Int8& Int8::operator=( const Int8 &right )
{
	Copy(right);
	return *this;
}

Int8::~Int8()
{
}

Int8& Int8::operator=(const mdk::int8 right)
{
	m_data = right;
	return *this;
}
	
Int8& Int8::operator+=(const mdk::int8 right)
{
	m_data += right;
	return *this;
}

Int8& Int8::operator-=(const mdk::int8 right)
{
	m_data -= right;
	return *this;
}

Int8& Int8::operator*=(const mdk::int8 right)
{
	m_data *= right;
	return *this;
}

Int8& Int8::operator/=(const mdk::int8 right)
{
	m_data /= right;
	return *this;
}

}
namespace SSD
{

	Int8::Int8()
		:Exist::Value<mdk::int8>()
	{
		m_type = DataType::int8;
		Exist::IOBus::CreateExistObj( this, m_type );
	}

	Int8::Int8( const char *name )
		:Exist::Value<mdk::int8>(name, true)
	{
		m_type = DataType::int8;
		Exist::IOBus::CreateExistObj( this, m_type );
	}

	Int8::Int8( const Int8 &value )
		:Exist::Value<mdk::int8>(value)
	{
		m_type = DataType::int8;
		Exist::IOBus::CreateExistObj( this, m_type );
	}

	Int8& Int8::operator=( const Int8 &right )
	{
		Copy(right);
		return *this;
	}

	Int8::~Int8()
	{
	}

	Int8& Int8::operator=(const mdk::int8 right)
	{
		m_data = right;
		return *this;
	}

	Int8& Int8::operator+=(const mdk::int8 right)
	{
		m_data += right;
		return *this;
	}

	Int8& Int8::operator-=(const mdk::int8 right)
	{
		m_data -= right;
		return *this;
	}

	Int8& Int8::operator*=(const mdk::int8 right)
	{
		m_data *= right;
		return *this;
	}

	Int8& Int8::operator/=(const mdk::int8 right)
	{
		m_data /= right;
		return *this;
	}

}
