#pragma once
class CLoginService
{
	//��������
protected:
	//CCenterSocket					m_CenterSocket;						//��������
	CDataBaseSink					m_DataBaseSink;						//���ݿ⹳��
	CMsgTriggerServiceSink			m_MsgTriggerSink;						//���ȹ���

	//�������
protected:
	CKernelService					m_ServiceEngine;					//��������
	//��������
public:
	//���캯��
	CLoginService(void);
	//��������
	virtual ~CLoginService(void);

	//���ܺ���
public:
	//����״̬
	bool IsService();
	//��ʼ����
	bool BeginService();
	//ֹͣ����
	bool EndService();

	//�ڲ�����
private:
	//��ʾ����
	void TraceEventString(LPCTSTR pszString, enTraceLevel TraceLevel);
};

