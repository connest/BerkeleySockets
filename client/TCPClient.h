#pragma once

#include "IClient.h"
#include <arpa/inet.h>

class TCPClient : public IClient
{
public:
    TCPClient(const std::string& address, short port);
    virtual ~TCPClient() override;
    virtual int init() override;
    virtual int recv(char* buffer, int maxlength) override;
    virtual int send(const std::string& request) override;

private:
    int createMaterSocket();
    sockaddr_in getBindAddress(short port);
    int bindSocket(const sockaddr_in& server_address);
};


