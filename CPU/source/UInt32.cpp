// UInt32.cpp: implementation of the UInt32 class.
//
//////////////////////////////////////////////////////////////////////

#include "../../include/Exist/UInt32.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace Exist
{

UInt32::UInt32()
:Value<mdk::uint32>()
{
	m_type = DataType::uInt32;
	IOBus::CreateExistObj( this, m_type );
}

UInt32::UInt32( const char *name )
:Value<mdk::uint32>(name, false)
{
	m_type = DataType::uInt32;
	IOBus::CreateExistObj( this, m_type );
}

UInt32::UInt32( const UInt32 &value )
:Value<mdk::uint32>(value)
{
	m_type = DataType::uInt32;
	IOBus::CreateExistObj( this, m_type );
}

UInt32& UInt32::operator=( const UInt32 &right )
{
	Copy(right);
	return *this;
}

UInt32::~UInt32()
{
}

UInt32& UInt32::operator=(const mdk::uint32 right)
{
	m_data = right;
	return *this;
}
	
UInt32& UInt32::operator+=(const mdk::uint32 right)
{
	m_data += right;
	return *this;
}

UInt32& UInt32::operator-=(const mdk::uint32 right)
{
	m_data -= right;
	return *this;
}

UInt32& UInt32::operator*=(const mdk::uint32 right)
{
	m_data *= right;
	return *this;
}

UInt32& UInt32::operator/=(const mdk::uint32 right)
{
	m_data /= right;
	return *this;
}

}
namespace SSD
{

	UInt32::UInt32()
		:Exist::Value<mdk::uint32>()
	{
		m_type = DataType::uInt32;
		Exist::IOBus::CreateExistObj( this, m_type );
	}

	UInt32::UInt32( const char *name )
		:Exist::Value<mdk::uint32>(name, true)
	{
		m_type = DataType::uInt32;
		Exist::IOBus::CreateExistObj( this, m_type );
	}

	UInt32::UInt32( const UInt32 &value )
		:Exist::Value<mdk::uint32>(value)
	{
		m_type = DataType::uInt32;
		Exist::IOBus::CreateExistObj( this, m_type );
	}

	UInt32& UInt32::operator=( const UInt32 &right )
	{
		Copy(right);
		return *this;
	}

	UInt32::~UInt32()
	{
	}

	UInt32& UInt32::operator=(const mdk::uint32 right)
	{
		m_data = right;
		return *this;
	}

	UInt32& UInt32::operator+=(const mdk::uint32 right)
	{
		m_data += right;
		return *this;
	}

	UInt32& UInt32::operator-=(const mdk::uint32 right)
	{
		m_data -= right;
		return *this;
	}

	UInt32& UInt32::operator*=(const mdk::uint32 right)
	{
		m_data *= right;
		return *this;
	}

	UInt32& UInt32::operator/=(const mdk::uint32 right)
	{
		m_data /= right;
		return *this;
	}

}
