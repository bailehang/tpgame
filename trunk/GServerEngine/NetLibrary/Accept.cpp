#include "StdAfx.h"
#include "Accept.h"

bool  CAccept::_socket_bind( int port)
{
	if( ( m_Sockaccept = WSASocket( AF_INET , SOCK_STREAM ,0 ,NULL ,0 ,WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET )
			return	false;

	m_SockAddr.sin_family	   = AF_INET;
	m_SockAddr.sin_port		   = htons( (WORD) port );
	m_SockAddr.sin_addr.s_addr = htonl( INADDR_ANY );

	if ( ::bind( m_Sockaccept ,(SOCKADDR*)&m_SockAddr , sizeof(m_SockAddr)) == SOCKET_ERROR )
		return false;
	
	listen( m_Sockaccept , SOMAXCONN );

	return true;
}

unsigned long CAccept::_thread_func()
{
	SOCKADDR_IN		clientAddress;
	int				clientAddresslen;
	SOCKET			Client;
	CConnection*	pCon;

	clientAddresslen = sizeof(clientAddress);

	for ( ;; )
	{
		if( ( Client = WSAAccept( m_Sockaccept , (SOCKADDR*)&clientAddress ,& clientAddresslen ,NULL , 0)) == INVALID_SOCKET)
			break;

		/*
		 *	分配一个对象空间
		 */
		pCon =  ConnectionManager.GetNewConnection() ;
		if( !pCon )
		{
			closesocket( Client );
			continue;
		}

		/*
		 *	初始化数据
		 */
		pCon->Init( &clientAddress , Client );

		if ( !SetCompletionPort(Client ,pCon ) )
		{
			pCon->Release();
			continue;
		}
	
		/*
		 *	日志记录
		 */
		LOG("Connect Client UID:%d IP:%s [Active:%d Pool:%d]",
			pCon->GetUID(),pCon->GetIP(),ConnectionManager.GetActiveConnectionCount(),ConnectionManager.GetConnectionPoolCount());

	}
	return 0;
}


void  CAccept::_destroy_accept()
{
	closesocket( m_Sockaccept );
	
	_stop_thread();
}