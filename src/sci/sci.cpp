#include <sci.hpp>

#include <stdexcept>

namespace narcissus {
    namespace sci {

        sci::sci(std::uint32_t addr)
            : base_address(addr) ,rsr(), rdr(), tsr(), 
            tdr(), smr(), scr(), ssr(), brr(), scmr()
        {

        }

        std::uint8_t& sci::operator[](std::uint32_t address)
        {
            
            switch (address & 0xf) {
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

    } // namespace sci
} // namespace narcissus
