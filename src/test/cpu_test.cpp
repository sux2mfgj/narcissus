#include <gtest/gtest.h>

#include <cpu.hpp>

TEST(cpu, detection_operation){
    using namespace narcissus;
    cpu::h8_300 cpu;
    cpu.reset_exception();
    ASSERT_EQ(1, 1);
}
