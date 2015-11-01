#pragma once

#include <cstdint>
#include <array>

#include <gtest/gtest.h>

namespace narcissus {
    namespace cpu {

        enum operation {
            INVALID = 0,
            ADD_B_IMM,              // immediate
            ADD_B_R_R,              // register to register
            ADD_W_IMM,
            ADD_W_R_R,
            ADD_L_IMM,
            ADD_L_R_R,

            MOV_B_IMM,
            MOV_B_R_R,  
            MOV_B_R_IND,                // register indirect
            MOV_B_R_IND_WITH_DIS_16,    // register indirect with displacement(16 bit)
            MOV_B_R_IND_WITH_DIS_24,    // register indirect with displacement(24 bit)
            MOV_B_R_IND_POST_INC,       // rd increment after register indirect acdess
            MOV_B_ASB_8,                // absolute addressing 8
            MOV_B_ASB_16,               // absolute addressing 16
            MOV_B_ASB_24,               // absolute addressing 24
        };

        enum register_size { BYTE, WORD, LONG };

        const std::uint32_t ROM_SIZE = 0x00080000;

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
                        er32 = std::uint32_t(value);
                        break;
                }
            }

            std::uint32_t read(std::uint8_t source, register_size size)
            {
                //XXX
                std::cout << "source" <<er32 <<std::endl;
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

#define PC er[7].er32

        class h8_300 {
            public:
                h8_300(std::array<std::uint8_t, ROM_SIZE>&& mem);

            private:
                // registers
                register_t er[8];
                conditional_code_register ccr;

                // built-in memory
                std::array<std::uint8_t, ROM_SIZE> rom;
                // std::uint8_t ram[RAM_SIZE];

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

                FRIEND_TEST(cpu, ADD_B_IMM);
                FRIEND_TEST(cpu, ADD_B_R_R);
                FRIEND_TEST(cpu, ADD_W_IMM);
                FRIEND_TEST(cpu, ADD_W_R_R);
                FRIEND_TEST(cpu, ADD_L_IMM);
                FRIEND_TEST(cpu, ADD_L_R_R);
                FRIEND_TEST(cpu, MOV_B_IMM);
                FRIEND_TEST(cpu, MOV_B_R_IND);
        };

        //         std::uint8_t std::uint8_t::operator [](std::uint32_t) {
        //             return this[std::uint32_t];
        //         };
    }  // namespace cpu
}  // namespace narcissus
