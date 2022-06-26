cmake_minimum_required(VERSION 3.10)
find_package(Threads)
include_directories(${CMAKE_SOURCE_DIR}/thread)
include_directories(${CMAKE_SOURCE_DIR}/util)
include_directories(${CMAKE_SOURCE_DIR}/test)

project(speed_test)
add_executable(speed_test speed_test.cpp) 
target_link_libraries(speed_test ${CMAKE_THREAD_LIBS_INIT})