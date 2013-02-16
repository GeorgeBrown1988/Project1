#include "StdAfx.h"
#include "QueueService.h"

//构造函数
CDataQueueServiceThread::CDataQueueServiceThread()
{
	m_hCompletionPort=NULL;
	memset(m_cbBuffer,0,sizeof(m_cbBuffer));
#ifdef _DEBUG
	LOG(INFO) <<"CDataQueueServiceThread";
#endif
}

//析构函数
CDataQueueServiceThread::~CDataQueueServiceThread()
{
#ifdef _DEBUG
	LOG(INFO) <<"~CDataQueueServiceThread";
#endif
	m_hCompletionPort=NULL;
	memset(m_cbBuffer,0,sizeof(m_cbBuffer));
}

//配置函数
bool CDataQueueServiceThread::InitThread(HANDLE hCompletionPort)
{
	//效验参数
	////ASSERT(IsRuning()==false);
	////ASSERT(m_hCompletionPort==NULL);

	//设置变量
	m_hCompletionPort=hCompletionPort;
	memset(m_cbBuffer,0,sizeof(m_cbBuffer));

	return true;
}

//取消配置
bool CDataQueueServiceThread::UnInitThread()
{
	//效验参数
	////ASSERT(IsRuning()==false);

	//设置变量
	m_hCompletionPort=NULL;
	memset(m_cbBuffer,0,sizeof(m_cbBuffer));

	return true;
}

//运行函数
bool CDataQueueServiceThread::RepetitionRun()
{
	//效验参数
	////ASSERT(m_hCompletionPort!=NULL);

	//变量定义
	DWORD dwThancferred=0;
	OVERLAPPED * pOverLapped=NULL;
	CDataQueueService * pQueueService=NULL;

	//等待完成端口
	if (GetQueuedCompletionStatus(m_hCompletionPort,&dwThancferred,(PULONG_PTR)&pQueueService,&pOverLapped,INFINITE))
	{
		//判断退出
		if (pQueueService==NULL) return false;

		//获取数据
		tagDataHead DataHead;
		bool bSuccess=pQueueService->GetData(DataHead,m_cbBuffer,sizeof(m_cbBuffer));
		////ASSERT(bSuccess==true);

		//处理数据
		if (bSuccess==true) pQueueService->OnQueueServiceThread(DataHead,m_cbBuffer,DataHead.wDataSize);

		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////

//构造函数
CDataQueueService::CDataQueueService(void)
{
#ifdef _DEBUG
	LOG(INFO) <<"CDataQueueService";
#endif
	m_bService=false;
	m_hCompletionPort=NULL;
	m_pIQueueServiceSink=NULL;
}

//析构函数
CDataQueueService::~CDataQueueService(void)
{
	//停止服务
	EndService();
#ifdef _DEBUG
	LOG(INFO) <<"~CDataQueueService";
#endif
	return;
}

//设置接口
bool __cdecl CDataQueueService::SetDataQueueServiceSink(IDataQueueServiceSink * pobject)
{
	////ASSERT(pIUnknownEx!=NULL);
	m_pIQueueServiceSink=pobject;
	////ASSERT(m_pIQueueServiceSink!=NULL);
	return (m_pIQueueServiceSink!=NULL);
}

//负荷信息
bool __cdecl CDataQueueService::GetDataQueueInfo(tagBurthenInfo & DataQueueInfo)
{
	CThreadLockHandle LockHandle(&m_ThreadLock);
	return m_DataStorage.GetBurthenInfo(DataQueueInfo);
}

//加入数据
bool __cdecl CDataQueueService::AddToQueue(WORD wIdentifier, void * const pBuffer, WORD wDataSize)
{
	CThreadLockHandle LockHandle(&m_ThreadLock);
	m_DataStorage.AddData(wIdentifier,pBuffer,wDataSize);
	PostQueuedCompletionStatus(m_hCompletionPort,wDataSize,(ULONG_PTR)this,NULL);
	return true;
}

//开始服务
bool __cdecl CDataQueueService::StartService()
{
	//效验参数
	////ASSERT(m_bService==false);
	////ASSERT(m_hCompletionPort==NULL);
	////ASSERT(m_pIQueueServiceSink!=NULL);
	try{

	//建立完成端口
		m_hCompletionPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,NULL,1);
		if (m_hCompletionPort==NULL) throw TEXT("队列对象完成端口创建失败");

		//启动线程
		if (m_QueueServiceThread.InitThread(m_hCompletionPort)==false) throw TEXT("队列对象线程初始化失败");
		if (m_QueueServiceThread.StartThead()==false) throw TEXT("队列对象线程启动失败");
	}
	catch(CString szError)
	{
		MessageBox(NULL,szError,szError,MB_OK);
		#ifdef _DEBUG
			LOG(INFO)<<szError;
		#endif
		return false;
	}
		//设置参数
	m_bService=true;

	return true;
}

//停止服务
bool __cdecl CDataQueueService::EndService()
{
	//设置变量
	m_bService=false;

	//停止线程
	if (m_hCompletionPort!=NULL) PostQueuedCompletionStatus(m_hCompletionPort,0,NULL,NULL);
	m_QueueServiceThread.EndThread();
	m_QueueServiceThread.UnInitThread();

	//关闭完成端口
	if (m_hCompletionPort!=NULL)
	{
		CloseHandle(m_hCompletionPort);
		m_hCompletionPort=NULL;
	}

	//设置数据
	m_DataStorage.RemoveData(false);

	return true;
}

//提取数据
bool CDataQueueService::GetData(tagDataHead & DataHead, void * pBuffer, WORD wBufferSize)
{
	CThreadLockHandle LockHandle(&m_ThreadLock);
	return m_DataStorage.GetData(DataHead,pBuffer,wBufferSize);
}

//数据消息
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