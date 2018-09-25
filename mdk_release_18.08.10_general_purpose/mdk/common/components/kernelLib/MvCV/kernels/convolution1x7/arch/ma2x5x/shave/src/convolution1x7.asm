///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvConvolution1x7_asm,@function
.version 00.70.00

.data .data.Convolution1x7
.align 16
 convolution1x7_clampVal:
        .float16 255.0

.code .text.Convolution1x7_asm
.align 16

//void Convolution1x7_asm(u8 **in(i18), u8 **out(i17), half* filter(i16), u32 inWidth(i15))
mvcvConvolution1x7_asm:
	LSU0.LDIL i10 0x0 
	LSU0.LDIH i10 0x0
	LSU0.LDi.32 i0 	i18
	LSU0.LDi.32 i17 i17	
	LSU0.LDIL i7 convolution1x7_clampVal 
	LSU0.LDIH i7 convolution1x7_clampVal
	LSU0.LDO.64.L v3 i16 0x00
	LSU0.LDO.64.H v3 i16 0x08
	LSU0.LD.16R v21 i7
	IAU.SUB i0 i0 8	
	LSU1.LDi.128.u8.f16 v0 i0	 
	LSU1.LDi.128.u8.f16 v1 i0	
	nop
	NOP 2
	LSU1.LDi.128.u8.f16 v1 i0
	NOP 2
	CMU.ALIGNVEC v22 v0 v1 10 
	
	
 convolution1x7_loop:
.nowarn 10
	VAU.MACPZ.f16 v3 v22	|| LSU0.SWZV8 [00000000] 	|| CMU.ALIGNVEC v22 v0 v1 12
.nowarnend
	VAU.MACP.f16  v3 v22 	|| LSU0.SWZV8 [11111111] 	|| CMU.ALIGNVEC v22 v0 v1 14
	VAU.MACP.f16  v3 v22 	|| LSU0.SWZV8 [22222222] 	|| CMU.CPVV v0 v1
	VAU.MACP.f16  v3 v0 	|| LSU0.SWZV8 [33333333] 	|| CMU.ALIGNVEC v22 v0 v1 2
	VAU.MACP.f16  v3 v22 	|| LSU0.SWZV8 [44444444] 	|| CMU.ALIGNVEC v22 v0 v1 4
	VAU.MACP.f16  v3 v22 	|| LSU0.SWZV8 [55555555] 	|| CMU.ALIGNVEC v22 v0 v1 6	
	VAU.MACPW.f16 v2 v3 v22 || LSU0.SWZV8 [66666666]
	
	CMU.CLAMP0.f16 v2 v2 v21
	IAU.SUB i15 i15 8	
	PEU.PC1I NEQ || BRU.BRA convolution1x7_loop
	LSU0.ST.128.f16.u8 v2 i17		|| IAU.ADD i17 i17 i10
	CMU.ALIGNVEC v22 v0 v1 10       || LSU0.LDIL i10 8
	LSU1.LDi.128.u8.f16 v1 i0
	NOP 3
	
	
    BRU.JMP i30
	CMU.CLAMP0.f16 v2 v2 v21 
	NOP
	LSU0.ST.128.f16.u8 v2 i17
	NOP 3



.size mvcvConvolution1x7_asm,.-mvcvConvolution1x7_asm
.end
