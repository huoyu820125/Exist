// Int32.h: interface for the Int32 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef INT32_H
#define INT32_H

#include "./frame/Value.h"

namespace Exist
{

class Int32 : public Value<mdk::int32>
{
public:
	Int32( const char *name );
	Int32( const Int32 &value );
	Int32& operator=( const Int32 &right );
	virtual ~Int32();
	Int32& operator=(const mdk::int32 right);
	Int32& operator+=(const mdk::int32 right);
	Int32& operator-=(const mdk::int32 right);
	Int32& operator*=(const mdk::int32 right);
	Int32& operator/=(const mdk::int32 right);

protected:
	Int32();
};

}

namespace SSD
{

	class Int32 : public Exist::Value<mdk::int32>
	{
	public:
		Int32( const char *name );
		Int32( const Int32 &value );
		Int32& operator=( const Int32 &right );
		virtual ~Int32();
		Int32& operator=(const mdk::int32 right);
		Int32& operator+=(const mdk::int32 right);
		Int32& operator-=(const mdk::int32 right);
		Int32& operator*=(const mdk::int32 right);
		Int32& operator/=(const mdk::int32 right);

	protected:
		Int32();
	};

}

#endif // ifndef INT32_H
