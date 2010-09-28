// MainSocket.cpp : implementation file
//

#include "stdafx.h"
#include "MainSocket.h"


/////////////////////////////////////////////////////////////////////////////
// CMainSocket

CMainSocket::CMainSocket()
{
	
}

CMainSocket::~CMainSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CMainSocket, CProxySocket)
	//{{AFX_MSG_MAP(CMainSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CMainSocket member functions

void CMainSocket::SetFtpProxy(int nProxyType, LPCTSTR lpszProxyHost, int nProxyPort, BOOL bLogon, LPCTSTR lpszProxyUser, LPCTSTR lpszProxyPass)
{
	m_FtpProxyData.nProxyType	=	nProxyType;
	m_FtpProxyData.szProxyHost	=	lpszProxyHost;
	m_FtpProxyData.nProxyPort	=	nProxyPort;
	m_FtpProxyData.bUseLogon	=	bLogon;
	m_FtpProxyData.szProxyUser	=	lpszProxyUser;
	m_FtpProxyData.szProxyPass	=	lpszProxyPass;
}

void CMainSocket::OnResolve(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CProxySocket::OnResolve(nErrorCode);
	//m_pMainFrame->OnResolveHostname(nErrorCode);
}

void CMainSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CProxySocket::OnReceive(nErrorCode);
	//m_pMainFrame->OnMainSocketReceive(nErrorCode);
}

void CMainSocket::OnConnect(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CProxySocket::OnConnect(nErrorCode);
	//m_pMainFrame->OnMainSocketConnect(nErrorCode);
}

void CMainSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CProxySocket::OnClose(nErrorCode);
	//m_pMainFrame->OnMainSocketClose(nErrorCode);
}

void CMainSocket::OnProxyConnectCompleted(int nErrorCode)
{
	//连接代理服务器成功
	//m_pMainFrame->OnProxyConnectCompleted(nErrorCode);
}

void CMainSocket::OnProxyOperationFailed(int nOpID)
{
	//m_pMainFrame->OnProxyOperationFailed(nOpID);
}

void CMainSocket::OnListenFinished(unsigned long & retProxyIp, int & retProxyPort)
{
}

BOOL CMainSocket::ConnectHelper(LPCTSTR lpszHostAddress, UINT nHostPort)
{
	//使用FTP代理连接
	if(m_FtpProxyData.nProxyType)
	{
		m_szFwServer	=	m_FtpProxyData.szProxyHost;
		m_nFwPort		=	m_FtpProxyData.nProxyPort;

		//if(m_nFwPort != 21)
		//	m_szFwServer.Format("%s:%d", (LPCTSTR)m_szFwServer, m_nFwPort);
	}
	else
	{
		m_szFwServer	=	lpszHostAddress;
		m_nFwPort		=	nHostPort;
	}
	//通过防火墙连接
	return CProxySocket::ConnectHelper(m_szFwServer, m_nFwPort);
}
