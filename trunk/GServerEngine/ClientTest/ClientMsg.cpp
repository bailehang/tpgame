#include "StdAfx.h"
#include "ClientMsg.h"

long  CClientMsg::CountNum = 0; 

CClientMsg::CClientMsg(void)
{
	Log.LogInit("NetClientLog.txt",0,CONSOLE,0);
}

CClientMsg::~CClientMsg(void)
{

}

void CClientMsg::Init()
{
	NetworkInit();
}

void CClientMsg::AccedeProcess(CConnection *pCon, CMessage *pMsg)
{
	if ( !pCon->CheckActiveSocket() )	return ;
	pMsg->SetIndex( );
	char buf[1024];
	switch( pMsg->GetPtcl() )
	{

	case	CLIENT_DISCONNECT:
			sprintf( buf ,  "ip %s  socket %d Connection lost" ,pCon->GetIP(), pCon->GetSocket()  );
			ReleaseClient( pCon );
			LOG( buf  );
			break;

	case	CLIENT_GRACEFUL_DISCONNECT:
			sprintf( buf ,  "\n ip %s  Connection lost" , pCon->GetIP() );
			ReleaseClient( pCon );
			LOG( buf  );
			break;
	case	TEST_MSG_SPEEP:
			pMsg->GetBufferFromcharArray(buf,1024);
			long lVal = pMsg->GetObjDataFromcharArray<long>();
			LOG(" buf %s : %d ", buf , lVal);

			Sleep( 5 );
			CMessage	msg(TEST_MSG_SPEEP);
			msg.AddtocharArray( "this is  test");
			msg.AddtocharArray(++CountNum);
			msg.BuildPacketHeader();
			msg.SendPlayer<CConnection>(  pCon, true );
			break;
	}
}

bool CClientMsg::ConnectCacheServer( int count , char* ip,int port)
{
	for ( int i = 0 ; i < count ; i++ )
	{
		m_CServerCon[i] = ConnectionManager.GetNewConnection() ;
		if ( m_CServerCon == NULL )
		{
			return false ;
		}
		if(Connect(m_CServerCon[i],ip,port)) 
		{
			m_CServerCon[i]->SetActive();

			CMessage	msg(TEST_MSG_SPEEP);
			msg.AddtocharArray( "this is  test");
			msg.AddtocharArray(++CountNum);
			msg.BuildPacketHeader();
			msg.SendPlayer<CConnection>( m_CServerCon[i] , true );
		}
		Sleep( 50 );
	}
	printf( " Send finished " );
	//{
// 		msg.SendPlayer( &m_CServerCon ,true );
// 		msg.SendPlayer( &m_CServerCon ,true );
// 		msg.SendPlayer( &m_CServerCon ,true );
// 		msg.SendPlayer( &m_CServerCon ,true );
// 		msg.SendPlayer( &m_CServerCon ,true );
// // 		m_CServerCon.SendData( (char*)&msg, size, true );
// 		Sleep(100);
// 		m_CServerCon.SendData( (char*)&msg, size , true );
// 		Sleep(100);
// 		m_CServerCon.SendData( (char*)&msg, size,  true );
// 		Sleep(100);
// 		m_CServerCon.SendData( (char*)&msg, size , true );
// 		Sleep(100);
// 		m_CServerCon.SendData( (char*)&msg, size, true );
// 		Sleep(100);
// 		m_CServerCon.SendData( (char*)&msg, size , true );
// 		Sleep(100);

		//printf( " Send finished " );
		// CacheServerCon.SendPacket(&p_crr);
		// CacheWorker.WriteCachePacket(&p_crr);
//	}
	//m_CServerCon.Release();
	return 0;
}

void  CClientMsg::run()
{
	for ( ;; )
	{
		PACKETBUFFER_INTERFACE	* pMsg = (PACKETBUFFER_INTERFACE*)m_MsgQueue.PopMessage();
		if ( pMsg != NULL )
		{
			AccedeProcess( pMsg->pCon , (CMessage*)pMsg->Buffer );
			ReleaseBuffer( pMsg );  
		}
	}
}