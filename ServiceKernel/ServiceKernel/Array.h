#pragma once

//////////////////////////////////////////////////////////////////////////

//数组模板类
//template <class TYPE, class ARG_TYPE=const TYPE &> 
//class CArrayTemplate : public CObject
//{
//	//变量定义
//protected:
//	TYPE							* m_pData;							//数组指针
//	INT_PTR							m_nMaxCount;						//缓冲数目
//	INT_PTR							m_nGrowCount;						//增长数目
//	INT_PTR							m_nElementCount;					//元素数目
//
//	//函数定义
//public:
//	//构造函数
//	CArrayTemplate(void);
//	//析构函数
//	virtual ~CArrayTemplate(void);
//
//	//信息函数
//public:
//	//是否空组
//	bool IsEmpty() const;
//	//获取数目
//	INT_PTR GetCount() const;
//	//获取上限
//	INT_PTR GetUpperBound() const;
//
//	//功能函数
//public:
//	//获取缓冲
//	TYPE * GetData();
//	//获取缓冲
//	const TYPE * GetData() const;
//	//释放内存
//	void FreeExtra();
//	//增加元素
//	INT_PTR Add(ARG_TYPE newElement);
//	//拷贝数组
//	void Copy(const CArrayTemplate & Src);
//	//追加数组
//	INT_PTR Append(const CArrayTemplate & Src);
//	//获取元素
//	TYPE & GetAt(INT_PTR nIndex);
//	//获取元素
//	const TYPE & GetAt(INT_PTR nIndex) const;
//	//获取元素
//	TYPE & ElementAt(INT_PTR nIndex);
//	//获取元素
//	const TYPE & ElementAt(INT_PTR nIndex) const;
//
//	//操作函数
//public:
//	//设置大小
//	void SetSize(INT_PTR nNewSize);
//	//设置元素
//	void SetAt(INT_PTR nIndex, ARG_TYPE newElement);
//	//设置元素
//	void SetAtGrow(INT_PTR nIndex, ARG_TYPE newElement);
//	//插入数据
//	void InsertAt(INT_PTR nIndex, const CArrayTemplate & Src);
//	//插入数据
//	void InsertAt(INT_PTR nIndex, ARG_TYPE newElement, INT_PTR nCount=1);
//	//删除数据
//	void RemoveAt(INT_PTR nIndex, INT_PTR nCount=1);
//	//删除元素
//	void RemoveAll();
//
//	//操作重载
//public:
//	//操作重载
//	TYPE & operator[](INT_PTR nIndex);
//	//操作重载
//	const TYPE & operator[](INT_PTR nIndex) const;
//
//	//内部函数
//private:
//	//申请内存
//	void AllocMemory(INT_PTR nNewCount);
//};
//
////////////////////////////////////////////////////////////////////////////
//// CArrayTemplate<TYPE, ARG_TYPE> 内联函数
//
////是否空组
//template<class TYPE, class ARG_TYPE> 
//inline bool CArrayTemplate<TYPE, ARG_TYPE>::IsEmpty() const
//{
////	//ASSERT_VALID(this);
//	return (m_nElementCount==0);
//}
//
////获取数目
//template<class TYPE, class ARG_TYPE> 
//inline INT_PTR CArrayTemplate<TYPE, ARG_TYPE>::GetCount() const
//{
////	//ASSERT_VALID(this);
//	return m_nElementCount;
//}
//
////获取上限
//template<class TYPE, class ARG_TYPE> 
//inline INT_PTR CArrayTemplate<TYPE, ARG_TYPE>::GetUpperBound() const
//{
////	//ASSERT_VALID(this);
//	return m_nElementCount-1;
//}
//
////增加元素
//template<class TYPE, class ARG_TYPE> 
//inline INT_PTR CArrayTemplate<TYPE,ARG_TYPE>::Add(ARG_TYPE newElement)
//{
//	INT_PTR nIndex=m_nElementCount;
//	SetAtGrow(nIndex,newElement);
//	return nIndex;
//}
//
////操作重载
//template<class TYPE, class ARG_TYPE>
//inline TYPE & CArrayTemplate<TYPE, ARG_TYPE>::operator[](INT_PTR nIndex)
//{ 
//	return ElementAt(nIndex); 
//}
//
////操作重载
//template<class TYPE, class ARG_TYPE>
//inline const TYPE & CArrayTemplate<TYPE, ARG_TYPE>::operator[](INT_PTR nIndex) const
//{ 
//	return GetAt(nIndex); 
//}
//
////////////////////////////////////////////////////////////////////////////
//// CArrayTemplate<TYPE, ARG_TYPE> 外联函数
//
////构造函数
//template<class TYPE, class ARG_TYPE> 
//CArrayTemplate<TYPE, ARG_TYPE>::CArrayTemplate()
//{
//	m_pData=NULL;
//	m_nMaxCount=0;
//	m_nGrowCount=0;
//	m_nElementCount=0;
//	return;
//}
//
////构造函数
//template<class TYPE, class ARG_TYPE> 
//CArrayTemplate<TYPE,ARG_TYPE>::~CArrayTemplate()
//{
//	if (m_pData!=NULL)
//	{
//		for (INT_PTR i=0;i<m_nElementCount;i++)	(m_pData+i)->~TYPE();
//		delete [] (BYTE *)m_pData;
//		m_pData=NULL;
//	}
//	return;
//}
//
////获取缓冲
//template<class TYPE, class ARG_TYPE> 
//TYPE * CArrayTemplate<TYPE,ARG_TYPE>::GetData()
//{
////	//ASSERT_VALID(this);
//	return m_pData;
//}
//
////获取缓冲
//template<class TYPE, class ARG_TYPE> 
//const TYPE * CArrayTemplate<TYPE,ARG_TYPE>::GetData() const
//{
////	//ASSERT_VALID(this);
//	return m_pData;
//}
//
////释放内存
//template<class TYPE, class ARG_TYPE>
//void CArrayTemplate<TYPE,ARG_TYPE>::FreeExtra()
//{
////	//ASSERT_VALID(this);
//	if (m_nElementCount!=m_nMaxCount)
//	{
//		TYPE * pNewData=NULL;
//		if (m_nElementCount!=0)
//		{
//			pNewData=(TYPE *) new BYTE[m_nElementCount*sizeof(TYPE)];
//			memcpy(pNewData,m_pData,m_nElementCount*sizeof(TYPE));
//		}
//		delete [] (BYTE *)m_pData;
//		m_pData=pNewData;
//		m_nMaxCount=m_nElementCount;
//	}
//	return;
//}
//
////拷贝数组
//template<class TYPE, class ARG_TYPE> 
//void CArrayTemplate<TYPE,ARG_TYPE>::Copy(const CArrayTemplate & Src)
//{
//	//效验参数
////	//ASSERT_VALID(this);
//	//ASSERT(this!=&Src);
//	if (this==&Src) return;
//
//	//拷贝数组
//	AllocMemory(Src.m_nElementCount);
//	if (m_nElementCount>0)
//	{
//		for (INT_PTR i=0;i<m_nElementCount;i++) (m_pData+i)->~TYPE();
//		memset(m_pData,0,m_nElementCount*sizeof(TYPE));
//	}
//	for (INT_PTR i=0;i<Src.m_nElementCount;i++)	m_pData[i]=Src.m_pData[i];
//	m_nElementCount=Src.m_nElementCount;
//
//	return;
//}
//
////追加数组
//template<class TYPE, class ARG_TYPE> 
//INT_PTR CArrayTemplate<TYPE,ARG_TYPE>::Append(const CArrayTemplate & Src)
//{
//	//效验参数
////	//ASSERT_VALID(this);
//	////ASSERT(this!=&Src);
//	if (this==&Src)return m_nElementCount;// AfxThrowInvalidArgException();
//
//	//拷贝数组
//	if (Src.m_nElementCount>0)
//	{
//		INT_PTR nOldCount=m_nElementCount;
//		AllocMemory(m_nElementCount+Src.m_nElementCount);
//		for (INT_PTR i=0;i<Src.m_nElementCount;i++)	m_pData[m_nElementCount+i]=Src.m_pData[i];
//		m_nElementCount+=Src.m_nElementCount;
//	}
//
//	return m_nElementCount;
//}
//
////获取元素
//template<class TYPE, class ARG_TYPE> 
//TYPE & CArrayTemplate<TYPE,ARG_TYPE>::GetAt(INT_PTR nIndex)
//{
//	//ASSERT((nIndex>=0)&&(nIndex<m_nElementCount));
//	if ((nIndex>=0)&&(nIndex<m_nElementCount)) return m_pData[nIndex];
//	AfxThrowInvalidArgException();		
//}
//
////获取元素
//template<class TYPE, class ARG_TYPE> 
//const TYPE & CArrayTemplate<TYPE,ARG_TYPE>::GetAt(INT_PTR nIndex) const
//{
//	//ASSERT((nIndex>=0)&&(nIndex<m_nElementCount));
//	if ((nIndex>=0)&&(nIndex<m_nElementCount)) return m_pData[nIndex];
//	//AfxThrowInvalidArgException();		
//}
//
////获取元素
//template<class TYPE, class ARG_TYPE> 
//TYPE & CArrayTemplate<TYPE,ARG_TYPE>::ElementAt(INT_PTR nIndex)
//{
//	//ASSERT((nIndex>=0)&&(nIndex<m_nElementCount));
//	return m_pData[nIndex];
//	//return TYPE();
//	//AfxThrowInvalidArgException();		
//}
//
////获取元素
//template<class TYPE, class ARG_TYPE> 
//const TYPE & CArrayTemplate<TYPE,ARG_TYPE>::ElementAt(INT_PTR nIndex) const
//{
//	//ASSERT((nIndex>=0)&&(nIndex<m_nElementCount));
//	return m_pData[nIndex];
//	//AfxThrowInvalidArgException();		
//}
//
////设置大小
//template<class TYPE, class ARG_TYPE>
//void CArrayTemplate<TYPE,ARG_TYPE>::SetSize(INT_PTR nNewSize)
//{
//	//效验参数
////	//ASSERT_VALID(this);
//	//ASSERT(nNewSize>=0);
//	if (nNewSize<0)	//AfxThrowInvalidArgException();
//		return ;
//	
//	//设置大小
//	AllocMemory(nNewSize);
//	if (nNewSize>m_nElementCount)
//	{
//#pragma push_macro("new")
//#undef new
//		for (INT_PTR i=m_nElementCount;i<nNewSize;i++) ::new ((void *)(m_pData+i)) TYPE;
//#pragma pop_macro("new")
//	}
//	else if (nNewSize<m_nElementCount)
//	{
//		for (INT_PTR i=nNewSize;i<m_nElementCount;i++) (m_pData+i)->~TYPE();
//		memset(m_pData+nNewSize,0,(m_nElementCount-nNewSize)*sizeof(TYPE));
//	}
//	m_nElementCount=nNewSize;
//
//	return;
//}
//
////设置元素
//template<class TYPE, class ARG_TYPE>
//void CArrayTemplate<TYPE,ARG_TYPE>::SetAt(INT_PTR nIndex, ARG_TYPE newElement)
//{
//	//ASSERT((nIndex>=0)&&(nIndex<m_nElementCount));
//	if ((nIndex>=0)&&(nIndex<m_nElementCount)) m_pData[nIndex]=newElement;
//}
//
////设置元素
//template<class TYPE, class ARG_TYPE>
//void CArrayTemplate<TYPE,ARG_TYPE>::SetAtGrow(INT_PTR nIndex, ARG_TYPE newElement)
//{
//	//效验参数
////	//ASSERT_VALID(this);
//	//ASSERT(nIndex>=0);
//	if (nIndex<0) return ;//AfxThrowInvalidArgException();
//
//	//设置元素
//	if (nIndex>=m_nElementCount) SetSize(m_nElementCount+1);
//	m_pData[nIndex]=newElement;
//
//	return;
//}
//
////插入数据
//template<class TYPE, class ARG_TYPE>
//void CArrayTemplate<TYPE,ARG_TYPE>::InsertAt(INT_PTR nIndex, const CArrayTemplate & Src)
//{
//	//效验参数
////	//ASSERT_VALID(this);
//	//ASSERT(nStartIndex>=0);
//	if (nStartIndex<0) AfxThrowInvalidArgException();
//
//	if (Src.m_nElementCount>0)
//	{
//		//申请数组
//		if (nIndex<m_nElementCount)
//		{
//			INT_PTR nOldCount=m_nElementCount;
//			SetSize(m_nElementCount+Src.m_nElementCount);
//			for (INT_PTR i=0;i<nCount;i++) (m_pData+nOldCount+i)->~TYPE();
//			memmove(m_pData+nIndex+nCount,m_pData+nIndex,(nOldCount-nIndex)*sizeof(TYPE));
//			memset(m_pData+nIndex,0,Src.m_nElementCount*sizeof(TYPE));
//#pragma push_macro("new")
//#undef new
//			for (INT_PTR i=0;i<Src.m_nElementCount;i++) ::new (m_pData+nIndex+i) TYPE();
//#pragma pop_macro("new")
//		}
//		else SetSize(nIndex+nCount);
//
//		//拷贝数组
//		//ASSERT((nIndex+Src.m_nElementCount)<=m_nElementCount);
//		while (nCount--) m_pData[nIndex++]=newElement;
//	}
//
//	return;
//}
//
////插入数据
//template<class TYPE, class ARG_TYPE>
//void CArrayTemplate<TYPE,ARG_TYPE>::InsertAt(INT_PTR nIndex, ARG_TYPE newElement, INT_PTR nCount)
//{
//	//效验参数
////	//ASSERT_VALID(this);
//	//ASSERT(nIndex>=0);
//	//ASSERT(nCount>0);
//	if ((nIndex<0)||(nCount<=0)) AfxThrowInvalidArgException();
//
//	//申请数组
//	if (nIndex<m_nElementCount)
//	{
//		INT_PTR nOldCount=m_nElementCount;
//		SetSize(m_nElementCount+nCount);
//		for (INT_PTR i=0;i<nCount;i++) (m_pData+nOldCount+i)->~TYPE();
//		memmove(m_pData+nIndex+nCount,m_pData+nIndex,(nOldCount-nIndex)*sizeof(TYPE));
//		memset(m_pData+nIndex,0,nCount*sizeof(TYPE));
//#pragma push_macro("new")
//#undef new
//		for (INT_PTR i=0;i<nCount;i++) ::new (m_pData+nIndex+i) TYPE();
//#pragma pop_macro("new")
//	}
//	else SetSize(nIndex+nCount);
//
//	//拷贝数组
//	//ASSERT((nIndex+nCount)<=m_nElementCount);
//	while (nCount--) m_pData[nIndex++]=newElement;
//
//	return;
//}
//
////删除数据
//template<class TYPE, class ARG_TYPE>
//void CArrayTemplate<TYPE,ARG_TYPE>::RemoveAt(INT_PTR nIndex, INT_PTR nCount)
//{
//	//效验参数
/////	//ASSERT_VALID(this);
//	//ASSERT(nIndex>=0);
//	//ASSERT(nCount>=0);
//	//ASSERT(nIndex+nCount<=m_nElementCount);
//	if ((nIndex<0)||(nCount<0)||((nIndex+nCount>m_nElementCount))) return ;//AfxThrowInvalidArgException();
//
//	//删除数据
//	INT_PTR nMoveCount=m_nElementCount-(nIndex+nCount);
//	for (INT_PTR i=0;i<nCount;i++) (m_pData+nIndex+i)->~TYPE();
//	if (nMoveCount>0) memmove(m_pData+nIndex,m_pData+nIndex+nCount,nMoveCount*sizeof(TYPE));
//	m_nElementCount-=nCount;
//
//	return;
//}
//
////删除元素
//template<class TYPE, class ARG_TYPE>
//void CArrayTemplate<TYPE,ARG_TYPE>::RemoveAll()
//{
////	//ASSERT_VALID(this);
//	if (m_nElementCount>0)
//	{
//		for (INT_PTR i=0;i<m_nElementCount;i++) (m_pData+i)->~TYPE();
//		memset(m_pData,0,m_nElementCount*sizeof(TYPE));
//		m_nElementCount=0;
//	}
//	return;
//}
//
////申请内存
//template<class TYPE, class ARG_TYPE>
//void CArrayTemplate<TYPE,ARG_TYPE>::AllocMemory(INT_PTR nNewCount)
//{
//	//效验参数
////	//ASSERT_VALID(this);
//	//ASSERT(nNewCount>=0);
//
//	if (nNewCount>m_nMaxCount)
//	{
//		//计算数目
//		INT_PTR nGrowCount=m_nGrowCount;
//		if (nGrowCount==0)
//		{
//			nGrowCount=m_nElementCount/8;
//			nGrowCount=(nGrowCount<4)?4:((nGrowCount>1024)?1024:nGrowCount);
//		}
//		nNewCount+=nGrowCount;
//
//		//申请内存
//		TYPE * pNewData=(TYPE *) new BYTE[nNewCount*sizeof(TYPE)];
//		memcpy(pNewData,m_pData,m_nElementCount*sizeof(TYPE));
//		memset(pNewData+m_nElementCount,0,(nNewCount-m_nElementCount)*sizeof(TYPE));
//		delete [] (BYTE *)m_pData;
//
//		//设置变量
//		m_pData=pNewData;
//		m_nMaxCount=nNewCount;
//	}
//}

