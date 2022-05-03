cmake_minimum_required(VERSION 3.10)
project(Yi)

aux_source_directory(${PROJECT_SOURCE_DIR} src_files)
aux_source_directory(${PROJECT_SOURCE_DIR}/socket src_files)


set(network_source ${src_files} PARENT_SCOPE)