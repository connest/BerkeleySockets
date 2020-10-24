#pragma once

#include "IClient.h"
#include <memory>
class ClientFactory
{
public:
    /**
     * @brief create client with type = T
     * @param address host ip-address
     * @param port port
     * @return polymorfic object with interface IClient
     */
    template<class T>
    static std::unique_ptr<IClient> create(std::string&& address, short&& port)
    {
        return std::make_unique<T>(std::forward<std::string>(address), std::forward<short>(port));
    }
};
