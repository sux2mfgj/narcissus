#include <iostream>
#include <fstream>
#include <memory>


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
//     cout <<endl;
    while (!file.eof()) {
        file.read(buf, sizeof(buf));
        for(auto j = 0; j < file.gcount(); j++){
            mem[i++] = buf[j];
//             cout <<  buf[j] << flush;
        }
    }

    file.close();

    
//     mem[0] = 0x00;
//     mem[1] = 0x00;
//     mem[2] = 0x01;
//     mem[3] = 0x00;

    // ADD.B #0xff, r2l
//     mem[0x100] = 0x8a;
//     mem[0x101] = 0xff;
//     cout << hex << (uint16_t)mem[0] << (uint16_t)mem[1] 
//         << (uint16_t)mem[2] << (uint16_t)mem[3] << endl;

    auto cpu = std::make_shared<cpu::h8_300>(move(mem));
    cpu::cpu_debuger debug(cpu);
    cpu->reset_exception();

    while (true) {
        auto pc = cpu->cycle();
        std::cout << std::hex << "pc: 0x" << pc << std::endl;
    }
}
