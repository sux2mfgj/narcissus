#pragma once
#include <gtest/gtest.h>

#include <cstdint>
#include <queue>
#include <thread>
#include <condition_variable>
#include <memory>

namespace narcissus {

    namespace h8_3069f{

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

        class cpu;

        class sci {

            public:
                sci(std::shared_ptr<std::condition_variable> cv, 
                        std::shared_ptr<bool> is_s);
                virtual ~sci();

            public:
                auto operator[](std::uint32_t address) -> std::uint8_t&;
                auto before_run(std::shared_ptr<cpu> c) -> void;

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

                std::thread read_thread;
                std::queue<std::uint8_t> read_buffer;

                std::shared_ptr<std::condition_variable> c_variable_ptr;
                std::shared_ptr<bool> is_sleep;
                std::weak_ptr<cpu> controller;

            private:
                auto work(void) -> void;

        };
    } // namespace sci
} // namespace narcissus
