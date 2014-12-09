// RHTable.cpp: implementation of the RHTable class.
//
//////////////////////////////////////////////////////////////////////

#include "RHTable.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "../Micro-Development-Kit/include/mdk/mapi.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RHTable::RHTable()
{
	m_onBit64 = false;
	int *bit = NULL;
	if ( 8 == sizeof(bit) ) m_onBit64 = true;

#if (m_onBit64)
	if ( m_onBit64 ) m_maxHash = MAX_HASH_64;
#else
	m_maxHash = MAX_HASH_32;
#endif
	m_bRemote = false;
	HashFunction = NULL;
	m_pHashTable = NULL;
	Expand(0);
}

RHTable::RHTable(unsigned long size)
{
	m_onBit64 = false;
	int *bit = NULL;
	if ( 8 == sizeof(bit) ) m_onBit64 = true;

#if (m_onBit64)
	if ( m_onBit64 ) m_maxHash = MAX_HASH_64;
#else
	m_maxHash = MAX_HASH_32;
#endif
	m_bRemote = false;
	HashFunction = NULL;
	m_pHashTable = NULL;
	Expand(size);
}

RHTable::~RHTable()
{
	ReleaseHashTable();
}

void RHTable::SetHashFunction( pHashFunction hf )
{
	HashFunction = hf;
}

unsigned long RHTable::NextPower(unsigned long size)
{
    unsigned long i = 8;
	if (size >= m_maxHash) return m_maxHash;
	while(1) 
	{
        if (i >= size) return i;
        i *= 2;
    }
}

unsigned int RHTable::DJBHash(const unsigned char *buf, int len) 
{
    unsigned int hash = 5381;

    while (len--)
	{
        hash = ((hash << 5) + hash) + (*buf++); /* hash * 33 + c */
	}

    return hash;
}

bool RHTable::KeyCmp( unsigned char *key1, int size1, unsigned char *key2, int size2 )
{
	if ( size1 != size2 ) return false;
	int i = 0;
	for ( i = 0; i < size1; i++ )
	{
		if ( key1[i] != key2[i] ) return false;
	}
	
	return true;
}

bool RHTable::Expand(unsigned long size)
{
	HASH_TABLE *pHashTable = new HASH_TABLE;
	if ( NULL == pHashTable ) return false;
	memset( pHashTable, 0, sizeof(HASH_TABLE) );

	size = NextPower(size);
	if ( NULL != m_pHashTable && m_pHashTable->size >= size ) 
	{
		if ( m_pHashTable->size * 16 > m_maxHash ) size = m_maxHash;
		else size = m_pHashTable->size * 16;
	}
	
	pHashTable->size = size;
	pHashTable->sizemask = size - 1;
	pHashTable->buckets = (BUCKET*)malloc(sizeof(BUCKET)*size);
	if ( NULL == pHashTable->buckets ) 
	{
		delete pHashTable;
		return false;
	}
	memset(pHashTable->buckets, 0, sizeof(BUCKET)*size);
	pHashTable->next = m_pHashTable;
	pHashTable->pre = NULL;
	if ( NULL != m_pHashTable ) 
	{
		m_pHashTable->pre = pHashTable;
	}
	m_pHashTable = pHashTable;
	
	return true;
}

void RHTable::SetRemoteMode( bool bRemote )
{
	m_bRemote = bRemote;
}

unsigned int RHTable::RemoteHash( mdk::uint8 key )
{
	return RemoteHash( (unsigned char*)&key, sizeof(mdk::uint8) );
}

unsigned int RHTable::RemoteHash( mdk::uint16 key )
{
	return RemoteHash( (unsigned char*)&key, sizeof(mdk::uint16) );
}

unsigned int RHTable::RemoteHash( mdk::uint32 key )
{
	return RemoteHash( (unsigned char*)&key, sizeof(mdk::uint32) );
}

unsigned int RHTable::RemoteHash( mdk::uint64 key )
{
	return RemoteHash( (unsigned char*)&key, sizeof(mdk::uint64) );
}

unsigned int RHTable::RemoteHash( unsigned char *key, unsigned int size )
{
	mdk::mdk_assert(NULL != HashFunction);
	unsigned char hashKey[256];
	unsigned int hashSize;
	HashFunction( hashKey, hashSize, key, size );
	return DJBHash( hashKey, hashSize );
}

