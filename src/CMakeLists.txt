cmake_minimum_required(VERSION 3.20)
project(src)

set(CMAKE_CXX_STANDARD 14)
set(THREADS_PREFER_PTHREAD_FLAG ON)
add_executable(src server.cpp)
