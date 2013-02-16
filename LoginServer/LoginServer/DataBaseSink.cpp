#include "StdAfx.h"
#include "DataBaseSink.h"
#include "LoginServer.h"

CDataBaseSink::CDataBaseSink(void)
{
}


CDataBaseSink::~CDataBaseSink(void)
{
}
//��������

//ģ������
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
			LOG(INFO)<<"��������ӿڲ�ѯʧ��";
		#endif
		TraceEventString("�޷��������ݿ�",Level_Exception);
		return false;
	}
}
//ģ��ر�
bool __cdecl CDataBaseSink::EndService(CObject * pObject)
{
	if(m_pUserDataBase)
		m_pUserDataBase->CloseDataBase(true);
	SafeDelete(m_pUserDataBase);
	return true;
}
//���ݲ�������
bool __cdecl CDataBaseSink::OnDataBaseRequest(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	switch (DataBaseEvent.wRequestID)
	{
	case DBR_GP_LOGON_BY_ACCOUNTS:		//�ʺŵ�¼
		{
			return OnRequestLogon(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_GP_LOGON_BY_USERID:		//I D ��¼
		{
			return OnRequestLogon(DataBaseEvent,pDataBuffer,wDataSize);
		}
	case DBR_GP_REGISTER_ACCOUNTS:		//ע���ʺ�
		{
			return OnRequestLogon(DataBaseEvent,pDataBuffer,wDataSize);
		}
	}

	return false;
}


//���ú���
bool CDataBaseSink::InitDataBaseSink(IKernelService *pObject)
{

	try
		{

			//��ȡ�߼�����
			CMsgTriggerService * pIIMsgTriggerService = NULL;
			if (pObject->GetMsgTriggerService(&pIIMsgTriggerService) == false) 
			{
				#ifdef _DEBUG
					LOG(INFO)<<"��������ӿڲ�ѯʧ��";
				#endif
				throw 0;
			}

			//����֪ͨ���
			CDataQueueService * pIQueueService = NULL;
			if(pIIMsgTriggerService->GetMsgTriggerQueueService(&pIQueueService) == false)
			{
				#ifdef _DEBUG
					LOG(INFO)<<"����������л�ȡʧ��";
				#endif
				throw 0;
			}
			if (m_QueueServiceEvent.SetQueueService(pIQueueService) == false) 
			{
				#ifdef _DEBUG
					LOG(INFO)<<"�߼�����֪ͨ�ӿ�����ʧ��";
				#endif
				throw 0;
			}
	}
	catch(...)
	{
		TraceEventString("InitDataBaseSink �쳣",Level_Exception);
		return false;
	}
	return true;
}


//��¼������
bool CDataBaseSink::OnRequestLogon(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	;
}
//I D �洢����
LONG CDataBaseSink::SPLogonByUserID(DWORD dwGameID, LPCTSTR pszPassword, DWORD dwClientIP, LPCTSTR pszComputerID)
{
	;
}
//�ʺŴ洢����
LONG CDataBaseSink::SPLogonByAccounts(LPCTSTR pszAccounts, LPCTSTR pszPassword, DWORD dwClientIP, LPCTSTR pszComputerID)
{
	;
}
//ע��洢����
LONG CDataBaseSink::SPRegisterAccounts(LPCTSTR pszAccounts, LPCTSTR pszPassword, LPCTSTR pszSpreader, WORD wFaceID, BYTE cbGender, DWORD dwClientIP, LPCTSTR pszComputerID)
{
	;
}

