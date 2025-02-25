cmake_minimum_required(VERSION 3.16)

target_sources(ipc_lib
  PRIVATE
    util.cpp
)
add_subdirectory(ipc)
