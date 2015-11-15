instruction list
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
    - [x] MOV.B Rs, @ERd
            : MOV_B_R_R_IND
    - [x] MOV.B Rs, @(d:16, ERd)
            : MOV_B_R_R_IND_WITH_DIS_16

* mov.w
    
    - [x] MOV.W #xx:16, Rd
            : MOV_W_IMM_R
    - [x] MOV.W Rs, Rd
            : MOV_W_R_R
    - [x] MOV.W @ERs, Rd
            : MOV_W_R_IND_R
    - [x] MOV.W @aa:24, Rd
            : MOV_W_ABS_24_R
    - [x] MOV.W Rs, @aa:24
            : MOV_W_R_ABS_24
    - [x] MOV.W Rs, @(d:16, ERd)
            : MOV_W_R_IND_WITH_DIS_16
    - [ ] MOV.W Rs, @(d:24, ERd)
            : MOV_W_R_IND_WITH_DIS_24
    - [x] MOV.W @(d:16, ERs), Rd
            : MOV_W_IND_WITH_DIS_16_R

* mov.l

    - [x] MOV.L #xx:32, ERd
            : MOV_L_IMM_L
    - [x] MOV.L Rs, Rd
            : MOV_L_R_R
    - [x] MOV.L @ERs, ERd
            : MOV_L_R_IND_R




