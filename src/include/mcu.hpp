#pragma once 

#include <cstdint>
#include <array>

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

        class mcu {
            
            public:
                mcu(std::array<std::uint8_t, ROM_SIZE>&& init_rom);
               
                std::uint8_t& operator[] (std::uint32_t address);

            private:
                std::array<std::uint8_t, ROM_SIZE> rom;
                std::array<std::uint8_t, RAM_SIZE> ram;
                std::array<sci::sci, 3> serial_controler_interface;
               
        };

    } // namespace cpu
} // namespace narcissus
