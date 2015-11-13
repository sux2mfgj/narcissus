#include <sci.hpp>

#include <stdexcept>
#include <iostream>
#include <string>

namespace narcissus {
    namespace sci {

        sci::sci()
            : rsr(), rdr(), tsr(), tdr(), smr(), scr(), 
            ssr((std::uint8_t)ssr_bits::rdrf), brr(), scmr(), access_flags(0)
        {
            read_thread = std::thread(
                    [&]{
                        while (true) {
                            if(ssr & (std::uint8_t)ssr_bits::rdrf){

                                char buf;
                                std::cin.read(&buf, sizeof(buf));
                                std::clog << std::hex << "in(hex): " << buf << std::endl;
                                rdr = (std::uint8_t)buf;
                                ssr |= (std::uint8_t)ssr_bits::rdrf;
                            }
                        }
                    });
            read_thread.detach();
        } 

        sci::~sci()
        {
            work();
//             std::quick_exit(EXIT_SUCCESS);
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
                    return rdr;
                case 0x6:
                    return scmr;

                default:
                    throw std::out_of_range("in sci");
            }
        }

        auto sci::work() -> void
        {

            if(access_flags & (std::uint8_t)access_flag::ssr){

                if((ssr & (std::uint8_t)ssr_bits::tdre) != (std::uint8_t)ssr_bits::tdre){
                    std::string s;
                    s.push_back(tdr);
                    std::cout << (std::uint8_t)tdr << std::flush;

                    std::clog << std::hex << "out(hex): " << (std::uint16_t)tdr << std::endl;

                    ssr |= (std::uint8_t)ssr_bits::tdre;
                }

//                 if(!(ssr & (std::uint8_t)ssr_bits::rdrf)){

//                     ssr |= (std::uint8_t)ssr_bits::rdrf;
//                 }

                access_flags &= ~(std::uint8_t)access_flag::ssr;
            }
        }


    } // namespace sci
} // namespace narcissus
