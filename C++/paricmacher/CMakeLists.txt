cmake_minimum_required(VERSION 3.14)
project(paricmacher)

set(CMAKE_CXX_STANDARD 14)

find_package (Threads)

add_executable(paricmacher main.cpp)

target_link_libraries (paricmacher ${CMAKE_THREAD_LIBS_INIT})