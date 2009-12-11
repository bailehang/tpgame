#include "StdAfx.h"
#include "ServerMsg.h"

CServerMsg::CServerMsg(void)
{
	Log.LogInit("NetServerLog.txt",LOGFILE,0,OUTPUT);
}

CServerMsg::~CServerMsg(void)
{

}


void CServerMsg::AccedeProcess(CConnection *pCon, CMessage *pMsg)
{
	if ( !pCon->CheckActiveSocket() )	return ;
	char buf[1024]; 

	pMsg->SetIndex( );
	switch( pMsg->GetPtcl() )
	{
	case	CLIENT_DISCONNECT:
			sprintf( buf ,  "ip %s  socket %d Connection lost" ,pCon->GetIP(), pCon->GetSocket()  );
			ReleaseClient( pCon );
			LOG( buf  );
			break;
	case	CLIENT_GRACEFUL_DISCONNECT:
			ReleaseClient( pCon );
			LOG(" ReleaseConnection" );
			break;
	case	TEST_MSG_SPEEP:
			pMsg->GetBufferFromcharArray(buf,1024);
			long lVal = pMsg->GetObjDataFromcharArray<long>();
			LOG(" buf %s : %d ", buf , lVal);

			Sleep( 5 );
			CMessage	msg(TEST_MSG_SPEEP);
			msg.AddtocharArray( "this is  test");
			msg.AddtocharArray(lVal);
			msg.BuildPacketHeader();
			msg.SendPlayer<CConnection>( pCon , true );
			break;
	}
}

bool  CServerMsg::ConnectCacheServer(char* ip,int port)
{
	if(Connect(&m_pCon,ip,port)) 
	{
		m_pCon.SetActive();
	}
	return TRUE;
}

void  CServerMsg::run()
{
	for ( ;; )
	{
		PACKETBUFFER_INTERFACE	* pMsg = (PACKETBUFFER_INTERFACE*)m_MsgQueue.PopMessage();
		if ( pMsg != NULL )
		{
			AccedeProcess( pMsg->pCon , (CMessage*)pMsg->Buffer );
			
			/*
			 *	 Õ∑≈œ˚œ¢
			 */
			ReleaseBuffer( pMsg );  
		}
	}
}