#include "StdAfx.h"
#include "MsgTriggerServiceSink.h"
#include "CenterServer.h"

CMsgTriggerServiceSink::CMsgTriggerServiceSink(void)
{
	m_pITCPSocketService = NULL;
}


CMsgTriggerServiceSink::~CMsgTriggerServiceSink(void)
{
}

	//调度模块启动
bool __cdecl CMsgTriggerServiceSink::StartService(CObject * pICObject)
{
	//效验参数
	if(pICObject == NULL)	
		return false;
	return true;
}
	//调度模块关闭
bool __cdecl CMsgTriggerServiceSink::EndService(CObject * pICObject)
{
	//重置列表
	if (m_ServerListCenter.GetGameServerCount()) 
	{
		m_ServerListCenter.ResetServerList();
	}
	return true;
}
	//事件处理接口
bool __cdecl CMsgTriggerServiceSink::OnMsgTrigger(WORD wIdentifier, void * pDataBuffer, WORD wDataSize, DWORD dwInsertTime)
{
	return false;
}

//配置函数
bool CMsgTriggerServiceSink::InitMsgTriggerSink(CKernelService * pICObject)
{
	if(pICObject == NULL)
		return false;
	if(pICObject->GetTCPSocketService(&m_pITCPSocketService) == false)
		return false;
	IDataBaseService *pIDataBaseService;
	if(pICObject->GetDataBaseService(&pIDataBaseService) == false)
		return false;
	IDataQueueService *pIDataQueueService;
	if(pIDataBaseService->GetQueueService(&pIDataQueueService) == false)
		return false;
	IDataQueueSink *pIDataQueueSink = dynamic_cast<IDataQueueSink *>(pIDataQueueService);
	if(m_DataBaseEvent.SetQueueService(pIDataQueueSink) == false)
		return false;
	return true;
}

//数据库事件
bool __cdecl CMsgTriggerServiceSink::OnEventDataBase(void * pDataBuffer, WORD wDataSize, NTY_DataBaseEvent * pDataBaseEvent)
{
	return false;
}
//网络应答事件
bool __cdecl CMsgTriggerServiceSink::OnEventSocketAccept(NTY_SocketAcceptEvent * pSocketAcceptEvent)
{
	return true;;
}
//网络读取事件
bool __cdecl CMsgTriggerServiceSink::OnEventSocketRead(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent)
{
	switch (Command.wMainCmdID)
	{
	//列表消息
	case MDM_CS_SERVER_LIST:			
		{
			return OnSocketMainServerList(Command,
									pDataBuffer,
									wDataSize,
									pSocketReadEvent);
		}

	//房间管理
	case MDM_CS_SERVER_MANAGER:			
		{
			return OnSocketMainServerManager(Command,
									pDataBuffer,
									wDataSize,
									pSocketReadEvent);
		}
	}

	return false;
}
//网络关闭事件
bool __cdecl CMsgTriggerServiceSink::OnEventSocketClose(NTY_SocketCloseEvent * pSocketCloseEvent)
{
	return true;
}

