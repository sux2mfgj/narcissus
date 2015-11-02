#include <gtest/gtest.h>

#include <cpu.hpp>
using namespace std;

namespace narcissus {

    namespace cpu {

        // ADD
        TEST(cpu, ADD_B_IMM)
        {
            array<std::uint8_t, cpu::ROM_SIZE> mem = {0};
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

            ASSERT_EQ(cpu.pc, 0x102);
        }

        TEST(cpu, ADD_B_R_R)
        {
            array<std::uint8_t, cpu::ROM_SIZE> mem = {0};
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

            ASSERT_EQ(cpu.pc, 0x102);
        }

        TEST(cpu, ADD_W_IMM)
        {
            array<std::uint8_t, cpu::ROM_SIZE> mem = {0};
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
            ASSERT_EQ(cpu.pc, 0x104);
        }

        TEST(cpu, ADD_W_R_R)
        {
            array<std::uint8_t, cpu::ROM_SIZE> mem = {0};
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
            ASSERT_EQ(cpu.pc, 0x102);
        }

        TEST(cpu, ADD_L_IMM)
        {
            array<std::uint8_t, cpu::ROM_SIZE> mem = {0};
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
            ASSERT_EQ(cpu.pc, 0x106);
        }

        TEST(cpu, ADD_L_R_R)
        {
            array<std::uint8_t, cpu::ROM_SIZE> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            // ADD.L re2, re1
            mem[0x100] = 0x0a;
            mem[0x101] = 0xa1;

            cpu::h8_300 cpu(move(mem));
            cpu.reset_exception();

            cpu.er[2].er32 = 0x12345678;

            ASSERT_EQ(cpu::operation::ADD_L_R_R, cpu.detect_operation());
            ASSERT_EQ(true, cpu.cycle());
            ASSERT_EQ(cpu.er[2].er32, 0x12345678);
            ASSERT_EQ(cpu.pc, 0x102);
        }

        //MOV
        TEST(cpu, MOV_B_IMM)
        {
            array<std::uint8_t, cpu::ROM_SIZE> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            // MOV.B #0x12, r2h
            mem[0x100] = 0xf2;
            mem[0x101] = 0x12;

            cpu::h8_300 cpu(move(mem));
            cpu.reset_exception();

            ASSERT_EQ(cpu::operation::MOV_B_IMM, cpu.detect_operation());
            ASSERT_EQ(true, cpu.cycle());
            ASSERT_EQ(cpu.er[2].h, 0x12);
            ASSERT_EQ(cpu.pc, 0x102);
        }

        //         TEST(cpu, MOV_B_R_IND){
        //             TODO
        //             using namespace std;
        //             array<std::uint8_t, cpu::ROM_SIZE> mem = {0};
        //             mem[0] = 0x00;
        //             mem[1] = 0x00;
        //             mem[2] = 0x01;
        //             mem[3] = 0x00;

        //             MOV.B @er2, r3l
        //             mem[0x100] = 0x68;
        //             mem[0x101] = 0x2b;

        //             cpu::h8_300 cpu(move(mem));
        //             cpu.reset_exception();

        //             cpu.

        //             ASSERT_EQ(cpu::operation::MOV_B_R_IND, cpu.detect_operation());
        //             ASSERT_EQ(true, cpu.cycle());
        //             ASSERT_EQ()
        //             ASSERT_EQ(cpu.pc, 0x102);
        //         }

        TEST(cpu, MOV_W_IMM)
        {
            array<std::uint8_t, cpu::ROM_SIZE> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            // mov.w #0x1, r0
            mem[0x100] = 0x79;
            mem[0x101] = 0x00;
            mem[0x102] = 0x00;
            mem[0x103] = 0x01;

            cpu::h8_300 cpu(move(mem));
            cpu.reset_exception();

            ASSERT_EQ(cpu::operation::MOV_W_IMM, cpu.detect_operation());
            ASSERT_EQ(true, cpu.cycle());
            ASSERT_EQ(0x0001, cpu.er[0].r);
            ASSERT_EQ(cpu.pc, 0x104);
        }

        TEST(cpu, MOV_L_IMM)
        {
            array<std::uint8_t, cpu::ROM_SIZE> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            // MOV.L #0xffff00, er7
            // 7a 07 00 ff
            // ff 00
            mem[0x100] = 0x7a;
            mem[0x101] = 0x07;
            mem[0x102] = 0x00;
            mem[0x103] = 0xff;
            mem[0x104] = 0xff;
            mem[0x105] = 0x00;

            cpu::h8_300 cpu(move(mem));
            cpu.reset_exception();

            ASSERT_EQ(cpu::operation::MOV_L_IMM, cpu.detect_operation());
            ASSERT_EQ(true, cpu.cycle());
            ASSERT_EQ(cpu.er[7].er32, 0x00ffff00);
            ASSERT_EQ(cpu.pc, 0x106);
        }

