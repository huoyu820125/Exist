// TCPWorker.h: interface for the TCPWorker class.
//
//////////////////////////////////////////////////////////////////////

#ifndef TCPWORKER_H
#define TCPWORKER_H

#include "mdk/ConfigFile.h"
#include "mdk/Logger.h"
#include "../common/BStructSvr.h"
#include "../common/common.h"
#include "../common/Protocol.h"

#include <vector>
#include <map>

/*
	tcp工作类
	接收tcp消息，执行业务
 */
class TCPWorker : public BStructSvr
{
	/*
	 *	设备映射表
	 *	key = host.ID()
	 */
	typedef std::map<int, Device::INFO> DeviceMap;
public:
	TCPWorker( char *cfgFile );
	virtual ~TCPWorker();
	mdk::Logger& GetLog();

	virtual int Main();
	virtual void OnInvalidMsg(mdk::STNetHost &host, ErrorType type, unsigned char *msg, unsigned short len);//非法报文
	virtual void OnWork( mdk::STNetHost &host, MSG_HEADER *header, unsigned char *pData );
	void OnCloseConnect(mdk::STNetHost &host);

private:
	bool ReadConfig();
	//////////////////////////////////////////////////////////////////////////
	//报文响应
	bool OnPlugIn(mdk::STNetHost &host, MSG_HEADER *header, MSG_PLUG_IN_QUERY *pData);//设备请求插入,报文正确返回true,否则返回false

	//////////////////////////////////////////////////////////////////////////
	//通知设备
	void SetDeviceId( mdk::STNetHost &host, unsigned char deviceId );//设置设备ID
	void RunDevice( Device::INFO &device );//运行设备
	void DevicePostion( mdk::STNetHost &host  );//通知设备位置

private:
	mdk::Logger m_log;//运行log
	mdk::ConfigFile m_cfgFile;//配置文件
	DeviceMap m_exists;//已插入的外存条
	DeviceMap m_hardDisks;//已插入的固态硬盘
	DeviceMap m_screens;//已插入的显示屏
	DeviceMap m_cpus;//已插入的CPU
	int m_existCount;//支持系统运行需要插入的外存条数量，m_existCount <= 0 表示不需要硬盘
	int m_hardDiskCount;//支持系统运行最少需要插入的硬盘数量，m_hardDiskCount <= 0 表示不需要硬盘
	

	int m_svrPort;//接受exist进程连接的端口
	int m_heartTime;//exist心跳时间

};

#endif // !defined TCPWORKER_H
