cmake_minimum_required (VERSION 3.2 FATAL_ERROR)
project(Fraction VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
add_executable(fraction
    src/main.cpp src/fraction.hpp)

