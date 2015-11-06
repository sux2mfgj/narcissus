#include <cpu.hpp>
#include <string>

namespace narcissus {
    namespace cpu {

        h8_300::h8_300(std::array<std::uint8_t, ROM_SIZE>&& mem)
            : er(), sp(), ccr(), pc(), memory(move(mem))
        {
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
            std::cout << "pc : 0x" << std::hex << pc << std::endl;
            std::cout << "sp : 0x" << std::hex << sp << std::endl;
//             if(pc == 0x1c8){
//                 std::string s;
//                 std::cin >> s;
//             }

            switch (detect_operation()) {
                //                 case ADD_B_IMM: {
                //                     auto rd = memory[pc] & 0x0f;
                //                     auto imm = memory[pc + 1];
                //                     if (!register_write_immediate(rd, imm,
                //                     register_size::BYTE)) {
                //                         return false;
                //                     }

                //                     pc += 2;
                //                     break;
                //                 }

                //                 case ADD_B_R_R:
                //                 {
                //                     auto rs = (memory[pc + 1] & 0xf0) >> 4;
                //                     auto rd = memory[pc + 1] & 0x0f;

                //                     if(!register_write_register(rd, rs,
                //                     register_size::BYTE)){
                //                         return false;
                //                     }

                //                     pc += 2;
                //                     break;
                //                 }

                //                 case ADD_W_IMM:
                //                 {
                //                     auto rd = (memory[pc + 1] & 0xf);
                //                     std::uint16_t imm = std::uint16_t(memory[pc + 2])
                //                     << 8;
                //                     imm |= std::uint16_t(memory[pc + 3]);

                //                     if(!register_write_immediate(rd, imm,
                //                     register_size::WORD))
                //                     {
                //                         return false;
                //                     }

                //                     pc += 4;
                //                     break;
                //                 }

                //                 case ADD_W_R_R:
                //                 {
                //                     auto rs = (memory[pc + 1] & 0xf0) >> 4;
                //                     auto rd = (memory[pc + 1] & 0x0f);

                //                     if(!register_write_register(rd, rs,
                //                     register_size::WORD)){
                //                         return false;
                //                     }

                //                     pc += 2;

                //                     break;
                //                 }

                //                 case ADD_L_IMM:
                //                 {
                //                     std::uint8_t erd = memory[pc + 1] & 0x7;
                //                     auto imm = std::uint32_t(memory[pc + 2]) << 24;
                //                     imm |= std::uint32_t(memory[pc + 3]) << 16;
                //                     imm |= std::uint32_t(memory[pc + 4]) << 8;
                //                     imm |= std::uint32_t(memory[pc + 5]);

                //                     if(!register_write_immediate(std::uint8_t(erd),
                //                     imm, register_size::LONG)){
                //                         return false;
                //                     }

                //                     pc += 6;
                //                     break;
                //                 }

                //                 case ADD_L_R_R:
                //                 {
                //                     auto ers = (memory[pc + 1] & 0x70) >> 8;
                //                     auto erd = memory[pc + 1] & 0x07;

                //                     if(!register_write_register(erd, ers,
                //                     register_size::LONG)){
                //                         return false;
                //                     }

                //                     pc += 2;
                //                     break;
                //                 }

                case ADDS_4:
                {
                    auto erd = memory[pc + 1] & 0x7;

                    auto result = er[erd].er32 + 4;

                    if (!register_write_immediate(erd, result, register_size::LONG)) {
                        return false;
                    }

                    pc += 2;
                    return true;
                }

                case ADD_B_IMM_R:
                {
                    auto rd = memory[pc] & 0xf;
                    auto imm = memory[pc + 1];

                    auto rd_value = er[rd & 0x7].read(rd, register_size::BYTE);
                    //TODO imm have to cast by integer?
                    auto result = rd_value + imm;

                    if(!register_write_immediate(rd, result, register_size::BYTE)){
                        return false;
                    }

                    update_ccr_sub(rd_value, imm, result, register_size::BYTE);
                    pc += 2;

                    return true;
                }

                case ADD_L_IMM_R:
                {
                    auto erd = memory[pc + 1] & 0x7;
                    auto imm = (std::uint32_t)memory[pc + 2] << 24;
                    imm |= (std::uint32_t)memory[pc + 3] << 16;
                    imm |= (std::uint32_t)memory[pc + 4] << 8;
                    imm |= (std::uint32_t)memory[pc + 5];

                    auto erd_value = er[erd].er32;
                    auto result = (std::int32_t)erd_value + (std::int32_t)imm;
                    std::cout << erd_value << ":" 
                        << imm << std::endl;

                    if(!register_write_immediate(erd, result, register_size::LONG)){
                        return false;
                    }

                    update_ccr_sub(erd_value, imm, result, register_size::LONG);
                    pc += 6;
                    return true;
                }

                case SUB_B_R_R: 
                {
                    auto rs = (memory[pc + 1]) >> 4;
                    auto rd = (memory[pc + 1]) & 0xf;

                    auto src_value = er[rs & 0x7].read(rs, register_size::BYTE);
                    auto dest_value = er[rd & 0x7].read(rd, register_size::BYTE);
//                         std::cout << dest_value << ":" << src_value << std::endl;
                    auto imm = dest_value - src_value;

                    if (!register_write_immediate(rd, imm, register_size::BYTE)) {
                        return false;
                    }

                    update_ccr_sub(src_value, dest_value, imm, register_size::BYTE);

                    pc += 2;
                    break;
                }

                case SUB_W_R_R: 
                {
                    auto rs = memory[pc + 1] >> 0x4;
                    auto rd = memory[pc + 1] & 0xf;

                    auto src_value = er[rs & 0x7].read(rs, register_size::WORD);
                    auto dest_value = er[rd & 0x7].read(rd, register_size::WORD);

//                         std::cout << src_value << ":" << dest_value << std::endl;
//                         std::cout << rs << ":" << rd << std::endl;
                    //                     dest_value -= src_value;
                    auto result = dest_value - src_value;
                    if (!register_write_immediate(rd, result, register_size::WORD)) {
                        return false;
                    }
                    update_ccr_sub(src_value, dest_value, result, register_size::WORD);

                    pc += 2;
                    break;
                }

                case SUB_L_R_R:
                {
                    auto ers = (memory[pc + 1] >> 4) & 0x1;
                    auto erd = memory[pc + 1] & 0x7;

                    auto ers_value = er[ers].er32;
                    auto erd_value = er[erd].er32;

                    auto result = erd_value - ers_value;
                    if (!register_write_immediate(erd, result, register_size::LONG)) {
                        return false;
                    }

                    update_ccr_sub(ers_value, erd_value, result, register_size::LONG);

                    pc += 2;
                    break;
                }

                case SUB_WITH_SIGN_EXT_4:
                {
                    auto erd = memory[pc + 1] & 0x7;;

                    er[erd].er32 -= 4;

                    pc += 2;
                    break;
                }

                case MOV_B_IMM: 
                {
                    auto rd = memory[pc] & 0x0f;
                    auto imm = memory[pc + 1];

                    if (!register_write_immediate(rd, imm, register_size::BYTE)) {
                        return false;
                    }

                    update_ccr_mov(imm, register_size::BYTE);
                    pc += 2;
                    break;
                }

                case MOV_B_R_R:
                {
                    auto rs = memory[pc + 1] >> 4;
                    auto rd = memory[pc + 1] & 0xf;

                    auto result = er[rs & 0x7].read(rs, register_size::BYTE);
                    if(!register_write_immediate(rd, result, register_size::BYTE)){
                        return false;
                    }

                    update_ccr_mov(result, register_size::BYTE);
                    pc += 2;
                    break;
                }

                case MOV_B_R_IND: 
                {
                    auto ers = (memory[pc + 1] & 0x70) >> 4;
                    auto rd = memory[pc + 1] & 0x0f;

                    auto src_reg_value = er[ers & 0x7].read(ers, register_size::LONG);
                    auto result = memory[src_reg_value];

                    if (!register_write_immediate(rd, result, register_size::BYTE)) {
                        return false;
                    }
                    update_ccr_mov(result, register_size::BYTE);
                    pc += 2;

//                         std::cout << ers << ":" << rd << ":" << (uint16_t)result << ":"
//                             << src_reg_value << std::endl;
                    break;
                }

                    //                 case MOV_B_R_IND:
                    //                 {
                    //                     auto erd = (memory[pc + 1] & 0x70) >> 4;
                    //                     auto rs = memory[pc + 1] & 0x8;

                    //                     auto src_value = er[rs & 0x7].read(rs,
                    //                     register_size::BYTE);
                    //                     auto dest_value = er[erd & 0x7].read(erd,
                    //                     register_size::LONG);
                    //                     memory[dest_value] = src_value;
                    //
                    //                     update_ccr_mov(src_value, register_size::BYTE);
                    //                     pc += 2;
                    //                     break;
                    //                 }

                case MOV_B_IND_WITH_DIS_16_R: 
                {
                    auto ers = (memory[pc + 1] >> 4) & 0x7;
                    auto rd = memory[pc + 1] & 0xf;
                    auto disp = (std::uint16_t)(memory[pc + 2]) << 8;
                    disp |= (std::uint16_t)memory[pc + 3];

                    auto src_value = er[ers].er32;
                    auto addr = src_value + (std::int16_t)disp;

                    auto result = memory[addr];

                    if (!register_write_immediate(rd, result, register_size::BYTE)) {
                        return false;
                    }

//                     std::cout << src_value << ":" << disp << std::endl;
//                     std::cout <<std::hex<< addr <<":"<< (std::uint16_t)result << std::endl;

                    update_ccr_mov(result, register_size::BYTE);
                    pc += 4;
                    break;
                }

                case MOV_B_R_IND_WITH_DIS_16:
                {

                    auto erd = (memory[pc + 1] >> 4) & 0x7;
                    auto rs = (memory[pc + 1]) & 0xf;
                    auto disp = (std::uint16_t)(memory[pc + 2]) << 8;
                    disp |= (std::uint16_t)(memory[pc + 3]);

                    auto src_value = er[rs & 0x7].read(rs, register_size::BYTE);
                    auto dest_addr = er[erd & 0x7].read(erd, register_size::LONG);

//                     std::cout <<  (std::uint16_t)(rs & 0x7) << ":" << erd << std::endl;
//                     std::cout << std::hex << "dest_addr: " << dest_addr << std::endl;

                    dest_addr += (std::int16_t)disp;
                    memory[dest_addr] = src_value;

                    update_ccr_mov(src_value, register_size::BYTE);
                    pc += 4;
                    break;
                }

                case MOV_B_R_IND_POST_INC:
                    {
                        auto ers = (memory[pc + 1] >> 4) & 0x7;
                        auto rd = (memory[pc + 1]) & 0xf;

                        auto result = memory[er[ers].er32];
                        if(!register_write_immediate(rd, result, register_size::BYTE)){
                            return false;
                        }
                        update_ccr_mov(result ,register_size::BYTE);

                        er[ers].er32 += 1;
                        pc += 2;
                        return true;
                    }


                case MOV_W_IMM: {
                                    auto rd = memory[pc + 1] & 0x7;
                                    auto imm = uint16_t(memory[pc + 2]) << 8;
                                    imm |= uint16_t(memory[pc + 3]);

                                    if (!register_write_immediate(rd, imm, register_size::WORD)) {
                                        return false;
                                    }

                                    update_ccr_mov(imm, register_size::WORD);
                                    pc += 4;
                                    break;
                                }

                case MOV_W_R_R:
                {
                    auto rs = memory[pc + 1] >> 4;
                    auto rd = memory[pc + 1] & 0xf;

                    if(!register_write_register(rd, rs, register_size::WORD)){
                        return false;
                    }

                    
//                     std::cout << "[mov.w rs, rd]" << std::endl;
//                     std::cout <<  "rs[" << rs << "]: " << er[rs & 0x7].read(rs, register_size::WORD)
//                         << " -> "
//                         << "rd[" << rd << "]: " << er[rd & 0x7].read(rd, register_size::WORD)
//                         << std::endl;

//                     std::cout <<  er[rs & 0x7].read(rs, register_size::LONG)<< std::endl;
//                     std::cout <<  er[rd & 0x7].read(rd, register_size::LONG)<< std::endl;

                    update_ccr_mov(er[rs & 0x7].read(rs, register_size::WORD), 
                            register_size::WORD);

                    pc += 2;
                    return true;
                }

                case MOV_L_IMM: {
                                    auto erd = memory[pc + 1] & 0x7;
                                    auto imm = std::uint32_t(memory[pc + 2]) << 24;
                                    imm |= std::uint32_t(memory[pc + 3]) << 16;
                                    imm |= std::uint32_t(memory[pc + 4]) << 8;
                                    imm |= std::uint32_t(memory[pc + 5]);

//                                     std::cout << imm << std::endl;
                                    if (!register_write_immediate(erd, imm, register_size::LONG)) {
                                        return false;
                                    }

                                    update_ccr_mov(imm, register_size::LONG);
                                    pc += 6;
                                    return true;
                                }

                case MOV_L_R_R: {
                                    auto ers = std::uint32_t(memory[pc + 1] & 0x70) >> 4;
                                    auto erd = std::uint32_t(memory[pc + 1] & 0x07);

                                    auto src_value = er[ers].er32;
                                    er[erd].er32 = src_value;

                                    update_ccr_mov(src_value, register_size::LONG);
                                    pc += 2;
                                    return true;
                                }

                case MOV_L_R_IND: {
                                      auto erd = (memory[pc + 3] & 0x70) >> 4;
                                      auto ers = (memory[pc + 3] & 0x07);

                                      auto erd_val = er[erd].er32;
                                      auto ers_val = er[ers].er32;

                                      erd_val -= 4;
                                      //
                                      memory[erd_val] = ers_val >> 24;
                                      memory[erd_val + 1] = ers_val >> 16;
                                      memory[erd_val + 2] = ers_val >> 8;
                                      memory[erd_val + 3] = ers_val;

                                      er[erd].er32 = erd_val;

                                      update_ccr_mov(ers_val, register_size::LONG);
                                      pc += 4;

                                      return true;
                                  }

                case MOV_L_IND_WITH_DIS_24_R: 
                {
                    auto ers = memory[pc + 3] >> 4;
                    auto erd = memory[pc + 5] & 0x7;

                    auto disp = std::uint32_t(memory[pc + 7]) << 16;
                    disp |= std::uint32_t(memory[pc + 8]) << 8;
                    disp |= std::uint32_t(memory[pc + 9]);

                    auto addr = (std::int32_t)(er[ers].er32) + (std::int32_t)disp;

//                     std::cout << std::hex 
//                         << "ers : " << ers << ", " 
//                         << "erd : " << erd << ", " << "disp : " << disp << ", "
//                         << (std::uint32_t)er[ers].er32 << std::endl;

                    auto dest_value = std::uint32_t(memory[addr]) << 24;
                    dest_value |= std::uint32_t(memory[addr + 1]) << 16;
                    dest_value |= std::uint32_t(memory[addr + 2]) << 8;
                    dest_value |= std::uint32_t(memory[addr + 3]);

//                     std::cout << "dest_value : " << dest_value << std::endl;

                    er[erd].er32 = dest_value;

                    update_ccr_mov(dest_value, register_size::LONG);
                    pc += 10;
                    return true;
                }

                case MOV_L_R_IND_POST_INC: {
                                               auto ers = memory[pc + 3] >> 4;
                                               auto erd = memory[pc + 3] & 0x07;

                                               auto source_addr = er[ers].er32;

                                               auto dest_value = memory[source_addr++] << 24;
                                               dest_value |= memory[source_addr++] << 16;
                                               dest_value |= memory[source_addr++] << 8;
                                               dest_value |= memory[source_addr++];

                                               er[erd].er32 = dest_value;
                                               er[ers].er32 = source_addr;
//                                                std::cout << erd << ":" << ers << std::endl;
//                                                std::cout << dest_value << ":" << source_addr << std::endl;

                                               update_ccr_mov(dest_value, register_size::LONG);
                                               pc += 4;

                                               return true;
                                           }

                case BEQ: 
                {
                    auto disp = memory[pc + 1];

                    pc += 2;
                    if (ccr.zero != 0x1) {
                        return true;
                    }

                    pc += (std::int8_t)disp;
                    return true;
                }

                case BRA: {
                              auto disp = (std::int8_t)memory[pc + 1];
                              pc += 2;
                              pc += disp;

                              return true;
                          }

                case BNE:
                {
                    auto disp = memory[pc + 1];
                    pc += 2;
                    if(ccr.zero == 1) {
                        pc += disp;
                    }
                    return true;
                }

                case CMP_B_IMM:
                {
                    auto rd = memory[pc] & 0xf;
                    auto imm = memory[pc + 1];

                    auto rd_value = er[rd & 0x7].read(rd, register_size::BYTE);
                    auto result = rd_value - imm;

                    update_ccr_sub(rd_value, imm, result, register_size::BYTE);
                    pc += 2;
                    return true;
                }

                case AND_W:
                {
                    auto rd = memory[pc + 1] & 0xf;
                    auto imm = (std::uint16_t)memory[pc + 2] << 8;
                    imm |= (std::uint16_t)memory[pc + 3];

                    auto rd_value = (std::uint16_t)er[rd & 0x7].read(rd, register_size::WORD);
                    auto result = rd_value & imm;
                    if(!register_write_immediate(rd, result, register_size::WORD)){
                        return false;
                    }

//                     std::cout << "and_w: " << result << std::endl;
                    update_ccr_mov(result, register_size::WORD);
                    pc += 4;
                    return true;
                }

                case AND_B_IMM:
                {
                    auto rd = memory[pc] & 0xf;
                    auto imm = memory[pc + 1];

                    auto rd_value = er[rd & 0x7].read(rd, register_size::BYTE);
                    auto result = rd_value & imm;

                    if (!register_write_immediate(rd, result, register_size::BYTE)) {
                        return false;
                    }
                    pc += 2;
                    update_ccr_mov(result, register_size::BYTE);
                    return true;
                }

                case JSR_ABS: {
                                  auto abs = std::uint32_t(memory[pc + 1]) << 16;
                                  abs |= std::uint32_t(memory[pc + 2]) << 8;
                                  abs |= std::uint32_t(memory[pc + 3]);

                                  pc += 4;

                                  memory[--sp] = (std::uint8_t)(pc & 0x0000ff);
                                  memory[--sp] = (std::uint8_t)((pc >> 8) & 0x0000ff);
                                  memory[--sp] = (std::uint8_t)((pc >> 16) & 0x0000ff);
                                  memory[--sp] = 0x00;

                                  pc = abs;
                                  return true;
                              }

                case EXTS_L: 
                {
                    auto erd = memory[pc + 1] & 0x7;
                    auto val = er[erd].er32;
                    uint32_t sign = (uint32_t)(val & 0x00008000);
                    sign |= sign << 1;  // 0x00018000;
                    sign |= sign << 1;  // 0x00038000;
                    sign |= sign << 2;  // 0x000f8000;
                    sign |= sign << 4;  // 0x00ff8000;
                    sign |= sign << 8;  // 0xffff8000;

//                     auto result = (val & 0x00008fff) | sign;
                    auto result = (val & 0xffff) | sign;
                    er[erd].er32 = result; //(val & 0x00008fff) | sign;

//                     std::cout << "[exts.l erd]" << std::endl;
//                     std::cout << std::hex << "0x" << val << " -> 0x" << result << std::endl;

                    update_ccr_mov(result, register_size::LONG);
                    pc += 2;
                    return true;
                }

                case SHLL_L: 
                {
                    auto erd = memory[pc + 1] & 0x7;
                    er[erd].er32 = er[erd].er32 << 1;
//                     std::cout << "shll: " << er[erd].er32 << std::endl;

                    update_ccr_shll(er[erd].er32, register_size::LONG);
                    pc += 2;
                    return true;
                }

                case RTS: {
                              // memory[sp]: is reserved
                              memory[sp++];
                              auto return_addr = (std::uint32_t)memory[sp++] << 16;
                              return_addr |= (std::uint32_t)memory[sp++] << 8;
                              return_addr |= (std::uint32_t)memory[sp++];

                              pc = return_addr;

                              return true;
                          }

                case operation::INVALID:
                          std::cout << "INVALID opecode: " << std::hex << "0x" << std::flush;
                          std::cout << std::setw(2) << std::setfill('0')
                              << (std::uint16_t)(memory[pc]) << std::flush;
                          std::cout << std::setw(2) << std::setfill('0')
                              << (std::uint16_t)(memory[pc + 1]) << std::endl;

                          std::cout << "pc             : 0x" << std::setw(8)
                              << std::setfill('0') << (std::uint32_t)pc << std::endl;

                          return false;

                default:
                          std::cout << "implement yet" << std::endl;
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

            auto op3 = memory[pc + 2];
            auto ch = op3 >> 4;
            auto cl = op3 & 0x0f;

            switch (ah) {
                case 0:
                    switch (al) {
                        case 1:
                            switch (bh) {
                                case 0:
                                    switch (bl) {
                                        case 0:
                                            switch (ch) {
                                                case 6:
                                                    switch (cl) {
                                                        case 0xd: {
                                                                      auto dh =
                                                                          memory[pc + 3] & 0x80;
                                                                      if (dh != 0x80) {
                                                                          return operation::
                                                                              MOV_L_R_IND_POST_INC;
                                                                      }
                                                                      return operation::
                                                                          MOV_L_R_IND;
                                                                  }
                                                        default:
                                                                  return operation::INVALID;
                                                    }
                                                case 7:
                                                    switch (cl) {
                                                        case 8: {
                                                                    auto t =
                                                                        memory[pc + 5] >> 4;
                                                                    switch (t) {
                                                                        case 0x2:
                                                                            return operation::
                                                                                MOV_L_IND_WITH_DIS_24_R;
                                                                        default:
                                                                            return operation::
                                                                                INVALID;
                                                                    }
                                                                }
                                                        default:
                                                                return operation::INVALID;
                                                    }
                                                default:
                                                    return operation::INVALID;
                                            }
                                        default:
                                            return operation::INVALID;
                                    }
                                default:
                                    return operation::INVALID;
                            }
                        case 8:
                            //                             return operation::ADD_B_R_R;
                            return operation::INVALID;
                        case 9:
                            //                             return operation::ADD_W_R_R;
                            return operation::INVALID;
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
                                    //                                     return
                                    //                                     operation::ADD_L_R_R;

                                default:
                                    return operation::INVALID;
                            }

                        case 0xb:
                            switch (bh) {
                                case 0x0:
//                                     return operation::ADDS_1;
                                case 0x8:
//                                     return operation::ADDS_2;
                                case 0x9:
                                    return operation::ADDS_4;

                                default:
                                    return operation::INVALID;
                            }

                        case 0xc:
                            return operation::MOV_B_R_R;

                        case 0xd:
                            return operation::MOV_W_R_R;

                        case 0xf:
                            return operation::MOV_L_R_R;

                        default:
                            return operation::INVALID;
                    }
                case 1:
                    switch (al) {
                        case 0:
                            
                            
                            switch (bh) {
                                case 3:
                                    return operation::SHLL_L;
                                default:
                                    return operation::INVALID;
                            }
                        case 7:
                            switch (bh) {
                                case 0xf:
                                    return operation::EXTS_L;

                                default:
                                    return operation::INVALID;
                            }
                        case 8:
                            return operation::SUB_B_R_R;

                        case 9:
                            return operation::SUB_W_R_R;

                        case 0xa:
                            if((bh >> 3) & 0x1){
                                return operation::SUB_L_R_R;
                            }
                            return operation::INVALID;

                        case 0xb:
                            switch (bh) {
                                case 0:
//                                     return operation::SUB_WITH_SIGN_EXT_1;
                                    return operation::INVALID;
                                case 8:
//                                     return operation::SUB_WITH_SIGN_EXT_2;
                                    return operation::INVALID;
                                case 9:
                                    return operation::SUB_WITH_SIGN_EXT_4;

                                default:
                                    return operation::INVALID;
                            }

                        default:
                            return operation::INVALID;
                    }

                case 4:
                    switch (al) {
                        case 0:
                            return operation::BRA;

                        case 6:
                            return operation::BNE;

                        case 7:
                            return operation::BEQ;

                        default:
                            return operation::INVALID;
                    }

                case 5:
                    switch (al) {
                        case 0x4:
                            switch (bh) {
                                case 0x7:
                                    switch (bl) {
                                        case 0x0:
                                            return operation::RTS;

                                        default:
                                            return operation::INVALID;
                                    }
                                default:
                                    return operation::INVALID;
                            }

                        case 0xe:
                            return operation::JSR_ABS;

                        default:
                            return operation::INVALID;
                    }

                case 6:
                    switch (al) {
                        case 0x8:
                            return operation::MOV_B_R_IND;

                        case 0xc:
                            return operation::MOV_B_R_IND_POST_INC;

                        case 0xe:
                            {
                                auto t = memory[pc + 1] >> 7;
                                switch (t) {
                                    case 0:
                                        return operation::MOV_B_IND_WITH_DIS_16_R;

                                    case 1:
                                        return operation::MOV_B_R_IND_WITH_DIS_16;

                                    default:
                                        return operation::INVALID;
                                }
                            }
                        default:
                            return operation::INVALID;
                    }

                case 7:
                    switch (al) {
                        case 9:
                            switch (bh) {
                                case 1:
                                    //                                     return
                                    //                                     operation::ADD_W_IMM;
                                case 0:
                                    return operation::MOV_W_IMM;
                                case 6:
                                    return operation::AND_W;
                                default:
                                    return INVALID;
                            }


                        case 0xa:
                            switch (bh) {
                                case 0:
                                    return operation::MOV_L_IMM;
                                case 1:
                                    return operation::ADD_L_IMM_R;
                                default:
                                    return INVALID;
                            }
                        default:
                            return INVALID;
                    }

                case 8:
                    return operation::ADD_B_IMM_R;

                case 0xa:
                    return operation::CMP_B_IMM;

                case 0xe:
                    return operation::AND_B_IMM;

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
                    er[destination & 0x7].write(destination, immediate, size);
                    break;

                case WORD:
                    er[destination & 0x7].write(destination, immediate, size);
                    break;

                case LONG:
                    er[destination & 0xf].write(destination, immediate, size);
                    break;
            }

            return true;
        }

