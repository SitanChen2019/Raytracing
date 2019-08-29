cmake_minimum_required(VERSION 3.14)
project(Raytracing)

set(CMAKE_CXX_STANDARD 14)

file(GLOB core_files src/*.h scr/*.cpp)
add_executable(Raytracing
        main.cpp
        ${core_files} core/Math.h core/Ray.h core/core.h)