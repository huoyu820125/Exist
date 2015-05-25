// NoDB.cpp: implementation of the NoDB class.
//
//////////////////////////////////////////////////////////////////////

#include "NoDB.h"

#include <cstdio>
#include <cstdlib>

#ifdef WIN32
#else
#include <sys/time.h>
#endif

#include "mdk/Socket.h"
#include "mdk/mapi.h"
#include "../common/Protocol.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NoDB::NoDB(char* cfgFile)
:m_cfgFile(cfgFile)
{
#ifdef EXIST_DEVICE
	m_log.SetLogName( "Exist" );
#endif
#ifdef SSD_DEVICE
	m_log.SetLogName( "SolidStateDrive" );
#endif
	m_log.SetPrintLog( true );
	m_log.SetMaxLogSize( 10 );
	m_log.SetMaxExistDay( 30 );
	OpenNoDelay();
	m_context.device.status = Device::Status::unPlugIn;
#ifdef EXIST_DEVICE
	m_context.device.type = Device::Type::exist;
#else
	m_context.device.type = Device::Type::ssd;
#endif
	if ( !m_cfgFile["opt"]["id"].IsNull() )
	{
		int deviceId = m_cfgFile["opt"]["id"];
		if ( 0 > deviceId || deviceId > 255 )
		{
			m_log.Info( "Run", "错误的配置：设备ID只能是0～255", m_context.device.deviceId );
			exit(0);
		}
		m_context.device.deviceId = deviceId;
		m_log.Info( "Run", "找到设备ID:%d", m_context.device.deviceId );
	}
	else m_context.device.deviceId = -1;
	m_context.device.wanIP = (std::string)m_cfgFile["opt"]["wan ip"];
	m_context.device.wanPort = m_cfgFile["opt"]["wan port"];
	m_context.device.lanIP = (std::string)m_cfgFile["opt"]["lan ip"];
	m_context.device.lanPort = m_cfgFile["opt"]["lan port"];

	m_motherBoard.status = Device::Status::unknow;
	m_motherBoard.wanIP = (std::string)m_cfgFile["Mother"]["ip"];
	m_motherBoard.wanPort = m_cfgFile["Mother"]["port"];

	m_context.m_bStop = false;
	m_context.maxMemory = m_cfgFile["opt"]["max memory"];
	m_nosqlMem.SetRemoteMode(true);
	m_nosqlDB.SetRemoteMode(true);
	m_int8Mem.SetRemoteMode(true);
	m_int8DB.SetRemoteMode(true);
	m_uint8Mem.SetRemoteMode(true);
	m_uint8DB.SetRemoteMode(true);
	m_int16Mem.SetRemoteMode(true);
	m_int16DB.SetRemoteMode(true);
	m_uint16Mem.SetRemoteMode(true);
	m_uint16DB.SetRemoteMode(true);
	m_int32Mem.SetRemoteMode(true);
	m_int32DB.SetRemoteMode(true);
	m_uint32Mem.SetRemoteMode(true);
	m_uint32DB.SetRemoteMode(true);
	m_int64Mem.SetRemoteMode(true);
	m_int64DB.SetRemoteMode(true);
	m_uint64Mem.SetRemoteMode(true);
	m_uint64DB.SetRemoteMode(true);
	m_floatMem.SetRemoteMode(true);
	m_floatDB.SetRemoteMode(true);
	m_doubleMem.SetRemoteMode(true);
	m_doubleDB.SetRemoteMode(true);

#ifdef SSD_DEVICE
	m_existFS.SetRootDir(((std::string)m_cfgFile["opt"]["data root dir"]).c_str());
	m_maxCachedTime = m_cfgFile["opt"]["max cached time"];
	m_maxCachedCount = m_cfgFile["opt"]["max cached count"];
	ReadData();//硬盘是数据是持久化的，所以要先读取数据
#endif
	m_log.Info( "Run", "寻找主板(%s %d)", m_motherBoard.wanIP.c_str(), m_motherBoard.wanPort );
	Connect( m_motherBoard.wanIP.c_str(), m_motherBoard.wanPort, NULL, 5 );//寻找主板
}

NoDB::~NoDB()
{
}

mdk::Logger& NoDB::GetLog()
{
	return m_log;
}

/*
 *	服务器主线程
 *  定时写文件
 */
int NoDB::Main()
{
	time_t tCurTime = time( NULL );
	Heartbeat( tCurTime );//发送心跳
#ifdef SSD_DEVICE
	SaveData( tCurTime );//持久化数据
#endif

	if ( false )
	{
		exit(0);
	}
	return true;
}

//连接到达响应
void NoDB::OnConnect(mdk::STNetHost &host)
{
	std::string ip;
	int port;
	if ( host.IsServer() )
	{
		host.GetServerAddress( ip, port );
		
		//已连接主板
		if ( ip == m_motherBoard.wanIP && port == (int)m_motherBoard.wanPort )
		{
			m_motherBoard.host = host;
			m_motherBoard.status = Device::Status::running;
			//插入主板
			m_log.Info( "Run", "插入主板" );
			MSG_PLUG_IN_QUERY *pData = (MSG_PLUG_IN_QUERY*)GetDataBuffer();
			pData->deviceId = -1;
			if ( !m_cfgFile["opt"]["id"].IsNull() ) 
			{
				pData->deviceId = m_context.device.deviceId;
			}
			if (  Device::Type::ssd == m_context.device.type ) pData->device = (unsigned char)Device::Type::ssd;
			else pData->device = (unsigned char)Device::Type::exist;
			strcpy( pData->wanIP, m_context.device.wanIP.c_str() );
			pData->wanPort = m_context.device.wanPort;
			strcpy( pData->lanIP, m_context.device.lanIP.c_str() );
			pData->lanPort = m_context.device.lanPort;
			SendMsg( host, MsgId::plugInQuery, sizeof(MSG_PLUG_IN_QUERY) );
			return;
		}

		m_log.Info( "Waring", "未知设备(%s %d)，断开链接", ip.c_str(), port );
		host.Close();
		return;
	}

	host.GetAddress( ip, port );
	if ( Device::Type::exist == m_context.device.type ) m_log.Info( "Run", "发现CPU(%s %d)", ip.c_str(), port );
	else m_log.Info( "Run", "发现外存条(%s %d)", ip.c_str(), port );

}

