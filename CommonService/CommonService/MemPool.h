#pragma once
#include <list>
using namespace std;
struct BlockInfo
{
	DWORD dwOffset;//偏移量
	DWORD dwDataSize;//实际数据大小
	DWORD dwMemSize;//内存块大小
};
class CMemPool
{
public:
	CMemPool(void);
	~CMemPool(void);
private:
	list<BYTE *> m_DataList;
	list<BlockInfo> m_ActiveDataIndex;
	list<BlockInfo> m_FreeDataIndex;
public:
	BYTE * ElementAt(int pos);//获取活动数据块位置
	void   RemoveAt(int pos);//移除活动数据块位置


};

