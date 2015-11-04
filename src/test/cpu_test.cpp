#include <gtest/gtest.h>

#include <cpu.hpp>
using namespace std;

namespace narcissus {

    namespace cpu {

        // ADD
//         TEST(cpu, ADD_B_IMM)
//         {
//             array<std::uint8_t, cpu::ROM_SIZE> mem = {0};
//             mem[0] = 0x00;
//             mem[1] = 0x00;
//             mem[2] = 0x01;
//             mem[3] = 0x00;

//            // ADD.B #0xff, r2l
//             mem[0x100] = 0x8a;
//             mem[0x101] = 0xff;

//             cpu::h8_300 cpu(move(mem));
//             cpu.reset_exception();

//             ASSERT_EQ(cpu::operation::ADD_B_IMM, cpu.detect_operation());

//             ASSERT_EQ(true, cpu.cycle());

//             ASSERT_EQ(cpu.er[2].l, 0x0ff);

//             ASSERT_EQ(cpu.pc, 0x102);
//         }

//         TEST(cpu, ADD_B_R_R)
//         {
//             array<std::uint8_t, cpu::ROM_SIZE> mem = {0};
//             mem[0] = 0x00;
//             mem[1] = 0x00;
//             mem[2] = 0x01;
//             mem[3] = 0x00;

            // ADD.B r2h, r4l
//             mem[0x100] = 0x08;
//             mem[0x101] = 0x2c;

//             cpu::h8_300 cpu(move(mem));
//             cpu.reset_exception();

//             cpu.er[2].h = 0x12;

//             ASSERT_EQ(cpu::operation::ADD_B_R_R, cpu.detect_operation());

//             ASSERT_EQ(true, cpu.cycle());

//             ASSERT_EQ(cpu.er[2].h, 0x12);

//             ASSERT_EQ(cpu.pc, 0x102);
//         }

//         TEST(cpu, ADD_W_IMM)
//         {
//             array<std::uint8_t, cpu::ROM_SIZE> mem = {0};
//             mem[0] = 0x00;
//             mem[1] = 0x00;
//             mem[2] = 0x01;
//             mem[3] = 0x00;

//             ADD.W #0x1234, e3
//             mem[0x100] = 0x79;
//             mem[0x101] = 0x1b;
//             mem[0x102] = 0x12;
//             mem[0x103] = 0x34;

//             cpu::h8_300 cpu(move(mem));
//             cpu.reset_exception();

//             ASSERT_EQ(cpu::operation::ADD_W_IMM, cpu.detect_operation());

//             ASSERT_EQ(true, cpu.cycle());
//             ASSERT_EQ(cpu.er[3].e, 0x1234);
//             ASSERT_EQ(cpu.pc, 0x104);
//         }

//         TEST(cpu, ADD_W_R_R)
//         {
//             array<std::uint8_t, cpu::ROM_SIZE> mem = {0};
//             mem[0] = 0x00;
//             mem[1] = 0x00;
//             mem[2] = 0x01;
//             mem[3] = 0x00;

//             ADD.W e3, r6
//             mem[0x100] = 0x09;
//             mem[0x101] = 0xb6;

//             cpu::h8_300 cpu(move(mem));
//             cpu.reset_exception();

//             cpu.er[3].e = 0x5678;

//             ASSERT_EQ(cpu::operation::ADD_W_R_R, cpu.detect_operation());
//             ASSERT_EQ(true, cpu.cycle());
//             ASSERT_EQ(cpu.er[6].r, 0x5678);
//             ASSERT_EQ(cpu.pc, 0x102);
//         }

//         TEST(cpu, ADD_L_IMM)
//         {
//             array<std::uint8_t, cpu::ROM_SIZE> mem = {0};
//             mem[0] = 0x00;
//             mem[1] = 0x00;
//             mem[2] = 0x01;
//             mem[3] = 0x00;

////             ADD.L #0x12345678, r5
//             mem[0x100] = 0x7a;
//             mem[0x101] = 0x15;
//             mem[0x102] = 0x12;
//             mem[0x103] = 0x34;
//             mem[0x104] = 0x56;
//             mem[0x105] = 0x78;

//             cpu::h8_300 cpu(move(mem));
//             cpu.reset_exception();

//             ASSERT_EQ(cpu::operation::ADD_L_IMM, cpu.detect_operation());
//             ASSERT_EQ(true, cpu.cycle());
//             ASSERT_EQ(cpu.er[5].er32, 0x12345678);
//             ASSERT_EQ(cpu.pc, 0x106);
//         }

//         TEST(cpu, ADD_L_R_R)
//         {
//             array<std::uint8_t, cpu::ROM_SIZE> mem = {0};
//             mem[0] = 0x00;
//             mem[1] = 0x00;
//             mem[2] = 0x01;
//             mem[3] = 0x00;

//             ADD.L re2, re1
//             mem[0x100] = 0x0a;
//             mem[0x101] = 0xa1;

//             cpu::h8_300 cpu(move(mem));
//             cpu.reset_exception();

//             cpu.er[2].er32 = 0x12345678;

//             ASSERT_EQ(cpu::operation::ADD_L_R_R, cpu.detect_operation());
//             ASSERT_EQ(true, cpu.cycle());
//             ASSERT_EQ(cpu.er[2].er32, 0x12345678);
//             ASSERT_EQ(cpu.pc, 0x102);
//         }
        
