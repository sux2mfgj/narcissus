#include <sci.hpp>

#include <stdexcept>
#include <iostream>
#include <string>

namespace narcissus {
    namespace sci {

        sci::sci()
            : rsr(), rdr(), tsr(), tdr(), smr(), scr(), ssr(), brr(), scmr()
              ,access_flags(0)
        {}

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

            const static uint8_t flag_smr = 1 << 0;
            const static uint8_t flag_brr = 1 << 1;
            const static uint8_t flag_scr = 1 << 2;
            const static uint8_t flag_tdr = 1 << 3;
            const static uint8_t flag_ssr = 1 << 4;
            const static uint8_t flag_rdr = 1 << 5;
            const static uint8_t flag_scmr = 1 << 6;
            
            if(access_flags & flag_ssr){

                if((ssr & ssr_bits::tdre) != ssr_bits::tdre){
                    std::string s;
                    s.push_back(tdr);
//                     if(tdr != 0xd){
                        std::clog << std::hex << " 0x" << (std::uint32_t)tdr << std::flush;
//                     }
//                     if(tdr == 0x21){
//                         std::clog << std::endl;
//                     }

                    ssr |= ssr_bits::tdre;
                }
                access_flags &= ~flag_ssr;
            }

        }

    } // namespace sci
} // namespace narcissus
