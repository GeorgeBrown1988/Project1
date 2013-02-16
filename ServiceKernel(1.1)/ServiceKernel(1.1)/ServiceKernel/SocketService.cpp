#include "StdAfx.h"
#include "SocketService.h"
#include <shellapi.h>
//////////////////////////////////////////////////////////////////////////
//�궨��

#define TIME_BREAK_READY			9000L								//�ж�ʱ��
#define TIME_BREAK_CONNECT			4000L								//�ж�ʱ��
#define TIME_DETECT_SOCKET			20000L								//���ʱ��

//////////////////////////////////////////////////////////////////////////

//��������
#define QUEUE_SEND_REQUEST			1									//���ͱ�ʶ
#define QUEUE_SAFE_CLOSE			2									//��ȫ�ر�
#define QUEUE_ALLOW_BATCH			3									//����Ⱥ��
#define QUEUE_DETECT_SOCKET			4									//�������

//��������ṹ
struct tagSendDataRequest
{
	WORD							wMainCmdID;							//��������
	WORD							wSubCmdID;							//��������
	WORD							wIndex;								//��������
	WORD							wRountID;							//ѭ������
	WORD							wDataSize;							//���ݴ�С
	BYTE							cbSendBuf[SOCKET_PACKAGE];			//���ͻ���
};

//����Ⱥ��
struct tagAllowBatchSend
{
	WORD							wIndex;								//��������
	WORD							wRountID;							//ѭ������
	BYTE							cbAllow;							//�����־
};

//��ȫ�ر�
struct tagSafeCloseSocket
{
	WORD							wIndex;								//��������
	WORD							wRountID;							//ѭ������
};
//���캯��
COverLapped::COverLapped(enOperationType OperationType) : m_OperationType(OperationType)
{
	memset(&m_WSABuffer,0,sizeof(m_WSABuffer));
	memset(&m_OverLapped,0,sizeof(m_OverLapped));
}

//��������
COverLapped::~COverLapped()
{
}

//////////////////////////////////////////////////////////////////////////

//���캯��
COverLappedSend::COverLappedSend() : COverLapped(OperationType_Send)
{
	m_WSABuffer.len=0;
	m_WSABuffer.buf=(char *)m_cbBuffer;
}

//��������
COverLappedSend::~COverLappedSend()
{
}

CTCPSocketItem::CTCPSocketItem(WORD wIndex, IServerSocketItemSink * pIServerSocketItemSink)
	: m_wIndex(wIndex),m_pIServerSocketItemSink(pIServerSocketItemSink)
{
	//m_wIndex = wIndex;
	m_dwClientAddr = 0;						
	m_dwConnectTime = 0;					
	m_wRountID = 0;							
	m_hSocket = INVALID_SOCKET;							
	m_bNotify = true;							
	m_bRecvIng = false;							
	m_bCloseIng = false;						
	m_bAllowBatch = false;						
	m_wRecvSize = 0;						
	memset(m_cbRecvBuf,0 ,SOCKET_BUFFER);			
	m_dwSendTickCount = 0;					
	m_dwRecvTickCount = 0;					
	m_dwSendPacketCount = 0;				
	m_dwRecvPacketCount = 0;																
	m_pIServerSocketItemSink =NULL;	
#ifdef _DEBUG
	LOG(INFO) <<"CTCPSocketItem";
#endif
}


CTCPSocketItem::~CTCPSocketItem(void)
{
#ifdef _DEBUG
	LOG(INFO) <<"~CTCPSocketItem";
#endif
}
DWORD CTCPSocketItem::Attach(SOCKET hSocket, DWORD dwClientAddr)
{
	//���ñ���
	m_bNotify=false;
	m_bRecvIng=false;
	m_bCloseIng=false;
	m_hSocket=hSocket;
	m_dwClientAddr=dwClientAddr;
	m_dwRecvTickCount=GetTickCount();
	m_dwConnectTime=(DWORD)time(NULL);

	//����֪ͨ
	m_pIServerSocketItemSink->OnSocketAcceptEvent(this);

	return GetIdentifierID();
}
//���ͺ���
bool CTCPSocketItem::SendData(WORD wMainCmdID, WORD wSubCmdID, WORD wRountID)
{
	//Ч��״̬
	if (m_bCloseIng==true) return false;
	if (m_wRountID!=wRountID) return false;
	if (m_dwRecvPacketCount==0) return false;
	if (IsValidSocket()==false) return false;

	//Ѱ�ҷ��ͽṹ
	COverLappedSend * pOverLappedSend=GetSendOverLapped();
	//ASSERT(pOverLappedSend!=NULL);
	if (pOverLappedSend==NULL) return false;

	//��������
	CMD_Head * pHead=(CMD_Head *)pOverLappedSend->m_cbBuffer;
	pHead->CommandInfo.wMainCmdID=wMainCmdID;
	pHead->CommandInfo.wSubCmdID=wSubCmdID;
	WORD wSendSize=sizeof(CMD_Head);//EncryptBuffer(pOverLappedSend->m_cbBuffer,sizeof(CMD_Head),sizeof(pOverLappedSend->m_cbBuffer));
	pOverLappedSend->m_WSABuffer.len=wSendSize;

	//��������
	if (m_OverLappedSendActive.GetCount()==1)
	{
		DWORD dwThancferred=0;
		int iRetCode=WSASend(m_hSocket,&pOverLappedSend->m_WSABuffer,1,&dwThancferred,0,&pOverLappedSend->m_OverLapped,NULL);
		if ((iRetCode==SOCKET_ERROR)&&(WSAGetLastError()!=WSA_IO_PENDING))
		{
			OnSendCompleted(pOverLappedSend,0L);
			return false;
		}
	}

	return true;
}
//��ȡ���ͽṹ
COverLappedSend * CTCPSocketItem::GetSendOverLapped()
{
	//Ѱ�ҿ��нṹ
	COverLappedSend * pOverLappedSend=NULL;
	INT_PTR nFreeCount=m_OverLappedSendFree.GetCount();
	if (nFreeCount>0)
	{
		pOverLappedSend=m_OverLappedSendFree[nFreeCount-1];
		//ASSERT(pOverLappedSend!=NULL);
		m_OverLappedSendFree.RemoveAt(nFreeCount-1);
		m_OverLappedSendActive.Add(pOverLappedSend);
		return pOverLappedSend;
	}

	//�½����ͽṹ
	try
	{
		pOverLappedSend=new COverLappedSend;
		//ASSERT(pOverLappedSend!=NULL);
		m_OverLappedSendActive.Add(pOverLappedSend);
		return pOverLappedSend;
	}
	catch (...) { }
	return NULL;
}
////���ͺ���
//bool CTCPSocketItem::SendData(WORD wMainCmdID, WORD wSubCmdID, WORD wRountID)
//{
//	//Ч��״̬
//	if (m_bCloseIng==true) return false;
//	if (m_wRountID!=wRountID) return false;
//	if (m_dwRecvPacketCount==0) return false;
//	if (IsValidSocket()==false) return false;
//
//	//Ѱ�ҷ��ͽṹ
//	COverLappedSend * pOverLappedSend=GetSendOverLapped();
//	//ASSERT(pOverLappedSend!=NULL);
//	if (pOverLappedSend==NULL) return false;
//
//	//��������
//	CMD_Head * pHead=(CMD_Head *)pOverLappedSend->m_cbBuffer;
//	pHead->CommandInfo.wMainCmdID=wMainCmdID;
//	pHead->CommandInfo.wSubCmdID=wSubCmdID;
//	WORD wSendSize=sizeof(CMD_Head);//EncryptBuffer(pOverLappedSend->m_cbBuffer,sizeof(CMD_Head),sizeof(pOverLappedSend->m_cbBuffer));
//	pOverLappedSend->m_WSABuffer.len=wSendSize;
//
//	//��������
//	if (m_OverLappedSendActive.GetCount()==1)
//	{
//		DWORD dwThancferred=0;
//		int iRetCode=WSASend(m_hSocket,&pOverLappedSend->m_WSABuffer,1,&dwThancferred,0,&pOverLappedSend->m_OverLapped,NULL);
//		if ((iRetCode==SOCKET_ERROR)&&(WSAGetLastError()!=WSA_IO_PENDING))
//		{
//			OnSendCompleted(pOverLappedSend,0L);
//			return false;
//		}
//	}
//
//	return true;
//}

