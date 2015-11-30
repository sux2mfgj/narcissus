#pragma once

#include <memory>
#include <vector>

#include <boost/asio.hpp>

#include <narcissus.hpp>
#include <cpu.hpp>

namespace narcissus {

    namespace h8_3069f {

        class gdb_server 
            : public narcissus
        {
            public:
                gdb_server( 
                        std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size>&& memory,
                        boost::asio::io_service& io_service, std::uint16_t port);

                virtual ~gdb_server() = default;

            public:
                auto run() -> void;
                auto interrupt(h8_3069f::interrupts) -> bool;

            private:
                auto work(std::array<char, 1024>& data, size_t length) -> void;

            private:
                std::shared_ptr<h8_3069f::cpu> cpu_;
                boost::asio::ip::tcp::acceptor acceptor_;
                boost::asio::ip::tcp::socket socket_;

                std::vector<std::uint32_t> break_point_list_;
                
        };

        namespace  {
            constexpr auto check_sum(const char* text) -> std::uint32_t
            {
                if(text[0] == '\0')
                {
                    return 0;
                }
                else 
                {
                    return check_sum(&text[1]) + (std::uint8_t)text[0];
                }
            }

        } // namespace 

    } // namespace h8_3069f
} // namespace narcissus
