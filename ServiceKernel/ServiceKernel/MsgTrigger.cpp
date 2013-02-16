#include "StdAfx.h"
#include "MsgTrigger.h"



//构造函数
CMsgTriggerService::CMsgTriggerService(void)
{
#ifdef _DEBUG
	LOG(INFO) <<"CMsgTriggerService";
#endif
	m_bService=false;
	m_pITCPSocketService=NULL;
	m_pIMsgTriggerServiceSink=NULL;
	return;
}

//析构函数
CMsgTriggerService::~CMsgTriggerService(void)
{
#ifdef _DEBUG
	LOG(INFO) <<"~CMsgTriggerService";
#endif
}


//启动服务
bool __cdecl CMsgTriggerService::StartService()
{
	//判断状态
	if (m_bService==true) 
	{
		#ifdef _DEBUG
			LOG(INFO) <<"调度引擎重复启动，启动操作忽略";
		#endif
		// LOG(INFO)<<(TEXT("调度引擎重复启动，启动操作忽略"),Level_Warning);
		return true;
	}

	//外挂接口
	if (m_pIMsgTriggerServiceSink==NULL)
	{
		#ifdef _DEBUG
			LOG(INFO) <<"调度引擎外挂服务不存在";
		#endif
		// LOG(INFO)<<(TEXT("调度引擎外挂服务不存在"),Level_Exception);
		return false;
	}

	//设置队列
	if (m_RequestQueueService.SetDataQueueServiceSink(this)==false)
	{
		#ifdef _DEBUG
			LOG(INFO) <<"调度引擎与队列服务绑定失败";
		#endif
		// LOG(INFO)<<(TEXT("调度引擎与队列服务绑定失败"),Level_Exception);
		return false;
	}

	//启动外挂
	if (m_pIMsgTriggerServiceSink->StartService(this)==false)
	{
		#ifdef _DEBUG
			LOG(INFO) <<"调度引擎外挂服务启动失败";
		#endif
		// LOG(INFO)<<(TEXT("调度引擎外挂服务启动失败"),Level_Exception);
		return false;
	}

	//启动队列
	if (m_RequestQueueService.StartService()==false)
	{
		#ifdef _DEBUG
			LOG(INFO) <<"调度引擎队列服务启动失败";
		#endif
		// LOG(INFO)<<(TEXT("调度引擎队列服务启动失败"),Level_Exception);
		return false;
	}

	//设置变量
	m_bService=true;

	return true;
}
//停止服务
bool __cdecl CMsgTriggerService::EndService()
{
	//设置变量
	m_bService=false;

	//停止请求队列
	m_RequestQueueService.EndService();

	//停止外挂
	if (m_pIMsgTriggerServiceSink!=NULL)
	{
		m_pIMsgTriggerServiceSink->EndService(this);//(IMsgTriggerServiceSink*)
	}

	return true;
}

//设置网络
bool __cdecl CMsgTriggerService::SetSocketService(CObject * pIObject)
{
	//ASSERT(pIObject!=NULL);
	m_pITCPSocketService=dynamic_cast<ITCPSocketService*>(pIObject);
	//ASSERT(m_pITCPSocketService!=NULL);
	return (m_pITCPSocketService!=NULL);
}

//注册钩子
bool __cdecl CMsgTriggerService::SetMsgTriggerServiceSink(CObject * pIObject)
{
	//效验参数
	//ASSERT(pIObject!=NULL);
	//ASSERT(m_pIMsgTriggerServiceSink==NULL);IAttemperEngineSink
	if (pIObject==NULL) return false;
	if (m_pIMsgTriggerServiceSink!=NULL) return false;

	//查询接口
	m_pIMsgTriggerServiceSink=dynamic_cast<IMsgTriggerServiceSink*>(pIObject);
	if (m_pIMsgTriggerServiceSink==NULL)
	{
		#ifdef _DEBUG
			LOG(INFO) <<"调度引擎外挂服务接口获取失败，挂接操作失败";
		#endif
		// LOG(INFO)<<(TEXT("调度引擎外挂服务接口获取失败，挂接操作失败"),Level_Exception);
		return false;
	}

	return true;
}

