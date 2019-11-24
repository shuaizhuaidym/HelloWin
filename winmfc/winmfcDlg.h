
// winmfcDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CwinmfcDlg 对话框
class CwinmfcDlg : public CDialogEx
{
// 构造
public:
	CwinmfcDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_WINMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	CMFCEditBrowseCtrl mEditBrowse;


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeMfceditbrowse1();
	afx_msg void OnEnUpdateMfceditbrowse1();
	afx_msg void OnBnClickedMfcbutton1();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedMfcbtnEnc();
	CEdit mEditPwd;
};
