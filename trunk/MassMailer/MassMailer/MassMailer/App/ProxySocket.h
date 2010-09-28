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
void SetProxy(int nProxyType, CString ProxyHost, int nProxyPort);
void SetProxy(int nProxyType, CString ProxyHost, int nProxyPort,CString ProxyUser, CString ProxyPass);
	
Call one of this functions to set the proxy type.
Parametes:
- nProxyType specifies the Proxy Type.
- ProxyHost and nProxyPort specify the address of the proxy
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

#if !defined(AFX_ASYNCSOCKSIFIEDSOCKET_H__9699EDE5_B447_4AF6_8E5D_A1580015E4BF__INCLUDED_)
#define AFX_ASYNCSOCKSIFIEDSOCKET_H__9699EDE5_B447_4AF6_8E5D_A1580015E4BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AsyncSocksifiedSocket.h : Header-Datei
//

#include "AsynSocket.h"

#define PROXYOP_CONNECT					1
#define PROXYOP_LISTEN					2

//Errorcodes
#define PROXYERROR_NOERROR				0
#define PROXYERROR_NOCONN				1 //Can't connect to proxy server, use GetLastError for more information
#define PROXYERROR_REQUESTFAILED		2 //Request failed, can't send data
#define PROXYERROR_AUTHREQUIRED			3 //Authentication required
#define PROXYERROR_AUTHTYPEUNKNOWN		4 //Authtype unknown or not supported
#define PROXYERROR_AUTHFAILED			5 //Authentication failed
#define PROXYERROR_AUTHNOLOGON			6
#define PROXYERROR_CANTRESOLVEHOST		7

//Proxytypes
#define PROXYTYPE_NOPROXY				0
#define PROXYTYPE_SOCKS4				1
#define PROXYTYPE_SOCKS4A				2
#define PROXYTYPE_SOCKS5				3
#define PROXYTYPE_HTTP11				4

/////////////////////////////////////////////////////////////////////////////
// Befehlsziel CProxySocket 
class CProxySocket : public CAsynSocket
{
// Attribute
protected:
	/////////////////////////////////////////////
	//	Socks代理，支持SOCKS4/4A,socks5,HTTP1.1
	/////////////////////////////////////////////
	class CSocksData
	{
	public:
		CSocksData()
		{
			nProxyType	=	PROXYTYPE_NOPROXY;
		}

		int		nProxyType;
		CString szProxyHost;
		int		nProxyPort;
		CString szProxyUser;
		CString	szProxyPass;
		BOOL	bUseLogon;
	};

// Operationen
public:
	CProxySocket();
	virtual ~CProxySocket();

	BOOL	IsConnectToProxy(){return m_bConnectProxy;}

// 躡erschreibungen
public:
	virtual void Close();
	virtual BOOL Listen();
	virtual BOOL Listen(unsigned long serverIp);

	void SetProxy(int nProxyType); //No proxy
	void SetProxy(int nProxyType, LPCTSTR lpszProxyHost, int nProxyPort);
	void SetProxy(int nProxyType, LPCTSTR lpszProxyHost, int nProxyPort, LPCTSTR lpszProxyUser, LPCTSTR lpszProxyPass); //Only SOCKS5 proxy
	//Sets the proxy details. 
	//nProxyType - Type of the proxy. May be PROXYTYPE_NOPROXY,PROXYTYPE_SOCKS4 and PROXYTYPE_SOCKS5
	//ProxyHost - The address of the proxy. Can be either IP or URL
	//ProxyPort - The port of the proxy
	//ProxyUser - the username for SOCKS5 proxies
	//ProxyPass - the password for SOCKS5 proxies
	
	int GetProxyType() const;
	//Returns the type of the proxy

	//是否使用代理服务器
	BOOL IsSocksified(){return m_ProxyData.nProxyType;}

	const int GetLastProxyError() const;
	//Returns the last error occured in CProxySocket

	BOOL GetProxyPeerName( CString& rPeerAddress, UINT& rPeerPort );
	BOOL GetProxyPeerName( SOCKADDR* lpSockAddr, int* lpSockAddrLen );
	//Returns the address of the server behind the SOCKS proxy you are connected to
	
	// Vom Klassen-Assistenten generierte virtuelle Funktions黚erschreibungen
	//{{AFX_VIRTUAL(CProxySocket)
	public:
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
	virtual int Receive(void* lpBuf, int nBufLen, int nFlags = 0);
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CProxySocket)
		// HINWEIS - Der Klassen-Assistent f黦t hier Member-Funktionen ein und entfernt diese.
	//}}AFX_MSG

protected:
	void Reset();
	void ClearBuffer();		 //Clears the receive buffer

	virtual void OnResolve(int nErrorCode);
	virtual BOOL ConnectHelper(LPCTSTR lpszHostAddress, UINT nHostPort);
	virtual BOOL ConnectHelper(const SOCKADDR * lpSockAddr, int nSockAddrLen);

	virtual void OnProxyOperationFailed(int nOpID) = 0;
	virtual void OnListenFinished(unsigned long & retProxyIp, int & retProxyPort) = 0;

	///连接代理服务器成功
	virtual void OnProxyConnectCompleted(int nErrorCode){}

// Implementierung
protected:
	char *			m_pRecvBuffer;		//The receive buffer
	int				m_nRecvBufferLen;	//Length of the RecvBuffer
	int				m_nRecvBufferPos;	//Position within the receive buffer
	CString			m_strBuffer;		//Recvbuffer needed by HTTP1.1 proxy
	int				m_nProxyOpState;	//State of an operation
	int				m_nProxyOpID;		//Currently active operation (0 if none)
	CString			m_ProxyPeerHost;
	int				m_nProxyPeerPort;	//Port of the server you are connected to, retrieve via GetProxyPeerName
	ULONG			m_nProxyPeerIp;		//IP of the server you are connected to, retrieve via GetProxyPeerName
	int				m_nProxyError;		//The latest error
	CSocksData		m_ProxyData;		//Structure to hold the data set by SetProxy
	BOOL			m_bConnectProxy;	//是否连接到代理服务器，默认为FALSE

	//连接的目标主机
	CString			m_szTempHost;
	UINT			m_nTempPort;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f黦t unmittelbar vor der vorhergehenden Zeile zus鋞zliche Deklarationen ein.

#endif // AFX_ASYNCSOCKSIFIEDSOCKET_H__9699EDE5_B447_4AF6_8E5D_A1580015E4BF__INCLUDED_
