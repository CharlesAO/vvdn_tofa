///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
///// This kernel performs 1 maximum line from 9 input lines
///// @param[in] in        - 3 Input lines
///// @param[out] out      - 1 Output line
///// @param[in] width     - Width of input line

//--------------------------------------------------------------------------------------------------------------
.type mvcvMaximumV9_asm,@function
.version 00.50.05
.data .rodata.vect_mask
.align 4
.salign 16

.code .text.MaximumV9 //text
.salign 16

.align 16
//void mvcvMaximumV9(u8** in, u8** out, u32 width)
//                         i18      i17       i16
mvcvMaximumV9_asm:
.nowarn 9,10,11,12
	IAU.SUB i19 i19 8
	LSU0.ST.64.l v24  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v24  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v25  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v25  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v26  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v26  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v27  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v27  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v28  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v28  i19
lsu0.ldi.32 i0 i18
lsu0.ldi.32 i1 i18
lsu0.ldi.32 i2 i18 || iau.shr.u32 i15 i16 4
lsu0.ldi.32 i3 i18 || IAU.SHL i14 i15 4
lsu0.ldi.32 i4 i18 || IAU.SUB i13 i16 i14
lsu0.ldi.32 i5 i18
lsu0.ldi.32 i6 i18
lsu0.ldi.32 i7 i18
lsu0.ldi.32 i8 i18

lsu0.ldi.32 i12 i17 || lsu1.ldil i11 mvcvMaximumV9loop
                       lsu1.ldih i11 mvcvMaximumV9loop


lsu0.ldo.64.l v0  i0  0 || lsu1.ldo.64.h v0  i0  8 || iau.add i0  i0  16
lsu0.ldo.64.l v1  i1  0 || lsu1.ldo.64.h v1  i1  8 || iau.add i1  i1  16
lsu0.ldo.64.l v2  i2  0 || lsu1.ldo.64.h v2  i2  8 || iau.add i2  i2  16 || CMU.CMZ.i32 i15
PEU.PC1C eq || BRU.BRA mvcvMaximumV9loop_compensate
lsu0.ldo.64.l v3  i3  0 || lsu1.ldo.64.h v3  i3  8 || iau.add i3  i3  16
lsu0.ldo.64.l v4  i4  0 || lsu1.ldo.64.h v4  i4  8 || iau.add i4  i4  16
lsu0.ldo.64.l v5  i5  0 || lsu1.ldo.64.h v5  i5  8 || iau.add i5  i5  16
lsu0.ldo.64.l v6  i6  0 || lsu1.ldo.64.h v6  i6  8 || iau.add i6  i6  16
lsu0.ldo.64.l v7  i7  0 || lsu1.ldo.64.h v7  i7  8 || iau.add i7  i7  16
lsu0.ldo.64.l v8  i8  0 || lsu1.ldo.64.h v8  i8  8 || iau.add i8  i8  16
lsu0.ldo.64.l v16  i0  0 || lsu1.ldo.64.h v16  i0  8 || iau.add i0  i0  16
lsu0.ldo.64.l v17  i1  0 || lsu1.ldo.64.h v17  i1  8 || iau.add i1  i1  16
lsu0.ldo.64.l v18  i2  0 || lsu1.ldo.64.h v18  i2  8 || iau.add i2  i2  16
lsu0.ldo.64.l v19  i3  0 || lsu1.ldo.64.h v19  i3  8 || iau.add i3  i3  16
lsu0.ldo.64.l v20  i4  0 || lsu1.ldo.64.h v20  i4  8 || iau.add i4  i4  16
lsu0.ldo.64.l v21  i5  0 || lsu1.ldo.64.h v21  i5  8 || iau.add i5  i5  16
lsu0.ldo.64.l v22  i6  0 || lsu1.ldo.64.h v22  i6  8 || iau.add i6  i6  16
lsu0.ldo.64.l v23  i7  0 || lsu1.ldo.64.h v23  i7  8 || iau.add i7  i7  16
lsu0.ldo.64.l v24  i8  0 || lsu1.ldo.64.h v24  i8  8 || iau.add i8  i8  16


                         lsu0.ldo.64.l v0  i0  0 || lsu1.ldo.64.h v0  i0  8 || iau.add i0  i0  16   || bru.rpl i11 i15
                         lsu0.ldo.64.l v1  i1  0 || lsu1.ldo.64.h v1  i1  8 || iau.add i1  i1  16
