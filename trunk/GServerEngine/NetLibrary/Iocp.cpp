#include "StdAfx.h"
#include "iocp.h"

void  CIOCP::StartIOCP()
{
	CreateCompletionPortHandle();
	StartWorkerThread();
}

void  CIOCP::StopIOCP()
{
	DestroyWorker();
	WSACleanup();
}


void  CIOCP::StartWorkerThread()
{
	HANDLE		hThread;

	SYSTEM_INFO	sys_info;
	GetSystemInfo( &sys_info );

	m_WorkerNum  = sys_info.dwNumberOfProcessors * 2  + 2;
	if(	m_WorkerNum> MAX_WORKER_COUNT )
		m_WorkerNum = MAX_WORKER_COUNT;

	for ( int count = 0 ; count < m_WorkerNum ; count ++ )
	{
		hThread	=  (HANDLE) _beginthreadex( NULL , 0 , _work_thread_ , (LPVOID)this , 0 , NULL );
		m_hEvent[count]  =  CreateEvent( NULL , FALSE , FALSE , NULL );
		CloseHandle( hThread );
	}
}

void  CIOCP::DestroyWorker()
{
	m_HandleIndex = 0;
	
	for(  int i = 0 ; i < m_WorkerNum ; i++ )
		PostQueuedCompletionStatus( GetCompletionHandle() , 0 , WORKER_DESTROY_KEY , NULL );

	WaitForMultipleObjects( m_WorkerNum , m_hEvent , TRUE , INFINITE );
	
	for(  int i = 0 ; i < m_WorkerNum ; i++ )
		CloseHandle( m_hEvent[i] );

	return ;
}

void  CIOCP::SetWorkerEvent()
{
	sync::scope_guard  sguid( m_Scection ) ;

	SetEvent( m_hEvent[m_HandleIndex] );

	m_HandleIndex ++;
}

void  CIOCP::ConnectionError(CConnection* pCon)
{
	sync::scope_guard  sguid( m_Scection ) ;

	if (pCon					 &&
		pCon->QesActive()		 &&
		pCon->CheckActiveSocket()&&
		pCon->GetOverState() == GOOD
		)
	{
		CMessage  msg(CLIENT_DISCONNECT);
		msg.BuildPacketHeader();
		PacketManager.WritePacket( msg.GetPacket(), msg.GetLength() , pCon );
		PacketManager.SetPacketEvent();
		
		pCon->SetOverBad() ;
	}
}

unsigned int __stdcall WINAPI CIOCP::_work_thread_(LPVOID pVoid)
{
	CIOCP* This = (CIOCP*)pVoid;

	CConnection*	pCon;
	LPOVERLAPPED	lpOverlapped;
	DWORD			dwTransferred;	

	long			RestBytes;

	char*			pPacket;
	long			PacketSize;

	long			ErrorCode = 0;

	while(TRUE)
	{
WORKER:
			if(GetQueuedCompletionStatus(This->GetCompletionHandle(),&dwTransferred,(DWORD*)&pCon,&lpOverlapped,INFINITE) ==0)
			{
				if( lpOverlapped != NULL )
				{
					This->ConnectionError( pCon );
				}
				else
					LOG("GetQueuedCompletionStatus Error! : code %d",GetLastError());

				continue;
			}

			if ( ((long)pCon) == WORKER_DESTROY_KEY )
			{
				This->SetWorkerEvent() ;
				return 0;
			}

			if ( pCon == NULL || lpOverlapped == NULL )
			{
				This->ConnectionError( pCon );
				continue ;
			}

			/// Disconnection
			if ( dwTransferred == 0)
			{
				This->ConnectionError( pCon );
				continue ;
			}

			if (  pCon->GetOverState() == GOOD	&& lpOverlapped == pCon->GetRecvOverLapped() )
			{
				RestBytes = (int)dwTransferred + pCon->m_SaveRecvBytes;
				// 组包,可能一个条消息包括几条数据包
				for ( ;; )
				{
					/// 是否得到一个数据包
					pPacket = pCon->GetSamplePacket( RestBytes,&PacketSize,&ErrorCode);
					if ( pPacket == NULL )
					{
						if ( ErrorCode )
						{
							if(ErrorCode == PACKET_SAMPLING_OK)				LOG("Packet Error. IP:%s",pCon->GetIP());
		
							ErrorCode = 0 ;
							This->ConnectionError( pCon );
							goto WORKER;
						}

						pCon->SaveRecvBuffer( RestBytes );
						break; 
					}
					if(!This->PacketManager.WritePacket(pPacket,PacketSize,pCon))
					{
						// 写包失败，然后关闭套接字
						pCon->m_ReadIndex -= PacketSize;
						pCon->SaveRecvBuffer(RestBytes);

						LOG("Fail WritePacket. IP:%s",pCon->GetIP());

						ErrorCode = 0;
						This->ConnectionError(pCon);
						goto WORKER;
					}

					if((RestBytes -= PacketSize) <= 0) 
					{
						pCon->m_SaveRecvBytes = 0; 
						break;
					}
				}
				This->PacketManager.SetPacketEvent();
				pCon->RecvPacket() ;
			}
	}

	return 0;
}