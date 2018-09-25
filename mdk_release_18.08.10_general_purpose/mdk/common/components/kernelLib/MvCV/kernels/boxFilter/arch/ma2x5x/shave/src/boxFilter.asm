///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

//--------------------------------------------------------------------------------------------------------------
.type mvcvBoxfilter_asm,@function
.version 00.50.05
.data .rodata.vect_mask 
.align 4
 boxFilter_vect_mask:
		.short 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x0000 
		.short 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff
 boxFilter_vect_mask2:
		.short 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x0000 
		.short 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff
 boxFilter_vect_mask3:
		.short 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x0000 
		.short 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff
 boxFilter_vect_mask4:
		.short 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x0000 
		.short 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff

.code .text.boxfilter //text

mvcvBoxfilter_asm:
.nowarn
	LSU0.ldil i1 0x0 || LSU1.ldih i1 0x0
	LSU0.ldil i2 boxFilter_second || LSU1.ldih i2 boxFilter_second
	CMU.CMII.i32 i1 i15
	PEU.PC1C EQ || BRU.JMP i2
nop 6
	

	LSU0.ldil i1 boxFilter_greater || LSU1.ldih i1 boxFilter_greater
	LSU0.ldil i2 0x8 || LSU1.ldih i2 0x0
	CMU.CMII.i32 i16 i2 
	PEU.PC1C GT || BRU.JMP i1
nop 6 
	LSU0.ldil i1 boxFilter_vect_mask || LSU1.ldih i1 boxFilter_vect_mask
	LSU0.LD.64.L   v0  i1 || IAU.ADD i1, i1, 8
	LSU0.LD.64.H   v0  i1 || IAU.ADD i1, i1, 8
	LSU0.LD.64.L   v1  i1 || IAU.ADD i1, i1, 8
	LSU0.LD.64.H   v1  i1 || IAU.ADD i1, i1, 8
	LSU0.ldil i4 boxFilter_mainloop || LSU1.ldih i4 boxFilter_mainloop
	VAU.XOR v4 v4 v4
	LSU0.ldil i5 0x1 || LSU1.ldih i5 0x0
	CMU.cpii.i32.f32 i0 i5
	IAU.MUL i7 i16 i16
nop
	CMU.cpii.i32.f32 i11 i7
nop
	SAU.DIV.f32 i12 i0 i11
	LSU0.LD.32  i17  i17

	LSU0.ldil i2 0x8 || LSU1.ldih i2 0x0
	IAU.sub i3 i2 i16
	BRU.RPI i3 || CMU.ALIGNVEC V0, v0 v0 2 || VAU.AND v1 v1 v0 //mask for pixels
	IAU.SUB i3 i3 i3 //contor for pixels
	IAU.sub i6 i6 i6 //contor for lines to substract
	IAU.sub i8 i8 i8 //contor for lines 
	
	LSU0.ldil i13 0x3 || LSU1.ldih i13 0x0
	
 boxFilter_mainloop:
	LSU0.LD.32  i1  i18 || IAU.ADD i18 i18 4
	IAU.ADD i6 i6 4
	IAU.ADD i8 i8 1
	CMU.CMII.i32 i8 i16
nop 3
	IAU.ADD i1 i1 i3
	LSU0.LD.128.U8.U16 v3 i1
	PEU.PC1C LT || BRU.JMP i4
nop 5
	VAU.ADD.i16 v4 v4 v3

nop
	IAU.SUB i18 i18 i6 || VAU.AND v4 v4 v1 
	IAU.SUB i6 i6 i6   || SAU.SUMX.U16 i5 v4 
	IAU.SUB i8 i8 i8
	CMU.cpii.i32.f32 i0 i5
	VAU.XOR v4 v4 v4 || 	IAU.ADD i3 i3 1
	SAU.MUL.f32 i0 i0 i12
	CMU.CMII.i32 i3 i14	
	PEU.PC1C LT || BRU.JMP i4
	CMU.CPII.f32.i32s i5 i0
nop
	LSU0.STI.8 i5 i17
nop 3

	bru.jmp i30
