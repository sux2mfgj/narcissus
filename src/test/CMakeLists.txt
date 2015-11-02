include_directories(
    ${gtest_SOURCE_DIR}/include
    ../include
    )

add_executable(cpu_test cpu_test.cpp ../cpu/cpu.cpp ../cpu/mcu.cpp)

target_link_libraries(
    cpu_test 
    gtest 
    gtest_main
)

add_test(
    NAME cpu
    COMMAND cpu_test
    )