        // SUB
        TEST(SUB_B_R_R, 0)
        {
            array<std::uint8_t, cpu::ROM_SIZE> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //sub.b r0l, r0l
            mem[0x100] = 0x18;
            mem[0x101] = 0x82;

            cpu::h8_300 cpu(move(mem));
            cpu.reset_exception();

            cpu.er[2].h = 0x34;
            cpu.er[0].l = 0x45;

            ASSERT_EQ(cpu::operation::SUB_B_R_R, cpu.detect_operation());
            ASSERT_EQ(true, cpu.cycle());
            ASSERT_EQ((0x34 - 0x45) & 0xff, cpu.er[2].h);
            ASSERT_EQ(0x102, cpu.pc);

//             std::cout << "ccr: " << (uint16_t)cpu.ccr.byte << std::endl;
            ASSERT_EQ(0b10001001, cpu.ccr.byte); //1000 1001
        }

        TEST(SUB_W_R_R, 0)
        {
            array<std::uint8_t, cpu::ROM_SIZE> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //sub.w   r0,r0
            //19 00
            mem[0x100] = 0x19;
            mem[0x101] = 0x00;

            cpu::h8_300 cpu(move(mem));
            cpu.reset_exception();

            cpu.er[0].r = 0x1234;

            ASSERT_EQ(cpu::operation::SUB_W_R_R, cpu.detect_operation());
            ASSERT_EQ(true, cpu.cycle());
            ASSERT_EQ(0x0, cpu.er[0].r);
            ASSERT_EQ(0x102, cpu.pc);

            ASSERT_EQ(0b10000100, cpu.ccr.byte);
        }

        TEST(SUB_W_R_R, 1)
        {
            array<std::uint8_t, cpu::ROM_SIZE> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //sub.w   r0,r0
            //19 00
            mem[0x100] = 0x19;
            mem[0x101] = 0x03;

            cpu::h8_300 cpu(move(mem));
            cpu.reset_exception();

            cpu.er[0].r = 0x5555;
            cpu.er[3].r = 0x1111;

            ASSERT_EQ(cpu::operation::SUB_W_R_R, cpu.detect_operation());
            ASSERT_EQ(true, cpu.cycle());
            ASSERT_EQ((0x1111 - 0x5555) & 0xffff, cpu.er[3].r);
            ASSERT_EQ(0x102, cpu.pc);

//             std::cout << (uint16_t)cpu.ccr.byte << std::endl;
            //0x89
            //10001001 
            ASSERT_EQ(0b10001001, cpu.ccr.byte);
        }

        //MOV
        TEST(MOV_B_IMM, 0)
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

            ASSERT_EQ(0b10000000, cpu.ccr.byte);
        }

        TEST(MOV_B_IMM, 1)
        {
            array<std::uint8_t, cpu::ROM_SIZE> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            // MOV.B #0x12, r2h
            mem[0x100] = 0xf2;
            mem[0x101] = 0x00;

            cpu::h8_300 cpu(move(mem));
            cpu.reset_exception();

            ASSERT_EQ(cpu::operation::MOV_B_IMM, cpu.detect_operation());
            ASSERT_EQ(true, cpu.cycle());
            ASSERT_EQ(cpu.er[2].h, 0x0);
            ASSERT_EQ(cpu.pc, 0x102);

            ASSERT_EQ(0b10000100, cpu.ccr.byte);

        }

