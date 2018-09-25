///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvAccumulateWeighted_asm,@function
.version 00.51.04

.data .rodata.AccumulateWeighted
.align 4
 accumulateWeighted_vect_mask:
		.float32    1, 1, 1, 1

.code .text.AccumulateWeighted

mvcvAccumulateWeighted_asm:
//void AccumulateWeighted(u8** srcAddr, u8** maskAddr, fp32** destAddr,u32 width, fp32 alpha)
//                             i18         i17              i16          i15         i113
LSU0.ldil i0, 0x0   || LSU1.ldih i0, 0x3f80
LSU0.ldil i12, accumulateWeighted_loop   || LSU1.ldih i12, accumulateWeighted_loop
IAU.SHR.u32 i15  i15 3
sau.sub.f32 i1 i0 i14
LSU1.LD.32  i18  i18 || LSU0.LD.32  i17  i17
LSU0.LD.32  i16  i16
nop 6
IAU.sub i13 i16 0
cmu.cpivr.x32 v0 i1 //(1-alpha)
cmu.cpivr.x32 v1 i14//(alpha

LSU0.LD.128.U8.F16 v2 i18 || LSU1.LD.128.U8.F16 v3 i17  || IAU.ADD i17 i17 8  || SAU.ADD.i32 i18 i18 8 
LSU0.LDo.64.L  v4 i13  0   || LSU1.LDO.64.H v4 i13  8 
LSU0.LDo.64.L  v5 i13  16   || LSU1.LDO.64.H v5 i13  24 || IAU.ADD i13 i13 32
nop 4
CMU.CPVV.f16l.f32 v6 v2
CMU.CPVV.f16h.f32 v7 v2 || vau.mul.f32 v12 v6 v1
CMU.CPVV.f16l.f32 v8 v3 || vau.mul.f32 v13 v7 v1
cmu.cpvv v14  v4                                         || LSU0.LD.128.U8.F16 v2 i18 || LSU1.LD.128.U8.F16 v3 i17  || IAU.ADD i17 i17 8  || SAU.ADD.i32 i18 i18 8 
cmu.cpvv v15  v5        || vau.mul.f32 v10 v14 v0        || LSU0.LDo.64.L  v4 i13  0   || LSU1.LDO.64.H v4 i13  8        
CMU.CPVV.f16h.f32 v9 v3 || vau.mul.f32 v11 v15 v0        || LSU0.LDo.64.L  v5 i13  16   || LSU1.LDO.64.H v5 i13  24 || IAU.ADD i13 i13 32
CMU.CMZ.f32 v8
CMU.CMZ.f32 v9 || PEU.PVV32 GT || vau.add.f32 v14 v10 v12 
nop 3





CMU.CPVV.f16l.f32 v6 v2 || PEU.PVV32 GT   || vau.add.f32 v15 v11 v13 || bru.rpl i12 i15
 accumulateWeighted_loop:
 .nowarn 9
CMU.CPVV.f16h.f32 v7 v2 || vau.mul.f32 v12 v6 v1 || LSU0.LD.128.U8.F16 v2 i18 || LSU1.LD.128.U8.F16 v3 i17  || IAU.ADD i17 i17 8 
.nowarnend
CMU.CPVV.f16l.f32 v8 v3 || vau.mul.f32 v13 v7 v1 ||LSU0.STO.64.L  v14 i16  0    || LSU1.STO.64.H v14 i16  8 
cmu.cpvv v14  v4                                         || LSU0.STO.64.L  v15 i16  16   || LSU1.STO.64.H v15 i16  24 || IAU.ADD i16 i16 32 || SAU.ADD.i32 i18 i18 8 
cmu.cpvv v15  v5        || vau.mul.f32 v10 v14 v0        || LSU0.LDo.64.L  v4 i13  0   || LSU1.LDO.64.H v4 i13  8        
.nowarn 12
CMU.CPVV.f16h.f32 v9 v3 || vau.mul.f32 v11 v15 v0        || LSU0.LDo.64.L  v5 i13  16   || LSU1.LDO.64.H v5 i13  24 || IAU.ADD i13 i13 32
.nowarnend
CMU.CMZ.f32 v8
CMU.CMZ.f32 v9 || PEU.PVV32 GT || vau.add.f32 v14 v10 v12 



BRU.JMP i30
NOP 6
.size mvcvAccumulateWeighted_asm,.-mvcvAccumulateWeighted_asm
.end
