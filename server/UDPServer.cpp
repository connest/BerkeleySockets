#include "UDPServer.h"


#include <iostream>
#include <unistd.h>
#include <netdb.h>

UDPServer::UDPServer(short port)
    : IServer(port)
{

}

UDPServer::~UDPServer() {

    std::cout<< "Close UDP server..." <<std::endl;
    if(m_socket > 0)
        close(m_socket);
}

int UDPServer::init(IServerProcessor *processor)
{
    this->processor = processor;

    addrinfo *servinfo, *p;

    int res = getAddressInfo(port, servinfo);
    if(res)
        return -1;


    for(p = servinfo; p; p = p->ai_next) {
        res = createMasterSocket(p);
        if(res)
            continue;

        res = bindMasterSocket(p);
        if(res)
            continue;

        break;
    }

    if (p == NULL) {
        std::cerr<<"listener: failed to bind socket" <<std::endl;
        return -2;
    }

    freeaddrinfo(servinfo);

    return 0;
}

void UDPServer::start()
{
    std::cout<<"UDP server waits for connections on port " << port <<std::endl;

    sockaddr_storage client;
    while(true)
    {
        auto result = readFromClient(client);
        if(!result.second)
            continue;

//        std::cout << result.first << std::endl;
        std::string response = processor->process(std::move(result.first));

        sendToClient(client, response);
    }
}

int UDPServer::readFromClient(const sockaddr_storage &client, int maxlength)
{
    socklen_t addr_len = sizeof (client);
    return recvfrom(m_socket, buf, maxlength , 0, (struct sockaddr *)&client, &addr_len);
}

std::pair<std::string, bool> UDPServer::readFromClient(const sockaddr_storage &client)
{
    int length = readFromClient(client, 1023);
    if (length < 0) {
        std::cerr<<"recvfrom error, errno: " << errno <<std::endl;
        return {{}, false};
    }

    buf[length] = 0;

    std::string request;
    request.reserve(length + 1);
    request.assign(buf);

    return {std::move(request), true};

}

int UDPServer::sendToClient(const sockaddr_storage &client, const std::string &response)
{
    int res = sendToClient(client, response.data(), response.length());
    if(res < 0) {
        std::cerr<<"sendto error, errno: " << errno <<std::endl;
        return -1;
    }
    return 0;
}

int UDPServer::sendToClient(const sockaddr_storage &client, const char *data, int length)
{
    int out_len {0};
    const char *p = data;
    while (length) {
        std::cout<< "data: '"<< p << "'" << std::endl << "length: " << length <<std::endl;
        out_len = sendto(m_socket, p, length, 0, (struct sockaddr *)&client, sizeof (client));
        if (out_len <= 0) {
            std::cerr<<"send error: " << errno <<std::endl;
            return -1;
        }
        p += out_len;
        length -= out_len;
    }
    return 0;

//    int out_len;
//    const char *p;
//    for (p = data; length; length -= out_len) {
//        out_len = sendto(m_socket, p, length, 0, (struct sockaddr *)&client, sizeof (client));
//        if (out_len < 0) {
//            std::cerr<<"send error: " << errno <<std::endl;
//            return -1;
//        }
//        p += out_len;
//    }
//    return 0;
}

int UDPServer::getAddressInfo(short port, addrinfo *&servinfo)
{
    addrinfo hints {
        .ai_family = AF_UNSPEC,
                .ai_socktype = SOCK_DGRAM
    };

    hints.ai_flags = AI_PASSIVE;


    int res = getaddrinfo(NULL, std::to_string(port).data(), &hints, &servinfo);
    if (res) {
        std::cerr << "getaddrinfo: " << gai_strerror(res) << std::endl;
        return -1;
    }

    return 0;
}

int UDPServer::createMasterSocket(const addrinfo *servinfo)
{
    m_socket = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (m_socket < 0) {
        std::cerr<<"error: socket: " << errno <<std::endl;
        return -1;
    }

    return 0;
}

int UDPServer::bindMasterSocket(const addrinfo *servinfo)
{
    int res = bind(m_socket, servinfo->ai_addr, servinfo->ai_addrlen);
    if (res < 0) {
        close(m_socket);
        std::cerr<<"Cannot bind IPv4, errno: " << errno <<std::endl;
        return -1;
    }

    return 0;
}
