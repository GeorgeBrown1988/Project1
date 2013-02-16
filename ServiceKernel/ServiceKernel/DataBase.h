#pragma once
#include "Interface.h"
//#include "Thread.h"
//MYSQL���ݿ����װ
//
#include "QueueService.h"
#include <mysql.h>
#include <vector>
using namespace std;
struct tagConnectInfo
{
	CString							m_szDataBaseIP;					//���ݿ�����IP
	UINT							m_uPort;						//�˿ں�
	CString							m_szUserName;					//�û���
	CString							m_szPassWord;					//����
	CString							m_szDataBase;					//���ݿ�����
};
//1115��Ӵ������
//1115ִ��SQL�洢���̻�
typedef struct tagConnectInfo ConnectInfo;
//Mysql
//interface IDataBase : public CObject
//{
//public:
//	//�������ݿ�
//	virtual bool __cdecl ConnectDataBase()=NULL;
//	//�ر�����
//	virtual void __cdecl CloseDataBase(bool)=NULL;
//	//����������Ϣ
//	virtual void __cdecl SettingConnectInfo(CString & szIP,UINT uPort,CString & szUser,CString & szPass,CString & szDB)=NULL;
//	//�Ƿ����Ӵ���
//	virtual bool __cdecl IsConnectError()=NULL; 
//	//��ȡ������Ϣ
//	virtual CString __cdecl GetErrorInfo()=NULL;
//	//���������Ϣ
//	virtual void __cdecl ClearConnectInfo()=NULL;
//	//�������Ա���
//	virtual void __cdecl SetLanguage(CString &)=NULL;
//	//�Ƿ����
//	//virtual bool __cdecl IsValid()=NULL;
//	//ִ�д洢����
//	virtual int __cdecl SQLProc(CString & )=NULL;
//	//
//	virtual int __cdecl GetDataBaseType() = NULL;
//	//���ɽ��
//	virtual bool __cdecl GetFieldValue(int row,int col,CString &)=NULL;
//	//
//	virtual bool __cdecl GetFieldValue(int row,int col,int &)=NULL;
//	//
//	virtual bool __cdecl GetFieldValue(int row,int col,float &)=NULL;
//	//
//	//virtual bool __cdecl GetFieldValue(int row,int col,BYTE*,int size)=NULL;
//	//��ȡ���
//	virtual void __cdecl GetResult(int &row,int &col)=NULL;
//	virtual bool __cdecl GetStatus()=NULL;
//	//�ͷŽ����
//	virtual void __cdecl FreeResult()=NULL;
//};
class CMysqlDataBase : public IDataBase 
{
public:
	CMysqlDataBase();
	virtual ~CMysqlDataBase();
//��Ա����
public:
	//�������ݿ�
	virtual bool __cdecl ConnectDataBase();
	//�ر�����
	virtual void __cdecl CloseDataBase(bool);
	//����������Ϣ
	virtual void __cdecl SettingConnectInfo(CString  szIP,UINT uPort,CString  szUser,CString  szPass,CString  szDB);
	//�Ƿ����Ӵ���
	virtual bool __cdecl IsConnectError(); 
	//

