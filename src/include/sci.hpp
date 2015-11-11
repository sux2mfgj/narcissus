#pragma once
#include <gtest/gtest.h>

#include <cstdint>
#include <queue>

namespace narcissus {
    namespace sci {

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

        enum class access_flag : std::uint8_t
        {
            smr = 1 << 0,
            brr = 1 << 1,
            scr = 1 << 2,
            tdr = 1 << 3,
            ssr = 1 << 4,
            rdr = 1 << 5,
            scmr = 1 << 6,
        };

        class sci {
            
            public:
                sci();
                virtual ~sci();
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

                std::queue<std::uint8_t> input_buffer;

            private:
                auto work(void) -> void;

            // for test
            private:
                FRIEND_TEST(ACCESS_FLAG, 0);
                FRIEND_TEST(PUTC_SERIAL, 0);


        };

    } // namespace sci
} // namespace narcissus
