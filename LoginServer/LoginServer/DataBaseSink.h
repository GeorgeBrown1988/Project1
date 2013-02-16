#pragma once

//数据库引擎钩子
class CDataBaseSink : public IDataBaseSink
{
	//函数定义
public:
	//构造函数
	CDataBaseSink();
	//析构函数
	virtual ~CDataBaseSink();

	//调度接口
public:
	//模块启动
	virtual bool __cdecl StartService(CObject * pObject);
	//模块关闭
	virtual bool __cdecl EndService(CObject * pObject);
	//数据操作处理
	virtual bool __cdecl OnDataBaseRequest(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);

	//配置函数 
public:
	//配置函数
	bool InitDataBaseSink(IKernelService *pObject);

	//处理函数
private:
	//登录请求处理
	bool OnRequestLogon(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);

	//存储过程
private:
	//I D 存储过程
	LONG SPLogonByUserID(DWORD dwGameID, LPCTSTR pszPassword, DWORD dwClientIP, LPCTSTR pszComputerID);
	//帐号存储过程
	LONG SPLogonByAccounts(LPCTSTR pszAccounts, LPCTSTR pszPassword, DWORD dwClientIP, LPCTSTR pszComputerID);
	//注册存储过程
	LONG SPRegisterAccounts(LPCTSTR pszAccounts, LPCTSTR pszPassword, LPCTSTR pszSpreader, WORD wFaceID, BYTE cbGender, DWORD dwClientIP, LPCTSTR pszComputerID);
	//组件变量
protected:
	CMysqlDataBase					*m_pUserDataBase;						//用户数据库
	CQueueServiceEvent				m_QueueServiceEvent;					//调度通知
};
