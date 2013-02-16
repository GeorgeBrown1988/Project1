#pragma once
#include "CenterServer.h"
#include "DataBaseSink.h"
#include "MsgTriggerServiceSink.h"
class CCenterService
{
public:
	CCenterService();
	virtual ~CCenterService();

	//功能函数
public:
	//开始服务
	bool StartService();
	//停止服务
	bool EndService();
	//服务状态
	bool IsService();
	//
	void InitCenterService();
	//
private:
	//内核服务
	CKernelService						*m_pKernelService;
	//
	CDataBaseSink						m_DataBaseSink;
	//
	CMsgTriggerServiceSink				m_MsgTriggerServiceSink;						//调度钩子
};

