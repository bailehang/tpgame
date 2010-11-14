// QQToolDlg.h : 头文件
//

#pragma once

class CQQToolDlgAutoProxy;


// CQQToolDlg 对话框
class CQQToolDlg : public CDialog
{
	DECLARE_DYNAMIC(CQQToolDlg);
	friend class CQQToolDlgAutoProxy;

// 构造
public:
	CQQToolDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CQQToolDlg();

// 对话框数据
	enum { IDD = IDD_QQTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	CQQToolDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
};
