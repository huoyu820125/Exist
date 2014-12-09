// UInt64.cpp: implementation of the UInt64 class.
//
//////////////////////////////////////////////////////////////////////

#include "../../include/Exist/UInt64.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace Exist
{

UInt64::UInt64()
:Value<mdk::uint64>()
{
	m_type = DataType::uInt64;
	IOBus::CreateExistObj( this, m_type );
}

UInt64::UInt64( const char *name )
:Value<mdk::uint64>(name, false)
{
	m_type = DataType::uInt64;
	IOBus::CreateExistObj( this, m_type );
}

UInt64::UInt64( const UInt64 &value )
:Value<mdk::uint64>(value)
{
	m_type = DataType::uInt64;
	IOBus::CreateExistObj( this, m_type );
}

UInt64& UInt64::operator=( const UInt64 &right )
{
	Copy(right);
	return *this;
}

UInt64::~UInt64()
{
}

UInt64& UInt64::operator=(const mdk::uint64 right)
{
	m_data = right;
	return *this;
}
	
UInt64& UInt64::operator+=(const mdk::uint64 right)
{
	m_data += right;
	return *this;
}

UInt64& UInt64::operator-=(const mdk::uint64 right)
{
	m_data -= right;
	return *this;
}

UInt64& UInt64::operator*=(const mdk::uint64 right)
{
	m_data *= right;
	return *this;
}

UInt64& UInt64::operator/=(const mdk::uint64 right)
{
	m_data /= right;
	return *this;
}

}
namespace SSD
{

	UInt64::UInt64()
		:Exist::Value<mdk::uint64>()
	{
		m_type = DataType::uInt64;
		Exist::IOBus::CreateExistObj( this, m_type );
	}

	UInt64::UInt64( const char *name )
		:Exist::Value<mdk::uint64>(name, true)
	{
		m_type = DataType::uInt64;
		Exist::IOBus::CreateExistObj( this, m_type );
	}

	UInt64::UInt64( const UInt64 &value )
		:Exist::Value<mdk::uint64>(value)
	{
		m_type = DataType::uInt64;
		Exist::IOBus::CreateExistObj( this, m_type );
	}

	UInt64& UInt64::operator=( const UInt64 &right )
	{
		Copy(right);
		return *this;
	}

	UInt64::~UInt64()
	{
	}

	UInt64& UInt64::operator=(const mdk::uint64 right)
	{
		m_data = right;
		return *this;
	}

	UInt64& UInt64::operator+=(const mdk::uint64 right)
	{
		m_data += right;
		return *this;
	}

	UInt64& UInt64::operator-=(const mdk::uint64 right)
	{
		m_data -= right;
		return *this;
	}

	UInt64& UInt64::operator*=(const mdk::uint64 right)
	{
		m_data *= right;
		return *this;
	}

	UInt64& UInt64::operator/=(const mdk::uint64 right)
	{
		m_data /= right;
		return *this;
	}

}
