#pragma once

#include "IClient.h"
class TCPClient : public IClient
{
public:
    TCPClient(const std::string& address, short port);
    virtual ~TCPClient() override;
    virtual int init() override;
    virtual int recv(char* buffer, int maxlength) override;
    virtual int send(const std::string& request) override;
};


