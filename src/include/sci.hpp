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

        class sci {
            
            public:
                sci();
                std::uint8_t& operator[](std::uint32_t address);

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

            // for test
            private:
                FRIEND_TEST(ACCESS_FLAG, 0);


        };

    } // namespace sci
} // namespace narcissus
