#include "StdAfx.h"
#include "DataBase.h"

CMysqlDataBase::CMysqlDataBase()
{
	//TRACE(L"CDataBase\n");
	Mysql_Handle=NULL;
	Mysql_Result=NULL;
	Mysql_FieldName=NULL;
	Mysql_ResultVec.clear();
	m_bStatus = false;
	m_bCommit = true;//AutoCommit
	m_eType = DBMYSQL;
#ifdef _DEBUG
	LOG(INFO) <<"CMysqlDataBase";
#endif
}
void CMysqlDataBase::SetConf(CString &szLang,bool AutoCommit)
{
	SetLanguage(szLang);
	SetAutoCommit(AutoCommit);
}
CMysqlDataBase::~CMysqlDataBase()
{
#ifdef _DEBUG
	LOG(INFO) <<"~CMysqlDataBase";
#endif
	if(Mysql_Handle)
		this->CloseDataBase(true);
	Mysql_Handle=NULL;
	Mysql_Result=NULL;
	Mysql_FieldName=NULL;
	Mysql_ResultVec.clear();
	m_bStatus = false;
	m_bCommit = true;

}
bool  CMysqlDataBase::ConnectDataBase()
{
	if(m_bStatus)
		return true;
	Mysql_Handle = mysql_init(0);
	if(!mysql_real_connect(Mysql_Handle,m_ConnectInfo.m_szDataBaseIP,m_ConnectInfo.m_szUserName,m_ConnectInfo.m_szPassWord,m_ConnectInfo.m_szDataBase,0,NULL,0))
	{
		try
		{
			m_bStatus = false;
			throw 0;
		
		}
		catch(...)
		{
			#ifdef _DEBUG
				LOG(INFO)<<"连接数据库失败";
			#endif
			CDBError e(this);
			e.ShowErrorMsg();
		}
		return false;
	}
	//m_bStatus = true;
	return true;
}
void  CMysqlDataBase::CloseDataBase(bool flags)
{
	if(flags)
	{
		mysql_close(Mysql_Handle);
		Mysql_Handle = NULL;
		ClearConnectInfo();
		m_bStatus = false;
	}
	else
		m_bStatus = false;
}
void  CMysqlDataBase::ClearConnectInfo()
{
	m_ConnectInfo.m_szDataBase.Empty();
	m_ConnectInfo.m_szDataBaseIP.Empty();
	m_ConnectInfo.m_szPassWord.Empty();
	m_ConnectInfo.m_szUserName.Empty();
	m_ConnectInfo.m_uPort=0;
	m_col = -1;
	//m_bStatus = false;
}

void   CMysqlDataBase::SettingConnectInfo(CString  szIP,UINT uPort,CString szUser,CString  szPass,CString  szDB)
{
	m_ConnectInfo.m_szDataBase.Format("%s",szDB);
	m_ConnectInfo.m_szDataBaseIP.Format("%s",szIP);
	m_ConnectInfo.m_szPassWord.Format("%s",szPass);
	m_ConnectInfo.m_szUserName.Format("%s",szUser);
	m_ConnectInfo.m_uPort=uPort;
	//MessageBox(NULL,m_ConnectInfo.m_szDataBase,m_ConnectInfo.m_szDataBase,MB_OK);
	//MessageBox(NULL,m_ConnectInfo.m_szPassWord,m_ConnectInfo.m_szPassWord,MB_OK);
}

