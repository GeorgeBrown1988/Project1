#include "StdAfx.h"
#include "MsgTrigger.h"



//���캯��
CMsgTriggerService::CMsgTriggerService(void)
{
#ifdef _DEBUG
	LOG(INFO) <<"CMsgTriggerService";
#endif
	m_bService=false;
	m_pITCPSocketService=NULL;
	m_pIMsgTriggerServiceSink=NULL;
	return;
}

//��������
CMsgTriggerService::~CMsgTriggerService(void)
{
#ifdef _DEBUG
	LOG(INFO) <<"~CMsgTriggerService";
#endif
}


//��������
bool __cdecl CMsgTriggerService::StartService()
{
	//�ж�״̬
	if (m_bService==true) 
	{
		#ifdef _DEBUG
			LOG(INFO) <<"���������ظ�������������������";
		#endif
		// LOG(INFO)<<(TEXT("���������ظ�������������������"),Level_Warning);
		return true;
	}

	//��ҽӿ�
	if (m_pIMsgTriggerServiceSink==NULL)
	{
		#ifdef _DEBUG
			LOG(INFO) <<"����������ҷ��񲻴���";
		#endif
		// LOG(INFO)<<(TEXT("����������ҷ��񲻴���"),Level_Exception);
		return false;
	}

	//���ö���
	if (m_RequestQueueService.SetDataQueueServiceSink(this)==false)
	{
		#ifdef _DEBUG
			LOG(INFO) <<"������������з����ʧ��";
		#endif
		// LOG(INFO)<<(TEXT("������������з����ʧ��"),Level_Exception);
		return false;
	}

	//�������
	if (m_pIMsgTriggerServiceSink->StartService(this)==false)
	{
		#ifdef _DEBUG
			LOG(INFO) <<"����������ҷ�������ʧ��";
		#endif
		// LOG(INFO)<<(TEXT("����������ҷ�������ʧ��"),Level_Exception);
		return false;
	}

	//��������
	if (m_RequestQueueService.StartService()==false)
	{
		#ifdef _DEBUG
			LOG(INFO) <<"����������з�������ʧ��";
		#endif
		// LOG(INFO)<<(TEXT("����������з�������ʧ��"),Level_Exception);
		return false;
	}

	//���ñ���
	m_bService=true;

	return true;
}
//ֹͣ����
bool __cdecl CMsgTriggerService::EndService()
{
	//���ñ���
	m_bService=false;

	//ֹͣ�������
	m_RequestQueueService.EndService();

	//ֹͣ���
	if (m_pIMsgTriggerServiceSink!=NULL)
	{
		m_pIMsgTriggerServiceSink->EndService(this);//(IMsgTriggerServiceSink*)
	}

	return true;
}

//��������
bool __cdecl CMsgTriggerService::SetSocketService(CObject * pIObject)
{
	//ASSERT(pIObject!=NULL);
	m_pITCPSocketService=dynamic_cast<ITCPSocketService*>(pIObject);
	//ASSERT(m_pITCPSocketService!=NULL);
	return (m_pITCPSocketService!=NULL);
}

//ע�ṳ��
bool __cdecl CMsgTriggerService::SetMsgTriggerServiceSink(CObject * pIObject)
{
	//Ч�����
	//ASSERT(pIObject!=NULL);
	//ASSERT(m_pIMsgTriggerServiceSink==NULL);IAttemperEngineSink
	if (pIObject==NULL) return false;
	if (m_pIMsgTriggerServiceSink!=NULL) return false;

	//��ѯ�ӿ�
	m_pIMsgTriggerServiceSink=dynamic_cast<IMsgTriggerServiceSink*>(pIObject);
	if (m_pIMsgTriggerServiceSink==NULL)
	{
		#ifdef _DEBUG
			LOG(INFO) <<"����������ҷ���ӿڻ�ȡʧ�ܣ��ҽӲ���ʧ��";
		#endif
		// LOG(INFO)<<(TEXT("����������ҷ���ӿڻ�ȡʧ�ܣ��ҽӲ���ʧ��"),Level_Exception);
		return false;
	}

	return true;
}

