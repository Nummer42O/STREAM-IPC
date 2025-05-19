cmake_minimum_required(VERSION 3.16)

target_sources(ipc_lib
  PRIVATE
    ipc-client.cpp
    ipc-exceptions.cpp
    ipc-server.cpp
    util.cpp
    sharedMem.cpp
)
