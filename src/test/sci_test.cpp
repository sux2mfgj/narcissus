#include <gtest/gtest.h>

#include <array>

#include <sci.hpp>
#include <mcu.hpp>
using namespace std;

namespace narcissus {
    namespace sci {

        TEST(ACCESS_FLAG, 0)
        {
//             array<std::uint8_t, cpu::ROM_SIZE> mem = {0};
//             mem[0] = 0x00;
//             mem[1] = 0x00;
//             mem[2] = 0x01;
//             mem[3] = 0x00;

//             cpu::mcu mcu(move(mem));

//             mcu[cpu::SCI0_BASE_ADDR] = 0x00;
//             mcu[cpu::SCI0_BASE_ADDR + 1] = 0x01;
//             mcu[cpu::SCI0_BASE_ADDR + 2] = 0x01;
//             mcu[cpu::SCI0_BASE_ADDR + 3] = 0x01;
//             mcu[cpu::SCI0_BASE_ADDR + 4] = 0x01;
//             mcu[cpu::SCI0_BASE_ADDR + 5] = 0x01;
//             mcu[cpu::SCI0_BASE_ADDR + 6] = 0x01;

//             ASSERT_EQ(0x00000000, access_flags)

            sci sci0;
            sci0[cpu::SCI0_BASE_ADDR] = 0x00;
            sci0[cpu::SCI0_BASE_ADDR + 1] = 0x00;
            sci0[cpu::SCI0_BASE_ADDR + 2] = 0x00;
            sci0[cpu::SCI0_BASE_ADDR + 3] = 0x00;
            sci0[cpu::SCI0_BASE_ADDR + 4] = 0x00;
            sci0[cpu::SCI0_BASE_ADDR + 5] = 0x00;
            sci0[cpu::SCI0_BASE_ADDR + 6] = 0x00;

            ASSERT_EQ(0x7f, sci0.access_flags);
        }
        
    } // namespace sci
} // namespace narcissus
