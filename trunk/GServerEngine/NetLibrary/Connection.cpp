#include "StdAfx.h"
#include "Connection.h"
#include <iostream>

CConnection::~CConnection(void)
{	}


void  CConnection::Init( SOCKADDR_IN * addr , SOCKET sock )
{
	memset( m_ip , 0 , 16 );
	strcpy_s( m_ip , inet_ntoa(addr->sin_addr) );
	InitSocket( addr , sock );

	m_DeleyCount	= 0;
}

char * CConnection::GetSamplePacket(long RestBytes,long* PacketSize,long* ErrorCode)
{
	if(RestBytes >= PACK_HEADER_LEN )																		
	{	
		CPacket* pPacket = (CPacket*)GetPacket();		

		/// 如果数据包过大
		if ( pPacket->GetSize() > BUFFER_SIZE )
		{
			*ErrorCode = PACKET_SIZE_ERROR;
			return NULL;
		}

		/// 验证是否是完整的数据包
		if(RestBytes >= pPacket->GetSize())														
		{
			m_ReadIndex += pPacket->GetSize();
			*PacketSize =  pPacket->GetSize();
			return  (char*)pPacket;
		}
	}

	*ErrorCode = PACKET_SAMPLING_OK;
	return NULL;
}

void CConnection::SendPacket(CMessage* pMsg,bool bUse )
{
	if(  !QesActive()	)			return ;

	SendData( pMsg->GetPacket() , pMsg->GetLength() , bUse );
}

void CConnection::RecvPacket()
{
	RecvData();
}

void CConnection::Release()
{
	next = prev = NULL;

	ReleaseSocket();
}
