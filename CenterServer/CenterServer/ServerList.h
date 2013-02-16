#pragma once
#include <map>
using namespace std;
//////////////////////////////////////////////////////////////////////////
//�ṹ����

////��Ϸ����
//struct tagGameServerItem
//{
//	DWORD							dwUpdateTime;						//����ʱ��
//	tagGameServer					GameServer;							//������Ϣ
//};


//��˵��
//typedef CListTemplate<tagGameServer *>								CGameServerItemArray;
typedef map<DWORD,tagGameServer *>		CServerItemMap;

//////////////////////////////////////////////////////////////////////////

//�������б���
class CServerList
{
	//��������
protected:
	CServerItemMap					m_ServerItemMap;					//��������

//	//�洢����
//protected:
//	CGameServerItemArray			m_ServerItemStore;					//�洢����

	//��������
public:
	//���캯��
	CServerList(void);
	//��������
	virtual ~CServerList(void);
	//����ӿ�
public:
	//���뷿��
	virtual bool __cdecl InsertGameServer(tagGameServer * pGameServer);

	//ɾ���ӿ�
public:
	//ɾ������
	virtual bool __cdecl DeleteGameServer(WORD wKindID, WORD wServerID);

	//ö�ٽӿ�
public:
	//ö�ٷ���
	virtual tagGameServer * const __cdecl EmunGameServerItem(int & Pos);

	//���ҽӿ�
public:
	//���ҷ���
	virtual tagGameServer * const __cdecl SearchGameServer(WORD wKindID, WORD wServerID);

	//��Ŀ�ӿ�
public:
	//������Ŀ
	virtual DWORD __cdecl GetGameServerCount() { return (DWORD)m_ServerItemMap.size(); }

	//ά���ӿ�
public:
	//�����б�
	virtual void __cdecl ResetServerList();
	//�����б�
	virtual bool __cdecl LoadServerList();
	//��������
	virtual bool __cdecl UpdateServerOnLineCount(WORD wKindID, WORD wServerID, DWORD dwOnLineCount);

//	//���ؽӿ�
//private:
//	//��������
//	void LoadGameType(IDataBase * pIDataBase);
//	//��������
//	void LoadGameKind(IDataBase * pIDataBase);
//	//����վ��
//	void LoadGameStation(IDataBase * pIDataBase);

	//�ڲ�����
private:
	//��������
	void UpdateKindOnLineCount(WORD wKindID);
};
