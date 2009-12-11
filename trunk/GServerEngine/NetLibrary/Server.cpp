#include "StdAfx.h"
#include "Server.h"
#include <MMSystem.h>

#pragma  comment(lib,"Winmm.lib")


unsigned long  CServer::_thread_func()
{
	PACKETBUFFER_INTERFACE	*pPi  = NULL;
	int						Count = 0;
	long					time  = 0;

	for ( ;; )
	{
		WaitForSingleObject( PacketManager.GetPacketEvent() , 1 );

		if(	!_check_thread_live()	)	return 0;

		time	=	timeGetTime();
		
		SendProcess( time );
		///
		///  每次只处理100个数据包
		/// 
		for ( Count = 0 ; Count < 100 ; Count ++ )
		{
			if( !(pPi = GetBuffer() ))  break;


			if (  pPi->pCon )
			{
				if( !pPi->pCon->CheckActiveSocket() || DefenceDosAttack(pPi->pCon))
					ReleaseBuffer( pPi );

				else
				{
					if ( ((CMessage*)pPi->Buffer)->GetPtcl()	==  COMMAND_CHECK_NETSTATE )
					{
						pPi->pCon->SendPacket(((CMessage*)pPi->Buffer));
						ReleaseBuffer(pPi);
					}	
					else if(((CMessage*)pPi->Buffer)->GetPtcl() == CLIENT_KEEPALIVE)
					{
						pPi->pCon->SetPacketRecvTime(time);
						ReleaseBuffer(pPi);
					} 
					else 
					{
						pPi->pCon->SetPacketRecvTime(time);
						PacketProcess(pPi);
					}
				}
			}

			else
				PacketProcess( pPi );
		}
	}
	return 0;
}

void	CServer::GetServerNameAndIP(char *host, char *ip)
{
	if ( gethostname( host , 128 ) == 0 )
	{
		PHOSTENT	hostinfo = NULL;
		if(	 (hostinfo == gethostbyname(host)) != NULL )
			strcpy(ip,inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list));
	}
}

void	CServer::Read(int MaxConnection , SOCK_TYPE Type , int SockSendBufferSize,bool bUseKeepAliveOption , bool bUseDosDefenceOption )
{
	NetworkInit();

	ConnectionManager.InitConnectionManager( MaxConnection,
		Type,
		SockSendBufferSize,
		bUseKeepAliveOption,
		KEEPALIVE_TIMER);

	PacketManager.PacketBufferAlloc();
	PacketManager.SetDosDefenceOption( bUseDosDefenceOption);

	m_Iocp.StartIOCP();

	_start_thread() ;

}


void  CServer::Start( char * ServerName , int ServerPort )
{
	char  HostName [ 128 ];
	char  Ip[20];

	m_AcceptManager.CreateAccept( ServerPort );
	GetServerNameAndIP( HostName ,Ip );

}

void CServer::Stop()
{
	m_AcceptManager.CloseAccept();
	m_Iocp.StopIOCP();

	_stop_thread();
}

bool  CServer::Connect(CConnection *pCon, char *ip, int port, long uid )
{
	if( uid < CONNECTOR_UID )	return false;

	SOCKET  ConnectionSocket  = socket( AF_INET , SOCK_STREAM , 0 );
	if ( ConnectionSocket == INVALID_SOCKET )		
		return false;

	SOCKADDR_IN		ServerAddr;
	memset( &ServerAddr , 0 , sizeof(ServerAddr) );

	ServerAddr.sin_addr.s_addr		=  inet_addr( ip );
	ServerAddr.sin_family			=  AF_INET;
	ServerAddr.sin_port				=  htons( port );

	if (  connect(ConnectionSocket,(SOCKADDR*)&ServerAddr,sizeof(ServerAddr)) < 0 )	
		return  false;

	pCon->Init( &ServerAddr,ConnectionSocket );
	pCon->SetUID( uid );

	SetCompletionPort( ConnectionSocket , pCon );

	return true;
}

bool  CServer::DisConnect(CConnection *pCon)
{
	if ( pCon  &&
		pCon->CheckActiveSocket()
		)
	{
		pCon->Release();
		pCon->SetWait();
	}

	return false;
}

PACKETBUFFER_INTERFACE	* CServer::GetBuffer()
{
	return  (PACKETBUFFER_INTERFACE*)PacketManager.ReadPacket();
}

void  CServer::ReleaseBuffer(PACKETBUFFER_INTERFACE *pPacketBuffer)
{
	PacketManager.ReleasePacket(pPacketBuffer);
}

bool  CServer::ReleaseConnection(CConnection * pCon)
{
	if ( ConnectionManager.ReleaseConnection(pCon) )
	{
		LOG("Release Client UID:%d IP:%s [Active:%d Pool:%d]",
			pCon->GetUID(),pCon->GetIP(),ConnectionManager.GetActiveConnectionCount(),ConnectionManager.GetConnectionPoolCount());

		return  true;
	}

	return false;
}


void   CServer::SendProcess( long NowTime  )
{
	CConnection * pSendCon;

	int  Count	=	ConnectionManager.GetMaxConnectionCount();

	for ( int i = 0 ;  i < Count ; i++ )
	{
		pSendCon  = ConnectionManager.GetConnection( i );

		if ( !pSendCon)
			continue;

		if ( pSendCon->CheckActiveSocket() && pSendCon->m_SaveSendBytes )
		{
			if( pSendCon->m_SendErrorCode == 0 )
				pSendCon->SaveSendData();

			/*
			 *	记录发生错误的文本日志
			 */
			else if( pSendCon->m_SendErrorCode == OVER_BUFFER_ERROR)
			{
				LOG("Send Error : %s => Over Buffer Size",pSendCon->GetIP());
				ReleaseClient(pSendCon);
			}

			else if( pSendCon->m_SendErrorCode == NETWORK_ERROR )
			{
				LOG("Send Error : %s => Send() Error",pSendCon->GetIP());
				ReleaseClient(pSendCon);
			}

			else
			{
				LOG("Send Error : %s",pSendCon->GetIP());
				ReleaseClient(pSendCon);
			}
		}
	}

}

bool  CServer::DefenceDosAttack(CConnection *pCon)
{
	if ( pCon->m_SaveCountoRecvPacket > MAX_SAVECOUNT_OF_RECVPACKET )
	{
		LOG("Defence DOS Attack : %s",pCon->GetIP());
		ReleaseClient(pCon);

		return true;
	}
	return false;
}

bool  CServer::ReleaseClient(CConnection* pCon)
{
	pCon->Release();
	return TRUE;
}

void  CServer::PacketProcess(PACKETBUFFER_INTERFACE* pPacketBuffer)
{
	CMessage *msg = (CMessage*)pPacketBuffer->Buffer;

	m_MsgQueue.PushMessage( (PACKETBUFFER*) pPacketBuffer );
}
