// colors.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "colors.h"

#define MAX_LOADSTRING 100

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK ScrollProc(HWND,UINT,WPARAM,LPARAM);

int idFocus;
WNDPROC OldScroll[3];

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_COLORS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_COLORS));

	// Main message loop:
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_COLORS));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_COLORS);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

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
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static COLORREF crPrim[3]={RGB(255,0,0) ,RGB(0,255,0), RGB(0,0,255)};
	static HBRUSH hBrush[3], hBrushStatic;

	static HWND hwndScroll[3],hwndLabel[3],hwndValue[3],hwndRect;
	static int color[3],cyChar;
	static RECT rcColor;
	static TCHAR* szColorLabel[] = {TEXT("Red"), TEXT("Green"), TEXT("Blue")};

	HINSTANCE hInstance;
	int i, cxClient, cyClient;
	TCHAR szBuffer[10];

	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		hInstance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
		hwndRect = CreateWindow(TEXT("static"),NULL, WS_CHILD|WS_VISIBLE|SS_WHITERECT, 0, 0, 0, 0, hWnd,(HMENU)9, hInstance, NULL);
		for(int i=0 ; i<3; i++)
		{
			hwndScroll[i]=CreateWindow(TEXT("scrollbar"), NULL, WS_CHILD|WS_VISIBLE|WS_TABSTOP|SBS_VERT, 0, 0, 0, 0, hWnd, (HMENU)i, hInstance, NULL);
			SetScrollRange(hwndScroll[i], SB_CTL, 0, 255, FALSE);
			SetScrollPos(hwndScroll[i], SB_CTL, 0, FALSE);

			hwndLabel[i] = CreateWindow(TEXT("STATIC"), szColorLabel[i], WS_CHILD|WS_VISIBLE|SS_CENTER, 0, 0, 0, 0, hWnd,(HMENU)(i+3), hInstance, NULL);
			hwndValue[i] = CreateWindow(TEXT("static"), TEXT("0"), WS_CHILD|WS_VISIBLE|SS_CENTER, 0, 0, 0, 0, hWnd, (HMENU)(i+6), hInstance, NULL);

			OldScroll[i] = (WNDPROC)SetWindowLong(hwndScroll[i], GWL_WNDPROC, (LONG)ScrollProc);

			hBrush[i] = CreateSolidBrush(crPrim[i]);
		}
		hBrushStatic = CreateSolidBrush(GetSysColor(COLOR_BTNHIGHLIGHT));

		cyChar = HIWORD(GetDialogBaseUnits());
		break;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);

		SetRect(&rcColor, cxClient/2, 0, cxClient, cyClient);

		MoveWindow(hwndRect, 0, 0, cxClient/2, cyClient, TRUE);
		for(int i=0;i<3;i++)
		{
			MoveWindow(hwndScroll[i], (2*i+1) * cxClient / 14, 2*cyChar, cxClient / 14, cyClient - 4 * cyChar, TRUE);
			MoveWindow(hwndLabel[i], (4*i+1) * cxClient / 28, cyChar / 2, cxClient / 7, cyChar, TRUE);
			MoveWindow(hwndValue[i], (4*i+1) * cxClient / 28, cyChar - 3*cyChar / 2, cxClient / 7, cyChar, TRUE);
		}
		SetFocus(hWnd);
		break;
	case WM_SETFOCUS:
		SetFocus(hwndScroll[idFocus]);
		break;
	case WM_VSCROLL:
		i = GetWindowLong((HWND)lParam, GWL_ID);
		switch(LOWORD(wParam))
		{
		case SB_PAGEDOWN:
			color[i] += 15;
			break;
		case SB_LINEDOWN:
			color[i] = min(255, color[i]+1);
			break;
		case SB_PAGEUP:
			color[i] -= 15;
			break;
		case SB_LINEUP:
			color[i] = max(0, color[i]-1);
			break;
		case SB_TOP:
			color[i] = 0;
			break;
		case SB_BOTTOM:
			color[i] = 255;
			break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			color[i] = HIWORD(wParam);
			break;
		default:
			break;
		}
		SetScrollPos(hwndScroll[i], SB_CTL, color[i], TRUE);
		wsprintf(szBuffer, TEXT("%i"), color[i]);
		SetWindowText(hwndValue[i],szBuffer);
		DeleteObject((HBRUSH)SetClassLong(hWnd, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(color[0], color[1], color[2]))));

		InvalidateRect(hWnd, &rcColor, TRUE);
		return 0;
	case WM_CTLCOLORSCROLLBAR:
		i = GetWindowLong((HWND)lParam, GWL_ID); 
		return (LRESULT)hBrush[i];
	case WM_CTLCOLORSTATIC:
		i = GetWindowLong((HWND)lParam, GWL_ID);
		if(i>=3 && i<=8)//static text controls
		{
			SetTextColor((HDC)wParam, crPrim[i%3]);
			SetBkColor((HDC)wParam, GetSysColor(COLOR_BTNHIGHLIGHT));
			return (LRESULT)hBrushStatic;
		}
		break;
	case WM_SYSCOLORCHANGE:
		DeleteObject(hBrushStatic);
		hBrushStatic = CreateSolidBrush(GetSysColor(COLOR_BTNHIGHLIGHT));
		return 0;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
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
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		DeleteObject((HBRUSH)SetClassLong((hWnd), GCL_HBRBACKGROUND, (LONG)GetStockObject(WHITE_BRUSH)));
		for(int I=0;I<3;I++)
		{
			DeleteObject(hBrush[I]);
		}
		DeleteObject(hBrushStatic);

		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
LRESULT CALLBACK ScrollProc(HWND hwnd,UINT message,WPARAM wParam, LPARAM lParam)
{
	int id = GetWindowLong(hwnd, GWL_ID);
	switch(message)
	{
	case WM_KEYDOWN:
		if(wParam == VK_TAB)
		{
			SetFocus(GetDlgItem(GetParent(hwnd), (id + (GetKeyState(VK_SHIFT)<0 ? 2 : 1))%3));
		}
		break;
	case WM_SETFOCUS:
		idFocus = id;
		break;
	}
	return CallWindowProc(OldScroll[id], hwnd, message, wParam, lParam);
}

// Message handler for about box.
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
