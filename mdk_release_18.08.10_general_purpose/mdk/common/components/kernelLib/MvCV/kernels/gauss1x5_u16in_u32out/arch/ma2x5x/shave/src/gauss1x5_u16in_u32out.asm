///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvGauss1x5_u16in_u32out_asm,@function
.version 00.50.05

.data .rodata.gauss1x5_u16in_u32out 
.align 16


.code .text.gauss1x5_u16in_u32out //text
.align 16

mvcvGauss1x5_u16in_u32out_asm:
//void gauss1x5_u16in_u32out(u16** inLine, u32** out, u32 width)//
//                                 i18      i17        i16      
LSU0.ldil i5, mvcvGauss1x5_u16in_u32out_asm_loop || LSU1.ldih i5, mvcvGauss1x5_u16in_u32out_asm_loop || iau.shr.u32 i16 i16 3
LSU0.ldil i1, 0x6 || LSU1.ldih i1, 0x0
LSU0.ldil i0, 0x20 || LSU1.ldih i0, 0x0 || cmu.cpivr.x32 v7 i1

lsu0.ld.32 i18 i18	
lsu0.ld.32 i17 i17	
IAU.SUB i19 i19 16  
LSU0.STo.64.l v24  i19 0  || LSU1.STo.64.h v24  i19 8 
nop 4

iau.sub i18 i18 8
LSU0.LDi.128.u16.u32 v0 i18 
LSU0.LDi.128.u16.u32 v1 i18 
LSU0.LDi.128.u16.u32 v2 i18 
LSU0.LDi.128.u16.u32 v14 i18 
nop 6

cmu.alignvec v3 v0 v1 8
cmu.alignvec v4 v0 v1 12
cmu.alignvec v5 v1 v2 4
cmu.alignvec v6 v1 v2 8

cmu.alignvec v15 v1 v2 8   
cmu.alignvec v16 v1 v2 12   || LSU0.LDi.128.u16.u32 v2 i18 
cmu.alignvec v17 v2 v14 4   || LSU0.LDi.128.u16.u32 v14 i18 
cmu.alignvec v18 v2 v14 8


vau.mul.i32 v19 v2 v7  || cmu.cpvv v0 v2
vau.mul.i32 v8 v1 v7   || cmu.cpvv v1 v14
nop                                                         
                                   
vau.add.i32 v10 v4 v5   || bru.rpl i5 i16
vau.add.i32 v20 v16 v17
vau.add.i32 v9 v3 v6       || cmu.alignvec v3 v0 v1 8
vau.add.i32 v21 v15 v18    || cmu.alignvec v4 v0 v1 12
vau.shl.x32 v11 v10 2      || cmu.alignvec v5 v1 v2 4
vau.shl.x32 v22 v20 2      || cmu.alignvec v6 v1 v2 8
mvcvGauss1x5_u16in_u32out_asm_loop:
vau.add.i32 v12 v8 v9      || cmu.alignvec v15 v1 v2 8   
vau.add.i32 v23 v19 v21    || cmu.alignvec v16 v1 v2 12   || LSU0.LDi.128.u16.u32 v2 i18 
vau.add.i32 v13 v12 v11    || cmu.alignvec v17 v2 v14 4   || LSU0.LDi.128.u16.u32 v14 i18 
vau.add.i32 v24 v23 v22    || cmu.alignvec v18 v2 v14 8
vau.mul.i32 v19 v2 v7  || cmu.cpvv v0 v2  || lsu0.sto.64.l v13 i17 0  || lsu1.sto.64.h v13 i17 8
vau.mul.i32 v8 v1 v7   || cmu.cpvv v1 v14 || lsu0.sto.64.l v24 i17 16 || lsu1.sto.64.h v24 i17 24 || iau.add i17 i17 i0
nop 


LSU0.LDo.64.h  v24  i19 8 || LSU1.LDo.64.l  v24  i19 0 ||  IAU.ADD i19 i19 16 || bru.jmp i30
nop 6
.size mvcvGauss1x5_u16in_u32out_asm,.-mvcvGauss1x5_u16in_u32out_asm
.end
