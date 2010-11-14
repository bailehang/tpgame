// DlgProxy.cpp : 实现文件
//

#include "stdafx.h"
#include "QQTool.h"
#include "DlgProxy.h"
#include "QQToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CQQToolDlgAutoProxy

IMPLEMENT_DYNCREATE(CQQToolDlgAutoProxy, CCmdTarget)

CQQToolDlgAutoProxy::CQQToolDlgAutoProxy()
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
		ASSERT_KINDOF(CQQToolDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CQQToolDlg)))
		{
			m_pDialog = reinterpret_cast<CQQToolDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CQQToolDlgAutoProxy::~CQQToolDlgAutoProxy()
{
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	// 	析构函数调用 AfxOleUnlockApp。
	//  除了做其他事情外，这还将销毁主对话框
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CQQToolDlgAutoProxy::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清理代码。

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CQQToolDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CQQToolDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// 注意: 我们添加了对 IID_IQQTool 的支持
//  以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {CB5CE328-C140-47CF-B529-A203B421EB83}
static const IID IID_IQQTool =
{ 0xCB5CE328, 0xC140, 0x47CF, { 0xB5, 0x29, 0xA2, 0x3, 0xB4, 0x21, 0xEB, 0x83 } };

BEGIN_INTERFACE_MAP(CQQToolDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CQQToolDlgAutoProxy, IID_IQQTool, Dispatch)
END_INTERFACE_MAP()

// IMPLEMENT_OLECREATE2 宏在此项目的 StdAfx.h 中定义
// {947F7674-4BD9-4BEA-A579-FC92EC020E80}
IMPLEMENT_OLECREATE2(CQQToolDlgAutoProxy, "QQTool.Application", 0x947f7674, 0x4bd9, 0x4bea, 0xa5, 0x79, 0xfc, 0x92, 0xec, 0x2, 0xe, 0x80)


// CQQToolDlgAutoProxy 消息处理程序
