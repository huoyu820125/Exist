// UInt64.h: interface for the UInt64 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef U_INT64_H
#define U_INT64_H

#include "./frame/Value.h"

namespace Exist
{

class UInt64 : public Value<mdk::uint64>
{
public:
	UInt64( const char *name );
	UInt64( const UInt64 &value );
	UInt64& operator=( const UInt64 &right );
	virtual ~UInt64();
	UInt64& operator=(const mdk::uint64 right);
	UInt64& operator+=(const mdk::uint64 right);
	UInt64& operator-=(const mdk::uint64 right);
	UInt64& operator*=(const mdk::uint64 right);
	UInt64& operator/=(const mdk::uint64 right);

protected:
	UInt64();
};

}

namespace SSD
{

	class UInt64 : public Exist::Value<mdk::uint64>
	{
	public:
		UInt64( const char *name );
		UInt64( const UInt64 &value );
		UInt64& operator=( const UInt64 &right );
		virtual ~UInt64();
		UInt64& operator=(const mdk::uint64 right);
		UInt64& operator+=(const mdk::uint64 right);
		UInt64& operator-=(const mdk::uint64 right);
		UInt64& operator*=(const mdk::uint64 right);
		UInt64& operator/=(const mdk::uint64 right);

	protected:
		UInt64();
	};

}

#endif // ifndef U_INT64_H
