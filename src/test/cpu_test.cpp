#include <gtest/gtest.h>

#include <memory>
#include <thread>

#include <cpu.hpp>

namespace narcissus {
    namespace h8_3069f {

        TEST(read_register, 0)
        {
            std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0x100] = 0x0a;
            mem[0x101] = 0x00;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));

            cpu->interrupt(h8_3069f::interrupts::reset);

            auto rs = cpu->read_register_fields(0x100, cpu::value_place::low, false);

            ASSERT_EQ(0xa, rs);
        }

        TEST(read_register, 1)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0x100] = 0x70;
            mem[0x101] = 0x00;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            auto ers = cpu->read_register_fields(0x100, cpu::value_place::high, true);

            ASSERT_EQ(0x7, ers);
        }

        TEST(read_imm, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0x101] = 0x12;
            mem[0x102] = 0x34;
            mem[0x103] = 0x56;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            auto imm = cpu->read_immediate(0x101, 3);
            ASSERT_EQ(0x123456, imm);
        }

        TEST(write_imm, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->write_immediate(0x100, 3, 0x123456);
            ASSERT_EQ(cpu->memory[0x100], 0x12);
            ASSERT_EQ(cpu->memory[0x101], 0x34);
            ASSERT_EQ(cpu->memory[0x102], 0x56);
        }

        TEST(read_value , 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[2].l = 0x12;
            cpu->er[2].h = 0x34;

            cpu->er[5].er = 0x12345678;

            ASSERT_EQ(0x12, cpu->read_register(0xa, register_size::BYTE));
            ASSERT_EQ(0x34, cpu->read_register(0x2, register_size::BYTE));
            ASSERT_EQ(0x3412, cpu->read_register(0x2, register_size::WORD));
            ASSERT_EQ(0x12345678, cpu->read_register(5, register_size::LONG));
        }

        TEST(write_register, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->write_register(0x2, 0x12, register_size::BYTE);
            std::cout << cpu->er[2].er << std::endl;
            ASSERT_EQ(0x12, cpu->er[2].h);

            cpu->write_register(0xa, 0x34, register_size::BYTE);
            ASSERT_EQ(0x34, cpu->er[2].l);

            cpu->write_register(0x5, 0x12345678, register_size::LONG);
            ASSERT_EQ(0x12345678, cpu->er[5].er);


        }

        TEST(ADD_L_R_R, 0)
                {
                   std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
                    mem[0] = 0x00;
                    mem[1] = 0x00;
                    mem[2] = 0x01;
                    mem[3] = 0x00;

                    //0a d2           
                    //add.l   er5,er2
                    mem[0x100] = 0x0a;
                    mem[0x101] = 0xd2;

                    auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
                    cpu->interrupt(h8_3069f::interrupts::reset);

                    cpu->er[5].er = 0x12345678;
                    cpu->er[2].er = 0x87654321;


                    ASSERT_EQ(h8_3069f::operation::ADD_L_R_R, cpu->detect_operation());
                    ASSERT_EQ(0x102, cpu->cycle());
                    ASSERT_EQ(0x99999999, cpu->er[2].er);
                }

        // SUB
        TEST(SUB_B_R_R, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //sub.b r0l, r0l
            mem[0x100] = 0x18;
            mem[0x101] = 0x82;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[2].h = 0x34;
            cpu->er[0].l = 0x45;

            ASSERT_EQ(h8_3069f::operation::SUB_B_R_R, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());
            ASSERT_EQ((0x34 - 0x45) & 0xff, cpu->er[2].h);

            //             std::cout << "ccr: " << (uint16_t)cpu->ccr.byte << std::endl;
            ASSERT_EQ(0b10001001, cpu->ccr.byte); //1000 1001
        }

        TEST(SUB_W_R_R, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //sub.w   r0,r0
            //19 00
            mem[0x100] = 0x19;
            mem[0x101] = 0x00;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[0].r = 0x1234;

            ASSERT_EQ(h8_3069f::operation::SUB_W_R_R, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());
            ASSERT_EQ(0x0, cpu->er[0].r);

            ASSERT_EQ(0b10000100, cpu->ccr.byte);
        }

        TEST(SUB_W_R_R, 1)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //sub.w   r0,r0
            //19 00
            mem[0x100] = 0x19;
            mem[0x101] = 0x03;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[0].r = 0x5555;
            cpu->er[3].r = 0x1111;

            ASSERT_EQ(h8_3069f::operation::SUB_W_R_R, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());
            ASSERT_EQ((0x1111 - 0x5555) & 0xffff, cpu->er[3].r);

            //             std::cout << (uint16_t)cpu->ccr.byte << std::endl;
            //0x89
            //10001001 
            ASSERT_EQ(0b10001001, cpu->ccr.byte);
        }

        TEST(SUBS_4, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //1b 97           
            //subs    #4,er7
            mem[0x100] = 0x1b;
            mem[0x101] = 0x97;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[7].er = 0x00ffff00;

            ASSERT_EQ(h8_3069f::operation::SUBS_4, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());
            ASSERT_EQ(0x00ffff00 - 4, cpu->er[7].er);
        }

        //MOV
        TEST(MOV_B_IMM_R, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            // MOV.B #0x12, r2h
            mem[0x100] = 0xf2;
            mem[0x101] = 0x12;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            ASSERT_EQ(h8_3069f::operation::MOV_B_IMM_R, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());
            ASSERT_EQ(cpu->er[2].h, 0x12);

            ASSERT_EQ(0b10000000, cpu->ccr.byte);
        }

        TEST(MOV_B_IMM_R, 1)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            // MOV.B #0x12, r2h
            mem[0x100] = 0xf2;
            mem[0x101] = 0x00;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            ASSERT_EQ(h8_3069f::operation::MOV_B_IMM_R, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());
            ASSERT_EQ(cpu->er[2].h, 0x0);

            ASSERT_EQ(0b10000100, cpu->ccr.byte);

        }

        TEST(MOV_B_R_R, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //0c 8c           
            //mov.b   r0l,r4l
            mem[0x100] = 0x0c;
            mem[0x101] = 0x8c;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[0].l = 0x12;

            ASSERT_EQ(h8_3069f::operation::MOV_B_R_R, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());
            ASSERT_EQ(cpu->er[4].l, 0x12);
            ASSERT_EQ(0b10000000, cpu->ccr.byte);
        }

        TEST(MOV_B_R_R_IND_WITH_DIS_16, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
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

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            //source
            cpu->er[0].l = 0x12;

            //destination
            cpu->er[3].er = 0x00000200;

            ASSERT_EQ(h8_3069f::operation::MOV_B_R_R_IND_WITH_DIS_16, cpu->detect_operation());
            ASSERT_EQ(0x104, cpu->cycle());
            ASSERT_EQ(0x12, cpu->memory[0x200 + 0x2]);

            ASSERT_EQ(0b10000000, cpu->ccr.byte);
        }

        TEST(MOV_B_R_IND_WITH_DIS_16_R, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //6e 6a ff ff     
            //mov.b   @(0x0100:16,er6),r2l
            mem[0x100] = 0x6e;
            mem[0x101] = 0x6a;
            mem[0x102] = 0x01;
            mem[0x103] = 0x00;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[6].er = 0x100;
            cpu->memory[0x200] = 0x12;

            ASSERT_EQ(h8_3069f::operation::MOV_B_R_IND_WITH_DIS_16_R, cpu->detect_operation());
            ASSERT_EQ(0x104, cpu->cycle());
            ASSERT_EQ(0x12, cpu->er[2].l);

            ASSERT_EQ(0b10000000, cpu->ccr.byte);
        }

        TEST(MOV_B_R_IND_POST_INC_R, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //mov.b   @er4+,r0l
            //6c 48           
            mem[0x100] = 0x6c;
            mem[0x101] = 0x48;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[4].er = 0x120;
            cpu->memory[0x120] = 0x12;

            ASSERT_EQ(h8_3069f::operation::MOV_B_R_IND_POST_INC_R, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());
            ASSERT_EQ(0x12, cpu->er[0].l);
            ASSERT_EQ(0x121, cpu->er[4].er);
        }

        TEST(MOV_W_IMM_R, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            // mov.w #0x1, r0
            mem[0x100] = 0x79;
            mem[0x101] = 0x00;
            mem[0x102] = 0x00;
            mem[0x103] = 0x01;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            ASSERT_EQ(h8_3069f::operation::MOV_W_IMM_R, cpu->detect_operation());
            ASSERT_EQ(0x104, cpu->cycle());
            ASSERT_EQ(0x0001, cpu->er[0].r);

            ASSERT_EQ(0b10000000, cpu->ccr.byte);
        }

        TEST(MOV_L_IMM_R, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
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

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            ASSERT_EQ(h8_3069f::operation::MOV_L_IMM_R, cpu->detect_operation());
            ASSERT_EQ(0x106, cpu->cycle());
            ASSERT_EQ(cpu->er[7].er, 0x00ffff00);

            ASSERT_EQ(0b10000000, cpu->ccr.byte);
        }

        TEST(MOV_L_R_R_IND_PRE_DEC, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            // mov.l er6,@-er7
            mem[0x100] = 0x01;
            mem[0x101] = 0x00;
            mem[0x102] = 0x6d;
            mem[0x103] = 0xf6;  // 1111 0110

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[6].er = 0x12345678;
            cpu->sp = 0x00ffff00;

            ASSERT_EQ(h8_3069f::operation::MOV_L_R_R_IND_PRE_DEC, cpu->detect_operation());
            ASSERT_EQ(0x104, cpu->cycle());
            ASSERT_EQ(0x12, cpu->memory[0xffff00 - 4]);
            ASSERT_EQ(0x34, cpu->memory[0xffff00 - 3]);
            ASSERT_EQ(0x56, cpu->memory[0xffff00 - 2]);
            ASSERT_EQ(0x78, cpu->memory[0xffff00 - 1]);

            ASSERT_EQ(0b10000000, cpu->ccr.byte);
        }

        TEST(MOV_L_R_R, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            // mov.l er7, er6
            mem[0x100] = 0x0f;
            mem[0x101] = 0xf6;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[7].er = 0x00ffff00;

            ASSERT_EQ(h8_3069f::operation::MOV_L_R_R, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());
            ASSERT_EQ(0x00ffff00, cpu->er[6].er);

            ASSERT_EQ(0b10000000, cpu->ccr.byte);
        }

        TEST(MOV_L_R_IND_WITH_DIS_24_R, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
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

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[0].er = 0x00000200;

            auto addr = cpu->er[0].er + 0x238;
            cpu->memory[addr] = 0x12;
            cpu->memory[addr + 1] = 0x34;
            cpu->memory[addr + 2] = 0x56;
            cpu->memory[addr + 3] = 0x78;

            ASSERT_EQ(h8_3069f::operation::MOV_L_R_IND_WITH_DIS_24_R, cpu->detect_operation());
            ASSERT_EQ(0x10a, cpu->cycle());
            ASSERT_EQ(cpu->er[3].er, 0x12345678);

            ASSERT_EQ(0b10000000, cpu->ccr.byte);
        }

        TEST(MOV_L_R_IND_POST_INC_R, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
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

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->sp = 0xffff00;
            auto value = 0x12345678;
            cpu->memory[--cpu->sp] = (uint8_t)value;
            cpu->memory[--cpu->sp] = (uint8_t)(value >> 8);
            cpu->memory[--cpu->sp] = (uint8_t)(value >> 16);
            cpu->memory[--cpu->sp] = (uint8_t)(value >> 24);

            //             std::cout << "stack top:" << std::endl;
            //             for (auto i = 0; i < 4; i++) {
            //                 std::cout << cpu->memory[cpu->sp + i - 4] << std::endl;
            //             }
            //             std::cout << cpu->memory[cpu->sp] << std::endl;

            ASSERT_EQ(h8_3069f::operation::MOV_L_R_IND_POST_INC_R, cpu->detect_operation());
            ASSERT_EQ(0x104, cpu->cycle());
            ASSERT_EQ(0x12345678, cpu->er[6].er);

            ASSERT_EQ(0b10000000, cpu->ccr.byte);
        }
        //bcc
        TEST(BEQ_8, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //beq .+8
            mem[0x100] = 0x47;
            mem[0x101] = 0x08;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            //             std::cout << "zero :" << cpu->ccr.zero << std::endl;
            cpu->ccr.zero = 1;
            //             std::cout << "zero :" << cpu->ccr.zero << std::endl;

            ASSERT_EQ(h8_3069f::operation::BEQ_8, cpu->detect_operation());
            ASSERT_EQ(0x10a, cpu->cycle());
        }

        TEST(BRA_8, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            // bra     .-2 (0x100)
            // 40 fe           
            mem[0x100] = 0x40;
            mem[0x101] = 0xfe;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            ASSERT_EQ(h8_3069f::operation::BRA_8, cpu->detect_operation());
            ASSERT_EQ(0x100, cpu->cycle());
        }

        TEST(BNE_8, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //46 0a           
            //bne     .+10 (0x110)
            mem[0x100] = 0x46;
            mem[0x101] = 0x0a;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->ccr.zero = 0;

            ASSERT_EQ(h8_3069f::operation::BNE_8, cpu->detect_operation());
            ASSERT_EQ(0x102 + 0xa, cpu->cycle());


        }

        TEST(CMP_B_IMM, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //a8 0a           
            //cmp.b   #0xa,r0l
            mem[0x100] = 0xa8;
            mem[0x101] = 0x0a;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[0].l = 0xa;

            ASSERT_EQ(h8_3069f::operation::CMP_B_IMM, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());
            ASSERT_EQ(0b10000100, cpu->ccr.byte);
        }

        TEST(CMP_B_IMM, 1)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //a8 0a           
            //cmp.b   #0xa,r0l
            mem[0x100] = 0xa8;
            mem[0x101] = 0x0a;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[0].l = 0x9;

            ASSERT_EQ(h8_3069f::operation::CMP_B_IMM, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());
            ASSERT_EQ(0b10001001, cpu->ccr.byte);
        }

        //jsr
        TEST(JSR_ABS, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //             jsr @0x10c:24
            mem[0x100] = 0x5e;
            mem[0x101] = 0x00;
            mem[0x102] = 0x01;
            mem[0x103] = 0x0c;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->sp = 0x00ffff00;

            ASSERT_EQ(h8_3069f::operation::JSR_ABS, cpu->detect_operation());
            ASSERT_EQ(0x10c, cpu->cycle());

            //             cout << "0x" << cpu->sp << endl;
            //             cout << "0x" << cpu->pc << endl;
            //             cout << "0x" << (uint32_t)cpu->memory[cpu->sp + 1] << endl;
            //             cout << "0x" << (uint32_t)cpu->memory[cpu->sp + 2] << endl;
            //             cout << "0x" << (uint32_t)cpu->memory[cpu->sp + 3] << endl;

            // cpu->memory[cpu->pc]: is reserved
            auto return_addr = (uint32_t)cpu->memory[cpu->sp + 1] << 16;
            return_addr |= (uint32_t)cpu->memory[cpu->sp + 2] << 8;
            return_addr |= (uint32_t)cpu->memory[cpu->sp + 3];

            ASSERT_EQ(0x104, return_addr);
        }

        TEST(EXTS_L, 0) 
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //exts.l er0
            mem[0x100] = 0x17;
            mem[0x101] = 0xf0;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[0].er = 0x00008001;

            ASSERT_EQ(h8_3069f::operation::EXTS_L, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());
            ASSERT_EQ(0xffff8001, cpu->er[0].er);

            ASSERT_EQ(0b10001000, cpu->ccr.byte);
        }

        TEST(EXTS_L, 1) 
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //exts.l er0
            mem[0x100] = 0x17;
            mem[0x101] = 0xf0;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[0].er = 0x00010000;

            ASSERT_EQ(h8_3069f::operation::EXTS_L, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());
            ASSERT_EQ(0x00000000, cpu->er[0].er);

            ASSERT_EQ(0b10000100, cpu->ccr.byte);
        }

        TEST(SHLL_L, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            // shhl.l er0
            mem[0x100] = 0x10;
            mem[0x101] = 0x30;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[0].er = 0x00000001;

            ASSERT_EQ(h8_3069f::operation::SHLL_L, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());
            ASSERT_EQ(0x00000002, cpu->er[0].er);

            ASSERT_EQ(0b10000000, cpu->ccr.byte);
        }

        TEST(RTS, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
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

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->sp = 0x00ffff00;

            // jsr 
            ASSERT_EQ(h8_3069f::operation::JSR_ABS, cpu->detect_operation());
            ASSERT_EQ(0x110, cpu->cycle());
            //             std::cout << "jsr" << std::endl;
            // rts
            ASSERT_EQ(h8_3069f::operation::RTS, cpu->detect_operation());
            ASSERT_EQ(0x104, cpu->cycle());


        }

        TEST(MOV_W_R_R, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //0d 05           
            //mov.w   r0,r5
            mem[0x100] = 0x0d;
            mem[0x101] = 0x05;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[0].r = 0x12;

            ASSERT_EQ(h8_3069f::operation::MOV_W_R_R, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());
            ASSERT_EQ(0x12, cpu->er[5].r);

        }

        TEST(AND_W_IMM, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //79 60 00 80     
            //and.w   #0x80,r0
            mem[0x100] = 0x79;
            mem[0x101] = 0x60;
            mem[0x102] = 0x00;
            mem[0x103] = 0x80;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[0].r = 0xff0f;

            ASSERT_EQ(h8_3069f::operation::AND_W_IMM, cpu->detect_operation());
            ASSERT_EQ(0x104, cpu->cycle());
            ASSERT_EQ(0x0000, cpu->er[0].r);
            ASSERT_EQ(0b10000100, cpu->ccr.byte);
        }

        TEST(AND_W_IMM, 1)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //79 60 00 80     
            //and.w   #0x8000,r0
            mem[0x100] = 0x79;
            mem[0x101] = 0x60;
            mem[0x102] = 0x80;
            mem[0x103] = 0x00;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[0].r = 0x8000;

            ASSERT_EQ(h8_3069f::operation::AND_W_IMM, cpu->detect_operation());
            ASSERT_EQ(0x104, cpu->cycle());
            ASSERT_EQ(0b10001000, cpu->ccr.byte);
        }

        TEST(register, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));

            cpu->er[0].e = 0x1234;
            cpu->er[0].r = 0x5678;

            cpu->er[1].h = 0x12;
            cpu->er[1].l = 0x34;

            ASSERT_EQ(0x12345678, cpu->er[0].er);
            ASSERT_EQ(0x1234, cpu->er[1].r);
        }

        TEST(AND_B_IMM, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //ea 7f           
            //and.b   #0x7f,r2l
            mem[0x100] = 0xea;
            mem[0x101] = 0x7f;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[2].l = 0x7f;

            ASSERT_EQ(h8_3069f::operation::AND_B_IMM, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());
            ASSERT_EQ(0b10000000, cpu->ccr.byte);
            ASSERT_EQ(cpu->er[2].l, 0x7f);
        }

        TEST(ADDS_4, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //0b 97           
            //adds    #4,er7
            mem[0x100] = 0x0b;
            mem[0x101] = 0x97;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[7].er = 0x12;

            ASSERT_EQ(h8_3069f::operation::ADDS_4, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());
            ASSERT_EQ(0b10000000, cpu->ccr.byte);
            ASSERT_EQ(cpu->er[7].er, 0x12 + 4);

        }

        TEST(SUB_L_R_R, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //1a 80           
            //sub.l   er0,er0
            mem[0x100] = 0x1a;
            mem[0x101] = 0x80;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            ASSERT_EQ(h8_3069f::operation::SUB_L_R_R, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());
            ASSERT_EQ(0x00000000, cpu->er[0].er);
        }

        TEST(ADD_B_IMM_R, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //88 10           
            //add.b   #0x10,r0l
            mem[0x100] = 0x88;
            mem[0x101] = 0x10;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[0].l = 0x12;

            ASSERT_EQ(h8_3069f::operation::ADD_B_IMM_R, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());
            ASSERT_EQ(0x22, cpu->er[0].l);

        }

        TEST(ADD_L_IMM_R, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //7a 17 ff ff     
            //add.l   #0xfffffff4,er7
            mem[0x100] = 0x7a;
            mem[0x101] = 0x17;
            mem[0x102] = 0xff;
            mem[0x103] = 0xff;
            mem[0x104] = 0xff;
            mem[0x105] = 0xf4;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[7].er = 0x12345678;

            ASSERT_EQ(h8_3069f::operation::ADD_L_IMM_R, cpu->detect_operation());
            ASSERT_EQ(0x106, cpu->cycle());
            ASSERT_EQ((std::int32_t)0x12345678+ (std::int32_t)0xfffffff4, cpu->er[7].er);
        }

        TEST(SUBS_1, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //1b 03           
            //subs    #1,er3
            mem[0x100] = 0x1b;
            mem[0x101] = 0x03;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[3].er = 0x12345678;

            ASSERT_EQ(h8_3069f::operation::SUBS_1, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());
            ASSERT_EQ(0x12345678 - 1, cpu->er[3].er);

        }

        TEST(MOV_B_R_IND_WITH_DIS_24_R, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;
            //  

            //78 20 6a 2a     
            //00 00 03 ea
            //mov.b   @(0x3ea:32,er2),r2l
            mem[0x100] = 0x78;
            mem[0x101] = 0x20;
            mem[0x102] = 0x6a;
            mem[0x103] = 0x2a;
            mem[0x104] = 0x00;
            mem[0x105] = 0x00;
            mem[0x106] = 0x03;
            mem[0x107] = 0xea;

            mem[0x100 + 0x3ea] = 0x12;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[2].er = 0x100;

            ASSERT_EQ(h8_3069f::operation::MOV_B_R_IND_WITH_DIS_24_R, cpu->detect_operation());
            ASSERT_EQ(0x108, cpu->cycle());
            ASSERT_EQ(0x12, cpu->er[2].l);
        }

        TEST(SHLR_L, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //11 32           
            //shlr.l  er2
            mem[0x100] = 0x11;
            mem[0x101] = 0x32;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[2].er = 0x00000100;

            ASSERT_EQ(h8_3069f::operation::SHLR_L, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());
            ASSERT_EQ(0x00000080, cpu->er[2].er);

        }

        TEST(DEC_W_1, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //1b 50           
            //dec.w   #1,r0
            mem[0x100] = 0x1b;
            mem[0x101] = 0x50;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[0].r = 0x0100;

            ASSERT_EQ(h8_3069f::operation::DEC_W_1, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());
            ASSERT_EQ(0x00ff, cpu->er[0].r);
        }

        TEST(MOV_B_R_R_IND, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //68 ba           
            //mov.b   r2l,@er3
            mem[0x100] = 0x68;
            mem[0x101] = 0xba;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[2].l = 0x12;
            cpu->er[3].er = 0xffff00;

            ASSERT_EQ(h8_3069f::operation::MOV_B_R_R_IND, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());
 
            ASSERT_EQ(0x12, cpu->memory[0xffff00]);
        }

        TEST(MOV_B_R_IND_R, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //mov.b   @er4,r2l
            //68 4a           
            mem[0x100] = 0x68;
            mem[0x101] = 0x4a;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[4].er = 0x120;
            cpu->memory[0x120] = 0x12;

            ASSERT_EQ(h8_3069f::operation::MOV_B_R_IND_R, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());
            ASSERT_EQ(0x12, cpu->er[2].l);
        }

        TEST(ADDS_1, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //0b 00           
            //adds    #1,er0
            mem[0x100]  = 0x0b;
            mem[0x101]  = 0x00;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[0].er = 0x0fffffff;

            ASSERT_EQ(h8_3069f::operation::ADDS_1, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());
 
            ASSERT_EQ(0x10000000, cpu->er[0].er);
        }

        TEST(SUB_L_IMM_R, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //7a 32 00 ff     
            //fc 30
            //sub.l   #0xfffc20,er2
            mem[0x100] = 0x7a;
            mem[0x101] = 0x32;
            mem[0x102] = 0x00;
            mem[0x103] = 0xff;
            mem[0x104] = 0xfc;
            mem[0x105] = 0x20;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[2].er = 0x12345678;

            ASSERT_EQ(h8_3069f::operation::SUB_L_IMM_R, cpu->detect_operation());
            ASSERT_EQ(0x106, cpu->cycle());

            ASSERT_EQ(0x12345678 - 0xfffc20, cpu->er[2].er);
        }

        TEST(BLE_8, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //4f 0a           
            //ble .+0xa (0x104 + 0xa)
            mem[0x100] = 0x4f;
            mem[0x101] = 0x0a;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->ccr.zero = 0;
            cpu->ccr.negative = 0;
            cpu->ccr.over_flow = 1;

            ASSERT_EQ(h8_3069f::operation::BLE_8, cpu->detect_operation());
            ASSERT_EQ(0x102 + 0xa, cpu->cycle());
        }

        TEST(MOV_W_ABS_24_R, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //6b 20 00 ff     
            //fc 20
            //mov.w   @0xfffc20:32,r0
            mem[0x100] = 0x6b;
            mem[0x101] = 0x20;
            mem[0x102] = 0x00;
            mem[0x103] = 0xff;
            mem[0x104] = 0xfc;
            mem[0x105] = 0x20;


            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->memory[0xfffc20] = 0x12;
            cpu->memory[0xfffc21] = 0x34;

            ASSERT_EQ(h8_3069f::operation::MOV_W_ABS_24_R, cpu->detect_operation());
            ASSERT_EQ(0x106, cpu->cycle());

            ASSERT_EQ(0x1234, cpu->er[0].er);
        }

        TEST(MOV_W_R_ABS_24, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //6b a2 00 ff     mov.w   r2,@0xfffc20:32
            //fc 20
            mem[0x100] = 0x6b;
            mem[0x101] = 0xa2;
            mem[0x102] = 0x00;
            mem[0x103] = 0xff;
            mem[0x104] = 0xfc;
            mem[0x105] = 0x20;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[2].er = 0x1234;

            ASSERT_EQ(h8_3069f::operation::MOV_W_R_ABS_24, cpu->detect_operation());
            ASSERT_EQ(0x106, cpu->cycle());

            ASSERT_EQ(0x12, cpu->memory[0xfffc20]);
            ASSERT_EQ(0x34, cpu->memory[0xfffc21]);

        }

        TEST(EXTU_W, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //5fa:   17 50           
            //extu.w  r0
            mem[0x100] = 0x17;
            mem[0x101] = 0x50;
            
            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[0].r = 0x1234;
 
            ASSERT_EQ(h8_3069f::operation::EXTU_W, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());

            ASSERT_EQ(cpu->er[0].r, 0x0034);
        }

        TEST(CMP_B_R_R, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //1c ab           
            //cmp.b   r2l,r3l
            mem[0x100] = 0x1c;
            mem[0x101] = 0xab;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[2].l = 0x12;
            cpu->er[3].l = 0x12;

            ASSERT_EQ(h8_3069f::operation::CMP_B_R_R, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());

            //TODO
//             ASSERT_EQ(0x80000000, cpu->ccr.byte);
        }

        TEST(BLS_8, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //43 06           
            //bls .+6 (0x376)
            mem[0x100] = 0x43;
            mem[0x101] = 0x06;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->ccr.carry = 1;

            ASSERT_EQ(h8_3069f::operation::BLS_8, cpu->detect_operation());
            ASSERT_EQ(0x102 + 6, cpu->cycle());
        }

        TEST(BNE_16, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;
            //58 60 00 9c     
            //bne .+0x9c
            mem[0x100] = 0x58;
            mem[0x101] = 0x60;
            mem[0x102] = 0x00;
            mem[0x103] = 0x9c;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            ASSERT_EQ(h8_3069f::operation::BNE_16, cpu->detect_operation());
            ASSERT_EQ(0x104 + 0x9c, cpu->cycle());

        }

        TEST(MOV_L_ABS_24_R, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //01 00 6b 20     
            //00 ff fc 20 
            //mov.l   @0xfffc20:32,er0
            mem[0x100] = 0x01;
            mem[0x101] = 0x00;
            mem[0x102] = 0x6b;
            mem[0x103] = 0x20;
            mem[0x104] = 0x00;
            mem[0x105] = 0xff;
            mem[0x106] = 0xfc;
            mem[0x107] = 0x20;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->memory[0xfffc20] = 0x12;
            cpu->memory[0xfffc21] = 0x34;
            cpu->memory[0xfffc22] = 0x56;
            cpu->memory[0xfffc23] = 0x78;

            ASSERT_EQ(h8_3069f::operation::MOV_L_ABS_24_R, cpu->detect_operation());
            ASSERT_EQ(0x108, cpu->cycle());

            ASSERT_EQ(0x12345678, cpu->er[0].er);

        }

        TEST(BRA_16, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x02;
            mem[3] = 0xa0;

            //58 00 fe bc     
            //bra .-324 (0x160)
            mem[0x2a0] = 0x58;
            mem[0x2a1] = 0x00;
            mem[0x2a2] = 0xfe;
            mem[0x2a3] = 0xbc;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            ASSERT_EQ(h8_3069f::operation::BRA_16, cpu->detect_operation());
            ASSERT_EQ(0x160, cpu->cycle());
        }

        TEST(MOV_L_R_ABS_24, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //01 00 6b a0     
            //00 ff fc 40
            //mov.l   er0,@0xfffc40:32
            mem[0x100] = 0x01;
            mem[0x101] = 0x00;
            mem[0x102] = 0x6b;
            mem[0x103] = 0xa0;
            mem[0x104] = 0x00;
            mem[0x105] = 0xff;
            mem[0x106] = 0xfc;
            mem[0x107] = 0x40;
            
            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[0].er = 0x12345678;

            ASSERT_EQ(h8_3069f::operation::MOV_L_R_ABS_24, cpu->detect_operation());
            ASSERT_EQ(0x108, cpu->cycle());

            ASSERT_EQ(0x12, cpu->memory[0xfffc40]);
            ASSERT_EQ(0x34, cpu->memory[0xfffc41]);
            ASSERT_EQ(0x56, cpu->memory[0xfffc42]);
            ASSERT_EQ(0x78, cpu->memory[0xfffc43]);
        }

        TEST(MOV_L_R_R_IND_WITH_DIS_16, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //01 00 6f e0     
            //ff fc                                       
            //mov.l   er0,@(0xfffc:16,er6)
            mem[0x100] = 0x01;
            mem[0x101] = 0x00;
            mem[0x102] = 0x6f;
            mem[0x103] = 0xe0;
            mem[0x104] = 0xff;
            mem[0x105] = 0xfc;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[0].er = 0x12345678;
            cpu->er[6].er = 0x00ffff00;

            ASSERT_EQ(h8_3069f::operation::MOV_L_R_R_IND_WITH_DIS_16, cpu->detect_operation());
            ASSERT_EQ(0x106, cpu->cycle());

            auto addr = 0xffff00 + (std::int16_t)0xfffc;
            ASSERT_EQ(0x12, cpu->memory[addr]);
            ASSERT_EQ(0x34, cpu->memory[addr + 1]);
            ASSERT_EQ(0x56, cpu->memory[addr + 2]);
            ASSERT_EQ(0x78, cpu->memory[addr + 3]);
        }

        TEST(MOV_W_R_R_IND_WITH_DIS_16, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //6f e2 ff fa     
            //mov.w   r2,@(0xfffa:16,er6)
            mem[0x100] = 0x6f;
            mem[0x101] = 0xe2;
            mem[0x102] = 0xff;
            mem[0x103] = 0xfa;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[2].r = 0x1234;
            cpu->er[6].er = 0x00ffff00;

            ASSERT_EQ(h8_3069f::operation::MOV_W_R_R_IND_WITH_DIS_16, cpu->detect_operation());
            ASSERT_EQ(0x104, cpu->cycle());

            auto addr = 0x00ffff00 + (std::int16_t)0xfffa;
            ASSERT_EQ(cpu->memory[addr], 0x12);
            ASSERT_EQ(cpu->memory[addr + 1], 0x34);

        }

        TEST(MOV_W_R_IND_WITH_DIS_16_R, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //6f 63 ff fa     
            //mov.w   @(0xfffa:16,er6),r3
            mem[0x100] = 0x6f;
            mem[0x101] = 0x63;
            mem[0x102] = 0xff;
            mem[0x103] = 0xfa;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            auto addr = 0x00ffff00 + (std::int16_t)0xfffa;

            cpu->er[6].er = 0x00ffff00;
            cpu->memory[addr] = 0x12;
            cpu->memory[addr + 1] = 0x34;

            ASSERT_EQ(h8_3069f::operation::MOV_W_R_IND_WITH_DIS_16_R, cpu->detect_operation());
            ASSERT_EQ(0x104, cpu->cycle());

            ASSERT_EQ(cpu->er[3].r, 0x1234);
        }

        TEST(BGE_8, 0)
        {
           std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //4c 0a           
            //bge .+10 
            mem[0x100] = 0x4c;
            mem[0x101] = 0x0a;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->ccr.negative = 1;
            cpu->ccr.over_flow = 1;

            ASSERT_EQ(h8_3069f::operation::BGE_8, cpu->detect_operation());
            ASSERT_EQ(0x102 + 10, cpu->cycle());

        }

        TEST(BEQ_16, 0){
            std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            // 58 70 00 be     
            // beq .+190 (0x730)
            mem[0x100] = 0x58;
            mem[0x101] = 0x70;
            mem[0x102] = 0x00;
            mem[0x103] = 0xbe;
            
            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->ccr.zero = 1;

            ASSERT_EQ(h8_3069f::operation::BEQ_16, cpu->detect_operation());
            ASSERT_EQ(0x104 + 190, cpu->cycle());
        }

        TEST(CMP_L_IMM, 0){
            std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //7a 24 00 1e     
            //84 7f
            //cmp.l   #0x1e847f,er4
            mem[0x100] = 0x7a;
            mem[0x101] = 0x24;
            mem[0x102] = 0x00;
            mem[0x103] = 0x1e;
            mem[0x104] = 0x84;
            mem[0x105] = 0x7f;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[4].er = 0x1e847f;

            ASSERT_EQ(h8_3069f::operation::CMP_L_IMM, cpu->detect_operation());
            ASSERT_EQ(0x106, cpu->cycle());

            ASSERT_EQ(0b10000100, cpu->ccr.byte);
        }

        TEST(MOV_L_R_IND_WITH_DIS_16_R, 0){
            std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //mov.l   @(0xfffc:16,er6),er4
            //01 00 6f 64     
            //ff fc
            mem[0x100] = 0x01;
            mem[0x101] = 0x00;
            mem[0x102] = 0x6f;
            mem[0x103] = 0x64;
            mem[0x104] = 0xff;
            mem[0x105] = 0xfc;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[6].er = 0x00ffff00;
            auto addr = 0x00ffff00 + (std::int16_t)0xfffc;
            cpu->memory[addr] = 0x12;
            cpu->memory[addr + 1] = 0x34;
            cpu->memory[addr + 2] = 0x56;
            cpu->memory[addr + 3] = 0x78;

            ASSERT_EQ(h8_3069f::operation::MOV_L_R_IND_WITH_DIS_16_R, cpu->detect_operation());
            ASSERT_EQ(0x106, cpu->cycle());

            ASSERT_EQ(cpu->er[4].er, 0x12345678);
        }

        TEST(XOR_B_R_R, 0){
            std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //15 8a           
            //xor.b   r0l,r2l
            mem[0x100] = 0x15;
            mem[0x101] = 0x8a;
            
            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[0].l = 0x12;
            cpu->er[2].l = 0x34;
            
            ASSERT_EQ(h8_3069f::operation::XOR_B_R_R, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());

            ASSERT_EQ(0x12 ^ 0x34, cpu->er[2].l);
        }

        TEST(BGT_8, 0){
            std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //4e 10           
            //bgt .+16 (0x1cc)
            mem[0x100] = 0x4e;
            mem[0x101] = 0x10;
            
            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->ccr.zero = 0;
            cpu->ccr.negative = 1;
            cpu->ccr.over_flow = 1;

            ASSERT_EQ(h8_3069f::operation::BGT_8, cpu->detect_operation());
            ASSERT_EQ(0x102 + 16, cpu->cycle());
        }

        TEST(MOV_L_R_R_IND, 0){
            std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //01 00 69 23     mov.l   @er2,er3
            mem[0x100] = 0x01;
            mem[0x101] = 0x00;
            mem[0x102] = 0x69;
            mem[0x103] = 0x23;
            
            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);
            
            cpu->er[2].er = 0x00000200;
            cpu->memory[0x200] = 0x12;
            cpu->memory[0x201] = 0x34;
            cpu->memory[0x202] = 0x56;
            cpu->memory[0x203] = 0x78;

            ASSERT_EQ(h8_3069f::operation::MOV_L_R_R_IND, cpu->detect_operation());
            ASSERT_EQ(0x104, cpu->cycle());

            ASSERT_EQ(0x12345678, cpu->er[3].er);
        }

        TEST(ADD_B_R_R, 0){
            std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //08 8b           
            //add.b   r0l,r3l
            mem[0x100] = 0x08;
            mem[0x101] = 0x8b;
            
            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[0].l = 0x12;
            cpu->er[3].l = 0x34;

            ASSERT_EQ(h8_3069f::operation::ADD_B_R_R, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());

            ASSERT_EQ(0x12 + 0x34, cpu->er[3].l);
        }

        TEST(MOV_L_R_IND_R, 0){
            std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //mov.l   er3,@er2
            //01 00 69 a3     
            mem[0x100] = 0x01;
            mem[0x101] = 0x00;
            mem[0x102] = 0x69;
            mem[0x103] = 0xa3;
            
            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[3].er = 0x12345678;
            cpu->er[2].er = 0x200;

            ASSERT_EQ(h8_3069f::operation::MOV_L_R_IND_R, cpu->detect_operation());
            ASSERT_EQ(0x104, cpu->cycle());

            ASSERT_EQ(cpu->memory[0x200], 0x12);
            ASSERT_EQ(cpu->memory[0x201], 0x34);
            ASSERT_EQ(cpu->memory[0x202], 0x56);
            ASSERT_EQ(cpu->memory[0x203], 0x78);
        }

        TEST(CMP_L_R_R, 0){
            std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //1f c5           
            //cmp.l   er4,er5
            mem[0x100] = 0x1f;
            mem[0x101] = 0xc5;
            
            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[5].er = 0x12345678;
            cpu->er[4].er = 0x87654321;

            ASSERT_EQ(h8_3069f::operation::CMP_L_R_R, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());

//             std::cout << std::hex << (std::uint16_t)cpu->ccr.byte << std::endl;
            ASSERT_EQ(0b10001011, cpu->ccr.byte);
        }


        TEST(XOR_B_IMM_R, 0){
            std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //da 07           
            //xor.b   #0x7,r2l
            mem[0x100] = 0xda;
            mem[0x101] = 0x07;
            
            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[2].l = 0x8;

            ASSERT_EQ(h8_3069f::operation::XOR_B_IMM_R, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());
            ASSERT_EQ(0b10000000, (std::uint8_t)cpu->ccr.byte);
        }

        TEST(DEC_W_2, 0){
            std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //dec.w   #2,r2
            //1b d2           
            mem[0x100] = 0x1b;
            mem[0x101] = 0xd2;
            
            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[2].r = 0x1112;

            ASSERT_EQ(h8_3069f::operation::DEC_W_2, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());

            ASSERT_EQ(0x1110, cpu->er[2].r);
        }

        TEST(DEC_L_1, 0){
            std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //1b 72           dec.l   #1,er2
            mem[0x100] = 0x1b;
            mem[0x101] = 0x72;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[2].er = 0x12345678;

            ASSERT_EQ(h8_3069f::operation::DEC_L_1, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());

            ASSERT_EQ(0x12345678 - 1, cpu->er[2].er);
        }

        TEST(ADD_W_IMM_R, 0){
            std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //79 12 ff d2     
            //add.w   #0xffd2,r2
            mem[0x100] = 0x79;
            mem[0x101] = 0x12;
            mem[0x102] = 0xff;
            mem[0x103] = 0xd2;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[2].r = 0x1234;

            ASSERT_EQ(h8_3069f::operation::ADD_W_IMM_R, cpu->detect_operation());
            ASSERT_EQ(0x104, cpu->cycle());

            ASSERT_EQ(0x1234 + (std::int16_t)0xffd2, cpu->er[2].r);
        }

        TEST(SHAR_W, 0){
            std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //11 92        
            //shar.w  r2
            mem[0x100] = 0x11;
            mem[0x101] = 0x92;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[2].r = 0x1234;

            ASSERT_EQ(h8_3069f::operation::SHAR_W, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());

            ASSERT_EQ(0x1234 >> 1, cpu->er[2].r);
        }

        TEST(CMP_W_R_R, 0){
            std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //1d 23           
            //cmp.w   r2,r3
            mem[0x100] = 0x1d;
            mem[0x101] = 0x23;
            
            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            ASSERT_EQ(h8_3069f::operation::CMP_W_R_R, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());

            ASSERT_EQ(0b10000100, cpu->ccr.byte);
        }

        TEST(BGE_16, 0){
            std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //58 c0 00 e2     bge .+226 (0x8ce)
            mem[0x100] = 0x58;
            mem[0x101] = 0xc0;
            mem[0x102] = 0x00;
            mem[0x103] = 0xe2;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->ccr.negative = 0;
            cpu->ccr.over_flow = 0;

            ASSERT_EQ(h8_3069f::operation::BGE_16, cpu->detect_operation());
            ASSERT_EQ(0x104 + 226, cpu->cycle());
        }

        TEST(MULXS_W_R_R, 0){
            std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //01 c0 52 32     
            //mulxs.w r3,er2
            mem[0x100] = 0x01;
            mem[0x101] = 0xc0;
            mem[0x102] = 0x52;
            mem[0x103] = 0x32;
            
            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[2].er = 0x12345678;
            cpu->er[3].r = 0x1234;

            ASSERT_EQ(h8_3069f::operation::MULXS_W_R_R, cpu->detect_operation());
            ASSERT_EQ(0x104, cpu->cycle());

            ASSERT_EQ((std::int16_t)0x5678 * (std::int16_t)0x1234, cpu->er[2].er);
        }

        TEST(JSR_R, 0){
            std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //5d 40           
            //jsr @er4
            mem[0x100] = 0x5d;
            mem[0x101] = 0x40;
            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[4].er = 0x400;
            cpu->sp = 0xffff00;

            ASSERT_EQ(h8_3069f::operation::JSR_R, cpu->detect_operation());
            ASSERT_EQ(0x400, cpu->cycle());
        }

        TEST(ORC, 0){
            std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //04 c0           
            //orc #0xc0
            mem[0x100] = 0x04;
            mem[0x101] = 0xc0;
            
            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            ASSERT_EQ(h8_3069f::operation::ORC, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());
        }

        TEST(MOV_L_R_R_IND_WITH_DIS_24, 0){
            std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //mov.l   er1,@(0xffbf20:32,er0)
            //01 00 78 80     
            //6b a1 00 ff
            //bf 20
            mem[0x100] = 0x01;
            mem[0x101] = 0x00;
            mem[0x102] = 0x78;
            mem[0x103] = 0x80;
            mem[0x104] = 0x6b;
            mem[0x105] = 0xa1;
            mem[0x106] = 0x00;
            mem[0x107] = 0xff;
            mem[0x108] = 0xbf;
            mem[0x109] = 0x20;
            
            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[1].er = 0x12345678; // src
            cpu->er[0].er = 0x00000200; // dest

            ASSERT_EQ(h8_3069f::operation::MOV_L_R_R_IND_WITH_DIS_24, cpu->detect_operation());
            ASSERT_EQ(0x10a, cpu->cycle());

            auto addr = 0x200 + (std::int32_t)0xffbf20;
            ASSERT_EQ(0x12, cpu->memory[addr++]);
            ASSERT_EQ(0x34, cpu->memory[addr++]);
            ASSERT_EQ(0x56, cpu->memory[addr++]);
            ASSERT_EQ(0x78, cpu->memory[addr]);
        }

        TEST(CMP_W_IMM, 0){
            std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //cmp.w   #0x2,r4
            //79 24 00 02     
            mem[0x100] = 0x79;
            mem[0x101] = 0x24;
            mem[0x102] = 0x00;
            mem[0x103] = 0x02;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[4].er = 0x12345678;

            ASSERT_EQ(h8_3069f::operation::CMP_W_IMM, cpu->detect_operation());
            ASSERT_EQ(0x104, cpu->cycle());

            ASSERT_EQ(0b10000000, cpu->ccr.byte);
        }

        TEST(OR_B_IMM_R, 0){
            std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //ca 40           
            //or.b    #0x40,r2l
            mem[0x100] = 0xca;
            mem[0x101] = 0x40;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[2].l = 0x40;

            ASSERT_EQ(h8_3069f::operation::OR_B_IMM_R, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());

            ASSERT_EQ(0b10000000, cpu->ccr.byte);
        }

        TEST(ANDC, 0){
            std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //06 3f           
            //andc    #0x3f
            mem[0x100] = 0x06;
            mem[0x101] = 0x3f;
            
            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            ASSERT_EQ(h8_3069f::operation::ANDC, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());
        }

        TEST(SLEEP, 0){
            std::array<std::uint8_t, 
                (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //01 80          
            //sleep
            mem[0x100] = 0x01;
            mem[0x101] = 0x80;

            auto cpu = std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            //TODO fix below
//             std::thread th(
//                 [&]
//                 {
//                     std::chrono::seconds d(1);
//                     std::this_thread::sleep_for(d);
//                     while (*(cpu->is_sleep)) {}
//                     *(cpu->is_sleep) = false;
//                     std::weak_ptr<h8_3069f::cpu> wc = cpu;
//                     if(auto ptr = wc.lock()){
//                         ptr->interrupt(h8_3069f::interrupts::rxi1);
//                         ptr->c_variable_ptr->notify_all();
//                     }
//                 });
//             th.detach();

            ASSERT_EQ(h8_3069f::operation::SLEEP, cpu->detect_operation());
            //TODO with above code
//             ASSERT_EQ(0x102, cpu->cycle());
        }

        TEST(MOV_B_R_R_IND_WITH_DIS_24, 0){
            std::array<std::uint8_t,
                (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            // mov.b   r0l,@(0xffc5ec:32,er2)
            //78 20 6a a8     
            //00 ff c5 ec
            mem[0x100] = 0x78;
            mem[0x101] = 0x20;
            mem[0x102] = 0x6a;
            mem[0x103] = 0xa8;
            mem[0x104] = 0x00;
            mem[0x105] = 0xff;
            mem[0x106] = 0xc5;
            mem[0x107] = 0xec;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[0].l = 0x12;
            cpu->er[2].er = 0x00000000;
            cpu->ccr.interrupt_mask = 1;

            ASSERT_EQ(h8_3069f::operation::MOV_B_R_R_IND_WITH_DIS_24, cpu->detect_operation());
            ASSERT_EQ(0x108, cpu->cycle());

            auto addr = (std::int32_t)0xffc5ec;
            ASSERT_EQ(0x12, cpu->memory[addr]);
        }

        TEST(RTE, 0){
            std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            // 56 70           
            // rte
            mem[0x100] = 0x56;
            mem[0x101] = 0x70;
            
            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->sp = 0x00ffff00 - 4;
            cpu->memory[cpu->sp] = 0b10001011;
            cpu->memory[cpu->sp + 1] = 0x34;
            cpu->memory[cpu->sp + 2] = 0x56;
            cpu->memory[cpu->sp + 3] = 0x78;

            ASSERT_EQ(h8_3069f::operation::RTE, cpu->detect_operation());
            ASSERT_EQ(0x00345678, cpu->cycle());

            ASSERT_EQ(0x00ffff00, cpu->sp);
        }

        TEST(TRAPA, 0)
        {
            std::array<std::uint8_t,
                (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            // 57 00           
            // trapa   #0x0
            mem[0x100] = 0x57;
            mem[0x101] = 0x00;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->sp = 0x00ffff00;
            cpu->ccr.byte = 0b10000000;
            cpu->memory[0x20] = 0x00;
            cpu->memory[0x21] = 0x12;
            cpu->memory[0x22] = 0x34;
            cpu->memory[0x23] = 0x56;

            ASSERT_EQ(h8_3069f::operation::TRAPA, cpu->detect_operation());
            ASSERT_EQ(0x123456, cpu->cycle());

            ASSERT_EQ(0xffff00 - 4, cpu->sp);
            ASSERT_EQ(0b10000000, cpu->memory[cpu->sp]);    //ccr
            ASSERT_EQ(0x00, cpu->memory[cpu->sp + 1]);          //pc 
            ASSERT_EQ(0x01, cpu->memory[cpu->sp + 2]);          //pc
            ASSERT_EQ(0x02, cpu->memory[cpu->sp + 3]);              //pc
        }    

        TEST(er7, 0)
        {
            std::array<std::uint8_t,
                (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            auto cpu =std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[7].er = 0x12345678;
            ASSERT_EQ(cpu->sp, 0x12345678);

            cpu->sp = 0x87654321;
            ASSERT_EQ(cpu->er[7].er, 0x87654321);
        }

        TEST(OR_W_IMM_R, 0)
        {
            std::array<std::uint8_t, 
                (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //79 4a c0 00     
            //or.w    #0xc000,e2
            mem[0x100] = 0x79;
            mem[0x101] = 0x4a;
            mem[0x102] = 0xc0;
            mem[0x103] = 0x00;

            auto cpu = std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[2].e = 0xffff;

            ASSERT_EQ(h8_3069f::operation::OR_W_IMM_R,
                    cpu->detect_operation());
            ASSERT_EQ(0x104, cpu->cycle());
            ASSERT_EQ(0xffff, cpu->er[2].e);
        }

        TEST(BTST_IMM_R, 0)
        {
            std::array<std::uint8_t, 
                (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //73 0a           
            //btst    #0x0,r2l
            mem[0x100] = 0x73;
            mem[0x101] = 0x0a;

            auto cpu = std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[2].l = 0b00000000;
            
            ASSERT_EQ(h8_3069f::operation::BTST_IMM_R, 
                    cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());
            ASSERT_EQ(0b10000100, cpu->ccr.byte);
        }

        TEST(BHI_16, 0)
        {
            std::array<std::uint8_t, 
                (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //58 20 00 aa     
            //bhi .+170 (0xffc990)
            mem[0x100] = 0x58;
            mem[0x101] = 0x20;
            mem[0x102] = 0x00;
            mem[0x103] = 0xaa;

            auto cpu = std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->ccr.carry = 0;
            cpu->ccr.zero = 0;

            ASSERT_EQ(h8_3069f::operation::BHI_16, cpu->detect_operation());
            ASSERT_EQ(0x104 + 170, cpu->cycle());
        }

         TEST(EXTU_L, 0)
        {
            std::array<std::uint8_t, 
                (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //17 72           
            //extu.l  er2
            mem[0x100] = 0x17;
            mem[0x101] = 0x72;

            auto cpu = std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[2].er = 0xffff1234;

            ASSERT_EQ(h8_3069f::operation::EXTU_L, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());
            ASSERT_EQ(0x00001234, cpu->er[2].er);
        }       

        TEST(JMP_R, 0)
        {
            std::array<std::uint8_t, 
                (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;
            
            //59 20           
            //jmp @er2
            mem[0x100] = 0x59;
            mem[0x101] = 0x20;

            auto cpu = std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[2].er = 0x12345678;

            ASSERT_EQ(h8_3069f::operation::JMP_R, cpu->detect_operation());
            ASSERT_EQ(0x12345678, cpu->cycle());
        }

        TEST(MOV_W_R_IND_R, 0)
        {
            std::array<std::uint8_t, 
                (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //69 53           
            //mov.w   @er5,r3
            mem[0x100] = 0x69;
            mem[0x101] = 0x53;

            auto cpu = std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[5].er = 0x00ffff00;
            cpu->memory[0xffff00] = 0x12;
            cpu->memory[0xffff01] = 0x34;

            ASSERT_EQ(h8_3069f::operation::MOV_W_R_IND_R, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());

            ASSERT_EQ(0x1234, cpu->er[3].r);
        }

        TEST(BLT_8, 0)
        {
            std::array<std::uint8_t, 
                (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //4d 04           
            //blt .+4 (0xffc988)
            mem[0x100] = 0x4d;
            mem[0x101] = 0x04;

            auto cpu = std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->ccr.negative = 0;
            cpu->ccr.over_flow = 1;

            ASSERT_EQ(h8_3069f::operation::BLT_8, cpu->detect_operation());
            ASSERT_EQ(0x102 + 4, cpu->cycle());
        }

        TEST(MOV_W_R_R_IND, 0)
        {
            std::array<std::uint8_t, 
                (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //69 d2           
            //mov.w   r2,@er5
            mem[0x100] = 0x69;
            mem[0x101] = 0xd2;

            auto cpu = std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[2].r = 0x1234;
            cpu->er[5].er = 0x00ffff00;

            ASSERT_EQ(h8_3069f::operation::MOV_W_R_R_IND, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());
            ASSERT_EQ(cpu->memory[0x00ffff00], 0x12);
            ASSERT_EQ(cpu->memory[0x00ffff01], 0x34);
        }

        TEST(BLE_16, 0)
        {
            std::array<std::uint8_t, 
                (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //58 f0 00 48     
            //ble .-184 (0xffce2a)
            mem[0x100] = 0x58;
            mem[0x101] = 0xf0;
            mem[0x102] = 0x00;
            mem[0x103] = 0x48;

            auto cpu = std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->ccr.zero = 1;
            cpu->ccr.negative = 0;
            cpu->ccr.over_flow = 1;

            ASSERT_EQ(h8_3069f::operation::BLE_16, cpu->detect_operation());
            ASSERT_EQ(0x104 + 0x48, cpu->cycle());
        }  
            
        TEST(BHI_8, 0)
        {
            std::array<std::uint8_t, 
                (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //42 1e           
            //bhi .+30 (0xffcd9e)
            mem[0x100] = 0x42;
            mem[0x101] = 0x1e;

            auto cpu = std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->ccr.carry = 0;
            cpu->ccr.zero = 0;

            ASSERT_EQ(h8_3069f::operation::BHI_8, cpu->detect_operation());
            ASSERT_EQ(0x102 + 30, cpu->cycle());
        }

        TEST(SUBS_2, 0)
        {
            std::array<std::uint8_t, 
                (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};
            mem[0] = 0x00;
            mem[1] = 0x00;
            mem[2] = 0x01;
            mem[3] = 0x00;

            //1b 81           
            //subs    #2,er1
            mem[0x100] = 0x1b;
            mem[0x101] = 0x81;

            auto cpu = std::make_shared<h8_3069f::cpu>(std::move(mem));
            cpu->interrupt(h8_3069f::interrupts::reset);

            cpu->er[1].er = 0x12345678;

            ASSERT_EQ(h8_3069f::operation::SUBS_2, cpu->detect_operation());
            ASSERT_EQ(0x102, cpu->cycle());
            ASSERT_EQ(0x12345678 - 2, cpu->er[1].er);
        }

    }  // namespace cpu
}  // namespace narcissus

// TODO added code for condition code register
