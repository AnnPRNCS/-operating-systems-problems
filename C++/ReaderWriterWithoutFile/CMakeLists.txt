cmake_minimum_required(VERSION 3.14)
project(ReaderWriterWithoutFile)

set(CMAKE_CXX_STANDARD 14)

find_package (Threads)

add_executable(ReaderWriterWithoutFile main.cpp)

target_link_libraries (ReaderWriterWithoutFile ${CMAKE_THREAD_LIBS_INIT})