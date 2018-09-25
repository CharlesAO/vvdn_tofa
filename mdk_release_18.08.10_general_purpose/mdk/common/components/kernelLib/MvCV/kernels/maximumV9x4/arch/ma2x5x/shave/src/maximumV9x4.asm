///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
///// This kernel performs 4 maximum lines for each 9 input lines, in order,  of 12 input lines
///// @param[in] in        - 9 Input lines      
///// @param[out] out      - 4 Output lines    
///// @param[in] width     - Width of input line

//--------------------------------------------------------------------------------------------------------------
.type mvcvMaximumV9x4_asm,@function
.version 00.50.05
.data .rodata.vect_mask 
.align 4
.salign 16
 MaximumV9x4_vect_mask:
		.float16    0.1111
 MaximumV9x4_vect_mask2:
		.short    1


.code .text.MaximumV9x4 //text
.salign 16

.align 16  
//void mvcvMaximumV9x4(u8** in, u8** out, u32 width)
//                         i18      i17       i16
mvcvMaximumV9x4_asm:
.nowarn 9,10,11,12
lsu0.ldi.32 i0 i18 || iau.shr.u32 i16 i16 3
lsu0.ldi.32 i1 i18
lsu0.ldi.32 i2 i18
lsu0.ldi.32 i3 i18
lsu0.ldi.32 i4 i18
lsu0.ldi.32 i5 i18
lsu0.ldi.32 i6 i18
lsu0.ldi.32 i7 i18
lsu0.ldi.32 i8 i18
lsu0.ldi.32 i9 i18
lsu0.ldi.32 i10 i18 
lsu0.ldi.32 i18 i18

lsu0.ldi.32 i12 i17 || lsu1.ldil i11 mvcvMaximumV9x4loop
lsu0.ldi.32 i13 i17 || lsu1.ldih i11 mvcvMaximumV9x4loop
lsu0.ldi.32 i14 i17
lsu0.ldi.32 i15 i17

lsu0.ldo.64.l v0  i0  0 || lsu1.ldo.64.h v0  i0  8 || iau.add i0  i0  16 
lsu0.ldo.64.l v1  i1  0 || lsu1.ldo.64.h v1  i1  8 || iau.add i1  i1  16
lsu0.ldo.64.l v2  i2  0 || lsu1.ldo.64.h v2  i2  8 || iau.add i2  i2  16
lsu0.ldo.64.l v3  i3  0 || lsu1.ldo.64.h v3  i3  8 || iau.add i3  i3  16
lsu0.ldo.64.l v4  i4  0 || lsu1.ldo.64.h v4  i4  8 || iau.add i4  i4  16
lsu0.ldo.64.l v5  i5  0 || lsu1.ldo.64.h v5  i5  8 || iau.add i5  i5  16
lsu0.ldo.64.l v6  i6  0 || lsu1.ldo.64.h v6  i6  8 || iau.add i6  i6  16
lsu0.ldo.64.l v7  i7  0 || lsu1.ldo.64.h v7  i7  8 || iau.add i7  i7  16
lsu0.ldo.64.l v8  i8  0 || lsu1.ldo.64.h v8  i8  8 || iau.add i8  i8  16
lsu0.ldo.64.l v9  i9  0 || lsu1.ldo.64.h v9  i9  8 || iau.add i9  i9  16
lsu0.ldo.64.l v10 i10 0 || lsu1.ldo.64.h v10 i10 8 || iau.add i10 i10 16
lsu0.ldo.64.l v11 i18 0 || lsu1.ldo.64.h v11 i18 8 || iau.add i18 i18 16
nop 
cmu.max.f16 v12 v0 v1    
cmu.max.f16 v13 v2 v3
cmu.max.f16 v14 v4 v5
cmu.max.f16 v15 v6 v7
cmu.max.f16 v16 v8 v9    || bru.rpl i11 i16 
cmu.max.f16 v17 v10 v11  || lsu0.ldo.64.l v0  i0  0 || lsu1.ldo.64.h v0  i0  8 || iau.add i0  i0  16 
cmu.max.f16 v18 v12 v13  || lsu0.ldo.64.l v5  i5  0 || lsu1.ldo.64.h v5  i5  8 || iau.add i5  i5  16 
cmu.max.f16 v19 v13 v14  || lsu0.ldo.64.l v2  i2  0 || lsu1.ldo.64.h v2  i2  8 || iau.add i2  i2  16
cmu.max.f16 v20 v14 v15  || lsu0.ldo.64.l v6  i6  0 || lsu1.ldo.64.h v6  i6  8 || iau.add i6  i6  16
cmu.max.f16 v21 v15 v16  || lsu0.ldo.64.l v4  i4  0 || lsu1.ldo.64.h v4  i4  8 || iau.add i4  i4  16
cmu.max.f16 v22 v16 v17  || lsu0.ldo.64.l v1  i1  0 || lsu1.ldo.64.h v1  i1  8 || iau.add i1  i1  16 
cmu.max.f16 v23 v18 v20  || lsu0.ldo.64.l v3  i3  0 || lsu1.ldo.64.h v3  i3  8 || iau.add i3  i3  16
cmu.max.f16 v19 v19 v21  || lsu0.ldo.64.l v7  i7  0 || lsu1.ldo.64.h v7  i7  8 || iau.add i7  i7  16
cmu.max.f16 v20 v20 v22  || lsu0.ldo.64.l v8  i8  0 || lsu1.ldo.64.h v8  i8  8 || iau.add i8  i8  16
cmu.max.f16 v23 v23 v8   || lsu0.ldo.64.l v9  i9  0 || lsu1.ldo.64.h v9  i9  8 || iau.add i9  i9  16
mvcvMaximumV9x4loop:
cmu.max.f16 v22 v19 v1   || lsu0.ldo.64.l v10 i10 0 || lsu1.ldo.64.h v10 i10 8 || iau.add i10 i10 16
cmu.max.f16 v21 v19 v10  || lsu0.ldo.64.l v11 i18 0 || lsu1.ldo.64.h v11 i18 8 || iau.add i18 i18 16
cmu.max.f16 v20 v20 v3   
lsu0.sto.64.l v23 i12 0 || lsu1.sto.64.h v23 i12 8 || iau.add i12 i12 16 || cmu.max.f16 v14 v4 v5
lsu0.sto.64.l v22 i13 0 || lsu1.sto.64.h v22 i13 8 || iau.add i13 i13 16 || cmu.max.f16 v12 v0 v1
lsu0.sto.64.l v21 i14 0 || lsu1.sto.64.h v21 i14 8 || iau.add i14 i14 16 || cmu.max.f16 v13 v2 v3
lsu0.sto.64.l v20 i15 0 || lsu1.sto.64.h v20 i15 8 || iau.add i15 i15 16 || cmu.max.f16 v15 v6 v7

bru.jmp i30 
nop 6   
.nowarnend 
.size mvcvMaximumV9x4_asm,.-mvcvMaximumV9x4_asm
.end
