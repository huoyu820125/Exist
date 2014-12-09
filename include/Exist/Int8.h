// Int8.h: interface for the Int8 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef INT8_H
#define INT8_H

#include "./frame/Value.h"

namespace Exist
{

class Int8 : public Value<mdk::int8>
{
public:
	Int8( const char *name );
	Int8( const Int8 &value );
	Int8& operator=( const Int8 &right );
	virtual ~Int8();
	Int8& operator=(const mdk::int8 right);
	Int8& operator+=(const mdk::int8 right);
	Int8& operator-=(const mdk::int8 right);
	Int8& operator*=(const mdk::int8 right);
	Int8& operator/=(const mdk::int8 right);

protected:
	Int8();
};

}

namespace SSD
{

	class Int8 : public Exist::Value<mdk::int8>
	{
	public:
		Int8( const char *name );
		Int8( const Int8 &value );
		Int8& operator=( const Int8 &right );
		virtual ~Int8();
		Int8& operator=(const mdk::int8 right);
		Int8& operator+=(const mdk::int8 right);
		Int8& operator-=(const mdk::int8 right);
		Int8& operator*=(const mdk::int8 right);
		Int8& operator/=(const mdk::int8 right);

	protected:
		Int8();
	};

}

#endif // ifndef INT8_H
