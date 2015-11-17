include_directories(
    ${gtest_SOURCE_DIR}/include
    ../include
    )

# cpu test
add_executable(cpu_test cpu_test.cpp ../cpu/cpu.cpp ../cpu/mcu.cpp ../cpu/sci.cpp)
target_link_libraries(
    cpu_test 
    gtest 
    gtest_main
)
add_test(
    NAME cpu
    COMMAND cpu_test
    )

