cmake_minimum_required(VERSION 3.22)
project(Version1 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_SYSTEM_NAME Windows)

add_executable(Version1
        main.c ai_evaluation.c util.c ai_generation.h ai_generation.c tests.c repetition_table.c zobrist_hashing.c
        additional_functionality/precompute_bitboards.h
        additional_functionality/precompute_bitboards.c
        additional_functionality/precomputed_data.h
        board.h
        board.c
)
