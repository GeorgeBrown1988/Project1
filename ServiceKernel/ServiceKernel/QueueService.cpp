#include "StdAfx.h"
#include "QueueService.h"

//���캯��
CDataQueueServiceThread::CDataQueueServiceThread()
{
	m_hCompletionPort=NULL;
	memset(m_cbBuffer,0,sizeof(m_cbBuffer));
#ifdef _DEBUG
	LOG(INFO) <<"CDataQueueServiceThread";
#endif
}

//��������
CDataQueueServiceThread::~CDataQueueServiceThread()
{
#ifdef _DEBUG
	LOG(INFO) <<"~CDataQueueServiceThread";
#endif
	m_hCompletionPort=NULL;
	memset(m_cbBuffer,0,sizeof(m_cbBuffer));
}

//���ú���
bool CDataQueueServiceThread::InitThread(HANDLE hCompletionPort)
{
	//Ч�����
	////ASSERT(IsRuning()==false);
	////ASSERT(m_hCompletionPort==NULL);

	//���ñ���
	m_hCompletionPort=hCompletionPort;
	memset(m_cbBuffer,0,sizeof(m_cbBuffer));

	return true;
}

//ȡ������
bool CDataQueueServiceThread::UnInitThread()
{
	//Ч�����
	////ASSERT(IsRuning()==false);

	//���ñ���
	m_hCompletionPort=NULL;
	memset(m_cbBuffer,0,sizeof(m_cbBuffer));

	return true;
}

//���к���
bool CDataQueueServiceThread::RepetitionRun()
{
	//Ч�����
	////ASSERT(m_hCompletionPort!=NULL);

	//��������
	DWORD dwThancferred=0;
	OVERLAPPED * pOverLapped=NULL;
	CDataQueueService * pQueueService=NULL;

	//�ȴ���ɶ˿�
	if (GetQueuedCompletionStatus(m_hCompletionPort,&dwThancferred,(PULONG_PTR)&pQueueService,&pOverLapped,INFINITE))
	{
		//�ж��˳�
		if (pQueueService==NULL) return false;

		//��ȡ����
		tagDataHead DataHead;
		bool bSuccess=pQueueService->GetData(DataHead,m_cbBuffer,sizeof(m_cbBuffer));
		////ASSERT(bSuccess==true);

		//��������
		if (bSuccess==true) pQueueService->OnQueueServiceThread(DataHead,m_cbBuffer,DataHead.wDataSize);

		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CDataQueueService::CDataQueueService(void)
{
#ifdef _DEBUG
	LOG(INFO) <<"CDataQueueService";
#endif
	m_bService=false;
	m_hCompletionPort=NULL;
	m_pIQueueServiceSink=NULL;
}

//��������
CDataQueueService::~CDataQueueService(void)
{
	//ֹͣ����
	EndService();
#ifdef _DEBUG
	LOG(INFO) <<"~CDataQueueService";
#endif
	return;
}

//���ýӿ�
bool __cdecl CDataQueueService::SetDataQueueServiceSink(IDataQueueServiceSink * pobject)
{
	////ASSERT(pIUnknownEx!=NULL);
	m_pIQueueServiceSink=pobject;
	////ASSERT(m_pIQueueServiceSink!=NULL);
	return (m_pIQueueServiceSink!=NULL);
}

//������Ϣ
bool __cdecl CDataQueueService::GetDataQueueInfo(tagBurthenInfo & DataQueueInfo)
{
	CThreadLockHandle LockHandle(&m_ThreadLock);
	return m_DataStorage.GetBurthenInfo(DataQueueInfo);
}

//��������
bool __cdecl CDataQueueService::AddToQueue(WORD wIdentifier, void * const pBuffer, WORD wDataSize)
{
	CThreadLockHandle LockHandle(&m_ThreadLock);
	m_DataStorage.AddData(wIdentifier,pBuffer,wDataSize);
	PostQueuedCompletionStatus(m_hCompletionPort,wDataSize,(ULONG_PTR)this,NULL);
	return true;
}

//��ʼ����
bool __cdecl CDataQueueService::StartService()
{
	//Ч�����
	////ASSERT(m_bService==false);
	////ASSERT(m_hCompletionPort==NULL);
	////ASSERT(m_pIQueueServiceSink!=NULL);
	try{

	//������ɶ˿�
		m_hCompletionPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,NULL,1);
		if (m_hCompletionPort==NULL) throw TEXT("���ж�����ɶ˿ڴ���ʧ��");

		//�����߳�
		if (m_QueueServiceThread.InitThread(m_hCompletionPort)==false) throw TEXT("���ж����̳߳�ʼ��ʧ��");
		if (m_QueueServiceThread.StartThead()==false) throw TEXT("���ж����߳�����ʧ��");
	}
	catch(CString szError)
	{
		MessageBox(NULL,szError,szError,MB_OK);
		#ifdef _DEBUG
			LOG(INFO)<<szError;
		#endif
		return false;
	}
		//���ò���
	m_bService=true;

	return true;
}

//ֹͣ����
bool __cdecl CDataQueueService::EndService()
{
	//���ñ���
	m_bService=false;

	//ֹͣ�߳�
	if (m_hCompletionPort!=NULL) PostQueuedCompletionStatus(m_hCompletionPort,0,NULL,NULL);
	m_QueueServiceThread.EndThread();
	m_QueueServiceThread.UnInitThread();

	//�ر���ɶ˿�
	if (m_hCompletionPort!=NULL)
	{
		CloseHandle(m_hCompletionPort);
		m_hCompletionPort=NULL;
	}

	//��������
	m_DataStorage.RemoveData(false);

	return true;
}

//��ȡ����
bool CDataQueueService::GetData(tagDataHead & DataHead, void * pBuffer, WORD wBufferSize)
{
	CThreadLockHandle LockHandle(&m_ThreadLock);
	return m_DataStorage.GetData(DataHead,pBuffer,wBufferSize);
}

//������Ϣ
void CDataQueueService::OnQueueServiceThread(const tagDataHead & DataHead, void * pBuffer, WORD wDataSize)
{
	////ASSERT(m_pIQueueServiceSink!=NULL);
	try	
	{ 
		m_pIQueueServiceSink->OnDataQueueServiceSink(DataHead.wIdentifier,pBuffer,DataHead.wDataSize,DataHead.dwInsertTime); 
	}
	catch (...) {}
	return;
}