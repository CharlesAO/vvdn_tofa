.type mvcvConvolution3x3_asm,@function
.version 00.51.05

.data .data.convolution3x3
.align 16
 convolution3x3_clampVal:
        .float16 255.0

.code .text.convolution3x3

//void Convolution3x3_asm(u8** in(i18), u8** out(i17), half conv[9](i16), u32 inWidth(i15))
mvcvConvolution3x3_asm:
.nowarn
	LSU0.LDi.64  i0  i18		 
    LSU0.LDi.32  i2  i18		 
	LSU1.LDO.64.L v0 i16 0x00
	LSU1.LDO.64.H v0 i16 0x08
	LSU0.LDO.64.L v1 i16 0x10
    nop
	IAU.SHR.i32 i15 i15 3	
	IAU.SUB i0 i0 0x08
	IAU.SUB i1 i1 0x08
	IAU.SUB i2 i2 0x08
	LSU0.LDi.128.u8f.f16 v2 i0	|| LSU1.LDi.128.u8f.f16 v3  i1
	LSU0.LDi.128.u8f.f16 v4 i2	|| IAU.ADD i15 i15 1
	LSU0.LDi.128.u8f.f16 v5 i0	
	LSU0.LDi.128.u8f.f16 v6 i1	|| LSU1.LDi.128.u8f.f16 v7  i2	
	LSU0.LDi.128.u8f.f16 v8 i0	
	LSU0.LDi.128.u8f.f16 v9 i1	|| LSU1.LDi.128.u8f.f16 v10 i2
	LSU0.LDiL i10 convolution3x3_loop 
	LSU1.LDiH i10 convolution3x3_loop
	LSU1.LD.32 i17 i17
	CMU.ALIGNVEC v11 v2  v5 14                
	CMU.ALIGNVEC v12 v3  v6 14	|| LSU0.LDiL i5, convolution3x3_clampVal
	CMU.ALIGNVEC v14 v5  v8 2	|| LSU1.LDiH i5, convolution3x3_clampVal
	CMU.ALIGNVEC v13 v4  v7 14	|| LSU0.LD.16r v21, i5
	CMU.ALIGNVEC v15 v6  v9 2	|| IAU.SUB i4 i4 i4
	CMU.ALIGNVEC v16 v7 v10 2	|| IAU.SUB i3 i3 i3
	
	
	
	VAU.MACPZ.f16 v0  v5 		|| LSU1.SWZV8 [11111111]	|| CMU.CPVV v2 v5	|| BRU.RPL i10 i15 _loop_Convolution3x3_asm 
	VAU.MACP.f16  v0  v6 		|| LSU1.SWZV8 [44444444] 	|| CMU.CPVV v3 v6
	VAU.MACP.f16  v0  v7 		|| LSU1.SWZV8 [77777777]	|| CMU.CPVV v4 v7    
	VAU.MACP.f16  v0  v11 		|| LSU1.SWZV8 [00000000]	|| LSU0.LDi.128.u8f.f16 v8  i0
	VAU.MACP.f16  v0  v14 		|| LSU1.SWZV8 [22222222]	|| LSU0.LDi.128.u8f.f16 v9  i1
	VAU.MACP.f16  v0  v12 		|| LSU1.SWZV8 [33333333]	|| LSU0.LDi.128.u8f.f16 v10 i2  	
	VAU.MACP.f16  v0  v15 		|| LSU1.SWZV8 [55555555]	|| CMU.CPVV v5 v8 
	VAU.MACP.f16  v0  v13 		|| LSU1.SWZV8 [66666666]    || CMU.CPVV v6 v9
	VAU.MACPW.f16 v20 v1 v16 	|| LSU1.SWZV8 [00000000]    || CMU.CPVV v7 v10  

	CMU.CMII.i32 i3 i4	
 convolution3x3_loop:
	peu.pc1c NEQ 				|| LSU0.STI.128.f16.u8f v20 i17 
	CMU.ALIGNVEC v11 v2 v5 14	|| IAU.ADD i3 i3 1	
	CMU.ALIGNVEC v12 v3 v6 14	
	CMU.ALIGNVEC v13 v4 v7 14	
	CMU.ALIGNVEC v14 v5 v8  2	
	CMU.ALIGNVEC v15 v6 v9  2
	CMU.ALIGNVEC v16 v7 v10 2
	_loop_Convolution3x3_asm: 	
		
		
	BRU.JMP i30
	NOP 6
	
.size mvcvConvolution3x3_asm,.-mvcvConvolution3x3_asm
.nowarnend
.end	
	