        TEST(MOV_B_R_IND, 0){
            array<std::uint8_t, cpu::ROM_SIZE> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //MOV.B r0l, @er2
            mem[0x100] = 0x68;
            mem[0x101] = 0xb8;

            cpu::h8_300 cpu(move(mem));
            cpu.reset_exception();

            cpu.er[0].l = 0x12;
            cpu.er[3].er32 = 0x00000200;

            ASSERT_EQ(cpu::operation::MOV_B_R_IND, cpu.detect_operation());
            ASSERT_EQ(true, cpu.cycle());
            ASSERT_EQ(0x12, cpu.memory[0x200]);
            ASSERT_EQ(cpu.pc, 0x102);
        }

        TEST(MOV_B_R_IND_WITH_DIS_16, 0)
        {
            array<std::uint8_t, cpu::ROM_SIZE> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            // mov.b   r0l,@(0x2:16,er3)
            // 6e b8 00 02     
            mem[0x100] = 0x6e;
            mem[0x101] = 0xb8;
            mem[0x102] = 0x00;
            mem[0x103] = 0x02;

            cpu::h8_300 cpu(move(mem));
            cpu.reset_exception();

            //source
            cpu.er[0].l = 0x12;

            //destination
            cpu.er[3].er32 = 0x00000200;

            ASSERT_EQ(cpu::operation::MOV_B_R_IND_WITH_DIS_16, cpu.detect_operation());
            ASSERT_EQ(true, cpu.cycle());
            ASSERT_EQ(0x12, cpu.memory[0x200 + 0x2]);
            ASSERT_EQ(cpu.pc, 0x104);

            ASSERT_EQ(0b10000000, cpu.ccr.byte);
        }

        TEST(MOV_W_IMM, 0)
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

            ASSERT_EQ(0b10000000, cpu.ccr.byte);
        }

        TEST(MOV_L_IMM, 0)
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

            ASSERT_EQ(0b10000000, cpu.ccr.byte);
        }

        TEST(MOV_L_R_IND, 0)
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

            ASSERT_EQ(0b10000000, cpu.ccr.byte);
        }

        TEST(MOV_L_R_R, 0)
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

            cpu.er[7].er32 = 0x00ffff00;

            ASSERT_EQ(cpu::operation::MOV_L_R_R, cpu.detect_operation());
            ASSERT_EQ(true, cpu.cycle());
            ASSERT_EQ(0x00ffff00, cpu.er[6].er32);
            ASSERT_EQ(cpu.pc, 0x102);

            ASSERT_EQ(0b10000000, cpu.ccr.byte);
        }

        TEST(MOV_L_R_IND_WITH_DIS_24, 0)
        {
            array<std::uint8_t, cpu::ROM_SIZE> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

//          01 00 78 00     mov.l   @(0x238:32,er0),er3
//          6b 23 00 00
//          02 38
            mem[0x100] = 0x01;
            mem[0x101] = 0x00;
            mem[0x102] = 0x78;
            mem[0x103] = 0x00;
            mem[0x104] = 0x6b;
            mem[0x105] = 0x23;
            mem[0x106] = 0x00;
            mem[0x107] = 0x00;
            mem[0x108] = 0x02;
            mem[0x109] = 0x38;

            cpu::h8_300 cpu(move(mem));
            cpu.reset_exception();

            cpu.er[0].er32 = 0x00000200;

            auto addr = cpu.er[0].er32 + 0x238;
            cpu.memory[addr] = 0x12;
            cpu.memory[addr + 1] = 0x34;
            cpu.memory[addr + 2] = 0x56;
            cpu.memory[addr + 3] = 0x78;

            ASSERT_EQ(cpu::operation::MOV_L_R_IND_WITH_DIS_24, cpu.detect_operation());
            ASSERT_EQ(true, cpu.cycle());
            ASSERT_EQ(cpu.er[3].er32, 0x12345678);
            ASSERT_EQ(cpu.pc, 0x10a);

            ASSERT_EQ(0b10000000, cpu.ccr.byte);
        }

        TEST(MOV_L_R_IND_POST_INC, 0)
        {
            array<std::uint8_t, cpu::ROM_SIZE> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //mov.l   @er7+,er6     :(pop er6)
            //01 00 6d 76     
            mem[0x100] = 0x01;
            mem[0x101] = 0x00;
            mem[0x102] = 0x6d;
            mem[0x103] = 0x76;
 
            cpu::h8_300 cpu(move(mem));
            cpu.reset_exception();

            cpu.sp = 0xffff00;
            auto value = 0x12345678;
            cpu.memory[--cpu.sp] = (uint8_t)value;
            cpu.memory[--cpu.sp] = (uint8_t)(value >> 8);
            cpu.memory[--cpu.sp] = (uint8_t)(value >> 16);
            cpu.memory[--cpu.sp] = (uint8_t)(value >> 24);

//             std::cout << "stack top:" << std::endl;
//             for (auto i = 0; i < 4; i++) {
//                 std::cout << cpu.memory[cpu.sp + i - 4] << std::endl;
//             }
//             std::cout << cpu.memory[cpu.sp] << std::endl;

            ASSERT_EQ(cpu::operation::MOV_L_R_IND_POST_INC, cpu.detect_operation());
            ASSERT_EQ(true, cpu.cycle());
            ASSERT_EQ(0x12345678, cpu.er[6].er32);
            ASSERT_EQ(cpu.pc, 0x104);

            ASSERT_EQ(0b10000000, cpu.ccr.byte);
        }
        //bcc
        TEST(BEQ, 0)
        {
            array<std::uint8_t, cpu::ROM_SIZE> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //beq .+8
            mem[0x100] = 0x47;
            mem[0x101] = 0x08;

            cpu::h8_300 cpu(move(mem));
            cpu.reset_exception();

//             std::cout << "zero :" << cpu.ccr.zero << std::endl;
            cpu.ccr.zero = 1;
//             std::cout << "zero :" << cpu.ccr.zero << std::endl;

            ASSERT_EQ(cpu::operation::BEQ, cpu.detect_operation());
            ASSERT_EQ(true, cpu.cycle());
            ASSERT_EQ(cpu.pc, 0x102 + 0x8);
        }

        //jsr
        TEST(JSR_ABS, 0)
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

