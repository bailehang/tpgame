#include "stdafx.h"
#include "SockSendMail.h"
#include "../Public/BaseDef.h"
#include "../Public/Singleton.h"
#include "../Public/STLFun.h"
#include "../Public/Base.h"

bool  SocketSendToMail::Send( size_t index )
{
	size_t tsize = GetInstObj(MailLoginInfo).m_Vec.size();
	if ( tsize <= m_SendID )
	{
		return false ;
	}				  

	tagSend		 m_Send     = GetInstObj(MailLoginInfo).m_Vec[ m_SendID ];

	tagSendInfo& m_SendInfo = GetInstObj(tagSendInfo);

	char	     Buf[1024];

	if( !CreateSocket() )
		return false;

	if( !m_Socket.Connect(m_Send.smtp.c_str(), 25 ) )
	{
		m_Socket.Close();
		return FALSE;
	}

	if( !CheckResponse( "220" , Buf ) )
	{
		m_Socket.Close();
		return false;
	}

	if (!CheckAccount(m_Send.smtp,m_Send.user,m_Send.pword) )
	{
		ReleaseSocket();
		return false;
	}

	//// 验证通过发送数据

	if(m_list.size()<=0) return false;

	string strTmp;

	//发送MAIL FROM:<abc@xyz.com>
	strTmp="MAIL FROM:<"+m_Send.login+">\r\n";
	if(m_Socket.Send(strTmp.c_str(),strTmp.length()) == SOCKET_ERROR)
	{
		int elen = WSAGetLastError();
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse("250", Buf )) return false;

	//发送RCPT To:<abc@xyz.com>
	for ( std::list<std::string>::iterator itr = m_list.begin() ; itr != m_list.end() ; itr ++ )
	{
		strTmp="RCPT To:<"+(*itr)+">\r\n";

		if(m_Socket.Send(strTmp.c_str(),strTmp.length()) == SOCKET_ERROR)
		{
			int elen = WSAGetLastError();
			ReleaseSocket();
			return false;
		}
		if(!CheckResponse("250", Buf )) return false;
	}

	//发送"DATA\r\n"
	if(m_Socket.Send("DATA\r\n",strlen("DATA\r\n")) == SOCKET_ERROR)
	{
		int elen = WSAGetLastError();
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse("354", Buf )) return false;


	strTmp="MIME-Version: 1.0\r\n";

	//"Mail From:SenderName<xxx@mail.com>\r\n"
	strTmp+="From: "+m_Send.name+"<"+m_Send.login+">\r\n";

	strTmp+="To: ";//friends<"+m_Send.login;
	//发送RCPT To:<abc@xyz.com>
	for ( std::list<std::string>::iterator itr = m_list.begin() ; itr != m_list.end() ; itr ++ )
	{
		strTmp+="'firends'<"+(*itr)+">,";
	}
 	strTmp+="\r\n";

	//"Subject: 邮件主题\r\n"
//	strTmp+="Subject: "+m_SendInfo.subject+"\r\n";
	
	//strTmp+="X-mailer:  Foxmail 6, 14, 103, 24 [cn]\r\n";

	strTmp+="Content-Type: multipart/mixed;boundary='=====f0g_ftlTZ385rJVM22x_wrf9tPFVaIG5j3_TP====='\r\n\r\n";

	//将邮件内容发送出去
	if(m_Socket.Send(strTmp.c_str(),strTmp.length() ) == SOCKET_ERROR)
	{
		ReleaseSocket();

		return false;	
	}

	strTmp="--=====f0g_ftlTZ385rJVM22x_wrf9tPFVaIG5j3_TP=====\r\n" ;
	strTmp+="Content-Type: text/html; charset=gb2312\r\n" ;

	strTmp+="Content-Transfer-Encoding: 8bit\r\n\r\n";

	//邮件内容
	strTmp+=m_SendInfo.Context+"\r\n\r\n";
	strTmp+="--=====f0g_ftlTZ385rJVM22x_wrf9tPFVaIG5j3_TP=====--\r\n.\r\n";

	//将邮件内容发送出去
	if(m_Socket.Send(strTmp.c_str(),strTmp.length() ) == SOCKET_ERROR)
	{
		ReleaseSocket();

		return false;	
	}
	if(!CheckResponse("250", Buf )) return false;

	if(m_Socket.Send("QUIT\r\n",strlen("QUIT\r\n") ) == SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}

	if(!CheckResponse("221", Buf )) return false;

	ReleaseSocket();
	return true;
}

bool  SocketSendToMail::CheckAccount(std::string ip,std::string name,std::string pass)
{
	CBase base64;

	char Buf[1024];
	//向服务器发送"HELO "+服务器名
	string strTmp="HELO SmtpServer\r\n";
	if(m_Socket.Send( strTmp.c_str(),strTmp.length() )  == SOCKET_ERROR)	
	{
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse("250", Buf )) return false;

	if(m_Socket.Send("AUTH LOGIN\r\n",strlen("AUTH LOGIN\r\n")) == SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse("334", Buf )) return false;

	//发送经base64编码的用户名
	string strUserName=base64.Encode((unsigned char *)name.c_str(),name.length())+"\r\n";

	if( m_Socket.Send(strUserName.c_str(),strUserName.length() ) ==  SOCKET_ERROR) 
	{
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse("334", Buf )) return false;

	//发送经base64编码的密码
	string strPassword=base64.Encode((unsigned char *)pass.c_str(),pass.length())+"\r\n";

	if( m_Socket.Send(strPassword.c_str(),strPassword.length() ) ==  SOCKET_ERROR) 
	{
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse("235", Buf )) return false;
	return true;
}

