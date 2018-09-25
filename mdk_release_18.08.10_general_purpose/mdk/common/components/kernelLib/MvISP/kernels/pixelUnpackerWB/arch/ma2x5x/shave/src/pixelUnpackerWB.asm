///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
.type mvispPixelUnpackerWB_asm,@function
.version 00.50.03

.data .rodata.pixelUnpackerWB
.align 4
 pixelUnpackerWB_mask:
		.long 0x00000003	
 pixelUnpackerWB_clamp:
		.long 0x0000ffff
//-------------------------------------------------------------------------------	
.code .text.pixelUnpackerWB
//	void mvispPixelUnpackerWB(	u32* src32,	u8* src8,	u16* dest,	u32 width, u8 shift, u16 *awbCoef, u32 line)
//			   				i18			i17			i16			i15			i14			i13			i12
		
mvispPixelUnpackerWB_asm:
.nowarn
    lsu0.ldil i1 0xffff
	lsu0.ldil i7 pixelUnpackerWB_mask 					|| lsu1.ldih i7 pixelUnpackerWB_mask
	lsu0.ldil i6 pixelUnpackerWB_loopPixelUnpackerWB 	|| lsu1.ldih i6 pixelUnpackerWB_loopPixelUnpackerWB
	lsu0.ldil i8 pixelUnpackerWB_clamp 					|| lsu1.ldih i8 pixelUnpackerWB_clamp
	lsu0.ld.32 i7 i7						|| iau.shr.u32 i5 i15 3		|| lsu1.ld.128.u16.u32 v21 i13		
	vau.xor v23 v23 v23						|| lsu0.ldil i7 0x06
	vau.xor v22 v22 v22						|| iau.xor i4 i4 i4			
	cmu.cpii.u8.u16 i14 i14 				|| iau.incs i4 0x01			|| sau.sub.i32 i9 i7 i14
	lsu0.ldi.64.l v0 i18					|| lsu1.ldi.32r v20 i8		
	iau.and i12 i12 i4						|| cmu.cpivr.x32 v19 i9		 
	iau.sub i12 i12 0x00					|| cmu.cpivr.x16 v5 i14
	cmu.cpivr.x16 v14 i1                    || lsu0.ldil i10 0
	cmu.cpivr.x16 v15 i1
	cmu.cpiv.x32  v14.2 i1
	cmu.cpiv.x32  v14.3 i10
	cmu.cpiv.x32  v15.0 i10
	cmu.cpiv.x32  v15.1 i10
	peu.pc1i eq || cmu.cpvi.x32 i0 v21.1
	peu.pc1i eq || cmu.cpvi.x32 i3 v21.2 	
	peu.pc1i gt || cmu.cpvi.x32 i0 v21.0
	peu.pc1i gt || cmu.cpvi.x32 i3 v21.3 
	
	cmu.cpiv.x32 v21.0 i0	|| lsu1.ldi.16r v17 i1	
	cmu.cpiv.x32 v21.1 i3	|| vau.shr.u32 v20 v20 v19
	cmu.cpiv.x32 v21.2 i0	|| iau.xor i0 i0 i0
	cmu.cpiv.x32 v21.3 i3	|| lsu1.ldi.16r v16 i1

	
	cmu.cpiv.x16 v18.0 i7.l	|| iau.shl i7 i7 2 	
	cmu.cpiv.x16 v18.1 i7.l	|| iau.shl i7 i7 2 	
	cmu.cpiv.x16 v18.2 i7.l	|| iau.shl i7 i7 2 	
	cmu.cpiv.x16 v18.3 i7.l	|| iau.shl i7 i7 2 
	cmu.cpiv.x16 v18.4 i7.l	|| iau.shl i7 i7 2 
	cmu.cpiv.x16 v18.5 i7.l	|| iau.shl i7 i7 2 
	cmu.cpiv.x16 v18.6 i7.l	|| iau.shl i7 i7 2 
	cmu.cpiv.x16 v18.7 i7.l	|| iau.xor i7 i7 i7	
	
	cmu.cpiv.x16 v17.4 i0.0 || iau.incs i7 2		|| lsu1.ldi.16r v1 i17	
	cmu.cpiv.x16 v17.3 i7.l || iau.incs i7 2
	cmu.cpiv.x16 v17.2 i7.l || iau.incs i7 2
	cmu.cpiv.x16 v17.1 i7.l || iau.incs i7 2
	cmu.cpiv.x16 v17.0 i7.l || iau.incs i7 -2
	cmu.cpiv.x16 v16.7 i7.l || iau.incs i7 -2
	cmu.cpiv.x16 v16.6 i7.l || iau.incs i7 -2
	cmu.cpiv.x16 v16.5 i7.l 
	cmu.cpiv.x16 v16.4 i0.0

	vau.and v2 v1 v18		|| cmu.vilv.x8 v4 v0 v23 v0	
	vau.shl.x16 v3 v2 v17	|| lsu1.ldi.16r v1 i17	
	vau.shr.u16 v6 v2 v16	|| lsu0.ldi.64.l v0 i18	
	nop
	vau.and v3 v3 v14
	vau.and v6 v6 v15
	vau.or v7 v3 v6		
	vau.or v8 v7 v4			|| lsu0.swzv8 [40516273]
	vau.shl.x16 v9 v8 v5
	nop	
	cmu.vilv.x16 v10 v11 v23 v9
	vau.mul.i32 v12 v10 v21
	vau.mul.i32 v13 v11 v21
	nop
	vau.shr.u32 v14 v12 8
	vau.shr.u32 v15 v13 8
	cmu.vilv.x8 v4 v0 v23 v0
	vau.and v2 v1 v18			
	vau.shl.x16 v3 v2 v17		|| lsu1.ldi.16r v1 i17			
	vau.shr.u16 v6 v2 v16		|| lsu0.ldi.64.l v0 i18	
	
	

	cmu.min.u32 v14 v14 v20     || bru.rpl i6 i5 pixelUnpackerWB_endLoopPixelUnpackerWB				
 	vau.or v7 v3 v6				|| cmu.min.u32 v15 v15 v20
	vau.or v8 v7 v4				|| lsu0.swzv8 [40516273]
	vau.shl.x16 v9 v8 v5		|| cmu.vdilv.x16 v7 v22 v14 v15
	lsu0.st.64.l v22 i16		|| lsu1.sto.64.h v22 i16 0x08	 
	vau.and v2 v1 v18 			|| cmu.vilv.x16 v10 v11 v23 v9
	
 pixelUnpackerWB_loopPixelUnpackerWB:	
	vau.mul.i32 v12 v10 v21	
	vau.mul.i32 v13 v11 v21	
	iau.incs i16 0x10		
	vau.shr.u32 v14 v12 8
	vau.shr.u32 v15 v13 8		|| cmu.vilv.x8 v4 v0 v23 v0
	vau.shl.x16 v3 v2 v17		|| lsu1.ldi.16r v1 i17
	vau.shr.u16 v6 v2 v16		|| lsu0.ldi.64.l v0 i18	

	
 pixelUnpackerWB_endLoopPixelUnpackerWB:		
	bru.jmp i30
	nop 6
.nowarnend	
.size mvispPixelUnpackerWB_asm,.-mvispPixelUnpackerWB_asm
.end
