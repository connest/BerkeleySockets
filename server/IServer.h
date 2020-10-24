#pragma once

#include "IServerProcessor.h"

class IServer {

protected:
    short port;
    char buf[1024];
    int m_socket {0};
    IServerProcessor* processor;
public:
    IServer(short port)
        : port {port}
    {

    }
    virtual ~IServer()
    {

    }

    /**
     * @brief init master socket
     * @param processor object which process readed data
     */
    virtual void init(IServerProcessor* processor) = 0;

    /**
     * @brief start read-write loop
     */
    virtual void start() = 0;
};
