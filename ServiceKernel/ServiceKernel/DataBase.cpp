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
				LOG(INFO)<<"�������ݿ�ʧ��";
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
    while (Mysql_RowNum = mysql_fetch_row(Mysql_Result))//ÿ�����ÿһ��
    {  
            unsigned int field_count;
            field_count =0;
            while(field_count < mysql_field_count(Mysql_Handle))//��
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
	szMsg.Format("���ݿ����Ӵ�����Ϣ��%s",m_pDB->GetErrorInfo());
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
//	memset(m_cbBuffer,0,MAX_QUEUE_PACKET);		//���ջ���	
//	m_hCompletionPort = NULL;					//��ɶ˿�
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
//	//���ñ���
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
//	//����ʵ��ID
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
////���нӿ�
//void __cdecl CDataBaseService::OnDataQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize, DWORD dwInsertTime)
//{
//	//�ж�״̬
//	if (m_bService==false) return;
//
//	//������
//	switch (wIdentifier)
//	{
//	case EVENT_DATABASE:
//		{
//			//Ч�����
//			//ASSERT(pBuffer!=NULL);
//			//ASSERT(wDataSize>=sizeof(NTY_DataBaseEvent));
//			if (wDataSize<sizeof(NTY_DataBaseEvent)) return;
//
//			//��������
//			NTY_DataBaseEvent * pDataBaseEvent=(NTY_DataBaseEvent *)pBuffer;
//			WORD wHandleBuffer=wDataSize-sizeof(NTY_DataBaseEvent);
//
//			//��������
//			//ASSERT(m_pIDataBaseSink!=NULL);
//			m_pIDataBaseSink->OnDataBaseRequest(*pDataBaseEvent,pDataBaseEvent+1,wHandleBuffer);
//
//			return;
//		}
//	}
//
//	return;
//}

//���캯��
CDataBaseService::CDataBaseService(void)
{
#ifdef _DEBUG
	LOG(INFO) <<"CDataBaseService";
#endif
	//���ñ���
	m_bService=false;
	m_pIDataBaseSink=NULL;

	return;
}

//��������
CDataBaseService::~CDataBaseService(void)
{
#ifdef _DEBUG
	LOG(INFO) <<"~CDataBaseService";
#endif
}

//ע��ӿ�
bool __cdecl CDataBaseService::SetDataBaseSink(CObject * pIObject)
{
	//Ч�����
	////ASSERT(pIUnknownEx!=NULL);
	////ASSERT(m_pIDataBaseSink==NULL);
	if (pIObject==NULL) return false;
	if (m_pIDataBaseSink!=NULL) return false;

	//��ѯ�ӿ�
	m_pIDataBaseSink=dynamic_cast<IDataBaseSink*>(pIObject);
	if(m_pIDataBaseSink==NULL)
	{
#ifdef _DEBUG
		LOG(INFO)<<"���ݿ�������ҷ���ӿڻ�ȡʧ�ܣ��ҽӲ���ʧ��";
#endif
		return false;
	}

	return true;
}

//��ȡ�ӿ�
bool __cdecl CDataBaseService::GetQueueService(CDataQueueService ** pIObject)
{
	*pIObject = &m_RequestQueueService;
	if(*pIObject)
		return true;
	else
		return false;
}

//��������
bool __cdecl CDataBaseService::StartService()
{
	//�ж�״̬
	if (m_bService==true) 
	{
#ifdef _DEBUG
		 LOG(INFO)<<(TEXT("���ݿ������ظ�������������������"),Level_Warning);
#endif
		return true;
	}

	//��ҽӿ�
	if (m_pIDataBaseSink==NULL)
	{
#ifdef _DEBUG
		 LOG(INFO)<<"���ݿ�������ҷ��񲻴���";
#endif
		return false;
	}

	//���ö���
	if (m_RequestQueueService.SetDataQueueServiceSink(this)==false)
	{
#ifdef _DEBUG
		 LOG(INFO)<<"���ݿ���������з����ʧ��";
#endif
		return false;
	}

	//�������
	if (m_pIDataBaseSink->StartService(this)==false)//NEW
	{
#ifdef _DEBUG
		 LOG(INFO)<<"���ݿ�������ҷ�������ʧ��";
#endif
		return false;
	}

	//��������
	if (m_RequestQueueService.StartService()==false)
	{
#ifdef _DEBUG
		 LOG(INFO)<<"���ݿ�������з�������ʧ��";
#endif
		return false;
	}

	//���ñ���
	m_bService=true;

	return true;
}

//ֹͣ����
bool __cdecl CDataBaseService::EndService()
{
	//���ñ���
	m_bService=false;

	//ֹͣ�������
	m_RequestQueueService.EndService();

	//ֹͣ���
	if (m_pIDataBaseSink!=NULL)
	{
		m_pIDataBaseSink->EndService(this);
	}

	return true;
}

//���нӿ�
void __cdecl CDataBaseService::OnDataQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize, DWORD dwInsertTime)
{
	//�ж�״̬
	if (m_bService==false) return;

	//������
	switch (wIdentifier)
	{
	case EVENT_DATABASE:
		{
			//Ч�����
			//ASSERT(pBuffer!=NULL);
			//ASSERT(wDataSize>=sizeof(NTY_DataBaseEvent));
			if (wDataSize<sizeof(NTY_DataBaseEvent)) return;

			//��������
			NTY_DataBaseEvent * pDataBaseEvent=(NTY_DataBaseEvent *)pBuffer;
			WORD wHandleBuffer=wDataSize-sizeof(NTY_DataBaseEvent);

			//��������
			//ASSERT(m_pIDataBaseSink!=NULL);
			m_pIDataBaseSink->OnDataBaseRequest(*pDataBaseEvent,pDataBaseEvent+1,wHandleBuffer);

			return;
		}
	}

	return;
}

//////////////////////////////////////////////////////////////////////////

