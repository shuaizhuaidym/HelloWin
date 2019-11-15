// environ.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "environ.h"

#define MAX_LOADSTRING 100

#define ID_LIST 1
#define ID_TEXT 2

// ȫ�ֱ���:
HINSTANCE hInst = NULL;								// ��ǰʵ��
TCHAR szTitle1[MAX_LOADSTRING] = {_T('\0')};					// �������ı�
TCHAR szWindowClass1[MAX_LOADSTRING] = {_T('\0')};			// ����������

// �˴���ģ���а����ĺ�����ǰ������:
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

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle1, MAX_LOADSTRING);
	LoadString(hInstance, IDC_ENVIRON, szWindowClass1, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ENVIRON));

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
//
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
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

		// �����˵�ѡ��:
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
		// TODO: �ڴ���������ͼ����...
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