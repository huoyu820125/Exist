// Float.cpp: implementation of the Float class.
//
//////////////////////////////////////////////////////////////////////

#include "../../include/Exist/Float.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace Exist
{

Float::Float()
:Value<float>()
{
	m_type = DataType::sFloat;
	IOBus::CreateExistObj( this, m_type );
}

Float::Float( const char *name )
:Value<float>(name, false)
{
	m_type = DataType::sFloat;
	IOBus::CreateExistObj( this, m_type );
}

Float::Float( const Float &value )
:Value<float>(value)
{
	m_type = DataType::sFloat;
	IOBus::CreateExistObj( this, m_type );
}

Float& Float::operator=( const Float &right )
{
	Copy(right);
	return *this;
}

Float::~Float()
{
}

Float& Float::operator=(const float right)
{
	m_data = right;
	return *this;
}
	
Float& Float::operator+=(const float right)
{
	m_data += right;
	return *this;
}

Float& Float::operator-=(const float right)
{
	m_data -= right;
	return *this;
}

Float& Float::operator*=(const float right)
{
	m_data *= right;
	return *this;
}

Float& Float::operator/=(const float right)
{
	m_data /= right;
	return *this;
}

}
namespace SSD
{

	Float::Float()
		:Exist::Value<float>()
	{
		m_type = DataType::sFloat;
		Exist::IOBus::CreateExistObj( this, m_type );
	}

	Float::Float( const char *name )
		:Exist::Value<float>(name, true)
	{
		m_type = DataType::sFloat;
		Exist::IOBus::CreateExistObj( this, m_type );
	}

	Float::Float( const Float &value )
		:Exist::Value<float>(value)
	{
		m_type = DataType::sFloat;
		Exist::IOBus::CreateExistObj( this, m_type );
	}

	Float& Float::operator=( const Float &right )
	{
		Copy(right);
		return *this;
	}

	Float::~Float()
	{
	}

	Float& Float::operator=(const float right)
	{
		m_data = right;
		return *this;
	}

	Float& Float::operator+=(const float right)
	{
		m_data += right;
		return *this;
	}

	Float& Float::operator-=(const float right)
	{
		m_data -= right;
		return *this;
	}

	Float& Float::operator*=(const float right)
	{
		m_data *= right;
		return *this;
	}

	Float& Float::operator/=(const float right)
	{
		m_data /= right;
		return *this;
	}

}
