#pragma once

#include <string>
class IClient
{
protected:
    std::string address;
    short port;
    int sockfd {0};
public:
    /**
     * @brief IClient constructor
     * @param address host ip-address
     * @param port port
     */
    IClient(const std::string& address, short port)
        : address {address}
        , port {port}
    {

    }
    virtual ~IClient()
    {

    }

    /**
     * @brief init connection
     * @return error code or 0
     */
    virtual int init() = 0;

    /**
     * @brief read from connection
     * @param buffer destination
     * @param maxlength buffer size
     * @return if error -1 of readed length
     */
    virtual int recv(char* buffer, int maxlength) = 0;

    /**
     * @brief write to connection
     * @param request data for write
     * @return if error -1 of writed length
     */
    virtual int send(const std::string& request) = 0;
};

