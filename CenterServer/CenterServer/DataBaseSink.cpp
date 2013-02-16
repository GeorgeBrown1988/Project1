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

	//���ݿ�ģ������
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
			LOG(INFO)<<"��������ӿڲ�ѯʧ��";
		#endif
		TraceEventString("�޷��������ݿ�",Level_Exception);
		return false;
	}
}
	//���ݿ�ģ��ر�
bool __cdecl CDataBaseSink::EndService(CObject * pObject)
{
	if(m_pDataBase)
		m_pDataBase->CloseDataBase(true);
	SafeDelete(m_pDataBase);
	return true;
}
	//���ݲ�������
bool __cdecl CDataBaseSink::OnDataBaseRequest(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)
{
	return true;
}
bool CDataBaseSink::InitDataBaseSink(IKernelService *pObject)
{

	try
		{//��ѯ�ӿ�
			//IKernelService * pIKernelService=static_cast<CKernelService *>(pObject);
			//if (pIKernelService==NULL) 
			//{
			//	#ifdef _DEBUG
			//		LOG(INFO)<<"��������ӿڲ�ѯʧ��";
			//	#endif
			//	throw 0;
			//}

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