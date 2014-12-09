// UInt32.h: interface for the UInt32 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef U_INT32_H
#define U_INT32_H

#include "./frame/Value.h"

namespace Exist
{

class UInt32 : public Value<mdk::uint32>
{
public:
	UInt32( const char *name );
	UInt32( const UInt32 &value );
	UInt32& operator=( const UInt32 &right );
	virtual ~UInt32();
	UInt32& operator=(const mdk::uint32 right);
	UInt32& operator+=(const mdk::uint32 right);
	UInt32& operator-=(const mdk::uint32 right);
	UInt32& operator*=(const mdk::uint32 right);
	UInt32& operator/=(const mdk::uint32 right);

protected:
	UInt32();
};

}

namespace SSD
{

	class UInt32 : public Exist::Value<mdk::uint32>
	{
	public:
		UInt32( const char *name );
		UInt32( const UInt32 &value );
		UInt32& operator=( const UInt32 &right );
		virtual ~UInt32();
		UInt32& operator=(const mdk::uint32 right);
		UInt32& operator+=(const mdk::uint32 right);
		UInt32& operator-=(const mdk::uint32 right);
		UInt32& operator*=(const mdk::uint32 right);
		UInt32& operator/=(const mdk::uint32 right);

	protected:
		UInt32();
	};

}

#endif // ifndef U_INT32_H
