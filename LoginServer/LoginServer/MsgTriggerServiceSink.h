#pragma once

//class CMsgTriggerServiceSink :
//	public IMsgTriggerServiceSink
//{
//public:
//	CMsgTriggerServiceSink(void);
//	~CMsgTriggerServiceSink(void);
//	//����ӿ�
//public:
//	//����ģ������
//	virtual bool __cdecl StartService(CObject * pICObject);
//	//����ģ��ر�
//	virtual bool __cdecl EndService(CObject * pICObject);
//	//�¼�����ӿ�
//	virtual bool __cdecl OnMsgTrigger(WORD wIdentifier, void * pBuffer, WORD wDataSize, DWORD dwInsertTime);
//	//�¼��ӿ�
//public:
//	//���ݿ��¼�
//	virtual bool __cdecl OnEventDataBase(void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent);
//	//����Ӧ���¼�
//	virtual bool __cdecl OnEventSocketAccept(NTY_SocketAcceptEvent * pSocketAcceptEvent);
//	//�����ȡ�¼�
//	virtual bool __cdecl OnEventSocketRead(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent);
//	//����ر��¼�
//	virtual bool __cdecl OnEventSocketClose(NTY_SocketCloseEvent * pSocketCloseEvent);
//
//	//���ú��� 
//public:
//	//���ú���
//	bool InitMsgTriggerSink(CKernelService * pICObject);
//
//	//���ݿ⺯��
//private:
//	//�û���¼�ɹ�
//	bool OnDBLogonSuccess(void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent);
//	//�û���¼ʧ��
//	bool OnDBLogonError(void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent);
//
//	//���纯��
//private:
//	//��¼��Ϣ����
//	bool OnSocketMainLogon(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent);
//
//	//��������
//private:
//	//�б���Ϣ����
//	bool OnCenterMainServerList(CMD_Command Command, void * pDataBuffer, WORD wDataSize);
//
//	//��������
//protected:
////	CInitParamter					* m_pInitParamter;					//���ò���
//	tagConnectItemInfo				* m_pConnectItemInfo;				//������Ϣ
//
//	//���ķ�����
////protected:
////	enSocketState					m_SocketState;						//����״̬
////	CCenterSocket					* m_pCenterSocket;					//��������
//
//	//�������
//protected:
//	CServerList						m_ServerList;						//�������б�
//	CQueueServiceEvent				m_DataBaseEvent;					//���ݿ�֪ͨ
//
//	//�ӿڱ���
//protected:
//	ITCPSocketService				* m_pITCPSocketService;				//����ӿ�
//};
//
