// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"
#ifdef _DEBUG
#include <glog\logging.h>
#pragma comment(lib,"libglog.lib")
#endif
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#include <atlbase.h>
#include <atlstr.h>
#include <commctrl.h>
#include <ShellAPI.h>
#include <CommDlg.h>
#include <Psapi.h>
#pragma comment(lib,"psapi.lib")
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"CommonServiceD.lib")
#ifdef _DEBUG
#pragma comment(lib,"ServiceKernelD.lib")
#else
#pragma comment(lib,"ServiceKernel.lib")
#endif

#include "../../GlobalDef/GlobalDef.h"
#include "../../GlobalDef/GlobalCmd.h"
#include "../../GlobalDef/CenterCmd.h"
#include "../../ServiceKernel/ServiceKernel/Interface.h"
#include "../../ServiceKernel/ServiceKernel/KernelService.h"
#include "../../ServiceKernel/ServiceKernel/DataBase.h"

// TODO: reference additional headers your program requires here
