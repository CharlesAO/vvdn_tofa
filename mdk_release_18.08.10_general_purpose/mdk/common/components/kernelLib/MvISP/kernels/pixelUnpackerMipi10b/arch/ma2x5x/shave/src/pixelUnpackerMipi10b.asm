///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Special kernel to interface to Myriad2 Mipi block
///            Converts 32bit value received as [12b | 10b | 10b]  to [6b | 10b | 6b | 10b]
///

.type _Z29mvispPixelUnpackerMipi10b_asmPjPtjjS_i,@function
.version 00.50.02

.data .rodata.pixelUnpackerMipi10b
.align 4
 pixelUnpackerMipi10b_mask1:
		.long 0x000003ff
 pixelUnpackerMipi10b_mask2:
		.long 0x000ffc00

//-------------------------------------------------------------------------------
.code .text.pixelUnpackerMipi10b

//void pixelUnpackerMipi10b_asm(u32 *in, u16 *out, u32 width, u32 lineNo, u32 coefs[4], int noMipiRxWorkaround)
//			   					i18			i17		   i16		   i15			i14		                 i13
_Z29mvispPixelUnpackerMipi10b_asmPjPtjjS_i:
  .nowarn
	cmu.cmz.i32 i13
	peu.pc1c neq || bru.bra ___noWorkaround
	nop 6
	lsu0.ldil i1 pixelUnpackerMipi10b_mask1 	|| lsu1.ldih i1 pixelUnpackerMipi10b_mask1
	lsu0.ldil i2 pixelUnpackerMipi10b_mask2 	|| lsu1.ldih i2 pixelUnpackerMipi10b_mask2
	lsu0.ldxv v0 i18		|| iau.xor i0 i0 i0
	lsu1.ld.32r v21 i1		|| iau.incs i0 0x06
	lsu1.ld.32r v22 i2		|| cmu.cpivr.x32 v23 i0 	|| iau.incs i0 0x0A
	lsu0.ld.32 i3 i14		|| lsu1.ldo.32 i4 i14 0x04	|| iau.incs i18 0x10
	lsu0.ldo.32 i5 i14 0x08 || lsu1.ldo.32 i6 i14 0x0C
	iau.shr.u32 i11 i16 4
	nop 2
	lsu0.ldil i8 0x0001
	iau.and i15 i15 i8
	cmu.cmii.u32 i15 i8		|| lsu0.ldxvi v10 i18 i0
	peu.pc1c lt	|| cmu.cpii i7 i4
	peu.pc1c lt	|| iau.shl i9 i3 16
	peu.pc1c lt	|| iau.or i10 i7 i9

	peu.pc1c eq	|| cmu.cpii i7 i6
	peu.pc1c eq	|| iau.shl i9 i5 16
	peu.pc1c eq	|| iau.or i10 i7 i9

	lsu0.ldil i12 pixelUnpackerMipi10b_loopUnpackMipi	|| lsu1.ldih i12 pixelUnpackerMipi10b_loopUnpackMipi
	cmu.cpivr.x32 v20 i10


	vau.and v1 v0 v21
	vau.and v11 v10 v21
	vau.and v2 v0 v22
	lsu0.ldxvi v0 i18 i0	|| vau.and v12 v10 v22
	vau.shl.x32 v3 v2 v23
	vau.shl.x32 v13 v12 v23	|| lsu0.ldxvi v10 i18 i0
	cmu.cmii.u32 i16 i0
	peu.pc1c lte || bru.bra pixelUnpackerMipi10b_endUnpackMipi
	iau.xor i1 i1 i1
	vau.or v4 v1 v3
	vau.or v14 v11 v13
	vau.isubs.u16 v5 v4 v20
	vau.isubs.u16 v15 v14 v20
	nop

	vau.and v1 v0 v21		|| bru.rpl i12 i11 pixelUnpackerMipi10b_endUnpackMipi
	vau.and v11 v10 v21		|| lsu0.ldxvi v0 i18 i0
	vau.and v2 v0 v22		|| iau.incs i16 -16
	vau.and v12 v10 v22		|| lsu0.ldxvi v10 i18 i0
	vau.shl.x32 v3 v2 v23

 pixelUnpackerMipi10b_loopUnpackMipi:
	vau.shl.x32 v13 v12 v23
	lsu1.stxvi v5 i17 i0
	vau.or v4 v1 v3
	vau.or v14 v11 v13
	vau.isubs.u16 v5 v4 v20
	vau.isubs.u16 v15 v14 v20
	lsu1.stxvi v15 i17 i0


 pixelUnpackerMipi10b_endUnpackMipi:
 	cmu.cmz.i16 i16
 	peu.pc1c eq || bru.jmp i30
	iau.sub i4 i16 7
	cmu.cmii.u32 i4 i1
	nop 4
	peu.pc1c gt || lsu0.sto.64.l v5 i17 0x00
	peu.pc1c gt || lsu0.sto.64.h v5 i17 0x08
	iau.sub i4 i16 15
	cmu.cmii.i32 i4 i1
	peu.pc1c gt || lsu0.sto.64.l v15 i17 0x10
	peu.pc1c gt || lsu0.sto.64.h v15 i17 0x18

	bru.jmp i30
	nop 6



	___noWorkaround:
	lsu0.ldil i1 0x0001 || lsu1.ldxv v0 i18    || iau.add i0 i18 0x10
	iau.and i15 i15 i1  || lsu0.ldxv v1 i0
	cmu.cmz.i32 i15     || iau.add i18 i0 0x10 || lsu1.ldil i3 0x20
	peu.pc1c eq  || lsu1.ld.32 i1 i14
	peu.pc1c eq  || lsu1.ldo.32 i2 i14 0x4
	peu.pc1c neq || lsu1.ldo.32 i1 i14 0x8
	peu.pc1c neq || lsu1.ldo.32 i2 i14 0xC
	lsu0.ldxv v3 i18 || iau.add i0 i18 0x10
	lsu1.ldxv v4 i0  || iau.add i18 i0 0x10
	lsu0.ldil i5 ___noWorkaroundloop || iau.add i8 i18 0x10 || cmu.cmii.u32 i16 i3
	lsu0.ldih i5 ___noWorkaroundloop || iau.add i7 i17 0x10
    peu.pc1c lt || bru.bra ___noWorkaroundCompesate
    iau.shr.u32 i6 i16 5
    iau.shl i1 i1 0x10
    iau.or i2 i2 i1     || lsu1.ldxvi v0 i18 i3
    cmu.cpivr.x32 v2 i2 || lsu0.ldxvi v1 i8 i3

    vau.isubs.u16 v7 v0 v2  || lsu1.ldil i4 0x18
    vau.isubs.u16 v8 v1 v2
    nop 10

    lsu1.stxvi v7 i17 i3 || bru.rpl i5 i6 ___noWorkaroundEnd
    ___noWorkaroundloop:
    lsu0.stxvi v8 i7 i3
    vau.isubs.u16 v5 v3 v2 || lsu1.ldxvi v3 i18 i3
    vau.isubs.u16 v6 v4 v2 || lsu0.ldxvi v4 i8 i3
    vau.isubs.u16 v7 v0 v2 || lsu1.ldxvi v0 i18 i3
    vau.isubs.u16 v8 v1 v2 || lsu0.ldxvi v1 i8 i3
    lsu1.stxvi v5 i17 i3
	lsu0.stxvi v6 i7 i3
   	___noWorkaroundEnd:


   	___noWorkaroundCompesate:
    iau.and i9 i16 i4
    cmu.cmz.i32 i9
    peu.pc1c eq || bru.jmp i30
    nop
    iau.incs i9 -8
    nop 4

    bru.jmp i30
   	lsu0.stxvi v7 i17 i3 || cmu.cmz.i32 i9
   	peu.pc1c gt 		 || lsu1.stxvi v8 i7 i3
   	iau.incs i9 -8       ||  vau.isubs.u16 v5 v3 v2
    cmu.cmz.i32 i9
    peu.pc1c gt         || lsu1.stxvi v5 i17 i3
    nop

    //deprecated
    //void pixelUnpackerMipi10b_asm(u32 *in, u16 *out, u32 width, u32 lineNo, u32 coefs[4])//
    //		   						i18			i17			i16			i15			i14

    _Z29mvispPixelUnpackerMipi10b_asmPjPtjjS_:
    	lsu0.ldil i1 pixelUnpackerMipi10b_mask1 	|| lsu1.ldih i1 pixelUnpackerMipi10b_mask1
    	lsu0.ldil i2 pixelUnpackerMipi10b_mask2 	|| lsu1.ldih i2 pixelUnpackerMipi10b_mask2
    	lsu0.ldxv v0 i18		|| iau.xor i0 i0 i0
    	lsu1.ld.32r v21 i1		|| iau.incs i0 0x06
    	lsu1.ld.32r v22 i2		|| cmu.cpivr.x32 v23 i0 	|| iau.incs i0 0x0A
    	lsu0.ld.32 i3 i14		|| lsu1.ldo.32 i4 i14 0x04	|| iau.incs i18 0x10
    	lsu0.ldo.32 i5 i14 0x08 || lsu1.ldo.32 i6 i14 0x0C
    	iau.shr.u32 i13 i16 4
    	nop 2
    	lsu0.ldil i8 0x0001
    	iau.and i15 i15 i8
    	cmu.cmii.u32 i15 i8		|| lsu0.ldxvi v10 i18 i0
    	peu.pc1c lt	|| cmu.cpii i7 i4
    	peu.pc1c lt	|| iau.shl i9 i3 16
    	peu.pc1c lt	|| iau.or i10 i7 i9

    	peu.pc1c eq	|| cmu.cpii i7 i6
    	peu.pc1c eq	|| iau.shl i9 i5 16
    	peu.pc1c eq	|| iau.or i10 i7 i9

    	lsu0.ldil i12 pixelUnpackerMipi10b_loopUnpackMipiDeprecated	|| lsu1.ldih i12 pixelUnpackerMipi10b_loopUnpackMipiDeprecated
    	cmu.cpivr.x32 v20 i10


    	vau.and v1 v0 v21
    	vau.and v11 v10 v21
    	vau.and v2 v0 v22
    	lsu0.ldxvi v0 i18 i0	|| vau.and v12 v10 v22
    	vau.shl.x32 v3 v2 v23
    	vau.shl.x32 v13 v12 v23	|| lsu0.ldxvi v10 i18 i0
    	cmu.cmii.u32 i16 i0
    	peu.pc1c lte || bru.bra pixelUnpackerMipi10b_endUnpackMipiDeprecated
    	iau.xor i1 i1 i1
    	vau.or v4 v1 v3
    	vau.or v14 v11 v13
    	vau.isubs.u16 v5 v4 v20
    	vau.isubs.u16 v15 v14 v20
    	nop


    	vau.and v1 v0 v21		|| bru.rpl i12 i13 pixelUnpackerMipi10b_endUnpackMipiDeprecated
    	vau.and v11 v10 v21		|| lsu0.ldxvi v0 i18 i0
    	vau.and v2 v0 v22		|| iau.incs i16 -16
    	vau.and v12 v10 v22		|| lsu0.ldxvi v10 i18 i0
    	vau.shl.x32 v3 v2 v23

     pixelUnpackerMipi10b_loopUnpackMipiDeprecated:
    	vau.shl.x32 v13 v12 v23
    	lsu1.stxvi v5 i17 i0
    	vau.or v4 v1 v3
    	vau.or v14 v11 v13
    	vau.isubs.u16 v5 v4 v20
    	vau.isubs.u16 v15 v14 v20
    	lsu1.stxvi v15 i17 i0


     pixelUnpackerMipi10b_endUnpackMipiDeprecated:
    	iau.sub i4 i16 7
    	cmu.cmii.u32 i4 i1
    	peu.pc1c gt || lsu0.sto.64.l v5 i17 0x00
    	peu.pc1c gt || lsu0.sto.64.h v5 i17 0x08
    	iau.sub i4 i16 15
    	cmu.cmii.i32 i4 i1
    	peu.pc1c gt || lsu0.sto.64.l v15 i17 0x10
    	peu.pc1c gt || lsu0.sto.64.h v15 i17 0x18

    	bru.jmp i30
    	nop 6
.nowarnend
.size _Z29mvispPixelUnpackerMipi10b_asmPjPtjjS_i,.-_Z29mvispPixelUnpackerMipi10b_asmPjPtjjS_i
.end
