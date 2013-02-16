#pragma once

//���ݿ����湳��
class CDataBaseSink : public IDataBaseSink
{
	//��������
public:
	//���캯��
	CDataBaseSink();
	//��������
	virtual ~CDataBaseSink();

	//���Ƚӿ�
public:
	//ģ������
	virtual bool __cdecl StartService(CObject * pObject);
	//ģ��ر�
	virtual bool __cdecl EndService(CObject * pObject);
	//���ݲ�������
	virtual bool __cdecl OnDataBaseRequest(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);

	//���ú��� 
public:
	//���ú���
	bool InitDataBaseSink(IKernelService *pObject);

	//������
private:
	//��¼������
	bool OnRequestLogon(const NTY_DataBaseEvent & DataBaseEvent, void * pDataBuffer, WORD wDataSize);

	//�洢����
private:
	//I D �洢����
	LONG SPLogonByUserID(DWORD dwGameID, LPCTSTR pszPassword, DWORD dwClientIP, LPCTSTR pszComputerID);
	//�ʺŴ洢����
	LONG SPLogonByAccounts(LPCTSTR pszAccounts, LPCTSTR pszPassword, DWORD dwClientIP, LPCTSTR pszComputerID);
	//ע��洢����
	LONG SPRegisterAccounts(LPCTSTR pszAccounts, LPCTSTR pszPassword, LPCTSTR pszSpreader, WORD wFaceID, BYTE cbGender, DWORD dwClientIP, LPCTSTR pszComputerID);
	//�������
protected:
	CMysqlDataBase					*m_pUserDataBase;						//�û����ݿ�
	CQueueServiceEvent				m_QueueServiceEvent;					//����֪ͨ
};