//连接断开响应
void NoDB::OnCloseConnect(mdk::STNetHost &host)
{
	mdk::STNetHost emptyHost;
	std::string ip;
	int port;
	if ( host.IsServer() )
	{
		host.GetServerAddress( ip, port );
		
		//没插紧主板
		if ( ip == m_motherBoard.wanIP && port == (int)m_motherBoard.wanPort )
		{
			m_log.Info( "Error", "插槽松动" );
			m_motherBoard.host = emptyHost;
			m_motherBoard.status = Device::Status::unknow;
			return;
		}

		m_log.Info( "Waring", "已断开未知设备(%s %d)", ip.c_str(), port );

		return;
	}

	host.GetAddress( ip, port );
	if ( Device::Type::exist == m_context.device.type ) m_log.Info( "Run", "CPU(%s %d)丢失", ip.c_str(), port );
	else m_log.Info( "Run", "外存条(%s %d)丢失", ip.c_str(), port );

	return;
}

void NoDB::OnInvalidMsg(mdk::STNetHost &host, ErrorType type, unsigned char *msg, unsigned short len)
{
	std::string ip;
	int port;
	if ( host.IsServer() ) host.GetServerAddress( ip, port );
	else host.GetAddress( ip, port );
	m_log.StreamInfo( "Error", msg, len, "主机(%s-%d)发送非法信号", ip.c_str(), port );
	host.Close();
}

//数据到达响应
void NoDB::OnWork( mdk::STNetHost &host, MSG_HEADER *header, unsigned char *pData )
{
	if ( OnGuideMsg( host, header, pData ) ) return;
	OnClientMsg( host, header, pData );
}

//接收Guide连接上的数据并处理，如果host不是Guide则什么都不做，返回false
bool NoDB::OnGuideMsg( mdk::STNetHost &host, MSG_HEADER *header, unsigned char *pData )
{
	if ( host.ID() != m_motherBoard.host.ID() ) return false;
	
	switch( header->msgId )
	{
	case MsgId::setDeviceId :
		OnSetDeviceId( host, header, pData );
		break;
	case MsgId::devicePostion :
		OnDevicePostion( host, header, pData );
		break;
	case MsgId::runDevice :
		OnRunDevice( host, header, pData );
		break;
	default:
		break;
	}

	return true;
}

//心跳
void NoDB::Heartbeat( time_t tCurTime )
{
	static time_t tLastHeart = tCurTime;
	if ( -1 == m_motherBoard.host.ID() ) return; //未插到主板，不发送心跳
	if ( tCurTime - tLastHeart <= 60 ) return; //发送间隔未到1分钟，不发送

	SendMsg( m_motherBoard.host, MsgId::heartbeat, 0 );
	tLastHeart = tCurTime;
}

void NoDB::OnSetDeviceId( mdk::STNetHost &host, MSG_HEADER *header, unsigned char *pData )
{
	if ( header->msgSize != sizeof(MSG_SET_DEVICE_ID) ) return;
	if ( !m_cfgFile["opt"]["id"].IsNull() )
	{
		m_log.Info( "Waring", "已有设备ID:%d，拒绝修改", m_context.device.deviceId );
		return;
	}

	MSG_SET_DEVICE_ID *pParam = (MSG_SET_DEVICE_ID*)pData;
	m_cfgFile["opt"]["id"] = m_context.device.deviceId = pParam->deviceId;
	m_cfgFile.Save();
	m_log.Info( "Run", "保存设备ID:%d", m_context.device.deviceId );
}

void NoDB::OnDevicePostion( mdk::STNetHost &host, MSG_HEADER *header, unsigned char *pData )
{
	if ( Device::Status::running == m_context.device.status ) return;
	if ( header->msgSize != sizeof(MSG_DEVICE_POSTION) ) return;
	if ( m_context.device.type == Device::Type::ssd ) return;//硬盘不依赖任何设备，不接收任何设备信息

	m_log.Info( "Run", "保存%s信息", Device::Descript(Device::Type::ssd) );
	MSG_DEVICE_POSTION *pParam = (MSG_DEVICE_POSTION*)pData;

	int i = 0;
	Device::Type::Type device;
	Device::INFO *pHardDisk;
	mdk::uint64 ip64;
	bool validHD = false;
	for ( i = 0; i < pParam->size; i++ )
	{
		device = (Device::Type::Type)pParam->devices[i].device;
		if ( Device::Type::ssd != device ) continue;
		pHardDisk = new Device::INFO;
		pHardDisk->deviceId = pParam->devices[i].deviceId;
		pHardDisk->type = device;
		pHardDisk->status = Device::Status::running;
		pHardDisk->wanIP = pParam->devices[i].wanIP;
		pHardDisk->wanPort = pParam->devices[i].wanPort;
		pHardDisk->lanIP = pParam->devices[i].lanIP;
		pHardDisk->lanPort = pParam->devices[i].lanPort;

		validHD = false;
		if ( mdk::addrToI64( ip64, pHardDisk->wanIP.c_str(), pHardDisk->wanPort ) )
		{
			validHD = true;
		}
		else 
		{
			m_log.Info( "Waring", "错误的固态硬盘外网地址%s %d", pHardDisk->wanIP.c_str(), pHardDisk->wanPort );
		}
		if ( mdk::addrToI64( ip64, pHardDisk->lanIP.c_str(), pHardDisk->lanPort ) )
		{
			validHD = true;
		}
		else 
		{
			m_log.Info( "Waring", "错误的固态硬盘内网地址%s %d", pHardDisk->lanIP.c_str(), pHardDisk->lanPort );
		}
		if ( validHD )
		{
			m_hardDisks[pHardDisk->deviceId] = pHardDisk;
		}
		else 
		{
			delete pHardDisk;
		}
	}
}

