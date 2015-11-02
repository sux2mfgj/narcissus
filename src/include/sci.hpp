#pragma once

#include <cstdint>

namespace narcissus {
    namespace sci {

        const std::uint32_t SCI0_BASE_ADDR = 0xffffb0;
        const std::uint32_t SCI1_BASE_ADDR = 0xffffb8;
        const std::uint32_t SCI2_BASE_ADDR = 0xffffc0;

        class sci {
            
            public:
                sci(std::uint32_t base_addr);
                std::uint8_t& operator[](std::uint32_t address);

            private:
                std::uint32_t base_address;

                std::uint8_t rsr;
                std::uint8_t rdr;
                std::uint8_t tsr;
                std::uint8_t tdr;
                std::uint8_t smr;
                std::uint8_t scr;
                std::uint8_t ssr;
                std::uint8_t brr;
                std::uint8_t scmr;
                
        };

    } // namespace sci
} // namespace narcissus
