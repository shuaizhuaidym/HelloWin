// btnlook.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "btnlook.h"

#define MAX_LOADSTRING 100

#define NUM (sizeof button / sizeof button[0])

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

// �˴���ģ���а����ĺ�����ǰ������:
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

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_BTNLOOK, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow)) {
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BTNLOOK));

	// ����Ϣѭ��:
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BTNLOOK));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_BTNFACE+1);//COLOR_WINDOW
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_BTNLOOK);
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
		//ѭ��������ť
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
		//��ӡ�ı�
		TextOut(hdc, 24*cxChar, cyChar * (rect.bottom/cyChar - 1),
			szBuffer,
			wsprintf(szBuffer, szFormat, message == WM_DRAWITEM ? TEXT("WM_DRAWITEM"):TEXT("WM_COMMAND"), HIWORD(wParam), LOWORD(wParam), HIWORD(lParam), HIWORD(lParam)));
		//�ͷ���Դ���ػ洰���û�����
		ReleaseDC(hWnd, hdc);
		ValidateRect(hWnd, &rect);


		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		//BS_CHECKBOX ��Ҫ��������ѡ��״̬
		if(wmId == 2)
		{
			//MessageBox(hWnd, TEXT("checkbox"), TEXT("YES"), MB_OK);
			SendMessage((HWND)lParam, BM_SETCHECK, (WPARAM)!SendMessage((HWND)lParam, BM_GETCHECK, 0, 0), 0);
		}
		// �����˵�ѡ��:
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
		// TODO: �ڴ���������ͼ����...
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
/**
*��ȡ��ť����
*/
bool isButtonType(HWND hBtn, DWORD dwType)
{
	//assert(dwType <= BS_TYPEMASK);
	LONG lStyle = GetWindowLong(hBtn, GWL_STYLE);
	return (lStyle & BS_TYPEMASK) == dwType;
}
