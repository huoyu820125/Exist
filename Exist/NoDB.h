// NoDB.h: interface for the NoDB class.
//
//////////////////////////////////////////////////////////////////////

#ifndef NOSQL_H
#define NOSQL_H

#include <ctime>
#include "mdk/ConfigFile.h"
#include "mdk/Signal.h"
#include "mdk/Logger.h"
#include "mdk/Thread.h"
#include "mdk/Lock.h"
#include "mdk/Socket.h"
#include "RHTable.h"
#include <string>
#include <map>

#include "../common/common.h"
#include "../common/BStructSvr.h"
#include "../common/ExistFS.h"
#include "../include/Exist/frame/ExistType.h"
//创建数据/数据的引用
typedef	struct CREATE_DATA
{
	exist::DATA_KEY	key;				//数据key
	unsigned char	size;				//data类型有效，容器类型无效
	char			protracted;			//数据是持久化，非0是，0否（预留字段，暂不实现持久化功能）
}CREATE_DATA;

class NoDB : public BStructSvr
{
/*
 *	NoDB上下文
 */
typedef struct NO_DB_CONTEXT
{
	Device::INFO device;
//	/*
//		分片号
//		distributed=true并且role=piece时必须配置，否则忽略
//		piece类型的结点负责提供服务的数据所在的数据区间编号
//		片号从0开始,片号的计算公式为:数据的hashexist::VALUE/分片大小
//		比如总数据中hashexist::VALUE最大的1234,分片大小被设置为100,那么总数据就被分为了11个分片
//		0号分片上是hashexist::VALUE为0~99的数据 0~99之间任何数字/100 = 0,
//		1号分片上是hashexist::VALUE为100~199的数据 100~199之间任何数字/100 = 1,
//		...
//		10号分片上是hashexist::VALUE为1000~1999的数据 1000~1999之间任何数字/100 = 10,
//
//		配置小于0的片号时,表示分片启动后,不加载数据,只是挂在网络中待机，
//		等待用户指令确定对几号片的数据进行加载,提供服务
//	*/
//	int pieceNo;
	unsigned int maxMemory;
	bool m_bStop;
}NO_DB_CONTEXT;

public:
	NoDB(char* cfgFile);
	virtual ~NoDB();
	mdk::Logger& GetLog();
		
protected:
	//服务器主线程
	virtual int Main();
	
	void OnConnect(mdk::STNetHost &host);//连接到达响应
	virtual void OnInvalidMsg(mdk::STNetHost &host, ErrorType type, unsigned char *msg, unsigned short len);
	virtual void OnWork( mdk::STNetHost &host, MSG_HEADER *header, unsigned char *pData );//报文被响应返回true,否则返回false

	//接收Guide连接上的数据并处理，如果host不是Guide则什么都不做，返回false
	bool OnGuideMsg( mdk::STNetHost &host, MSG_HEADER *header, unsigned char *pData );
	//连接断开响应
	void OnCloseConnect(mdk::STNetHost &host);
	//结点校正响应
	void OnSetDeviceId( mdk::STNetHost &host, MSG_HEADER *header, unsigned char *pData );
	//设置数据库
	void OnDevicePostion( mdk::STNetHost &host, MSG_HEADER *header, unsigned char *pData );
	//设置主片
	void OnRunDevice( mdk::STNetHost &host, MSG_HEADER *header, unsigned char *pData );
		
	//接收client连接上的数据并处理，如果host是Guide则什么都不做，返回false
	bool OnClientMsg( mdk::STNetHost &host, MSG_HEADER *header, unsigned char *pData );

	void OnCreateData(CREATE_DATA *pParam, unsigned char *path, int size);
	void OnDeleteData(exist::DATA_KEY *pParam, unsigned char *path, int size);
	void OnWriteData(WRITE_DATA *pParam, unsigned char* pData, unsigned char *path, int size);
	void OnReadData(mdk::STNetHost &host, READ_DATA *pParam, unsigned char *path, int size);

private:
	void Heartbeat( time_t tCurTime );//心跳
	static void AddSelf( exist::VALUE *pValue, unsigned char *pData );
	static void SubtractSelf( exist::VALUE *pValue, unsigned char *pData );
	static void MultiplySelf( exist::VALUE *pValue, unsigned char *pData );
	static bool DivideSelf( exist::VALUE *pValue, unsigned char *pData );
	RHTable* GetNosql(bool isProtracted, DataType::DataType type, unsigned char *path, int size, exist::VALUE **pParent);
	RHTable* FindNosql(bool isProtracted, RHTable* pNosql, unsigned char *path, int size, exist::VALUE **pParent);
	exist::VALUE* CreateData(CREATE_DATA *pParam, unsigned char *path, int size);//创建数据
	mdk::Socket* GetSSDConnect(int keyHashid);//取得固态硬盘的同步连接

private:
	mdk::Logger m_log;
	mdk::ConfigFile m_cfgFile;
	NO_DB_CONTEXT m_context;//NoDB上下文
	Device::INFO m_motherBoard;//主板
	mdk::Thread t_exit;//退出程序线程

	//Exist数据进...Mem
	//SSD数据进...DB
	RHTable m_nosqlMem;//内存数据总表
	RHTable m_nosqlDB;//持久化数据总表
	//各基础类型对应内存表与持久化表
	RHTable m_int8Mem;
	RHTable m_int8DB;
	RHTable m_uint8Mem;
	RHTable m_uint8DB;
	RHTable m_int16Mem;
	RHTable m_int16DB;
	RHTable m_uint16Mem;
	RHTable m_uint16DB;
	RHTable m_int32Mem;
	RHTable m_int32DB;
	RHTable m_uint32Mem;
	RHTable m_uint32DB;
	RHTable m_int64Mem;
	RHTable m_int64DB;
	RHTable m_uint64Mem;
	RHTable m_uint64DB;
	RHTable m_floatMem;
	RHTable m_floatDB;
	RHTable m_doubleMem;
	RHTable m_doubleDB;

	//////////////////////////////////////////////////////////////////////////
	//持久化相关
private:
	void ReadDataFromSSD( exist::VALUE *pValue, unsigned char *path, int size );//从固态硬盘取数据
	void SaveDataToSSD( exist::VALUE *pValue, unsigned char *path, int size );//保存数据到固态硬盘

	void ReadData();//读取数据到固态硬盘
	const char* ReadRootData();//读取根目录数据到固态硬盘
	void ReadDataFromFile( exist::VALUE *pValue );//从文件读取一条数据
	void AddSaveWaiter(exist::VALUE *pValue, WRITE_DATA *pParam);//将更新加入m_waitSaveDatas
	void SaveData(time_t tCurTime);//数据持久化

private:
	typedef std::map<int, Device::INFO*> DeviceMap;
	DeviceMap m_hardDisks;//固态硬盘
	std::map<int,mdk::Socket>	m_hardConnects;//固态硬盘同步连接
	std::map<exist::VALUE*, exist::VALUE*>	m_waitSaveDatas;//待写入数据，5S/1000条更新合并写入
	int m_maxCachedTime;//最大缓存数据(单位S)，开始写入文件
	int m_maxCachedCount;//最大缓存数据条数(比如数据A更新了3次，数据B新创建，是有2条数据待写入)，开始写入文件

	exist::ExistFS m_existFS;//文件系统接口
};

#endif // !defined NOSQL_H
