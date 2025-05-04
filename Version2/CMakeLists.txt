cmake_minimum_required(VERSION 3.22)
project(Version2 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Version2
        evaluation.c
        evaluation.h
        generation.c
        generation.h
        main.c
        main.h
        tests.c
        tests.h
        structs.h
        setup.c
        setup.h
        moves_external.c
        moves_external.h
        pregenerate.h
        pregenerate.c
        util_board.c
        util_board.h
)
