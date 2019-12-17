// HEAD.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "HEAD.h"

#define ID_LIST 1
#define ID_TEXT 1

#define MAXREAD 8192

#define DIRATTR (DDL_READWRITE | DDL_READONLY | DDL_HIDDEN | DDL_SYSTEM | DDL_DIRECTORY | DDL_ARCHIVE | DDL_DRIVES)
#define DTFLAGS (DT_WORDBREAK | DT_EXPANDTABS | DT_NOCLIP | DT_NOPREFIX)

#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);

LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ListProc(HWND, UINT, WPARAM, LPARAM);

WNDPROC OldList;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_HEAD, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HEAD));

	// ����Ϣѭ��:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//  ����ϣ��
//  �˴�������ӵ� Windows 95 �еġ�RegisterClassEx������֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷���
//  ���ô˺���ʮ����Ҫ������Ӧ�ó���Ϳ��Ի�ù����ġ���ʽ��ȷ�ġ�Сͼ�ꡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HEAD));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_HEAD);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	static BOOL	bValidFile;
	static BYTE  buffer[MAXREAD];
	static HWND  hwndList, hwndText;
	static RECT  rect;

	static TCHAR szFile[MAX_PATH + 1];
	HANDLE hFile;
	HDC hdc;
	int i, cxChar, cyChar;
	PAINTSTRUCT ps;
	TCHAR szBuffer[MAX_PATH + 1];

	switch (message)
	{
	case WM_CREATE:
		cxChar = LOWORD(GetDialogBaseUnits());
		cyChar = HIWORD(GetDialogBaseUnits());
		rect.left = 20 * cxChar;
		rect.top = 3 * cyChar;
		
		//�ļ��б�
		hwndList = CreateWindow(TEXT("listbox"), NULL, 
			WS_CHILDWINDOW | WS_VISIBLE | LBS_STANDARD, 
			cxChar, cyChar * 3, 
			cxChar * 13 + GetSystemMetrics(SM_CXVSCROLL),
			cyChar * 10, 
			hWnd, (HMENU)ID_LIST, 
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), 
			NULL);

		GetCurrentDirectory(MAX_PATH + 1, szBuffer);
		
		//��ʾ���ݵ�����
		hwndText = CreateWindow(TEXT("static"), szBuffer, 
			WS_CHILDWINDOW | WS_VISIBLE | SS_LEFT, 
			cxChar, cyChar, cxChar * MAX_PATH, cyChar, 
			hWnd, (HMENU)ID_TEXT, 
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), 
			NULL);

		OldList = (WNDPROC)SetWindowLong(hwndList, GWL_WNDPROC, (LPARAM)ListProc);
		SendMessage(hwndList, LB_DIR, DIRATTR, (LPARAM)TEXT("*.*"));
		break;

	case WM_SIZE:
		rect.right  = LOWORD(lParam);
		rect.bottom = HIWORD(lParam);
		return 0;

	case WM_SETFOCUS:
		SetFocus(hwndList);
		return 0;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		
		// �����˵�ѡ��:
		switch (wmId)
		{
		case ID_LIST:
			if(wmEvent == LBN_DBLCLK)
			{
				if(LB_ERR == (i = SendMessage(hwndList, LB_GETCURSEL, 0 , 0))){break;}
				SendMessage(hwndList, LB_GETTEXT, i, (LPARAM)szBuffer);

				if(INVALID_HANDLE_VALUE != (hFile = CreateFile(szBuffer, GENERIC_READ, FILE_SHARE_READ, NULL,  OPEN_EXISTING, 0 ,NULL)))
				{
					CloseHandle(hFile);
					bValidFile = TRUE;
					lstrcpy(szFile, szBuffer);
					GetCurrentDirectory(MAX_PATH + 1, szBuffer);

					if(szBuffer[lstrlen(szBuffer)]-1 != '\\')
					{
						lstrcat(szBuffer, TEXT("\\"));
					}
					SetWindowText(hwndText, szBuffer);
				}
				else
				{
					bValidFile = FALSE;
					szBuffer[lstrlen(szBuffer) == '\0'];
					//if the dir not work, maybe wrong drive,try it
					if(!SetCurrentDirectory(szBuffer + 1))
					{
						szBuffer[3] = ':';
						szBuffer[4] = '\0';
						SetCurrentDirectory(szBuffer + 2);
					}
					//get the new directory name,fill the list box
					GetCurrentDirectory(MAX_PATH +1, szBuffer);
					SetWindowText(hwndText, szBuffer);
					SendMessage(hwndList, LB_RESETCONTENT, 0, 0);
					SendMessage(hwndList, LB_DIR, DIRATTR, (LPARAM)TEXT("*.*"));
				}
				InvalidateRect(hWnd, NULL, TRUE);
			}
			return 0;

		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		//TODO
		if(!bValidFile){break;}

		if(INVALID_HANDLE_VALUE == (hFile = CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0 , NULL)))
		{
			bValidFile = FALSE;
			break;
		}
		ReadFile(hFile, buffer, MAXREAD, (LPDWORD)&i, NULL);
		CloseHandle(hFile);
		//i== buffer size of byte,paint it to window

		hdc = BeginPaint(hWnd, &ps);
		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
		SetTextColor(hdc, GetSysColor(COLOR_BTNTEXT));
		SetBkColor(hdc, GetSysColor(COLOR_BTNFACE));

		//ASSUME FILE IS ASCII
		DrawTextA(hdc, (PCSTR)buffer, i, &rect, DTFLAGS);
		
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

//�б���¼�
LRESULT CALLBACK ListProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	if(message == WM_KEYDOWN && wparam == VK_RETURN)
	{
		SendMessage(GetParent(hwnd), WM_COMMAND, MAKELONG(1, LBN_DBLCLK), (LPARAM)hwnd);
	}
	return CallWindowProc(OldList, hwnd, message, wparam, lparam);
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
