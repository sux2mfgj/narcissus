cmake_minimum_required(VERSION 2.8)

include_directories(
    ../include
    ${Boost_INCLUDE_DIRS}
    ${gtest_SOURCE_DIR}/include
    )

add_library(cpu STATIC
    cpu.cpp
    mcu.cpp
    sci.cpp
    )
