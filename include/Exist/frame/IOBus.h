// IOBus.h: interface for the IOBus class.
//
//////////////////////////////////////////////////////////////////////

#ifndef IOBUS_H
#define IOBUS_H

#include <cstring>
#include <map>
#include "mdk/Lock.h"
#include "mdk/mapi.h"
#include "mdk/FixLengthInt.h"
#include "ExistType.h"

namespace Exist
{

#define MAX_KEY_SIZE 64

/*
 	将外存条插入主板
	必须在第一访问外存前，调用此方法，将外存条插好
	成功返回true，失败返回false，错误原因请查看Exist.log日志
 */
bool PlugIn();
class ConnectPool;
/*
	IO总线类
		Exist的数据访问接口
		任何数据类都只是一个访问接口，所以IOBus是所有接口类的基类
 */
class IOBus
{
	//////////////////////////////////////////////////////////////////////////
	//运行时类型操作
public:
	static char s_defaultObj[1024];//默认对象，用于调用private protected类型的默认构造函数
	static std::map<void*, int> s_memoryMap;
	static mdk::Mutex s_lockMemoryMap;
	static void CreateExistObj( void *pObj, DataType::DataType type );
	static void ReleaseExistObj( void *pObj );
	static DataType::DataType GetDataType( void *pData );
	//////////////////////////////////////////////////////////////////////////
	//链接池
	static ConnectPool s_connectPool;

	//工具方法
	static int Hash( char *key, int keySize )//计算hash
	{
		return 0;
	}
	
	//////////////////////////////////////////////////////////////////////////
	//本地接口
public:
	void Init( const char *key, int keySize, bool protracted );
	virtual ~IOBus();
	IOBus& operator=( const IOBus &right );

	bool IsProtracted();
	char* Key();
	int KeySize();
	DataType::DataType Type();
	DataType::DataType ElementType();
	void DeleteData();
	
protected:
	IOBus();
	IOBus( const char *name, bool protracted );
	IOBus( const IOBus &container );
	void Copy( const IOBus &right );
	//元素在容器中的Key(局部Key)转换成元素在Exist上的真实Key(全局Key)
	void ElementRealKey( const void* localKey, const int &localKeySize, char *globalkey, int &globalkeySize );
	
	
protected:
	bool			m_protracted;			//数据是持久化，非0是，0否（预留字段，暂不实现持久化功能）
	/*
		因为运行时类型判断的方法是，将一个对象强转成IOBus之后，查看m_type指向的数据。
		但是，不排除某个指针指向的地址强转成IOBus后,m_type指向的数据正好是DataType定义值的巧合性，
		所以需要检查CalCheckData()计算后，与此成员是否相同
	*/
	DataType::DataType	m_type;				//自身类型，给GetElementType()用
	char				m_size;				//数据大小，data有效，容器无效
	DataType::DataType	m_elementType;		//元素类型，通过GetElementType()得到
	DataType::DataType	m_elementKeyType;	//元素Key类型
	
private:
	int				m_keySize;
	char			m_key[MAX_KEY_SIZE+1];	//本线路对应数据的key，value在Exist上
	unsigned int	m_hash;					//key通过hash计算后得到的hashid

	//////////////////////////////////////////////////////////////////////////
	//Exist::IO接口
public:
	bool DeleteData( unsigned int hash, char *key, int keySize );
	bool QueryData(int initSize);//请求数据
	bool SeekMsg();//跳过1条报文
	bool ReadData( void *pData, int &size );//读取数据，成功返回true,pData长度不足(size回返回需要的长度)或者连接断开返回false
	//updateType 更新类型= += -= *= /=
	bool WriteData( void *pData, int size, UpdateType::UpdateType updateType );

	bool ReadData( int index, void *pData, int &size );
	bool WriteData( int index, void *pData, int size );
};

}

#endif // ifndef IOBUS_H
