#pragma once
//#include "Thread.h"
#include "QueueService.h"
#include "QueueServiceEvent.h"
#include "../../CommonService/CommonService/Thread.h"
//��������
enum enOperationType
{
	OperationType_Send,				//��������
	OperationType_Recv,				//��������
};
class CTCPSocketItem;
class COverLapped;
class CServerSocketRSThread;
class COverLappedSend;
class CTCPSocketService;
class CSocketAcceptThread;
template <enOperationType OperationType> class CTOverLapped;

typedef class CTOverLapped<OperationType_Recv> COverLappedRecv;
typedef CListTemplate<COverLappedSend *> COverLappedSendPtr;
typedef CListTemplate<COverLappedRecv *> COverLappedRecvPtr;
////////////////////
//���Ӷ���ص��ӿ�
interface IServerSocketItemSink
{
	//Ӧ����Ϣ
	virtual bool OnSocketAcceptEvent(CTCPSocketItem * pServerSocketItem)=NULL;
	//��ȡ��Ϣ
	virtual bool OnSocketReadEvent(CMD_Command Command, void * pBuffer, WORD wDataSize, CTCPSocketItem * pServerSocketItem)=NULL;
	//�ر���Ϣ
	virtual bool OnSocketCloseEvent(CTCPSocketItem * pServerSocketItem)=NULL;
};
//�ص��ṹ��
class COverLapped:public CObject
{
	//��������
public:
	WSABUF							m_WSABuffer;						//����ָ��
	OVERLAPPED						m_OverLapped;						//�ص��ṹ
	const enOperationType			m_OperationType;					//��������

	//��������
public:
	//���캯��
	COverLapped(enOperationType OperationType);
	//��������
	virtual ~COverLapped();

	//��Ϣ����
public:
	//��ȡ����
	enOperationType GetOperationType() { return m_OperationType; }
};

//�����ص��ṹ
class COverLappedSend : public COverLapped
{
	//���ݱ���
public:
	BYTE							m_cbBuffer[SOCKET_BUFFER];			//���ݻ���

	//��������
public:
	//���캯��
	COverLappedSend();
	//��������
	virtual ~COverLappedSend();
};

//�ص��ṹģ��
template <enOperationType OperationType> class CTOverLapped : public COverLapped
{
	//��������
public:
	//���캯��
	CTOverLapped() : COverLapped(OperationType) {}
	//��������
	virtual ~CTOverLapped() {}
};

//TCP SOCKET ��
class CTCPSocketItem:public CObject
{
	//��������
protected:
	DWORD							m_dwClientAddr;						//���ӵ�ַ
	DWORD							m_dwConnectTime;					//����ʱ��

	//���ı���
protected:
	WORD							m_wRountID;							//ѭ������
	SOCKET							m_hSocket;							//SOCKET ���

	//״̬����
protected:
	bool							m_bNotify;							//֪ͨ��־
	bool							m_bRecvIng;							//���ձ�־
	bool							m_bCloseIng;						//�رձ�־
	bool							m_bAllowBatch;						//����Ⱥ��
	WORD							m_wRecvSize;						//���ճ���
	BYTE							m_cbRecvBuf[SOCKET_BUFFER];			//���ջ���

	//��������
protected:
	DWORD							m_dwSendTickCount;					//����ʱ��
	DWORD							m_dwRecvTickCount;					//����ʱ��
	DWORD							m_dwSendPacketCount;				//���ͼ���
	DWORD							m_dwRecvPacketCount;				//���ܼ���

	//�ڲ�����
protected:
	const WORD						m_wIndex;							//��������
	CThreadLock						m_SocketLock;						//ͬ������
	IServerSocketItemSink			* m_pIServerSocketItemSink;			//�ص��ӿ�
	COverLappedRecv					m_OverLappedRecv;					//�ص��ṹ
	COverLappedSendPtr				m_OverLappedSendFree;				//�ص��ṹ
	COverLappedSendPtr				m_OverLappedSendActive;				//�ص��ṹ

	//��������
public:
	//���캯��
	CTCPSocketItem(WORD wIndex, IServerSocketItemSink * pIServerSocketItemSink);
	//��������
	virtual ~CTCPSocketItem(void);

