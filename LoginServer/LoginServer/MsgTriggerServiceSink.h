#pragma once

//class CMsgTriggerServiceSink :
//	public IMsgTriggerServiceSink
//{
//public:
//	CMsgTriggerServiceSink(void);
//	~CMsgTriggerServiceSink(void);
//	//管理接口
//public:
//	//调度模块启动
//	virtual bool __cdecl StartService(CObject * pICObject);
//	//调度模块关闭
//	virtual bool __cdecl EndService(CObject * pICObject);
//	//事件处理接口
//	virtual bool __cdecl OnMsgTrigger(WORD wIdentifier, void * pBuffer, WORD wDataSize, DWORD dwInsertTime);
//	//事件接口
//public:
//	//数据库事件
//	virtual bool __cdecl OnEventDataBase(void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent);
//	//网络应答事件
//	virtual bool __cdecl OnEventSocketAccept(NTY_SocketAcceptEvent * pSocketAcceptEvent);
//	//网络读取事件
//	virtual bool __cdecl OnEventSocketRead(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent);
//	//网络关闭事件
//	virtual bool __cdecl OnEventSocketClose(NTY_SocketCloseEvent * pSocketCloseEvent);
//
//	//配置函数 
//public:
//	//配置函数
//	bool InitMsgTriggerSink(CKernelService * pICObject);
//
//	//数据库函数
//private:
//	//用户登录成功
//	bool OnDBLogonSuccess(void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent);
//	//用户登录失败
//	bool OnDBLogonError(void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent);
//
//	//网络函数
//private:
//	//登录消息处理
//	bool OnSocketMainLogon(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent);
//
//	//中心网络
//private:
//	//列表消息处理
//	bool OnCenterMainServerList(CMD_Command Command, void * pDataBuffer, WORD wDataSize);
//
//	//变量定义
//protected:
////	CInitParamter					* m_pInitParamter;					//配置参数
//	tagConnectItemInfo				* m_pConnectItemInfo;				//连接信息
//
//	//中心服务器
////protected:
////	enSocketState					m_SocketState;						//连接状态
////	CCenterSocket					* m_pCenterSocket;					//中心连接
//
//	//辅助组件
//protected:
//	CServerList						m_ServerList;						//服务器列表
//	CQueueServiceEvent				m_DataBaseEvent;					//数据库通知
//
//	//接口变量
//protected:
//	ITCPSocketService				* m_pITCPSocketService;				//网络接口
//};
//
