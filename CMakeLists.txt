cmake_minimum_required(VERSION 3.10)
project(Yi)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CXX_FLAGS
#  -O2
#  -std=c++17
#  -rdynamic
#  -lpthread
#  -fPIC
#  -lstdc++
#  -pipe
#  -latomic
#  )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++17 -rdynamic -lpthread -fPIC -lstdc++ -pipe -latomic -lprotobuf")
message(STATUS "CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(network)
add_subdirectory(thread)
add_subdirectory(util)
add_subdirectory(test)
add_subdirectory(serialization)

add_library(${PROJECT_NAME} STATIC ${network_source} ${thread_source} ${util_source} ${serialization_source})