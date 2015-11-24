#pragma once

#include <cstdint>
#include <array>
#include <condition_variable>
#include <thread>
#include <memory>

#include <gtest/gtest.h>

#include <mcu.hpp>

namespace narcissus {
    namespace h8_3069f {

        enum class operation : std::uint8_t
        {
            INVALID = 0,

            // mov.b
            MOV_B_IMM_R,
            MOV_B_R_R,  
            MOV_B_R_IND_R,              // register indirect (rd = @ers)  
            MOV_B_R_IND_WITH_DIS_16_R,    // register indirect with displacement(16 bit)
            MOV_B_R_IND_WITH_DIS_24_R,    // register indirect with displacement(24 bit)
            MOV_B_R_IND_POST_INC_R,       // rd increment after register indirect acdess
//             MOV_B_ABS_8_R,                // absolute addressing 8
//             MOV_B_ABS_16_R,               // absolute addressing 16
//             MOV_B_ABS_24_R,               // absolute addressing 24
            MOV_B_R_R_IND,              // register indirect (@erd = rs)
            MOV_B_R_R_IND_WITH_DIS_16,    // register indirect with displacement(16 bit)
            MOV_B_R_R_IND_WITH_DIS_24,    // register indirect with displacement(16 bit)
//             MOV_B_R_R_IND_PRE_DEC,
//             MOV_B_R_ABS_8,
//             MOV_B_R_ABS_16,
//             MOV_B_R_ABS_24,

            // mov.w
            MOV_W_IMM_R,
            MOV_W_R_R,  
//             MOV_W_R_IND_R,              // register indirect (rd = @ers)  
            MOV_W_R_IND_WITH_DIS_16_R,    // register indirect with displacement(16 bit)
//             MOV_W_R_IND_WITH_DIS_24_R,    // register indirect with displacement(24 bit)
//             MOV_W_R_IND_POST_INC_R,       // rd increment after register indirect acdess
//             MOV_W_ABS_8_R,                // absolute addressing 8
//             MOV_W_ABS_16_R,               // absolute addressing 16
            MOV_W_ABS_24_R,               // absolute addressing 24
//             MOV_W_R_R_IND,              // register indirect (@erd = rs)
            MOV_W_R_R_IND_WITH_DIS_16,    // register indirect with displacement(16 bit)
//             MOV_W_R_R_IND_WITH_DIS_24,    // register indirect with displacement(16 bit)
//             MOV_W_R_R_IND_PRE_DEC,
//             MOV_W_R_ABS_8,
//             MOV_W_R_ABS_16,
            MOV_W_R_ABS_24,

            // mov.l
            MOV_L_IMM_R,
            MOV_L_R_R,  
            MOV_L_R_IND_R,              // register indirect (rd = @ers)  
            MOV_L_R_IND_WITH_DIS_16_R,    // register indirect with displacement(16 bit)
            MOV_L_R_IND_WITH_DIS_24_R,    // register indirect with displacement(24 bit)
            MOV_L_R_IND_POST_INC_R,       // rd increment after register indirect acdess
//             MOV_L_ABS_8_R,                // absolute addressing 8
//             MOV_L_ABS_16_R,               // absolute addressing 16
            MOV_L_ABS_24_R,               // absolute addressing 24
            MOV_L_R_R_IND,              // register indirect (@erd = rs)
            MOV_L_R_R_IND_WITH_DIS_16,    // register indirect with displacement(16 bit)
            MOV_L_R_R_IND_WITH_DIS_24,    // register indirect with displacement(16 bit)
            MOV_L_R_R_IND_PRE_DEC,
//             MOV_L_R_ABS_8,
//             MOV_L_R_ABS_16,
            MOV_L_R_ABS_24,

            // pop/push
            //POP_W_R
            //POP_L_R
            //PUSH_W_R
            //PUSH_L_R

            // movfpe
            //MOVFPE_IMM_16_R
            //MOVTPE_R_IMM_16

