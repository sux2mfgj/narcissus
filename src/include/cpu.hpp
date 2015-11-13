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
            ADD_L_R_R,

            SUB_B_R_R,                      // substruct rd from rs
            SUB_W_R_R,
            SUB_L_R_R,
            SUB_L_IMM_R,

            SUB_WITH_SIGN_EXT_1,
//             SUB_WITH_SIGN_EXT_2,
            SUB_WITH_SIGN_EXT_4,            // subtract 4 with sign extention

            ADD_B_IMM_R,

            ADD_L_IMM_R,

            ADDS_1,                     // add 1 with sign extention
//             ADDS_2,                     // add 2 with sign extention
            ADDS_4,                     // add 4 with sign extention
            
            MOV_B_IMM,
            MOV_B_R_R,  
            MOV_B_R_R_IND,              // register indirect (@erd = rs)
            MOV_B_R_IND_R,              // register indirect (rd = @ers)  
            MOV_B_R_IND_WITH_DIS_16,    // register indirect with displacement(16 bit)
            MOV_B_IND_WITH_DIS_16_R,    // register indirect with displacement(16 bit)
            MOV_B_IND_WITH_DIS_24_R,    // register indirect with displacement(24 bit)
            MOV_B_R_IND_POST_INC,       // rd increment after register indirect acdess
//             MOV_B_ASB_8,                // absolute addressing 8
//             MOV_B_ASB_16,               // absolute addressing 16
//             MOV_B_ASB_24,               // absolute addressing 24

            MOV_W_IMM,
            MOV_W_R_R,
            MOV_W_ASB_24_R,         
            MOV_W_R_ABS_24,
            MOV_W_R_IND_WITH_DIS_16,
            MOV_W_IND_WITH_DIS_16_R,


            MOV_L_IMM, 
            MOV_L_R_R,
            MOV_L_R_IND_PRE_DEC,

            MOV_L_IND_WITH_DIS_16_R,
            MOV_L_IND_WITH_DIS_24_R,    // MEMORY to register
//             MOV_L_R_IND_WITH_DIS_24, // register to memory
            MOV_L_R_IND_POST_INC,
            MOV_L_IMM_ABS_24_R, 
            MOV_L_R_IMM_ABS_24,
            MOV_L_R_IND_WITH_DIS_16,

            BEQ_8,                        // branch equal
            BEQ_16,                        // branch equal
            BRA_8,                        // branch always(true)
            BRA_16,                        // branch always(true)
            BNE_8,                        // branch not equal
            BNE_16,                        // branch not equal
            BLE_8,                      // branch less or equal
//             BLE_16,                      // branch less or equal
            BLS_8,                     // branch low or same
//             BLS_16,                     // branch low or same
            BGT_8,                  // greater or equal
//             BGT_16,
            BGE_8,                  //branch greater or equal
//             BGE_16,

            CMP_B_IMM,                  // compare register and immediate
            CMP_B_R_R,                  // compare register and  register
            CMP_L_IMM,

//             DEC_B,                   // decrement
            DEC_W_1,                      
