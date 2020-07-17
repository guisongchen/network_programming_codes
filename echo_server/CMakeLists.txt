cmake_minimum_required(VERSION 3.17)

project(ECHO)

message(STATUS "current src path: " ${PROJECT_SOURCE_DIR})

add_executable(echo_server echo_server.cpp)
add_executable(echo_client echo_client.cpp)