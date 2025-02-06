cmake_minimum_required(VERSION 3.16)
project(test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)


find_package(ament_cmake REQUIRED)
find_package(ipc REQUIRED)


add_executable(primary
  src/primary.cpp
)
add_executable(secondary
  src/secondary.cpp
)
target_link_libraries(primary
  PRIVATE
    ipc::ipc_lib
)
target_link_libraries(secondary
  PRIVATE
    ipc::ipc_lib
)

install(
  TARGETS primary secondary
  RUNTIME DESTINATION bin
)

ament_package()