// IReceiver.cpp: implementation of the IReceiver class.
//
//////////////////////////////////////////////////////////////////////

#include "BStructSvr.h"
#include <cstdio>
#include "common.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BStructSvr::BStructSvr()
{
	m_iBuffer = new unsigned char[MAX_MSG_SIZE];
	mdk::mdk_assert( NULL != m_iBuffer );
	m_oBuffer = new unsigned char[MAX_MSG_SIZE];
	mdk::mdk_assert( NULL != m_oBuffer );
}

BStructSvr::~BStructSvr()
{
	if ( NULL != m_iBuffer )
	{
		delete[]m_iBuffer;
		m_iBuffer = NULL;
	}
	if ( NULL != m_oBuffer )
	{
		delete[]m_oBuffer;
		m_oBuffer = NULL;
	}
}

void BStructSvr::OnMsg(mdk::STNetHost &host)
{
	//////////////////////////////////////////////////////////////////////////
	//接收报文
	MSG_HEADER header;
	if ( !host.Recv((unsigned char*)&header, MSG_HEAD_SIZE, false) ) return;
	if ( header.msgSize > MAX_DATA_SIZE ) 
	{
		host.Recv( m_iBuffer, MSG_HEAD_SIZE );
		OnInvalidMsg(host, outBufSize, m_iBuffer, MSG_HEAD_SIZE);
		return;
	}
	if ( !host.Recv(m_iBuffer, header.msgSize + MSG_HEAD_SIZE) ) return;

	OnWork( host, &header, &m_iBuffer[MSG_HEAD_SIZE] );
}

void BStructSvr::OnInvalidMsg(mdk::STNetHost &host, ErrorType type, unsigned char *msg, unsigned short len)
{
	printf( "非法报文::Close()\n" );
	host.Close();
}

unsigned char* BStructSvr::GetDataBuffer()
{
	return &m_oBuffer[MSG_HEAD_SIZE];
}

void BStructSvr::SendMsg( mdk::STNetHost &host, MsgId::MsgId msgId, short size )
{
	if ( -1 == host.ID() ) return;
	MSG_HEADER *header = (MSG_HEADER *)m_oBuffer;
	header->msgId = (unsigned short)msgId;
	header->msgSize = size;
	host.Send( m_oBuffer, MSG_HEAD_SIZE + size );
}
