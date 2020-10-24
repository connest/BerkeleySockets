#pragma once

#include "IClient.h"
#include <memory>
class ClientFactory
{
public:
    template<class T>
    static std::unique_ptr<IClient> create(std::string&& address, short&& port)
    {
        return std::make_unique<T>(std::forward<std::string>(address), std::forward<short>(port));
    }
};
