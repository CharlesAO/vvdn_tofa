/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvComputeADPyrOnePos_asm,@function
.version 00.51.04

.data .data.mvcvComputeADPyrOnePos
.align 4

.code .text.mvcvComputeADPyrOnePos

.lalign
//void mvcvmvcvComputeADPyrOnePos_asm_test(unsigned char *input1, unsigned char *input2, unsigned char *output, unsigned char min2, unsigned char penaltyP2)//
//                                                  i18                  i17                     i16                   i15                    i14
mvcvComputeADPyrOnePos_asm:

LSU0.ldil i0, mvcvComputeADPyrOnePos_asm_loop || LSU1.ldih i0, mvcvComputeADPyrOnePos_asm_loop



LSU0.LDi.128.u8.u16   v0  i18   	|| iau.shr.u32 i15 i15 3 
iau.sub i17 i17 8
LSU0.LD.128.u8.u16   v1  i17   || IAU.ADD i17 i17 8	 
LSU0.LD.128.u8.u16   v2  i17   || IAU.ADD i17 i17 8	 
LSU0.LD.128.u8.u16   v3  i17   	 
nop 5
cmu.alignvec v4 v1 v2 14 || vau.xor v6 v6 v6  || LSU0.LDi.128.u8.u16   v0  i18   	 
cmu.alignvec v5 v2 v3 2                       || iau.sub i17 i17 8
VAU.ADIFF.u16 v19 v0 v4                       || LSU0.LD.128.u8.u16   v1  i17   || IAU.ADD i17 i17 8
VAU.ADIFF.u16 v9 v0 v2                        || LSU0.LD.128.u8.u16   v2  i17   || IAU.ADD i17 i17 8
VAU.ADIFF.u16 v20 v0 v5                       || LSU0.LD.128.u8.u16   v3  i17 


CMU.VILV.x16 v19 v9 v9  v19
bru.rpl i0 i15
CMU.VILV.x16 v20 v8  v6 v20
CMU.VILV.x32  v19 v20   v20 v19
CMU.VILV.x32 v9  v8  v8 v9
cmu.cpvv  v7 v19  || LSU0.SWZC8 [65421033] 
cmu.cpvv  v23 v20 || LSU0.SWZC8 [65423333] 
cmu.alignvec v4 v1 v2 14 || vau.xor v6 v6 v6  || LSU0.LDi.128.u8.u16   v0  i18   	 
 mvcvComputeADPyrOnePos_asm_loop:
cmu.alignvec v5 v2 v3 2                       || iau.sub i17 i17 8
cmu.cpvv  v17 v9  || LSU1.SWZC8 [65334210] ||   VAU.ADIFF.u16 v19 v0 v4                       || LSU0.LD.128.u8.u16   v1  i17   || IAU.ADD i17 i17 8
cmu.cpvv  v18 v8  || LSU1.SWZC8 [33654210] ||    VAU.ADIFF.u16 v9 v0 v2                       || LSU0.LD.128.u8.u16   v2  i17   || IAU.ADD i17 i17 8
cmu.alignvec v14 v7 v20 4                  ||  VAU.ADIFF.u16 v20 v0 v5                        || LSU0.LD.128.u8.u16   v3  i17 

cmu.alignvec v15 v23 v17 8      || LSU1.sti.128.u16.u8   v14  i16             
cmu.alignvec v16 v17 v18 12     || LSU1.sti.128.u16.u8   v15  i16     
                                   LSU1.sti.128.u16.u8   v16  i16 || CMU.VILV.x16 v19 v9 v9  v19


BRU.JMP i30
nop 6

.size mvcvComputeADPyrOnePos_asm,.-mvcvComputeADPyrOnePos_asm
.end
