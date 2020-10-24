#pragma once

#include <vector>
#include <poll.h>

#include "IServer.h"
class TCPServer : public IServer
{
    std::vector<pollfd> pollfds;
public:
    TCPServer(short port);

    virtual ~TCPServer() override;

    virtual void init(IServerProcessor* processor) override;
    virtual void start() override;
private:
    void porcessConnection(int fd);

    int createNewConnection(int fd);
    void closeConnection(int fd);
    int readFromClient(int fd, char* buf, int maxlength);
    int sendToClient(int fd, const char* data, int length);
    void setBlocking(int fd, bool val);

    void pollfdsAdd(int fd);
    void pollfdsDel(int fd);
};
