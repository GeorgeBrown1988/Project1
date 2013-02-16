#pragma once
//产品注册信息
const char szProductName[16]="测试游戏";
const char szSystemRegKey[16]="TestGame";
#ifdef _MSC_VER
//网络信息
const unsigned int  default_uMaxConnect=30000;										//最大连接数
//计算数组维数
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))

//计算字符长度
#define CountString(String) ((unsigned int)((lstrlen(String)+1)*sizeof(char)))

//安全删除指针
#define  SafeDelete(pData)	{ try { delete pData; } catch (...) { } pData=NULL; } 

//安全删除指针
#define  SafeDeleteArray(pData)	{ try { delete [] pData; } catch (...) { } pData=NULL; } 
//服务信息
enum eServiceKind {NET_SERVICE=1,DB_SERVICE,DETECT_SERVICE,EVENT_SERVICE,LOG_SERVICE};
static unsigned int global_object_count = 0;

static CString DebugOutString;
class __declspec(dllexport) CObject 
{
	public:
		//构造函数递增引用计数器
		CObject()
		{
			InterlockedIncrement(&global_object_count);
			#ifdef _DEBUG
				LOG(INFO) <<global_object_count;
			#endif
//			TRACE(L"%d\n",global_object_count);
        };
		//析构函数递减引用计数器
		virtual ~CObject()
		{
			InterlockedDecrement(&global_object_count);
			#ifdef _DEBUG
				LOG(INFO) <<global_object_count;
			#endif
        };
private:
		CObject& operator=(const CObject & obj);

		CObject(const CObject &obj);

};

//输出等级
enum enTraceLevel
{
	Level_Normal					=0,									//普通消息
	Level_Warning					=1,									//警告消息
	Level_Exception					=2,									//异常消息
    Level_Debug						=3									//调试消息
};
#endif
//////////////////////////////////////////////////////////////////////////
//事件定义

//事件标识
#define EVENT_CONTROL				0x0001								//控制消息
#define EVENT_TIMER					0x0002								//定时器引擎
#define EVENT_DATABASE				0x0003								//数据库请求
#define EVENT_SOCKET_ACCEPT			0x0004								//网络应答
#define EVENT_SOCKET_READ			0x0005								//网络读取
#define EVENT_SOCKET_CLOSE			0x0006								//网络关闭

//事件定义
//常量宏定义
#define EVENT_LEVEL_COUNT			4									//事件等级
#define MAX_QUEUE_PACKET			10240								//最大队列
#define INDEX_ALL_SOCKET			0xFFFF								//所有连接
#define TIMER_REPEAT_TIMER			DWORD(-1)							//重复次数
//消息人 
#define CLIENT_REQUEST				1									//客户端消息
#define NETGATE_REQUEST				2									//网关消息
#define DATABASE_REQUEST			3									//数据库消息
#define GAMESERVER_REQUEST			4									//游戏服务端消息 
//事件标识
#define EVENT_SOCKET_ACCEPT			0x0004								//网络应答
#define EVENT_SOCKET_READ			0x0005								//网络读取
#define EVENT_SOCKET_CLOSE			0x0006								//网络关闭

#define MAX_CHAT_LEN					512									//聊天长度
#define INVALID_TABLE					((WORD)(-1))						//无效桌子号
#define INVALID_CHAIR					((WORD)(-1))						//无效椅子号
#define MAIN_DLG_CLASSNAME				TEXT("WHQPGamePlaza")				//窗口注册名

//端口定义
#define PORT_VIDEO_SERVER				7600								//视频服务器
#define PORT_LOGON_SERVER				9000								//登陆服务器
#define PORT_CENTER_SERVER				9010								//中心服务器

//网络数据定义
#define SOCKET_VER						65									//数据包版本
#define SOCKET_PACKAGE					2046								//最大网络包
#define SOCKET_BUFFER					(sizeof(CMD_Head)+SOCKET_PACKAGE+2*sizeof(DWORD))
//内核命令码
#define MDM_KN_COMMAND					0									//内核命令
#define SUB_KN_DETECT_SOCKET			1									//检测命令

//IPC 数据定义
#define IPC_VER							0x0001								//IPC 版本
#define IPC_IDENTIFIER					0x0001								//标识号码
#define IPC_PACKAGE						4096								//最大 IPC 包
#define IPC_BUFFER						(sizeof(IPC_Head)+IPC_PACKAGE)		//缓冲长度
//////////////////////////////////////////////////////////////////////////

//长度宏定义
#define TYPE_LEN						32									//种类长度
#define KIND_LEN						32									//类型长度
#define STATION_LEN						32									//站点长度
#define SERVER_LEN						32									//房间长度
#define MODULE_LEN						32									//进程长度

//性别定义
#define GENDER_NULL						0									//未知性别
#define GENDER_BOY						1									//男性性别
#define GENDER_GIRL						2									//女性性别

//游戏类型
#define GAME_GENRE_SCORE				0x0001								//点值类型
#define GAME_GENRE_GOLD					0x0002								//金币类型
#define GAME_GENRE_MATCH				0x0004								//比赛类型
#define GAME_GENRE_EDUCATE				0x0008								//训练类型
//////////////////////////////////////////////////////////////////////////

//用户状态定义
#define US_NULL							0x00								//没有状态
#define US_FREE							0x01								//站立状态
#define US_SIT							0x02								//坐下状态
#define US_READY						0x03								//同意状态
#define US_LOOKON						0x04								//旁观状态
#define US_PLAY							0x05								//游戏状态
#define US_OFFLINE						0x06								//断线状态

//长度宏定义
//Warning 与Mysql库命名冲突NAME_LEN
#define USERNAME_LEN						32									//名字长度
#define PASS_LEN						33									//密码长度
#define EMAIL_LEN						32									//邮箱长度
#define GROUP_LEN						32									//社团长度
#define COMPUTER_ID_LEN					33									//机器序列
#define UNDER_WRITE_LEN					33									//个性签名
