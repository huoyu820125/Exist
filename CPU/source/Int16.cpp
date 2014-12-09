// Int16.cpp: implementation of the Int16 class.
//
//////////////////////////////////////////////////////////////////////

#include "../../include/Exist/Int16.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace Exist
{

	Int16::Int16()
		:Value<mdk::int16>()
	{
		m_type = DataType::int16;
		IOBus::CreateExistObj( this, m_type );
	}

	Int16::Int16( const char *name )
		:Value<mdk::int16>(name, false)
	{
		m_type = DataType::int16;
		IOBus::CreateExistObj( this, m_type );
	}

	Int16::Int16( const Int16 &value )
		:Value<mdk::int16>(value)
	{
		m_type = DataType::int16;
		IOBus::CreateExistObj( this, m_type );
	}

	Int16& Int16::operator=( const Int16 &right )
	{
		Copy(right);
		return *this;
	}

	Int16::~Int16()
	{
	}

	Int16& Int16::operator=(const mdk::int16 right)
	{
		m_data = right;
		return *this;
	}

	Int16& Int16::operator+=(const mdk::int16 right)
	{
		m_data += right;
		return *this;
	}

	Int16& Int16::operator-=(const mdk::int16 right)
	{
		m_data -= right;
		return *this;
	}

	Int16& Int16::operator*=(const mdk::int16 right)
	{
		m_data *= right;
		return *this;
	}

	Int16& Int16::operator/=(const mdk::int16 right)
	{
		m_data /= right;
		return *this;
	}

}
namespace SSD
{

	Int16::Int16()
		:Exist::Value<mdk::int16>()
	{
		m_type = DataType::int16;
		Exist::IOBus::CreateExistObj( this, m_type );
	}

	Int16::Int16( const char *name )
		:Exist::Value<mdk::int16>(name, true)
	{
		m_type = DataType::int16;
		Exist::IOBus::CreateExistObj( this, m_type );
	}

	Int16::Int16( const Int16 &value )
		:Exist::Value<mdk::int16>(value)
	{
		m_type = DataType::int16;
		Exist::IOBus::CreateExistObj( this, m_type );
	}

	Int16& Int16::operator=( const Int16 &right )
	{
		Copy(right);
		return *this;
	}

	Int16::~Int16()
	{
	}

	Int16& Int16::operator=(const mdk::int16 right)
	{
		m_data = right;
		return *this;
	}

	Int16& Int16::operator+=(const mdk::int16 right)
	{
		m_data += right;
		return *this;
	}

	Int16& Int16::operator-=(const mdk::int16 right)
	{
		m_data -= right;
		return *this;
	}

	Int16& Int16::operator*=(const mdk::int16 right)
	{
		m_data *= right;
		return *this;
	}

	Int16& Int16::operator/=(const mdk::int16 right)
	{
		m_data /= right;
		return *this;
	}

}
