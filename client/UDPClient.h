#pragma once


#include <string>
#include <arpa/inet.h>

#include "IClient.h"
class UDPClient : public IClient
{
    sockaddr_in server_address;
public:
    UDPClient(const std::string& address, short port);
    virtual ~UDPClient();
    virtual int init() override;
    virtual int recv(char* buffer, int maxlength) override;
    virtual int send(const std::string& request) override;

private:
    int getBindAddress();
    int bindSocket();
};
