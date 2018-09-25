///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvConvolution1x9_asm,@function
.version 00.70.00

.data .data.Convolution1x9
.align 16
 convolution1x9_clampVal:
        .float16 255.0


.code .text.Convolution1x9_asm
.align 16

//void Convolution1x9_asm(u8 **in(i18), u8 **out(i17), half* filter(i16), u32 inWidth(i15))
mvcvConvolution1x9_asm:  
  
	//CMU.CPTI    i14 P_CFG
	//lsu0.ldil i13, 0x0042     ||  lsu1.ldih i13, 0x0000
	//IAU.OR i14 i14 i13
	//CMU.CPIT   P_CFG i14
	 
  
	LSU0.LDIL i1 0X00 || LSU1.LDIH i1 0X00
	LSU0.LDi.32 i0 	i18
	LSU0.LDi.32 i17 i17	
	LSU0.LDIL i7 convolution1x9_clampVal || LSU1.LDIH i7 convolution1x9_clampVal
	LSU0.LDO.64.L v3 i16 0x00
	LSU0.LDO.64.H v3 i16 0x08
	LSU0.LDO.64.L v4 i16 0x10
	LSU0.LD.16R v21 i7
	IAU.SUB i0 i0 8	
	LSU1.LDi.128.u8.f16 v0 i0	 
	LSU1.LDi.128.u8.f16 v1 i0	
	NOP 4
	LSU1.LDi.128.u8.f16 v1 i0
	NOP
	CMU.ALIGNVEC v22 v0 v1 8 
	
	
 convolution1x9_loop:
	VAU.MACPZ.f16 v3 v22	|| LSU0.SWZV8 [00000000] 	|| CMU.ALIGNVEC v22 v0 v1 10
	VAU.MACP.f16  v3 v22 	|| LSU0.SWZV8 [11111111] 	|| CMU.ALIGNVEC v22 v0 v1 12
	VAU.MACP.f16  v3 v22 	|| LSU0.SWZV8 [22222222] 	|| CMU.ALIGNVEC v22 v0 v1 14
	VAU.MACP.f16  v3 v22 	|| LSU0.SWZV8 [33333333] 	|| CMU.CPVV v0 v1
	VAU.MACP.f16  v3 v0 	|| LSU0.SWZV8 [44444444] 	|| CMU.ALIGNVEC v22 v0 v1 2
	VAU.MACP.f16  v3 v22 	|| LSU0.SWZV8 [55555555] 	|| CMU.ALIGNVEC v22 v0 v1 4
	VAU.MACP.f16  v3 v22 	|| LSU0.SWZV8 [66666666] 	|| CMU.ALIGNVEC v22 v0 v1 6
	VAU.MACP.f16  v3 v22 	|| LSU0.SWZV8 [77777777] 	|| CMU.ALIGNVEC v22 v0 v1 8
	VAU.MACPW.f16 v2 v4 v22 || LSU0.SWZV8 [00000000]
	
	CMU.CLAMP0.f16 v2 v2 v21
	IAU.SUB i15 i15 8	
	PEU.PC1I NEQ || BRU.BRA convolution1x9_loop
	LSU0.ST.128.f16.u8 v2 i17 || IAU.ADD i17 i17 i1 || LSU1.LDIL i1 0x08
	NOP 2
	LSU1.LDi.128.u8.f16 v1 i0
	NOP 2
	
	
    BRU.JMP i30
	CMU.CLAMP0.f16 v2 v2 v21 
	NOP
	LSU0.ST.128.f16.u8 v2 i17 
	NOP 3
  
  
.size mvcvConvolution1x9_asm,.-mvcvConvolution1x9_asm
.end