bool  CMysqlDataBase::IsConnectError()
{
	if(Mysql_Handle)
		return false;
	return true;
}
CString  CMysqlDataBase::GetErrorInfo()
{
//	if(!m_bStatus)
//		return L"SQL Empty!";
	return CString(mysql_error(Mysql_Handle));
}
int  CMysqlDataBase::SQLProc(CString & pSQL)
{
	m_bStatus = true;
	//if(!m_bStatus)
	//	return -1;

	return mysql_real_query(Mysql_Handle,pSQL,pSQL.GetLength());
}
void CMysqlDataBase::GetResult(int &row,int &col)
{
	if(!m_bStatus)
		return ;
	Mysql_Result = mysql_store_result(Mysql_Handle);
    row = mysql_num_rows(Mysql_Result);
    col = mysql_field_count(Mysql_Handle);
	m_col = col;
    while (Mysql_RowNum = mysql_fetch_row(Mysql_Result))//每次输出每一行
    {  
            unsigned int field_count;
            field_count =0;
            while(field_count < mysql_field_count(Mysql_Handle))//列
            {
				//CString temp(Mysql_RowNum[field_count]);
				Mysql_ResultVec.push_back(Mysql_RowNum[field_count]);
					field_count++;
            }
	}
	mysql_free_result(Mysql_Result);
}
bool __cdecl CMysqlDataBase::GetFieldValue(int row,int col,CString &str)
{
	if(!m_bStatus)
		return str = L"0";
		return str = (Mysql_ResultVec[row*m_col+col]);
}
//bool __cdecl CDataBase::GetFieldValue(int row,int col,BYTE *pbuf,int size)
//{
//
//	return true;
//}
bool __cdecl CMysqlDataBase::GetFieldValue(int row,int col,int &val)
{
	if(!m_bStatus)
		return val = 0;
		return val = atoi(Mysql_ResultVec[row*m_col+col]);
}
bool __cdecl CMysqlDataBase::GetFieldValue(int row,int col,float &val)
{
	if(!m_bStatus)
		return val = 0.0;
		return val = atof(Mysql_ResultVec[row*m_col+col]);
}
void CMysqlDataBase::FreeResult()
{
    Mysql_ResultVec.clear();
	m_bStatus = false;
}
void  CMysqlDataBase::SetLanguage(CString & language)
{
	m_szLanguage = language;
	mysql_set_character_set(Mysql_Handle,m_szLanguage);
}
bool CMysqlDataBase::SetAutoCommit(bool flags)
{
	m_bCommit = flags;
	mysql_autocommit(Mysql_Handle,m_bCommit);
	return m_bCommit;
}
bool CMysqlDataBase::OnCommit()
{
		return mysql_commit(Mysql_Handle);
}
CDBError::CDBError(IDataBase *pdb)
{
	////ASSERT(pdb);
#ifdef _DEBUG
	LOG(INFO) <<"CDBError";
#endif
	m_pDB = pdb;
}
CDBError::~CDBError()
{
#ifdef _DEBUG
	LOG(INFO) <<"~CDBError";
#endif
	m_pDB = NULL;
}
void CDBError::ShowErrorMsg()
{

	CString szMsg;
	szMsg.Format("数据库连接错误消息：%s",m_pDB->GetErrorInfo());
#ifdef _DEBUG
	LOG(INFO) <<szMsg.GetBuffer(0);
#endif
}
//
//CDBPoolMng::CDBPoolMng()
//{
//	;
//}
//CDBPoolMng::~CDBPoolMng()
//{
//	int size = m_DBConnectVec.size();
//	if(size > 0)
//	{
//		for(int count = 0;count <size;count++)
//			delete m_DBConnectVec[count];
//	m_DBConnectVec.clear();
//	}
//	
//}
//void __cdecl CDBPoolMng::InitDataBasePool(INT poolsize)
//{
//	CString tempDB=L"test";
//	//CString tempIP=L"admin";
//	CString tempHost=L"localhost";
//	CString tempPass=L"123456";
//	CString tempUser=L"admin";
//	UINT tempPort=3306;
//	for(int count = 0;count < poolsize; ++count)
//	{
//		m_DBConnectVec.push_back(new CDataBase);//.SettingParamter(tempHost,tempPort,tempUser,tempPass,tempDB);
//		m_DBConnectVec[count]->SettingConnectInfo(tempHost,tempPort,tempUser,tempPass,tempDB);
//	}
//}
//bool __cdecl CDBPoolMng::StartDataBasePool()
//{
//	int size = m_DBConnectVec.size();
//	for(int count = 0;count < size; ++count)
//	{
//		m_DBConnectVec[count]->ConnectDataBase();
//	}
//	return true;
//}
//bool __cdecl CDBPoolMng::EndDataBasePool()
//{
//	int size = m_DBConnectVec.size();
//	for(int count = 0;count < size; ++count)
//	{
//		m_DBConnectVec[count]->CloseDataBase(true);
//	}
//	return true;
//}
//void __cdecl CDBPoolMng::CloseInstance(INT pos)
//{
//	if(pos < m_DBConnectVec.size())
//		m_DBConnectVec[pos]->CloseDataBase(false);
//}
//CDataBase * __cdecl CDBPoolMng::GetFreeInstance()
//{
//	int size = m_DBConnectVec.size();
//	for(int pos=0;pos<size;pos++)
//	{
//		if(m_DBConnectVec[pos]->GetStatus() == false && m_DBConnectVec[pos]->IsConnectError() == false)
//			return m_DBConnectVec[pos];
//	}
//	return NULL;
//}
//int __cdecl CDBPoolMng::GetFreeInstanceIndex()
//{
//	int size = m_DBConnectVec.size();
//	for(int pos=0;pos<size;pos++)
//	{
//		if(m_DBConnectVec[pos]->GetStatus() == false && m_DBConnectVec[pos]->IsConnectError() == false)
//			return pos;
//	}
//	return -1;
//}
//void __cdecl CDBPoolMng::GetDataBasePoolInfo(int & size, int &freecount)
//{
//	size = m_DBConnectVec.size();
//	for(int pos=0;pos<size;pos++)
//	{
//		if(m_DBConnectVec[pos]->GetStatus() == false)
//			++freecount;
//	}
//}
//void __cdecl CDBPoolMng::GetDataBasePoolStatus(vector<bool> &vec)
//{
//	int size = m_DBConnectVec.size();
//	for(int pos=0;pos<size;pos++)
//	{
//		if(m_DBConnectVec[pos]->GetStatus() == false)
//			vec.push_back(false);
//		else
//			vec.push_back(true);
//	}
//}
//void __cdecl CDBPoolMng::UnInitDataBasePool()
//{
//	int size = m_DBConnectVec.size();
//	for(int pos = 0;pos<size; ++pos)
//	{
//		delete m_DBConnectVec.at(pos);
//	}
//	m_DBConnectVec.clear();
//}
//CDataBaseWorkThread::CDataBaseWorkThread()
//{
//	memset(m_cbBuffer,0,MAX_QUEUE_PACKET);		//接收缓冲	
//	m_hCompletionPort = NULL;					//完成端口
//}
//CDataBaseWorkThread::~CDataBaseWorkThread()
//{
//	m_hCompletionPort=NULL;
//	memset(m_cbBuffer,0,sizeof(m_cbBuffer));
//}
//bool CDataBaseWorkThread::InitThread()
//{
//	return true;
//}
//bool CDataBaseWorkThread::InitThread(HANDLE hCompletionPort)
//{
////	//ASSERT(IsRuning()==false);
////	//ASSERT(m_hCompletionPort==NULL);
//
//	//设置变量
//	m_hCompletionPort=hCompletionPort;
//	memset(m_cbBuffer,0,sizeof(m_cbBuffer));
//	return true;
//}
//bool CDataBaseWorkThread::UnInitThread()
//{
//	CloseHandle(m_hCompletionPort);
//	m_hCompletionPort = NULL;
//	return true;
//}
//bool CDataBaseWorkThread::GetStatus()
//{
//	if(IsRuning() == true)
//		return true;
//	return false;
//}
//bool CDataBaseWorkThread::RepetitionRun()
//{
//	if(m_hCompletionPort == NULL)
//		return false;
//
//	DWORD dwThancferred=0;
//	OVERLAPPED * pOverLapped=NULL;
//	if (GetQueuedCompletionStatus(m_hCompletionPort,&dwThancferred,(PULONG_PTR)NULL,&pOverLapped,INFINITE))
//	{
//		;
//	}
//
//	return false;
//}
//CDataBaseService::CDataBaseService(void)
//{
//	m_pDBPool = new CDBPoolMng;
//	m_bService = false;
//}
//CDataBaseService::~CDataBaseService(void)
//{
//	delete m_pDBPool;
//	m_bService = false;
//}
//bool __cdecl CDataBaseService::StartService()
//{
//	m_pDBPool->InitDataBasePool(3);
//	m_pDBPool->StartDataBasePool();
//	m_bService = true;
//	return true;
//}
//bool __cdecl CDataBaseService::EndService()
//{
//	m_pDBPool->EndDataBasePool();
//	m_pDBPool->UnInitDataBasePool();
//	m_bService = false;
//	return true;
//}
//	//请求实例ID
//int __cdecl CDataBaseService::RequestDataBaseRes()
//{
//	if(m_pDBPool == NULL)
//		return -1;
//	return m_pDBPool->GetFreeInstanceIndex();
//}
//	
//void __cdecl CDataBaseService::ConfDataBaseRes(int res,int langindex,bool autocommit)
//{
//	switch(langindex)
//	{
//		case 0:
//			m_pDBPool->GetInstance(res)->SetConf(CString(L"gbk"),autocommit);
//			return ;
//		case 1:
//			m_pDBPool->GetInstance(res)->SetConf(CString(L"utf-8"),autocommit);
//			return ;
//		default:
//			m_pDBPool->GetInstance(res)->SetConf(CString(L"gbk"),autocommit);
//			return ;
//	}
//
//}
//	
//void __cdecl CDataBaseService::SQLQuery(int res,CString &sql)
//{
//	 m_pDBPool->GetInstance(res)->SQLProc(sql);
//	
//}
//	
//void __cdecl CDataBaseService::GetResult(int res,int &row,int &col)
//{
//	m_pDBPool->GetInstance(res)->GetResult(row,col);
//	
//}	
//bool __cdecl CDataBaseService::GetFieldValue(int res,int row,int col,int &val)
//{
//	return m_pDBPool->GetInstance(res)->GetFieldValue(row,col,val);
//}
//bool __cdecl CDataBaseService::GetFieldValue(int res,int row,int col,float &val)
//{
//	return m_pDBPool->GetInstance(res)->GetFieldValue(row,col,val);
//}
//bool __cdecl CDataBaseService::GetFieldValue(int res,int row,int col,CString &val)
//{
//	return m_pDBPool->GetInstance(res)->GetFieldValue(row,col,val);
//}
////bool __cdecl CDataBaseService::GetFieldValue(int res,int row,int col,BYTE* val,int size)
////{
////	//MARK
////	return m_pDBPool->GetInstance(res)->GetFieldValue(row,col,val,size);
////}
//	
//bool __cdecl CDataBaseService::GetResStatus(int res)
//{
//	return m_pDBPool->GetInstance(res)->GetStatus();
//	//return pDB->GetStatus();
//}
//
//void  __cdecl CDataBaseService::FreeResult(int res)
//{
//	 m_pDBPool->GetInstance(res)->FreeResult();
//	//pDB->FreeResult();
//}
//void __cdecl CDataBaseService::ReleaseDataBaseRes(int res)
//{
//	 m_pDBPool->GetInstance(res)->CloseDataBase(false);
//}
////队列接口
//void __cdecl CDataBaseService::OnDataQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize, DWORD dwInsertTime)
//{
//	//判断状态
//	if (m_bService==false) return;
//
//	//请求处理
//	switch (wIdentifier)
//	{
//	case EVENT_DATABASE:
//		{
//			//效验参数
//			//ASSERT(pBuffer!=NULL);
//			//ASSERT(wDataSize>=sizeof(NTY_DataBaseEvent));
//			if (wDataSize<sizeof(NTY_DataBaseEvent)) return;
//
//			//变量定义
//			NTY_DataBaseEvent * pDataBaseEvent=(NTY_DataBaseEvent *)pBuffer;
//			WORD wHandleBuffer=wDataSize-sizeof(NTY_DataBaseEvent);
//
//			//处理数据
//			//ASSERT(m_pIDataBaseSink!=NULL);
//			m_pIDataBaseSink->OnDataBaseRequest(*pDataBaseEvent,pDataBaseEvent+1,wHandleBuffer);
//
//			return;
//		}
//	}
//
//	return;
//}

