/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvComputeCombinedCost64_asm,@function
.version 00.51.04

.data .data.ComputeCombinedCost64
.align 4

.code .text.ComputeCombinedCost64

.lalign
//mvcvComputeCombinedCost64, u8* disparityCost, u8* adCost, u32 alpha, u32 beta, u32 width, u32 normFactor
//                                  i18             i17         i16        i15       i14        i13
mvcvComputeCombinedCost64_asm:

IAU.SUB i19 i19 16 || LSU0.ldil i1, 0x3c00 || LSU1.ldih i1, 0x3c00           || CMU.CPII.i32.f16  i13 i13
LSU0.STo.64.l v24  i19 0  || LSU1.STo.64.h v24  i19 8 || IAU.SUB i19 i19 16  || CMU.CPII.i32.f16  i16 i16
LSU0.STo.64.l v25  i19 0  || LSU1.STo.64.h v25  i19 8                        || CMU.CPII.i32.f16  i15 i15 || sau.div.f16 i1 i1 i13
nop 7
sau.mul.f16 i16 i16 i1
sau.mul.f16 i15 i15 i1
nop 
cmu.cpivr.x16 v16 i16
cmu.cpivr.x16 v17 i15
cmu.cpii i13 i18

LSU0.ldil i0, ComputeCombinedCost64_loop || LSU1.ldih i0, ComputeCombinedCost64_loop
LSU1.LDi.128.U8f.F16   v0    i13 || LSU0.LDi.128.U8f.F16   v1   i17
LSU1.LDi.128.U8f.F16   v2    i13 || LSU0.LDi.128.U8f.F16   v3   i17
LSU1.LDi.128.U8f.F16   v4    i13 || LSU0.LDi.128.U8f.F16   v5   i17
LSU1.LDi.128.U8f.F16   v6    i13 || LSU0.LDi.128.U8f.F16   v7   i17
LSU1.LDi.128.U8f.F16   v8    i13 || LSU0.LDi.128.U8f.F16   v9   i17
LSU1.LDi.128.U8f.F16   v10   i13 || LSU0.LDi.128.U8f.F16   v11   i17
LSU1.LDi.128.U8f.F16   v12   i13 || LSU0.LDi.128.U8f.F16   v13   i17
LSU1.LDi.128.U8f.F16   v14   i13 || LSU0.LDi.128.U8f.F16   v15   i17
vau.mul.f16 v0   v0    v16
vau.mul.f16 v1   v1    v17
vau.mul.f16 v2   v2    v16
vau.mul.f16 v3   v3    v17
vau.mul.f16 v4   v4    v16
vau.mul.f16 v5   v5    v17 
vau.mul.f16 v6   v6    v16
vau.mul.f16 v7   v7    v17
vau.mul.f16 v8   v8    v16  || bru.rpl i0 i14
vau.mul.f16 v9   v9    v17
vau.mul.f16 v10   v10    v16
vau.mul.f16 v11   v11    v17
vau.mul.f16 v12   v12    v16
vau.mul.f16 v13   v13    v17
vau.mul.f16 v14   v14    v16
vau.mul.f16 v15   v15    v17
vau.add.f16 v18   v0  v1    || LSU1.LDi.128.U8f.F16   v0    i13 || LSU0.LDi.128.U8f.F16   v1   i17
vau.add.f16 v19   v2  v3    || LSU1.LDi.128.U8f.F16   v2    i13 || LSU0.LDi.128.U8f.F16   v3   i17
vau.add.f16 v20   v4  v5    || LSU1.LDi.128.U8f.F16   v4    i13 || LSU0.LDi.128.U8f.F16   v5   i17
vau.add.f16 v21   v6  v7    || LSU1.LDi.128.U8f.F16   v6    i13 || LSU0.LDi.128.U8f.F16   v7   i17
vau.add.f16 v22   v8  v9    || LSU1.LDi.128.U8f.F16   v8    i13 || LSU0.LDi.128.U8f.F16   v9   i17
vau.add.f16 v23   v10  v11  || LSU1.LDi.128.U8f.F16   v10   i13 || LSU0.LDi.128.U8f.F16   v11   i17
vau.add.f16 v24   v12  v13  || LSU1.LDi.128.U8f.F16   v12   i13 || LSU0.LDi.128.U8f.F16   v13   i17
vau.add.f16 v25   v14  v15  || LSU1.LDi.128.U8f.F16   v14   i13 || LSU0.LDi.128.U8f.F16   v15   i17
LSU0.STi.128.f16.u8f v18  i18 || vau.mul.f16 v0   v0    v16
ComputeCombinedCost64_loop:
LSU0.STi.128.f16.u8f v19  i18 || vau.mul.f16 v1   v1    v17
LSU0.STi.128.f16.u8f v20  i18 || vau.mul.f16 v2   v2    v16
LSU0.STi.128.f16.u8f v21  i18 || vau.mul.f16 v3   v3    v17
LSU0.STi.128.f16.u8f v22  i18 || vau.mul.f16 v4   v4    v16
LSU0.STi.128.f16.u8f v23  i18 || vau.mul.f16 v5   v5    v17 
LSU0.STi.128.f16.u8f v24  i18 || vau.mul.f16 v6   v6    v16
LSU0.STi.128.f16.u8f v25  i18 || vau.mul.f16 v7   v7    v17

                    

LSU0.LDo.64.l  v25  i19 0 || LSU1.LDo.64.h  v25  i19 8 ||  IAU.ADD i19 i19 16
LSU0.LDo.64.l  v24  i19 0 || LSU1.LDo.64.h  v24  i19 8 ||  IAU.ADD i19 i19 16 || BRU.jmp i30
nop 6


.size mvcvComputeCombinedCost64_asm,.-mvcvComputeCombinedCost64_asm
.end
