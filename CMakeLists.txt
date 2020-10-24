cmake_minimum_required(VERSION 3.5)

project(Test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



set(server_headers
    server/server.h
    server/serverprocessor.h
    server/serverprocessorexercise.h

    server/UDPServer.h
    )

set(server_sources
    server/server.cpp
    server/serverprocessor.cpp
    server/serverprocessorexercise.cpp
    server/main.cpp
    )


set(client_headers
    client/IClient.h
    client/UDPClient.h
    client/TCPClient.h
    client/ClientFactory.h
    )

set(client_sources
    client/main.cpp
    client/UDPClient.cpp
    client/TCPCLient.cpp
    )


add_executable(Test ${server_headers} ${server_sources})
add_executable(Test_client ${client_headers} ${client_sources})
