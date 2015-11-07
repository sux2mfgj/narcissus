#pragma once

#include <cstdint>
#include <array>

#include <gtest/gtest.h>

#include <mcu.hpp>

namespace narcissus {
    namespace cpu {

        enum class operation : std::uint8_t
        {
            INVALID = 0,
//             ADD_B_IMM,              // immediate
//             ADD_B_R_R,              // register to register
//             ADD_W_IMM,
//             ADD_W_R_R,
//             ADD_L_IMM,
//             ADD_L_R_R,

            SUB_B_R_R,                      // substruct rd from rs
            SUB_W_R_R,
            SUB_L_R_R,

            SUB_WITH_SIGN_EXT_1,
//             SUB_WITH_SIGN_EXT_2,
            SUB_WITH_SIGN_EXT_4,            // subtract 4 with sign extention

            ADD_B_IMM_R,

            ADD_L_IMM_R,

            ADDS_4,                     // add with sign extention
            

            
            MOV_B_IMM,
            MOV_B_R_R,  
            MOV_B_R_IND,                // register indirect
            MOV_B_R_IND_WITH_DIS_16,    // register indirect with displacement(16 bit)
            MOV_B_IND_WITH_DIS_16_R,    // register indirect with displacement(16 bit)
//             MOV_B_IND_WITH_DIS_24_R,    // register indirect with displacement(24 bit)
            MOV_B_R_IND_POST_INC,       // rd increment after register indirect acdess
//             MOV_B_ASB_8,                // absolute addressing 8
//             MOV_B_ASB_16,               // absolute addressing 16
//             MOV_B_ASB_24,               // absolute addressing 24

            MOV_W_IMM,
            MOV_W_R_R,


            MOV_L_IMM, 
            MOV_L_R_R,
            MOV_L_R_IND,

            MOV_L_IND_WITH_DIS_24_R,    // MEMORY to register
//             MOV_L_R_IND_WITH_DIS_24, // register to memory
            MOV_L_R_IND_POST_INC,

            BEQ,                        // branch equal
            BRA,                        // branch always(true)
            BNE,                        // branch not equal

            CMP_B_IMM,                  // compare register and immediate

            AND_W,                      // AND logical
            AND_B_IMM,

            SHLL_L,                     // shift logical left

            JSR_ABS,                    // jump to subroutine use absolute address
            EXTS_L,                     // extend as signed
            RTS,
        };

        enum class register_size : std::uint32_t
        { 
            BYTE = 7, 
            WORD = 15, 
            LONG = 31
        };

        union register_t {
            register_t() : er32(0) {}

            auto write(std::uint8_t destination, std::uint32_t value, register_size size)
                -> void
            {
                switch (size) {
                    case register_size::BYTE:
                        if((destination & 0x8) != 0x8) {
                            h = std::uint8_t(value);
                        }
                        else {
                            l = std::uint8_t(value);
                        }
                        break;
                    case register_size::WORD:
                        if((destination & 0x8) != 0x8){
                            r = std::uint16_t(value);
                        }
                        else {
                            e = std::uint16_t(value);
                        }
                        break;
                    case register_size::LONG:
                        er32 = value;
                        break;
                }
            }

            auto read(std::uint8_t source, register_size size)
                -> std::uint32_t 
            {
                switch (size) {
                    case register_size::BYTE:
                        if((source & 0x8) != 0x8){
                            return h;
                        }
                        else {
                            return l;
                        }
                    case register_size::WORD:
                        if((source & 0x8) != 0x8) {
                            return r;
                        }
                        else {
                            return e;
                        }
                    case register_size::LONG:
                        return er32;
                }
            }

            std::uint32_t er32;

            struct {
                union {
                    std::uint16_t r;
                    struct {
                        std::uint8_t l;
                        std::uint8_t h;
                    };
                };
                std::uint16_t e;
            };
        };

        union conditional_code_register {
            conditional_code_register() : byte(0) {}

            std::uint8_t byte;
            struct {
                std::uint8_t carry : 1;
                std::uint8_t over_flow : 1;
                std::uint8_t zero : 1;
                std::uint8_t negative : 1;
                std::uint8_t user : 1;
                std::uint8_t half_carry : 1;
                std::uint8_t user_interrupt : 1;
                std::uint8_t interrupt_mask: 1;
            };
        };

        class h8_300 {

            enum class value_place : std::uint8_t
            {
                high, 
                low,
            };

            public:
                h8_300(std::array<std::uint8_t, ROM_SIZE>&& mem);

            private:
                // registers
                register_t er[7];
                union {
                    register_t er7; // er[7];
                    std::uint32_t sp;
                };

