#include <cpu.hpp>

namespace narcissus {
    namespace cpu {
        
        h8_300::h8_300() : er(), pc(), ccr(), rom()
        {
            
        }

        void h8_300::reset_exception()
        {
            // load pc from rom[0] ~ rom[3]
            auto reset_addr = uint32_t(rom[0]) << 24;
            reset_addr |= uint32_t(rom[1]) << 16;
            reset_addr |= uint32_t(rom[2]) << 8;
            reset_addr |= uint32_t(rom[3]);

            // reset ccr 
            ccr.interrupt_mask = 1;
        }



    } // namespace cpu
} // namespace narcissus
