///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

//--------------------------------------------------------------------------------------------------------------
.type mvcvCvtColorRGBtoChromaNV12_asm,@function
.version 00.50.37.8

.code .text.cvtColorRGBtoChromaNV12_asm //text
mvcvCvtColorRGBtoChromaNV12_asm:
//void cvtColorRGBtoChromaNV12(UInt8* ln1RGB[3], UInt8* ln2RGB[3], u8* yOut, float mat[3x3], float offset[1x3], u32 width)
//                    		         i18               i17            i16         i15             i14              i13

	IAU.SUB i19 i19 8         || LSU0.LDIL i11 0x001F   || LSU1.LDIH i11 0x0
	LSU0.ST.64.L v24  i19     || IAU.SUB i19 i19 8		|| LSU1.LDIL i10, 16
	lsu0.ldo.64.L v0, i15 12  
	lsu0.ldo.64.h v0, i15 20  || lsu1.ldo.64.l v23, i14 4 || iau.add i15 i15 28
	lsu0.ld.64.l v22, i15     
	lsu0.ldo.32 i0 i18 0x00
	lsu0.ldo.32 i1 i17 0x00   || iau.and i8 i13 i11
	lsu0.ldo.32 i2 i18 0x04
	lsu0.ldo.32 i3 i17 0x04   || iau.shr.u32 i6 i13 5 
	lsu0.ldo.32 i4 i18 0x08
	LSU0.ST.64.H v24  i19     || IAU.SUB i19 i19 8 || lsu1.ldo.32 i5 i17 0x08
	LSU0.ST.64.L v25  i19     || IAU.SUB i19 i19 8 || cmu.cpvv.f32.f16 v23 v23 //v23 offset{1,2}
	LSU1.ST.64.H v25  i19     || IAU.SUB i19 i19 8 || lsu0.ldxvi v1 i0 i10  || cmu.cpvv.f32.f16 v0 v0
	lsu1.ldxvi v2 i1 i10      || cmu.cpvv.f32.f16 v22 v22
	lsu0.ldxvi v3 i2 i10      || cmu.alignvec v21 v0 v0 8
	lsu1.ldxvi v4 i3 i10      || cmu.alignvec v0 v21 v22 8//v0 matCoefs{3,4,5,6,7,8}
	lsu0.ldxvi v5 i4 i10  
	lsu1.ldxvi v6 i5 i10 
	lsu0.ldxvi v7  i0 i10  
	lsu1.ldxvi v8  i1 i10 
	lsu0.ldxvi v9  i2 i10 
	lsu1.ldxvi v10 i3 i10 
	lsu0.ldxvi v11 i4 i10 
	lsu1.ldxvi v12 i5 i10 
	lsu0.ldxvi v1 i0 i10  
	lsu1.ldxvi v2 i1 i10 
	lsu0.ldxvi v3 i2 i10 
	lsu1.ldxvi v4 i3 i10 
	lsu0.ldxvi v5 i4 i10 
	lsu1.ldxvi v6 i5 i10 
	vau.avg.x8 v13 v1 v2    || lsu0.ldxvi v7  i0 i10 
	vau.avg.x8 v15 v3 v4    || lsu1.ldxvi v8  i1 i10 
	vau.avg.x8 v17 v5 v6    || lsu0.ldxvi v9  i2 i10 
	vau.avg.x8 v14 v7 v8    || lsu1.ldxvi v10 i3 i10 
	vau.avg.x8 v16 v9 v10   || lsu0.ldxvi v11 i4 i10 
	vau.avg.x8 v18 v11 v12  || lsu1.ldxvi v12 i5 i10 
	
	LSU0.ST.64.L v26  i19  || IAU.SUB i19 i19 8 || cmu.vdilv.x8 v14 v13 v13 v14  
	LSU0.ST.64.H v26  i19  || IAU.SUB i19 i19 8 || cmu.vdilv.x8 v16 v15 v15 v16 || LSU1.LDIL i9 mvcvCvtColorRGBtoChromaNV12_asm_final   
	LSU0.ST.64.L v27  i19  || IAU.SUB i19 i19 8 || cmu.vdilv.x8 v18 v17 v17 v18 || LSU1.LDIH i9 mvcvCvtColorRGBtoChromaNV12_asm_final  
	LSU0.ST.64.H v27  i19  || IAU.SUB i19 i19 8 || vau.avg.x8 v13 v14 v13 
	LSU0.ST.64.L v28  i19  || IAU.SUB i19 i19 8 || vau.avg.x8 v15 v16 v15  
	LSU0.ST.64.H v28  i19  || IAU.SUB i19 i19 8 || vau.avg.x8 v17 v18 v17
	LSU0.ST.64.L v29  i19  || IAU.SUB i19 i19 8 || cmu.alignvec v14 v13 v13 8 
	LSU0.ST.64.H v29  i19  || IAU.SUB i19 i19 8 || cmu.alignvec v16 v15 v15 8 
	LSU0.ST.64.L v30  i19  || IAU.SUB i19 i19 8 || cmu.alignvec v18 v17 v17 8  
	LSU0.ST.64.H v30  i19  || IAU.SUB i19 i19 8 || vau.avg.x8 v8 v7 v8  
	LSU0.ST.64.L v31  i19  || IAU.SUB i19 i19 8 || vau.avg.x8 v10 v9 v10  
	LSU0.ST.64.H v31  i19  || vau.avg.x8 v11 v11 v12  

	cmu.cpvv.u8f.f16 v1 v13  || vau.avg.x8 v13 v1 v2  || LSU0.LDIL i15 0x01 || LSU1.LDIH i15 0x00 
	cmu.cpvv.u8f.f16 v3 v15  || vau.avg.x8 v15 v3 v4  || LSU0.LDIL i7 mvcvCvtColorRGBtoChromaNV12_asm_loop   || LSU1.LDIH i7 mvcvCvtColorRGBtoChromaNV12_asm_loop 
	cmu.cpvv.u8f.f16 v5 v17  || vau.avg.x8 v17 v5 v6 
	cmu.cpvv.u8f.f16 v2 v14  || vau.mul.f16 v26 v0 v1 || lsu0.swzv8 [00000000]          || lsu1.ldxvi v1 i0 i10  //--u
	cmu.cpvv.u8f.f16 v4 v16  || vau.mul.f16 v27 v0 v3 || lsu0.swzv8 [11111111]  
	cmu.cpvv.u8f.f16 v6 v18  || vau.mul.f16 v29 v0 v2 || lsu0.swzv8 [00000000]          || lsu1.ldxvi v2 i1 i10 
	vau.mul.f16 v30 v0 v4    || lsu0.swzv8 [11111111] 
	vau.mul.f16 v28 v0 v5    || lsu0.swzv8 [22222222]  || lsu1.ldxvi v3 i2 i10  
	vau.mul.f16 v31 v0 v6    || lsu0.swzv8 [22222222]  || cmu.vdilv.x8 v14 v13 v13 v8 
	vau.mul.f16 v19 v0 v1    || lsu0.swzv8 [33333333]  || lsu1.ldxvi v4 i3 i10          || cmu.vdilv.x8 v16 v15 v15 v10 //--v
	vau.mul.f16 v20 v0 v2    || lsu0.swzv8 [33333333]  || cmu.vdilv.x8 v18 v17 v17 v11
	vau.mul.f16 v21 v0 v3    || lsu0.swzv8 [44444444]  || lsu1.ldxvi v5 i4 i10
	vau.mul.f16 v22 v0 v4    || lsu0.swzv8 [44444444]
	vau.add.f16 v26 v26 v27  || lsu0.ldxvi v9  i2 i10
	vau.add.f16 v29 v29 v30  || lsu1.ldxvi v8  i1 i10
	vau.mul.f16 v24 v0 v5    || lsu0.swzv8 [55555555]           
	vau.add.f16 v28 v28 v26  || lsu0.ldxvi v6 i5 i10 
	vau.add.f16 v31 v31 v29  || lsu1.ldxvi v7  i0 i10          
	vau.mul.f16 v25 v0 v6    || lsu0.swzv8 [55555555]   
	vau.add.f16 v19 v19 v21                      	   || lsu1.ldxvi v10 i3 i10
	vau.add.f16 v20 v20 v22                       	   || lsu0.ldxvi v11 i4 i10  
	vau.add.f16 v27 v23 v28  || lsu1.swzv8 [00000000]   		
	vau.add.f16 v30 v23 v31  || lsu1.swzv8 [00000000]   || LSU0.LD.64.L v12 i5     || IAU.INCS I5, 8
	vau.add.f16 v24 v24 v19                            || LSU0.LD.64.H v12 i5     || IAU.INCS I5, 8  || LSU1.LDIL i11, 32 //0x20
	vau.add.f16 v25 v25 v20  || cmu.cmii.u32 i13 i11   || LSU0.LDIL i14 mvcvCvtColorRGBtoChromaNV12_asm_final2  || LSU1.LDIH i14 mvcvCvtColorRGBtoChromaNV12_asm_final2		
    peu.pc1c eq || bru.jmp i9  
	vau.avg.x8 v13 v14 v13  
	vau.avg.x8 v15 v16 v15 
	vau.avg.x8 v17 v18 v17			 
	cmu.alignvec v14 v13 v13 8 || vau.avg.x8 v8 v7 v8     
	cmu.alignvec v16 v15 v15 8 || vau.avg.x8 v10 v9 v10    
	cmu.alignvec v18 v17 v17 8 || vau.avg.x8 v11 v11 v12  
	
	 cmu.cmii.u32 i6 i15  ||  iau.sub i6 i6 1
	 peu.pc1c eq || bru.jmp i9  
	 nop 6
		
	cmu.cpvv.u8f.f16 v1 v13  || vau.avg.x8 v13 v1 v2      || bru.rpl i7 i6  
	cmu.cpvv.u8f.f16 v3 v15  || vau.avg.x8 v15 v3 v4      
	cmu.cpvv.u8f.f16 v5 v17  || vau.avg.x8 v17 v5 v6      
	cmu.cpvv.u8f.f16 v2 v14  || vau.add.f16 v21 v23 v24   || lsu0.swzv8 [11111111]  
	cmu.cpvv.u8f.f16 v4 v16  || vau.add.f16 v22 v23 v25   || lsu0.swzv8 [11111111]  
	cmu.cpvv.u8f.f16 v6 v18  || vau.mul.f16 v26 v0 v1     || lsu0.swzv8 [00000000]          || LSU1.LDO.64.H v1 i0 0X08    
	vau.mul.f16 v27 v0 v3	 || lsu0.swzv8 [11111111]      || LSU1.LD.64.L v1 i0            || IAU.INCS I0, 0X10 
	vau.mul.f16 v29 v0 v2	 || lsu0.swzv8 [00000000]      ||cmu.vilv.x16 v25 v24 v21 v27
	vau.mul.f16 v30 v0 v4    || lsu0.swzv8 [11111111]      ||cmu.vilv.x16 v22 v21 v22 v30   || lsu1.sti.128.f16.u8f v25 i16 
	vau.mul.f16 v28 v0 v5    || lsu0.swzv8 [22222222]                                       || lsu1.sti.128.f16.u8f v24 i16   
	vau.mul.f16 v31 v0 v6    || lsu0.swzv8 [22222222]      || cmu.vdilv.x8 v14 v13 v13 v8   || lsu1.sti.128.f16.u8f v22 i16  
	vau.mul.f16 v19 v0 v1    || lsu0.swzv8 [33333333]      || cmu.vdilv.x8 v16 v15 v15 v10  || lsu1.sti.128.f16.u8f v21 i16   
	vau.mul.f16 v20 v0 v2    || lsu0.swzv8 [33333333]      || cmu.vdilv.x8 v18 v17 v17 v11
	vau.mul.f16 v21 v0 v3    || lsu0.swzv8 [44444444]      || LSU1.LDO.64.H v2 i1 0X08  
	vau.mul.f16 v22 v0 v4    || lsu0.swzv8 [44444444]      || LSU1.LD.64.L v2 i1            || IAU.INCS I1, 0X10 
	vau.add.f16 v26 v26 v27  || LSU0.LDO.64.H v3 i2 0X08  || LSU1.LD.64.L v3 i2            || IAU.INCS I2, 0X10  
	vau.add.f16 v29 v29 v30  || LSU0.LDO.64.H v4 i3 0X08  || LSU1.LD.64.L v4 i3            || IAU.INCS I3, 0X10        
	vau.mul.f16 v24 v0 v5    || lsu0.swzv8 [55555555]      || LSU1.LDO.64.H v5 i4, 0X08       
	vau.add.f16 v28 v28 v26  || LSU0.LDO.64.H v9 i2 0X08  || LSU1.LD.64.L v5 i4   		   || IAU.INCS I4, 0X10     
	vau.add.f16 v31 v31 v29  || LSU0.LDO.64.H v8 i1 0X08  || LSU1.LD.64.L v9  i2    	   || IAU.INCS I2, 0X10     
	vau.mul.f16 v25 v0 v6    || lsu0.swzv8 [55555555]      || LSU1.LD.64.L v8  i1	       || IAU.INCS I1, 0X10    
	vau.add.f16 v19 v19 v21  || LSU0.LDO.64.H v6 i5 0X08  || LSU1.LD.64.L v6 i5            || IAU.INCS I5, 0X10                         
	vau.add.f16 v20 v20 v22  || LSU0.LDO.64.H v7 i0 0X08  || LSU1.LD.64.L v7  i0           || IAU.INCS I0, 0X10        
	vau.add.f16 v27 v23 v28  || lsu1.swzv8 [00000000]	  || LSU0.LDo.64.H v10 i3, 0X08  
	vau.add.f16 v30 v23 v31  || lsu1.swzv8 [00000000] 	  || LSU0.LD.64.L v10 i3		   || IAU.INCS I3, 0X10    
	vau.add.f16 v24 v24 v19  ||LSU0.LDO.64.H v11 i4, 0X08 || LSU1.LD.64.L v11 i4    	   || IAU.INCS I4, 0X10                         
	vau.add.f16 v25 v25 v20  ||LSU0.LDO.64.H v12 i5, 0X08 || LSU1.LD.64.L v12 i5   		   || IAU.INCS I5, 0X10
