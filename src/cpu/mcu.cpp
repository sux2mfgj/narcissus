#include <mcu.hpp>
#include <exception>

#include <iostream>

namespace narcissus {
    namespace cpu {

        mcu::mcu(std::array<std::uint8_t, ROM_SIZE>&& init_rom) 
            : rom(move(init_rom)), ram(), 
            serial_controler_interface{
                sci::SCI0_BASE_ADDR, sci::SCI1_BASE_ADDR, sci::SCI2_BASE_ADDR}
        {
//             serial_controler_interface = sci()
//             serial_controler_interface{0xffffb0, 0xffffb8, 0xffffc0};
//             sci[1](0xffffb8);
//             sci[2](0xffffc0);
        }


        std::uint8_t& mcu::operator[] (std::uint32_t address)
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
            if(address >= sci::SCI0_BASE_ADDR && address < sci::SCI1_BASE_ADDR)
            {
                return serial_controler_interface[0][address];
            }
            else if(address >= sci::SCI1_BASE_ADDR && address < sci::SCI2_BASE_ADDR)
            {
                return serial_controler_interface[1][address];
            }
            else if(address >= sci::SCI2_BASE_ADDR && address < 0xffffc6)
            {
                return serial_controler_interface[2][address];
            }

            std::cout << "memory access error: 0x"<< address << std::endl;
            throw std::out_of_range("access error");
        }
    } // namespace cpu
} // namespace narcissus
