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

        auto is_debug_mode = false;
//     auto is_debug_mode = true;

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

    // setup break point
    std::uint32_t bp;
    std::vector<std::uint32_t> break_points;
    while (is_debug_mode) {
        std::cout << "break point> 0x";
        std::cin >> std::hex >> bp;
        if(bp < 0x100){
            break;
        }
        break_points.push_back(bp);
    }
    sort(break_points.begin(), break_points.end());;

    std::cout << "start" << std::endl;

    std::uint32_t before_pc;
    std::uint8_t stop_count = 0;

    std::string s = "c";
    std::string before = s;
    while (true) {
        auto pc = cpu->cycle();
        if(is_debug_mode){

            std::cout << std::hex << "pc: 0x" << pc << std::endl;
            auto t = break_points.end() != find(break_points.begin(), break_points.end(), pc);
            if((t || s[0] != 's') && is_debug_mode){
                std::cout << ">";
                std::getline(std::cin, s);

                if(s.empty()){
                    s = before;
                }

                if(s[0] == 'p'){
                    std::cout << debug << std::endl;
                }
                if(s[0] == 'q'){
                    break;
                }
                before = s;
            }

        }
            if(before_pc != pc){
                before_pc = pc;
                stop_count = 0;
            }
            else {
                stop_count++;
                if(stop_count > 10){
                    std::cout << "finish" << std::endl; 
                    break;
                }
            }
    }

    //     cpu->closing();

    //command:
    //  - [c] continue
    //  - [p] print registers
    //  - [q] quit

    return 0;
}
