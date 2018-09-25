/// @file
/// @copyright Three code copyright Movidius Ltd 2015, Three rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///
    
.type mvcvAggregateFivePaths32_asm,@function
.version 00.51.04

.data .data.AggregateFivePaths32
.align 4

.code .text.AggregateFivePaths32

.lalign
//void mvcvAggregateFivePaths32(u8* path0, u8* path1, u8* path2, u8* path3, u8* path4, u8* out, u32 width)
//                                  i18        i17        i16        i15       i14      i13         i12
mvcvAggregateFivePaths32_asm:
.nowarn 10
LSU0.ldil i0, 0x3266 || LSU1.ldil i1, mvcvAggregateFivePaths32_loop
cmu.cpivr.x16 v0 i0  || LSU1.ldih i1, mvcvAggregateFivePaths32_loop  


LSU0.LDi.128.U8.F16    v1   i18  || LSU1.LDi.128.U8.F16    v3   i16  
LSU0.LDi.128.U8.F16    v2   i17  || LSU1.LDi.128.U8.F16    v4   i15            
LSU0.LDi.128.U8.F16    v5   i14  || LSU1.LDi.128.U8.F16     v7   i18 
LSU0.LDi.128.U8.F16     v8   i17 || LSU1.LDi.128.U8.F16     v9   i16            
LSU0.LDi.128.U8.F16    v10   i15 || LSU1.LDi.128.U8.F16    v11   i14
LSU0.LDi.128.U8.F16    v12   i18 || LSU1.LDi.128.U8.F16    v13   i17 
LSU0.LDi.128.U8.F16    v14   i16 || LSU1.LDi.128.U8.F16    v15   i15 
LSU0.LDi.128.U8.F16    v16   i14 || LSU1.LDi.128.U8.F16    v17   i18  
LSU0.LDi.128.U8.F16    v18   i17 || LSU1.LDi.128.U8.F16    v19   i16            
LSU0.LDi.128.U8.F16    v20   i15 || LSU1.LDi.128.U8.F16    v21   i14

vau.add.f16 v2 v1 v2    
vau.add.f16 v4 v3 v4
vau.add.f16  v8 v7  v8
vau.add.f16 v10 v9 v10
vau.add.f16 v5 v2 v5
vau.add.f16 v11 v8 v11  
vau.add.f16 v13 v12 v13 || LSU0.LDi.128.U8.F16    v1   i18  || LSU1.LDi.128.U8.F16    v3   i16  || bru.rpl i1 i12    
vau.add.f16 v5 v5 v4    || LSU0.LDi.128.U8.F16    v2   i17  || LSU1.LDi.128.U8.F16    v4   i15 
vau.add.f16 v11 v11 v10 || LSU0.LDi.128.U8.F16    v5   i14  || LSU1.LDi.128.U8.F16     v7   i18 
vau.add.f16 v15 v14 v15 || LSU0.LDi.128.U8.F16     v8   i17 || LSU1.LDi.128.U8.F16     v9   i16   
vau.mul.f16 v5 v5 v0    || LSU0.LDi.128.U8.F16    v10   i15 || LSU1.LDi.128.U8.F16    v11   i14 
vau.mul.f16 v11 v11 v0  || LSU0.LDi.128.U8.F16    v12   i18 || LSU1.LDi.128.U8.F16    v13   i17 
vau.add.f16 v16 v13 v16 || LSU0.LDi.128.U8.F16    v14   i16 || LSU1.LDi.128.U8.F16    v15   i15 
vau.add.f16 v18 v17 v18 || LSU0.STi.128.f16.u8     v5 i13   || LSU1.LDi.128.U8.F16    v16   i14
vau.add.f16 v20 v19 v20 || LSU0.STi.128.f16.u8    v11 i13   || LSU1.LDi.128.U8.F16    v17   i18  
vau.add.f16 v22 v16 v15 || LSU0.LDi.128.U8.F16    v18   i17 || LSU1.LDi.128.U8.F16    v19   i16       
vau.add.f16 v23 v18 v21 || LSU0.LDi.128.U8.F16    v20   i15 || LSU1.LDi.128.U8.F16    v21   i14
vau.add.f16 v2 v1 v2   
vau.mul.f16 v22 v22 v0
mvcvAggregateFivePaths32_loop:
vau.add.f16 v23 v23 v20
vau.add.f16 v4 v3 v4
vau.add.f16  v8 v7  v8  || LSU0.STi.128.f16.u8 v22 i13                       
vau.mul.f16 v23 v23 v0
vau.add.f16 v10 v9 v10
vau.add.f16 v5 v2 v5
vau.add.f16 v11 v8 v11  || LSU0.STi.128.f16.u8 v23 i13   
  
bru.jmp i30            
nop 6                  
.nowarnend
.size mvcvAggregateFivePaths32_asm,.-mvcvAggregateFivePaths32_asm
.end
