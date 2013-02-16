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
    
//��������
public:
    //��ʼ������
    bool InitSocket(char *ip,int port,QWidget *pSink);
    //��ȡ����״̬
    bool GetStatus();
    //���ʹ����ݵ�����
    bool SendData(int MainCmd,int SubCmd,char *pData ,int DataSize );
    //���Ͳ������ݵ�����
    bool SendData(int MainCmd,int SubCmd);
    //��������
    bool RecvData();

//��Ӻ���
public:
    //
    virtual bool OnSocketConnect();
    //
    virtual bool OnSocketRead(int MainCmd,int SubCmd, void *pDataBuffer,int DataSize);
    //
    virtual bool OnSocketClose();

//�ڲ�����
private:
    //���ݻ�����
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
    //��ȡ����������
    virtual int GetErrorInfo();
    //��ȡ������Ϣ
    virtual char* GetErrorMessage();
private:
    //
    QClientSocket *m_pSocket;
};

#endif // CLIENTSOCKET_H