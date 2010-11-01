#include "stdafx.h"
#include "SendMailApp.h"
#include "SendToMail.h"
#include "SockSendMail.h"
#include "Thread.h"
#include "../Public/Singleton.h"
#include "../Public/STLFun.h"
#include "../Public/GameSetup.h"


#define  SAFE_DELETE( P ) { if(P!=NULL) { delete P; P=NULL;}}

SendMailApp::SendMailApp()
{
	m_RasDial = new MyRasDial("test","test","宽带连接");

	GetInstObj(CGameSetup).Load();
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
		m_TPool = new tp_ipc_peer_namespace::ctpool( 1 );
		
		std::list<std::string>& RsSendList = GetInstObj(DestList).m_SendList;

		/// 发送地址数量
		long  DestCount = RsSendList.size();
		/// 发送帐号信息
		long  AccountCount = GetInstObj( MailLoginInfo ).m_Vec.size();
		/// 每次发送的联系人数量
		const long  MTime = 3;

		size_t  SendNum = (DestCount / MTime) + (DestCount % MTime > 0 ? 1 : 0) ;
		
		/// 联系人数量
		long  LinkNum = 0 ;
	
		std::list<std::string>::iterator BeginItr = RsSendList.begin();

		for (size_t i = 0 ; i < SendNum ; i ++  )
		{
			//SendToMail*  mail = new SendToMail( LinkNum % AccountCount );
			SocketSendToMail*  mail = new SocketSendToMail( LinkNum % AccountCount );

			std::list<std::string>  tmpList;

			ListCopy( BeginItr, RsSendList.end(),tmpList,MTime );

			///每个帐号发送数量
			mail->AppendUser( tmpList );

			//tp_ipc_peer_namespace::task<SendToMail> *task =  new tp_ipc_peer_namespace::task<SendToMail>( mail );

			//m_TPool->push<SendToMail>( mail );
			m_TPool->push<SocketSendToMail> ( mail );

			LinkNum ++ ;
		}
		
		m_TPool->Start();

		/// 发送邮件
		for ( ;  ; )
		{
			bool Exit = m_TPool->IsExit();
			if ( Exit )
			{
				break;
			}
		}
	}

//	m_RasDial->Stop();
}

