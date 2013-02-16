#pragma once
#include "CenterServer.h"
class CDataBaseSink :
	public IDataBaseSink
{
public:
	CDataBaseSink(void);
	~CDataBaseSink(void);
	//调度接口
public:
	//数据库模块启动
	virtual bool __cdecl StartService(CObject * pObject);
	//数据库模块关闭
	virtual bool __cdecl EndService(CObject * pObject);
	//数据操作处理
	virtual bool __cdecl OnDataBaseRequest(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);

	//配置函数 
public:
	//初始化数据库模块
	bool InitDataBaseSink(IKernelService *pObject);

private:
	//数据库
	CMysqlDataBase			*m_pDataBase;
	//数据队列事件
	CQueueServiceEvent		m_QueueServiceEvent;

};

