#include "StdAfx.h"
#include "SocketService.h"
#include <shellapi.h>
//////////////////////////////////////////////////////////////////////////
//宏定义

#define TIME_BREAK_READY			9000L								//中断时间
#define TIME_BREAK_CONNECT			4000L								//中断时间
#define TIME_DETECT_SOCKET			20000L								//监测时间

//////////////////////////////////////////////////////////////////////////

//动作定义
#define QUEUE_SEND_REQUEST			1									//发送标识
#define QUEUE_SAFE_CLOSE			2									//安全关闭
#define QUEUE_ALLOW_BATCH			3									//允许群发
#define QUEUE_DETECT_SOCKET			4									//检测连接

//发送请求结构
struct tagSendDataRequest
{
	WORD							wMainCmdID;							//主命令码
	WORD							wSubCmdID;							//子命令码
	WORD							wIndex;								//连接索引
	WORD							wRountID;							//循环索引
	WORD							wDataSize;							//数据大小
	BYTE							cbSendBuf[SOCKET_PACKAGE];			//发送缓冲
};

//设置群发
struct tagAllowBatchSend
{
	WORD							wIndex;								//连接索引
	WORD							wRountID;							//循环索引
	BYTE							cbAllow;							//允许标志
};

//安全关闭
struct tagSafeCloseSocket
{
	WORD							wIndex;								//连接索引
	WORD							wRountID;							//循环索引
};
//构造函数
COverLapped::COverLapped(enOperationType OperationType) : m_OperationType(OperationType)
{
	memset(&m_WSABuffer,0,sizeof(m_WSABuffer));
	memset(&m_OverLapped,0,sizeof(m_OverLapped));
}

//析构函数
COverLapped::~COverLapped()
{
}

//////////////////////////////////////////////////////////////////////////

//构造函数
COverLappedSend::COverLappedSend() : COverLapped(OperationType_Send)
{
	m_WSABuffer.len=0;
	m_WSABuffer.buf=(char *)m_cbBuffer;
}

