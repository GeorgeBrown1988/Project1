#include "StdAfx.h"
//#include "MsgTriggerServiceSink.h"
//
//
//CMsgTriggerServiceSink::CMsgTriggerServiceSink(void)
//{
//}
//
//
//CMsgTriggerServiceSink::~CMsgTriggerServiceSink(void)
//{
//}
//
////宏定义
//#define TIME_RECONNECT						15000L						//重连时间
//#define TIME_UPDATE_LIST					30000L						//更新时间
//
////定时器 ID
//#define IDI_CONNECT_CENTER_SERVER			1							//连接服务器
//#define IDI_UPDATE_SERVER_LIST				2							//更新列表
//
////////////////////////////////////////////////////////////////////////////
//
////构造函数
//CMsgTriggerServiceSink::CMsgTriggerServiceSink()
//{
//
//	//接口变量
//	m_pITCPSocketService = NULL;
//
//	return;
//}
//
////析构函数
//CMsgTriggerServiceSink::~CMsgTriggerServiceSink()
//{
//}
//
//
////调度模块启动
//bool __cdecl CMsgTriggerServiceSink::StartService(CObject * pObject)
//{
//
//	//创建辅助变量
//	//m_pConnectItemInfo=new tagConnectItemInfo [m_pInitParamter->m_wMaxConnect];
//	//if (m_pConnectItemInfo==NULL) return false;
//	//memset(m_pConnectItemInfo,0,sizeof(tagConnectItemInfo)*m_pInitParamter->m_wMaxConnect);
//
//	////连接中心服务器
//	//DWORD dwServerIP=inet_addr(m_pInitParamter->m_szCenterServerAddr);
//	//m_pCenterSocket->Connect(dwServerIP,PORT_CENTER_SERVER);
//
//	return true;
//}
//
////调度模块关闭
//bool __cdecl CMsgTriggerServiceSink::EndService(CObject * pICObject)
//{
//	//删除数据
////	SafeDeleteArray(m_pConnectItemInfo);
//
//	return true;
//}
//
////事件处理接口
//bool __cdecl CMsgTriggerServiceSink::OnMsgTrigger(WORD wIdentifier, void * pDataBuffer, WORD wDataSize, DWORD dwInsertTime)
//{
//	//switch (wIdentifier)
//	//{
//	//case EVENT_CENTER_CONNECT:	//中心连接事件
//	//	{
//	//		//效验大小
//	//		ASSERT(wDataSize==sizeof(NTY_CenterConnect));
//	//		if (wDataSize!=sizeof(NTY_CenterConnect)) return false;
//
//	//		//消息处理
//	//		NTY_CenterConnect * pCenterSocketConnect=(NTY_CenterConnect *)pDataBuffer;
//
//	//		return OnEventCenterSocketConnect(pCenterSocketConnect->iErrorCode);
//	//	}
//	//case EVENT_CENTER_READ:		//中心读取事件
//	//	{
//	//		//效验大小
//	//		NTY_CenterRead * pCenterSocketRead=(NTY_CenterRead *)pDataBuffer;
//	//		ASSERT(wDataSize>=((sizeof(NTY_CenterRead)-sizeof(pCenterSocketRead->cbReadBuffer))));
//	//		ASSERT(wDataSize==(sizeof(NTY_CenterRead)-sizeof(pCenterSocketRead->cbReadBuffer)+pCenterSocketRead->wDataSize));
//	//		if (wDataSize<(sizeof(NTY_CenterRead)-sizeof(pCenterSocketRead->cbReadBuffer))) return false;
//	//		if (wDataSize!=sizeof(NTY_CenterRead)-sizeof(pCenterSocketRead->cbReadBuffer)+pCenterSocketRead->wDataSize) return false;
//
//	//		//处理消息
//	//		bool bSuccess=false;
//	//		try 
//	//		{ 
//	//			bSuccess=OnEventCenterSocketRead(pCenterSocketRead->Command,pCenterSocketRead->cbReadBuffer,pCenterSocketRead->wDataSize);
//	//		}
//	//		catch (...)	{ }
//	//		if (bSuccess==false) m_pCenterSocket->CloseSocket(true);
//
//	//		return true;
//	//	}
//	//case EVENT_CENTER_CLOSE:		//中心关闭事件
//	//	{
//	//		//效验大小
//	//		ASSERT(wDataSize==sizeof(NTY_CenterClose));
//	//		if (wDataSize!=sizeof(NTY_CenterClose)) return false;
//
//	//		//消息处理
//	//		NTY_CenterClose * pCenterSocketClose=(NTY_CenterClose *)pDataBuffer;
//	//		OnEventCenterSocketClose(pCenterSocketClose->bCloseByServer);
//
//	//		return true;
//	//	}
//	//}
//
//	return false;
//}
//
//
////配置函数
//bool CMsgTriggerServiceSink::InitMsgTriggerSink(CKernelService * pICObject)
//{
//	////效验参数
//	//ASSERT(pIUnknownEx!=NULL);
//	//ASSERT(pInitParamter!=NULL);
//	//ASSERT(pCenterSocket!=NULL);
//
//	////设置变量
//	//m_pInitParamter=pInitParamter;
//	//m_pCenterSocket=pCenterSocket;
//
//	////服务引擎
//	//IServiceEngine * pIServiceEngine=(IServiceEngine *)pIUnknownEx->QueryInterface(IID_IServiceEngine,VER_IServiceEngine);
//	//if (pIServiceEngine==NULL)
//	//{
//	//	m_pIEventService->ShowEventNotify(TEXT("获取服务引擎服务失败"),Level_Exception);
//	//	return false;
//	//}
//
//	////定时器引擎
//	//m_pITimerEngine=(ITimerEngine *)pIServiceEngine->GetTimerEngine(IID_ITimerEngine,VER_ITimerEngine);
//	//if (m_pITimerEngine==NULL)
//	//{
//	//	m_pIEventService->ShowEventNotify(TEXT("获取定时器引擎服务失败"),Level_Exception);
//	//	return false;
//	//}
//
//	////网络引擎
//	//m_pITCPSocketService=(ITCPSocketEngine *)pIServiceEngine->GetTCPSocketEngine(IID_ITCPSocketEngine,VER_ITCPSocketEngine);
//	//if (m_pITCPSocketService==NULL)
//	//{
//	//	m_pIEventService->ShowEventNotify(TEXT("获取网络引擎服务失败"),Level_Exception);
//	//	return false;
//	//}
//
//	////数据库引擎
//	//IDataBaseEngine * pIDataBaseEngine=(IDataBaseEngine *)pIServiceEngine->GetDataBaseEngine(IID_IDataBaseEngine,VER_IDataBaseEngine);
//	//if (pIDataBaseEngine==NULL)
//	//{
//	//	m_pIEventService->ShowEventNotify(TEXT("获取数据库引擎服务失败"),Level_Exception);
//	//	return false;
//	//}
//
//	////数据库触发
//	//IUnknownEx * pIQueueService=(IUnknownEx *)pIDataBaseEngine->GetQueueService(IID_IUnknownEx,VER_IUnknownEx);
//	//if (m_DataBaseEvent.SetQueueService(pIQueueService)==false)
//	//{
//	//	m_pIEventService->ShowEventNotify(TEXT("获取数据库触发服务失败"),Level_Exception);
//	//	return false;
//	//}
//
//	////列表组件
//	//if (m_ServerList.SetSocketEngine(m_pITCPSocketService)==false)
//	//{
//	//	m_pIEventService->ShowEventNotify(TEXT("列表组件与网络引擎绑定失败"),Level_Exception);
//	//	return false;
//	//}
//
//	return true;
//}
//
////定时器事件
////bool CMsgTriggerServiceSink::OnEventTimer(WORD wTimerID, WPARAM wBindParam)
////{
////	switch (wTimerID)
////	{
////	case IDI_CONNECT_CENTER_SERVER:		//连接中心服务器
////		{
////			DWORD dwServerIP=inet_addr(m_pInitParamter->m_szCenterServerAddr);
////			m_pCenterSocket->Connect(dwServerIP,PORT_CENTER_SERVER);
////			return true;
////		}
////	case IDI_UPDATE_SERVER_LIST:		//更新服务器列表
////		{
////			if (m_SocketState==SocketState_Connected)
////			{
////				m_pCenterSocket->SendData(MDM_CS_SERVER_LIST,SUB_CS_GET_SERVER_LIST,NULL,0);
////			}
////			return true;
////		}
////	}
////	return false;
////}
//
////数据库事件
//bool CMsgTriggerServiceSink::OnEventDataBase(void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent)
//{
//	switch (pDataBaseEvent->wRequestID)
//	{
//	case DBR_GP_LOGON_SUCCESS:		//登录成功
//		{
//			return OnDBLogonSuccess(pDataBuffer,wDataSize,pDataBaseEvent);
//		}
//	case DBR_GP_LOGON_ERROR:		//登录失败
//		{
//			return OnDBLogonError(pDataBuffer,wDataSize,pDataBaseEvent);
//		}
//	}
//
//	return false;
//}
//
////用户登录成功
//bool CMsgTriggerServiceSink::OnDBLogonSuccess(void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent)
//{
//	//效验参数
//	if (wDataSize!=sizeof(DBR_GP_LogonSuccess)) return false;
//
//	//判断在线
//
//	tagConnectItemInfo * pConnectItemInfo=(m_pConnectItemInfo+pDataBaseEvent->wIndex);
//	if (pConnectItemInfo->bConnect==FALSE) return true;
//	if (pConnectItemInfo->wRoundID!=pDataBaseEvent->wRoundID) return true;
//
//	//构造数据
//	BYTE cbBuffer[SOCKET_PACKAGE];
//	CMD_GP_LogonSuccess * pCMDLogonSuccess=(CMD_GP_LogonSuccess *)cbBuffer;
//	DBR_GP_LogonSuccess * pDBRLogonSuccess=(DBR_GP_LogonSuccess *)pDataBuffer;
//	pCMDLogonSuccess->wFaceID=pDBRLogonSuccess->wFaceID;
//	pCMDLogonSuccess->cbGender=pDBRLogonSuccess->cbGender;
//	pCMDLogonSuccess->cbMember=pDBRLogonSuccess->cbMember;
//	pCMDLogonSuccess->dwUserID=pDBRLogonSuccess->dwUserID;
//	pCMDLogonSuccess->dwGameID=pDBRLogonSuccess->dwGameID;
//	pCMDLogonSuccess->dwExperience=pDBRLogonSuccess->dwExperience;
//
//	//叠加数据
//	CSendPacketHelper SendPacket(cbBuffer+sizeof(CMD_GP_LogonSuccess),sizeof(cbBuffer)-sizeof(CMD_GP_LogonSuccess));
//	SendPacket.AddPacket(pDBRLogonSuccess->szPassword,CountString(pDBRLogonSuccess->szPassword),DTP_USER_PASS);
//	SendPacket.AddPacket(pDBRLogonSuccess->szAccounts,CountString(pDBRLogonSuccess->szAccounts),DTP_USER_ACCOUNTS);
//	SendPacket.AddPacket(pDBRLogonSuccess->szUnderWrite,CountString(pDBRLogonSuccess->szUnderWrite),DTP_UNDER_WRITE);
//
//	//社团信息
//	if (pDBRLogonSuccess->szGroupName[0]!=0)
//	{
//		SendPacket.AddPacket(pDBRLogonSuccess->szGroupName,CountString(pDBRLogonSuccess->szGroupName),DTP_USER_GROUP_NAME);
//	}
//	
//	//站点主页
//	if (m_pInitParamter->m_szMainPage[0]!=0)
//	{
//		SendPacket.AddPacket(m_pInitParamter->m_szMainPage,CountString(m_pInitParamter->m_szMainPage),DTP_STATION_PAGE);
//	}
//
//	//发送登录结果
//	WORD wIndex=pDataBaseEvent->wIndex;
//	WORD wRoundID=pDataBaseEvent->wRoundID;
//	WORD wSendSize=sizeof(CMD_GP_LogonSuccess)+SendPacket.GetDataSize();
//	m_pITCPSocketService->SendData(wIndex,wRoundID,MDM_GP_LOGON,SUB_GP_LOGON_SUCCESS,cbBuffer,wSendSize);
//
//	//列表配置
//	CMD_GP_ListConfig ListConfig;
//	ListConfig.bShowOnLineCount=TRUE;
//	m_pITCPSocketService->SendData(wIndex,wRoundID,MDM_GP_SERVER_LIST,SUB_GP_LIST_CONFIG,&ListConfig,sizeof(ListConfig));
//
//	//发送游戏列表
//	m_ServerList.SendGameTypeList(wIndex,wRoundID);
//	m_ServerList.SendGameKindList(wIndex,wRoundID);
//	m_ServerList.SendGameStationList(wIndex,wRoundID);
//	m_ServerList.SendGameServerList(wIndex,wRoundID);
//
//	//发送登录完成
//	m_pITCPSocketService->SendData(wIndex,wRoundID,MDM_GP_LOGON,SUB_GP_LOGON_FINISH);
//
//	//关闭连接
//	m_pITCPSocketService->ShutDownSocket(wIndex,wRoundID);
//
//	return true;
//}
//
////用户登录失败
//bool CMsgTriggerServiceSink::OnDBLogonError(void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent)
//{
//	//效验参数
//
//	//判断在线
//	tagConnectItemInfo * pConnectItemInfo=(m_pConnectItemInfo+pDataBaseEvent->wIndex);
//	if (pConnectItemInfo->bConnect==FALSE) return true;
//	if (pConnectItemInfo->wRoundID!=pDataBaseEvent->wRoundID) return true;
//
//	//变量定义
//	CMD_GP_LogonError LogonError;
//	DBR_GP_LogonError * pLogonError=(DBR_GP_LogonError *)pDataBuffer;
//	pLogonError->szErrorDescribe[CountString(pLogonError->szErrorDescribe)-1]=0;
//
//	//构造数据
//	LogonError.lErrorCode=pLogonError->lErrorCode;
//	lstrcpyn(LogonError.szErrorDescribe,pLogonError->szErrorDescribe,sizeof(LogonError.szErrorDescribe));
//	WORD wDescribeSize=CountString(LogonError.szErrorDescribe);
//
//	//发送数据
//	WORD wSendSize=sizeof(LogonError)-sizeof(LogonError.szErrorDescribe)+wDescribeSize;
//	m_pITCPSocketService->SendData(pDataBaseEvent->wIndex,pDataBaseEvent->wRoundID,MDM_GP_LOGON,
//		SUB_GP_LOGON_ERROR,&LogonError,wSendSize);
//
//	//关闭连接
//	m_pITCPSocketService->ShutDownSocket(pDataBaseEvent->wIndex,pDataBaseEvent->wRoundID);
//
//	return true;
//}
//
////网络应答事件
//bool __cdecl CMsgTriggerServiceSink::OnEventSocketAccept(NTY_SocketAcceptEvent * pSocketAcceptEvent)
//{
//	//效验参数
//
//	//设置数据
//	tagConnectItemInfo * pConnectItemInfo=m_pConnectItemInfo+pSocketAcceptEvent->wIndex;
//	pConnectItemInfo->bConnect=TRUE;
//	pConnectItemInfo->wRoundID=pSocketAcceptEvent->wRoundID;
//	pConnectItemInfo->dwClientIP=pSocketAcceptEvent->dwClientIP;
//
//	return true;
//}
//
////网络读取事件
//bool __cdecl CMsgTriggerServiceSink::OnEventSocketRead(CMD_Command Command, 
//													void * pDataBuffer, 
//													WORD wDataSize, 
//													NTY_SocketReadEvent * pSocketReadEvent)
//{
//	switch (Command.wMainCmdID) 
//	{
//	//登录消息
//	case MDM_GP_LOGON:				
//		{
//			return OnSocketMainLogon(Command,pDataBuffer,wDataSize,pSocketReadEvent);
//		}
//	}
//
//	return false;
//}
//
////网络关闭事件
//bool __cdecl CMsgTriggerServiceSink::OnEventSocketClose(NTY_SocketCloseEvent * pSocketCloseEvent)
//{
//	//效验参数
//	ASSERT(pSocketCloseEvent->wIndex<m_pInitParamter->m_wMaxConnect);
//
//	//设置数据
//	tagConnectItemInfo * pConnectItemInfo=m_pConnectItemInfo+pSocketCloseEvent->wIndex;
//	pConnectItemInfo->wRoundID++;
//	pConnectItemInfo->dwClientIP=0;
//	pConnectItemInfo->bConnect=FALSE;
//
//	return true;
//}
//
////中心连接事件
//bool __cdecl CMsgTriggerServiceSink::OnEventCenterSocketConnect(int iErrorCode)
//{
//	//错误判断
//	if (iErrorCode!=0)
//	{
//		m_SocketState=SocketState_NoConnect;
//		m_pITimerEngine->SetTimer(IDI_CONNECT_CENTER_SERVER,TIME_RECONNECT,1,NULL);
//		return true;
//	}
//
//	//设置变量
//	m_SocketState=SocketState_Connected;
//
//	//获取列表
//	m_pCenterSocket->SendData(MDM_CS_SERVER_LIST,SUB_CS_GET_SERVER_LIST,NULL,0);
//
//	//设置定时器
//	m_pITimerEngine->SetTimer(IDI_UPDATE_SERVER_LIST,TIME_UPDATE_LIST,TIMER_REPEAT_TIMER,NULL);
//
//	return true;
//}
//
////中心读取事件
//bool __cdecl CMsgTriggerServiceSink::OnEventCenterSocketRead(CMD_Command Command, void * pDataBuffer, WORD wDataSize)
//{
//	switch (Command.wMainCmdID)
//	{
//	case MDM_CS_SERVER_LIST:	//列表消息
//		{
//			return OnCenterMainServerList(Command,pDataBuffer,wDataSize);
//		}
//	}
//	return true;
//}
//
////中心关闭事件
//bool __cdecl CMsgTriggerServiceSink::OnEventCenterSocketClose(bool bCloseByServer)
//{
//	////设置变量
//	//m_SocketState=SocketState_NoConnect;
//
//	////重新连接
//	//if (bCloseByServer==true) m_pITimerEngine->SetTimer(IDI_CONNECT_CENTER_SERVER,TIME_RECONNECT,1,NULL);
//
//	return true;
//}
//
////登录消息处理
//bool CMsgTriggerServiceSink::OnSocketMainLogon(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent)
//{
//	switch (Command.wSubCmdID)
//	{
//	case SUB_GP_LOGON_ACCOUNTS:		//帐号登录
//		{
//			//效验参数
////			ASSERT(wDataSize>=sizeof(CMD_GP_LogonByAccounts));
//			if (wDataSize<sizeof(CMD_GP_LogonByAccounts)) return false;
//
//			//处理消息
//			CMD_GP_LogonByAccounts * pLogonByAccounts=(CMD_GP_LogonByAccounts *)pDataBuffer;
//			pLogonByAccounts->szAccounts[CountArray(pLogonByAccounts->szAccounts)-1]=0;
//			pLogonByAccounts->szPassWord[CountArray(pLogonByAccounts->szPassWord)-1]=0;
//
//			//连接信息
////			ASSERT(pSocketReadEvent->wIndex<m_pInitParamter->m_wMaxConnect);
//			DWORD dwClientAddr=(m_pConnectItemInfo+pSocketReadEvent->wIndex)->dwClientIP;
//
//			//效验版本
//			if (pLogonByAccounts->dwPlazaVersion!=VER_PLAZA_FRAME)
//			{
//				//获取版本
//				WORD wLow=LOWORD(pLogonByAccounts->dwPlazaVersion);
//				WORD wHigh=HIWORD(pLogonByAccounts->dwPlazaVersion);
//
//				//构造数据
//				CMD_GP_Version Version;
//				Version.bNewVersion=TRUE;
//				Version.bAllowConnect=(wHigh==VER_PLAZA_HIGH)?TRUE:FALSE;
//
//				//发送数据
//				WORD wIndex=pSocketReadEvent->wIndex;
//				WORD wRountID=pSocketReadEvent->wRoundID;
//				m_pITCPSocketService->SendData(wIndex,wRountID,MDM_GP_SYSTEM,SUB_GP_VERSION,&Version,sizeof(Version));
//
//				//判断关闭
//				if (Version.bAllowConnect==FALSE)
//				{
//					return true;
//				}
//			}
//
//			//构造数据
//			DBR_GP_LogonByAccounts LogonByAccounts;
//			ZeroMemory(&LogonByAccounts,sizeof(LogonByAccounts));
//
//			//设置变量
//			LogonByAccounts.dwClientIP=dwClientAddr;
//			lstrcpyn(LogonByAccounts.szAccounts,pLogonByAccounts->szAccounts,sizeof(LogonByAccounts.szAccounts));
//			lstrcpyn(LogonByAccounts.szPassWord,pLogonByAccounts->szPassWord,sizeof(LogonByAccounts.szPassWord));
//
//			//变量定义
//			VOID * pDataBuffer=NULL;
//			tagDataDescribe DataDescribe;
//			CRecvPacketHelper RecvPacket(pLogonByAccounts+1,wDataSize-sizeof(CMD_GP_LogonByAccounts));
//
//			//扩展信息
//			while (true)
//			{
//				//提取数据
//				pDataBuffer=RecvPacket.GetData(DataDescribe);
//				if (DataDescribe.wDataDescribe==DTP_NULL) break;
//
//				//数据分析
//				switch (DataDescribe.wDataDescribe)
//				{
//				case DTP_COMPUTER_ID:		//机器标识
//					{
//						ASSERT(pDataBuffer!=NULL);
//						ASSERT(DataDescribe.wDataSize==sizeof(tagClientSerial));
//						if (DataDescribe.wDataSize==sizeof(tagClientSerial))
//						{
//							BYTE * pcbByte=(BYTE *)pDataBuffer;
//							for (INT i=0;i<sizeof(tagClientSerial)/sizeof(BYTE);i++)
//							{
//								ASSERT(CountArray(LogonByAccounts.szComputerID)>((i*2)+1));
//								_stprintf(&LogonByAccounts.szComputerID[i*2],TEXT("%02X"),*(pcbByte+i));
//							}
//						}
//
//						break;
//					}
//				}
//			}
//
//			//投递数据库 重要的
//			m_DataBaseEvent.PostDataBaseEvent(DBR_GP_LOGON_BY_ACCOUNTS,pSocketReadEvent->wIndex,
//				pSocketReadEvent->wRoundID,&LogonByAccounts,sizeof(LogonByAccounts));
//
//			return true;
//		}
//	case SUB_GP_LOGON_USERID:		//ID 登录
//		{
//			//效验参数
//			if (wDataSize<sizeof(CMD_GP_LogonByUserID)) return false;
//
//			//处理消息
//			CMD_GP_LogonByUserID * pLogonByUserID=(CMD_GP_LogonByUserID *)pDataBuffer;
//			pLogonByUserID->szPassWord[CountArray(pLogonByUserID->szPassWord)-1]=0;
//
//			//连接信息
//
//			DWORD dwClientAddr=(m_pConnectItemInfo+pSocketReadEvent->wIndex)->dwClientIP;
//
//			//效验版本
//			if (pLogonByUserID->dwPlazaVersion!=VER_PLAZA_FRAME)
//			{
//				//获取版本
//				WORD wLow=LOWORD(pLogonByUserID->dwPlazaVersion);
//				WORD wHigh=HIWORD(pLogonByUserID->dwPlazaVersion);
//
//				//构造数据
//				CMD_GP_Version Version;
//				Version.bNewVersion=TRUE;
//				Version.bAllowConnect=(wHigh==VER_PLAZA_HIGH)?TRUE:FALSE;
//
//				//发送数据
//				WORD wIndex=pSocketReadEvent->wIndex;
//				WORD wRountID=pSocketReadEvent->wRoundID;
//				m_pITCPSocketService->SendData(wIndex,wRountID,MDM_GP_SYSTEM,SUB_GP_VERSION,&Version,sizeof(Version));
//
//				//判断关闭
//				if (Version.bAllowConnect==FALSE)
//				{
//					return true;
//				}
//			}
//
//			//构造数据
//			DBR_GP_LogonByUserID LogonByUserID;
//			memset(&LogonByUserID,0,sizeof(LogonByUserID));
//			LogonByUserID.dwClientIP=dwClientAddr;
//			LogonByUserID.dwUserID=pLogonByUserID->dwUserID;
//			lstrcpyn(LogonByUserID.szPassWord,pLogonByUserID->szPassWord,sizeof(LogonByUserID.szPassWord));
//
//			//变量定义
//			VOID * pDataBuffer=NULL;
//			tagDataDescribe DataDescribe;
//			CRecvPacketHelper RecvPacket(pLogonByUserID+1,wDataSize-sizeof(CMD_GP_LogonByUserID));
//
//			//扩展信息
//			while (true)
//			{
//				//提取数据
//				pDataBuffer=RecvPacket.GetData(DataDescribe);
//				if (DataDescribe.wDataDescribe==DTP_NULL) break;
//
//				//数据分析
//				switch (DataDescribe.wDataDescribe)
//				{
//				case DTP_COMPUTER_ID:		//机器标识
//					{
//						ASSERT(pDataBuffer!=NULL);
//						ASSERT(DataDescribe.wDataSize==sizeof(tagClientSerial));
//						if (DataDescribe.wDataSize==sizeof(tagClientSerial))
//						{
//							BYTE * pcbByte=(BYTE *)pDataBuffer;
//							for (INT i=0;i<sizeof(tagClientSerial)/sizeof(BYTE);i++)
//							{
//								ASSERT(CountArray(LogonByUserID.szComputerID)>((i*2)+1));
//								_stprintf(&LogonByUserID.szComputerID[i*2],TEXT("%02X"),*(pcbByte+i));
//							}
//						}
//
//						break;
//					}
//				}
//			}
//
//			//投递数据库
//			m_DataBaseEvent.PostDataBaseEvent(DBR_GP_LOGON_BY_USERID,pSocketReadEvent->wIndex,
//				pSocketReadEvent->wRoundID,&LogonByUserID,sizeof(LogonByUserID));
//
//			return true;
//		}
//	case SUB_GP_REGISTER_ACCOUNTS:		//注册帐号
//		{
//			//效验参数
//			if (wDataSize<sizeof(CMD_GP_RegisterAccounts)) return false;
//
//			//处理消息
//			CMD_GP_RegisterAccounts * pRegisterAccounts=(CMD_GP_RegisterAccounts *)pDataBuffer;
//			pRegisterAccounts->szAccounts[CountArray(pRegisterAccounts->szAccounts)-1]=0;
//			pRegisterAccounts->szPassWord[CountArray(pRegisterAccounts->szPassWord)-1]=0;
//
//			//连接信息
//			//ASSERT(pSocketReadEvent->wIndex<m_pInitParamter->m_wMaxConnect);
//			DWORD dwClientAddr=(m_pConnectItemInfo+pSocketReadEvent->wIndex)->dwClientIP;
//
//			//效验版本
//			if (pRegisterAccounts->dwPlazaVersion!=VER_PLAZA_FRAME)
//			{
//				//获取版本
//				WORD wLow=LOWORD(pRegisterAccounts->dwPlazaVersion);
//				WORD wHigh=HIWORD(pRegisterAccounts->dwPlazaVersion);
//
//				//构造数据
//				CMD_GP_Version Version;
//				Version.bNewVersion=TRUE;
//				Version.bAllowConnect=(wHigh==VER_PLAZA_HIGH)?TRUE:FALSE;
//
//				//发送数据
//				WORD wIndex=pSocketReadEvent->wIndex;
//				WORD wRountID=pSocketReadEvent->wRoundID;
//				m_pITCPSocketService->SendData(wIndex,wRountID,MDM_GP_SYSTEM,SUB_GP_VERSION,&Version,sizeof(Version));
//
//				//判断关闭
//				if (Version.bAllowConnect==FALSE)
//				{
//					return true;
//				}
//			}
//
//			//构造数据
//			DBR_GP_RegisterAccounts RegisterAccounts;
//			memset(&RegisterAccounts,0,sizeof(RegisterAccounts));
//			RegisterAccounts.dwClientIP=dwClientAddr;
//			RegisterAccounts.wFaceID=pRegisterAccounts->wFaceID;
//			RegisterAccounts.cbGender=pRegisterAccounts->cbGender;
//			lstrcpyn(RegisterAccounts.szSpreader,pRegisterAccounts->szSpreader,sizeof(RegisterAccounts.szSpreader));
//			lstrcpyn(RegisterAccounts.szAccounts,pRegisterAccounts->szAccounts,sizeof(RegisterAccounts.szAccounts));
//			lstrcpyn(RegisterAccounts.szPassWord,pRegisterAccounts->szPassWord,sizeof(RegisterAccounts.szPassWord));
//
//			//变量定义
//			VOID * pDataBuffer=NULL;
//			tagDataDescribe DataDescribe;
//			CRecvPacketHelper RecvPacket(pRegisterAccounts+1,wDataSize-sizeof(CMD_GP_RegisterAccounts));
//
//			//扩展信息
//			while (true)
//			{
//				//提取数据
//				pDataBuffer=RecvPacket.GetData(DataDescribe);
//				if (DataDescribe.wDataDescribe==DTP_NULL) break;
//
//				//数据分析
//				switch (DataDescribe.wDataDescribe)
//				{
//				case DTP_COMPUTER_ID:		//机器标识
//					{
//						ASSERT(pDataBuffer!=NULL);
//						ASSERT(DataDescribe.wDataSize==sizeof(tagClientSerial));
//						if (DataDescribe.wDataSize==sizeof(tagClientSerial))
//						{
//							BYTE * pcbByte=(BYTE *)pDataBuffer;
//							for (INT i=0;i<sizeof(tagClientSerial)/sizeof(BYTE);i++)
//							{
//								ASSERT(CountArray(RegisterAccounts.szComputerID)>((i*2)+1));
//								_stprintf(&RegisterAccounts.szComputerID[i*2],TEXT("%02X"),*(pcbByte+i));
//							}
//						}
//
//						break;
//					}
//				}
//			}
//
//			//投递数据库
//			m_DataBaseEvent.PostDataBaseEvent(DBR_GP_REGISTER_ACCOUNTS,pSocketReadEvent->wIndex,
//				pSocketReadEvent->wRoundID,&RegisterAccounts,sizeof(RegisterAccounts));
//
//			return true;
//		}
//	}
//	return false;
//}
//
////列表消息处理
//bool CMsgTriggerServiceSink::OnCenterMainServerList(CMD_Command Command, void * pDataBuffer, WORD wDataSize)
//{
//	//ASSERT(Command.wMainCmdID==MDM_CS_SERVER_LIST);
//	switch (Command.wSubCmdID)
//	{
//	case SUB_CS_LIST_INFO:		//列表信息
//		{
//			//效验参数
//			//ASSERT(wDataSize==sizeof(CMD_CS_ListInfo));
//			if (wDataSize!=sizeof(CMD_CS_ListInfo)) return false;
//
//			//消息处理
//			m_ServerList.ResetServerListBuffer();
//
//			return true;
//		}
//	case SUB_CS_LIST_TYPE:		//种类信息
//		{
//			//效验参数
//			//ASSERT(wDataSize>=sizeof(tagGameType));
//			//ASSERT(wDataSize%sizeof(tagGameType)==0);
//
//			//消息处理
//			DWORD dwCount=wDataSize/sizeof(tagGameType);
//			tagGameType * pGameType=(tagGameType *)pDataBuffer;
//			m_ServerList.AppendGameTypeBuffer(pGameType,dwCount);
//
//			return true;
//		}
//	case SUB_CS_LIST_KIND:		//类型信息
//		{
//			//效验参数
//			//ASSERT(wDataSize>=sizeof(tagGameKind));
//			//ASSERT(wDataSize%sizeof(tagGameKind)==0);
//
//			//消息处理
//			DWORD dwCount=wDataSize/sizeof(tagGameKind);
//			tagGameKind * pGameKind=(tagGameKind *)pDataBuffer;
//			m_ServerList.AppendGameKindBuffer(pGameKind,dwCount);
//
//			return true;
//		}
//	//case SUB_CS_LIST_STATION:	//站点信息
//	//	{
//	//		//效验参数
//	//		//消息处理
//	//		DWORD dwCount=wDataSize/sizeof(tagGameStation);
//	//		tagGameStation * pGameStation=(tagGameStation *)pDataBuffer;
//	//		m_ServerList.AppendGameStationBuffer(pGameStation,dwCount);
//
//	//		return true;
//	//	}
//	case SUB_CS_LIST_SERVER:	//房间信息
//		{
//			//效验参数
//			//ASSERT(wDataSize>=sizeof(tagGameServer));
//			//ASSERT(wDataSize%sizeof(tagGameServer)==0);
//
//			//消息处理
//			DWORD dwCount=wDataSize/sizeof(tagGameServer);
//			tagGameServer * pGameServer=(tagGameServer *)pDataBuffer;
//			m_ServerList.AppendGameServerBuffer(pGameServer,dwCount);
//
//			return true;
//		}
//	case SUB_CS_LIST_FINISH:	//更新完成
//		{
//			//消息处理
//			m_ServerList.ActiveServerListBuffer();
//
//			return true;
//		}
//	}
//
//	return true;
//}