#include <list>
using namespace std;
template <class TYPE, class ARG_TYPE=const TYPE &> 
class CListTemplate :public CObject
{
	//变量定义
protected:
	TYPE							* m_pData;							//数组指针
	INT_PTR							m_nMaxCount;						//缓冲数目
	INT_PTR							m_nGrowCount;						//增长数目
	INT_PTR							m_nElementCount;					//元素数目
	list<TYPE>						m_list;

	//函数定义
public:
	//构造函数
	CListTemplate(void);
	//析构函数
	virtual ~CListTemplate(void);

	//信息函数
public:
	//是否空组
	bool IsEmpty() const;
	//获取数目
	INT_PTR GetCount() const;
	//获取上限
	INT_PTR GetUpperBound() const;

	//功能函数
public:
	//获取缓冲
	TYPE * GetData();
	//
	//获取缓冲
	list<TYPE> GetList();
	//
	const list<TYPE> GetList() const;
	//const TYPE * GetData() const;
	//释放内存
	void FreeExtra();
	//增加元素
	INT_PTR Add(ARG_TYPE newElement);
	//拷贝数组
	void Copy(const CListTemplate & Src);
	//追加数组
	INT_PTR Append(const CListTemplate & Src);
	//获取元素
	TYPE & GetAt(INT_PTR nIndex);
	//获取元素
	const TYPE & GetAt(INT_PTR nIndex) const;
	//获取元素
	TYPE & ElementAt(INT_PTR nIndex);
	//获取元素
	const TYPE & ElementAt(INT_PTR nIndex) const;

