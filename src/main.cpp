#include <iostream>
#include <fstream>
#include <memory>

#include <boost/program_options.hpp>

#include <cpu.hpp>
#include <debug.hpp>

int main(int argc, char const* argv[])
{
    using namespace narcissus;
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
    std::array<uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};

    file.open(vm["image"].as<std::string>(), std::ios::in | std::ios::binary);

    auto i = 0;
    while (!file.eof()) {
        mem[i++] = (std::uint8_t)file.get();
    }
    file.close();

    auto cpu = h8_3069f::cpu::create(move(mem));
    h8_3069f::cpu_debuger debug(cpu);
    cpu->interrupt(h8_3069f::interrupts::reset);

    cpu->run();
    return 0;
}
