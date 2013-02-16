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
//���뷿��
bool __cdecl CServerList::InsertGameServer(tagGameServer * pGameServer)
{
	//Ч�����
	if (pGameServer==NULL) return false;

		//�����ִ�
	tagGameServer * pGameServerItem=NULL;
	DWORD dwKey=MAKELONG(pGameServer->wKindID,pGameServer->wServerID);
	if (m_ServerItemMap[dwKey] != NULL) return false;
	try
	{
		pGameServerItem=new tagGameServer;
		if (pGameServerItem==NULL) return false;
		//��������
		CopyMemory(&pGameServerItem,pGameServer,sizeof(tagGameServer));
		m_ServerItemMap[dwKey]= pGameServerItem;

		CString szBuffer;
		szBuffer.Format("%s  KindID��%ld ServerID��%ldע��ɹ�",
			pGameServer->szServerName,pGameServer->wKindID,pGameServer->wServerID);
		TraceEventString(szBuffer,Level_Normal);

		//��������
		UpdateKindOnLineCount(pGameServer->wKindID);

		return true;
	}
	catch (...) {}

	return false;
}
//ɾ������
bool __cdecl CServerList::DeleteGameServer(WORD wKindID, WORD wServerID)
{
	//���ҷ���
	tagGameServer * pGameServerItem=NULL;
	DWORD dwKey=MAKELONG(wKindID,wServerID);
	if (m_ServerItemMap[dwKey]==NULL) return false;

	//ɾ������
	delete m_ServerItemMap[dwKey];
	m_ServerItemMap.erase(dwKey);
	//m_ServerItemStore.Add(pGameServerItem);

	//��������
	UpdateKindOnLineCount(wKindID);

	return false;
}

//ö�ٷ���
tagGameServer * const __cdecl CServerList::EmunGameServerItem(int & Pos)
{
	//��������
	if (Pos < 0)
		return NULL;

	if (Pos==0) return NULL;

	//���ҷ���
	map<DWORD,tagGameServer *>::const_iterator ptr = m_ServerItemMap.begin();
	for(int i=0;i<m_ServerItemMap.size();i++)
	{	ptr++;
		if(i == Pos)
			return ptr->second;
	}
	return NULL;
}
//���ҷ���
tagGameServer * const __cdecl CServerList::SearchGameServer(WORD wKindID, WORD wServerID)
{
	tagGameServer * pGameServerItem=NULL;
	DWORD dwKey=MAKELONG(wKindID,wServerID);
	pGameServerItem = m_ServerItemMap[dwKey];
	return pGameServerItem;
}

//�����б�
void __cdecl CServerList::ResetServerList()
{
	//ɾ������
	for(map<DWORD,tagGameServer *>::iterator ptr = m_ServerItemMap.begin();
		ptr != m_ServerItemMap.end();ptr++)
	{
		delete ptr->second;
	}
	m_ServerItemMap.clear();
}
//�����б�
bool __cdecl CServerList::LoadServerList()
{
	return true;
}
//��������
bool __cdecl CServerList::UpdateServerOnLineCount(WORD wKindID, WORD wServerID, DWORD dwOnLineCount)
{
	tagGameServer * pGameServerItem=SearchGameServer(wKindID,wServerID);
	if (pGameServerItem!=NULL)
	{
		//���·���
		//pGameServerItem->dwUpdateTime=(DWORD)time(NULL);
		pGameServerItem->dwOnLineCount=dwOnLineCount;

		//��������
		UpdateKindOnLineCount(wKindID);

		return true;
	}

	return false;
}


//��������
void CServerList::UpdateKindOnLineCount(WORD wKindID)
{
	return;
}