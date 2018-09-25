///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvAccumulateSquare_asm,@function
.version 00.51.04


// /------------------/
//    CODE SECTION
// /------------------/
.code .text.AccumulateSquare_asm
//void AccumulateSquare(u8** srcAddr, u8** maskAddr, fp32** destAddr, u32 width, u32 height)
//                           i18            i17           i16            i15         i14
mvcvAccumulateSquare_asm:

LSU1.LD.32  i18  i18 || LSU0.LD.32  i17  i17
LSU0.LD.32  i16  i16
nop 6
cmu.cpii i14 i16
LSU0.ldil i0, accumulateSquare_loop   || LSU1.ldih i0, accumulateSquare_loop
IAU.SHR.u32 i15  i15 3

LSU0.LD.128.U8.F16 v0 i18 || LSU1.LD.128.U8.F16 v1 i17  || IAU.ADD i17 i17 8  || SAU.ADD.i32 i18 i18 8 
LSU0.LDo.64.L  v2 i14  0   || LSU1.LDO.64.H v2 i14  8 
LSU0.LDo.64.L  v3 i14  16   || LSU1.LDO.64.H v3 i14  24 || IAU.ADD i14 i14 32

nop 4

CMU.CPVV.f16l.f32 v7 v0                          
CMU.CPVV.f16h.f32 v8 v0 || vau.mul.f32 v11 v7 v7 
CMU.CPVV.f16l.f32 v5 v1 || vau.mul.f32 v12 v8 v8 
cmu.cpvv v13 v2
cmu.cpvv v14 v3
CMU.CPVV.f16h.f32 v6 v1                                   || LSU0.LD.128.U8.F16 v0 i18 || LSU1.LD.128.U8.F16 v1 i17  || IAU.ADD i17 i17 8  || SAU.ADD.i32 i18 i18 8 
CMU.CMZ.f32 v5                                            || LSU0.LDo.64.L  v2 i14  0   || LSU1.LDO.64.H v2 i14  8 
CMU.CMZ.f32 v6 || PEU.PVV32 GT || vau.add.f32 v13 v11 v13 || LSU0.LDo.64.L  v3 i14  16   || LSU1.LDO.64.H v3 i14  24 || IAU.ADD i14 i14 32
nop 4



CMU.CPVV.f16l.f32 v7 v0                              || PEU.PVV32 GT || vau.add.f32 v14 v12 v14 || bru.rpl i0 i15
 accumulateSquare_loop:
 .nowarn 10
CMU.CPVV.f16h.f32 v8 v0 || vau.mul.f32 v11 v7 v7     || LSU0.LD.128.U8.F16 v0 i18 || LSU1.LD.128.U8.F16 v1 i17  
CMU.CPVV.f16l.f32 v5 v1 || vau.mul.f32 v12 v8 v8     || LSU0.STO.64.L v13  i16  0  || LSU1.STO.64.H v13 i16  8
.nowarnend
cmu.cpvv v13 v2                                      || LSU0.STO.64.L v14 i16  16 || LSU1.STO.64.H v14 i16  24 || IAU.ADD i16 i16 32
cmu.cpvv v14 v3                                      || LSU0.LDo.64.L  v2 i14  0   || LSU1.LDO.64.H v2 i14  8  || IAU.ADD i17 i17 8  || SAU.ADD.i32 i18 i18 8  
CMU.CPVV.f16h.f32 v6 v1                              || LSU0.LDo.64.L  v3 i14  16   || LSU1.LDO.64.H v3 i14  24 || IAU.ADD i14 i14 32     
CMU.CMZ.f32 v5                                            
CMU.CMZ.f32 v6 || PEU.PVV32 GT || vau.add.f32 v13 v11 v13 





    BRU.JMP i30
NOP 6

.size mvcvAccumulateSquare_asm,.-mvcvAccumulateSquare_asm
.end
