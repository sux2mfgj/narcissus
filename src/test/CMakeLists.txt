include_directories(
    ${gtest_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
    ../include
    )

# cpu test
add_executable(cpu_test cpu_test.cpp ../cpu/cpu.cpp ../cpu/mcu.cpp ../cpu/sci.cpp)
target_link_libraries(
    cpu_test 
    gtest 
    gtest_main
    ${Boost_LIBRARIES}
)
add_test(
    NAME cpu
    COMMAND cpu_test
    )

