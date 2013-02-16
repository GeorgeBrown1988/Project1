#ifndef INTERFACE_H
#define INTERFACE_H
#include "../../MyProject/GlobalDef/GlobalDef.h"
#include "../../MyProject/GlobalDef/GlobalCmd.h"
//�׽��ֻ�������С
#define SOCKET_PACKET_SIZE 4096
//�ⲿ�ӿ�
class IClientSocketSink
{
public:
    virtual bool OnSocketConnect()=NULL;
    virtual bool OnSocketRead(int MainCmd,int SubCmd, void *pDataBuffer,int DataSize)=NULL;
    virtual bool OnSocketClose()=NULL;
};
class ISocketError
{
public:
    virtual int GetErrorInfo()=NULL;
    virtual char* GetErrorMessage()=NULL;
};
struct DataPacket
{
    int MainCmd;
    int SubCmd;
    int PacketSize;
    char *pBufferData;
    int DataSize;
};

#endif // INTERFACE_H
