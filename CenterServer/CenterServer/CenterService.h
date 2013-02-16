#pragma once
#include "CenterServer.h"
#include "DataBaseSink.h"
#include "MsgTriggerServiceSink.h"
class CCenterService
{
public:
	CCenterService();
	virtual ~CCenterService();

	//���ܺ���
public:
	//��ʼ����
	bool StartService();
	//ֹͣ����
	bool EndService();
	//����״̬
	bool IsService();
	//
	void InitCenterService();
	//
private:
	//�ں˷���
	CKernelService						*m_pKernelService;
	//
	CDataBaseSink						m_DataBaseSink;
	//
	CMsgTriggerServiceSink				m_MsgTriggerServiceSink;						//���ȹ���
};

