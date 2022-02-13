cmake_minimum_required(VERSION 3.10)
project(network)

aux_source_directory(${PROJECT_SOURCE_DIR}/io/noBuffer src_files)
aux_source_directory(${PROJECT_SOURCE_DIR}/io/withBuffer src_files)


set(network_io_src ${src_files} PARENT_SCOPE)