#pragma once
#include "Interface.h"
#include "Thread.h"
#include "DataStorage.h"
#include "../../CommonService/CommonService/Thread.h"
//////////////////////////////////////////////////////////////////////////
//�����߳���
class CDataQueueServiceThread : public CThread
{
	//��Ա����
	friend class CDataQueueService;

	//���ö���
protected:
	HANDLE							m_hCompletionPort;					//��ɶ˿�

	//��������
private:
	BYTE							m_cbBuffer[MAX_QUEUE_PACKET];		//���ջ���

	//��������
protected:
	//���캯��
	CDataQueueServiceThread();
	//��������
	virtual ~CDataQueueServiceThread(void);

	//���ܺ���
public:
	//���ú���
	bool InitThread(HANDLE hCompletionPort);
	//ȡ������
	bool UnInitThread();

	//���غ���
private:
	//���к���
	virtual bool RepetitionRun();
};
//
//���ݶ�����
class CDataQueueService : public IDataQueueService,public IDataQueueSink 
{
	friend class CDataQueueServiceThread;

	//��������
protected:
	bool							m_bService;							//�����־
	HANDLE							m_hCompletionPort;					//��ɶ˿�
	IDataQueueServiceSink			*m_pIQueueServiceSink;				//�ص��ӿ�

	//�������
protected:
	CThreadLock						m_ThreadLock;						//�߳���
	CDataStorage					m_DataStorage;						//���ݴ洢
	CDataQueueServiceThread			m_QueueServiceThread;				//�����߳�

	//��������
public:
	//���캯��
	CDataQueueService(void);
	//��������
	virtual ~CDataQueueService(void);

	//���нӿ�
public:
	//��������
	virtual bool __cdecl AddToQueue(WORD wIdentifier, void * const pBuffer, WORD wDataSize);
	//����ӿ�
public:
	//��ʼ����
	virtual bool __cdecl StartService();
	//ֹͣ����
	virtual bool __cdecl EndService();
	//���ýӿ�
	virtual bool __cdecl SetDataQueueServiceSink(IDataQueueServiceSink * pobj);
	//������Ϣ
	virtual bool __cdecl GetDataQueueInfo(tagBurthenInfo & BurthenInfo);
	//��������
private:
	//��ȡ����
	bool GetData(tagDataHead & DataHead, void * pBuffer, WORD wBufferSize);
	//������Ϣ
	void OnQueueServiceThread(const tagDataHead & DataHead, void * pBuffer, WORD wDataSize);

private:
	CDataStorage	m_DataQueue;
	CThreadLock     m_Lock;
};