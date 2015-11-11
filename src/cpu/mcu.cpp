#include <mcu.hpp>
#include <exception>

#include <iostream>

namespace narcissus {
    namespace cpu {

        mcu::mcu(std::array<std::uint8_t, ROM_SIZE>&& init_rom) 
            : rom(move(init_rom)), ram() //, is_contitue(true)
        {
            sci_1 = std::make_shared<sci::sci>();

//             read_thread = std::thread(
//                     [this]()
//                     {
//                         char c; 

//                         while (true) 
//                         {
//                             {
//                                 std::unique_lock<std::mutex> lock(mtx);

//                                 cd.wait(lock, [this]{
//                                         return !(ssr & (std::uint8_tssr_bits::rdrf) || !is_contitue;
//                                         });

//                                 if(!is_contitue){
//                                     break;
//                                 }
//                             }

//                             std::cin >> c;
//                             rdr = c; // rdr = c;
//                             ssr |= (std::uint8_t)ssr_bits::rdrf;
//                         }
//                         std::cout << "rdr : " << rdr << std::endl;

//                     });
        }

        mcu::~mcu() 
        {
//             (*sci_1)[0x000000];
//             {
//                 std::lock_guard<std::mutex> lock(mtx);
//                 is_contitue = false;
//             }
//             cd.notify_one();
//             read_thread.join();
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

            //add MMI/O
            //TODO
            //consider about sci0 and sci2.
                if(address >= SCI1_BASE_ADDR && address < SCI2_BASE_ADDR)
            {
                return (*sci_1)[address];
            }

            std::cout << "memory access error: 0x"<< address << std::endl;
            throw std::out_of_range("access error");
        }

    } // namespace cpu
} // namespace narcissus
