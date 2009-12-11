#include "StdAfx.h"
#include "Network.h"

CConnectionManager			CLibObject::ConnectionManager;
CPacketManager				CLibObject::PacketManager;
CLogManager					CLibObject::Log;
HANDLE						CNetwork::hCompletionPort;

bool CNetwork::NetworkInit()
{
	WSADATA wsadata;
	return (WSAStartup(MAKEWORD(2,2),&wsadata) == 0); 
}

void CNetwork::CreateCompletionPortHandle()
{
	hCompletionPort = ::CreateIoCompletionPort( INVALID_HANDLE_VALUE , 0 , 0 , 0 );
}

bool CNetwork::SetCompletionPort(SOCKET sock,CConnection* pCon)
{
	if(::CreateIoCompletionPort(reinterpret_cast<HANDLE>(sock),GetCompletionHandle(),reinterpret_cast<ULONG_PTR>(pCon),0)==NULL) 
		return false;

	pCon->RecvPacket(); 
	return true;
}
