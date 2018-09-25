///
/// @file      
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Extract Chroma differences CrCgCb from (debayerd) RGB image
///

//--------------------------------------------------------------------------------------------------------------
.type mvispGenChromaRGB16_asm,@function
.version 00.51.04
.data .rodata.genChromaRGB16
.align 16
 genChromaRGB16_clamp:
	 .float16 255.0
 genChromaRGB16_val:
	 .float16 85.0
 genChromaRGB16_val2:
	 .short 0x01
.nowarn
.code .text.genChromaRGB16 
mvispGenChromaRGB16_asm:
// void genChromaRGB16(u16** inRGB, u8 *inY, u8** outC, int width, int eps, int nBits, float * chromaScale)
//                          i18       i17        i16       i15        i14      i13           i12
    IAU.SUB i19 i19 8
	LSU0.ST.64.l v24  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v24  i19	
	
	lsu0.ldo.64.l v24 i12 0 || lsu1.ldo.64.h v24 i12 8
	nop 10
	cmu.cpvv.f32.f16 v24 v24
	nop 10
	LSU0.LDIL i12 genChromaRGB16_loop   ||  LSU1.LDIH i12 genChromaRGB16_loop 
	LSU0.LDIL i7 genChromaRGB16_clamp  ||  LSU1.LDIH i7 genChromaRGB16_clamp 
	LSU0.LDIL i8 genChromaRGB16_val    ||  LSU1.LDIH i8 genChromaRGB16_val 
	LSU0.LDIL i9 genChromaRGB16_val2    ||  LSU1.LDIH i9 genChromaRGB16_val2 

	LSU0.LD.32 i0 i18 || iau.add i18 i18 4  // in
	LSU0.LD.32 i1 i18 || iau.add i18 i18 4  
	LSU0.LD.32 i2 i18 
	LSU0.LD.32 i3 i16 || iau.add i16 i16 4  // out
	LSU0.LD.32 i4 i16 || iau.add i16 i16 4  
	LSU0.LD.32 i5 i16 
	LSU0.LD.16R v21 i8  || iau.shr.u32 i15 i15 3  || LSU1.LD.16R v18, i7
	LSU0.LD.16R v22 i9 
	nop 6
	cmu.cpivr.x16 v20 i14 
	cmu.cpivr.x16 v19 i13 
	cmu.cpvv.i16.f16 v20 v20
	cmu.cpvi.x16 i11.l v22.0 
	nop
	iau.sub i10 i13 8 
	nop
	iau.shl i6 i11 i10
	nop 
	cmu.cpivr.x16 v23 i6 
	
	cmu.cpvv.i16.f16 v23 v23
	cmu.cpvi.x16 i6.l v21.0
	cmu.cpvi.x16 i6.h v21.0
	
	lsu0.ld.128.u8.f16 v0 i17 || iau.add i17 i17 8 
	lsu0.ldo.64.l v3 i0 0 || lsu1.ldo.64.h v3 i0 8 || iau.add i0 i0 16 //R
	lsu0.ldo.64.l v4 i1 0 || lsu1.ldo.64.h v4 i1 8 || iau.add i1 i1 16 //G
	lsu0.ldo.64.l v5 i2 0 || lsu1.ldo.64.h v5 i2 8 || iau.add i2 i2 16 //B
	nop 4
	vau.add.f16 v1 v0 v20  
	nop 2
	vau.mul.f16 v2 v1 v23 
	nop 2
	cmu.cpvi.x16 i10.l v2.0  || lsu0.ld.128.u8.f16 v0 i17 || iau.add i17 i17 8  
	cmu.cpvi.x16 i10.h v2.1 
	cmu.cpvi.x16 i9.l  v2.2 
	cmu.cpvi.x16 i9.h  v2.3
	cmu.cpvi.x16 i18.l v2.4 
	cmu.cpvi.x16 i18.h v2.5 
	cmu.cpvi.x16 i13.l v2.6 
	cmu.cpvi.x16 i13.h v2.7 || vau.add.f16 v1 v0 v20
	sau.div.f16 i16 i6 i10 
	sau.div.f16 i11 i6 i9  || cmu.cpvv.i16.f16 v3 v3
	sau.div.f16 i8 i6 i18  || cmu.cpvv.i16.f16 v4 v4
	sau.div.f16 i14 i6 i13 || cmu.cpvv.i16.f16 v5 v5
	nop 4
	cmu.cpiv.x32 v7.0 i16   || vau.mul.f16 v2 v1 v23 
	cmu.cpiv.x32 v7.1 i11   || lsu0.ldo.64.l v3 i0 0 || lsu1.ldo.64.h v3 i0 8 || iau.add i0 i0 16
	cmu.cpiv.x32 v7.2 i8    || lsu0.ldo.64.l v4 i1 0 || lsu1.ldo.64.h v4 i1 8 || iau.add i1 i1 16
	
	cmu.cpiv.x32 v7.3 i14   || lsu0.ldo.64.l v5 i2 0 || lsu1.ldo.64.h v5 i2 8 || iau.add i2 i2 16   || bru.rpl i12 i15 _end_delay_loop_genChromaRGB16
	cmu.cpvi.x16 i10.l v2.0 || vau.mul.f16 v6 v3 v7  || lsu0.ld.128.u8.f16 v0 i17 || iau.add i17 i17 8
	cmu.cpvi.x16 i10.h v2.1 || vau.mul.f16 v8 v4 v7  
	cmu.cpvi.x16 i9.l  v2.2 || vau.mul.f16 v9 v5 v7  
	cmu.cpvi.x16 i9.h  v2.3 || vau.mul.f16 v6 v24 v6  || LSU1.SWZV8 [00000000]
	cmu.cpvi.x16 i18.l v2.4 || vau.mul.f16 v8 v24 v8  || LSU1.SWZV8 [11111111]
	cmu.cpvi.x16 i18.h v2.5 || vau.mul.f16 v9 v24 v9  || LSU1.SWZV8 [22222222]
	cmu.cpvi.x16 i13.l v2.6   || sau.div.f16 i16 i6 i10  
	cmu.cpvi.x16 i13.h v2.7   || sau.div.f16 i11 i6 i9  
	cmu.clamp0.f16 v6 v6 v18  || sau.div.f16 i8 i6 i18  
	cmu.clamp0.f16 v8 v8 v18  || vau.add.f16 v1 v0 v20
	cmu.clamp0.f16 v9 v9 v18  || sau.div.f16 i14 i6 i13 
	genChromaRGB16_loop:
	cmu.cpvv.i16.f16 v3 v3 || lsu0.sti.128.f16.u8 v6 i3  
	cmu.cpvv.i16.f16 v4 v4 || lsu0.sti.128.f16.u8 v8 i4 || vau.mul.f16 v2 v1 v23
	cmu.cpvv.i16.f16 v5 v5 || lsu0.sti.128.f16.u8 v9 i5
	nop 
	cmu.cpiv.x32 v7.0 i16  
	cmu.cpiv.x32 v7.1 i11  || lsu0.ldo.64.l v3 i0 0 || lsu1.ldo.64.h v3 i0 8 || iau.add i0 i0 16 
	cmu.cpiv.x32 v7.2 i8   || lsu0.ldo.64.l v4 i1 0 || lsu1.ldo.64.h v4 i1 8 || iau.add i1 i1 16
	_end_delay_loop_genChromaRGB16:

	LSU0.LD.64.h  v24  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v24  i19 || IAU.ADD i19 i19 8	
	bru.jmp i30
	nop 6
	
.size mvispGenChromaRGB16_asm,.-mvispGenChromaRGB16_asm
.nowarnend
.end
	
