///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvispPixelUnpacker_asm,@function
.version 00.50.00

.data .rodata.pixelUnpacker
.align 4
 pixelUnpacker_mask:
		.long 0x00000300

//-------------------------------------------------------------------------------
.code .text.pixelUnpacker
//	void mvispPixelUnpacker(	u32* src32,	u8* src8,	u16* dest,	u32 width, u8 shift)
//			   				i18			i17			i16			i15			i14
mvispPixelUnpacker_asm:
.nowarn
	lsu0.ldi.16 i2 i17					|| lsu1.ldi.64.l v0 i18
	lsu0.ldil i12 pixelUnpacker_mask 				|| lsu1.ldih i12 pixelUnpacker_mask
	lsu0.ld.32 i12 i12					|| iau.shr.u32 i5 i15 3
	lsu0.ldil i13 pixelUnpacker_loopPixelUnpacker 	|| lsu1.ldih i13 pixelUnpacker_loopPixelUnpacker
	vau.xor v23 v23 v23
	vau.xor v22 v22 v22
	lsu0.ldi.64.l v0 i18
	cmu.cpii.u8.u16 i14 i14
	cmu.cpivr.x16 v5 i14

	cmu.vilv.x8 v2 v3 v23 v0	|| sau.and i3 i2 i12	|| iau.shr.u32 	i4 i2 0x02
	cmu.cpiv.x16 v22.7 i3.l		|| sau.and i3 i4 i12	|| iau.shr.u32 	i4 i2 0x04

	cmu.cpiv.x16 v22.5 i3.l		|| sau.and i3 i4 i12	|| iau.shr.u32 	i4 i2 0x06
	cmu.cpiv.x16 v22.3 i3.l		|| sau.and i3 i4 i12	|| iau.shl 		i4 i2 0x02 || lsu0.ldi.16 i2 i17
	cmu.cpiv.x16 v22.1 i3.l	 	|| sau.and i3 i4 i12	|| iau.shl 		i4 i2 0x04
	cmu.cpiv.x16 v22.0 i3.l	 	|| sau.and i3 i4 i12	|| iau.shl 		i4 i2 0x06
	cmu.cpiv.x16 v22.2 i3.l	 	|| sau.and i3 i4 i12	|| iau.shl 		i4 i2 0x08
	cmu.cpiv.x16 v22.4 i3.l		|| sau.and i3 i4 i12
	cmu.cpiv.x16 v22.6 i3.l


	vau.or v4 v2 v22			|| lsu0.ldi.64.l v0 i18			|| bru.rpl i13 i5 pixelUnpacker_endLoopPixelUnpacker
	cmu.vilv.x8 v2 v3 v23 v0	|| sau.and i3 i2 i12			|| iau.shr.u32 	i4 i2 0x02
	cmu.cpiv.x16 v22.7 i3.l		|| sau.and i3 i4 i12			|| iau.shr.u32 	i4 i2 0x04
	vau.shl.x16 v4 v4 v5
 pixelUnpacker_loopPixelUnpacker:
	cmu.cpiv.x16 v22.5 i3.l		|| sau.and i3 i4 i12			|| iau.shr.u32 	i4 i2 0x06
	cmu.cpiv.x16 v22.3 i3.l		|| sau.and i3 i4 i12			|| iau.shl 		i4 i2 0x02  || lsu0.ldi.16 i2 i17
	cmu.cpiv.x16 v22.1 i3.l	 	|| sau.and i3 i4 i12			|| iau.shl 		i4 i2 0x04	|| lsu0.sti.64.l v4 i16
	cmu.cpiv.x16 v22.0 i3.l	 	|| sau.and i3 i4 i12			|| iau.shl 		i4 i2 0x06
	cmu.cpiv.x16 v22.2 i3.l	 	|| sau.and i3 i4 i12			|| iau.shl 		i4 i2 0x08
	cmu.cpiv.x16 v22.4 i3.l		|| sau.and i3 i4 i12
	cmu.cpiv.x16 v22.6 i3.l		|| lsu0.sti.64.h v4 i16


 pixelUnpacker_endLoopPixelUnpacker:


	bru.jmp i30
	nop 6
	.nowarnend
.size mvispPixelUnpacker_asm,.-mvispPixelUnpacker_asm
.end
