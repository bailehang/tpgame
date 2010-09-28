#include "stdafx.h"
#include "ProxyCheck.h"
#include "../Public/BaseDef.h"

CProxyCheck::CProxyCheck()
{
	ProxyIpAdd = ProxyType = "";
	ProxyPort = 0;
}

CProxyCheck::~CProxyCheck()
{

}

CProxyCheck::CProxyCheck(CString ip, CString type, CString port)
{
	AfxSocketInit();

	ProxyIpAdd= ip;
	ProxyType = type;
	ProxyPort = atol(port.GetBuffer());
}

bool  CProxyCheck::Check()
{
	if( !Checkout())
		return false;

	return true;
}

bool  CProxyCheck::Checkout()
{
	if ( !CheckSock.Create() )
	{
		return false;
	}

	if ( ProxyType == "SOCKS 4")
	{
		if (!CheckSock.Connect(ProxyIpAdd,ProxyPort))
		{
			CheckSock.Close();
			return false;
		}


	}
	else if ( ProxyType == "SOCKS 5")
	{
		if (!CheckSock.Connect(ProxyIpAdd,ProxyPort))
		{
			CheckSock.Close();
			return false;
		}
		/*
		char buff[600];
		struct sock5req1 *m_proxyreq1;
		m_proxyreq1 = (struct sock5req1 *)buff;
		m_proxyreq1->Ver = 5;
		m_proxyreq1->nMethods = 2;
		m_proxyreq1->Methods[0] = 0;
		m_proxyreq1->Methods[1] = 2;
		CheckSock.Send(buff,4);
		struct sock5ans1 *m_proxyans1;
		m_proxyans1 = (struct sock5ans1 *)buff;
		memset(buff,0,600);
		CheckSock.Receive(buff,600);
		if(m_proxyans1->Ver != 5 || (m_proxyans1->Method!=0 && m_proxyans1->Method!=2))
		{
			//m_sError = _T("通过代理连接主站不成功!");
			CheckSock.Close();
			return FALSE;
		}

		/// 需要密码验证
		if(m_proxyans1->Method == 2)
		{
			/*
			int nUserLen = strlen(g_ProxyInfo.m_strProxyUser);
			int nPassLen = strlen(g_ProxyInfo.m_strProxyPass);
			struct authreq *m_authreq;
			m_authreq = (struct authreq *)buff;
			m_authreq->Ver = 1;
			m_authreq->Ulen = nUserLen;
			strcpy(m_authreq->Name,g_ProxyInfo.m_strProxyUser);
			m_authreq->PLen = nPassLen;
			strcpy(m_authreq->Pass,g_ProxyInfo.m_strProxyPass);
			ClientSock.Send(buff,513);
			struct authans *m_authans;
			m_authans = (struct authans *)buff;
			memset(buff,0,600);
			ClientSock.Receive(buff,600);
			if(m_authans->Ver != 1 || m_authans->Status != 0)
			{
				//m_sError = _T("代理服务器用户验证不成功!");
				CheckSock.Close();
				return FALSE;
			}
			
		}
		// 测试主站
		struct sock5req2 *m_proxyreq2;
		m_proxyreq2 = (struct sock5req2 *)buff;
		m_proxyreq2->Ver = 5;
		m_proxyreq2->Cmd = 1;
		m_proxyreq2->Rsv = 0;
		m_proxyreq2->Atyp = 1;
		unsigned long tmpLong = inet_addr("211.43.197.200");
		unsigned short port = ntohs(25);
		memcpy(m_proxyreq2->other,&tmpLong,4);
		memcpy(m_proxyreq2->other+4,&port,2);
		CheckSock.Send(buff,sizeof(struct sock5req2)+5);
		struct sock5ans2 *m_proxyans2;
		memset(buff,0,600);
		m_proxyans2 = (struct sock5ans2 *)buff;
		CheckSock.Receive(buff,600);
		if(m_proxyans2->Ver != 5 || m_proxyans2->Rep != 0)
		{
			CheckSock.Close();
			return FALSE;
		}
		 */
	}
	else if ( ProxyType == "HTTP")
	{
		if (!CheckSock.Connect(ProxyIpAdd,ProxyPort))
		{
			CheckSock.Close();
			return false;
		}
		/*
		char buff[600];
		//sprintf( buff, "%s%s:%d%s","CONNECT ",GetServerHostName(),GetPort()," HTTP/1.1\r\nUser-Agent: MyApp/0.1\r\n\r\n");
		CheckSock.Send(buff,strlen(buff)); //发送请求
		memset(buff,0,600);
		CheckSock.Receive(buff,600);
		if(strstr(buff, "HTTP/1.0 200 Connection established") == NULL) //连接不成功
		{
			CheckSock.Close(); 
			return FALSE;
		}
	    */
	}
	return false;
}