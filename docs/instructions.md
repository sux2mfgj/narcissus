implemented
---

* mov.b
    
    - [x] MOV.B #xx:8, Rd           
            : MOV_B_IMM_R
    - [x] MOV.B Rs, Rd  
            : MOV_B_R_R
    - [x] MOV.B @ERs, Rd  
            : MOV_B_R_IND_R
    - [x] MOV.B @(d:16, ERs), Rd  
            : MOV_B_R_IND_WITH_DIS_16_R
    - [x] MOV.B @(d:24, ERs), Rd  
            : MOV_B_R_IND_WITH_DIS_24_R
    - [x] MOV.B @ERs+, Rd  
            : MOV_B_R_IND_POST_INC_R,
    - [ ] MOV.B @aa:8, Rd  
            : MOV_B_ABS_8_R
    - [ ] MOV.B @aa:16, Rd  
            : MOV_B_ABS_16_R
    - [ ] MOV.B @aa:24, Rd  
            : MOV_B_ABS_24_R
    - [x] MOV.B Rs, @ERd  
            : MOV_B_R_R_IND
    - [x] MOV.B Rs, @(d:16, ERd)  
            : MOV_B_R_R_IND_WITH_DIS_16
    - [ ] MOV.B Rs, @(d:24, ERd)  
            : MOV_B_R_R_IND_WITH_DIS_24
    - [ ] MOV.B Rs, @-ERd
            : MOV_B_R_R_IND_PRE_DEC
    - [ ] MOV.B Rs, @aa:8
            : MOV_B_R_ABS_8
    - [ ] MOV.B Rs, @aa:16
            : MOV_B_R_ABS_16
    - [ ] MOV.B Rs, @aa:24
            : MOV_B_R_ABS_24

* mov.w
    
    - [x] MOV.W #xx:8, Rd           
            : MOV_W_IMM_R
    - [x] MOV.W Rs, Rd  
            : MOV_W_R_R
    - [ ] MOV.W @ERs, Rd  
            : MOV_W_R_IND_R
    - [x] MOV.W @(d:16, ERs), Rd  
            : MOV_W_R_IND_WITH_DIS_16_R
    - [ ] MOV.W @(d:24, ERs), Rd  
            : MOV_W_R_IND_WITH_DIS_24_R
    - [ ] MOV.W @ERs+, Rd  
            : MOV_W_R_IND_POST_INC_R,
    - [ ] MOV.W @aa:8, Rd  
            : MOV_W_ABS_8_R
    - [ ] MOV.W @aa:16, Rd  
            : MOV_W_ABS_16_R
    - [x] MOV.W @aa:24, Rd  
            : MOV_W_ABS_24_R
    - [ ] MOV.W Rs, @ERd  
            : MOV_W_R_R_IND
    - [x] MOV.W Rs, @(d:16, ERd)  
            : MOV_W_R_R_IND_WITH_DIS_16
    - [ ] MOV.W Rs, @(d:24, ERd)  
            : MOV_W_R_R_IND_WITH_DIS_24
    - [ ] MOV.W Rs, @-ERd
            : MOV_W_R_R_IND_PRE_DEC
    - [ ] MOV.W Rs, @aa:8
            : MOV_W_R_ABS_8
    - [ ] MOV.W Rs, @aa:16
            : MOV_W_R_ABS_16
    - [x] MOV.W Rs, @aa:24
            : MOV_W_R_ABS_24

* mov.l
    
    - [x] MOV.L #xx:8, Rd           
            : MOV_L_IMM_R
    - [x] MOV.L Rs, Rd  
            : MOV_L_R_R
    - [x] MOV.L @ERs, Rd  
            : MOV_L_R_IND_R
    - [x] MOV.L @(d:16, ERs), Rd  
            : MOV_L_R_IND_WITH_DIS_16_R
    - [x] MOV.L @(d:24, ERs), Rd  
            : MOV_L_R_IND_WITH_DIS_24_R
    - [x] MOV.L @ERs+, Rd  
            : MOV_L_R_IND_POST_INC_R,
    - [ ] MOV.L @aa:8, Rd  
            : MOV_L_ABS_8_R
    - [ ] MOV.L @aa:16, Rd  
            : MOV_L_ABS_16_R
    - [x] MOV.L @aa:24, Rd  
            : MOV_L_ABS_24_R
    - [x] MOV.L Rs, @ERd  
            : MOV_L_R_R_IND
    - [x] MOV.L Rs, @(d:16, ERd)  
            : MOV_L_R_R_IND_WITH_DIS_16
    - [ ] MOV.L Rs, @(d:24, ERd)  
            : MOV_L_R_R_IND_WITH_DIS_24
    - [x] MOV.L Rs, @-ERd
            : MOV_L_R_R_IND_PRE_DEC
    - [ ] MOV.L Rs, @aa:8
            : MOV_L_R_ABS_8
    - [ ] MOV.L Rs, @aa:16
            : MOV_L_R_ABS_16
    - [x] MOV.L Rs, @aa:24
            : MOV_L_R_ABS_24


