// DlgProxy.cpp : 实现文件
//

#include "stdafx.h"
#include "SendMail.h"
#include "DlgProxy.h"
#include "SendMailDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSendMailDlgAutoProxy

IMPLEMENT_DYNCREATE(CSendMailDlgAutoProxy, CCmdTarget)

CSendMailDlgAutoProxy::CSendMailDlgAutoProxy()
{
	EnableAutomation();
	
	// 为使应用程序在自动化对象处于活动状态时一直保持 
	//	运行，构造函数调用 AfxOleLockApp。
	AfxOleLockApp();

	// 通过应用程序的主窗口指针
	//  来访问对话框。设置代理的内部指针
	//  指向对话框，并设置对话框的后向指针指向
	//  该代理。
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CSendMailDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CSendMailDlg)))
		{
			m_pDialog = reinterpret_cast<CSendMailDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CSendMailDlgAutoProxy::~CSendMailDlgAutoProxy()
{
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	// 	析构函数调用 AfxOleUnlockApp。
	//  除了做其他事情外，这还将销毁主对话框
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CSendMailDlgAutoProxy::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清理代码。

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CSendMailDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CSendMailDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// 注意: 我们添加了对 IID_ISendMail 的支持
//  以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {C5BE5524-3082-42CF-96C2-593FFCD61681}
static const IID IID_ISendMail =
{ 0xC5BE5524, 0x3082, 0x42CF, { 0x96, 0xC2, 0x59, 0x3F, 0xFC, 0xD6, 0x16, 0x81 } };

BEGIN_INTERFACE_MAP(CSendMailDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CSendMailDlgAutoProxy, IID_ISendMail, Dispatch)
END_INTERFACE_MAP()

// IMPLEMENT_OLECREATE2 宏在此项目的 StdAfx.h 中定义
// {676DA748-4C70-45F6-A2C5-50F651227F93}
IMPLEMENT_OLECREATE2(CSendMailDlgAutoProxy, "SendMail.Application", 0x676da748, 0x4c70, 0x45f6, 0xa2, 0xc5, 0x50, 0xf6, 0x51, 0x22, 0x7f, 0x93)


// CSendMailDlgAutoProxy 消息处理程序
