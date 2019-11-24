
// winmfcDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "winmfc.h"
#include "winmfcDlg.h"
#include "afxdialogex.h"

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

// CwinmfcDlg �Ի���

CwinmfcDlg::CwinmfcDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CwinmfcDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CwinmfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//�󶨿ؼ�
	DDX_Control(pDX, IDC_MFCEDITBROWSE1, mEditBrowse);  // IDC_STATICΪ�ؼ���ID��
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


// CwinmfcDlg ��Ϣ�������

BOOL CwinmfcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//mEditBrowse.EnableFileBrowseButton();
	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	//�����ļ���ʽ����
	mEditBrowse.EnableFileBrowseButton(_T(""),_T("Bash Files(*.sh)|*.sh|Text Files(*.txt)|*.txt|All Files (*.*)|*.*||"));
	
	//���ð�ťͼ��
	//HICON hIcon = NULL;
	//hIcon = theApp.LoadIcon(IDI_ICON3);
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BITMAP1);

	//mEditBrowse.SetBrowseButtonImage(bitmap, true);

	//��ȡȫ·����

	//��1��Ϊ�ؼ����Value����Cstring���͵ı�����UpdateData(TRUE);����

	//��2���������CString mFilePath;

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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

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
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CwinmfcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CwinmfcDlg::OnEnChangeMfceditbrowse1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}



void CwinmfcDlg::OnEnUpdateMfceditbrowse1()
{

	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// �������Խ� EM_SETEVENTMASK ��Ϣ���͵��ÿؼ���
	// ͬʱ�� ENM_UPDATE ��־�������㵽 lParam �����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CwinmfcDlg::OnBnClickedMfcbtnEnc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

