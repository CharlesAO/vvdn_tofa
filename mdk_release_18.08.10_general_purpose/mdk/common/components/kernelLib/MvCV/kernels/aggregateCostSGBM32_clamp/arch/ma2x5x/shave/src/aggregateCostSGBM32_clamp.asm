/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvAggregateCostSGBM32_clamp_asm,@function
.version 00.51.04

.data .data.aggregateCostSGBM32_clamp
.align 4

.code .text.aggregateCostSGBM32_clamp

.lalign
//void mvcvAggregateCostSGBM32_clamp(u8* input1, u8* input2, u8* output, u16 penaltyP1, u16 penaltyP2, u8 min2, u8 activateClamp)//
//                                        i18         i17        i16         i15             i14 		  i13		i12	
mvcvAggregateCostSGBM32_clamp_asm:

lsu0.ldil i1, 0x00ff 		|| lsu1.ldih i1, 0x0 
lsu0.ldi.128.u8.u16 v0 i18  || iau.and i13 i13 i1      || cmu.cpiv.x16 v23.0 i15.l    // v23 = penaltyP1
lsu0.ldi.128.u8.u16 v1 i18  || iau.add i0 i14 i13      || cmu.cpivr.x16 v22 i13 	  // v22 = min2
lsu0.ldi.128.u8.u16 v2 i18  || cmu.cpivr.x16 v21 i0  || vau.xor v12,v12,v12		  // v21 = penaltyP2 + min2
lsu0.ld.128.u8.u16 v3 i18   || cmu.vszm.byte i1,i12,[000d]                            
nop 3                                                                                 
vau.add.i16 v12,v0,v12      || LSU1.SWZV8  [65432101]  || cmu.cpivr.x16 v20 i1      //v20 -mask used for (u8)result part
cmu.alignvec v5 v0 v1 2     
cmu.alignvec v7 v1 v2 2     || vau.xor v11,v11,v11
cmu.alignvec v13 v0 v1 14   || vau.add.i16 v11,v3,v11  || LSU0.SWZV8  [67654321] 
cmu.alignvec v9 v2 v3 2
cmu.alignvec v14 v1 v2 14
cmu.alignvec v15 v2 v3 14
cmu.min.u16 v12  v12  v5 
cmu.min.u16 v13  v13  v7 
cmu.min.u16 v14  v14  v9   || vau.add.i16 v12 v23 v12  || LSU0.SWZV8  [00000000]
cmu.min.u16 v15  v15  v11  || vau.add.i16 v13 v23 v13  || LSU0.SWZV8  [00000000]
cmu.min.u16 v4 v0 v21      || vau.add.i16 v14 v23 v14  || LSU0.SWZV8  [00000000]       //min_a
cmu.min.u16 v6 v1 v21      || vau.add.i16 v15 v23 v15  || LSU0.SWZV8  [00000000]       //min_a
cmu.min.u16 v8 v2 v21                                                                  //min_a
cmu.min.u16 v10 v3 v21     || lsu1.ldi.128.u8.u16 v4 i17                               //min_a
cmu.min.u16 v12  v12  v4   || lsu1.ldi.128.u8.u16 v5 i17 
cmu.min.u16 v13  v13  v6   || lsu1.ldi.128.u8.u16 v6 i17
cmu.min.u16 v14  v14  v8   || vau.sub.i16 v12 v12 v22  || lsu1.ldi.128.u8.u16 v7 i17
cmu.min.u16 v15  v15  v10  || vau.sub.i16 v13 v13 v22
vau.sub.i16 v14 v14 v22
vau.sub.i16 v15 v15 v22
vau.add.i16 v4 v4 v12 
vau.add.i16 v5 v5 v13 
vau.add.i16 v6 v6 v14 
vau.add.i16 v7 v7 v15
nop
vau.and v4,v4,v20     || BRU.jmp i30
vau.and v5,v5,v20     || lsu0.sti.128.u16.u8 v4 i16
vau.and v6,v6,v20     || lsu0.sti.128.u16.u8 v5 i16
vau.and v7,v7,v20     || lsu0.sti.128.u16.u8 v6 i16
lsu0.st.128.u16.u8 v7 i16
nop 2


.size mvcvAggregateCostSGBM32_clamp_asm,.-mvcvAggregateCostSGBM32_clamp_asm
.end
