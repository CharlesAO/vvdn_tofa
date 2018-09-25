///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvWarpMeshSampleFp16bit_asm,@function
.version 00.51.04

.data .data.warpMeshSampleFp16bit
.align 4

.code .text.warpMeshSampleFp16bit
//void mvcvwarpMeshSampleFp16bit(half **tile, half *out_tile, half *mx, half *my, u32 tile_width, u32 tile_height)
//                                  i18           i17           i16       i15         i14            i13

mvcvWarpMeshSampleFp16bit_asm:
IAU.SUB i19 i19 16 || LSU0.ldil i0, 0x3c00 || LSU1.ldih i0, 0x3c00
LSU0.STo.64.l v24  i19 0  || LSU1.STo.64.h v24  i19 8 || IAU.SUB i19 i19 16
LSU0.STo.64.l v25  i19 0  || LSU1.STo.64.h v25  i19 8 || IAU.SUB i19 i19 16
LSU0.STo.64.l v26  i19 0  || LSU1.STo.64.h v26  i19 8 || IAU.SUB i19 i19 16
LSU0.STo.64.l v27  i19 0  || LSU1.STo.64.h v27  i19 8 || IAU.SUB i19 i19 16
LSU0.STo.64.l v28  i19 0  || LSU1.STo.64.h v28  i19 8 || IAU.SUB i19 i19 16
LSU0.STo.64.l v29  i19 0  || LSU1.STo.64.h v29  i19 8 || IAU.SUB i19 i19 16
LSU0.STo.64.l v30  i19 0  || LSU1.STo.64.h v30  i19 8 || IAU.SUB i19 i19 16
LSU0.STo.64.l v31  i19 0  || LSU1.STo.64.h v31  i19 8 || IAU.SUB i19 i19 8
LSU0.ST.64  i20  i19 || IAU.SUB i19 i19 8
LSU1.ST.64  i22  i19 || IAU.SUB i19 i19 8
LSU0.ST.64  i24  i19 || IAU.SUB i19 i19 8
LSU1.ST.64  i26  i19 || IAU.SUB i19 i19 8



LSU0.LDo.64.l v0  i16 0   || LSU1.LDo.64.h v0 i16 8 || iau.add i16 i16 16 //xcoord
LSU0.LDo.64.l v2  i15 0   || LSU1.LDo.64.h v2 i15 8 || iau.add i15 i15 16 //ycoord
LSU0.ST.64  i28  i19      || IAU.SUB i19 i19 8


cmu.cpivr.x32 v26 i0  || lsu0.ldil i1 0x4  || lsu1.ldih i1 0x4
cmu.cpivr.x32  v5 i18 || lsu0.ldil i2 0x4  || lsu1.ldih i2 0x0
cmu.cpivr.x32 v24 i1  || iau.xor i1 i1 i1  || lsu0.ldil i0 mvcvWarpMeshSampleFp16bit_loop  || lsu1.ldih i0 mvcvWarpMeshSampleFp16bit_loop
cmu.cpivr.x32 v25 i2  || lsu0.ldil i1 32   || LSU1.ST.64  i30  i19

cmu.cpvv.f16.i16s  v10 v0 //x
cmu.cpvv.f16.i16s  v11 v2 //y
cmu.cpvv.i16.f16 v12 v10
cmu.cpvv.i16.f16 v13 v11

vau.mul.i16 v1 v24 v11
vau.sub.f16 v16 v0 v12  //frac x
vau.sub.f16 v29 v2 v13  //frac y
cmu.cpvv.i16.i32 v22 v1
vau.sub.f16 v21 v26  v16 || cmu.SHRIV.x32  v1 v1 i10 //1-frac x
vau.sub.f16  v7 v26  v29 || cmu.SHRIV.x32  v1 v1 i10 //1-frac y

cmu.cpvv.i16.i32 v3 v1

