#include "TCPServer.h"


#include <iostream>
#include <string>
#include <algorithm>

#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

TCPServer::TCPServer(short port)
    : IServer(port)
{

}

TCPServer::~TCPServer()
{
    std::cout<< "Close TCP server..." <<std::endl;
    if(m_socket > 0)
        close(m_socket);
}

void TCPServer::init(IServerProcessor *processor)
{
    this->processor = processor;

    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_socket < 0) {
        std::cerr<<"error: socket: " << errno <<std::endl;
        exit(1);
    }


    sockaddr_in bind_addr4 = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr = {
            .s_addr = htonl(INADDR_ANY),
        },
    };

    int res = bind(m_socket, (struct sockaddr *)&bind_addr4, sizeof(bind_addr4));
    if (res == -1) {
        std::cerr<<"Cannot bind IPv4, errno: " << errno <<std::endl;
        exit(1);
    }

    setBlocking(m_socket, false);
}

void TCPServer::start()
{
    int res;

    listen(m_socket, SOMAXCONN);

    //Add master socket
    pollfdsAdd(m_socket);


    std::cout<<"TCP server waits for connections on port " << port <<std::endl;

    while ( true ) {
        res = poll(pollfds.data(), pollfds.size(), -1);
        if (res == -1) {
            std::cerr<<"poll error: " << errno <<std::endl;
            continue;
        }

        for (nfds_t i = 0; i < pollfds.size(); i++) {

            //Cannot read?
            if (!(pollfds[i].revents & POLLIN))
                continue;

            int fd = pollfds[i].fd;

            //Is master socket?
            if (i < 1) {
                createNewConnection(fd);

            } else {
                porcessConnection(fd);
            }
        }
    }
}

void TCPServer::porcessConnection(int fd)
{
    int length = readFromClient(fd, buf, 1024);

    if(length < 0) {
        closeConnection(fd);

        //clear buf
        buf[0] = 0;
        return;
    }

    buf[length] = 0;

    std::string request;
    request.reserve(length + 1);
    request.assign(buf);


    std::string response = processor->process(std::move(request));

    int res = sendToClient(fd, response.data(), response.length());

    if(res < 0) {
        std::cerr << "Send fail: " << errno << std::endl;
        closeConnection(fd);
    }

    //clear buf
    buf[0] = 0;
}

int TCPServer::createNewConnection(int fd)
{
    sockaddr_storage client_addr;
    socklen_t client_addr_len = sizeof(client_addr);



    int client = accept(fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client < 0) {
        std::cerr<<"accept error: " << errno <<std::endl;
        return -1;
    }


    static char addr_str[32];
    void *addr = &((struct sockaddr_in *)&client_addr)->sin_addr;
    inet_ntop(client_addr.ss_family, addr, addr_str, sizeof(addr_str));

    pollfdsAdd(client);
    setBlocking(client, false);


    std::cout<<"connection from " << addr_str << " fd = " << client <<std::endl;

    return 0;

}

void TCPServer::closeConnection(int fd)
{
    std::cout<<"Close the connection fd = " << fd <<std::endl;

    pollfdsDel(fd);
    close(fd);
}

int TCPServer::readFromClient(int fd, char *buf, int maxlength)
{
    int length = recv(fd, buf, maxlength, 0);
    if (length <= 0) {
        if (length < 0)
            std::cerr<<"recv error: " << errno <<std::endl;

        return -1;
    }
    return length;
}

int TCPServer::sendToClient(int fd, const char *data, int length)
{
    int out_len;
    const char *p;
    for (p = data; length; length -= out_len) {
        out_len = send(fd, p, length, 0);
        if (out_len < 0) {
            std::cerr<<"send error: " << errno <<std::endl;
            return -1;
        }
        p += out_len;
    }
    return 0;
}

void TCPServer::setBlocking(int fd, bool val)
{
    int fl, res;

    fl = fcntl(fd, F_GETFL, 0);
    if (fl == -1) {
        std::cerr<<"fcntl error: " << errno <<std::endl;
        exit(1);
    }

    if (val) {
        fl &= ~O_NONBLOCK;
    } else {
        fl |= O_NONBLOCK;
    }

    res = fcntl(fd, F_SETFL, fl);
    if (res == -1) {
        std::cerr<<"fcntl error: " << errno <<std::endl;
        exit(1);
    }
}

void TCPServer::pollfdsAdd(int fd)
{
    pollfds.push_back({
                          .fd = fd,
                          .events = POLLIN
                      });
}

void TCPServer::pollfdsDel(int fd)
{
    pollfds.erase(std::remove_if(pollfds.begin(),
                                 pollfds.end(),
                                 [=](const auto& current){
        return current.fd == fd;
    }), pollfds.end());
}
