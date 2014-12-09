// Float.h: interface for the Float class.
//
//////////////////////////////////////////////////////////////////////

#ifndef FLOAT_H
#define FLOAT_H

#include "./frame/Value.h"

namespace Exist
{

class Float : public Value<float>
{
public:
	Float( const char *name );
	Float( const Float &value );
	Float& operator=( const Float &right );
	virtual ~Float();
	Float& operator=(const float right);
	Float& operator+=(const float right);
	Float& operator-=(const float right);
	Float& operator*=(const float right);
	Float& operator/=(const float right);

protected:
	Float();
};

}

namespace SSD
{

	class Float : public Exist::Value<float>
	{
	public:
		Float( const char *name );
		Float( const Float &value );
		Float& operator=( const Float &right );
		virtual ~Float();
		Float& operator=(const float right);
		Float& operator+=(const float right);
		Float& operator-=(const float right);
		Float& operator*=(const float right);
		Float& operator/=(const float right);

	protected:
		Float();
	};

}

#endif // ifndef FLOAT_H