mvcvCvtColorRGBtoChromaNV12_asm_loop:	
    NOP	
	vau.avg.x8 v13 v14 v13 
	vau.avg.x8 v15 v16 v15 
	vau.avg.x8 v17 v18 v17							 
	cmu.alignvec v14 v13 v13 8 || vau.avg.x8 v8 v7 v8 
	cmu.alignvec v16 v15 v15 8 || vau.avg.x8 v10 v9 v10 
	cmu.alignvec v18 v17 v17 8 || vau.avg.x8 v11 v11 v12
   

mvcvCvtColorRGBtoChromaNV12_asm_final:
 	nop 
	vau.add.f16 v21 v23 v24 || lsu0.swzv8 [11111111]  
	vau.add.f16 v22 v23 v25 || lsu0.swzv8 [11111111]  
	nop 
	cmu.vilv.x16 v28 v29 v21 v27  
	cmu.vilv.x16 v30 v31 v22 v30   
	lsu0.sti.128.f16.u8f v28 i16   
	lsu0.sti.128.f16.u8f v29 i16   
	lsu0.sti.128.f16.u8f v30 i16   
	lsu0.sti.128.f16.u8f v31 i16   
 
	
	cmu.cmz.i32 i8 
	peu.pc1c eq || bru.jmp i14
	
	cmu.cpvv.u8f.f16 v1 v13   
    cmu.cpvv.u8f.f16 v3 v15  
    cmu.cpvv.u8f.f16 v5 v17 
    cmu.cpvv.u8f.f16 v2 v14  || vau.mul.f16 v15 v0 v1 || lsu0.swzv8 [00000000]  
    cmu.cpvv.u8f.f16 v4 v16  || vau.mul.f16 v7 v0 v3 || lsu0.swzv8 [11111111]  
    cmu.cpvv.u8f.f16 v6 v18  || vau.mul.f16 v12 v0 v2 || lsu0.swzv8 [00000000] 
	vau.mul.f16 v11 v0 v4    || lsu0.swzv8 [11111111] 
	vau.mul.f16 v13 v0 v5    || lsu0.swzv8 [22222222]  
	vau.mul.f16 v10 v0 v6    || lsu0.swzv8 [22222222]  
	vau.mul.f16 v19 v0 v1    || lsu0.swzv8 [33333333]  
	vau.mul.f16 v20 v0 v2    || lsu0.swzv8 [33333333]   
	vau.mul.f16 v21 v0 v3    || lsu0.swzv8 [44444444]  
	vau.mul.f16 v22 v0 v4    || lsu0.swzv8 [44444444]
		
	vau.add.f16 v15 v15 v7                             
	vau.add.f16 v12 v12 v11   
