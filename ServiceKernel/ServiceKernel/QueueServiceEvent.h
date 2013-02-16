#ifndef QUEUE_SERVICE_EVENT_HEAD_FILE
#define QUEUE_SERVICE_EVENT_HEAD_FILE

#pragma once

//#include "ServerKernel.h"
//
////////////////////////////////////////////////////////////////////////////
//��˵��
//���ݶ����¼�������
//ʹ���ߣ�ҵ���߼�����Ա
//ʹ�ó��ϣ���½����ˣ���Ϸ�����
//�����ݶ��з��͡������ݿ��¼�����������¼��������ȡ�¼�������ر��¼�
//ʹ�ò���:�ȵ���SetQueueService ���ú����ݶ��нӿڣ�Ȼ����ݷ����¼�Post��Ӧ��Event
//����ӵ���ߣ�
//CDataBaseSink
//CAttemperEngineSink
//CTCPSocketService

//�����ݿ�Ͷ��ע���¼�
//m_DataBaseEvent.PostDataBaseEvent(DBR_GP_REGISTER_ACCOUNTS,pSocketReadEvent->wIndex,
//	pSocketReadEvent->wRoundID,&RegisterAccounts,sizeof(RegisterAccounts));
//�����ݿ�Ͷ�ݵ�½�¼�
//m_DataBaseEvent.PostDataBaseEvent(DBR_GP_LOGON_BY_ACCOUNTS,pSocketReadEvent->wIndex,
//	pSocketReadEvent->wRoundID,&LogonByAccounts,sizeof(LogonByAccount	
//������
//CQueueService::AddToQueue->
//CDataStorage::AddData->
//PostQueuedCompletionStatus->
//CQueueServiceThread::RepetitionRun()
//GetQueuedCompletionStatus
//���ݶ����¼�������
class CQueueServiceEvent : public CObject
{
	//��������
public:
	CThreadLock						m_BufferLock;						//ͬ������
	IDataQueueSink *				m_pIQueueService;					//���нӿ�
	BYTE							m_cbBuffer[MAX_QUEUE_PACKET];		//���ջ���

	//��������
public:
	//���캯��
	CQueueServiceEvent() { m_pIQueueService=NULL; }
	//��������
	virtual ~CQueueServiceEvent() {}

	//������
public:
	//���ýӿ�
	bool SetQueueService(IDataQueueSink * pIObject)
	{
		//ASSERT(pIObject!=NULL);
		m_pIQueueService=pIObject;
		//ASSERT(m_pIQueueService!=NULL);
		return (m_pIQueueService!=NULL);
	}
	//��ȡ�ӿ�
	void * GetQueueService()
	{
		if (m_pIQueueService==NULL) return NULL;
		return m_pIQueueService;
	}
	//�Ƿ���Ч
	bool IsValid() { return (m_pIQueueService!=NULL); }

	//֪ͨ����
public:
	//���ݿ��¼�
	bool PostDataBaseEvent(WORD wRequestID, WORD wIndex, WORD wRoundID, const void * pDataBuffer, WORD wDataSize)
	{
		//Ч�����
		//ASSERT(m_pIQueueService!=NULL);
		//ASSERT((wDataSize+sizeof(NTY_DataBaseEvent))<=MAX_QUEUE_PACKET);
		if (m_pIQueueService==NULL) return false;
		if ((wDataSize+sizeof(NTY_DataBaseEvent))>MAX_QUEUE_PACKET) return false;

		//��������
		CThreadLockHandle BufferLockHandle(&m_BufferLock);

		//Ͷ����Ϣ
		NTY_DataBaseEvent * pDataBaseEvent=(NTY_DataBaseEvent *)m_cbBuffer;
		pDataBaseEvent->wIndex=wIndex;
		pDataBaseEvent->wRequestID=wRequestID;
		pDataBaseEvent->wRoundID=wRoundID;
		if (wDataSize>0)
		{
			//ASSERT(pDataBuffer!=NULL);
			CopyMemory(m_cbBuffer+sizeof(NTY_DataBaseEvent),pDataBuffer,wDataSize);
		}
		m_pIQueueService->AddToQueue(EVENT_DATABASE,m_cbBuffer,sizeof(NTY_DataBaseEvent)+wDataSize);

		return true;
	}
	//����Ӧ���¼�
	bool PostSocketAcceptEvent(WORD wIndex, WORD wRoundID, DWORD dwClientIP)
	{
		//Ч�����
		//ASSERT(m_pIQueueService!=NULL);
		if (m_pIQueueService==NULL) return false;

		//��������
		CThreadLockHandle BufferLockHandle(&m_BufferLock);

		//Ͷ����Ϣ
		NTY_SocketAcceptEvent * pSocketAcceptEvent=(NTY_SocketAcceptEvent *)m_cbBuffer;
		pSocketAcceptEvent->wIndex=wIndex;
		pSocketAcceptEvent->wRoundID=wRoundID;
		pSocketAcceptEvent->dwClientIP=dwClientIP;
		m_pIQueueService->AddToQueue(EVENT_SOCKET_ACCEPT,m_cbBuffer,sizeof(NTY_SocketAcceptEvent));

		return true;
	}
	//�����ȡ�¼�
	bool PostSocketReadEvent(WORD wIndex, WORD wRoundID, CMD_Command Command, const void * pDataBuffer, WORD wDataSize)
	{
		//Ч�����
		//ASSERT(m_pIQueueService!=NULL);
		//ASSERT((wDataSize+sizeof(NTY_SocketReadEvent))<=MAX_QUEUE_PACKET);
		if (m_pIQueueService==NULL) return false;
		if ((wDataSize+sizeof(NTY_SocketReadEvent))>MAX_QUEUE_PACKET) return false;

		//��������
		CThreadLockHandle BufferLockHandle(&m_BufferLock);

		//Ͷ����Ϣ
		NTY_SocketReadEvent * pSocketReadEvent=(NTY_SocketReadEvent *)m_cbBuffer;
		pSocketReadEvent->wIndex=wIndex;
		pSocketReadEvent->wRoundID=wRoundID;
		pSocketReadEvent->Command=Command;
		pSocketReadEvent->wDataSize=wDataSize;
		if (wDataSize>0)
		{
			//ASSERT(pDataBuffer!=NULL);
			CopyMemory(m_cbBuffer+sizeof(NTY_SocketReadEvent),pDataBuffer,wDataSize);
		}
		m_pIQueueService->AddToQueue(EVENT_SOCKET_READ,m_cbBuffer,sizeof(NTY_SocketReadEvent)+wDataSize);

		return false;
	}
	//����ر��¼�
	bool PostSocketCloseEvent(WORD wIndex, WORD wRoundID, DWORD dwClientIP, DWORD dwConnectSecond)
	{
		//Ч�����
		//ASSERT(m_pIQueueService!=NULL);
		if (m_pIQueueService==NULL) return false;

		//��������
		CThreadLockHandle BufferLockHandle(&m_BufferLock);

		//Ͷ����Ϣ
		NTY_SocketCloseEvent * pSocketCloseEvent=(NTY_SocketCloseEvent *)m_cbBuffer;
		pSocketCloseEvent->wIndex=wIndex;
		pSocketCloseEvent->wRoundID=wRoundID;
		pSocketCloseEvent->dwClientIP=dwClientIP;
		pSocketCloseEvent->dwConnectSecond=dwConnectSecond;
		m_pIQueueService->AddToQueue(EVENT_SOCKET_CLOSE,m_cbBuffer,sizeof(NTY_SocketCloseEvent));

		return true;
	}
};

//////////////////////////////////////////////////////////////////////////

#endif