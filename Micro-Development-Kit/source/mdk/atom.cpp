#include "../../include/mdk/atom.h"

#ifdef WIN32
#include <windows.h>
#endif

namespace mdk
{

//����,������ֵ
uint32 AtomSelfAdd(void * var) 
{
#ifdef WIN32
  return InterlockedIncrement((long *)(var)); // NOLINT
#else
  return __sync_add_and_fetch((uint32 *)(var), 1); // NOLINT
#endif
}

//�Լ�,������ֵ
uint32 AtomSelfDec(void * var) 
{
#ifdef WIN32
  return InterlockedDecrement((long *)(var)); // NOLINT
#else
  return __sync_add_and_fetch((uint32 *)(var), -1); // NOLINT
#endif
}

//��һ��ֵ,���ؾ�ֵ
uint32 AtomAdd(void * var, const uint32 value) 
{
#ifdef WIN32
  return InterlockedExchangeAdd((long *)(var), value); // NOLINT
#else
  return __sync_fetch_and_add((uint32 *)(var), value);  // NOLINT
#endif
}

//��һ��ֵ,���ؾ�ֵ
uint32 AtomDec(void * var, int32 value) 
{
	value = value * -1;
#ifdef WIN32
	return InterlockedExchangeAdd((long *)(var), value); // NOLINT
#else
	return __sync_fetch_and_add((uint32 *)(var), value);  // NOLINT
#endif
}

//��ֵ,windows�·�����ֵ��linux�·��ؾ�ֵ
uint32 AtomSet(void * var, const uint32 value) 
{
#ifdef WIN32
	::InterlockedExchange((long *)(var), (long)(value)); // NOLINT
#else
	__sync_lock_test_and_set((uint32 *)(var), value);  // NOLINT
#endif
	return value;
}

//ȡֵ
uint32 AtomGet(void * var) 
{
#ifdef WIN32
  return InterlockedExchangeAdd((long *)(var), 0); // NOLINT
#else
  return __sync_fetch_and_add((uint32 *)(var), 0);  // NOLINT
#endif
}

} //namespace mdk
