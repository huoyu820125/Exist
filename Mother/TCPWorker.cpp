// TCPWorker.cpp: implementation of the TCPWorker class.
//
//////////////////////////////////////////////////////////////////////

#include "TCPWorker.h"
#include <algorithm>

#ifdef WIN32
#include <io.h>
#include <direct.h>
#include <time.h>
#else
#include <unistd.h>
#include <sys/time.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <cstring>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TCPWorker::TCPWorker(char *cfgFile)
:m_cfgFile( cfgFile )
{
	m_log.SetLogName( "Mother" );
	m_log.SetPrintLog( true );
	m_log.SetMaxLogSize( 10 );
	m_log.SetMaxExistDay( 30 );

	ReadConfig();
	SetAverageConnectCount(1000);
	SetHeartTime( m_heartTime );
	Listen(m_svrPort);
}

TCPWorker::~TCPWorker()
{
}

bool TCPWorker::ReadConfig()
{
	m_svrPort = m_cfgFile["opt"]["port"];
	m_heartTime = m_cfgFile["opt"]["heartTime"];
	if ( 0 >= m_svrPort ) m_svrPort = 7250;
	if (  120 > m_heartTime ) m_heartTime = 120;

	m_existCount = m_cfgFile["Exist"]["count"];
	m_hardDiskCount = m_cfgFile["Hard disk"]["count"];
	
	return true;
}

mdk::Logger& TCPWorker::GetLog()
{
	return m_log;
}

/*
 *	服务器主线程
 *  定时写文件
 */
int TCPWorker::Main()
{
	time_t tCurTime = time( NULL );
	if ( false )
	{
		exit(0);
	}

	return true;
}

void TCPWorker::OnCloseConnect(mdk::STNetHost &host)
{
	DeviceMap::iterator it = m_hardDisks.begin();
	for ( ; it != m_hardDisks.end(); it++ ) 
	{
		if ( it->second.host.ID() == host.ID() )
		{
			m_log.Info( "Error", "%s(%s %d-%s %d)未插紧", Device::Descript(it->second.type), it->second.wanIP.c_str(), it->second.wanPort, it->second.lanIP.c_str(), it->second.lanPort );
			m_hardDisks.erase( it );
			return;
		}
	}


	it = m_exists.begin();
	for ( ; it != m_exists.end(); it++ ) 
	{
		if ( it->second.host.ID() == host.ID() )
		{
			m_log.Info( "Error", "%s(%s %d-%s %d)未插紧", Device::Descript(it->second.type), it->second.wanIP.c_str(), it->second.wanPort, it->second.lanIP.c_str(), it->second.lanPort );
			m_exists.erase( it );
			return;
		}
	}

	it = m_screens.begin();
	for ( ; it != m_screens.end(); it++ ) 
	{
		if ( it->second.host.ID() == host.ID() )
		{
			m_log.Info( "Error", "%s(%s %d-%s %d)未插紧", Device::Descript(it->second.type), it->second.wanIP.c_str(), it->second.wanPort, it->second.lanIP.c_str(), it->second.lanPort );
			m_screens.erase( it );
			return;
		}
	}

	it = m_cpus.begin();
	for ( ; it != m_cpus.end(); it++ ) 
	{
		if ( it->second.host.ID() == host.ID() )
		{
			m_log.Info( "Error", "%s(%s %d-%s %d)未插紧", Device::Descript(it->second.type), it->second.wanIP.c_str(), it->second.wanPort, it->second.lanIP.c_str(), it->second.lanPort );
			m_cpus.erase( it );
			return;
		}
	}


	return;
}

void TCPWorker::OnInvalidMsg(mdk::STNetHost &host, ErrorType type, unsigned char *msg, unsigned short len)
{
	std::string ip;
	int port;
	if ( host.IsServer() ) host.GetServerAddress( ip, port );
	else host.GetAddress( ip, port );
	m_log.StreamInfo( "Error", msg, len, "主机(%s-%d)发送非法格式报文", ip.c_str(), port );
	host.Close();
}

/**
 * 数据到达，回调方法
 * 
 * 派生类实现具体断开连接业务处理
 * 
*/
void TCPWorker::OnWork( mdk::STNetHost &host, MSG_HEADER *header, unsigned char *pData )
{
	bool isValidMsg = true;
	switch( header->msgId )
	{
	case MsgId::heartbeat://心跳
		break;
	case MsgId::plugInQuery ://设备请求插入
		isValidMsg = OnPlugIn(host, header, (MSG_PLUG_IN_QUERY*)pData);
		break;
	default:
		isValidMsg = false;
		break;
	}

	if ( !isValidMsg ) OnInvalidMsg( host, BStructSvr::paramError, (unsigned char*)header, header->msgSize + MSG_HEAD_SIZE );

	return;
}

