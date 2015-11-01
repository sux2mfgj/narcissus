#include <cpu.hpp>

namespace narcissus {
    namespace cpu {

        h8_300::h8_300(std::array<std::uint8_t, ROM_SIZE>&& mem) : er(), ccr(), rom(move(mem))
        {
//             rom = move(mem);
        }

        void h8_300::reset_exception()
        {
            // load pc from rom[0] ~ rom[3]
            auto reset_addr = std::uint32_t(rom[0]) << 24;
            reset_addr |= std::uint32_t(rom[1]) << 16;
            reset_addr |= std::uint32_t(rom[2]) << 8;
            reset_addr |= std::uint32_t(rom[3]);

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
                    std::uint16_t imm = std::uint16_t(rom[PC + 2]) << 8;
                    imm |= std::uint16_t(rom[PC + 3]);

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
                    std::uint8_t erd = rom[PC + 1] & 0x7;
                    auto imm = std::uint32_t(rom[PC + 2]) << 24;
                    imm |= std::uint32_t(rom[PC + 3]) << 16;
                    imm |= std::uint32_t(rom[PC + 4]) << 8;
                    imm |= std::uint32_t(rom[PC + 5]);

                    if(!register_write_immediate(std::uint8_t(erd), imm, register_size::LONG)){
                        return false;
                    }

                    PC += 6;
                    break;
                }

                case ADD_L_R_R:
                {
                    auto ers = (rom[PC + 1] & 0x70) >> 8;
                    auto erd = rom[PC + 1] & 0x07;
                    
                    if(!register_write_register(erd, ers, register_size::LONG)){
                        return false;
                    }

                    PC += 2;
                    break;
                }

                case MOV_B_IMM:
                {
                    auto rd = rom[PC] & 0x0f;
                    auto imm = rom[PC + 1];

                    if(!register_write_immediate(rd, imm, register_size::BYTE)){
                        return false;
                    }
                    PC += 2;

                    break;
                }

                case INVALID:

                    std::cout << "INVALID opecode: " << std::hex << "0x" << std::flush;
                    std::cout << std::setw(2) << std::setfill('0') 
                        << (std::uint16_t)(rom[PC]) << std::flush;
                    std::cout << std::setw(2) << std::setfill('0') 
                        << (std::uint16_t)(rom[PC+1]) << std::endl;

                    std::cout << "0x" << std::setw(8) << std::setfill('0')
                        << (std::uint32_t)PC << std::endl;

                    return false;
            }

            return true;
        }

        operation h8_300::detect_operation()
        {
            std::uint8_t op = rom[PC];

            auto ah = op >> 4;
            auto al = op & 0x0f;

            auto op2 = rom[er[7].er32 + 1];
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