RHTable::ELEMENT* RHTable::Find(unsigned char* key, unsigned int size, unsigned int hashValue,  bool bInsert )
{
	BUCKET *pSaveBucket = NULL;
	ELEMENT *pFrontE = NULL;
	ELEMENT *pFindE = NULL;
	unsigned char hashKey[256];
	unsigned int hashSize;
	if ( !m_bRemote ) HashFunction( hashKey, hashSize, key, size );
	unsigned int idx = 0;
	HASH_TABLE *pHashTable = m_pHashTable;
	do
	{
		idx = hashValue&pHashTable->sizemask;
		if (NULL == pSaveBucket) pSaveBucket = &pHashTable->buckets[idx];
		pFindE = pHashTable->buckets[idx].head;
		while ( NULL != pFindE )
		{
			if ( pFindE->isDel 
				|| hashValue != pFindE->hashValue 
				|| !KeyCmp( pFindE->key, pFindE->keySize, key, size ) ) 
			{
				pFrontE = pFindE;
				pFindE = pFindE->next;
				continue;
			}
			break;
		}
		if ( NULL != pFindE ) break;//找到对象
		pHashTable = pHashTable->next;//旧表中查询
	}
	while (NULL != pHashTable);

	if ( NULL == pFindE  ) //没有找到
	{
		if ( !bInsert ) return NULL;
		ELEMENT *pNew = pSaveBucket->head;
		while ( NULL != pNew )
		{
			if ( pNew->isDel ) break;
			pNew = pNew->next;
		}
		bool bNew = false;
		if ( NULL == pNew ) 
		{
			pNew = new ELEMENT;
			if ( NULL == pNew ) return NULL;
			pNew->next = pSaveBucket->head;
			bNew = true;
		}
		pNew->value = NULL;
		pNew->key = new unsigned char[size];
		if ( NULL == pNew->key )
		{
			delete pNew;
			return NULL;
		}
		pNew->hashValue = hashValue;
		memcpy( pNew->key, key, size );
		pNew->keySize = size;
		pNew->isDel = false;
		if ( bNew ) pSaveBucket->head = pNew;
		return pNew;
	}

	if ( pSaveBucket == &pHashTable->buckets[idx] ) return pFindE;
	//数据迁移
	if ( pHashTable->buckets[idx].head == pFindE ) pHashTable->buckets[idx].head = pFindE->next;
	else pFrontE->next = pFindE->next;
	pFindE->next = pSaveBucket->head;
	pSaveBucket->head = pFindE;
	pHashTable->count--;
	m_pHashTable->count++;
	if ( 0 == pHashTable->count ) ReleaseOldHashTable();
	return pFindE;
}

void RHTable::ReleaseOldHashTable()
{
	HASH_TABLE *pEmpty = NULL;
	HASH_TABLE *pHashTable = m_pHashTable;
	while ( NULL != pHashTable->next )
	{
		if ( 0 < pHashTable->next->count ) 
		{
			pHashTable = pHashTable->next;
			continue;
		}
		pEmpty = pHashTable->next;
		if ( NULL != pHashTable->next->next ) 
		{
			pHashTable->next->next->pre = pHashTable->next->pre;
		}
		pHashTable->next = pHashTable->next->next;
		/*
			不用遍历buckets中head
			因为使count减小到 = 0的操作只有1种,就是3参数的Find(key,size,insert)
			而Find就会将旧hash表中元素移到新表
			所以count = 0就一定是所有head指针=NULL
		 */
		delete pEmpty->buckets;
		delete pEmpty;
	}
}

RHTable::OP_R* RHTable::Insert(unsigned char *key, unsigned int size, void *value, unsigned int hashValue)
{
	mdk::mdk_assert(m_bRemote||NULL != HashFunction);
	static OP_R res;
	res.bSuccess = false;
	res.pInsert = Find( key, size, hashValue, true );
	if ( NULL == res.pInsert ) return &res;
	if ( NULL != res.pInsert->value ) return &res;
	res.pInsert->value = value;
	res.bSuccess = true;
	m_pHashTable->count++;
	if ( 5 <= m_pHashTable->count / m_pHashTable->size )Expand(0);//平均碰撞最大容忍为5

	return &res;
}

void* RHTable::Find(unsigned char *key, unsigned int size, unsigned int hashValue )
{
	mdk::mdk_assert(m_bRemote||NULL != HashFunction);
	ELEMENT *pFindE = Find( key, size, hashValue, false );
	if ( NULL == pFindE ) return NULL;
	return pFindE->value;
}

void* RHTable::Update(unsigned char *key, unsigned int size, void *value, unsigned int hashValue)
{
	mdk::mdk_assert(m_bRemote||NULL != HashFunction);
	ELEMENT *pFindE = Find( key, size, hashValue, false );
	void *old = NULL;
	if ( NULL == pFindE ) return old;
	old = pFindE->value;
	pFindE->value = value;
	return old;
}

void RHTable::Delete(unsigned char *key, unsigned int size, unsigned int hashValue)
{
	mdk::mdk_assert(m_bRemote||NULL != HashFunction);
	ELEMENT *pFindE = Find( key, size, hashValue, false );
	if ( NULL == pFindE ) return;
	pFindE->isDel = true;
	pFindE->value = NULL;
	delete pFindE->key;
	pFindE->key = NULL;
	pFindE->keySize = 0;
	m_pHashTable->count--;
	return;
}

unsigned long RHTable::Count()
{
	unsigned long count = 0;
	HASH_TABLE *pHashTable = m_pHashTable;
	for ( ; NULL != pHashTable; pHashTable = pHashTable->next )
	{
		count += pHashTable->count;
	}
	
	return count;
}

bool RHTable::IsEmpty()
{
	return 0 == Count();
}

