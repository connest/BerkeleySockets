#include "UDPClient.h"


#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>

UDPClient::UDPClient(const std::string &address, short port)
    : IClient(address, port)
{

}

UDPClient::~UDPClient()
{
    std::cout << "Close the UDP client..." <<std::endl;
    if(sockfd > 0)
        close(sockfd);
}

int UDPClient::init()
{
    int res = getBindAddress();
    if(res)
        return -1;

    res = bindSocket();
    if(res)
        return -2;

    return 0;
}

int UDPClient::recv(char *buffer, int maxlength)
{
    int length = recvfrom(sockfd, buffer, maxlength, 0, NULL, NULL);
    if(length < 0) {
        std::cerr<<"Read error"<<std::endl;
        return -1;
    }

    buffer[length] = 0;
    return length;
}

int UDPClient::send(const std::string &request)
{
    return sendto(sockfd, request.data(), request.length(), 0,
                  (struct sockaddr*)&server_address, sizeof(server_address));
}

int UDPClient::getBindAddress()
{
    int res = inet_pton(AF_INET, address.data(), &server_address.sin_addr);
    if(res <= 0) {
        std::cerr<<"inet_pton error occured" <<std::endl;
        return -1;
    }
    return 0;
}

int UDPClient::bindSocket()
{
    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr<<"Error : Could not create socket" <<std::endl;
        return -1;
    }

    return 0;
}