//构造函数
CDataBaseService::CDataBaseService(void)
{
#ifdef _DEBUG
	LOG(INFO) <<"CDataBaseService";
#endif
	//设置变量
	m_bService=false;
	m_pIDataBaseSink=NULL;

	return;
}

//析构函数
CDataBaseService::~CDataBaseService(void)
{
#ifdef _DEBUG
	LOG(INFO) <<"~CDataBaseService";
#endif
}

//注册接口
bool __cdecl CDataBaseService::SetDataBaseSink(CObject * pIObject)
{
	//效验参数
	////ASSERT(pIUnknownEx!=NULL);
	////ASSERT(m_pIDataBaseSink==NULL);
	if (pIObject==NULL) return false;
	if (m_pIDataBaseSink!=NULL) return false;

	//查询接口
	m_pIDataBaseSink=dynamic_cast<IDataBaseSink*>(pIObject);
	if(m_pIDataBaseSink==NULL)
	{
#ifdef _DEBUG
		LOG(INFO)<<"数据库引擎外挂服务接口获取失败，挂接操作失败";
#endif
		return false;
	}

	return true;
}

//获取接口
bool __cdecl CDataBaseService::GetQueueService(CDataQueueService ** pIObject)
{
	*pIObject = &m_RequestQueueService;
	if(*pIObject)
		return true;
	else
		return false;
}

