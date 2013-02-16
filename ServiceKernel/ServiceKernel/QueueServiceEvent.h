#ifndef QUEUE_SERVICE_EVENT_HEAD_FILE
#define QUEUE_SERVICE_EVENT_HEAD_FILE

#pragma once

//#include "ServerKernel.h"
//
////////////////////////////////////////////////////////////////////////////
//类说明
//数据队列事件触发器
//使用者：业务逻辑程序员
//使用场合：登陆服务端，游戏服务端
//向数据队列发送——数据库事件，网络接受事件，网络读取事件，网络关闭事件
//使用步骤:先调用SetQueueService 设置好数据队列接口，然后根据服务事件Post相应的Event
//对象拥有者：
//CDataBaseSink
//CAttemperEngineSink
//CTCPSocketService

//向数据库投递注册事件
//m_DataBaseEvent.PostDataBaseEvent(DBR_GP_REGISTER_ACCOUNTS,pSocketReadEvent->wIndex,
//	pSocketReadEvent->wRoundID,&RegisterAccounts,sizeof(RegisterAccounts));
//向数据库投递登陆事件
//m_DataBaseEvent.PostDataBaseEvent(DBR_GP_LOGON_BY_ACCOUNTS,pSocketReadEvent->wIndex,
//	pSocketReadEvent->wRoundID,&LogonByAccounts,sizeof(LogonByAccount	
//触发：
//CQueueService::AddToQueue->
//CDataStorage::AddData->
//PostQueuedCompletionStatus->
//CQueueServiceThread::RepetitionRun()
//GetQueuedCompletionStatus
//数据队列事件触发器
class CQueueServiceEvent : public CObject
{
	//变量定义
public:
	CThreadLock						m_BufferLock;						//同步锁定
	IDataQueueSink *				m_pIQueueService;					//队列接口
	BYTE							m_cbBuffer[MAX_QUEUE_PACKET];		//接收缓冲

	//函数定义
public:
	//构造函数
	CQueueServiceEvent() { m_pIQueueService=NULL; }
	//析构函数
	virtual ~CQueueServiceEvent() {}

	//管理函数
public:
	//设置接口
	bool SetQueueService(IDataQueueSink * pIObject)
	{
		//ASSERT(pIObject!=NULL);
		m_pIQueueService=pIObject;
		//ASSERT(m_pIQueueService!=NULL);
		return (m_pIQueueService!=NULL);
	}
	//获取接口
	void * GetQueueService()
	{
		if (m_pIQueueService==NULL) return NULL;
		return m_pIQueueService;
	}
	//是否有效
	bool IsValid() { return (m_pIQueueService!=NULL); }

	//通知函数
public:
	//数据库事件
	bool PostDataBaseEvent(WORD wRequestID, WORD wIndex, WORD wRoundID, const void * pDataBuffer, WORD wDataSize)
	{
		//效验参数
		//ASSERT(m_pIQueueService!=NULL);
		//ASSERT((wDataSize+sizeof(NTY_DataBaseEvent))<=MAX_QUEUE_PACKET);
		if (m_pIQueueService==NULL) return false;
		if ((wDataSize+sizeof(NTY_DataBaseEvent))>MAX_QUEUE_PACKET) return false;

		//缓冲锁定
		CThreadLockHandle BufferLockHandle(&m_BufferLock);

		//投递消息
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
	//网络应答事件
	bool PostSocketAcceptEvent(WORD wIndex, WORD wRoundID, DWORD dwClientIP)
	{
		//效验参数
		//ASSERT(m_pIQueueService!=NULL);
		if (m_pIQueueService==NULL) return false;

		//缓冲锁定
		CThreadLockHandle BufferLockHandle(&m_BufferLock);

		//投递消息
		NTY_SocketAcceptEvent * pSocketAcceptEvent=(NTY_SocketAcceptEvent *)m_cbBuffer;
		pSocketAcceptEvent->wIndex=wIndex;
		pSocketAcceptEvent->wRoundID=wRoundID;
		pSocketAcceptEvent->dwClientIP=dwClientIP;
		m_pIQueueService->AddToQueue(EVENT_SOCKET_ACCEPT,m_cbBuffer,sizeof(NTY_SocketAcceptEvent));

		return true;
	}
	//网络读取事件
	bool PostSocketReadEvent(WORD wIndex, WORD wRoundID, CMD_Command Command, const void * pDataBuffer, WORD wDataSize)
	{
		//效验参数
		//ASSERT(m_pIQueueService!=NULL);
		//ASSERT((wDataSize+sizeof(NTY_SocketReadEvent))<=MAX_QUEUE_PACKET);
		if (m_pIQueueService==NULL) return false;
		if ((wDataSize+sizeof(NTY_SocketReadEvent))>MAX_QUEUE_PACKET) return false;

		//缓冲锁定
		CThreadLockHandle BufferLockHandle(&m_BufferLock);

		//投递消息
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
	//网络关闭事件
	bool PostSocketCloseEvent(WORD wIndex, WORD wRoundID, DWORD dwClientIP, DWORD dwConnectSecond)
	{
		//效验参数
		//ASSERT(m_pIQueueService!=NULL);
		if (m_pIQueueService==NULL) return false;

		//缓冲锁定
		CThreadLockHandle BufferLockHandle(&m_BufferLock);

		//投递消息
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