//列表消息处理
bool CMsgTriggerServiceSink::OnSocketMainServerList(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent)
{
	//ASSERT(Command.wMainCmdID==MDM_CS_SERVER_LIST);
	switch (Command.wSubCmdID)
	{
	//获取列表
	case SUB_CS_GET_SERVER_LIST:	
		{
			//变量定义
			WORD wIndex=pSocketReadEvent->wIndex;
			WORD wRountID=pSocketReadEvent->wRoundID;

			//列表信息
			CMD_CS_ListInfo ListInfo;
			memset(&ListInfo,0,sizeof(ListInfo));
			//ListInfo.dwTypeCount=m_ServerListCenter->GetGameTypeCount();
			//ListInfo.dwKindCount=m_ServerListCenter->GetGameKindCount();
			ListInfo.dwServerCount=m_ServerListCenter.GetGameServerCount();
			//ListInfo.dwStationCount=m_ServerListCenter->GetGameStationCount();
			m_pITCPSocketService->SendData(wIndex,wRountID,MDM_CS_SERVER_LIST,SUB_CS_LIST_INFO,&ListInfo,sizeof(ListInfo));

			//类型信息
			int Pos=0;
			WORD wSendSize=0L;
			BYTE cbBuffer[SOCKET_PACKAGE];
			//for (DWORD i=0;i<ListInfo.dwTypeCount;i++)
			//{
			//	if ((wSendSize+sizeof(tagGameType))>sizeof(cbBuffer))
			//	{
			//		m_pITCPSocketEngine->SendData(wIndex,wRountID,MDM_CS_SERVER_LIST,SUB_CS_LIST_TYPE,cbBuffer,wSendSize);
			//		wSendSize=0;
			//	}
			//	tagGameTypeItem * pGameTypeItem=m_ServerListCenter->EmunGameTypeItem(Pos);
			//	CopyMemory(cbBuffer+wSendSize,&pGameTypeItem->GameType,sizeof(tagGameType));
			//	wSendSize+=sizeof(tagGameType);
			//}
			//if (wSendSize>0) m_pITCPSocketEngine->SendData(wIndex,wRountID,MDM_CS_SERVER_LIST,SUB_CS_LIST_TYPE,cbBuffer,wSendSize);

			//种类信息
			//Pos=NULL;
			//wSendSize=0L;
			//for (DWORD i=0;i<ListInfo.dwKindCount;i++)
			//{
			//	if ((wSendSize+sizeof(tagGameKind))>sizeof(cbBuffer))
			//	{
			//		m_pITCPSocketEngine->SendData(wIndex,wRountID,MDM_CS_SERVER_LIST,SUB_CS_LIST_KIND,cbBuffer,wSendSize);
			//		wSendSize=0;
			//	}
			//	tagGameKindItem * pGameKindItem=m_ServerListCenter->EmunGameKindItem(Pos);
			//	CopyMemory(cbBuffer+wSendSize,&pGameKindItem->GameKind,sizeof(tagGameKind));
			//	wSendSize+=sizeof(tagGameKind);
			//}
			//if (wSendSize>0) m_pITCPSocketEngine->SendData(wIndex,wRountID,MDM_CS_SERVER_LIST,SUB_CS_LIST_KIND,cbBuffer,wSendSize);

			//站点信息
			//Pos=NULL;
			//wSendSize=0L;
			//for (DWORD i=0;i<ListInfo.dwStationCount;i++)
			//{
			//	if ((wSendSize+sizeof(tagGameStation))>sizeof(cbBuffer))
			//	{
			//		m_pITCPSocketEngine->SendData(wIndex,wRountID,MDM_CS_SERVER_LIST,SUB_CS_LIST_STATION,cbBuffer,wSendSize);
			//		wSendSize=0;
			//	}
			//	tagGameStationItem * pGameStationItem=m_ServerListCenter->EmunGameStationItem(Pos);
			//	CopyMemory(cbBuffer+wSendSize,&pGameStationItem->GameStation,sizeof(tagGameStation));
			//	wSendSize+=sizeof(tagGameStation);
			//}
			//if (wSendSize>0) m_pITCPSocketEngine->SendData(wIndex,wRountID,MDM_CS_SERVER_LIST,SUB_CS_LIST_STATION,cbBuffer,wSendSize);

			//房间信息
			Pos=NULL;
			wSendSize=0L;
			for (DWORD i=0;i<ListInfo.dwServerCount;i++)
			{
				if ((wSendSize+sizeof(tagGameServer))>sizeof(cbBuffer))
				{
					m_pITCPSocketService->SendData(wIndex,wRountID,MDM_CS_SERVER_LIST,SUB_CS_LIST_SERVER,cbBuffer,wSendSize);
					wSendSize=0;
				}
				tagGameServer * pGameServerItem=m_ServerListCenter.EmunGameServerItem(Pos);
				CopyMemory(cbBuffer+wSendSize,&pGameServerItem,sizeof(tagGameServer));
				wSendSize+=sizeof(tagGameServer);
			}
			if (wSendSize>0) m_pITCPSocketService->SendData(wIndex,wRountID,MDM_CS_SERVER_LIST,SUB_CS_LIST_SERVER,cbBuffer,wSendSize);

			//发送完成
			m_pITCPSocketService->SendData(wIndex,wRountID,MDM_CS_SERVER_LIST,SUB_CS_LIST_FINISH);

			return true;
		}
	}

	return false;
}
//房间消息处理
bool CMsgTriggerServiceSink::OnSocketMainServerManager(CMD_Command Command, void * pDataBuffer, WORD wDataSize, NTY_SocketReadEvent * pSocketReadEvent)
{
	//ASSERT(Command.wMainCmdID==MDM_CS_SERVER_MANAGER);
	switch (Command.wSubCmdID)
	{
	//房间注册
	case SUB_CS_REG_GAME_SERVER:		
		{
			//效验参数
			//ASSERT(wDataSize==sizeof(CMD_CS_RegGameServer));
			if (wDataSize!=sizeof(CMD_CS_RegGameServer)) return false;

			//消息处理
			CMD_CS_RegGameServer * pRegGameServer=(CMD_CS_RegGameServer *)pDataBuffer;
			tagGameServer * pGameServer=&pRegGameServer->GameServer;
			tagGameServer * pGameServerItem=m_ServerListCenter.SearchGameServer(pGameServer->wKindID,pGameServer->wServerID);
			if (pGameServerItem!=NULL)
			{
				//pGameServerItem->dwUpdateTime=(DWORD)time(NULL);
				CopyMemory(&pGameServerItem,pGameServer,sizeof(tagGameServer));
			}
			else m_ServerListCenter.InsertGameServer(pGameServer);

			return true;
		}

	//注销房间
	case SUB_CS_UNREG_GAME_SERVER:		
		{
			//效验参数
			//ASSERT(wDataSize==sizeof(CMD_CS_UnRegGameServer));
			if (wDataSize!=sizeof(CMD_CS_UnRegGameServer)) return false;

			//消息处理
			CMD_CS_UnRegGameServer * pUnRegGameServer=(CMD_CS_UnRegGameServer *)pDataBuffer;
			m_ServerListCenter.DeleteGameServer(pUnRegGameServer->wKindID,pUnRegGameServer->wServerID);

			return true;
		}

	//更新人数
	case SUB_CS_SERVER_ONLINE_COUNT:	
		{
			//效验参数
			//ASSERT(wDataSize==sizeof(CMD_CS_ServerOnLineCount));
			if (wDataSize!=sizeof(CMD_CS_ServerOnLineCount)) return false;

			//消息处理
			CMD_CS_ServerOnLineCount * pServerOnLineCount=(CMD_CS_ServerOnLineCount *)pDataBuffer;
			WORD wKindID=pServerOnLineCount->wKindID;
			WORD wServerID=pServerOnLineCount->wServerID;
			m_ServerListCenter.UpdateServerOnLineCount(wKindID,wServerID,pServerOnLineCount->dwOnLineCount);

			return true;
		}
	}

	return false;
}
