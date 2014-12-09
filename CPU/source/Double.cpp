// Double.cpp: implementation of the Double class.
//
//////////////////////////////////////////////////////////////////////

#include "../../include/Exist/Double.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace Exist
{

Double::Double()
:Value<double>()
{
	m_type = DataType::sDouble;
	IOBus::CreateExistObj( this, m_type );
}

Double::Double( const char *name )
:Value<double>(name, false)
{
	m_type = DataType::sDouble;
	IOBus::CreateExistObj( this, m_type );
}

Double::Double( const Double &value )
:Value<double>(value)
{
	m_type = DataType::sDouble;
	IOBus::CreateExistObj( this, m_type );
}

Double& Double::operator=( const Double &right )
{
	Copy(right);
	return *this;
}

Double::~Double()
{
}

Double& Double::operator=(const double right)
{
	m_data = right;
	return *this;
}
	
Double& Double::operator+=(const double right)
{
	m_data += right;
	return *this;
}

Double& Double::operator-=(const double right)
{
	m_data -= right;
	return *this;
}

Double& Double::operator*=(const double right)
{
	m_data *= right;
	return *this;
}

Double& Double::operator/=(const double right)
{
	m_data /= right;
	return *this;
}

}

namespace SSD
{

	Double::Double()
		:Exist::Value<double>()
	{
		m_type = DataType::sDouble;
		Exist::IOBus::CreateExistObj( this, m_type );
	}

	Double::Double( const char *name )
		:Exist::Value<double>(name, true)
	{
		m_type = DataType::sDouble;
		Exist::IOBus::CreateExistObj( this, m_type );
	}

	Double::Double( const Double &value )
		:Exist::Value<double>(value)
	{
		m_type = DataType::sDouble;
		Exist::IOBus::CreateExistObj( this, m_type );
	}

	Double& Double::operator=( const Double &right )
	{
		Copy(right);
		return *this;
	}

	Double::~Double()
	{
	}

	Double& Double::operator=(const double right)
	{
		m_data = right;
		return *this;
	}

	Double& Double::operator+=(const double right)
	{
		m_data += right;
		return *this;
	}

	Double& Double::operator-=(const double right)
	{
		m_data -= right;
		return *this;
	}

	Double& Double::operator*=(const double right)
	{
		m_data *= right;
		return *this;
	}

	Double& Double::operator/=(const double right)
	{
		m_data /= right;
		return *this;
	}

}
