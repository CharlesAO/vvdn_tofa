///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Takes 16 bits(10bpp) value as input and compress the data by splitting 
///            into  two components, 8bits and 2bits, store components in separate buffers///
///

.type mvispPixelPacker10b_asm,@function
.version 00.50.01

.data .rodata.pixelPacker10b
.align 4
 pixelPacker10b_mask:
		.short 0x0303

//-------------------------------------------------------------------------------	
.code .text.pixelPacker10b
//	void mvispPixelPacker10b(	u16** src, 	u32** dst32, u8** dest8	u32 width)
//			   					i18			i17			i16			i15		
mvispPixelPacker10b_asm:
	.nowarn
	lsu0.ldil i0 pixelPacker10b_mask	|| lsu1.ldih i0 pixelPacker10b_mask
	lsu0.ld.16r v23 i0		|| vau.xor v0 v0 v0	|| iau.shr.u32 i13 i15 3	
	lsu0.ld.64.l v1 i18 || lsu1.ldo.64.h v1 i18 0x08
	lsu0.ldil i14 pixelPacker10b_loopPixelPacker10b	||	lsu1.ldih i14 pixelPacker10b_loopPixelPacker10b
	iau.incs i18 0x10
	iau.xor i1 i1 i1
	iau.xor i2 i2 i2
	iau.xor i3 i3 i3
	iau.xor i4 i4 i4
	
	
	IAU.XOR i11 i11 i11
	IAu.ADD i11 i11 8
	CMU.CMII.u32 i15 i11
	PEU.PCXC EQ,0 || BRU.BRA .Lexception8
	nop 6
	IAU.ADD i11 i11 8
	CMU.CMII.u32 i15 i11
	PEU.PCXC EQ,0 || BRU.BRA .Lexception16
	nop 6
	
	
	CMU.VDILV.x8 v3 v4 v1 v0
	vau.and v5 v3 v23		|| lsu0.ld.64.l v1 i18 	|| lsu1.ldo.64.h v1 i18 0x08
	cmu.cpvi.x16 i1.l v5.0	|| iau.incs i18 0x10	|| lsu0.sti.64.l v4 i17
	cmu.cpvi.x16 i2.l v5.1	|| iau.shl i5 i1 6	
	cmu.cpvi.x16 i3.l v5.2	|| iau.shl i6 i2 4	
	cmu.cpvi.x16 i4.l v5.3	|| iau.shl i7 i3 2		|| sau.or i8 i5 i6
	sau.or i9 i8 i7
	sau.or i10 i9 i4		|| lsu0.ld.64.l v1 i18 	|| lsu1.ldo.64.h v1 i18 0x08
	lsu0.sti.16 i10 i16		|| iau.incs i18 0x10

	CMU.VDILV.x8 v3 v4 v1 v0
	vau.and v5 v3 v23		|| lsu0.sti.64.l v4 i17 
	cmu.cpvi.x16 i1.l v5.0	
	cmu.cpvi.x16 i2.l v5.1	|| iau.shl i5 i1 6	|| sau.sub.i32 i13 i13 0x03
	cmu.cpvi.x16 i3.l v5.2	|| iau.shl i6 i2 4	

	
 pixelPacker10b_loopPixelPacker10b:
	lsu0.ld.64.l v1 i18 	|| lsu1.ldo.64.h v1 i18 0x08 	|| peu.pc1s gt			|| bru.jmp i14
	cmu.cpvi.x16 i4.l v5.3	|| iau.shl i7 i3 2				|| sau.or i8 i5 i6
	sau.or i9 i8 i7			|| CMU.VDILV.x8 v3 v4 v1 v3	
	iau.or i10 i9 i4		|| vau.and v5 v3 v23
	cmu.cpvi.x16 i1.l v5.0	|| lsu0.sti.16 i10 i16	  || iau.incs i18 0x10	|| sau.sub.i32 i13 i13 0x01
	cmu.cpvi.x16 i2.l v5.1	|| iau.shl i5 i1 6				|| lsu0.sti.64.l v4 i17 	
	cmu.cpvi.x16 i3.l v5.2	|| iau.shl i6 i2 4		
	
	bru.jmp i30
	cmu.cpvi.x16 i4.l v5.3	|| iau.shl i7 i3 2
	sau.or i8 i5 i6
	sau.or i9 i8 i7			
	sau.or i10 i9 i4		
	lsu0.st.16 i10 i16	
	nop



.Lexception16:

    CMU.VDILV.x8 v3 v4 v1 v0
	vau.and v5 v3 v23		|| lsu0.ld.64.l v1 i18 	|| lsu1.ldo.64.h v1 i18 0x08
	cmu.cpvi.x16 i1.l v5.0	|| iau.incs i18 0x10	|| lsu0.sti.64.l v4 i17
	cmu.cpvi.x16 i2.l v5.1	|| iau.shl i5 i1 6	
	cmu.cpvi.x16 i3.l v5.2	|| iau.shl i6 i2 4	
	cmu.cpvi.x16 i4.l v5.3	|| iau.shl i7 i3 2		|| sau.or i8 i5 i6
	sau.or i9 i8 i7
	sau.or i10 i9 i4		|| lsu0.ld.64.l v1 i18 	|| lsu1.ldo.64.h v1 i18 0x08
	lsu0.sti.16 i10 i16		|| iau.incs i18 0x10

	CMU.VDILV.x8 v3 v4 v1 v0
	vau.and v5 v3 v23		|| lsu0.sti.64.l v4 i17 
	cmu.cpvi.x16 i1.l v5.0	
	cmu.cpvi.x16 i2.l v5.1	|| iau.shl i5 i1 6	|| sau.sub.i32 i13 i13 0x03
	cmu.cpvi.x16 i3.l v5.2	|| iau.shl i6 i2 4		

	lsu0.ld.64.l v1 i18 	|| lsu1.ldo.64.h v1 i18 0x08 	
	cmu.cpvi.x16 i4.l v5.3	|| iau.shl i7 i3 2				|| sau.or i8 i5 i6
	sau.or i9 i8 i7			
	iau.or i10 i9 i4		
	cmu.cpvi.x16 i1.l v5.0	|| lsu0.sti.16 i10 i16	  || iau.incs i18 0x10
	
    BRU.JMP i30
	nop 6

.Lexception8:

    CMU.VDILV.x8 v3 v4 v1 v0
	vau.and v5 v3 v23		|| lsu0.ld.64.l v1 i18 	|| lsu1.ldo.64.h v1 i18 0x08
	cmu.cpvi.x16 i1.l v5.0	|| iau.incs i18 0x10	|| lsu0.sti.64.l v4 i17
	cmu.cpvi.x16 i2.l v5.1	|| iau.shl i5 i1 6	
	cmu.cpvi.x16 i3.l v5.2	|| iau.shl i6 i2 4	
	cmu.cpvi.x16 i4.l v5.3	|| iau.shl i7 i3 2		|| sau.or i8 i5 i6
	sau.or i9 i8 i7
	sau.or i10 i9 i4		|| lsu0.ld.64.l v1 i18 	|| lsu1.ldo.64.h v1 i18 0x08
	lsu0.sti.16 i10 i16		|| iau.incs i18 0x10

BRU.JMP i30
nop 6
	.nowarnend
.size mvispPixelPacker10b_asm,.-mvispPixelPacker10b_asm
.end
