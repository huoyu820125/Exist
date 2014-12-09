// Int16.h: interface for the Int16 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef INT16_H
#define INT16_H

#include "./frame/Value.h"

namespace Exist
{

class Int16 : public Value<mdk::int16>
{
public:
	Int16( const char *name );
	Int16( const Int16 &value );
	Int16& operator=( const Int16 &right );
	virtual ~Int16();
	Int16& operator=(const mdk::int16 right);
	Int16& operator+=(const mdk::int16 right);
	Int16& operator-=(const mdk::int16 right);
	Int16& operator*=(const mdk::int16 right);
	Int16& operator/=(const mdk::int16 right);

protected:
	Int16();
};

}

namespace SSD
{

	class Int16 : public Exist::Value<mdk::int16>
	{
	public:
		Int16( const char *name );
		Int16( const Int16 &value );
		Int16& operator=( const Int16 &right );
		virtual ~Int16();
		Int16& operator=(const mdk::int16 right);
		Int16& operator+=(const mdk::int16 right);
		Int16& operator-=(const mdk::int16 right);
		Int16& operator*=(const mdk::int16 right);
		Int16& operator/=(const mdk::int16 right);

	protected:
		Int16();
	};

}

#endif // ifndef INT16_H