	//��ʶ����
public:
	//��ȡ����
	WORD GetIndex() { return m_wIndex; }
	//��ȡ����
	WORD GetRountID() { return m_wRountID; }
	//��ȡ��ʶ
	DWORD GetIdentifierID() { return MAKELONG(m_wIndex,m_wRountID); }

	//��������
public:
	//��ȡ��ַ
	DWORD GetClientAddr() { return m_dwClientAddr; }
	//����ʱ��
	DWORD GetConnectTime() { return m_dwConnectTime; }
	//����ʱ��
	DWORD GetSendTickCount() { return m_dwSendTickCount; }
	//����ʱ��
	DWORD GetRecvTickCount() { return m_dwRecvTickCount; }
	//��������
	CThreadLock * GetSignedLock() { return &m_SocketLock; }
	//��׼����
	bool IsReadySend() { return m_dwRecvPacketCount>0L; }
	//�Ƿ�Ⱥ��
	bool IsAllowBatch() { return m_bAllowBatch; }
	//�ж�����
	bool IsValidSocket() { return (m_hSocket!=INVALID_SOCKET); }
	//֪ͨ�ӿ�
public:
	//�������֪ͨ
	bool OnSendCompleted(COverLappedSend * pOverLappedSend, DWORD dwThancferred);
	//�������֪ͨ
	bool OnRecvCompleted(COverLappedRecv * pOverLappedRecv, DWORD dwThancferred);
	//�ر����֪ͨ
	bool OnCloseCompleted();

	//���ܺ���
public:
	//�󶨶���
	DWORD Attach(SOCKET hSocket, DWORD dwClientAddr);
	//���ͺ���
	bool SendData(WORD wMainCmdID, WORD wSubCmdID, WORD wRountID);
	//���ͺ���
	bool SendData(void * pData, WORD wDataSize, WORD wMainCmdID, WORD wSubCmdID, WORD wRountID);
	//���ղ���
	bool RecvData();
	//�ر�����
	bool CloseSocket(WORD wRountID);
	//���ùر�
	bool ShutDownSocket(WORD wRountID);
	//����Ⱥ��
	bool AllowBatchSend(WORD wRountID, bool bAllowBatch);
	//���ñ���
	void ResetSocketData();
	//
	inline COverLappedSend * GetSendOverLapped();
};

//��˵��
typedef CListTemplate<CTCPSocketItem *> CServerSocketItemPtrArray;
typedef CListTemplate<CServerSocketRSThread *> CServerSocketRSThreadPtrArray;
//Ӧ���̶߳���


//��д�߳���
class CServerSocketRSThread : public CThread
{
	//��������
protected:
	HANDLE							m_hCompletionPort;					//��ɶ˿�

	//��������
public:
	//���캯��
	CServerSocketRSThread(void);
	//��������
	virtual ~CServerSocketRSThread(void);

	//���ܺ���
public:
	//���ú���
	bool InitThread(HANDLE hCompletionPort);

	//���غ���
private:
	//���к���
	virtual bool RepetitionRun();
};
class CSocketAcceptThread : public CThread
{
	//��������
protected:
	SOCKET							m_hListenSocket;					//��������
	HANDLE							m_hCompletionPort;					//��ɶ˿�
	CTCPSocketService				* m_pTCPSocketManager;				//����ָ��

	//��������
public:
	//���캯��
	CSocketAcceptThread(void);
	//��������
	virtual ~CSocketAcceptThread(void);

	//���ܺ���
public:
	//���ú���
	bool InitThread(HANDLE hCompletionPort, SOCKET hListenSocket, CTCPSocketService * pTCPSocketManager);

	//���غ���
private:
	//���к���
	virtual bool RepetitionRun();
};

//////////////////////////////////////////////////////////////////////////

//����߳���
class CSocketDetectThread : public CThread
{
	//��������
protected:
	DWORD							m_dwTickCount;						//����ʱ��
	CTCPSocketService				* m_pTCPSocketManager;				//����ָ��

	//��������
public:
	//���캯��
	CSocketDetectThread(void);
	//��������
	virtual ~CSocketDetectThread(void);

