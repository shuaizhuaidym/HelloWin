// HEAD.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "HEAD.h"

#define ID_LIST 1
#define ID_TEXT 1

#define MAXREAD 8192

#define DIRATTR (DDL_READWRITE | DDL_READONLY | DDL_HIDDEN | DDL_SYSTEM | DDL_DIRECTORY | DDL_ARCHIVE | DDL_DRIVES)
#define DTFLAGS (DT_WORDBREAK | DT_EXPANDTABS | DT_NOCLIP | DT_NOPREFIX)

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明:
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

 	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_HEAD, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HEAD));

	// 主消息循环:
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
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//  仅当希望
//  此代码与添加到 Windows 95 中的“RegisterClassEx”函数之前的 Win32 系统兼容时，才需要此函数及其用法。
//  调用此函数十分重要，这样应用程序就可以获得关联的“格式正确的”小图标。
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
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

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
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
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
		
		//文件列表
		hwndList = CreateWindow(TEXT("listbox"), NULL, 
			WS_CHILDWINDOW | WS_VISIBLE | LBS_STANDARD, 
			cxChar, cyChar * 3, 
			cxChar * 13 + GetSystemMetrics(SM_CXVSCROLL),
			cyChar * 10, 
			hWnd, (HMENU)ID_LIST, 
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), 
			NULL);

		GetCurrentDirectory(MAX_PATH + 1, szBuffer);
		
		//显示内容的区域
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
		
		// 分析菜单选择:
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

//列表框事件
LRESULT CALLBACK ListProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	if(message == WM_KEYDOWN && wparam == VK_RETURN)
	{
		SendMessage(GetParent(hwnd), WM_COMMAND, MAKELONG(1, LBN_DBLCLK), (LPARAM)hwnd);
	}
	return CallWindowProc(OldList, hwnd, message, wparam, lparam);
}

// “关于”框的消息处理程序。
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
