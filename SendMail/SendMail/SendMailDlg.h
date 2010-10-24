// SendMailDlg.h : 头文件
//

#pragma once

class CSendMailDlgAutoProxy;


// CSendMailDlg 对话框
class CSendMailDlg : public CDialog
{
	DECLARE_DYNAMIC(CSendMailDlg);
	friend class CSendMailDlgAutoProxy;

// 构造
public:
	CSendMailDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CSendMailDlg();

// 对话框数据
	enum { IDD = IDD_SENDMAIL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	CSendMailDlgAutoProxy* m_pAutoProxy;
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
