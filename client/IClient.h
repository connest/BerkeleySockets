#pragma once

#include <string>
class IClient
{
protected:
    std::string address;
    short port;
    int sockfd {0};
public:
    IClient(const std::string& address, short port)
        : address {address}
        , port {port}
    {

    }
    virtual ~IClient()
    {

    }
    virtual int init() = 0;
    virtual int recv(char* buffer, int maxlength) = 0;
    virtual int send(const std::string& request) = 0;
};

