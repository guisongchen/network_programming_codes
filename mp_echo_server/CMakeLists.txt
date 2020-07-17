cmake_minimum_required(VERSION 3.17)

project(MP_ECHO)

message(STATUS "current src path: " ${PROJECT_SOURCE_DIR})

add_executable(mp_echo_server mp_echo_server.cpp)
add_executable(mp_echo_client mp_echo_client.cpp)