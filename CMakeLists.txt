cmake_minimum_required(VERSION 3.10)
project(hft_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O3 -march=native -mtune=native -DNDEBUG")

include_directories(include)
add_executable(hft_engine src/main.cpp)
