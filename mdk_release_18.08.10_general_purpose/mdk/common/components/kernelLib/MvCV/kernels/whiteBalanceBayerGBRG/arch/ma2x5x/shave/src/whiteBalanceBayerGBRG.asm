///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

//--------------------------------------------------------------------------------------------------------------
.type mvcvWhiteBalanceBayerGBRG_asm,@function
.version 00.50.37.8
.data .rodata.whiteBalanceBayerGBRG_asm 
.align 4
// .align 16

 whiteBalanceBayerGBRG_clamp:
	 .float16 255.0

.code .text.whiteBalanceRGB_asm //text
mvcvWhiteBalanceBayerGBRG_asm:
//void whiteBalanceBayerGBRG(u16** inGBRG, u16 awbCoef[3], u16** outGBRG, u16 clamp[1], u32 width, u8 line)
//                                 i18           i17            i16          i15           i14       i13  
	.nowarn
	LSU0.LDIL i3 whiteBalanceBayerGBRG_loop  	|| LSU1.LDIH i3 whiteBalanceBayerGBRG_loop 
	LSU0.LDIL i4 whiteBalanceBayerGBRG_loop2  	|| LSU1.LDIH i4 whiteBalanceBayerGBRG_loop2 
	LSU0.LDIL i6 whiteBalanceBayerGBRG_lineGR  || LSU1.LDIH i6 whiteBalanceBayerGBRG_lineGR 
	LSU0.LDIL i7 whiteBalanceBayerGBRG_compensateGR  	|| LSU1.LDIH i7 whiteBalanceBayerGBRG_compensateGR 
	LSU0.LDIL i8 whiteBalanceBayerGBRG_compensateBG  	|| LSU1.LDIH i8 whiteBalanceBayerGBRG_compensateBG 

	LSU0.LD.32  i18 i18 || LSU1.LD.32 i16 i16 
	lsu0.ldil i5 0x1 || lsu1.ldih i5 0x0
	LSU0.LD.128.u16.u32 v0 i17 || iau.shr.u32 i2 i14 4 
	LSU0.LD.32r v10 i15
	nop 6
			
	iau.and i0 i13 i5
	cmu.cmz.i32 i0
	peu.pc1c neq || bru.jmp i6 
	nop 6
	
 whiteBalanceBayerGBRG_lineBG:	
	cmu.cmz.i8 i2
	peu.pc1c eq || bru.jmp i8
	nop 6
	lsu0.ldi.128.i16.i32 v1 i18  
	lsu0.ldi.128.i16.i32 v2 i18
	lsu0.ldi.128.i16.i32 v3 i18 
	lsu0.ldi.128.i16.i32 v4 i18
	nop 3
	vau.mul.i32 v5 v0 v1 || lsu0.SWZMV4.word [2121] [uuuu] || lsu1.ldi.128.i16.i32 v1 i18 
	vau.mul.i32 v6 v0 v2 || lsu0.SWZMV4.word [2121] [uuuu] || lsu1.ldi.128.i16.i32 v2 i18 
	vau.mul.i32 v7 v0 v3 || lsu0.SWZMV4.word [2121] [uuuu] || lsu1.ldi.128.i16.i32 v3 i18 
	vau.mul.i32 v8 v0 v4 || lsu0.SWZMV4.word [2121] [uuuu] || lsu1.ldi.128.i16.i32 v4 i18
	nop 
	vau.shr.u32 v9  v5 15 
	vau.shr.u32 v11 v6 15 
	vau.shr.u32 v12 v7 15
	vau.shr.u32 v13 v8 15
	
	vau.mul.i32 v5 v0 v1 || lsu0.SWZMV4.word [2121] [uuuu] || cmu.clamp0.i32 v9  v9 v10  || lsu1.ldi.128.i16.i32 v1 i18 
	vau.mul.i32 v6 v0 v2 || lsu0.SWZMV4.word [2121] [uuuu] || cmu.clamp0.i32 v11 v11 v10 || lsu1.ldi.128.i16.i32 v2 i18
	vau.mul.i32 v7 v0 v3 || lsu0.SWZMV4.word [2121] [uuuu] || cmu.clamp0.i32 v12 v12 v10 || lsu1.ldi.128.i16.i32 v3 i18
	vau.mul.i32 v8 v0 v4 || lsu0.SWZMV4.word [2121] [uuuu] || cmu.clamp0.i32 v13 v13 v10 || lsu1.ldi.128.i16.i32 v4 i18
	nop
	vau.shr.u32 v9  v5 15 || lsu0.sti.128.i32.i16 v9 i16 || bru.rpl i3 i2 
 
	vau.shr.u32 v11 v6 15 || lsu0.sti.128.i32.i16 v11 i16
