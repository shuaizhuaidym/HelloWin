
// winmfcDlg.cpp : ʵ���ļ�

#include "stdafx.h"
#include "Winbase.h"

#include "winmfc.h"
#include "winmfcDlg.h"
#include "afxdialogex.h"
#include "string"
#include "DESEncoder.h"

#include "DESDecoder.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();	
// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()
//end CAboutDlg

// CwinmfcDlg �Ի���

CwinmfcDlg::CwinmfcDlg(CWnd* pParent /*=NULL*/) : CDialogEx(CwinmfcDlg::IDD, pParent), mSzEncAlg(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(AFX_IDI_STD_FRAME);
}

void CwinmfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//�󶨹����ؼ�
	DDX_Control(pDX, IDC_MFCEDITBROWSE_SRC, mEditBrowseSrc);
	DDX_Control(pDX, IDC_MFCEDITBROWSE_DST, mEditBrowseDst);
	DDX_Control(pDX, IDC_RBTN_DEFAULT, mBtnAlgDefault);
	DDX_Control(pDX, IDC_EDITPWD, mCEditPwd);
	DDX_Control(pDX, IDC_MFCBTN_ENC, mCMFCBtnEnc);
	DDX_Control(pDX, IDC_MFCBTN_DEC, mCMFCBtnDec);
}

BEGIN_MESSAGE_MAP(CwinmfcDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RD_DEFAULT, &CwinmfcDlg::OnBnClickedRdAlg)
	ON_BN_CLICKED(IDC_RBTN_DES, &CwinmfcDlg::OnBnClickedRbtnDes)
	ON_BN_CLICKED(IDC_RBTN_AES, &CwinmfcDlg::OnBnClickedRbtnAes)
	ON_BN_CLICKED(IDC_RBTN_SM4, &CwinmfcDlg::OnBnClickedRbtnSm4)
	ON_BN_CLICKED(IDC_MFCBTN_ENC ,&CwinmfcDlg::OnBnClickedMfcbtnEnc)
	ON_BN_CLICKED(IDC_MFCBTN_DEC, &CwinmfcDlg::OnBnClickedMfcbtnDec)
END_MESSAGE_MAP()

// CwinmfcDlg ��Ϣ�������

BOOL CwinmfcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// ��������...���˵�����ӵ�ϵͳ�˵��У�IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�ִ�д˲���
	SetIcon(m_hIcon, TRUE);		// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	
	//�����ļ���ʽ����
	mEditBrowseSrc.EnableFileBrowseButton(_T(""),_T("Text Files(*.txt)|*.txt|Bash Files(*.sh)|*.sh|All Files (*.*)|*.*||"));
	mEditBrowseDst.EnableFileBrowseButton(_T(""),_T("Text Files(*.txt)|*.txt|Bash Files(*.sh)|*.sh|All Files (*.*)|*.*||"));
	SetDlgItemText(IDC_EDITPWD, L"dsserver");
	
	//���ð�ťͼ�꣬���Խ��Ĭ�ϰ�ť��win10����˸����
	//HICON hIcon = NULL;
	//hIcon = theApp.LoadIcon(IDI_ICON3);
	//CBitmap bitmap;
	//bitmap.LoadBitmap(IDB_BITMAP1);
	//mEditBrowse.SetBrowseButtonImage(bitmap, true);

	//����ѡ��Ĭ�ϼ����㷨
	mBtnAlgDefault.SetCheck(1);
	mSzEncAlg = _T("DES");


	//����������ɫ
	CFont* font = GetFont();

	LOGFONT logFont;
	font->GetLogFont(&logFont);
	logFont.lfWeight = FW_BOLD;
	//FIXME �ֲ���Ч
	//_tcsncpy_s(logFont.lfFaceName, LF_FACESIZE, _T("Times New Roman"), 17);

	m_font.CreateFontIndirect(&logFont);

	mCMFCBtnEnc.SetFont(&m_font);
	mCMFCBtnEnc.SetTextColor(RGB(222, 0, 0));

	mCMFCBtnDec.SetFont(&m_font);
	mCMFCBtnDec.SetTextColor(RGB(0, 128, 0));


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CwinmfcDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó����⽫�ɿ���Զ���ɡ�

void CwinmfcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CwinmfcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//���ܰ�ť����¼�
void CwinmfcDlg::OnBnClickedMfcbtnEnc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strSrcPath;
	GetDlgItemText(IDC_MFCEDITBROWSE1, strSrcPath);

	//CFile fileSrc;
	//CFileException ex;
	//CString fileText;
	//char* content;
	// open the source file for reading
	//if (!fileSrc.Open(strSrcPath, CFile::modeRead | CFile::shareDenyWrite, &ex))
	//{
		// complain if an error happened ,no need to delete the ex object
	//	TCHAR szError[1024];
	//	CString strErrMsg;
	//	ex.GetErrorMessage(szError, 1024);
		
	//	strErrMsg.Format(_T("Couldn't open source file: %s\n"), szError);
	//	Message(strErrMsg.GetBuffer());
	//}
	//else
	//{
		//UINT nBytes = (UINT)fileSrc.GetLength();
		//int nChars = nBytes / sizeof(TCHAR);

		//content = (char*)malloc(nBytes * sizeof(char));
		//nBytes = fileSrc.Read(content, nBytes);

		//file.SeekToBegin();
		//nBytes = file.Read(fileText.GetBuffer(nChars), nBytes);

		//wchar_t wtext[4096];
		//mbstowcs(wtext, content, strlen(content)+1);//Plus null
		//LPWSTR ptr = wtext;

		//ANSI��ʽ������ȷ��ʾ��UTF-8����
		//const WCHAR *wcharContent;
		// required size
		//int nChars2 = MultiByteToWideChar(CP_ACP, 0, content, -1, NULL, 0);
		// allocate it
		//wcharContent = new WCHAR[nChars2];
		//MultiByteToWideChar(CP_ACP, 0, content, -1, (LPWSTR)wcharContent, nChars2);

		//��ʾ�ļ�ԭ�ĵ��ı���
		//CWnd *pwndStatic = this->GetDlgItem(IDC_EDIT1);
		//if (pwndStatic)
		//{
		//	pwndStatic->SetWindowTextW((LPWSTR)wcharContent);
		//}
		//fileText.ReleaseBuffer(nChars);
		

		//��ȡ��������
		CString strPwd;
		mCEditPwd.GetWindowText(strPwd);
		
		//Ŀ���ļ�·��
		CString strDstPath;
		GetDlgItemText(IDC_MFCEDITBROWSE_DST, strDstPath);
		//free(content);		
		//delete [] wcharContent;
		DESEncoder* encoder = new DESEncoder(*this);
		encoder->MYEncryptFile((LPTSTR)(LPCTSTR)strSrcPath, (LPTSTR)(LPCTSTR)strDstPath, (LPTSTR)(LPCTSTR)strPwd);
		delete encoder;
	//}
}

void CwinmfcDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}
   
void CwinmfcDlg::OnBnClickedRdAlg()
{
	mSzEncAlg = _T("DES");
}

void CwinmfcDlg::OnBnClickedRbtnDes()
{
	mSzEncAlg = _T("DES");
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CwinmfcDlg::OnBnClickedRbtnAes()
{
	mSzEncAlg = _T("AES");
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CwinmfcDlg::OnBnClickedRbtnSm4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	mSzEncAlg = _T("SM4");
}
/**
void ConvertGBKToUtf8(CString& strGBK) {
    int len=MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, NULL,0);
    unsigned short * wszUtf8 = new unsigned short[len+1];
    memset(wszUtf8, 0, len * 2 + 2);
    MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, wszUtf8, len);

    len = WideCharToMultiByte(CP_UTF8, 0, wszUtf8, -1, NULL, 0, NULL, NULL); 
    char *szUtf8=new char[len + 1];
    memset(szUtf8, 0, len + 1);
    WideCharToMultiByte (CP_UTF8, 0, wszUtf8, -1, szUtf8, len, NULL,NULL);

    strGBK = szUtf8;
    delete[] szUtf8;
    delete[] wszUtf8;
}

void ConvertUtf8ToGBK(CString& strUtf8) {
    int len=MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, NULL,0);
    unsigned short * wszGBK = new unsigned short[len+1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, wszGBK, len);

    len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL); 
    char *szGBK=new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte (CP_ACP, 0, wszGBK, -1, szGBK, len, NULL,NULL);

    strUtf8 = szGBK;
    delete[] szGBK;
    delete[] wszGBK;
}
void ConvertUTF8ToANSI(char *strUtf8, char *strAnsi)
{
    int len=MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, NULL,0); 
    unsigned short * wszAnsi = new unsigned short[len+1]; 
    memset(wszAnsi, 0, len * 2 + 2); 
    MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, wszAnsi, len); 
    len = WideCharToMultiByte(CP_ACP, 0, wszAnsi, -1, NULL, 0, NULL, NULL); 
    WideCharToMultiByte (CP_ACP, 0, wszAnsi, -1, strAnsi, len, NULL,NULL); 
 
    delete[] wszAnsi; 
}
*/
/**�ڿ���̨��ʾ��ʾ��Ϣ*/
void CwinmfcDlg::Message(TCHAR* msg)
{
	if(wcslen(msg) > 0)
	{
		CWnd *pwndStatic = GetDlgItem(IDC_EDIT1);
		if (pwndStatic)
		{
			pwndStatic->SetWindowTextW(msg);
		}
		
	}
}

int isUnicode()
{
	std::string s = "Hallo!";
	std::wstring ws = L"Hello!";

	if (::IsTextUnicode(ws.c_str(), ws.length(), NULL) == 1)
	{
		// is unicode
		int i = 0;
	}
	else
	{
		// no unicode
		int i = 1;
	}
	return 0;
}

void CwinmfcDlg::OnBnClickedMfcbtnDec()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strSrcPath;
	GetDlgItemText(IDC_MFCEDITBROWSE1, strSrcPath);

	//��ȡ��������
		CString strPwd;
		mCEditPwd.GetWindowText(strPwd);
		
		//Ŀ���ļ�·��
		CString strDstPath;
		GetDlgItemText(IDC_MFCEDITBROWSE_DST, strDstPath);

		DESDecoder* decoder = new DESDecoder();
		decoder->MyDecryptFile((LPTSTR)(LPCTSTR)strSrcPath, (LPTSTR)(LPCTSTR)strDstPath, (LPTSTR)(LPCTSTR)strPwd);
		delete decoder;
}
