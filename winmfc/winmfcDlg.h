
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

	CButton mBtnAlgDefault;



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
	afx_msg void OnEnChangeMfceditbrowseSrc();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedMfcbtnEnc();
	afx_msg void OnBnClickedRdAlg();
	afx_msg void OnEnChangeMfceditbrowseDst();
	afx_msg void OnEnUpdateMfceditbrowseDst();
protected:
	// �����㷨
	wchar_t* mSzEncAlg;
private:
	CEdit mCEditPwd;
public:
	afx_msg void OnBnClickedRbtnDes();
	afx_msg void OnBnClickedRbtnAes();
	afx_msg void OnBnClickedRbtnSm4();
};
