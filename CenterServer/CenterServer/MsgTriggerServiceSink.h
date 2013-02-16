#pragma once
#include "stdafx.h"
#include "ServerList.h"
class CMsgTriggerServiceSink :public IMsgTriggerServiceSink
{

public:

	CMsgTriggerServiceSink(void);
	
	virtual ~CMsgTriggerServiceSink(void);

	//组件定义
protected:
	CQueueServiceEvent				m_DataBaseEvent;					//数据库通知
	//接口变量
	ITCPSocketService				* m_pITCPSocketService;				//网络接口
	//
	CServerList						m_ServerListCenter;


	//调度接口
public:
	//调度模块启动
	virtual bool __cdecl StartService(CObject * pICObject);
	//调度模块关闭
	virtual bool __cdecl EndService(CObject * pICObject);
	//事件处理接口
	virtual bool __cdecl OnMsgTrigger(WORD wIdentifier, void * pDataBuffer, WORD wDataSize, DWORD dwInsertTime);

	//配置函数 
public:
	//配置函数
	bool InitMsgTriggerSink(CKernelService * pICObject);

	//事件接口
public:
	//数据库事件
	virtual bool __cdecl OnEventDataBase(void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent);
	//网络应答事件
	virtual bool __cdecl OnEventSocketAccept(NTY_SocketAcceptEvent * pSocketAcceptEvent);
	//网络读取事件
	virtual bool __cdecl OnEventSocketRead(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent);
	//网络关闭事件
	virtual bool __cdecl OnEventSocketClose(NTY_SocketCloseEvent * pSocketCloseEvent);

	//网络函数
private:
	//列表消息处理
	bool OnSocketMainServerList(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent);
	//房间消息处理
	bool OnSocketMainServerManager(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent);

};

