/*CProxySocket by Tim Kosse (Tim.Kosse@gmx.de)
                 Version 1.4 (2002-03-21)
--------------------------------------------------------

Introduction:
-------------

This class is a CAsyncSocket derived class. With this class you
can connect through SOCKS4/5 and HTTP 1.1 proxies. This class works 
as semi-transparent layer between you own class and CAsyncSocket.
This class is used in FileZilla, a powerful open-source FTP client.
It can be found under http://sourceforge.net/projects/filezilla
For more information about SOCKS4/5 goto 
http://www.socks.nec.com/socksprot.html
For more information about HTTP 1.1 goto http://www.rfc-editor.org
and search for RFC2616

How to use?
-----------

You don't have to change much in you already existing code to use
CProxySocket.
To use it you've to derive you own class from CProxySocket and 
you've to provide a definition for OnProxyOperationFailed(int nOpID) and 
OnListenFinished(unsigned long &retProxyIp,int &retProxyPort)=0;
When you create an instance of your derived class, you should call
SetProxy.
If you overload the OnReceive or OnConnect functions, you have to call
CProxySocket::OnReceive and CProxySocket::OnConnect
as first instrucion in your OnReceive and OnConnect functions.
Sample code:
CMyClass::OnReceive(int nErrorCode)
{
	CProxySocket::OnReceive(nErrorCode);
	//Add you own implementation below:
	...
}

If an operation fails, OnProxyOperationFailed is called. There
you can handle the error. int nOpID specifies the type of the operation
that failed: PROXYOP_CONNECT when a connection attempt failed and
PROXYOP_LISTEN when creating a listen socket failed.

If you want to use CProxySocket to create a listen socket, you
have to use this overloaded function:
virtual BOOL Listen(unsigned long serverIp);
serverIP is the IP of the server you are already connected 
through the SOCKS proxy. You can't use listen sockets over a
SOCKS proxy without a primary connection. Listen sockets are only 
supported by SOCKS proxies, this won't work with HTTP proxies.
When the listen socket is created successfully, OnListenFinished is
called. The parameters unsigned long &retProxyIp and int &retProxyPort 
will tell you the ip and the port of the listen socket.
After it you have to handle the OnAccept message and accept the 
connection.

Description of the new functions and their parameters:
------------------------------------------------------

void SetProxy(int nProxyType);
void SetProxy(int nProxyType, CString szProxyHost, int nProxyPort);
void SetProxy(int nProxyType, CString szProxyHost, int nProxyPort,CString ProxyUser, CString ProxyPass);
	
Call one of this functions to set the proxy type.
Parametes:
- nProxyType specifies the Proxy Type.
- szProxyHost and nProxyPort specify the address of the proxy
- ProxyUser and ProxyPass are only available for SOCKS5 proxies.

supported proxy types:
PROXYTYPE_NOPROXY
PROXYTYPE_SOCKS4
PROXYTYPE_SOCKS4A
PROXYTYPE_SOCKS5
PROXYTYPE_HTTP11

There are also some other functions:

GetProxyPeerName
Like GetPeerName of CAsyncSocket, but returns the address of the
server connected through the proxy.	If using proxies, GetPeerName 
only returns the address of the proxy.

int GetProxyType();
Returns the used proxy

const int GetLastProxyError() const;
Returns the last proxy error

Version history
---------------

- 1.4 added UNICODE support
- 1.3 added basic HTTP1.1 authentication
      fixed memory leak in SOCKS5 code
	  OnSocksOperationFailed will be called after Socket has been closed
      fixed some minor bugs
- 1.2 renamed into CProxySocket
      added HTTP1.1 proxy support
- 1.1 fixes all known bugs, mostly with SOCKS5 authentication
- 1.0 initial release
*/

#include "stdafx.h"
#include "ProxySocket.h"
#include "Base64coding.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProxySocket

CProxySocket::CProxySocket()
{
	m_nProxyOpID		=	0;
	m_nProxyOpState		=	0;
	m_pRecvBuffer		=	0;
	m_nRecvBufferPos	=	0;
	m_nProxyPeerIp		=	0;
	m_nProxyPeerPort	=	0;
	m_ProxyPeerHost		=	_T("");
	m_nProxyError		=	PROXYERROR_NOERROR;

	m_szTempHost		=	_T("");
	m_nTempPort			=	0;

	m_bConnectProxy		=	FALSE;
}

CProxySocket::~CProxySocket()
{
	ClearBuffer();
}

