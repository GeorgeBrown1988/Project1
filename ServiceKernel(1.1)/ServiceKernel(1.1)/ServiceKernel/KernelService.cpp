#include "StdAfx.h"
#include "KernelService.h"


//���캯��
CKernelService::CKernelService()
{
#ifdef _DEBUG
	LOG(INFO) <<"CKernelService";
#endif
	m_bService=false;
	return;
}

//��������
CKernelService::~CKernelService()
{
#ifdef _DEBUG
	LOG(INFO) <<"~CKernelService";
#endif
	//ֹͣ����
	EndService();

	return;
}


//��ȡ��������ӿ�
void * __cdecl CKernelService::GetMsgTriggerService()
{
	return (void*)&m_MsgTriggerService;
}

//��ȡ��������ӿ�
void * __cdecl CKernelService::GetTCPSocketService()
{
	return (void*)&m_TCPSocketService;
}

//��ȡ���ݿ�����ӿ�
void * __cdecl CKernelService::GetDataBaseService()
{
	return (void*)&m_DataBaseService;
}



//��ȡ���ݿ���нӿ�
void * __cdecl CKernelService::GetDataBaseQueueService()
{
	return (void*)&m_DataBaseService;//.GetQueueService;//mark
}

//��ȡ���ȶ��нӿ�
void * __cdecl CKernelService::GetMsgTriggerQueueService()
{
	return m_MsgTriggerService.GetMsgTriggerQueueService();
}

//�ⲿ����
bool __cdecl CKernelService::ControlService(void * pBuffer, WORD wDataSize)
{
	//Ч��״̬
	if (m_bService==false) 
	{
		#ifdef _DEBUG
				LOG(INFO)<<"�������洦��ֹͣ״̬�У����Ʋ�������";
		#endif
		return false;
	}

	//��������
	CDataQueueService * pIQueueService=(CDataQueueService *)m_MsgTriggerService.GetMsgTriggerQueueService();
	if (pIQueueService==NULL) 
	{
		#ifdef _DEBUG
				LOG(INFO)<<"��ȡ�������津���ӿ�ʧ��";
		#endif	
		return false;
	}

	//Ͷ��ָ��
	if (pIQueueService->AddToQueue(EVENT_CONTROL,pBuffer,wDataSize)==false)
	{
		#ifdef _DEBUG
				LOG(INFO)<<"Ͷ�ݿ�����Ϣָ��ʧ��";
		#endif	
//		m_EventTrace.LOG(INFO)<<(TEXT("Ͷ�ݿ�����Ϣָ��ʧ��"),Level_Exception);
		return false;
	}

	return true;
}

//��������
bool __cdecl CKernelService::StartService()
{
	//�ж�״̬
	if (m_bService==true)
	{
		#ifdef _DEBUG
				LOG(INFO)<<"���������ظ�������������������";
		#endif	
//		m_EventTrace.LOG(INFO)<<(TEXT("���������ظ�������������������"),Level_Warning);
		return true;
	}

	//�ӿڲ�ѯ
	ITCPSocketService * pISocketService=&m_TCPSocketService;
	CObject * pIMsgTriggerService=(CObject *)m_MsgTriggerService.GetMsgTriggerQueueService();

	//�����
	if (m_MsgTriggerService.SetSocketService(pISocketService)==false)
	{
		#ifdef _DEBUG
			LOG(INFO)<<"������������������ʧ��";
		#endif
		return false;
	}
	//if (m_TimerService.SetTimerServiceSink(pIMsgTriggerService)==false)
	//{
	////	 LOG(INFO)<<(TEXT("��ʱ����������������ʧ��"),Level_Exception);
	//	return false;
	//}
	if (m_TCPSocketService.SetSocketServiceSink(pIMsgTriggerService)==false)
	{
		#ifdef _DEBUG
			LOG(INFO)<<"������������������ʧ��";
		#endif
		return false;
	}

	//�����������
	//if (m_TimerService.StartService()==false) return false;
	if (m_DataBaseService.StartService()==false) return false;
	//if (m_AsynchronismService.StartService()==false) return false;
	if (m_MsgTriggerService.StartService()==false) return false;
	if (m_TCPSocketService.StartService()==false) return false;

	//���ñ���
	m_bService=true;

	return true;
}

//ֹͣ����
bool __cdecl CKernelService::EndService()
{
	//���ñ���
	m_bService=false;

	//ɾ����ʱ��
	//m_TimerService.KillAllTimer();

	//ֹͣ��������
	m_MsgTriggerService.EndService();
	m_DataBaseService.EndService();
	//m_TimerService.EndService();
	m_TCPSocketService.EndService();
//	m_EventTrace.LOG(INFO)<<(TEXT("����ر�"),Level_Normal);
	//m_AsynchronismService.EndService();

	return true;
}



//ע�ṳ��
bool __cdecl CKernelService::SetMsgTriggerServiceSink(CObject * pIObject)
{
	//IMsgTriggerServiceSink *pMsgTriggerService = dynamic_cast<IMsgTriggerServiceSink*>(pIObject); 
	return m_MsgTriggerService.SetMsgTriggerServiceSink(pIObject);
}

//ע�ṳ��
bool __cdecl CKernelService::SetDataBaseSink(CObject * pIObject)
{
	return m_DataBaseService.SetDataBaseSink(pIObject);
}


//��������
bool __cdecl CKernelService::InitKernelService(WORD wListenPort, WORD wMaxSocketItem)
{
	if (m_TCPSocketService.SetServicePort(wListenPort)==false) return false;
	if (m_TCPSocketService.SetMaxSocketItem(wMaxSocketItem)==false) return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////

