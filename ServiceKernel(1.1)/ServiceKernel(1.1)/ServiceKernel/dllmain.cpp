// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "EventService.h"
#include "DataBase.h"
#include "QueueService.h"
#include "SocketService.h"
#include "KernelService.h"
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

void * __cdecl CreateKernelService()
{
	try{

		CKernelService *pObject=new CKernelService();
		if(pObject == NULL)
			throw ;
		return pObject;
	}
	catch(...)
	{
		return NULL;
	}
	

}

void * __cdecl CreateDataBaseService()
{
	try{

		CDataBaseService *pObject=new CDataBaseService();
		if(pObject == NULL)
			throw ;
		return pObject;
	}
	catch(...)
	{
		return NULL;
	}
}
void * __cdecl CreateDataQueueService()
{
	try{

		CDataQueueService *pObject=new CDataQueueService();
		if(pObject == NULL)
			throw ;
		return pObject;
	}
	catch(...)
	{
		return NULL;
	}
	

}
void * __cdecl CreateDataBase()
{
	try{

		CMysqlDataBase *pObject=new CMysqlDataBase();
		if(pObject == NULL)
			throw ;
		return pObject;
	}
	catch(...)
	{
		return NULL;
	}
}
void * __cdecl CreateTCPScoektService()
{
	try{

		CTCPSocketService *pObject=new CTCPSocketService();
		if(pObject == NULL)
			throw ;
		return pObject;
	}
	catch(...)
	{
		return NULL;
	}
}
//void TRACE(TCHAR *pszFormat,...)
//{
//	TCHAR szOutPut[255];
//     va_list pArgList;
//     va_start(pArgList,pszFormat) ;
//     _vsntprintf(szOutPut,sizeof(szOutPut)/sizeof(TCHAR),pszFormat,pArgList) ;
//     va_end(pArgList);
//	 ::OutputDebugString(szOutPut);
//}
//void //ASSERT(bool status)
//{
//	if(!status)
//		MessageBox(NULL,L"Error",L"Error",MB_OK);
//}