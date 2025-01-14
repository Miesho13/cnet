cmake_minimum_required(VERSION 3.22)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_COMPILER clang)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g -O0")

project(cnetlib C)

file(GLOB_RECURSE CNET_SRC "src/*.c")

add_library(cnetlib STATIC ${CNET_SRC})

add_executable(client_test ./test/main_test.c)
add_executable(udp_echo_server ./test/udp_connection.c)
add_executable(async_test ./test/async_test.c)

target_link_libraries(client_test cnetlib)
target_link_libraries(async_test cnetlib)
target_link_libraries(udp_echo_server cnetlib)

