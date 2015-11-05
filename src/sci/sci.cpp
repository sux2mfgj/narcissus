#include <sci.hpp>

#include <stdexcept>
#include <iostream>

namespace narcissus {
    namespace sci {

        sci::sci()
            : rsr(), rdr(), tsr(), tdr(), smr(), scr(), ssr(), brr(), scmr()
              ,access_flags(0)
        {}

        std::uint8_t& sci::operator[](std::uint32_t address)
        {
            access_flags |= 1 << (address & 0xf);

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

    } // namespace sci
} // namespace narcissus