void NoDB::OnRunDevice( mdk::STNetHost &host, MSG_HEADER *header, unsigned char *pData )
{
	if ( Device::Status::running == m_context.device.status ) return;
	if ( header->msgSize != 0 ) return;
	m_log.Info( "Run", "%s开始工作 listen%d", Device::Descript(m_context.device.type), m_context.device.lanPort );
	if ( !Listen( m_context.device.lanPort ) )
	{
		m_log.Info( "Run", "监听端口%d失败", m_context.device.lanPort );
	}
	m_context.device.status = Device::Status::running;
	m_log.Info( "Run", "开始工作" );
}

bool NoDB::OnClientMsg( mdk::STNetHost &host, MSG_HEADER *header, unsigned char *pData )
{
/*
	{
		static mdk::uint64 count = 0;
		static mdk::uint64 start = 0;
		static mdk::uint64 use = 0;
		if ( 0 == count ) 
		{
			start = GetTickCount();
		}
		count++;
		if ( 0 == count % 1000 ) 
		{
			use = GetTickCount() - start;
			m_log.Info( "统计", "处理性能：%llu次操作，耗时%llums 平均%fms/请求 ", 
				count, use, use * 1.0 / count );
		}
	}
*/
	switch ( header->msgId )
	{
	case MsgId::deleteData://MSG_HEADER+数据pData(DATA_KEY+path)
		OnDeleteData((exist::DATA_KEY*)pData, 
			header->msgSize == sizeof(exist::DATA_KEY)?NULL:&pData[sizeof(exist::DATA_KEY)], 
			header->msgSize-sizeof(exist::DATA_KEY));
		break;
	case MsgId::writeData://MSG_HEADER+数据pData(WRITE_DATA+path+data)
		{
			WRITE_DATA *pParam = (WRITE_DATA*)pData;
			int pathSize = header->msgSize - sizeof(WRITE_DATA) - pParam->size;
			unsigned char *pValue = &pData[sizeof(WRITE_DATA) + pathSize];
			unsigned char *path = NULL;
			if ( 0 < pathSize ) path = &pData[sizeof(WRITE_DATA)];
			OnWriteData(pParam, pValue, path, pathSize);
		}
		break;
	case MsgId::readData://MSG_HEADER+数据pData(DATA_KEY+path)
		OnReadData(host, (READ_DATA*)pData, 
			header->msgSize == sizeof(READ_DATA)?NULL:&pData[sizeof(READ_DATA)], 
			header->msgSize-sizeof(READ_DATA));
		break;
	default:
		break;
	}

	return true;
}

void NoDB::AddSelf( exist::VALUE *pValue, unsigned char *pData )
{
	if ( DataType::int8 == pValue->key.type )
	{
		mdk::int8 *vl, *vr;
		vl = (mdk::int8*)pValue->pData;
		vr = (mdk::int8*)pData;
		*vl += *vr;
	}
	if ( DataType::uInt8 == pValue->key.type )
	{
		mdk::uint8 *vl, *vr;
		vl = (mdk::uint8*)pValue->pData;
		vr = (mdk::uint8*)pData;
		*vl += *vr;
	}
	if ( DataType::int16 == pValue->key.type )
	{
		mdk::int16 *vl, *vr;
		vl = (mdk::int16*)pValue->pData;
		vr = (mdk::int16*)pData;
		*vl += *vr;
	}
	if ( DataType::uInt16 == pValue->key.type )
	{
		mdk::uint16 *vl, *vr;
		vl = (mdk::uint16*)pValue->pData;
		vr = (mdk::uint16*)pData;
		*vl += *vr;
	}
	if ( DataType::int32 == pValue->key.type )
	{
		mdk::int32 *vl, *vr;
		vl = (mdk::int32*)pValue->pData;
		vr = (mdk::int32*)pData;
		*vl += *vr;
	}
	if ( DataType::uInt32 == pValue->key.type )
	{
		mdk::uint32 *vl, *vr;
		vl = (mdk::uint32*)pValue->pData;
		vr = (mdk::uint32*)pData;
		*vl += *vr;
	}
	if ( DataType::int64 == pValue->key.type )
	{
		mdk::int64 *vl, *vr;
		vl = (mdk::int64*)pValue->pData;
		vr = (mdk::int64*)pData;
		*vl += *vr;
	}
	if ( DataType::uInt64 == pValue->key.type )
	{
		mdk::uint64 *vl, *vr;
		vl = (mdk::uint64*)pValue->pData;
		vr = (mdk::uint64*)pData;
		*vl += *vr;
	}
	if ( DataType::sFloat == pValue->key.type )
	{
		float *vl, *vr;
		vl = (float*)pValue->pData;
		vr = (float*)pData;
		*vl += *vr;
	}
	if ( DataType::sDouble == pValue->key.type )
	{
		double *vl, *vr;
		vl = (double*)pValue->pData;
		vr = (double*)pData;
		*vl += *vr;
	}
}