//获取接口
bool __cdecl CMsgTriggerService::GetMsgTriggerQueueService(CDataQueueService ** pIObject)
{
	*pIObject = dynamic_cast<CDataQueueService*>(&m_RequestQueueService);
	if(pIObject)
		return true;
	else
		return false;
}

//队列接口
void __cdecl CMsgTriggerService::OnDataQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize, DWORD dwInsertTime)
{
	//内核事件
	//ASSERT(m_pIMsgTriggerServiceSink!=NULL);
	switch (wIdentifier)
	{
	//case EVENT_TIMER:			//定时器事件
	//	{
	//		//效验参数
	//		//ASSERT(wDataSize==sizeof(NTY_TimerEvent));
	//		if (wDataSize!=sizeof(NTY_TimerEvent)) return;

	//		//处理消息
	//		NTY_TimerEvent * pTimerEvent=(NTY_TimerEvent *)pBuffer;
	//		m_pIMsgRoutServiceSink->OnEventTimer(pTimerEvent->wTimerID,pTimerEvent->wBindParam);

	//		return;
	//	}
	case EVENT_DATABASE:		//数据库事件
		{
			//效验参数
			//ASSERT(wDataSize>=sizeof(NTY_DataBaseEvent));
			if (wDataSize<sizeof(NTY_DataBaseEvent)) return;

			//处理消息
			NTY_DataBaseEvent * pDataBaseEvent=(NTY_DataBaseEvent *)pBuffer;
			m_pIMsgTriggerServiceSink->OnEventDataBase(pDataBaseEvent+1,wDataSize-sizeof(NTY_DataBaseEvent),pDataBaseEvent);

			return;
		}
	case EVENT_SOCKET_ACCEPT:	//网络应答事件
		{
			//效验大小
			//ASSERT(wDataSize==sizeof(NTY_SocketAcceptEvent));
			if (wDataSize!=sizeof(NTY_SocketAcceptEvent)) return;

			//处理消息
			NTY_SocketAcceptEvent * pSocketAcceptEvent=(NTY_SocketAcceptEvent *)pBuffer;
			m_pIMsgTriggerServiceSink->OnEventSocketAccept(pSocketAcceptEvent);

			return;
		}
	case EVENT_SOCKET_READ:		//网络读取事件
		{
			//效验大小
			NTY_SocketReadEvent * pSocketReadEvent=(NTY_SocketReadEvent *)pBuffer;
			//ASSERT(wDataSize>=sizeof(NTY_SocketReadEvent));
			//ASSERT(wDataSize==(sizeof(NTY_SocketReadEvent)+pSocketReadEvent->wDataSize));
			if (wDataSize<sizeof(NTY_SocketReadEvent)) return;
			if (wDataSize!=(sizeof(NTY_SocketReadEvent)+pSocketReadEvent->wDataSize)) return;

			//处理消息
			bool bSuccess=false;
			try 
			{ 
				bSuccess=m_pIMsgTriggerServiceSink->OnEventSocketRead(pSocketReadEvent->Command,pSocketReadEvent+1,pSocketReadEvent->wDataSize,pSocketReadEvent);
			}
			catch (...)	{ }
			if (bSuccess==false) m_pITCPSocketService->CloseSocket(pSocketReadEvent->wIndex,pSocketReadEvent->wRoundID);

			return;
		}
	case EVENT_SOCKET_CLOSE:	//网络关闭事件
		{
			//效验大小
			//ASSERT(wDataSize==sizeof(NTY_SocketCloseEvent));
			if (wDataSize!=sizeof(NTY_SocketCloseEvent)) return;

			//处理消息
			NTY_SocketCloseEvent * pSocketCloseEvent=(NTY_SocketCloseEvent *)pBuffer;
			m_pIMsgTriggerServiceSink->OnEventSocketClose(pSocketCloseEvent);

			return;
		}
	}

	//其他事件
	m_pIMsgTriggerServiceSink->OnMsgTrigger(wIdentifier,pBuffer,wDataSize,dwInsertTime); 

	return;
}
