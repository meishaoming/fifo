cmake_minimum_required(VERSION 2.6)
set(TARGET test-fifo)

set(CMAKE_VERBOSE_MAKEFILE 1)
include_directories(../)

add_executable(${TARGET} test.cpp ../fifo.c)

target_link_libraries(${TARGET} gtest)

