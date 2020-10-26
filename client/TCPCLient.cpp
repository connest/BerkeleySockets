#include "TCPClient.h"

#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>


TCPClient::TCPClient(const std::string &address, short port)
    : IClient(address, port)
{

}


TCPClient::~TCPClient()
{
    std::cout<< "Close TCP connection" << std::endl;
    if(sockfd > 0)
        close(sockfd);
}
int TCPClient::init()
{
    int res;

    res = createMaterSocket();
    if(res)
        return -1;


    sockaddr_in server_address = getBindAddress(port);

    res = bindSocket(server_address);
    if(res)
        return -2;


    return 0;
}


int TCPClient::recv(char *buffer, int maxlength)
{
    int length = read(sockfd, buffer, maxlength - 1);
    if(length < 0) {
        std::cerr<<"Read error"<<std::endl;
        return -1;
    }

    buffer[length] = 0;
    return length;
}

int TCPClient::send(const std::string &request)
{
    return ::send(sockfd, request.data(), request.length(), 0);
}

int TCPClient::createMaterSocket()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        std::cerr<<"Error : Could not create socket" <<std::endl;
        return -1;
    }

    return 0;
}

sockaddr_in TCPClient::getBindAddress(short port)
{
    sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);

    int res = inet_pton(AF_INET, address.data(), &server_address.sin_addr);
    if(res <= 0) {
        std::cerr<<"inet_pton error occured" <<std::endl;
    }

    return server_address;
}

int TCPClient::bindSocket(const sockaddr_in& server_address)
{
    int res = connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address));
    if(res < 0) {
        std::cerr<<"Error : Connect Failed"<<std::endl;
        return -1;
    }
    return 0;
}
