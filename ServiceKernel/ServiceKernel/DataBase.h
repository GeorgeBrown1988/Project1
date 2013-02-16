#pragma once
#include "Interface.h"
//#include "Thread.h"
//MYSQL数据库类封装
//
#include "QueueService.h"
#include <mysql.h>
#include <vector>
using namespace std;
struct tagConnectInfo
{
	CString							m_szDataBaseIP;					//数据库主机IP
	UINT							m_uPort;						//端口号
	CString							m_szUserName;					//用户名
	CString							m_szPassWord;					//密码
	CString							m_szDataBase;					//数据库名称
};
//1115添加错误对象
//1115执行SQL存储过程化
typedef struct tagConnectInfo ConnectInfo;
//Mysql
//interface IDataBase : public CObject
//{
//public:
//	//连接数据库
//	virtual bool __cdecl ConnectDataBase()=NULL;
//	//关闭连接
//	virtual void __cdecl CloseDataBase(bool)=NULL;
//	//设置连接信息
//	virtual void __cdecl SettingConnectInfo(CString & szIP,UINT uPort,CString & szUser,CString & szPass,CString & szDB)=NULL;
//	//是否连接错误
//	virtual bool __cdecl IsConnectError()=NULL; 
//	//获取错误信息
//	virtual CString __cdecl GetErrorInfo()=NULL;
//	//清除连接信息
//	virtual void __cdecl ClearConnectInfo()=NULL;
//	//设置语言编码
//	virtual void __cdecl SetLanguage(CString &)=NULL;
//	//是否可用
//	//virtual bool __cdecl IsValid()=NULL;
//	//执行存储过程
//	virtual int __cdecl SQLProc(CString & )=NULL;
//	//
//	virtual int __cdecl GetDataBaseType() = NULL;
//	//生成结果
//	virtual bool __cdecl GetFieldValue(int row,int col,CString &)=NULL;
//	//
//	virtual bool __cdecl GetFieldValue(int row,int col,int &)=NULL;
//	//
//	virtual bool __cdecl GetFieldValue(int row,int col,float &)=NULL;
//	//
//	//virtual bool __cdecl GetFieldValue(int row,int col,BYTE*,int size)=NULL;
//	//获取结果
//	virtual void __cdecl GetResult(int &row,int &col)=NULL;
//	virtual bool __cdecl GetStatus()=NULL;
//	//释放结果集
//	virtual void __cdecl FreeResult()=NULL;
//};
class CMysqlDataBase : public IDataBase 
{
public:
	CMysqlDataBase();
	virtual ~CMysqlDataBase();
//成员函数
public:
	//连接数据库
	virtual bool __cdecl ConnectDataBase();
	//关闭连接
	virtual void __cdecl CloseDataBase(bool);
	//设置连接信息
	virtual void __cdecl SettingConnectInfo(CString  szIP,UINT uPort,CString  szUser,CString  szPass,CString  szDB);
	//是否连接错误
	virtual bool __cdecl IsConnectError(); 
	//