//             DEC_W_2,                      
//             DEC_L,

            AND_W,                      // AND logical
            AND_B_IMM,

            SHLL_L,                     // shift logical left
            SHLR_L,                     // shift logical right

            JSR_ABS,                    // jump to subroutine use absolute address
            EXTS_L,                     // extend as signed

            EXTU_W,                     // extend as unsigned

            RTS,

            XOR_B_R_R,                  //exclusive or logical direct registers
        };

        enum class register_size : std::uint32_t
        { 
            BYTE = 7, 
            WORD = 15, 
            LONG = 31
        };

        union register_t {
            register_t() : er(0) {}

            std::uint32_t er;

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
                virtual ~h8_300() = default;;

            private:
                h8_300(h8_300 const&) = delete;
                h8_300(h8_300&&) = delete;
                h8_300& operator =(h8_300 const&) = delete;
                h8_300& operator =(h8_300&&) = delete;

            private:
                // registers
                std::array<register_t, 7> er;
                union {
                    register_t er7; // er[7];
                    std::uint32_t sp;
                };

                conditional_code_register ccr;
                std::uint32_t pc;

                mcu memory;

            public:
                auto cycle(void) -> std::uint32_t;
                auto reset_exception(void) -> void;
                auto quick_exit(void) -> void;

            private:
                auto detect_operation(void) -> operation;

                //for ccr
                auto update_ccr_sub(std::uint32_t value_0, std::uint32_t value_1, 
                        std::uint64_t result, register_size size) -> void;
                auto update_ccr_mov(std::uint64_t value, register_size size) -> void;
                auto update_ccr_shll(std::uint64_t value, register_size size) -> void;
                auto update_ccr_shlr(std::uint32_t value, register_size size) -> void;

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

            public:
                friend class cpu_debuger;

            // use macro for test
            public:

                // for utility function
                FRIEND_TEST(read_register, 0);
                FRIEND_TEST(read_register, 1);
                FRIEND_TEST(read_imm, 0);
                FRIEND_TEST(write_imm, 0);
                FRIEND_TEST(read_value, 0);
                FRIEND_TEST(write_register, 0);


                // for instruction
//                 FRIEND_TEST(cpu, ADD_B_IMM);
//                 FRIEND_TEST(cpu, ADD_B_R_R);
//                 FRIEND_TEST(cpu, ADD_W_IMM);
//                 FRIEND_TEST(cpu, ADD_W_R_R);
//                 FRIEND_TEST(cpu, ADD_L_IMM);
                FRIEND_TEST(ADD_L_R_R, 0);

                FRIEND_TEST(MOV_B_IMM, 0);
                FRIEND_TEST(MOV_B_IMM, 1);
                FRIEND_TEST(MOV_W_IMM, 0);
                FRIEND_TEST(MOV_L_IMM, 0);
                FRIEND_TEST(JSR_ABS, 0);
                FRIEND_TEST(MOV_L_R_IND_PRE_DEC, 0);
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
                FRIEND_TEST(BEQ_8, 0);
                FRIEND_TEST(BRA_8, 0);
                FRIEND_TEST(MOV_B_R_IND_POST_INC, 0);
                FRIEND_TEST(MOV_B_R_R, 0);
                FRIEND_TEST(CMP_B_IMM, 0);
                FRIEND_TEST(CMP_B_IMM, 1);
                FRIEND_TEST(BNE_8, 0);
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
                FRIEND_TEST(MOV_B_IND_WITH_DIS_24_R, 0);
                FRIEND_TEST(SHLR_L, 0);
                FRIEND_TEST(DEC_W_1, 0);
                FRIEND_TEST(MOV_B_R_IND_R, 0);
                FRIEND_TEST(MOV_B_R_R_IND, 0);
                FRIEND_TEST(ADDS_1, 0);
                FRIEND_TEST(SUB_L_IMM_R, 0);
                FRIEND_TEST(BLE_8, 0);
                FRIEND_TEST(MOV_W_ASB_24_R, 0);
                FRIEND_TEST(MOV_W_R_ABS_24, 0);
                FRIEND_TEST(EXTU_W, 0);
                FRIEND_TEST(CMP_B_R_R, 0);
                FRIEND_TEST(BLS_8, 0);
                FRIEND_TEST(BNE_16, 0);
                FRIEND_TEST(MOV_L_IMM_ABS_24_R, 0);
                FRIEND_TEST(BRA_16, 0);
                FRIEND_TEST(MOV_L_R_IMM_ABS_24, 0);
                FRIEND_TEST(MOV_L_R_IND_WITH_DIS_16, 0);
                FRIEND_TEST(MOV_W_R_IND_WITH_DIS_16, 0);
                FRIEND_TEST(MOV_W_IND_WITH_DIS_16_R, 0);
                FRIEND_TEST(BGE_8, 0);
                FRIEND_TEST(BEQ_16, 0);
                FRIEND_TEST(CMP_L_IMM, 0);
                FRIEND_TEST(MOV_L_IND_WITH_DIS_16_R, 0);
                FRIEND_TEST(XOR_B_R_R, 0);
                FRIEND_TEST(BGT_8, 0);
        };

    }  // namespace cpu
}  // namespace narcissus
