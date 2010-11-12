#include "stdafx.h"
#include "SendMailApp.h"
#include "SendToMail.h"
#include "SockSendMail.h"
#include "Thread.h"
#include "../Public/BaseDef.h"
#include "../Public/Singleton.h"
#include "../Public/STLFun.h"
#include "../Public/GameSetup.h"

tp_ipc_peer_namespace::ctpool*  g_CurrThread = NULL;
SendMailApp::SendMailApp()
{
	m_RasDial = new MyRasDial("test","test","宽带连接");
}

SendMailApp::~SendMailApp()
{
	SAFE_DELETE(m_RasDial);
	SAFE_DELETE(m_TPool);
}

bool  SendMailApp::IsSendMail()
{
	return m_TPool->IsExit();
}


void  SendMailApp::Start()
{
	if ( 1 /*m_RasDial && m_RasDial->Listen()*/ )
	{
		
		m_TPool = new tp_ipc_peer_namespace::ctpool( );
		g_CurrThread = m_TPool;
		InitPool( m_TPool , GetInstObj(DestList).m_SendListOne , 1 );
		InitPool( m_TPool , GetInstObj(DestList).m_SendListTwo , 2 );

		m_TPool->Start( 1 );

		/// 发送邮件
		for ( ;  !m_TPool->IsExit() ; )
		{
 			if( m_TPool->IsRas() )
			{
				//m_RasDial->Stop();
				//m_RasDial->Listen();
				Sleep( 1000 );
				m_TPool->RasStates( false );
			}
		}

		SAFE_DELETE(m_TPool);
		SAFE_DELETE(g_CurrThread);
		//	m_RasDial->Stop();
	}


}

void  SendMailApp::InitPool(tp_ipc_peer_namespace::ctpool* pool,std::list<std::string>& RsSendList,long  index)
{
	//std::list<std::string>& RsSendList = GetInstObj(DestList).m_SendListOne;
	
	if( pool == NULL )
		return ;

	/// 发送地址数量
	long  DestCount = RsSendList.size();
	
	long  Num1 = GetInstObj( MailLoginInfo ).Num1;
	long  Num2 = GetInstObj( MailLoginInfo ).Num2;

	/// 发送帐号信息
	long  AccountCount = (index == 1) ? Num1 : Num2 ;

	/// 每次发送的联系人数量
	const long  MTime = GetInstObj( tagGlobalSetup).SendNum ;
	/// 
	size_t  SendNum = (DestCount / MTime) + (DestCount % MTime > 0 ? 1 : 0) ;

	/// 联系人数量
	long  LinkNum = 0 ;

	std::list<std::string>::iterator BeginItr = RsSendList.begin();

	for (size_t i = 0 ; i < SendNum ; i ++  )
	{
		//SendToMail*  mail = new SendToMail( LinkNum % AccountCount );
		long indx =	 (index == 2 ? Num1 : 0);
		SocketSendToMail*  mail = new SocketSendToMail( indx+(LinkNum % AccountCount) );

		std::list<std::string>  tmpList;

		ListCopy( BeginItr, RsSendList.end(),tmpList, MTime );

		///每个帐号发送数量
		mail->AppendUser( tmpList );

		//m_TPool->push<SendToMail>( mail );
		if( index == 1)
			pool->push<SocketSendToMail> ( mail );
		else
			pool->push_back<SocketSendToMail>( mail );

		LinkNum ++ ;
	}

}