#include "StdAfx.h"
#include "ConnectionManager.h"

CConnectionManager::CConnectionManager(void)
{
	m_MaxCount			 = 0;
	m_ActiveConnectCount = 0;
	m_ConnectionAccount  = NULL;
}

CConnectionManager::~CConnectionManager(void)
{
	m_ConnectionPool.ReleaseList();

	CConnection* pConnection = NULL;
	for(int i=0;i< m_MaxCount ;i++)
	{
		pConnection = m_ConnectionPool.GetMemoryNode(i);
		if(pConnection)
		{
			pConnection->CSocket::~CSocket();
		}
	}

	SAFE_DELETE_ARRAY ( m_ConnectionAccount );
}

bool  CConnectionManager::InitConnectionManager(int Size, SOCK_TYPE Type, int SendBufferSize, bool bUseKeepAlive, long KeepAliveTimer)
{

	m_MaxCount = Size ;
	
	if ( !m_ConnectionPool.MemoryAlloc( Size) )
		return false;

	m_ConnectionAccount	=  new BYTE[sizeof(Size)+1];

	memset( m_ConnectionAccount , 0 , sizeof(m_ConnectionAccount) );

	CConnection	* pCon	=  NULL;

	for ( int i = 0 ; i < m_MaxCount ; i++ )
	{
		pCon = m_ConnectionPool.GetMemoryNode( i );

		if ( pCon != NULL )
		{
			pCon->CSocket::CSocket( Type , SendBufferSize );

			pCon->SetUID( i+1 );
			pCon->SetWait();
			pCon->SetOverReady();
			
		}
	}
	return true;
}

CConnection	* CConnectionManager::GetNewConnection()
{
	sync::scope_guard  guard ( m_Section );

	CConnection *pCon =  m_ConnectionPool.GetNewMemoryNode();
	if ( !pCon )
	{
		return NULL;
	}


	if ( !m_ConnectionAccount[ (long)(pCon->GetUID()) ] )
	{
		pCon->SetActive();
		pCon->SetPacketRecvTime( timeGetTime() );

		m_ActiveConnectCount ++;
		
		m_ConnectionAccount[ pCon->GetUID() ]  = 1;
	}

	return pCon ;
}

bool  CConnectionManager::ReleaseConnection( CConnection* pConnection )
{
	sync::scope_guard  guard( m_Section );
	
	if( pConnection								&&
		pConnection->QesActive()				&&
		pConnection->CheckActiveSocket()		&&
		m_ConnectionAccount[pConnection->GetUID()]
		)
	{

		/*
		 *	查找对象指针，然后释放内存空间
		 */
		CConnection* pTemp = m_ConnectionPool.GetHead();
		while(pTemp)
		{
			pTemp = pTemp->next;

			if(pTemp == NULL)
				return false;
			else if ( pTemp != pConnection)
				break;
		}

		pConnection->SetWait();

		pConnection->Release();
		pConnection->SetClientInfo(NULL);

		m_ConnectionPool.ReleaseNodeMemory(pConnection);

		m_ActiveConnectCount--;

		m_ConnectionAccount[pConnection->GetUID()] = 0;

		return true;
	}

	return false;
}

CConnection * CConnectionManager::GetConnection(int index)
{
	if	(	index > m_MaxCount )
		return NULL;

	return m_ConnectionPool.GetMemoryNode(  index ) ;	
}