void NoDB::SubtractSelf( exist::VALUE *pValue, unsigned char *pData )
{
	if ( DataType::int8 == pValue->key.type )
	{
		mdk::int8 *vl, *vr;
		vl = (mdk::int8*)pValue->pData;
		vr = (mdk::int8*)pData;
		*vl -= *vr;
	}
	if ( DataType::uInt8 == pValue->key.type )
	{
		mdk::uint8 *vl, *vr;
		vl = (mdk::uint8*)pValue->pData;
		vr = (mdk::uint8*)pData;
		*vl -= *vr;
	}
	if ( DataType::int16 == pValue->key.type )
	{
		mdk::int16 *vl, *vr;
		vl = (mdk::int16*)pValue->pData;
		vr = (mdk::int16*)pData;
		*vl -= *vr;
	}
	if ( DataType::uInt16 == pValue->key.type )
	{
		mdk::uint16 *vl, *vr;
		vl = (mdk::uint16*)pValue->pData;
		vr = (mdk::uint16*)pData;
		*vl -= *vr;
	}
	if ( DataType::int32 == pValue->key.type )
	{
		mdk::int32 *vl, *vr;
		vl = (mdk::int32*)pValue->pData;
		vr = (mdk::int32*)pData;
		*vl -= *vr;
	}
	if ( DataType::uInt32 == pValue->key.type )
	{
		mdk::uint32 *vl, *vr;
		vl = (mdk::uint32*)pValue->pData;
		vr = (mdk::uint32*)pData;
		*vl -= *vr;
	}
	if ( DataType::int64 == pValue->key.type )
	{
		mdk::int64 *vl, *vr;
		vl = (mdk::int64*)pValue->pData;
		vr = (mdk::int64*)pData;
		*vl -= *vr;
	}
	if ( DataType::uInt64 == pValue->key.type )
	{
		mdk::uint64 *vl, *vr;
		vl = (mdk::uint64*)pValue->pData;
		vr = (mdk::uint64*)pData;
		*vl -= *vr;
	}
	if ( DataType::sFloat == pValue->key.type )
	{
		float *vl, *vr;
		vl = (float*)pValue->pData;
		vr = (float*)pData;
		*vl -= *vr;
	}
	if ( DataType::sDouble == pValue->key.type )
	{
		double *vl, *vr;
		vl = (double*)pValue->pData;
		vr = (double*)pData;
		*vl -= *vr;
	}
}

void NoDB::MultiplySelf( exist::VALUE *pValue, unsigned char *pData )
{
	if ( DataType::int8 == pValue->key.type )
	{
		mdk::int8 *vl, *vr;
		vl = (mdk::int8*)pValue->pData;
		vr = (mdk::int8*)pData;
		*vl *= *vr;
	}
	if ( DataType::uInt8 == pValue->key.type )
	{
		mdk::uint8 *vl, *vr;
		vl = (mdk::uint8*)pValue->pData;
		vr = (mdk::uint8*)pData;
		*vl *= *vr;
	}
	if ( DataType::int16 == pValue->key.type )
	{
		mdk::int16 *vl, *vr;
		vl = (mdk::int16*)pValue->pData;
		vr = (mdk::int16*)pData;
		*vl *= *vr;
	}
	if ( DataType::uInt16 == pValue->key.type )
	{
		mdk::uint16 *vl, *vr;
		vl = (mdk::uint16*)pValue->pData;
		vr = (mdk::uint16*)pData;
		*vl *= *vr;
	}
	if ( DataType::int32 == pValue->key.type )
	{
		mdk::int32 *vl, *vr;
		vl = (mdk::int32*)pValue->pData;
		vr = (mdk::int32*)pData;
		*vl *= *vr;
	}
	if ( DataType::uInt32 == pValue->key.type )
	{
		mdk::uint32 *vl, *vr;
		vl = (mdk::uint32*)pValue->pData;
		vr = (mdk::uint32*)pData;
		*vl *= *vr;
	}
	if ( DataType::int64 == pValue->key.type )
	{
		mdk::int64 *vl, *vr;
		vl = (mdk::int64*)pValue->pData;
		vr = (mdk::int64*)pData;
		*vl *= *vr;
	}
	if ( DataType::uInt64 == pValue->key.type )
	{
		mdk::uint64 *vl, *vr;
		vl = (mdk::uint64*)pValue->pData;
		vr = (mdk::uint64*)pData;
		*vl *= *vr;
	}
	if ( DataType::sFloat == pValue->key.type )
	{
		float *vl, *vr;
		vl = (float*)pValue->pData;
		vr = (float*)pData;
		*vl *= *vr;
	}
	if ( DataType::sDouble == pValue->key.type )
	{
		double *vl, *vr;
		vl = (double*)pValue->pData;
		vr = (double*)pData;
		*vl *= *vr;
	}
}

