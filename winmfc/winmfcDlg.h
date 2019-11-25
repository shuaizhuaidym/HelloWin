
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

	CButton mBtnAlgDefault;



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
	afx_msg void OnEnChangeMfceditbrowseSrc();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedMfcbtnEnc();
	afx_msg void OnBnClickedRdAlg();
	afx_msg void OnEnChangeMfceditbrowseDst();
	afx_msg void OnEnUpdateMfceditbrowseDst();
protected:
	// 加密算法
	wchar_t* mSzEncAlg;
private:
	CEdit mCEditPwd;
public:
	afx_msg void OnBnClickedRbtnDes();
	afx_msg void OnBnClickedRbtnAes();
	afx_msg void OnBnClickedRbtnSm4();
};
