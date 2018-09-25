///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Up scale by two in each direction with bilinear (sw) kernel.
///            U16 in and out
///

//--------------------------------------------------------------------------------------------------------------
.type mvispScale2xBilinHV025_U16ToU16_asm,@function
.version 00.51.0
.data .rodata.mvispScale2xBilinHV025_U16ToU16_asm 
.align 16
scale2xBilinHV025_vect:                           
        .word 0x00000003, 0x00000009    

//void mvispScale2xBilinHV025_U16ToU16_asm(u16** in, u16** out, u32 inWidth)//
//                          		             i18        i17        i16       
.nowarn	
.code .text.scale2xBilinHV025_U16ToU16_asm 
mvispScale2xBilinHV025_U16ToU16_asm:
	
	
	lsu1.ld.64 i1 i18                || lsu0.ldil i0 scale2xBilinHV025_vect
  lsu1.ld.32 i17 i17               || lsu0.ldih i0 scale2xBilinHV025_vect
  lsu1.ldi.32r v22 i0              || lsu0.ldil i5 ___loop2xBilinHV025_U16ToU16 
  lsu1.ldil i7 0x0010               || lsu0.ldih i5 ___loop2xBilinHV025_U16ToU16
  lsu1.ld.32r v23 i0               || iau.xor i15 i15 i15
  iau.incs i15 1
  cmu.cpivr.x32 v0 i15   
  iau.sub i3 i1 0x8                 || sau.sub.i32 i4 i2 0x08
  lsu1.ldi.128.u16.u32 v3 i1      || lsu0.ldi.128.u16.u32 v4 i2 
  lsu1.ld.128.u16.u32 v1 i3       || lsu0.ld.128.u16.u32 v2 i4 
  iau.shr.u32 i16 i16 3
  nop 
  lsu1.ldi.128.u16.u32 v13 i1     || lsu0.ldi.128.u16.u32 v4 i2 
  nop 3


 
  cmu.alignvec v21 v2 v4 12 //v21 in[1][-1]
  vau.mul.i32 v6 v4 v23  || cmu.cpvv v2 v4
  vau.mul.i32 v5 v3 v22  || cmu.alignvec v19 v1 v3 12 //v19 in [0][-1]
  vau.mul.i32 v7 v21 v22 || cmu.alignvec v20 v2 v4 4 //v20 in[1][1]
  vau.mul.i32 v8 v20 v22 || cmu.cpvv v1 v3
                              cmu.alignvec v18 v1 v13 4 // v18 in[0][1]
  vau.add.i32 v10 v5 v19 || cmu.cpvv v3 v13
  vau.add.i32 v11 v5 v18 || cmu.alignvec v21 v2 v4 12 //v21 in[1][-1]
  vau.add.i32 v10 v10 v7
  vau.add.i32 v11 v11 v6
  vau.add.i32 v10 v10 v6
  vau.add.i32 v11 v11 v8
  vau.shr.u32 v14 v10 4
  vau.shr.u32 v15 v11 4 || lsu1.ldi.128.u16.u32 v13 i1     || lsu0.ldi.128.u16.u32 v4 i2 
  nop
  vau.shl.x32 v16 v15 16
  nop
  vau.or v17 v16 v14  
  
  
  //START LOOP
  vau.mul.i32 v6 v4 v23  || cmu.cpvv v2 v4  || bru.rpl i5 i16 ___end2xBilinHV025_U16ToU16
  vau.mul.i32 v5 v3 v22  || cmu.alignvec v19 v1 v3 12 //v19 in [0][-1]
  vau.mul.i32 v7 v21 v22 || cmu.alignvec v20 v2 v4 4 //v20 in[1][1]
  vau.mul.i32 v8 v20 v22 || cmu.cpvv v1 v3
                              cmu.alignvec v18 v1 v13 4 // v18 in[0][1]
  vau.add.i32 v11 v5 v18 || cmu.cpvv v3 v13
  vau.add.i32 v10 v5 v19 || cmu.alignvec v21 v2 v4 12 //v21 in[1][-1]
  vau.add.i32 v11 v11 v6 || lsu1.stxvi v17 i17 i7
  vau.add.i32 v10 v10 v7 
   ___loop2xBilinHV025_U16ToU16:
  vau.add.i32 v11 v11 v8  
  vau.add.i32 v10 v10 v6
  vau.shr.u32 v15 v11 4 || lsu1.ldi.128.u16.u32 v13 i1     || lsu0.ldi.128.u16.u32 v4 i2 
  vau.shr.u32 v14 v10 4  
  vau.shl.x32 v16 v15 16
  nop
  vau.or v17 v16 v14 
  ___end2xBilinHV025_U16ToU16:

  bru.jmp i30
	nop 6

.size mvispScale2xBilinHV025_U16ToU16_asm,.-mvispScale2xBilinHV025_U16ToU16_asm
.nowarnend
.end
	
