#include <gtest/gtest.h>

#include <cpu.hpp>

namespace narcissus {

    namespace cpu {

        TEST(cpu, ADD_B_IMM){
            using namespace std;

            array<uint8_t, cpu::ROM_SIZE> mem = {0};
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

            ASSERT_EQ(true, cpu.cycle());

            ASSERT_EQ(cpu.er[2].l, 0x0ff);

            ASSERT_EQ(cpu.er[7].er32, 0x102);
        }

        TEST(cpu, ADD_B_R_R) {
            using namespace std;

            array<uint8_t, cpu::ROM_SIZE> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            // ADD.B r2h, r4l
            mem[0x100] = 0x08;
            mem[0x101] = 0x2c;

            cpu::h8_300 cpu(move(mem));
            cpu.reset_exception();

            cpu.er[2].h = 0x12;

            ASSERT_EQ(cpu::operation::ADD_B_R_R, cpu.detect_operation());

            ASSERT_EQ(true, cpu.cycle());

            ASSERT_EQ(cpu.er[2].h, 0x12);

            ASSERT_EQ(cpu.er[7].er32, 0x102);
        }

    } // namespace cpu
} // namespace narcissus