//��ȡ�ӿ�
bool __cdecl CMsgTriggerService::GetMsgTriggerQueueService(CDataQueueService ** pIObject)
{
	*pIObject = dynamic_cast<CDataQueueService*>(&m_RequestQueueService);
	if(pIObject)
		return true;
	else
		return false;
}

//���нӿ�
void __cdecl CMsgTriggerService::OnDataQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize, DWORD dwInsertTime)
{
	//�ں��¼�
	//ASSERT(m_pIMsgTriggerServiceSink!=NULL);
	switch (wIdentifier)
	{
	//case EVENT_TIMER:			//��ʱ���¼�
	//	{
	//		//Ч�����
	//		//ASSERT(wDataSize==sizeof(NTY_TimerEvent));
	//		if (wDataSize!=sizeof(NTY_TimerEvent)) return;

	//		//������Ϣ
	//		NTY_TimerEvent * pTimerEvent=(NTY_TimerEvent *)pBuffer;
	//		m_pIMsgRoutServiceSink->OnEventTimer(pTimerEvent->wTimerID,pTimerEvent->wBindParam);

	//		return;
	//	}
	case EVENT_DATABASE:		//���ݿ��¼�
		{
			//Ч�����
			//ASSERT(wDataSize>=sizeof(NTY_DataBaseEvent));
			if (wDataSize<sizeof(NTY_DataBaseEvent)) return;

			//������Ϣ
			NTY_DataBaseEvent * pDataBaseEvent=(NTY_DataBaseEvent *)pBuffer;
			m_pIMsgTriggerServiceSink->OnEventDataBase(pDataBaseEvent+1,wDataSize-sizeof(NTY_DataBaseEvent),pDataBaseEvent);

			return;
		}
	case EVENT_SOCKET_ACCEPT:	//����Ӧ���¼�
		{
			//Ч���С
			//ASSERT(wDataSize==sizeof(NTY_SocketAcceptEvent));
			if (wDataSize!=sizeof(NTY_SocketAcceptEvent)) return;

			//������Ϣ
			NTY_SocketAcceptEvent * pSocketAcceptEvent=(NTY_SocketAcceptEvent *)pBuffer;
			m_pIMsgTriggerServiceSink->OnEventSocketAccept(pSocketAcceptEvent);

			return;
		}
	case EVENT_SOCKET_READ:		//�����ȡ�¼�
		{
			//Ч���С
			NTY_SocketReadEvent * pSocketReadEvent=(NTY_SocketReadEvent *)pBuffer;
			//ASSERT(wDataSize>=sizeof(NTY_SocketReadEvent));
			//ASSERT(wDataSize==(sizeof(NTY_SocketReadEvent)+pSocketReadEvent->wDataSize));
			if (wDataSize<sizeof(NTY_SocketReadEvent)) return;
			if (wDataSize!=(sizeof(NTY_SocketReadEvent)+pSocketReadEvent->wDataSize)) return;

			//������Ϣ
			bool bSuccess=false;
			try 
			{ 
				bSuccess=m_pIMsgTriggerServiceSink->OnEventSocketRead(pSocketReadEvent->Command,pSocketReadEvent+1,pSocketReadEvent->wDataSize,pSocketReadEvent);
			}
			catch (...)	{ }
			if (bSuccess==false) m_pITCPSocketService->CloseSocket(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID);

			return;
		}
	case EVENT_SOCKET_CLOSE:	//����ر��¼�
		{
			//Ч���С
			//ASSERT(wDataSize==sizeof(NTY_SocketCloseEvent));
			if (wDataSize!=sizeof(NTY_SocketCloseEvent)) return;

			//������Ϣ
			NTY_SocketCloseEvent * pSocketCloseEvent=(NTY_SocketCloseEvent *)pBuffer;
			m_pIMsgTriggerServiceSink->OnEventSocketClose(pSocketCloseEvent);

			return;
		}
	}

	//�����¼�
	m_pIMsgTriggerServiceSink->OnMsgTrigger(wIdentifier,pBuffer,wDataSize,dwInsertTime); 

	return;
}