* mov.l

    - [x] MOV.L #xx:32, ERd  
            : MOV_L_IMM_L
    - [x] MOV.L Rs, Rd  
            : MOV_L_R_R
    - [x] MOV.L ERs, @ERd  
            : MOV_L_R_R_IND
    - [x] MOV.L @ERs, ERd  
            : MOV_L_R_IND_R
    - [x] MOV.L @ERs, ERd  
            : MOV_L_R_IND_R
    - [x] MOV.L ERs, @-ERd  
            : MOV_L_R_R_IND_PRE_DEC
    - [x] MOV.L @(d:16, ERs), ERd  
            : MOV_L_R_IND_WITH_DIS_16_R
    - [x] MOV.L @(d:24, ERs), ERd  
            : MOV_L_R_IND_WITH_DIS_24_R
    - [x] MOV.L @ERs+, ERd  
            : MOV_L_R_IND_POST_INC_R
    - [x] MOV.L @aa:24, ERd  
            : MOV_L_IMM_ABS_24_R
    - [x] MOV.L ERs, @aa:24  
            : MOV_L_R_IMM_ABS_24
    - [x] MOV.L ERs, @(d:16, ERd)  
            : MOV_L_R_R_IND_WITH_DIS_16

* pop/push
    - [ ] POP.W Rn
            : POP_W_R
    - [ ] POP.L ERn
            : POP_L_R
    - [ ] PUSH.W Rn
            : PUSH_W_R
    - [ ] PUSH.L ERn
            : PUSH_L_R

* movfpe
    - [ ] MOVFPE @aa:16, Rd
            : MOVFPE_IMM_16_R
* movtpe
    - [ ] MOVTPE Rs, @aa:16
            : MOVTPE_R_IMM_16

* add
    - [x] ADD.B #xx:8, Rd
            : ADD_B_IMM_R
    - [x] ADD.B Rs, Rd
            : ADD_B_R_R
    - [x] ADD.W #xx:16, Rd
            : ADD_W_IMM_R
    - [ ] ADD.W Rs, Rd
            : ADD_W_R_R
    - [x] ADD.L #xx:32, ERd
            : ADD_L_IMM_R
    - [x] ADD.L ERs, ERd
            : ADD_L_R_R

* addx
    - [ ] ADDX.B #xx:8, Rd
            : ADDX_B_IMM_R
    - [ ] ADDX.B Rs, Rd
            : ADDX_B_R_R

* adds
    - [x] ADDS.L #1 ERd
            : ADDS_1
    - [ ] ADDS.L #2 ERd
            : ADDS_2
    - [x] ADDS.L #4 ERd
            : ADDS_4

* inc
    - [ ] INC.B Rd
            : INC_B
    - [ ] INC.W #1, Rd
            : INC_W_1_R
    - [ ] INC.W #2, Rd
            : INC_W_2_R
    - [ ] INC.L #1, Rd
            : INC_L_1_R
    - [ ] INC.L #2, Rd
            : INC_L_2_R
* daa
    - [ ] DAA Rd
            : DAA_R

* sub
    - [x] SUB.B Rs, Rd
            : SUB_B_R_R
    - [ ] SUB.W #xx:16, Rd
            : SUB_W_IMM_R
    - [x] SUB.W Rs, Rd
            : SUB_W_R_R
    - [x] SUB.L #xx:32, ERd
            : SUB_L_IMM_R
    - [x] SUB.L ERs, ERd
            : SUB_L_R_R

* subx
    - [ ] SUBX #xx:8, Rd
            : SUBX_IMM_R
    - [ ] SUBX Rs, Rd
            : SUBX_R_R

* subs
    - [x] SUBS #1, ERd
            : SUBS_1
    - [ ] SUBS #2, ERd
            : SUBS_2
    - [x] SUBS #4, ERd
            : SUBS_4

* dec
    - [ ] DEC.B Rd
            : DEC_B
    - [x] DEC.W #1, Rd
            : DEC_W_1
    - [x] DEC.W #2, Rd
            : DEC_W_2
    - [x] DEC.L #1, Rd
            : DEC_L_1
    - [ ] DEC.L #2, Rd
            : DEC_L_2

* das
    - [ ] DAS Rd
            : DAS_R

* mulxu
    - [ ] MULXU.B Rs, Rd
            : MULXU_B_R_R
    - [ ] MULXU.W Rs, Rd
            : MULXU_W_R_R

