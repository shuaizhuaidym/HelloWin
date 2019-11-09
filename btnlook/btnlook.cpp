// btnlook.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "btnlook.h"

#define MAX_LOADSTRING 100

#define NUM (sizeof button / sizeof button[0])

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

bool isButtonType(HWND hBtn, DWORD dwType);

struct{
	int iStyle;
	TCHAR *szText;
}
button[]={
	BS_PUSHBUTTON, TEXT("PUSHBUTTON"),
	BS_DEFPUSHBUTTON, TEXT("DEFPUSHBUTTON"),
	BS_CHECKBOX, TEXT("CHECKBOX"),
	BS_AUTOCHECKBOX, TEXT("AUTOCHECKBOX"),
	BS_RADIOBUTTON, TEXT("RADIOBUTTON"),

	BS_3STATE, TEXT("3STATE"),
	BS_AUTO3STATE, TEXT("AUTO3STATE"),
	BS_GROUPBOX, TEXT("GROUPBOX"),
	BS_AUTORADIOBUTTON, TEXT("AUTORADIOBUTTON"),
	BS_OWNERDRAW, TEXT("OWNERDRAW")
};

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_BTNLOOK, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow)) {
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BTNLOOK));

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BTNLOOK));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_BTNFACE+1);//COLOR_WINDOW
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_BTNLOOK);
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
	PAINTSTRUCT ps;
	HDC hdc;

	static HWND hwndButton[NUM];
	static RECT rect;
	static TCHAR szTop[]   =TEXT("message		lParam			wParam"),
				 szUnd[]   =TEXT("_______		______			______"),
				 szFormat[]=TEXT("%-16s%04X-%04X %04X-%04X"),
				 szBuffer[50];
	static int cxChar, cyChar;
	int i;

	switch (message)
	{
	case WM_CREATE:
		cxChar = LOWORD(GetDialogBaseUnits());
		cyChar = HIWORD(GetDialogBaseUnits());
		//循环创建按钮
		for(i = 0; i < NUM; i++){
			hwndButton[i] = CreateWindow(TEXT("BUTTON"), button[i].szText, WS_CHILD|WS_VISIBLE|button[i].iStyle,
				cxChar, cyChar * (1 + 2*i), 20*cxChar, 7*cyChar / 4,
				hWnd,(HMENU)i, ((LPCREATESTRUCT)lParam)->hInstance,(LPVOID)i);
		}
		return 0;

	case WM_SIZE:
		rect.left   = 24*cxChar;
		rect.top    = 2*cyChar;
		rect.right  = LOWORD(lParam);
		rect.bottom = HIWORD(lParam);
		return 0;

	case WM_DRAWITEM:
	case WM_COMMAND:
		ScrollWindow(hWnd, 0, -cyChar, &rect, &rect);
		hdc = GetDC(hWnd);
		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
		//打印文本
		TextOut(hdc, 24*cxChar, cyChar * (rect.bottom/cyChar - 1),
			szBuffer,
			wsprintf(szBuffer, szFormat, message == WM_DRAWITEM ? TEXT("WM_DRAWITEM"):TEXT("WM_COMMAND"), HIWORD(wParam), LOWORD(wParam), HIWORD(lParam), HIWORD(lParam)));
		//释放资源、重绘窗口用户区域
		ReleaseDC(hWnd, hdc);
		ValidateRect(hWnd, &rect);


		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		//BS_CHECKBOX 需要代码设置选中状态
		if(wmId == 2)
		{
			//MessageBox(hWnd, TEXT("checkbox"), TEXT("YES"), MB_OK);
			SendMessage((HWND)lParam, BM_SETCHECK, (WPARAM)!SendMessage((HWND)lParam, BM_GETCHECK, 0, 0), 0);
		}
		// 分析菜单选择:
		switch (wmId)
		{
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
		InvalidateRect(hWnd,&rect,TRUE);
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		SelectObject(hdc,GetStockObject(SYSTEM_FIXED_FONT));
		SetBkMode(hdc,TRANSPARENT);
		TextOut(hdc,24*cxChar,cyChar,szTop,lstrlen(szTop));
		TextOut(hdc,24*cxChar,cyChar,szUnd,lstrlen(szUnd));

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
/**
*获取按钮类型
*/
bool isButtonType(HWND hBtn, DWORD dwType)
{
	//assert(dwType <= BS_TYPEMASK);
	LONG lStyle = GetWindowLong(hBtn, GWL_STYLE);
	return (lStyle & BS_TYPEMASK) == dwType;
}
