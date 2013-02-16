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

//��ʼ����
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
		CString szError("���ķ���������ʧ��");
		TraceEventString(szError,Level_Exception);
		return false;
	}
	return true;
	
}
//ֹͣ����
bool CCenterService::EndService()
{
	if(m_pKernelService->EndService() == false)
	{
		MessageBox(NULL,"�ر�ʧ��","�ر�ʧ��",MB_OK);
		return false;
	}
	if(m_pKernelService != NULL)
		 SafeDelete(m_pKernelService);
	return true;
}
//����״̬
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
