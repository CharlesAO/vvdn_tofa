/// @file
/// @copyright Three code copyright Movidius Ltd 2015, Three rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvAggregateFivePaths64_asm,@function
.version 00.51.04

.data .data.AggregateFivePaths64
.align 4

.code .text.AggregateFivePaths64

.lalign
//void, mvcvAggregateFivePaths64, u8* path0, u8* path1, u8* path2, u8* path3, u8* path4, u8* out, u32 width
//                                  i18        i17        i16        i15       i14         i13      i12 
mvcvAggregateFivePaths64_asm:

LSU1.ldil i1, mvcvAggregateFivePaths64_loop
LSU1.ldih i1, mvcvAggregateFivePaths64_loop    
LSU1.ldil i0, 0x3266
cmu.cpivr.x16 v0 i0   

LSU0.LDi.128.U8.F16     v1   i18 || LSU1.LDi.128.U8.F16     v2   i17     
LSU0.LDi.128.U8.F16     v3   i16 || LSU1.LDi.128.U8.F16     v4   i15   
LSU0.LDi.128.U8.F16     v5   i14 || LSU1.LDi.128.U8.F16     v6   i18    
LSU0.LDi.128.U8.F16     v7   i17 || LSU1.LDi.128.U8.F16     v8   i16      
LSU0.LDi.128.U8.F16     v9   i15 || LSU1.LDi.128.U8.F16    v10   i14   
LSU0.LDi.128.U8.F16    v11   i18 || LSU1.LDi.128.U8.F16    v12   i17       
LSU0.LDi.128.U8.F16    v13   i16 || LSU1.LDi.128.U8.F16    v14   i15   
LSU0.LDi.128.U8.F16    v15   i14 || LSU1.LDi.128.U8.F16    v16   i18    
    
 

vau.add.f16 v2 v1 v2             
vau.add.f16 v4 v3 v4
vau.add.f16 v7 v6 v7
vau.add.f16 v9 v8 v9       || LSU0.LDi.128.U8.F16    v17   i17 || LSU1.LDi.128.U8.F16    v18   i16  
vau.add.f16 v4 v2 v4       || LSU0.LDi.128.U8.F16    v19   i15 || LSU1.LDi.128.U8.F16    v21   i14  
vau.add.f16 v12 v11 v12    || LSU0.LDi.128.U8.F16     v1   i18 || LSU1.LDi.128.U8.F16     v2   i17 
vau.add.f16 v9 v7 v9       || LSU0.LDi.128.U8.F16     v3   i16 || LSU1.LDi.128.U8.F16     v4   i15  
vau.add.f16 v5 v5 v4       || LSU0.LDi.128.U8.F16     v5   i14 || LSU1.LDi.128.U8.F16     v6   i18 
vau.add.f16 v14 v13 v14    || LSU0.LDi.128.U8.F16     v7   i17 || LSU1.LDi.128.U8.F16     v8   i16 || bru.rpl i1 i12         
vau.add.f16 v10 v9 v10     || LSU0.LDi.128.U8.F16     v9   i15 || LSU1.LDi.128.U8.F16    v10   i14 
vau.mul.f16 v5 v5 v0       || LSU0.LDi.128.U8.F16    v11   i18 || LSU1.LDi.128.U8.F16    v12   i17 
vau.add.f16 v14 v12 v14    || LSU0.LDi.128.U8.F16    v13   i16 || LSU1.LDi.128.U8.F16    v14   i15  
vau.mul.f16 v10 v10 v0     || LSU0.LDi.128.U8.F16    v15   i14 || LSU1.LDi.128.U8.F16    v16   i18    
vau.add.f16 v17 v16 v17    || LSU0.STi.128.f16.u8     v5   i13 || LSU1.LDi.128.U8.F16    v17   i17
vau.add.f16 v23 v15 v14    || LSU0.LDi.128.U8.F16    v19   i15 || LSU1.LDi.128.U8.F16    v18   i16  
vau.add.f16 v22 v18 v19    || LSU0.STi.128.f16.u8    v10   i13 || LSU1.LDi.128.U8.F16    v20   i14   
vau.add.f16 v2 v1 v2
vau.mul.f16 v23 v23 v0
vau.add.f16 v22 v17 v22
vau.add.f16 v4 v3 v4
vau.add.f16 v7 v6 v7       || LSU0.STi.128.f16.u8 v23 i13
vau.add.f16 v21 v22 v21
vau.add.f16 v9 v8 v9
vau.add.f16 v4 v2 v4
vau.mul.f16 v21 v21 v0
vau.add.f16 v12 v11 v12
vau.add.f16 v9 v7 v9       || LSU0.LDi.128.U8.F16     v1   i18
vau.add.f16 v5 v5 v4       || LSU0.STi.128.f16.u8    v21   i13  || LSU1.LDi.128.U8.F16     v2   i17   
vau.add.f16 v14 v13 v14    || LSU0.LDi.128.U8.F16     v3   i16  || LSU1.LDi.128.U8.F16     v4   i15  
vau.add.f16 v10 v9 v10     || LSU0.LDi.128.U8.F16     v5   i14  || LSU1.LDi.128.U8.F16     v6   i18 
vau.mul.f16 v5 v5 v0       || LSU0.LDi.128.U8.F16     v7   i17  || LSU1.LDi.128.U8.F16     v8   i16  
vau.add.f16 v14 v12 v14    || LSU0.LDi.128.U8.F16     v9   i15  || LSU1.LDi.128.U8.F16    v10   i14   
vau.mul.f16 v10 v10 v0     || LSU0.LDi.128.U8.F16    v11   i18  || LSU1.LDi.128.U8.F16    v12   i17   
vau.add.f16 v17 v16 v17    || LSU0.STi.128.f16.u8     v5   i13  || LSU1.LDi.128.U8.F16    v13   i16
vau.add.f16 v23 v15 v14    || LSU0.LDi.128.U8.F16    v15   i14  || LSU1.LDi.128.U8.F16    v14   i15 
vau.add.f16 v19 v18 v19    || LSU0.STi.128.f16.u8    v10   i13  || LSU1.LDi.128.U8.F16    v16   i18   
vau.add.f16 v2 v1 v2       || LSU0.LDi.128.U8.F16    v17   i17 || LSU1.LDi.128.U8.F16    v18   i16  
vau.mul.f16 v23 v23 v0     || LSU0.LDi.128.U8.F16    v19   i15 || LSU1.LDi.128.U8.F16    v21   i14  
vau.add.f16 v19 v17 v19
vau.add.f16 v4 v3 v4
vau.add.f16 v7 v6 v7       || LSU0.STi.128.f16.u8 v23 i13                             
mvcvAggregateFivePaths64_loop:
vau.add.f16 v20 v19 v20
vau.add.f16 v9 v8 v9       
vau.add.f16 v4 v2 v4      
vau.mul.f16 v20 v20 v0     || LSU0.LDi.128.U8.F16     v1   i18 || LSU1.LDi.128.U8.F16     v2   i17 
vau.add.f16 v12 v11 v12    || LSU0.LDi.128.U8.F16     v3   i16 || LSU1.LDi.128.U8.F16     v4   i15  
vau.add.f16 v9 v7 v9       || LSU1.LDi.128.U8.F16     v6   i18 
vau.add.f16 v5 v5 v4       || LSU1.LDi.128.U8.F16     v5   i14 || LSU0.STi.128.f16.u8 v20 i13 



.nowarnend								   
                      
bru.jmp i30            
nop 6                  
.size mvcvAggregateFivePaths64_asm,.-mvcvAggregateFivePaths64_asm
.end
