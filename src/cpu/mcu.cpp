#include <mcu.hpp>
#include <exception>

namespace narcissus {
    namespace cpu {

        mcu::mcu(std::array<std::uint8_t, ROM_SIZE>&& init_rom) 
            : rom(move(rom)), ram()
        {
        }

        std::uint8_t& mcu::operator[] (std::uint32_t address)
        {
            if(address > ROM_BASE_ADDR && address < ROM_END_ADDR) {
                return rom[address];
            }

            if(address > RAM_BASE_ADDR && address < RAM_END_ADDR) {
                return ram[address];
            }

            //TODO
            //add MMI/O

            throw std::out_of_range("access error");
        }
    } // namespace cpu
} // namespace narcissus