            // add
            ADD_B_IMM_R,              // immediate
            ADD_B_R_R,              // register to register
            ADD_W_IMM_R,
//             ADD_W_R_R,
            ADD_L_IMM_R,
            ADD_L_R_R,

            // addx
            //ADDX_B_IMM_R
            //ADDX_B_R_R
            
            // adds
            ADDS_1,                     // add 1 with sign extention
//             ADDS_2,                     // add 2 with sign extention
            ADDS_4,                     // add 4 with sign extention

            // inc
            //INC_B,
            //INC_W_1_R,
            //INC_W_2_R,
            //INC_L_1_R,
            //INC_L_2_R,

            // daa
            //DAA_R

            //sub
            SUB_B_R_R,
            //SUB_W_IMM_R,
            SUB_W_R_R,
            SUB_L_IMM_R,
            SUB_L_R_R,

            // subx
            //SUBX_IMM_R,
            //SUBX_R_R,

            // subs
            SUBS_1,
            // SUBS_2,
            SUBS_4,

            // dec
//             DEC_B,                   // decrement
            DEC_W_1,                      
            DEC_W_2,                      
            DEC_L_1,
//             DEC_L_2,

            // das
            //DAS_R,

            // mulxu
            //MULXU_B_R_R,
            //MULXU_W_R_R,

            // mulxu
            //MULXS_B_R_R,
            MULXS_W_R_R,

            // cmp
            CMP_B_IMM,                  // compare register and immediate
            CMP_B_R_R,                  // compare register and  register
            CMP_W_IMM,
            CMP_W_R_R,
            CMP_L_IMM,
            CMP_L_R_R,

            //TODO [WIP] arrange enum
            // ----
//             SUB_WITH_SIGN_EXT_1,
//             SUB_WITH_SIGN_EXT_4,            // subtract 4 with sign extention

//             MULXS_B_R_R,
//             MULXS_W_R_R,                //multiply extend as signed
//             MULXS_L_R_R,

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
            BGE_16,

//             CMP_B_IMM,                  // compare register and immediate
//             CMP_B_R_R,                  // compare register and  register
//             CMP_W_R_R,
//             CMP_L_IMM,
//             CMP_L_R_R,

            AND_W_IMM,                      // AND logical
            AND_B_IMM,
            ANDC,

            SHLL_L,                     // shift logical left
            SHLR_L,                     // shift logical right
            SHAR_W,
//             SHAL_L,
//             SHAR_L,

            JSR_R,
            JSR_ABS,                    // jump to subroutine use absolute address
//             JSR_ABS_IND,
            EXTS_L,                     // extend as signed

            EXTU_W,                     // extend as unsigned

            RTS,                    // return from subroutine
            RTE,                    // return from exception
            NOP,

            XOR_B_R_R,                  //exclusive or logical direct registers
            XOR_B_IMM_R,
            OR_B_IMM_R,
//             OR_B_R_R
            ORC,            // inclusive or controle register
            SLEEP,
            TRAPA,
        };

        enum class register_size : std::uint32_t
        { 
            BYTE = 7, 
            WORD = 15, 
            LONG = 31
        };

        union register_t 
        {
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

        union conditional_code_register 
        {
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

        enum class interrupts : std::uint8_t
        {
            reset = 0,
            nmi = 7,
            // 8 ~ 11
            trap_0,
            trap_1,
            trap_2,
            trap_3,
            // 12 ~ 17
            external_0, 
            external_1, 
            external_2, 
            external_3, 
            external_4, 
            external_5, 
            // 52 ~ 63
            // sci0
            eri0 = 52,
            rxi0,
            txi0,
            tei0,
            // sci1
            eri1,
            rxi1,
            txi1,
            tei1,
            // sci2
            eri2,
            rxi2,
            txi2,
            tei2,
        };

