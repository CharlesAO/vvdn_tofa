///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvSLaplacian3x3Fp16ToFp16_asm,@function
.version 00.51.04

.code .text.slaplacian3x3Fp16ToFp16


mvcvSLaplacian3x3Fp16ToFp16_asm:
lsu0.ld.32  i0  i18 || iau.add i18 i18 4
lsu0.ld.32  i1  i18 || iau.add i18 i18 4 
lsu0.ld.32  i2  i18 || iau.add i18 i18 4 || lsu1.ld.32  i17  i17 
lsu0.ldil i5 sLaplacian3x3Fp16ToFp16_loop || lsu1.ldih i5 sLaplacian3x3Fp16ToFp16_loop
	IAU.SHR.u32 i6 i16 3 
LSU0.LDIL i7 0x4400 || LSU1.LDIH i7 0x0 	
cmu.cpivr.x16 v22 i7
nop


IAU.SUB i1 i1 16 
lsu0.ldo.64.l v1 i1 0  || lsu1.ldo.64.h v1 i1 8   || IAU.ADD i1 i1 16
lsu0.ldo.64.l v3 i1 0  || lsu1.ldo.64.h v3 i1 8   || IAU.ADD i1 i1 16


lsu0.ldo.64.l v4 i1 0  || lsu1.ldo.64.h v4 i1 8   || IAU.ADD i1 i1 16 
lsu0.ldo.64.l v0 i0 0  || lsu1.ldo.64.h v0 i0 8   || IAU.ADD i0 i0 16
lsu0.ldo.64.l v2 i2 0  || lsu1.ldo.64.h v2 i2 8   || IAU.ADD i2 i2 16
nop 3


cmu.alignvec v5 v1 v3 14 
cmu.alignvec v6 v3 v4 2 || lsu0.ldo.64.l v4 i1 0  || lsu1.ldo.64.h v4 i1 8   || IAU.ADD i1 i1 16 
vau.mul.f16 v7 v3 v22 || cmu.cpvv v1 v3 || lsu0.ldo.64.l v0 i0 0  || lsu1.ldo.64.h v0 i0 8   || IAU.ADD i0 i0 16
cmu.cpvv v3 v4 || lsu0.ldo.64.l v2 i2 0  || lsu1.ldo.64.h v2 i2 8   || IAU.ADD i2 i2 16
vau.add.f16 v8 v0 v2
vau.add.f16 v9 v5 v6
nop 




cmu.alignvec v5 v1 v3 14 || bru.rpl i5 i6 
sLaplacian3x3Fp16ToFp16_loop:
.nowarn 9
vau.add.f16 v10 v9 v8 || cmu.alignvec v6 v3 v4 2 || lsu0.ldo.64.l v4 i1 0  || lsu1.ldo.64.h v4 i1 8   || IAU.ADD i1 i1 16 
.nowarnend
vau.mul.f16 v7 v3 v22 || cmu.cpvv v1 v3 || lsu0.ldo.64.l v0 i0 0  || lsu1.ldo.64.h v0 i0 8   || IAU.ADD i0 i0 16
cmu.cpvv v3 v4 || lsu0.ldo.64.l v2 i2 0  || lsu1.ldo.64.h v2 i2 8   || IAU.ADD i2 i2 16
vau.sub.f16 v11 v7 v10
vau.add.f16 v8 v0 v2
vau.add.f16 v9 v5 v6
lsu0.sto.64.l v11 i17 0  || lsu1.sto.64.h v11 i17 8   || IAU.ADD i17 i17 16






bru.jmp i30
nop 6
.size mvcvSLaplacian3x3Fp16ToFp16_asm,.-mvcvSLaplacian3x3Fp16ToFp16_asm
.end
