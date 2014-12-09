// Int32.cpp: implementation of the Int32 class.
//
//////////////////////////////////////////////////////////////////////

#include "../../include/Exist/Int32.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace Exist
{

Int32::Int32()
:Value<mdk::int32>()
{
	m_type = DataType::int32;
	IOBus::CreateExistObj( this, m_type );
}

Int32::Int32( const char *name )
:Value<mdk::int32>(name, false)
{
	m_type = DataType::int32;
	IOBus::CreateExistObj( this, m_type );
}

Int32::Int32( const Int32 &value )
:Value<mdk::int32>(value)
{
	m_type = DataType::int32;
	IOBus::CreateExistObj( this, m_type );
}

Int32& Int32::operator=( const Int32 &right )
{
	Copy(right);
	return *this;
}

Int32::~Int32()
{
}

Int32& Int32::operator=(const mdk::int32 right)
{
	m_data = right;
	return *this;
}
	
Int32& Int32::operator+=(const mdk::int32 right)
{
	m_data += right;
	return *this;
}

Int32& Int32::operator-=(const mdk::int32 right)
{
	m_data -= right;
	return *this;
}

Int32& Int32::operator*=(const mdk::int32 right)
{
	m_data *= right;
	return *this;
}

Int32& Int32::operator/=(const mdk::int32 right)
{
	m_data /= right;
	return *this;
}

}
namespace SSD
{

	Int32::Int32()
		:Exist::Value<mdk::int32>()
	{
		m_type = DataType::int32;
		Exist::IOBus::CreateExistObj( this, m_type );
	}

	Int32::Int32( const char *name )
		:Exist::Value<mdk::int32>(name, true)
	{
		m_type = DataType::int32;
		Exist::IOBus::CreateExistObj( this, m_type );
	}

	Int32::Int32( const Int32 &value )
		:Exist::Value<mdk::int32>(value)
	{
		m_type = DataType::int32;
		Exist::IOBus::CreateExistObj( this, m_type );
	}

	Int32& Int32::operator=( const Int32 &right )
	{
		Copy(right);
		return *this;
	}

	Int32::~Int32()
	{
	}

	Int32& Int32::operator=(const mdk::int32 right)
	{
		m_data = right;
		return *this;
	}

	Int32& Int32::operator+=(const mdk::int32 right)
	{
		m_data += right;
		return *this;
	}

	Int32& Int32::operator-=(const mdk::int32 right)
	{
		m_data -= right;
		return *this;
	}

	Int32& Int32::operator*=(const mdk::int32 right)
	{
		m_data *= right;
		return *this;
	}

	Int32& Int32::operator/=(const mdk::int32 right)
	{
		m_data /= right;
		return *this;
	}

}
