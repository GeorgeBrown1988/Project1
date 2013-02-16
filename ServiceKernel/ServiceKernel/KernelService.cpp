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
bool __cdecl CKernelService::GetMsgTriggerService(IMsgTriggerService ** pIObject)
{
	*pIObject = dynamic_cast<CMsgTriggerService*>(&m_MsgTriggerService);
	if(*pIObject)
		return true;
	else
		return false;
}

//��ȡ��������ӿ�
bool __cdecl CKernelService::GetTCPSocketService(ITCPSocketService ** pIObject)
{
	*pIObject = dynamic_cast<CTCPSocketService*>(&m_TCPSocketService);
	if(*pIObject)
		return true;
	else
		return false;	//m_TCPSocketService;
}

//��ȡ���ݿ�����ӿ�
bool __cdecl CKernelService::GetDataBaseService(IDataBaseService ** pIObject)
{
	*pIObject = dynamic_cast<CDataBaseService*>(&m_DataBaseService);
	if(*pIObject)
		return true;
	else
		return false;
}

	////��ȡ���ݿ�����ӿ�
	//virtual bool __cdecl GetDataBaseService(IDataBaseService * pIObject)=NULL;
	////��ȡ��������ӿ�
	//virtual bool __cdecl GetMsgTriggerService(IMsgTriggerService * pIObject)=NULL;
	////��ȡ��������ӿ�
	//virtual bool __cdecl GetTCPSocketService(ITCPSocketService * pIObject)=NULL;
	////��ȡ�ӿ�
	//virtual bool __cdecl GetDataBaseQueueService(IDataQueueService * pIObject)=NULL;
	////��ȡ�ӿ�
	//virtual bool __cdecl GetMsgTriggerQueueService(IDataQueueService * pIObject)=NULL;

//��ȡ���ݿ���нӿ�
bool __cdecl CKernelService::GetDataBaseQueueService(IDataQueueService ** pIObject)
{
	return m_DataBaseService.GetQueueService(pIObject);//.GetQueueService;//mark
}

//��ȡ���ȶ��нӿ�
bool __cdecl CKernelService::GetMsgTriggerQueueService(IDataQueueService ** pIObject)
{
	return m_MsgTriggerService.GetMsgTriggerQueueService(pIObject);
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
	IDataQueueService * pIQueueService;
	if(m_MsgTriggerService.GetMsgTriggerQueueService(&pIQueueService)==false)
	{
		#ifdef _DEBUG
				LOG(INFO)<<"��ȡ�������津���ӿ�ʧ��";
		#endif	
		return false;
	}

	//Ͷ��ָ��
	IDataQueueSink *pDataQueueSink = dynamic_cast<IDataQueueSink*>(pIQueueService);
	if (pDataQueueSink->AddToQueue(EVENT_CONTROL,pBuffer,wDataSize)==false)
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
	IDataQueueService * pIMsgTriggerQueueService;
	if(m_MsgTriggerService.GetMsgTriggerQueueService(&pIMsgTriggerQueueService) == false)
	{
		#ifdef _DEBUG
			LOG(INFO)<<"����������л�ȡʧ��";
		#endif
		return false;
	}
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
	if (m_TCPSocketService.SetSocketServiceSink(pIMsgTriggerQueueService)==false)
	{
		#ifdef _DEBUG
			LOG(INFO)<<"�������������������а�ʧ��";
		#endif
		return false;
	}
	//MessageBox(NULL,"TEST","TEST",MB_OK);
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
bool __cdecl CKernelService::InitKernelService(INT wListenPort, INT wMaxSocketItem)
{
	if (m_TCPSocketService.SetServicePort(wListenPort)==false) return false;
	if (m_TCPSocketService.SetMaxSocketItem(wMaxSocketItem)==false) return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////

