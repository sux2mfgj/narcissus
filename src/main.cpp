#include <iostream>

#include <cpu.hpp>

int main(int argc, char const* argv[])
{
    using namespace narcissus;

    cpu::h8_300 cpu;
    cpu.reset_exception();
    
    return 0;
}
