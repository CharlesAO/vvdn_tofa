///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

//--------------------------------------------------------------------------------------------------------------
.type mvcvCvtColorRGBtoLumaNV12_asm,@function
.version 00.51.04
	
.code .text.cvtColorRGBtoLumaNV12_asm //text
mvcvCvtColorRGBtoLumaNV12_asm:
//void cvtColorRGBtoLumaNV12(u8* inR, u8* inG, u8* inB, u8* yOut, float mat[3x3], float offset[1x3], u32 width)
//                    		   i18      i17      i16       i15      i14                 i13           i12
.nowarn
	 LSU0.LD.64.L v0, i14 || iau.add i14 i14 8 || LSU1.LD.32 i13, i13
	 LSU0.LD.64.h v0, i14 || iau.shr.u32 i2 i12 4   
	 nop 5
	 	
	 lsu0.ldi.128.U8f.F16 v1 i18 || LSU1.LDIL i8 mvcvCvtColorRGBtoLumaNV12_final  
	 lsu0.ldi.128.U8f.F16 v2 i17 || cmu.cpii.f32.f16 i13 i13 || LSU1.LDIH i8 mvcvCvtColorRGBtoLumaNV12_final 
	 lsu0.ldi.128.U8f.F16 v3 i16 || LSU1.LDIL i3 mvcvCvtColorRGBtoLumaNV12_loop  
	 lsu0.ldi.128.U8f.F16 v4 i18 || LSU1.LDIH i3 mvcvCvtColorRGBtoLumaNV12_loop
	 lsu0.ldi.128.U8f.F16 v5 i17 || cmu.cpivr.x16 v23, i13 
	 lsu0.ldi.128.U8f.F16 v6 i16 || LSU1.LDIL i9 0x0F
	 cmu.cpvv.f32.f16 v0 v0    
									
	 vau.mul.f16 v7 v0 v1     || lsu0.swzv8 [00000000]	|| lsu1.ldi.128.U8f.F16 v1 i18   
     vau.mul.f16 v8 v0 v2     || lsu0.swzv8 [11111111] 	|| lsu1.ldi.128.U8f.F16 v2 i17   
     vau.mul.f16 v9 v0 v3     || lsu0.swzv8 [22222222] 	|| lsu1.ldi.128.U8f.F16 v3 i16     
	 vau.mul.f16 v10 v0 v4    || lsu0.swzv8 [00000000]  || lsu1.ldi.128.U8f.F16 v4 i18   
     vau.mul.f16 v11 v0 v5    || lsu0.swzv8 [11111111] 	|| lsu1.ldi.128.U8f.F16 v5 i17 || iau.and i10 i12 i9
	 
	 vau.mul.f16 v12 v0 v6    || lsu0.swzv8 [22222222] 	|| lsu1.ldi.128.U8f.F16 v6 i16	|| bru.rpl i3 i2 _end_delay_loop_LumaNV12
	 vau.add.f16 v13 v7 v8    
	 vau.add.f16 v14 v10 v11  
 	 vau.mul.f16 v7 v0 v1     || lsu0.swzv8 [00000000]	|| lsu1.ldi.128.U8f.F16 v1 i18 
	 vau.add.f16 v15 v13 v9   || lsu1.ldi.128.U8f.F16 v2 i17
	 	 mvcvCvtColorRGBtoLumaNV12_loop:

	 vau.add.f16 v16 v14 v12  || lsu1.ldi.128.U8f.F16 v3 i16

	 vau.mul.f16 v8 v0 v2     || lsu0.swzv8 [11111111] || lsu1.ldi.128.U8f.F16 v4 i18	
	 vau.add.f16 v17 v15 v23  || lsu1.ldi.128.U8f.F16 v5 i17
	 vau.add.f16 v18 v16 v23
	 vau.mul.f16 v9 v0 v3     || lsu0.swzv8 [22222222] 	
	 lsu1.sti.128.f16.u8f v17 i15 || vau.mul.f16 v10 v0 v4    || lsu0.swzv8 [00000000]  
	 lsu1.sti.128.f16.u8f v18 i15 || vau.mul.f16 v11 v0 v5    || lsu0.swzv8 [11111111] 	
	_end_delay_loop_LumaNV12:

	cmu.cmz.i32 i10
	peu.pc1c eq || bru.jmp i8
	
	vau.add.f16 v13 v7 v8
	nop 2
	vau.add.f16 v15 v13 v9
	nop 2
	vau.add.f16 v17 v15 v23
	nop 2
	lsu1.st.128.f16.u8f v17 i15 
	
mvcvCvtColorRGBtoLumaNV12_final:
	bru.jmp i30
	nop 6
.size mvcvCvtColorRGBtoLumaNV12_asm,.-mvcvCvtColorRGBtoLumaNV12_asm
.nowarnend
.end	
