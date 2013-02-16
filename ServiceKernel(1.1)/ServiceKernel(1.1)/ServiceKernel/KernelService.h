#pragma once
#include "SocketService.h"
#include "MsgTrigger.h"
#include "DataBase.h"



//服务引擎
class CKernelService : public IKernelService
{
	//变量定义
protected:
	bool								m_bService;						//运行标志

	//组件变量
protected:
	CDataBaseService						m_DataBaseService;				//数据库引擎
	CMsgTriggerService						m_MsgTriggerService;				//调度引擎
	CTCPSocketService						m_TCPSocketService;				//网络引擎

	//函数定义
public:
	//构造函数
	CKernelService();
	//析构函数
	virtual ~CKernelService();


	//服务接口
public:
	//启动服务
	virtual bool __cdecl StartService();
	//停止服务
	virtual bool __cdecl EndService();

	//配置接口
public:
	//设置事件
	//virtual bool __cdecl SetEventService(IEventService * pIObject);
	//设置钩子
	virtual bool __cdecl SetDataBaseSink(CObject * pIObject);
	//设置钩子
	virtual bool __cdecl SetMsgTriggerServiceSink(CObject * pIObject);
	//设置网络
	virtual bool __cdecl InitKernelService(WORD wListenPort, WORD wMaxSocketItem);
	//服务查询
public:
	//获取数据库引擎接口
	virtual void * __cdecl GetDataBaseService();
	//获取调度引擎接口
	virtual void * __cdecl GetMsgTriggerService();
	//获取网络引擎接口
	virtual void * __cdecl GetTCPSocketService();
	//获取数据库队列接口
	virtual void * __cdecl GetDataBaseQueueService();
	//获取调度队列接口
	virtual void * __cdecl GetMsgTriggerQueueService();

	//功能接口
public:
	//服务状态
	virtual bool __cdecl IsService() { return m_bService; }
	//外部控制
	virtual bool __cdecl ControlService(void * pBuffer, WORD wDataSize);
};
