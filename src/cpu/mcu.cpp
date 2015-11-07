#include <mcu.hpp>
#include <exception>

#include <iostream>

namespace narcissus {
    namespace cpu {

        mcu::mcu(std::array<std::uint8_t, ROM_SIZE>&& init_rom) 
            : rom(move(init_rom)), ram(), sci_channel{}
        {
//             serial_controler_interface = sci()
//             serial_controler_interface{0xffffb0, 0xffffb8, 0xffffc0};
//             sci[1](0xffffb8);
//             sci[2](0xffffc0);
        }


        auto mcu::operator[] (std::uint32_t address)
            -> std::uint8_t&
        {
//             std::cout << address << std::endl;
            if(address >= ROM_BASE_ADDR && address < ROM_END_ADDR) {
                return rom[address];
            }

            if(address >= RAM_BASE_ADDR && address < RAM_END_ADDR) {
                return ram[address - RAM_BASE_ADDR];
            }

            //TODO
            //add MMI/O
            if(address >= SCI0_BASE_ADDR && address < SCI1_BASE_ADDR)
            {
                return sci_channel[0][address];
            }
            else if(address >= SCI1_BASE_ADDR && address < SCI2_BASE_ADDR)
            {
                return sci_channel[1][address];
            }
            else if(address >= SCI2_BASE_ADDR && address < 0xffffc6)
            {
                return sci_channel[2][address];
            }

            std::cout << "memory access error: 0x"<< address << std::endl;
            throw std::out_of_range("access error");
        }


    } // namespace cpu
} // namespace narcissus
