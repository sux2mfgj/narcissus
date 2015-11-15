#include <string>

#include <cpu.hpp>
#include <cassert>


namespace narcissus {
    namespace cpu {

        h8_300::h8_300(std::array<std::uint8_t, ROM_SIZE>&& mem)
            : er(), sp(), ccr(), pc(), memory(move(mem))
        {}

        auto h8_300::reset_exception() -> void
        {
            // load pc from memory[0] ~ memory[3]
            auto reset_addr = read_immediate(0, 4);

            pc = reset_addr;

            ccr.byte = 0b00000000;
            ccr.interrupt_mask = 1;
        }

        auto h8_300::cycle() -> std::uint32_t
        {

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

                case operation::ADD_B_R_R:
                    {
                        auto rs = read_register_fields(pc + 1, value_place::high, false);
                        auto rd = read_register_fields(pc + 1, value_place::low, false);

                        auto rs_value = read_register(rs, register_size::BYTE);
                        auto rd_value = read_register(rd, register_size::BYTE);

                        auto result = rs_value + rd_value;
                        write_register(rd, result, register_size::BYTE);

                        update_ccr_sub(rd_value, rs_value, result, register_size::BYTE);

                        pc += 2;
                        break;
                    }

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

                case operation::ADD_L_R_R:
                {
                    auto ers = read_register_fields(pc + 1, value_place::high, true);
                    auto erd = read_register_fields(pc + 1, value_place::low, true);

                    auto ers_value = read_register(ers, register_size::LONG);
                    auto erd_value = read_register(erd, register_size::LONG);

                    auto result = ers_value + erd_value;

                    write_register(erd, result, register_size::LONG);
                    pc += 2;
                    break;
                }

                case operation::ADDS_4:
                {
                    auto erd = read_register_fields(pc + 1, value_place::low, false);
//                     auto erd = memory[pc + 1] & 0x7;

                    auto result = er[erd].er + 4;

                    write_register(erd, result, register_size::LONG);

                    pc += 2;
                    break;
                }

                case operation::ADDS_1:
                {
                    auto erd = read_register_fields(pc + 1, value_place::low, true);
                    auto erd_value = read_register(erd, register_size::LONG);

                    auto result = erd_value + 1;

                    write_register(erd, result, register_size::LONG);
                    pc += 2;
                    break;
                }

                case operation::ADD_B_IMM_R:
                {
                    auto rd = read_register_fields(pc, value_place::low, false);
                        //memory[pc] & 0xf;
                    auto imm = memory[pc + 1];

                    auto rd_value = read_register(rd, register_size::BYTE);
                    
                    //TODO Do i have to cast by integer?
                    auto result = rd_value + imm;

                    write_register(rd, result, register_size::BYTE);

                    update_ccr_sub(rd_value, imm, result, register_size::BYTE);
                    pc += 2;

                    break;
                }

                case operation::ADD_L_IMM_R:
                {
                    auto erd = read_register_fields(pc + 1, value_place::low, true);
                    auto imm = read_immediate(pc + 2, 4);

                    auto erd_value = er[erd].er;
                    auto result = (std::int32_t)erd_value + (std::int32_t)imm;

                    write_register(erd, result, register_size::LONG);

                    update_ccr_sub(erd_value, imm, result, register_size::LONG);
                    pc += 6;
                    break;
                }

                case operation::SUB_B_R_R: 
                {
                    auto rs = read_register_fields(pc + 1, value_place::high, false);
                    auto rd = read_register_fields(pc + 1, value_place::low, false);

                    auto src_value = read_register(rs, register_size::BYTE);
                    auto dest_value = read_register(rd, register_size::BYTE);

                    auto imm = dest_value - src_value;

                    write_register(rd, imm, register_size::BYTE);

                    update_ccr_sub(src_value, dest_value, imm, register_size::BYTE);

                    pc += 2;
                    break;
                }

                case operation::SUB_W_R_R: 
                {
                    auto rs = read_register_fields(pc + 1, value_place::high, false);
                    auto rd = read_register_fields(pc + 1, value_place::low, false);

                    auto src_value = read_register(rs, register_size::WORD);
                    auto dest_value = read_register(rd, register_size::WORD);

                    auto result = dest_value - src_value;
        
                    write_register(rd, result, register_size::WORD);
                    update_ccr_sub(src_value, dest_value, result, register_size::WORD);

                    pc += 2;
                    break;
                }

                case operation::SUB_L_R_R:
                {
                    auto ers = read_register_fields(pc + 1, value_place::high, true);
                    auto erd = read_register_fields(pc + 1, value_place::low, true);

                    auto ers_value = er[ers].er;
                    auto erd_value = er[erd].er;

                    auto result = erd_value - ers_value;
                
                    write_register(erd, result, register_size::LONG);

                    update_ccr_sub(ers_value, erd_value, result, register_size::LONG);

                    pc += 2;
                    break;
                }

                // h8/300h シリーズ　プログラミングマニュアルに無い？
                case operation::SUB_L_IMM_R:
                {
                    auto erd = read_register_fields(pc + 1, value_place::low, true);

                    auto imm = read_immediate(pc + 2, 4);

                    auto erd_value = read_register(erd, register_size::LONG);

                    auto result = erd_value  - imm;

                    write_register(erd, result, register_size::LONG);

                    update_ccr_sub(erd_value, imm, result, register_size::LONG);
                    pc += 6; 
                    break;
                }

                case operation::SUB_WITH_SIGN_EXT_1:
                {
                    auto erd = read_register_fields(pc + 1, value_place::low, true);

                    er[erd].er -= 1;

                    pc += 2;
                    break;
                }

                case operation::SUB_WITH_SIGN_EXT_4:
                {
                    auto erd = read_register_fields(pc + 1, value_place::low, true);

                    er[erd].er -= 4;

                    pc += 2;
                    break;
                }

                case operation::MOV_B_IMM: 
                {
                    auto rd = read_register_fields(pc, value_place::low, false);

                    auto imm = memory[pc + 1];

                    write_register(rd, imm, register_size::BYTE);

                    update_ccr_mov(imm, register_size::BYTE);
                    pc += 2;
                    break;
                }

                case operation::MOV_B_R_R:
                {

                    auto rs = read_register_fields(pc + 1, value_place::high, false);
                    auto rd = read_register_fields(pc + 1, value_place::low, false);

                    auto result = read_register(rs, register_size::BYTE);
                    write_register(rd, result, register_size::BYTE);

                    update_ccr_mov(result, register_size::BYTE);
                    pc += 2;
                    break;
                }

                case operation::MOV_B_R_R_IND: 
                {
                    auto erd = read_register_fields(pc + 1, value_place::high, true);
                    auto rs = read_register_fields(pc + 1, value_place::low, false);

                    auto dest_addr = read_register(erd, register_size::LONG);
                    auto result = read_register(rs, register_size::BYTE);
                    write_immediate(dest_addr, 1, result);

                    update_ccr_mov(result, register_size::BYTE);
                    pc += 2;

                    break;
                }

                case operation::MOV_B_R_IND_R:
                {
                    auto ers = read_register_fields(pc + 1, value_place::high, true);
                    auto rd = read_register_fields(pc + 1, value_place::low, false);

                    auto ers_value = read_register(ers, register_size::LONG);
                    auto result = read_immediate(ers_value, 1);

                    write_register(rd, result, register_size::BYTE);

                    update_ccr_mov(result, register_size::BYTE);
                    pc += 2;

                    break;
                }

                    //                 case operation::MOV_B_R_IND:
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

                case operation::MOV_B_IND_WITH_DIS_16_R: 
                {
                    auto ers = read_register_fields(pc + 1, value_place::high, true);
                    auto rd = read_register_fields(pc + 1, value_place::low, false);

                    auto disp = (std::uint16_t)read_immediate(pc + 2, 2);

                    auto src_value = er[ers].er;
                    auto addr = src_value + (std::int16_t)disp;

                    auto result = memory[addr];

                    write_register(rd, result, register_size::BYTE);

                    update_ccr_mov(result, register_size::BYTE);
                    pc += 4;
                    break;
                }

                case operation::MOV_B_IND_WITH_DIS_24_R:
                {
                    auto ers = read_register_fields(pc + 1, value_place::high, true);
                    auto rd = read_register_fields(pc + 3, value_place::low, false);

                    auto imm = read_immediate(pc + 5, 3);

                    auto ers_value = read_register(ers, register_size::LONG);
                    auto addr = ers_value + (std::int32_t)imm;

                    auto result = memory[addr];

                    write_register(rd, result, register_size::BYTE);

                    pc += 8;
                    break;
                }

                case operation::MOV_B_R_IND_WITH_DIS_16:
                {

                    auto erd = read_register_fields(pc + 1, value_place::high, true);
                    auto rs = read_register_fields(pc + 1, value_place::low, false);
                
                    auto disp = (std::uint16_t)read_immediate(pc + 2, 2);

                    auto src_value = read_register(rs, register_size::BYTE);
                    auto dest_addr = read_register(erd, register_size::LONG);

                    dest_addr += (std::int16_t)disp;
                    memory[dest_addr] = src_value;

                    update_ccr_mov(src_value, register_size::BYTE);
                    pc += 4;
                    break;
                }

                case operation::MOV_B_R_IND_POST_INC:
                {
        
                    auto ers = read_register_fields(pc + 1, value_place::high, true);
                    auto rd = read_register_fields(pc + 1, value_place::low, false);

                    auto result = memory[er[ers].er];
                    write_register(rd, result, register_size::BYTE);

                    update_ccr_mov(result ,register_size::BYTE);

                    er[ers].er += 1;
                    pc += 2;
                    break;
                }


                case operation::MOV_W_IMM: 
                {
                    auto rd = read_register_fields(pc + 1, value_place::low, false);

                    auto imm = (std::uint16_t)read_immediate(pc + 2, 2);

                    write_register(rd, imm, register_size::WORD);

                    update_ccr_mov(imm, register_size::WORD);
                    pc += 4;
                    break;
                }

                case operation::MOV_W_R_R:
                {
                    auto rs = read_register_fields(pc + 1, value_place::high, false);
                    auto rd = read_register_fields(pc + 1, value_place::low, false);

                    auto result = read_register(rs, register_size::WORD);
                    write_register(rd, result, register_size::WORD);

                    write_register(rd, result, register_size::WORD);

                    update_ccr_mov(result,
                            register_size::WORD);

                    pc += 2;
                    break;
                }

                case operation::MOV_W_ASB_24_R:
                {
                    auto rd = read_register_fields(pc + 1, value_place::low, false);
                    auto abs = read_immediate(pc + 3, 3);

                    auto result = read_immediate(abs, 2);

                    write_register(rd, result, register_size::WORD);
                    update_ccr_mov(result, register_size::WORD);

                    pc += 6;
                    break;
                }

                case operation::MOV_W_R_ABS_24:
                {
                    auto rs = read_register_fields(pc + 1, value_place::low, false);
                    auto abs = read_immediate(pc + 3, 3);

                    auto result = read_register(rs, register_size::WORD);

                    write_immediate(abs, 2, result);
                    update_ccr_mov(result, register_size::WORD);

                    pc += 6;
                    break;
                }

                case operation::MOV_W_R_IND_WITH_DIS_16:
                {
                    auto rs = read_register_fields(pc + 1, value_place::low, false);
                    auto erd = read_register_fields(pc + 1, value_place::high, false);

                    auto disp = (std::int16_t)read_immediate(pc + 2, 2);

                    auto addr = read_register(erd, register_size::LONG);
                    addr += disp;

                    auto result = read_register(rs, register_size::WORD);
                    write_immediate(addr, 2, result);

                    update_ccr_mov(result, register_size::WORD);
                    pc += 4;
                    break;
                }

                case operation::MOV_W_IND_WITH_DIS_16_R:
                {
                    auto ers = read_register_fields(pc + 1, value_place::high, true);
                    auto rd = read_register_fields(pc + 1, value_place::low, false);
                    auto disp = (std::int16_t)read_immediate(pc + 2, 2);

                    auto addr = read_register(ers, register_size::LONG);
                    addr += disp;

                    auto result = read_immediate(addr, 2);

                    write_register(rd, result, register_size::WORD);

                    update_ccr_mov(result, register_size::WORD);
                    pc += 4;
                    break;
                }

                case operation::MOV_L_IMM: 
                {
                    auto erd = read_register_fields(pc + 1, value_place::low, true);
    
                    auto imm = read_immediate(pc + 2, 4);

                    write_register(erd, imm, register_size::LONG);

                    update_ccr_mov(imm, register_size::LONG);
                    pc += 6;
                    break;
                }

                case operation::MOV_L_R_R: 
                {
                    auto ers = read_register_fields(pc + 1, value_place::high, true);
                    auto erd = read_register_fields(pc + 1, value_place::low, true);

                    auto src_value = er[ers].er;
                    er[erd].er = src_value;

                    update_ccr_mov(src_value, register_size::LONG);
                    pc += 2;
                    break;
                }

                case operation::MOV_L_R_R_IND:
                {
                    auto ers = read_register_fields(pc + 3, value_place::high, true);
                    auto erd = read_register_fields(pc + 3, value_place::low, true);

                    auto ers_value = read_register(ers, register_size::LONG);

                    auto result = read_immediate(ers_value, 4);
                    write_register(erd, result, register_size::LONG);

                    update_ccr_mov(result, register_size::LONG);

                    pc += 4;
                    break;
                }

                case operation::MOV_L_R_IND_R:
                {
                    auto erd = read_register_fields(pc + 3, value_place::high, true);
                    auto ers = read_register_fields(pc + 3, value_place::low, true);

                    auto result = read_register(ers, register_size::LONG);
                    auto erd_value = read_register(erd, register_size::LONG);

                    write_immediate(erd_value, 4, result);
                    update_ccr_mov(result, register_size::LONG);

                    pc += 4;
                    break;
                }

                case operation::MOV_L_R_IND_PRE_DEC: 
                {
                    auto erd = read_register_fields(pc + 3, value_place::high, true);
                    auto ers = read_register_fields(pc + 3, value_place::low, true);

                    auto dest_addr = er[erd].er;
                    auto result = er[ers].er;

                    dest_addr -= 4;
                    
                    write_immediate(dest_addr, 4, result);

                    er[erd].er = dest_addr;

                    update_ccr_mov(result, register_size::LONG);
                    pc += 4;

                    break;
                }

                case operation::MOV_L_IND_WITH_DIS_24_R: 
                {
                    auto ers = read_register_fields(pc + 3, value_place::high, true);
                    auto erd = read_register_fields(pc + 5, value_place::low, true);

                    auto disp = read_immediate(pc + 7, 3);

                    auto addr = (std::int32_t)(er[ers].er) + (std::int32_t)disp;

                    auto dest_value = read_immediate(addr, 4);

                    er[erd].er = dest_value;

                    update_ccr_mov(dest_value, register_size::LONG);
                    pc += 10;
                    break;
                }

                case operation::MOV_L_IND_WITH_DIS_16_R:
                {
                    auto ers = read_register_fields(pc + 3, value_place::high, true);
                    auto erd = read_register_fields(pc + 3, value_place::low, true);

                    auto disp = (std::int16_t)read_immediate(pc + 4, 2);

                    auto addr = read_register(ers, register_size::LONG);
                    addr += disp;

                    auto result = read_immediate(addr, 4);

                    write_register(erd, result, register_size::LONG);
                    update_ccr_mov(result, register_size::LONG);
                    pc += 6;
                    break;
                }

                case operation::MOV_L_R_IND_WITH_DIS_16:
                {
                    auto erd = read_register_fields(pc + 3, value_place::high, true);
                    auto ers = read_register_fields(pc + 3, value_place::low, true);

                    auto disp = (std::int16_t)read_immediate(pc + 4, 2);

                    auto result = read_register(ers, register_size::LONG);
                    auto erd_value = read_register(erd, register_size::LONG);

                    auto addr = erd_value + disp;
                    write_immediate(addr, 4, result);

                    update_ccr_mov(result, register_size::LONG);
                    pc += 6;
                    break;
                }

                case operation::MOV_L_R_IND_POST_INC: 
                {
                    auto ers = read_register_fields(pc + 3, value_place::high, true);
                    auto erd = read_register_fields(pc + 3, value_place::low, true);

                    auto source_addr = er[ers].er;

                    auto dest_value = read_immediate(source_addr, 4);
                    source_addr += 4;

                    er[erd].er = dest_value;
                    er[ers].er = source_addr;

                    update_ccr_mov(dest_value, register_size::LONG);
                    pc += 4;

                    break;
                }

                case operation::MOV_L_IMM_ABS_24_R:
                {
                    auto erd = read_register_fields(pc + 3, value_place::low, true);
                    auto abs = read_immediate(pc + 5, 3);

                    auto result = read_immediate(abs, 4);
                    write_register(erd, result, register_size::LONG);

                    update_ccr_mov(result, register_size::LONG);
                    pc += 8;

                    break;
                }

                case operation::MOV_L_R_IMM_ABS_24:
                {
                    auto ers = read_register_fields(pc + 3, value_place::low, true);
                    auto abs = read_immediate(pc + 5, 3);

                    auto result = read_register(ers, register_size::LONG);
                    write_immediate(abs, 4, result);

                    update_ccr_mov(result, register_size::LONG);
                    pc += 8;

                    break;
                }

                case operation::BEQ_8: 
                {
                    auto disp = memory[pc + 1];

                    pc += 2;
                    if (ccr.zero != 0x1) {
                        break;
                    }

                    pc += (std::int8_t)disp;
                    break;
                }

                case operation::BEQ_16:
                {
                    auto disp = (std::int16_t)read_immediate(pc + 2, 2);
                    if(ccr.zero == 1){
                        pc += disp;
                    }

                    pc += 4;
                    break;
                }

                case operation::BRA_8: 
                {
                    auto disp = (std::int8_t)memory[pc + 1];
                    pc += 2;
                    pc += disp;

                    break;
                }

                case operation::BRA_16:
                {
                    auto disp = read_immediate(pc + 2, 2);
                    pc += (std::int16_t)disp + 4;
                    break;
                }

                case operation::BNE_8:
                {
                    auto disp = memory[pc + 1];
                    pc += 2;
                    if(ccr.zero == 0) {
                        pc += (std::int8_t)disp;
                    }
                    break;
                }

                case operation::BNE_16:
                {

                    auto disp = read_immediate(pc + 2, 2);
                    if(ccr.zero == 0){
                        pc += (std::int16_t)disp;
                    }
                    pc += 4;
                    break;
                }

                case operation::BLE_8:
                {
                    auto disp = read_immediate(pc + 1, 1);

                    pc += 2;
                    if(ccr.zero || (ccr.negative ^ ccr.over_flow))
                    {
                        pc += disp;
                    }
                    break;
                }

                case operation::BLS_8:
                {
                    auto disp = read_immediate(pc + 1, 1);
                    
                    if(ccr.carry || ccr.zero){
                        pc += (std::uint8_t)disp;
                    }
                    pc += 2;

                    break;
                }

                case operation::BGE_8:
                {
                    auto disp = read_immediate(pc + 1, 1);

                    if(ccr.negative == ccr.over_flow)
                    {
                        pc += (std::int8_t)disp;
                    }
                    pc += 2;
                    break;
                }

                case operation::BGT_8:
                {
                    auto disp = (std::int8_t)read_immediate(pc + 1, 1);
                    if(!(ccr.zero || (ccr.negative ^ ccr.over_flow)))
                    {
                        pc += disp;
                    }
                    pc += 2;
                    break;
                }

                case operation::CMP_B_IMM:
                {
                    auto rd = read_register_fields(pc, value_place::low, false);
//                     auto rd = memory[pc] & 0xf;
                    auto imm = memory[pc + 1];

                    auto rd_value = read_register(rd, register_size::BYTE);
                    auto result = rd_value - imm;

                    update_ccr_sub(rd_value, imm, result, register_size::BYTE);
                    pc += 2;
                    break;
                }

                case operation::CMP_B_R_R:
                {
                    auto rs = read_register_fields(pc + 1, value_place::high, false);
                    auto rd = read_register_fields(pc + 1, value_place::low, false);

                    auto rs_value = read_register(rs, register_size::BYTE);
                    auto rd_value = read_register(rd, register_size::BYTE);

                    auto result = rd_value - rs_value;

                    update_ccr_sub(rs_value, rd_value, result, register_size::BYTE);
                    pc += 2;
                    break;
                }

                case operation::CMP_L_IMM:
                {
                    auto erd = read_register_fields(pc + 1, value_place::low, true);
                    auto imm = read_immediate(pc + 2, 4);

                    auto erd_value = read_register(erd, register_size::LONG);

                    std::uint64_t result = (std::int32_t)erd_value - (std::int32_t)imm;

                    update_ccr_sub(erd_value, imm, result, register_size::LONG);
                    pc += 6;
                    break;
                }

                case operation::CMP_L_R_R:
                {
                    auto ers = read_register_fields(pc + 1, value_place::high, true);
                    auto erd = read_register_fields(pc + 1, value_place::low, true);

                    auto ers_value = read_register(ers, register_size::LONG);
                    auto erd_value = read_register(erd, register_size::LONG);

                    std::uint64_t result = (std::int32_t)erd_value - (std::int32_t)ers_value;

                    update_ccr_sub(erd_value, ers_value, result, register_size::LONG);
                    pc += 2;

                    break;
                }

                case operation::AND_W:
                {
                    auto rd = read_register_fields(pc + 1, value_place::low, false);

                    auto imm = (std::uint16_t)read_immediate(pc + 2, 2);

                    auto rd_value = read_register(rd, register_size::WORD);
                    auto result = rd_value & imm;
                    write_register(rd, result, register_size::WORD);

                    update_ccr_mov(result, register_size::WORD);
                    pc += 4;
                    break;
                }

                case operation::DEC_W_1:
                {
                    auto rd = read_register_fields(pc + 1, value_place::low, false);

                    auto rd_value = read_register(rd, register_size::WORD);
                    auto result = rd_value - 1;

                    write_register(rd, result, register_size::WORD);
                    update_ccr_sub(rd_value, 1, result, register_size::WORD);

                    pc += 2;
                    break;
                }

                case operation::AND_B_IMM:
                {
                    auto rd = read_register_fields(pc, value_place::low, false);

                    auto imm = memory[pc + 1];

                    auto rd_value = read_register(rd, register_size::BYTE);
                    auto result = rd_value & imm;

                    write_register(rd, result, register_size::BYTE);

                    pc += 2;
                    update_ccr_mov(result, register_size::BYTE);
                    break;
                }

                case operation::JSR_ABS: 
                {
                    auto abs = read_immediate(pc + 1, 3);

                    pc += 4;

                    memory[--sp] = (std::uint8_t)(pc & 0x0000ff);
                    memory[--sp] = (std::uint8_t)((pc >> 8) & 0x0000ff);
                    memory[--sp] = (std::uint8_t)((pc >> 16) & 0x0000ff);
                    memory[--sp] = 0x00;

                    pc = abs;
                    break;
                }

                case operation::EXTS_L: 
                {
                    auto erd = read_register_fields(pc + 1, value_place::low, true);

                    auto val = er[erd].er;
                    uint32_t sign = (uint32_t)(val & 0x00008000);
                    sign |= sign << 1;  // 0x00018000;
                    sign |= sign << 1;  // 0x00038000;
                    sign |= sign << 2;  // 0x000f8000;
                    sign |= sign << 4;  // 0x00ff8000;
                    sign |= sign << 8;  // 0xffff8000;

                    auto result = (val & 0xffff) | sign;
                    er[erd].er = result; //(val & 0x00008fff) | sign;

                    update_ccr_mov(result, register_size::LONG);
                    pc += 2;
                    break;
                }

                case operation::EXTU_W:
                {
                    auto rd = read_register_fields(pc + 1, value_place::low, false);

                    auto rd_value = read_register(rd, register_size::WORD);

                    auto result = rd_value & 0x00ff;

                    write_register(rd, result, register_size::WORD);
                    update_ccr_mov(result, register_size::WORD);

                    pc += 2;
                    break;
                }

                case operation::SHLL_L: 
                {
                    auto erd = read_register_fields(pc + 1, value_place::low, true);
                    er[erd].er = er[erd].er << 1;

                    update_ccr_shll(er[erd].er, register_size::LONG);
                    pc += 2;
                    break;
                }

                case operation::SHLR_L:
                {
                    auto erd = read_register_fields(pc + 1, value_place::low, true);
                    auto result = read_register(erd, register_size::LONG);
                    result = result >> 1;

                    write_register(erd, result, register_size::LONG);
                    update_ccr_shlr(result, register_size::LONG);

                    pc += 2;
                    break;
                }

                case operation::RTS: 
                {
                    // memory[sp]: is reserved
                    memory[sp++];
                    auto return_addr = read_immediate(sp, 3);
                    sp += 3;

                    pc = return_addr;

                    break;
                }

                case operation::XOR_B_R_R:
                {
                    auto rs = read_register_fields(pc + 1, value_place::high, false);
                    auto rd = read_register_fields(pc + 1, value_place::low, false);

                    auto rs_value = read_register(rs, register_size::BYTE);
                    auto rd_value = read_register(rd, register_size::BYTE);

                    auto result = rd_value ^ rs_value;
                    write_register(rd, result, register_size::BYTE);

                    update_ccr_mov(result, register_size::BYTE);
                    
                    pc += 2;
                    break;
                }

                case operation::XOR_B_R_IMM:
                {
                    auto rd = read_register_fields(pc, value_place::low, false);
                    auto imm = (std::uint8_t)read_immediate(pc + 1, 1);

                    auto rd_value = (std::uint8_t)read_register(rd, register_size::BYTE);

                    auto result = rd_value ^ imm;

                    update_ccr_mov(result, register_size::BYTE);
                    pc += 2;
                    break;
                }

                case operation::INVALID:
                      std::clog << "INVALID opecode: " << std::hex << "0x" << std::flush;
                      std::clog << std::setw(2) << std::setfill('0')
                          << (std::uint16_t)(memory[pc]) << std::flush;
                      std::clog << std::setw(2) << std::setfill('0')
                          << (std::uint16_t)(memory[pc + 1]) << std::endl;

                      std::clog << "pc             : 0x" << std::setw(8)
                          << std::setfill('0') << (std::uint32_t)pc << std::endl;

                    assert(false);


                default:
                    std::cout << "implement yet" << std::endl;
                    assert(false);
            }

