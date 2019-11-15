// environ.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "environ.h"

#define MAX_LOADSTRING 100

#define ID_LIST 1
#define ID_TEXT 2

// 全局变量:
HINSTANCE hInst = NULL;								// 当前实例
TCHAR szTitle1[MAX_LOADSTRING] = {_T('\0')};					// 标题栏文本
TCHAR szWindowClass1[MAX_LOADSTRING] = {_T('\0')};			// 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void FillListBox(HWND hwndList);

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
	LoadString(hInstance, IDS_APP_TITLE, szTitle1, MAX_LOADSTRING);
	LoadString(hInstance, IDC_ENVIRON, szWindowClass1, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ENVIRON));

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
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ENVIRON));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_ENVIRON);
	wcex.lpszClassName	= szWindowClass1;
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
   //CreateWindowW(lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam)
   hWnd = CreateWindow(_T("ENVIRON"), _T("ENVIRON"), WS_OVERLAPPEDWINDOW, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

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
	PAINTSTRUCT ps;
	HDC hdc;

	static HWND hwndList, hwndText;
	int iIndex, iLength, cxChar, cyChar;
	TCHAR* pVarName, *pVarValue;

	switch (message)
	{
	case WM_CREATE:
		cxChar = LOWORD(GetDialogBaseUnits());
		cyChar = HIWORD(GetDialogBaseUnits());

		hwndList = CreateWindow(TEXT("listbox"), NULL, WS_CHILD|WS_VISIBLE|LBS_STANDARD, cxChar, 3*cyChar, 16*cxChar + GetSystemMetrics(SM_CXVSCROLL),	cyChar*5,
			hWnd, (HMENU)ID_LIST,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		hwndText = CreateWindow(TEXT("static"), NULL, WS_CHILD|WS_VISIBLE|SS_LEFT,      cxChar, cyChar,   GetSystemMetrics(SM_CXSCREEN),              cyChar, 
			hWnd, (HMENU)ID_TEXT, 
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		FillListBox(hwndList);
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
		case LBN_SELCHANGE:
			if(wmId == ID_LIST)
			{
				//get current selection
				iIndex = SendMessage(hwndList, LB_GETCURSEL, 0, 0);
				iLength = SendMessage(hwndList, LB_GETTEXTLEN, iIndex, 0) + 1;
				pVarName = (TCHAR*)calloc(iLength, sizeof(TCHAR));
				SendMessage(hwndList, LB_GETTEXT, iIndex, (LPARAM)pVarName);

				//get environment string
				iLength = GetEnvironmentVariable(pVarName, NULL, 0);
				pVarValue = (TCHAR*)calloc(iLength, sizeof(TCHAR));
				GetEnvironmentVariable(pVarName, pVarValue, iLength);

				//show it in window
				SetWindowText(hwndText, pVarValue);
				free(pVarName);
				free(pVarValue);			
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
		return 0
			;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
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
void FillListBox(HWND hwndList)
{
	int iLength;
	TCHAR *pVarBlock, *pVarBeg, *pVarEnd, *pVarName, *pAddr;

	pVarBlock = GetEnvironmentStrings();
	pAddr = pVarBlock;
	while(*pVarBlock)
	{
		if(*pVarBlock != _T('='))//skip variable names begin with '='
		{
			pVarBeg = pVarBlock;//begining of variable name
			while(*pVarBlock++ != _T('='));//scan util '='			
			//points to '=' sign
			pVarEnd = pVarBlock - 1;
			//;ength og variable name
			iLength = pVarEnd - pVarBeg;
			//allocate memory for varialbe name termining zero. copy the variable name and append zero
			pVarName = (TCHAR*)calloc(iLength + 1, sizeof(TCHAR));
			CopyMemory(pVarName, pVarBeg, iLength * sizeof(TCHAR));
			pVarName[iLength] = _T('\0');
			SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)pVarName);
			free(pVarName);
			
		}
		while(*pVarBlock++ != _T('\0'));

	}
	FreeEnvironmentStrings(pAddr);

}