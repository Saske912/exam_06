cmake_minimum_required(VERSION 3.19)
project(mini_serv C)

set(CMAKE_C_STANDARD 99)

add_executable(mini_serv main.c list.c header.h buffers.c)