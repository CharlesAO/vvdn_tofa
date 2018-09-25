/// @file
/// @copyright Three code copyright Movidius Ltd 2015, Three rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvAggregateThreePaths64_asm,@function
.version 00.51.04

.data .data.AggregateThreePaths64
.align 4

.code .text.AggregateThreePaths64

.lalign
//void, mvcvAggregateThreePaths64, u8* path0, u8* path1, u8* path2, u8* out, u32 width
//                                  i18        i17        i16        i15       i14   
mvcvAggregateThreePaths64_asm:
LSU0.LDi.128.U8.F16    v1   i18    
LSU0.LDi.128.U8.F16    v2   i17     
LSU0.LDi.128.U8.F16    v3   i16 || LSU1.ldil i1, mvcvAggregateThreePaths64_loop
LSU0.LDi.128.U8.F16    v4   i18 || LSU1.ldih i1, mvcvAggregateThreePaths64_loop    
LSU0.LDi.128.U8.F16    v5   i17 || LSU1.ldil i0, 0x3555    
LSU0.LDi.128.U8.F16    v6   i16 || cmu.cpivr.x16 v0 i0   
LSU0.LDi.128.U8.F16    v7   i18     
LSU0.LDi.128.U8.F16    v8   i17 
LSU0.LDi.128.U8.F16    v9   i16 
LSU0.LDi.128.U8.F16   v10   i18 
LSU0.LDi.128.U8.F16   v11   i17 || vau.add.f16 v13 v1 v2 
LSU0.LDi.128.U8.F16   v12   i16 
LSU0.LDi.128.U8.F16    v1   i18 || vau.add.f16 v14 v4 v5  
LSU0.LDi.128.U8.F16    v2   i17 || vau.add.f16 v13 v13 v3
LSU0.LDi.128.U8.F16    v3   i16 || vau.add.f16 v15 v7 v8 
LSU0.LDi.128.U8.F16    v4   i18 || vau.add.f16 v14 v14 v6  || bru.rpl i1 i14       
LSU0.LDi.128.U8.F16    v5   i17 || vau.mul.f16 v13 v13 v0    
LSU0.LDi.128.U8.F16    v6   i16 || vau.add.f16 v16 v10 v11  
LSU0.LDi.128.U8.F16    v7   i18 || vau.add.f16 v15 v15 v9     
LSU0.LDi.128.U8.F16    v8   i17 || vau.mul.f16 v14 v14 v0    
LSU0.LDi.128.U8.F16    v9   i16 || vau.add.f16 v17 v1 v2
LSU0.LDi.128.U8.F16   v10   i18 || vau.add.f16 v16 v16 v12
LSU0.LDi.128.U8.F16   v11   i17 || vau.mul.f16 v15 v15 v0
.nowarn 10
LSU0.LDi.128.U8.F16   v12   i16 || vau.add.f16 v18 v4 v5                                       || LSU1.LDi.128.U8.F16    v1   i18
                                   vau.add.f16 v17 v17 v3                                      || LSU1.LDi.128.U8.F16    v2   i17
                                   vau.mul.f16 v16 v16 v0                                      || LSU1.LDi.128.U8.F16    v3   i16
                                   vau.add.f16 v19 v7 v8                                       || LSU1.LDi.128.U8.F16    v4   i18
                                   vau.add.f16 v18 v18 v6                                      || LSU1.LDi.128.U8.F16    v5   i17
                                   vau.mul.f16 v17 v17 v0                                      || LSU1.LDi.128.U8.F16    v6   i16
                                   vau.add.f16 v20 v10 v11                                     || LSU1.LDi.128.U8.F16    v7   i18
                                   vau.add.f16 v19 v19 v9                                      || LSU1.LDi.128.U8.F16    v8   i17
                                   vau.mul.f16 v18 v18 v0    || LSU0.STi.128.f16.u8 v13 i15    || LSU1.LDi.128.U8.F16    v9   i16
mvcvAggregateThreePaths64_loop:
                                   vau.add.f16 v20 v20 v12   || LSU0.STi.128.f16.u8 v14 i15    || LSU1.LDi.128.U8.F16   v10   i18
                                   vau.add.f16 v13 v1 v2     || LSU0.STi.128.f16.u8 v15 i15    || LSU1.LDi.128.U8.F16   v11   i17 
                                   vau.mul.f16 v19 v19 v0    || LSU0.STi.128.f16.u8 v16 i15    || LSU1.LDi.128.U8.F16   v12   i16 
                                   vau.mul.f16 v20 v20 v0    || LSU0.STi.128.f16.u8 v17 i15    
                                   vau.add.f16 v14 v4 v5     || LSU0.STi.128.f16.u8 v18 i15    || LSU1.LDi.128.U8.F16    v1   i18
                                   vau.add.f16 v13 v13 v3    || LSU0.STi.128.f16.u8 v19 i15    || LSU1.LDi.128.U8.F16    v2   i17
                                   vau.add.f16 v15 v7 v8     || LSU0.STi.128.f16.u8 v20 i15    || LSU1.LDi.128.U8.F16    v3   i16
.nowarnend								   
                      
bru.jmp i30            
nop 6                  
.size mvcvAggregateThreePaths64_asm,.-mvcvAggregateThreePaths64_asm
.end