	//操作函数
public:
	//设置大小
	void SetSize(INT_PTR nNewSize);
	//设置元素
	void SetAt(INT_PTR nIndex, ARG_TYPE newElement);
	//设置元素
	void SetAtGrow(INT_PTR nIndex, ARG_TYPE newElement);
	//插入数据
	//void InsertAt(INT_PTR nIndex, const CListTemplate & Src);
	//插入数据
	void InsertAt(INT_PTR nIndex, ARG_TYPE newElement, INT_PTR nCount=1);
	//删除数据
	void RemoveAt(INT_PTR nIndex, INT_PTR nCount=1);
	//删除元素
	void RemoveAll();

	//操作重载
public:
	//操作重载
	TYPE & operator[](INT_PTR nIndex);
	//操作重载
	const TYPE & operator[](INT_PTR nIndex) const;
	//内部函数
private:
	//申请内存
	void AllocMemory(INT_PTR nNewCount);

};

//////////////////////////////////////////////////////////////////////////
// CListTemplate<TYPE, ARG_TYPE> 内联函数

//是否空组
template<class TYPE, class ARG_TYPE> 
inline bool CListTemplate<TYPE, ARG_TYPE>::IsEmpty() const
{
//	//ASSERT_VALID(this);
	return (m_list.size()==0);
}