whiteBalanceBayerGBRG_loop:
	vau.shr.u32 v12 v7 15 || lsu0.sti.128.i32.i16 v12 i16
	vau.shr.u32 v13 v8 15 || lsu0.sti.128.i32.i16 v13 i16
	vau.mul.i32 v5 v0 v1 || lsu0.SWZMV4.word [2121] [uuuu] || cmu.clamp0.i32 v9  v9 v10  || lsu1.ldi.128.i16.i32 v1 i18 
	vau.mul.i32 v6 v0 v2 || lsu0.SWZMV4.word [2121] [uuuu] || cmu.clamp0.i32 v11 v11 v10 || lsu1.ldi.128.i16.i32 v2 i18
	vau.mul.i32 v7 v0 v3 || lsu0.SWZMV4.word [2121] [uuuu] || cmu.clamp0.i32 v12 v12 v10 || lsu1.ldi.128.i16.i32 v3 i18
	vau.mul.i32 v8 v0 v4 || lsu0.SWZMV4.word [2121] [uuuu] || cmu.clamp0.i32 v13 v13 v10 || lsu1.ldi.128.i16.i32 v4 i18
	nop
	
	nop
	iau.sub i18 i18 96
 whiteBalanceBayerGBRG_compensateBG:
    IAU.SHR.U32 i11 i14 3               
    IAU.AND i9 i11 i5                             
    CMU.CMZ.i32 i9                     
    PEU.PCXC EQ,0 || BRU.JMP i30        
    nop 6                              
 
	lsu0.ldi.128.i16.i32 v1 i18 
	lsu0.ldi.128.i16.i32 v2 i18
	nop 6
	vau.mul.i32 v3 v0 v1 || lsu0.SWZMV4.word [2121] [uuuu] 
	vau.mul.i32 v4 v0 v2 || lsu0.SWZMV4.word [2121] [uuuu] 
	nop 
	vau.shr.u32 v5 v3 15
	vau.shr.u32 v6 v4 15
	cmu.clamp0.i32 v7 v5 v10
	cmu.clamp0.i32 v8 v6 v10
	lsu0.sti.128.i32.i16 v7 i16
	lsu0.sti.128.i32.i16 v8 i16
	
BRU.JMP i30   
nop 6         
	
