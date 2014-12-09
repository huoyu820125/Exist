// Int64.h: interface for the Int64 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef INT64_H
#define INT64_H

#include "./frame/Value.h"

namespace Exist
{

class Int64 : public Value<mdk::int64>
{
public:
	Int64( const char *name );
	Int64( const Int64 &value );
	Int64& operator=( const Int64 &right );
	virtual ~Int64();
	Int64& operator=(const mdk::int64 right);
	Int64& operator+=(const mdk::int64 right);
	Int64& operator-=(const mdk::int64 right);
	Int64& operator*=(const mdk::int64 right);
	Int64& operator/=(const mdk::int64 right);

protected:
	Int64();
};

}

namespace SSD
{

	class Int64 : public Exist::Value<mdk::int64>
	{
	public:
		Int64( const char *name );
		Int64( const Int64 &value );
		Int64& operator=( const Int64 &right );
		virtual ~Int64();
		Int64& operator=(const mdk::int64 right);
		Int64& operator+=(const mdk::int64 right);
		Int64& operator-=(const mdk::int64 right);
		Int64& operator*=(const mdk::int64 right);
		Int64& operator/=(const mdk::int64 right);

	protected:
		Int64();
	};

}

#endif // ifndef INT64_H
