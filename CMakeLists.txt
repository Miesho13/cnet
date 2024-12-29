cmake_minimum_required(VERSION 3.22)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_COMPILER clang)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g -O0")

project(cnetlib C)

file(GLOB_RECURSE CNET_SRC "src/*.c")

add_library(cnetlib STATIC ${CNET_SRC})

add_executable(test ./test/main_test.c)
add_executable(test_server ./test/udp_connection.c)

target_link_libraries(test cnetlib)
target_link_libraries(test_server cnetlib)