bool NoDB::DivideSelf( exist::VALUE *pValue, unsigned char *pData )
{
	if ( DataType::int8 == pValue->key.type )
	{
		mdk::int8 *vl, *vr;
		vr = (mdk::int8*)pData;
		if ( 0 == *vr ) return false;
		vl = (mdk::int8*)pValue->pData;
		*vl /= *vr;
	}
	if ( DataType::uInt8 == pValue->key.type )
	{
		mdk::uint8 *vl, *vr;
		vr = (mdk::uint8*)pData;
		if ( 0 == *vr ) return false;
		vl = (mdk::uint8*)pValue->pData;
		*vl /= *vr;
	}
	if ( DataType::int16 == pValue->key.type )
	{
		mdk::int16 *vl, *vr;
		vr = (mdk::int16*)pData;
		if ( 0 == *vr ) return false;
		vl = (mdk::int16*)pValue->pData;
		*vl /= *vr;
	}
	if ( DataType::uInt16 == pValue->key.type )
	{
		mdk::uint16 *vl, *vr;
		vr = (mdk::uint16*)pData;
		if ( 0 == *vr ) return false;
		vl = (mdk::uint16*)pValue->pData;
		*vl /= *vr;
	}
	if ( DataType::int32 == pValue->key.type )
	{
		mdk::int32 *vl, *vr;
		vr = (mdk::int32*)pData;
		if ( 0 == *vr ) return false;
		vl = (mdk::int32*)pValue->pData;
		*vl /= *vr;
	}
	if ( DataType::uInt32 == pValue->key.type )
	{
		mdk::uint32 *vl, *vr;
		vr = (mdk::uint32*)pData;
		if ( 0 == *vr ) return false;
		vl = (mdk::uint32*)pValue->pData;
		*vl /= *vr;
	}
	if ( DataType::int64 == pValue->key.type )
	{
		mdk::int64 *vl, *vr;
		vr = (mdk::int64*)pData;
		if ( 0 == *vr ) return false;
		vl = (mdk::int64*)pValue->pData;
		*vl /= *vr;
	}
	if ( DataType::uInt64 == pValue->key.type )
	{
		mdk::uint64 *vl, *vr;
		vr = (mdk::uint64*)pData;
		if ( 0 == *vr ) return false;
		vl = (mdk::uint64*)pValue->pData;
		*vl /= *vr;
	}
	if ( DataType::sFloat == pValue->key.type )
	{
		float *vl, *vr;
		vr = (float*)pData;
		if ( 0.0 == *vr ) return false;
		vl = (float*)pValue->pData;
		*vl /= *vr;
	}
	if ( DataType::sDouble == pValue->key.type )
	{
		double *vl, *vr;
		vr = (double*)pData;
		if ( 0.0 == *vr ) return false;
		vl = (double*)pValue->pData;
		*vl /= *vr;
	}

	return true;
}

RHTable* NoDB::GetNosql(bool isProtracted, DataType::DataType type, unsigned char *path, int size, exist::VALUE **pParent)
{
	if ( NULL == path )
	{
		if ( DataType::int8 == type ) return isProtracted?&m_int8DB:&m_int8Mem;
		if ( DataType::uInt8 == type ) return isProtracted?&m_uint8DB:&m_uint8Mem;
		if ( DataType::int16 == type ) return isProtracted?&m_int16DB:&m_int16Mem;
		if ( DataType::uInt16 == type ) return isProtracted?&m_uint16DB:&m_uint16Mem;
		if ( DataType::int32 == type ) return isProtracted?&m_int32DB:&m_int32Mem;
		if ( DataType::uInt32 == type ) return isProtracted?&m_uint32DB:&m_uint32Mem;
		if ( DataType::int64 == type ) return isProtracted?&m_int64DB:&m_int64Mem;
		if ( DataType::uInt64 == type ) return isProtracted?&m_uint64DB:&m_uint64Mem;
		if ( DataType::sFloat == type ) return isProtracted?&m_floatDB:&m_floatMem;
		if ( DataType::sDouble == type ) return isProtracted?&m_doubleDB:&m_doubleMem;

		return isProtracted?&m_nosqlDB:&m_nosqlMem;
	}
	if ( size > (int)(10 * sizeof(exist::DATA_KEY)) ) 
	{
		m_log.Info("Error","容器嵌套超过10层，暂不支持");
		return NULL;
	}
	return FindNosql(isProtracted, &m_nosqlDB, path, size, pParent);
}

RHTable* NoDB::FindNosql(bool isProtracted, RHTable* pNosql, unsigned char *path, int size, exist::VALUE **pParent)
{
	RHTable &nosql = isProtracted?m_nosqlDB:m_nosqlMem;
	if ( size < (int)sizeof(exist::DATA_KEY) ) return NULL;
	exist::DATA_KEY *pKey = (exist::DATA_KEY*)path;
	exist::VALUE *pValue = (exist::VALUE *)nosql.Find( (unsigned char*)pKey->key, pKey->keySize, pKey->hashid );
	if ( sizeof(exist::DATA_KEY) == size ) 
	{
		*pParent = pValue;
		return (RHTable*)pValue->pData;
	}
	return FindNosql( isProtracted, (RHTable*)pValue->pData, &path[sizeof(exist::DATA_KEY)], size - sizeof(exist::DATA_KEY), pParent );
}

exist::VALUE* NoDB::CreateData(CREATE_DATA *pParam, unsigned char *path, int size)
{
	exist::VALUE *pParent = NULL;
	RHTable *pNosql = GetNosql(pParam->protracted, (DataType::DataType)pParam->key.type, path, size, &pParent);
	if ( NULL == pNosql ) return NULL;
	exist::VALUE *pValue = (exist::VALUE*)pNosql->Find((unsigned char*)pParam->key.key, pParam->key.keySize, 
		pParam->key.hashid);
	if ( NULL != pValue ) return NULL;

	pValue = new exist::VALUE;
	if ( NULL == pValue )
	{
		m_log.Info( "Error", "内存不足，无法创建数据" );
		return NULL;
	}
	pValue->protracted = pParam->protracted;
	pValue->key = pParam->key;
	pValue->delMark = false;
	pValue->size = 0;
	pValue->pData = NULL;
	pValue->pParent = pParent;
	pValue->idxAble = false;

	if ( DataType::IsValue(pParam->key.type) )//对于数值数据类型初始化值为0
	{
		pValue->size = pParam->size;
		pValue->pData = new char[pValue->size];
		memset( pValue->pData, 0, pValue->size );
	}
	else if ( DataType::stream == pParam->key.type)//变长类型，请期待
	{
	}
	else if ( DataType::IsContainer(pParam->key.type) )//容器类型，请期待
	{
	}
	pNosql->Insert((unsigned char*)pParam->key.key, pParam->key.keySize, pValue, pParam->key.hashid);

	return pValue;
}

