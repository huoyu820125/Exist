// Int64.cpp: implementation of the Int64 class.
//
//////////////////////////////////////////////////////////////////////

#include "../../include/Exist/Int64.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace Exist
{

Int64::Int64()
:Value<mdk::int64>()
{
	m_type = DataType::int64;
	IOBus::CreateExistObj( this, m_type );
}

Int64::Int64( const char *name )
:Value<mdk::int64>(name, false)
{
	m_type = DataType::int64;
	IOBus::CreateExistObj( this, m_type );
}

Int64::Int64( const Int64 &value )
:Value<mdk::int64>(value)
{
	m_type = DataType::int64;
	IOBus::CreateExistObj( this, m_type );
}

Int64& Int64::operator=( const Int64 &right )
{
	Copy(right);
	return *this;
}

Int64::~Int64()
{
}

Int64& Int64::operator=(const mdk::int64 right)
{
	m_data = right;
	return *this;
}
	
Int64& Int64::operator+=(const mdk::int64 right)
{
	m_data += right;
	return *this;
}

Int64& Int64::operator-=(const mdk::int64 right)
{
	m_data -= right;
	return *this;
}

Int64& Int64::operator*=(const mdk::int64 right)
{
	m_data *= right;
	return *this;
}

Int64& Int64::operator/=(const mdk::int64 right)
{
	m_data /= right;
	return *this;
}

}
namespace SSD
{

	Int64::Int64()
		:Exist::Value<mdk::int64>()
	{
		m_type = DataType::int64;
		Exist::IOBus::CreateExistObj( this, m_type );
	}

	Int64::Int64( const char *name )
		:Exist::Value<mdk::int64>(name, true)
	{
		m_type = DataType::int64;
		Exist::IOBus::CreateExistObj( this, m_type );
	}

	Int64::Int64( const Int64 &value )
		:Exist::Value<mdk::int64>(value)
	{
		m_type = DataType::int64;
		Exist::IOBus::CreateExistObj( this, m_type );
	}

	Int64& Int64::operator=( const Int64 &right )
	{
		Copy(right);
		return *this;
	}

	Int64::~Int64()
	{
	}

	Int64& Int64::operator=(const mdk::int64 right)
	{
		m_data = right;
		return *this;
	}

	Int64& Int64::operator+=(const mdk::int64 right)
	{
		m_data += right;
		return *this;
	}

	Int64& Int64::operator-=(const mdk::int64 right)
	{
		m_data -= right;
		return *this;
	}

	Int64& Int64::operator*=(const mdk::int64 right)
	{
		m_data *= right;
		return *this;
	}

	Int64& Int64::operator/=(const mdk::int64 right)
	{
		m_data /= right;
		return *this;
	}

}
