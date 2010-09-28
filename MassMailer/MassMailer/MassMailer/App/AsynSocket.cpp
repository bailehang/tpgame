// AsynSocket.cpp: implementation of the CAsynSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AsynSocket.h"
#include "Base64coding.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CAsynSocket::CSocketWnd, CWnd)
	//{{AFX_MSG_MAP(CAsynSocket::CSocketWnd)
	ON_MESSAGE(WM_SOCKET_NOTIFY, &CAsynSocket::CSocketWnd::OnWinsocketNotify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

LRESULT CAsynSocket::CSocketWnd::OnWinsocketNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 0 && lParam == 0)
		return 0;

	int nErrorCode = WSAGETSELECTERROR(lParam);
	if(m_bResolved && m_hAsyncGetHostByName == (HANDLE)wParam)	//需要解析主机名
	{
		m_hAsyncGetHostByName	=	NULL;
		m_bResolved				=	FALSE;

		m_pSocket->_OnResolve(nErrorCode);
	}
	else	//已经获取到了主机名，是其他Socket消息
	{
		if((SOCKET)wParam!=m_pSocket->m_hSocket)
			return 0;

		//////////////////////////////////////////////////////////////////
		//lParam = 658833440;	在有proxy的环境容易发生此类消息进行过滤
		//////////////////////////////////////////////////////////////////
		UINT nEvent = WSAGETSELECTEVENT(lParam);
		if(nEvent==FD_CLOSE && nErrorCode==WSAECONNABORTED)
			return 0;

		switch (nEvent)
		{
		case FD_READ:				//读
			{
				DWORD nBytes;
				if (!m_pSocket->IOCtl(FIONREAD, &nBytes))
					nErrorCode = WSAGetLastError();
				if (nBytes != 0 || nErrorCode != 0)
					m_pSocket->_OnReceive(nErrorCode);
			}
			break;
		case FD_WRITE:				//写
			m_pSocket->_OnSend(nErrorCode);
			break;
		case FD_OOB:
			m_pSocket->_OnOOBData(nErrorCode);
			break;
		case FD_ACCEPT:				//接收
			m_pSocket->_OnAccept(nErrorCode);
			break;
		case FD_CONNECT:			//连接
			m_pSocket->_OnConnect(nErrorCode);
			break;
		case FD_CLOSE:				//关闭
			m_pSocket->_OnClose(nErrorCode);
			break;
		default:
			break;
		}
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAsynSocket::CAsynSocket()
{
	m_pSocketWnd		=	NULL;
	m_hSocket			=	INVALID_SOCKET;
	memset(&m_szHostBuffer, 0, sizeof(m_szHostBuffer));
	m_sockAddr.sin_addr.s_addr = INADDR_NONE;
}

CAsynSocket::~CAsynSocket()
{
}

BOOL CAsynSocket::Create(UINT nSocketPort, int nSocketType, long lEvent, LPCTSTR lpszSocketAddress)
{
	m_pSocketWnd = new CSocketWnd(this);
	m_pSocketWnd->m_hWnd = NULL;
	if (!m_pSocketWnd->CreateEx(0, AfxRegisterWndClass(0), 
		_T(""), WS_OVERLAPPED, 0, 0, 0, 0, NULL, NULL))
	{
		TRACE0("Warning: unable to create socket notify window!\n");
		return FALSE;
	}
	if (Socket(nSocketType, lEvent))
	{
		if (Bind(nSocketPort,lpszSocketAddress))
			return TRUE;
		int nResult = GetLastError();
		Close();
		WSASetLastError(nResult);
	}
	return FALSE;
}

BOOL CAsynSocket::ConnectHelper(LPCTSTR lpszHostAddress, UINT nHostPort)
{
	ASSERT(lpszHostAddress != NULL);
	memset(&m_sockAddr, 0, sizeof(m_sockAddr));

	LPSTR lpszAscii = (LPTSTR)lpszHostAddress;
	m_sockAddr.sin_family			=	AF_INET;
	m_sockAddr.sin_port				=	htons((u_short)nHostPort);
	m_sockAddr.sin_addr.s_addr		=	inet_addr(lpszAscii);

	if(m_pSocketWnd->m_bResolved)
		m_pSocketWnd->m_bResolved	=	FALSE;

	if(m_pSocketWnd->m_hAsyncGetHostByName)
	{
		::WSACancelAsyncRequest(m_pSocketWnd->m_hAsyncGetHostByName);
		m_pSocketWnd->m_hAsyncGetHostByName = NULL;
	}

	if (m_sockAddr.sin_addr.s_addr == INADDR_NONE)
	{
		m_pSocketWnd->m_bResolved	=	TRUE;
		m_pSocketWnd->m_hAsyncGetHostByName = ::WSAAsyncGetHostByName(m_pSocketWnd->m_hWnd, WM_SOCKET_NOTIFY, lpszAscii, m_szHostBuffer, MAXGETHOSTSTRUCT);
		return TRUE;
	}
	else
	{
		m_pSocketWnd->m_bResolved	=	FALSE;
		return ConnectHelper((SOCKADDR*)&m_sockAddr, sizeof(m_sockAddr));
	}
}

BOOL CAsynSocket::GetPeerName(CString& rPeerAddress, UINT& rPeerPort)
{
	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = GetPeerName((SOCKADDR*)&sockAddr, &nSockAddrLen);
	if (bResult)
	{
		rPeerPort = ntohs(sockAddr.sin_port);
		rPeerAddress = inet_ntoa(sockAddr.sin_addr);
	}
	return bResult;
}

BOOL CAsynSocket::GetSockName(CString & rSocketAddress, UINT& rSocketPort)
{
	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = GetSockName((SOCKADDR*)&sockAddr, &nSockAddrLen);
	if (bResult)
	{
		rSocketPort = ntohs(sockAddr.sin_port);
		rSocketAddress = inet_ntoa(sockAddr.sin_addr);
	}
	return bResult;
}

void CAsynSocket::Close()
{
	//关闭窗口
	if(m_pSocketWnd)
	{
		m_pSocketWnd->DestroyWindow();
		delete m_pSocketWnd;
		m_pSocketWnd = NULL;
	}

	//断开连接
	if (m_hSocket != INVALID_SOCKET)
	{
		ShutDown();

		VERIFY(SOCKET_ERROR != closesocket(m_hSocket));
		m_hSocket = INVALID_SOCKET;
	}
}

BOOL CAsynSocket::Socket(int nSocketType, long lEvent,	int nProtocolType, int nAddressFormat)
{
	ASSERT(m_hSocket == INVALID_SOCKET);

	m_hSocket = socket(nAddressFormat,nSocketType,nProtocolType);
	if (m_hSocket != INVALID_SOCKET)
		return AsyncSelect(lEvent);

	return FALSE;
}

BOOL CAsynSocket::AsyncSelect(long lEvent)
{
	ASSERT(m_hSocket != INVALID_SOCKET);
	return WSAAsyncSelect(m_hSocket, m_pSocketWnd->m_hWnd, WM_SOCKET_NOTIFY, lEvent) != SOCKET_ERROR;
}

BOOL CAsynSocket::Attach(SOCKET hSocket, long lEvent)
{
	ASSERT(hSocket != INVALID_SOCKET);

	m_hSocket = hSocket;

	m_pSocketWnd = new CSocketWnd(this);
	m_pSocketWnd->m_hWnd = NULL;
	if (!m_pSocketWnd->CreateEx(0, AfxRegisterWndClass(0), 
		_T(""), WS_OVERLAPPED, 0, 0, 0, 0, NULL, NULL))
	{
		TRACE0("Warning: unable to create socket notify window!\n");
		return FALSE;
	}

	return AsyncSelect(lEvent);
}

BOOL CAsynSocket::Accept(CAsynSocket & rConnectedSocket, SOCKADDR * lpSockAddr, int* lpSockAddrLen)
{
	return TRUE;
}

int CAsynSocket::ReceiveFrom(void* lpBuf, int nBufLen, CString& rSocketAddress, UINT& rSocketPort, int nFlags)
{
	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	int nSockAddrLen = sizeof(sockAddr);
	int nResult = ReceiveFrom(lpBuf, nBufLen, (SOCKADDR*)&sockAddr, &nSockAddrLen, nFlags);
	if(nResult != SOCKET_ERROR)
	{
		rSocketPort = ntohs(sockAddr.sin_port);
		rSocketAddress = inet_ntoa(sockAddr.sin_addr);
	}
	return nResult;
}

int CAsynSocket::SendTo(const void* lpBuf, int nBufLen, UINT nHostPort, LPCTSTR lpszHostAddress, int nFlags)
{
	//USES_CONVERSION;

	SOCKADDR_IN sockAddr;
	memset(&sockAddr,0,sizeof(sockAddr));

	//LPSTR lpszAscii = T2A((LPTSTR)lpszHostAddress);
	LPSTR lpszAscii = (LPTSTR)lpszHostAddress;
	sockAddr.sin_family = AF_INET;

	if (lpszAscii == NULL)
		sockAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	else
	{
		sockAddr.sin_addr.s_addr = inet_addr(lpszAscii);
		if (sockAddr.sin_addr.s_addr == INADDR_NONE)
		{
			LPHOSTENT lphost;
			lphost = gethostbyname(lpszAscii);
			if (lphost != NULL)
				sockAddr.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
			else
			{
				WSASetLastError(WSAEINVAL);
				return SOCKET_ERROR;
			}
		}
	}

	sockAddr.sin_port = htons((u_short)nHostPort);

	return SendTo(lpBuf, nBufLen, (SOCKADDR*)&sockAddr, sizeof(sockAddr), nFlags);
}


BOOL CAsynSocket::Bind(UINT nSocketPort, LPCTSTR lpszSocketAddress)
{
	//USES_CONVERSION;

	SOCKADDR_IN sockAddr;
	memset(&sockAddr,0,sizeof(sockAddr));

	//LPSTR lpszAscii = T2A((LPTSTR)lpszSocketAddress);
	LPSTR lpszAscii = (LPTSTR)lpszSocketAddress;
	sockAddr.sin_family = AF_INET;

	if (lpszAscii == NULL)
		sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
	{
		DWORD lResult = inet_addr(lpszAscii);
		if (lResult == INADDR_NONE)
		{
			WSASetLastError(WSAEINVAL);
			return FALSE;
		}
		sockAddr.sin_addr.s_addr = lResult;
	}

	sockAddr.sin_port = htons((u_short)nSocketPort);

	return Bind((SOCKADDR*)&sockAddr, sizeof(sockAddr));
}

void CAsynSocket::_OnResolve(int nErrorCode)
{
	if(nErrorCode == 0)
	{
		hostent	* he = (hostent*)m_szHostBuffer;
		m_sockAddr.sin_addr.s_addr = *((u_long*)he->h_addr_list[0]);

		memset(&m_szHostBuffer, 0, sizeof(m_szHostBuffer));
	}
	else
		WSASetLastError(WSAEINVAL);

	OnResolve(nErrorCode);
}

void CAsynSocket::_OnConnect(int nErrorCode)
{
	OnConnect(nErrorCode);
}

void CAsynSocket::_OnClose(int nErrorCode)
{
	AsyncSelect(0);

	if(m_pSocketWnd)
	{
		m_pSocketWnd->DestroyWindow();
		delete m_pSocketWnd;
		m_pSocketWnd = NULL;
	}	

	OnClose(nErrorCode);
}

void CAsynSocket::_OnAccept(int nErrorCode)
{
	OnAccept(nErrorCode);
}

void CAsynSocket::_OnReceive(int nErrorCode)
{
	OnReceive(nErrorCode);
}

void CAsynSocket::_OnSend(int nErrorCode)
{
	OnSend(nErrorCode);
}

void CAsynSocket::_OnAttach()
{
	OnAttach();
}

void CAsynSocket::_OnOOBData(int nErrorCode)
{
	OnOOBData(nErrorCode);
}

void CAsynSocket::OnResolve(int nErrorCode)
{
}

void CAsynSocket::OnConnect(int nErrorCode)
{
}

void CAsynSocket::OnClose(int nErrorCode)
{
}

void CAsynSocket::OnAccept(int nErrorCode)
{
}

void CAsynSocket::OnReceive(int nErrorCode)
{
}

void CAsynSocket::OnSend(int nErrorCode)
{
}

void CAsynSocket::OnAttach()
{
}

void CAsynSocket::OnClear()
{
}

void CAsynSocket::OnOOBData(int nErrorCode)
{
}

BOOL CAsynSocket::Connect(LPCTSTR lpszHostAddress, UINT nHostPort)
{
	return ConnectHelper(lpszHostAddress, nHostPort);
}

BOOL CAsynSocket::Connect(const SOCKADDR * lpSockAddr, int nSockAddrLen)
{
	return ::connect(m_hSocket, lpSockAddr, nSockAddrLen);
}

BOOL CAsynSocket::Connect()
	{ return ConnectHelper((SOCKADDR*)&m_sockAddr, sizeof(m_sockAddr)); }

BOOL CAsynSocket::ConnectHelper(const SOCKADDR* lpSockAddr, int nSockAddrLen)
{ 
	return Connect(lpSockAddr, nSockAddrLen);
}

int CAsynSocket::SendTo(const void* lpBuf, int nBufLen, const SOCKADDR* lpSockAddr, int nSockAddrLen, int nFlags)
	{ return ::sendto(m_hSocket, (LPSTR)lpBuf, nBufLen, nFlags, lpSockAddr, nSockAddrLen); }

int CAsynSocket::Send(const void* lpBuf, int nBufLen, int nFlags)
	{ return ::send(m_hSocket, (LPSTR)lpBuf, nBufLen, nFlags); }

int CAsynSocket::Receive(void* lpBuf, int nBufLen, int nFlags)
	{ return ::recv(m_hSocket, (LPSTR)lpBuf, nBufLen, nFlags); }

BOOL CAsynSocket::Bind(const SOCKADDR* lpSockAddr, int nSockAddrLen)
	{ return (SOCKET_ERROR != ::bind(m_hSocket, lpSockAddr, nSockAddrLen)); }

BOOL CAsynSocket::Listen(int nConnectionBacklog)
	{ return (SOCKET_ERROR != ::listen(m_hSocket, nConnectionBacklog)); }

BOOL CAsynSocket::GetPeerName(SOCKADDR* lpSockAddr, int* lpSockAddrLen)
	{ return (SOCKET_ERROR != ::getpeername(m_hSocket, lpSockAddr, lpSockAddrLen)); }

BOOL CAsynSocket::GetSockName(SOCKADDR* lpSockAddr, int* lpSockAddrLen)
	{ return (SOCKET_ERROR != ::getsockname(m_hSocket, lpSockAddr, lpSockAddrLen)); }

BOOL CAsynSocket::GetSockOpt(int nOptionName, void* lpOptionValue, int* lpOptionLen, int nLevel)
	{ return (SOCKET_ERROR != ::getsockopt(m_hSocket, nLevel, nOptionName, (LPSTR)lpOptionValue, lpOptionLen)); }

BOOL CAsynSocket::SetSockOpt(int nOptionName, const void* lpOptionValue, int nOptionLen, int nLevel)
	{ return (SOCKET_ERROR != ::setsockopt(m_hSocket, nLevel, nOptionName, (LPCSTR)lpOptionValue, nOptionLen)); }

BOOL CAsynSocket::IOCtl(long lCommand, DWORD* lpArgument)
	{ return (SOCKET_ERROR != ::ioctlsocket(m_hSocket, lCommand, lpArgument)); }

int CAsynSocket::ReceiveFrom(void* lpBuf, int nBufLen, SOCKADDR* lpSockAddr, int* lpSockAddrLen, int nFlags)
	{ 	return ::recvfrom(m_hSocket, (LPSTR)lpBuf, nBufLen, nFlags, lpSockAddr, lpSockAddrLen); }

BOOL CAsynSocket::ShutDown(int nHow)
	{ return (SOCKET_ERROR != ::shutdown(m_hSocket,nHow)); }
