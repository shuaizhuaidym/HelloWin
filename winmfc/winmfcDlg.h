
// winmfcDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CwinmfcDlg �Ի���
class CwinmfcDlg : public CDialogEx
{
// ����
public:
	CwinmfcDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_WINMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	CMFCEditBrowseCtrl mEditBrowse;


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