//启动服务
bool __cdecl CDataBaseService::StartService()
{
	//判断状态
	if (m_bService==true) 
	{
#ifdef _DEBUG
		 LOG(INFO)<<(TEXT("数据库引擎重复启动，启动操作忽略"),Level_Warning);
#endif
		return true;
	}

	//外挂接口
	if (m_pIDataBaseSink==NULL)
	{
#ifdef _DEBUG
		 LOG(INFO)<<"数据库引擎外挂服务不存在";
#endif
		return false;
	}

	//设置队列
	if (m_RequestQueueService.SetDataQueueServiceSink(this)==false)
	{
#ifdef _DEBUG
		 LOG(INFO)<<"数据库引擎与队列服务绑定失败";
#endif
		return false;
	}

	//启动外挂
	if (m_pIDataBaseSink->StartService(this)==false)//NEW
	{
#ifdef _DEBUG
		 LOG(INFO)<<"数据库引擎外挂服务启动失败";
#endif
		return false;
	}

	//启动队列
	if (m_RequestQueueService.StartService()==false)
	{
#ifdef _DEBUG
		 LOG(INFO)<<"数据库引擎队列服务启动失败";
#endif
		return false;
	}

	//设置变量
	m_bService=true;

	return true;
}

//停止服务
bool __cdecl CDataBaseService::EndService()
{
	//设置变量
	m_bService=false;

	//停止请求队列
	m_RequestQueueService.EndService();

	//停止外挂
	if (m_pIDataBaseSink!=NULL)
	{
		m_pIDataBaseSink->EndService(this);
	}

	return true;
}

//队列接口
void __cdecl CDataBaseService::OnDataQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize, DWORD dwInsertTime)
{
	//判断状态
	if (m_bService==false) return;

	//请求处理
	switch (wIdentifier)
	{
	case EVENT_DATABASE:
		{
			//效验参数
			//ASSERT(pBuffer!=NULL);
			//ASSERT(wDataSize>=sizeof(NTY_DataBaseEvent));
			if (wDataSize<sizeof(NTY_DataBaseEvent)) return;

			//变量定义
			NTY_DataBaseEvent * pDataBaseEvent=(NTY_DataBaseEvent *)pBuffer;
			WORD wHandleBuffer=wDataSize-sizeof(NTY_DataBaseEvent);

			//处理数据
			//ASSERT(m_pIDataBaseSink!=NULL);
			m_pIDataBaseSink->OnDataBaseRequest(*pDataBaseEvent,pDataBaseEvent+1,wHandleBuffer);

			return;
		}
	}

	return;
}

//////////////////////////////////////////////////////////////////////////