cmu.cpvv.i16.i32 v17 v10
cmu.SHRIV.x32  v10 v10 i10 || vau.add.i32 v12 v5 v22
cmu.SHRIV.x32  v10 v10 i10 || vau.add.i32 v6 v5 v3
cmu.cpvv.i16.i32 v9 v10    || lsu0.cp i2 v12.0  || lsu1.cp i3 v12.1 || vau.shl.x32 v17,v17,1



LSU0.LD.64  i11  i2  || LSU1.LDo.64.l v0  i16 0                      ||   cmu.cpvid  i4 v12.2 || vau.shl.x32 v9,v9,1
LSU0.LD.64  i13  i3  || LSU1.LDo.64.l v2  i15 0                      ||     cmu.cpvid  i6 v6.0
LSU0.LD.64  i20  i4  || LSU1.LDo.64.h v0 i16 8 || iau.add i16 i16 16 ||     cmu.cpvid  i8 v6.2
LSU0.LD.64  i22  i5  || LSU1.LDo.64.h v2 i15 8 || iau.add i15 i15 16 || cmu.cpvid i2 v17.0
LSU0.LD.64  i24  i6  || cmu.cpvid i4 v17.2
LSU0.LD.64  i26  i7  || cmu.cpvid i6  v9.0
LSU0.LD.64  i28  i8  || cmu.cpvv v4 v21
LSU0.LD.64  i30  i9  || cmu.cpvid i8  v9.2     || iau.add i11 i11  i2
LSU0.LD.64.l   v14 i11     || iau.add i13 i13  i3  || cmu.cpvv v8 v7
LSU0.LD.64.l   v14 i13        || iau.add i20 i20  i4  || cmu.cpvv.f16.i16s  v10 v0 //x
LSU0.LD.64.l   v14 i20     || iau.add i22 i22  i5  || cmu.cpvv.f16.i16s  v11 v2 //y
LSU0.LD.64.l   v14 i22     || iau.add i24 i24  i6  || cmu.cpvv.i16.f16 v12 v10
LSU0.LD.64.l   v14 i24     || iau.add i26 i26  i7  || cmu.cpvv.i16.f16 v13 v11   || vau.mul.i16 v1 v24 v11
LSU0.LD.64.l   v14 i26     || iau.add i28 i28  i8                                || vau.sub.f16 v16 v0 v12  //frac x
LSU0.LD.64.l   v14 i28     || iau.add i30 i30  i9                                || vau.sub.f16 v29 v2 v13  || cmu.cpvv v15 v29 //ycoord          //frac y
LSU0.LD.64.l   v14 i30     || lsu1.cp   i11 v14.0                          || iau.add i12 i12 i2  || cmu.cpvv v18 v16 //xcoord
LSU0.LD.64.l   v14 i12     || lsu1.cp   i12 v14.0                          || iau.add i14 i14 i3  || vau.sub.f16 v21 v26  v16 || cmu.cpvv.i16.i32 v22 v1 //1-frac x
LSU0.LD.64.l   v14 i14     || lsu1.cp   i13 v14.0                          || iau.add i21 i21 i4  || vau.sub.f16  v7 v26  v29 || cmu.SHRIV.x32  v1 v1 i10//1-frac y
LSU0.LD.64.l   v14 i21     || lsu1.cp   i14 v14.0   || iau.add i23 i23 i5  || cmu.SHRIV.x32  v1 v1 i10
LSU0.LD.64.l   v14 i23     || lsu1.cp   i20 v14.0   || iau.add i25 i25 i6  || cmu.cpvv.i16.i32 v3 v1
LSU0.LD.64.l   v14 i25     || lsu1.cp   i21 v14.0   || iau.add i27 i27 i7  || cmu.cpvv.i16.i32 v17 v10   || vau.add.i32 v12 v5 v22
LSU0.LD.64.l   v14 i27     || lsu1.cp   i22 v14.0   || iau.add i29 i29 i8  || cmu.SHRIV.x32  v10 v10 i10 || vau.add.i32 v6 v5 v3
LSU0.LD.64.l   v14 i29     || lsu1.cp   i23 v14.0   || iau.add i31 i31 i9  || cmu.SHRIV.x32  v10 v10 i10
LSU0.LD.64.l   v14 i31     || lsu1.cp   i24 v14.0                             || cmu.cpvv.i16.i32 v9 v10
                                    lsu1.cp   i25 v14.0  || cmu.cpivd v30.0 i11   || vau.shl.x32 v17,v17,1
                                    lsu1.cp   i26 v14.0  || cmu.cpivd v30.2 i13   || vau.shl.x32 v9,v9,1
                                    lsu1.cp   i27 v14.0  || cmu.cpivd v27.0 i20   || lsu0.cp  i2 v12.0
                                    lsu1.cp   i28 v14.0  || cmu.cpivd v27.2 i22   || lsu0.cp  i3 v12.1
                                    lsu1.cp   i29 v14.0  || cmu.cpivd v28.0 i24
                                    lsu1.cp   i30 v14.0  || cmu.cpivd v28.2 i26
                                    lsu1.cp   i31 v14.0  || cmu.cpivd v31.0 i28
