// UInt16.cpp: implementation of the UInt16 class.
//
//////////////////////////////////////////////////////////////////////

#include "../../include/Exist/UInt16.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace Exist
{

UInt16::UInt16()
:Value<mdk::uint16>()
{
	m_type = DataType::uInt16;
	IOBus::CreateExistObj( this, m_type );
}

UInt16::UInt16( const char *name )
:Value<mdk::uint16>(name, false)
{
	m_type = DataType::uInt16;
	IOBus::CreateExistObj( this, m_type );
}

UInt16::UInt16( const UInt16 &value )
:Value<mdk::uint16>(value)
{
	m_type = DataType::uInt16;
	IOBus::CreateExistObj( this, m_type );
}

UInt16& UInt16::operator=( const UInt16 &right )
{
	Copy(right);
	return *this;
}

UInt16::~UInt16()
{
}

UInt16& UInt16::operator=(const mdk::uint16 right)
{
	m_data = right;
	return *this;
}
	
UInt16& UInt16::operator+=(const mdk::uint16 right)
{
	m_data += right;
	return *this;
}

UInt16& UInt16::operator-=(const mdk::uint16 right)
{
	m_data -= right;
	return *this;
}

UInt16& UInt16::operator*=(const mdk::uint16 right)
{
	m_data *= right;
	return *this;
}

UInt16& UInt16::operator/=(const mdk::uint16 right)
{
	m_data /= right;
	return *this;
}

}
namespace SSD
{

	UInt16::UInt16()
		:Exist::Value<mdk::uint16>()
	{
		m_type = DataType::uInt16;
		Exist::IOBus::CreateExistObj( this, m_type );
	}

	UInt16::UInt16( const char *name )
		:Exist::Value<mdk::uint16>(name, true)
	{
		m_type = DataType::uInt16;
		Exist::IOBus::CreateExistObj( this, m_type );
	}

	UInt16::UInt16( const UInt16 &value )
		:Exist::Value<mdk::uint16>(value)
	{
		m_type = DataType::uInt16;
		Exist::IOBus::CreateExistObj( this, m_type );
	}

	UInt16& UInt16::operator=( const UInt16 &right )
	{
		Copy(right);
		return *this;
	}

	UInt16::~UInt16()
	{
	}

	UInt16& UInt16::operator=(const mdk::uint16 right)
	{
		m_data = right;
		return *this;
	}

	UInt16& UInt16::operator+=(const mdk::uint16 right)
	{
		m_data += right;
		return *this;
	}

	UInt16& UInt16::operator-=(const mdk::uint16 right)
	{
		m_data -= right;
		return *this;
	}

	UInt16& UInt16::operator*=(const mdk::uint16 right)
	{
		m_data *= right;
		return *this;
	}

	UInt16& UInt16::operator/=(const mdk::uint16 right)
	{
		m_data /= right;
		return *this;
	}

}
