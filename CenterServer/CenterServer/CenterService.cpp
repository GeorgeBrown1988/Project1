#include "StdAfx.h"
#include "CenterService.h"


CCenterService::CCenterService()
{

//	m_LogCount = 0;
}


CCenterService::~CCenterService()
{
	;
}

//开始服务
bool CCenterService::StartService()
{
	SafeDelete(m_pKernelService);
	try
	{
		m_pKernelService = static_cast<CKernelService *>(CreateKernelService());
		if(false == m_pKernelService->SetMsgTriggerServiceSink(dynamic_cast<CObject*>(&m_MsgTriggerServiceSink)))
			throw 0;
		if(false == m_pKernelService->SetDataBaseSink(dynamic_cast<CObject*>(&m_DataBaseSink)))
			throw 0;
		if(false == m_DataBaseSink.InitDataBaseSink(m_pKernelService))
			throw 0;
		if(false == m_MsgTriggerServiceSink.InitMsgTriggerSink(m_pKernelService))
			throw 0;
		m_pKernelService->InitKernelService(::g_uCenterServerPort,512);
		if(false == m_pKernelService->StartService())
		{
			SafeDelete(m_pKernelService);
			throw 0;
		}
	
	}
	catch(...)
	{
		CString szError("中心服务器启动失败");
		TraceEventString(szError,Level_Exception);
		return false;
	}
	return true;
	
}
//停止服务
bool CCenterService::EndService()
{
	if(m_pKernelService->EndService() == false)
	{
		MessageBox(NULL,"关闭失败","关闭失败",MB_OK);
		return false;
	}
	if(m_pKernelService != NULL)
		 SafeDelete(m_pKernelService);
	return true;
}
//服务状态
bool CCenterService::IsService()
{
	if(m_pKernelService != NULL)
		return m_pKernelService->IsService();
	return false;
}

void CCenterService::InitCenterService()
{
	m_pKernelService->InitKernelService(::g_uCenterServerPort,512);
	CTCPSocketService *pTCPSocketService;
	if(m_pKernelService->GetTCPSocketService(&pTCPSocketService) == false)
	{
		;
	}
	m_MsgTriggerServiceSink.InitMsgTriggerSink(m_pKernelService);
	//
}
