// MsgApp.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "MsgApp.h"
#define TIMER_NOTE 10001
HINSTANCE hInst;								// current instance
HWND hMainWndHwnd;
INT_PTR CALLBACK MainWndProc(HWND hMainWndHwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	hInst = hInstance;
	InitCommonControls();
	DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hMainWndHwnd, MainWndProc);
}
INT_PTR CALLBACK MainWndProc(HWND hMainWndHwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG:
		{
			int x = GetSystemMetrics(SM_CXSCREEN);
			int y = GetSystemMetrics(SM_CYSCREEN);
			MoveWindow(hMainWndHwnd,x-300,y-200,x,y,true);
			RECT rc;
			GetClientRect(hMainWndHwnd,&rc);
			HWND bt = GetDlgItem(hMainWndHwnd,IDC_CLOSE);
			MoveWindow(bt,rc.left+251,rc.top+1,rc.right-1,rc.top+21,true);
			
			SetDlgItemText(hMainWndHwnd,IDC_TEXT,GetCommandLine());
			AnimateWindow(hMainWndHwnd,3000,AW_CENTER|AW_BLEND);
			SetTimer(hMainWndHwnd,TIMER_NOTE,2000,NULL);
			return true;
		}
		case WM_COMMAND:
		{
			if(wParam == IDC_CLOSE)
			{
				::PostQuitMessage(0);
				return true;
			}
		}
		case WM_LBUTTONDOWN:
		{
			::PostQuitMessage(0);
			return true;
		}
		case WM_TIMER:
		{
			AnimateWindow(hMainWndHwnd,3000,AW_HIDE|AW_CENTER|AW_BLEND);
			KillTimer(hMainWndHwnd,TIMER_NOTE);
			::PostQuitMessage(0);
			return true;
		}
	}
	return DefWindowProc(hMainWndHwnd,uMsg,wParam,lParam);
}