        TEST(cpu, MOV_L_R_IND)
        {
            array<std::uint8_t, cpu::ROM_SIZE> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            // mov.l er6,@-er7
            mem[0x100] = 0x01;
            mem[0x101] = 0x00;
            mem[0x102] = 0x6d;
            mem[0x103] = 0xf6;  // 1111 0110

            cpu::h8_300 cpu(move(mem));
            cpu.reset_exception();

            cpu.er[6].er32 = 0x12345678;
            cpu.sp = 0x00ffff00;

            ASSERT_EQ(cpu::operation::MOV_L_R_IND, cpu.detect_operation());
            ASSERT_EQ(true, cpu.cycle());
            ASSERT_EQ(0x12, cpu.memory[0xffff00 - 4]);
            ASSERT_EQ(0x34, cpu.memory[0xffff00 - 3]);
            ASSERT_EQ(0x56, cpu.memory[0xffff00 - 2]);
            ASSERT_EQ(0x78, cpu.memory[0xffff00 - 1]);
            ASSERT_EQ(cpu.pc, 0x104);
        }

        TEST(cpu, MOV_L_R_R)
        {
            array<std::uint8_t, cpu::ROM_SIZE> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            // mov.l er7, er6
            mem[0x100] = 0x0f;
            mem[0x101] = 0xf6;

            cpu::h8_300 cpu(move(mem));
            cpu.reset_exception();

            cpu.er[7].er32 = 0x12345678;

            ASSERT_EQ(cpu::operation::MOV_L_R_R, cpu.detect_operation());
            ASSERT_EQ(true, cpu.cycle());
            ASSERT_EQ(0x12345678, cpu.er[6].er32);
            ASSERT_EQ(cpu.pc, 0x102);
        }

        TEST(cpu, JSR_ABS)
        {
            array<std::uint8_t, cpu::ROM_SIZE> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //             jsr @0x10c:24
            mem[0x100] = 0x5e;
            mem[0x101] = 0x00;
            mem[0x102] = 0x01;
            mem[0x103] = 0x0c;

            cpu::h8_300 cpu(move(mem));
            cpu.reset_exception();

            cpu.sp = 0x00ffff00;

            ASSERT_EQ(cpu::operation::JSR_ABS, cpu.detect_operation());
            ASSERT_EQ(true, cpu.cycle());
            ASSERT_EQ(cpu.pc, 0x10c);

            cout << cpu.sp << endl;
            cout << cpu.pc << endl;
            cout << (uint32_t)cpu.memory[cpu.sp + 1] << endl;
            cout << (uint32_t)cpu.memory[cpu.sp + 2] << endl;
            cout << (uint32_t)cpu.memory[cpu.sp + 3] << endl;
        }

        TEST(cpu, EXTS_L) 
        {
            array<std::uint8_t, cpu::ROM_SIZE> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //exts.l er0
            mem[0x100] = 0x17;
            mem[0x101] = 0xf0;

            cpu::h8_300 cpu(move(mem));
            cpu.reset_exception();

            cpu.er[0].er32 = 0x00008001;

            ASSERT_EQ(cpu::operation::EXTS_L, cpu.detect_operation());
            ASSERT_EQ(true, cpu.cycle());
            ASSERT_EQ(0xffff8001, cpu.er[0].er32);
            ASSERT_EQ(cpu.pc, 0x102);
        }

        TEST(cpu, SHLL_L)
        {
             array<std::uint8_t, cpu::ROM_SIZE> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            // shhl.l er0
            mem[0x100] = 0x10;
            mem[0x101] = 0x30;

            cpu::h8_300 cpu(move(mem));
            cpu.reset_exception();

            cpu.er[0].er32 = 0x00000001;

            ASSERT_EQ(cpu::operation::SHLL_L, cpu.detect_operation());
            ASSERT_EQ(true, cpu.cycle());
            ASSERT_EQ(0x00000002, cpu.er[0].er32);
            ASSERT_EQ(cpu.pc, 0x102);
           
        }

    }  // namespace cpu
}  // namespace narcissus

// TODO added code for condition code register
