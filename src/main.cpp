#include <iostream>
#include <fstream>

#include <cpu.hpp>

int main(int argc, char const* argv[])
{
    using namespace narcissus;
    using namespace std;


    std::fstream file;
    char buf[16];
    array<uint8_t, cpu::ROM_SIZE> mem = {0};

    file.open("./binary/kzload.bin", std::ios::in | std::ios::binary);

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
//     std::cout << std::hex << (uint16_t)mem[0] << (uint16_t)mem[1] 
//         << (uint16_t)mem[2] << (uint16_t)mem[3] << std::endl;

    cpu::h8_300 cpu(move(mem));
    cpu.reset_exception();

    if(!cpu.cycle()){
        std::cout << "cycle failed" << std::endl;
    }
    
    return 0;
}