void NoDB::OnCreateData(CREATE_DATA *pParam, unsigned char *path, int size)
{
	exist::VALUE *pValue = CreateData(pParam, path, size);
	if ( NULL == pValue ) return;

#ifdef EXIST_DEVICE
	if ( DataType::IsValue(pParam->key.type) 
		|| DataType::stream == pParam->key.type )
	{
		ReadDataFromSSD(pValue, path, size);
	}
	else if ( DataType::IsContainer(pParam->key.type) )
	{
	}
#endif

#ifdef SSD_DEVICE
	if ( DataType::IsValue(pParam->key.type) 
		|| DataType::stream == pParam->key.type )
	{
		m_existFS.CreateData(pValue);

	}
	else if ( DataType::IsContainer(pParam->key.type) )
	{
		m_existFS.CreateTable(pParam, sizeof(CREATE_DATA), path, size);
	}
	else
	{
	}
#endif
}

void NoDB::OnDeleteData(exist::DATA_KEY *pParam, unsigned char *path, int size)
{
	//暂不支持删除功能
	return;

	exist::VALUE *pParent = NULL;
	RHTable *pNosql = GetNosql(true, (DataType::DataType)pParam->type, path, size, &pParent);
	if ( NULL == pNosql ) return;

	exist::VALUE *pValue = (exist::VALUE*)pNosql->Find((unsigned char*)pParam->key, pParam->keySize, pParam->hashid);
	if ( NULL == pValue ) return;

	if ( pValue->delMark ) return;

	pValue->delMark = true;
	if ( DataType::IsContainer(pValue->key.type) )//容器类要先清空元素
	{
		return;
	}
	if ( pValue->protracted )//持久化得要修改文件
	{

	}

	//删除数据
	pNosql->Delete((unsigned char*)pParam->key, pParam->keySize, pParam->hashid);
	if ( NULL != pValue->pData )
	{
		delete[]pValue->pData;
		pValue->pData = NULL;
	}
	delete pValue;
}

void NoDB::OnWriteData(WRITE_DATA *pParam, unsigned char* pData, unsigned char *path, int size)
{
	exist::VALUE *pParent = NULL;
	RHTable *pNosql = GetNosql(pParam->protracted, (DataType::DataType)pParam->key.type, path, size, &pParent);
	if ( NULL == pNosql ) return;

	exist::VALUE *pValue = (exist::VALUE*)pNosql->Find((unsigned char*)pParam->key.key, pParam->key.keySize, pParam->key.hashid);
	if ( NULL == pValue ) 
	{
		CREATE_DATA param;
		param.key = pParam->key;
		param.protracted = pParam->protracted;
		param.size = pParam->size;
		OnCreateData(&param, path, size);
		pValue = (exist::VALUE*)pNosql->Find((unsigned char*)pParam->key.key, pParam->key.keySize, pParam->key.hashid);
		if ( NULL == pValue )
		{
			m_log.Info( "Error", "写数据失败：无法创建数据" );
			return;
		}
	}

	if ( pParam->size == pValue->size )
	{
		if ( UpdateType::utCopy == pParam->updateType )
		{
			memcpy(pValue->pData, pData, pValue->size);
		}
		else if ( DataType::IsValue(pValue->key.type) ) 
		{
			if ( UpdateType::utAddSelf == pParam->updateType )
			{
				AddSelf(pValue, pData);
			}
			else if ( UpdateType::utSubtractSelf == pParam->updateType )
			{
				SubtractSelf(pValue, pData);
			}
			else if ( UpdateType::utMultiplySelf == pParam->updateType )
			{
				MultiplySelf(pValue, pData);
			}
			else if ( UpdateType::utDivideSelf == pParam->updateType )
			{
				DivideSelf(pValue, pData);
			}
		}
		else
		{
			m_log.Info( "Waring", "对非数值型数据申请运算操作" );
			return;
		}
	}
	else 
	{
		if ( NULL != pValue->pData )
		{
			delete[]pValue->pData;
			pValue->pData = NULL;
		}
		pValue->size = pParam->size;
		pValue->pData = new char[pValue->size];
		memcpy(pValue->pData, pData, pValue->size);
	}
	//////////////////////////////////////////////////////////////////////////
	//持久化
#ifdef EXIST_DEVICE
	SaveDataToSSD(pValue, path, size);
#endif
#ifdef SSD_DEVICE
	AddSaveWaiter(pValue, pParam);//当前设备是固态硬盘，存入队列，待达到条件合并
	SaveData(time(NULL));
#endif
}

void NoDB::OnReadData(mdk::STNetHost &host, READ_DATA *pParam, unsigned char *path, int size)
{
	exist::VALUE *pParent = NULL;
	RHTable *pNosql = GetNosql(pParam->protracted, (DataType::DataType)pParam->key.type, path, size, &pParent);
	if ( NULL == pNosql ) return;

	MSG_HEADER header;
	header.msgId = MsgId::readData;
	exist::VALUE *pValue = (exist::VALUE*)pNosql->Find((unsigned char*)pParam->key.key, pParam->key.keySize, pParam->key.hashid);
	if ( NULL == pValue ) 
	{
		CREATE_DATA param;
		param.key = pParam->key;
		param.protracted = pParam->protracted;
		param.size = pParam->initSize;
		OnCreateData(&param, path, size);
		pValue = (exist::VALUE*)pNosql->Find((unsigned char*)pParam->key.key, pParam->key.keySize, pParam->key.hashid);
		if ( NULL == pValue )
		{
			m_log.Info( "Error", "读数据失败：无法创建数据" );
		}
	}
	if ( NULL == pValue || 0 >= pValue->size ) 
	{
		header.msgSize = 0;
		host.Send((unsigned char*)&header, sizeof(MSG_HEADER));
		return;
	}
	header.msgSize = pValue->size;
	host.Send((unsigned char*)&header, sizeof(MSG_HEADER));
	host.Send((unsigned char*)pValue->pData, header.msgSize);
}

