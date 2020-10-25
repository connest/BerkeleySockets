#pragma once

#include "IServer.h"
#include <sys/socket.h>
#include <netdb.h>


class UDPServer : public IServer
{
public:
    UDPServer(short port);
    virtual ~UDPServer();
    virtual int init(IServerProcessor* processor) override;
    virtual void start() override;
private:
    int readFromClient(const sockaddr_storage& client, int maxlength);
    std::pair<std::string, bool> readFromClient(const sockaddr_storage& client);

    int sendToClient(const sockaddr_storage& client, const char* data, int length);
    int sendToClient(const sockaddr_storage& client, const std::string& response);

    int getAddressInfo(short port, addrinfo*& servinfo);
    int createMasterSocket(const addrinfo* servinfo);
    int bindMasterSocket(const addrinfo* servinfo);


};