//获取数目
template<class TYPE, class ARG_TYPE> 
inline INT_PTR CListTemplate<TYPE, ARG_TYPE>::GetCount() const
{
//	//ASSERT_VALID(this);
	return m_list.size();
}

//获取上限
template<class TYPE, class ARG_TYPE> 
inline INT_PTR CListTemplate<TYPE, ARG_TYPE>::GetUpperBound() const
{
//	//ASSERT_VALID(this);
	return m_list.max_size();
}

//增加元素
template<class TYPE, class ARG_TYPE> 
inline INT_PTR CListTemplate<TYPE,ARG_TYPE>::Add(ARG_TYPE newElement)
{
	m_list.push_back(newElement);
	//INT_PTR nIndex=m_nElementCount;
	//SetAtGrow(nIndex,newElement);
	return m_list.size();
}

//操作重载
template<class TYPE, class ARG_TYPE>
inline TYPE & CListTemplate<TYPE, ARG_TYPE>::operator[](INT_PTR nIndex)
{ 
	list<TYPE>::iterator iter=m_list.begin();
	advance(iter,nIndex);
	return *iter;
	//return ElementAt(nIndex); 
}

//操作重载
template<class TYPE, class ARG_TYPE>
inline const TYPE & CListTemplate<TYPE, ARG_TYPE>::operator[](INT_PTR nIndex) const
{ 
	list<TYPE>::const_iterator iter=m_list.begin();
	advance(iter,nIndex);
	return *iter;
	//return GetAt(nIndex); 
}

