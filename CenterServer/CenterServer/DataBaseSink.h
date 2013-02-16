#pragma once
#include "CenterServer.h"
class CDataBaseSink :
	public IDataBaseSink
{
public:
	CDataBaseSink(void);
	~CDataBaseSink(void);
	//���Ƚӿ�
public:
	//���ݿ�ģ������
	virtual bool __cdecl StartService(CObject * pObject);
	//���ݿ�ģ��ر�
	virtual bool __cdecl EndService(CObject * pObject);
	//���ݲ�������
	virtual bool __cdecl OnDataBaseRequest(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);

	//���ú��� 
public:
	//��ʼ�����ݿ�ģ��
	bool InitDataBaseSink(IKernelService *pObject);

private:
	//���ݿ�
	CMysqlDataBase			*m_pDataBase;
	//���ݶ����¼�
	CQueueServiceEvent		m_QueueServiceEvent;

};