	//���ܺ���
public:
	//���ú���
	bool InitThread(CTCPSocketService * pTCPSocketManager);

	//���غ���
private:
	//���к���
	virtual bool RepetitionRun();
};

//���������
class CTCPSocketService : public ITCPSocketService, public ITCPSocketServiceManager, 
	public IServerSocketItemSink, public IDataQueueServiceSink ,public CObject
{
	friend class CServerSocketRSThread;
	friend class CSocketAcceptThread;

	//��������
protected:
	CThreadLock						m_ItemLocked;						//����ͬ��
	CServerSocketItemPtrArray		m_FreeSocketItem;					//��������
	CServerSocketItemPtrArray		m_ActiveSocketItem;					//�����
	CServerSocketItemPtrArray		m_StorageSocketItem;				//�洢����

	//��������
protected:
	DWORD							m_dwLastDetect;						//���ʱ��
	CServerSocketItemPtrArray		m_TempSocketItem;					//��ʱ����

	//���ñ���
protected:
	INT							m_wListenPort;						//�����˿�
	INT							m_wMaxSocketItem;					//�������
	CQueueServiceEvent				m_MsgTriggerEvent;					//֪ͨ���

	//�ں˱���
protected:
	bool							m_bService;							//�����־
	SOCKET							m_hServerSocket;					//���Ӿ��
	HANDLE							m_hCompletionPort;					//��ɶ˿�
	CDataQueueService				m_SendQueueService;					//���ж���
	CSocketDetectThread				m_SocketDetectThread;				//����߳�
	CSocketAcceptThread				m_SocketAcceptThread;				//Ӧ���߳�
	CServerSocketRSThreadPtrArray	m_SocketRSThreadArray;				//��д�߳�

	//��������
public:
	//���캯��
	CTCPSocketService(void);
	//��������
	virtual ~CTCPSocketService(void);


	//����ӿ�
public:
	//��������
	virtual bool __cdecl StartService();
	//ֹͣ����
	virtual bool __cdecl EndService();
	//���ö˿�
	virtual bool __cdecl SetServicePort(INT wListenPort);
	//������Ŀ
	virtual bool __cdecl SetMaxSocketItem(WORD wMaxSocketItem);
	//���ýӿ�
	virtual bool __cdecl SetSocketServiceSink(CObject * pIObject);

	//���нӿ�
public:
	//֪ͨ�ص�����
	virtual void __cdecl OnDataQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize, DWORD dwInsertTime);

	//����ӿ�
public:
	//�������
	virtual bool __cdecl DetectSocket();
	//���ͺ���
	virtual bool __cdecl SendData(WORD wIndex, WORD wRountID, WORD wMainCmdID, WORD wSubCmdID);
	//���ͺ���
	virtual bool __cdecl SendData(WORD wIndex, WORD wRountID, WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize);
	//��������
	virtual bool __cdecl SendDataBatch(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize);
	//�ر�����
	virtual bool __cdecl CloseSocket(WORD wIndex, WORD wRountID);
	//���ùر�
	virtual bool __cdecl ShutDownSocket(WORD wIndex, WORD wRountID);
	//����Ⱥ��
	virtual bool __cdecl AllowBatchSend(WORD wIndex, WORD wRountID, bool bAllow);

	//֪ͨ�ӿ�
public:
	//Ӧ����Ϣ
	virtual bool OnSocketAcceptEvent(CTCPSocketItem * pServerSocketItem);
	//��ȡ��Ϣ
	virtual bool OnSocketReadEvent(CMD_Command Command, void * pBuffer, WORD wDataSize, CTCPSocketItem * pServerSocketItem);
	//�ر���Ϣ
	virtual bool OnSocketCloseEvent(CTCPSocketItem * pServerSocketItem);

	//�ڲ�����
protected:
	//������ж���
	CTCPSocketItem * ActiveSocketItem();
	//��ȡ����
	CTCPSocketItem * EnumSocketItem(WORD wIndex);
	//�ͷ����Ӷ���
	bool FreeSocketItem(CTCPSocketItem * pTCPSocketItem);
};

