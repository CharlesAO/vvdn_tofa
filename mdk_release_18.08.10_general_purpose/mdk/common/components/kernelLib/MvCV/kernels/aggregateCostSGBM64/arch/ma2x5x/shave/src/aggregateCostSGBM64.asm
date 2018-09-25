/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvAggregateCostSGBM64_asm,@function
.version 00.51.04

.data .data.aggregateCostSGBM64
.align 4

.code .text.aggregateCostSGBM64

.lalign
//void mvcvAggregateCostSGBM64_asm_test(unsigned char *input1, unsigned char *input2, unsigned char *output, unsigned char min2, unsigned char penaltyP2)//
//                                                  i18                  i17                     i16                   i15                    i14
mvcvAggregateCostSGBM64_asm:

LSU0.ldil i1, 0x00ff || LSU1.ldih i1, 0x0 || iau.xor i0 i0 i0// mask
LSU0.ldil i4, 0x000A || LSU1.ldih i4, 0x0 || IAU.AND i15 i15 i1 // 10 -PenaltyP1






lsu0.ldo.64.l v0 i18  0 || lsu1.ldo.64.h v0 i18  8 || IAU.AND i14 i14 i1 || cmu.cpivr.x8 v18 i4 
lsu0.ldo.64.l v1 i18 16 || lsu1.ldo.64.h v1 i18 24 || iau.add i0 i15 i14 || cmu.cpivr.x8 v19 i15
lsu0.ldo.64.l v2 i18 32 || lsu1.ldo.64.h v2 i18 40 || IAU.and i0 i0 i1 // safety for overflow case
lsu0.ldo.64.l v3 i18 48 || lsu1.ldo.64.h v3 i18 56 || cmu.cpivr.x8 v20 i0 
lsu0.ldo.64.l v14 i17  0 || lsu1.ldo.64.h v14 i17  8
lsu0.ldo.64.l v15 i17 16 || lsu1.ldo.64.h v15 i17 24
lsu0.ldo.64.l v16 i17 32 || lsu1.ldo.64.h v16 i17 40
lsu0.ldo.64.l v17 i17 48 || lsu1.ldo.64.h v17 i17 56 || cmu.alignvec v4 v0 v0 1
cmu.alignvec v5 v4 v0 15 //i-1
cmu.alignvec v6 v0 v1 1  //i+1
cmu.alignvec v7 v0 v1 15 //i-1
cmu.alignvec v8 v1 v2 1  //i+1
cmu.alignvec  v9 v1 v2 15 //i-1
cmu.alignvec v10 v2 v3 1  //i+1
cmu.alignvec v11 v2 v3 15 //i-1
cmu.alignvec v12 v3 v3 15
cmu.alignvec v13 v3 v12 1 //i+1
cmu.min.u8 v21 v0 v20 
cmu.min.u8 v22 v1 v20 
cmu.min.u8 v23 v2 v20 
cmu.min.u8 v0 v3 v20 
cmu.min.u8 v5  v5 v6 
cmu.min.u8 v7  v7 v8 
cmu.min.u8 v9  v9 v10  || vau.add.i8 v5  v5  v18
cmu.min.u8 v11 v11 v13 || vau.add.i8 v7  v7  v18
cmu.min.u8 v5  v5  v21 || vau.add.i8 v9  v9  v18
cmu.min.u8 v7  v7  v22 || vau.add.i8 v11 v11 v18
cmu.min.u8 v9  v9  v23 || vau.add.i8 v14 v14 v5  
cmu.min.u8 v11 v11 v0  || vau.add.i8 v15 v15 v7 
vau.add.i8 v16 v16 v9 
vau.add.i8 v17 v17 v11 || BRU.jmp i30
vau.sub.i8 v14 v14 v19 
vau.sub.i8 v15 v15 v19 
vau.sub.i8 v16 v16 v19 || lsu0.sto.64.l v14 i16  0 || lsu1.sto.64.h v14 i16  8
vau.sub.i8 v17 v17 v19 || lsu0.sto.64.l v15 i16 16 || lsu1.sto.64.h v15 i16 24
lsu0.sto.64.l v16 i16 32 || lsu1.sto.64.h v16 i16 40
lsu0.sto.64.l v17 i16 48 || lsu1.sto.64.h v17 i16 56




.size mvcvAggregateCostSGBM64_asm,.-mvcvAggregateCostSGBM64_asm
.end
