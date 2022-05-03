cmake_minimum_required(VERSION 3.10)

project(Yi)

aux_source_directory(LockFreeQueue.h src_files)
aux_source_directory(Singleton.h src_files)

SET(util_source ${src_files} PARENT_SCOPE)