                conditional_code_register ccr;
                std::uint32_t pc;

                mcu memory;

            public:
                auto cycle(void) -> bool;
                auto reset_exception(void) -> void;
                
            private:
                auto detect_operation(void) -> operation;

                auto register_write_immediate(std::uint8_t destination,
                        std::uint32_t immediate,
                        register_size size) -> bool;
                auto register_write_register(std::uint8_t destination,
                        std::uint8_t source,
                        register_size size) -> bool;

                //for ccr
                auto update_ccr_sub(std::uint32_t value_0, std::uint32_t value_1, 
                        std::uint64_t result, register_size size) -> void;
                auto update_ccr_mov(std::uint64_t value, register_size size) -> void;
                auto update_ccr_shll(std::uint64_t value, register_size size) -> void;

                //memory
                auto read_register_fields(std::uint32_t address, value_place place, bool is_32bit)
                    -> std::uint8_t;
                auto read_immediate(std::uint32_t address, std::uint8_t number_of_byte)
                    -> std::uint32_t;
                auto write_immediate(std::uint32_t base, std::uint8_t number_of_byte, 
                        std::uint32_t immediate) 
                    -> void;

                //register
                auto read_register(std::uint8_t source, register_size size) -> std::uint32_t;
                auto write_register(std::uint8_t destination, 
                        std::uint32_t value, register_size size) -> void;



            // use macro for test
            public:

                // for utility function
                FRIEND_TEST(read_register, 0);
                FRIEND_TEST(read_register, 1);
                FRIEND_TEST(read_imm, 0);

//                 FRIEND_TEST(cpu, ADD_B_IMM);
//                 FRIEND_TEST(cpu, ADD_B_R_R);
//                 FRIEND_TEST(cpu, ADD_W_IMM);
//                 FRIEND_TEST(cpu, ADD_W_R_R);
//                 FRIEND_TEST(cpu, ADD_L_IMM);
//                 FRIEND_TEST(cpu, ADD_L_R_R);

                FRIEND_TEST(MOV_B_IMM, 0);
                FRIEND_TEST(MOV_B_IMM, 1);
                FRIEND_TEST(MOV_B_R_IND, 0);
                FRIEND_TEST(MOV_W_IMM, 0);
                FRIEND_TEST(MOV_L_IMM, 0);
                FRIEND_TEST(JSR_ABS, 0);
                FRIEND_TEST(MOV_L_R_IND, 0);
                FRIEND_TEST(MOV_L_R_R, 0);
                FRIEND_TEST(EXTS_L, 0);
                FRIEND_TEST(SHLL_L, 0);
                FRIEND_TEST(MOV_L_IND_WITH_DIS_24_R, 0);
                FRIEND_TEST(SUB_B_R_R, 0);
                FRIEND_TEST(MOV_B_IND_WITH_DIS_16_R, 0);
                FRIEND_TEST(MOV_B_R_IND_WITH_DIS_16, 0);
                FRIEND_TEST(SUB_W_R_R, 0);
                FRIEND_TEST(SUB_W_R_R, 1);
                FRIEND_TEST(RTS, 0);
                FRIEND_TEST(MOV_L_R_IND_POST_INC, 0);
                FRIEND_TEST(BEQ, 0);
                FRIEND_TEST(BRA, 0);
                FRIEND_TEST(MOV_B_R_IND_POST_INC, 0);
                FRIEND_TEST(MOV_B_R_R, 0);
                FRIEND_TEST(CMP_B_IMM, 0);
                FRIEND_TEST(CMP_B_IMM, 1);
                FRIEND_TEST(BNE, 0);
                FRIEND_TEST(SUB_WITH_SIGN_EXT_4, 0);
                FRIEND_TEST(MOV_W_R_R, 0);
                FRIEND_TEST(EXTS_L, 1);
                FRIEND_TEST(AND_W, 0);
                FRIEND_TEST(AND_W, 1);
                FRIEND_TEST(register, 0);
                FRIEND_TEST(AND_B_IMM, 0);
                FRIEND_TEST(ADDS_4, 0);
                FRIEND_TEST(SUB_L_R_R, 0);
                FRIEND_TEST(ADD_B_IMM_R, 0);
                FRIEND_TEST(ADD_L_IMM_R, 0);
                FRIEND_TEST(SUB_WITH_SIGN_EXT_1, 0);
//                 FRIEND_TEST(MOV_B_IND_WITH_DIS_24_R, 0);

        };

        //         std::uint8_t std::uint8_t::operator [](std::uint32_t) {
        //             return this[std::uint32_t];
        //         };
    }  // namespace cpu
}  // namespace narcissus