//////////////////////////////////////////////////////////////////////////
// CListTemplate<TYPE, ARG_TYPE> 外联函数

//构造函数
template<class TYPE, class ARG_TYPE> 
CListTemplate<TYPE, ARG_TYPE>::CListTemplate()
{
	return;
}

//构造函数
template<class TYPE, class ARG_TYPE> 
CListTemplate<TYPE,ARG_TYPE>::~CListTemplate()
{
	m_list.clear();
	return;
}

//拷贝数组
template<class TYPE, class ARG_TYPE> 
void CListTemplate<TYPE,ARG_TYPE>::Copy(const CListTemplate & Src)
{
	//效验参数
//	//ASSERT_VALID(this);
	//ASSERT(this!=&Src);
	if (this==&Src) return;

	m_list.clear();
	Append(Src);
	//m_list = Src;
//	m_list.push_back(Src);
	//拷贝数组
	//AllocMemory(Src.m_nElementCount);
	//if (m_nElementCount>0)
	//{
	//	for (INT_PTR i=0;i<m_nElementCount;i++) (m_pData+i)->~TYPE();
	//	memset(m_pData,0,m_nElementCount*sizeof(TYPE));
	//}
	//for (INT_PTR i=0;i<Src.m_nElementCount;i++)	m_pData[i]=Src.m_pData[i];
	//m_nElementCount=Src.m_nElementCount;
	return;
}
//追加数组
template<class TYPE, class ARG_TYPE> 
INT_PTR CListTemplate<TYPE,ARG_TYPE>::Append(const CListTemplate & Src)
{
	//效验参数
//	//ASSERT_VALID(this);
	////ASSERT(this!=&Src);
	if (this==&Src)return m_list.size();// //AfxThrowInvalidArgException();
	//m_list.(Src);
	if (Src.GetCount() < 0) return m_list.size();
	for(int i=0;i<Src.GetCount();i++)
		m_list.push_back(Src.GetAt(i));
	return m_list.size();
	//拷贝数组
	//if (Src.m_nElementCount>0)
	//{
	//	INT_PTR nOldCount=m_nElementCount;
	//	AllocMemory(m_nElementCount+Src.m_nElementCount);
	//	for (INT_PTR i=0;i<Src.m_nElementCount;i++)	m_pData[m_nElementCount+i]=Src.m_pData[i];
	//	m_nElementCount+=Src.m_nElementCount;
	//}

	//return m_nElementCount;
}

