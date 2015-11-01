#pragma once

#include <cstdint>

namespace narcissus {
    namespace cpu {

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
                h8_300();

            private:
                //registers
                register_t er[7];
                er7_register pc;
                conditional_code_register ccr;

                //built-in memory
                std::uint8_t rom[ROM_SIZE];
                //std::uint8_t ram[RAM_SIZE];
            
            public:
                void reset_exception();
        };
    } // namespace cpu
} // namespace narcissus

