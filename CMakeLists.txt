cmake_minimum_required(VERSION 3.14)
project(MPI)

set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_C_COMPILER mpicc)
SET(CMAKE_CXX_COMPILER mpicxx)

add_executable(ex01 ex01.cpp )
add_executable(ex02 ex02.cpp)
add_executable(ex03 ex03.cpp)