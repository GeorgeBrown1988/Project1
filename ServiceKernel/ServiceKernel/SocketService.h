#pragma once
//#include "Thread.h"
#include "QueueService.h"
#include "QueueServiceEvent.h"
#include "../../CommonService/CommonService/Thread.h"
//控制类型
enum enOperationType
{
	OperationType_Send,				//发送类型
	OperationType_Recv,				//接收类型
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
//连接对象回调接口
interface IServerSocketItemSink
{
	//应答消息
	virtual bool OnSocketAcceptEvent(CTCPSocketItem * pServerSocketItem)=NULL;
	//读取消息
	virtual bool OnSocketReadEvent(CMD_Command Command, void * pBuffer, WORD wDataSize, CTCPSocketItem * pServerSocketItem)=NULL;
	//关闭消息
	virtual bool OnSocketCloseEvent(CTCPSocketItem * pServerSocketItem)=NULL;
};
//重叠结构类
class COverLapped:public CObject
{
	//变量定义
public:
	WSABUF							m_WSABuffer;						//数据指针
	OVERLAPPED						m_OverLapped;						//重叠结构
	const enOperationType			m_OperationType;					//操作类型

	//函数定义
public:
	//构造函数
	COverLapped(enOperationType OperationType);
	//析构函数
	virtual ~COverLapped();

	//信息函数
public:
	//获取类型
	enOperationType GetOperationType() { return m_OperationType; }
};

//接收重叠结构
class COverLappedSend : public COverLapped
{
	//数据变量
public:
	BYTE							m_cbBuffer[SOCKET_BUFFER];			//数据缓冲

	//函数定义
public:
	//构造函数
	COverLappedSend();
	//析构函数
	virtual ~COverLappedSend();
};

//重叠结构模板
template <enOperationType OperationType> class CTOverLapped : public COverLapped
{
	//函数定义
public:
	//构造函数
	CTOverLapped() : COverLapped(OperationType) {}
	//析构函数
	virtual ~CTOverLapped() {}
};

//TCP SOCKET 类
class CTCPSocketItem:public CObject
{
	//连接属性
protected:
	DWORD							m_dwClientAddr;						//连接地址
	DWORD							m_dwConnectTime;					//连接时间

	//核心变量
protected:
	WORD							m_wRountID;							//循环索引
	SOCKET							m_hSocket;							//SOCKET 句柄

	//状态变量
protected:
	bool							m_bNotify;							//通知标志
	bool							m_bRecvIng;							//接收标志
	bool							m_bCloseIng;						//关闭标志
	bool							m_bAllowBatch;						//接受群发
	WORD							m_wRecvSize;						//接收长度
	BYTE							m_cbRecvBuf[SOCKET_BUFFER];			//接收缓冲

	//计数变量
protected:
	DWORD							m_dwSendTickCount;					//发送时间
	DWORD							m_dwRecvTickCount;					//接受时间
	DWORD							m_dwSendPacketCount;				//发送计数
	DWORD							m_dwRecvPacketCount;				//接受计数

	//内部变量
protected:
	const WORD						m_wIndex;							//连接索引
	CThreadLock						m_SocketLock;						//同步锁定
	IServerSocketItemSink			* m_pIServerSocketItemSink;			//回调接口
	COverLappedRecv					m_OverLappedRecv;					//重叠结构
	COverLappedSendPtr				m_OverLappedSendFree;				//重叠结构
	COverLappedSendPtr				m_OverLappedSendActive;				//重叠结构

	//函数定义
public:
	//构造函数
	CTCPSocketItem(WORD wIndex, IServerSocketItemSink * pIServerSocketItemSink);
	//析够函数
	virtual ~CTCPSocketItem(void);

	//标识函数
public:
	//获取索引
	WORD GetIndex() { return m_wIndex; }
	//获取计数
	WORD GetRountID() { return m_wRountID; }
	//获取标识
	DWORD GetIdentifierID() { return MAKELONG(m_wIndex,m_wRountID); }

	//辅助函数
public:
	//获取地址
	DWORD GetClientAddr() { return m_dwClientAddr; }
	//连接时间
	DWORD GetConnectTime() { return m_dwConnectTime; }
	//发送时间
	DWORD GetSendTickCount() { return m_dwSendTickCount; }
	//接收时间
	DWORD GetRecvTickCount() { return m_dwRecvTickCount; }
	//锁定对象
	CThreadLock * GetSignedLock() { return &m_SocketLock; }
	//是准备好
	bool IsReadySend() { return m_dwRecvPacketCount>0L; }
	//是否群发
	bool IsAllowBatch() { return m_bAllowBatch; }
	//判断连接
	bool IsValidSocket() { return (m_hSocket!=INVALID_SOCKET); }
	//通知接口
public:
	//发送完成通知
	bool OnSendCompleted(COverLappedSend * pOverLappedSend, DWORD dwThancferred);
	//接收完成通知
	bool OnRecvCompleted(COverLappedRecv * pOverLappedRecv, DWORD dwThancferred);
	//关闭完成通知
	bool OnCloseCompleted();

	//功能函数
public:
	//绑定对象
	DWORD Attach(SOCKET hSocket, DWORD dwClientAddr);
	//发送函数
	bool SendData(WORD wMainCmdID, WORD wSubCmdID, WORD wRountID);
	//发送函数
	bool SendData(void * pData, WORD wDataSize, WORD wMainCmdID, WORD wSubCmdID, WORD wRountID);
	//接收操作
	bool RecvData();
	//关闭连接
	bool CloseSocket(WORD wRountID);
	//设置关闭
	bool ShutDownSocket(WORD wRountID);
	//允许群发
	bool AllowBatchSend(WORD wRountID, bool bAllowBatch);
	//重置变量
	void ResetSocketData();
	//
	inline COverLappedSend * GetSendOverLapped();
};

//类说明
typedef CListTemplate<CTCPSocketItem *> CServerSocketItemPtrArray;
typedef CListTemplate<CServerSocketRSThread *> CServerSocketRSThreadPtrArray;
//应答线程对象


//读写线程类
class CServerSocketRSThread : public CThread
{
	//变量定义
protected:
	HANDLE							m_hCompletionPort;					//完成端口