//获取元素
template<class TYPE, class ARG_TYPE> 
TYPE & CListTemplate<TYPE,ARG_TYPE>::GetAt(INT_PTR nIndex)
{
	//ASSERT((nIndex>=0)&&(nIndex<m_nElementCount));
	if ((nIndex>=0)&&(nIndex<m_list.size()))
	{
		list<TYPE>::iterator iter=m_list.begin();
		advance(iter,nIndex);
		return *iter;	
	}
	return TYPE();
	////AfxThrowInvalidArgException();		
}

//获取元素
template<class TYPE, class ARG_TYPE> 
const TYPE & CListTemplate<TYPE,ARG_TYPE>::GetAt(INT_PTR nIndex) const
{
	//ASSERT((nIndex>=0)&&(nIndex<m_nElementCount));
	if ((nIndex>=0)&&(nIndex<m_list.size()))
	{
		list<TYPE>::const_iterator iter=m_list.begin();
		advance(iter,nIndex);
		return *iter;	
	}
	return TYPE();
	////AfxThrowInvalidArgException();		
}

//获取元素
template<class TYPE, class ARG_TYPE> 
TYPE & CListTemplate<TYPE,ARG_TYPE>::ElementAt(INT_PTR nIndex)
{
	//ASSERT((nIndex>=0)&&(nIndex<m_nElementCount));
	return GetAt(nIndex);
	//return TYPE();
	////AfxThrowInvalidArgException();		
}

//获取元素
template<class TYPE, class ARG_TYPE> 
const TYPE & CListTemplate<TYPE,ARG_TYPE>::ElementAt(INT_PTR nIndex) const
{
	//ASSERT((nIndex>=0)&&(nIndex<m_nElementCount));
	return GetAt(nIndex);
	////AfxThrowInvalidArgException();		
}

