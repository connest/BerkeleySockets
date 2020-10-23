#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <errno.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

class Logic {
public:
    static std::string process(const std::string& str)
    {
        return str;
    }
};

class Server {
public:
    Server(short port);

    void init()
    {
        int res;
        sockaddr_in bind_addr4 = {
            .sin_family = AF_INET,
                    .sin_port = htons(port),
                    .sin_addr = {
                .s_addr = htonl(INADDR_ANY),
            },
        };

        m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (m_socket < 0) {
            std::cerr<<"error: socket: " << errno <<std::endl;
            exit(1);
        }

        res = bind(m_socket, (struct sockaddr *)&bind_addr4, sizeof(bind_addr4));
        if (res == -1) {
            std::cerr<<"Cannot bind IPv4, errno: " << errno <<std::endl;
        }
        num_servs++;

        setblocking(m_socket, false);
    }
    void start()
    {
        int res;

        listen(m_socket, SOMAXCONN);
        pollfds_add(m_socket);


        std::cout<<"Asynchronous TCP echo server waits for connections on port " << port <<std::endl;

        while (1) {
            struct sockaddr_storage client_addr;
            socklen_t client_addr_len = sizeof(client_addr);
            char addr_str[32];

            nfds_t pollnum = pollfds.size();
            res = poll(pollfds.data(), pollnum, -1);
            if (res == -1) {
                std::cerr<<"poll error: " << errno <<std::endl;
                continue;
            }

            for (int i = 0; i < pollnum; i++) {
                if (!(pollfds[i].revents & POLLIN)) {
                    continue;
                }
                int fd = pollfds[i].fd;
                if (i < num_servs) {
                    /* If server socket */
                    int client = accept(fd, (struct sockaddr *)&client_addr,
                                        &client_addr_len);
                    void *addr = &((struct sockaddr_in *)&client_addr)->sin_addr;

                    if (client < 0) {
                        std::cerr<<"accept error: " << errno <<std::endl;
                        continue;
                    }
                    inet_ntop(client_addr.ss_family, addr, addr_str, sizeof(addr_str));

                    std::cout<<"connection from " << addr_str << " fd = " << client <<std::endl;


                    if (pollfds_add(client) < 0) {
                        static char msg[] = "Too many connections\n";

                        res = send(client, msg, sizeof(msg) - 1, 0);
                        if (res < 0) {
                            std::cerr<<"send error: " << errno <<std::endl;
                        }
                        close(client);
                    } else {
                        setblocking(client, false);
                    }



                } else {
                    int len = recv(fd, buf, sizeof(buf), 0);
                    if (len <= 0) {
                        if (len < 0) {
                            std::cerr<<"recv error: " << errno <<std::endl;
                        }
                        close_connection(fd);
                    } else {
                        send_to_client(fd, buf, len);
                    }
                }
            }
        }
    }
private:
    short port;
    char buf[1024];

    int num_servs = 0;
    int m_socket;
    static const int NUM_FDS {SOMAXCONN};
//    struct pollfd pollfds[NUM_FDS];
    std::vector<pollfd> pollfds;

    void close_connection(int fd)
    {
        pollfds_del(fd);
        close(fd);

        std::cout<<"close the connection fd = " << fd <<std::endl;
    }
    void send_to_client(int fd, const char* data, int length)
    {
        int out_len;
        const char *p;
        for (p = buf; length; length -= out_len) {
            out_len = send(fd, p, length, 0);
            if (out_len < 0) {

                std::cerr<<"send error: " << errno <<std::endl;
                close_connection(fd);
            }
            p += out_len;
        }
    }
    static void setblocking(int fd, bool val)
    {
        int fl, res;

        fl = fcntl(fd, F_GETFL, 0);
        if (fl == -1) {
            exit(1);
        }

        if (val) {
            fl &= ~O_NONBLOCK;
        } else {
            fl |= O_NONBLOCK;
        }

        res = fcntl(fd, F_SETFL, fl);
        if (fl == -1) {
            exit(1);
        }
    }

    int pollfds_add(int fd)
    {
/*        int i;
        if (pollnum < NUM_FDS) {
            i = pollnum++;
        } else {
            for (i = 0; i < NUM_FDS; i++) {
                if (pollfds[i].fd < 0)
                    break;
            }

            return -1;
        }

        pollfds[i].fd = fd;
        pollfds[i].events = POLLIN;*/
        pollfds.push_back({
                             .fd = fd,
                              .events = POLLIN
                          });


        return 0;
    }

    void pollfds_del(int fd)
    {
//        for (int i = 0; i < pollnum; i++) {
//            if (pollfds[i].fd == fd) {
//                pollfds[i].fd = -1;
//                break;
//            }
//        }
        pollfds.erase(std::remove_if(pollfds.begin(), pollfds.end(), [=](const auto& current){
            return current.fd == fd;
        }));
    }
};