//���ͺ���
bool CTCPSocketItem::SendData(void * pData, WORD wDataSize, WORD wMainCmdID, WORD wSubCmdID, WORD wRountID)
{
	//Ч�����
	//ASSERT(wDataSize<=SOCKET_PACKAGE);

	//Ч��״̬
	if (m_bCloseIng==true) return false;
	if (m_wRountID!=wRountID) return false;
	if (m_dwRecvPacketCount==0) return false;
	if (IsValidSocket()==false) return false;
	if (wDataSize>SOCKET_PACKAGE) return false;

	//Ѱ�ҷ��ͽṹ
	COverLappedSend * pOverLappedSend=GetSendOverLapped();
	//ASSERT(pOverLappedSend!=NULL);
	if (pOverLappedSend==NULL) return false;

	//��������
	CMD_Head * pHead=(CMD_Head *)pOverLappedSend->m_cbBuffer;
	pHead->CommandInfo.wMainCmdID=wMainCmdID;
	pHead->CommandInfo.wSubCmdID=wSubCmdID;
	if (wDataSize>0)
	{
		//ASSERT(pData!=NULL);
		memcpy(pHead+1,pData,wDataSize);
	}
	WORD wSendSize=sizeof(CMD_Head)+wDataSize;//EncryptBuffer(pOverLappedSend->m_cbBuffer,sizeof(CMD_Head)+wDataSize,sizeof(pOverLappedSend->m_cbBuffer));
	pOverLappedSend->m_WSABuffer.len=wSendSize;

	//��������
	if (m_OverLappedSendActive.GetCount()==1)
	{
		DWORD dwThancferred=0;
		int iRetCode=WSASend(m_hSocket,&pOverLappedSend->m_WSABuffer,1,&dwThancferred,0,&pOverLappedSend->m_OverLapped,NULL);
		if ((iRetCode==SOCKET_ERROR)&&(WSAGetLastError()!=WSA_IO_PENDING))
		{
			OnSendCompleted(pOverLappedSend,0L);
			return false;
		}
	}

	return true;
}

//Ͷ�ݽ���
bool CTCPSocketItem::RecvData()
{
	//Ч�����
	//ASSERT(m_bRecvIng==false);
	//ASSERT(m_hSocket!=INVALID_SOCKET);

	//�жϹر�
	if (m_bCloseIng==true)
	{
		if (m_OverLappedSendActive.GetCount()==0) CloseSocket(m_wRountID);
		return false;
	}

	//��������
	m_bRecvIng=true;
	DWORD dwThancferred=0,dwFlags=0;
	int iRetCode=WSARecv(m_hSocket,&m_OverLappedRecv.m_WSABuffer,1,&dwThancferred,&dwFlags,&m_OverLappedRecv.m_OverLapped,NULL);
	if ((iRetCode==SOCKET_ERROR)&&(WSAGetLastError()!=WSA_IO_PENDING))
	{
		m_bRecvIng=false;
		CloseSocket(m_wRountID);
		return false;
	}

	return true;
}

//�ر�����
bool CTCPSocketItem::CloseSocket(WORD wRountID)
{
	//״̬�ж�
	if (m_wRountID!=wRountID) return false;

	//�ر�����
	if (m_hSocket!=INVALID_SOCKET)
	{
		closesocket(m_hSocket);
		m_hSocket=INVALID_SOCKET;
	}

	//�жϹر�
	if ((m_bRecvIng==false)&&(m_OverLappedSendActive.GetCount()==0)) OnCloseCompleted();

	return true;
}

//���ùر�
bool CTCPSocketItem::ShutDownSocket(WORD wRountID)
{
	return true;

	//״̬�ж�
	if (m_wRountID!=wRountID) return false;
	if (m_hSocket==INVALID_SOCKET) return false;

	//���ñ���
	if (m_bCloseIng==false)
	{
		m_bCloseIng=true;
		if (m_OverLappedSendActive.GetCount()==0) CloseSocket(wRountID);
	}

	return true;
}