//Die folgenden Zeilen nicht bearbeiten. Sie werden vom Klassen-Assistenten ben鰐igt.
#if 0
BEGIN_MESSAGE_MAP(CProxySocket, CAsynSocket)
	//{{AFX_MSG_MAP(CProxySocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// Member-Funktion CProxySocket 

void CProxySocket::SetProxy(int nProxyType)
{
	ASSERT(nProxyType==PROXYTYPE_NOPROXY);
	ASSERT(!m_nProxyOpID);
	m_ProxyData.nProxyType=nProxyType;
}

void CProxySocket::SetProxy(int nProxyType, LPCTSTR lpszProxyHost, int nProxyPort)
{
	//Validate the parameters
	ASSERT(nProxyType==PROXYTYPE_SOCKS4 || nProxyType==PROXYTYPE_SOCKS4A || 
		   nProxyType==PROXYTYPE_SOCKS5 || nProxyType==PROXYTYPE_HTTP11);
	ASSERT(!m_nProxyOpID);
	m_ProxyData.nProxyType	=	nProxyType;

	ASSERT(lpszProxyHost != NULL);
	ASSERT(nProxyPort>0);
	ASSERT(nProxyPort<=65535);

	m_ProxyData.szProxyHost	=	lpszProxyHost;
	m_ProxyData.nProxyPort	=	nProxyPort;
	m_ProxyData.bUseLogon	=	FALSE;

	m_ProxyData.szProxyUser	=	_T("");
	m_ProxyData.szProxyPass	=	_T("");
}

void CProxySocket::SetProxy(int nProxyType, LPCTSTR lpszProxyHost, int nProxyPort, LPCTSTR lpszProxyUser, LPCTSTR lpszProxyPass)
{
	//Validate the parameters
	ASSERT(!m_nProxyOpID);
	m_ProxyData.nProxyType=nProxyType;

	ASSERT(lpszProxyHost != NULL);
	ASSERT(nProxyPort>0);
	ASSERT(nProxyPort<=65535);

	m_ProxyData.szProxyHost	=	lpszProxyHost;
	m_ProxyData.nProxyPort	=	nProxyPort;

	m_ProxyData.szProxyUser	=	lpszProxyUser;
	m_ProxyData.szProxyPass	=	lpszProxyPass;
	if(!m_ProxyData.szProxyUser.IsEmpty() || 
		!m_ProxyData.szProxyPass.IsEmpty() )
		m_ProxyData.bUseLogon	=	TRUE;
	else
		m_ProxyData.bUseLogon	=	FALSE;
}

int CProxySocket::Send(const void* lpBuf, int nBufLen, int nFlags) 
{
	m_nProxyError = PROXYERROR_NOERROR;
	if (m_nProxyOpID)
	{
		WSASetLastError(WSAEWOULDBLOCK);
		return SOCKET_ERROR;
	}
	return CAsynSocket::Send(lpBuf, nBufLen, nFlags);
}

BOOL CProxySocket::Listen(unsigned long serverIp)
{
	m_nProxyError=PROXYERROR_NOERROR;
	//Conect to proxy server
	BOOL res=CAsynSocket::Connect(m_ProxyData.szProxyHost,m_ProxyData.nProxyPort);
	if (!res)
	{
		if (GetLastError()!=WSAEWOULDBLOCK)
		{
			m_nProxyError=PROXYERROR_NOCONN;
			return FALSE;
		}
	}
	m_nProxyPeerPort	=	0;
	m_nProxyPeerIp		=	serverIp;
	m_nProxyOpID		=	PROXYOP_LISTEN;

	return TRUE;
}

BOOL CProxySocket::Listen()
{
	m_nProxyError = PROXYERROR_NOERROR;
	return CAsynSocket::Listen();
}

BOOL CProxySocket::GetProxyPeerName(CString & rPeerAddress, UINT & rPeerPort)
{
	ASSERT(m_ProxyData.nProxyType);

	BOOL res = GetPeerName(rPeerAddress, rPeerPort);
	if (res)
	{
		rPeerPort = m_nProxyPeerPort;
		rPeerAddress.Format(_T("%d.%d.%d.%d"), (m_nProxyPeerIp>>24)%256, (m_nProxyPeerIp>>16)%256, (m_nProxyPeerIp>>8)%256, m_nProxyPeerIp%256);
	}
	return res;
}