        bool h8_300::register_write_register(std::uint8_t destination,
                std::uint8_t source,
                register_size size)
        {
            if (destination > 0xf || source > 0xf) {
                return false;
            }

            auto tmp = er[source & 0x7].read(source, size);
            er[destination & 0x7].write(destination, tmp, size);

            //             switch (size) {
            //                 case register_size::BYTE:
            //                     {

            //                         auto tmp = er[source & 0x7].read(source,
            //                         register_size::BYTE);
            //                         er[destination & 0x7].write(destination, tmp,
            //                         register_size::BYTE);
            //                         break;
            //                     }
            //                 case register_size::WORD:
            //                     {
            //                         uint16_t tmp = er[source & 0x7].read(source,
            //                         register_size::WORD);
            //                         er[destination & 0x7].write(destination, tmp,
            //                         register_size::BYTE) ;
            //
            //                         break;
            //                     }
            //                 case register_size::LONG:
            //                     break;

            //             }
            return true;
        }

        void h8_300::update_ccr_sub(uint32_t value_0,
                uint32_t value_1,
                uint64_t result,
                register_size size)
        {
            auto carry_shift_size = size + 1;
            int sign_0 = value_0 >> size;
            int sign_1 = value_1 >> size;
            int sign_result = result >> size;

            ccr.carry = (result >> carry_shift_size) & 0x1;

            if (result == 0) {
                ccr.zero = 1;
            } else {
                ccr.zero = 0;
            }

            ccr.negative = (sign_result >> size) & 0x1;
            ccr.over_flow = sign_0 != sign_1 && sign_1 != sign_result;
        }

        void h8_300::update_ccr_mov(uint64_t value, register_size size)
        {
            ccr.over_flow = 0;

            if (value == 0) {
                ccr.zero = 1;
            } else {
                ccr.zero = 0;
            }

            //             std::cout << value << std::endl;
            ccr.negative = (value >> size) & 0x1;
        }

        void h8_300::update_ccr_shll(uint64_t value, register_size size)
        {
            update_ccr_mov(value, size);
            ccr.carry = 0x00010000 & value;
        }

    }  // namespace cpu
}  // namespace narcissus
