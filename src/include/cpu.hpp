#pragma once

#include <cstdint>
#include <array>

namespace narcissus {
    namespace cpu {

        enum operation {
            INVALID = 0,
            ADD_B_IMM,
            ADD_B_R_R,
            ADD_W_IMM,
            ADD_W_R_R,
            ADD_L_IMM,
            ADD_L_R_R,
        };

        const std::uint32_t ROM_SIZE = 0x00080000;

        union register_t{

            register_t() : er32(0){}

            std::uint32_t er32;

            struct er{
                std::uint16_t e;
                struct r {
                    std::uint8_t h;
                    std::uint8_t l;
                };
            };
        };

        union er7_register {

            er7_register() : er7(){}
            
            register_t er7;
            struct pc{
                int other : 8;
                int value : 24;
            };
        };

        struct conditional_code_register {

            conditional_code_register() 
                : interrupt_mask(0), user_interrupt(0), half_carry(0),
                user(0), negative(0), zero(0), over_flow(0), carry(0) {}

            void reset()
            {
                interrupt_mask = 1;
                user_interrupt = 0;
                half_carry = 0;
                user =0;
                negative = 0;
                zero = 0;
                over_flow = 0;
                carry = 0;
            }

            int interrupt_mask  : 1;
            int user_interrupt  : 1;
            int half_carry      : 1;
            int user            : 1;
            int negative        : 1;
            int zero            : 1;
            int over_flow       : 1;
            int carry           : 1;
        };

        class h8_300{
            public:
                h8_300(std::array<uint8_t, ROM_SIZE>&& mem); 

            private:
                //registers
                register_t er[7];
                er7_register er7;
                conditional_code_register ccr;

                //built-in memory
                std::array<std::uint8_t, ROM_SIZE> rom;
                //std::uint8_t ram[RAM_SIZE];
            
            public:
                void reset_exception();
                void cycle();
                operation detect_operation();
        };

//         std::uint8_t std::uint8_t::operator [](std::uint32_t) {
//             return this[std::uint32_t];
//         };
    } // namespace cpu
} // namespace narcissus

