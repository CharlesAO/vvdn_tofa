/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.version 00.51.04

.data .data.disp2depth
.align 4

.code .text.disp2depth

//                      i18      i17       i16           i15                i14
//mvcvdisp2depth(u8* in, half* out, half* lut_table, u32 flip_disp2depth, u32 width)
.nowarn
.lalign
mvcvDisp2depth_asm:
vau.xor v1 v1 v1 //0
cmu.cpivr.x32 v2 i16 //lut address
lsu0.ldil i2 mvcvDisp2depthasm_loop || lsu1.ldih i2 mvcvDisp2depthasm_loop
iau.shr.u32 i13 i14 4 

cmu.cmz.i32 i15
peu.pc1c neq || bru.bra .LmvcvDisp2depthasm_second
nop 6

bru.rpl i2 i13
lsu0.ldo.64.l v0 i18 0 || lsu1.ldo.64.h v0 i18 8 || iau.add i18 i18 16
nop 6
cmu.vilv.x8 v4 v3  v1 v0
cmu.vilv.x16 v6 v5  v1 v3
cmu.vilv.x16 v8 v7  v1 v4
//v8 v7 v6 v5 order of the address

vau.shl.x32 v8 v8 1
vau.shl.x32 v7 v7 1
vau.shl.x32 v6 v6 1
vau.shl.x32 v5 v5 1

//lut compute
vau.add.i32  v9 v8 v2
vau.add.i32 v10 v7 v2
vau.add.i32 v11 v6 v2
vau.add.i32 v12 v5 v2


lsu1.cp  i0  v9.0
lsu1.cp  i0  v9.1 || lsu0.ld.16 i1 i0
lsu1.cp  i0  v9.2 || lsu0.ld.16 i1 i0
lsu1.cp  i0  v9.3 || lsu0.ld.16 i1 i0
lsu1.cp  i0 v10.0 || lsu0.ld.16 i1 i0
lsu1.cp  i0 v10.1 || lsu0.ld.16 i1 i0
lsu1.cp  i0 v10.2 || lsu0.ld.16 i1 i0
lsu1.cp  i0 v10.3 || lsu0.ld.16 i1 i0
lsu1.cp  i0 v11.0 || lsu0.ld.16 i1 i0 || cmu.cpiv.x16 v13.0 i1.l
lsu1.cp  i0 v11.1 || lsu0.ld.16 i1 i0 || cmu.cpiv.x16 v13.1 i1.l
lsu1.cp  i0 v11.2 || lsu0.ld.16 i1 i0 || cmu.cpiv.x16 v13.2 i1.l
lsu1.cp  i0 v11.3 || lsu0.ld.16 i1 i0 || cmu.cpiv.x16 v13.3 i1.l
lsu1.cp  i0 v12.0 || lsu0.ld.16 i1 i0 || cmu.cpiv.x16 v13.4 i1.l
lsu1.cp  i0 v12.1 || lsu0.ld.16 i1 i0 || cmu.cpiv.x16 v13.5 i1.l
lsu1.cp  i0 v12.2 || lsu0.ld.16 i1 i0 || cmu.cpiv.x16 v13.6 i1.l
lsu1.cp  i0 v12.3 || lsu0.ld.16 i1 i0 || cmu.cpiv.x16 v13.7 i1.l
                     lsu0.ld.16 i1 i0 || cmu.cpiv.x16 v14.0 i1.l
                                         cmu.cpiv.x16 v14.1 i1.l
                                         cmu.cpiv.x16 v14.2 i1.l
                                         cmu.cpiv.x16 v14.3 i1.l
                                         cmu.cpiv.x16 v14.4 i1.l
                                         cmu.cpiv.x16 v14.5 i1.l
                                         cmu.cpiv.x16 v14.6 i1.l
                                         cmu.cpiv.x16 v14.7 i1.l
lsu0.sto.64.l v13 i17  0 || lsu1.sto.64.h v13 i17 8 || iau.add i17 i17 16
lsu0.sto.64.l v14 i17  0 || lsu1.sto.64.h v14 i17 8 || iau.add i17 i17 16
mvcvDisp2depthasm_loop:
nop 7

