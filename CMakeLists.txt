cmake_minimum_required(VERSION 3.19)
project(TP2 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(TP2
        algoritmo.c
        algoritmo.h
        funcao.c
        funcao.h
        main.c
        utils.c
        utils.h)
