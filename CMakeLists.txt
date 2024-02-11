cmake_minimum_required(VERSION 3.22)

project(cnetlib)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g")

file(GLOB_RECURSE CNET_SRC "src/*.c")

add_library(cnetlib STATIC ${CNET_SRC})

add_executable(test test/main_test.c)

target_link_libraries(test cnetlib)

