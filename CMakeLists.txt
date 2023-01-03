cmake_minimum_required(VERSION 3.16)
project(Schach C)

set(CMAKE_C_STANDARD 11)

add_executable(Schach
        main.c init.c ai_evaluation.c util.c ai_generation.h ai_generation.c)
