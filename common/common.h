#ifndef COMMOND_H
#define COMMOND_H

#include "frame/netserver/STNetServer.h"
#include "Protocol.h"

#ifdef WIN32
#include "windows.h"
#else
mdk::uint64 GetTickCount();
#endif

//设备
namespace Device
{
	//类型
	namespace Type
	{
		enum Type
		{
			motherboard = 0,	//主板：	ASUS Z9PE-D8 WS 超强双芯片工作站主板！支持4路SLI 与CrossfireX多重图形技术高速流畅运行！（京 东 价：￥4999.00）
			exist = 1,			//外存条：	创星世纪(CtreaStar)（世界上第一根外存条，参考价：￥1.00）
			ssd = 2,			//固态硬盘：饥饿鲨（OCZ） 企业级SSD RevoDrive 350系列 960G PCI-E固态硬盘（京 东 价：￥9599.00）
			cpu = 3,			//CPU：		Intel core(TM) i7-4960X 3.6GHZ,15MB LGA2011（京 东 价：￥7699.00）
			screen = 4,			//显示屏：	SAMSUNG LED广视角 MD230 23英寸6连屏！（京 东 价：￥29998.00）
			touch = 5,			//触摸屏：	Goodview（仙视） GM84S1 84英寸LED背光交互式智能6点触控触摸屏！（京 东 价：￥59999.00）
		};
	}

	//类型
	namespace Status
	{
		enum Status
		{
			unknow = 0,
			unPlugIn = 1,
			idle = 2,
			waitDevice = 3,
			running = 4,
			loadData = 5,
			closing = 6,
		};
	}

	typedef struct INFO
	{
		unsigned char deviceId;
		Type::Type type;
		Status::Status status;//状态
		mdk::STNetHost host;//主机
		std::string wanIP;//外网ip
		std::string lanIP;//内网ip
		unsigned int wanPort;//外网服务端口
		unsigned int lanPort;//内网服务端口
	}INFO;

	const char* Descript( Device::Type::Type type );
	const char* Descript( Device::Status::Status status );
}

namespace Exist
{
	unsigned char* GetDataBuffer( unsigned char *msg );
	void SendMsg( mdk::Socket &recver, short msgId, unsigned char *data, short size );
	int Recv( mdk::Socket &sender, MSG_HEADER &header, unsigned char *buf );
}

#endif //COMMOND_H
