///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///
        
.type mvcvGauss5x1_u32in_u16out_asm,@function
.version 00.50.05

.data .rodata.Gauss5x1_u32in_u16out 
.align 16

.code .text.Gauss5x1_u32in_u16out //text
.align 16

mvcvGauss5x1_u32in_u16out_asm:
//            (u32** inLine, u16** out, u32 width)//
//                 i18         i17        i16      
lsu0.ldo.32 i0 i18 0  || LSU1.ldil i5, mvcvGauss5x1_u32in_u16out_asm_loop
lsu0.ldo.32 i1 i18 4  || LSU1.ldih i5, mvcvGauss5x1_u32in_u16out_asm_loop
lsu0.ldo.32 i2 i18 8  || LSU1.ldil i7, 0x10   
lsu0.ldo.32 i3 i18 12 || LSU1.ldil i6, 0x6    
lsu0.ldo.32 i4 i18 16 || LSU1.ldil i8, 0x80   
lsu0.ld.32 i17 i17    || LSU1.ldil i9, 0xffff 
LSU1.ldih i7, 0x0
LSU1.ldih i6, 0x0 || IAU.SHR.u32 i16 i16 3
LSU1.ldih i8, 0x0 || cmu.cpivr.x32 v0 i6



IAU.SUB i19 i19 16  || LSU1.ldih i9, 0x0 || cmu.cpivr.x32 v12 i8
LSU0.STo.64.l v24  i19 0  || LSU1.STo.64.h v24  i19 8 || IAU.SUB i19 i19 16 || cmu.cpivr.x32 v15 i9
LSU0.STo.64.l v25  i19 0  || LSU1.STo.64.h v25  i19 8 || IAU.SUB i19 i19 16
LSU0.STo.64.l v26  i19 0  || LSU1.STo.64.h v26  i19 8 || IAU.SUB i19 i19 16
LSU0.STo.64.l v27  i19 0  || LSU1.STo.64.h v27  i19 8 || IAU.SUB i19 i19 16 
LSU0.STo.64.l v28  i19 0  || LSU1.STo.64.h v28  i19 8 || IAU.SUB i19 i19 16
LSU0.STo.64.l v29  i19 0  || LSU1.STo.64.h v29  i19 8 

lsu0.ldo.64.l v1 i0 0 || lsu1.ldo.64.h v1 i0 8 || iau.add i0 i0 i7 
lsu0.ldo.64.l v2 i1 0 || lsu1.ldo.64.h v2 i1 8 || iau.add i1 i1 i7 
lsu0.ldo.64.l v3 i2 0 || lsu1.ldo.64.h v3 i2 8 || iau.add i2 i2 i7 
lsu0.ldo.64.l v4 i3 0 || lsu1.ldo.64.h v4 i3 8 || iau.add i3 i3 i7 
lsu0.ldo.64.l v5 i4 0 || lsu1.ldo.64.h v5 i4 8 || iau.add i4 i4 i7 
lsu0.ldo.64.l v17 i0 0 || lsu1.ldo.64.h v17 i0 8 || iau.add i0 i0 i7 
lsu0.ldo.64.l v18 i1 0 || lsu1.ldo.64.h v18 i1 8 || iau.add i1 i1 i7 
lsu0.ldo.64.l v19 i2 0 || lsu1.ldo.64.h v19 i2 8 || iau.add i2 i2 i7 
lsu0.ldo.64.l v20 i3 0 || lsu1.ldo.64.h v20 i3 8 || iau.add i3 i3 i7 
lsu0.ldo.64.l v21 i4 0 || lsu1.ldo.64.h v21 i4 8 || iau.add i4 i4 i7 
nop 5
vau.add.i32 v6 v1 v5
vau.add.i32 v22 v17 v21
vau.add.i32 v7 v2 v4      || lsu0.ldo.64.l v1 i0 0 || lsu1.ldo.64.h v1 i0 8 || iau.add i0 i0 i7 
vau.add.i32 v23 v18 v20   || lsu0.ldo.64.l v2 i1 0 || lsu1.ldo.64.h v2 i1 8 || iau.add i1 i1 i7 
vau.mul.i32 v8 v3 v0      || lsu0.ldo.64.l v3 i2 0 || lsu1.ldo.64.h v3 i2 8 || iau.add i2 i2 i7 
vau.mul.i32 v24 v19 v0    || lsu0.ldo.64.l v4 i3 0 || lsu1.ldo.64.h v4 i3 8 || iau.add i3 i3 i7 
                             lsu0.ldo.64.l v5 i4 0 || lsu1.ldo.64.h v5 i4 8 || iau.add i4 i4 i7 