	//函数定义
public:
	//构造函数
	CServerSocketRSThread(void);
	//析构函数
	virtual ~CServerSocketRSThread(void);

	//功能函数
public:
	//配置函数
	bool InitThread(HANDLE hCompletionPort);

	//重载函数
private:
	//运行函数
	virtual bool RepetitionRun();
};
class CSocketAcceptThread : public CThread
{
	//变量定义
protected:
	SOCKET							m_hListenSocket;					//监听连接
	HANDLE							m_hCompletionPort;					//完成端口
	CTCPSocketService				* m_pTCPSocketManager;				//管理指针

	//函数定义
public:
	//构造函数
	CSocketAcceptThread(void);
	//析构函数
	virtual ~CSocketAcceptThread(void);

	//功能函数
public:
	//配置函数
	bool InitThread(HANDLE hCompletionPort, SOCKET hListenSocket, CTCPSocketService * pTCPSocketManager);

	//重载函数
private:
	//运行函数
	virtual bool RepetitionRun();
};

//////////////////////////////////////////////////////////////////////////

//检测线程类
class CSocketDetectThread : public CThread
{
	//变量定义
protected:
	DWORD							m_dwTickCount;						//积累时间
	CTCPSocketService				* m_pTCPSocketManager;				//管理指针

	//函数定义
public:
	//构造函数
	CSocketDetectThread(void);
	//析构函数
	virtual ~CSocketDetectThread(void);

	//功能函数
public:
	//配置函数
	bool InitThread(CTCPSocketService * pTCPSocketManager);

	//重载函数
private:
	//运行函数
	virtual bool RepetitionRun();
};

//网络管理类
class CTCPSocketService : public ITCPSocketService, public ITCPSocketServiceManager, 
	public IServerSocketItemSink, public IDataQueueServiceSink ,public CObject
{
	friend class CServerSocketRSThread;
	friend class CSocketAcceptThread;

	//变量定义
protected:
	CThreadLock						m_ItemLocked;						//连接同步
	CServerSocketItemPtrArray		m_FreeSocketItem;					//空闲连接
	CServerSocketItemPtrArray		m_ActiveSocketItem;					//活动连接
	CServerSocketItemPtrArray		m_StorageSocketItem;				//存储连接

	//辅助变量
protected:
	DWORD							m_dwLastDetect;						//检测时间
	CServerSocketItemPtrArray		m_TempSocketItem;					//临时连接

	//配置变量
protected:
	INT							m_wListenPort;						//监听端口
	INT							m_wMaxSocketItem;					//最大连接
	CQueueServiceEvent				m_MsgTriggerEvent;					//通知组件

	//内核变量
protected:
	bool							m_bService;							//服务标志
	SOCKET							m_hServerSocket;					//连接句柄
	HANDLE							m_hCompletionPort;					//完成端口
	CDataQueueService				m_SendQueueService;					//队列对象
	CSocketDetectThread				m_SocketDetectThread;				//检测线程
	CSocketAcceptThread				m_SocketAcceptThread;				//应答线程
	CServerSocketRSThreadPtrArray	m_SocketRSThreadArray;				//读写线程

	//函数定义
public:
	//构造函数
	CTCPSocketService(void);
	//析构函数
	virtual ~CTCPSocketService(void);


	//服务接口
public:
	//启动服务
	virtual bool __cdecl StartService();
	//停止服务
	virtual bool __cdecl EndService();
	//设置端口
	virtual bool __cdecl SetServicePort(INT wListenPort);
	//设置数目
	virtual bool __cdecl SetMaxSocketItem(WORD wMaxSocketItem);
	//设置接口
	virtual bool __cdecl SetSocketServiceSink(CObject * pIObject);

	//队列接口
public:
	//通知回调函数
	virtual void __cdecl OnDataQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize, DWORD dwInsertTime);

	//网络接口
public:
	//检测连接
	virtual bool __cdecl DetectSocket();
	//发送函数
	virtual bool __cdecl SendData(WORD wIndex, WORD wRountID, WORD wMainCmdID, WORD wSubCmdID);
	//发送函数
	virtual bool __cdecl SendData(WORD wIndex, WORD wRountID, WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize);
	//批量发送
	virtual bool __cdecl SendDataBatch(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize);
	//关闭连接
	virtual bool __cdecl CloseSocket(WORD wIndex, WORD wRountID);
	//设置关闭
	virtual bool __cdecl ShutDownSocket(WORD wIndex, WORD wRountID);
	//允许群发
	virtual bool __cdecl AllowBatchSend(WORD wIndex, WORD wRountID, bool bAllow);

	//通知接口
public:
	//应答消息
	virtual bool OnSocketAcceptEvent(CTCPSocketItem * pServerSocketItem);
	//读取消息
	virtual bool OnSocketReadEvent(CMD_Command Command, void * pBuffer, WORD wDataSize, CTCPSocketItem * pServerSocketItem);
	//关闭消息
	virtual bool OnSocketCloseEvent(CTCPSocketItem * pServerSocketItem);

	//内部函数
protected:
	//激活空闲对象
	CTCPSocketItem * ActiveSocketItem();
	//获取对象
	CTCPSocketItem * EnumSocketItem(WORD wIndex);
	//释放连接对象
	bool FreeSocketItem(CTCPSocketItem * pTCPSocketItem);
};

