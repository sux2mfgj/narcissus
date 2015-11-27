#include <gdb.hpp>
#include <array>
#include <cassert>

namespace narcissus {

    //XXX
    namespace h8_3069f {

    gdb_server::gdb_server(
            std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size>&& memory,
            boost::asio::io_service& io_service,
            std::uint16_t port)
        : cpu_(std::make_shared<h8_3069f::cpu>(std::move(memory))), 
        acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
        socket_(io_service)
    {}

    auto gdb_server::run() -> void
    {
        cpu_->before_run();
        acceptor_.accept(socket_);
        try
        {
            while (true) 
            {
                std::cout << "#a" << std::endl;
                std::array<char, 1024> data{0};

                boost::system::error_code error;
                size_t length = socket_.read_some(boost::asio::buffer(data), error);
                if(error == boost::asio::error::eof)
                {
                    break;
                }
                else if(error)
                {
                    throw boost::system::system_error(error);
                }
                std::cout << std::string(data.data()) << std::endl;

                auto r = work(data, length);
                std::cout << "#b" << std::endl;
                boost::asio::write(socket_, boost::asio::buffer(r.c_str(), r.length()));
                std::cout << "#c" << std::endl;
            }
        }
        catch(std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }
    }

    auto gdb_server::work(std::array<char, 1024>& data, size_t length) -> std::string
    {
        auto i = 0;
        for ( i = 0; i < length; i++) {
            if(data[i] == '$')
            {
                break;
            }
        }

        if(i >= length)
        {
            return "";
        }

        std::string return_packet;

        std::cout << data[i+1] << std::endl;
        switch (data[++i]) {
            // read register
            case '?':
                assert(false);
                break;

            // return cpu register
            case 'g':
                assert(false);
                break;

            // single step
            case 's':
            {
                auto pc = cpu_->cycle();
                return "+";
            }

            case 'q':
            {
                //$qSupported:multiprocess+;swbreak+;hwbreak+;qRelocInsn+#c9
                
                switch (data[++i]) {
                    case 'S':
                        //TODO: qSupported
                        return "multiprocess-";

                    case 'T':
                        //TODO $qTStatus#49
                        return "tnotrun:0";
                }
            }

            case '+':
            {
                //TODO
                return "+";
            }

            case 'H':
            {
                return "OK";
            }
        }
        std::cout << "reach end [work]" << std::endl;
        assert(false);
    }

    auto gdb_server::interrupt(::narcissus::h8_3069f::interrupts int_num) -> bool
    {
        return cpu_->interrupt(int_num);
    }

    //XXX
    } // namespace h8_3069f

} // namespace narcissus