vau.shl.x32 v9 v7 2       || lsu0.ldo.64.l v17 i0 0 || lsu1.ldo.64.h v17 i0 8 || iau.add i0 i0 i7 
vau.shl.x32 v25 v23 2     || lsu0.ldo.64.l v18 i1 0 || lsu1.ldo.64.h v18 i1 8 || iau.add i1 i1 i7 
vau.add.i32 v10 v6 v8     || lsu0.ldo.64.l v19 i2 0 || lsu1.ldo.64.h v19 i2 8 || iau.add i2 i2 i7 
vau.add.i32 v26 v22 v24   || lsu0.ldo.64.l v20 i3 0 || lsu1.ldo.64.h v20 i3 8 || iau.add i3 i3 i7 
vau.add.i32 v11 v9 v10    || lsu0.ldo.64.l v21 i4 0 || lsu1.ldo.64.h v21 i4 8 || iau.add i4 i4 i7 
vau.add.i32 v27 v25 v26   
vau.add.i32 v13 v11 v12
vau.add.i32 v28 v27 v12
vau.shr.u32 v14 v13 8
vau.shr.u32 v29 v28 8
nop



vau.add.i32 v6 v1 v5      || bru.rpl i5 i16
vau.add.i32 v22 v17 v21   || cmu.vdilv.x16 v15 v14 v14 v29 
vau.add.i32 v7 v2 v4      || lsu0.ldo.64.l v1 i0 0 || lsu1.ldo.64.h v1 i0 8 || iau.add i0 i0 i7 
vau.add.i32 v23 v18 v20   || lsu0.ldo.64.l v2 i1 0 || lsu1.ldo.64.h v2 i1 8 || iau.add i1 i1 i7 
vau.mul.i32 v8 v3 v0      || lsu0.ldo.64.l v3 i2 0 || lsu1.ldo.64.h v3 i2 8 || iau.add i2 i2 i7 
vau.mul.i32 v24 v19 v0    || lsu0.ldo.64.l v4 i3 0 || lsu1.ldo.64.h v4 i3 8 || iau.add i3 i3 i7 
                             lsu0.ldo.64.l v5 i4 0 || lsu1.ldo.64.h v5 i4 8 || iau.add i4 i4 i7 
vau.shl.x32 v9 v7 2       || lsu0.ldo.64.l v17 i0 0 || lsu1.ldo.64.h v17 i0 8 || iau.add i0 i0 i7 
vau.shl.x32 v25 v23 2     || lsu0.ldo.64.l v18 i1 0 || lsu1.ldo.64.h v18 i1 8 || iau.add i1 i1 i7 
vau.add.i32 v10 v6 v8     || lsu0.ldo.64.l v19 i2 0 || lsu1.ldo.64.h v19 i2 8 || iau.add i2 i2 i7 
mvcvGauss5x1_u32in_u16out_asm_loop:
.nowarn 10
vau.add.i32 v26 v22 v24   || lsu0.ldo.64.l v20 i3 0 || lsu1.ldo.64.h v20 i3 8 || iau.add i3 i3 i7 
.nowarnend
vau.add.i32 v11 v9 v10    || lsu0.ldo.64.l v21 i4 0 || lsu1.ldo.64.h v21 i4 8 || iau.add i4 i4 i7 
vau.add.i32 v27 v25 v26   || lsu0.sto.64.l v14 i17 0 || lsu1.sto.64.h v14 i17 8 || iau.add i17 i17 i7 
vau.add.i32 v13 v11 v12
vau.add.i32 v28 v27 v12
vau.shr.u32 v14 v13 8
vau.shr.u32 v29 v28 8




LSU0.LDo.64.h  v29  i19 8 || LSU1.LDo.64.l  v29  i19 0 ||  IAU.ADD i19 i19 16
LSU0.LDo.64.h  v28  i19 8 || LSU1.LDo.64.l  v28  i19 0 ||  IAU.ADD i19 i19 16
LSU0.LDo.64.h  v27  i19 8 || LSU1.LDo.64.l  v27  i19 0 ||  IAU.ADD i19 i19 16
LSU0.LDo.64.h  v26  i19 8 || LSU1.LDo.64.l  v26  i19 0 ||  IAU.ADD i19 i19 16
LSU0.LDo.64.h  v25  i19 8 || LSU1.LDo.64.l  v25  i19 0 ||  IAU.ADD i19 i19 16
LSU0.LDo.64.h  v24  i19 8 || LSU1.LDo.64.l  v24  i19 0 ||  IAU.ADD i19 i19 16 || bru.jmp i30
nop 6

        
.size mvcvGauss5x1_u32in_u16out_asm,.-mvcvGauss5x1_u32in_u16out_asm
.end
