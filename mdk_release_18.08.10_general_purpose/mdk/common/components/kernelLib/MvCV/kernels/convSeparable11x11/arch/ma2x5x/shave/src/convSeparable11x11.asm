///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief apply a convolution11x11 filter, in the case of symmetric kernel.
///

.type mvcvConvSeparable11x11_asm,@function
.version 00.51.04

.code .text.convSeparable11x11

//void convSeparable11x11_asm(u8** out(i18), u8** in(i17), fp32 conv[6](i16), u32 inWidth(i15))

mvcvConvSeparable11x11_asm: 
     
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
	 LSU0.ST.64.H v30  i19  || IAU.SUB i19 i19 8
	 LSU0.ST.64.L v31  i19  || IAU.SUB i19 i19 8
	 LSU0.ST.64.H v31  i19  

	 lsu0.ldil i11 convSeparable11x11_loop  ||  lsu1.ldih i11 convSeparable11x11_loop
	 nop 6
	 LSU0.LDi.64 i0 i17
	 LSU0.LDi.64 i2 i17
	 LSU0.LDi.64 i4 i17  
	 LSU0.LDi.64 i6 i17 
	 LSU0.LDi.64 i8 i17 
	 LSU0.LDi.32 i10 i17 || LSU1.LD.32  i18  i18
	 
	 LSU0.LDO.64.L v20 i16 0x00	|| LSU1.LDO.64.H v20 i16 0x08
	 nop 6
	
	 cmu.cpvv.f32.f16 v20, v20 || iau.shr.u32 i12, i15, 3
	 cmu.cpvi.x16 i13.l v20.0    || iau.sub i0 i0 8
	 cmu.cpivr.x16 v21 i13 	     || iau.sub i1 i1 8  //conv[0]
	 cmu.cpvi.x16 i13.l v20.1    || iau.sub i2 i2 8
	 cmu.cpivr.x16 v22 i13 	     || iau.sub i3 i3 8  //conv[1]
	 cmu.cpvi.x16 i13.l v20.2    || iau.sub i4 i4 8
	 cmu.cpivr.x16 v23 i13 	     || iau.sub i5 i5 8  //conv[2]
	 cmu.cpvi.x16 i13.l v20.3    || iau.sub i6 i6 8
	 cmu.cpivr.x16 v24 i13 	     || iau.sub i7 i7 8  //conv[3]
	 LSU0.LDO.64.L v20 i16 0x10  || iau.sub i8 i8 8
	 nop 6
	 cmu.cpvv.f32.f16 v20, v20   || iau.sub i9 i9 8
	 cmu.cpvi.x16 i13.l v20.0    || iau.sub i10 i10 8
	 cmu.cpivr.x16 v25 i13   			//conv[4]
	 cmu.cpvi.x16 i13.l v20.1   
	 cmu.cpivr.x16 v26 i13 				//conv[5]
	//-----------------------------------------------------------------
	
	 lsu0.ldi.128.u8f.f16 v0, i0 || lsu1.ldi.128.u8f.f16 v10, i10  //|| bru.rpl i11 i12
	 lsu0.ldi.128.u8f.f16 v1, i1 || lsu1.ldi.128.u8f.f16 v9, i9
	 lsu0.ldi.128.u8f.f16 v2, i2 || lsu1.ldi.128.u8f.f16 v8, i8 
	 lsu0.ldi.128.u8f.f16 v3, i3 || lsu1.ldi.128.u8f.f16 v7, i7 
	 lsu0.ldi.128.u8f.f16 v4, i4 || lsu1.ldi.128.u8f.f16 v6, i6 
	 lsu0.ldi.128.u8f.f16 v5, i5 
	 nop 

	 vau.add.f16 v0 v0 v10  
	 vau.add.f16 v1 v1 v9  
	 vau.add.f16 v2 v2 v8  
	 vau.add.f16 v3 v3 v7 
	 vau.add.f16 v4 v4 v6
	 
	 vau.mul.f16 v11 v0 v21
	 vau.mul.f16 v9 v1 v22
	 vau.mul.f16 v8 v2 v23  || lsu0.ldi.128.u8f.f16 v0, i0 || lsu1.ldi.128.u8f.f16 v10, i10  
	 vau.mul.f16 v7 v3 v24
	 vau.mul.f16 v6 v4 v25
	 vau.mul.f16 v5 v5 v26  
	
	 vau.add.f16 v11 v11 v9  || lsu0.ldi.128.u8f.f16 v1, i1 || lsu1.ldi.128.u8f.f16 v9, i9
	 vau.add.f16 v12 v8 v7   || lsu0.ldi.128.u8f.f16 v2, i2 || lsu1.ldi.128.u8f.f16 v8, i8 
	 vau.add.f16 v13 v6 v5   || lsu0.ldi.128.u8f.f16 v3, i3 || lsu1.ldi.128.u8f.f16 v7, i7 
		vau.add.f16 v0 v0 v10  
	 vau.add.f16 v11 v11 v12 || lsu0.ldi.128.u8f.f16 v4, i4 || lsu1.ldi.128.u8f.f16 v6, i6 
	 nop 2
	 vau.add.f16 v13 v13 v11 || lsu0.ldi.128.u8f.f16 v5, i5 //11111 
	
	 vau.add.f16 v1 v1 v9  
	 vau.add.f16 v2 v2 v8  
	 vau.add.f16 v3 v3 v7 
	 vau.add.f16 v4 v4 v6
	 
	 vau.mul.f16 v11 v0 v21
	 vau.mul.f16 v9 v1 v22
	 vau.mul.f16 v8 v2 v23 || lsu0.ldi.128.u8f.f16 v0, i0 || lsu1.ldi.128.u8f.f16 v10, i10  
	 vau.mul.f16 v7 v3 v24
	 vau.mul.f16 v6 v4 v25
	 vau.mul.f16 v5 v5 v26   
	                         
	 vau.add.f16 v11 v11 v9  || lsu0.ldi.128.u8f.f16 v1, i1 || lsu1.ldi.128.u8f.f16 v9, i9
	 vau.add.f16 v12 v8 v7   || lsu0.ldi.128.u8f.f16 v2, i2 || lsu1.ldi.128.u8f.f16 v8, i8 
	 vau.add.f16 v14 v6 v5   || lsu0.ldi.128.u8f.f16 v3, i3 || lsu1.ldi.128.u8f.f16 v7, i7 
		vau.add.f16 v0 v0 v10                    
	 vau.add.f16 v11 v11 v12 || lsu0.ldi.128.u8f.f16 v4, i4 || lsu1.ldi.128.u8f.f16 v6, i6 
	 nop 2                   
	 vau.add.f16 v14 v14 v11 || lsu0.ldi.128.u8f.f16 v5, i5 //2222 
	
	 vau.add.f16 v1 v1 v9   
	 vau.add.f16 v2 v2 v8   
	 vau.add.f16 v3 v3 v7   
	 vau.add.f16 v4 v4 v6   
	 
	 vau.mul.f16 v11 v0 v21  || lsu0.ldi.128.u8f.f16 v0, i0 || lsu1.ldi.128.u8f.f16 v10, i10
	 vau.mul.f16 v9 v1 v22   
	 vau.mul.f16 v12 v2 v23  || lsu0.ldi.128.u8f.f16 v2, i2 || lsu1.ldi.128.u8f.f16 v8, i8
	 vau.mul.f16 v7 v3 v24  
     vau.mul.f16 v6 v4 v25  
     vau.mul.f16 v5 v5 v26  
     
     vau.add.f16 v11 v11 v9  || cmu.alignvec v16 v13 v14 6   || lsu0.ldi.128.u8f.f16 v1, i1 || lsu1.ldi.128.u8f.f16 v9, i9
     vau.add.f16 v12 v12 v7  || cmu.alignvec v17 v13 v14 8   || lsu0.ldi.128.u8f.f16 v3, i3 || lsu1.ldi.128.u8f.f16 v7, i7
     vau.add.f16 v15 v6 v5   || cmu.alignvec v18 v13 v14 10  || lsu0.ldi.128.u8f.f16 v4, i4 || lsu1.ldi.128.u8f.f16 v6, i6
		vau.add.f16 v0 v0 v10  
     vau.add.f16 v11 v11 v12 || cmu.alignvec v19 v13 v14 12 || lsu0.ldi.128.u8f.f16 v5, i5
		vau.add.f16 v2 v2 v8
		vau.add.f16 v1 v1 v9
	 vau.add.f16 v15 v15 v11 || cmu.alignvec v20 v13 v14 14   //333333
		vau.add.f16 v3 v3 v7 
		vau.add.f16 v4 v4 v6
		vau.mul.f16 v10 v0 v21
		vau.mul.f16 v9 v1 v22  || bru.rpl i11 i12
		vau.mul.f16 v8 v2 v23
		vau.mul.f16 v7 v3 v24
		vau.mul.f16 v6 v4 v25
		vau.mul.f16 v5 v5 v26   
		vau.add.f16 v10 v10 v9  
	 	vau.add.f16 v12 v8 v7  
	vau.mul.f16 v11 v14 v26   || cmu.alignvec v27 v14 v15 10 
	vau.add.f16 v27 v27 v16   || cmu.alignvec v28 v14 v15 8  
	vau.add.f16 v28 v28 v17   || cmu.alignvec v29 v14 v15 6  
	vau.add.f16 v29 v29 v18   || cmu.alignvec v30 v14 v15 4  
	vau.add.f16 v30 v30 v19   || cmu.alignvec v31 v14 v15 2    
	vau.add.f16 v31 v31 v20   || cmu.cpvv v13 v14            || lsu0.ldi.128.u8f.f16 v3, i3 || lsu1.ldi.128.u8f.f16 v7, i7 
	vau.mul.f16 v27 v27 v21   || cmu.cpvv v14 v15            || lsu0.ldi.128.u8f.f16 v2, i2 || lsu1.ldi.128.u8f.f16 v8, i8
		vau.add.f16 v15 v6 v5 
	vau.mul.f16 v28 v28 v22	  || cmu.alignvec v16 v13 v14 6  || lsu0.ldi.128.u8f.f16 v1, i1 || lsu1.ldi.128.u8f.f16 v9, i9
		vau.add.f16 v12 v10 v12								 || lsu0.ldi.128.u8f.f16 v0, i0 || lsu1.ldi.128.u8f.f16 v10, i1
	vau.mul.f16 v29 v29 v23   || cmu.alignvec v17 v13 v14 8  || lsu0.ldi.128.u8f.f16 v4, i4 || lsu1.ldi.128.u8f.f16 v6, i6
	vau.mul.f16 v30 v30 v24   || cmu.alignvec v18 v13 v14 10 ||  lsu0.ldi.128.u8f.f16 v5, i5
		 vau.add.f16 v15 v15 v12 
	vau.mul.f16 v31 v31 v25   || cmu.alignvec v19 v13 v14 12
	vau.add.f16 v28 v28 v27   || cmu.alignvec v20 v13 v14 14
	vau.add.f16 v29 v29 v30
	vau.add.f16 v31 v31 v11
		vau.add.f16 v0 v0 v10 
 convSeparable11x11_loop:
	vau.add.f16 v29 v29 v28
		vau.add.f16 v1 v1 v9  
		vau.add.f16 v2 v2 v8 
	vau.add.f16 v31 v31 v29 
		vau.add.f16 v3 v3 v7 
		vau.add.f16 v4 v4 v6
		vau.mul.f16 v10 v0 v21 || lsu0.sti.128.f16.u8f v31, i18

		
	 LSU0.LD.64.H  v31  i19 || IAU.ADD i19 i19 8    
	 LSU0.LD.64.L  v31  i19 || IAU.ADD i19 i19 8
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
	

	BRU.jmp i30
	nop 6

.size mvcvConvSeparable11x11_asm,.-mvcvConvSeparable11x11_asm
.end
 
 
