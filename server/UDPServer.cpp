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

void UDPServer::init(IServerProcessor *processor)
{
    this->processor = processor;

    addrinfo *servinfo, *p;

    addrinfo hints {
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_DGRAM
    };

    hints.ai_flags = AI_PASSIVE;


    int res = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &servinfo);
    if (res) {
        std::cerr << "getaddrinfo: " << gai_strerror(res) << std::endl;
        exit(1);
    }


    // цикл по всем результатам и связывание с первым возможным
    for(p = servinfo; p != NULL; p = p->ai_next) {
        m_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (m_socket < 0) {
            std::cerr<<"error: socket: " << errno <<std::endl;
            continue;
        }

        res = bind(m_socket, p->ai_addr, p->ai_addrlen);
        if (res < 0) {
            close(m_socket);
            std::cerr<<"Cannot bind IPv4, errno: " << errno <<std::endl;
            continue;
        }

        break;
    }

    if (p == NULL) {
        std::cerr<<"listener: failed to bind socket" <<std::endl;
        exit(1);
    }
    freeaddrinfo(servinfo);


}

void UDPServer::start()
{
    std::cout<<"UDP server waits for connections on port " << port <<std::endl;

    sockaddr_storage client;
    while(true)
    {
        int length = readFromClient(1024, client);
        if (length < 0) {
            std::cerr<<"recvfrom error, errno: " << errno <<std::endl;
            continue;
        }

        buf[length] = 0;

        std::string request;
        request.reserve(length + 1);
        request.assign(buf);


        std::cout << request << std::endl;
        std::string response = processor->process(std::move(request));

        int res = sendToClient(response.data(), response.length(), client);
        if(res < 0) {
            std::cerr<<"sendto error, errno: " << errno <<std::endl;
            continue;
        }
    }



    close(m_socket);
}

int UDPServer::readFromClient(int maxlength, sockaddr_storage &client)
{
    socklen_t addr_len = sizeof (client);
    return recvfrom(m_socket, buf, maxlength , 0, (struct sockaddr *)&client, &addr_len);
}

int UDPServer::sendToClient(char *data, int length, sockaddr_storage &client)
{
    int out_len;
    const char *p;
    for (p = data; length; length -= out_len) {
        out_len = sendto(m_socket, p, length, 0, (struct sockaddr *)&client, sizeof (client));
        if (out_len < 0) {
            std::cerr<<"send error: " << errno <<std::endl;
            return -1;
        }
        p += out_len;
    }
    return 0;
}
