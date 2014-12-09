// ConnectPool.h: interface for the ConnectPool class.
//
//////////////////////////////////////////////////////////////////////

#ifndef CONNECTPOOL_H
#define CONNECTPOOL_H

#include "mdk/Socket.h"
#include "mdk/Thread.h"
#include "mdk/Signal.h"
#include "mdk/Logger.h"
#include "mdk/Lock.h"
#include <map>
#include <vector>
#include <string>
#include "../../../common/common.h"

namespace Exist
{
	
class ConnectPool  
{
public:
	ConnectPool();
	virtual ~ConnectPool();

	bool PlugIn();
	mdk::Socket& GetSocket( unsigned int hashId );

private:
	bool CreateDir( const char *strDir );
	bool CreateFile( const char *strFile );
private:
	mdk::Logger m_log;
	mdk::Thread m_guideThread;
	void* RemoteCall GuideThread( void * );
	Device::INFO										m_cpu;//设备信息
	std::vector<Device::INFO>							m_existAdrs;//外存地址表
	bool												m_sockInited;//windows下已调用WSAStartup()
	mdk::Signal											m_sigInited;//初始化信号	
	mdk::Mutex											m_connectsLock;//m_connectMap线程安全
	std::map<mdk::uint64, std::vector<mdk::Socket> >	m_connectMap;//[线程Id]<->[线程独占的外存连接表]映射
	
};

}
#endif // ifndef CONNECTPOOL_H

