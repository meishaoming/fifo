cmake_minimum_required(VERSION 2.6)
set(TARGET test-ringbuffer)

include_directories(../)

add_executable(${TARGET} test.cpp ../ringbuffer.c)

target_link_libraries(${TARGET} gtest)

