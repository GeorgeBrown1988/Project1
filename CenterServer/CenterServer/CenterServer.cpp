// CenterServer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "CenterServer.h"
#include "Config.h"
#include "CenterService.h"
#define MAX_LOADSTRING 100
//Global Var
// TrayIcon
#define ID_TASKICON 100
#define UPDATECTRL_TIMER 1000
#define WM_ICONNOTIFY           (WM_USER + 101)
NOTIFYICONDATA nid;
HINSTANCE AppInstance;
HWND g_hMainWndHwnd;
HWND g_hListWndHwnd;
INT g_uCenterServerPort;
INT g_uDataBasePort;
CString g_szDataBaseAddr;
CString g_szDataBaseName;
CString g_szDataBaseUser;
CString g_szDataBasePass;
//
CCenterService g_CenterService;
void InitControl()
{
	InitTrayIcon(g_hMainWndHwnd);
	InitListCtrl(g_hMainWndHwnd);
}
void ShowOrHide()
{
	if(IsWindowVisible(g_hMainWndHwnd))
	{	//还原窗口
		ShowWindow(g_hMainWndHwnd,SW_HIDE);

	}
	else
	{
		//隐藏窗口
		ShowWindow(g_hMainWndHwnd,SW_SHOW);
	}
}
//显示错误
void TraceEventString(CString szString, enTraceLevel TraceLevel)
{
//enum enTraceLevel
//{
//	Level_Normal					=0,									//普通消息
//	Level_Warning					=1,									//警告消息
//	Level_Exception					=2,									//异常消息
//	Level_Debug						=3,									//调试消息
//};
	#ifdef _DEBUG
		LOG(INFO)<<szString;
	#endif
	if(TraceLevel == Level_Normal)
		InsertListItem(::g_hListWndHwnd,"Normal",szString);
	if(TraceLevel == Level_Warning)
		InsertListItem(::g_hListWndHwnd,"Warning",szString);
	if(TraceLevel == Level_Exception)
		InsertListItem(::g_hListWndHwnd,"Exception",szString);
	if(TraceLevel == Level_Debug)
		InsertListItem(::g_hListWndHwnd,"Debug",szString);
}
void ExitProcessEx()
{
	if(g_CenterService.IsService())
		g_CenterService.EndService();
	Shell_NotifyIcon(NIM_DELETE, &nid);
	PostQuitMessage(0);
}
void InitTrayIcon(HWND hwnd)
{
			nid.cbSize = sizeof(NOTIFYICONDATA);
			nid.hWnd = hwnd;
			nid.uID = ID_TASKICON;
			nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
			nid.uCallbackMessage = WM_ICONNOTIFY;
			nid.hIcon = LoadIcon(AppInstance, MAKEINTRESOURCE(IDI_CENTERSERVER));// IMAGE_ICON, 16, 16, 0);
			strcpy(nid.szTip, TEXT("中心服务器"));
			Shell_NotifyIcon(NIM_ADD, &nid);
}

/*
[CenterServer]
CenterServerAddr=127.0.0.1
CenterServerPort=8998
DataBaseName=ServerInfoDB
DataBaseUser=sxm
DataBasePass=123456

*/
void InitDataToControl()
{
	SetDlgItemText(::g_hMainWndHwnd,IDC_DBADDR,::g_szDataBaseAddr);//数据库地址
	SetDlgItemInt(::g_hMainWndHwnd,IDC_LISTENPORT,::g_uCenterServerPort,false);
	SetDlgItemText(::g_hMainWndHwnd,IDC_DBNAME,::g_szDataBaseName);
	SetDlgItemText(::g_hMainWndHwnd,IDC_DBUSER,::g_szDataBaseUser);
}
void SelectConfigFile()
{
	TCHAR szFileName[MAX_PATH]="";
	OPENFILENAME file={0};
	file.lStructSize = sizeof(file);
	file.lpstrFile = szFileName;
	file.nMaxFile = MAX_PATH;
	file.lpstrFilter = "Config Files(*.ini)\0*.ini\0All Files\0*.*\0\0";
	file.nFilterIndex = 1;
	if(GetOpenFileName(&file))
	{
		ReadConfigFile(szFileName);
		InitDataToControl();
	}
	
}


