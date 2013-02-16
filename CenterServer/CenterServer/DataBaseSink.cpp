#include "StdAfx.h"
#include "DataBaseSink.h"


CDataBaseSink::CDataBaseSink(void)
{
	m_pDataBase = NULL;
}


CDataBaseSink::~CDataBaseSink(void)
{
	m_pDataBase = NULL;
	//m_pQueueServiceEvent = NULL;
}

	//数据库模块启动
bool __cdecl CDataBaseSink::StartService(CObject * pObject)
{
	m_pDataBase = static_cast<CMysqlDataBase*>(CreateDataBase());
	if(m_pDataBase ==NULL)
		return false;
	m_pDataBase->SettingConnectInfo(::g_szDataBaseAddr,::g_uDataBasePort,::g_szDataBaseUser,::g_szDataBasePass,::g_szDataBaseName);
	#ifdef _DEBUG
		LOG(INFO)<<"DataBaseAddr:"<<g_szDataBaseAddr<<"DataBasePort:"<<::g_uDataBasePort<<"DataBaseUser:"<<g_szDataBaseUser<<"DataBasePass:"<<g_szDataBasePass<<"DataBaseName:"<<g_szDataBaseName;
	#endif
	if(m_pDataBase->ConnectDataBase())
		return true;
	else
	{
		#ifdef _DEBUG
			LOG(INFO)<<"服务引擎接口查询失败";
		#endif
		TraceEventString("无法连接数据库",Level_Exception);
		return false;
	}
}
	//数据库模块关闭
bool __cdecl CDataBaseSink::EndService(CObject * pObject)
{
	if(m_pDataBase)
		m_pDataBase->CloseDataBase(true);
	SafeDelete(m_pDataBase);
	return true;
}
	//数据操作处理
bool __cdecl CDataBaseSink::OnDataBaseRequest(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	return true;
}
bool CDataBaseSink::InitDataBaseSink(IKernelService *pObject)
{

	try
		{//查询接口
			//IKernelService * pIKernelService=static_cast<CKernelService *>(pObject);
			//if (pIKernelService==NULL) 
			//{
			//	#ifdef _DEBUG
			//		LOG(INFO)<<"服务引擎接口查询失败";
			//	#endif
			//	throw 0;
			//}

			//获取逻辑引擎
			CMsgTriggerService * pIIMsgTriggerService = NULL;
			if (pObject->GetMsgTriggerService(&pIIMsgTriggerService) == false) 
			{
				#ifdef _DEBUG
					LOG(INFO)<<"调度引擎接口查询失败";
				#endif
				throw 0;
			}

			//设置通知组件
			CDataQueueService * pIQueueService = NULL;
			if(pIIMsgTriggerService->GetMsgTriggerQueueService(&pIQueueService) == false)
			{
				#ifdef _DEBUG
					LOG(INFO)<<"调度引擎队列获取失败";
				#endif
				throw 0;
			}
			if (m_QueueServiceEvent.SetQueueService(pIQueueService) == false) 
			{
				#ifdef _DEBUG
					LOG(INFO)<<"逻辑引擎通知接口设置失败";
				#endif
				throw 0;
			}
	}
	catch(...)
	{
		TraceEventString("InitDataBaseSink 异常",Level_Exception);
		return false;
	}
	return true;
}