//设置大小
//template<class TYPE, class ARG_TYPE>
//void CListTemplate<TYPE,ARG_TYPE>::SetSize(INT_PTR nNewSize)
//{
//	//效验参数
////	//ASSERT_VALID(this);
//	//ASSERT(nNewSize>=0);
//	if (nNewSize<0)	////AfxThrowInvalidArgException();
//		return ;
//	
//	//设置大小
//	AllocMemory(nNewSize);
//	if (nNewSize>m_nElementCount)
//	{
//#pragma push_macro("new")
//#undef new
//		for (INT_PTR i=m_nElementCount;i<nNewSize;i++) ::new ((void *)(m_pData+i)) TYPE;
//#pragma pop_macro("new")
//	}
//	else if (nNewSize<m_nElementCount)
//	{
//		for (INT_PTR i=nNewSize;i<m_nElementCount;i++) (m_pData+i)->~TYPE();
//		memset(m_pData+nNewSize,0,(m_nElementCount-nNewSize)*sizeof(TYPE));
//	}
//	m_nElementCount=nNewSize;
//
//	return;
//}

//设置元素
template<class TYPE, class ARG_TYPE>
void CListTemplate<TYPE,ARG_TYPE>::SetAt(INT_PTR nIndex, ARG_TYPE newElement)
{
	//ASSERT((nIndex>=0)&&(nIndex<m_nElementCount));
	//if ((nIndex>=0)&&(nIndex<m_list.size())) m_pData[nIndex]=newElement;
	if ((nIndex>=0)&&(nIndex<m_list.size()))
	{
		list<TYPE>::const_iterator iter=m_list.begin();
		advance(iter,nIndex);
		iter = newElement;
		//return iter;	
	}
}

//设置元素
//template<class TYPE, class ARG_TYPE>
//void CListTemplate<TYPE,ARG_TYPE>::SetAtGrow(INT_PTR nIndex, ARG_TYPE newElement)
//{
//	//效验参数
////	//ASSERT_VALID(this);
//	//ASSERT(nIndex>=0);
//	if (nIndex<0) return ;////AfxThrowInvalidArgException();
//
//	//设置元素
//	if (nIndex>=m_nElementCount) SetSize(m_nElementCount+1);
//	m_pData[nIndex]=newElement;
//
//	return;
//}

//插入数据
//template<class TYPE, class ARG_TYPE>
//void CListTemplate<TYPE,ARG_TYPE>::InsertAt(INT_PTR nIndex, const CListTemplate & Src)
//{
//	//效验参数
//	ASSERT_EQ(nIndex >= 0);
//	ASSERT_EQ(m_list.size()>=nIndex);
//	list<TYPE>::iterator iter=m_list.begin();
//	advance(iter,nIndex);
//	m_list.splice(iter,Src.GetData());
////	//ASSERT_VALID(this);
//	//ASSERT(nStartIndex>=0);
////	if (nStartIndex<0) //AfxThrowInvalidArgException();
////
////	if (Src.m_nElementCount>0)
////	{
////		//申请数组
////		if (nIndex<m_nElementCount)
////		{
////			INT_PTR nOldCount=m_nElementCount;
////			SetSize(m_nElementCount+Src.m_nElementCount);
////			for (INT_PTR i=0;i<nCount;i++) (m_pData+nOldCount+i)->~TYPE();
////			memmove(m_pData+nIndex+nCount,m_pData+nIndex,(nOldCount-nIndex)*sizeof(TYPE));
////			memset(m_pData+nIndex,0,Src.m_nElementCount*sizeof(TYPE));
////#pragma push_macro("new")
////#undef new
////			for (INT_PTR i=0;i<Src.m_nElementCount;i++) ::new (m_pData+nIndex+i) TYPE();
////#pragma pop_macro("new")
////		}
////		else SetSize(nIndex+nCount);
////
////		//拷贝数组
////		//ASSERT((nIndex+Src.m_nElementCount)<=m_nElementCount);
////		while (nCount--) m_pData[nIndex++]=newElement;
////	}
//
//	return;
//}

