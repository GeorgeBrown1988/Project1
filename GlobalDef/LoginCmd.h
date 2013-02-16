
//���ݿ������ʶ
#define	DBR_GP_LOGON_BY_ACCOUNTS	1									//�ʺŵ�¼
#define	DBR_GP_LOGON_BY_USERID		2									//I D ��¼
#define DBR_GP_REGISTER_ACCOUNTS	3									//ע���ʺ�

//���ݿ������ʶ
#define DBR_GP_LOGON_SUCCESS		100									//��¼�ɹ�
#define DBR_GP_LOGON_ERROR			101									//��¼ʧ��

//�ʺŵ�¼
struct DBR_GP_LogonByAccounts
{
	long							dwClientIP;							//���ӵ�ַ
	char							szAccounts[USERNAME_LEN];				//��¼�ʺ�
	char							szPassWord[PASS_LEN];				//��¼����
	char							szComputerID[COMPUTER_ID_LEN];		//��������
};

//ID ��¼
struct DBR_GP_LogonByUserID
{
	long							dwClientIP;							//���ӵ�ַ
	long							dwUserID;							//�û� I D
	char							szPassWord[PASS_LEN];				//��¼����
	char							szComputerID[COMPUTER_ID_LEN];		//��������
};

//�ʺ�ע��
struct DBR_GP_RegisterAccounts
{
	int								wFaceID;							//ע��ͷ��
	unsigned char 					cbGender;							//�û��Ա�
	long							dwClientIP;							//���ӵ�ַ
	char							szSpreader[USERNAME_LEN];				//�Ƽ�����
	char							szAccounts[USERNAME_LEN];				//��¼�ʺ�
	char							szPassWord[PASS_LEN];				//��¼����
	char							szComputerID[COMPUTER_ID_LEN];		//��������
};

//��¼�ɹ�
struct DBR_GP_LogonSuccess
{
	//��������
	long							dwUserID;							//�û���ʶ
	long							dwGameID;							//��Ϸ��ʶ
	long							dwExperience;						//������ֵ
	char							szPassword[PASS_LEN];				//��¼����
	char							szAccounts[USERNAME_LEN];				//��¼�ʺ�
	char							szGroupName[GROUP_LEN];				//��������

	//�û�����
	int								wFaceID;							//ͷ���ʶ
	unsigned char					cbGender;							//�û��Ա�
	unsigned char					cbMember;							//��Ա�ȼ�
	char							szUnderWrite[UNDER_WRITE_LEN];		//����ǩ��

	//������Ϣ
	char							szDescribeString[128];				//������Ϣ
};

//��¼ʧ��
struct DBR_GP_LogonError
{
	long							lErrorCode;							//�������
	char							szErrorDescribe[128];				//������Ϣ
};


//�㳡�汾
#define VER_PLAZA_LOW					1								//�㳡�汾
#define VER_PLAZA_HIGH					1								//�㳡�汾
#define VER_PLAZA_FRAME					MAKELONG(VER_PLAZA_LOW,VER_PLAZA_HIGH)

//////////////////////////////////////////////////////////////////////////
//��¼�����ʶ

#define ERC_GP_LOGON_SUCCESS			0								//��½�ɹ�
#define ERC_GP_ACCOUNTS_NOT_EXIST		1								//�ʺŲ�����
#define ERC_GP_LONG_NULLITY				2								//��ֹ��¼
#define ERC_GP_PASSWORD_ERCOR			3								//�������

//////////////////////////////////////////////////////////////////////////
//��¼������

#define MDM_GP_LOGON					1								//�㳡��¼

#define SUB_GP_LOGON_ACCOUNTS			1								//�ʺŵ�¼
#define SUB_GP_LOGON_USERID				2								//I D ��¼
#define SUB_GP_REGISTER_ACCOUNTS		3								//ע���ʺ�

#define SUB_GP_LOGON_SUCCESS			100								//��½�ɹ�
#define SUB_GP_LOGON_ERROR				101								//��½ʧ��
#define SUB_GP_LOGON_FINISH				102								//��½���

//�ʺŵ�¼
struct CMD_GP_LogonByAccounts
{
	long								dwPlazaVersion;					//�㳡�汾
	char								szAccounts[USERNAME_LEN];			//��¼�ʺ�
	char								szPassWord[PASS_LEN];			//��¼����
};

//I D ��¼
struct CMD_GP_LogonByUserID
{
	long								dwPlazaVersion;					//�㳡�汾
	long								dwUserID;						//�û� I D
	char								szPassWord[PASS_LEN];			//��¼����
};

//ע���ʺ�
struct CMD_GP_RegisterAccounts
{
	int								wFaceID;						//ͷ���ʶ
	char								cbGender;						//�û��Ա�
	long								dwPlazaVersion;					//�㳡�汾
	char								szSpreader[USERNAME_LEN];			//�ƹ�����
	char								szAccounts[USERNAME_LEN];			//��¼�ʺ�
	char								szPassWord[PASS_LEN];			//��¼����
};

//��½�ɹ�
struct CMD_GP_LogonSuccess
{
	int								wFaceID;						//ͷ������
	char								cbGender;						//�û��Ա�
	char								cbMember;						//��Ա�ȼ�
	long								dwUserID;						//�û� I D
	long								dwGameID;						//��Ϸ I D
	long								dwExperience;					//�û�����
};

//��½ʧ��
struct CMD_GP_LogonError
{
	long								lErrorCode;						//�������
	char								szErrorDescribe[128];			//������Ϣ
};

//////////////////////////////////////////////////////////////////////////
//��Ϸ�б�������

#define MDM_GP_SERVER_LIST				2								//�б���Ϣ

#define SUB_GP_LIST_TYPE				100								//�����б�
#define SUB_GP_LIST_KIND				101								//�����б�
#define SUB_GP_LIST_STATION				102								//վ���б�
#define SUB_GP_LIST_SERVER				103								//�����б�
#define SUB_GP_LIST_FINISH				104								//�������
#define SUB_GP_LIST_CONFIG				105								//�б�����

#define SUB_GP_LIST_PROCESS				106								//�����б�

//�б�����
struct CMD_GP_ListConfig
{
	char								bShowOnLineCount;				//��ʾ����
};

//////////////////////////////////////////////////////////////////////////
//ϵͳ������

#define MDM_GP_SYSTEM					3								//ϵͳ����

#define SUB_GP_VERSION					100								//�汾֪ͨ
#define SUB_SP_SYSTEM_MSG				101								//ϵͳ��Ϣ

//�汾֪ͨ
struct CMD_GP_Version
{
	BYTE								bNewVersion;					//���°汾
	BYTE								bAllowConnect;					//��������
};