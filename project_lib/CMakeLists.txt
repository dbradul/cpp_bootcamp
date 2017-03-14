 cmake_minimum_required(VERSION 2.4)

project(main)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")
#set the default path for built executables to the "bin" directory
#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
#set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR})
add_library(projectlib_cmake project_lib.cpp)
add_library(projectlib_cmake_sh SHARED project_lib.cpp)
add_executable(main_cmake main.cpp)
target_link_libraries(main_cmake projectlib_cmake)