	//
	virtual int __cdecl GetDataBaseType(){return m_eType;};
	//获取错误信息
	virtual CString __cdecl GetErrorInfo();
	//清除连接信息
	virtual void __cdecl ClearConnectInfo();
	//
	virtual void __cdecl ClearResult(){;}
	//设置语言编码
	virtual void __cdecl SetLanguage(CString &);
	//
	virtual bool __cdecl SetAutoCommit(bool );
	//
	virtual void __cdecl SetConf(CString &,bool );
	//
	virtual bool __cdecl OnCommit();
//外部功能
public:
	//执行存储过程
	virtual int __cdecl	SQLProc(CString & );
	//生成结果
	virtual bool __cdecl GetFieldValue(int row,int col,CString &);
	//
	virtual bool __cdecl GetFieldValue(int row,int col ,int &);
	//
	virtual bool __cdecl GetFieldValue(int row,int col ,float &);
	//
	//bool __cdecl GetFieldValue(int row,int col,BYTE *pbuf,int size);
	//获取结果
	virtual void __cdecl GetResult(int &row,int &col);
	//释放结果集
	virtual void __cdecl FreeResult();
	//
	virtual bool __cdecl GetStatus(){return m_bStatus;};
	//
	int __cdecl HandleWork(){;};
	//bool SQLExecute();
//成员变量
private:
	//MYSQL 数据库内部结构体
	MYSQL								*Mysql_Handle;					//MYSQL 实例句柄
	MYSQL_RES							*Mysql_Result;					//MYSQL 结果集
	MYSQL_FIELD							*Mysql_FieldName;				//MYSQL 字段列名
	MYSQL_ROW							Mysql_RowNum;					//MYSQL 字段行号
	vector<CString>						Mysql_ResultVec;
//成员变量
	CString								m_ConnectString;				//连接字符串
	ConnectInfo							m_ConnectInfo; 					//连接信息	
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
//		//友员定义
//	friend class CDataBase;
//public:
//	CDataBaseWorkThread();
//	virtual ~CDataBaseWorkThread();
//public:
//
//	//功能函数
//public:
//	//配置函数
//	bool __cdecl InitThread(HANDLE hCompletionPort);
//	//取消配置
//	bool __cdecl UnInitThread();
//	//
//	bool __cdecl InitThread();
//	//
//	bool __cdecl GetStatus();
//	//重载函数
//private:
//	//运行函数
//	virtual bool RepetitionRun();
//	//辅助变量
//private:
//	BYTE							m_cbBuffer[MAX_QUEUE_PACKET];		//接收缓冲	
//
//
//	//配置定义
//protected:
//	HANDLE							m_hCompletionPort;					//完成端口
//};
//class CDBPoolMng : public CObject
//{
//public:
//	CDBPoolMng();
//	virtual ~CDBPoolMng();
//public:
//	//初始化连接池
//	void __cdecl InitDataBasePool(INT poolsize);
//	//反初始化连接池
//	void __cdecl UnInitDataBasePool();
//	//启动数据库连接池服务
//	bool __cdecl StartDataBasePool();
//	//关闭数据库连接池服务
//	bool __cdecl EndDataBasePool();
//	//关闭一个实例
//	void __cdecl CloseInstance(INT );
//	//获得一个空闲的连接实例
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
//	//获取连接池负载信息
//	void __cdecl GetDataBasePoolInfo(int & , int &);
//	//获取连接池负载实况
//	void __cdecl GetDataBasePoolStatus(vector<bool> &);
//	//
//	
//	private:
//		//连接实例集
//		vector<CDataBase*>					m_DBConnectVec;
//		vector<CDataBaseWorkThread*>			m_DBThreadVec;
//		CThreadLock						m_ThreadLock;						//线程锁
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
//	//请求实例ID
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
//	//结果
//	virtual bool __cdecl GetFieldValue(int ins,int row,int col,CString &);
//	//结果
//	virtual bool __cdecl GetFieldValue(int ins,int row,int col,int &);
//	//结果
//	virtual bool __cdecl GetFieldValue(int ins,int row,int col,float &);
//	//
//	virtual bool __cdecl GetResStatus(int res);
//
//	virtual void __cdecl FreeResult(int res);
//
//	virtual void __cdecl ReleaseDataBaseRes(int res);
//	//
//	//virtual void __cdecl DataBaseHandle(){;};
//	//队列接口
//public:
//	//队列接口
//	virtual void __cdecl OnDataQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize, DWORD dwInsertTime);
//
//public:
//	//CDBPoolMng						*m_pDBPool;
//	bool							m_bService;
//	IDataBaseSink					* m_pIDataBaseSink;					//通知钩子
//};


//////////////////////////////////////////////////////////////////////////

//数据库管理类
class CDataBaseService : public IDataBaseService, public IDataQueueServiceSink,public CObject
{
	//内核变量
protected:
	bool							m_bService;							//运行标志
	CDataQueueService				m_RequestQueueService;				//队列对象
	IDataBaseSink					* m_pIDataBaseSink;					//通知钩子

	//函数定义
public:
	//构造函数
	CDataBaseService(void);
	//析构函数
	virtual ~CDataBaseService(void);


	//服务接口
public:
	//启动服务
	virtual bool __cdecl StartService();
	//停止服务
	virtual bool __cdecl EndService();
	//注册钩子
	virtual bool __cdecl SetDataBaseSink(CObject * pIObject);
	//获取接口
	virtual bool __cdecl GetQueueService(CDataQueueService ** pIObject);

	//队列接口
public:
	//队列接口
	virtual void __cdecl OnDataQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize, DWORD dwInsertTime);
};
