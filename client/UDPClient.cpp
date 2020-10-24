#include "UDPClient.h"

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
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);

    inet_pton(AF_INET, address.c_str(), &server_address.sin_addr);


    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr<<"Error : Could not create socket" <<std::endl;
        return 1;
    }

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
