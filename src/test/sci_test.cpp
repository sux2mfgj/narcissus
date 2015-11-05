#include <gtest/gtest.h>

#include <array>

#include <sci.hpp>
#include <mcu.hpp>
using namespace std;

namespace narcissus {
    namespace sci {

        TEST(ACCESS_FLAG, 0)
        {
//             sci sci0;
//             sci0[cpu::SCI0_BASE_ADDR] = 0x00;
//             sci0[cpu::SCI0_BASE_ADDR + 1] = 0x00;
//             sci0[cpu::SCI0_BASE_ADDR + 2] = 0x00;
//             sci0[cpu::SCI0_BASE_ADDR + 3] = 0x00;
//             sci0[cpu::SCI0_BASE_ADDR + 4] = 0x00;
//             sci0[cpu::SCI0_BASE_ADDR + 5] = 0x00;
//             sci0[cpu::SCI0_BASE_ADDR + 6] = 0x00;

//             ASSERT_EQ(0x7f, sci0.access_flags);
        }

        TEST(PUTC_SERIAL, 0)
        {
            sci sci0;
            ASSERT_EQ(0, 0);
        }
        
    } // namespace sci
} // namespace narcissus