BOOL CProxySocket::GetProxyPeerName( SOCKADDR* lpSockAddr, int* lpSockAddrLen )
{
	ASSERT(m_ProxyData.nProxyType);
	BOOL res=GetPeerName(lpSockAddr,lpSockAddrLen);
	if (res)
	{
		LPSOCKADDR_IN addr=(LPSOCKADDR_IN)lpSockAddr;
		addr->sin_port=m_nProxyPeerPort;
		addr->sin_addr.S_un.S_addr=m_nProxyPeerIp;		
	}
	return res;
}

const int CProxySocket::GetLastProxyError() const
{
	return m_nProxyError;
}

int CProxySocket::Receive(void* lpBuf, int nBufLen, int nFlags) 
{
	m_nProxyError=PROXYERROR_NOERROR;
	if (m_nProxyOpID)
	{
		WSASetLastError(WSAEWOULDBLOCK);
		return SOCKET_ERROR;
	}
	else
		return CAsynSocket::Receive(lpBuf, nBufLen, nFlags);
}

int CProxySocket::GetProxyType() const
{
	return m_ProxyData.nProxyType;
}

void CProxySocket::Reset()
{
	m_nProxyOpState	=	0;
	m_nProxyOpID	=	0;
}

void CProxySocket::ClearBuffer()
{
	m_strBuffer="";

	if (m_pRecvBuffer)
	{
		delete m_pRecvBuffer;
		m_pRecvBuffer =	NULL;
	}
	m_nRecvBufferLen=0;
	m_nRecvBufferPos=0;
}

void CProxySocket::Close()
{
	ClearBuffer();
	Reset();
	CAsynSocket::Close();
}

void CProxySocket::OnResolve(int nErrorCode)
{
	CAsynSocket::OnResolve(nErrorCode);

	if(nErrorCode)
	{
		//解析目标主机名失败，直接连接代理服务器
		if(m_bConnectProxy == FALSE)
		{
			//Can't resolve hostname
			if (m_ProxyData.nProxyType == PROXYTYPE_SOCKS4A || 
				m_ProxyData.nProxyType == PROXYTYPE_SOCKS5 || 
				m_ProxyData.nProxyType == PROXYTYPE_HTTP11  )
			{
				//Can send domain names to proxy
				//Conect to proxy server
				m_nProxyPeerPort	=	htons((u_short)m_nTempPort);
				m_nProxyPeerIp		=	0;
				m_ProxyPeerHost		=	m_szTempHost;
				m_nProxyOpID		=	PROXYOP_CONNECT;

				m_szTempHost		=	_T("");
				m_nTempPort			=	0;

				m_bConnectProxy		=	TRUE;
				CAsynSocket::Connect(m_ProxyData.szProxyHost, m_ProxyData.nProxyPort);
				return;
			}
			else
			{
				m_nProxyError		=	PROXYERROR_CANTRESOLVEHOST;
				WSASetLastError(WSAEINVAL);
				return ;
			}
		}
		else
		{
			m_nProxyError		=	PROXYERROR_CANTRESOLVEHOST;
			WSASetLastError(WSAEINVAL);
			return ;
		}
	}
}

BOOL CProxySocket::ConnectHelper(LPCTSTR lpszHostAddress, UINT nHostPort)
{
	m_szTempHost	=	lpszHostAddress;
	m_nTempPort		=	nHostPort;

	return CAsynSocket::ConnectHelper(lpszHostAddress, nHostPort);
}

BOOL CProxySocket::ConnectHelper(const SOCKADDR* lpSockAddr, int nSockAddrLen)
{
	m_nProxyError = PROXYERROR_NOERROR;

	if(m_ProxyData.nProxyType && m_bConnectProxy == FALSE)
	{
		//连接目标主机前先连接SOCKS代理
		LPSOCKADDR_IN sockAddr	=	(LPSOCKADDR_IN)lpSockAddr;
		
		//保存服务器详细资料
		m_nProxyPeerIp			=	sockAddr->sin_addr.S_un.S_addr;
		m_nProxyPeerPort		=	sockAddr->sin_port;
		m_ProxyPeerHost			=	_T("");
		m_nProxyOpID			=	PROXYOP_CONNECT;

		m_bConnectProxy			=	TRUE;
		return CAsynSocket::ConnectHelper(m_ProxyData.szProxyHost,m_ProxyData.nProxyPort);
	}
	else
	{
		//直接连接
		return CAsynSocket::ConnectHelper(lpSockAddr, nSockAddrLen);
	}
}

