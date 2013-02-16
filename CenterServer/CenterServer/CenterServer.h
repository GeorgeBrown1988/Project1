#pragma once

#include "resource.h"


void InitTrayIcon(HWND);
INT_PTR CALLBACK MainWndProc(HWND hMainWndHwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
void ShowOrHide();
void ExitProcessEx();
void InitControl();
void InitListCtrl(HWND);
void SelectConfigFile();
void ReadConfigFile(CString );
void TraceEventString(CString szString, enTraceLevel TraceLevel);
void SendLogMsg(enTraceLevel ,CString msg);
void InitDataToControl();
char* GetTimeStamp();
void InsertListItem(HWND hwnd,CString szLevel,CString szEvent);
extern HINSTANCE AppInstance;
extern HWND g_hMainWndHwnd;
extern HWND g_hListWndHwnd;
extern INT g_uCenterServerPort;
extern INT g_uDataBasePort;
extern CString g_szDataBaseAddr;
extern CString g_szDataBaseName;
extern CString g_szDataBaseUser;
extern CString g_szDataBasePass;