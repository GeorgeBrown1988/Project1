
//数据库请求标识
#define	DBR_GP_LOGON_BY_ACCOUNTS	1									//帐号登录
#define	DBR_GP_LOGON_BY_USERID		2									//I D 登录
#define DBR_GP_REGISTER_ACCOUNTS	3									//注册帐号

//数据库输出标识
#define DBR_GP_LOGON_SUCCESS		100									//登录成功
#define DBR_GP_LOGON_ERROR			101									//登录失败

//帐号登录
struct DBR_GP_LogonByAccounts
{
	long							dwClientIP;							//连接地址
	char							szAccounts[USERNAME_LEN];				//登录帐号
	char							szPassWord[PASS_LEN];				//登录密码
	char							szComputerID[COMPUTER_ID_LEN];		//机器序列
};

//ID 登录
struct DBR_GP_LogonByUserID
{
	long							dwClientIP;							//连接地址
	long							dwUserID;							//用户 I D
	char							szPassWord[PASS_LEN];				//登录密码
	char							szComputerID[COMPUTER_ID_LEN];		//机器序列
};

//帐号注册
struct DBR_GP_RegisterAccounts
{
	int								wFaceID;							//注册头像
	unsigned char 					cbGender;							//用户性别
	long							dwClientIP;							//连接地址
	char							szSpreader[USERNAME_LEN];				//推荐人名
	char							szAccounts[USERNAME_LEN];				//登录帐号
	char							szPassWord[PASS_LEN];				//登录密码
	char							szComputerID[COMPUTER_ID_LEN];		//机器序列
};

//登录成功
struct DBR_GP_LogonSuccess
{
	//属性资料
	long							dwUserID;							//用户标识
	long							dwGameID;							//游戏标识
	long							dwExperience;						//经验数值
	char							szPassword[PASS_LEN];				//登录密码
	char							szAccounts[USERNAME_LEN];				//登录帐号
	char							szGroupName[GROUP_LEN];				//社团名字

	//用户资料
	int								wFaceID;							//头像标识
	unsigned char					cbGender;							//用户性别
	unsigned char					cbMember;							//会员等级
	char							szUnderWrite[UNDER_WRITE_LEN];		//个性签名

	//描述信息
	char							szDescribeString[128];				//描述消息
};

//登录失败
struct DBR_GP_LogonError
{
	long							lErrorCode;							//错误代码
	char							szErrorDescribe[128];				//错误消息
};


//广场版本
#define VER_PLAZA_LOW					1								//广场版本
#define VER_PLAZA_HIGH					1								//广场版本
#define VER_PLAZA_FRAME					MAKELONG(VER_PLAZA_LOW,VER_PLAZA_HIGH)

//////////////////////////////////////////////////////////////////////////
//登录错误标识

#define ERC_GP_LOGON_SUCCESS			0								//登陆成功
#define ERC_GP_ACCOUNTS_NOT_EXIST		1								//帐号不存在
#define ERC_GP_LONG_NULLITY				2								//禁止登录
#define ERC_GP_PASSWORD_ERCOR			3								//密码错误

//////////////////////////////////////////////////////////////////////////
//登录命令码

#define MDM_GP_LOGON					1								//广场登录

#define SUB_GP_LOGON_ACCOUNTS			1								//帐号登录
#define SUB_GP_LOGON_USERID				2								//I D 登录
#define SUB_GP_REGISTER_ACCOUNTS		3								//注册帐号

#define SUB_GP_LOGON_SUCCESS			100								//登陆成功
#define SUB_GP_LOGON_ERROR				101								//登陆失败
#define SUB_GP_LOGON_FINISH				102								//登陆完成

//帐号登录
struct CMD_GP_LogonByAccounts
{
	long								dwPlazaVersion;					//广场版本
	char								szAccounts[USERNAME_LEN];			//登录帐号
	char								szPassWord[PASS_LEN];			//登录密码
};

//I D 登录
struct CMD_GP_LogonByUserID
{
	long								dwPlazaVersion;					//广场版本
	long								dwUserID;						//用户 I D
	char								szPassWord[PASS_LEN];			//登录密码
};

//注册帐号
struct CMD_GP_RegisterAccounts
{
	int								wFaceID;						//头像标识
	char								cbGender;						//用户性别
	long								dwPlazaVersion;					//广场版本
	char								szSpreader[USERNAME_LEN];			//推广人名
	char								szAccounts[USERNAME_LEN];			//登录帐号
	char								szPassWord[PASS_LEN];			//登录密码
};

//登陆成功
struct CMD_GP_LogonSuccess
{
	int								wFaceID;						//头像索引
	char								cbGender;						//用户性别
	char								cbMember;						//会员等级
	long								dwUserID;						//用户 I D
	long								dwGameID;						//游戏 I D
	long								dwExperience;					//用户经验
};

//登陆失败
struct CMD_GP_LogonError
{
	long								lErrorCode;						//错误代码
	char								szErrorDescribe[128];			//错误消息
};

//////////////////////////////////////////////////////////////////////////
//游戏列表命令码

#define MDM_GP_SERVER_LIST				2								//列表信息

#define SUB_GP_LIST_TYPE				100								//类型列表
#define SUB_GP_LIST_KIND				101								//种类列表
#define SUB_GP_LIST_STATION				102								//站点列表
#define SUB_GP_LIST_SERVER				103								//房间列表
#define SUB_GP_LIST_FINISH				104								//发送完成
#define SUB_GP_LIST_CONFIG				105								//列表配置

#define SUB_GP_LIST_PROCESS				106								//进程列表

//列表配置
struct CMD_GP_ListConfig
{
	char								bShowOnLineCount;				//显示人数
};

//////////////////////////////////////////////////////////////////////////
//系统命令码

#define MDM_GP_SYSTEM					3								//系统命令

#define SUB_GP_VERSION					100								//版本通知
#define SUB_SP_SYSTEM_MSG				101								//系统消息

//版本通知
struct CMD_GP_Version
{
	BYTE								bNewVersion;					//更新版本
	BYTE								bAllowConnect;					//允许连接
};