//析构函数
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
	//设置变量
	m_bNotify=false;
	m_bRecvIng=false;
	m_bCloseIng=false;
	m_hSocket=hSocket;
	m_dwClientAddr=dwClientAddr;
	m_dwRecvTickCount=GetTickCount();
	m_dwConnectTime=(DWORD)time(NULL);

	//发送通知
	m_pIServerSocketItemSink->OnSocketAcceptEvent(this);

	return GetIdentifierID();
}
//发送函数
bool CTCPSocketItem::SendData(WORD wMainCmdID, WORD wSubCmdID, WORD wRountID)
{
	//效验状态
	if (m_bCloseIng==true) return false;
	if (m_wRountID!=wRountID) return false;
	if (m_dwRecvPacketCount==0) return false;
	if (IsValidSocket()==false) return false;

	//寻找发送结构
	COverLappedSend * pOverLappedSend=GetSendOverLapped();
	//ASSERT(pOverLappedSend!=NULL);
	if (pOverLappedSend==NULL) return false;

	//构造数据
	CMD_Head * pHead=(CMD_Head *)pOverLappedSend->m_cbBuffer;
	pHead->CommandInfo.wMainCmdID=wMainCmdID;
	pHead->CommandInfo.wSubCmdID=wSubCmdID;
	WORD wSendSize=sizeof(CMD_Head);//EncryptBuffer(pOverLappedSend->m_cbBuffer,sizeof(CMD_Head),sizeof(pOverLappedSend->m_cbBuffer));
	pOverLappedSend->m_WSABuffer.len=wSendSize;

	//发送数据
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
//获取发送结构
COverLappedSend * CTCPSocketItem::GetSendOverLapped()
{
	//寻找空闲结构
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

	//新建发送结构
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
////发送函数
//bool CTCPSocketItem::SendData(WORD wMainCmdID, WORD wSubCmdID, WORD wRountID)
//{
//	//效验状态
//	if (m_bCloseIng==true) return false;
//	if (m_wRountID!=wRountID) return false;
//	if (m_dwRecvPacketCount==0) return false;
//	if (IsValidSocket()==false) return false;
//
//	//寻找发送结构
//	COverLappedSend * pOverLappedSend=GetSendOverLapped();
//	//ASSERT(pOverLappedSend!=NULL);
//	if (pOverLappedSend==NULL) return false;
//
//	//构造数据
//	CMD_Head * pHead=(CMD_Head *)pOverLappedSend->m_cbBuffer;
//	pHead->CommandInfo.wMainCmdID=wMainCmdID;
//	pHead->CommandInfo.wSubCmdID=wSubCmdID;
//	WORD wSendSize=sizeof(CMD_Head);//EncryptBuffer(pOverLappedSend->m_cbBuffer,sizeof(CMD_Head),sizeof(pOverLappedSend->m_cbBuffer));
//	pOverLappedSend->m_WSABuffer.len=wSendSize;
//
//	//发送数据
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

//发送函数
bool CTCPSocketItem::SendData(void * pData, WORD wDataSize, WORD wMainCmdID, WORD wSubCmdID, WORD wRountID)
{
	//效验参数
	//ASSERT(wDataSize<=SOCKET_PACKAGE);

	//效验状态
	if (m_bCloseIng==true) return false;
	if (m_wRountID!=wRountID) return false;
	if (m_dwRecvPacketCount==0) return false;
	if (IsValidSocket()==false) return false;
	if (wDataSize>SOCKET_PACKAGE) return false;

	//寻找发送结构
	COverLappedSend * pOverLappedSend=GetSendOverLapped();
	//ASSERT(pOverLappedSend!=NULL);
	if (pOverLappedSend==NULL) return false;

	//构造数据
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

	//发送数据
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

//投递接收
bool CTCPSocketItem::RecvData()
{
	//效验变量
	//ASSERT(m_bRecvIng==false);
	//ASSERT(m_hSocket!=INVALID_SOCKET);

	//判断关闭
	if (m_bCloseIng==true)
	{
		if (m_OverLappedSendActive.GetCount()==0) CloseSocket(m_wRountID);
		return false;
	}

	//接收数据
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

//关闭连接
bool CTCPSocketItem::CloseSocket(WORD wRountID)
{
	//状态判断
	if (m_wRountID!=wRountID) return false;

	//关闭连接
	if (m_hSocket!=INVALID_SOCKET)
	{
		closesocket(m_hSocket);
		m_hSocket=INVALID_SOCKET;
	}

	//判断关闭
	if ((m_bRecvIng==false)&&(m_OverLappedSendActive.GetCount()==0)) OnCloseCompleted();

	return true;
}

//设置关闭
bool CTCPSocketItem::ShutDownSocket(WORD wRountID)
{
	return true;

	//状态判断
	if (m_wRountID!=wRountID) return false;
	if (m_hSocket==INVALID_SOCKET) return false;

	//设置变量
	if (m_bCloseIng==false)
	{
		m_bCloseIng=true;
		if (m_OverLappedSendActive.GetCount()==0) CloseSocket(wRountID);
	}

	return true;
}

//允许群发
bool CTCPSocketItem::AllowBatchSend(WORD wRountID, bool bAllowBatch)
{
	//状态判断
	if (m_wRountID!=wRountID) return false;
	if (m_hSocket==INVALID_SOCKET) return false;

	//设置变量
	m_bAllowBatch=bAllowBatch;

	return true;
}

//重置变量
void CTCPSocketItem::ResetSocketData()
{
	//效验状态
	//ASSERT(m_hSocket==INVALID_SOCKET);

	//重置数据
	m_wRountID++;
	m_wRecvSize=0;
	m_dwClientAddr=0;
	m_dwConnectTime=0;
	m_dwSendTickCount=0;
	m_dwRecvTickCount=0;
	m_dwSendPacketCount=0;
	m_dwRecvPacketCount=0;

	//状态变量
	m_bNotify=true;
	m_bRecvIng=false;
	m_bCloseIng=false;
	m_bAllowBatch=false;
	m_OverLappedSendFree.Append(m_OverLappedSendActive);
	m_OverLappedSendActive.RemoveAll();

	return;
}

//发送完成函数
bool CTCPSocketItem::OnSendCompleted(COverLappedSend * pOverLappedSend, DWORD dwThancferred)
{
	//效验变量
	//ASSERT(pOverLappedSend!=NULL);
	//ASSERT(m_OverLappedSendActive.GetCount()>0);
	//ASSERT(pOverLappedSend==m_OverLappedSendActive[0]);

	//释放发送结构
	m_OverLappedSendActive.RemoveAt(0);
	m_OverLappedSendFree.Add(pOverLappedSend);

	//设置变量
	if (dwThancferred!=0) m_dwSendTickCount=GetTickCount();

	//判断关闭
	if (m_hSocket==INVALID_SOCKET)
	{
		m_OverLappedSendFree.Append(m_OverLappedSendActive);
		m_OverLappedSendActive.RemoveAll();
		CloseSocket(m_wRountID);
		return true;
	}

	//继续发送数据
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

	//判断关闭
	if (m_bCloseIng==true) 
		CloseSocket(m_wRountID);

	return true;
}

//接收完成函数
bool CTCPSocketItem::OnRecvCompleted(COverLappedRecv * pOverLappedRecv, DWORD dwThancferred)
{
	//效验数据
	//ASSERT(m_bRecvIng==true);

	//设置变量
	m_bRecvIng=false;
	m_dwRecvTickCount=GetTickCount();

	//判断关闭
	if (m_hSocket==INVALID_SOCKET)
	{
		CloseSocket(m_wRountID);
		return true;
	}

	//接收数据
	int iRetCode=recv(m_hSocket,(char *)m_cbRecvBuf+m_wRecvSize,sizeof(m_cbRecvBuf)-m_wRecvSize,0);
	if (iRetCode<=0)
	{
		CloseSocket(m_wRountID);
		return true;
	}

	//接收完成
	m_wRecvSize+=iRetCode;
	BYTE cbBuffer[SOCKET_BUFFER];
	CMD_Head * pHead=(CMD_Head *)m_cbRecvBuf;

	//处理数据
	try
	{
		while (m_wRecvSize>=sizeof(CMD_Head))
		{
			//效验数据
			WORD wPacketSize=pHead->CmdInfo.wDataSize;
			if (wPacketSize>SOCKET_BUFFER) throw TEXT("数据包超长");
			if (wPacketSize<sizeof(CMD_Head)) throw TEXT("数据包非法");
			if (pHead->CmdInfo.cbMessageVer!=SOCKET_VER) throw TEXT("数据包版本错误");
			if (m_wRecvSize<wPacketSize) break;

			//提取数据
			CopyMemory(cbBuffer,m_cbRecvBuf,wPacketSize);
			WORD wRealySize=wPacketSize;//CrevasseBuffer(cbBuffer,wPacketSize);
			//ASSERT(wRealySize>=sizeof(CMD_Head));
			m_dwRecvPacketCount++;

			//解释数据
			WORD wDataSize=wRealySize-sizeof(CMD_Head);
			void * pDataBuffer=cbBuffer+sizeof(CMD_Head);
			CMD_Command Command=((CMD_Head *)cbBuffer)->CommandInfo;

			//内核命令
			if (Command.wMainCmdID==MDM_KN_COMMAND)
			{
				switch (Command.wSubCmdID)
				{
				case SUB_KN_DETECT_SOCKET:	//网络检测
					{
						break;
					}
				default: throw TEXT("非法命令码");
				}
			}
			else 
			{
				//消息处理
				m_pIServerSocketItemSink->OnSocketReadEvent(Command,pDataBuffer,wDataSize,this);			
			}

			//删除缓存数据
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

//关闭完成通知
bool CTCPSocketItem::OnCloseCompleted()
{
	//效验状态
	//ASSERT(m_hSocket==INVALID_SOCKET);
	//ASSERT(m_OverLappedSendActive.GetCount()==0);

	//关闭事件
	//ASSERT(m_bNotify==false);
	if (m_bNotify==false)
	{
		m_bNotify=true;
		m_pIServerSocketItemSink->OnSocketCloseEvent(this);
	}

	//状态变量
	ResetSocketData();

	return true;
}

//构造函数
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

//析构函数
CTCPSocketService::~CTCPSocketService(void)
{
	//停止服务
	EndService();
#ifdef _DEBUG
	LOG(INFO) <<"~CTCPSocketService";
#endif
	//释放存储连接
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



//设置接口
bool __cdecl CTCPSocketService::SetSocketServiceSink(CObject * pIObject)
{
	//状态判断
	if (m_bService==true) 
	{
		#ifdef _DEBUG
			LOG(INFO) <<"网络引擎处于服务状态，绑定操作忽略";
		#endif
		// LOG(INFO)<<(TEXT("网络引擎处于服务状态，绑定操作忽略"),Level_Exception);
		return false;
	}

	;
	//设置接口
	if (m_MsgTriggerEvent.SetQueueService(dynamic_cast<IDataQueueSink*>(pIObject))==false)
	{
		#ifdef _DEBUG
			LOG(INFO) <<"网络引擎与触发服务绑定失败";
		#endif
		// LOG(INFO)<<(TEXT("网络引擎与触发服务绑定失败"),Level_Exception);
		return false;
	}
	
	return true;
}

//设置端口
bool __cdecl CTCPSocketService::SetServicePort(WORD wListenPort)
{
	//效验状态
	if (m_bService==true) 
	{
		#ifdef _DEBUG
			LOG(INFO) <<"网络引擎处于服务状态，端口绑定操作忽略";
		#endif
		// LOG(INFO)<<(TEXT("网络引擎处于服务状态，端口绑定操作忽略"),Level_Exception);
		return false;
	}

	//判断参数
	if (wListenPort==0)
	{
		#ifdef _DEBUG
			LOG(INFO) <<"网络端口错误，端口绑定操作失败";
		#endif
		// LOG(INFO)<<(TEXT("网络端口错误，端口绑定操作失败"),Level_Exception);
		return false;
	}

	//设置变量
	m_wListenPort=wListenPort;

	return true;
}

//设置数目
bool __cdecl CTCPSocketService::SetMaxSocketItem(WORD wMaxSocketItem)
{
	m_wMaxSocketItem=wMaxSocketItem;
	return true;
}

//启动服务
bool __cdecl CTCPSocketService::StartService()
{
	DWORD i = 0;
	//效验状态
	if (m_bService==true) 
	{
		// LOG(INFO)<<(TEXT("网络引擎重复启动，启动操作忽略"),Level_Warning);
		return true;
	}

	//判断端口
	if (m_wListenPort==0)
	{
		#ifdef _DEBUG
			LOG(INFO) <<"网络引擎监听端口无效";
		#endif
		// LOG(INFO)<<(TEXT("网络引擎监听端口无效"),Level_Exception);
		return false;
	}

	//绑定对象
	if (m_SendQueueService.SetDataQueueServiceSink(this)==false)
	{
		#ifdef _DEBUG
			LOG(INFO) <<"网络引擎与触发服务绑定失败";
		#endif
		// LOG(INFO)<<(TEXT("网络引擎与触发服务绑定失败"),Level_Exception);
		return false;
	}

	try
	{
		//获取系统信息
		SYSTEM_INFO SystemInfo;
		GetSystemInfo(&SystemInfo);
		DWORD dwThreadCount=SystemInfo.dwNumberOfProcessors;

		//建立完成端口
		m_hCompletionPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,NULL,SystemInfo.dwNumberOfProcessors);
		if (m_hCompletionPort==NULL) throw TEXT("网络引擎完成端口创建失败");

		//建立监听SOCKET
		struct sockaddr_in SocketAddr;
		memset(&SocketAddr,0,sizeof(SocketAddr));
		SocketAddr.sin_addr.s_addr=INADDR_ANY;
		SocketAddr.sin_family=AF_INET;
		SocketAddr.sin_port=htons(m_wListenPort);
		m_hServerSocket=WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,NULL,0,WSA_FLAG_OVERLAPPED);
		if (m_hServerSocket==INVALID_SOCKET) throw TEXT("网络引擎监听 SOCKET 创建失败");
		int iErrorCode=bind(m_hServerSocket,(SOCKADDR*)&SocketAddr,sizeof(SocketAddr));
		if (iErrorCode==SOCKET_ERROR) throw TEXT("网络引擎监听端口被占用，端口绑定失败");
		iErrorCode=listen(m_hServerSocket,200);
		if (iErrorCode==SOCKET_ERROR) throw TEXT("网络引擎监听端口被占用，端口监听失败");

		//启动发送队列
		bool bSuccess=m_SendQueueService.StartService();
		if (bSuccess==false) throw TEXT("网络引擎发送队列服务启动失败");

		//建立读写线程
		for (i=0;i<dwThreadCount;i++)
		{
			CServerSocketRSThread * pServerSocketRSThread=new CServerSocketRSThread();
			if (pServerSocketRSThread==NULL) throw TEXT("网络引擎读写线程服务创建失败");
			bSuccess=pServerSocketRSThread->InitThread(m_hCompletionPort);
			if (bSuccess==false) throw TEXT("网络引擎读写线程服务配置失败");
			m_SocketRSThreadArray.Add(pServerSocketRSThread);
		}

		//建立应答线程
		bSuccess=m_SocketAcceptThread.InitThread(m_hCompletionPort,m_hServerSocket,this);
		if (bSuccess==false) throw TEXT("网络引擎网络监听线程服务配置");

		//运行读写线程
		for (i=0;i<dwThreadCount;i++)
		{
			CServerSocketRSThread * pServerSocketRSThread=m_SocketRSThreadArray[i];
			//ASSERT(pServerSocketRSThread!=NULL);
			bSuccess=pServerSocketRSThread->StartThead();
			if (bSuccess==false) throw TEXT("网络引擎读写线程服务启动失败");
		}

		//网络检测线程
		m_SocketDetectThread.InitThread(this);
		bSuccess=m_SocketDetectThread.StartThead();
		if (bSuccess==false) throw TEXT("网络引检测线程服务启动失败");

		//运行应答线程
		bSuccess=m_SocketAcceptThread.StartThead();
		if (bSuccess==false) throw TEXT("网络引擎监听线程服务启动失败");

		//设置变量
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

//停止服务
bool __cdecl CTCPSocketService::EndService()
{
	//设置变量
	m_bService=false;
	m_dwLastDetect=0L;

	//停止检测线程
	m_SocketDetectThread.EndThread();

	//终止应答线程
	if (m_hServerSocket!=INVALID_SOCKET)
	{
		closesocket(m_hServerSocket);
		m_hServerSocket=INVALID_SOCKET;
	}
	m_SocketAcceptThread.EndThread();

	//停止发送队列
	m_SendQueueService.EndService();

	//释放读写线程
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

	//关闭完成端口
	if (m_hCompletionPort!=NULL)
	{
		CloseHandle(m_hCompletionPort);
		m_hCompletionPort=NULL;
	}

	//关闭连接
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

//应答消息
bool CTCPSocketService::OnSocketAcceptEvent(CTCPSocketItem * pServerSocketItem)
{
	//效验数据
	//ASSERT(pServerSocketItem!=NULL);
	////ASSERT(m_AttemperEvent.IsValid()==true);

	//投递消息
	if (m_bService==false) return false;
	m_MsgTriggerEvent.PostSocketAcceptEvent(pServerSocketItem->GetIndex(),pServerSocketItem->GetRountID(),pServerSocketItem->GetClientAddr());

	return true;
}

//网络读取消息
bool CTCPSocketService::OnSocketReadEvent(CMD_Command Command, void * pBuffer, WORD wDataSize, CTCPSocketItem * pServerSocketItem)
{
	//效验数据
	//ASSERT(pServerSocketItem!=NULL);
	////ASSERT(m_AttemperEvent.IsValid()==true);

	//效验状态
	if (m_bService==false) return false;
	m_MsgTriggerEvent.PostSocketReadEvent(pServerSocketItem->GetIndex(),pServerSocketItem->GetRountID(),Command,pBuffer,wDataSize);

	return true;
}

//网络关闭消息
bool CTCPSocketService::OnSocketCloseEvent(CTCPSocketItem * pServerSocketItem)
{
	//效验参数
	//ASSERT(pServerSocketItem!=NULL);
	////ASSERT(m_AttemperEvent.IsValid()==true);

	try
	{
		//效验状态
		if (m_bService==false) return false;

		//计算时间
		WORD wIndex=pServerSocketItem->GetIndex();
		WORD wRountID=pServerSocketItem->GetRountID();
		DWORD dwClientAddr=pServerSocketItem->GetClientAddr();
		DWORD dwConnectTime=pServerSocketItem->GetConnectTime();
		m_MsgTriggerEvent.PostSocketCloseEvent(wIndex,wRountID,dwClientAddr,dwConnectTime);

		//释放连接
		FreeSocketItem(pServerSocketItem);
	}
	catch (...) {}

	return true;
}

//通知回调函数（发送队列线程调用）
void __cdecl CTCPSocketService::OnDataQueueServiceSink(WORD wIdentifier, void * pBuffer, WORD wDataSize, DWORD dwInsertTime)
{
	switch (wIdentifier)
	{
	case QUEUE_SEND_REQUEST:		//发送请求
		{
			//效验数据
			tagSendDataRequest * pSendDataRequest=(tagSendDataRequest *)pBuffer;
			//ASSERT(wDataSize>=(sizeof(tagSendDataRequest)-sizeof(pSendDataRequest->cbSendBuf)));
			//ASSERT(wDataSize==(pSendDataRequest->wDataSize+sizeof(tagSendDataRequest)-sizeof(pSendDataRequest->cbSendBuf)));

			//发送数据
			if (pSendDataRequest->wIndex==INDEX_ALL_SOCKET)
			{
				//获取活动项
				CThreadLockHandle ItemLockedHandle(&m_ItemLocked);
				m_TempSocketItem.Copy(m_ActiveSocketItem);
				ItemLockedHandle.UnLock();

				//循环发送数据
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
				//单项发送
				CTCPSocketItem * pServerSocketItem=EnumSocketItem(pSendDataRequest->wIndex);
				CThreadLockHandle SocketLockHandle(pServerSocketItem->GetSignedLock());
				pServerSocketItem->SendData(pSendDataRequest->cbSendBuf,pSendDataRequest->wDataSize,pSendDataRequest->wMainCmdID,
					pSendDataRequest->wSubCmdID,pSendDataRequest->wRountID);
			}

			break;
		}
	case QUEUE_SAFE_CLOSE:		//安全关闭
		{
			//效验数据
			//ASSERT(wDataSize==sizeof(tagSafeCloseSocket));
			tagSafeCloseSocket * pSafeCloseSocket=(tagSafeCloseSocket *)pBuffer;

			//安全关闭
			CTCPSocketItem * pServerSocketItem=EnumSocketItem(pSafeCloseSocket->wIndex);
			CThreadLockHandle SocketLockHandle(pServerSocketItem->GetSignedLock());
			pServerSocketItem->ShutDownSocket(pSafeCloseSocket->wRountID);

			break;
		}
	case QUEUE_ALLOW_BATCH:		//允许群发
		{
			//效验数据
			//ASSERT(wDataSize==sizeof(tagAllowBatchSend));
			tagAllowBatchSend * pAllowBatchSend=(tagAllowBatchSend *)pBuffer;

			//设置群发
			CTCPSocketItem * pServerSocketItem=EnumSocketItem(pAllowBatchSend->wIndex);
			CThreadLockHandle SocketLockHandle(pServerSocketItem->GetSignedLock());
			pServerSocketItem->AllowBatchSend(pAllowBatchSend->wRountID,pAllowBatchSend->cbAllow?true:false);

			break;
		}
	case QUEUE_DETECT_SOCKET:	//检测连接
		{
			//获取活动项
			CThreadLockHandle ItemLockedHandle(&m_ItemLocked);
			m_TempSocketItem.Copy(m_ActiveSocketItem);
			ItemLockedHandle.UnLock();

			//构造数据
			CMD_KN_DetectSocket DetectSocket;
			ZeroMemory(&DetectSocket,sizeof(DetectSocket));

			//变量定义
			WORD wRountID=0;
			DWORD dwNowTickCount=GetTickCount();
			DWORD dwBreakTickCount=__max(dwNowTickCount-m_dwLastDetect,TIME_BREAK_READY);

			//设置变量
			m_dwLastDetect=GetTickCount();

			//检测连接
			for (INT_PTR i=0;i<m_TempSocketItem.GetCount();i++)
			{
				//变量定义
				CTCPSocketItem * pServerSocketItem=m_TempSocketItem[i];
				DWORD dwRecvTickCount=pServerSocketItem->GetRecvTickCount();
				CThreadLockHandle SocketLockHandle(pServerSocketItem->GetSignedLock());

				//间隔检查
				if (dwRecvTickCount>=dwNowTickCount) continue;

				//检测连接
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

				//发送数据
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

//获取空闲对象
CTCPSocketItem * CTCPSocketService::ActiveSocketItem()
{
	CThreadLockHandle ItemLockedHandle(&m_ItemLocked,true);

	//获取空闲对象
	CTCPSocketItem * pServerSocketItem=NULL;
	if (m_FreeSocketItem.GetCount()>0)
	{
		INT_PTR nItemPostion=m_FreeSocketItem.GetCount()-1;
		pServerSocketItem=m_FreeSocketItem[nItemPostion];
		//ASSERT(pServerSocketItem!=NULL);
		m_FreeSocketItem.RemoveAt(nItemPostion);
		m_ActiveSocketItem.Add(pServerSocketItem);
	}

	//创建新对象
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

//获取连接对象
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

//释放连接对象
bool CTCPSocketService::FreeSocketItem(CTCPSocketItem * pServerSocketItem)
{
	//效验参数
	//ASSERT(pServerSocketItem!=NULL);

	//释放对象
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

	//释放失败
	//ASSERT(FALSE);
	return false;
}

//检测连接
bool __cdecl CTCPSocketService::DetectSocket()
{
	return m_SendQueueService.AddToQueue(QUEUE_DETECT_SOCKET,NULL,0);
}


//发送函数
bool __cdecl CTCPSocketService::SendData(WORD wIndex, WORD wRountID, WORD wMainCmdID, WORD wSubCmdID)
{
	//效益状态
	//ASSERT(m_bService==true);
	if (m_bService==false) return false;

	//构造数据
	tagSendDataRequest SendRequest;
	SendRequest.wMainCmdID=wMainCmdID;
	SendRequest.wSubCmdID=wSubCmdID;
	SendRequest.wIndex=wIndex;
	SendRequest.wRountID=wRountID;
	SendRequest.wDataSize=0;

	//加入发送队列
	WORD wSendSize=sizeof(SendRequest)-sizeof(SendRequest.cbSendBuf);
	return m_SendQueueService.AddToQueue(QUEUE_SEND_REQUEST,&SendRequest,wSendSize);
}

//发送函数
bool __cdecl CTCPSocketService::SendData(WORD wIndex, WORD wRountID, WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	//效益状态
	//ASSERT(m_bService==true);
	if (m_bService==false) return false;

	//效益数据
	//ASSERT((wDataSize+sizeof(CMD_Head))<=SOCKET_PACKAGE);
	if ((wDataSize+sizeof(CMD_Head))>SOCKET_PACKAGE) return false;

	//构造数据
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

	//加入发送队列
	WORD wSendSize=sizeof(SendRequest)-sizeof(SendRequest.cbSendBuf)+wDataSize;
	return m_SendQueueService.AddToQueue(QUEUE_SEND_REQUEST,&SendRequest,wSendSize);
}

//批量发送
bool __cdecl CTCPSocketService::SendDataBatch(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	//效益状态
	if (m_bService==false) return false;

	//效益数据
	//ASSERT((wDataSize+sizeof(CMD_Head))<=SOCKET_PACKAGE);
	if ((wDataSize+sizeof(CMD_Head))>SOCKET_PACKAGE) return false;

	//构造数据
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

	//加入发送队列
	WORD wSendSize=sizeof(SendRequest)-sizeof(SendRequest.cbSendBuf)+wDataSize;
	return m_SendQueueService.AddToQueue(QUEUE_SEND_REQUEST,&SendRequest,wSendSize);
}

//关闭连接
bool __cdecl CTCPSocketService::CloseSocket(WORD wIndex, WORD wRountID)
{
	CTCPSocketItem * pServerSocketItem=EnumSocketItem(wIndex);
	if (pServerSocketItem==NULL) return false;
	CThreadLockHandle SocketLockHandle(pServerSocketItem->GetSignedLock());
	return pServerSocketItem->CloseSocket(wRountID);
}

//设置关闭
bool __cdecl CTCPSocketService::ShutDownSocket(WORD wIndex, WORD wRountID)
{
	tagSafeCloseSocket SafeCloseSocket;
	SafeCloseSocket.wIndex=wIndex;
	SafeCloseSocket.wRountID=wRountID;
	return m_SendQueueService.AddToQueue(QUEUE_SAFE_CLOSE,&SafeCloseSocket,sizeof(SafeCloseSocket));
}

//允许群发
bool __cdecl CTCPSocketService::AllowBatchSend(WORD wIndex, WORD wRountID, bool bAllow)
{
	tagAllowBatchSend AllowBatchSend;
	AllowBatchSend.wIndex=wIndex;
	AllowBatchSend.wRountID=wRountID;
	AllowBatchSend.cbAllow=bAllow;
	return m_SendQueueService.AddToQueue(QUEUE_ALLOW_BATCH,&AllowBatchSend,sizeof(AllowBatchSend));
}
//////////////////////////////////////////////////////////////////////////

//构造函数
CServerSocketRSThread::CServerSocketRSThread(void)
{
	m_hCompletionPort=NULL;
}

//析构函数
CServerSocketRSThread::~CServerSocketRSThread(void)
{
}

//配置函数
bool CServerSocketRSThread::InitThread(HANDLE hCompletionPort)
{
	//ASSERT(hCompletionPort!=NULL);
	m_hCompletionPort=hCompletionPort;
	return true;
}

//运行函数
bool CServerSocketRSThread::RepetitionRun()
{
	//效验参数
	//ASSERT(m_hCompletionPort!=NULL);

	//变量定义
	DWORD dwThancferred=0;					
	OVERLAPPED * pOverLapped=NULL;
	COverLapped * pSocketLapped=NULL;
	CTCPSocketItem * pServerSocketItem=NULL;

	//等待完成端口
	BOOL bSuccess=GetQueuedCompletionStatus(m_hCompletionPort,&dwThancferred,(PULONG_PTR)&pServerSocketItem,&pOverLapped,INFINITE);
	if ((bSuccess==FALSE)&&(GetLastError()!=ERROR_NETNAME_DELETED)) return false;
	if ((pServerSocketItem==NULL)&&(pOverLapped==NULL)) return false;

	//处理操作
	//ASSERT(pOverLapped!=NULL);
	//ASSERT(pServerSocketItem!=NULL);
	pSocketLapped=CONTAINING_RECORD(pOverLapped,COverLapped,m_OverLapped);
	switch (pSocketLapped->GetOperationType())
	{
	case OperationType_Recv:	//SOCKET 数据读取
		{
			COverLappedRecv * pOverLappedRecv=(COverLappedRecv *)pSocketLapped;
			CThreadLockHandle SocketLockHandle(pServerSocketItem->GetSignedLock());
			pServerSocketItem->OnRecvCompleted(pOverLappedRecv,dwThancferred);
			break;
		}
	case OperationType_Send:	//SOCKET 数据发送
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

//构造函数
CSocketAcceptThread::CSocketAcceptThread(void)
{
	m_hCompletionPort=NULL;
	m_pTCPSocketManager=NULL;
	m_hListenSocket=INVALID_SOCKET;
#ifdef _DEBUG
	LOG(INFO) <<"CSocketAcceptThread";
#endif
}

//析构函数
CSocketAcceptThread::~CSocketAcceptThread(void)
{
#ifdef _DEBUG
	LOG(INFO) <<"~CSocketAcceptThread";
#endif
}

//配置函数
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

//运行函数
bool CSocketAcceptThread::RepetitionRun()
{
	//效验参数
	//ASSERT(m_hCompletionPort!=NULL);
	//ASSERT(m_pTCPSocketManager!=NULL);

	//设置变量
	SOCKADDR_IN	SocketAddr;
	CTCPSocketItem * pServerSocketItem=NULL;
	SOCKET hConnectSocket=INVALID_SOCKET;
	int nBufferSize=sizeof(SocketAddr);

	try
	{
		//监听连接
		hConnectSocket=WSAAccept(m_hListenSocket,(SOCKADDR *)&SocketAddr,&nBufferSize,NULL,NULL);
		if (hConnectSocket==INVALID_SOCKET) return false;

		//获取连接
		pServerSocketItem=m_pTCPSocketManager->ActiveSocketItem();
		if (pServerSocketItem==NULL) throw TEXT("申请连接对象失败");

		//激活对象
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

//构造函数
CSocketDetectThread::CSocketDetectThread(void)
{
	m_dwTickCount=0;;
	m_pTCPSocketManager=NULL;
#ifdef _DEBUG
	LOG(INFO) <<"CSocketDetectThread";
#endif
}

//析构函数
CSocketDetectThread::~CSocketDetectThread(void)
{
#ifdef _DEBUG
	LOG(INFO) <<"~CSocketDetectThread";
#endif
}

//配置函数
bool CSocketDetectThread::InitThread(CTCPSocketService * pTCPSocketManager)
{
	//效验参数
	//ASSERT(pTCPSocketManager!=NULL);

	//设置变量
	m_dwTickCount=0L;
	m_pTCPSocketManager=pTCPSocketManager;

	return true;
}

//运行函数
bool CSocketDetectThread::RepetitionRun()
{
	//效验参数
	//ASSERT(m_pTCPSocketManager!=NULL);

	//设置间隔
	Sleep(500);
	m_dwTickCount+=500L;

	//检测连接
	if (m_dwTickCount>20000L)
	{
		m_dwTickCount=0L;
		m_pTCPSocketManager->DetectSocket();
	}

	return true;
}
