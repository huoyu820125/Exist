// UInt8.h: interface for the UInt8 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef U_INT8_H
#define U_INT8_H

#include "./frame/Value.h"

namespace Exist
{

class UInt8 : public Value<mdk::uint8>
{
public:
	UInt8( const char *name );
	UInt8( const UInt8 &value );
	UInt8& operator=( const UInt8 &right );
	virtual ~UInt8();
	UInt8& operator=(const mdk::uint8 right);
	UInt8& operator+=(const mdk::uint8 right);
	UInt8& operator-=(const mdk::uint8 right);
	UInt8& operator*=(const mdk::uint8 right);
	UInt8& operator/=(const mdk::uint8 right);

protected:
	UInt8();
};

}

namespace SSD
{

	class UInt8 : public Exist::Value<mdk::uint8>
	{
	public:
		UInt8( const char *name );
		UInt8( const UInt8 &value );
		UInt8& operator=( const UInt8 &right );
		virtual ~UInt8();
		UInt8& operator=(const mdk::uint8 right);
		UInt8& operator+=(const mdk::uint8 right);
		UInt8& operator-=(const mdk::uint8 right);
		UInt8& operator*=(const mdk::uint8 right);
		UInt8& operator/=(const mdk::uint8 right);

	protected:
		UInt8();
	};

}

#endif // ifndef U_INT8_H
