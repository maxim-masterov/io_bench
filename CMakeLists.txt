cmake_minimum_required(VERSION 3.23)
project(IO_bench)

set(CMAKE_CXX_STANDARD 17)

add_executable(IO_bench main.cpp Vector.h IOManager.h)
