#include "stdafx.h"
#include "SendMail.h"
#include "../Public/Base.h"
#include "../Public/BaseDef.h"

#include <afxsock.h>
#include <Afxmt.h>

CSendMail::CSendMail(CString ip,CString port,CString user,CString pass)
{
	m_ipServer = ip;
	m_Port = port;
	m_User = user;
	m_PassWord = pass;
}

CSendMail::~CSendMail()
{
	
}

bool  CSendMail::Connect(CString Ip,CString Port,CString type,bool Proxy)
{
	m_MainSocket.SetProxy(PROXYTYPE_SOCKS5,Ip,atol(Port),"","");

	if( !CreateSocket() )
		return false;

	if ( Proxy )
	{
	   //if( !m_Socket.Connect( Ip , atol(Port.GetBuffer()) ) )
	   //   return false;
		if( !m_MainSocket.Connect(m_ipServer,atol(m_Port) ) )
			return false;
	   if ( type == "SOCKS 4")
	   {

	   }
	   else if ( type == "SOCKS 5")
	   {
		   /*
		   char reply[255];
		   unsigned char sommand[10];
		   memset(sommand,0,10);
		   sommand[0]=5;
		   //CAsyncProxySocket supports to logon types: No logon and 
		   //cleartext username/password (if set) logon
		   sommand[1]=1; //Number of logon types
		   sommand[2]=0; //2=user/pass, 0=no logon
		   sommand[3]=3; //2=user/pass, 0=no logon
		   int len=4;	 //length of request
		   int res=m_MainSocket.Send(sommand,len);
		   */
		   /*if (res==SOCKET_ERROR || res<len)
		   {
			   if (GetLastError()!=WSAEWOULDBLOCK || res<len)
			   {
				   return false;
			   }
		   }
		   long  byteRecv = 0;
		   m_MainSocket.Receive(reply,255);
		   if ( reply[0] != 5 || reply[1] != 0)
		   {
			   return false;
		   }

		   try
		   {
			   char *command=new char[100];
			   memset(command,0,100);
			   command[0]=5;
			   command[1]=1;
			   command[2]=0;
			   command[3]=1;
			   int len=4;

			   unsigned long  tmpLong = inet_addr(m_ipServer.GetBuffer()); 
			   unsigned short port    = ntohs(atol(m_Port));

			   memcpy( command+4,							//Copy the IP
				   (char *)&tmpLong,
				   4 );											
			   memcpy( command+8,							//Copy the port
				   (char *)&port,
				   2 );											
			   command[10] = 0;
			   int res=m_MainSocket.Send(command,10);
			   long  byteRecv = 0;
			   memset(reply,0,255);
			   m_MainSocket.Receive(reply,255);
			   delete [] command;
			   if (res==SOCKET_ERROR || res<len)
			   {
				   return false;
			   }

			   if ( reply[0] != 5)							//Reply code should be 0
			   { 
				   // SOCKS: Reply code is invalid
				   return false;
			   }
			   if ( reply[1] == 0 )						//Request granted!
			   {
				   return true;
			   }
			   if (!CheckAccount() )
			   {
				   ReleaseSocket();
				   return false;
			   }
				
			   return true;
		   }
		   catch (...)
		   {
			   return false;
		   } */
		   /*
		   int  retval = -1;
		   int  lPacketLen = 6;

		   try
		   {
			   char reply[255];
			   memset(reply,0,255);
			   char *packet = new char[lPacketLen];
			   packet[0] = 5;							//Socks version
			   packet[1] = 2;									//No of methods
			   packet[2] = 0;									//No auth required
			   packet[3] = 2;									//for UserName and Password
			   packet[4] = 3;									//chap
			   packet[5] = 0;									

			   Sleep( 1000 );
			   long  bytesSend = 0;
			   m_Socket.Send(packet,lPacketLen);
			   long  byteRecv = 0;
			   m_Socket.Receive(reply,255);
			   delete [] packet;

			   if ( reply[0] != 5 )
			   {
				   return false;
			   }
			   if ( reply[1] == 0 )
			   {
				   char *packet1=new char[12];					//Allocate a packet
				   memset(packet1 , '\0' , 12);
				   packet1[0] = 5;								//Socks version
				   packet1[1] = 1;								//No of methods
				   packet1[2] = 0;								//No authuntication
				   packet1[3] = 3;								//for domainname
				   packet1[4] = 1;								//for ip v4 address

				   unsigned long  tmpLong = inet_addr("123.125.50.112"); 
				   unsigned short port    = ntohs(25);

				   memcpy( packet1+5,							//Copy the IP
					   (char *)&tmpLong,
					   4 );											
				   memcpy( packet1+9,							//Copy the port
					   (char *)&port,
					   2 );											
				   packet1[11] = 0;

				   m_Socket.Send(packet1,12);
				   long  byteRecv = 0;
				   memset(reply,0,255);
				   m_Socket.Receive(reply,255);
				   delete [] packet1;

				   if ( reply[0] != 5)							//Reply code should be 0
				   { 
					   // SOCKS: Reply code is invalid
					   return false;
				   }
				   if ( reply[1] == 0 )						//Request granted!
				   {
					   return true;
				   }
			   }

			   int retval = reply[1];
			   //printf("retval =%d", retval);
			   bool m_IsError = true;
			   char m_szErrorMessage[255];
			   switch ( retval )									//Error codes
			   {

			   case 1:
				   {
					   strcpy( m_szErrorMessage , "SOCKS: general SOCKS server failure\0" );
					   break;
				   }
			   case 2:
				   {
					   strcpy( m_szErrorMessage , "SOCKS: connection not allowed by ruleset\0" );
					   break;
				   }
			   case 3:
				   {
					   strcpy( m_szErrorMessage , "SOCKS: Network unreachable\0" );
					   break;
				   }
			   case 4:
				   {
					   strcpy( m_szErrorMessage , "SOCKS: Host unreachable\0" );
					   break;
				   }
			   case 5:
				   {
					   strcpy( m_szErrorMessage , "SOCKS: Connection refused\0" );
					   break;
				   }
			   case 6:
				   {
					   strcpy( m_szErrorMessage , "SOCKS: TTL expired\0" );
					   break;
				   }
			   case 7:
				   {
					   strcpy( m_szErrorMessage , "SOCKS: Command not supported\0" );
					   break;
				   }
			   case 8:
				   {
					   strcpy( m_szErrorMessage , "SOCKS: Address type not supported\0" );
					   break;
				   }
			   case 9:
				   {
					   strcpy( m_szErrorMessage , "SOCKS: to X'FF' unassigned\0" );
					   break;
				   }            				
			   default:
				   {
					   strcpy( m_szErrorMessage , "SOCKS: General Failure or Failed to connect destination server.\0" );
					   break;
				   }
			   }
			   return retval >=0 ;

		   }
		   catch (...)
		   {
			   return false;
		   }
		   char buff[600];
		   struct sock5req2 *m_proxyreq2;
		   m_proxyreq2 = (struct sock5req2 *)buff;
		   m_proxyreq2->Ver = 5;
		   m_proxyreq2->Cmd = 1;
		   m_proxyreq2->Rsv = 0;
		   m_proxyreq2->Atyp = 1;
		   m_Socket.Send(buff,4);
		   struct sock5ans2 *m_proxyans1;
		   m_proxyans1 = (struct sock5ans2 *)buff;
		   memset(buff,0,600);
		   m_Socket.Receive(buff,600);
		   if(m_proxyans1->Ver != 5 || m_proxyans1->Rep != 0)
		   {
			   m_Socket.Close();
			   return FALSE;
		   }
		   */
	   }
	   else if ( type == "HTTP")
	   {
		   
		   char buff[600];
		   sprintf( buff, "%s%s:%d%s","CONNECT ","119.75.218.45",80," HTTP/1.1\r\nUser-Agent: MyApp/0.1\r\n\r\n");
		   m_Socket.Send(buff,strlen(buff)); //发送请求
		   memset(buff,0,600);
		   m_Socket.Receive(buff,600);
		   if(strstr(buff, "HTTP/1.0 200 Connection established") == NULL) //连接不成功
		   {
			m_Socket.Close(); 
			return FALSE;
		   }
		   
	   }
	}
	else
	{
		if( !m_Socket.Connect( m_ipServer, atol(m_Port.GetBuffer() ) ) )
		{
			m_Socket.Close();
			return FALSE;
		}
		if( !CheckResponse( "220" ) )
		{
			m_Socket.Close();
			return FALSE;
		}

		//向服务器发送"HELO "+服务器名
		string strTmp="HELO "+m_ipServer+"\r\n";
		if(m_Socket.Send( strTmp.c_str(),strTmp.length() )  == SOCKET_ERROR)	
		{
			ReleaseSocket();
			return false;
		}
		if(!CheckResponse("250")) return false;

		if (!CheckAccount() )
		{
			ReleaseSocket();
			return false;
		}
		return true;
	}
	return false;
}

