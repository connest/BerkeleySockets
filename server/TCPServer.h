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

    virtual int init(IServerProcessor* processor) override;
    virtual void start() override;
private:
    void processConnection(int fd);

    int createNewConnection(int fd);
    void closeConnection(int fd);

    int readFromClient(int fd, char* buf, int maxlength);
    std::pair<std::string, bool> readFromClient(int fd);

    int sendToClient(int fd, const char* data, int length);
    int sendToClient(int fd, const std::string& response);

    int setBlocking(int fd, bool val);
    void pollfdsAdd(int fd);
    void pollfdsDel(int fd);

    int createMasterSocket();
    int bindMasterSocket(short port);

    int poll();
    void pollEvents();
};
