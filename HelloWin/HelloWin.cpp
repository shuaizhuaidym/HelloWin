#include <Windows.h>
#include "systems.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow){

	static TCHAR szAppName[]=TEXT("Hello Windows");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.style=CS_HREDRAW|CS_VREDRAW;	
	wndclass.lpfnWndProc=WndProc;
	wndclass.cbClsExtra=0;
	wndclass.cbWndExtra=0;
	wndclass.hInstance=hInstance;
	wndclass.hIcon=LoadIcon(NULL,IDI_APPLICATION);

	wndclass.hCursor=LoadCursor(NULL,IDC_ARROW);
	wndclass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);

	wndclass.lpszMenuName=NULL;
	wndclass.lpszClassName=szAppName;

	if(!RegisterClass(&wndclass)){
		MessageBox(NULL,TEXT("This program requires Windows NT"),szAppName,MB_ICONERROR);
		return 0;
	}
	
	hwnd=CreateWindow(	szAppName,				  //CLASS NAME
						TEXT("THE HELLO PROGRAM!"),//CAPTION
						WS_OVERLAPPEDWINDOW|WS_VSCROLL,	//STYLE

						CW_USEDEFAULT,//INIT X POSITION
						CW_USEDEFAULT,//INIT Y POSITION
						CW_USEDEFAULT,//INIT X SIZE
						CW_USEDEFAULT,//INIT Y SIZE

						NULL,//parent window handle
						NULL,//menu handle
						hInstance,//instance handle
						NULL);//creation parameters

	ShowWindow(hwnd,iCmdShow);
	UpdateWindow(hwnd);

	while(GetMessage(&msg,NULL,0,0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

//打印文字窗口过程
/*LRESULT CALLBACK WndProc2(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	switch(message){
	case WM_CREATE:
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hwnd,&ps);

				//font process
		{
			WCHAR sFname[32]=TEXT("黑体");
			HFONT hf;
			LOGFONT lf;
			lf.lfHeight=120;
			lf.lfWidth=30;
			lf.lfEscapement=0;
			//lf.lfFaceName=sFname;
			wcscpy(lf.lfFaceName,sFname);

			lf.lfUnderline=FALSE; 
			lf.lfItalic=FALSE; 
			lf.lfStrikeOut=FALSE; 
			lf.lfCharSet=GB2312_CHARSET;
			hf=CreateFontIndirect(&lf);
			
			SelectObject(hdc,hf);
		}

		GetClientRect(hwnd,&rect);

		DrawText(hdc,TEXT("Hello windows 成功！"),-1,&rect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		EndPaint(hwnd,&ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;		
	}
	return DefWindowProc(hwnd,message,wParam,lParam);
}
*/
//打印系统参数过程
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	static int cxChar, cxCaps, cyChar, cyClient, iVscrollPos;
	HDC hdc;
	int i,y;
	PAINTSTRUCT ps;
	TCHAR	szBuffer[10];
	TEXTMETRIC tm;

	switch(message){
	case WM_CREATE:
		hdc = GetDC(hwnd);

		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily &1 ? 3 : 2) * cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;

		ReleaseDC(hwnd,hdc);

		SetScrollRange(hwnd, SB_VERT, 0, NUMLINES - 1, FALSE);
		SetScrollPos(hwnd, SB_VERT, iVscrollPos, TRUE);
		return 0;

	case WM_SIZE:
		//MessageBox(hwnd,TEXT("Resizing window"),TEXT("Event"),MB_OK);
		cyClient = HIWORD(lParam);
		return 0;
	
	case WM_VSCROLL:
		switch(LOWORD(wParam)){
		case SB_LINEUP:
			iVscrollPos -= 1;
			break;
		case SB_LINEDOWN:
			iVscrollPos += 1;
			break;
		case SB_PAGEUP:
			iVscrollPos -= cyClient/cyChar;
			break;
		case SB_PAGEDOWN:
			iVscrollPos += cyClient/cyChar;
			break;
			//鼠标拖动事件,当wParam低字节是SB_PAGEDOWN时,高字节是滑块当前位置,在滚动条位置最大和最小值之间;如果此间没有调用SetScrollPos,松开鼠标滑块会复位
		case SB_THUMBTRACK:
			//MessageBox(hwnd,TEXT("Draging scroll bar."),TEXT("Event"),MB_OK);
			break;
			//当wParam低位是SB_THUMBPOSITION时高位是松开鼠标时滑块的位置;对于其他滚动动作wParam高位忽略
		case SB_THUMBPOSITION://鼠标拖动
			iVscrollPos = HIWORD(wParam);
			MessageBox(hwnd,TEXT("End scroll bar."),TEXT("Event"),MB_OK);
			break;
		default:
			break;
		}
		iVscrollPos = max(0, min(iVscrollPos, NUMLINES - 1));
		if(iVscrollPos != GetScrollPos(hwnd, SB_VERT))
		{
			SetScrollPos(hwnd, SB_VERT, iVscrollPos, TRUE);
			InvalidateRect(hwnd, NULL,TRUE);
		}
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd,&ps);
		for(i = 0;i<NUMLINES;i++){
			TextOut(hdc,0,cyChar*i,sysmetrics[i].szLabel,lstrlen(sysmetrics[i].szLabel));
			TextOut(hdc,22*cxCaps,cyChar*i,sysmetrics[i].szDesc,lstrlen(sysmetrics[i].szDesc));
			
			SetTextAlign(hdc,TA_RIGHT|TA_TOP);

			TextOut(hdc,22*cxCaps+40*cxChar,cyChar*i,szBuffer,wsprintf(szBuffer,TEXT("%5d"),GetSystemMetrics(sysmetrics[i].iIndex)));

			SetTextAlign(hdc,TA_LEFT|TA_TOP);
		}
		EndPaint(hwnd,&ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;		
	}
	return DefWindowProc(hwnd,message,wParam,lParam);
}