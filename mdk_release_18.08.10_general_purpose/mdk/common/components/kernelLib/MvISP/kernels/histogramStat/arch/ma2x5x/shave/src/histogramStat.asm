///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Calculate histogram statistics for RGB image
///

//--------------------------------------------------------------------------------------------------------------
.type mvispHistogramStat_asm,@function
.version 00.50.00
.data .rodata.histogramStat_asm 
.align 4

//.align 16

.code .text.histogramStat_asm //text
.lalign
mvispHistogramStat_asm:
////								i18         i17         i16  		i15   		i14   		i13  		i12			i11          
////void mvispHistogramStat(u8 *inputR, u8 *inputG, u8 *inputB, u32 *histR, u32 *histG, u32 *histB, u32 width, u32 step) 
	
	iau.incs i19 -16	|| cmu.cpzv v0 0x3
	lsu0.stxv v24 i19	|| iau.incs i19 -16	|| cmu.cpzv v4 0x3
	lsu1.stxv v25 i19	|| iau.incs i19 -16	|| cmu.cpzv v8 0x3
	lsu0.stxv v26 i19	|| iau.incs i19 -16	|| cmu.cpzv v12 0x3
	lsu1.stxv v27 i19	|| iau.incs i19 -16	|| cmu.cpzv v16 0x3
	lsu0.stxv v28 i19	|| iau.incs i19 -16	|| cmu.cpzv v20 0x3
	lsu1.stxv v29 i19	|| iau.incs i19 -16	
	lsu0.stxv v30 i19	|| iau.incs i19 -16	
	lsu1.stxv v31 i19	|| iau.shl i8 i11 3 
	
	
	lsu0.ld.128.u8.u16 v24 i18	|| iau.incs i8 8
	sau.div.u32 i7 i12 i8
	lsu1.ld.128.u8.u16 v25 i17	
	lsu0.ld.128.u8.u16 v26 i16	
	iau.xor i2 i2 i2
	iau.xor i1 i1 i1
	iau.xor i0 i0 i0
	vau.xor v31 v31 v31
	cmu.cpii i10 i15
	lsu0.ldil i6 0x10

	vau.shr.u16 v27 v24 2
	vau.shr.u16 v28 v25 2	
	vau.shr.u16 v29 v26 2	|| lsu0.ldil i4 0x08
	lsu1.ldil i9 histogramStat_loopHistStats
	lsu1.ldih i9 histogramStat_loopHistStats	
	iau.mul i5 i7 i8
	nop
	iau.sub i5 i12 i5
	peu.pc1i gt || iau.incs i7 1

	
	cmu.cpvi.x16 i0.l v27.0	|| bru.rpl i9 i7 histogramStat_endLoopHistStat
	cmu.cpvi.x16 i1.l v28.0
	cmu.cpvi.x16 i2.l v29.0
	cmu.lut.u16 i3 i0 0x00
	cmu.lut.u16 i4 i1 0x01
	cmu.lut.u16 i5 i2 0x02	|| iau.incs i3 1
	cmu.lutw.x16 i3 i0 0x00	|| iau.incs i4 1
	cmu.lutw.x16 i4 i1 0x01	|| iau.incs i5 1
	cmu.lutw.x16 i5 i2 0x02
	
	cmu.cpvi.x16 i0.l v27.1	
	cmu.cpvi.x16 i1.l v28.1	
	cmu.cpvi.x16 i2.l v29.1		
	cmu.lut.u16 i3 i0 0x00
	cmu.lut.u16 i4 i1 0x01
	cmu.lut.u16 i5 i2 0x02	|| iau.incs i3 1
	cmu.lutw.x16 i3 i0 0x00	|| iau.incs i4 1
	cmu.lutw.x16 i4 i1 0x01	|| iau.incs i5 1
	cmu.lutw.x16 i5 i2 0x02
	
	cmu.cpvi.x16 i0.l v27.2	|| iau.add i18 i18 i8
	cmu.cpvi.x16 i1.l v28.2	|| iau.add i17 i17 i8
	cmu.cpvi.x16 i2.l v29.2	|| iau.add i16 i16 i8
	cmu.lut.u16 i3 i0 0x00
	cmu.lut.u16 i4 i1 0x01
	cmu.lut.u16 i5 i2 0x02	|| iau.incs i3 1
	cmu.lutw.x16 i3 i0 0x00	|| iau.incs i4 1
	cmu.lutw.x16 i4 i1 0x01	|| iau.incs i5 1
	cmu.lutw.x16 i5 i2 0x02
	
	cmu.cpvi.x16 i0.l v27.3	|| lsu0.ld.128.u8.u16 v24 i18
	cmu.cpvi.x16 i1.l v28.3	|| lsu1.ld.128.u8.u16 v25 i17
	cmu.cpvi.x16 i2.l v29.3	|| lsu0.ld.128.u8.u16 v26 i16
	cmu.lut.u16 i3 i0 0x00
	cmu.lut.u16 i4 i1 0x01
	cmu.lut.u16 i5 i2 0x02	|| iau.incs i3 1
	cmu.lutw.x16 i3 i0 0x00	|| iau.incs i4 1
	cmu.lutw.x16 i4 i1 0x01	|| iau.incs i5 1
	cmu.lutw.x16 i5 i2 0x02
	
	cmu.cpvi.x16 i0.l v27.4
	cmu.cpvi.x16 i1.l v28.4
	cmu.cpvi.x16 i2.l v29.4
	cmu.lut.u16 i3 i0 0x00
	cmu.lut.u16 i4 i1 0x01
	cmu.lut.u16 i5 i2 0x02	|| iau.incs i3 1
	cmu.lutw.x16 i3 i0 0x00	|| iau.incs i4 1
	cmu.lutw.x16 i4 i1 0x01	|| iau.incs i5 1
	cmu.lutw.x16 i5 i2 0x02
	
	cmu.cpvi.x16 i0.l v27.5
	cmu.cpvi.x16 i1.l v28.5
	cmu.cpvi.x16 i2.l v29.5
	cmu.lut.u16 i3 i0 0x00
	cmu.lut.u16 i4 i1 0x01
	cmu.lut.u16 i5 i2 0x02	|| iau.incs i3 1
	cmu.lutw.x16 i3 i0 0x00	|| iau.incs i4 1
	cmu.lutw.x16 i4 i1 0x01	|| iau.incs i5 1
	cmu.lutw.x16 i5 i2 0x02
	
	cmu.cpvi.x16 i0.l v27.6
	cmu.cpvi.x16 i1.l v28.6
	cmu.cpvi.x16 i2.l v29.6
	cmu.lut.u16 i3 i0 0x00
	cmu.lut.u16 i4 i1 0x01
	cmu.lut.u16 i5 i2 0x02	|| iau.incs i3 1
	cmu.lutw.x16 i3 i0 0x00	|| iau.incs i4 1
	cmu.lutw.x16 i4 i1 0x01	|| iau.incs i5 1
	cmu.lutw.x16 i5 i2 0x02
	
	cmu.cpvi.x16 i0.l v27.7	
	cmu.cpvi.x16 i1.l v28.7
 histogramStat_loopHistStats:
	cmu.cpvi.x16 i2.l v29.7
	cmu.lut.u16 i3 i0 0x00	|| vau.shr.u16 v27 v24 2
	cmu.lut.u16 i4 i1 0x01	|| vau.shr.u16 v28 v25 2
	cmu.lut.u16 i5 i2 0x02	|| iau.incs i3 1
	cmu.lutw.x16 i3 i0 0x00	|| iau.incs i4 1
	cmu.lutw.x16 i4 i1 0x01	|| iau.incs i5 1
	cmu.lutw.x16 i5 i2 0x02	|| vau.shr.u16 v29 v26 2	
 histogramStat_endLoopHistStats:


	lsu0.ldxv v24 i15		|| iau.incs i15 16
	lsu1.ldxvi v25 i15 i6
	cmu.vilv.x16 v28 v29 v31 v0	 
	nop 3
	lsu0.ldxv v26 i15		|| iau.incs i15 16
	lsu1.ldxvi v27 i15 i6	
	vau.add.i32 v0 v24 v28 
	vau.add.i32 v30 v25 v29	|| cmu.vilv.x16 v28 v29 v31 v1
	lsu0.stxv v0 i10		|| iau.incs i10 16
	lsu1.stxvi v30 i10 i6		 
	lsu0.ldxv v24 i15	|| iau.incs i15 16
	lsu1.ldxvi v25 i15 i6	 
	vau.add.i32 v1 v26 v28 
	vau.add.i32 v30 v27 v29	|| cmu.vilv.x16 v28 v29 v31 v2
	lsu0.stxv v1 i10		|| iau.incs i10 16
	lsu1.stxvi v30 i10 i6
	lsu0.ldxv v26 i15		|| iau.incs i15 16
	lsu1.ldxvi v27 i15 i6	
	vau.add.i32 v2 v24 v28 
	vau.add.i32 v30 v25 v29	|| 	cmu.vilv.x16 v28 v29 v31 v3	
	lsu0.stxv v2 i10		|| iau.incs i10 16
	lsu1.stxvi v30 i10 i6 
	lsu0.ldxv v24 i15		|| iau.incs i15 16
	lsu1.ldxvi v25 i15 i6	
	vau.add.i32 v3 v26 v28 
	vau.add.i32 v30 v27 v29	|| cmu.vilv.x16 v28 v29 v31 v4	
	lsu0.stxv v3 i10 		|| iau.incs i10 16	
	lsu1.stxvi v30 i10 i6
	lsu0.ldxv v26 i15		|| iau.incs i15 16
	lsu1.ldxvi v27 i15 i6	
	vau.add.i32 v4 v24 v28 
	vau.add.i32 v30 v25 v29	|| cmu.vilv.x16 v28 v29 v31 v5	 
	lsu0.stxv v4 i10		|| iau.incs i10 16
	lsu1.stxvi v30 i10 i6
	lsu0.ldxv v24 i15		|| iau.incs i15 16
	lsu1.ldxvi v25 i15 i6	
	vau.add.i32 v5 v26 v28 
	vau.add.i32 v30 v27 v29	|| cmu.vilv.x16 v28 v29 v31 v6	 
	lsu0.stxv v5 i10		|| iau.incs i10 16	|| cmu.cpii i11 i14
	lsu1.stxvi v30 i10 i6
	lsu0.ldxv v26 i15		|| iau.incs i15 16	
	lsu1.ldxvi v27 i15 i6	
	vau.add.i32 v6 v24 v28 	
	vau.add.i32 v30 v25 v29	|| cmu.vilv.x16 v28 v29 v31 v7
	lsu0.stxv v6 i10		|| iau.incs i10 16	
	lsu1.stxvi v30 i10 i6	
	lsu0.ldxv v24 i14		|| iau.incs i14 16
	lsu1.ldxvi v25 i14 i6		
	vau.add.i32 v7 v26 v28 
	vau.add.i32 v30 v27 v29	|| cmu.vilv.x16 v28 v29 v31 v8	 	
	lsu0.stxv v7 i10		|| iau.incs i10 16
	lsu1.stxv v30 i10
	lsu0.ldxv v24 i14		|| iau.incs i14 16
	lsu1.ldxvi v25 i14 i6		
	vau.add.i32 v8 v24 v28 
	vau.add.i32 v30 v25 v29	|| cmu.vilv.x16 v28 v29 v31 v9	
	lsu0.stxv v8 i11		|| iau.incs i11 16
	lsu1.stxvi v30 i11 i6
	lsu0.ldxv v24 i14		|| iau.incs i14 16
	lsu1.ldxvi v25 i14 i6		
	vau.add.i32 v9 v24 v28 
	vau.add.i32 v30 v25 v29	|| cmu.vilv.x16 v28 v29 v31 v10
	lsu0.stxv v9 i11 		|| iau.incs i11 16
	lsu1.stxvi v30 i11 i6
	lsu0.ldxv v24 i14		|| iau.incs i14 16
	lsu1.ldxvi v25 i14 i6		
	vau.add.i32 v10 v24 v28 
	vau.add.i32 v30 v25 v29	|| cmu.vilv.x16 v28 v29 v31 v11
	lsu0.stxv v10 i11 		|| iau.incs i11 16
	lsu1.stxvi v30 i11 i6 
	lsu0.ldxv v24 i14		|| iau.incs i14 16
	lsu1.ldxvi v25 i14 i6		
	vau.add.i32 v11 v24 v28 
	vau.add.i32 v30 v25 v29	|| cmu.vilv.x16 v28 v29 v31 v12
	lsu0.stxv v11 i11		|| iau.incs i11 16
	lsu1.stxvi v30 i11 i6
	lsu0.ldxv v24 i14		|| iau.incs i14 16
	lsu1.ldxvi v25 i14 i6	
	vau.add.i32 v12 v24 v28 
	vau.add.i32 v30 v25 v29	|| cmu.vilv.x16 v28 v29 v31 v13
	lsu0.stxv v12 i11		|| iau.incs i11 16
	lsu1.stxvi v30 i11 i6
	lsu0.ldxv v24 i14		|| iau.incs i14 16
	lsu1.ldxvi v25 i14 i6		
	vau.add.i32 v13 v24 v28 
	vau.add.i32 v30 v25 v29	|| cmu.vilv.x16 v28 v29 v31 v14
	lsu0.stxv v13 i11		|| iau.incs i11 16	|| cmu.cpii i10 i13
	lsu1.stxvi v30 i11 i6
	lsu0.ldxv v24 i14		|| iau.incs i14 16 	
	lsu1.ldxvi v25 i14 i6	
	vau.add.i32 v14 v24 v28 
	vau.add.i32 v30 v25 v29	|| cmu.vilv.x16 v28 v29 v31 v15
	lsu0.stxv v14 i11		|| iau.incs i11 16
	lsu1.stxvi v30 i11 i6 	
	lsu0.ldxv v24 i13		|| iau.incs i13 16
	lsu1.ldxvi v25 i13 i6		
	vau.add.i32 v15 v24 v28 
	vau.add.i32 v30 v25 v29	|| cmu.vilv.x16 v28 v29 v31 v16	
	lsu0.stxv v15 i11		|| iau.incs i11 16
	lsu1.stxv v30 i11
	lsu0.ldxv v24 i13		|| iau.incs i13 16
	lsu1.ldxvi v25 i13 i6		
	vau.add.i32 v16 v24 v28 
	vau.add.i32 v30 v25 v29	|| cmu.vilv.x16 v28 v29 v31 v17
	lsu0.stxv v16 i10		|| iau.incs i10 16
	lsu1.stxvi v30 i10 i6
	lsu0.ldxv v24 i13		|| iau.incs i13 16
	lsu1.ldxvi v25 i13 i6	
	vau.add.i32 v17 v24 v28 
	vau.add.i32 v30 v25 v29	|| cmu.vilv.x16 v28 v29 v31 v18
	lsu0.stxv v17 i10		|| iau.incs i10 16
	lsu1.stxvi v30 i10 i6
	lsu0.ldxv v24 i13		|| iau.incs i13 16
	lsu1.ldxvi v25 i13 i6		
	vau.add.i32 v18 v24 v28 
	vau.add.i32 v30 v25 v29	|| cmu.vilv.x16 v28 v29 v31 v19	
	lsu0.stxv v18 i10		|| iau.incs i10 16
	lsu1.stxvi v30 i10 i6
	lsu0.ldxv v24 i13		|| iau.incs i13 16
	lsu1.ldxvi v25 i13 i6	
	vau.add.i32 v19 v24 v28 
	vau.add.i32 v30 v25 v29	|| cmu.vilv.x16 v28 v29 v31 v20
	lsu0.stxv v19 i10		|| iau.incs i10 16
	lsu1.stxvi v30 i10 i6
	lsu0.ldxv v24 i13		|| iau.incs i13 16
	lsu1.ldxvi v25 i13 i6	
	vau.add.i32 v20 v24 v28 
	vau.add.i32 v30 v25 v29	|| cmu.vilv.x16 v28 v29 v31 v21
	lsu0.stxv v20 i10		|| iau.incs i10 16
	lsu1.stxvi v30 i10 i6
	lsu0.ldxv v24 i13		|| iau.incs i13 16
	lsu1.ldxvi v25 i13 i6		
	vau.add.i32 v21 v24 v28 
	vau.add.i32 v30 v25 v29	|| cmu.vilv.x16 v28 v29 v31 v22
	lsu0.stxv v21 i10		|| iau.incs i10 16
	lsu1.stxvi v30 i10 i6
	lsu0.ldxv v24 i13		|| iau.incs i13 16
	lsu1.ldxvi v25 i13 i6
	vau.add.i32 v22 v24 v28 
	vau.add.i32 v30 v25 v29	|| cmu.vilv.x16 v28 v29 v31 v23
	lsu0.stxv v22 i10		|| iau.incs i10 16 
	lsu1.stxvi v30 i10 i6
	nop 2
	vau.add.i32 v23 v24 v28 
	vau.add.i32 v30 v25 v29	
	lsu0.stxv v23 i10		|| iau.incs i10 16	
	lsu1.stxv v30 i10
	

	
	lsu0.ldxv v31 i19	|| iau.incs i19 16
	lsu1.ldxv v30 i19	|| iau.incs i19 16
	lsu0.ldxv v29 i19	|| iau.incs i19 16
	lsu1.ldxv v28 i19	|| iau.incs i19 16
	lsu0.ldxv v27 i19	|| iau.incs i19 16
	lsu1.ldxv v26 i19	|| iau.incs i19 16
	lsu0.ldxv v25 i19	|| iau.incs i19 16
	lsu1.ldxv v24 i19	|| iau.incs i19 16
	
	
	bru.jmp i30
	nop 6
	
.size mvispHistogramStat_asm,.-mvispHistogramStat_asm
.end
	
	 
		

	
	
