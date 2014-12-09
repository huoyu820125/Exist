// BStructSvr.h: interface for the BStructSvr class.
//
//////////////////////////////////////////////////////////////////////

#ifndef BSTRUCTSVR_H
#define BSTRUCTSVR_H

#include "frame/netserver/STNetServer.h"
#include "mdk/mapi.h"
#include "Protocol.h"

class BStructSvr : public mdk::STNetServer
{
protected:
	enum ErrorType
	{
		outBufSize = 0,		//报文超出最大长度
		resolveUnable = 1,	//无法解析成BStruct
		noMsgId = 2,		//缺少参数MsgId
		paramError = 3,		//参数错误：缺少参数/参数类型不正确
	};

public:
	BStructSvr();
	virtual ~BStructSvr();

	virtual void OnMsg(mdk::STNetHost &host);//接收报文，成功触发OnWork()，失败触发OnInvalidMsg()
	virtual void OnWork( mdk::STNetHost &host, MSG_HEADER *header, unsigned char *pData ) = 0;//报文响应
	virtual void OnInvalidMsg(mdk::STNetHost &host, ErrorType type, unsigned char *msg, unsigned short len);//非法报文
	unsigned char* GetDataBuffer();//取得发送报文流数据收地址
	void SendMsg( mdk::STNetHost &host, MsgId::MsgId msgId, short size );//将构造好的报文发送出去

private:
	unsigned char *m_iBuffer;//接收报文缓冲
	unsigned char *m_oBuffer;//发送报文缓冲
};

#endif // #ifndef BSTRUCTSVR_H
