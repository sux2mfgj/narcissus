#include <gdb.hpp>

#include <array>
#include <cassert>
#include <algorithm>

namespace narcissus {
    namespace h8_3069f {

        gdb_server::gdb_server(
                std::array<std::uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size>&& memory,
                boost::asio::io_service& io_service,
                std::uint16_t port)
            : cpu_(std::make_shared<h8_3069f::cpu>(std::move(memory))), 
            acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
            socket_(io_service), break_point_list_()
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
                    std::clog << "receive: " << std::string(data.data()) << std::endl;

                    work(data, length);
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

            auto read_register = [this]() -> std::string
            {
                std::stringstream stream;
                stream << std::hex;
                for (auto&& c : cpu_->er) {
                    stream << std::setw(4) << std::setfill('0') << c.r; 
                }
                stream << cpu_->sp << (std::uint16_t)cpu_->ccr.byte << cpu_->pc;
                return stream.str();
            };

            switch (data[++i]) {
                // read register
                case '?':
                    {
                        //$?#3f
                        ack();
                        reply("S000");
                        break;
                    }
                // continue
                case 'c':
                    {
                        ack();
                        while (true) {
                            auto pc = cpu_->cycle();
                            if(break_point_list_.size() != 0)
                            {
                                auto itr = std::find(break_point_list_.begin(), break_point_list_.end(), pc);
                                if(itr != break_point_list_.end())
                                {
                                    reply("S00;swbreak");
                                    break;
                                }
                            }
                        }
                        break;
                    }
                    // return cpu register
                case 'g':
                    {
                        //0000000000000000000000000000fffef884020c
                        ack();
                        std::stringstream stream;
                        for (auto&& r : cpu_->er) {
                            stream << std::hex << std::setw(8) << std::setfill('0') << r.er;
                        }
                        stream << std::setw(8) << std::setfill('0') << std::hex << cpu_->sp;
                        stream << std::setw(8) << std::setfill('0') << std::hex << (std::uint16_t)cpu_->ccr.byte;
                        stream << std::setw(8) << std::setfill('0') << std::hex << cpu_->pc;
                        reply(stream.str());
                        break;
                    }

                case 'm':
                    {
                        //$m208,4#67
                        ack();
                        char buf[10];
                        for(auto j = 0; j < 8; ++j)
                        {
                            if(data[i + 1] == ',')
                            {
                                buf[j] = '\0';
                                break;
                            }

                            buf[j] = data[++i];
                        }
                        int addr = std::stoi(std::string(buf), nullptr, 16);

                        i++;
                        for(auto j = 0; j < 9; ++j)
                        {
                            if(data[i + 1] == '#')
                            {
                                buf[j] = '\0';
                                break;
                            }

                            buf[j] = data[++i];
                        }
                        int length = std::stoi(std::string(buf), nullptr, 16);

                        std::stringstream stream;
                        for(auto j= 0; j< length; ++j)
                        {
                            stream << std::hex << std::setw('2') << std::setfill('0') 
                                << (std::uint16_t)cpu_->memory[addr + j];
                        }

                        reply(stream.str());
                        break;
                    }
                    // single step
                case 's':
                    {
                        //$s#73
                        ack();
                        auto pc = cpu_->cycle();
                        //TODO
                        //reply important register value
                        reply("S00");
                        break;
                    }

                case 'v':
                    {
                        //$vCont?#49+
                        ack();
                        reply("vCont;c;s");
                        break;
                    }

                case 'p':
                    {
                        //$pc#d3
                        // reply register value
                        ack();
                        switch(data[++i])
                        {
                            
                            case '0':
                            case '0' + 1:
                            case '0' + 2:
                            case '0' + 3:
                            case '0' + 4:
                            case '0' + 5:
                            case '0' + 6:
                                {
                                    //$p6#a6+
                                    std::stringstream stream;
                                    stream << std::hex << std::setw(8) << std::setfill('0')
                                        << cpu_->er['0' - data[i]].er;
                                    reply(stream.str());
                                    break;
                                }

                            case '0' + 7:
                                {
                                    //$p7#a7
                                    std::stringstream stream;
                                    stream << std::hex << std::setw(8) << std::setfill('0')
                                        << cpu_->sp;
                                    reply(stream.str());
                                    break;
                                }

                            case '0' + 8:
                                {
                                    std::stringstream stream;
                                    stream << std::hex << std::setw(4) << std::setfill('0')
                                        << (std::uint16_t)cpu_->ccr.byte;
                                    reply(stream.str());
                                    break;
                                }

                            case '0' + 9:
                                {
                                    //$p9#a9+
                                    std::stringstream stream;
                                    stream << std::hex << std::setw(4) << std::setfill('0')
                                        << cpu_->pc;
                                    reply(stream.str());
                                    break;
                                }

                            case 'a':
                            case 'a' + 1:
                            case 'a' + 2:
                                {
                                    reply("0000");
                                    break;
                                }

                            default:
                                assert(false);
                        }
                        break;
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
                                    reply("QC00");
                                    break;
                                }
                            case 'O':
                                {
                                    //$qOffsets#4b
                                    ack();
                                    reply("Text=00000000;Data=00fffc20;Bss=00fffc30");
                                    break;
                                }
                            case 'S':
                                {
                                    //$qSupported:multiprocess+;swbreak+;hwbreak+;qRelocInsn+#c9
                                    ack();
                                    reply("multiprocess-;swbreak+;qRelocInsn-");
                                    break;
                                }
                            case 'T':
                                {
                                    switch (data[++i]) {
                                        case 'S':
                                            {
                                                //$qTStatus#49
                                                ack();
//                                                 reply("T0;tstop:0");
                                                reply("T0");
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
                                        default:
                                            assert(false);
                                    }
                                }
                                break;
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

                                        default:
                                            assert(false);
                                    }
                                    break;
                                }
                            case 's':
                                {
                                    switch (data[++i]) {
                                        case 'T':
                                            //$qsThreadInfo#c8+
                                            ack();
                                            reply("l");
                                            break;

                                        default:
                                            assert(false);
                                    }
                                    break;
                                }

