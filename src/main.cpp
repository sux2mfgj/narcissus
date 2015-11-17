#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>

#include <cpu.hpp>
#include <debug.hpp>


int main(int argc, char const* argv[])
{
    using namespace narcissus;
    using namespace std;

    fstream file;
    char buf[16];

    array<uint8_t, cpu::ROM_SIZE> mem = {0};

    if(argc < 2){
        return -1;
    }
    file.open(argv[1], ios::in | ios::binary);

    auto i = 0;
    while (!file.eof()) {
        file.read(buf, sizeof(buf));
        for(auto j = 0; j < file.gcount(); j++){
            mem[i++] = buf[j];
        }
    }
    file.close();

    auto cpu = std::make_shared<cpu::h8_300>(move(mem));
    cpu::cpu_debuger debug(cpu);
    cpu->reset_exception();

    cpu->run();
//     auto before_pc = 0;
//     auto stop_count = 0;

//     while (true) {
//         auto pc = cpu->cycle();
//         std::clog << std::hex << "pc: 0x" << pc << std::endl;
//         if(before_pc != pc){
//             before_pc = pc;
//             stop_count = 0;            
//         }
//         else {
//             if(stop_count++ > 10)
//             {
//                 std::clog << "finish" << std::endl;
//                 break;
//             }
//         }
//     }
//     
    return 0;
}
