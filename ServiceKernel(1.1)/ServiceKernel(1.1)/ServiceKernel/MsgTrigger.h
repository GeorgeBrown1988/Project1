#pragma once
#include "Interface.h"
#include "QueueService.h"
//���ȹ�����
class CMsgTriggerService : public IMsgTriggerService, public IDataQueueServiceSink,public CObject
{
	//���ı���
protected:
	bool							m_bService;							//���б�־
	CDataQueueService				m_RequestQueueService;				//���ж���

	//�ӿڱ���
protected:
	ITCPSocketService				* m_pITCPSocketService;				//��������
	IMsgTriggerServiceSink			* m_pIMsgTriggerServiceSink;			//�ҽӽӿ�

	//��������
public:
	//���캯��
	CMsgTriggerService(void);
	//��������
	virtual ~CMsgTriggerService(void);


	//����ӿ�
public:
	//��������
	virtual bool __cdecl StartService();
	//ֹͣ����
	virtual bool __cdecl EndService();
	//��������
	virtual bool __cdecl SetSocketService(CObject * pIObject);
	//ע�ṳ��
	virtual bool __cdecl SetMsgTriggerServiceSink(CObject * pIObject);
	//��ȡ�ӿ�
	virtual void * __cdecl GetMsgTriggerQueueService();

	//���нӿ�
public:
	//�����ӿ�
	virtual void __cdecl OnDataQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize, DWORD dwInsertTime);
};