//             cout << "0x" << cpu.sp << endl;
//             cout << "0x" << cpu.pc << endl;
//             cout << "0x" << (uint32_t)cpu.memory[cpu.sp + 1] << endl;
//             cout << "0x" << (uint32_t)cpu.memory[cpu.sp + 2] << endl;
//             cout << "0x" << (uint32_t)cpu.memory[cpu.sp + 3] << endl;

            // cpu.memory[cpu.pc]: is reserved
            auto return_addr = (uint32_t)cpu.memory[cpu.sp + 1] << 16;
            return_addr |= (uint32_t)cpu.memory[cpu.sp + 2] << 8;
            return_addr |= (uint32_t)cpu.memory[cpu.sp + 3];

            ASSERT_EQ(0x104, return_addr);
        }

        TEST(EXTS_L, 0) 
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

            ASSERT_EQ(0b10001000, cpu.ccr.byte);
        }

        TEST(SHLL_L, 0)
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
           
            ASSERT_EQ(0b10000000, cpu.ccr.byte);
        }

        TEST(RTS, 0)
        {
            array<std::uint8_t, cpu::ROM_SIZE> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;


            // jsr @0x000110:24
            mem[0x100] = 0x5e;
            mem[0x101] = 0x00;
            mem[0x102] = 0x01;
            mem[0x103] = 0x10;

            // rts
            // 54 70
            mem[0x110] = 0x54;
            mem[0x111] = 0x70;

            cpu::h8_300 cpu(move(mem));
            cpu.reset_exception();

            cpu.sp = 0x00ffff00;

            // jsr 
            ASSERT_EQ(cpu::operation::JSR_ABS, cpu.detect_operation());
            ASSERT_EQ(true, cpu.cycle());
//             std::cout << "jsr" << std::endl;
            // rts
            ASSERT_EQ(cpu::operation::RTS, cpu.detect_operation());
            ASSERT_EQ(true, cpu.cycle());


            ASSERT_EQ(cpu.pc, 0x104);
        }

    }  // namespace cpu
}  // namespace narcissus

// TODO added code for condition code register
