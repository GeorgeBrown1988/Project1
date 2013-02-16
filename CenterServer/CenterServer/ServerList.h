#pragma once
#include <map>
using namespace std;
//////////////////////////////////////////////////////////////////////////
//结构定义

////游戏房间
//struct tagGameServerItem
//{
//	DWORD							dwUpdateTime;						//更新时间
//	tagGameServer					GameServer;							//房间信息
//};


//类说明
//typedef CListTemplate<tagGameServer *>								CGameServerItemArray;
typedef map<DWORD,tagGameServer *>		CServerItemMap;

//////////////////////////////////////////////////////////////////////////

//服务器列表类
class CServerList
{
	//索引管理
protected:
	CServerItemMap					m_ServerItemMap;					//索引数组

//	//存储管理
//protected:
//	CGameServerItemArray			m_ServerItemStore;					//存储数组

	//函数定义
public:
	//构造函数
	CServerList(void);
	//析构函数
	virtual ~CServerList(void);
	//插入接口
public:
	//插入房间
	virtual bool __cdecl InsertGameServer(tagGameServer * pGameServer);

	//删除接口
public:
	//删除房间
	virtual bool __cdecl DeleteGameServer(WORD wKindID, WORD wServerID);

	//枚举接口
public:
	//枚举房间
	virtual tagGameServer * const __cdecl EmunGameServerItem(int & Pos);

	//查找接口
public:
	//查找房间
	virtual tagGameServer * const __cdecl SearchGameServer(WORD wKindID, WORD wServerID);

	//数目接口
public:
	//房间数目
	virtual DWORD __cdecl GetGameServerCount() { return (DWORD)m_ServerItemMap.size(); }

	//维护接口
public:
	//重置列表
	virtual void __cdecl ResetServerList();
	//加载列表
	virtual bool __cdecl LoadServerList();
	//更新人数
	virtual bool __cdecl UpdateServerOnLineCount(WORD wKindID, WORD wServerID, DWORD dwOnLineCount);

//	//加载接口
//private:
//	//加载类型
//	void LoadGameType(IDataBase * pIDataBase);
//	//加载种类
//	void LoadGameKind(IDataBase * pIDataBase);
//	//加载站点
//	void LoadGameStation(IDataBase * pIDataBase);

	//内部函数
private:
	//更新人数
	void UpdateKindOnLineCount(WORD wKindID);
};
