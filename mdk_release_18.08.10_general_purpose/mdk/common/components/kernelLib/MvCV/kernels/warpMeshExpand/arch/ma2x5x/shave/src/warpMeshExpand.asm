///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvWarpMeshExpand_asm,@function
.version 00.51.04

.data .data.mvcvWarpMeshExpand
.align 4
 mvcvWarpMeshExpand_indx:
		.float16    16, 15, 14, 13, 12, 11, 10, 9, 8,  7,  6,  5,  4,  3,  2,  1
		.float16     0,  1,  2,  3,  4,  5,  6, 7, 8,  9, 10, 11, 12, 13, 14, 15, 0.0625

.code .text.mvcvWarpMeshExpand
//void mvcvmvcvWarpMeshExpand(half *xcoord, half *ycoord, half *mx, half *my)
//                                  i18      i17            i16        i15

mvcvWarpMeshExpand_asm:

LSU0.ldil i0, mvcvWarpMeshExpand_indx || LSU1.ldih i0, mvcvWarpMeshExpand_indx
LSU0.LDo.64.l v2  i18 0  || LSU1.LDo.64.h v2 i18 8 //xcoord
LSU0.LDo.64.l v4  i0 0   || LSU1.LDo.64.h v4  i0 8                           //16....9 indx
LSU0.LDo.64.l v5  i0 16  || LSU1.LDo.64.h v5  i0 24                          //8....1 indx
LSU0.LDo.64.l v6  i0 32  || LSU1.LDo.64.h v6  i0 40                          //0....7 indx
LSU0.LDo.64.l v7  i0 48  || LSU1.LDo.64.h v7  i0 56 || iau.add i0 i0 64      //8....15 indx
LSU0.LDo.64.l v3  i17 0  || LSU1.LDo.64.h v3 i17 8 //ycoord

LSU0.ldil i1, 0x3c00 || LSU1.ldih i1, 0x3c00
lsu0.ldil i7 mvcvWarpMeshExpand_loop  || lsu1.ldih i7 mvcvWarpMeshExpand_loop

vau.mul.f16  v8 v2 v4 || lsu1.swzv8 [00000000] || lsu0.ld.16r v23 i0 || IAU.SUB i19 i19 16  //1/16
vau.mul.f16  v9 v2 v5 || lsu1.swzv8 [00000000] || LSU0.STo.64.l v24 i19 0
vau.mul.f16 v10 v2 v6 || lsu1.swzv8 [11111111] || LSU0.STO.64.h v24 i19 8 || IAU.SUB i19 i19 16
vau.mul.f16 v11 v2 v7 || lsu1.swzv8 [11111111] || LSU0.STo.64.l v25 i19 0
vau.mul.f16 v16 v2 v4 || lsu1.swzv8 [22222222] || LSU0.STO.64.h v25 i19 8 || IAU.SUB i19 i19 16
vau.mul.f16 v17 v2 v5 || lsu1.swzv8 [22222222] || LSU0.STo.64.l v26 i19 0
vau.mul.f16 v18 v2 v6 || lsu1.swzv8 [33333333] || LSU0.STO.64.h v26 i19 8
vau.mul.f16 v19 v2 v7 || lsu1.swzv8 [33333333]


vau.add.f16 v12 v8 v10
vau.add.f16 v13 v9 v11
vau.add.f16 v20 v16 v18
vau.add.f16 v21 v17 v19
vau.mul.f16 v14 v12 v23
//xcoord[0] * (mesh_gran_x - j) + xcoord[1] * j
vau.mul.f16 v15 v13 v23
//xcoord[2] * (mesh_gran_x - j) + xcoord[3] * j
vau.mul.f16 v22 v20 v23
vau.mul.f16 v24 v21 v23
vau.mul.f16  v8 v3 v4 || lsu1.swzv8 [00000000]
vau.mul.f16  v9 v3 v5 || lsu1.swzv8 [00000000]
vau.mul.f16 v10 v3 v6 || lsu1.swzv8 [11111111]
vau.mul.f16 v11 v3 v7 || lsu1.swzv8 [11111111]
vau.mul.f16 v16 v3 v4 || lsu1.swzv8 [22222222]
vau.mul.f16 v17 v3 v5 || lsu1.swzv8 [22222222]
vau.mul.f16 v18 v3 v6 || lsu1.swzv8 [33333333]
vau.mul.f16 v19 v3 v7 || lsu1.swzv8 [33333333]
vau.add.f16 v12 v8 v10 || lsu0.ldil i0 0x10 || lsu1.ldih i0 0x10
vau.add.f16 v13 v9 v11 || lsu0.ldil i6 0x0  || lsu1.ldih i6 0x0
vau.add.f16 v20 v16 v18 || CMU.CPII.i16.f16 i0 i0
vau.add.f16 v21 v17 v19 || CMU.CPII.i16.f16 i6 i6
vau.mul.f16 v25 v12 v23    //ycoord[0] * (mesh_gran_x - j) + ycoord[1] * j
vau.mul.f16 v26 v13 v23
vau.mul.f16 v7 v20 v23  || cmu.cpivr.x32 v5 i0//(ycoord[2] * (mesh_gran_x - j) + ycoord[3] * j
vau.mul.f16 v6 v21 v23  || cmu.cpivr.x32 v4 i6


vau.mul.f16 v8 v5 v14
vau.mul.f16 v9 v5 v15
vau.mul.f16 v10 v4 v22
vau.mul.f16 v11 v4 v24
sau.sub.f16 i0 i0 i1
vau.add.f16 v12 v8 v10
vau.add.f16 v13 v9 v11





sau.add.f16 i6 i6 i1  ||  bru.rps i7 16
vau.mul.f16 v0 v12 v23
vau.mul.f16 v1 v13 v23
vau.mul.f16 v16 v5 v25
vau.mul.f16 v17 v5 v26 || lsu0.sto.64.l v0 i16  0
vau.mul.f16 v20 v4 v7  || cmu.cpivr.x32 v5 i0 || lsu0.sto.64.h v0 i16 8
vau.mul.f16 v21 v4 v6  || cmu.cpivr.x32 v4 i6 || lsu0.sto.64.l v1 i16 16
vau.mul.f16 v8 v5 v14 || lsu0.sto.64.h v1 i16 24 || iau.add i16 i16 32
vau.add.f16 v18 v16 v20
vau.add.f16 v19 v17 v21
vau.mul.f16 v9 v5 v15
mvcvWarpMeshExpand_loop:
vau.mul.f16 v2 v18 v23
vau.mul.f16 v3 v19 v23
vau.mul.f16 v10 v4 v22
vau.mul.f16 v11 v4 v24 || lsu0.sto.64.l v2 i15  0
sau.sub.f16 i0 i0 i1 || lsu0.sto.64.h v2 i15 8
vau.add.f16 v12 v8 v10 || lsu0.sto.64.l v3 i15 16
vau.add.f16 v13 v9 v11 || lsu0.sto.64.h v3 i15 24 || iau.add i15 i15 32
//*******************************************






LSU0.LDo.64.h v26 i19 8 || LSU1.LDO.64.l v26 i19 0 || IAU.ADD i19 i19 16
LSU0.LDo.64.h v25 i19 8 || LSU1.LDO.64.l v25 i19 0 || IAU.ADD i19 i19 16
LSU0.LDo.64.h v24 i19 8 || LSU1.LDO.64.l v24 i19 0 || IAU.ADD i19 i19 16 || BRU.jmp i30
nop 6

.size mvcvWarpMeshExpand_asm,.-mvcvWarpMeshExpand_asm
.end