            return pc; 
        }

        auto h8_300::detect_operation() -> operation
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
                                                {
                                                    auto dh = memory[pc + 3] >> 4;
                                                    auto dl = memory[pc + 3] & 0xf;
                                                    auto e = memory[pc + 4];
                                                    switch (cl) {
                                                        case 9:
                                                        {
                                                            if(!(dh&0x8) && !(dl&0x8))
                                                            {
                                                                return operation::MOV_L_R_R_IND;
                                                            }
                                                            if((dh&0x8) && !(dl&0x8))
                                                            {
                                                                return operation::MOV_L_R_IND_R;
                                                            }

                                                            return operation::INVALID;
                                                        }

                                                        case 0xb:
                                                        {
                                                            if((dh == 2) && !(dl & 0x8) && e == 0)
                                                            {
                                                                return operation::MOV_L_IMM_ABS_24_R; 
                                                            }

                                                            if((dh == 0xa) && !(dl & 0x8) && e == 0)
                                                            {
                                                                return operation::MOV_L_R_IMM_ABS_24;
                                                            }

                                                            return operation::INVALID;
                                                        }
                                                        case 0xd: 
                                                        {
                                                            if ((dh & 0x8) && !(dl &0x8)) {
                                                                return operation::MOV_L_R_IND_PRE_DEC;
                                                            }
                                                            return operation::
                                                                MOV_L_R_IND_POST_INC;
                                                        }

                                                        case 0xf:
                                                        {
                                                            if((dh & 0x8) && !(dl & 0x8))
                                                            {
                                                                return operation::MOV_L_R_IND_WITH_DIS_16;
                                                            }

                                                            if(!(dh & 0x8) && !(dl & 0x8)){
                                                                return operation::MOV_L_IND_WITH_DIS_16_R;
                                                            }
                                                            return operation::INVALID;

                                                        }

                                                    default:
                                                            return operation::INVALID;
                                                    }
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
                                                                            return operation::INVALID;
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
                            return operation::ADD_B_R_R;
                        case 9:
                            //                             return operation::ADD_W_R_R;
                            return operation::INVALID;
                        case 0xa:
                            if((bh & 0x8) && !(bl & 0x8)){
                                return operation::ADD_L_R_R;
                            }
                            switch (bh) {
                                default:
                                    return operation::INVALID;
                            }

                        case 0xb:
                            switch (bh) {
                                case 0x0:
                                    return operation::ADDS_1;
                                case 0x8:
//                                     return operation::ADDS_2;
                                    return operation::INVALID;
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
                        case 1:
                            switch (bh) {
                                case 3:
                                {
                                    auto t = bl & 0x8;
                                    if(t == 0x0){
                                        return operation::SHLR_L;
                                    }
                                    return operation::INVALID;
                                }
                                default:
                                    return operation::INVALID;
                            }

                        case 5:
                            return operation::XOR_B_R_R;

                        case 7:
                            switch (bh) {
                                case 5:
                                    return operation::EXTU_W;

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
                                    return operation::SUB_WITH_SIGN_EXT_1;
                                case 5:
                                    return operation::DEC_W_1;
                                case 8:
//                                     return operation::SUB_WITH_SIGN_EXT_2;
                                    return operation::INVALID;
                                case 9:
                                    return operation::SUB_WITH_SIGN_EXT_4;
                                case 0xd:
//                                     return operation::DEC_W_2;

                                default:
                                    return operation::INVALID;
                            }

                        case 0xc:
                            return operation::CMP_B_R_R;

                        case 0xf:
                            if((bh & 0x8) && !(bl & 0x8)){
                                return operation::CMP_L_R_R;
                            }

                        default:
                            return operation::INVALID;
                    }

                case 4:
                    switch (al) {
                        case 0:
                            return operation::BRA_8;

                        case 3:
                            return operation::BLS_8;

                        case 6:
                            return operation::BNE_8;

                        case 7:
                            return operation::BEQ_8;

                        case 0xc:
                            return operation::BGE_8;

                        case 0xe:
                            return operation::BGT_8;

                        case 0xf:
                            return operation::BLE_8;

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

                        case 8:
                            switch (bh) {
                                case 0:
                                    switch (bl) {
                                        case 0:
                                            return operation::BRA_16;

                                        default:
                                            return operation::INVALID;
                                    }
                                case 6:
                                    switch (bl) {
                                        case 0:
                                            return operation::BNE_16;
                                        default:
                                            return operation::INVALID;
                                    }
                                case 7:
                                    switch (bl) {
                                        case 0:
                                            return operation::BEQ_16;
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
                            if((bh & 0x8) == 0x8){
                                return operation::MOV_B_R_R_IND;
                            }
                            else {
                                return operation::MOV_B_R_IND_R;
                            }

                        case 0xb:
                            switch (bh) {
                                case 2:
                                    if((ch == 0) && (cl == 0)){
                                        return operation::MOV_W_ASB_24_R;
                                    }
                                    return operation::INVALID;

                                case 0xa:
                                    if((ch == 0) && (cl == 0))
                                    {
                                        return operation::MOV_W_R_ABS_24;
                                    }
                                    return operation::INVALID;

                                default:
                                    return operation::INVALID;
                            }


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
                        case 0xf:
                            {
                                if(bh & 0x8) 
                                {
                                    return operation::MOV_W_R_IND_WITH_DIS_16;
                                }
                                else {
                                    return operation::MOV_W_IND_WITH_DIS_16_R;
                                }
                            }
                        default:
                            return operation::INVALID;
                    }

                case 7:
                    switch (al) {
                        case 8:
                        {
                            auto t = bh >> 3;
                            if (t == 0x0) {
                                switch (bl) {
                                    case 0:
                                        switch (ch) {
                                            case 6:
                                                switch (cl) {
                                                    case 0xa:
                                                    {
                                                        auto dh = memory[pc + 3] >> 4;
                                                        auto dl = memory[pc + 3] & 0xf;
                                                        auto eh = memory[pc + 4] >> 4;
                                                        auto el = memory[pc + 4] & 0xf;
                                                        if(dh == 0x2 && eh == 0 && el == 0){
                                                            return operation::MOV_B_IND_WITH_DIS_24_R;
                                                        }
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
                            }
                            return operation::INVALID;
                        }
                        case 9:
                            switch (bh) {
                                case 1:
                                    return operation::INVALID;
                                case 0:
                                    return operation::MOV_W_IMM;
                                case 6:
                                    return operation::AND_W;
                                default:
                                    return operation::INVALID;
                            }


                        case 0xa:
                            switch (bh) {
                                case 0:
                                    return operation::MOV_L_IMM;
                                case 1:
                                    return operation::ADD_L_IMM_R;

                                case 2:
                                    if(!(bl & 0x8)){
                                        return operation::CMP_L_IMM;
                                    }
                                    return operation::INVALID;
                                case 3:
                                    if((bl & 0x8) == 0x0)
                                    {
                                        return operation::SUB_L_IMM_R;
                                    }
                                    return operation::INVALID;

                                default:
                                    return operation::INVALID;
                            }
                        default:
                            return operation::INVALID;
                    }

                case 8:
                    return operation::ADD_B_IMM_R;

                case 0xa:
                    return operation::CMP_B_IMM;

                case 0xd:
                    return operation::XOR_B_R_IMM;

                case 0xe:
                    return operation::AND_B_IMM;

                case 0xf:
                    return operation::MOV_B_IMM;

                default:
                    return operation::INVALID;
            }
        }

        auto h8_300::update_ccr_sub(std::uint32_t value_0,
                std::uint32_t value_1,
                std::uint64_t result,
                register_size r_size)
            -> void
        {
            auto size = (std::uint32_t)r_size;
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

        auto h8_300::update_ccr_mov(std::uint64_t value, register_size size)
            -> void
        {
            ccr.over_flow = 0;

            if (value == 0) {
                ccr.zero = 1;
            } else {
                ccr.zero = 0;
            }

            ccr.negative = (value >> (std::uint32_t)size) & 0x1;
        }

        auto h8_300::update_ccr_shll(std::uint64_t value, register_size size)
            -> void
        {
            update_ccr_mov(value, size);
            ccr.carry = 0x00010000 & value;
        }

        auto h8_300::update_ccr_shlr(std::uint32_t value, register_size size)
            -> void
        {
            update_ccr_shll(value, size);
            ccr.negative = 0;
        }

        auto h8_300::read_register_fields(std::uint32_t address, 
                value_place place, bool is_32bit) -> std::uint8_t
        {
            auto reg = memory[address];

            switch (place) {
                case value_place::high:
                    reg = (reg & 0xf0) >> 4;
                    break;

                case value_place::low:
                    reg = reg & 0x0f;
                    break;
            }

            if(is_32bit) {
                return reg & 0x7;
            }
            else {
                return reg;
            }
        }

        auto h8_300::read_immediate(std::uint32_t address, 
                std::uint8_t number_of_byte) -> std::uint32_t
        {
            auto imm = (std::uint32_t)memory[address] << (8 * (number_of_byte - 1));
            for(auto i = 1; i < number_of_byte; ++i)
            {
                imm |= (std::uint32_t)memory[address + i] << ((number_of_byte - i - 1) * 8);
            }

            return imm;
        }

        auto h8_300::write_immediate(std::uint32_t base, 
                std::uint8_t number_of_byte, std::uint32_t immediate) -> void
        {
            for(auto i = 0; i < number_of_byte; ++i)
            {
                memory[base + i] = immediate >> (number_of_byte - i - 1) * 8;
            }
        }

        auto h8_300::read_register(std::uint8_t source, register_size size) 
            -> std::uint32_t
        {
            auto reg = er[source & 0x7];
            switch (size) {
                case register_size::BYTE:
                    if((source & 0x8) != 0x8){
                        return reg.h;
                    }
                    else {
                        return reg.l;
                    }
                case register_size::WORD:
                    if((source & 0x8) != 0x8) {
                        return reg.r;
                    }
                    else {
                        return reg.e;
                    }
                case register_size::LONG:
                    return reg.er;
            }
        }

        auto h8_300::write_register(std::uint8_t destination, 
                        std::uint32_t value, register_size size) -> void
        {
            auto &reg = er[destination & 0x7];
            switch (size) {
                case register_size::BYTE:
                    if((destination & 0x8) != 0x8) {
                        reg.h = std::uint8_t(value);
                    }
                    else {
                        reg.l = std::uint8_t(value);
                    }
                    break;
                case register_size::WORD:
                    if((destination & 0x8) != 0x8){
                        reg.r = std::uint16_t(value);
                    }
                    else {
                        reg.e = std::uint16_t(value);
                    }
                    break;
                case register_size::LONG:
                    reg.er = value;
                    break;
            }
        }

    }  // namespace cpu
}  // namespace narcissus
