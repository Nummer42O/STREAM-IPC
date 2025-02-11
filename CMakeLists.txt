cmake_minimum_required(VERSION 3.16)
project(ipc
  VERSION 0.1.0
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(ament_cmake)

add_library(ipc_lib
  SHARED
    src/ipc-client.cpp
    src/ipc-exceptions.cpp
    src/ipc-server.cpp
    src/util.cpp
)
target_include_directories(ipc_lib PRIVATE include)


if (ament_cmake_FOUND)
  # ROS2 context install rules
  ament_export_targets(ipc_lib HAS_LIBRARY_TARGET)

  install(
    DIRECTORY include/ipc
    DESTINATION include
  )
  install(
    TARGETS ipc_lib
    EXPORT ipc_lib
    LIBRARY DESTINATION lib
    INCLUDES DESTINATION include
  )

  ament_package()
endif ()