//--------------------------------------------------------------------------------------------------------------------------------------

 whiteBalanceBayerGBRG_lineGR: 
	cmu.cmz.i8 i2
	peu.pc1c eq || bru.jmp i7
	nop 6
	lsu0.ldi.128.i16.i32 v1 i18  
	lsu0.ldi.128.i16.i32 v2 i18
	lsu0.ldi.128.i16.i32 v3 i18 
	lsu0.ldi.128.i16.i32 v4 i18
	nop 3 
	vau.mul.i32 v5 v0 v1 || lsu0.SWZMV4.word [1010] [uuuu] || lsu1.ldi.128.i16.i32 v1 i18 
	vau.mul.i32 v6 v0 v2 || lsu0.SWZMV4.word [1010] [uuuu] || lsu1.ldi.128.i16.i32 v2 i18 
	vau.mul.i32 v7 v0 v3 || lsu0.SWZMV4.word [1010] [uuuu] || lsu1.ldi.128.i16.i32 v3 i18 
	vau.mul.i32 v8 v0 v4 || lsu0.SWZMV4.word [1010] [uuuu] || lsu1.ldi.128.i16.i32 v4 i18
	nop 
	vau.shr.u32 v9  v5 15 
	vau.shr.u32 v11 v6 15 
	vau.shr.u32 v12 v7 15
	vau.shr.u32 v13 v8 15
	
	vau.mul.i32 v5 v0 v1 || lsu0.SWZMV4.word [1010] [uuuu] || cmu.clamp0.i32 v9  v9 v10  || lsu1.ldi.128.i16.i32 v1 i18 
	vau.mul.i32 v6 v0 v2 || lsu0.SWZMV4.word [1010] [uuuu] || cmu.clamp0.i32 v11 v11 v10 || lsu1.ldi.128.i16.i32 v2 i18
	vau.mul.i32 v7 v0 v3 || lsu0.SWZMV4.word [1010] [uuuu] || cmu.clamp0.i32 v12 v12 v10 || lsu1.ldi.128.i16.i32 v3 i18
	vau.mul.i32 v8 v0 v4 || lsu0.SWZMV4.word [1010] [uuuu] || cmu.clamp0.i32 v13 v13 v10 || lsu1.ldi.128.i16.i32 v4 i18
	nop
	vau.shr.u32 v9  v5 15 || lsu0.sti.128.i32.i16 v9 i16 || bru.rpl i4 i2 
 
	vau.shr.u32 v11 v6 15 || lsu0.sti.128.i32.i16 v11 i16
whiteBalanceBayerGBRG_loop2:
	vau.shr.u32 v12 v7 15 || lsu0.sti.128.i32.i16 v12 i16
	vau.shr.u32 v13 v8 15 || lsu0.sti.128.i32.i16 v13 i16
	vau.mul.i32 v5 v0 v1 || lsu0.SWZMV4.word [1010] [uuuu] || cmu.clamp0.i32 v9  v9 v10  || lsu1.ldi.128.i16.i32 v1 i18 
	vau.mul.i32 v6 v0 v2 || lsu0.SWZMV4.word [1010] [uuuu] || cmu.clamp0.i32 v11 v11 v10 || lsu1.ldi.128.i16.i32 v2 i18
	vau.mul.i32 v7 v0 v3 || lsu0.SWZMV4.word [1010] [uuuu] || cmu.clamp0.i32 v12 v12 v10 || lsu1.ldi.128.i16.i32 v3 i18
	vau.mul.i32 v8 v0 v4 || lsu0.SWZMV4.word [1010] [uuuu] || cmu.clamp0.i32 v13 v13 v10 || lsu1.ldi.128.i16.i32 v4 i18
	nop
	
	nop
	iau.sub i18 i18 96
 whiteBalanceBayerGBRG_compensateGR:
    IAU.SHR.U32 i11 i14 3               
    IAU.AND i9 i11 i5                   
    CMU.CMZ.i32 i9                     
    PEU.PCXC EQ,0 || BRU.JMP i30       
    nop 6                              
    
	lsu0.ldi.128.i16.i32 v1 i18 
	lsu0.ldi.128.i16.i32 v2 i18
	nop 6
	vau.mul.i32 v3 v0 v1 || lsu0.SWZMV4.word [1010] [uuuu] 
	vau.mul.i32 v4 v0 v2 || lsu0.SWZMV4.word [1010] [uuuu] 
	nop 
	vau.shr.u32 v5 v3 15
	vau.shr.u32 v6 v4 15
	cmu.clamp0.i32 v7 v5 v10
	cmu.clamp0.i32 v8 v6 v10
	lsu0.sti.128.i32.i16 v7 i16
	lsu0.sti.128.i32.i16 v8 i16
	
	

	bru.jmp i30
	nop 6
	
.size mvcvWhiteBalanceBayerGBRG_asm,.-mvcvWhiteBalanceBayerGBRG_asm
.nowarnend
.end

