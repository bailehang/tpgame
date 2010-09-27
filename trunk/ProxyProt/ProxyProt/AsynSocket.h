// AsynSocket.h: interface for the CAsynSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASYNSOCKET_H__188D4D8F_7958_490B_B082_089700BFFB1B__INCLUDED_)
#define AFX_ASYNSOCKET_H__188D4D8F_7958_490B_B082_089700BFFB1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_SOCKET_NOTIFY				0x0373

class CAsynSocket : public CObject
{
public:
	CAsynSocket();
	virtual BOOL Create(UINT nSocketPort = 0, int nSocketType = SOCK_STREAM, 
		long lEvent = FD_READ|FD_WRITE|FD_OOB|FD_ACCEPT|FD_CONNECT|FD_CLOSE, LPCTSTR lpszSocketAddress = NULL);

protected:
	class CSocketWnd : public CWnd
	{
		friend class CAsynSocket;
	public:
		CSocketWnd(CAsynSocket * pSocket)
		{
			m_pSocket		= pSocket;
			m_bResolved		= FALSE;		//已经完全成功解析了主机名为地址，可以直接连接
			m_hAsyncGetHostByName = NULL;
		}
		~CSocketWnd()
		{
			if(m_hAsyncGetHostByName)
			{
				WSACancelAsyncRequest(m_hAsyncGetHostByName);
				m_hAsyncGetHostByName	=	NULL;
			}
		}

	private:
		CAsynSocket	*	m_pSocket;
		BOOL			m_bResolved;
		HANDLE			m_hAsyncGetHostByName;

	protected:
		afx_msg LRESULT OnWinsocketNotify(WPARAM wParam, LPARAM lParam);
		DECLARE_MESSAGE_MAP()
	};

// Attributes
public:
	SOCKET m_hSocket;
	BOOL Attach(SOCKET hSocket, long lEvent = FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE);

	BOOL GetPeerName(CString& rPeerAddress, UINT& rPeerPort);
	BOOL GetPeerName(SOCKADDR* lpSockAddr, int* lpSockAddrLen);

	BOOL GetSockName(CString& rSocketAddress, UINT& rSocketPort);
	BOOL GetSockName(SOCKADDR* lpSockAddr, int* lpSockAddrLen);

	BOOL SetSockOpt(int nOptionName, const void* lpOptionValue,	int nOptionLen, int nLevel = SOL_SOCKET);
	BOOL GetSockOpt(int nOptionName, void* lpOptionValue, int* lpOptionLen, int nLevel = SOL_SOCKET);

	// Operations
protected:
	virtual BOOL ConnectHelper(LPCTSTR lpszHostAddress, UINT nHostPort);
	virtual BOOL ConnectHelper(const SOCKADDR * lpSockAddr, int nSockAddrLen);

public:
	virtual BOOL Accept(CAsynSocket & rConnectedSocket,	SOCKADDR* lpSockAddr = NULL, int* lpSockAddrLen = NULL);

	BOOL Bind(UINT nSocketPort, LPCTSTR lpszSocketAddress = NULL);
	BOOL Bind (const SOCKADDR* lpSockAddr, int nSockAddrLen);

	virtual void Close();

	BOOL Connect(LPCTSTR lpszHostAddress, UINT nHostPort);
	BOOL Connect();
	BOOL Connect(const SOCKADDR * lpSockAddr, int nSockAddrLen);

	BOOL IOCtl(long lCommand, DWORD* lpArgument);

	BOOL Listen(int nConnectionBacklog=5);

	virtual int Receive(void* lpBuf, int nBufLen, int nFlags = 0);

	int ReceiveFrom(void* lpBuf, int nBufLen, CString& rSocketAddress, UINT& rSocketPort, int nFlags = 0);
	int ReceiveFrom(void* lpBuf, int nBufLen, SOCKADDR* lpSockAddr, int* lpSockAddrLen, int nFlags = 0);

	enum { receives = 0, sends = 1, both = 2 };
	BOOL ShutDown(int nHow = sends);

	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);

	int SendTo(const void* lpBuf, int nBufLen, UINT nHostPort, LPCTSTR lpszHostAddress = NULL, int nFlags = 0);
	int SendTo(const void* lpBuf, int nBufLen, const SOCKADDR* lpSockAddr, int nSockAddrLen, int nFlags = 0);

	BOOL AsyncSelect(long lEvent = FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE);

protected:
	void _OnResolve(int nErrorCode);
	void _OnConnect(int nErrorCode);
	void _OnClose(int nErrorCode);
	void _OnAccept(int nErrorCode);
	void _OnReceive(int nErrorCode);
	void _OnSend(int nErrorCode);
	void _OnAttach();
	void _OnOOBData(int nErrorCode);

	virtual void OnResolve(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnAccept(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnAttach();
	virtual void OnClear();
	virtual void OnOOBData(int nErrorCode);

// Implementation
public:
	virtual ~CAsynSocket();
	BOOL Socket(int nSocketType=SOCK_STREAM, long lEvent =
		FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE,
		int nProtocolType = 0, int nAddressFormat = PF_INET);

private:
	CSocketWnd	*	m_pSocketWnd;
	char			m_szHostBuffer[MAXGETHOSTSTRUCT+1];
	SOCKADDR_IN		m_sockAddr;

	friend CAsynSocket::CSocketWnd;
};

#endif // !defined(AFX_ASYNSOCKET_H__188D4D8F_7958_490B_B082_089700BFFB1B__INCLUDED_)
