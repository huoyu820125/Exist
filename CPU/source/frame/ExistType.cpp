#include "../../../include/Exist/frame/ExistType.h"
/*
	VC++6.0不支持在DataType名称空间作用域内使用DataType::
	为兼容VC++6.0、VS、linux，同时避免名称冲突，只能定义全局static函数，然后在namespace中调用
*/
static bool IsValue( const char dt )
{
	if ( DataType::sFloat == dt
		|| DataType::sDouble == dt
		|| DataType::int8 == dt
		|| DataType::uInt8 == dt
		|| DataType::int16 == dt
		|| DataType::uInt16 == dt
		|| DataType::int32 == dt
		|| DataType::uInt32 == dt
		|| DataType::int64 == dt
		|| DataType::uInt64 == dt
		) 
	{
		return true;
	}
	
	return false;
}

static bool IsContainer( const char dt )
{
	if ( DataType::vector == dt
		|| DataType::map == dt ) 
	{
		return true;
	}
	
	return false;
}


namespace DataType
{
	bool IsValue( const char dt )
	{
		return ::IsValue(dt);
	}

	bool IsContainer( const char dt )
	{
		return ::IsContainer(dt);
	}
}