* mulxs
    - [ ] MULXS.B Rs, Rd
            : MULXS_B_R_R
    - [x] MULXS.W Rs, Rd
            : MULXS_W_R_R

* cmp
    - [x] CMP.B #xx:8, Rd
            : CMP_B_IMM
    - [x] CMP.B Rs, Rd
            : CMP_B_R_R
    - [ ] CMP.W #xx:16, Rd
            : CMP_W_IMM
    - [x] CMP.W Rs, Rd
            : CMP_W_R_R
    - [x] CMP.L #xx:32, Rd
            : CMP_L_IMM
    - [x] CMP.L Rs, Rd
            : CMP_L_R_R
 
WIP

* bcc
    - [x] beq d:8  
            : BEQ_8
    - [x] beq d:16  
            : BEQ_16
    - [x] bra d:8  
            : BRA_8
    - [x] bra d:16  
            : BRA_16
    - [x] bne d:8  
            : BNE_8
    - [x] bne d:16  
            : BNE_16
    - [x] ble d:8  
            : BLE_8
    - [ ] ble d:16  
            : BLE_16
    - [x] bls d:8  
            : BLS_8
    - [ ] bls d:16  
            : BLS_16
    - [x] bgt d:8  
            : BGT_8
    - [ ] bgt d:16  
            : BGT_16
    - [x] bge d:8  
            : BGE_8
    - [ ] bge d:16  
            : BGE_16
 
* cmp
    - [x] CMP.B #xx:8 Rd  
            : CMP_B_IMM
    - [x] CMP.B Rd, Rd  
            : CMP_B_R_R
    - [ ] CMP.W #xx:16 Rd  
            : CMP_W_IMM
    - [ ] CMP.W Rd, Rd  
            : CMP_W_R_R
    - [x] CMP.L #xx:32, ERd  
            : CMP_L_IMM
    - [x] CMP.L ERs, ERd  
            : CMP_L_R_R

* dec
    - [ ] DEC.B Rd  
            : DEC_B
    - [x] DEC.W #1, Rd  
            : DEC_W_1
    - [ ] DEC.W #2, Rd  
            : DEC_W_2
    - [ ] DEC.L #1, ERd  
            : DEC_L_1
    - [ ] DEC.L #1, ERd  
            : DEC_L_1

* and
    - [x] AND.B #xx:8, Rd  
            : AND_B_IMM
    - [ ] AND.B Rs, Rd  
            : AND_B_R_R
    - [x] AND.W #xx:16, Rd  
            : AND_W_IMM
    - [ ] AND.W Rs, Rd  
            : AND_W_R_R
    - [ ] AND.L #xx:32, ERd  
            : AND_L_IMM
    - [ ] AND.L ERs, ERd  
            : AND_L_R_R

* shift

    - [ ] SHAL.B Rd  
            : SHAL_B
    - [ ] SHAL.W Rd  
            : SHAL_W
    - [ ] SHAL.L Rd  
            : SHAL_L
    - [ ] SHAR.B Rd  
            : SHAR_B
    - [ ] SHAR.W Rd  
            : SHAR_W
    - [ ] SHAR.L Rd  
            : SHAR_L
    - [ ] SHLL.B Rd  
            : SHLL_B
    - [ ] SHLL.W Rd  
            : SHLL_W
    - [x] SHLL.L ERd  
            : SHLL_L
    - [ ] SHRL.B Rd  
            : SHLR_B
    - [ ] SHRL.W Rd  
            : SHLR_W
    - [x] SHRL.L ERd  
            : SHLR_L

* jmp
    - [ ] JMP @ERn  
            : JMP_R
    - [ ] JMP @aa:24  
            : JMP_ABS
    - [ ] JMP @@aa:8  
            : JMP_IND_IMM

    - [ ] JSR @ERn  
            : JSR_R
    - [x] JSR @aa:24  
            : JSR_ABS
    - [ ] JSR @@aa:8  
            : JSR_IND_IMM

    - [x] RTS  
            : RTS

* exts
    - [ ] EXTS.W Rd  
            : EXTS_W 
    - [x] EXTS.L ERd  
            : EXTS_L 

    - [x] EXTU.W Rd  
            : EXTU_W
    - [ ] EXTU.L ERd  
            : EXTU_L

* xor
    - [x] XOR.B #xx:8, Rd  
            : XOR_B_IMM_R
    - [x] XOR.B Rs, Rd  
            : XOR_B_R_R
    - [ ] XOR.W #xx:8, Rd  
            : XOR_W_IMM_R
    - [ ] XOR.W Rs, Rd  
            : XOR_W_R_R
    - [ ] XOR.L #xx:8, ERd  
            : XOR_L_IMM_R
    - [ ] XOR.L ERs, ERd  
            : XOR_L_R_R
