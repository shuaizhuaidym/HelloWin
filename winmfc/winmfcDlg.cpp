
// winmfcDlg.cpp : 实现文件

#include "stdafx.h"
#include "winmfc.h"
#include "winmfcDlg.h"
#include "afxdialogex.h"
#include "DESEnc.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();	
// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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

// CwinmfcDlg 对话框

CwinmfcDlg::CwinmfcDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CwinmfcDlg::IDD, pParent)
	, mSzEncAlg(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CwinmfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//绑定关联控件
	DDX_Control(pDX, IDC_MFCEDITBROWSE_SRC, mEditBrowseSrc);
	DDX_Control(pDX, IDC_MFCEDITBROWSE_DST, mEditBrowseDst);
	DDX_Control(pDX, IDC_RBTN_DEFAULT, mBtnAlgDefault);
	DDX_Control(pDX, IDC_EDITPWD, mCEditPwd);
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
END_MESSAGE_MAP()


// CwinmfcDlg 消息处理程序

BOOL CwinmfcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 将“关于...”菜单项添加到系统菜单中；IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动执行此操作
	SetIcon(m_hIcon, TRUE);		// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	//设置文件格式过滤
	mEditBrowseSrc.EnableFileBrowseButton(_T(""),_T("Bash Files(*.sh)|*.sh|Text Files(*.txt)|*.txt|All Files (*.*)|*.*||"));
	mEditBrowseDst.EnableFileBrowseButton(_T(""),_T("Bash Files(*.sh)|*.sh|Text Files(*.txt)|*.txt|All Files (*.*)|*.*||"));
	SetDlgItemText(IDC_EDITPWD, L"dsserver");
	
	//设置按钮图标
	//HICON hIcon = NULL;
	//hIcon = theApp.LoadIcon(IDI_ICON3);
	//CBitmap bitmap;
	//bitmap.LoadBitmap(IDB_BITMAP1);

	//mEditBrowse.SetBrowseButtonImage(bitmap, true);

	mBtnAlgDefault.SetCheck(1);//设置选中默认加密算法
	mSzEncAlg = _T("DES");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CwinmfcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CwinmfcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CwinmfcDlg::OnBnClickedMfcbtnEnc()
{
	// TODO: 在此添加控件通知处理程序代码
	CString selectedPath;
	GetDlgItemText(IDC_MFCEDITBROWSE1, selectedPath);

	CFile fileSrc;
	CFileException ex;
	//CString fileText;
	char* content;
	// open the source file for reading
	if (!fileSrc.Open(selectedPath, CFile::modeRead | CFile::shareDenyWrite, &ex))
	{
		// complain if an error happened ,no need to delete the ex object
		TCHAR szError[1024];
		CString strErrMsg;
		ex.GetErrorMessage(szError, 1024);
		
		strErrMsg.Format(_T("Couldn't open source file: %s\n"), szError);
		Message(strErrMsg.GetBuffer());
	}
	else
	{
		UINT nBytes = (UINT)fileSrc.GetLength();
		int nChars = nBytes / sizeof(TCHAR);

		content = (char*)malloc(nBytes * sizeof(char));
		nBytes = fileSrc.Read(content, nBytes);

		//file.SeekToBegin();
		//nBytes = file.Read(fileText.GetBuffer(nChars), nBytes);

		//wchar_t wtext[4096];
		//mbstowcs(wtext, content, strlen(content)+1);//Plus null
		//LPWSTR ptr = wtext;

		//ANSI格式可以正确显示，UTF-8乱码
		const WCHAR *wcharContent;
		// required size
		int nChars2 = MultiByteToWideChar(CP_ACP, 0, content, -1, NULL, 0);
		// allocate it
		wcharContent = new WCHAR[nChars2];
		MultiByteToWideChar(CP_ACP, 0, content, -1, (LPWSTR)wcharContent, nChars2);

		//显示文件原文到文本框
		CWnd *pwndStatic = this->GetDlgItem(IDC_EDIT1);
		if (pwndStatic)
		{
			pwndStatic->SetWindowTextW((LPWSTR)wcharContent);
		}
		//fileText.ReleaseBuffer(nChars);
		free(content);
		// delete it
		delete [] wcharContent;

		//获取加密密码
		CString strPwd;
		mCEditPwd.GetWindowText(strPwd);

		CString cs (strPwd);

		//加密文件					
		DESEnc* enc = new DESEnc();

		// Convert a TCHAR string to a LPCSTR
		CT2CA pszConvertedAnsiString (cs);
		// construct a std::string using the LPCSTR input
		std::string strStd (pszConvertedAnsiString);

		std::string secret = enc->Encrypt(content, strStd);//std::string s((LPCTSTR)cs);

		//将加密内容写入目标文件
		CString strDstPath;
		GetDlgItemText(IDC_MFCEDITBROWSE_DST, strDstPath);

		CFile fileDst;
		CFileException ex2;

		if (fileDst.Open(strDstPath,  CFile::modeCreate | CFile::modeWrite|CFile::typeBinary, &ex2))
		{
			fileDst.Write(secret.c_str(), secret.length());
		}
		else
		{//打开文件失败
			// complain if an error happened ,no need to delete the ex object
			TCHAR szError[1024];
			CString strErrMsg;
			ex.GetErrorMessage(szError, 1024);

			strErrMsg.Format(_T("Couldn't open source file: %s\n"), szError);
			Message(strErrMsg.GetBuffer());			
		}

	}
}
/*write file*/
void writeFile(TCHAR* pszFileName, std::string content)
{
	CFile	myFile;
	CFileException fileException;
	TCHAR	szBuffer[100]; 
	UINT    nActual = 0;
	CString fileText;
	if ( myFile.Open(pszFileName, CFile::modeCreate | CFile::modeReadWrite ))
	{
		myFile.Write( szBuffer, sizeof( szBuffer ) ); 
		myFile.Flush(); 
		TRACE( _T("Can't open file %s, error = %u\n"),
			pszFileName, fileException.m_cause );
	}
}
/*read file*/
void readFile(TCHAR* filePath, char* buffer)
{						   
	CFile	myFile;
	TCHAR	szBuffer[100]; 
	UINT    nActual = 0;
	CString fileText;
	if ( myFile.Open( _T("c:\\test\\myfile.dat"), CFile::modeCreate | CFile::modeReadWrite ) )
	{
		myFile.Write( szBuffer, sizeof( szBuffer ) ); 
		myFile.Flush();
		myFile.Seek( 0, CFile::begin );
		nActual = myFile.Read( szBuffer, sizeof( szBuffer ) ); 
	}
}

void CwinmfcDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}
   
void CwinmfcDlg::OnBnClickedRdAlg()
{
	mSzEncAlg = _T("DES");
}

void CwinmfcDlg::OnBnClickedRbtnDes()
{
	mSzEncAlg = _T("DES");
	// TODO: 在此添加控件通知处理程序代码
}


void CwinmfcDlg::OnBnClickedRbtnAes()
{
	mSzEncAlg = _T("AES");
	// TODO: 在此添加控件通知处理程序代码
}


void CwinmfcDlg::OnBnClickedRbtnSm4()
{
	// TODO: 在此添加控件通知处理程序代码
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