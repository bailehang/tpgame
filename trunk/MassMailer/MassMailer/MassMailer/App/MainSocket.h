#if !defined(AFX_MAINSOCKET_H__B18909E8_90B4_4494_81CB_CFF84B6A4CB8__INCLUDED_)
#define AFX_MAINSOCKET_H__B18909E8_90B4_4494_81CB_CFF84B6A4CB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainSocket.h : header file
//

#include "ProxySocket.h"

#define	LO											-2
#define	ER											-1

//ProxyType
#define	PROXYTYPE_NONE								0	// no firewall
#define PROXYTYPE_SITE_HOSTNAME						1	// SITE hostname
#define	PROXYTYPE_USER_AFTER_LOGON					2	// USER after logon
#define	PROXYTYPE_PROXY_OPEN						3	// proxy OPEN
#define	PROXYTYPE_TRANSPARENT						4	// Transparent
#define	PROXYTYPE_USER_WITH_NO_LOGON				5	// USER with no logon
#define	PROXYTYPE_USER_FIRID_REMOTEHOST				6	// USER fireID@remotehost
#define	PROXYTYPE_USER_REMOTEID_REMOTEHOST_FIREID	7	// USER remoteID@remotehost fireID
#define	PROXYTYPE_USER_REMOTEID_FIREID_REMOTEHOST	8	// USER remoteID@fireID@remotehost

/////////////////////////////////////////////////////////////////////////////
// CMainSocket command target
class CMainFrame;
class CMainSocket : public CProxySocket
{
	friend class CMainFrame;
// Attributes
public:
	//////////////////////////////
	//	FTP代理支持用于登录主机
	//////////////////////////////
	class CFtpProxyData
	{
	public:
		CFtpProxyData()
		{
			nProxyType	=	PROXYTYPE_NONE;
			nProxyPort	=	21;
			nOpState	=	-1;
			bUseLogon	=	FALSE;

			//LoadSetting();
		}
		virtual ~CFtpProxyData()
		{
		}

		int			nProxyType;			//代理类型
		CString		szProxyHost;		//代理服务器地址
		int			nProxyPort;			//代理服务器端口
		CString		szProxyUser;		//代理服务器验证帐号
		CString		szProxyPass;		//代理服务器验证密码
		int			nOpState;			//当前状态
		BOOL		bUseLogon;			//是否需要登录
	};

// Operations
public:
	CMainSocket();
	virtual ~CMainSocket();

	void SetFtpProxy(int nProxyType, LPCTSTR lpszProxyHost, int nProxyPort, BOOL bLogon = FALSE, LPCTSTR lpszProxyUser = NULL, LPCTSTR lpszProxyPass = NULL);

protected:
	virtual BOOL ConnectHelper(LPCTSTR lpszHostAddress, UINT nHostPort);
	virtual void OnProxyConnectCompleted(int nErrorCode);

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainSocket)
	public:
	virtual void OnResolve(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnProxyOperationFailed(int nOpID);
	virtual void OnListenFinished(unsigned long &retProxyIp,int &retProxyPort);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CMainSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	CMainFrame	*	m_pMainFrame;
	CFtpProxyData 	m_FtpProxyData;

	CString			m_szFwServer;			//连接的主机名
	UINT			m_nFwPort;				//连接端口
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINSOCKET_H__B18909E8_90B4_4494_81CB_CFF84B6A4CB8__INCLUDED_)