RHTable::Iterator RHTable::Begin()
{
	m_it.pHeadHashTable = m_pHashTable;
	m_it.pHashTable = m_pHashTable;
	m_it.pElement = NULL;
	m_it.idx = 0;
	
	while ( NULL != m_it.pHashTable )
	{
		for ( ; m_it.idx < m_it.pHashTable->size; m_it.idx++ )
		{
			m_it.pElement = m_it.pHashTable->buckets[m_it.idx].head;
			while ( NULL != m_it.pElement )
			{
				if ( m_it.pElement->isDel ) 
				{
					m_it.pElement = m_it.pElement->next;
					continue;
				}
				return m_it;
			}
		}
		m_it.idx = 0;
		m_it.pHashTable = m_it.pHashTable->next;
		m_it.pElement = NULL;
	}
	
	return m_it;
}

RHTable::Iterator RHTable::End()
{
	m_it.pHeadHashTable = m_pHashTable;
	m_it.pHashTable = NULL;
	m_it.pElement = NULL;
	m_it.idx = 0;
	return m_it;
}

void RHTable::Clear()
{
	HASH_TABLE *pHashTable = m_pHashTable;
	ELEMENT *pElement = NULL;
	unsigned int idx = 0;
	
	while ( NULL != pHashTable )
	{
		for ( ; idx < pHashTable->size; idx++ )
		{
			pElement = pHashTable->buckets[idx].head;
			while ( NULL != pElement )
			{
				if ( !pElement->isDel ) 
				{
					pElement->isDel = true;
					pElement->value = NULL;
					delete pElement->key;
					pElement->key = NULL;
					pElement->keySize = 0;
					pHashTable->count--;
				}
				pElement = pElement->next;
			}
		}
		idx = 0;
		pHashTable = pHashTable->next;
	}
	
	return;
}

void RHTable::ReleaseHashTable()
{
	HASH_TABLE *pEmpty = NULL;
	unsigned int i = 0;
	ELEMENT *pDelE = NULL;
	ELEMENT *pHead = NULL;
	while ( NULL != m_pHashTable )
	{
		for ( i = 0; i < m_pHashTable->size; i++ )
		{
			pHead = m_pHashTable->buckets[i].head;
			while ( NULL != pHead )
			{
				if ( NULL != pHead->key )
				{
					delete pHead->key;
					pHead->key = NULL;
				}
				pDelE = pHead;
				pHead = pHead->next;
				delete pDelE;
				pDelE = NULL;
			}
		}
		pEmpty = m_pHashTable;
		m_pHashTable = m_pHashTable->next;
		delete pEmpty->buckets;
		delete pEmpty;
	}
}

RHTable::Iterator& RHTable::Iterator::operator++(int)
{
	++(*this);
	return *this;
}

RHTable::Iterator& RHTable::Iterator::operator++()
{
	if ( NULL == pElement ) return *this;
	pElement = pElement->next;
	while ( NULL != pHashTable )
	{
		for ( ; idx < pHashTable->size; )
		{
			while ( NULL != pElement )
			{
				if ( pElement->isDel ) 
				{
					pElement = pElement->next;
					continue;
				}
				return *this;
			}
			idx++;
			pElement = pHashTable->buckets[idx].head;
		}
		idx = 0;
		pElement = NULL;
		pHashTable = pHashTable->next;
		if ( NULL == pHashTable ) break;
		pElement = pHashTable->buckets[idx].head;
	}
	return *this;
}

RHTable::Iterator& RHTable::Iterator::operator--(int)
{
	--(*this);
	return *this;
}

RHTable::Iterator& RHTable::Iterator::operator--()
{
	unsigned int i = idx;
	HASH_TABLE *pCurTable = pHashTable;
	if ( NULL == pElement && 0 == idx && NULL == pHashTable ) 
	{
		pCurTable = pHeadHashTable;
		while ( NULL != pCurTable->next ) pCurTable = pCurTable->next;
		i = pCurTable->sizemask;
	}
	ELEMENT *pCur = pCurTable->buckets[i].head;
	ELEMENT *pPre = NULL;
	while ( NULL != pCurTable )
	{
		for ( ; 0 <= i && i < pCurTable->size; )
		{
			while ( NULL != pCur && pCur != pElement )
			{
				if ( !pCur->isDel ) pPre = pCur;
				pCur = pCur->next;
			}
			if ( NULL != pPre || 0 == i ) break;
			i--;
			pCur = pCurTable->buckets[i].head;
		}
		if ( NULL != pPre || NULL == pCurTable->pre ) break;
		pCurTable = pCurTable->pre;
		i = pCurTable->sizemask;
		pCur = pCurTable->buckets[i].head;
	}
	if ( NULL != pPre )
	{
		pHashTable = pCurTable;
		pElement = pPre;
		idx = i;
	}
	return *this;
}

bool RHTable::Iterator::operator==(RHTable::Iterator it)
{
	if ( pElement != it.pElement ) return false;
	if ( idx != it.idx ) return false;
	if ( pHashTable != it.pHashTable ) return false;
	return true;
}

bool RHTable::Iterator::operator!=(Iterator it)
{
	if ( pElement != it.pElement ) return true;
	if ( idx != it.idx ) return true;
	if ( pHashTable != it.pHashTable ) return true;
	return false;
}

