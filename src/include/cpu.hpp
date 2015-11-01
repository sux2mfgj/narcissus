#pragma once

#include <cstdint>
#include <array>

#include <gtest/gtest.h>

namespace narcissus {
    namespace cpu {

        enum operation {
            INVALID = 0,
            ADD_B_IMM,
            ADD_B_R_R,
            ADD_W_IMM,
//            ADD_W_R_R,
            //             ADD_L_IMM,
            //             ADD_L_R_R,
        };

        enum register_size { BYTE, WORD, LONG };

        const std::uint32_t ROM_SIZE = 0x00080000;

        union register_t {
            register_t() : er32(0) {}

            void write(uint8_t destination, uint32_t value, register_size size)
            {
                switch (size) {
                    case register_size::BYTE:
                        if((destination & 0x8) != 0x8) {
                            h = uint8_t(value);
                        }
                        else {
                            l = uint8_t(value);
                        }
                        break;
                    case register_size::WORD:
                        if((destination & 0x8) != 0x8){
                            r = uint16_t(value);
                        }
                        else {
                            e = uint16_t(value);
                        }
                        break;
                    case register_size::LONG:
                        //TODO
                        break;
                }
            }

            uint32_t read(uint8_t source, register_size size)
            {
                switch (size) {
                    case register_size::BYTE:
                        if((source & 0x8) != 0x8){
                            return h;
                        }
                        else {
                            return l;
                        }
                    case register_size::WORD:
                        //TODO
                        return -1;
                    case register_size::LONG:
                        //TODO
                        return -1;
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
                bool register_write_immediate(uint8_t destination,
                        uint32_t immediate,
                        register_size size);
                bool register_write_register(uint8_t destination,
                        uint8_t source,
                        register_size size);

                FRIEND_TEST(cpu, ADD_B_IMM);
                FRIEND_TEST(cpu, ADD_B_R_R);
                FRIEND_TEST(cpu, ADDR_W_IMM);

        };

        //         std::uint8_t std::uint8_t::operator [](std::uint32_t) {
        //             return this[std::uint32_t];
        //         };
    }  // namespace cpu
}  // namespace narcissus
