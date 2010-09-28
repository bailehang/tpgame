/** 
 *  brief  : 封装socket的发送邮件信息
 *	author ：expter
 */

#pragma once 

#include "MainSocket.h"

#include <list>
#include <vector>
#include <map>
using namespace std;


class CSendMail
{

public:
	CSendMail(CString ip,CString port,CString user,CString pass);
	virtual ~CSendMail();


public:

	///Connect Send Server 
	bool  Connect(CString Ip="",CString Port="",CString type="",bool Proxy=false);

	/// Send UserData
	bool  SendData(CString SendFrom,vector<string> SendtoList,
				   CString SendName,CString ReceiverName,
				   CString Subject,CString Content,
				   bool    IsHtml);

private:

	///Create Socket
	bool CreateSocket();	

	///ReleaseSocket
	void ReleaseSocket();

	///Check Response
	bool CheckResponse(CString RecvCode);

	///check account
	bool CheckAccount();

private:

	CString   m_ipServer;
	CString	  m_Port;
	CString   m_PassWord;
	CString   m_User;

	long	  ProxyPort;
	CString   ProxyIpAdd;
	CString   ProxyType;

	CSocket	  m_Socket;
	WSADATA	  m_WsaData;
	HOSTENT*  m_HostTent;
    SOCKADDR_IN  m_SockAddr;

	CMainSocket  m_MainSocket;
	

};
