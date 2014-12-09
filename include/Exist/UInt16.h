// UInt16.h: interface for the UInt16 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef U_INT16_H
#define U_INT16_H

#include "./frame/Value.h"

namespace Exist
{

class UInt16 : public Value<mdk::uint16>
{
public:
	UInt16( const char *name );
	UInt16( const UInt16 &value );
	UInt16& operator=( const UInt16 &right );
	virtual ~UInt16();
	UInt16& operator=(const mdk::uint16 right);
	UInt16& operator+=(const mdk::uint16 right);
	UInt16& operator-=(const mdk::uint16 right);
	UInt16& operator*=(const mdk::uint16 right);
	UInt16& operator/=(const mdk::uint16 right);

protected:
	UInt16();
};

}

namespace SSD
{

	class UInt16 : public Exist::Value<mdk::uint16>
	{
	public:
		UInt16( const char *name );
		UInt16( const UInt16 &value );
		UInt16& operator=( const UInt16 &right );
		virtual ~UInt16();
		UInt16& operator=(const mdk::uint16 right);
		UInt16& operator+=(const mdk::uint16 right);
		UInt16& operator-=(const mdk::uint16 right);
		UInt16& operator*=(const mdk::uint16 right);
		UInt16& operator/=(const mdk::uint16 right);

	protected:
		UInt16();
	};

}
#endif // ifndef U_INT16_H