//����Ⱥ��
bool CTCPSocketItem::AllowBatchSend(WORD wRountID, bool bAllowBatch)
{
	//״̬�ж�
	if (m_wRountID!=wRountID) return false;
	if (m_hSocket==INVALID_SOCKET) return false;

	//���ñ���
	m_bAllowBatch=bAllowBatch;

	return true;
}

//���ñ���
void CTCPSocketItem::ResetSocketData()
{
	//Ч��״̬
	//ASSERT(m_hSocket==INVALID_SOCKET);

	//��������
	m_wRountID++;
	m_wRecvSize=0;
	m_dwClientAddr=0;
	m_dwConnectTime=0;
	m_dwSendTickCount=0;
	m_dwRecvTickCount=0;
	m_dwSendPacketCount=0;
	m_dwRecvPacketCount=0;

	//״̬����
	m_bNotify=true;
	m_bRecvIng=false;
	m_bCloseIng=false;
	m_bAllowBatch=false;
	m_OverLappedSendFree.Append(m_OverLappedSendActive);
	m_OverLappedSendActive.RemoveAll();

	return;
}

//������ɺ���
bool CTCPSocketItem::OnSendCompleted(COverLappedSend * pOverLappedSend, DWORD dwThancferred)
{
	//Ч�����
	//ASSERT(pOverLappedSend!=NULL);
	//ASSERT(m_OverLappedSendActive.GetCount()>0);
	//ASSERT(pOverLappedSend==m_OverLappedSendActive[0]);

	//�ͷŷ��ͽṹ
	m_OverLappedSendActive.RemoveAt(0);
	m_OverLappedSendFree.Add(pOverLappedSend);

	//���ñ���
	if (dwThancferred!=0) m_dwSendTickCount=GetTickCount();

	//�жϹر�
	if (m_hSocket==INVALID_SOCKET)
	{
		m_OverLappedSendFree.Append(m_OverLappedSendActive);
		m_OverLappedSendActive.RemoveAll();
		CloseSocket(m_wRountID);
		return true;
	}

	//������������
	if (m_OverLappedSendActive.GetCount()>0)
	{
		DWORD dwThancferred=0;
		pOverLappedSend=m_OverLappedSendActive[0];
		//ASSERT(pOverLappedSend!=NULL);
		int iRetCode=WSASend(m_hSocket,&pOverLappedSend->m_WSABuffer,1,&dwThancferred,0,&pOverLappedSend->m_OverLapped,NULL);
		if ((iRetCode==SOCKET_ERROR)&&(WSAGetLastError()!=WSA_IO_PENDING))
		{
			m_OverLappedSendFree.Append(m_OverLappedSendActive);
			m_OverLappedSendActive.RemoveAll();
			CloseSocket(m_wRountID);
			return false;
		}
		return true;
	}

	//�жϹر�
	if (m_bCloseIng==true) 
		CloseSocket(m_wRountID);

	return true;
}

//������ɺ���
bool CTCPSocketItem::OnRecvCompleted(COverLappedRecv * pOverLappedRecv, DWORD dwThancferred)
{
	//Ч������
	//ASSERT(m_bRecvIng==true);

	//���ñ���
	m_bRecvIng=false;
	m_dwRecvTickCount=GetTickCount();

	//�жϹر�
	if (m_hSocket==INVALID_SOCKET)
	{
		CloseSocket(m_wRountID);
		return true;
	}

	//��������
	int iRetCode=recv(m_hSocket,(char *)m_cbRecvBuf+m_wRecvSize,sizeof(m_cbRecvBuf)-m_wRecvSize,0);
	if (iRetCode<=0)
	{
		CloseSocket(m_wRountID);
		return true;
	}

	//�������
	m_wRecvSize+=iRetCode;
	BYTE cbBuffer[SOCKET_BUFFER];
	CMD_Head * pHead=(CMD_Head *)m_cbRecvBuf;

	//��������
	try
	{
		while (m_wRecvSize>=sizeof(CMD_Head))
		{
			//Ч������
			WORD wPacketSize=pHead->CmdInfo.wDataSize;
			if (wPacketSize>SOCKET_BUFFER) throw TEXT("���ݰ�����");
			if (wPacketSize<sizeof(CMD_Head)) throw TEXT("���ݰ��Ƿ�");
			if (pHead->CmdInfo.cbMessageVer!=SOCKET_VER) throw TEXT("���ݰ��汾����");
			if (m_wRecvSize<wPacketSize) break;

			//��ȡ����
			CopyMemory(cbBuffer,m_cbRecvBuf,wPacketSize);
			WORD wRealySize=wPacketSize;//CrevasseBuffer(cbBuffer,wPacketSize);
			//ASSERT(wRealySize>=sizeof(CMD_Head));
			m_dwRecvPacketCount++;

			//��������
			WORD wDataSize=wRealySize-sizeof(CMD_Head);
			void * pDataBuffer=cbBuffer+sizeof(CMD_Head);
			CMD_Command Command=((CMD_Head *)cbBuffer)->CommandInfo;

			//�ں�����
			if (Command.wMainCmdID==MDM_KN_COMMAND)
			{
				switch (Command.wSubCmdID)
				{
				case SUB_KN_DETECT_SOCKET:	//������
					{
						break;
					}
				default: throw TEXT("�Ƿ�������");
				}
			}
			else 
			{
				//��Ϣ����
				m_pIServerSocketItemSink->OnSocketReadEvent(Command,pDataBuffer,wDataSize,this);			
			}

			//ɾ����������
			m_wRecvSize-=wPacketSize;
			MoveMemory(m_cbRecvBuf,m_cbRecvBuf+wPacketSize,m_wRecvSize);
		}
	}
	catch (...)
	{ 
		CloseSocket(m_wRountID);	
		return false;
	}

	return RecvData();
}

//�ر����֪ͨ
bool CTCPSocketItem::OnCloseCompleted()
{
	//Ч��״̬
	//ASSERT(m_hSocket==INVALID_SOCKET);
	//ASSERT(m_OverLappedSendActive.GetCount()==0);

	//�ر��¼�
	//ASSERT(m_bNotify==false);
	if (m_bNotify==false)
	{
		m_bNotify=true;
		m_pIServerSocketItemSink->OnSocketCloseEvent(this);
	}

	//״̬����
	ResetSocketData();

	return true;
}

