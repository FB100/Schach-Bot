cmake_minimum_required(VERSION 3.16)
project(Version1 C)

set(CMAKE_C_STANDARD 11)

add_executable(Version1
        main.c ai_evaluation.c util.c ai_generation.h ai_generation.c tests.c repetition_table.c zobrist_hashing.c
)
