#pragma once
#include "stdafx.h"

enum DBTYPE{DBMYSQL=1,DBORACLE=2,DBMSSQL=3,DBSQLITE=4};

class CDataQueueService;
class CDataBaseService;
class CMsgTriggerService;
class CTCPSocketService;
interface IDataQueueServiceSink;
//连接对象回调接口
//interface IServerSocketItemSink
//{
//	//应答消息
//	virtual bool OnSocketAcceptEvent(CServerSocketItem * pServerSocketItem)=NULL;
//	//读取消息
//	virtual bool OnSocketReadEvent(CMD_Command Command, void * pBuffer, WORD wDataSize, CServerSocketItem * pServerSocketItem)=NULL;
//	//关闭消息
//	virtual bool OnSocketCloseEvent(CServerSocketItem * pServerSocketItem)=NULL;
//};

//方案一数据队列为单独的服务 
interface IDataQueueService:public CObject
{
	//启动服务 
	virtual bool __cdecl StartService()=NULL;
	//结束服务 
	virtual bool __cdecl EndService()=NULL;
	//设置接口
	virtual bool __cdecl SetDataQueueServiceSink(IDataQueueServiceSink * pobj)=NULL;
	//获取负荷信息 
	virtual bool __cdecl GetDataQueueInfo(tagBurthenInfo &)=NULL;

};
//数据队列接口
interface IDataQueueSink:public CObject 
{
	//加入数据
	virtual bool __cdecl AddToQueue(WORD wIdentifier, void * const pBuffer, WORD wDataSize)=NULL;
};
//数据队列类钩子接口
interface IDataQueueServiceSink:public CObject
{
	//通知回调函数
	virtual void __cdecl OnDataQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize, DWORD dwInsertTime)=NULL;
};
/*
	说明：为兼容旧系统，函数参数尽量与旧系统相同。
*/
interface IDBError : public CObject
{
	//显示错误消息
	virtual void __cdecl ShowErrorMsg()=NULL;
};
interface IDataBase : public CObject
{
public:
	//连接数据库
	virtual bool __cdecl ConnectDataBase()=NULL;
	//关闭连接
	virtual void __cdecl CloseDataBase(bool)=NULL;
	//设置连接信息
	virtual void __cdecl SettingConnectInfo(CString szIP,UINT uPort,CString szUser,CString szPass,CString szDB)=NULL;
	//是否连接错误
	virtual bool __cdecl IsConnectError()=NULL; 
	//获取错误信息
	virtual CString __cdecl GetErrorInfo()=NULL;
	//清除连接信息
	virtual void __cdecl ClearConnectInfo()=NULL;
	//设置语言编码
	virtual void __cdecl SetLanguage(CString &)=NULL;
	//是否可用
	//virtual bool __cdecl IsValid()=NULL;
	//执行存储过程
	virtual int __cdecl SQLProc(CString & )=NULL;
	//
	virtual int __cdecl GetDataBaseType() = NULL;
	//生成结果
	virtual bool __cdecl GetFieldValue(int row,int col,CString &)=NULL;
	//
	virtual bool __cdecl GetFieldValue(int row,int col,int &)=NULL;
	//
	virtual bool __cdecl GetFieldValue(int row,int col,float &)=NULL;
	//
	//virtual bool __cdecl GetFieldValue(int row,int col,BYTE*,int size)=NULL;
	//获取结果
	virtual void __cdecl GetResult(int &row,int &col)=NULL;
	virtual bool __cdecl GetStatus()=NULL;
	//释放结果集
	virtual void __cdecl FreeResult()=NULL;
};
//interface IDataBaseService
//{
//	virtual bool __cdecl StartService()=NULL;
//
//	virtual bool __cdecl EndService()=NULL;
//
//	virtual int __cdecl RequestDataBaseRes()=NULL;
//	//
//	virtual void __cdecl ConfDataBaseRes(int res,int langindex,bool autocommit)=NULL;
//	//
//	virtual void __cdecl SQLQuery(int res,CString &)=NULL;
//	//
//	virtual void __cdecl GetResult(int res,int &,int &)=NULL;
//	//MARK
//	virtual bool __cdecl GetFieldValue(int res,int row,int col,int &)=NULL;
//	//
//	virtual bool __cdecl GetFieldValue(int res,int row,int col,float &)=NULL;
//	//
//	//virtual bool __cdecl GetFieldValue(int res,int row,int col,BYTE *,int size)=NULL;
//	//
//	virtual bool __cdecl GetResStatus(int res)=NULL;
//
//	virtual void __cdecl FreeResult(int res)=NULL;
//
//	virtual void __cdecl ReleaseDataBaseRes(int res)=NULL;
//};

//数据库钩子接口
interface IDataBaseSink : public CObject
{
	//数据库模块启动
	virtual bool __cdecl StartService(CObject * pObject)=NULL;
	//数据库模块关闭
	virtual bool __cdecl EndService(CObject * pObject)=NULL;
	//数据操作处理
	virtual bool __cdecl OnDataBaseRequest(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)=NULL;
};
//数据库引擎接口
interface IDataBaseService : public CObject
{
	//启动服务
	virtual bool __cdecl StartService()=NULL;
	//停止服务
	virtual bool __cdecl EndService()=NULL;
	//注册钩子
	virtual bool __cdecl SetDataBaseSink(CObject * pIObject)=NULL;
	//获取接口
	virtual bool __cdecl GetQueueService(IDataQueueService ** pIObject)=NULL;
};