                            default:
                                {
                                    assert(false);
                                }
                        }
                        break;
                    }
                // remove break point
                case 'z':
                    {
                        switch (data[++i]) {
                            case '0':
                                {
                                    ack();
                                    //$z0,210,2#c7
                                    if(break_point_list_.size() == 0)
                                    {
                                        //TODO
                                        reply("E");
                                        break;
                                    }

                                    ++i;
                                    char buf[9];
                                    for(auto j = 0; j < 9; ++j)
                                    {
                                        if(data[i + 1] == ',')
                                        {
                                            buf[j] = '\0';
                                            break;
                                        }

                                        buf[j]  = data[++i];
                                    }
                                    auto b_addr = std::stoi(std::string(buf), nullptr, 16);

                                    auto itr = 
                                        std::find(break_point_list_.begin(), break_point_list_.end(), b_addr);

                                    if(itr != break_point_list_.end())
                                    {
                                        break_point_list_.erase(itr);
                                        reply("OK");
                                    }
                                    else {
                                        //TODO add error code after "Ew"
                                        reply("E");
                                    }

                                    break;
                                }
                            default:
                                assert(false);
                        }
                        break;
                    }
                // insert break point
                case 'Z':
                    {
                        switch (data[++i]) {
                            case '0':
                                {
                                    //$Z0,766,2#0f
                                    //TODO
                                    ack();
                                    char buf[9];
                                    ++i;
                                    for(auto j = 0; j < 9; ++j)
                                    {
                                        if(data[i + 1] == ',')
                                        {
                                            buf[j] = '\0';
                                            break;
                                        }

                                        buf[j]  = data[++i];
                                    }
                                    auto b_addr = std::stoi(std::string(buf), nullptr, 16);

                                    break_point_list_.push_back(b_addr);
                                    reply("OK");
                                    break;
                                }
                                
                            default:
                                assert(false);
                        }
                        break;
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
                // register write
                case 'P':
                    {
                        //$P9=0100#87
                        ack();

                        char reg_num = data[++i] - '0';
                        switch (reg_num) {
                            case 0:
                            case 1:
                            case 2:
                            case 3:
                            case 4:
                            case 5:
                            case 6:
                                //er register
                                assert(false);
                                break;

                            case 7:
                                // sp
                                assert(false);
                                break;
                            case 8:
                                // ccr
                                assert(false);
                                break;
                            case 9:
                                {
                                    // pc
                                    char buf[8];
                                    ++i;
                                    for(auto j = 0;; ++j)
                                    {
                                        if(data[i + 1] == '#')
                                        {
                                            buf[j] = '\0';
                                            break;
                                        }
                                        buf[j] = data[++i];
                                    }
                                    cpu_->pc = std::stoi(std::string(buf), nullptr, 16);
                                    break;
                                }
                                
                            default:
                                assert(false);
                        }

                        reply("OK");
                        break;
                    }

                case 'X':
                    {
                        //$X0,0:#1e
                        ack();
                        char buf[8];

                        // read addr
                        for (auto j = 0; j < 8; j++) 
                        {
                            if(data[i + 1] == ',' ){
                                buf[j] = '\0';
                                ++i;
                                break;
                            }
                            buf[j] = data[++i];
                        }
                        std::string addr(buf);

                        // read length
                        for (auto j = 0;; j++) 
                        {
                            if(data[i+1] == ':') 
                            {
                                buf[j] = '\0';
                                ++i;
                                break;
                            }
                            buf[j] = data[++i];
                        }
                        std::string length(buf);

                        int len = std::stoi(length, nullptr, 16);
                        for(auto j = 0; j < len; ++j)
                        {
                            cpu_->memory[std::stoi(addr, nullptr, 16) + j] = (std::uint8_t)data[++i];

                        }

                        reply("OK");
                        break;
                    }

                default:
                    {
                        //TODO
                        assert(false);
                        break;
                    }
            }
        }

        auto gdb_server::interrupt(::narcissus::h8_3069f::interrupts int_num) -> bool
        {
            return cpu_->interrupt(int_num);
        }
    } // namespace h8_3069f

} // namespace narcissus

