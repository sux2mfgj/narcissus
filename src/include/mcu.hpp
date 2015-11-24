#pragma once 

#include <cstdint>
#include <array>
#include <memory>
#include <thread>
#include <condition_variable>
#include <mutex>

#include <sci.hpp>

namespace narcissus {
    namespace h8_3069f {

        class cpu;
        class sci;

        enum class mem_info : std::uint32_t
        {  
            rom_base_addr = 0x000000,
            rom_end_addr  = 0x080000,
            rom_size = rom_end_addr - rom_base_addr,

//             mode 5 
            ram_base_addr = 0xffbf20,
            ram_end_addr = 0xffff1f,
            ram_size = ram_end_addr - ram_base_addr,
            sci0_base_addr = 0xffffb0,
            sci1_base_addr = 0xffffb8,
            sci2_base_addr = 0xffffc0,
        };

        class mcu {
           
            public:
                mcu(std::array<std::uint8_t, (std::uint32_t)mem_info::rom_size>&& init_rom,
                        std::shared_ptr<std::condition_variable> c_variable_ptr,
                        std::shared_ptr<bool> is_sleep, std::shared_ptr<std::mutex> m);
                virtual ~mcu() = default;
               
            public:
                auto operator[] (std::uint32_t address) -> std::uint8_t&;
                auto before_run(std::shared_ptr<cpu>) -> void;

            private:
                std::array<std::uint8_t, (std::uint32_t)mem_info::rom_size> rom;
                std::array<std::uint8_t, (std::uint32_t)mem_info::ram_size> ram;
                std::shared_ptr<sci> sci_1;
        };

    } // namespace h8_3069f
} // namespace narcissus
