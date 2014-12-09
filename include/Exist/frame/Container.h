// Container.h: interface for the CContainer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef CONTAINER_H
#define CONTAINER_H

#include "KVData.h"
#include "Element.h"
#include <typeinfo>
namespace Exist
{
/*
	容器基础类
		非用户接口类，对用户透明
		Map, Vector, String等，所有容器接口都从此类派生
 */
template<class T>
class Container : public KVData<T, Element<T> >
{
public:
	Container( const Container &value )
		:KVData<T, Element<T> >(value)
	{
		InitElementType();
	}
	
	Container& operator=( const Container &right )
	{
		this->Copy(right);
		return *this;
	}

	virtual ~Container()
	{
	}

	int Size()
	{
		int size = 0;
		int sizeLen = 0;
		if ( !this->ReadData( this->m_hash, this->m_key, this->m_keySize, &size, sizeLen) ) return 0;
		return size;
	}

	Element<T> operator[]( const int index )
	{
		return Get( index );
	}

	Element<T> Get( const int index )
	{
		Element<T> e(this, 0, &index, sizeof(int));
		return e;
	}
	
	void Delete( const int index )
	{
	}

protected:
	Container()
		:KVData<T, Element<T> >()
	{
		InitElementType();
	}
	
	Container( const char *name, bool protracted )
		:KVData<T, Element<T> >(name, protracted)
	{
		InitElementType();
	}
	
	void InitElementType()
	{
		T e = (*(T*)IOBus::s_defaultObj);
		this->m_elementType = IOBus::GetDataType(&e);
		if ( DataType::uninit == this->m_elementType )
		{
			mdk::int8 i8;
			mdk::uint8 u8;
			mdk::int16 i16;
			mdk::uint16 u16;
			mdk::int32 i32;
			mdk::uint32 u32;
			mdk::int64 i64;
			mdk::uint64 u64;
			float f;
			double dbl;
			if ( typeid(&i8) == typeid( &e ) ) this->m_elementType = DataType::int8;
			else if ( typeid(&u8) == typeid( &e ) ) this->m_elementType = DataType::uInt8;
			else if ( typeid(&i16) == typeid( &e ) ) this->m_elementType = DataType::int16;
			else if ( typeid(&u16) == typeid( &e ) ) this->m_elementType = DataType::uInt16;
			else if ( typeid(&i32) == typeid( &e ) ) this->m_elementType = DataType::int32;
			else if ( typeid(&u32) == typeid( &e ) ) this->m_elementType = DataType::uInt32;
			else if ( typeid(&i64) == typeid( &e ) ) this->m_elementType = DataType::int64;
			else if ( typeid(&u64) == typeid( &e ) ) this->m_elementType = DataType::uInt64;
			else if ( typeid(&f) == typeid( &e ) ) this->m_elementType = DataType::sFloat;
			else if ( typeid(&dbl) == typeid( &e ) ) this->m_elementType = DataType::sDouble;
			else;
		}
	}
	
private:

};

}

#endif //ifndef CONTAINER_H
