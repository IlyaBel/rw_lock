cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 20)

project(rw_lock)
add_executable(${PROJECT_NAME} "main.cpp" "rw_lock.cpp")
