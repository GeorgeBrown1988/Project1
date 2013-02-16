#pragma once
#include "stdafx.h"

enum DBTYPE{DBMYSQL=1,DBORACLE=2,DBMSSQL=3,DBSQLITE=4};

class CDataQueueService;
class CDataBaseService;
class CMsgTriggerService;
class CTCPSocketService;
interface IDataQueueServiceSink;
//���Ӷ���ص��ӿ�
//interface IServerSocketItemSink
//{
//	//Ӧ����Ϣ
//	virtual bool OnSocketAcceptEvent(CServerSocketItem * pServerSocketItem)=NULL;
//	//��ȡ��Ϣ
//	virtual bool OnSocketReadEvent(CMD_Command Command, void * pBuffer, WORD wDataSize, CServerSocketItem * pServerSocketItem)=NULL;
//	//�ر���Ϣ
//	virtual bool OnSocketCloseEvent(CServerSocketItem * pServerSocketItem)=NULL;
//};

//����һ���ݶ���Ϊ�����ķ��� 
interface IDataQueueService:public CObject
{
	//�������� 
	virtual bool __cdecl StartService()=NULL;
	//�������� 
	virtual bool __cdecl EndService()=NULL;
	//���ýӿ�
	virtual bool __cdecl SetDataQueueServiceSink(IDataQueueServiceSink * pobj)=NULL;
	//��ȡ������Ϣ 
	virtual bool __cdecl GetDataQueueInfo(tagBurthenInfo &)=NULL;

};
//���ݶ��нӿ�
interface IDataQueueSink:public CObject 
{
	//��������
	virtual bool __cdecl AddToQueue(WORD wIdentifier, void * const pBuffer, WORD wDataSize)=NULL;
};
//���ݶ����๳�ӽӿ�
interface IDataQueueServiceSink:public CObject
{
	//֪ͨ�ص�����
	virtual void __cdecl OnDataQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize, DWORD dwInsertTime)=NULL;
};
/*
	˵����Ϊ���ݾ�ϵͳ�����������������ϵͳ��ͬ��
*/
interface IDBError : public CObject
{
	//��ʾ������Ϣ
	virtual void __cdecl ShowErrorMsg()=NULL;
};
interface IDataBase : public CObject
{
public:
	//�������ݿ�
	virtual bool __cdecl ConnectDataBase()=NULL;
	//�ر�����
	virtual void __cdecl CloseDataBase(bool)=NULL;
	//����������Ϣ
	virtual void __cdecl SettingConnectInfo(CString szIP,UINT uPort,CString szUser,CString szPass,CString szDB)=NULL;
	//�Ƿ����Ӵ���
	virtual bool __cdecl IsConnectError()=NULL; 
	//��ȡ������Ϣ
	virtual CString __cdecl GetErrorInfo()=NULL;
	//���������Ϣ
	virtual void __cdecl ClearConnectInfo()=NULL;
	//�������Ա���
	virtual void __cdecl SetLanguage(CString &)=NULL;
	//�Ƿ����
	//virtual bool __cdecl IsValid()=NULL;
	//ִ�д洢����
	virtual int __cdecl SQLProc(CString & )=NULL;
	//
	virtual int __cdecl GetDataBaseType() = NULL;
	//���ɽ��
	virtual bool __cdecl GetFieldValue(int row,int col,CString &)=NULL;
	//
	virtual bool __cdecl GetFieldValue(int row,int col,int &)=NULL;
	//
	virtual bool __cdecl GetFieldValue(int row,int col,float &)=NULL;
	//
	//virtual bool __cdecl GetFieldValue(int row,int col,BYTE*,int size)=NULL;
	//��ȡ���
	virtual void __cdecl GetResult(int &row,int &col)=NULL;
	virtual bool __cdecl GetStatus()=NULL;
	//�ͷŽ����
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

//���ݿ⹳�ӽӿ�
interface IDataBaseSink : public CObject
{
	//���ݿ�ģ������
	virtual bool __cdecl StartService(CObject * pObject)=NULL;
	//���ݿ�ģ��ر�
	virtual bool __cdecl EndService(CObject * pObject)=NULL;
	//���ݲ�������
	virtual bool __cdecl OnDataBaseRequest(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize)=NULL;
};
//���ݿ�����ӿ�
interface IDataBaseService : public CObject
{
	//��������
	virtual bool __cdecl StartService()=NULL;
	//ֹͣ����
	virtual bool __cdecl EndService()=NULL;
	//ע�ṳ��
	virtual bool __cdecl SetDataBaseSink(CObject * pIObject)=NULL;
	//��ȡ�ӿ�
	virtual bool __cdecl GetQueueService(IDataQueueService ** pIObject)=NULL;
};

//TCP SOCKET �������ӿ�
interface ITCPSocketServiceManager : public CObject
{
	//���ýӿ�
	virtual bool __cdecl SetSocketServiceSink(CObject * pIObject)=NULL;
	//���ö˿�
	virtual bool __cdecl SetServicePort(INT wListenPort)=NULL;
	//������Ŀ
	virtual bool __cdecl SetMaxSocketItem(WORD wMaxSocketItem)=NULL;
	//��������
	virtual bool __cdecl StartService()=NULL;
	//ֹͣ����
	virtual bool __cdecl EndService()=NULL;
};
//TCP SOCKET ����ӿ�
interface ITCPSocketService : public CObject
{
	//�������
	virtual bool __cdecl DetectSocket()=NULL;
	//���ͺ���
	virtual bool __cdecl SendData(WORD wIndex, WORD wRoundID, WORD wMainCmdID, WORD wSubCmdID)=NULL;
	//���ͺ���
	virtual bool __cdecl SendData(WORD wIndex, WORD wRoundID, WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)=NULL;
	//��������
	virtual bool __cdecl SendDataBatch(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)=NULL;
	//�ر�����
	virtual bool __cdecl CloseSocket(WORD wIndex, WORD wRoundID)=NULL;
	//�ر�����
	virtual bool __cdecl ShutDownSocket(WORD wIndex, WORD wRoundID)=NULL;
	//����Ⱥ��
	virtual bool __cdecl AllowBatchSend(WORD wIndex, WORD wRoundID, bool bAllow)=NULL;
};
//����ģ�鹳�ӽӿ�
interface IMsgTriggerServiceSink : public CObject
{
	//����ӿ�
public:
	//����ģ������
	virtual bool __cdecl StartService(CObject * pICObject)=NULL;
	//����ģ��ر�
	virtual bool __cdecl EndService(CObject * pICObject)=NULL;
	//�¼�����ӿ�
	virtual bool __cdecl OnMsgTrigger(WORD wIdentifier, void * pBuffer, WORD wDataSize, DWORD dwInsertTime)=NULL;

