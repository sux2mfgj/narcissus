cmake_minimum_required(VERSION 2.8)
project(narcissus)

set(CMAKE_CXX_FLAGS "-std=c++14")

enable_testing()

add_subdirectory(googletest/googletest)
add_subdirectory(src/test)

add_subdirectory(src/cpu)
include_directories(
    src/include
    ${gtest_SOURCE_DIR}/include
)

add_executable(narcissus src/main.cpp)

target_link_libraries(
    narcissus
    cpu
) 
