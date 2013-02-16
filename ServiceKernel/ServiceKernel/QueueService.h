#pragma once
#include "Interface.h"
#include "Thread.h"
#include "DataStorage.h"
#include "../../CommonService/CommonService/Thread.h"
//////////////////////////////////////////////////////////////////////////
//队列线程类
class CDataQueueServiceThread : public CThread
{
	//友员定义
	friend class CDataQueueService;

	//配置定义
protected:
	HANDLE							m_hCompletionPort;					//完成端口

	//辅助变量
private:
	BYTE							m_cbBuffer[MAX_QUEUE_PACKET];		//接收缓冲

	//函数定义
protected:
	//构造函数
	CDataQueueServiceThread();
	//析构函数
	virtual ~CDataQueueServiceThread(void);

	//功能函数
public:
	//配置函数
	bool InitThread(HANDLE hCompletionPort);
	//取消配置
	bool UnInitThread();

	//重载函数
private:
	//运行函数
	virtual bool RepetitionRun();
};
//
//数据队列类
class CDataQueueService : public IDataQueueService,public IDataQueueSink 
{
	friend class CDataQueueServiceThread;

	//变量定义
protected:
	bool							m_bService;							//服务标志
	HANDLE							m_hCompletionPort;					//完成端口
	IDataQueueServiceSink			*m_pIQueueServiceSink;				//回调接口

	//组件变量
protected:
	CThreadLock						m_ThreadLock;						//线程锁
	CDataStorage					m_DataStorage;						//数据存储
	CDataQueueServiceThread			m_QueueServiceThread;				//队列线程

	//函数定义
public:
	//构造函数
	CDataQueueService(void);
	//析构函数
	virtual ~CDataQueueService(void);

	//队列接口
public:
	//加入数据
	virtual bool __cdecl AddToQueue(WORD wIdentifier, void * const pBuffer, WORD wDataSize);
	//管理接口
public:
	//开始服务
	virtual bool __cdecl StartService();
	//停止服务
	virtual bool __cdecl EndService();
	//设置接口
	virtual bool __cdecl SetDataQueueServiceSink(IDataQueueServiceSink * pobj);
	//负荷信息
	virtual bool __cdecl GetDataQueueInfo(tagBurthenInfo & BurthenInfo);
	//辅助函数
private:
	//提取数据
	bool GetData(tagDataHead & DataHead, void * pBuffer, WORD wBufferSize);
	//数据消息
	void OnQueueServiceThread(const tagDataHead & DataHead, void * pBuffer, WORD wDataSize);

private:
	CDataStorage	m_DataQueue;
	CThreadLock     m_Lock;
};