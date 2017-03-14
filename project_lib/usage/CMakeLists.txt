cmake_minimum_required(VERSION 3.0)
project(UsageExample)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

add_subdirectory(dir1)
add_subdirectory(dir2)

link_directories(../lib)
include_directories(..)
add_executable(${PROJECT_NAME} main.cpp)


target_link_libraries(${PROJECT_NAME} PUBLIC projectlib_cmake_sh)
target_link_libraries(${PROJECT_NAME} PUBLIC dir1 dir2)
