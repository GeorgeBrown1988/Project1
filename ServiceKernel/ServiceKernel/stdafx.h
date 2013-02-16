// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#ifdef _DEBUG
#include <glog\logging.h>
#pragma comment(lib,"libglog.lib")
#endif
#include <windows.h>
#include <WinSock2.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#include <atlbase.h>
#include <atlstr.h>
#include <time.h>
#include <commctrl.h>
#pragma comment(lib,"comctl32.lib")
#include "../../GlobalDef/GlobalDef.h"
#include "../../GlobalDef/GlobalCmd.h"
#include "Array.h"
//void TRACE(TCHAR *pszFormat,...);
//ASSERT(bool status);
#pragma warning(disable: 4584)  
#pragma warning(disable: 4996) 
#pragma warning(disable: 4018)  
#pragma warning(disable: 4244)  
#pragma warning(disable: 4800) 

// TODO: reference additional headers your program requires here
