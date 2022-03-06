cmake_minimum_required(VERSION 3.21)
project(For_Test)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(For_Test
        MYALLOCATOR.h
        main.cpp "congestion control_Network.cpp")
