#include "stdafx.h"
#include "ssocket.h"

CSocket::CSocket(SOCK_TYPE Type,int Size)
{
	m_Socket	= INVALID_SOCKET;
	m_SockType	= Type;

	m_SendBufSize	= Size;
	m_SendBuffer	= new char [Size];
}

CSocket::~CSocket()
{
	ReleaseSocket();

	delete [] m_SendBuffer;
}

void CSocket::InitSocket(SOCKADDR_IN* addr, SOCKET sock)
{	
	memcpy(&m_Socket,addr,sizeof(SOCKADDR_IN));
	m_Socket = sock;

	m_ReadIndex				= 0;
	m_SaveRecvBytes			= 0;

	m_SaveSendBytes			= 0;

	m_SendErrorCode			= 0;
	m_SaveCountoRecvPacket	= 0;

	m_OverState = GOOD;
}

void CSocket::InitOverLapped(OVER_TYPE Type)
{
	if(Type==RECV_OVERLAPPED)
		ZeroMemory(&m_RecvOver,sizeof(OVERLAPPED));
	else if(Type==SEND_OVERLAPPED)
		ZeroMemory(&m_SendOver,sizeof(OVERLAPPED));
	else if(Type==BOTH_OVERLAPPED)
	{
		ZeroMemory(&m_RecvOver,sizeof(OVERLAPPED));
		ZeroMemory(&m_SendOver,sizeof(OVERLAPPED));
	}
}

void CSocket::ReleaseSocket()
{
	LINGER LingerStruct;
	LingerStruct.l_onoff=1;
	LingerStruct.l_linger=0;
	setsockopt(m_Socket,SOL_SOCKET,SO_LINGER,(char*)&LingerStruct,sizeof(LingerStruct));

	shutdown(m_Socket,SD_BOTH);
	closesocket(m_Socket);

	m_Socket	= INVALID_SOCKET;

	m_OverState	= READY;
}

void CSocket::SendData(const char* packet,int packetsize,bool bUseNegleAlgorithm)
{	
	int Result;
	int Size =  packetsize + m_SaveSendBytes;

	if(Size > m_SendBufSize)
	{ 
		m_SendErrorCode = OVER_BUFFER_ERROR;
		return; 
	}

	memcpy(m_SendBuffer+m_SaveSendBytes,packet,packetsize);

	if( !bUseNegleAlgorithm || Size >= 1024 )
	{
		InitOverLapped(SEND_OVERLAPPED);

		m_WsaSbuf.buf	= m_SendBuffer;
		m_WsaSbuf.len	= Size; 

		m_DwBytesSent		= 0;

		Result = WSASend(m_Socket,&m_WsaSbuf,1,&m_DwBytesSent,0,&m_SendOver,NULL);

		if(Result==SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError())) 
		{
			m_SendErrorCode = NETWORK_ERROR;
			return;		
		}

		m_SaveSendBytes = 0;
	}

	else m_SaveSendBytes = Size;
}

void CSocket::SaveSendData()
{
	int Result;

	InitOverLapped(SEND_OVERLAPPED);

	m_WsaSbuf.buf	= m_SendBuffer;
	m_WsaSbuf.len	= m_SaveSendBytes; 

	m_DwBytesSent		= 0;

	Result = WSASend(m_Socket,&m_WsaSbuf,1,&m_DwBytesSent,0,&m_SendOver,NULL);

	if(Result==SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError())) 
	{
		m_SendErrorCode = NETWORK_ERROR;
		return;		
	}

	m_SaveSendBytes = 0;	
}

void CSocket::RecvData()
{  
	m_ReadIndex = 0;

	m_WsaRbuf.buf = m_RecvBuffer + m_SaveRecvBytes;
	m_WsaRbuf.len = BUFFER_SIZE- m_SaveRecvBytes; 

	m_DwBytesRecv	= 0;
	m_DwFlag		= MSG_PARTIAL;

	InitOverLapped(RECV_OVERLAPPED);
	WSARecv(m_Socket,&m_WsaRbuf,1,&m_DwBytesRecv,&m_DwFlag,&m_RecvOver,NULL);
}

/// 分离数据包
void CSocket::SaveRecvBuffer(int SaveLen)
{
	memmove(m_RecvBuffer,m_RecvBuffer + m_ReadIndex, SaveLen);

	m_SaveRecvBytes = SaveLen;
	m_ReadIndex = 0;
}

long CSocket::GetPacketHead()
{
	CPacket* pPacket = (CPacket*)(m_RecvBuffer + m_ReadIndex);

	return pPacket->GetMark();
}

char* CSocket::GetPacket()
{
	char* pPacket = m_RecvBuffer + m_ReadIndex;

	return pPacket;
}

