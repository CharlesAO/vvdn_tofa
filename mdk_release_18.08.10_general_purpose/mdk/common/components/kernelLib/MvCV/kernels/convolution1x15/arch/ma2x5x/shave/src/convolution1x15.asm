///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvConvolution1x15_asm,@function
.version 00.70.00

.code .text.Convolution1x15_asm
.align 16

//void Convolution1x15_asm(u8 **in(i18), u8 **out(i17), half* filter(i16), u32 inWidth(i15))
mvcvConvolution1x15_asm:

	
	LSU0.LDi.32 i0 	i18
	LSU0.LDi.32 i17 i17	
	LSU0.LDO.64.L v3 i16 0x00
	LSU0.LDO.64.H v3 i16 0x08
	LSU0.LDO.64.L v4 i16 0x10
	LSU0.LDO.64.H v4 i16 0x18
  NOP
	IAU.SUB i0 i0 8	
	LSU1.LDi.128.u8f.f16 v0  i0	 
	LSU1.LDi.128.u8f.f16 v1  i0	
  iau.xor i10 i10 i10
	NOP 5
	CMU.ALIGNVEC v22 v0 v1 2 
	
	
 convolution1x15_loop:
.nowarn 10
	VAU.MACPZ.f16 v3 v22	|| LSU0.SWZV8 [00000000] 	|| CMU.ALIGNVEC v22 v0 v1 4		|| LSU1.LDi.128.u8f.f16 v1 i0
.nowarnend
	VAU.MACP.f16  v3 v22 	|| LSU0.SWZV8 [11111111] 	|| CMU.ALIGNVEC v22 v0 v1 6
	VAU.MACP.f16  v3 v22 	|| LSU0.SWZV8 [22222222] 	|| CMU.ALIGNVEC v22 v0 v1 8
	VAU.MACP.f16  v3 v22 	|| LSU0.SWZV8 [33333333] 	|| CMU.ALIGNVEC v22 v0 v1 10
	VAU.MACP.f16  v3 v22 	|| LSU0.SWZV8 [44444444] 	|| CMU.ALIGNVEC v22 v0 v1 12
	VAU.MACP.f16  v3 v22 	|| LSU0.SWZV8 [55555555] 	|| CMU.ALIGNVEC v22 v0 v1 14
	VAU.MACP.f16  v3 v22 	|| LSU0.SWZV8 [66666666] 	|| CMU.CPVV v0 v1
	VAU.MACP.f16  v3 v0 	|| LSU0.SWZV8 [77777777] 	|| CMU.ALIGNVEC v22 v0 v1 2
	VAU.MACP.f16  v4 v22 	|| LSU0.SWZV8 [00000000] 	|| CMU.ALIGNVEC v22 v0 v1 4
	VAU.MACP.f16  v4 v22 	|| LSU0.SWZV8 [11111111] 	|| CMU.ALIGNVEC v22 v0 v1 6
	VAU.MACP.f16  v4 v22 	|| LSU0.SWZV8 [22222222] 	|| CMU.ALIGNVEC v22 v0 v1 8
	VAU.MACP.f16  v4 v22 	|| LSU0.SWZV8 [33333333] 	|| CMU.ALIGNVEC v22 v0 v1 10
	VAU.MACP.f16  v4 v22 	|| LSU0.SWZV8 [44444444] 	|| CMU.ALIGNVEC v22 v0 v1 12
	VAU.MACP.f16  v4 v22 	|| LSU0.SWZV8 [55555555] 	|| CMU.ALIGNVEC v22 v0 v1 14
	VAU.MACPW.f16 v2 v4 v22 || LSU0.SWZV8 [66666666] || cmu.cmz.i32 i10
	
	peu.pc1c GT || LSU0.STi.128.f16.u8f v2 i17
	
	IAU.SUB i15 i15 8
	PEU.PC1I NEQ || BRU.BRA convolution1x15_loop
	CMU.ALIGNVEC v22 v0 v1 2 || iau.incs i10 1
	NOP 5
	
	
  BRU.JMP i30
	LSU0.STi.128.f16.u8f v2 i17
	NOP 5

.size mvcvConvolution1x15_asm,.-mvcvConvolution1x15_asm
.end