        class cpu 
            : public std::enable_shared_from_this<cpu>
        {
            enum class value_place : std::uint8_t
            {
                high, 
                low,
            };

            private:
                class create_helper;

            public:
                static std::shared_ptr<cpu> create(std::array<std::uint8_t, 
                        (std::uint32_t)mem_info::rom_size>&& mem);
                virtual ~cpu() = default;;

            private:
                cpu(std::array<std::uint8_t, (std::uint32_t)mem_info::rom_size>&& mem);
                cpu(cpu const&) = delete;
                cpu(cpu&&) = delete;
                cpu& operator =(cpu const&) = delete;
                cpu& operator =(cpu&&) = delete;

            private:
                // registers
                std::array<register_t, 7> er;
                union {
                    register_t er7; // er[7];
                    std::uint32_t sp;
                };

                conditional_code_register ccr;
                std::uint32_t pc;

                std::shared_ptr<bool> is_sleep;
                std::shared_ptr<std::condition_variable> c_variable_ptr;
                mcu memory;

                std::mutex cv_mutex;

            public:
                auto run(void) -> void;
                auto reset_exception(void) -> void;
                auto interrupt(interrupts int_num) -> bool;

            private:
                auto cycle(void) -> std::uint32_t;
                auto detect_operation(void) -> operation;

                //for ccr
                auto update_ccr_sub(std::uint32_t value_0, std::uint32_t value_1, 
                        std::uint64_t result, register_size size) -> void;
                auto update_ccr_mov(std::uint64_t value, register_size size) -> void;
                auto update_ccr_shll(std::uint64_t value, register_size size) -> void;
                auto update_ccr_shlr(std::uint32_t value, register_size size) -> void;
                auto update_ccr_mulx(std::uint32_t result, register_size size) -> void;

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

            private:
                auto detect_mov_0_1_0(void) -> operation;
                auto detect_mov_6(std::uint8_t num) -> operation;
                auto detect_mov_7_8(void) -> operation;
                auto detect_mulx(void) -> operation;

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
                FRIEND_TEST(ADD_L_R_R, 0);
                FRIEND_TEST(MOV_B_IMM_R, 0);
                FRIEND_TEST(MOV_B_IMM_R, 1);
                FRIEND_TEST(MOV_W_IMM_R, 0);
                FRIEND_TEST(MOV_L_IMM_R, 0);
                FRIEND_TEST(JSR_ABS, 0);
                FRIEND_TEST(MOV_L_R_R_IND_PRE_DEC, 0);
                FRIEND_TEST(MOV_L_R_R, 0);
                FRIEND_TEST(EXTS_L, 0);
                FRIEND_TEST(SHLL_L, 0);
                FRIEND_TEST(MOV_L_R_IND_WITH_DIS_24_R, 0);
                FRIEND_TEST(SUB_B_R_R, 0);
                FRIEND_TEST(MOV_B_R_IND_WITH_DIS_16_R, 0);
                FRIEND_TEST(MOV_B_R_R_IND_WITH_DIS_16, 0);
                FRIEND_TEST(SUB_W_R_R, 0);
                FRIEND_TEST(SUB_W_R_R, 1);
                FRIEND_TEST(RTS, 0);
                FRIEND_TEST(MOV_L_R_IND_POST_INC_R, 0);
                FRIEND_TEST(BEQ_8, 0);
                FRIEND_TEST(BRA_8, 0);
                FRIEND_TEST(MOV_B_R_IND_POST_INC_R, 0);
                FRIEND_TEST(MOV_B_R_R, 0);
                FRIEND_TEST(CMP_B_IMM, 0);
                FRIEND_TEST(CMP_B_IMM, 1);
                FRIEND_TEST(BNE_8, 0);
                FRIEND_TEST(SUBS_4, 0);
                FRIEND_TEST(MOV_W_R_R, 0);
                FRIEND_TEST(EXTS_L, 1);
                FRIEND_TEST(AND_W_IMM, 0);
                FRIEND_TEST(AND_W_IMM, 1);
                FRIEND_TEST(register, 0);
                FRIEND_TEST(AND_B_IMM, 0);
                FRIEND_TEST(ADDS_4, 0);
                FRIEND_TEST(SUB_L_R_R, 0);
                FRIEND_TEST(ADD_B_IMM_R, 0);
                FRIEND_TEST(ADD_L_IMM_R, 0);
                FRIEND_TEST(SUBS_1, 0);
                FRIEND_TEST(MOV_B_R_IND_WITH_DIS_24_R, 0);
                FRIEND_TEST(SHLR_L, 0);
                FRIEND_TEST(DEC_W_1, 0);
                FRIEND_TEST(MOV_B_R_IND_R, 0);
                FRIEND_TEST(MOV_B_R_R_IND, 0);
                FRIEND_TEST(ADDS_1, 0);
                FRIEND_TEST(SUB_L_IMM_R, 0);
                FRIEND_TEST(BLE_8, 0);
                FRIEND_TEST(MOV_W_ABS_24_R, 0);
                FRIEND_TEST(MOV_W_R_ABS_24, 0);
                FRIEND_TEST(EXTU_W, 0);
                FRIEND_TEST(CMP_B_R_R, 0);
                FRIEND_TEST(BLS_8, 0);
                FRIEND_TEST(BNE_16, 0);
                FRIEND_TEST(MOV_L_ABS_24_R, 0);
                FRIEND_TEST(BRA_16, 0);
                FRIEND_TEST(MOV_L_R_ABS_24, 0);
                FRIEND_TEST(MOV_L_R_R_IND_WITH_DIS_16, 0);
                FRIEND_TEST(MOV_W_R_R_IND_WITH_DIS_16, 0);
                FRIEND_TEST(MOV_W_R_IND_WITH_DIS_16_R, 0);
                FRIEND_TEST(BGE_8, 0);
                FRIEND_TEST(BEQ_16, 0);
                FRIEND_TEST(CMP_L_IMM, 0);
                FRIEND_TEST(MOV_L_R_IND_WITH_DIS_16_R, 0);
                FRIEND_TEST(XOR_B_R_R, 0);
                FRIEND_TEST(BGT_8, 0);
                FRIEND_TEST(MOV_L_R_R_IND, 0);
                FRIEND_TEST(ADD_B_R_R, 0);
                FRIEND_TEST(MOV_L_R_IND_R, 0);
                FRIEND_TEST(CMP_L_R_R, 0);
                FRIEND_TEST(XOR_B_IMM_R, 0);
                FRIEND_TEST(DEC_W_2, 0);
                FRIEND_TEST(DEC_L_1, 0);
                FRIEND_TEST(ADD_W_IMM_R, 0);
                FRIEND_TEST(SHAR_W, 0);
                FRIEND_TEST(CMP_W_R_R, 0);
                FRIEND_TEST(BGE_16, 0);
                FRIEND_TEST(MULXS_W_R_R, 0);
                FRIEND_TEST(JSR_R, 0);
                FRIEND_TEST(ORC, 0);
                FRIEND_TEST(MOV_L_R_R_IND_WITH_DIS_24, 0);
                FRIEND_TEST(CMP_W_IMM, 0);
                FRIEND_TEST(OR_B_IMM_R, 0);
                FRIEND_TEST(ANDC, 0);
                FRIEND_TEST(SLEEP, 0);
                FRIEND_TEST(MOV_B_R_R_IND_WITH_DIS_24, 0);
                FRIEND_TEST(RTE, 0);
                FRIEND_TEST(TRAPA, 0);
        };

        class cpu::create_helper : public cpu
        {
            public:

                create_helper(std::array<std::uint8_t, (std::uint32_t)mem_info::rom_size>&& mem)
                    : cpu(std::move(mem))
                {}
        };

    }  // namespace h8_3069f

}  // namespace narcissus
