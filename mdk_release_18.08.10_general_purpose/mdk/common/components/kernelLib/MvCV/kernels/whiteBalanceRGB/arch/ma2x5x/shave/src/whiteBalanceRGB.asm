///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

//--------------------------------------------------------------------------------------------------------------
.type mvcvWhiteBalanceRGB_asm,@function
.version 00.50.37.8
.data .rodata.whiteBalanceRGB_asm 
.align 4
.code .text.whiteBalanceRGB_asm //text
mvcvWhiteBalanceRGB_asm:
//void whiteBalanceRGB(u16** inR, u16** inG, u16** inB, u16 awbCoef[3], 16** oR, u16** oG, u16** oB, u16 clamp, u32 width) 
//                          i18       i17        i16          i15           i14       i13       i12       i11        i10
	
	LSU0.LDIL i3 whiteBalanceRGB_loop  			  || LSU1.LDIH i3 whiteBalanceRGB_loop 
	LSU0.LD.32 i16 i16 || LSU1.LD.32 i10 i19
	LSU0.LD.32 i18 i18 || LSU1.LD.32 i17 i17 
	LSU0.LD.32 i14 i14 || LSU1.LD.32 i13 i13
	LSU0.LD.32 i12 i12
	nop 3
	LSU0.LD.128.u16.u32 v0 i15 || iau.shr.u32 i2 i10 3 // || CMU.CPIVR.x32 v10 i11	
	LSU0.LD.32r v10 i11


	nop 6
	//loading RGB	
	lsu0.ldi.128.i16.i32 v1 i18 
	lsu0.ldi.128.i16.i32 v2 i17
	lsu0.ldi.128.i16.i32 v3 i16
	lsu0.ldi.128.i16.i32 v4 i18
	lsu0.ldi.128.i16.i32 v5 i17
	lsu0.ldi.128.i16.i32 v6 i16
	nop 
	
	vau.mul.i32 v7 v0 v1 || lsu0.SWZMV4.word [0000] [uuuu]  
	vau.mul.i32 v8 v0 v2 || lsu0.SWZMV4.word [1111] [uuuu]    
	vau.mul.i32 v9 v0 v3 || lsu0.SWZMV4.word [2222] [uuuu]  
		vau.mul.i32 v14 v0 v4 || lsu0.SWZMV4.word [0000] [uuuu] 
		vau.mul.i32 v15 v0 v5 || lsu0.SWZMV4.word [1111] [uuuu] 
		vau.mul.i32 v16 v0 v6 || lsu0.SWZMV4.word [2222] [uuuu] || lsu1.ldi.128.i16.i32 v1 i18
	nop 
	vau.shr.u32 v11 v7 15 || lsu0.ldi.128.i16.i32 v2 i17 || bru.rpl i3 i2
	vau.shr.u32 v12 v8 15 ||lsu0.ldi.128.i16.i32 v3 i16
	vau.shr.u32 v13 v9 15  || cmu.clamp0.i32 v11 v11 v10 || lsu0.ldi.128.i16.i32 v4 i18 
	vau.shr.u32 v17 v14 15 || cmu.clamp0.i32 v12 v12 v10 || lsu0.ldi.128.i16.i32 v5 i17
	vau.shr.u32 v18 v15 15 || cmu.clamp0.i32 v13 v13 v10 || lsu0.sti.128.i32.i16 v11 i14 || lsu1.ldi.128.i16.i32 v6 i16

	vau.shr.u32 v19 v16 15 || cmu.clamp0.i32 v17 v17 v10 || lsu0.sti.128.i32.i16 v12 i13
 whiteBalanceRGB_loop:
 .nowarn 10
	vau.mul.i32 v7 v0 v1 || lsu0.SWZMV4.word [0000] [uuuu] || cmu.clamp0.i32 v18 v18 v10 || lsu1.sti.128.i32.i16 v13 i12
.nowarnend
	vau.mul.i32 v8 v0 v2 || lsu0.SWZMV4.word [1111] [uuuu] || cmu.clamp0.i32 v19 v19 v10 || lsu1.sti.128.i32.i16 v17 i14
	vau.mul.i32 v9 v0 v3 || lsu0.SWZMV4.word [2222] [uuuu] || lsu1.sti.128.i32.i16 v18 i13
	vau.mul.i32 v14 v0 v4 || lsu0.SWZMV4.word [0000] [uuuu] || lsu1.sti.128.i32.i16 v19 i12
	vau.mul.i32 v15 v0 v5 || lsu0.SWZMV4.word [1111] [uuuu] //|| bru.rpl i3 i2
	vau.mul.i32 v16 v0 v6 || lsu0.SWZMV4.word [2222] [uuuu] || lsu1.ldi.128.i16.i32 v1 i18
	nop
	
	
	bru.jmp i30
	nop 6
	
.size mvcvWhiteBalanceRGB_asm,.-mvcvWhiteBalanceRGB_asm
.end
	
