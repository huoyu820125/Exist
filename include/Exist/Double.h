// Double.h: interface for the Double class.
//
//////////////////////////////////////////////////////////////////////

#ifndef DOUBLE_H
#define DOUBLE_H

#include "./frame/Value.h"

namespace Exist
{

class Double : public Value<double>
{
public:
	Double( const char *name );
	Double( const Double &value );
	Double& operator=( const Double &right );
	virtual ~Double();
	Double& operator=(const double right);
	Double& operator+=(const double right);
	Double& operator-=(const double right);
	Double& operator*=(const double right);
	Double& operator/=(const double right);

protected:
	Double();
};

}

namespace SSD
{

	class Double : public Exist::Value<double>
	{
	public:
		Double( const char *name );
		Double( const Double &value );
		Double& operator=( const Double &right );
		virtual ~Double();
		Double& operator=(const double right);
		Double& operator+=(const double right);
		Double& operator-=(const double right);
		Double& operator*=(const double right);
		Double& operator/=(const double right);

	protected:
		Double();
	};

}

#endif // ifndef DOUBLE_H
