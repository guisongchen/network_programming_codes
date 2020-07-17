cmake_minimum_required(VERSION 3.17)

project(SELECT_ECHO)

message(STATUS "current src path: " ${PROJECT_SOURCE_DIR})

add_executable(select_echo_server select_echo_server.cpp)
add_executable(select_echo_client select_echo_client.cpp)