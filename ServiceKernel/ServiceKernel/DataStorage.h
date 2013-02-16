#ifndef DATA_STORAGE_HEAD_FILE
#define DATA_STORAGE_HEAD_FILE

#pragma once
#include "Interface.h"
//////////////////////////////////////////////////////////////////////////
//�ṹ�嶨��

//�ڴ�ش洢��
//ÿһ��Ԫ�ؽڵ������ݶ���ͷ+���ݿ�+���ݿ��С���
//��GetData��ʱ�����ݴ��ڴ���Ƴ�
//ʹ���ߣ��κγ���Ա
//ʹ�ó��ϣ����з������������
//struct tagDataHead
//{
//	WORD							wDataSize;							//���ݴ�С
//	WORD							wIdentifier;						//���ͱ�ʶ
//	DWORD							dwInsertTime;						//����ʱ��
//};
//////////////////////////////////////////////////////////////////////////

//���ݴ洢��
class CDataStorage : public CObject
{
	//���ݱ���
protected:
	DWORD							m_dwDataSize;						//���ݴ�С
	DWORD							m_dwBufferSize;						//����������
	DWORD							m_dwInsertPos;						//���ݲ���λ
	DWORD							m_dwTerminalPos;					//���ݽ���λ
	DWORD							m_dwDataQueryPos;					//���ݲ�ѯλ
	DWORD							m_dwDataPacketCount;				//���ݰ���Ŀ
	BYTE							* m_pDataStorageBuffer;				//����ָ��

	//��������
public:
	//���캯��
	CDataStorage(void);
	//��������
	virtual ~CDataStorage(void);

	//���ܺ���
public:
	//������Ϣ
	bool GetBurthenInfo(tagBurthenInfo & BurthenInfo);
	//��������
	bool AddData(WORD wIdentifier, void * const pBuffer, WORD wDataSize);
	//��ȡ����
	bool GetData(tagDataHead & DataHead, void * pBuffer, WORD wBufferSize);
	//ɾ������
	void RemoveData(bool bFreeMemroy);
};

//////////////////////////////////////////////////////////////////////////

#endif