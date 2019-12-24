cmake_minimum_required(VERSION 3.14)
project(philosofi)

set(CMAKE_CXX_STANDARD 14)

find_package (Threads)

add_executable(philosofi main.cpp)

target_link_libraries (philosofi ${CMAKE_THREAD_LIBS_INIT})