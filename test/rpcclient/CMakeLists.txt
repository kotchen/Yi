cmake_minimum_required(VERSION 3.10)
find_package(Threads)
find_package(Protobuf)
include_directories(${CMAKE_SOURCE_DIR}/thread)
include_directories(${CMAKE_SOURCE_DIR}/serialization)
include_directories(${CMAKE_SOURCE_DIR}/util)
include_directories(${CMAKE_SOURCE_DIR}/test)

project(rpcclient)
add_executable(rpcclient client.cpp RpcClient.cpp) 
target_link_libraries(rpcclient ${CMAKE_THREAD_LIBS_INIT} ${Protobuf_LIBRARIES} Yi)

