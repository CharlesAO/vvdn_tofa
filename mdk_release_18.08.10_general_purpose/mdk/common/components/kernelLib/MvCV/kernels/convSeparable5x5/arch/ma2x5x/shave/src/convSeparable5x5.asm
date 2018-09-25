///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief apply a convolution 5x5 filter, in the case of symmetric kernel.
///

.type mvcvConvSeparable5x5_asm,@function
.version 00.51.04

.code .text.convSeparable5x5

//void convSeparable5x5_asm(u8** out(i18), u8** in(i17), fp32 conv[3](i16), u32 inWidth(i15))
//                               i18           i17                 i16             i15

mvcvConvSeparable5x5_asm: 

	IAU.SUB i19 i19 8
	LSU0.ST.64.L v24  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.H v24  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.L v25  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.H v25  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.L v26  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.H v26  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.L v27  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.H v27  i19  


	lsu0.ldil i7 convSeparable5x5_loop   			||  lsu1.ldih i7 convSeparable5x5_loop
	nop 6
	LSU0.LDi.64 i0 i17
	LSU0.LDi.64 i2 i17
	LSU0.LDi.32 i4 i17 || LSU1.LD.32  i18  i18
	LSU0.LDO.64.L v20 i16 0x00		|| LSU1.LDO.64.H v20 i16 0x08
	nop 6
	
	cmu.cpvv.f32.f16 v20 v20 || iau.shr.u32 i10, i15, 3
	cmu.cpvi.x16 i5.l v20.0  || iau.sub i0 i0 8   
	cmu.cpivr.x16 v21 i5     || iau.sub i1 i1 8   
	cmu.cpvi.x16 i5.l v20.1  || iau.sub i2 i2 8
	cmu.cpivr.x16 v22 i5     || iau.sub i3 i3 8 
	cmu.cpvi.x16 i5.l v20.2  || iau.sub i4 i4 8
	cmu.cpivr.x16 v23 i5    
	
	
	lsu0.ldi.128.u8f.f16 v0, i0 || lsu1.ldi.128.u8f.f16 v4, i4 
	lsu0.ldi.128.u8f.f16 v1, i1 || lsu1.ldi.128.u8f.f16 v3, i3 
	lsu0.ldi.128.u8f.f16 v2, i2 || lsu1.ldi.128.u8f.f16 v5, i0  
	lsu0.ldi.128.u8f.f16 v9, i4 || lsu1.ldi.128.u8f.f16 v6, i1
	lsu0.ldi.128.u8f.f16 v8, i3 || lsu1.ldi.128.u8f.f16 v7, i2
	nop 2
	vau.add.f16 v12 v0 v4 
	vau.add.f16 v20 v1 v3 
	vau.mul.f16 v2 v2 v23 
	vau.mul.f16 v12 v12 v21  
	vau.mul.f16 v20 v20 v22
	nop
	vau.add.f16 v4 v2 v12
		vau.add.f16 v5 v5 v9  || lsu0.ldi.128.u8f.f16 v0, i0 || lsu1.ldi.128.u8f.f16 v4, i4
		vau.add.f16 v6 v6 v8  || lsu0.ldi.128.u8f.f16 v1, i1 || lsu1.ldi.128.u8f.f16 v3, i3
	vau.add.f16 v16 v4 v20 //11111
	
	vau.mul.f16 v7 v7 v23 || lsu0.ldi.128.u8f.f16 v2, i2 
	vau.mul.f16 v5 v5 v21 
	vau.mul.f16 v6 v6 v22 
	nop 
	vau.add.f16 v9 v7 v5  

	vau.add.f16 v12 v0 v4  || lsu0.ldi.128.u8f.f16 v0, i0 || lsu1.ldi.128.u8f.f16 v4, i4
	vau.add.f16 v20 v1 v3  || lsu0.ldi.128.u8f.f16 v1, i1 || lsu1.ldi.128.u8f.f16 v3, i3 
	
	vau.add.f16 v11 v9 v6  || lsu0.ldi.128.u8f.f16 v2, i2 //2222

	vau.mul.f16 v8 v2 v23 
	vau.mul.f16 v12 v12 v21  
	vau.mul.f16 v20 v20 v22 
	nop
	vau.add.f16 v8 v8 v12       || cmu.alignvec v14 v16 v11 12 
		vau.add.f16 v12 v0 v4   || lsu0.ldi.128.u8f.f16 v0, i0 || lsu1.ldi.128.u8f.f16 v4, i4  
		vau.add.f16 v7 v1 v3    || lsu0.ldi.128.u8f.f16 v1, i1 || lsu1.ldi.128.u8f.f16 v3, i3 
	vau.add.f16 v17 v8 v20      || cmu.alignvec v15 v16 v11 14 
		vau.mul.f16 v8 v2 v23   || lsu0.ldi.128.u8f.f16 v2, i2 
		vau.mul.f16 v12 v12 v21 
		vau.mul.f16 v7 v7 v22 
	vau.mul.f16 v10 v11 v23     || cmu.alignvec v18 v11 v17 4 || bru.rpl i7 i10  
	vau.add.f16 v24 v14 v18     || cmu.alignvec v19 v11 v17 2 
	vau.add.f16 v25 v19 v15	    || cmu.cpvv v16 v11            
		vau.add.f16 v8 v8 v12 							
	vau.mul.f16 v26 v24 v21     || cmu.cpvv v11 v17 
	vau.mul.f16 v25 v25 v22	    || cmu.alignvec v14 v16 v11 12
		vau.add.f16 v17 v8 v7 
 convSeparable5x5_loop:
	vau.add.f16 v9 v10 v26      || cmu.alignvec v15 v16 v11 14
		vau.add.f16 v12 v0 v4   || lsu0.ldi.128.u8f.f16 v0, i0 || lsu1.ldi.128.u8f.f16 v4, i4
	    vau.add.f16 v7 v1 v3    || lsu0.ldi.128.u8f.f16 v1, i1 || lsu1.ldi.128.u8f.f16 v3, i3
	vau.add.f16 v27 v9 v25
		vau.mul.f16 v8 v2 v23   || lsu0.ldi.128.u8f.f16 v2, i2 
		vau.mul.f16 v12 v12 v21 
		vau.mul.f16 v7 v7 v22   || lsu0.sti.128.f16.u8f v27, i18 
	
	
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

.size mvcvConvSeparable5x5_asm,.-mvcvConvSeparable5x5_asm
.end

	 
