cmake_minimum_required(VERSION 2.8)
project(narcissus)

set(CMAKE_CXX_FLAGS "-std=c++14 -O0")

# Boost
set(BOOST_ROOT $ENV{HOME}/local/)
#set(Boost_ADDITIONAL_VERSION "1.59" "1.59.0")
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)


find_package(Boost 1.59.0 COMPONENTS program_options)
if(NOT Boost_FOUND)
    message(SEND_ERROR "Boost is not found")
endif()

enable_testing()

add_subdirectory(googletest/googletest)
add_subdirectory(src/test)
add_subdirectory(src/cpu)

include_directories(
    src/include
    ${gtest_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
)

add_definitions(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})

add_executable(narcissus src/main.cpp)
target_link_libraries(
    narcissus
    cpu
    #    sci
    pthread
    ${Boost_LIBRARIES}
) 
