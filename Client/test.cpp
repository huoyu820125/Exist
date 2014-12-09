// test.cpp : Defines the entry point for the console application.
//

#include <string>
#include "../include/Exist/Exist.h"

#include <stdio.h>
#ifdef WIN32
#ifdef _DEBUG
#pragma comment ( lib, "../lib/cpu_d.lib" )
#pragma comment ( lib, "mdk_d.lib" )
#else
#pragma comment ( lib, "../lib/cpu.lib" )
#pragma comment ( lib, "mdk.lib" )
#endif
#endif

#ifdef WIN32
#include "windows.h"
#else
mdk::uint64 GetTickCount();
#endif

#include "mdk/Socket.h"
#include <sstream>

#include <algorithm>
#include <iostream>
#include <ctime>

#pragma pack(1)
typedef struct MSG_HEADER
{
	unsigned short		msgId;				//报文ID
	unsigned short		msgSize;			//报文长度
}MSG_HEADER;
#pragma pack()

int main(int argc, char* argv[])
{
	Exist::PlugIn();
	{//测试10万次写入+30万次查询

		int i = 0;
		char key[256];
		float value;
		mdk::uint64 clt = GetTickCount();
		mdk::uint64 opt = GetTickCount();
		mdk::uint64 start = GetTickCount();
		unsigned char pData[256];
		int size;
		char pValue[256];
		for ( i = 0; i < 100000; i++ )
		{
			sprintf( key, "%d", i );
			sprintf( pValue, "%de", i );
			SSD::Stream data(key);
			size = 256;
			data.GetData(pData, size);//第1条读请求
			if ( size != strlen(pValue) || 0 != memcmp( pData, pValue, size ) )
			{
				/*
					上次写入的数据，读出来应该相同，第一次运行测试程序时，
					因为数据尚未存入Exist,所以不匹配是正常
				*/
				printf( "初始化值与以前写入值不=\n" );
			}
			sprintf( pValue, "%de", i );
			data.SetData( pValue, strlen(pValue) );//第1条写请求
			size = 256;
			data.GetData(pData, size);//第2条读请求
			if ( 0 != memcmp( pData, pValue, size ) )
			{
				printf( "读出与写入不=\n" );
			}
		}
		mdk::uint64 useTime = GetTickCount() - start;
		printf( "10万次写入+20万次查询耗时：%llu毫秒，平均%d条请求/秒\n", useTime, (100000+200000)*1000/useTime );
	}

	{//测试50万次写入+70万次查询
		int i = 0;
		char key[256];
		float value;
		mdk::uint64 start = GetTickCount();
		for ( i = 0; i < 100000; i++ )
		{
			sprintf( key, "%d", i );
			SSD::Float data(key);
			float checkData = data;//第1条读请求
			value = i + 0.5;
			value += 2.5;
			value *= 2.5;
			value -= 2.5;
			value /= 2.5;
			if ( value != checkData )
			{
				/*
					上次写入的数据，读出来应该相同，第一次运行测试程序时，
					因为数据尚未存入Exist,所以不匹配是正常
				*/
				printf( "初始化值与以前写入值不=\n" );
			}
			data = i + 0.5;//第1条写请求
			value = i + 0.5;
			checkData = data;//第2条读请求
			if ( value != checkData )
			{
				printf( "读出与写入不=\n" );
			}
			data += 2.5;//第2条写请求
 			value += 2.5;
			checkData = data;//第3条读请求
			if ( value != checkData )
			{
				printf( "读出与+运算不=\n" );
			}
			data *= 2.5;//第3条写请求
			value *= 2.5;
			checkData = data;//第4条读请求
			if ( value != checkData )
			{
				printf( "读出与*运算不=\n" );
			}
			data -= 2.5;//第4条写请求
			value -= 2.5;
			checkData = data;//第5条读请求
			if ( value != checkData )
			{
				printf( "读出与-运算不=\n" );
			}
			data /= 2.5;//第5条写请求
			value /= 2.5;
			checkData = data;//第6条读请求
		}
		mdk::uint64 useTime = GetTickCount() - start;
		printf( "50万次写入+60万次查询耗时：%llu毫秒，平均%d条请求/秒\n", useTime, (600000+500000)*1000/useTime );
	}

	printf( "测试程序完成，退出测试\n" );
	return 0;
}

