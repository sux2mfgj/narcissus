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
                    std::array<char, 1024> data{0};

                    boost::system::error_code error;
                    size_t length = socket_.read_some(boost::asio::buffer(data), error);
                    if(error == boost::asio::error::eof)
                    {
                        std::cout << "eof" << std::endl;
                        break;
                    }
                    else if(error)
                    {
                        throw boost::system::system_error(error);
                    }
                    std::cout << "receive: " << std::string(data.data()) << std::endl;

//                     auto r = work(data, length);
                    work(data, length);
//                     if(!r.empty())
//                     {
//                         std::cout << "send:    " << r.c_str() << std::endl;
//                         boost::asio::write(socket_, boost::asio::buffer(r.c_str(), r.length()));
//                     }
                }
            }
            catch(std::exception& e)
            {
                std::cout << e.what() << std::endl;
            }
        }

        //TODO
        auto gdb_server::work(std::array<char, 1024>& data, size_t length) 
            -> void
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
                return;
            }

            std::stringstream stream;
            auto ack = [this]
            {
                boost::asio::write(socket_, boost::asio::buffer("+", 1));
            };

            auto reply = [this](std::string s)
            {
                std::stringstream stream;
                stream << "$" << s << "#" 
                    << std::hex << std::setfill('0') << std::setw(2) 
                    << (check_sum(s.c_str()) % 0x100);

                boost::asio::write(socket_, boost::asio::buffer(stream.str().c_str(), stream.str().length()));
            };

            switch (data[++i]) {
                // read register
                case '?':
                    {
                        //$?#3f
                        ack();
//                         reply("T001");
                        reply("S00");
                        break;
                    }
                    // return cpu register
                case 'g':
                    assert(false);
                    break;
                    // single step
                case 's':
                    {
                        auto pc = cpu_->cycle();
                        assert(false);
                    }

                case 'q':
                    {
                        switch (data[++i]) {
                            case 'A':
                                {
                                    //$qAttached#8f
                                    ack();
                                    reply("l");
                                    break;
                                }
                            case 'C':
                                {
                                    //$qC#b4
                                    ack();
                                    reply("QC0");
                                    break;
                                }
                            case 'S':
                                {
                                    //$qSupported:multiprocess+;swbreak+;hwbreak+;qRelocInsn+#c9
                                    ack();
                                    reply("multiprocess-");
                                    break;
                                }
                            case 'T':
                                {
                                    switch (data[++i]) {
                                        case 'S':
                                            {
                                                //$qTStatus#49
                                                ack();
                                                reply("T0;tnotrun:0");
                                                break;
                                            }

                                        case 'f':
                                            {
                                                //TODO $qTfV#81
                                                ack();
                                                reply("1:0000000000000000:1:61");
                                                break;
                                            }

                                        case 's':
                                            {
                                                //$qTsV#8e
                                                ack();
                                                reply("l");
                                                break;
                                            }

                                    }
                                }
                            case 'f':
                                {
                                    switch(data[++i]){
                                        case 'T':
                                            {
                                                //$qfThreadInfo#bb
                                                ack();
                                                reply("m 0");
                                                break;
                                            }
                                    }
                                }
                            case 's':
                            {
                                switch (data[++i]) {
                                    case 'T':
                                        //$qsThreadInfo#c8+
                                        ack();
                                        reply("l");
                                        break;
                                }
                            }
                        }
                    }

                case '+':
                    {
                        reply("");
                        break;
                    }

                case 'H':
                    {
                        //$Hg0#df
                        ack();
                        reply("");
                        break;
                    }

                default:
                    {
                        //TODO
                        assert(false);
                        //$Hg0#df
//                         ack();
//                         reply("");
//                         break;
                    }
            }
        }

        auto gdb_server::interrupt(::narcissus::h8_3069f::interrupts int_num) -> bool
        {
            return cpu_->interrupt(int_num);
        }

        //XXX
    } // namespace h8_3069f

} // namespace narcissus

