cmake_minimum_required(VERSION 3.5)

project(Test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(headers
    server.h
    )

set(sources
    server.cpp
    )
add_executable(Test main.cpp ${headers} ${sources})
