#pragma once
class CLoginService
{
	//变量定义
protected:
	//CCenterSocket					m_CenterSocket;						//中心连接
	CDataBaseSink					m_DataBaseSink;						//数据库钩子
	CMsgTriggerServiceSink			m_MsgTriggerSink;						//调度钩子

	//组件变量
protected:
	CKernelService					m_ServiceEngine;					//服务引擎
	//函数定义
public:
	//构造函数
	CLoginService(void);
	//析构函数
	virtual ~CLoginService(void);

	//功能函数
public:
	//服务状态
	bool IsService();
	//开始服务
	bool BeginService();
	//停止服务
	bool EndService();

	//内部函数
private:
	//显示错误
	void TraceEventString(LPCTSTR pszString, enTraceLevel TraceLevel);
};