.lalign    
mvcvCvtColorRGBtoChromaNV12_asm_final2:	
.nowarn 10
	vau.mul.f16 v9 v0 v5    || lsu0.swzv8 [55555555]  || LSU1.LD.64.H  v31  i19 || IAU.ADD i19 i19 8         
.nowarnend
	vau.add.f16 v13 v13 v15                          || LSU1.LD.64.L  v31  i19 || IAU.ADD i19 i19 8
	vau.add.f16 v10 v10 v12                          || LSU1.LD.64.H  v30  i19 || IAU.ADD i19 i19 8 
	vau.mul.f16 v8 v0 v6    || lsu0.swzv8 [55555555]  || LSU1.LD.64.L  v30  i19 || IAU.ADD i19 i19 8
	vau.add.f16 v7 v23 v13  || lsu0.swzv8 [00000000]  || LSU1.LD.64.H  v29  i19 || IAU.ADD i19 i19 8 
	vau.add.f16 v5 v23 v10  || lsu0.swzv8 [00000000]  || LSU1.LD.64.L  v29  i19 || IAU.ADD i19 i19 8
	vau.add.f16 v19 v19 v21                          || LSU1.LD.64.H  v28  i19 || IAU.ADD i19 i19 8 
	vau.add.f16 v20 v20 v22                          || LSU1.LD.64.L  v28  i19 || IAU.ADD i19 i19 8
											            LSU1.LD.64.H  v27  i19 || IAU.ADD i19 i19 8 
	vau.add.f16 v9 v9 v19   						 || LSU1.LD.64.L  v27  i19 || IAU.ADD i19 i19 8
	vau.add.f16 v8 v8 v20   						 || LSU1.LD.64.H  v26  i19 || IAU.ADD i19 i19 8 
											            LSU1.LD.64.L  v26  i19 || IAU.ADD i19 i19 8
 	vau.add.f16 v21 v23 v9 || lsu0.swzv8 [11111111]   || LSU1.LD.64.H  v25  i19 || IAU.ADD i19 i19 8 
	vau.add.f16 v22 v23 v8 || lsu0.swzv8 [11111111]   || LSU1.LD.64.L  v25  i19 || IAU.ADD i19 i19 8
												        LSU1.LD.64.H  v24  i19 || IAU.ADD i19 i19 8 
	cmu.vilv.x16 v1 v2 v21 v7   				     || LSU1.LD.64.L  v24  i19 || IAU.ADD i19 i19 8
	cmu.vilv.x16 v3 v4 v22 v5                   || iau.sub i8 i8 8
	peu.pc1i gte || lsu0.sti.128.f16.u8f v1 i16 || iau.sub i8 i8 8
	peu.pc1i gte ||	lsu0.sti.128.f16.u8f v2 i16 || iau.sub i8 i8 8
	peu.pc1i gte || lsu0.sti.128.f16.u8f v3 i16 || iau.sub i8 i8 8
	peu.pc1i gte || lsu0.st.128.f16.u8f  v4 i16     


	
bru.jmp i30
nop 6
.size mvcvCvtColorRGBtoChromaNV12_asm,.-mvcvCvtColorRGBtoChromaNV12_asm
.end
