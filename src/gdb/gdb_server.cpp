#include <gdb_server.hpp>

namespace narcissus {
    namespace debug {

        gdb_server::gdb_server(boost::asio::io_service &io_service, uint16_t port)
            : acceptor_(io_service, 
                    boost::asio::ip::tcp::endpoint(
                        boost::asio::ip::tcp::v4(), port)),
                socket_(io_service)
        {}


    } // namespace debug
} // namespace narcissus