	//
	virtual int __cdecl GetDataBaseType(){return m_eType;};
	//��ȡ������Ϣ
	virtual CString __cdecl GetErrorInfo();
	//���������Ϣ
	virtual void __cdecl ClearConnectInfo();
	//
	virtual void __cdecl ClearResult(){;}
	//�������Ա���
	virtual void __cdecl SetLanguage(CString &);
	//
	virtual bool __cdecl SetAutoCommit(bool );
	//
	virtual void __cdecl SetConf(CString &,bool );
	//
	virtual bool __cdecl OnCommit();
//�ⲿ����
public:
	//ִ�д洢����
	virtual int __cdecl	SQLProc(CString & );
	//���ɽ��
	virtual bool __cdecl GetFieldValue(int row,int col,CString &);
	//
	virtual bool __cdecl GetFieldValue(int row,int col ,int &);
	//
	virtual bool __cdecl GetFieldValue(int row,int col ,float &);
	//
	//bool __cdecl GetFieldValue(int row,int col,BYTE *pbuf,int size);
	//��ȡ���
	virtual void __cdecl GetResult(int &row,int &col);
	//�ͷŽ����
	virtual void __cdecl FreeResult();
	//
	virtual bool __cdecl GetStatus(){return m_bStatus;};
	//
	int __cdecl HandleWork(){;};
	//bool SQLExecute();
//��Ա����
private:
	//MYSQL ���ݿ��ڲ��ṹ��
	MYSQL								*Mysql_Handle;					//MYSQL ʵ�����
	MYSQL_RES							*Mysql_Result;					//MYSQL �����
	MYSQL_FIELD							*Mysql_FieldName;				//MYSQL �ֶ�����
	MYSQL_ROW							Mysql_RowNum;					//MYSQL �ֶ��к�
	vector<CString>						Mysql_ResultVec;
//��Ա����
	CString								m_ConnectString;				//�����ַ���
	ConnectInfo							m_ConnectInfo; 					//������Ϣ	
	CString								m_szLanguage;
	bool								m_bStatus;
	bool								m_bCommit;
	int									m_col;
	enum DBTYPE							m_eType;
};
class CDBError :public IDBError, public CObject
{
public:
	//
	CDBError(IDataBase*);
	//
	~CDBError();
	//
	virtual void __cdecl ShowErrorMsg();
private:
	IDataBase *m_pDB;
};
//class CDataBaseWorkThread :public CThread
//{
//		//��Ա����
//	friend class CDataBase;
//public:
//	CDataBaseWorkThread();
//	virtual ~CDataBaseWorkThread();
//public:
//
//	//���ܺ���
//public:
//	//���ú���
//	bool __cdecl InitThread(HANDLE hCompletionPort);
//	//ȡ������
//	bool __cdecl UnInitThread();
//	//
//	bool __cdecl InitThread();
//	//
//	bool __cdecl GetStatus();
//	//���غ���
//private:
//	//���к���
//	virtual bool RepetitionRun();
//	//��������
//private:
//	BYTE							m_cbBuffer[MAX_QUEUE_PACKET];		//���ջ���	
//
//
//	//���ö���
//protected:
//	HANDLE							m_hCompletionPort;					//��ɶ˿�
//};
//class CDBPoolMng : public CObject
//{
//public:
//	CDBPoolMng();
//	virtual ~CDBPoolMng();
//public:
//	//��ʼ�����ӳ�
//	void __cdecl InitDataBasePool(INT poolsize);
//	//����ʼ�����ӳ�
//	void __cdecl UnInitDataBasePool();
//	//�������ݿ����ӳط���
//	bool __cdecl StartDataBasePool();
//	//�ر����ݿ����ӳط���
//	bool __cdecl EndDataBasePool();
//	//�ر�һ��ʵ��
//	void __cdecl CloseInstance(INT );
//	//���һ�����е�����ʵ��
//	CDataBase* __cdecl GetFreeInstance();
//	//
//	int __cdecl GetFreeInstanceIndex();
//	//
//	inline CDataBase* GetInstance(int inst)
//	{
//		CThreadLock Lock(m_ThreadLock);
//		if(m_DBConnectVec.size()>inst)
//			return m_DBConnectVec[inst];
//		return NULL;
//	}
//	//��ȡ���ӳظ�����Ϣ
//	void __cdecl GetDataBasePoolInfo(int & , int &);
//	//��ȡ���ӳظ���ʵ��
//	void __cdecl GetDataBasePoolStatus(vector<bool> &);
//	//
//	
//	private:
//		//����ʵ����
//		vector<CDataBase*>					m_DBConnectVec;
//		vector<CDataBaseWorkThread*>			m_DBThreadVec;
//		CThreadLock						m_ThreadLock;						//�߳���
//};
//MARK
//class CDataBaseService : public IDataBaseService,public CObject,public IDataQueueServiceSink
//{
//public:
//	CDataBaseService();
//	virtual ~CDataBaseService();
//
//	virtual bool __cdecl StartService();
//	virtual bool __cdecl EndService();
//	//����ʵ��ID
//	virtual int __cdecl RequestDataBaseRes();
//	//
//	virtual void __cdecl ConfDataBaseRes(int res,int langindex,bool autocommit);
//	//
//	virtual void __cdecl SQLQuery(int res,CString &);
//	//
//	virtual void __cdecl GetResult(int res,int &,int &);
//	//MARK
//	//virtual CString __cdecl GetVal(int res,int row,int col);
//	//
//	//���
//	virtual bool __cdecl GetFieldValue(int ins,int row,int col,CString &);
//	//���
//	virtual bool __cdecl GetFieldValue(int ins,int row,int col,int &);
//	//���
//	virtual bool __cdecl GetFieldValue(int ins,int row,int col,float &);
//	//
//	virtual bool __cdecl GetResStatus(int res);
//
//	virtual void __cdecl FreeResult(int res);
//
//	virtual void __cdecl ReleaseDataBaseRes(int res);
//	//
//	//virtual void __cdecl DataBaseHandle(){;};
//	//���нӿ�
//public:
//	//���нӿ�
//	virtual void __cdecl OnDataQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize, DWORD dwInsertTime);
//
//public:
//	//CDBPoolMng						*m_pDBPool;
//	bool							m_bService;
//	IDataBaseSink					* m_pIDataBaseSink;					//֪ͨ����
//};


//////////////////////////////////////////////////////////////////////////

//���ݿ������
class CDataBaseService : public IDataBaseService, public IDataQueueServiceSink,public CObject
{
	//�ں˱���
protected:
	bool							m_bService;							//���б�־
	CDataQueueService				m_RequestQueueService;				//���ж���
	IDataBaseSink					* m_pIDataBaseSink;					//֪ͨ����

	//��������
public:
	//���캯��
	CDataBaseService(void);
	//��������
	virtual ~CDataBaseService(void);


	//����ӿ�
public:
	//��������
	virtual bool __cdecl StartService();
	//ֹͣ����
	virtual bool __cdecl EndService();
	//ע�ṳ��
	virtual bool __cdecl SetDataBaseSink(CObject * pIObject);
	//��ȡ�ӿ�
	virtual bool __cdecl GetQueueService(CDataQueueService ** pIObject);

	//���нӿ�
public:
	//���нӿ�
	virtual void __cdecl OnDataQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize, DWORD dwInsertTime);
};
