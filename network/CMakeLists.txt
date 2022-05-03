cmake_minimum_required(VERSION 3.10)
project(Yi)

aux_source_directory(Net.h src_files)
aux_source_directory(Net.cpp src_files)


set(yi_source ${src_files} PARENT_SCOPE)