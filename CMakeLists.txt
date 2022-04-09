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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++17 -rdynamic -lpthread -fPIC -lstdc++ -pipe -latomic")
message(STATUS "CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")

add_subdirectory(network)
add_subdirectory(timer)
add_subdirectory(test)

add_library(${PROJECT_NAME} STATIC ${network_io_src} ${timer_src})