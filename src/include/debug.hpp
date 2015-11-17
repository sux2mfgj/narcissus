#pragma once
#include <memory>
#include <sstream>
#include <ostream>

namespace narcissus {
    namespace h8_3069f {

        class cpu_debuger 
        {
            public:
                cpu_debuger(std::shared_ptr<cpu> c): controller(c){}
                 
            private:
                std::shared_ptr<cpu> controller;

            public:
                auto values(void) const -> std::string
                {
                    std::ostringstream os;
                    for(auto i = 0; i < controller->er.size(); ++i){
                        os << std::hex << i << ":" << controller->er[i].er << std::endl;;
                    }

                    os << std::hex << "ccr: "<< (std::uint16_t)controller->ccr.byte << std::endl;
                    return os.str();
                }
        };

        
        template<typename CharT, typename Traits>
        std::basic_ostream<CharT, Traits>& 
            operator<<(std::basic_ostream<CharT, Traits>& os, const cpu_debuger& debug)
        {
            os << debug.values();
            return os;
        }
        
    } // namespace h8_3069f
} // namespace narcissus
