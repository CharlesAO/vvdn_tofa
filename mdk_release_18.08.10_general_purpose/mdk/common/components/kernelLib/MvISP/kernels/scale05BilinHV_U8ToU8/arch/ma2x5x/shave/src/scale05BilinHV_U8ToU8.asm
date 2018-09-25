///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Down scale by half in each direction wil bilinear (sw) kernel.
///            U8 in and out
///

//--------------------------------------------------------------------------------------------------------------
.type mvispScale05BilinHV_U8ToU8_asm,@function
.version 00.50.0
.data .rodata.scale05BilinHV_U8ToU8_asm 
.align 16
 scale05BilinHV_U8ToU8_shiftVal:
		.short 0x0002
		
.code .text.scale05BilinHV_U8ToU8_asm 
mvispScale05BilinHV_U8ToU8_asm:
//void scale05BilinHV_Fp16U8(half** in, u8* out, u32 inWidth)//
//                          		i18        i17        i16       
	.nowarn
	
	lsu0.ldi.32 i0 i18					|| lsu1.ldil i15 scale05BilinHV_U8ToU8_loopSc05BiHVu8u8
	lsu0.ldil i0 scale05BilinHV_U8ToU8_shiftVal			|| lsu1.ldih i0 scale05BilinHV_U8ToU8_shiftVal
	lsu0.ldih i15 scale05BilinHV_U8ToU8_loopSc05BiHVu8u8	|| lsu1.ld.32 i1 i18	
	iau.shr.u32 i13 i16 4				|| lsu0.ld.16r v10 i0	
	iau.xor i8 i8 i8
	iau.incs i8 0x0f
	iau.and i11 i16 i8
	peu.pc1i eq || iau.incs i13 -1
	
	
	lsu0.ldi.128.u8.u16 v0 i0	
	lsu0.ldi.128.u8.u16 v1 i1	
	lsu0.ldi.128.u8.u16 v2 i0	 
	lsu0.ldi.128.u8.u16 v3 i1	 
	
	lsu1.ldi.128.u8.u16 v23 i0
	lsu1.ldi.128.u8.u16 v22 i1
	lsu1.ldi.128.u8.u16 v21 i0
	lsu1.ldi.128.u8.u16 v20 i1
	nop 2
	iau.sub i12 i13 0x00
	
	vau.add.i16 v4 v0 v1			|| lsu0.ldi.128.u8.u16 v0 i0	
	vau.add.i16 v5 v2 v3			|| lsu0.ldi.128.u8.u16 v1 i1	
	vau.add.i16 v19 v23 v22			|| lsu0.ldi.128.u8.u16 v2 i0	
	vau.add.i16 v18 v21 v20			|| lsu0.ldi.128.u8.u16 v3 i1
	
	peu.pc1i lte || bru.bra scale05BilinHV_U8ToU8_endSc05BiHVu8u8
	cmu.vdilv.x16 v6 v7 v4 v5		|| lsu1.ldi.128.u8.u16 v23 i0
	cmu.vdilv.x16 v16 v17 v19 v18	|| lsu1.ldi.128.u8.u16 v22 i1
	vau.add.i16 v8 v6 v7			|| lsu1.ldi.128.u8.u16 v21 i0
	vau.add.i16 v15 v16 v17			|| lsu1.ldi.128.u8.u16 v20 i1
	vau.shr.u16 v9 v8 v10
	vau.shr.u16 v14 v15 v10
	nop	
	
	vau.add.i16 v4 v0 v1			|| lsu0.ldi.128.u8.u16 v0 i0	|| bru.rpl i15 i13 scale05BilinHV_U8ToU8_endSc05BiHVu8u8
	vau.add.i16 v5 v2 v3			|| lsu0.ldi.128.u8.u16 v1 i1	|| lsu1.sti.128.u16.u8 v9 i17	|| iau.add i4 i1 0x10

 scale05BilinHV_U8ToU8_loopSc05BiHVu8u8:
	vau.add.i16 v19 v23 v22			|| lsu0.ldi.128.u8.u16 v2 i0	|| lsu1.sti.128.u16.u8 v14 i17	|| iau.add i2 i0 0x10	
	vau.add.i16 v18 v21 v20			|| lsu0.ldi.128.u8.u16 v3 i1	|| lsu1.ld.128.u8.u16 v22 i4	|| iau.add i3 i1 0x10	|| cmu.vdilv.x16 v6 v7 v4 v5
	vau.add.i16 v8 v6 v7			|| lsu0.ldi.128.u8.u16 v23 i0	|| lsu1.ldi.128.u8.u16 v21 i2	
	cmu.vdilv.x16 v16 v17 v19 v18	|| lsu0.ldi.128.u8.u16 v20 i3	
	vau.add.i16 v15 v16 v17																			|| iau.incs i0 0x08	
	vau.shr.u16 v9 v8 v10																			|| iau.incs i1 0x10
	vau.shr.u16 v14 v15 v10
	
	
 scale05BilinHV_U8ToU8_endSc05BiHVu8u8:		
	lsu1.sti.128.u16.u8 v9 i17
	iau.incs i11 0
	peu.pc1i eq || lsu1.st.128.u16.u8 v14 i17
	
	bru.jmp i30
	nop 6
	.nowarnend
.size mvispScale05BilinHV_U8ToU8_asm,.-mvispScale05BilinHV_U8ToU8_asm
.end
	