	//�¼��ӿ�
public:
	//���ݿ��¼�
	virtual bool __cdecl OnEventDataBase(void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent)=NULL;
	//����Ӧ���¼�
	virtual bool __cdecl OnEventSocketAccept(NTY_SocketAcceptEvent * pSocketAcceptEvent)=NULL;
	//�����ȡ�¼�
	virtual bool __cdecl OnEventSocketRead(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent)=NULL;
	//����ر��¼�
	virtual bool __cdecl OnEventSocketClose(NTY_SocketCloseEvent * pSocketCloseEvent)=NULL;
};
//��������ӿ�
interface IMsgTriggerService : public CObject
{
	//����ӿ�
public:
	//��������
	virtual bool __cdecl StartService()=NULL;
	//ֹͣ����
	virtual bool __cdecl EndService()=NULL;
	//��������
	virtual bool __cdecl SetSocketService(CObject * pIObject)=NULL;
	//���ù���
	virtual bool __cdecl SetMsgTriggerServiceSink(CObject * pIObject)=NULL;
	//��ȡ�ӿ�
	virtual bool __cdecl GetMsgTriggerQueueService(IDataQueueService ** pIObject)=NULL;
};

//��������ӿ�
interface IKernelService : public CObject
{
	//����ӿ�
public:
	//��������
	virtual bool __cdecl StartService()=NULL;
	//ֹͣ����
	virtual bool __cdecl EndService()=NULL;

	//���ýӿ�
public:
	//�����¼�
	//virtual bool __cdecl SetEventService(IEventService * pIObject)=NULL;
	//���ù���
	virtual bool __cdecl SetDataBaseSink(CObject * pIObject)=NULL;
	//���ù���
	virtual bool __cdecl SetMsgTriggerServiceSink(CObject * pIObject)=NULL;
	//������������ӿ�
	virtual bool __cdecl InitKernelService(INT wListenPort, INT wMaxSocketItem)=NULL;

	//�����ѯ
public:
	//��ȡ���ݿ�����ӿ�
	virtual bool __cdecl GetDataBaseService(IDataBaseService ** pIObject)=NULL;
	//��ȡ��������ӿ�
	virtual bool __cdecl GetMsgTriggerService(IMsgTriggerService ** pIObject)=NULL;
	//��ȡ��������ӿ�
	virtual bool __cdecl GetTCPSocketService(ITCPSocketService ** pIObject)=NULL;
	//��ȡ�ӿ�
	virtual bool __cdecl GetDataBaseQueueService(IDataQueueService ** pIObject)=NULL;
	//��ȡ�ӿ�
	virtual bool __cdecl GetMsgTriggerQueueService(IDataQueueService ** pIObject)=NULL;

	//���ܽӿ�
public:
	//����״̬
	virtual bool __cdecl IsService()=NULL;
	//�ⲿ����
	virtual bool __cdecl ControlService(void * pBuffer, WORD wDataSize)=NULL;
};

//interface IContainer
//{
//	//��
//	virtual void __cdecl Add(int nIndex,CObject &) = NULL;
//	virtual void __cdecl Append(CObject &) = NULL;
//	//ɾ
//	virtual void __cdecl RemoveAll() = NULL;
//	virtual void __cdecl RemoveAt(int nIndex) = NULL;
//	//��
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
