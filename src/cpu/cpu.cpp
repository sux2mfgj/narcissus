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
                    if (!register_write_immediate(rd, imm, register_size::BYTE)) {
                        return false;
                    }

                    er[7].er32 += 2;
                    break;
                }
                case ADD_B_R_R:
                {
                    auto rs = (rom[er[7].er32 + 1] & 0xf0) >> 4;
                    auto rd = rom[er[7].er32 + 1] & 0x0f;

                    if(!register_write_register(rd, rs, register_size::BYTE)){
                        return false;
                    }

                    er[7].er32 += 2;
                    break;        
                }

                case ADD_W_IMM:
                {
                    auto rd = (rom[PC + 1] & 0xf);
                    uint16_t imm = uint16_t(rom[PC + 2]) << 8;
                    imm |= uint16_t(rom[PC + 3]);

                    if(!register_write_immediate(rd, imm, register_size::WORD))
                    {
                        return false;
                    }

                    PC += 4;
                    break;
                }

                case ADD_W_R_R:
                {
                    auto rs = (rom[PC + 1] & 0xf0) >> 4;
                    auto rd = (rom[PC + 1] & 0x0f);

                    if(!register_write_register(rd, rs, register_size::WORD)){
                        return false;
                    }

                    PC += 2;

                    break;
                }

                case ADD_L_IMM:
                {
                    uint8_t erd = rom[PC + 1] & 0x7;
                    auto imm = uint32_t(rom[PC + 2]) << 24;
                    imm |= uint32_t(rom[PC + 3]) << 16;
                    imm |= uint32_t(rom[PC + 4]) << 8;
                    imm |= uint32_t(rom[PC + 5]);

                    //XXX
                    std::cout << erd << ":" <<imm << std::endl;

                    if(!register_write_immediate(uint8_t(erd), imm, register_size::LONG)){
                        return false;
                    }

                    PC += 6;
                    break;
                }

                case ADD_L_R_R:
                {
                    auto ers = (rom[PC + 1] & 0x70) >> 8;
                    auto erd = rom[PC + 1] & 0x07;
                    
                    std::cout << "ers" << ers << ":" << erd << std::endl;
                    if(!register_write_register(erd, ers, register_size::LONG)){
                        return false;
                    }

                    PC += 2;
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

            auto op2 = rom[er[7].er32 + 1];
            auto bh = op2 >> 4;
            auto bl = op2 & 0x0f;

            switch (ah) {
                case 8:
                    return operation::ADD_B_IMM;

                case 7:
                    switch(al) {
                        case 9:
                            switch (bh) {
                                case 1:
                                    return operation::ADD_W_IMM;

                                default:
                                    return INVALID;
                            }
                            
                        case 0xa:
                            switch(bh) {
                                case 1:
                                    return operation::ADD_L_IMM;
                                default:
                                    return INVALID;
                            }
                        default:
                            return INVALID;
                    }

                case 0:
                    switch (al) {
                        case 8:
                            return operation::ADD_B_R_R;
                        case 9:
                            return operation::ADD_W_R_R;
                        case 0xa:
                            switch (bh) {
                                case 0x8:
                                case 0x9:
                                case 0xa:
                                case 0xb:
                                case 0xc:
                                case 0xd:
                                case 0xe:
                                case 0xf:
                                    return operation::ADD_L_R_R;

                                default:
                                    return operation::INVALID;
                            }
                        default:
                            return operation::INVALID;
                    }

                default:
                    return operation::INVALID;
            }
        }

        bool h8_300::register_write_immediate(uint8_t destination,
                uint32_t immediate,
                register_size size)
        {

            //XXX
            std::cout << uint8_t(destination) << " : " << immediate << std::endl;
            switch (size) {
                case BYTE:
                    if (immediate > 0xff) {
                        return false;
                    }
                    break;

                case WORD:
                    if(immediate > 0xffff){
                        return false;
                    }
                    break;
                case LONG:
                    break;
            }
            er[destination & 0x7].write(destination, immediate, size);
            return true;
        }

        bool h8_300::register_write_register(uint8_t destination,
                uint8_t source,
                register_size size)
        {
            if(destination > 0xf || source > 0xf) {
                return false;
            }

            auto tmp = er[source & 0x7].read(source, size);
            er[destination & 0x7].write(destination, tmp, size);
            //XXX
            std::cout << destination << ":" << source << ":" << uint16_t(tmp) << std::endl;

//             switch (size) {
//                 case register_size::BYTE: 
//                     {

//                         auto tmp = er[source & 0x7].read(source, register_size::BYTE);
//                         er[destination & 0x7].write(destination, tmp, register_size::BYTE);
//                         break;
//                     }
//                 case register_size::WORD:
//                     {
//                         uint16_t tmp = er[source & 0x7].read(source, register_size::WORD);
//                         er[destination & 0x7].write(destination, tmp, register_size::BYTE) ;
//                         
//                         break;
//                     }
//                 case register_size::LONG:
//                     break;

//             }
            return true;
        }


    }  // namespace cpu
}  // namespace narcissus
