#pragma once

#include <memory>

#include <boost/asio.hpp>

#include <narcissus.hpp>
#include <cpu.hpp>

namespace narcissus {

    //XXX
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
//             auto read_packet(std::array<char, 1024>, size_t length) -> void;
            auto work(std::array<char, 1024>& data, size_t length) -> std::string;

        private:
            std::shared_ptr<h8_3069f::cpu> cpu_;
            boost::asio::ip::tcp::acceptor acceptor_;
            boost::asio::ip::tcp::socket socket_;



    };

    //XXX
    } // namespace h8_3069f
} // namespace narcissus