void TCPWorker::SetDeviceId( mdk::STNetHost &host, unsigned char deviceId )
{
	MSG_SET_DEVICE_ID *pData = (MSG_SET_DEVICE_ID*)GetDataBuffer();
	pData->deviceId = deviceId;
	SendMsg( host, MsgId::setDeviceId, sizeof(MSG_SET_DEVICE_ID) );
}

void TCPWorker::RunDevice( Device::INFO &device )
{
	m_log.Info( "Run", "运行%s(%s %d-%s %d)", Device::Descript( device.type ), 
		device.wanIP.c_str(), device.wanPort, device.lanIP.c_str(), device.lanPort ); 
	DevicePostion( device.host );

	SendMsg( device.host, MsgId::runDevice, 0 );
	device.status = Device::Status::running;
}

void TCPWorker::DevicePostion( mdk::STNetHost &host  )
{
	MSG_DEVICE_POSTION *pData = (MSG_DEVICE_POSTION*)GetDataBuffer();
	pData->size = 0;

	//所有外存条
	std::vector<int> ids;
	DeviceMap::iterator it = m_exists.begin();
	for ( ids.clear(); it != m_exists.end(); it++ ) ids.push_back( it->first );
	std::make_heap(ids.begin(), ids.end());//构造堆
	std::sort_heap(ids.begin(), ids.end());//堆排序
	
	unsigned int i = 0;
	for ( i = 0; i < ids.size(); i++ )
	{
		pData->devices[pData->size].device = (char)m_exists[ids[i]].type;
		pData->devices[pData->size].deviceId = (unsigned char)m_exists[ids[i]].deviceId;
		strcpy( pData->devices[pData->size].wanIP, m_exists[ids[i]].wanIP.c_str() );
		pData->devices[pData->size].wanPort = m_exists[ids[i]].wanPort;
		strcpy( pData->devices[pData->size].lanIP, m_exists[ids[i]].lanIP.c_str() );
		pData->devices[pData->size].lanPort = m_exists[ids[i]].lanPort;
		pData->size++;
	}

	//所有固态硬盘
	it = m_hardDisks.begin();
	for ( ids.clear(); it != m_hardDisks.end(); it++ ) ids.push_back( it->first );
	std::make_heap(ids.begin(), ids.end());//构造堆
	std::sort_heap(ids.begin(), ids.end());//堆排序
	
	for ( i = 0; i < ids.size(); i++ )
	{
		pData->devices[pData->size].device = (char)m_hardDisks[ids[i]].type;
		pData->devices[pData->size].deviceId = (unsigned char)m_hardDisks[ids[i]].deviceId;
		strcpy( pData->devices[pData->size].wanIP, m_hardDisks[ids[i]].wanIP.c_str() );
		pData->devices[pData->size].wanPort = m_hardDisks[ids[i]].wanPort;
		strcpy( pData->devices[pData->size].lanIP, m_hardDisks[ids[i]].lanIP.c_str() );
		pData->devices[pData->size].lanPort = m_hardDisks[ids[i]].lanPort;
		pData->size++;
	}

	//所有触摸屏
	it = m_screens.begin();
	for ( ids.clear(); it != m_screens.end(); it++ ) ids.push_back( it->first );
	std::make_heap(ids.begin(), ids.end());//构造堆
	std::sort_heap(ids.begin(), ids.end());//堆排序
	
	for ( i = 0; i < ids.size(); i++ )
	{
		pData->devices[pData->size].device = (char)m_screens[ids[i]].type;
		pData->devices[pData->size].deviceId = (unsigned char)m_screens[ids[i]].deviceId;
		strcpy( pData->devices[pData->size].wanIP, m_screens[ids[i]].wanIP.c_str() );
		pData->devices[pData->size].wanPort = m_screens[ids[i]].wanPort;
		strcpy( pData->devices[pData->size].lanIP, m_screens[ids[i]].lanIP.c_str() );
		pData->devices[pData->size].lanPort = m_screens[ids[i]].lanPort;
		pData->size++;
	}
	//所有CPU
	it = m_cpus.begin();
	for ( ids.clear(); it != m_cpus.end(); it++ ) ids.push_back( it->first );
	std::make_heap(ids.begin(), ids.end());//构造堆
	std::sort_heap(ids.begin(), ids.end());//堆排序
	
	for ( i = 0; i < ids.size(); i++ )
	{
		pData->devices[pData->size].device = (char)m_cpus[ids[i]].type;
		pData->devices[pData->size].deviceId = (unsigned char)m_cpus[ids[i]].deviceId;
		strcpy( pData->devices[pData->size].wanIP, m_cpus[ids[i]].wanIP.c_str() );
		pData->devices[pData->size].wanPort = m_cpus[ids[i]].wanPort;
		strcpy( pData->devices[pData->size].lanIP, m_cpus[ids[i]].lanIP.c_str() );
		pData->devices[pData->size].lanPort = m_cpus[ids[i]].lanPort;
		pData->size++;
	}

	SendMsg( host, MsgId::devicePostion, sizeof(MSG_DEVICE_POSTION) );
}

