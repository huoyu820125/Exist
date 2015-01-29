#include <cstdio>
#include "TCPWorker.h"
#include "mdk/mapi.h"

#ifdef WIN32
#ifdef _DEBUG
#pragma comment ( lib, "../Micro-Development-Kit/lib/mdk_d.lib" )
#else
#pragma comment ( lib, "../Micro-Development-Kit/lib/mdk.lib" )
#endif
#endif

//状态服务器
int main( int argc, char** argv )
{
	char exeDir[256];
	int size = 256;
	mdk::GetExeDir( exeDir, size );//取得可执行程序位置
	char configFile[256];
	sprintf( configFile, "%s/../conf/Mother.cfg", exeDir );
	
	TCPWorker ser( configFile );
	ser.GetLog().Info( "info:", "读取配置%s", configFile );
	const char *ret = ser.Start();
	if ( NULL != ret )
	{
		ser.GetLog().Info( "info:","激活%s失败:%s", Device::Descript(Device::Type::motherboard), ret );
		return 0;
	}
	ser.GetLog().Info("info:", "%s开始工作", Device::Descript(Device::Type::motherboard) );
	ser.WaitStop();
	ser.GetLog().Info("info:", "%s已关闭电源", Device::Descript(Device::Type::motherboard) );

	return 0;
}
