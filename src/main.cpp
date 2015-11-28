#include <iostream>

#include <fstream>
#include <memory>
#include <cstdbool>

#include <boost/program_options.hpp>
#include <boost/asio.hpp>

#include <narcissus.hpp>
#include <cpu.hpp>
#include <gdb.hpp>

int main(int argc, char const* argv[])
{
    namespace na = narcissus;
    namespace po = boost::program_options;

    po::variables_map vm;
    // program_options
    auto error_message("error:\n\t");
    try 
    {
        po::options_description desc("Allowd options");
        desc.add_options()
            ("help,h", "print this menu")
            ("image,i", po::value<std::string>(), "run image to <file_path>")
            ("debug,d", "enable debug mode")
            ("port,p", po::value<std::uint16_t>()->default_value(2159), "for remote debug port")
//             ("version,v", "narcissus v0.01")
        ;

        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if(vm.count("help"))
        {
            std::cout << desc << std::endl;
            return 0;
        }

        if(!vm.count("image"))
        {
            std::cerr << error_message << "you should set --image options" << std::endl;
            return 1;
        }
    } 
    catch (std::exception& e) 
    {
        std::cerr << error_message << e.what() << std::endl;
        return 1;
    }

    std::fstream file;
    std::array<uint8_t, (std::uint32_t)na::h8_3069f::mem_info::rom_size> mem = {0};

    file.open(vm["image"].as<std::string>(), std::ios::in | std::ios::binary);

    {
        auto i = 0;
        while (!file.eof()) {
            mem[i++] = (std::uint8_t)file.get();
        }
    }
    file.close();

    // for signal handler and server
    boost::asio::io_service io_service;

    // for gdb 
    std::shared_ptr<na::narcissus> nar;
    if(vm.count("debug"))
    {
        std::clog << "debug mode start" << std::endl; 
        nar = std::make_shared<na::h8_3069f::gdb_server>(std::move(mem),
                io_service, vm["port"].as<std::uint16_t>());
    }
    else {
        nar = std::make_shared<na::h8_3069f::cpu>(std::move(mem));
    }

    nar->interrupt(na::h8_3069f::interrupts::reset);
    nar->run();

    return 0;
}
