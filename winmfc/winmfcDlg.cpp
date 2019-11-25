
// winmfcDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "winmfc.h"
#include "winmfcDlg.h"
#include "afxdialogex.h"
#include "DESEnc.h"
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

CwinmfcDlg::CwinmfcDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CwinmfcDlg::IDD, pParent)
	, mSzEncAlg(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CwinmfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//�󶨿ؼ�
	DDX_Control(pDX, IDC_MFCEDITBROWSE1, mEditBrowse);  // IDC_MFCEDITBROWSE1Ϊ�ؼ���ID��
	DDX_Control(pDX, IDC_RBTN_DEFAULT, mBtnAlgDefault);
	DDX_Control(pDX, IDC_EDITPWD, mCEditPwd);
}

BEGIN_MESSAGE_MAP(CwinmfcDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_MFCEDITBROWSE_SRC, &CwinmfcDlg::OnEnChangeMfceditbrowseSrc)
	ON_EN_UPDATE(IDC_MFCEDITBROWSE_DST, &CwinmfcDlg::OnEnUpdateMfceditbrowseDst)
	ON_BN_CLICKED(IDC_MFCBTN_ENC, &CwinmfcDlg::OnBnClickedMfcbtnEnc)
	ON_BN_CLICKED(IDC_RD_DEFAULT, &CwinmfcDlg::OnBnClickedRdAlg)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE_DST, &CwinmfcDlg::OnEnChangeMfceditbrowseDst)
	ON_BN_CLICKED(IDC_RBTN_DES, &CwinmfcDlg::OnBnClickedRbtnDes)
	ON_BN_CLICKED(IDC_RBTN_AES, &CwinmfcDlg::OnBnClickedRbtnAes)
	ON_BN_CLICKED(IDC_RBTN_SM4, &CwinmfcDlg::OnBnClickedRbtnSm4)
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
	//CBitmap bitmap;
	//bitmap.LoadBitmap(IDB_BITMAP1);

	//mEditBrowse.SetBrowseButtonImage(bitmap, true);

	//��ȡȫ·����
	//��1��Ϊ�ؼ����Value����Cstring���͵ı�����UpdateData(TRUE);���� 
	//��2���������CString mFilePath;
	// GetDlgItemText(IDC_MFCEDITBROWSE1, mFilePath);

	mBtnAlgDefault.SetCheck(1);//ѡ��Ĭ�ϼ����㷨
	mSzEncAlg = _T("DES");

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



void CwinmfcDlg::OnEnChangeMfceditbrowseSrc()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}



void CwinmfcDlg::OnEnUpdateMfceditbrowseDst()
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
		 
		const WCHAR *pwcsName;
		// required size
		int nChars2 = MultiByteToWideChar(CP_ACP, 0, content, -1, NULL, 0);
		// allocate it
		pwcsName = new WCHAR[nChars2];
		MultiByteToWideChar(CP_ACP, 0, content, -1, (LPWSTR)pwcsName, nChars2);
		// use it....

		CWnd *pwndStatic = this->GetDlgItem(IDC_EDIT1);
		if (pwndStatic)
		{
			pwndStatic->SetWindowTextW((LPWSTR)pwcsName);
		}
		//fileText.ReleaseBuffer(nChars);
		free(content);
		// delete it
		delete [] pwcsName;

		CString strPwd;
		mCEditPwd.GetWindowText(strPwd);

		MessageBox(strPwd, _T("title"), MB_OK);
	}
						
	DESEnc* enc = new DESEnc();
	std::string secret = enc->Encrypt("abc","111111");
	MessageBox((LPCTSTR)secret.c_str(), L"SECRET", MB_OK);
}
/*write file*/
void writeFile(TCHAR* filePath, std::string content)
{
	/*TCHAR* pszFileName = _T("c:\\test\\myfile.dat");
	CFile myFile;
	CFileException fileException;

	if ( !myFile.Open( pszFileName, CFile::modeCreate |   
		CFile::modeReadWrite, &fileException ) )
	{
		TRACE( _T("Can't open file %s, error = %u\n"),
			pszFileName, fileException.m_cause );
	}*/	
	CFile	myFile;
	TCHAR	szBuffer[100]; 
	UINT    nActual = 0;
	CString fileText;
	if ( myFile.Open(filePath, CFile::modeCreate | CFile::modeReadWrite ))
	{
		myFile.Write( szBuffer, sizeof( szBuffer ) ); 
		myFile.Flush(); 
	}
}
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
	MessageBox(_T("default alg"), _T("ALG"), MB_OK);
	CWnd* cwndDft = GetDlgItem(IDC_RBTN_DEFAULT) ;
	mSzEncAlg = _T("DES");

	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CwinmfcDlg::OnEnChangeMfceditbrowseDst()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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