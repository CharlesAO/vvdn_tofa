///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvjpegGetBlockY420_asm,@function
.version 00.51.04

.data .data.jpegGetBlockY420
.align 4
block_coord:
          .int 0, 0, 8, 0, 0, 8, 8, 8

 .code .text.jpegGetBlockY420
//  void mvcvjpegGetBlockY420(u32 mode, u8 *imginfo_Y, u32 imginfo_width, u32 imgspec_x,u32 imgspec_y, short imgspecY[8][8])
//                                i18             i17              i16           i15              i14           i13           


mvcvjpegGetBlockY420_asm:
   
lsu0.ldil i0, block_coord   || lsu1.ldih i0, block_coord   || iau.shl i18,i18,3
lsu0.ldil i3, 0x80          || lsu1.ldih i3, 0x0           || iau.add i0,i0,i18
lsu0.ld.64  i1  i0
nop 6
iau.add i2,i2,i14           || sau.add.i32 i1,i1,i15
nop
iau.mul i2,i2,i16           || sau.add.i32 i17,i17,i1
nop
iau.add i17,i17,i2          || cmu.cpivr.x16 v8,i3

lsu0.ld.128.u8.u16 v0 i17   || iau.add i17,i17,i16
lsu0.ld.128.u8.u16 v1 i17   || iau.add i17,i17,i16
lsu0.ld.128.u8.u16 v2 i17   || iau.add i17,i17,i16
lsu0.ld.128.u8.u16 v3 i17   || iau.add i17,i17,i16
lsu0.ld.128.u8.u16 v4 i17   || iau.add i17,i17,i16
lsu0.ld.128.u8.u16 v5 i17   || iau.add i17,i17,i16
lsu0.ld.128.u8.u16 v6 i17   || iau.add i17,i17,i16
lsu0.ld.128.u8.u16 v7 i17   || vau.sub.i16 v0,v0,v8        || iau.add i8 i13 0x0

vau.sub.i16 v1,v1,v8
vau.sub.i16 v2,v2,v8        || lsu0.sto.64.l v0  i8 0x0    || lsu1.sto.64.h v0  i8  0x8 ||    iau.add i9 i8  0x10
vau.sub.i16 v3,v3,v8        || lsu0.sto.64.l v1  i9 0x0    || lsu1.sto.64.h v1  i9  0x8 ||    iau.add i10 i9 0x10
vau.sub.i16 v4,v4,v8        || lsu0.sto.64.l v2  i10 0x0   || lsu1.sto.64.h v2  i10 0x8 ||    iau.add i11 i10 0x10 || bru.jmp i30
vau.sub.i16 v5,v5,v8        || lsu0.sto.64.l v3  i11 0x0   || lsu1.sto.64.h v3  i11 0x8 ||    iau.add i12 i11 0x10
vau.sub.i16 v6,v6,v8        || lsu0.sto.64.l v4  i12 0x0   || lsu1.sto.64.h v4  i12 0x8 ||    iau.add i13 i12 0x10
.nowarn 12
vau.sub.i16 v7,v7,v8        || lsu0.sto.64.l v5  i13 0x0   || lsu1.sto.64.h v5  i13 0x8 ||    iau.add i14 i13 0x10
.nowarnend
lsu0.sto.64.l v6  i14 0x0   || lsu1.sto.64.h v6  i14 0x8   ||    iau.add i15 i14 0x10
lsu0.sto.64.l v7  i15 0x0   || lsu1.sto.64.h v7  i15 0x8
nop 



.size mvcvjpegGetBlockY420_asm,.-mvcvjpegGetBlockY420_asm
.end
