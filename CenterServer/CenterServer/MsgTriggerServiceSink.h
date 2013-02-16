#pragma once
#include "stdafx.h"
#include "ServerList.h"
class CMsgTriggerServiceSink :public IMsgTriggerServiceSink
{

public:

	CMsgTriggerServiceSink(void);
	
	virtual ~CMsgTriggerServiceSink(void);

	//�������
protected:
	CQueueServiceEvent				m_DataBaseEvent;					//���ݿ�֪ͨ
	//�ӿڱ���
	ITCPSocketService				* m_pITCPSocketService;				//����ӿ�
	//
	CServerList						m_ServerListCenter;


	//���Ƚӿ�
public:
	//����ģ������
	virtual bool __cdecl StartService(CObject * pICObject);
	//����ģ��ر�
	virtual bool __cdecl EndService(CObject * pICObject);
	//�¼�����ӿ�
	virtual bool __cdecl OnMsgTrigger(WORD wIdentifier, void * pDataBuffer, WORD wDataSize, DWORD dwInsertTime);

	//���ú��� 
public:
	//���ú���
	bool InitMsgTriggerSink(CKernelService * pICObject);

	//�¼��ӿ�
public:
	//���ݿ��¼�
	virtual bool __cdecl OnEventDataBase(void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent);
	//����Ӧ���¼�
	virtual bool __cdecl OnEventSocketAccept(NTY_SocketAcceptEvent * pSocketAcceptEvent);
	//�����ȡ�¼�
	virtual bool __cdecl OnEventSocketRead(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent);
	//����ر��¼�
	virtual bool __cdecl OnEventSocketClose(NTY_SocketCloseEvent * pSocketCloseEvent);

	//���纯��
private:
	//�б���Ϣ����
	bool OnSocketMainServerList(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent);
	//������Ϣ����
	bool OnSocketMainServerManager(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent);

};