nop 6
	
	
	//k_size>8<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 boxFilter_greater:

	LSU0.ldil i1 boxFilter_vect_mask2 || LSU1.ldih i1 boxFilter_vect_mask2
	LSU0.LD.64.L   v0  i1 || IAU.ADD i1, i1, 8
	LSU0.LD.64.H   v0  i1 || IAU.ADD i1, i1, 8
	LSU0.LD.64.L   v1  i1 || IAU.ADD i1, i1, 8
	LSU0.LD.64.H   v1  i1 || IAU.ADD i1, i1, 8
	LSU0.ldil i4 boxFilter_mainloop2 || LSU1.ldih i4 boxFilter_mainloop2
	VAU.XOR v4 v4 v4 //sum for first part
	VAU.XOR v5 v5 v5 //sum for second part
	LSU0.ldil i5 0x1 || LSU1.ldih i5 0x0
	CMU.cpii.i32.f32 i0 i5
	IAU.MUL i7 i16 i16
nop
	CMU.cpii.i32.f32 i11 i7
nop
	SAU.DIV.f32 i12 i0 i11
	LSU0.LD.32  i17  i17
	LSU0.ldil i2 0x8 || LSU1.ldih i2 0x0
	IAU.SUB i9 i16 i2
	IAU.sub i3 i2 i9
	BRU.RPI i3 || CMU.ALIGNVEC V0, v0 v0 2 || VAU.AND v1 v1 v0 //mask for pixels
	IAU.SUB i3 i3 i3 //contor for pixels
	IAU.sub i6 i6 i6 //contor for lines to substract
	IAU.sub i8 i8 i8 //contor for lines 
	
	
	LSU0.ldil i13 0x3 || LSU1.ldih i13 0x0
	
 boxFilter_mainloop2:
 
	LSU0.LD.32  i1  i18 || IAU.ADD i18 i18 4
 
	IAU.ADD i6 i6 4
	IAU.ADD i8 i8 1
	CMU.CMII.i32 i8 i16
nop 3
	IAU.ADD i1 i1 i3
	LSU0.LD.128.U8.U16 v3 i1 || IAU.ADD i1 i1 8
	LSU0.LD.128.U8.U16 v6 i1
nop 5
	PEU.PC1C LT || BRU.JMP i4
	VAU.ADD.i16 v4 v4 v3
	VAU.ADD.i16 v5 v5 v6
nop 4
	IAU.SUB i18 i18 i6
	IAU.SUB i6 i6 i6 || SAU.SUB.i32 i8 i8 i8
	VAU.AND v5 v5 v1
	SAU.SUMX.U16 i10 v5 
	SAU.SUMX.U16 i5 v4
	VAU.XOR v5 v5 v5 || IAU.ADD i3 i3 1
	IAU.ADD i5 i10 i5
	
	CMU.cpii.i32.f32 i0 i5
	VAU.XOR v4 v4 v4
	SAU.MUL.f32 i0 i0 i12
	CMU.CMII.i32 i3 i14	
	PEU.PC1C LT || BRU.JMP i4
	CMU.CPII.f32.i32s i5 i0
nop 2
	LSU0.STI.8 i5 i17
nop 2
	
	bru.jmp i30
nop 6
	
	
 boxFilter_second:
	LSU0.ldil i1 boxFilter_greater2 || LSU1.ldih i1 boxFilter_greater2
	LSU0.ldil i2 0x8 || LSU1.ldih i2 0x0
	CMU.CMII.i32 i16 i2 
	PEU.PC1C GT || BRU.JMP i1
nop 6 
	LSU0.ldil i1 boxFilter_vect_mask3 || LSU1.ldih i1 boxFilter_vect_mask3
	LSU0.LD.64.L   v0  i1 || IAU.ADD i1, i1, 8
	LSU0.LD.64.H   v0  i1 || IAU.ADD i1, i1, 8
	LSU0.LD.64.L   v1  i1 || IAU.ADD i1, i1, 8
	LSU0.LD.64.H   v1  i1 || IAU.ADD i1, i1, 8
	LSU0.ldil i4 boxFilter_mainloop3 || LSU1.ldih i4 boxFilter_mainloop3
	VAU.XOR v4 v4 v4
	LSU0.ldil i5 0x1 || LSU1.ldih i5 0x0
	CMU.cpii.i32.f32 i0 i5
	IAU.MUL i7 i16 i16
nop
	CMU.cpii.i32.f32 i11 i7
nop
	SAU.DIV.f32 i12 i0 i0
	LSU0.LD.32  i17  i17

	LSU0.ldil i2 0x8 || LSU1.ldih i2 0x0
	IAU.sub i3 i2 i16
	BRU.RPI i3 || CMU.ALIGNVEC V0, v0 v0 2 || VAU.AND v1 v1 v0 //mask for pixels
	IAU.SUB i3 i3 i3 //contor for pixels
	IAU.sub i6 i6 i6 //contor for lines to substract
	IAU.sub i8 i8 i8 //contor for lines 
	
	LSU0.ldil i13 0x3 || LSU1.ldih i13 0x0
	
 boxFilter_mainloop3:
	LSU0.LD.32  i1  i18 || IAU.ADD i18 i18 4
	IAU.ADD i6 i6 4
	IAU.ADD i8 i8 1
	CMU.CMII.i32 i8 i16
