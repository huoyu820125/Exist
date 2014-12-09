// Vector.h: interface for the Vector class.
//
//////////////////////////////////////////////////////////////////////

#ifndef VECTOR_H
#define VECTOR_H

#include "./frame/Container.h"

namespace Exist
{
	
template<class Key, class T>
class Vector : public Container<T>
{
public:
	Vector( const char *name )
		:Container<T>(name, false)
	{
		this->m_type = DataType::vector;
		IOBus::CreateExistObj( this, this->m_type );
	}
	
	Vector( const Vector &value )
		:Container<T>(value)
	{
		this->m_type = DataType::vector;
		IOBus::CreateExistObj( this, this->m_type );
	}
	
	Vector& operator=( const Vector &right )
	{
		Copy(right);
		return *this;
	}

	virtual ~Vector()
	{
	}

protected:
	Vector()
		:Container<T>()
	{
		this->m_type = DataType::vector;
		IOBus::CreateExistObj( this, this->m_type );
	}

private:
};

}

#endif // ifndef VECTOR_H
