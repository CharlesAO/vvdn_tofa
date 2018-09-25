/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvAggregateCostSGBM64_clamp_asm,@function
.version 00.51.04

.data .data.aggregateCostSGBM64_clamp
.align 4

.code .text.aggregateCostSGBM64_clamp

.lalign
//void mvcvAggregateCostSGBM64_clamp(u8* input1, u8* input2, u8* output, u16 penaltyP1, u16 penaltyP2, u8 min2, u8 activateClamp)//
//                                        i18         i17        i16         i15             i14 		  i13		i12	
mvcvAggregateCostSGBM64_clamp_asm:
	 
lsu0.ldi.128.u8.u16 v8 i18     	 				// v2 = penaltyP1
lsu0.ldi.128.u8.u16 v1 i18  || lsu1.ldil i1, 0x00ff     	 					// v1 = min2
lsu0.ldi.128.u8.u16 v2 i18  || iau.and i13 i13 i1	// v0 = penaltyP2 + min2
lsu0.ldi.128.u8.u16 v3 i18  || iau.add i0 i14 i13
lsu0.ldi.128.u8.u16 v4 i18  || cmu.cpivr.x16 v0 i0              							//v28 -mask used for (u8)result part
lsu0.ldi.128.u8.u16 v5 i18  || cmu.vszm.byte i1,i12,[000d]
lsu0.ldi.128.u8.u16 v6 i18  || vau.xor v16,v16,v16
lsu0.ldi.128.u8.u16 v7 i18  || vau.add.i16 v16,v8,v16 || LSU1.SWZV8  [65432101]  
nop
cmu.alignvec v9 v8 v1 2
cmu.alignvec v11 v1 v2 2
cmu.alignvec v17 v8 v1 14
cmu.alignvec v13 v2 v3 2
cmu.alignvec v18 v1 v2 14
cmu.alignvec v15 v3 v4 2
cmu.alignvec v19 v2 v3 14
cmu.min.u16 v8 v8 v0  //min_a
cmu.min.u16 v10 v1 v0  //min_a
cmu.min.u16 v12 v2 v0  //min_a
cmu.min.u16 v14 v3 v0  || lsu0.cp v2.0 i15//min_a
cmu.min.u16 v16  v16  v9 
cmu.min.u16 v17  v17  v11 
cmu.min.u16 v18  v18  v13 
cmu.min.u16 v19  v19  v15 

vau.add.i16 v16 v2 v16  || LSU0.SWZV8  [00000000] // + penaltyP1
vau.add.i16 v17 v2 v17  || LSU0.SWZV8  [00000000]  || cmu.cpivr.x16 v1 i13// + penaltyP1
vau.add.i16 v18 v2 v18  || LSU0.SWZV8  [00000000]  || cmu.min.u16 v16  v16  v8// + penaltyP1
vau.add.i16 v19 v2 v19  || LSU0.SWZV8  [00000000]  || cmu.min.u16 v17  v17  v10// + penaltyP1
cmu.min.u16 v18  v18  v12
cmu.min.u16 v19  v19  v14
nop
cmu.alignvec v9 v4 v5 2  || vau.sub.i16 v16 v16 v1 
cmu.alignvec v20 v3 v4 14|| vau.sub.i16 v17 v17 v1
cmu.alignvec v11 v5 v6 2 || vau.sub.i16 v18 v18 v1 
cmu.alignvec v21 v4 v5 14|| vau.sub.i16 v19 v19 v1 
cmu.alignvec v13 v6 v7 2
cmu.alignvec v22 v5 v6 14
cmu.alignvec v23 v6 v7 14

cmu.min.u16 v8 v4 v0     || vau.xor v15,v15,v15//min_a
cmu.min.u16 v10 v5 v0    || vau.add.i16 v15,v7,v15  || LSU0.SWZV8  [67654321]   //min_a
cmu.min.u16 v12 v6 v0    || lsu1.ldi.128.u8.u16 v8 i17 	//min_a
cmu.min.u16 v14 v7 v0    || lsu1.ldi.128.u8.u16 v9 i17  	//min_a
cmu.min.u16 v20  v20  v9  || lsu1.ldi.128.u8.u16 v10 i17
cmu.min.u16 v21  v21  v11 || lsu1.ldi.128.u8.u16 v11 i17
cmu.min.u16 v22  v22  v13 || vau.add.i16 v20 v2 v20  || LSU0.SWZV8  [00000000] || lsu1.ldi.128.u8.u16 v12 i17// + penaltyP1
cmu.min.u16 v23  v23  v15 || vau.add.i16 v21 v2 v21  || LSU0.SWZV8  [00000000] || lsu1.ldi.128.u8.u16 v13 i17// + penaltyP1
cmu.min.u16 v20  v20  v8  || vau.add.i16 v22 v2 v22  || LSU0.SWZV8  [00000000] || lsu1.ldi.128.u8.u16 v14 i17// + penaltyP1
cmu.min.u16 v21  v21  v10 || vau.add.i16 v23 v2 v23  || LSU0.SWZV8  [00000000] || lsu1.ldi.128.u8.u16 v15 i17// + penaltyP1
cmu.min.u16 v22  v22  v12 || vau.sub.i16 v20 v20 v1 
cmu.min.u16 v23  v23  v14 || vau.sub.i16 v21 v21 v1

vau.sub.i16 v22 v22 v1  
vau.sub.i16 v23 v23 v1 
vau.add.i16 v8 v8 v16 
vau.add.i16 v9 v9 v17 
vau.add.i16 v10 v10 v18 
vau.add.i16 v11 v11 v19
vau.add.i16 v12 v12 v20 
vau.add.i16 v13 v13 v21 
vau.add.i16 v14 v14 v22 || cmu.cpivr.x16 v7 i1  
vau.add.i16 v15 v15 v23
nop
vau.and v8,v8,v7
vau.and v9,v9,v7       ||  lsu0.sti.128.u16.u8 v8 i16
vau.and v10,v10,v7     ||  lsu0.sti.128.u16.u8 v9 i16
vau.and v11,v11,v7     ||  lsu0.sti.128.u16.u8 v10 i16 || BRU.jmp i30
vau.and v12,v12,v7     ||  lsu0.sti.128.u16.u8 v11 i16
vau.and v13,v13,v7     ||  lsu0.sti.128.u16.u8 v12 i16
vau.and v14,v14,v7     ||  lsu0.sti.128.u16.u8 v13 i16
vau.and v15,v15,v7     ||  lsu0.sti.128.u16.u8 v14 i16
lsu0.st.128.u16.u8 v15 i16
nop

.size mvcvAggregateCostSGBM64_clamp_asm,.-mvcvAggregateCostSGBM64_clamp_asm
.end
