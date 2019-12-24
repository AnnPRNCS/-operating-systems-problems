cmake_minimum_required(VERSION 3.14)
project(barber)

set(CMAKE_CXX_STANDARD 14)

find_package (Threads)

add_executable(barber main.cpp)

target_link_libraries (barber ${CMAKE_THREAD_LIBS_INIT})