//���캯��
CTCPSocketService::CTCPSocketService(void)
{
	m_bService=false;
	m_wListenPort=0;
	m_dwLastDetect=0;
	m_wMaxSocketItem=0;
	m_hCompletionPort=NULL;
	m_hServerSocket=INVALID_SOCKET;
#ifdef _DEBUG
	LOG(INFO) <<"CTCPSocketService";
#endif
	return;
}

//��������
CTCPSocketService::~CTCPSocketService(void)
{
	//ֹͣ����
	EndService();
#ifdef _DEBUG
	LOG(INFO) <<"~CTCPSocketService";
#endif
	//�ͷŴ洢����
	CTCPSocketItem * pSocketItem=NULL;
	for (INT_PTR i=0;i<m_StorageSocketItem.GetCount();i++)
	{
		pSocketItem=m_StorageSocketItem[i];
		//ASSERT(pSocketItem!=NULL);
		SafeDelete(pSocketItem);
	}
	m_StorageSocketItem.RemoveAll();

	return;
}



//���ýӿ�
bool __cdecl CTCPSocketService::SetSocketServiceSink(CObject * pIObject)
{
	//״̬�ж�
	if (m_bService==true) 
	{
		#ifdef _DEBUG
			LOG(INFO) <<"�������洦�ڷ���״̬���󶨲�������";
		#endif
		// LOG(INFO)<<(TEXT("�������洦�ڷ���״̬���󶨲�������"),Level_Exception);
		return false;
	}

	;
	//���ýӿ�
	if (m_MsgTriggerEvent.SetQueueService(dynamic_cast<IDataQueueSink*>(pIObject))==false)
	{
		#ifdef _DEBUG
			LOG(INFO) <<"���������봥�������ʧ��";
		#endif
		// LOG(INFO)<<(TEXT("���������봥�������ʧ��"),Level_Exception);
		return false;
	}
	
	return true;
}

//���ö˿�
bool __cdecl CTCPSocketService::SetServicePort(WORD wListenPort)
{
	//Ч��״̬
	if (m_bService==true) 
	{
		#ifdef _DEBUG
			LOG(INFO) <<"�������洦�ڷ���״̬���˿ڰ󶨲�������";
		#endif
		// LOG(INFO)<<(TEXT("�������洦�ڷ���״̬���˿ڰ󶨲�������"),Level_Exception);
		return false;
	}

	//�жϲ���
	if (wListenPort==0)
	{
		#ifdef _DEBUG
			LOG(INFO) <<"����˿ڴ��󣬶˿ڰ󶨲���ʧ��";
		#endif
		// LOG(INFO)<<(TEXT("����˿ڴ��󣬶˿ڰ󶨲���ʧ��"),Level_Exception);
		return false;
	}

	//���ñ���
	m_wListenPort=wListenPort;

	return true;
}

//������Ŀ
bool __cdecl CTCPSocketService::SetMaxSocketItem(WORD wMaxSocketItem)
{
	m_wMaxSocketItem=wMaxSocketItem;
	return true;
}

//��������
bool __cdecl CTCPSocketService::StartService()
{
	DWORD i = 0;
	//Ч��״̬
	if (m_bService==true) 
	{
		// LOG(INFO)<<(TEXT("���������ظ�������������������"),Level_Warning);
		return true;
	}

	//�ж϶˿�
	if (m_wListenPort==0)
	{
		#ifdef _DEBUG
			LOG(INFO) <<"������������˿���Ч";
		#endif
		// LOG(INFO)<<(TEXT("������������˿���Ч"),Level_Exception);
		return false;
	}

	//�󶨶���
	if (m_SendQueueService.SetDataQueueServiceSink(this)==false)
	{
		#ifdef _DEBUG
			LOG(INFO) <<"���������봥�������ʧ��";
		#endif
		// LOG(INFO)<<(TEXT("���������봥�������ʧ��"),Level_Exception);
		return false;
	}

	try
	{
		//��ȡϵͳ��Ϣ
		SYSTEM_INFO SystemInfo;
		GetSystemInfo(&SystemInfo);
		DWORD dwThreadCount=SystemInfo.dwNumberOfProcessors;

		//������ɶ˿�
		m_hCompletionPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,NULL,SystemInfo.dwNumberOfProcessors);
		if (m_hCompletionPort==NULL) throw TEXT("����������ɶ˿ڴ���ʧ��");

		//��������SOCKET
		struct sockaddr_in SocketAddr;
		memset(&SocketAddr,0,sizeof(SocketAddr));
		SocketAddr.sin_addr.s_addr=INADDR_ANY;
		SocketAddr.sin_family=AF_INET;
		SocketAddr.sin_port=htons(m_wListenPort);
		m_hServerSocket=WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,NULL,0,WSA_FLAG_OVERLAPPED);
		if (m_hServerSocket==INVALID_SOCKET) throw TEXT("����������� SOCKET ����ʧ��");
		int iErrorCode=bind(m_hServerSocket,(SOCKADDR*)&SocketAddr,sizeof(SocketAddr));
		if (iErrorCode==SOCKET_ERROR) throw TEXT("������������˿ڱ�ռ�ã��˿ڰ�ʧ��");
		iErrorCode=listen(m_hServerSocket,200);
		if (iErrorCode==SOCKET_ERROR) throw TEXT("������������˿ڱ�ռ�ã��˿ڼ���ʧ��");

		//�������Ͷ���
		bool bSuccess=m_SendQueueService.StartService();
		if (bSuccess==false) throw TEXT("�������淢�Ͷ��з�������ʧ��");

		//������д�߳�
		for (i=0;i<dwThreadCount;i++)
		{
			CServerSocketRSThread * pServerSocketRSThread=new CServerSocketRSThread();
			if (pServerSocketRSThread==NULL) throw TEXT("���������д�̷߳��񴴽�ʧ��");
			bSuccess=pServerSocketRSThread->InitThread(m_hCompletionPort);
			if (bSuccess==false) throw TEXT("���������д�̷߳�������ʧ��");
			m_SocketRSThreadArray.Add(pServerSocketRSThread);
		}

		//����Ӧ���߳�
		bSuccess=m_SocketAcceptThread.InitThread(m_hCompletionPort,m_hServerSocket,this);
		if (bSuccess==false) throw TEXT("����������������̷߳�������");

		//���ж�д�߳�
		for (i=0;i<dwThreadCount;i++)
		{
			CServerSocketRSThread * pServerSocketRSThread=m_SocketRSThreadArray[i];
			//ASSERT(pServerSocketRSThread!=NULL);
			bSuccess=pServerSocketRSThread->StartThead();
			if (bSuccess==false) throw TEXT("���������д�̷߳�������ʧ��");
		}

		//�������߳�
		m_SocketDetectThread.InitThread(this);
		bSuccess=m_SocketDetectThread.StartThead();
		if (bSuccess==false) throw TEXT("����������̷߳�������ʧ��");

		//����Ӧ���߳�
		bSuccess=m_SocketAcceptThread.StartThead();
		if (bSuccess==false) throw TEXT("������������̷߳�������ʧ��");

		//���ñ���
		m_bService=true;
	}
	catch (LPCTSTR pszError)
	{
		#ifdef _DEBUG
			LOG(INFO) <<pszError;
		#endif
		ShellExecute(NULL,"open",pszError,NULL,NULL,SW_SHOWNORMAL); 
		return false;
	}

	return true;
}

