#include <sci.hpp>

#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>

namespace narcissus {
    namespace sci {

        sci::sci()
            : rsr(), rdr(), tsr(), tdr(), smr(), scr(), 
            ssr((std::uint8_t)ssr_bits::rdrf), brr(), scmr(), access_flags(0)
        {
            read_thread = std::thread(
                    [&]{
                        while (true) {
                            char buf;
                            std::cin.read(&buf, sizeof(buf));
                            read_buffer.push((std::uint8_t)buf);
                            ssr |= (std::uint8_t)ssr_bits::rdrf;
//                             std::clog << "in now: " << (std::uint8_t)buf << std::endl;
                        }
                    });
            read_thread.detach();
        } 

        sci::~sci()
        {
            work();
        }

        auto sci::operator[](std::uint32_t address) -> std::uint8_t&
        {
            work();
            access_flags |= 1 << (address & 0x7);

            switch (address & 0x7) {
                case 0x0:
                    return smr;
                case 0x1:
                    return brr;
                case 0x2:
                    return scr;
                case 0x3:
                    return tdr;
                case 0x4:
                    return ssr;
                case 0x5:
                    rdr = (std::uint8_t)read_buffer.front();
                    read_buffer.pop();
                    std::clog << std::hex << "in(hex): " << (std::uint16_t)rdr 
                        << "(" << (char)rdr << ")" << std::endl;
                    return rdr;
                case 0x6:
                    return scmr;

                default:
                    assert(false);
            }
        }

        auto sci::work() -> void
        {
            if(access_flags & (std::uint8_t)access_flag::ssr){

                if((ssr & (std::uint8_t)ssr_bits::tdre) != (std::uint8_t)ssr_bits::tdre){
                    std::cout << tdr << std::flush;

                    std::clog << std::hex << "out(hex): " << (std::uint16_t)tdr 
                        << "(" << (char)tdr << ")"<<std::endl;

                    ssr |= (std::uint8_t)ssr_bits::tdre;
                }

                if(!(ssr & (std::uint8_t)ssr_bits::rdrf))
                {
                    if(!read_buffer.empty()){
                        ssr |= (std::uint8_t)ssr_bits::rdrf;
                    }
                }

                access_flags &= ~(std::uint8_t)access_flag::ssr;
            }
        }


    } // namespace sci
} // namespace narcissus
