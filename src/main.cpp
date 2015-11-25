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

    try {
            
        // program_options
        po::options_description desc("Allowd options");
        desc.add_options()
            ("help", "print this menu")
        ;

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if(vm.count("help"))
        {
            std::cout << desc << std::endl;
            return 0;
        }
    } catch (std::exception& e) 
    {
        std::cerr << "error: \n\t"
            << e.what() << std::endl;
        return 1;
    }


    std::fstream file;
    char buf[16];

    std::array<uint8_t, (std::uint32_t)h8_3069f::mem_info::rom_size> mem = {0};

    if(argc < 2){
        return -1;
    }
    file.open(argv[1], std::ios::in | std::ios::binary);

    auto i = 0;
    while (!file.eof()) {
        file.read(buf, sizeof(buf));
        for(auto j = 0; j < file.gcount(); j++){
            mem[i++] = buf[j];
        }
    }
    file.close();

    auto cpu = h8_3069f::cpu::create(move(mem));
    h8_3069f::cpu_debuger debug(cpu);
    cpu->interrupt(h8_3069f::interrupts::reset);

    cpu->run();
    return 0;
}