mdk::Socket* NoDB::GetSSDConnect( int keyHashId )
{
	if ( 0 >= m_hardDisks.size() ) 
	{
		m_log.Info( "Error", "固态硬盘未配置" );
		return NULL;
	}

	int deviceId = keyHashId%m_hardDisks.size();
	mdk::Socket *pSSD = &m_hardConnects[deviceId];
	if ( !pSSD->IsClosed() ) return pSSD;

	pSSD->Init( mdk::Socket::tcp );
	Device::INFO *pAdr = m_hardDisks[deviceId];
	std::string ip;
	int port;
	if ( pAdr->wanIP == m_context.device.wanIP && pAdr->lanIP == m_context.device.lanIP )
	{
		ip = pAdr->lanIP;
		port = pAdr->lanPort;
	}
	else
	{
		ip = pAdr->wanIP;
		port = pAdr->lanPort;
	}

	if ( !pSSD->Connect( ip.c_str(), port ) )
	{
		m_log.Info( "Error", "硬盘(%s %d - %s %d)接触不良", 
			pAdr->wanIP.c_str(), pAdr->wanPort, pAdr->lanIP.c_str(), pAdr->lanPort );
		return NULL;
	}
	pSSD->SetNoDelay(true);

	return pSSD;
}

void NoDB::ReadDataFromSSD( exist::VALUE *pValue, unsigned char *path, int size )
{
	if ( !pValue->protracted ) return;

	mdk::Socket *pSSD = GetSSDConnect(pValue->key.hashid);
	if ( NULL == pSSD ) return;

	char msg[1024+(sizeof(exist::DATA_KEY)*10)];
	MSG_HEADER *pHeader = (MSG_HEADER*)msg;
	if ( DataType::IsContainer(pValue->key.type) ) return;//容器类数据存储，请期待

	{//查询数据:MSG_HEADER+READ_DATA+path
		READ_DATA *pParam = (READ_DATA*)&msg[sizeof(MSG_HEADER)];
		pHeader->msgId = MsgId::readData;
		pHeader->msgSize = sizeof(READ_DATA);
		pParam->key = pValue->key;
		pParam->protracted = true;
		pParam->initSize = pValue->size;
		pHeader->msgSize += size;
		memcpy(&msg[sizeof(MSG_HEADER)+sizeof(READ_DATA)], path, size);
		pSSD->Send(msg, sizeof(MSG_HEADER) + pHeader->msgSize);

		if ( 0 > pSSD->Receive(msg, sizeof(MSG_HEADER)) )
		{
			pSSD->Close();
			m_log.Info( "Error", "固态硬盘接触不良，无法读取持久化数据" );
			return;
		}
		if ( pHeader->msgSize <= 0 ) return;
		if ( DataType::stream == pValue->key.type )
		{
			pValue->size = pHeader->msgSize;
			pValue->pData = new char[pValue->size];
		}
		if ( pValue->size != pHeader->msgSize )
		{
			m_log.Info( "Error", "数据类型不匹配，无法读取持久化数据" );
			return;
		}
		if ( 0 > pSSD->Receive(pValue->pData, pValue->size) )
		{
			pSSD->Close();
			m_log.Info( "Error", "固态硬盘接触不良，无法读取持久化数据" );
			return;
		}
	}
}

void NoDB::SaveDataToSSD( exist::VALUE *pValue, unsigned char *path, int size )
{
	if ( !pValue->protracted ) return;

	mdk::Socket *pSSD = GetSSDConnect(pValue->key.hashid);
	if ( NULL == pSSD ) return;

	char msg[1024+(+sizeof(exist::DATA_KEY)*10)];
	MSG_HEADER *pHeader = (MSG_HEADER*)msg;
	if ( DataType::IsContainer(pValue->key.type) ) return;//容器类数据存储，请期待

	//写数据:MSG_HEADER+WRITE_DATA+path+data
	WRITE_DATA *pData = (WRITE_DATA*)&msg[sizeof(MSG_HEADER)];
	pHeader->msgId = MsgId::writeData;
	pHeader->msgSize = sizeof(WRITE_DATA) + size + pValue->size;
	pData->key = pValue->key;
	pData->updateType = UpdateType::utCopy;
	pData->size = pValue->size;
	memcpy(&msg[sizeof(MSG_HEADER)+sizeof(WRITE_DATA)], path, size);
	if ( 0 > pSSD->Send(msg, sizeof(MSG_HEADER)+sizeof(WRITE_DATA)+size) )
	{
		pSSD->Close();
		m_log.Info( "Error", "固态硬盘接触不良，无法持久化数据" );
		return;
	}
	if ( 0 > pSSD->Send(pValue->pData, pValue->size) )
	{
		pSSD->Close();
		m_log.Info( "Error", "固态硬盘接触不良，无法持久化数据" );
		return;
	}
}