//ֹͣ����
bool __cdecl CTCPSocketService::EndService()
{
	//���ñ���
	m_bService=false;
	m_dwLastDetect=0L;

	//ֹͣ����߳�
	m_SocketDetectThread.EndThread();

	//��ֹӦ���߳�
	if (m_hServerSocket!=INVALID_SOCKET)
	{
		closesocket(m_hServerSocket);
		m_hServerSocket=INVALID_SOCKET;
	}
	m_SocketAcceptThread.EndThread();

	//ֹͣ���Ͷ���
	m_SendQueueService.EndService();

	//�ͷŶ�д�߳�
	INT_PTR nCount=m_SocketRSThreadArray.GetCount(),i=0;
	if (m_hCompletionPort!=NULL)
	{
		for (i=0;i<nCount;i++) PostQueuedCompletionStatus(m_hCompletionPort,0,NULL,NULL);
	}
	for (i=0;i<nCount;i++)
	{
		CServerSocketRSThread * pSocketThread=m_SocketRSThreadArray[i];
		//ASSERT(pSocketThread!=NULL);
		pSocketThread->EndThread();
		SafeDelete(pSocketThread);
	}
	m_SocketRSThreadArray.RemoveAll();

	//�ر���ɶ˿�
	if (m_hCompletionPort!=NULL)
	{
		CloseHandle(m_hCompletionPort);
		m_hCompletionPort=NULL;
	}

	//�ر�����
	CTCPSocketItem * pServerSocketItem=NULL;
	for (i=0;i<m_ActiveSocketItem.GetCount();i++)
	{
		pServerSocketItem=m_ActiveSocketItem[i];
		//ASSERT(pServerSocketItem!=NULL);
		pServerSocketItem->CloseSocket(pServerSocketItem->GetRountID());
		pServerSocketItem->ResetSocketData();
	}
	m_FreeSocketItem.Append(m_ActiveSocketItem);
	m_ActiveSocketItem.RemoveAll();

	return true;
}

//Ӧ����Ϣ
bool CTCPSocketService::OnSocketAcceptEvent(CTCPSocketItem * pServerSocketItem)
{
	//Ч������
	//ASSERT(pServerSocketItem!=NULL);
	////ASSERT(m_AttemperEvent.IsValid()==true);

	//Ͷ����Ϣ
	if (m_bService==false) return false;
	m_MsgTriggerEvent.PostSocketAcceptEvent(pServerSocketItem->GetIndex(),pServerSocketItem->GetRountID(),pServerSocketItem->GetClientAddr());

	return true;
}

//�����ȡ��Ϣ
bool CTCPSocketService::OnSocketReadEvent(CMD_Command Command, void * pBuffer, WORD wDataSize, CTCPSocketItem * pServerSocketItem)
{
	//Ч������
	//ASSERT(pServerSocketItem!=NULL);
	////ASSERT(m_AttemperEvent.IsValid()==true);

	//Ч��״̬
	if (m_bService==false) return false;
	m_MsgTriggerEvent.PostSocketReadEvent(pServerSocketItem->GetIndex(),pServerSocketItem->GetRountID(),Command,pBuffer,wDataSize);

	return true;
}

//����ر���Ϣ
bool CTCPSocketService::OnSocketCloseEvent(CTCPSocketItem * pServerSocketItem)
{
	//Ч�����
	//ASSERT(pServerSocketItem!=NULL);
	////ASSERT(m_AttemperEvent.IsValid()==true);

	try
	{
		//Ч��״̬
		if (m_bService==false) return false;

		//����ʱ��
		WORD wIndex=pServerSocketItem->GetIndex();
		WORD wRountID=pServerSocketItem->GetRountID();
		DWORD dwClientAddr=pServerSocketItem->GetClientAddr();
		DWORD dwConnectTime=pServerSocketItem->GetConnectTime();
		m_MsgTriggerEvent.PostSocketCloseEvent(wIndex,wRountID,dwClientAddr,dwConnectTime);

		//�ͷ�����
		FreeSocketItem(pServerSocketItem);
	}
	catch (...) {}

	return true;
}