//插入数据
template<class TYPE, class ARG_TYPE>
void CListTemplate<TYPE,ARG_TYPE>::InsertAt(INT_PTR nIndex, ARG_TYPE newElement, INT_PTR nCount)
{
	EXPECT_TRUE(nIndex>=0);
	EXPECT_TRUE(m_list.size()>0);
	ASSERT_FALSE((nIndex<0)||(nCount<=0));
	if(nIndex < m_list.size())
	{
		list<TYPE>::iterator iter = m_list.begin();
		advance(iter,nIndex);
		for(int i=0;i<nCount;i++)
			m_list.insert(iter,newElement);
			//m_list.erase(iter);
	}
	else
	{
		list<TYPE>::iterator iter = m_list.begin();
		advance(iter,m_list.size());
		int nPos = nIndex - m_list.size();
		for(int i = 0;i<nPos;i++)
			m_list.push_back(TYPE());
		for(int i=0;i<nCount;i++)
			m_list.insert(iter,newElement);
	}
	//效验参数
//	//ASSERT_VALID(this);
	//ASSERT(nIndex>=0);
	//ASSERT(nCount>0);
//	if ((nIndex<0)||(nCount<=0)) //AfxThrowInvalidArgException();
//
//	//申请数组
//	if (nIndex<m_nElementCount)
//	{
//		INT_PTR nOldCount=m_nElementCount;
//		SetSize(m_nElementCount+nCount);
//		for (INT_PTR i=0;i<nCount;i++) (m_pData+nOldCount+i)->~TYPE();
//		memmove(m_pData+nIndex+nCount,m_pData+nIndex,(nOldCount-nIndex)*sizeof(TYPE));
//		memset(m_pData+nIndex,0,nCount*sizeof(TYPE));
//#pragma push_macro("new")
//#undef new
//		for (INT_PTR i=0;i<nCount;i++) ::new (m_pData+nIndex+i) TYPE();
//#pragma pop_macro("new")
//	}
//	else SetSize(nIndex+nCount);
//
//	//拷贝数组
//	//ASSERT((nIndex+nCount)<=m_nElementCount);
//	while (nCount--) m_pData[nIndex++]=newElement;

	return;
}

//删除数据
template<class TYPE, class ARG_TYPE>
void CListTemplate<TYPE,ARG_TYPE>::RemoveAt(INT_PTR nIndex, INT_PTR nCount)
{
	//效验参数
///	//ASSERT_VALID(this);
	//ASSERT(nIndex>=0);
	//ASSERT(nCount>=0);
	//ASSERT(nIndex+nCount<=m_nElementCount);
	//if ((nIndex<0)||(nCount<0)||((nIndex+nCount>m_nElementCount))) return ;////AfxThrowInvalidArgException();
	if ((nIndex>=0)&&(nIndex<m_list.size()))
	{
		list<TYPE>::iterator iter=m_list.begin();
		advance(iter,nIndex);
		m_list.erase(iter);
		//return iter;	
	}
	
	////删除数据
	//INT_PTR nMoveCount=m_nElementCount-(nIndex+nCount);
	//for (INT_PTR i=0;i<nCount;i++) (m_pData+nIndex+i)->~TYPE();
	//if (nMoveCount>0) memmove(m_pData+nIndex,m_pData+nIndex+nCount,nMoveCount*sizeof(TYPE));
	//m_nElementCount-=nCount;

	return;
}

//删除元素
template<class TYPE, class ARG_TYPE>
void CListTemplate<TYPE,ARG_TYPE>::RemoveAll()
{
//	//ASSERT_VALID(this);
	m_list.clear();
	
	return;
}
//获取缓冲
template<class TYPE, class ARG_TYPE> 
TYPE * CListTemplate<TYPE,ARG_TYPE>::GetData()
{
//	//ASSERT_VALID(this);
	struct test *p = new struct test[m_list.size()];
	struct test *ptr = p;
	list<TYPE>::iterator iter = m_list.begin();
	for(int i=0;iter!=m_list.end();++iter,++ptr)
	{
		//ptr=*iter;
		struct test temp = *iter;
		memcpy(ptr,&temp,sizeof(struct test));
	}
	return p;
}
//获取缓冲
template<class TYPE, class ARG_TYPE> 
list<TYPE> CListTemplate<TYPE,ARG_TYPE>::GetList()
{
//	//ASSERT_VALID(this);
	return m_list;
}
template<class TYPE, class ARG_TYPE> 
const list<TYPE> CListTemplate<TYPE,ARG_TYPE>::GetList() const
{
//	//ASSERT_VALID(this);
	return m_list;
}