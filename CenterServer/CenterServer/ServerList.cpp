#include "StdAfx.h"
#include "ServerList.h"
#include "CenterServer.h"

CServerList::CServerList(void)
{
}


CServerList::~CServerList(void)
{						
	m_ServerItemMap.clear();						
	//m_ServerItemStore.RemoveAll();						
}
//插入房间
bool __cdecl CServerList::InsertGameServer(tagGameServer * pGameServer)
{
	//效验参数
	if (pGameServer==NULL) return false;

		//查找现存
	tagGameServer * pGameServerItem=NULL;
	DWORD dwKey=MAKELONG(pGameServer->wKindID,pGameServer->wServerID);
	if (m_ServerItemMap[dwKey] != NULL) return false;
	try
	{
		pGameServerItem=new tagGameServer;
		if (pGameServerItem==NULL) return false;
		//插入数据
		CopyMemory(&pGameServerItem,pGameServer,sizeof(tagGameServer));
		m_ServerItemMap[dwKey]= pGameServerItem;

		CString szBuffer;
		szBuffer.Format("%s  KindID：%ld ServerID：%ld注册成功",
			pGameServer->szServerName,pGameServer->wKindID,pGameServer->wServerID);
		TraceEventString(szBuffer,Level_Normal);

		//更新人数
		UpdateKindOnLineCount(pGameServer->wKindID);

		return true;
	}
	catch (...) {}

	return false;
}
//删除房间
bool __cdecl CServerList::DeleteGameServer(WORD wKindID, WORD wServerID)
{
	//查找房间
	tagGameServer * pGameServerItem=NULL;
	DWORD dwKey=MAKELONG(wKindID,wServerID);
	if (m_ServerItemMap[dwKey]==NULL) return false;

	//删除数据
	delete m_ServerItemMap[dwKey];
	m_ServerItemMap.erase(dwKey);
	//m_ServerItemStore.Add(pGameServerItem);

	//更新人数
	UpdateKindOnLineCount(wKindID);

	return false;
}

//枚举房间
tagGameServer * const __cdecl CServerList::EmunGameServerItem(int & Pos)
{
	//调整参数
	if (Pos < 0)
		return NULL;

	if (Pos==0) return NULL;

	//查找房间
	map<DWORD,tagGameServer *>::const_iterator ptr = m_ServerItemMap.begin();
	for(int i=0;i<m_ServerItemMap.size();i++)
	{	ptr++;
		if(i == Pos)
			return ptr->second;
	}
	return NULL;
}
//查找房间
tagGameServer * const __cdecl CServerList::SearchGameServer(WORD wKindID, WORD wServerID)
{
	tagGameServer * pGameServerItem=NULL;
	DWORD dwKey=MAKELONG(wKindID,wServerID);
	pGameServerItem = m_ServerItemMap[dwKey];
	return pGameServerItem;
}

//重置列表
void __cdecl CServerList::ResetServerList()
{
	//删除房间
	for(map<DWORD,tagGameServer *>::iterator ptr = m_ServerItemMap.begin();
		ptr != m_ServerItemMap.end();ptr++)
	{
		delete ptr->second;
	}
	m_ServerItemMap.clear();
}
//加载列表
bool __cdecl CServerList::LoadServerList()
{
	return true;
}
//更新人数
bool __cdecl CServerList::UpdateServerOnLineCount(WORD wKindID, WORD wServerID, DWORD dwOnLineCount)
{
	tagGameServer * pGameServerItem=SearchGameServer(wKindID,wServerID);
	if (pGameServerItem!=NULL)
	{
		//更新房间
		//pGameServerItem->dwUpdateTime=(DWORD)time(NULL);
		pGameServerItem->dwOnLineCount=dwOnLineCount;

		//更新人数
		UpdateKindOnLineCount(wKindID);

		return true;
	}

	return false;
}


//更新人数
void CServerList::UpdateKindOnLineCount(WORD wKindID)
{
	return;
}