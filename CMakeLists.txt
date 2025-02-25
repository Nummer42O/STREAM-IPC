cmake_minimum_required(VERSION 3.16)
project(ipc
  VERSION 0.1.1
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(ament_cmake QUIET)

add_library(ipc_lib)
target_include_directories(ipc_lib
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
  PRIVATE src
)
add_subdirectory(src)


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
