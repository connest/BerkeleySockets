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

int TCPServer::init(IServerProcessor *processor)
{
    this->processor = processor;

    int res = createMasterSocket();
    if(res)
        return -1;

    res = bindMasterSocket(port);
    if(res)
        return -2;

    res = setBlocking(m_socket, false);
    if(res) {
        m_socket = -1; //already closed
        return -3;
    }


    pollfdsAdd(m_socket);

    return 0;
}

void TCPServer::start()
{
    int res = listen(m_socket, SOMAXCONN);
    if(res) {
        std::cerr << "listen error, errno: " <<errno <<std::endl;
        return ;
    }

    std::cout<<"TCP server waits for connections on port " << port <<std::endl;

    while ( true ) {
        //if poll error, try again
        if(poll())
            continue;

        // Process available connection events
        pollEvents();
    }
}

void TCPServer::processConnection(int fd)
{
    auto request = readFromClient(fd);

    //Fail to read?
    if(!request.second)
        return ;

    std::string response = processor->process(std::move(request.first));

    sendToClient(fd, response);

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


    int res = setBlocking(client, false);
    if(res)
        return -1;

    pollfdsAdd(client);


    std::cout<<"connection fd = " << client <<std::endl;

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

std::pair<std::string, bool> TCPServer::readFromClient(int fd)
{
    int length = readFromClient(fd, buf, 1023);

    if(length < 0) {
        closeConnection(fd);

        //clear buf
        buf[0] = 0;
        return {{}, false};
    }

    buf[length] = 0;

    std::string request;
    request.reserve(length + 1);
    request.assign(buf);

    return {std::move(request), true};
}

int TCPServer::sendToClient(int fd, const char *data, int length)
{

    int out_len {0};
    const char *p = data;
    while (length) {
        std::cout<< "data: '"<< p << "'" << std::endl << "length: " << length <<std::endl;
        out_len = send(fd, p, length, MSG_NOSIGNAL);
        if (out_len <= 0) {
            std::cerr<<"send error: " << errno <<std::endl;
            closeConnection(fd);
            return -1;
        }
        p += out_len;
        length -= out_len;
    }
    return 0;
}

int TCPServer::sendToClient(int fd, const std::string &response)
{
    return sendToClient(fd, response.c_str(), response.length() + 1);
}

int TCPServer::setBlocking(int fd, bool val)
{
    int fl, res;

    fl = fcntl(fd, F_GETFL, 0);
    if (fl == -1) {
        std::cerr<<"fcntl error: " << errno <<std::endl;
        closeConnection(fd);
        return -1;
    }

    if (val) {
        fl &= ~O_NONBLOCK;
    } else {
        fl |= O_NONBLOCK;
    }

    res = fcntl(fd, F_SETFL, fl);
    if (res == -1) {
        std::cerr<<"fcntl error: " << errno <<std::endl;
        closeConnection(fd);
        return -1;
    }

    return 0;
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

int TCPServer::createMasterSocket()
{
    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_socket < 0) {
        std::cerr<<"error: socket: " << errno <<std::endl;
        return -1;
    }

    return 0;
}

int TCPServer::bindMasterSocket(short port)
{
    sockaddr_in bind_addr4 = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr = {
            .s_addr = htonl(INADDR_ANY),
        },
    };

    int res = bind(m_socket, (struct sockaddr *)&bind_addr4, sizeof(bind_addr4));
    if (res < 0) {
        std::cerr<<"Cannot bind IPv4, errno: " << errno <<std::endl;
        return -1;
    }

    return 0;
}

int TCPServer::poll()
{
    int res = ::poll(pollfds.data(), pollfds.size(), -1);
    if (res < 0) {
        std::cerr<<"poll error: " << errno <<std::endl;
        return -1;
    }

    return 0;
}

void TCPServer::pollEvents()
{
    for (nfds_t i = 0; i < pollfds.size(); i++) {

        //Cannot read?
        if (!(pollfds[i].revents & POLLIN))
            continue;

        int fd = pollfds[i].fd;

        //Is master socket?
        if (i < 1) {
            createNewConnection(fd);

        } else {
            processConnection(fd);
        }
    }
}