bool  CSendMail::CreateSocket()
{
	return m_MainSocket.Create() > 0 ? true : false ;
}

void  CSendMail::ReleaseSocket()
{
	 m_Socket.Close();
}
bool  CSendMail::CheckResponse(CString RecvCode)
{
	try
	{
		 char Buf[1024];
		 m_Socket.Receive( Buf, 1024 );
		 //AfxMessageBox(Buf);
		 return RecvCode[0] == Buf[0] && \
				RecvCode[1] == Buf[1] && \
				RecvCode[2] == Buf[2] ? true : false;
	}
	catch(...)
	{
		return false;
	}
}

bool  CSendMail::CheckAccount()
{
	CBase base64;


	//发送"AUTH LOGIN"
	//if( m_Socket.Send(strPassword.c_str(),strPassword.length() ) ==  SOCKET_ERROR) 
	if(m_Socket.Send("AUTH LOGIN\r\n",strlen("AUTH LOGIN\r\n")) == SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse("334")) return false;

	//发送经base64编码的用户名
	string strUserName=base64.Encode((unsigned char *)m_User.GetBuffer(),m_User.GetLength())+"\r\n";

	if( m_Socket.Send(strUserName.c_str(),strUserName.length() ) ==  SOCKET_ERROR) 
	{
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse("334")) return false;

	//发送经base64编码的密码
	string strPassword=base64.Encode((unsigned char *)m_PassWord.GetBuffer(),m_PassWord.GetLength())+"\r\n";

	if( m_Socket.Send(strPassword.c_str(),strPassword.length() ) ==  SOCKET_ERROR) 
	{
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse("235")) return false;

	return true;
}


