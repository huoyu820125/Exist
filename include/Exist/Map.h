// Map.h: interface for the Map class.
//
//////////////////////////////////////////////////////////////////////

#ifndef MAP_H
#define MAP_H

#include "./frame/Container.h"
#include "./frame/Key.h"
namespace Exist
{
/*
 *	当key是int时
 *		[]运算符总是认为传入的int值是下标，会按照下标去定位元素
 *		如果希望用key定位，可以这样写：[(Exist::Key<int>)你传入的int值]
 *		告诉[]运算符你传入的int值是一个key，而不是下标
 *		如果key不是int，那么就不需要(Exist::Key<类型>)了
 */
template<class K, class T>
class Map :public Container<T>
{
	
public:
	Map( const char *name )
		:Container<T>(name, false)
	{
		this->m_type = DataType::map;
		IOBus::CreateExistObj( this, this->m_type );
		InitKeyType();
	}
	
	Map( const Map &value )
		:Container<T>(value)
	{
		this->m_type = DataType::map;
		IOBus::CreateExistObj( this, this->m_type );
		InitKeyType();
	}

	Map& operator=( const Map &right )
	{
		this->Copy(right);
		return *this;
	}

	virtual ~Map()
	{
	}

	bool Find( const K &key  )
	{
		char name[MAX_KEY_SIZE+1];
		int size = 0;
		this->ElementRealKey( &key, sizeof(K), name, size );

		return true;
	}
	
	Element<T> operator[]( const Exist::Key<K> &key )
	{
		K k = (Exist::Key<K>)key;
		char name[MAX_KEY_SIZE+1];
		int size = 0;
		this->ElementRealKey( &k, sizeof(K), name, size );
		Element<T> e(this, 1, name, size);
		return e;
	}
	
	void Delete( const Exist::Key<K> &key )
	{
		K k = (Exist::Key<K>)key;
		char name[MAX_KEY_SIZE+1];
		int size = 0;
		this->ElementRealKey( &k, sizeof(K), name, size );
		return;
	}
	
	Element<T> operator[]( const int &index )
	{
		return this->Get( index );
	}
	
	void Delete( const int index )
	{
		Container<T>::Delete(index);
	}

	T& operator+=(const T &right)
	{
		this->m_data += right;
		return *this;
	}

	
protected:
	Map()
		:Container<T>()
	{
		this->m_type = DataType::map;
		IOBus::CreateExistObj( this, this->m_type );
		InitKeyType();
	}
	
	void InitKeyType()
	{
		K k = (*(K*)IOBus::s_defaultObj);
		this->m_elementKeyType = IOBus::GetDataType(&k);
	}

};

}

#endif // ifndef MAP_H
