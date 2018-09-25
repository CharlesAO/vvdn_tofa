///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Desaturates areas which are already very close to grey.  
///            It removes residual low-frequency color noise, which is most 
///            noticeable in grey areas.  This filter calculates a metric which 
///            indicates how close a pixel is to grey (the smaller the value 
///            the closer the pixel is to grey.  A blend factor is calculated which 
///            is used to interpolate between the pixel's chrominance values, and 
///            the chrominance values of a grey pixel
///

//--------------------------------------------------------------------------------------------------------------
.type mvispGreyDesat_asm,@function
.version 00.50.37.8
.data .rodata.greyDesat_asm 
.align 4
//.align 16
 greyDesat_clamp:
	 .short 255
.code .text.greyDesat_asm //text
mvispGreyDesat_asm:
// void greyDesat_asm(u8** in, u8** out, int offset, int slope, int grey[3], u32 width)//
//                     i18       i17        i16          i15        i14        i13  
	
	IAU.SUB i19 i19 8
	LSU0.ST.64.L v24  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.H v24  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.L v25  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.H v25  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.L v26  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.H v26  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.L v27  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.H v27  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.L v28  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.H v28  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.L v29  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.H v29  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.L v30  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.H v30  i19
	
	LSU0.LDIL i6 greyDesat_loop   ||  LSU1.LDIH i6 greyDesat_loop 
	LSU0.LDIL i7 greyDesat_clamp  ||  LSU1.LDIH i7 greyDesat_clamp 
	
	nop 6
	LSU0.LD.32 i0 i18 || iau.add i18 i18 4  
	LSU0.LD.32 i1 i18 || iau.add i18 i18 4  
	LSU0.LD.32 i2 i18 || iau.add i18 i18 4  
	LSU0.LD.32 i3 i17 || iau.add i17 i17 4  
	LSU0.LD.32 i4 i17 || iau.add i17 i17 4  
	LSU0.LD.32 i5 i17 || iau.add i17 i17 4  
	LSU0.LD.16R v17, i7 //clamp
	iau.shr.u32 i10 i13 3  
	nop 6

	cmu.cpivr.x16 v19 i16
	cmu.cpivr.x16 v20 i15
	lsu0.ld.64.l v21 i14 || iau.add i14 i14 8
	lsu0.ld.64.h v21 i14
	nop 6
	cmu.cpvv.i32.i16s v21 v21
	nop
	cmu.cpvi.x16 i8.l v21.0 
	cmu.cpivr.x16 v22, i8 //g0 
	cmu.cpvi.x16 i8.l v21.1 
	cmu.cpivr.x16 v18, i8 //g1  
	cmu.cpvi.x16 i8.l v21.2 
	cmu.cpivr.x16 v21, i8 //g2
	   
	//---------
	lsu0.ldi.128.u8.u16 v0 i0 
	lsu0.ldi.128.u8.u16 v1 i1 
	lsu0.ldi.128.u8.u16 v2 i2 
	nop 4
	vau.adiff.i16 v3 v0 v22 
	vau.adiff.i16 v4 v1 v18
	vau.adiff.i16 v5 v2 v21
	vau.add.i16 v6 v3 v4 
	nop 2
	vau.add.i16 v7 v5 v6 
	nop 2
	vau.add.i16 v8 v7 v19  
	lsu0.ldi.128.u8.u16 v0 i0 
	nop 							
	vau.mul.i16 v9 v8 v20 || lsu0.ldi.128.u8.u16 v1 i1 
	nop 
	lsu0.ldi.128.u8.u16 v2 i2
	cmu.clamp0.i16 v10 v9 v17   
	vau.sub.i16 v11 v0 v22 
	vau.sub.i16 v12 v1 v18
	vau.sub.i16 v13 v2 v21 
	cmu.alignvec v27 v11 v10 8 || vau.adiff.i16 v3 v0 v22 
	cmu.alignvec v28 v12 v10 8 || vau.adiff.i16 v4 v1 v18
	cmu.alignvec v30 v13 v10 8 || vau.adiff.i16 v5 v2 v21 
	cmu.alignvec v23 v10 v19 8 || vau.add.i16 v6 v3 v4 
	nop 2
	cmu.cpvv.i16.i32 v10 v10 || vau.add.i16 v7 v5 v6
	cmu.cpvv.i16.i32 v23 v23  
	nop 
	cmu.cpvv.i16.i32 v11 v11 || vau.add.i16 v8 v7 v19
	cmu.cpvv.i16.i32 v12 v12 || vau.mul.i32 v14 v10 v11 
	
	vau.mul.i32 v15 v10 v12	 || cmu.cpvv.i16.i32 v13 v13 
	cmu.cpvv.i16.i32 v27 v27 || vau.mul.i16 v9 v8 v20 
	cmu.cpvv.i16.i32 v28 v28 || vau.mul.i32 v16 v10 v13 
	cmu.cpvv.i16.i32 v30 v30 || vau.mul.i32 v7 v23 v27
		
	vau.mul.i32 v8 v23 v28 || cmu.clamp0.i16 v10 v9 v17 
	vau.mul.i32 v9 v23 v30 
	nop
	vau.shr.i32 v24 v14 8 || cmu.alignvec v23 v10 v19 8 || lsu0.ldi.128.u8.u16 v0 i0 || bru.rpl i6 i10 _end_delay_loop_greyDesat
	vau.shr.i32 v25 v15 8 || cmu.cpvv.i16.i32 v23 v23 
	vau.shr.i32 v26 v16 8 || cmu.cpvv.i16.i32 v10 v10   || lsu0.ldi.128.u8.u16 v1 i1 
	vau.shr.i32 v7 v7 8   || cmu.cpvv.i32.i16s v14 v24  
	vau.shr.i32 v8 v8 8   || cmu.cpvv.i32.i16s v15 v25  || lsu0.ldi.128.u8.u16 v2 i2 
	vau.shr.i32 v9 v9 8   || cmu.cpvv.i32.i16s v16 v26 

	cmu.cpvv.i32.i16s v7 v7  || vau.sub.i16 v11 v0 v22
	cmu.cpvv.i32.i16s v8 v8  || vau.adiff.i16 v3 v0 v22
	cmu.cpvv.i32.i16s v9 v9  || vau.sub.i16 v12 v1 v18
	
	cmu.alignvec v24 v7 v14 8  || vau.adiff.i16 v4 v1 v18
	cmu.alignvec v14 v24 v7 8  || vau.sub.i16 v13 v2 v21
	cmu.alignvec v25 v8 v15 8  || vau.add.i16 v6 v3 v4
	cmu.alignvec v15 v25 v8 8  || vau.adiff.i16 v5 v2 v21
	cmu.alignvec v27 v11 v10 8 || vau.add.i16 v14 v14 v22 
	
	cmu.alignvec v26 v9 v16 8  || vau.add.i16 v7 v5 v6
	cmu.alignvec v16 v26 v9 8  || vau.add.i16 v15 v15 v18  
	cmu.cpvv.i16.i32 v11 v11   || vau.add.i16 v16 v16 v21   || lsu0.sti.128.u16.u8 v14 i3 
	cmu.alignvec v28 v12 v10 8 || vau.add.i16 v8 v7 v19
	cmu.cpvv.i16.i32 v12 v12   || vau.mul.i32 v14 v10 v11   || lsu0.sti.128.u16.u8 v15 i4 
 greyDesat_loop:
	cmu.alignvec v30 v13 v10 8 || vau.mul.i32 v15 v10 v12   || lsu0.sti.128.u16.u8 v16 i5
	cmu.cpvv.i16.i32 v13 v13   || vau.mul.i16 v9 v8 v20 
	cmu.cpvv.i16.i32 v27 v27   || vau.mul.i32 v16 v10 v13 
	cmu.cpvv.i16.i32 v28 v28   || vau.mul.i32 v7 v23 v27
	cmu.cpvv.i16.i32 v30 v30   || vau.mul.i32 v8 v23 v28 
	cmu.clamp0.i16 v10 v9 v17  || vau.mul.i32 v9 v23 v30 
	nop
	_end_delay_loop_greyDesat:
	
	LSU0.LD.64.H  v30  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v30  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v29  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v29  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v28  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v28  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v27  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v27  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v26  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v26  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v25  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v25  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v24  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v24  i19 || IAU.ADD i19 i19 8 
	
	
	bru.jmp i30
	nop 6
	
.size mvispGreyDesat_asm,.-mvispGreyDesat_asm
.end
	
	 
		

	
	
