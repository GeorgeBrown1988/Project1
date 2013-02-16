#include "StdAfx.h"
#include "DataBaseSink.h"
#include "LoginServer.h"

CDataBaseSink::CDataBaseSink(void)
{
}


CDataBaseSink::~CDataBaseSink(void)
{
}
//函数定义

//模块启动
bool __cdecl CDataBaseSink::StartService(CObject * pObject)
{
	m_pUserDataBase = static_cast<CMysqlDataBase*>(CreateDataBase());
	if(m_pUserDataBase ==NULL)
		return false;
	m_pUserDataBase->SettingConnectInfo(::g_szDataBaseAddr,::g_uDataBasePort,::g_szDataBaseUser,::g_szDataBasePass,::g_szDataBaseName);
	#ifdef _DEBUG
		LOG(INFO)<<"DataBaseAddr:"<<g_szDataBaseAddr<<"DataBasePort:"<<::g_uDataBasePort<<"DataBaseUser:"<<g_szDataBaseUser<<"DataBasePass:"<<g_szDataBasePass<<"DataBaseName:"<<g_szDataBaseName;
	#endif
	if(m_pUserDataBase->ConnectDataBase())
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
//模块关闭
bool __cdecl CDataBaseSink::EndService(CObject * pObject)
{
	if(m_pUserDataBase)
		m_pUserDataBase->CloseDataBase(true);
	SafeDelete(m_pUserDataBase);
	return true;
}
//数据操作处理
bool __cdecl CDataBaseSink::OnDataBaseRequest(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	switch (DataBaseEvent.wRequestID)
	{
	case DBR_GP_LOGON_BY_ACCOUNTS:		//帐号登录
		{
			return OnRequestLogon(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_GP_LOGON_BY_USERID:		//I D 登录
		{
			return OnRequestLogon(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_GP_REGISTER_ACCOUNTS:		//注册帐号
		{
			return OnRequestLogon(DataBaseEvent,pDataBuffer,wDataSize);
		}
	}

	return false;
}


//配置函数
bool CDataBaseSink::InitDataBaseSink(IKernelService *pObject)
{

	try
		{

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


//登录请求处理
bool CDataBaseSink::OnRequestLogon(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	;
}
//I D 存储过程
LONG CDataBaseSink::SPLogonByUserID(DWORD dwGameID, LPCTSTR pszPassword, DWORD dwClientIP, LPCTSTR pszComputerID)
{
	;
}
//帐号存储过程
LONG CDataBaseSink::SPLogonByAccounts(LPCTSTR pszAccounts, LPCTSTR pszPassword, DWORD dwClientIP, LPCTSTR pszComputerID)
{
	;
}
//注册存储过程
LONG CDataBaseSink::SPRegisterAccounts(LPCTSTR pszAccounts, LPCTSTR pszPassword, LPCTSTR pszSpreader, WORD wFaceID, BYTE cbGender, DWORD dwClientIP, LPCTSTR pszComputerID)
{
	;
}

