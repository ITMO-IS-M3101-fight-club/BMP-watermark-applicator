cmake_minimum_required(VERSION 3.10)
project(MyTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(run_test 
    src/main_test_checker.cpp 
    src/ImageBMP.cpp
)

target_include_directories(run_test PRIVATE include)