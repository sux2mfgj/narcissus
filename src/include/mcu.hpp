#pragma once 

#include <cstdint>
#include <array>
#include <memory>
#include <thread>
#include <condition_variable>
#include <mutex>

#include <sci.hpp>

namespace narcissus {
    namespace cpu {

        const std::uint32_t ROM_BASE_ADDR   = 0x00000000;
        const std::uint32_t ROM_END_ADDR    = 0x00080000;
        const std::size_t ROM_SIZE          = ROM_END_ADDR - ROM_BASE_ADDR;

        // mode 5
        const std::uint32_t RAM_BASE_ADDR   = 0x00ffbf20;
        const std::uint32_t RAM_END_ADDR    = 0x00ffff1f;
        const std::size_t RAM_SIZE          = RAM_END_ADDR - RAM_BASE_ADDR;

        const std::uint32_t SCI0_BASE_ADDR = 0xffffb0;
        const std::uint32_t SCI1_BASE_ADDR = 0xffffb8;
        const std::uint32_t SCI2_BASE_ADDR = 0xffffc0;

        class mcu {
            
            public:
                mcu(std::array<std::uint8_t, ROM_SIZE>&& init_rom);
                ~mcu();
               
                auto operator[] (std::uint32_t address) -> std::uint8_t&;

            private:
                std::array<std::uint8_t, ROM_SIZE> rom;
                std::array<std::uint8_t, RAM_SIZE> ram;
//                 std::array<std::shared_ptr<sci::sci>, 3> sci_channel;
                std::shared_ptr<sci::sci> sci_1;

                std::thread read_thread;

                bool is_contitue; 
                std::condition_variable cd;
                std::mutex mtx;

        };

    } // namespace cpu
} // namespace narcissus
