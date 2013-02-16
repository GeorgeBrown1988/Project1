#pragma once
/////////////////////////////////////////////////////////////////////////////////////////
//负荷信息
struct tagBurthenInfo
{
    long							dwDataSize;							//数据大小
    long							dwBufferSize;						//缓冲区长度
    long							dwDataPacketCount;					//数据包数目
};
//数据队列头
struct tagDataHead
{
    int 							wDataSize;							//数据大小
    int 							wIdentifier;						//类型标识
    long							dwInsertTime;						//插入时间
};

//数据包结构信息
struct CMD_Info
{
    int 								wDataSize;							//数据大小
    unsigned char						cbCheckCode;						//效验字段
    unsigned char						cbMessageVer;						//版本标识
};

//数据包命令信息
struct CMD_Command
{
    int 								wMainCmdID;							//主命令码
    int 								wSubCmdID;							//子命令码
};

//数据包传递包头
struct CMD_Head
{
	CMD_Info							CmdInfo;							//基础结构
	CMD_Command							CommandInfo;						//命令信息
};

//网络数据包缓冲
struct CMD_Buffer
{
	CMD_Head							Head;								//数据包头
    unsigned char						cbBuffer[SOCKET_PACKAGE];			//数据缓冲
};


//检测结构信息
struct CMD_KN_DetectSocket
{
    long								dwSendTickCount;					//发送时间
    long								dwRecvTickCount;					//接收时间
};

/////////////////////////////////////////////////////////////////////////////////////////


//IPC 数据包头
struct IPC_Head
{
    int 								wVersion;							//IPC 版本
    int 								wDataSize;							//数据大小
    int 								wMainCmdID;							//主命令码
    int 								wSubCmdID;							//子命令码
};

//IPC 缓冲结构
struct IPC_Buffer
{
	IPC_Head							Head;								//数据包头
    unsigned char						cbBuffer[IPC_PACKAGE];				//数据缓冲
};


//游戏类型结构
struct tagGameType
{
    int								wSortID;							//排序号码
    int								wTypeID;							//种类号码
    unsigned char						szTypeName[TYPE_LEN];				//种类名字
};

//游戏名称结构
struct tagGameKind
{
    int								wSortID;							//排序号码
    int								wTypeID;							//类型号码
    int								wKindID;							//名称号码
    long								dwMaxVersion;						//最新版本
    long								dwOnLineCount;						//在线数目
    unsigned char						szKindName[KIND_LEN];				//游戏名字
    unsigned char						szProcessName[MODULE_LEN];			//进程名字
};

//游戏房间列表结构
struct tagGameServer
{
    int								wKindID;							//名称号码
    int								wTypeID;							//类型号码
    int								wServerID;							//房间号码
    int								wServerPort;						//房间端口
    long								dwServerAddr;						//房间地址
    long								dwOnLineCount;						//在线人数
    unsigned char						szServerName[SERVER_LEN];			//房间名称
};



//用户积分信息
struct tagUserScore
{
    long								lScore;								//用户分数
    long								lWinCount;							//胜利盘数
    long								lLostCount;							//失败盘数
    long								lDrawCount;							//和局盘数
    long								lFleeCount;							//断线数目
    long								lExperience;						//用户经验
};

//用户状态信息
struct tagUserStatus
{
    int								wTableID;							//桌子号码
    int								wChairID;							//椅子位置
    unsigned char						cbUserStatus;						//用户状态
};

//用户基本信息结构
struct tagUserInfoHead
{
	//用户属性
    int								wFaceID;							//头像索引
    long								dwUserID;							//用户 I D
    long								dwGameID;							//游戏 I D
    long								dwGroupID;							//社团索引
    long								dwUserRight;						//用户等级
    long								dwMasterRight;						//管理权限

	//用户属性
    unsigned char						cbGender;							//用户性别
    unsigned char						cbMemberOrder;						//会员等级
    unsigned char						cbMasterOrder;						//管理等级

	//用户状态
    int								wTableID;							//桌子号码
    int								wChairID;							//椅子位置
    unsigned char						cbUserStatus;						//用户状态

	//用户积分
	tagUserScore						UserScoreInfo;						//积分信息
};

//用户信息结构
struct tagUserData
{
	//用户属性
    int								wFaceID;							//头像索引
    long								dwUserID;							//用户 I D
    long								dwGroupID;							//社团索引
    long								dwGameID;							//用户 I D
    long								dwUserRight;						//用户等级
    long								dwMasterRight;						//管理权限
    char        						szName[USERNAME_LEN];					//用户名字
    char								szGroupName[GROUP_LEN];				//社团名字
    char								szUnderWrite[UNDER_WRITE_LEN];		//个性签名

	//用户属性
    unsigned char						cbGender;							//用户性别
    unsigned char						cbMemberOrder;						//会员等级
    unsigned char						cbMasterOrder;						//管理等级

	//用户积分
    long								lScore;								//用户分数
    long								lWinCount;							//胜利盘数
    long								lLostCount;							//失败盘数
    long								lDrawCount;							//和局盘数
    long								lFleeCount;							//断线数目
    long								lExperience;						//用户经验

	//用户状态
    int								wTableID;							//桌子号码
    int								wChairID;							//椅子位置
    unsigned char								cbUserStatus;						//用户状态

	//其他信息
    unsigned char								cbCompanion;						//用户关系
};

//////////////////////////////////////////////////////////////////////////

//机器序列号结构
struct tagClientSerial
{
    long								dwSystemVer;						//系统版本
    long								dwComputerID[3];					//机器序列
};

//配置缓冲结构
struct tagOptionBuffer
{
    unsigned char								cbBufferLen;						//数据长度
    unsigned char								cbOptionBuf[32];					//设置缓冲
};

/////////////////////////////////////////////////////////////////////////////////////////

//数据库请求事件
struct NTY_DataBaseEvent
{
    int							wIndex;								//对象索引
    int							wRoundID;							//对象标识
    int							wRequestID;							//请求标识
};

//网络应答事件
struct NTY_SocketAcceptEvent
{
    int							wIndex;								//连接索引
    int							wRoundID;							//连接标识
    long							dwClientIP;							//连接地址
};

//网络读取事件
struct NTY_SocketReadEvent
{
    int							wIndex;								//连接索引
    int							wRoundID;							//连接标识
    int							wDataSize;							//数据大小
	CMD_Command						Command;							//命令信息
};

//网络关闭事件
struct NTY_SocketCloseEvent
{
    int							wIndex;								//连接索引
    int							wRoundID;							//连接标识
    long							dwClientIP;							//连接地址
    long							dwConnectSecond;					//连接时间
};
struct SYS_MSG
{
    long								dwMainCmd;
    long								dwSubCmd;
	void*								wParam;
	void*								lParam;
};
