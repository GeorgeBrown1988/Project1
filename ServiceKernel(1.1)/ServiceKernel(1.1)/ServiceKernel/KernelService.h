#pragma once
#include "SocketService.h"
#include "MsgTrigger.h"
#include "DataBase.h"



//��������
class CKernelService : public IKernelService
{
	//��������
protected:
	bool								m_bService;						//���б�־

	//�������
protected:
	CDataBaseService						m_DataBaseService;				//���ݿ�����
	CMsgTriggerService						m_MsgTriggerService;				//��������
	CTCPSocketService						m_TCPSocketService;				//��������

	//��������
public:
	//���캯��
	CKernelService();
	//��������
	virtual ~CKernelService();


	//����ӿ�
public:
	//��������
	virtual bool __cdecl StartService();
	//ֹͣ����
	virtual bool __cdecl EndService();

	//���ýӿ�
public:
	//�����¼�
	//virtual bool __cdecl SetEventService(IEventService * pIObject);
	//���ù���
	virtual bool __cdecl SetDataBaseSink(CObject * pIObject);
	//���ù���
	virtual bool __cdecl SetMsgTriggerServiceSink(CObject * pIObject);
	//��������
	virtual bool __cdecl InitKernelService(WORD wListenPort, WORD wMaxSocketItem);
	//�����ѯ
public:
	//��ȡ���ݿ�����ӿ�
	virtual void * __cdecl GetDataBaseService();
	//��ȡ��������ӿ�
	virtual void * __cdecl GetMsgTriggerService();
	//��ȡ��������ӿ�
	virtual void * __cdecl GetTCPSocketService();
	//��ȡ���ݿ���нӿ�
	virtual void * __cdecl GetDataBaseQueueService();
	//��ȡ���ȶ��нӿ�
	virtual void * __cdecl GetMsgTriggerQueueService();

	//���ܽӿ�
public:
	//����״̬
	virtual bool __cdecl IsService() { return m_bService; }
	//�ⲿ����
	virtual bool __cdecl ControlService(void * pBuffer, WORD wDataSize);
};
