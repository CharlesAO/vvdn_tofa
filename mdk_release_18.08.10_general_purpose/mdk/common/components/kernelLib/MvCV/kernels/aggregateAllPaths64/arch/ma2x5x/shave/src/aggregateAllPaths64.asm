/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvAggregateAllPaths64_asm,@function
.version 00.51.04

.data .data.AggregateAllPaths64
.align 4

.code .text.AggregateAllPaths64

.lalign
//void, mvcvAggregateAllPaths64, u8* path0, u8* path1, u8* path2, u8* path3, u8* out, u32 width
//                                  i18        i17        i16        i15       i14        i13
mvcvAggregateAllPaths64_asm:
	IAU.SUB i19 i19 8
	LSU0.ST.64.l v24  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v24  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v25  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v25  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v26  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v26  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v27  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v27  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v28  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v28  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v29  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v29  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v30  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v30  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v31  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v31  i19 


LSU0.ldil i0, 0x3400 || LSU1.ldih i0, 0x3400
LSU0.ldil i2, AggregateAllPaths64_loop || LSU1.ldih i2, AggregateAllPaths64_loop
cmu.cpivr.x32 v0 i0


LSU0.LDi.128.U8.F16   v1   i18                                                         
LSU0.LDi.128.U8.F16   v2   i17 
LSU0.LDi.128.U8.F16   v3   i16  || LSU1.LDi.128.U8.F16    v7   i18 
LSU0.LDi.128.U8.F16   v4   i15  || LSU1.LDi.128.U8.F16    v8   i17
LSU1.LDi.128.U8.F16   v21   i18 || LSU0.LDi.128.U8.F16    v9   i16
LSU1.LDi.128.U8.F16   v22   i17 || LSU0.LDi.128.U8.F16   v18   i15
LSU0.LDi.128.U8.F16   v23   i16 || LSU1.LDi.128.U8.F16   v27   i18
LSU0.LDi.128.U8.F16   v24   i15 || LSU1.LDi.128.U8.F16   v28   i17
LSU1.LDi.128.U8.F16   v1   i18  || LSU0.LDi.128.U8.F16   v29   i16
LSU1.LDi.128.U8.F16   v2   i17  || LSU0.LDi.128.U8.F16   v30   i15
LSU0.LDi.128.U8.F16   v3   i16  || LSU1.LDi.128.U8.F16    v7   i18 || vau.add.f16 v5 v1 v2
LSU0.LDi.128.U8.F16   v4   i15  || LSU1.LDi.128.U8.F16    v8   i17 || vau.add.f16 v6 v3 v4
LSU1.LDi.128.U8.F16   v21   i18 || LSU0.LDi.128.U8.F16    v9   i16 || vau.add.f16 v19 v7  v8


LSU1.LDi.128.U8.F16   v22   i17 || LSU0.LDi.128.U8.F16   v18   i15 || vau.add.f16 v20 v9 v18 || bru.rpl i2 i13
LSU0.LDi.128.U8.F16   v23   i16 || LSU1.LDi.128.U8.F16   v27   i18 || vau.add.f16 v10 v5 v6
LSU0.LDi.128.U8.F16   v24   i15 || LSU1.LDi.128.U8.F16   v28   i17 || vau.add.f16 v25 v21 v22
                                   LSU0.LDi.128.U8.F16   v29   i16 || vau.add.f16 v26 v23 v24
                                   LSU0.LDi.128.U8.F16   v30   i15 || vau.add.f16 v31 v27 v28
                                                                      vau.add.f16 v6 v29 v30
								                                      vau.add.f16 v12 v25 v26
								                                      vau.add.f16 v11 v19 v20
								                                      vau.add.f16 v13 v31 v6
													   
vau.add.f16 v5 v1 v2    || LSU0.LDi.128.U8.F16   v1   i18
vau.add.f16 v6 v3 v4    ||  LSU1.LDi.128.U8.F16   v2   i17
vau.add.f16 v19 v7  v8  || LSU0.LDi.128.U8.F16   v3   i16  || LSU1.LDi.128.U8.F16    v7   i18  
vau.add.f16 v20 v9 v18  || LSU0.LDi.128.U8.F16   v4   i15  || LSU1.LDi.128.U8.F16    v8   i17                                       
vau.add.f16 v14 v5 v6   || LSU1.LDi.128.U8.F16   v21   i18 || LSU0.LDi.128.U8.F16    v9   i16                                      
vau.add.f16 v25 v21 v22  || LSU1.LDi.128.U8.F16   v22   i17 || LSU0.LDi.128.U8.F16   v18   i15                                      
vau.add.f16 v26 v23 v24   || LSU0.LDi.128.U8.F16   v23   i16 || LSU1.LDi.128.U8.F16   v27   i18                                    
vau.add.f16 v31 v27 v28   || LSU0.LDi.128.U8.F16   v24   i15 || LSU1.LDi.128.U8.F16   v28   i17                                    
vau.add.f16 v6 v29 v30    || LSU0.LDi.128.U8.F16   v29   i16                                    
vau.add.f16 v16 v25 v26   || LSU0.LDi.128.U8.F16   v30   i15                                    
vau.add.f16 v15 v19 v20                                                                                                
vau.add.f16 v17 v31 v6                                        
vau.mul.f16 v10 v10 v0                                       
vau.mul.f16 v11 v11 v0                                       
vau.mul.f16 v12 v12 v0   || LSU0.LDi.128.U8.F16   v1   i18   ||  LSU1.LDi.128.U8.F16   v2   i17                                       
vau.mul.f16 v13 v13 v0 || LSU0.STi.128.f16.u8 v10 i14  
AggregateAllPaths64_loop:                                    
vau.mul.f16 v14 v14 v0 || LSU0.STi.128.f16.u8 v11 i14     ||   LSU1.LDi.128.U8.F16   v3   i16         
vau.mul.f16 v15 v15 v0 || LSU0.STi.128.f16.u8 v12 i14     ||   LSU1.LDi.128.U8.F16    v7   i18   
vau.mul.f16 v16 v16 v0 || LSU0.STi.128.f16.u8 v13 i14     || LSU1.LDi.128.U8.F16   v4   i15   
vau.mul.f16 v17 v17 v0 || LSU0.STi.128.f16.u8 v14 i14     || LSU1.LDi.128.U8.F16    v8   i17   
                          LSU0.STi.128.f16.u8 v15 i14     || vau.add.f16 v5 v1 v2
                          LSU0.STi.128.f16.u8 v16 i14     || LSU1.LDi.128.U8.F16   v21   i18 || vau.add.f16 v6 v3 v4
                          LSU0.STi.128.f16.u8 v17 i14     || LSU1.LDi.128.U8.F16    v9   i16 || vau.add.f16 v19 v7  v8




						  
	LSU0.LD.64.h  v31  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v31  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v30  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v30  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v29  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v29  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v28  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v28  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v27  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v27  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v26  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v26  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v25  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v25  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v24  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v24  i19 || IAU.ADD i19 i19 8	


bru.jmp i30
nop 6


.size mvcvAggregateAllPaths64_asm,.-mvcvAggregateAllPaths64_asm
.end
