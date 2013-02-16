#pragma once
////�ӿڶ���
//struct ServiceSink
//{
//	void 					*m_pQueue;
//	eServiceKind			m_eServiceKind;
//};
////ͬ������ӿ�
//class IThreadLock
//{
//	//��������
//	virtual void Lock()=NULL;
//	//�������� 
//	virtual void UnLock()=NULL;
//};
//class IService
//{
//	public:
//		//״̬�ж�
//		virtual bool IsRuning()=NULL;
//		//�����߳�
//		virtual bool StartThead()=NULL;
//		//ֹͣ�߳�
//		virtual bool StopThread()=NULL;
//		//��ֹ�߳�
//		virtual bool TerminateThread(DWORD dwExitCode)=NULL;
//		//Ͷ���¼�
//		virtual bool PostEvent(HANDLE ,void *)=NULL;
//		//���ض��й���
//		virtual bool SetQueueSink(void *)=NULL;
//		//�ⲿ������
//		virtual bool SetServiceSink(void *)=NULL;
//};
//
////�ٽ���ͬ����
//class CThreadLock : public IThreadLock
//{
//	//��������
//private:
//	CRITICAL_SECTION					m_csLock;					//�ٽ����
//
//	//��������
//public:
//	//���캯��
//	inline CThreadLock() { ::InitializeCriticalSection(&m_csLock); }
//	//��������
//	inline ~CThreadLock() { ::DeleteCriticalSection(&m_csLock); }
//
//	//���ܺ���
//public:
//	//��������
//	virtual inline void Lock() { ::EnterCriticalSection(&m_csLock); }
//	//�������� 
//	virtual inline void UnLock() { ::LeaveCriticalSection(&m_csLock); }
//};
////��ȫͬ���������
//class CThreadLockHandle
//{
//	//��������
//private:
//	int								m_nLockCount;				//��������
//	IThreadLock						* m_pIThreadLock;			//��������
//
//	//��������
//public:
//	//���캯��
//	CThreadLockHandle(IThreadLock * pIThreadLock, bool bAutoLock=true);
//	//��������
//	virtual ~CThreadLockHandle();
//
//	//���ܺ���
//public:
//	//��������
//	void Lock();
//	//�������� 
//	void UnLock();
//	//��ȡ��������
//	int inline GetLockCount() { return m_nLockCount; }
//};
////�̶߳�����
//class  CService :public IService
//{
//	//��������
//private:
//	volatile bool						m_bRun;							//���б�־
//	UINT								m_uThreadID;					//�̱߳�ʶ
//	HANDLE								m_hThreadHandle;				//�߳̾��
//	void*								m_pQueue;						//���й���
//	
//
//	//��������
//protected:
//	//���캯��
//	CService(void);
//	//��������
//	virtual ~CService(void);
//
//	//���ܺ���
//public:
//	//��ȡ��ʶ
//	UINT GetThreadID() { return m_uThreadID; }
//	//��ȡ���
//	HANDLE GetThreadHandle() { return m_hThreadHandle; }
//	//Ͷ����Ϣ
//	bool PostThreadMessage(UINT uMessage, WPARAM wParam, LPARAM lParam);
//
//	//�ڲ�����
//private:
//	//���к���
//	virtual bool RepetitionRun()=NULL;
//	//�̺߳���
//	static unsigned __stdcall ThreadFunction(LPVOID pThreadData);
//};
