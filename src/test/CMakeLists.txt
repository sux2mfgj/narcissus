include_directories(
    ${gtest_SOURCE_DIR}/include
    ../include
    )

# cpu test
add_executable(cpu_test cpu_test.cpp ../cpu/cpu.cpp ../cpu/mcu.cpp ../sci/sci.cpp)
target_link_libraries(
    cpu_test 
    gtest 
    gtest_main
)
add_test(
    NAME cpu
    COMMAND cpu_test
    )

# sci test
add_executable(sci_test sci_test.cpp ../sci/sci.cpp)
target_link_libraries(
    sci_test
    gtest
    gtest_main
    )
add_test(
    NAME sci
    COMMAND sci_test
    )
