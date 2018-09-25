///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvSLaplacian5x5_asm,@function
.version 00.51.04

.data .data.slaplacian5x5
.align 16
 sLaplacian5x5_clampVal:
        .float16 255.0
 sLaplacian5x5_laplacian:
        .short 16

.code .text.slaplacian5x5

//void laplacian3x3(u8** in, u8** out, u32 inWidth)//
//					i18         i17       i16
mvcvSLaplacian5x5_asm:

	IAU.SUB i19 i19 8
	LSU0.ST.64.L v24  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.H v24  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.L v25  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.H v25  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.L v26  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.H v26  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.L v27  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.H v27  i19 // || IAU.SUB i19 i19 8

	

	LSU0.LDIL i5 sLaplacian5x5_clampVal || LSU1.LDIH i5 sLaplacian5x5_clampVal
	LSU0.LDIL i6 sLaplacian5x5_laplacian || LSU1.LDIH i6 sLaplacian5x5_laplacian
	lsu0.ldil i7 sLaplacian5x5_loop || lsu1.ldih i7 sLaplacian5x5_loop
		
    LSU0.LD.32  i0  i18 || IAU.ADD i18 i18 4 
    LSU0.LD.32  i1  i18 || IAU.ADD i18 i18 4
    LSU0.LD.32  i2  i18 || IAU.ADD i18 i18 4 
    LSU0.LD.32  i3  i18 || IAU.ADD i18 i18 4 
    LSU0.LD.32  i4  i18 || IAU.ADD i18 i18 4 
	LSU0.LD.32  i17  i17
nop
	
	LSU0.LD.16R v22 i6 || LSU1.LD.16R v23 i5 // lapl coef and clmp
	IAU.SHR.u32 i8 i16 3 // for loop
	
	iau.sub i1 i1 8
	iau.sub i2 i2 8
	iau.sub i3 i3 8
	
	LSU0.LDi.128.U8.U16 v0 i0 
	
	LSU0.LDi.128.U8.U16 v1 i1 
	LSU0.LDi.128.U8.U16 v2 i1
	LSU0.LDi.128.U8.U16 v21 i3 
	LSU0.LDi.128.U8.U16 v7 i3 
	LSU0.LDi.128.U8.U16 v4 i2 
	LSU0.LDi.128.U8.U16 v5 i2
	
	LSU0.LDi.128.U8.U16 v3 i1 
	LSU0.LDi.128.U8.U16 v6 i2 
	LSU0.LDi.128.U8.U16 v8 i3 
	
	LSU0.LDi.128.U8.U16 v9 i4 
	nop 6

	
	VAU.SHL.X16 v18 v2 1 || CMU.ALIGNVEC v13 v4 v5 14 
	VAU.SHL.X16 v19 v13 1 || CMU.ALIGNVEC v14 v5 v6 2
	VAU.SHL.X16 v27 v14 1 
	VAU.SHL.X16 v24 v7 1 
	
	vau.mul.i16 v25 v5 v22 || bru.rpl i7 i8
	vau.accnz.u16 v0   || CMU.ALIGNVEC v10 v1 v2 14 
	vau.accn.u16 v10   || CMU.ALIGNVEC v12 v4 v5 12 
	vau.accn.u16 v18   || CMU.ALIGNVEC v17 v7 v8 2
	vau.accn.u16 v19   || CMU.ALIGNVEC v11 v2 v3 2 
	vau.accn.u16 v27   || CMU.ALIGNVEC v15 v5 v6 4
	vau.accn.u16 v11   || CMU.ALIGNVEC v16 v21 v7 14
	vau.accn.u16 v12   || cmu.cpvv v1 v2  || LSU0.LDi.128.U8.U16 v3 i1 || LSU1.LDi.128.U8.U16 v6 i2      
	vau.accp.u16 v25   || cmu.cpvv v2 v3  || LSU0.LDi.128.U8.U16 v0 i0       
	vau.accn.u16 v9    || cmu.cpvv v4 v5    
	vau.accn.u16 v24   || cmu.cpvv v5 v6  || LSU0.LDi.128.U8.U16 v9 i4      
 sLaplacian5x5_loop:
 .nowarn 10
	vau.accn.u16 v16   || cmu.cpvv v21 v7 || LSU0.LDi.128.U8.U16 v8 i3     
.nowarnend
	vau.accn.u16 v15   || cmu.cpvv v7 v8      
	vau.accnw.u16 v26 v17  || CMU.ALIGNVEC v13 v4 v5 14
	VAU.SHL.X16 v18 v2 1   || CMU.ALIGNVEC v14 v5 v6 2 
	VAU.SHL.X16 v27 v14 1  || cmu.clamp0.i16 v20 v26 v23 
	VAU.SHL.X16 v24 v7 1 
	VAU.SHL.X16 v19 v13 1  || LSU0.STi.128.U16.U8 v20 i17
	
	LSU0.LD.64.H  v27  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v27  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v26  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v26  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v25  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v25  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v24  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v24  i19 || IAU.ADD i19 i19 8
	
    BRU.jmp i30
	nop 6

.size mvcvSLaplacian5x5_asm,.-mvcvSLaplacian5x5_asm
.end
