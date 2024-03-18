cmake_minimum_required(VERSION 3.10)
project(Http)

add_executable(Http server.cpp http_tcpserver.cpp)
target_include_directories(Http PRIVATE .)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
