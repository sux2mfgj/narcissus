#include <cpu.hpp>

namespace narcissus {
    namespace cpu {

        h8_300::h8_300(std::array<std::uint8_t, ROM_SIZE>&& mem) 
            : er(), sp(), ccr(), pc(), memory(move(mem))
        {
//             rom = move(mem);
        }

        void h8_300::reset_exception()
        {
            // load pc from memory[0] ~ memory[3]
            auto reset_addr = std::uint32_t(memory[0]) << 24;
            reset_addr |= std::uint32_t(memory[1]) << 16;
            reset_addr |= std::uint32_t(memory[2]) << 8;
            reset_addr |= std::uint32_t(memory[3]);

            pc = reset_addr;

            ccr.byte = 0b00000000;
            ccr.interrupt_mask = 1;
        }

        bool h8_300::cycle()
        {
            switch (detect_operation()) {
                case ADD_B_IMM: {
                    auto rd = memory[pc] & 0x0f;
                    auto imm = memory[pc + 1];
                    if (!register_write_immediate(rd, imm, register_size::BYTE)) {
                        return false;
                    }

                    pc += 2;
                    break;
                }
                case ADD_B_R_R:
                {
                    auto rs = (memory[pc + 1] & 0xf0) >> 4;
                    auto rd = memory[pc + 1] & 0x0f;

                    if(!register_write_register(rd, rs, register_size::BYTE)){
                        return false;
                    }

                    pc += 2;
                    break;        
                }

                case ADD_W_IMM:
                {
                    auto rd = (memory[pc + 1] & 0xf);
                    std::uint16_t imm = std::uint16_t(memory[pc + 2]) << 8;
                    imm |= std::uint16_t(memory[pc + 3]);

                    if(!register_write_immediate(rd, imm, register_size::WORD))
                    {
                        return false;
                    }

                    pc += 4;
                    break;
                }

                case ADD_W_R_R:
                {
                    auto rs = (memory[pc + 1] & 0xf0) >> 4;
                    auto rd = (memory[pc + 1] & 0x0f);

                    if(!register_write_register(rd, rs, register_size::WORD)){
                        return false;
                    }

                    pc += 2;

                    break;
                }

                case ADD_L_IMM:
                {
                    std::uint8_t erd = memory[pc + 1] & 0x7;
                    auto imm = std::uint32_t(memory[pc + 2]) << 24;
                    imm |= std::uint32_t(memory[pc + 3]) << 16;
                    imm |= std::uint32_t(memory[pc + 4]) << 8;
                    imm |= std::uint32_t(memory[pc + 5]);

                    if(!register_write_immediate(std::uint8_t(erd), imm, register_size::LONG)){
                        return false;
                    }

                    pc += 6;
                    break;
                }

                case ADD_L_R_R:
                {
                    auto ers = (memory[pc + 1] & 0x70) >> 8;
                    auto erd = memory[pc + 1] & 0x07;
                    
                    if(!register_write_register(erd, ers, register_size::LONG)){
                        return false;
                    }

                    pc += 2;
                    break;
                }

                case MOV_B_IMM:
                {
                    auto rd = memory[pc] & 0x0f;
                    auto imm = memory[pc + 1];

                    if(!register_write_immediate(rd, imm, register_size::BYTE)){
                        return false;
                    }
                    pc += 2;

                    break;
                }

                case MOV_L_IMM:
                {
                    auto erd = memory[pc + 1] & 0x7;
                    auto imm = std::uint32_t(memory[pc + 2]) << 24;
                    imm |= std::uint32_t(memory[pc + 3]) << 16;
                    imm |= std::uint32_t(memory[pc + 4]) << 8;
                    imm |= std::uint32_t(memory[pc + 5]);

//                     std::cout << imm << std::endl;
                    if(!register_write_immediate(erd, imm, register_size::LONG)){
                        return false;
                    }
                    pc += 6;
                    return true;
                }

                //TODO yet complete below(have to mcu first than this case)
//                 case JSR_ABS:
//                 {
//                     auto abs = std::uint32_t(memory[pc + 1]) << 16;
//                     abs |= std::uint32_t(memory[pc + 2]) << 8;
//                     abs |= std::uint32_t(memory[pc + 3]);

                    //TODO [WIP] setup stack
//                     memory[--sp] = std::uint8_t(pc & 0x0000ff);
//                     memory[--sp] = std::uint8_t((pc >> 8) & 0x00ff00);
//                     memory[--sp] = std::uint8_t((pc >> 16) & 0xff0000);

//                     pc = abs;
//                     return true;
//                 }


                case INVALID:

                    std::cout << "INVALID opecode: " << std::hex << "0x" << std::flush;
                    std::cout << std::setw(2) << std::setfill('0') 
                        << (std::uint16_t)(memory[pc]) << std::flush;
                    std::cout << std::setw(2) << std::setfill('0') 
                        << (std::uint16_t)(memory[pc+1]) << std::endl;

                    std::cout << "pc             : 0x" << std::setw(8) << std::setfill('0')
                        << (std::uint32_t)pc << std::endl;

                    return false;
            }

            return true;
        }

        operation h8_300::detect_operation()
        {
            std::uint8_t op = memory[pc];

            auto ah = op >> 4;
            auto al = op & 0x0f;

            auto op2 = memory[pc + 1];
            auto bh = op2 >> 4;
            auto bl = op2 & 0x0f;

            switch (ah) { 

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

                case 5:
                    switch (al) {
                        case 0xe:
//                             return operation::JSR_ABS;

                        default:
                            return operation::INVALID;
                    }

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
                                case 0:
                                    return operation::MOV_L_IMM;
                                case 1:
                                    return operation::ADD_L_IMM;
                                default:
                                    return INVALID;
                            }
                        default:
                            return INVALID;
                    }

                case 8:
                    return operation::ADD_B_IMM;

                case 0xf:
                    return operation::MOV_B_IMM;

                default:
                    return operation::INVALID;
            }
        }

        bool h8_300::register_write_immediate(std::uint8_t destination,
                std::uint32_t immediate,
                register_size size)
        {

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

        bool h8_300::register_write_register(std::uint8_t destination,
                std::uint8_t source,
                register_size size)
        {
            if(destination > 0xf || source > 0xf) {
                return false;
            }

            auto tmp = er[source & 0x7].read(source, size);
            er[destination & 0x7].write(destination, tmp, size);

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
