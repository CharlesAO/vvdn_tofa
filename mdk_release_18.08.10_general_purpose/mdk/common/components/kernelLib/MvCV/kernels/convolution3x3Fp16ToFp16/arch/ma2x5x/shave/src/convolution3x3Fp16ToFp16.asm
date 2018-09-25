///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvConvolution3x3Fp16ToFp16_asm,@function
.version 00.51.04

//.data .data.convolution3x3Fp16ToFp16
//.align 4

//.align 16
 //convolution3x3Fp16ToFp16_clampVal:
  //      .float16 255.0


.code .text.convolution3x3Fp16ToFp16

//void Convolution3x3Fp16ToFp16_asm(half** in(i18), half** out(i17), half conv[9](i16), u32 inWidth(i15))
//internal computation are made on fp16, output result is fp16
mvcvConvolution3x3Fp16ToFp16_asm:
	LSU0.LDIL i12 ___loop       || LSU1.LDIH i12 ___loop
    LSU0.LDI.64  i0  i18  		|| IAU.SUB i10 i10 i10
	LSU0.LDI.32  i2  i18   		|| IAU.SUB i11 i11 i11
	LSU0.LDO.64.L v9 i16 0x00 	|| LSU1.LDO.64.H v9 i16 0x08
	LSU0.LDO.64.L v10 i16 0x10 	|| IAU.SHR.U32 i15 i15 3
	LSU1.LD.32 i17 i17
	nop 2
	IAU.SUB i0 i0 0x10
	IAU.SUB i1 i1 0x10
	IAU.SUB i2 i2 0x10
	
	LSU0.LD.64.L v0  i0			|| LSU1.LDO.64.H v0 i0  0x08	
	LSU0.LD.64.L v1  i1			|| LSU1.LDO.64.H v1 i1  0x08	|| IAU.ADD i0 i0 0x10
	LSU0.LD.64.L v2  i2   		|| LSU1.LDO.64.H v2 i2  0x08	|| IAU.ADD i1 i1 0x10
	LSU0.LD.64.L v3  i0			|| LSU1.LDO.64.H v3 i0  0x08	|| IAU.ADD i2 i2 0x10
	LSU0.LD.64.L v4  i1			|| LSU1.LDO.64.H v4 i1  0x08	|| IAU.ADD i0 i0 0x10
	LSU0.LD.64.L v5  i2 		|| LSU1.LDO.64.H v5 i2  0x08	|| IAU.ADD i1 i1 0x10
	LSU0.LD.64.L v6  i0 		|| LSU1.LDO.64.H v6 i0  0x08	|| IAU.ADD i2 i2 0x10
	LSU0.LD.64.L v7  i1 		|| LSU1.LDO.64.H v7 i1  0x08	|| IAU.ADD i0 i0 0x10
	LSU0.LD.64.L v8  i2 		|| LSU1.LDO.64.H v8 i2  0x08	|| IAU.ADD i1 i1 0x10
    nop 2
    CMU.ALIGNVEC v11 v0 v3 14   || IAU.ADD i2 i2 0x10
// start loop

	VAU.MACPZ.f16 v9 v11        || LSU1.SWZV8 [00000000]  		|| CMU.CPVV v0 v3					|| BRU.RPL i12 i15
	VAU.MACP.f16  v9 v0         || LSU1.SWZV8 [11111111]  		|| CMU.ALIGNVEC v11 v3 v6 2
	VAU.MACP.f16  v9 v11        || LSU1.SWZV8 [22222222]  		|| CMU.ALIGNVEC v11 v1 v4 14
	VAU.MACP.f16  v9 v11        || LSU1.SWZV8 [33333333]  		|| CMU.CPVV v1 v4
	VAU.MACP.f16  v9 v1         || LSU1.SWZV8 [44444444]  		|| CMU.ALIGNVEC v11 v4 v7, 2
	VAU.MACP.f16  v9 v11        || LSU1.SWZV8 [55555555]  		|| CMU.ALIGNVEC v11 v2 v5 14
	VAU.MACP.f16  v9 v11        || LSU1.SWZV8 [66666666]  		|| CMU.CPVV v2 v5
	VAU.MACP.f16  v9 v2         || LSU1.SWZV8 [77777777]  		|| CMU.ALIGNVEC v11 v5 v8 2
	VAU.MACPW.f16 v12 v10 v11   || LSU1.SWZV8 [00000000]  		|| CMU.CPVV v3 v6
	
	LSU0.LD.64.L v6 i0		    || LSU1.LDO.64.H v6 i0 0x08     || CMU.CPVV v4 v7 
	LSU0.LD.64.L v7 i1		    || LSU1.LDO.64.H v7 i1 0x08     || CMU.CPVV v5 v8 || IAU.ADD i0 i0 0x10
___loop:	                    
.nowarn 10
	LSU0.LD.64.L v8 i2		    || LSU1.LDO.64.H v8 i2 0x08		|| IAU.ADD i1 i1 0x10
.nowarnend
	CMU.CMII.i32 i10 i11        || IAU.ADD i2 i2 0x10
	PEU.PC1C NEQ 			    || LSU0.ST.64.L v12 i17  		|| LSU1.STO.64.H v12 i17 0x08       || IAU.ADD i17 i17 0x10
                                
	IAU.ADD i10 i10 1
    CMU.ALIGNVEC v11 v0 v3 14 
    NOP 2
    nop 6
    LSU0.ST.64.L v12 i17  	    || LSU1.STO.64.H v12 i17 0x08   									|| IAU.ADD i17 i17 0x10
	BRU.JMP i30
	
	nop 6
.size mvcvConvolution3x3Fp16ToFp16_asm,.-mvcvConvolution3x3Fp16ToFp16_asm
.end
