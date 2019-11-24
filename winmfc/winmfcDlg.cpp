
// winmfcDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "winmfc.h"
#include "winmfcDlg.h"
#include "afxdialogex.h"

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

// CwinmfcDlg 对话框

CwinmfcDlg::CwinmfcDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CwinmfcDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CwinmfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//绑定控件
	DDX_Control(pDX, IDC_MFCEDITBROWSE1, mEditBrowse);  // IDC_STATIC为控件的ID号
	DDX_Control(pDX, IDC_EDITPWD, mEditPwd);
}

BEGIN_MESSAGE_MAP(CwinmfcDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_MFCEDITBROWSE_SRC, &CwinmfcDlg::OnEnChangeMfceditbrowse1)
	ON_EN_UPDATE(IDC_MFCEDITBROWSE_DST, &CwinmfcDlg::OnEnUpdateMfceditbrowse1)
	ON_BN_CLICKED(IDC_MFCBTN_ENC, &CwinmfcDlg::OnBnClickedMfcbtnEnc)
END_MESSAGE_MAP()


// CwinmfcDlg 消息处理程序

BOOL CwinmfcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//mEditBrowse.EnableFileBrowseButton();
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	//设置文件格式过滤
	mEditBrowse.EnableFileBrowseButton(_T(""),_T("Bash Files(*.sh)|*.sh|Text Files(*.txt)|*.txt|All Files (*.*)|*.*||"));
	
	//设置按钮图标
	//HICON hIcon = NULL;
	//hIcon = theApp.LoadIcon(IDI_ICON3);
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BITMAP1);

	//mEditBrowse.SetBrowseButtonImage(bitmap, true);

	//获取全路径：

	//（1）为控件添加Value类别的Cstring类型的变量，UpdateData(TRUE);即可

	//（2）或定义变量CString mFilePath;

	// GetDlgItemText(IDC_MFCEDITBROWSE1, mFilePath);



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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

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

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CwinmfcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CwinmfcDlg::OnEnChangeMfceditbrowse1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}



void CwinmfcDlg::OnEnUpdateMfceditbrowse1()
{

	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数，以将 EM_SETEVENTMASK 消息发送到该控件，
	// 同时将 ENM_UPDATE 标志“或”运算到 lParam 掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CwinmfcDlg::OnBnClickedMfcbtnEnc()
{
	// TODO: 在此添加控件通知处理程序代码
	CString selectedPath;
	GetDlgItemText(IDC_MFCEDITBROWSE1, selectedPath);

	CFile file;
	CFileException ex;
	//CString fileText;
	char* content;
	// open the source file for reading
	if (!file.Open(selectedPath, CFile::modeRead | CFile::shareDenyWrite, &ex))
	{
		// complain if an error happened ,no need to delete the ex object
		TCHAR szError[1024];
		ex.GetErrorMessage(szError, 1024);
		TRACE1("Couldn't open source file: %s\n", szError);
	}
	else
	{
		UINT nBytes = (UINT)file.GetLength();
		int nChars = nBytes / sizeof(TCHAR);
		
		content = (char*)malloc(nBytes * sizeof(char));

		nBytes = file.Read(content, nBytes);

		//file.SeekToBegin();
		//nBytes = file.Read(fileText.GetBuffer(nChars), nBytes);

		//wchar_t wtext[4096];
		//mbstowcs(wtext, content, strlen(content)+1);//Plus null
		//LPWSTR ptr = wtext;
		 
		const WCHAR *pwcsContent;
		//required size
		int nChars2 = MultiByteToWideChar(CP_ACP, 0, content, -1, NULL, 0);
		//allocate it
		pwcsContent = new WCHAR[nChars2];
		MultiByteToWideChar(CP_ACP, 0, content, -1, (LPWSTR)pwcsContent, nChars2);		

		//display file content on static control
		CWnd *pwndStatic = this->GetDlgItem(IDC_EDIT_INFO);
		if (pwndStatic)
		{
			pwndStatic->SetWindowTextW((LPWSTR)pwcsContent);
		}
		//fileText.ReleaseBuffer(nChars);
		free(content);
		// delete it
		delete [] pwcsContent;

		mEditPwd.GetCapture();
		LPTSTR* pwd;
		CString ppwd;
		//mEditPwd.GetWindowText(pwd,100);
		mEditPwd.GetWindowText(ppwd);
		MessageBox(ppwd, _T("PWD"), MB_OK);


	}
								
	//MessageBox(selectedPath, _T("test"),MB_OK);
}
void readFile(TCHAR* filePath)
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


void CwinmfcDlg::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
}

