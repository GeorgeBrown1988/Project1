#include "StdAfx.h"
#include "Thread.h"
#include <time.h>

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//��������
struct tagThreadParameter
{
	bool							bSuccess;							//�Ƿ����
	HANDLE							hEventFinish;						//�¼����
	CThread					* m_pServiceThread;					//�߳�ָ��
};

//////////////////////////////////////////////////////////////////////////

//���캯��
CThreadLockHandle::CThreadLockHandle(IThreadLock * pIThreadLock, bool bAutoLock)
{
	////ASSERT(pIThreadLock!=NULL);
	m_nLockCount=0;
	m_pIThreadLock=pIThreadLock;
	if (bAutoLock) Lock();
//	TRACE(L"CThreadLockHandle\n");
	return;
}

//��������
CThreadLockHandle::~CThreadLockHandle()
{
	while (m_nLockCount>0) UnLock();
	//TRACE(L"~CThreadLockHandle\n");
}

//��������
void CThreadLockHandle::Lock()
{
	//Ч��״̬
	////ASSERT(m_nLockCount>=0);
	////ASSERT(m_pIThreadLock!=NULL);

	//��������
	m_nLockCount++;
	m_pIThreadLock->Lock();
}

//�������� 
void CThreadLockHandle::UnLock()
{
	//Ч��״̬
	////ASSERT(m_nLockCount>0);
	////ASSERT(m_pIThreadLock!=NULL);

	//�������
	m_nLockCount--;
	m_pIThreadLock->UnLock();
}

//////////////////////////////////////////////////////////////////////////

CThread::CThread(void)
{
	m_bRun=false;
	m_uThreadID=0;
	m_hThreadHandle=NULL;
}

CThread::~CThread(void)
{
	EndThread(INFINITE);
}


//״̬�ж�
bool CThread::IsRuning()
{
	if (m_hThreadHandle!=NULL)
	{
		DWORD dwRetCode=WaitForSingleObject(m_hThreadHandle,0);
		if (dwRetCode==WAIT_TIMEOUT) return true;
	}
	return false;
}

//�����߳�
bool CThread::StartThead()
{
	//Ч��״̬
	if (IsRuning()) return false;

	//�������
	if (m_hThreadHandle!=NULL) 
	{
		CloseHandle(m_hThreadHandle);
		m_hThreadHandle=NULL;
		m_uThreadID=0;
	}

	//�������
	tagThreadParameter ThreadParameter;
	ThreadParameter.bSuccess=false;
	ThreadParameter.m_pServiceThread=this;
	ThreadParameter.hEventFinish=CreateEvent(NULL,FALSE,FALSE,NULL);
	if (ThreadParameter.hEventFinish==NULL) return false;

	//�����߳�
	m_bRun=true;
	m_hThreadHandle=(HANDLE)::_beginthreadex(NULL,0,ThreadFunction,&ThreadParameter,0,&m_uThreadID);

	//�ȴ��¼�
	WaitForSingleObject(ThreadParameter.hEventFinish,INFINITE);

	//�ر��¼�
	CloseHandle(ThreadParameter.hEventFinish);

	//�жϴ���
	if (ThreadParameter.bSuccess==false) 
	{
		EndThread();
		return false;
	}

	return true;
}

//ֹͣ�߳�
bool CThread::EndThread(DWORD dwWaitSeconds)
{
	//ֹͣ�߳�
	if (IsRuning()==true)
	{
		m_bRun=false;
		DWORD dwRetCode=WaitForSingleObject(m_hThreadHandle,dwWaitSeconds);
		if (dwRetCode==WAIT_TIMEOUT) return false;
	}

	//���ñ���
	if (m_hThreadHandle!=NULL)
	{
		CloseHandle(m_hThreadHandle);
		m_hThreadHandle=NULL;
		m_uThreadID=0;
	}

	return true;
}

//��ֹ�߳�
bool CThread::TerminateThread(DWORD dwExitCode)
{
	//ֹͣ�߳�
	if (IsRuning()==true)
	{
		::TerminateThread(m_hThreadHandle,dwExitCode);
	}

	//���ñ���
	if (m_hThreadHandle!=NULL)
	{
		CloseHandle(m_hThreadHandle);
		m_hThreadHandle=NULL;
		m_uThreadID=0;
	}

	return true;
}

//Ͷ����Ϣ
bool CThread::PostThreadMessage(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	//״̬Ч��
	if (m_uThreadID==0) return false;

	//Ͷ����Ϣ
	BOOL bSuccess=::PostThreadMessage(m_uThreadID,uMessage,wParam,lParam);
	return bSuccess?true:false;
}

//�̺߳���
unsigned __stdcall CThread::ThreadFunction(LPVOID pThreadData)
{
	//��ȡ����
	////ASSERT(pThreadData!=NULL);
	tagThreadParameter * pThreadParameter=(tagThreadParameter *)pThreadData;
	CThread * pThread=pThreadParameter->m_pServiceThread;

	//����¼�
	srand((DWORD)time(NULL));

	//�����¼�
	try 
	{
		pThreadParameter->bSuccess=pThread->OnThreadStratEvent(); 
	} 
	catch (...) 
	{ 
		pThreadParameter->bSuccess=false; 
	}

	//�����¼�
	bool bSuccess=pThreadParameter->bSuccess;
	////ASSERT(pThreadParameter->hEventFinish!=NULL);
	SetEvent(pThreadParameter->hEventFinish);

	//�����߳�
	if (bSuccess==true)
	{
		while (pThread->m_bRun)
		{
			try
			{
				if (pThread->RepetitionRun()==false) break;
			}
			catch (...) {}
		}
	}

	//ֹͣ�¼�
	try 
	{ 
		pThread->OnThreadStopEvent(); 
	} 
	catch (...) 
	{
	}

	//��ֹ�߳�
	_endthreadex(0);

	return 0;
}
//////////////////////////////////////////////////////////////////////////