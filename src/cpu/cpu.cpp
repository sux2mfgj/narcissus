#include <cpu.hpp>

namespace narcissus {
    namespace cpu {

        h8_300::h8_300(std::array<uint8_t, ROM_SIZE>&& mem) : er(), ccr(), rom()
        {
            rom = move(mem);
        }

        void h8_300::reset_exception()
        {
            // load pc from rom[0] ~ rom[3]
            auto reset_addr = uint32_t(rom[0]) << 24;
            reset_addr |= uint32_t(rom[1]) << 16;
            reset_addr |= uint32_t(rom[2]) << 8;
            reset_addr |= uint32_t(rom[3]);

            er[7].er32 = reset_addr;

            ccr.byte = 0b00000000;
            ccr.interrupt_mask = 1;
        }

        bool h8_300::cycle()
        {
            switch (detect_operation()) {
                case ADD_B_IMM: {
                                    auto rd = rom[er[7].er32] & 0x0f;
                                    auto imm = rom[er[7].er32 + 1];
                                    if(!register_write(rd, imm, register_size::BYTE)){
                                        return false;
                                    }

                                    er[7].er32 += 2;
                                    break;
                                }
                case INVALID:
                                return false;
            }

            return true;
        }

        operation h8_300::detect_operation()
        {
            auto op = rom[er[7].er32];

            auto ah = op >> 4;
            auto al = op & 0x0f;

            switch (ah) {
                case 8:
                    return operation::ADD_B_IMM;
                    break;

                default:
                    return operation::INVALID;
            }
        }

        bool h8_300::register_write(int destination, uint32_t immediate, register_size size)
        {


            switch (size) {
                case BYTE:

                    if(immediate > 0xff) {
                        return false;
                    }
                    // high
                    if ((destination & 0x8) != 0x8) {
                        er[destination & 0x7].h = immediate;
                    }
                    // low
                    else {
                        er[destination & 0x7].l = immediate;
                    }
                    break;

                case WORD:
                    //TODO
                    break;
                case LONG:
                    //TODO
                    break;
            }
            return true;
        }

    }  // namespace cpu
}  // namespace narcissus