//֪ͨ�ص����������Ͷ����̵߳��ã�
void __cdecl CTCPSocketService::OnDataQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize, DWORD dwInsertTime)
{
	switch (wIdentifier)
	{
	case QUEUE_SEND_REQUEST:		//��������
		{
			//Ч������
			tagSendDataRequest * pSendDataRequest=(tagSendDataRequest *)pBuffer;
			//ASSERT(wDataSize>=(sizeof(tagSendDataRequest)-sizeof(pSendDataRequest->cbSendBuf)));
			//ASSERT(wDataSize==(pSendDataRequest->wDataSize+sizeof(tagSendDataRequest)-sizeof(pSendDataRequest->cbSendBuf)));

			//��������
			if (pSendDataRequest->wIndex==INDEX_ALL_SOCKET)
			{
				//��ȡ���
				CThreadLockHandle ItemLockedHandle(&m_ItemLocked);
				m_TempSocketItem.Copy(m_ActiveSocketItem);
				ItemLockedHandle.UnLock();

				//ѭ����������
				CTCPSocketItem * pServerSocketItem=NULL;
				for (INT_PTR i=0;i<m_TempSocketItem.GetCount();i++)
				{
					pServerSocketItem=m_TempSocketItem[i];
					//ASSERT(pServerSocketItem!=NULL);
					CThreadLockHandle SocketLockHandle(pServerSocketItem->GetSignedLock());
					if (pServerSocketItem->IsAllowBatch())
					{
						pServerSocketItem->SendData(pSendDataRequest->cbSendBuf,pSendDataRequest->wDataSize,pSendDataRequest->wMainCmdID,
							pSendDataRequest->wSubCmdID,pServerSocketItem->GetRountID());
					}
				}
			}
			else
			{
				//�����
				CTCPSocketItem * pServerSocketItem=EnumSocketItem(pSendDataRequest->wIndex);
				CThreadLockHandle SocketLockHandle(pServerSocketItem->GetSignedLock());
				pServerSocketItem->SendData(pSendDataRequest->cbSendBuf,pSendDataRequest->wDataSize,pSendDataRequest->wMainCmdID,
					pSendDataRequest->wSubCmdID,pSendDataRequest->wRountID);
			}

			break;
		}
	case QUEUE_SAFE_CLOSE:		//��ȫ�ر�
		{
			//Ч������
			//ASSERT(wDataSize==sizeof(tagSafeCloseSocket));
			tagSafeCloseSocket * pSafeCloseSocket=(tagSafeCloseSocket *)pBuffer;

			//��ȫ�ر�
			CTCPSocketItem * pServerSocketItem=EnumSocketItem(pSafeCloseSocket->wIndex);
			CThreadLockHandle SocketLockHandle(pServerSocketItem->GetSignedLock());
			pServerSocketItem->ShutDownSocket(pSafeCloseSocket->wRountID);

			break;
		}
	case QUEUE_ALLOW_BATCH:		//����Ⱥ��
		{
			//Ч������
			//ASSERT(wDataSize==sizeof(tagAllowBatchSend));
			tagAllowBatchSend * pAllowBatchSend=(tagAllowBatchSend *)pBuffer;

			//����Ⱥ��
			CTCPSocketItem * pServerSocketItem=EnumSocketItem(pAllowBatchSend->wIndex);
			CThreadLockHandle SocketLockHandle(pServerSocketItem->GetSignedLock());
			pServerSocketItem->AllowBatchSend(pAllowBatchSend->wRountID,pAllowBatchSend->cbAllow?true:false);

			break;
		}
	case QUEUE_DETECT_SOCKET:	//�������
		{
			//��ȡ���
			CThreadLockHandle ItemLockedHandle(&m_ItemLocked);
			m_TempSocketItem.Copy(m_ActiveSocketItem);
			ItemLockedHandle.UnLock();

			//��������
			CMD_KN_DetectSocket DetectSocket;
			ZeroMemory(&DetectSocket,sizeof(DetectSocket));

			//��������
			WORD wRountID=0;
			DWORD dwNowTickCount=GetTickCount();
			DWORD dwBreakTickCount=__max(dwNowTickCount-m_dwLastDetect,TIME_BREAK_READY);

			//���ñ���
			m_dwLastDetect=GetTickCount();

			//�������
			for (INT_PTR i=0;i<m_TempSocketItem.GetCount();i++)
			{
				//��������
				CTCPSocketItem * pServerSocketItem=m_TempSocketItem[i];
				DWORD dwRecvTickCount=pServerSocketItem->GetRecvTickCount();
				CThreadLockHandle SocketLockHandle(pServerSocketItem->GetSignedLock());

				//������
				if (dwRecvTickCount>=dwNowTickCount) continue;

				//�������
				if (pServerSocketItem->IsReadySend()==true)
				{
					if ((dwNowTickCount-dwRecvTickCount)>dwBreakTickCount)
					{
						pServerSocketItem->CloseSocket(pServerSocketItem->GetRountID());
						continue;
					}
				}
				else
				{
					if ((dwNowTickCount-dwRecvTickCount)>TIME_BREAK_CONNECT)
					{
						pServerSocketItem->CloseSocket(pServerSocketItem->GetRountID());
						continue;
					}
				}

				//��������
				if (pServerSocketItem->IsReadySend()==true)
				{
					wRountID=pServerSocketItem->GetRountID();
					DetectSocket.dwSendTickCount=GetTickCount();
					pServerSocketItem->SendData(&DetectSocket,sizeof(DetectSocket),MDM_KN_COMMAND,SUB_KN_DETECT_SOCKET,wRountID);
				}
			}

			break;
		}
	default: { ;}//ASSERT(FALSE); }
	}

	return;
}

//��ȡ���ж���
CTCPSocketItem * CTCPSocketService::ActiveSocketItem()
{
	CThreadLockHandle ItemLockedHandle(&m_ItemLocked,true);

	//��ȡ���ж���
	CTCPSocketItem * pServerSocketItem=NULL;
	if (m_FreeSocketItem.GetCount()>0)
	{
		INT_PTR nItemPostion=m_FreeSocketItem.GetCount()-1;
		pServerSocketItem=m_FreeSocketItem[nItemPostion];
		//ASSERT(pServerSocketItem!=NULL);
		m_FreeSocketItem.RemoveAt(nItemPostion);
		m_ActiveSocketItem.Add(pServerSocketItem);
	}

	//�����¶���
	if (pServerSocketItem==NULL)
	{
		WORD wStorageCount=(WORD)m_StorageSocketItem.GetCount();
		if (wStorageCount<m_wMaxSocketItem)
		{
			try
			{
				pServerSocketItem=new CTCPSocketItem(wStorageCount,this);
				if (pServerSocketItem==NULL) return NULL;
				m_StorageSocketItem.Add(pServerSocketItem);
				m_ActiveSocketItem.Add(pServerSocketItem);
			}
			catch (...) { return NULL; }
		}
	}

	return pServerSocketItem;
}

