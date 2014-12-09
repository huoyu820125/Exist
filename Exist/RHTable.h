// RHTable.h: interface for the RHTable class.
//
//远程哈稀表
//思路源于瘦服务端模式，hash算法由远端程序计算好，直接传递int型hash值到hash服务器，
//实现hash值的分布式计算，访问效率可达到普通哈希表20～30倍
//
//////////////////////////////////////////////////////////////////////

#ifndef HASHTABLE_H
#define HASHTABLE_H

#ifndef NULL
#define NULL 0
#endif

#ifdef WIN32
#include <windows.h>
#else
#endif
#define MAX_HASH_64 0x7fffffffffffffff
#define MAX_HASH_32 0x7fffffff
typedef	void (*pHashFunction)(unsigned char *hashKey, unsigned int &hashSize, unsigned char *key, unsigned int size );

#include "../Micro-Development-Kit/include/mdk/FixLengthInt.h"

class RHTable  
{
	struct ELEMENT;
	struct HASH_TABLE;
public:
//操作结果
typedef struct OP_R
{
	ELEMENT *pInsert;
	bool bSuccess;
}OP_R;

//迭代器
typedef struct Iterator
{
	Iterator& operator++();
	Iterator& operator++(int);
	Iterator& operator--();
	Iterator& operator--(int);
	bool operator==(Iterator it);
	bool operator!=(Iterator it);
	ELEMENT *pElement;
private:
	friend class RHTable;
	unsigned int idx;
	HASH_TABLE *pHashTable;
	HASH_TABLE *pHeadHashTable;
}Iterator;

private:
//元素
typedef struct ELEMENT
{
	unsigned int hashValue;
	unsigned char *key;
	unsigned short keySize;
	void *value;
private:
	friend class RHTable;
	friend struct Iterator;
	bool isDel;
	ELEMENT *next;//同一个桶内下一个元素
}ELEMENT;

//桶，保存发生hash碰撞的元素
typedef struct BUCKET
{
	ELEMENT *head;//桶中元素链表头
}BUCKET;

//哈稀表
typedef struct HASH_TABLE
{
	BUCKET *buckets;//hash数组
	unsigned long size;//hash数组大小
	unsigned long sizemask;//掩码size的全1表示
	HASH_TABLE *pre;//前一个表
	HASH_TABLE *next;//下一个表（旧hash表）
	unsigned long count;//实际元素数量
}HASH_TABLE;

public:
	RHTable();
	RHTable( unsigned long size );
	virtual ~RHTable();
	void SetRemoteMode( bool bRemote );
	void SetHashFunction( pHashFunction hf );

	//////////////////////////////////////////////////////////////////////////
	//client方法,各种类型值作key计算hash
	unsigned int RemoteHash(unsigned char *key, unsigned int size);
	unsigned int RemoteHash( mdk::uint8 key );
	unsigned int RemoteHash( mdk::uint16 key );
	unsigned int RemoteHash( mdk::uint32 key );
	unsigned int RemoteHash( mdk::uint64 key );
	
	//////////////////////////////////////////////////////////////////////////
	//server方法
	OP_R* Insert(unsigned char *key, unsigned int size, void *value, unsigned int hashValue = 0);
	void* Find(unsigned char *key, unsigned int size, unsigned int hashValue = 0 );
	void* Update(unsigned char *key, unsigned int size, void *value, unsigned int hashValue = 0);
	void Delete(unsigned char *key, unsigned int size, unsigned int hashValue = 0);
	unsigned long Count();
	bool IsEmpty();
	void Clear();
	Iterator Begin();
	Iterator End();

private:
	unsigned long NextPower(unsigned long size);//比size大的最小的2的n次幂数
	unsigned int DJBHash(const unsigned char *buf, int len);//C33算法hash转换函数
	bool KeyCmp( unsigned char *key1, int size1, unsigned char *key2, int size2 );//相同返回true
	bool Expand(unsigned long size);
	ELEMENT* Find(unsigned char *key, unsigned int size, unsigned int hashValue, bool bInsert );
	void ReleaseOldHashTable();//旧hash表如果为null则释放
	void ReleaseHashTable();
	//哈稀算法函数指针
	void (*HashFunction)(unsigned char *hashKey, unsigned int &hashSize, 
		unsigned char *key, unsigned int size );		
	
private:
	HASH_TABLE *m_pHashTable;
	bool m_onBit64;
	unsigned long m_maxHash;
	Iterator m_it;
	bool m_bRemote;
};

#endif // !defined(HASHTABLE_H)
