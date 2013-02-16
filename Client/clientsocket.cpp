#include "clientsocket.h"

QClientSocket::QClientSocket(QObject *parent) :
    QTcpSocket(parent)
{
}
//发送带数据的命令
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

//发送不带数据的命令
bool QClientSocket::SendData(int MainCmd,int SubCmd)
{
    DataPacket SendDataPacket;
    memset(SendDataPacket,0,sizeof(DataPacket));
    SendDataPacket.MainCmd = MainCmd;
    SendDataPacket.SubCmd  = SubCmd;
    QTcpSocket::write(SendDataPacket);
}
//接受数据
bool QClientSocket::RecvData()
{
    int ReadSize;
    QTcpSocket::read(m_cbBuffer,ReadSize);
    if(ReadSize <1)
        ;
    else
        ;
}
//初始化参数
bool QClientSocket::InitSocket(char *ip,int port,QWidget *pSink)
{
    ;
}
//获取连接状态
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