//TCP SOCKET 引擎管理接口
interface ITCPSocketServiceManager : public CObject
{
	//设置接口
	virtual bool __cdecl SetSocketServiceSink(CObject * pIObject)=NULL;
	//设置端口
	virtual bool __cdecl SetServicePort(INT wListenPort)=NULL;
	//设置数目
	virtual bool __cdecl SetMaxSocketItem(WORD wMaxSocketItem)=NULL;
	//启动服务
	virtual bool __cdecl StartService()=NULL;
	//停止服务
	virtual bool __cdecl EndService()=NULL;
};
//TCP SOCKET 引擎接口
interface ITCPSocketService : public CObject
{
	//检测连接
	virtual bool __cdecl DetectSocket()=NULL;
	//发送函数
	virtual bool __cdecl SendData(WORD wIndex, WORD wRoundID, WORD wMainCmdID, WORD wSubCmdID)=NULL;
	//发送函数
	virtual bool __cdecl SendData(WORD wIndex, WORD wRoundID, WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)=NULL;
	//批量发送
	virtual bool __cdecl SendDataBatch(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)=NULL;
	//关闭连接
	virtual bool __cdecl CloseSocket(WORD wIndex, WORD wRoundID)=NULL;
	//关闭连接
	virtual bool __cdecl ShutDownSocket(WORD wIndex, WORD wRoundID)=NULL;
	//允许群发
	virtual bool __cdecl AllowBatchSend(WORD wIndex, WORD wRoundID, bool bAllow)=NULL;
};
//调度模块钩子接口
interface IMsgTriggerServiceSink : public CObject
{
	//管理接口
public:
	//调度模块启动
	virtual bool __cdecl StartService(CObject * pICObject)=NULL;
	//调度模块关闭
	virtual bool __cdecl EndService(CObject * pICObject)=NULL;
	//事件处理接口
	virtual bool __cdecl OnMsgTrigger(WORD wIdentifier, void * pBuffer, WORD wDataSize, DWORD dwInsertTime)=NULL;

	//事件接口
public:
	//数据库事件
	virtual bool __cdecl OnEventDataBase(void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent)=NULL;
	//网络应答事件
	virtual bool __cdecl OnEventSocketAccept(NTY_SocketAcceptEvent * pSocketAcceptEvent)=NULL;
	//网络读取事件
	virtual bool __cdecl OnEventSocketRead(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent)=NULL;
	//网络关闭事件
	virtual bool __cdecl OnEventSocketClose(NTY_SocketCloseEvent * pSocketCloseEvent)=NULL;
};
//调度引擎接口
interface IMsgTriggerService : public CObject
{
	//管理接口
public:
	//启动服务
	virtual bool __cdecl StartService()=NULL;
	//停止服务
	virtual bool __cdecl EndService()=NULL;
	//设置网络
	virtual bool __cdecl SetSocketService(CObject * pIObject)=NULL;
	//设置钩子
	virtual bool __cdecl SetMsgTriggerServiceSink(CObject * pIObject)=NULL;
	//获取接口
	virtual bool __cdecl GetMsgTriggerQueueService(IDataQueueService ** pIObject)=NULL;
};

//服务引擎接口
interface IKernelService : public CObject
{
	//服务接口
public:
	//启动服务
	virtual bool __cdecl StartService()=NULL;
	//停止服务
	virtual bool __cdecl EndService()=NULL;

	//配置接口
public:
	//设置事件
	//virtual bool __cdecl SetEventService(IEventService * pIObject)=NULL;
	//设置钩子
	virtual bool __cdecl SetDataBaseSink(CObject * pIObject)=NULL;
	//设置钩子
	virtual bool __cdecl SetMsgTriggerServiceSink(CObject * pIObject)=NULL;
	//设置网络引擎接口
	virtual bool __cdecl InitKernelService(INT wListenPort, INT wMaxSocketItem)=NULL;

	//服务查询
public:
	//获取数据库引擎接口
	virtual bool __cdecl GetDataBaseService(IDataBaseService ** pIObject)=NULL;
	//获取调度引擎接口
	virtual bool __cdecl GetMsgTriggerService(IMsgTriggerService ** pIObject)=NULL;
	//获取网络引擎接口
	virtual bool __cdecl GetTCPSocketService(ITCPSocketService ** pIObject)=NULL;
	//获取接口
	virtual bool __cdecl GetDataBaseQueueService(IDataQueueService ** pIObject)=NULL;
	//获取接口
	virtual bool __cdecl GetMsgTriggerQueueService(IDataQueueService ** pIObject)=NULL;

	//功能接口
public:
	//服务状态
	virtual bool __cdecl IsService()=NULL;
	//外部控制
	virtual bool __cdecl ControlService(void * pBuffer, WORD wDataSize)=NULL;
};

//interface IContainer
//{
//	//增
//	virtual void __cdecl Add(int nIndex,CObject &) = NULL;
//	virtual void __cdecl Append(CObject &) = NULL;
//	//删
//	virtual void __cdecl RemoveAll() = NULL;
//	virtual void __cdecl RemoveAt(int nIndex) = NULL;
//	//改
//	virtual void __cdecl SetElement(int nIndex,CObject &) = NULL;
//	virtual CObject& __cdecl GetAt(int nIndex) = NULL;
//	//virtual CObject& operator[](INT_PTR nIndex) = NULL;
//	virtual int __cdecl GetCount() = NULL;
//	
//};
//////////////////////////////////////////////////////////////////////////
extern "C" __declspec(dllexport) void * __cdecl CreateKernelService();
extern "C" __declspec(dllexport) void * __cdecl CreateDataBaseService();
extern "C" __declspec(dllexport) void * __cdecl CreateDataQueueService();
extern "C" __declspec(dllexport) void * __cdecl CreateTCPSocketService();
extern "C" __declspec(dllexport) void * __cdecl CreateDataBase();
