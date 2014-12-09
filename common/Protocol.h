// Protocl.h: interface for the Protocl class.
//
//////////////////////////////////////////////////////////////////////

#ifndef PROTOCOL_H
#define PROTOCOL_H
#include "../include/Exist/frame/ExistType.h"

#pragma pack(1)
typedef struct MSG_HEADER
{
	unsigned short		msgId;				//报文ID
	unsigned short		msgSize;			//报文长度
}MSG_HEADER;
#pragma pack()

#define MSG_HEAD_SIZE		sizeof(MSG_HEADER) //报文头长度
#define MAX_VALUE_SIZE		5242880 //nosql记录数据最大长度5M
#define MAX_PARAM_SIZE		4096//报文参数最大长度
#define MAX_DATA_SIZE		(MAX_PARAM_SIZE + MAX_VALUE_SIZE) //报文数据最大长度
#define MAX_MSG_SIZE		(MSG_HEAD_SIZE + MAX_DATA_SIZE)//消息缓冲最大长度，报文头+变长参数结构+value数据最大长度
#define	MAX_DEVICE_COUNT	500	//整个分布式网络中能容纳的设备数量

//报文id
namespace MsgId
{
	enum MsgId
	{
		unknow = 0,//未知报文
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//分布式报文
		heartbeat = 1000,//心跳
		plugInQuery = 1001,//设备请求插入
		setDeviceId = 1002,//设置设备ID
		runDevice = 1003,//设备运行通知，告诉设备你可以开始工作了
		devicePostion = 1004,//设备位置通知

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//业务报文
		readData	= 3001,//读数据
		writeData	= 3002,//写数据
		deleteData = 3003,//删除数据
	};
}

#pragma pack(1)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//分布式报文
//心跳
typedef struct MSG_HEART_BEAT
{
}MSG_HEART_BEAT;

//设备请求插入
typedef struct MSG_PLUG_IN_QUERY
{
	short				deviceId;				//设备ID(选填)
	unsigned char		device;					//设备类型,参考DeviceType
	char				wanIP[16];				//外网IP (Device ！= cpu时，必填)
	int					wanPort;				//外网端口 (Device ！= cpu时，必填)
	char				lanIP[16];				//内网IP (Device ！= cpu时，必填)
	int					lanPort;				//内网端口 (Device ！= cpu时，必填)
}MSG_PLUG_IN_QUERY;

//设置设备ID
typedef struct MSG_SET_DEVICE_ID
{
	unsigned char		deviceId;		//设备id，同类型设备以id区分。256根外存条还不够 + 256个硬盘还不够你用？
}MSG_SET_DEVICE_ID;

//设备运行通知，告诉设备你可以开始工作了
typedef struct MSG_RUN_DEVICE
{
}MSG_RUN_DEVICE;

//设备位置通知
//设备数组，按照deviceId从小到大排好
typedef struct MSG_DEVICE_POSTION
{
	short				size;
	MSG_PLUG_IN_QUERY	devices[MAX_DEVICE_COUNT];
}MSG_DEVICE_POSTION;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//业务报文

//写数据
typedef struct WRITE_DATA
{
	exist::DATA_KEY	key;				//数据key
	char			updateType;			//更新类型参考enum UpdateType
	unsigned int	size;				//数据大小
	char			protracted;			//数据是持久化，非0是，0否（预留字段，暂不实现持久化功能）
}WRITE_DATA;

//读数据
typedef struct READ_DATA
{
	exist::DATA_KEY	key;				//数据key
	unsigned int	initSize;			//数据初始化大小（数据不存在时使用）
	char			protracted;			//数据是持久化，非0是，0否（预留字段，暂不实现持久化功能）
}READ_DATA;

#pragma pack()

#endif // !defined(PROTOCOL_H)
