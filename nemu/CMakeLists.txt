cmake_minimum_required(VERSION 3.9)
add_definitions(-DDIFF_TEST_QEMU)
set(CMAKE_CXX_STANDARD 17)
include_directories(include)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-restrict -Wall -Werror -ggdb3")
add_subdirectory(src)

