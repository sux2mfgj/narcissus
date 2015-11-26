#pragma once

#include <boost/asio.hpp>

namespace narcissus {
    namespace debug {
        class gdb_server {
            
            public:
                gdb_server(boost::asio::io_service& io_service, uint16_t port);
                virtual ~gdb_server() = default;

            public:
//                 run()

            private:
                boost::asio::ip::tcp::socket socket_;
                boost::asio::ip::tcp::acceptor acceptor_;
        };
    } // namespace debug
} // namespace narcissus
