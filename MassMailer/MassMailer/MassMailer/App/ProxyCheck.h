
#pragma once 

#include <afxsock.h>
#include <Afxmt.h>

class CProxyCheck
{
public:
	CProxyCheck();
	virtual ~CProxyCheck();

	CProxyCheck(CString,CString,CString);
	bool  Check();

private:

	bool	Checkout();

private:

	CSocket	CheckSock;
	long	ProxyPort;
	CString ProxyIpAdd;
	CString ProxyType;
	


};