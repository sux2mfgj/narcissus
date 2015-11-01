#include <gtest/gtest.h>

#include <cpu.hpp>

TEST(cpu, detection_operation){
    using namespace narcissus;
    using namespace std;

    array<uint8_t, cpu::ROM_SIZE> mem;
    mem[0] = 0x00;
    mem[1] = 0x00;
    mem[2] = 0x01;
    mem[3] = 0x00;

    // ADD.B #0xff, r2l
    mem[0x100] = 0x8a;
    mem[0x101] = 0xff;

    cpu::h8_300 cpu(move(mem));
    cpu.reset_exception();

    ASSERT_EQ(cpu::operation::ADD_B_IMM, cpu.detect_operation());
}
