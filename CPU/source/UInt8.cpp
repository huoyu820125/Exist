// UInt8.cpp: implementation of the UInt8 class.
//
//////////////////////////////////////////////////////////////////////

#include "../../include/Exist/UInt8.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace Exist
{

UInt8::UInt8()
:Value<mdk::uint8>()
{
	m_type = DataType::uInt8;
	IOBus::CreateExistObj( this, m_type );
}

UInt8::UInt8( const char *name )
:Value<mdk::uint8>(name, false)
{
	m_type = DataType::uInt8;
	IOBus::CreateExistObj( this, m_type );
}

UInt8::UInt8( const UInt8 &value )
:Value<mdk::uint8>(value)
{
	m_type = DataType::uInt8;
	IOBus::CreateExistObj( this, m_type );
}

UInt8& UInt8::operator=( const UInt8 &right )
{
	Copy(right);
	return *this;
}

UInt8::~UInt8()
{
}

UInt8& UInt8::operator=(const mdk::uint8 right)
{
	m_data = right;
	return *this;
}
	
UInt8& UInt8::operator+=(const mdk::uint8 right)
{
	m_data += right;
	return *this;
}

UInt8& UInt8::operator-=(const mdk::uint8 right)
{
	m_data -= right;
	return *this;
}

UInt8& UInt8::operator*=(const mdk::uint8 right)
{
	m_data *= right;
	return *this;
}

UInt8& UInt8::operator/=(const mdk::uint8 right)
{
	m_data /= right;
	return *this;
}

}
namespace SSD
{

	UInt8::UInt8()
		:Exist::Value<mdk::uint8>()
	{
		m_type = DataType::uInt8;
		Exist::IOBus::CreateExistObj( this, m_type );
	}

	UInt8::UInt8( const char *name )
		:Exist::Value<mdk::uint8>(name, true)
	{
		m_type = DataType::uInt8;
		Exist::IOBus::CreateExistObj( this, m_type );
	}

	UInt8::UInt8( const UInt8 &value )
		:Exist::Value<mdk::uint8>(value)
	{
		m_type = DataType::uInt8;
		Exist::IOBus::CreateExistObj( this, m_type );
	}

	UInt8& UInt8::operator=( const UInt8 &right )
	{
		Copy(right);
		return *this;
	}

	UInt8::~UInt8()
	{
	}

	UInt8& UInt8::operator=(const mdk::uint8 right)
	{
		m_data = right;
		return *this;
	}

	UInt8& UInt8::operator+=(const mdk::uint8 right)
	{
		m_data += right;
		return *this;
	}

	UInt8& UInt8::operator-=(const mdk::uint8 right)
	{
		m_data -= right;
		return *this;
	}

	UInt8& UInt8::operator*=(const mdk::uint8 right)
	{
		m_data *= right;
		return *this;
	}

	UInt8& UInt8::operator/=(const mdk::uint8 right)
	{
		m_data /= right;
		return *this;
	}

}
