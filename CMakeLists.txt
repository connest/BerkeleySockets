cmake_minimum_required(VERSION 3.5)

project(Test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

set(shared_headers
    shared/ArgsParser.h
    )

set(shared_sources
    shared/ArgsParser.cpp
    )



set(server_headers
    server/IServer.h
    server/ServerFactory.h
    server/IServerProcessor.h
    server/ServerProcessor.h
    server/UDPServer.h
    server/TCPServer.h
    )

set(server_sources
    server/TCPServer.cpp
    server/UDPServer.cpp
    server/ServerProcessor.cpp
    server/main.cpp
    )



set(client_headers
    client/IClient.h
    client/UDPClient.h
    client/TCPClient.h
    client/ClientFactory.h
    client/ClientProcessor.h
    )

set(client_sources
    client/main.cpp
    client/UDPClient.cpp
    client/TCPCLient.cpp
    client/ClientProcessor.cpp
    )


add_executable(Server ${server_headers} ${server_sources} ${shared_headers} ${shared_sources})
target_link_libraries(Server pthread)

add_executable(Client ${client_headers} ${client_sources} ${shared_headers} ${shared_sources})
