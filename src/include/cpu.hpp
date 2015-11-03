#pragma once

#include <cstdint>
#include <array>

#include <gtest/gtest.h>

#include <mcu.hpp>

namespace narcissus {
    namespace cpu {

        enum operation {
            INVALID = 0,
//             ADD_B_IMM,              // immediate
//             ADD_B_R_R,              // register to register
//             ADD_W_IMM,
//             ADD_W_R_R,
//             ADD_L_IMM,
//             ADD_L_R_R,

            SUB_B_R_R,                      // substruct rd from rs
            SUB_W_R_R,

            MOV_B_IMM,
//             MOV_B_R_R,  
            MOV_B_R_IND,                // register indirect
            MOV_B_R_IND_WITH_DIS_16,    // register indirect with displacement(16 bit)
//             MOV_B_R_IND_WITH_DIS_24,    // register indirect with displacement(24 bit)
//             MOV_B_R_IND_POST_INC,       // rd increment after register indirect acdess
//             MOV_B_ASB_8,                // absolute addressing 8
//             MOV_B_ASB_16,               // absolute addressing 16
//             MOV_B_ASB_24,               // absolute addressing 24

            MOV_W_IMM,


            MOV_L_IMM, 
            MOV_L_R_R,
            MOV_L_R_IND,

            MOV_L_R_IND_WITH_DIS_24,
            MOV_L_R_IND_POST_INC,

            SHLL_L,                     // shift logical left

            JSR_ABS,                    // jump to subroutine use absolute address
            EXTS_L,                     // extend as signed
            RTS,
        };

        enum register_size { BYTE, WORD, LONG };

        union register_t {
            register_t() : er32(0) {}

            void write(std::uint8_t destination, std::uint32_t value, register_size size)
            {
                switch (size) {
                    case register_size::BYTE:
                        if((destination & 0x8) != 0x8) {
                            h = std::uint8_t(value);
                        }
                        else {
                            l = std::uint8_t(value);
                        }
                        break;
                    case register_size::WORD:
                        if((destination & 0x8) != 0x8){
                            r = std::uint16_t(value);
                        }
                        else {
                            e = std::uint16_t(value);
                        }
                        break;
                    case register_size::LONG:
//                         std::cout << "val" << (destination & 0x7)<< std::endl;
                        er32 = value;
                        break;
                }
            }

            std::uint32_t read(std::uint8_t source, register_size size)
            {
                //XXX
//                 std::cout << "source" << er32 << std::endl;
                switch (size) {
                    case register_size::BYTE:
                        if((source & 0x8) != 0x8){
                            return h;
                        }
                        else {
                            return l;
                        }
                    case register_size::WORD:
                        if((source & 0x8) != 0x8) {
                            return r;
                        }
                        else {
                            return e;
                        }
                    case register_size::LONG:
                        return er32;
                }
            }

            std::uint32_t er32;

            struct {
                std::uint16_t e;
                union {
                    std::uint16_t r;
                    struct {
                        std::uint8_t h;
                        std::uint8_t l;
                    };
                };
            };
        };

        union conditional_code_register {
            conditional_code_register() : byte(0) {}

            std::uint8_t byte;
            struct {
                int interrupt_mask : 1;
                int user_interrupt : 1;
                int half_carry : 1;
                int user : 1;
                int negative : 1;
                int zero : 1;
                int over_flow : 1;
                int carry : 1;
            };
        };

        class h8_300 {
            public:
                h8_300(std::array<std::uint8_t, ROM_SIZE>&& mem);

            private:
                // registers
                register_t er[7];
                union {
                    register_t er7;
                    std::uint32_t sp;
                };

                conditional_code_register ccr;
                std::uint32_t pc;

                mcu memory;

                // built-in memory
//                 std::array<std::uint8_t, ROM_SIZE> rom;
//                  std::uint8_t ram[RAM_SIZE];
//                 std::array<std::uint8_t, RAM_END_ADDR - RAM_BASE_ADDR> ram;

            public:
                void reset_exception();
                bool cycle();
                operation detect_operation();
                bool register_write_immediate(std::uint8_t destination,
                        std::uint32_t immediate,
                        register_size size);
                bool register_write_register(std::uint8_t destination,
                        std::uint8_t source,
                        register_size size);

            // use macro for test
            public:
//                 FRIEND_TEST(cpu, ADD_B_IMM);
//                 FRIEND_TEST(cpu, ADD_B_R_R);
//                 FRIEND_TEST(cpu, ADD_W_IMM);
//                 FRIEND_TEST(cpu, ADD_W_R_R);
//                 FRIEND_TEST(cpu, ADD_L_IMM);
//                 FRIEND_TEST(cpu, ADD_L_R_R);

                FRIEND_TEST(cpu, MOV_B_IMM);
                FRIEND_TEST(cpu, MOV_B_R_IND);
                FRIEND_TEST(cpu, MOV_W_IMM);
                FRIEND_TEST(cpu, MOV_L_IMM);
                FRIEND_TEST(cpu, JSR_ABS);
                FRIEND_TEST(cpu, MOV_L_R_IND);
                FRIEND_TEST(cpu, MOV_L_R_R);
                FRIEND_TEST(cpu, EXTS_L);
                FRIEND_TEST(cpu, SHLL_L);
                FRIEND_TEST(cpu, MOV_L_R_IND_WITH_DIS_24);
                FRIEND_TEST(cpu, SUB_B_R_R);
                FRIEND_TEST(cpu, MOV_B_R_IND_WITH_DIS_16);
                FRIEND_TEST(cpu, SUB_W_R_R);
                FRIEND_TEST(cpu, RTS);
                FRIEND_TEST(cpu, MOV_L_R_IND_POST_INC);
        };

        //         std::uint8_t std::uint8_t::operator [](std::uint32_t) {
        //             return this[std::uint32_t];
        //         };
    }  // namespace cpu
}  // namespace narcissus
