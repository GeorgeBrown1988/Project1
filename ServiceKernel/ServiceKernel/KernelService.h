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
	virtual bool __cdecl InitKernelService(INT wListenPort, INT wMaxSocketItem);
	//�����ѯ
public:
	//��ȡ���ݿ�����ӿ�
	virtual bool __cdecl GetDataBaseService(IDataBaseService ** pIObject);
	//��ȡ��������ӿ�
	virtual bool __cdecl GetMsgTriggerService(IMsgTriggerService ** pIObject);
	//��ȡ��������ӿ�
	virtual bool __cdecl GetTCPSocketService(ITCPSocketService ** pIObject);
	//��ȡ���ݿ���нӿ�
	virtual bool __cdecl GetDataBaseQueueService(IDataQueueService ** pIObject);
	//��ȡ���ȶ��нӿ�
	virtual bool __cdecl GetMsgTriggerQueueService(IDataQueueService ** pIObject);
//MAKR
	//���ܽӿ�
public:
	//����״̬
	virtual bool __cdecl IsService() { return m_bService; }
	//�ⲿ����
	virtual bool __cdecl ControlService(void * pBuffer, WORD wDataSize);
};