void CProxySocket::OnConnect(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_nProxyError =	PROXYERROR_NOERROR;
	CAsynSocket::OnConnect(nErrorCode);

	if (nErrorCode)
	{
		//连接到代理服务器失败
		Reset();
		//Don't call OnProxyOperationFailed, since derived class should handle nErrorCode
		OnProxyConnectCompleted(nErrorCode);
		return;
	}
	else
	{
		if(m_ProxyData.nProxyType != PROXYTYPE_NOPROXY)
		{
			if (m_nProxyOpID == PROXYOP_CONNECT ||
				m_nProxyOpID == PROXYOP_LISTEN	)
			{
				if (m_nProxyOpState)
					return;
				ASSERT(m_ProxyData.nProxyType != PROXYTYPE_NOPROXY);
				ClearBuffer();

				OnProxyConnectCompleted(nErrorCode);

				if (m_ProxyData.nProxyType == PROXYTYPE_SOCKS4 ||
					m_ProxyData.nProxyType == PROXYTYPE_SOCKS4A )
				{
					//Socks4/Socks4A Proxy
					int nLength = 10 + m_ProxyPeerHost.GetLength();
					char * command = new char[nLength];
					memset(command, 0, nLength);
					int len = 9;
					command[0] = 4;
					command[1] = (m_nProxyOpID==PROXYOP_CONNECT) ? 1 : 2; //CONNECT or BIND request
					memcpy(&command[2], &m_nProxyPeerPort, 2); //Copy target address

					if (!m_nProxyPeerIp)
					{
						ASSERT(m_ProxyData.nProxyType == PROXYTYPE_SOCKS4A);
						ASSERT(m_ProxyPeerHost != _T(""));
						//Set the IP to 0.0.0.x (x is nonzero)
						command[4] = 0;
						command[5] = 0;
						command[6] = 0;
						command[7] = 1;
						//Add host as URL
						strcpy(&command[9], (LPCTSTR)m_ProxyPeerHost);
						len += m_ProxyPeerHost.GetLength() + 1;
					}
					else
					{
						memcpy(&command[4], &m_nProxyPeerIp, 4);
					}

					int res = CAsynSocket::Send(command,len); //Send command
					delete [] command;

					if (res == SOCKET_ERROR || res < len)
					{
						if (GetLastError() != WSAEWOULDBLOCK || res < len)
						{
							m_nProxyError = PROXYERROR_REQUESTFAILED;
							int ID = m_nProxyOpID;
							OnProxyOperationFailed(ID);
							
							ClearBuffer();
							return;
						}
					}
				}
				else
				if (m_ProxyData.nProxyType == PROXYTYPE_SOCKS5)
				{
					//SOCKS5 proxy
					//Send initialization request
					unsigned char command[10] = {0};
					command[0] = 5;
					//CProxySocket supports to logon types: No logon and 
					//cleartext username/password (if set) logon
					command[1] = m_ProxyData.bUseLogon ? 2 : 1; //Number of logon types
					command[2] = m_ProxyData.bUseLogon ? 2 : 0; //2=user/pass, 0=no logon
					int len = m_ProxyData.bUseLogon ? 4 : 3;	//length of request
					int res = CAsynSocket::Send(command,len);
					if (res == SOCKET_ERROR || res < len)
					{
						if (GetLastError() != WSAEWOULDBLOCK || res < len)
						{
							m_nProxyError = PROXYERROR_REQUESTFAILED;
							int ID = m_nProxyOpID;
							//Close();
							OnProxyOperationFailed(ID);

							ClearBuffer();
							return;
						}
					}
				}
				else
				if(m_ProxyData.nProxyType == PROXYTYPE_HTTP11)
				{
					//Http1.1 Proxy
					CString str;
					if (!m_ProxyData.bUseLogon)
					{
						str.Format("CONNECT %s:%d HTTP/1.1\r\nHost: %s:%d\r\n\r\n",
							m_ProxyPeerHost,ntohs(m_nProxyPeerPort), m_ProxyPeerHost, ntohs(m_nProxyPeerPort));
					}
					else
					{
						str.Format("CONNECT %s:%d HTTP/1.1\r\nHost: %s:%d\r\n",
							m_ProxyPeerHost,ntohs(m_nProxyPeerPort), m_ProxyPeerHost, ntohs(m_nProxyPeerPort));

						CString userpass = m_ProxyData.szProxyUser + ":" + m_ProxyData.szProxyPass;
						CString base64str;
						CByteArray array;
						for (int i=0;i<userpass.GetLength();i++)
							array.Add(userpass[i]);
						
						CBase64Coding base64coding;
						if (!base64coding.Encode(array,base64str))
						{
							m_nProxyError = PROXYERROR_REQUESTFAILED;
							int ID = m_nProxyOpID;
							//Close();
							OnProxyOperationFailed(ID);

							ClearBuffer();
							return;
						}

						str += _T("Authorization: Basic ") + base64str + _T("\r\n\r\n");
						str += _T("Proxy-Authorization: Basic ") + base64str + _T("\r\n\r\n");
					}
					int numsent = CAsynSocket::Send(str, str.GetLength());
					if (numsent == SOCKET_ERROR || numsent<str.GetLength())
					{
						if (GetLastError()!=WSAEWOULDBLOCK || numsent<str.GetLength())
						{
							m_nProxyError = PROXYERROR_REQUESTFAILED;
							int ID = m_nProxyOpID;
							//Close();
							OnProxyOperationFailed(ID);

							ClearBuffer();
							return;
						}
					}
				}
				else
					ASSERT(FALSE);
				//Now we'll wait for the response, handled in OnReceive
				m_nProxyOpState ++;
			}
		}
	}
}

void CProxySocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CAsynSocket::OnReceive(nErrorCode);

	if(m_ProxyData.nProxyType != PROXYTYPE_NOPROXY)
	{
		if (!m_nProxyOpID)
		return;

		if(!m_nProxyOpState) //We should not receive a response yet!
		{
			m_nProxyError = PROXYERROR_REQUESTFAILED;
			int ID = m_nProxyOpID;
			//Close();
			OnProxyOperationFailed(ID);

			ClearBuffer();
			return;
		}
	
		if (m_ProxyData.nProxyType == PROXYTYPE_SOCKS4 ||
			m_ProxyData.nProxyType == PROXYTYPE_SOCKS4A )
		{
			if (m_nProxyOpState == 1)
			{
				if (!m_pRecvBuffer)
				{
					m_pRecvBuffer = new char[9];
					memset(m_pRecvBuffer, 0, 9);
				}

				int numread = CAsynSocket::Receive(m_pRecvBuffer+m_nRecvBufferPos, 8-m_nRecvBufferPos);
				if (numread == SOCKET_ERROR)
				{
					if (GetLastError() != WSAEWOULDBLOCK)
					{
						m_nProxyError = PROXYERROR_REQUESTFAILED;
						int ID = m_nProxyOpID;
						//Close();
						OnProxyOperationFailed(ID);
					}
					ClearBuffer();
					return;
				}
				m_nRecvBufferPos += numread;

				if(m_nRecvBufferPos == 8)
				{
					if (m_pRecvBuffer[1] != 90 || m_pRecvBuffer[0] != 0)
					{
						m_nProxyError = PROXYERROR_REQUESTFAILED;
						int ID = m_nProxyOpID;
						//Close();
						OnProxyOperationFailed(ID);
						ClearBuffer();
						return;
					}
					if(m_nProxyOpID == PROXYOP_CONNECT)
					{
						//OK, we are connected with the remote server
						ClearBuffer();
						Reset();
						//Call OnSend
						OnSend(0);
						return;
					}
					else
					{
						//Listen socket created
						m_nProxyOpState ++;
						
						unsigned long ip;
						int port;

						memcpy(&ip, &m_pRecvBuffer[4], 4);
						if (!ip)
						{
							//No IP return, use the IP of the proxy server
							SOCKADDR SockAddr;
							memset(&SockAddr,0,sizeof(SockAddr));

							int SockAddrLen = sizeof(SockAddr);
							if (GetPeerName(&SockAddr, &SockAddrLen ))
							{
								ip = ((LPSOCKADDR_IN)&SockAddr)->sin_addr.S_un.S_addr;
							}
							else
							{
								m_nProxyError = PROXYERROR_REQUESTFAILED;
								int ID = m_nProxyOpID;
								//Close();
								OnProxyOperationFailed(ID);
								ClearBuffer();
								return;
							}
						}
						memcpy(&port, &m_pRecvBuffer[2], 2);
						OnListenFinished(ip, port);
					}
					ClearBuffer();
				}
			}
			else 
			if (m_nProxyOpID == 2)
			{
				if (!m_pRecvBuffer)
				{
					m_pRecvBuffer = new char[8];
					memset(m_pRecvBuffer, 0, 8);
				}

				int numread = CAsynSocket::Receive(m_pRecvBuffer+m_nRecvBufferPos, 8-m_nRecvBufferPos);
				if (numread == SOCKET_ERROR)
				{
					if (GetLastError() != WSAEWOULDBLOCK)
					{
						m_nProxyError = PROXYERROR_REQUESTFAILED;
						int ID = m_nProxyOpID;
						//Close();
						OnProxyOperationFailed(ID);
					}
					ClearBuffer();
					return;
				}
				m_nRecvBufferPos += numread;
				if (m_nRecvBufferPos == 8)
				{
					if (m_pRecvBuffer[1] != 90 || m_pRecvBuffer[0] != 0)
					{
						m_nProxyError = PROXYERROR_REQUESTFAILED;
						int ID = m_nProxyOpID;
						//Close();
						OnProxyOperationFailed(ID);
						ClearBuffer();
						return;
					}
					//Connection to remote server established
					ClearBuffer();
					Reset();
					OnAccept(0);
				}
			}
		}
		else 
		if (m_ProxyData.nProxyType == PROXYTYPE_SOCKS5)
		{	
			if (m_nProxyOpState == 1) //Get respone to initialization message
			{
				if (!m_pRecvBuffer)
				{
					m_pRecvBuffer = new char[2];
					memset(m_pRecvBuffer, 0, 2);
				}

				int numread = CAsynSocket::Receive(m_pRecvBuffer+m_nRecvBufferPos, 2-m_nRecvBufferPos);
				if (numread == SOCKET_ERROR)
				{
					if (GetLastError() != WSAEWOULDBLOCK)
					{
						m_nProxyError = PROXYERROR_REQUESTFAILED;
						int ID = m_nProxyOpID;
						//Close();
						OnProxyOperationFailed(ID);
					}
					ClearBuffer();
					return;
				}
				m_nRecvBufferPos += numread;
				if (m_nRecvBufferPos == 2)
				{
					if (m_pRecvBuffer[1] == 0xFF || m_pRecvBuffer[0] != 5)
					{
						m_nProxyError = PROXYERROR_AUTHREQUIRED;
						int ID = m_nProxyOpID;
						//Close();
						OnProxyOperationFailed(ID);
						
						ClearBuffer();
						return;
					}
					if (m_pRecvBuffer[1])
					{ 
						//不支持的验证类型
						if (m_pRecvBuffer[1] != 2)
						{
							//Unknown auth type
							m_nProxyError = PROXYERROR_AUTHTYPEUNKNOWN;
							int ID = m_nProxyOpID;
							//Close();
							OnProxyOperationFailed(ID);

							ClearBuffer();
							return;
						}
						
						if (!m_ProxyData.bUseLogon)
						{
							m_nProxyError = PROXYERROR_AUTHNOLOGON;
							int ID = m_nProxyOpID;
							//Close();
							OnProxyOperationFailed(ID);

							ClearBuffer();
							return;
						}
						//Send authentication
						unsigned char * buffer = new unsigned char[3+m_ProxyData.szProxyUser.GetLength() + m_ProxyData.szProxyPass.GetLength()+1];
						wsprintf((char *)buffer,"  %s %s", m_ProxyData.szProxyUser, m_ProxyData.szProxyPass);

						buffer[0] = 5;
						buffer[1] = static_cast<unsigned char>(m_ProxyData.szProxyUser.GetLength());
						ASSERT(m_ProxyData.szProxyPass.GetLength() <= 255);

						buffer[2+m_ProxyData.szProxyUser.GetLength()] = static_cast<unsigned char>(m_ProxyData.szProxyPass.GetLength());
						int len = 3 + m_ProxyData.szProxyUser.GetLength() + m_ProxyData.szProxyPass.GetLength();

						int res = CAsynSocket::Send(buffer, len);
						delete [] buffer;
						if (res == SOCKET_ERROR || res < len)
						{
							if (GetLastError() != WSAEWOULDBLOCK || res < len)
							{
								m_nProxyError = PROXYERROR_REQUESTFAILED;
								int ID = m_nProxyOpID;
								//Close();
								OnProxyOperationFailed(ID);

								ClearBuffer();
								return;
							}
						}
						ClearBuffer();
						m_nProxyOpState ++;
						return;
					}
				}
				//No auth needed
				//Send connection request
				char * command = new char[10+m_ProxyPeerHost.GetLength()+1];
				memset(command, 0, 10+m_ProxyPeerHost.GetLength()+1);
				command[0] = 5;
				command[1] = (m_nProxyOpID == PROXYOP_CONNECT) ? 1 : 2;
				command[2] = 0;
				command[3] = m_nProxyPeerIp ? 1 : 3;

				int len = 4;
				if (m_nProxyPeerIp)
				{
					memcpy(&command[len], &m_nProxyPeerIp, 4);
					len += 4;
				}
				else
				{
					command[len] = m_ProxyPeerHost.GetLength();
					strcpy(&command[len+1], m_ProxyPeerHost);
					len += m_ProxyPeerHost.GetLength()+1;
				}
				memcpy(&command[len], &m_nProxyPeerPort, 2);
				len += 2;
				int res = CAsynSocket::Send(command, len);
				delete [] command;
				
				if (res == SOCKET_ERROR || res < len)
				{
					if (GetLastError() != WSAEWOULDBLOCK || res < len)
					{
						m_nProxyError = PROXYERROR_REQUESTFAILED;
						int ID = m_nProxyOpID;
						//Close();
						OnProxyOperationFailed(ID);

						ClearBuffer();
						return;
					}
				}
				m_nProxyOpState += 2;
				ClearBuffer();
				return;
			}
			else 
			if (m_nProxyOpState==2)
			{
				//Response to the auth request
				if (!m_pRecvBuffer)
				{
					m_pRecvBuffer = new char[2];
					memset(m_pRecvBuffer, 0, 2);
				}
				int numread = CAsynSocket::Receive(m_pRecvBuffer+m_nRecvBufferPos, 2-m_nRecvBufferPos);				
				if (numread == SOCKET_ERROR)
				{
					if (GetLastError() != WSAEWOULDBLOCK)
					{
						m_nProxyError = PROXYERROR_AUTHFAILED;
						int ID = m_nProxyOpID;
						//Close();
						OnProxyOperationFailed(ID);
					}

					ClearBuffer();
					return;
				}
				m_nRecvBufferPos += numread;
				if (m_nRecvBufferPos == 2)
				{
					if (m_pRecvBuffer[1] != 0)
					{
						m_nProxyError = PROXYERROR_AUTHFAILED;
						int ID = m_nProxyOpID;
						//Close();
						OnProxyOperationFailed(ID);

						ClearBuffer();
						return;
					}
					char * command = new char[10+m_ProxyPeerHost.GetLength()];
					memset(command, 0, 10+m_ProxyPeerHost.GetLength()+1);
					command[0] = 5;
					command[1] = (m_nProxyOpID == PROXYOP_CONNECT) ? 1 : 2;
					command[2] = 0;
					command[3] = m_nProxyPeerIp ? 1 : 3;

					int len = 4;
					if (m_nProxyPeerIp)
					{
						memcpy(&command[len], &m_nProxyPeerIp, 4);
						len += 4;
					}
					else
					{
						command[len] = m_ProxyPeerHost.GetLength();
						strcpy(&command[len+1], m_ProxyPeerHost);
						len += m_ProxyPeerHost.GetLength()+1;
					}
					memcpy(&command[len], &m_nProxyPeerPort, 2);
					len += 2;

					int res = CAsynSocket::Send(command, len);
					if (res == SOCKET_ERROR || res < len)
					{
						if (GetLastError() != WSAEWOULDBLOCK || res < len)
						{
							m_nProxyError = PROXYERROR_REQUESTFAILED;
							int ID = m_nProxyOpID;
							//Close();
							OnProxyOperationFailed(ID);

							ClearBuffer();
							return;
						}
					}
					m_nProxyOpState ++;
					ClearBuffer();
					return;	
				}
			}
			else
			if (m_nProxyOpState == 3)
			{
				//Response to the connection request
				if (!m_pRecvBuffer)
				{
					m_pRecvBuffer = new char[10];
					memset(m_pRecvBuffer, 0, 10);
					m_nRecvBufferLen = 5;
				}
				int numread = CAsynSocket::Receive(m_pRecvBuffer+m_nRecvBufferPos, m_nRecvBufferLen-m_nRecvBufferPos);
				if (numread == SOCKET_ERROR)
				{
					if (GetLastError() != WSAEWOULDBLOCK)
					{
						m_nProxyError = PROXYERROR_REQUESTFAILED;
						int ID = m_nProxyOpID;
						//Close();
						OnProxyOperationFailed(ID);
					}

					ClearBuffer();
					return;
				}
				m_nRecvBufferPos += numread;
				if (m_nRecvBufferPos == m_nRecvBufferLen)
				{
					//Check for errors
					if (m_pRecvBuffer[1] != 0 || m_pRecvBuffer[0] != 5)
					{
						m_nProxyError = PROXYERROR_REQUESTFAILED;
						int ID = m_nProxyOpID;
						//Close();
						OnProxyOperationFailed(ID);

						ClearBuffer();
						return;
					}
					if (m_nRecvBufferLen == 5)
					{
						//Check which kind of address the response contains
						if (m_pRecvBuffer[3] == 1)
							m_nRecvBufferLen = 10;
						else
						{
							m_nRecvBufferLen += m_pRecvBuffer[4] + 2;
							char * tmp = new char[5];
							memcpy(tmp, m_pRecvBuffer, 5);
							
							delete m_pRecvBuffer;
							m_pRecvBuffer = new char[m_nRecvBufferLen+1];
							memcpy(m_pRecvBuffer, tmp, 5);
							delete tmp;
						}

						ClearBuffer();
						return;
					}
					
					if (m_nProxyOpID == PROXYOP_CONNECT)
					{
						//OK, we are connected with the remote server
						Reset();
						OnSend(0);
					}
					else
					{
						//Listen socket created
						m_nProxyOpState ++;
						unsigned long ip;
						int port;
						ASSERT(m_pRecvBuffer[3] == 1);
						memcpy(&ip, &m_pRecvBuffer[4], 4);
						memcpy(&port, &m_pRecvBuffer[8], 2);
						OnListenFinished(ip, port);
					}
					ClearBuffer();
				}
			}
			else
			if (m_nProxyOpState == 4)
			{
				if (!m_pRecvBuffer)
				{
					m_pRecvBuffer = new char[10];
					memset(m_pRecvBuffer, 0, 10);
				}

				int numread = CAsynSocket::Receive(m_pRecvBuffer+m_nRecvBufferPos, 10-m_nRecvBufferPos);
				if (numread == SOCKET_ERROR)
				{
					if (GetLastError() != WSAEWOULDBLOCK)
					{
						m_nProxyError = PROXYERROR_REQUESTFAILED;
						int ID = m_nProxyOpID;
						//Close();
						OnProxyOperationFailed(ID);
					}

					ClearBuffer();
					return;
				}
				m_nRecvBufferPos += numread;
				if (m_nRecvBufferPos == 10)
				{
					if (m_pRecvBuffer[1] != 0)
					{
						m_nProxyError = PROXYERROR_REQUESTFAILED;
						int ID = m_nProxyOpID;
						//Close();
						OnProxyOperationFailed(ID);

						ClearBuffer();
						return;
					}
					//Connection to remote server established
					ClearBuffer();
					Reset();
					OnAccept(0);
				}
			}
		}
		else
		if (m_ProxyData.nProxyType == PROXYTYPE_HTTP11)
		{
			char buffer[2];
			buffer[1]=0;
			for(;;)
			{
				int numread = CAsynSocket::Receive(buffer, 1);
				if (numread == SOCKET_ERROR)
				{
					if (GetLastError() != WSAEWOULDBLOCK)
					{
						m_nProxyError = PROXYERROR_REQUESTFAILED;
						int ID = m_nProxyOpID;
						//Close();
						OnProxyOperationFailed(ID);
					}

					ClearBuffer();
					return;
				}
				//Response begins with HTTP/
				m_strBuffer += buffer;
				CString start = "HTTP/";
				start = start.Left(m_strBuffer.GetLength());
				if (start != m_strBuffer.Left(start.GetLength()))
				{
					m_nProxyError = PROXYERROR_REQUESTFAILED;
					int ID = m_nProxyOpID;
					//Close();
					OnProxyOperationFailed(ID);

					ClearBuffer();
					return;
				}
				int pos = m_strBuffer.Find("\r\n");
				if (pos != -1)
				{
					int pos2 = m_strBuffer.Find(" ");
					if (pos2 == -1 || m_strBuffer[pos2+1] != '2' || pos2 > pos)
					{
						m_nProxyError = PROXYERROR_REQUESTFAILED;
						int ID = m_nProxyOpID;
						//Close();
						OnProxyOperationFailed(ID);

						ClearBuffer();
						return;
					}
				}
				if (m_strBuffer.Right(4) == "\r\n\r\n") //End of the HTTP header
				{
					Reset();
					ClearBuffer();
					return;
				}
			}
		}
	}
}
