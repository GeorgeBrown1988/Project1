#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H
#include "interface.h"
#include <QtNetwork/QtNetwork>

class QClientSocket : public QTcpSocket,public IClientSocketSink
{
    Q_OBJECT
public:
    explicit QClientSocket(QObject *parent = 0);
    
signals:
    
public slots:
    
//公共函数
public:
    //初始化参数
    bool InitSocket(char *ip,int port,QWidget *pSink);
    //获取连接状态
    bool GetStatus();
    //发送带数据的命令
    bool SendData(int MainCmd,int SubCmd,char *pData ,int DataSize );
    //发送不带数据的命令
    bool SendData(int MainCmd,int SubCmd);
    //接受数据
    bool RecvData();

//外接函数
public:
    //
    virtual bool OnSocketConnect();
    //
    virtual bool OnSocketRead(int MainCmd,int SubCmd, void *pDataBuffer,int DataSize);
    //
    virtual bool OnSocketClose();

//内部数据
private:
    //数据缓冲区
    char m_cbBuffer[SOCKET_PACKET_SIZE];
    //
    QWidget *m_pSink;
    //
    bool flags;

    char m_szIPAddress[32];
    //
    unsigned int m_PortNumber;
};
class CSocketError:public ISocketError
{
public:
    CSocketError(QClientSocket *p);
    ~CSocketError();
public:
    //获取错误特征码
    virtual int GetErrorInfo();
    //获取错误信息
    virtual char* GetErrorMessage();
private:
    //
    QClientSocket *m_pSocket;
};

#endif // CLIENTSOCKET_H
