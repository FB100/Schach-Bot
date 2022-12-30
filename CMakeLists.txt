cmake_minimum_required(VERSION 3.16)
project(Schach C)

set(CMAKE_C_STANDARD 11)

add_executable(Schach
        main.c init.c ai.c util.c)
