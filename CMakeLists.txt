cmake_minimum_required(VERSION 3.20)
project(brik VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -pedantic")
endif()

include_directories(src)

add_executable(brik
    src/main.c

    src/mem/arena.h
    src/mem/arena.c

    src/utils/hash_map.h
    src/utils/hash_map.c
)

add_executable(brik_tests
    tests/test_main.c

    tests/utils/test_hash_map.c

    src/mem/arena.h
    src/mem/arena.c

    src/utils/hash_map.h
    src/utils/hash_map.c
)