#pragma once
#include "Interface.h"
#include "QueueService.h"
//调度管理类
class CMsgTriggerService : public IMsgTriggerService, public IDataQueueServiceSink,public CObject
{
	//核心变量
protected:
	bool							m_bService;							//运行标志
	CDataQueueService				m_RequestQueueService;				//队列对象

	//接口变量
protected:
	ITCPSocketService				* m_pITCPSocketService;				//网络引擎
	IMsgTriggerServiceSink			* m_pIMsgTriggerServiceSink;			//挂接接口

	//函数定义
public:
	//构造函数
	CMsgTriggerService(void);
	//析构函数
	virtual ~CMsgTriggerService(void);


	//管理接口
public:
	//启动服务
	virtual bool __cdecl StartService();
	//停止服务
	virtual bool __cdecl EndService();
	//设置网络
	virtual bool __cdecl SetSocketService(CObject * pIObject);
	//注册钩子
	virtual bool __cdecl SetMsgTriggerServiceSink(CObject * pIObject);
	//获取接口
	virtual void * __cdecl GetMsgTriggerQueueService();

	//队列接口
public:
	//触发接口
	virtual void __cdecl OnDataQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize, DWORD dwInsertTime);
};