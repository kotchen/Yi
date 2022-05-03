find_package(Threads)

include_directories(${CMAKE_SOURCE_DIR}/thread)
include_directories(${CMAKE_SOURCE_DIR}/util)
include_directories(${CMAKE_SOURCE_DIR}/serialization)

project(test_thread_pool)
add_executable(test_thread_pool test_thread_pool.cpp ${CMAKE_SOURCE_DIR}/serialization/Request.pb.cc) 
target_link_libraries(test_thread_pool ${CMAKE_THREAD_LIBS_INIT})

project(test_lock_free_queue)
add_executable(test_lock_free_queue test_lock_free_queue.cpp) 
target_link_libraries(test_lock_free_queue ${CMAKE_THREAD_LIBS_INIT})