#include "clientsocket.h"

QClientSocket::QClientSocket(QObject *parent) :
    QTcpSocket(parent)
{
}
//���ʹ����ݵ�����
bool QClientSocket::SendData(int MainCmd,int SubCmd,char *pData ,int DataSize )
{
    memcpy(this->m_cbBuffer,pData,DataSize);
    DataPacket SendDataPacket;
    SendDataPacket.MainCmd = MainCmd;
    SendDataPacket.SubCmd  = SubCmd;
    SendDataPacket.pBufferData = m_cbBuffer;
    SendDataPacket.DataSize = DataSize;
    SendDataPacket.PacketSize = sizeof(int)+ sizeof(int)+sizeof(char)*DataSize+sizeof(int);
    QTcpSocket::write(SendDataPacket,SendDataPacket.PacketSize);
}

//���Ͳ������ݵ�����
bool QClientSocket::SendData(int MainCmd,int SubCmd)
{
    DataPacket SendDataPacket;
    memset(SendDataPacket,0,sizeof(DataPacket));
    SendDataPacket.MainCmd = MainCmd;
    SendDataPacket.SubCmd  = SubCmd;
    QTcpSocket::write(SendDataPacket);
}
//��������
bool QClientSocket::RecvData()
{
    int ReadSize;
    QTcpSocket::read(m_cbBuffer,ReadSize);
    if(ReadSize <1)
        ;
    else
        ;
}
//��ʼ������
bool QClientSocket::InitSocket(char *ip,int port,QWidget *pSink)
{
    ;
}
//��ȡ����״̬
bool QClientSocket::GetStatus()
{
    ;
}
//
virtual bool QClientSocket::OnSocketConnect()
{
    ;
}
//
virtual bool QClientSocket::OnSocketRead(int MainCmd,int SubCmd, void *pDataBuffer,int DataSize)
{
    ;
}
//
virtual bool QClientSocket::OnSocketClose()
{
    ;
}

