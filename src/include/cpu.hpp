#pragma once

#include <cstdint>
#include <array>

#include <gtest/gtest.h>

namespace narcissus {
    namespace cpu {

        enum operation {
            INVALID = 0,
            ADD_B_IMM,
            //             ADD_B_R_R,
            //             ADD_W_IMM,
            //             ADD_W_R_R,
            //             ADD_L_IMM,
            //             ADD_L_R_R,
        };

        enum register_size {
            BYTE,
            WORD,
            LONG
        };

        const std::uint32_t ROM_SIZE = 0x00080000;

        union register_t {
            register_t() : er32(0) {}

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
                h8_300(std::array<uint8_t, ROM_SIZE>&& mem);

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
                bool register_write(int destination, uint32_t immediate, register_size size);

            FRIEND_TEST(cpu, ADD_B_IMM);

        };

        //         std::uint8_t std::uint8_t::operator [](std::uint32_t) {
        //             return this[std::uint32_t];
        //         };
    }  // namespace cpu
}  // namespace narcissus
