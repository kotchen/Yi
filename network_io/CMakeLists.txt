cmake_minimum_required(VERSION 3.10)
project(network_io)

aux_source_directory(${PROJECT_SOURCE_DIR}/noBuffer src_files)
aux_source_directory(${PROJECT_SOURCE_DIR}/withBuffer src_files)


set(network_io_src ${src_files} PARENT_SCOPE)