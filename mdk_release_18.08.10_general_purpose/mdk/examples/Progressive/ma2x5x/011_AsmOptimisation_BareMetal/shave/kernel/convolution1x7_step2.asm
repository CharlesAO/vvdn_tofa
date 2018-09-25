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
    .type Convolution1x7_step2,@function
Convolution1x7_step2:
	LSU0.LDi.32 i0 	i18 || 	LSU1.LDi.32 i17 i17	
	LSU0.LDIL i7 .Lconvolution1x7_clampVal || LSU1.LDIH i7 .Lconvolution1x7_clampVal
	LSU0.LDIL i4 .Lconvolution1x7_loop || 	LSU1.LDIH i4 .Lconvolution1x7_loop || 	iau.shr.u32 i15 i15 3
	LSU0.LDO.64.L v3 i16 0x00 || LSU1.LDO.64.H v3 i16 0x08
	LSU0.LD.16R v23 i7 
nop 2
        IAU.SUB i0 i0 8	
	LSU1.LDi.128.u8.f16 v0 i0	 
	LSU1.LDi.128.u8.f16 v1 i0	
        nop 4

	LSU1.LDi.128.u8.f16 v2 i0 || bru.rpl i4 i15
	nop 6
	CMU.ALIGNVEC v22 v0 v1 10 
	CMU.ALIGNVEC v21 v0 v1 12
	CMU.ALIGNVEC v20 v0 v1 14
	CMU.ALIGNVEC v19 v1 v2 2
	CMU.ALIGNVEC v18 v1 v2 4
	CMU.ALIGNVEC v17 v1 v2 6	

	VAU.MACPZ.f16 v3 v22	|| LSU0.SWZV8 [00000000]
	VAU.MACP.f16  v3 v21 	|| LSU0.SWZV8 [11111111] 	
	VAU.MACP.f16  v3 v20 	|| LSU0.SWZV8 [22222222] 	
	VAU.MACP.f16  v3 v1 	|| LSU0.SWZV8 [33333333] 	
	VAU.MACP.f16  v3 v19 	|| LSU0.SWZV8 [44444444] 	
	VAU.MACP.f16  v3 v18 	|| LSU0.SWZV8 [55555555] 
	VAU.MACPW.f16 v4 v3 v17 || LSU0.SWZV8 [66666666]
	nop 7
.Lconvolution1x7_loop:
nop 2
	CMU.CPVV v0 v1
	CMU.CPVV v1 v2
    CMU.CLAMP0.f16 v4 v4 v23
    nop
    LSU0.STi.128.f16.u8 v4 i17
	
    BRU.JMP i30
    nop 6
    .size Convolution1x7_step2, .-Convolution1x7_step2

.end
