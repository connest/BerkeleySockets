#pragma once

#include "IServer.h"
#include <memory>
class ServerFactory
{
public:
    /**
     * @brief create server with type = T
     * @param port port
     * @return polymorfic object with interface IServer
     */
    template<class T>
    static std::unique_ptr<IServer> create(short&& port)
    {
        return std::make_unique<T>(std::forward<short>(port));
    }
};
