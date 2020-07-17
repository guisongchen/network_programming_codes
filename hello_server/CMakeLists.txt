cmake_minimum_required(VERSION 3.17)

project(HELLO)

message(STATUS "current src path: " ${PROJECT_SOURCE_DIR})

add_executable(hello_server hello_server.cpp)
add_executable(hello_client hello_client.cpp)