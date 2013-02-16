#include "StdAfx.h"
#include "KernelService.h"


//构造函数
CKernelService::CKernelService()
{
#ifdef _DEBUG
	LOG(INFO) <<"CKernelService";
#endif
	m_bService=false;
	return;
}

//析构函数
CKernelService::~CKernelService()
{
#ifdef _DEBUG
	LOG(INFO) <<"~CKernelService";
#endif
	//停止服务
	EndService();

	return;
}


//获取调度引擎接口
bool __cdecl CKernelService::GetMsgTriggerService(IMsgTriggerService ** pIObject)
{
	*pIObject = dynamic_cast<CMsgTriggerService*>(&m_MsgTriggerService);
	if(*pIObject)
		return true;
	else
		return false;
}

//获取网络引擎接口
bool __cdecl CKernelService::GetTCPSocketService(ITCPSocketService ** pIObject)
{
	*pIObject = dynamic_cast<CTCPSocketService*>(&m_TCPSocketService);
	if(*pIObject)
		return true;
	else
		return false;	//m_TCPSocketService;
}

//获取数据库引擎接口
bool __cdecl CKernelService::GetDataBaseService(IDataBaseService ** pIObject)
{
	*pIObject = dynamic_cast<CDataBaseService*>(&m_DataBaseService);
	if(*pIObject)
		return true;
	else
		return false;
}

	////获取数据库引擎接口
	//virtual bool __cdecl GetDataBaseService(IDataBaseService * pIObject)=NULL;
	////获取调度引擎接口
	//virtual bool __cdecl GetMsgTriggerService(IMsgTriggerService * pIObject)=NULL;
	////获取网络引擎接口
	//virtual bool __cdecl GetTCPSocketService(ITCPSocketService * pIObject)=NULL;
	////获取接口
	//virtual bool __cdecl GetDataBaseQueueService(IDataQueueService * pIObject)=NULL;
	////获取接口
	//virtual bool __cdecl GetMsgTriggerQueueService(IDataQueueService * pIObject)=NULL;

//获取数据库队列接口
bool __cdecl CKernelService::GetDataBaseQueueService(IDataQueueService ** pIObject)
{
	return m_DataBaseService.GetQueueService(pIObject);//.GetQueueService;//mark
}

//获取调度队列接口
bool __cdecl CKernelService::GetMsgTriggerQueueService(IDataQueueService ** pIObject)
{
	return m_MsgTriggerService.GetMsgTriggerQueueService(pIObject);
}

//外部控制
bool __cdecl CKernelService::ControlService(void * pBuffer, WORD wDataSize)
{
	//效验状态
	if (m_bService==false) 
	{
		#ifdef _DEBUG
				LOG(INFO)<<"服务引擎处于停止状态中，控制操作忽略";
		#endif
		return false;
	}

	//插入数据
	IDataQueueService * pIQueueService;
	if(m_MsgTriggerService.GetMsgTriggerQueueService(&pIQueueService)==false)
	{
		#ifdef _DEBUG
				LOG(INFO)<<"获取调度引擎触发接口失败";
		#endif	
		return false;
	}

	//投递指令
	IDataQueueSink *pDataQueueSink = dynamic_cast<IDataQueueSink*>(pIQueueService);
	if (pDataQueueSink->AddToQueue(EVENT_CONTROL,pBuffer,wDataSize)==false)
	{
		#ifdef _DEBUG
				LOG(INFO)<<"投递控制消息指令失败";
		#endif	
//		m_EventTrace.LOG(INFO)<<(TEXT("投递控制消息指令失败"),Level_Exception);
		return false;
	}

	return true;
}

//启动服务
bool __cdecl CKernelService::StartService()
{
	//判断状态
	if (m_bService==true)
	{
		#ifdef _DEBUG
				LOG(INFO)<<"服务引擎重复启动，启动操作忽略";
		#endif	
//		m_EventTrace.LOG(INFO)<<(TEXT("服务引擎重复启动，启动操作忽略"),Level_Warning);
		return true;
	}
	//接口查询
	ITCPSocketService * pISocketService=&m_TCPSocketService;
	IDataQueueService * pIMsgTriggerQueueService;
	if(m_MsgTriggerService.GetMsgTriggerQueueService(&pIMsgTriggerQueueService) == false)
	{
		#ifdef _DEBUG
			LOG(INFO)<<"调度引擎队列获取失败";
		#endif
		return false;
	}
	//绑定组件
	if (m_MsgTriggerService.SetSocketService(pISocketService)==false)
	{
		#ifdef _DEBUG
			LOG(INFO)<<"网络引擎与调度引擎绑定失败";
		#endif
		return false;
	}
	//if (m_TimerService.SetTimerServiceSink(pIMsgTriggerService)==false)
	//{
	////	 LOG(INFO)<<(TEXT("定时器引擎与调度引擎绑定失败"),Level_Exception);
	//	return false;
	//}
	if (m_TCPSocketService.SetSocketServiceSink(pIMsgTriggerQueueService)==false)
	{
		#ifdef _DEBUG
			LOG(INFO)<<"网络引擎与调度引擎队列绑定失败";
		#endif
		return false;
	}
	//MessageBox(NULL,"TEST","TEST",MB_OK);
	//启动引擎服务
	//if (m_TimerService.StartService()==false) return false;
	if (m_DataBaseService.StartService()==false) return false;
	//if (m_AsynchronismService.StartService()==false) return false;
	if (m_MsgTriggerService.StartService()==false) return false;
	if (m_TCPSocketService.StartService()==false) return false;

	//设置变量
	m_bService=true;

	return true;
}

//停止服务
bool __cdecl CKernelService::EndService()
{
	//设置变量
	m_bService=false;

	//删除定时器
	//m_TimerService.KillAllTimer();

	//停止服务引擎
	m_MsgTriggerService.EndService();
	m_DataBaseService.EndService();
	//m_TimerService.EndService();
	m_TCPSocketService.EndService();
//	m_EventTrace.LOG(INFO)<<(TEXT("服务关闭"),Level_Normal);
	//m_AsynchronismService.EndService();

	return true;
}



//注册钩子
bool __cdecl CKernelService::SetMsgTriggerServiceSink(CObject * pIObject)
{
	//IMsgTriggerServiceSink *pMsgTriggerService = dynamic_cast<IMsgTriggerServiceSink*>(pIObject); 
	return m_MsgTriggerService.SetMsgTriggerServiceSink(pIObject);
}

//注册钩子
bool __cdecl CKernelService::SetDataBaseSink(CObject * pIObject)
{
	return m_DataBaseService.SetDataBaseSink(pIObject);
}


//设置网络
bool __cdecl CKernelService::InitKernelService(INT wListenPort, INT wMaxSocketItem)
{
	if (m_TCPSocketService.SetServicePort(wListenPort)==false) return false;
	if (m_TCPSocketService.SetMaxSocketItem(wMaxSocketItem)==false) return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////

