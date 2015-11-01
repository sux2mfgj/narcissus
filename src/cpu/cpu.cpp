#include <cpu.hpp>

namespace narcissus {
    namespace cpu {
        
        h8_300::h8_300(std::array<uint8_t, ROM_SIZE>&& mem) : er(), er7(), ccr(), rom()
        {
            rom = move(mem); 
        }

        void h8_300::reset_exception()
        {
            // load pc from rom[0] ~ rom[3]
            auto reset_addr = uint32_t(rom[0]) << 24;
            reset_addr |= uint32_t(rom[1]) << 16;
            reset_addr |= uint32_t(rom[2]) << 8;
            reset_addr |= uint32_t(rom[3]);

            er7.er7.er32 = reset_addr;

            // reset ccr 
            ccr.reset();
        }

        void h8_300::cycle()
        {
            
        }

        operation h8_300::detect_operation()
        {
            auto op = rom[er7.er7.er32];

            auto ah = op >> 4;
            auto al = op & 0x0f;
            

            switch (ah) {
                case 8:
                    return operation::ADD_B_IMM;      
                    break;

                default:
                    return operation::INVALID;
            }
        }

    } // namespace cpu
} // namespace narcissus
