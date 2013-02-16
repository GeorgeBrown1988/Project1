#pragma once
/////////////////////////////////////////////////////////////////////////////////////////
//������Ϣ
struct tagBurthenInfo
{
    long							dwDataSize;							//���ݴ�С
    long							dwBufferSize;						//����������
    long							dwDataPacketCount;					//���ݰ���Ŀ
};
//���ݶ���ͷ
struct tagDataHead
{
    int 							wDataSize;							//���ݴ�С
    int 							wIdentifier;						//���ͱ�ʶ
    long							dwInsertTime;						//����ʱ��
};

//���ݰ��ṹ��Ϣ
struct CMD_Info
{
    int 								wDataSize;							//���ݴ�С
    unsigned char						cbCheckCode;						//Ч���ֶ�
    unsigned char						cbMessageVer;						//�汾��ʶ
};

//���ݰ�������Ϣ
struct CMD_Command
{
    int 								wMainCmdID;							//��������
    int 								wSubCmdID;							//��������
};

//���ݰ����ݰ�ͷ
struct CMD_Head
{
	CMD_Info							CmdInfo;							//�����ṹ
	CMD_Command							CommandInfo;						//������Ϣ
};

//�������ݰ�����
struct CMD_Buffer
{
	CMD_Head							Head;								//���ݰ�ͷ
    unsigned char						cbBuffer[SOCKET_PACKAGE];			//���ݻ���
};


//���ṹ��Ϣ
struct CMD_KN_DetectSocket
{
    long								dwSendTickCount;					//����ʱ��
    long								dwRecvTickCount;					//����ʱ��
};

/////////////////////////////////////////////////////////////////////////////////////////


//IPC ���ݰ�ͷ
struct IPC_Head
{
    int 								wVersion;							//IPC �汾
    int 								wDataSize;							//���ݴ�С
    int 								wMainCmdID;							//��������
    int 								wSubCmdID;							//��������
};

//IPC ����ṹ
struct IPC_Buffer
{
	IPC_Head							Head;								//���ݰ�ͷ
    unsigned char						cbBuffer[IPC_PACKAGE];				//���ݻ���
};


//��Ϸ���ͽṹ
struct tagGameType
{
    int								wSortID;							//�������
    int								wTypeID;							//�������
    unsigned char						szTypeName[TYPE_LEN];				//��������
};

//��Ϸ���ƽṹ
struct tagGameKind
{
    int								wSortID;							//�������
    int								wTypeID;							//���ͺ���
    int								wKindID;							//���ƺ���
    long								dwMaxVersion;						//���°汾
    long								dwOnLineCount;						//������Ŀ
    unsigned char						szKindName[KIND_LEN];				//��Ϸ����
    unsigned char						szProcessName[MODULE_LEN];			//��������
};

//��Ϸ�����б�ṹ
struct tagGameServer
{
    int								wKindID;							//���ƺ���
    int								wTypeID;							//���ͺ���
    int								wServerID;							//�������
    int								wServerPort;						//����˿�
    long								dwServerAddr;						//�����ַ
    long								dwOnLineCount;						//��������
    unsigned char						szServerName[SERVER_LEN];			//��������
};



//�û�������Ϣ
struct tagUserScore
{
    long								lScore;								//�û�����
    long								lWinCount;							//ʤ������
    long								lLostCount;							//ʧ������
    long								lDrawCount;							//�;�����
    long								lFleeCount;							//������Ŀ
    long								lExperience;						//�û�����
};

//�û�״̬��Ϣ
struct tagUserStatus
{
    int								wTableID;							//���Ӻ���
    int								wChairID;							//����λ��
    unsigned char						cbUserStatus;						//�û�״̬
};

//�û�������Ϣ�ṹ
struct tagUserInfoHead
{
	//�û�����
    int								wFaceID;							//ͷ������
    long								dwUserID;							//�û� I D
    long								dwGameID;							//��Ϸ I D
    long								dwGroupID;							//��������
    long								dwUserRight;						//�û��ȼ�
    long								dwMasterRight;						//����Ȩ��

	//�û�����
    unsigned char						cbGender;							//�û��Ա�
    unsigned char						cbMemberOrder;						//��Ա�ȼ�
    unsigned char						cbMasterOrder;						//����ȼ�

	//�û�״̬
    int								wTableID;							//���Ӻ���
    int								wChairID;							//����λ��
    unsigned char						cbUserStatus;						//�û�״̬

	//�û�����
	tagUserScore						UserScoreInfo;						//������Ϣ
};

//�û���Ϣ�ṹ
struct tagUserData
{
	//�û�����
    int								wFaceID;							//ͷ������
    long								dwUserID;							//�û� I D
    long								dwGroupID;							//��������
    long								dwGameID;							//�û� I D
    long								dwUserRight;						//�û��ȼ�
    long								dwMasterRight;						//����Ȩ��
    char        						szName[USERNAME_LEN];					//�û�����
    char								szGroupName[GROUP_LEN];				//��������
    char								szUnderWrite[UNDER_WRITE_LEN];		//����ǩ��

	//�û�����
    unsigned char						cbGender;							//�û��Ա�
    unsigned char						cbMemberOrder;						//��Ա�ȼ�
    unsigned char						cbMasterOrder;						//����ȼ�

	//�û�����
    long								lScore;								//�û�����
    long								lWinCount;							//ʤ������
    long								lLostCount;							//ʧ������
    long								lDrawCount;							//�;�����
    long								lFleeCount;							//������Ŀ
    long								lExperience;						//�û�����

	//�û�״̬
    int								wTableID;							//���Ӻ���
    int								wChairID;							//����λ��
    unsigned char								cbUserStatus;						//�û�״̬

	//������Ϣ
    unsigned char								cbCompanion;						//�û���ϵ
};

//////////////////////////////////////////////////////////////////////////

//�������кŽṹ
struct tagClientSerial
{
    long								dwSystemVer;						//ϵͳ�汾
    long								dwComputerID[3];					//��������
};

//���û���ṹ
struct tagOptionBuffer
{
    unsigned char								cbBufferLen;						//���ݳ���
    unsigned char								cbOptionBuf[32];					//���û���
};

/////////////////////////////////////////////////////////////////////////////////////////

//���ݿ������¼�
struct NTY_DataBaseEvent
{
    int							wIndex;								//��������
    int							wRoundID;							//�����ʶ
    int							wRequestID;							//�����ʶ
};

//����Ӧ���¼�
struct NTY_SocketAcceptEvent
{
    int							wIndex;								//��������
    int							wRoundID;							//���ӱ�ʶ
    long							dwClientIP;							//���ӵ�ַ
};

//�����ȡ�¼�
struct NTY_SocketReadEvent
{
    int							wIndex;								//��������
    int							wRoundID;							//���ӱ�ʶ
    int							wDataSize;							//���ݴ�С
	CMD_Command						Command;							//������Ϣ
};

//����ر��¼�
struct NTY_SocketCloseEvent
{
    int							wIndex;								//��������
    int							wRoundID;							//���ӱ�ʶ
    long							dwClientIP;							//���ӵ�ַ
    long							dwConnectSecond;					//����ʱ��
};
struct SYS_MSG
{
    long								dwMainCmd;
    long								dwSubCmd;
	void*								wParam;
	void*								lParam;
};