nop 3
	IAU.ADD i1 i1 i3
	LSU0.LD.128.U8.U16 v3 i1
	PEU.PC1C LT || BRU.JMP i4
nop 5
	VAU.ADD.i16 v4 v4 v3

nop
	IAU.SUB i18 i18 i6 || VAU.AND v4 v4 v1 
	IAU.SUB i6 i6 i6 || SAU.SUMX.U16 i5 v4 
	IAU.SUB i8 i8 i8
	CMU.cpii.i32.f32 i0 i5

	VAU.XOR v4 v4 v4 || 	IAU.ADD i3 i3 1
	SAU.MUL.f32 i0 i0 i12
	CMU.CMII.i32 i3 i14	
	PEU.PC1C LT || BRU.JMP i4
	CMU.CPII.f32.i32s i5 i0
nop
	LSU0.STI.16 i5 i17
nop 3

	bru.jmp i30
nop 6
	
	
	//k_size>8<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 boxFilter_greater2:

	LSU0.ldil i1 boxFilter_vect_mask4 || LSU1.ldih i1 boxFilter_vect_mask4
	LSU0.LD.64.L   v0  i1 || IAU.ADD i1, i1, 8
	LSU0.LD.64.H   v0  i1 || IAU.ADD i1, i1, 8
	LSU0.LD.64.L   v1  i1 || IAU.ADD i1, i1, 8
	LSU0.LD.64.H   v1  i1 || IAU.ADD i1, i1, 8
	LSU0.ldil i4 boxFilter_mainloop4 || LSU1.ldih i4 boxFilter_mainloop4
	VAU.XOR v4 v4 v4 //sum for first part
	VAU.XOR v5 v5 v5 //sum for second part
	LSU0.ldil i5 0x1 || LSU1.ldih i5 0x0
	CMU.cpii.i32.f32 i0 i5
	IAU.MUL i7 i16 i16
nop
	CMU.cpii.i32.f32 i11 i7
nop
	SAU.DIV.f32 i12 i0 i0
	LSU0.LD.32  i17  i17
	LSU0.ldil i2 0x8 || LSU1.ldih i2 0x0
	IAU.SUB i9 i16 i2
	IAU.sub i3 i2 i9
	BRU.RPI i3 || CMU.ALIGNVEC V0, v0 v0 2 || VAU.AND v1 v1 v0 //mask for pixels
	IAU.SUB i3 i3 i3 //contor for pixels
	IAU.sub i6 i6 i6 //contor for lines to substract
	IAU.sub i8 i8 i8 //contor for lines 
	
	
	LSU0.ldil i13 0x3 || LSU1.ldih i13 0x0
	
 boxFilter_mainloop4:
	LSU0.LD.32  i1  i18 || IAU.ADD i18 i18 4
	IAU.ADD i6 i6 4
	IAU.ADD i8 i8 1
	CMU.CMII.i32 i8 i16
nop 3
	IAU.ADD i1 i1 i3
	LSU0.LD.128.U8.U16 v3 i1 || IAU.ADD i1 i1 8
	LSU0.LD.128.U8.U16 v6 i1
nop 5
	PEU.PC1C LT || BRU.JMP i4
	VAU.ADD.i16 v4 v4 v3
	VAU.ADD.i16 v5 v5 v6
nop 4


	
	IAU.SUB i18 i18 i6
	IAU.SUB i6 i6 i6 || SAU.SUB.i32 i8 i8 i8
	VAU.AND v5 v5 v1
	SAU.SUMX.U16 i10 v5
	SAU.SUMX.U16 i5 v4 
	VAU.XOR v5 v5 v5 || IAU.ADD i3 i3 1
	IAU.ADD i5 i10 i5
	
	CMU.cpii.i32.f32 i0 i5
	VAU.XOR v4 v4 v4
	SAU.MUL.f32 i0 i0 i12
	CMU.CMII.i32 i3 i14	
	PEU.PC1C LT || BRU.JMP i4
	CMU.CPII.f32.i32s i5 i0
nop
	LSU0.STI.16 i5 i17
nop 3
	
	bru.jmp i30
nop 6
	.nowarnend
	
.size mvcvBoxfilter_asm,.-mvcvBoxfilter_asm
.end
