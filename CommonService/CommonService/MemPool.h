#pragma once
#include <list>
using namespace std;
struct BlockInfo
{
	DWORD dwOffset;//ƫ����
	DWORD dwDataSize;//ʵ�����ݴ�С
	DWORD dwMemSize;//�ڴ���С
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
	BYTE * ElementAt(int pos);//��ȡ����ݿ�λ��
	void   RemoveAt(int pos);//�Ƴ�����ݿ�λ��


};

