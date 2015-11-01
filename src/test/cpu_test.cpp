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

            ASSERT_EQ(cpu.PC, 0x102);
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

            ASSERT_EQ(cpu.PC, 0x102);
        }

        TEST(cpu, ADD_W_IMM) {
            using namespace std;
            array<uint8_t, cpu::ROM_SIZE> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            // ADD.W #0x1234, e3
            mem[0x100] = 0x79;
            mem[0x101] = 0x1b;
            mem[0x102] = 0x12;
            mem[0x103] = 0x34;

            cpu::h8_300 cpu(move(mem));
            cpu.reset_exception();
    
            ASSERT_EQ(cpu::operation::ADD_W_IMM, cpu.detect_operation());

            ASSERT_EQ(true, cpu.cycle());
            ASSERT_EQ(cpu.er[3].e, 0x1234);
            ASSERT_EQ(cpu.PC, 0x104);
        }

        TEST(cpu, ADD_W_R_R) {
            using namespace std;
            array<uint8_t, cpu::ROM_SIZE> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            // ADD.W e3, r6
            mem[0x100] = 0x09;
            mem[0x101] = 0xb6;

            cpu::h8_300 cpu(move(mem));
            cpu.reset_exception();

            cpu.er[3].e = 0x5678;

            ASSERT_EQ(cpu::operation::ADD_W_R_R, cpu.detect_operation());
            ASSERT_EQ(true, cpu.cycle());
            ASSERT_EQ(cpu.er[6].r, 0x5678);
            ASSERT_EQ(cpu.PC, 0x102);

        }

        TEST(cpu, ADD_L_IMM) {
            using namespace std;
            array<uint8_t, cpu::ROM_SIZE> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            // ADD.L #0x12345678, r5
            mem[0x100] = 0x7a;
            mem[0x101] = 0x15;
            mem[0x102] = 0x12;
            mem[0x103] = 0x34;
            mem[0x104] = 0x56;
            mem[0x105] = 0x78;

            cpu::h8_300 cpu(move(mem));
            cpu.reset_exception();

            ASSERT_EQ(cpu::operation::ADD_L_IMM, cpu.detect_operation());
            ASSERT_EQ(true, cpu.cycle());
            ASSERT_EQ(cpu.er[5].er32, 0x12345678);
            ASSERT_EQ(cpu.PC, 0x106);
        }

    } // namespace cpu
} // namespace narcissus
