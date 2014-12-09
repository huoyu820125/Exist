
#include "common.h"
#include "Protocol.h"
#include "mdk/Socket.h"
#include "mdk/mapi.h"

namespace Device
{

const char* Descript( Device::Type::Type type )
{
	if ( Device::Type::motherboard == type ) return "ASUS Z9PE-D8 WS 超强双芯片工作站主板";
	if ( Device::Type::exist == type ) return "创星世纪(CtreaStar)外存条";
	if ( Device::Type::ssd == type ) return "饥饿鲨（OCZ） 企业级SSD RevoDrive 350系列 960G PCI-E固态硬盘";
	if ( Device::Type::cpu == type ) return "Intel core(TM) i7-4960X 3.6GHZ,15MB LGA2011 CPU";
	if ( Device::Type::screen == type ) return "SAMSUNG LED广视角 MD230 23英寸6连屏超大显示屏";
	if ( Device::Type::touch == type ) return "仙视 GM84S1 84英寸LED背光交互式智能6点触控触摸屏";

	return "神器";
}

//状态描述
const char* Descript(Device::Status::Status status)
{
	if ( Device::Status::idle == status) return "idle";
	else if ( Device::Status::loadData == status) return "loadData";
	else if ( Device::Status::running == status) return "running";
	else if ( Device::Status::closing == status) return "closing";
	else if ( Device::Status::waitDevice == status) return "waitDevice";
	else if ( Device::Status::unPlugIn == status) return "unPlugIn";
	else if ( Device::Status::unknow == status) return "unknow";
	return "unknow";
}

}

namespace Exist
{

unsigned char* GetDataBuffer( unsigned char *msg )
{
	return &msg[MSG_HEAD_SIZE];
}

void SendMsg( mdk::Socket &recver, short msgId, unsigned char *data, short size )
{
	MSG_HEADER *header = (MSG_HEADER *)(data - sizeof(MSG_HEADER));
	header->msgId = msgId;
	header->msgSize = size;
	recver.Send( header, MSG_HEAD_SIZE + header->msgSize );
}

#define MAX_RECV_SIZE 4096
int Recv( mdk::Socket &sender, MSG_HEADER &header, unsigned char *buf )
{
	short len = sender.Receive( (unsigned char*)&header, MSG_HEAD_SIZE, true );
	if ( mdk::Socket::seError == len || mdk::Socket::seSocketClose == len ) 
	{
		return 1;//与主板失去链接
	}
	if ( header.msgSize > MAX_DATA_SIZE ) 
	{
		return 2;//主板发送信号超长
	}

	unsigned short unreadSize = header.msgSize + MSG_HEAD_SIZE;
	unsigned short readSize = 0;
	unsigned short pos = 0;
	while ( 0 < unreadSize )
	{
		readSize = unreadSize > MAX_RECV_SIZE?MAX_RECV_SIZE:unreadSize;
		readSize = sender.Receive( &buf[pos], readSize );
		if ( mdk::Socket::seError == readSize || mdk::Socket::seSocketClose == readSize ) 
		{
			return 1;//与主板失去链接
		}
		pos += readSize;
		unreadSize -= readSize;
	}

	return 0;
}

}

#ifdef WIN32
#else
mdk::uint64 GetTickCount()
{
	struct timespec ts;

	clock_gettime(CLOCK_REALTIME, &ts);

	return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}
#endif
