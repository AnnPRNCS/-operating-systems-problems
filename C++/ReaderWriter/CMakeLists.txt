cmake_minimum_required(VERSION 3.14)
project(ReaderWriter)

set(CMAKE_CXX_STANDARD 17)

find_package (Threads)

add_executable(ReaderWriter main.cpp writer.cpp)

target_link_libraries (ReaderWriter ${CMAKE_THREAD_LIBS_INIT})