cmu.cpivd  v31.2 i30

CMU.VDILV.x16 v19 v23 v30 v27 || bru.rpl i0 i1 
LSU0.LD.64  i11  i2  || LSU1.LDo.64.l v0  i16 0 || CMU.VDILV.x16 v20 v25 v28 v31 || vau.mul.f16 v23 v23 v4
LSU0.LD.64  i13  i3  || LSU1.LDo.64.l v2  i15 0 || cmu.cpvid  i4 v12.2           || vau.mul.f16 v19 v19 v18
LSU0.LD.64  i20  i4  || LSU1.LDo.64.h v0 i16 8  || iau.add i16 i16 16 || cmu.cpvid  i6 v6.0 || vau.mul.f16 v25 v25 v4
LSU0.LD.64  i22  i5  || LSU1.LDo.64.h v2 i15 8  || iau.add i15 i15 16 || cmu.cpvid  i8 v6.2 || vau.mul.f16 v20 v20 v18
LSU0.LD.64  i24  i6  || LSU1.cp i4 v17.2  || cmu.cpvid i2 v17.0
LSU0.LD.64  i26  i7  || LSU1.cp i5 v17.3  || cmu.cpvv v4 v21          || vau.add.f16 v23 v19 v23
LSU0.LD.64  i28  i8  || cmu.cpvid i6  v9.0
LSU0.LD.64  i30  i9  || cmu.cpvid i8  v9.2 || iau.add i11 i11  i2     || vau.add.f16 v25 v20 v25
LSU0.LD.32   i11  i11      || iau.add i13 i13  i3  || cmu.cpvv v8 v7              || vau.mul.f16 v23 v23 v8
LSU0.LD.32   i12  i13        || iau.add i20 i20  i4  || cmu.cpvv.f16.i16s  v10 v0
LSU0.LD.32   i13  i20      || iau.add i22 i22  i5  || cmu.cpvv.f16.i16s  v11 v2   || vau.mul.f16 v25 v25 v15//x//y
LSU0.LD.32   i14  i22      || iau.add i24 i24  i6  || cmu.cpvv.i16.f16 v12 v10
LSU0.LD.32   i20 i24       || iau.add i26 i26  i7  || cmu.cpvv.i16.f16 v13 v11    || vau.mul.i16 v1 v24 v11
LSU0.LD.32   i21 i26       || iau.add i28 i28  i8                                 || vau.sub.f16 v16 v0 v12  //frac x
LSU0.LD.32   i22 i28       || iau.add i12 i12 i2                                  || vau.sub.f16 v29 v2 v13   || cmu.cpvv v15 v29 //ycoord          //frac y
LSU0.LD.32   i24  i12      || iau.add i30 i30  i9 || cmu.cpvv v18 v16  || lsu1.cp v30.0 i11           || vau.add.f16 v25 v25 v23//xcoord
LSU0.LD.32   i23  i30      || iau.add i14 i14 i3  || vau.sub.f16 v21 v26  v16  || lsu1.cp v30.1 i12   || cmu.cpvv.i16.i32 v22 v1 //1-frac x
LSU0.LD.32   i25  i14      || iau.add i21 i21 i4  || vau.sub.f16  v7 v26  v29  || lsu1.cp v30.2 i13   || cmu.SHRIV.x32  v1 v1 i10//1-frac y
LSU0.LD.32   i26  i21      || iau.add i23 i23 i5  || cmu.SHRIV.x32  v1 v1 i10  || lsu1.cp v30.3 i14
LSU0.LD.32   i27  i23      || iau.add i25 i25 i6  || cmu.cpvv.i16.i32 v3 v1    || lsu1.cp v27.0 i20
LSU0.LD.32   i28  i25      || iau.add i27 i27 i7  || cmu.cpvv.i16.i32 v17 v10   || vau.add.i32 v12 v5 v22 || lsu1.cp v27.1 i21
LSU0.LD.32   i29  i27      || iau.add i29 i29 i8  || cmu.SHRIV.x32  v10 v10 i10 || vau.add.i32 v6 v5 v3   || lsu1.cp v27.2 i22
LSU0.LD.32   i30  i29      || iau.add i31 i31 i9  || cmu.SHRIV.x32  v10 v10 i10 || vau.shl.x32 v17,v17,1  || lsu1.cp v28.0 i24
LSU0.LD.32   i31  i31                                || cmu.cpvv.i16.i32 v9 v10    || lsu1.cp v27.3 i23
lsu1.cp v28.1 i25       
lsu1.cp v28.2 i26     || vau.shl.x32 v9,v9,1    
mvcvWarpMeshSampleFp16bit_loop:
.nowarn 10
lsu1.cp v28.3 i27         || lsu0.sti.64.l v25,i17
.nowarnend
lsu1.cp v31.0 i28       || lsu0.sti.64.h v25,i17
lsu1.cp v31.1 i29     
lsu1.cp v31.2 i30 
lsu1.cp v31.3 i31        || lsu0.cp  i2 v12.0
lsu0.cp  i3 v12.1
nop                     
                                         
    LSU0.LD.64  i30  i19 || IAU.ADD i19 i19 8
    LSU0.LD.64  i28  i19 || IAU.ADD i19 i19 8
    LSU0.LD.64  i26  i19 || IAU.ADD i19 i19 8
    LSU0.LD.64  i24  i19 || IAU.ADD i19 i19 8
    LSU0.LD.64  i22  i19 || IAU.ADD i19 i19 8
    LSU0.LD.64  i20  i19 || IAU.ADD i19 i19 8

    LSU0.LDo.64.h  v31  i19 8 || LSU1.LDo.64.l  v31  i19 0 ||  IAU.ADD i19 i19 16 
    LSU0.LDo.64.h  v30  i19 8 || LSU1.LDo.64.l  v30  i19 0 ||  IAU.ADD i19 i19 16
    LSU0.LDo.64.h  v29  i19 8 || LSU1.LDo.64.l  v29  i19 0 ||  IAU.ADD i19 i19 16
    LSU0.LDo.64.h  v28  i19 8 || LSU1.LDo.64.l  v28  i19 0 ||  IAU.ADD i19 i19 16
    LSU0.LDo.64.h  v27  i19 8 || LSU1.LDo.64.l  v27  i19 0 ||  IAU.ADD i19 i19 16 
    LSU0.LDo.64.h  v26  i19 8 || LSU1.LDo.64.l  v26  i19 0 ||  IAU.ADD i19 i19 16
    LSU0.LDo.64.h  v25  i19 8 || LSU1.LDo.64.l  v25  i19 0 ||  IAU.ADD i19 i19 16
    LSU0.LDo.64.h  v24  i19 8 || LSU1.LDo.64.l  v24  i19 0 ||  IAU.ADD i19 i19 16 || BRU.jmp i30
nop 6

.size mvcvWarpMeshSampleFp16bit_asm,.-mvcvWarpMeshSampleFp16bit_asm
.end
