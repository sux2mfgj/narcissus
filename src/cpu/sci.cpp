#include <sci.hpp>

#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>

#include <cpu.hpp>

namespace narcissus {
    namespace h8_3069f{

        sci::sci(std::shared_ptr<std::condition_variable> cv, 
                std::shared_ptr<bool> is_s, std::shared_ptr<std::mutex> m)
            : rsr(), rdr(), tsr(), tdr(), smr(), scr(), 
            ssr((std::uint8_t)ssr_bits::rdrf), brr(), scmr(), access_flags(0),
            c_variable_ptr(cv), is_sleep(is_s), cv_mutex_ptr(m)
        {} 

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

        auto sci::before_run(std::shared_ptr<cpu> c) -> void
        {
           controller = c; 
           read_thread = std::thread(
                    [&]
                    {
                        while (true) 
                        {
                            char buf;
                            std::cin.read(&buf, sizeof(buf));
                            read_buffer.push((std::uint8_t)buf);
                            ssr |= (std::uint8_t)ssr_bits::rdrf;

                            if(auto ptr = controller.lock())
                            {
                                if(scr & (std::uint8_t)scr_bits::rie)
                                {
                                    ptr->interrupt(h8_3069f::interrupts::rxi1);
                                    c_variable_ptr->notify_all();
                                }
                            }
                        }
                    });
            read_thread.detach();
        }

        auto sci::work() -> void
        {
            if(access_flags & (std::uint8_t)access_flag::ssr){

                if((ssr & (std::uint8_t)ssr_bits::tdre) != (std::uint8_t)ssr_bits::tdre)
                {
                    std::cout << tdr << std::flush;

                    std::clog << std::hex << "out(hex): " << (std::uint16_t)tdr 
                        << "(" << (char)tdr << ")"<<std::endl;

                    ssr |= (std::uint8_t)ssr_bits::tdre;

                    if(auto ptr = controller.lock())
                    {
                        if(scr & (std::uint8_t)scr_bits::tie)
                        {
                            ptr->interrupt(h8_3069f::interrupts::txi1);
                            c_variable_ptr->notify_all();
                        }
                    }
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

    } // namespace h8_3069f
} // namespace narcissus