//设备请求插入
bool TCPWorker::OnPlugIn(mdk::STNetHost &host, MSG_HEADER *header, MSG_PLUG_IN_QUERY *pData)
{
	//////////////////////////////////////////////////////////////////////////
	/*
		检查报文合法性
		1.该有的字段有没有
		2.长度是否与协议约定相等
		3.字符串等变长字段长度必须不小于1
	*/
	if ( header->msgSize != sizeof(MSG_PLUG_IN_QUERY) ) return false;
	Device::Type::Type device = (Device::Type::Type)pData->device;
	int deviceId = pData->deviceId;
	//////////////////////////////////////////////////////////////////////////
	//业务处理
	/*
		关于设备id分配规则
		按照插入顺序递增，
		数据分片时是认为外存和固态硬盘的id是0~n连续的，
		所以不可改变此id分配规则
	*/
	if ( Device::Type::ssd== pData->device ) //固态硬盘插入
	{
		if ( m_hardDiskCount == (int)m_hardDisks.size() ) 
		{
			m_log.Info( "Error", "硬盘插槽已插满，拒绝硬盘(%s %d-%s %d)插入", 
				pData->wanIP, pData->wanPort, pData->lanIP, pData->lanPort );
			host.Close();
			return true;
		}
		if ( -1 != pData->deviceId ) 
		{
			DeviceMap::iterator it =  m_hardDisks.find( deviceId );
			if ( it != m_hardDisks.end() )
			{
				m_log.Info( "Error", "硬盘设备ID重复，拒绝硬盘(%s %d-%s %d)插入", 
					pData->wanIP, pData->wanPort, pData->lanIP, pData->lanPort );
				host.Close();
				return true;
			}
		}
		else 
		{
			deviceId = m_hardDisks.size();
			while ( m_hardDisks.end() != m_hardDisks.find(deviceId) ) deviceId++;
			SetDeviceId( host, deviceId );
		}

		m_hardDisks[deviceId].deviceId = deviceId;
		m_hardDisks[deviceId].host = host;
		m_hardDisks[deviceId].lanIP = pData->lanIP;
		m_hardDisks[deviceId].lanPort = pData->lanPort;
		m_hardDisks[deviceId].wanIP = pData->wanIP;
		m_hardDisks[deviceId].wanPort = pData->wanPort;
		m_hardDisks[deviceId].type = device;
		m_hardDisks[deviceId].status = Device::Status::running;
		m_log.Info( "Run", "%s(%s %d-%s %d)插入", Device::Descript( device ), 
			pData->wanIP, pData->wanPort, pData->lanIP, pData->lanPort ); 
		RunDevice( m_hardDisks[deviceId] );//硬盘不需要任何支持设备1插入就可以开始运行

		if ( m_hardDiskCount == (int)m_hardDisks.size() ) //所有硬盘已运行，运行所有等待中的外存条
		{
			m_log.Info( "Run", "%d个%s已全部插入", m_hardDiskCount, Device::Descript( device ) ); 
			DeviceMap::iterator it = m_exists.begin();
			for ( ; it != m_exists.end(); it++ )
			{
				if ( Device::Status::waitDevice == it->second.status ) 
				{
					RunDevice( it->second );
				}
			}
		}

		return true;
	}

	if ( Device::Type::exist == device ) //外存条插入
	{
		if ( m_existCount == (int)m_exists.size() ) 
		{
			m_log.Info( "Error", "外存条插槽已插满，拒绝外存条(%s %d-%s %d)插入", 
				pData->wanIP, pData->wanPort, pData->lanIP, pData->lanPort );
			host.Close();
			return true;
		}

		if ( -1 != deviceId ) 
		{
			DeviceMap::iterator it =  m_exists.find( deviceId );
			if ( it != m_exists.end() )
			{
				m_log.Info( "Error", "外存条设备ID重复，拒绝外存条(%s %d-%s %d)插入", 
					pData->wanIP, pData->wanPort, pData->lanIP, pData->lanPort );
				host.Close();
				return true;
			}
		}
		else 
		{
			deviceId = m_exists.size();
			while ( m_exists.end() != m_exists.find(deviceId) ) deviceId++;
			SetDeviceId( host, deviceId );
		}

		m_exists[deviceId].deviceId = deviceId;
		m_exists[deviceId].host = host;
		m_exists[deviceId].lanIP = pData->lanIP;
		m_exists[deviceId].lanPort = pData->lanPort;
		m_exists[deviceId].wanIP = pData->wanIP;
		m_exists[deviceId].wanPort = pData->wanPort;
		m_exists[deviceId].type = device;
		m_exists[deviceId].status = Device::Status::waitDevice;
		m_log.Info( "Run", "%s(%s %d-%s %d)插入", Device::Descript( device ), 
			pData->wanIP, pData->wanPort, pData->lanIP, pData->lanPort ); 

		if ( m_hardDiskCount == (int)m_hardDisks.size() ) //外存条需要硬盘做持久化支持，要等配置指定的所有硬盘都插入了，才能开始运行
		{
			RunDevice( m_exists[deviceId] );
		}

		if ( m_existCount == (int)m_exists.size() ) //CPU需要外存条做数据支持，要等配置指定的所有外存条都插入了，才能开始运行
		{
			m_log.Info( "Run", "%d个%s已全部插入", m_existCount, Device::Descript( device ) ); 
			DeviceMap::iterator it = m_cpus.begin();
			for ( ; it != m_cpus.end(); it++ )
			{
				if ( Device::Status::waitDevice == it->second.status ) 
				{
					RunDevice( it->second );
				}
			}
		}
		return true;
	}

	if ( Device::Type::cpu== device ) //CPU插入
	{
		if ( -1 != deviceId ) 
		{
			DeviceMap::iterator it =  m_cpus.find( deviceId );
			if ( it != m_cpus.end() )
			{
				m_log.Info( "Error", "CPU设备ID重复，拒绝CPU插入" );
				host.Close();
				return true;
			}
		}
		else 
		{
			deviceId = m_cpus.size();
			while ( m_cpus.end() != m_cpus.find(deviceId) ) deviceId++;
			SetDeviceId( host, deviceId );
		}
		m_cpus[deviceId].deviceId = deviceId;
		m_cpus[deviceId].host = host;
		m_cpus[deviceId].type = device;
		m_cpus[deviceId].lanIP = pData->lanIP;
		m_cpus[deviceId].lanPort = pData->lanPort;
		m_cpus[deviceId].wanIP = pData->wanIP;
		m_cpus[deviceId].wanPort = pData->wanPort;
		m_cpus[deviceId].status = Device::Status::waitDevice;
		m_log.Info( "Run", "%s(%s %d-%s %d)插入", Device::Descript( device ), 
			pData->wanIP, pData->wanPort, pData->lanIP, pData->lanPort ); 

		if ( m_existCount == (int)m_exists.size() ) //CPU需要外存条做数据支持，要等配置指定的所有外存条都插入了，才能开始运行
		{
			RunDevice( m_cpus[deviceId] );
		}
		
		return true;
	}

	if ( Device::Type::screen== device || Device::Type::touch== device ) //显示屏插入
	{
		if ( -1 != deviceId ) 
		{
			DeviceMap::iterator it =  m_screens.find( deviceId );
			if ( it != m_cpus.end() )
			{
				m_log.Info( "Error", "触摸屏设备ID重复，拒绝触摸屏插入" );
				host.Close();
				return true;
			}
		}
		else 
		{
			deviceId = m_screens.size();
			while ( m_screens.end() != m_screens.find(deviceId) ) deviceId++;
			SetDeviceId( host, deviceId );
		}
		m_screens[deviceId].deviceId = deviceId;
		m_screens[deviceId].host = host;
		m_screens[deviceId].type = device;
		m_screens[deviceId].lanIP = pData->lanIP;
		m_screens[deviceId].lanPort = pData->lanPort;
		m_screens[deviceId].wanIP = pData->wanIP;
		m_screens[deviceId].wanPort = pData->wanPort;
		m_screens[deviceId].status = Device::Status::waitDevice;
		m_log.Info( "Run", "%s(%s %d-%s %d)插入", Device::Descript( device ), 
			pData->wanIP, pData->wanPort, pData->lanIP, pData->lanPort ); 
		RunDevice( m_screens[deviceId] );//触摸屏不需要任何支持设备1插入就可以开始运行

		return true;
	}

	return true;
}

