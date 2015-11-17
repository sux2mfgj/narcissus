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
    while (!file.eof()) {
        file.read(buf, sizeof(buf));
        for(auto j = 0; j < file.gcount(); j++){
            mem[i++] = buf[j];
        }
    }
    file.close();

    auto cpu = std::make_shared<cpu::h8_300>(move(mem));
    cpu::cpu_debuger debug(cpu);
    cpu->interrupt(cpu::interrupts::reset);

    cpu->run();
    return 0;
}
