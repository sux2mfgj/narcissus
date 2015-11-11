#pragma once
#include <gtest/gtest.h>

#include <cstdint>

namespace narcissus {
    namespace sci {

        const uint8_t SCR_TRANSMIT_INTERRUPT_ENABLE         = 0b10000000;
        const uint8_t SCR_RECEIVE_INTERRUPT_ENABLE          = 0b01000000;
        const uint8_t SCR_TRANSMIT_ENABLE                   = 0b00100000; 
        const uint8_t SCR_RECEIVE_ENABLE                    = 0b00010000;
        const uint8_t SCR_MULTIPROCESSOR_INTERRUPT_ENABLE   = 0b00001000;
//         const uint8_t SCR_
        
        enum class ssr_bits : std::uint8_t
        {
            mpbt = 1 << 0,
            mpb = 1 << 1,
            tend = 1 << 2,
            per = 1 << 3,
            ferers = 1 << 4,
            orer = 1 << 5,
            rdrf = 1 << 6,
            tdre = 1 << 7
        };

        class sci {
            
            public:
                sci();
                auto operator[](std::uint32_t address) -> std::uint8_t&;

            private:
                std::uint8_t rsr;
                std::uint8_t rdr;
                std::uint8_t tsr;
                std::uint8_t tdr;
                std::uint8_t smr;
                std::uint8_t scr;
                std::uint8_t ssr;
                std::uint8_t brr;
                std::uint8_t scmr;

                std::uint8_t access_flags;

            private:
                auto work(void) -> void;

            // for test
            private:
                FRIEND_TEST(ACCESS_FLAG, 0);
                FRIEND_TEST(PUTC_SERIAL, 0);


        };

    } // namespace sci
} // namespace narcissus