//��ȡ���Ӷ���
CTCPSocketItem * CTCPSocketService::EnumSocketItem(WORD wIndex)
{
	CThreadLockHandle ItemLockedHandle(&m_ItemLocked,true);
	if (wIndex<m_StorageSocketItem.GetCount())
	{
		CTCPSocketItem * pServerSocketItem=m_StorageSocketItem[wIndex];
		//ASSERT(pServerSocketItem!=NULL);
		return pServerSocketItem;
	}
	return NULL;
}

//�ͷ����Ӷ���
bool CTCPSocketService::FreeSocketItem(CTCPSocketItem * pServerSocketItem)
{
	//Ч�����
	//ASSERT(pServerSocketItem!=NULL);

	//�ͷŶ���
	CThreadLockHandle ItemLockedHandle(&m_ItemLocked,true);
	INT_PTR nActiveCount=m_ActiveSocketItem.GetCount();
	for (int i=0;i<nActiveCount;i++)
	{
		if (pServerSocketItem==m_ActiveSocketItem[i])
		{
			m_ActiveSocketItem.RemoveAt(i);
			m_FreeSocketItem.Add(pServerSocketItem);
			return true;
		}
	}

	//�ͷ�ʧ��
	//ASSERT(FALSE);
	return false;
}

//�������
bool __cdecl CTCPSocketService::DetectSocket()
{
	return m_SendQueueService.AddToQueue(QUEUE_DETECT_SOCKET,NULL,0);
}


//���ͺ���
bool __cdecl CTCPSocketService::SendData(WORD wIndex, WORD wRountID, WORD wMainCmdID, WORD wSubCmdID)
{
	//Ч��״̬
	//ASSERT(m_bService==true);
	if (m_bService==false) return false;

	//��������
	tagSendDataRequest SendRequest;
	SendRequest.wMainCmdID=wMainCmdID;
	SendRequest.wSubCmdID=wSubCmdID;
	SendRequest.wIndex=wIndex;
	SendRequest.wRountID=wRountID;
	SendRequest.wDataSize=0;

	//���뷢�Ͷ���
	WORD wSendSize=sizeof(SendRequest)-sizeof(SendRequest.cbSendBuf);
	return m_SendQueueService.AddToQueue(QUEUE_SEND_REQUEST,&SendRequest,wSendSize);
}

//���ͺ���
bool __cdecl CTCPSocketService::SendData(WORD wIndex, WORD wRountID, WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	//Ч��״̬
	//ASSERT(m_bService==true);
	if (m_bService==false) return false;

	//Ч������
	//ASSERT((wDataSize+sizeof(CMD_Head))<=SOCKET_PACKAGE);
	if ((wDataSize+sizeof(CMD_Head))>SOCKET_PACKAGE) return false;

	//��������
	tagSendDataRequest SendRequest;
	SendRequest.wMainCmdID=wMainCmdID;
	SendRequest.wSubCmdID=wSubCmdID;
	SendRequest.wIndex=wIndex;
	SendRequest.wRountID=wRountID;
	SendRequest.wDataSize=wDataSize;
	if (wDataSize>0)
	{
		//ASSERT(pData!=NULL);
		CopyMemory(SendRequest.cbSendBuf,pData,wDataSize);
	}

	//���뷢�Ͷ���
	WORD wSendSize=sizeof(SendRequest)-sizeof(SendRequest.cbSendBuf)+wDataSize;
	return m_SendQueueService.AddToQueue(QUEUE_SEND_REQUEST,&SendRequest,wSendSize);
}

//��������
bool __cdecl CTCPSocketService::SendDataBatch(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	//Ч��״̬
	if (m_bService==false) return false;

	//Ч������
	//ASSERT((wDataSize+sizeof(CMD_Head))<=SOCKET_PACKAGE);
	if ((wDataSize+sizeof(CMD_Head))>SOCKET_PACKAGE) return false;

	//��������
	tagSendDataRequest SendRequest;
	SendRequest.wMainCmdID=wMainCmdID;
	SendRequest.wSubCmdID=wSubCmdID;
	SendRequest.wIndex=INDEX_ALL_SOCKET;
	SendRequest.wRountID=0;
	SendRequest.wDataSize=wDataSize;
	if (wDataSize>0)
	{
		//ASSERT(pData!=NULL);
		CopyMemory(SendRequest.cbSendBuf,pData,wDataSize);
	}

	//���뷢�Ͷ���
	WORD wSendSize=sizeof(SendRequest)-sizeof(SendRequest.cbSendBuf)+wDataSize;
	return m_SendQueueService.AddToQueue(QUEUE_SEND_REQUEST,&SendRequest,wSendSize);
}

//�ر�����
bool __cdecl CTCPSocketService::CloseSocket(WORD wIndex, WORD wRountID)
{
	CTCPSocketItem * pServerSocketItem=EnumSocketItem(wIndex);
	if (pServerSocketItem==NULL) return false;
	CThreadLockHandle SocketLockHandle(pServerSocketItem->GetSignedLock());
	return pServerSocketItem->CloseSocket(wRountID);
}

//���ùر�
bool __cdecl CTCPSocketService::ShutDownSocket(WORD wIndex, WORD wRountID)
{
	tagSafeCloseSocket SafeCloseSocket;
	SafeCloseSocket.wIndex=wIndex;
	SafeCloseSocket.wRountID=wRountID;
	return m_SendQueueService.AddToQueue(QUEUE_SAFE_CLOSE,&SafeCloseSocket,sizeof(SafeCloseSocket));
}

//����Ⱥ��
bool __cdecl CTCPSocketService::AllowBatchSend(WORD wIndex, WORD wRountID, bool bAllow)
{
	tagAllowBatchSend AllowBatchSend;
	AllowBatchSend.wIndex=wIndex;
	AllowBatchSend.wRountID=wRountID;
	AllowBatchSend.cbAllow=bAllow;
	return m_SendQueueService.AddToQueue(QUEUE_ALLOW_BATCH,&AllowBatchSend,sizeof(AllowBatchSend));
}
//////////////////////////////////////////////////////////////////////////

