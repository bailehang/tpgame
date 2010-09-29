// MySocket.cpp: Implementierung der Klasse CMySocket.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MySocket.h"
#include "../../Public/Base.h"
#include <string>
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CMySocket::CMySocket()
{

}

CMySocket::~CMySocket()
{

}

void CMySocket::OnReceive(int nErrorCode)
{
	CAsyncProxySocket::OnReceive(nErrorCode);
	if (nErrorCode)
	{
		AfxMessageBox("Fatal error! Network subsystem failed!",MB_ICONSTOP);
		Close();
		return;
	}

	CString   Response;
	for(;;)
	{
		char buffer;
		int res=Receive(&buffer,1);
		if (res==SOCKET_ERROR || !res)
		{
			if (GetLastError()!=WSAEWOULDBLOCK || !res)
			{
				AfxMessageBox("Error! Connection has been closed!",MB_ICONEXCLAMATION);
				return;
			}
			break;
		}
		Response+=buffer;
		//m_pResponse->SetWindowText(str);
	}

	int  len  =	 strlen("AUTH LOGIN\r\n") ;
	int res=CAsyncSocket::Send("AUTH LOGIN\r\n",len);
	if (res==SOCKET_ERROR || res<len)
	{
		if (GetLastError()!=WSAEWOULDBLOCK || res<len)
		{
			//m_nProxyError=PROXYERROR_REQUESTFAILED;
			//int ID=m_nProxyOpID;
			Close();
			//OnProxyOperationFailed(ID);
			return;
		}
	}

	CBase base64;
	//发送经base64编码的用户名
	string strUserName=base64.Encode((unsigned char *)"tangpeng918",strlen("tangpeng918"))+"\r\n";

	if( CAsyncProxySocket::Send(strUserName.c_str(),strUserName.length() ) ==  SOCKET_ERROR) 
	{
		if (GetLastError()!=WSAEWOULDBLOCK || res<len)
		{
			//m_nProxyError=PROXYERROR_REQUESTFAILED;
			//int ID=m_nProxyOpID;
			Close();
			//OnProxyOperationFailed(ID);
			return;
		}
	}


	if(  CAsyncProxySocket::Send("tangpeng",strlen("tangpeng") ) ==  SOCKET_ERROR) 
	{
		if (GetLastError()!=WSAEWOULDBLOCK || res<len)
		{
			//m_nProxyError=PROXYERROR_REQUESTFAILED;
			//int ID=m_nProxyOpID;
			Close();
			//OnProxyOperationFailed(ID);
			return;
		}
	}
}


void CMySocket::OnProxyOperationFailed(int nOpID)
{
	AfxMessageBox("Error! Could not connect through proxy!",MB_ICONEXCLAMATION);
}

void CMySocket::OnListenFinished(unsigned long &retProxyIp,int &retProxyPort)
{
}
	
void CMySocket::OnConnect(int nErrorCode)
{
	CAsyncProxySocket::OnConnect(nErrorCode);
	if (nErrorCode)
	{
		if (GetProxyType())
			AfxMessageBox("Error! Could not connect to proxy server!",MB_ICONEXCLAMATION);
		else
			AfxMessageBox("Error! Could not connect to server!",MB_ICONEXCLAMATION);
		Close();
	}
}