bool  CSendMail::SendData(CString SendFrom,vector<string> SendtoList,
						  CString SendName,CString ReceiverName,
						  CString Subject,CString Content, bool IsHtml)
{
	if(SendFrom.IsEmpty() )    return false;
	if(SendtoList.size()<=0) return false;

	string strTmp;

	//发送MAIL FROM:<abc@xyz.com>
	strTmp="MAIL FROM:<"+SendFrom+">\r\n";
	if(m_Socket.Send(strTmp.c_str(),strTmp.length()) == SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse("250")) return false;

	//发送RCPT To:<abc@xyz.com>
	for(size_t i=0;i<SendtoList.size();i++)
	{
		strTmp="RCPT To:<"+SendtoList[i]+">\r\n";
		if(m_Socket.Send(strTmp.c_str(),strTmp.length()) == SOCKET_ERROR)
		{
			ReleaseSocket();
			return false;
		}
		if(!CheckResponse("250")) return false;
	}

	//发送"DATA\r\n"
	if(m_Socket.Send("DATA\r\n",strlen("DATA\r\n")) == SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse("354")) return false;


	//"Mail From:SenderName<xxx@mail.com>\r\n"
	strTmp="From:"+SendName+"<"+SendFrom+">\r\n";

	//"Subject: 邮件主题\r\n"
	strTmp+="Subject:"+Subject+"\r\n";

	//"MIME_Version:1.0\r\n"
	strTmp+="MIME_Version:1.0\r\n";

	//	//"X-Mailer:Smtp Client By xxx"//版权信息
	strTmp+="X-Mailer:"; strTmp+="Stmp by:Test!"; strTmp+="\r\n";

	//"MIME_Version:1.0\r\n"
	strTmp+="MIME_Version:1.0\r\n";

	//"Content-type:multipart/mixed;Boundary=xxx\r\n\r\n";
	strTmp+="Content-type:multipart/mixed;Boundary=";
	strTmp+="boundary";
	strTmp+="\r\n\r\n";

	//strTmp="RCPT To:<283899487@qq.com>\r\nRCPT To:<185123@qq.com>\r\n";

	if(m_Socket.Send(strTmp.c_str(),strTmp.length() ) == SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;	
	}

	//邮件主体
	strTmp="--";
	strTmp+="boundary";
	strTmp+="\r\n";
	strTmp+="Content-type:text/plain;Charset=gb2312\r\n";
	strTmp+="Content-Transfer-Encoding:8bit\r\n\r\n";

	//邮件内容
	strTmp+=Content+"\r\n\r\n";

	//将邮件内容发送出去
	if(m_Socket.Send(strTmp.c_str(),strTmp.length() ) == SOCKET_ERROR)
	{
		ReleaseSocket();

		return false;	
	}

	//界尾
	strTmp="--";
	strTmp+="boundary";
	strTmp+="--\r\n.\r\n";

	if(m_Socket.Send(strTmp.c_str(),strTmp.length() ) == SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}

	if(!CheckResponse("250")) return false;
	//	AfxMessageBox("here");
	//退出

	if(m_Socket.Send("QUIT\r\n",strlen("QUIT\r\n") ) == SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}

	if(!CheckResponse("221")) return false;

	ReleaseSocket();
	return true;
}