void SendLogMsg(enTraceLevel MsgLevel,CString msg)
{
	;
}

void ReadConfigFile(CString szFileName)
{
	CIniConfig IniConfig;
	IniConfig.GetConfigNum(szFileName,"CenterServer","CenterServerPort",::g_uCenterServerPort);
	IniConfig.GetConfigNum(szFileName,"CenterServer","DataBasePort",::g_uDataBasePort);
	IniConfig.GetConfigString(szFileName,"CenterServer","DataBaseName",::g_szDataBaseName);
	IniConfig.GetConfigString(szFileName,"CenterServer","DataBaseUser",::g_szDataBaseUser);
	IniConfig.GetConfigString(szFileName,"CenterServer","DataBasePass",::g_szDataBasePass);
	IniConfig.GetConfigString(szFileName,"CenterServer","DataBaseAddr",::g_szDataBaseAddr);
}

void InitListCtrl(HWND hwnd)
{
	LVCOLUMN ColInfo1 = {0};
	LVCOLUMN ColInfo2 = {0};
	LVCOLUMN ColInfo3 = {0};
	//
	ColInfo1.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
    ColInfo1.iSubItem = 0;
    ColInfo1.fmt = LVCFMT_LEFT;
    ColInfo1.cx = 90;
    ColInfo1.pszText = "TimeStamp";
    ColInfo1.cchTextMax = 50;
    ColInfo2.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
    ColInfo2.iSubItem = 0;
    ColInfo2.fmt = LVCFMT_LEFT;
    ColInfo2.cx = 80;
    ColInfo2.pszText = "Level";
    ColInfo2.cchTextMax = 50;
    ColInfo3.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
    ColInfo3.iSubItem = 0;
    ColInfo3.fmt = LVCFMT_LEFT;
    ColInfo3.cx = 130;
    ColInfo3.pszText = "Event";
    ColInfo3.cchTextMax = 50;
	::g_hListWndHwnd = GetDlgItem(hwnd,IDC_LOGLIST);
	::SendMessage(::g_hListWndHwnd, LVM_SETEXTENDEDLISTVIEWSTYLE, 0,
                    LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    ::SendMessage(::g_hListWndHwnd, LVM_INSERTCOLUMN, WPARAM(0), LPARAM(&ColInfo1));
    ::SendMessage(::g_hListWndHwnd, LVM_INSERTCOLUMN, WPARAM(1), LPARAM(&ColInfo2));
	::SendMessage(::g_hListWndHwnd, LVM_INSERTCOLUMN, WPARAM(2), LPARAM(&ColInfo3));
}
void InsertListItem(HWND hwnd,CString szLevel,CString szEvent)
{
	int count = (int)::SendMessage(hwnd,LVM_GETITEMCOUNT,0,0);
	LVITEM item;
	if(!hwnd)
		return ;
	if(count < 300)
	{
		CString szTime;
		SYSTEMTIME sysTime;
		GetLocalTime(&sysTime); 
		szTime.Format("%d%d%d%d%d%d",sysTime.wYear,sysTime.wMonth,sysTime.wDay,sysTime.wHour,sysTime.wMinute,sysTime.wMilliseconds);
		item.mask=LVIF_TEXT;
		item.pszText = szTime.GetBuffer(0);
		item.iItem=count;
		item.iSubItem=0;
		::SendMessage(hwnd, LVM_INSERTITEM, 0, LPARAM(&item));
	}
	else
	{
		;
	}
	if(szLevel.GetLength())
	{
		item.mask=LVIF_TEXT;
		item.pszText = szLevel.GetBuffer(0);
		item.iItem=count;
		item.iSubItem=1;
		::SendMessage(hwnd, LVM_SETITEM, 0, LPARAM(&item));
	}
	if(szEvent.GetLength())
	{
		item.mask=LVIF_TEXT;
		item.pszText = szEvent.GetBuffer(0);
		item.iItem=count;
		item.iSubItem=2;
		::SendMessage(hwnd, LVM_SETITEM, 0, LPARAM(&item));
	}
}
char* GetTimeStamp()
{
		CString szTime;
		SYSTEMTIME sysTime;
		GetLocalTime(&sysTime); 
		szTime.Format("%d%d%d%d%d%d",sysTime.wYear,sysTime.wMonth,sysTime.wDay,sysTime.wHour,sysTime.wMinute,sysTime.wMilliseconds);
		return szTime.GetBuffer(0);
}
INT_PTR CALLBACK MainWndProc(HWND hMainWndHwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG:
		{
			g_hMainWndHwnd = hMainWndHwnd;
			InitControl();
			TraceEventString("中心服务器运行",Level_Normal);
			return true;
		}
		case WM_ICONNOTIFY:
		{
			switch(lParam) 
			  {
				 case WM_LBUTTONDBLCLK:
				{
					// Load main window here
					PostMessage(hMainWndHwnd,WM_COMMAND,ID_MENU_VIEW,0);
					//OutputDebugString(TEXT("WM_LBUTTONDBLCLK\n"));
					return true;
				}
				 case WM_RBUTTONUP:
				{
					POINT point;
					HMENU hMenu, hSubMenu;
					INT iCmd;
					// Get mouse position
					GetCursorPos(&point);
					// Popup context menu
					hMenu = LoadMenu(AppInstance, MAKEINTRESOURCE(IDR_SYSMENU));
					hSubMenu = GetSubMenu(hMenu, 0);
					SetMenuDefaultItem(hSubMenu,ID_MENU_VIEW,FALSE);
					SetForegroundWindow(hMainWndHwnd);         // Per KB Article Q135788
					iCmd = TrackPopupMenu(hSubMenu,
						TPM_LEFTBUTTON|TPM_RIGHTBUTTON|TPM_LEFTALIGN|TPM_RETURNCMD,
						point.x, point.y, 0, hMainWndHwnd, NULL);
					if(iCmd)
						PostMessage(hMainWndHwnd,WM_COMMAND, iCmd, 0);   // Per KB Article Q135788
					DestroyMenu(hMenu);
				}
					return true;
				 default:
					return true;
			  }
		}
		case WM_CLOSE:
		{
			ShowOrHide();
			return true;
		}
		case WM_COMMAND:
		{
			switch(wParam)
			{
				case ID_MENU_VIEW:
				{
					ShowOrHide();
					return true;
				}
				case ID_MENU_EXIT:
				{
					PostMessage(hMainWndHwnd,WM_COMMAND,IDC_EXIT,0);
					return true;
				}
				case IDC_START:
				{
					if(g_CenterService.IsService())
					{
						SetDlgItemText(hMainWndHwnd,IDC_START,"Start");
						g_CenterService.EndService();
						//int count = (int)::SendMessage(hListview,LVM_GETITEMCOUNT,0,0);

					}
					else
					{
						if(g_CenterService.StartService())
							SetDlgItemText(hMainWndHwnd,IDC_START,"Stop");
					}
					return true;
				}
				case IDC_OPT:
				{
					SelectConfigFile();
					return true;
				}
				case IDC_EXIT:
				{
					if(g_CenterService.IsService())
					{
						if(IDYES !=MessageBox(hMainWndHwnd,TEXT("服务正在运行，确认退出?"),TEXT("Warning"),MB_YESNOCANCEL))
							return true;
					}
					ExitProcessEx();
					return true;
				}
				default:
				{
					return DefWindowProc(hMainWndHwnd,uMsg,wParam,lParam);
				}
			}//END SWITCH										
		}//END WM_COMMAND
		default:
		{
			return false;
			//如果return DefWindowProc(hMainWndHwnd,uMsg,wParam,lParam);会出错
			//如果return true;其他消息就被截断了
		}
	}//END SWITCH
	return false;
}
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
#ifdef _DEBUG
	google::InitGoogleLogging("CenterServer");    
    google::SetLogDestination(google::INFO,"D:\\MyProject\\");
#endif
	AppInstance=hInstance;
	DialogBox(AppInstance, MAKEINTRESOURCE(IDD_CENTERDLG),NULL, MainWndProc);
	//return (int) msg.wParam;
}