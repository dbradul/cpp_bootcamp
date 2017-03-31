cmake_minimum_required(VERSION 3.0)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")

add_library(projectlib_cmake project_lib.cpp)
add_library(projectlib_cmake_sh SHARED project_lib.cpp)
add_executable(main_cmake main.cpp)
target_link_libraries(main_cmake projectlib_cmake_sh)
