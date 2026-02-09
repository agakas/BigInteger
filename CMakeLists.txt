cmake_minimum_required(VERSION 3.10)
project(biginteger LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(biginteger
    src/main.cpp
    src/biginteger.cpp
    src/biginteger_literals.cpp
)

target_include_directories(biginteger PRIVATE include)