bru.jmp i30
nop 6




.LmvcvDisp2depthasm_second:

lsu0.ldil i2 mvcvDisp2depthasm_loop2 || lsu1.ldih i2 mvcvDisp2depthasm_loop2
iau.sub i17 i17 30

bru.rpl i2 i13
lsu0.ldo.64.l v0 i18 0 || lsu1.ldo.64.h v0 i18 8 || iau.add i18 i18 16
nop 6
cmu.vilv.x8 v4 v3  v1 v0
cmu.vilv.x16 v6 v5  v1 v3
cmu.vilv.x16 v8 v7  v1 v4
//v8 v7 v6 v5 order of the address

vau.shl.x32 v8 v8 1
vau.shl.x32 v7 v7 1
vau.shl.x32 v6 v6 1
vau.shl.x32 v5 v5 1

//lut compute
vau.add.i32  v9 v8 v2
vau.add.i32 v10 v7 v2
vau.add.i32 v11 v6 v2
vau.add.i32 v12 v5 v2


lsu1.cp  i0  v9.0
lsu1.cp  i0  v9.1 || lsu0.ld.16 i1 i0
lsu1.cp  i0  v9.2 || lsu0.ld.16 i1 i0
lsu1.cp  i0  v9.3 || lsu0.ld.16 i1 i0
lsu1.cp  i0 v10.0 || lsu0.ld.16 i1 i0
lsu1.cp  i0 v10.1 || lsu0.ld.16 i1 i0
lsu1.cp  i0 v10.2 || lsu0.ld.16 i1 i0
lsu1.cp  i0 v10.3 || lsu0.ld.16 i1 i0
lsu1.cp  i0 v11.0 || lsu0.ld.16 i1 i0 || cmu.cpiv.x16 v13.0 i1.l
lsu1.cp  i0 v11.1 || lsu0.ld.16 i1 i0 || cmu.cpiv.x16 v13.1 i1.l
lsu1.cp  i0 v11.2 || lsu0.ld.16 i1 i0 || cmu.cpiv.x16 v13.2 i1.l
lsu1.cp  i0 v11.3 || lsu0.ld.16 i1 i0 || cmu.cpiv.x16 v13.3 i1.l
lsu1.cp  i0 v12.0 || lsu0.ld.16 i1 i0 || cmu.cpiv.x16 v13.4 i1.l
lsu1.cp  i0 v12.1 || lsu0.ld.16 i1 i0 || cmu.cpiv.x16 v13.5 i1.l
lsu1.cp  i0 v12.2 || lsu0.ld.16 i1 i0 || cmu.cpiv.x16 v13.6 i1.l
lsu1.cp  i0 v12.3 || lsu0.ld.16 i1 i0 || cmu.cpiv.x16 v13.7 i1.l
                     lsu0.ld.16 i1 i0 || cmu.cpiv.x16 v14.0 i1.l
                                         cmu.cpiv.x16 v14.1 i1.l
                                         cmu.cpiv.x16 v14.2 i1.l
                                         cmu.cpiv.x16 v14.3 i1.l
                                         cmu.cpiv.x16 v14.4 i1.l
                                         cmu.cpiv.x16 v14.5 i1.l
                                         cmu.cpiv.x16 v14.6 i1.l
                                         cmu.cpiv.x16 v14.7 i1.l
                                         cmu.alignvec v15 v13 v13 0 || lsu1.swzc8 [01234567]
                                         cmu.alignvec v16 v14 v14 0 || lsu1.swzc8 [01234567]
lsu0.sto.64.l v16 i17   0 || lsu1.sto.64.h v16 i17  8 
lsu0.sto.64.l v15 i17  16 || lsu1.sto.64.h v15 i17 24 || iau.sub i17 i17 32
mvcvDisp2depthasm_loop2:
nop 7

bru.jmp i30
nop 6
.nowarnend
.end
