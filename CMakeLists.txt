cmake_minimum_required(VERSION 3.10)

project(BMPWatermarkApplicator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(bin)
add_subdirectory(tests)