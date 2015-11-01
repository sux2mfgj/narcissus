#include <iostream>

#include <cpu.hpp>

int main(int argc, char const* argv[])
{
    using namespace narcissus;
    using namespace std;

    array<uint8_t, cpu::ROM_SIZE> mem;


    cpu::h8_300 cpu(move(mem));
    cpu.reset_exception();
    
    return 0;
}
