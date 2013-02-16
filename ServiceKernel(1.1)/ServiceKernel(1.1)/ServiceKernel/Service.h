#pragma once
////接口定义
//struct ServiceSink
//{
//	void 					*m_pQueue;
//	eServiceKind			m_eServiceKind;
//};
////同步对象接口
//class IThreadLock
//{
//	//锁定函数
//	virtual void Lock()=NULL;
//	//解锁函数 
//	virtual void UnLock()=NULL;
//};
//class IService
//{
//	public:
//		//状态判断
//		virtual bool IsRuning()=NULL;
//		//启动线程
//		virtual bool StartThead()=NULL;
//		//停止线程
//		virtual bool StopThread()=NULL;
//		//中止线程
//		virtual bool TerminateThread(DWORD dwExitCode)=NULL;
//		//投递事件
//		virtual bool PostEvent(HANDLE ,void *)=NULL;
//		//挂载队列钩子
//		virtual bool SetQueueSink(void *)=NULL;
//		//外部服务钩子
//		virtual bool SetServiceSink(void *)=NULL;
//};
//
////临界区同步类
//class CThreadLock : public IThreadLock
//{
//	//变量定义
//private:
//	CRITICAL_SECTION					m_csLock;					//临界变量
//
//	//函数定义
//public:
//	//构造函数
//	inline CThreadLock() { ::InitializeCriticalSection(&m_csLock); }
//	//析构函数
//	inline ~CThreadLock() { ::DeleteCriticalSection(&m_csLock); }
//
//	//功能函数
//public:
//	//锁定函数
//	virtual inline void Lock() { ::EnterCriticalSection(&m_csLock); }
//	//解锁函数 
//	virtual inline void UnLock() { ::LeaveCriticalSection(&m_csLock); }
//};
////安全同步锁定句柄
//class CThreadLockHandle
//{
//	//变量定义
//private:
//	int								m_nLockCount;				//锁定计数
//	IThreadLock						* m_pIThreadLock;			//锁定对象
//
//	//函数定义
//public:
//	//构造函数
//	CThreadLockHandle(IThreadLock * pIThreadLock, bool bAutoLock=true);
//	//析构函数
//	virtual ~CThreadLockHandle();
//
//	//功能函数
//public:
//	//锁定函数
//	void Lock();
//	//解锁函数 
//	void UnLock();
//	//获取锁定次数
//	int inline GetLockCount() { return m_nLockCount; }
//};
////线程对象类
//class  CService :public IService
//{
//	//变量定义
//private:
//	volatile bool						m_bRun;							//运行标志
//	UINT								m_uThreadID;					//线程标识
//	HANDLE								m_hThreadHandle;				//线程句柄
//	void*								m_pQueue;						//队列钩子
//	
//
//	//函数定义
//protected:
//	//构造函数
//	CService(void);
//	//析构函数
//	virtual ~CService(void);
//
//	//功能函数
//public:
//	//获取标识
//	UINT GetThreadID() { return m_uThreadID; }
//	//获取句柄
//	HANDLE GetThreadHandle() { return m_hThreadHandle; }
//	//投递消息
//	bool PostThreadMessage(UINT uMessage, WPARAM wParam, LPARAM lParam);
//
//	//内部函数
//private:
//	//运行函数
//	virtual bool RepetitionRun()=NULL;
//	//线程函数
//	static unsigned __stdcall ThreadFunction(LPVOID pThreadData);
//};
