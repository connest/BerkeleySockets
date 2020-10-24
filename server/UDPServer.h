#pragma once

#include "IServer.h"
#include <sys/socket.h>


class UDPServer : public IServer
{
public:
    UDPServer(short port);
    virtual ~UDPServer();
    virtual void init(IServerProcessor* processor) override;
    virtual void start() override;
private:
    int readFromClient(int maxlength, sockaddr_storage& client);
    int sendToClient(char* data, int length, sockaddr_storage& client);
};
