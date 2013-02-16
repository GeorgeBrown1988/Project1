#pragma once
//��Ʒע����Ϣ
const char szProductName[16]="������Ϸ";
const char szSystemRegKey[16]="TestGame";
#ifdef _MSC_VER
//������Ϣ
const unsigned int  default_uMaxConnect=30000;										//���������
//��������ά��
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))

//�����ַ�����
#define CountString(String) ((unsigned int)((lstrlen(String)+1)*sizeof(char)))

//��ȫɾ��ָ��
#define  SafeDelete(pData)	{ try { delete pData; } catch (...) { } pData=NULL; } 

//��ȫɾ��ָ��
#define  SafeDeleteArray(pData)	{ try { delete [] pData; } catch (...) { } pData=NULL; } 
//������Ϣ
enum eServiceKind {NET_SERVICE=1,DB_SERVICE,DETECT_SERVICE,EVENT_SERVICE,LOG_SERVICE};
static unsigned int global_object_count = 0;

static CString DebugOutString;
class __declspec(dllexport) CObject 
{
	public:
		//���캯���������ü�����
		CObject()
		{
			InterlockedIncrement(&global_object_count);
			#ifdef _DEBUG
				LOG(INFO) <<global_object_count;
			#endif
//			TRACE(L"%d\n",global_object_count);
        };
		//���������ݼ����ü�����
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

//����ȼ�
enum enTraceLevel
{
	Level_Normal					=0,									//��ͨ��Ϣ
	Level_Warning					=1,									//������Ϣ
	Level_Exception					=2,									//�쳣��Ϣ
    Level_Debug						=3									//������Ϣ
};
#endif
//////////////////////////////////////////////////////////////////////////
//�¼�����

//�¼���ʶ
#define EVENT_CONTROL				0x0001								//������Ϣ
#define EVENT_TIMER					0x0002								//��ʱ������
#define EVENT_DATABASE				0x0003								//���ݿ�����
#define EVENT_SOCKET_ACCEPT			0x0004								//����Ӧ��
#define EVENT_SOCKET_READ			0x0005								//�����ȡ
#define EVENT_SOCKET_CLOSE			0x0006								//����ر�

//�¼�����
//�����궨��
#define EVENT_LEVEL_COUNT			4									//�¼��ȼ�
#define MAX_QUEUE_PACKET			10240								//������
#define INDEX_ALL_SOCKET			0xFFFF								//��������
#define TIMER_REPEAT_TIMER			DWORD(-1)							//�ظ�����
//��Ϣ�� 
#define CLIENT_REQUEST				1									//�ͻ�����Ϣ
#define NETGATE_REQUEST				2									//������Ϣ
#define DATABASE_REQUEST			3									//���ݿ���Ϣ
#define GAMESERVER_REQUEST			4									//��Ϸ�������Ϣ 
//�¼���ʶ
#define EVENT_SOCKET_ACCEPT			0x0004								//����Ӧ��
#define EVENT_SOCKET_READ			0x0005								//�����ȡ
#define EVENT_SOCKET_CLOSE			0x0006								//����ر�

#define MAX_CHAT_LEN					512									//���쳤��
#define INVALID_TABLE					((WORD)(-1))						//��Ч���Ӻ�
#define INVALID_CHAIR					((WORD)(-1))						//��Ч���Ӻ�
#define MAIN_DLG_CLASSNAME				TEXT("WHQPGamePlaza")				//����ע����

//�˿ڶ���
#define PORT_VIDEO_SERVER				7600								//��Ƶ������
#define PORT_LOGON_SERVER				9000								//��½������
#define PORT_CENTER_SERVER				9010								//���ķ�����

//�������ݶ���
#define SOCKET_VER						65									//���ݰ��汾
#define SOCKET_PACKAGE					2046								//��������
#define SOCKET_BUFFER					(sizeof(CMD_Head)+SOCKET_PACKAGE+2*sizeof(DWORD))
//�ں�������
#define MDM_KN_COMMAND					0									//�ں�����
#define SUB_KN_DETECT_SOCKET			1									//�������

//IPC ���ݶ���
#define IPC_VER							0x0001								//IPC �汾
#define IPC_IDENTIFIER					0x0001								//��ʶ����
#define IPC_PACKAGE						4096								//��� IPC ��
#define IPC_BUFFER						(sizeof(IPC_Head)+IPC_PACKAGE)		//���峤��
//////////////////////////////////////////////////////////////////////////

//���Ⱥ궨��
#define TYPE_LEN						32									//���೤��
#define KIND_LEN						32									//���ͳ���
#define STATION_LEN						32									//վ�㳤��
#define SERVER_LEN						32									//���䳤��
#define MODULE_LEN						32									//���̳���

//�Ա���
#define GENDER_NULL						0									//δ֪�Ա�
#define GENDER_BOY						1									//�����Ա�
#define GENDER_GIRL						2									//Ů���Ա�

//��Ϸ����
#define GAME_GENRE_SCORE				0x0001								//��ֵ����
#define GAME_GENRE_GOLD					0x0002								//�������
#define GAME_GENRE_MATCH				0x0004								//��������
#define GAME_GENRE_EDUCATE				0x0008								//ѵ������
//////////////////////////////////////////////////////////////////////////

//�û�״̬����
#define US_NULL							0x00								//û��״̬
#define US_FREE							0x01								//վ��״̬
#define US_SIT							0x02								//����״̬
#define US_READY						0x03								//ͬ��״̬
#define US_LOOKON						0x04								//�Թ�״̬
#define US_PLAY							0x05								//��Ϸ״̬
#define US_OFFLINE						0x06								//����״̬

//���Ⱥ궨��
//Warning ��Mysql��������ͻNAME_LEN
#define USERNAME_LEN						32									//���ֳ���
#define PASS_LEN						33									//���볤��
#define EMAIL_LEN						32									//���䳤��
#define GROUP_LEN						32									//���ų���
#define COMPUTER_ID_LEN					33									//��������
#define UNDER_WRITE_LEN					33									//����ǩ��
