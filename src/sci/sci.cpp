#include <sci.hpp>

#include <stdexcept>
#include <iostream>
#include <string>

namespace narcissus {
    namespace sci {

        sci::sci()
            : rsr(), rdr(), tsr(), tdr(), smr(), scr(), 
            ssr((std::uint8_t)ssr_bits::rdrf), brr(), scmr(), access_flags(0)
              ,input_buffer()
              //, is_continue(true)
        {

//             read_thread = std::thread(
//                     [this]()
//                     {
//                         char c; 

//                         while (true) 
//                         {
//                             {
//                                 std::unique_lock<std::mutex> lock(mtx);

//                                 cd.wait(lock, [this]{
//                                             return !(ssr & (std::uint8_t)ssr_bits::rdrf) || !is_continue;
//                                         });

//                                 if(!is_continue){
//                                     break;
//                                 }
//                             }

//                             std::cin >> c;
//                             rdr = c; 
//                             std::cout << "c: " << c << std::endl;
//                         }
//                     });
        }

        sci::~sci()
        {
            work();
//             {
//                 std::lock_guard<std::mutex> lock(mtx);
//                 is_continue = false;
//             }
//             cd.notify_one();
//             read_thread.join();
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
                    if(tdr != 0xd){
                        std::clog << (char)tdr << std::flush;
                    }

                    ssr |= (std::uint8_t)ssr_bits::tdre;
                }

                if(!(ssr & (std::uint8_t)ssr_bits::rdrf)){
                    //TODO 
                    //when read string, is some charcter lost?
                    
                    char c;

                    if(input_buffer.size() != 0){
                        std::string tmp;
                        std::cin >> tmp;

                        for(auto t : tmp)
                        {
                            input_buffer.push(t);
                        }
                    }
                    c = input_buffer.front();
                    input_buffer.pop();

                    rdr = c;
//                     cd.notify_one();
                    ssr |= (std::uint8_t)ssr_bits::rdrf;
                }

                access_flags &= ~(std::uint8_t)access_flag::ssr;
            }

        }

    } // namespace sci
} // namespace narcissus