const char* NoDB::ReadRootData()
{
	exist::VALUE rootMap;
	rootMap.key.key[0] = '\0';
	rootMap.key.keySize = 0;
	rootMap.protracted = true;
	rootMap.pParent = NULL;
	const char *ret;
	std::vector<exist::VALUE*> data;
	DataType::DataType typeList[256];
	typeList[0] = DataType::stream;
	typeList[1] = DataType::sFloat;
	typeList[2] = DataType::sDouble;
	typeList[3] = DataType::int8;
	typeList[4] = DataType::uInt8;
	typeList[5] = DataType::int16;
	typeList[6] = DataType::uInt16;
	typeList[7] = DataType::int32;
	typeList[8] = DataType::uInt32;
	typeList[9] = DataType::int64;
	typeList[10] = DataType::uInt64;
	int i = 0;
	for ( i = 0; i < 11; i++ )
	{
		rootMap.key.elementType = typeList[i];
		ret = m_existFS.ReadTable(&rootMap, data);//读取全局表int8数据
		if ( NULL != ret )
		{
			m_log.Info( "Error", "加载数据错误:%s", ret );
			mdk::mdk_assert(false);
			return ret;
		}
		unsigned int i = 0;
		exist::VALUE *pValue = NULL;
		exist::VALUE *pParent = NULL;
		RHTable *pNosql;
		for ( i = 0; i < data.size(); i++ )
		{
			pValue = data[i];
			pValue->pParent = NULL;
			pNosql = GetNosql(pValue->protracted, (DataType::DataType)pValue->key.type, NULL, 0, &pParent);
			pNosql->Insert((unsigned char*)pValue->key.key, pValue->key.keySize, pValue, pValue->key.hashid);
		}
		data.clear();
	}

	return NULL;
}
/*
	从硬盘读取数据
	单机Exist或分布式Exist的数据库结点，调用
*/
void NoDB::ReadData()
{
	m_log.Info( "Run", "正在加载数据..." );

	const char *ret = ReadRootData();
	if ( NULL != ret )
	{
		m_log.Info( "Error", "加载数据错误:%s", ret );
		mdk::mdk_assert(false);
		return;
	}
	std::vector<exist::VALUE*> data;
 	exist::VALUE *pValue = NULL;
 	exist::VALUE *pTable = NULL;
	unsigned int i = 0;
	RHTable *pNosql;
	//读取容器表数据
	ret = m_existFS.MoveFristTable();//移动到第一张表
	if ( NULL != ret )
	{
		m_log.Info( "Error", "加载数据错误:%s", ret );
		mdk::mdk_assert(false);
		return;
	}

	unsigned char createData[1024];
	short size;
	while ( true )
	{
		ret = m_existFS.GetTable( createData, size );//取当前表参数
		if ( 0 == size ) break;

		if ( NULL != ret )
		{
			m_log.Info( "Error", "加载数据错误:%s", ret );
			mdk::mdk_assert(false);
			return;
		}
		CREATE_DATA *pParam = (CREATE_DATA*)createData;
		unsigned char *path = NULL;
		size -= sizeof(CREATE_DATA);
		if ( 0 < size ) path = &createData[sizeof(CREATE_DATA)];
		pTable = CreateData(pParam, path, size);
		if ( NULL == pTable ) 
		{
			m_log.Info( "Waring", "创建重复的容器" );
			continue;
		}

		if ( DataType::IsValue(pTable->key.elementType) 
			|| DataType::stream > pTable->key.elementType ) //元素是数据类型，读取数据
		{
			data.clear();
			ret = m_existFS.ReadTable(pTable, data);//读取表数据
			if ( NULL != ret )
			{
				m_log.Info( "Error", "加载数据错误:%s", ret );
				mdk::mdk_assert(false);
				return;
			}
			pNosql = (RHTable*)pTable->pData;
			for ( i = 0; i < data.size(); i++ )
			{
				pValue = data[i];
				pValue->pParent = pTable;
				pNosql->Insert((unsigned char*)pValue->key.key, pValue->key.keySize, pValue, pValue->key.hashid);
			}
		}
	}
	m_log.Info( "Run", "加载完成" );

	return;
}

void NoDB::AddSaveWaiter(exist::VALUE *pValue, WRITE_DATA *pParam)
{
	if ( !pValue->protracted ) return;//数据不需要持久化

	if ( m_waitSaveDatas.end() != m_waitSaveDatas.find(pValue) ) return;//已经存在于队列中
	//写入队列，稍后合并写入
	/*
		pValue指向的内存只要被创建就不会被释放
		数据更新只是替换pValue->pData
		数据删除，只是标记，下次重起服务时不读入
	*/
	m_waitSaveDatas[pValue] = pValue;
}

void NoDB::ReadDataFromFile( exist::VALUE *pValue )
{
	if ( !pValue->protracted ) return;
//	m_existFS.ReadValue(pValue);
}

//数据持久化
void NoDB::SaveData( time_t tCurTime )
{
	static time_t tLastSave = tCurTime;
	if ( m_maxCachedCount > (int)m_waitSaveDatas.size() 
		&& m_maxCachedTime > tCurTime - tLastSave ) return;
	std::map<exist::VALUE*, exist::VALUE*>::iterator it = m_waitSaveDatas.begin();//待写入数据，5S/1000条更新合并写入
	exist::VALUE *pValue;
	bool delMark = false;
	const char *ret = NULL;
	for ( ; it != m_waitSaveDatas.end(); it++ )
	{
		delMark = false;
		pValue = it->second;
		while ( NULL != pValue ) 
		{
			if ( pValue->delMark ) 
			{
				delMark = true;
				break;
			}
			pValue = pValue->pParent;
		}
		if ( delMark ) continue;
		ret = m_existFS.WriteValue(it->second);
		if ( NULL != ret )
		{
			m_log.Info( "Error", "写数据失败：%s", ret );
		}
	}
	m_waitSaveDatas.clear();
	tLastSave = tCurTime;
}
