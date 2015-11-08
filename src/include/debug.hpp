#pragma once
#include <memory>
#include <sstream>
#include <ostream>

namespace narcissus {
    namespace cpu {

        class cpu_debuger 
        {
            public:
                cpu_debuger(std::shared_ptr<h8_300> c): cpu(c){}
                 
            private:
                std::shared_ptr<h8_300> cpu;

            public:
                auto values(void) const -> std::string
                {
                    std::ostringstream os;
                    for(auto i = 0; i < cpu->er.size(); ++i){
                        os << std::hex << i << ":" << cpu->er[i].er << std::endl;;
                    }
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
        
    } // namespace cpu
} // namespace narcissus
