///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.version 00.70.00

.data .data.Convolution1x7
.align 16
 .Lconvolution1x7_clampVal:
        .float16 255.0

.code .text.Convolution1x7_asm
.align 16

//void Convolution1x7_asm(u8 **in(i18), u8 **out(i17), half* filter(i16), u32 inWidth(i15))C
    .type Convolution1x7_step4,@function
Convolution1x7_step4:
	LSU0.LDi.32 i0 	i18 || 	LSU1.LDi.32 i17 i17	
	LSU0.LDIL i7 .Lconvolution1x7_clampVal || LSU1.LDIH i7 .Lconvolution1x7_clampVal
	LSU0.LDIL i4 .Lconvolution1x7_loop || 	LSU1.LDIH i4 .Lconvolution1x7_loop || 	iau.shr.u32 i15 i15 3
	LSU0.LDO.64.L v3 i16 0x00 || LSU1.LDO.64.H v3 i16 0x08
	LSU0.LD.16R v23 i7 
       nop 2
       IAU.SUB i0 i0 8	
	   LSU1.LDi.128.u8f.f16 v0 i0	 
	   LSU1.LDi.128.u8f.f16 v1 i0	
       nop 4
//Step 0
	   LSU1.LDi.128.u8f.f16 v2 i0 
	   nop 6
       CMU.ALIGNVEC v22 v0 v1 10 
	   CMU.ALIGNVEC v21 v0 v1 12
	   CMU.ALIGNVEC v20 v0 v1 14
	   CMU.ALIGNVEC v19 v1 v2 2
	   CMU.ALIGNVEC v18 v1 v2 4
	   vau.mul.f16 v13 v3 v1  || LSU0.SWZV8 [33333333] || CMU.ALIGNVEC v17 v1 v2 6   || LSU1.LDi.128.u8f.f16 v2 i0 	
//Step 1
        vau.mul.f16 v16 v3 v22 || LSU0.SWZV8 [00000000] || lsu1.cp v0 v1
        vau.mul.f16 v15 v3 v21 || LSU0.SWZV8 [11111111] || lsu1.cp v1 v2
        vau.mul.f16 v14 v3 v20 || LSU0.SWZV8 [22222222] || CMU.ALIGNVEC v22 v0 v1 10 
        vau.mul.f16 v12 v3 v19 || LSU0.SWZV8 [44444444] || CMU.ALIGNVEC v21 v0 v1 12
        vau.mul.f16 v11 v3 v18 || LSU0.SWZV8 [55555555] || CMU.ALIGNVEC v20 v0 v1 14
        vau.mul.f16 v10 v3 v17 || LSU0.SWZV8 [66666666] 
        vau.add.f16 v9 v16 v15 || CMU.ALIGNVEC v19 v1 v2 2
        vau.add.f16 v8 v14 v13 || CMU.ALIGNVEC v18 v1 v2 4
//Step 3
        vau.add.f16 v7 v12 v11 || bru.rpl i4 i15
        vau.mul.f16 v13 v3 v1  || LSU0.SWZV8 [33333333] || CMU.ALIGNVEC v17 v1 v2 6   || LSU1.LDi.128.u8f.f16 v2 i0 	
        vau.add.f16 v6 v9 v8
        vau.add.f16 v5 v7 v10 
        vau.mul.f16 v16 v3 v22 || LSU0.SWZV8 [00000000] || lsu1.cp v0 v1
        vau.mul.f16 v15 v3 v21 || LSU0.SWZV8 [11111111] || lsu1.cp v1 v2
        .Lconvolution1x7_loop:
.nowarn 10
        vau.add.f16 v4 v5 v6
.nowarnend
        vau.mul.f16 v14 v3 v20 || LSU0.SWZV8 [22222222] || CMU.ALIGNVEC v22 v0 v1 10 
        vau.mul.f16 v12 v3 v19 || LSU0.SWZV8 [44444444] || CMU.ALIGNVEC v21 v0 v1 12
        vau.mul.f16 v11 v3 v18 || LSU0.SWZV8 [55555555] || CMU.ALIGNVEC v20 v0 v1 14 || LSU1.STi.128.f16.u8f v4 i17
        vau.mul.f16 v10 v3 v17 || LSU0.SWZV8 [66666666]
        vau.add.f16 v9 v16 v15 || CMU.ALIGNVEC v19 v1 v2 2
        vau.add.f16 v8 v14 v13 || CMU.ALIGNVEC v18 v1 v2 4

    BRU.JMP i30
    nop 6
    .size Convolution1x7_step4, .-Convolution1x7_step4

.end