//���캯��
CServerSocketRSThread::CServerSocketRSThread(void)
{
	m_hCompletionPort=NULL;
}

//��������
CServerSocketRSThread::~CServerSocketRSThread(void)
{
}

//���ú���
bool CServerSocketRSThread::InitThread(HANDLE hCompletionPort)
{
	//ASSERT(hCompletionPort!=NULL);
	m_hCompletionPort=hCompletionPort;
	return true;
}

//���к���
bool CServerSocketRSThread::RepetitionRun()
{
	//Ч�����
	//ASSERT(m_hCompletionPort!=NULL);

	//��������
	DWORD dwThancferred=0;					
	OVERLAPPED * pOverLapped=NULL;
	COverLapped * pSocketLapped=NULL;
	CTCPSocketItem * pServerSocketItem=NULL;

	//�ȴ���ɶ˿�
	BOOL bSuccess=GetQueuedCompletionStatus(m_hCompletionPort,&dwThancferred,(PULONG_PTR)&pServerSocketItem,&pOverLapped,INFINITE);
	if ((bSuccess==FALSE)&&(GetLastError()!=ERROR_NETNAME_DELETED)) return false;
	if ((pServerSocketItem==NULL)&&(pOverLapped==NULL)) return false;

	//�������
	//ASSERT(pOverLapped!=NULL);
	//ASSERT(pServerSocketItem!=NULL);
	pSocketLapped=CONTAINING_RECORD(pOverLapped,COverLapped,m_OverLapped);
	switch (pSocketLapped->GetOperationType())
	{
	case OperationType_Recv:	//SOCKET ���ݶ�ȡ
		{
			COverLappedRecv * pOverLappedRecv=(COverLappedRecv *)pSocketLapped;
			CThreadLockHandle SocketLockHandle(pServerSocketItem->GetSignedLock());
			pServerSocketItem->OnRecvCompleted(pOverLappedRecv,dwThancferred);
			break;
		}
	case OperationType_Send:	//SOCKET ���ݷ���
		{
			COverLappedSend * pOverLappedSend=(COverLappedSend *)pSocketLapped;
			CThreadLockHandle SocketLockHandle(pServerSocketItem->GetSignedLock());
			pServerSocketItem->OnSendCompleted(pOverLappedSend,dwThancferred);
			break;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CSocketAcceptThread::CSocketAcceptThread(void)
{
	m_hCompletionPort=NULL;
	m_pTCPSocketManager=NULL;
	m_hListenSocket=INVALID_SOCKET;
#ifdef _DEBUG
	LOG(INFO) <<"CSocketAcceptThread";
#endif
}

//��������
CSocketAcceptThread::~CSocketAcceptThread(void)
{
#ifdef _DEBUG
	LOG(INFO) <<"~CSocketAcceptThread";
#endif
}

//���ú���
bool CSocketAcceptThread::InitThread(HANDLE hCompletionPort, SOCKET hListenSocket, CTCPSocketService * pTCPSocketManager)
{
	//ASSERT(hCompletionPort!=NULL);
	//ASSERT(pTCPSocketManager!=NULL);
	//ASSERT(hListenSocket!=INVALID_SOCKET);
	m_hListenSocket=hListenSocket;
	m_hCompletionPort=hCompletionPort;
	m_pTCPSocketManager=pTCPSocketManager;
	return true;
}

//���к���
bool CSocketAcceptThread::RepetitionRun()
{
	//Ч�����
	//ASSERT(m_hCompletionPort!=NULL);
	//ASSERT(m_pTCPSocketManager!=NULL);

	//���ñ���
	SOCKADDR_IN	SocketAddr;
	CTCPSocketItem * pServerSocketItem=NULL;
	SOCKET hConnectSocket=INVALID_SOCKET;
	int nBufferSize=sizeof(SocketAddr);

	try
	{
		//��������
		hConnectSocket=WSAAccept(m_hListenSocket,(SOCKADDR *)&SocketAddr,&nBufferSize,NULL,NULL);
		if (hConnectSocket==INVALID_SOCKET) return false;

		//��ȡ����
		pServerSocketItem=m_pTCPSocketManager->ActiveSocketItem();
		if (pServerSocketItem==NULL) throw TEXT("�������Ӷ���ʧ��");

		//�������
		CThreadLockHandle SocketLockHandle(pServerSocketItem->GetSignedLock());
		pServerSocketItem->Attach(hConnectSocket,SocketAddr.sin_addr.S_un.S_addr);
		CreateIoCompletionPort((HANDLE)hConnectSocket,m_hCompletionPort,(ULONG_PTR)pServerSocketItem,0);
		pServerSocketItem->RecvData();
	}
	catch (...)
	{
		#ifdef _DEBUG
			LOG(INFO) <<"CSocketAcceptThread throw";
		#endif
		if (hConnectSocket!=INVALID_SOCKET)	closesocket(hConnectSocket);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CSocketDetectThread::CSocketDetectThread(void)
{
	m_dwTickCount=0;;
	m_pTCPSocketManager=NULL;
#ifdef _DEBUG
	LOG(INFO) <<"CSocketDetectThread";
#endif
}

//��������
CSocketDetectThread::~CSocketDetectThread(void)
{
#ifdef _DEBUG
	LOG(INFO) <<"~CSocketDetectThread";
#endif
}

//���ú���
bool CSocketDetectThread::InitThread(CTCPSocketService * pTCPSocketManager)
{
	//Ч�����
	//ASSERT(pTCPSocketManager!=NULL);

	//���ñ���
	m_dwTickCount=0L;
	m_pTCPSocketManager=pTCPSocketManager;

	return true;
}

//���к���
bool CSocketDetectThread::RepetitionRun()
{
	//Ч�����
	//ASSERT(m_pTCPSocketManager!=NULL);

	//���ü��
	Sleep(500);
	m_dwTickCount+=500L;

	//�������
	if (m_dwTickCount>20000L)
	{
		m_dwTickCount=0L;
		m_pTCPSocketManager->DetectSocket();
	}

	return true;
}