cmu.max.f16 v12 v0 v1    || lsu0.ldo.64.l v2  i2  0 || lsu1.ldo.64.h v2  i2  8 || iau.add i2  i2  16
cmu.max.f16 v13 v2 v3    || lsu0.ldo.64.l v3  i3  0 || lsu1.ldo.64.h v3  i3  8 || iau.add i3  i3  16
cmu.max.f16 v14 v4 v5    || lsu0.ldo.64.l v4  i4  0 || lsu1.ldo.64.h v4  i4  8 || iau.add i4  i4  16
cmu.max.f16 v15 v6 v7    || lsu0.ldo.64.l v5  i5  0 || lsu1.ldo.64.h v5  i5  8 || iau.add i5  i5  16
cmu.max.f16 v12 v12 v13  || lsu0.ldo.64.l v6  i6  0 || lsu1.ldo.64.h v6  i6  8 || iau.add i6  i6  16
cmu.max.f16 v14 v14 v15  || lsu0.ldo.64.l v7  i7  0 || lsu1.ldo.64.h v7  i7  8 || iau.add i7  i7  16
cmu.max.f16 v25 v16 v17  || lsu0.ldo.64.l v23  i7  0 || lsu1.ldo.64.h v23  i7  8 || iau.add i7  i7  16
cmu.max.f16 v26 v18 v19  || lsu0.ldo.64.l v16  i0  0 || lsu1.ldo.64.h v16  i0  8 || iau.add i0  i0  16
cmu.max.f16 v27 v20 v21  || lsu0.ldo.64.l v17  i1  0 || lsu1.ldo.64.h v17  i1  8 || iau.add i1  i1  16
cmu.max.f16 v28 v22 v23  || lsu0.ldo.64.l v18  i2  0 || lsu1.ldo.64.h v18  i2  8 || iau.add i2  i2  16
cmu.max.f16 v25 v25 v26  || lsu0.ldo.64.l v19  i3  0 || lsu1.ldo.64.h v19  i3  8 || iau.add i3  i3  16
mvcvMaximumV9loop:
cmu.max.f16 v27 v27 v28  || lsu0.ldo.64.l v20  i4  0 || lsu1.ldo.64.h v20  i4  8 || iau.add i4  i4  16
cmu.max.f16 v14 v12 v14  || lsu0.ldo.64.l v21  i5  0 || lsu1.ldo.64.h v21  i5  8 || iau.add i5  i5  16
cmu.max.f16 v25 v25 v27  || lsu0.ldo.64.l v22  i6  0 || lsu1.ldo.64.h v22  i6  8 || iau.add i6  i6  16
cmu.max.f16 v14 v14 v8    || lsu0.ldo.64.l v8  i8  0 || lsu1.ldo.64.h v8  i8  8 || iau.add i8  i8  16
cmu.max.f16 v25 v25 v24  || lsu0.ldo.64.l v24  i8  0 || lsu1.ldo.64.h v24  i8  8 || iau.add i8  i8  16
lsu0.sto.64.l v14 i12 0 || lsu1.sto.64.h v14 i12 8 || iau.add i12 i12 16
lsu0.sto.64.l v25 i12 0 || lsu1.sto.64.h v25 i12 8 || iau.add i12 i12 16


mvcvMaximumV9loop_compensate:
nop 2
cmu.max.f16 v12 v0 v1
cmu.max.f16 v13 v2 v3
cmu.max.f16 v14 v4 v5
cmu.max.f16 v15 v6 v7
cmu.max.f16 v12 v12 v13
cmu.max.f16 v14 v14 v15
nop
cmu.max.f16 v14 v12 v14
nop
cmu.max.f16 v14 v14 v8 || iau.sub i13 i13 0
nop
PEU.PC1I gt || lsu0.sto.64.l v14 i12 0 || lsu1.sto.64.h v14 i12 8 || iau.add i12 i12 16



	LSU0.LD.64.h  v28  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.l  v28  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v27  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.l  v27  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v26  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.l  v26  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v25  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.l  v25  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v24  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.l  v24  i19 || IAU.ADD i19 i19 8

bru.jmp i30
nop 6
.nowarnend
.size mvcvMaximumV9_asm,.-mvcvMaximumV9_asm
.end
