///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvWarpMeshSample8bit_asm,@function
.version 00.51.04

.data .data.warpMeshSample8bit
.align 4

.code .text.warpMeshSample8bit
//void mvcvwarpMeshSample8bit(unsigned char *tile, unsigned char * out_tile, half *mx, half *my, unsigned int tile_width, unsigned int tile_height)
//                                  i18                      i17               i16           i15                 i14                     i13

mvcvWarpMeshSample8bit_asm:
    IAU.SUB i19 i19 16 || LSU0.ldil i0, 0x3c00 || LSU1.ldih i0, 0x3c00
	LSU0.STo.64.l v24  i19 0  || LSU1.STo.64.h v24  i19 8 || IAU.SUB i19 i19 16
	LSU0.STo.64.l v25  i19 0  || LSU1.STo.64.h v25  i19 8 || IAU.SUB i19 i19 16
	LSU0.STo.64.l v26  i19 0  || LSU1.STo.64.h v26  i19 8 || IAU.SUB i19 i19 16
	LSU0.STo.64.l v27  i19 0  || LSU1.STo.64.h v27  i19 8 || IAU.SUB i19 i19 16 || cmu.cpivr.x32 v26 i0
	LSU0.STo.64.l v28  i19 0  || LSU1.STo.64.h v28  i19 8 || IAU.SUB i19 i19 16
	LSU0.STo.64.l v29  i19 0  || LSU1.STo.64.h v29  i19 8 || IAU.SUB i19 i19 16
	LSU0.STo.64.l v30  i19 0  || LSU1.STo.64.h v30  i19 8 || IAU.SUB i19 i19 16
	LSU0.STo.64.l v31  i19 0  || LSU1.STo.64.h v31  i19 8 || IAU.SUB i19 i19 8
	LSU0.LDo.64.l v0  i16 0 ||  LSU1.LDo.64.h v0  i16 8 || iau.add i16 i16 16 //xcoord
	LSU0.LDo.64.l v2  i15 0 ||  LSU1.LDo.64.h v2  i15 8 || iau.add i15 i15 16 //ycoord	
	LSU0.ST.64  i20  i19 || IAU.SUB i19 i19 8
	LSU1.ST.64  i22  i19 || IAU.SUB i19 i19 8
	LSU0.ST.64  i24  i19 || IAU.SUB i19 i19 8
	LSU1.ST.64  i26  i19 || IAU.SUB i19 i19 8
    LSU0.ST.64  i28  i19 || IAU.SUB i19 i19 8
	LSU1.ST.64  i30  i19

	
	
	
 lsu0.ldil i1 0x4  || lsu1.ldih i1 0x4
cmu.cpivr.x32 v24 i1  || lsu0.ldil i2 0x4  || lsu1.ldih i2 0x0
cmu.cpivr.x32 v25 i2 




cmu.cpvv.f16.i16s  v10 v0 
cmu.cpvv.f16.i16s  v11 v2 
cmu.cpvv.i16.f16 v12 v10  
cmu.cpvv.i16.f16 v13 v11  || vau.mul.i16 v1 v24 v11


 
vau.sub.f16 v16 v0 v12  //frac x
vau.sub.f16 v29 v2 v13 || cmu.cpivr.x32  v5 i18 //frac y

cmu.cpvv.i16.i32 v22 v1   || iau.xor i1 i1 i1 //x
cmu.SHRIV.x32  v1 v1 i10  || lsu0.ldil i1 32//y
cmu.SHRIV.x32  v1 v1 i10  || lsu0.ldil i0 mvcvWarpMeshSample8bit_loop  || lsu1.ldih i0 mvcvWarpMeshSample8bit_loop
cmu.cpvv.i16.i32 v3 v1   
cmu.cpvv.i16.i32 v17 v10 
cmu.SHRIV.x32  v10 v10 i10  
cmu.SHRIV.x32  v10 v10 i10  || vau.add.i32 v12 v5 v22
cmu.cpvv.i16.i32 v9 v10     || vau.add.i32 v6 v5 v3
cmu.cpvid  i2 v12.0  || LSU0.LDo.64.l v0  i16 0 || vau.sub.f16 v21 v26  v16  //1-frac x  
cmu.cpvid  i4 v12.2  || LSU0.LDo.64.h v0  i16 8 || vau.sub.f16  v7 v26  v29 //1-frac y
cmu.cpvid  i6 v6.0   || LSU0.LDo.64.l v2  i15 0 || iau.add i16 i16 16 //xcoord                                                         
cmu.cpvid  i8 v6.2   || LSU0.LDo.64.h v2  i15 8 || iau.add i15 i15 16 //ycoord    

LSU0.ld.64  i11  i2   || lsu1.cp i2 v17.0                                              
LSU0.ld.64  i13  i3   || lsu1.cp i3 v17.1
LSU0.ld.64  i20  i4   || lsu1.cp i4 v17.2 
LSU0.ld.64  i22  i5   || lsu1.cp i5 v17.3 
LSU0.ld.64  i24  i6   || lsu1.cp i6  v9.0 
LSU0.ld.64  i26  i7   || lsu1.cp i7  v9.1 
LSU0.ld.64  i28  i8  
LSU0.ld.64  i30  i9                                      || iau.add i11 i11  i2                        
LSU0.LD.128.u8.f16   v14 i11   	                         || iau.add i13 i13  i3  
LSU0.LD.128.u8.f16   v14 i13 	                         || iau.add i20 i20  i4   
LSU0.LD.128.u8.f16   v14 i20                             || iau.add i22 i22  i5   || cmu.cpvid i8  v9.2 
LSU0.LD.128.u8.f16   v14 i22                             || iau.add i24 i24  i6   || cmu.cpvv.f16.i16s  v10 v0 //x
LSU0.LD.128.u8.f16   v14 i24                             || iau.add i26 i26  i7   || cmu.cpvv.f16.i16s  v11 v2 //y
LSU0.LD.128.u8.f16   v14 i26                             || iau.add i28 i28  i8   || cmu.cpvv.i16.f16 v12 v10          
LSU0.LD.128.u8.f16   v14 i28                             || iau.add i30 i30  i9   || cmu.cpvv.i16.f16 v13 v11 || lsu1.cp  v4 v21        
LSU0.LD.128.u8.f16   v14 i30     || lsu1.cp  i11 v14.0   || iau.add i12 i12 i2    || vau.mul.i16 v1 v24 v11   
LSU0.LD.128.u8.f16   v14 i12     || lsu1.cp  i12 v14.0   || iau.add i14 i14 i3    || vau.sub.f16 v16 v0 v12   || cmu.cpvv  v8 v7  
LSU0.LD.128.u8.f16   v14 i14     || lsu1.cp  i13 v14.0   || iau.add i21 i21 i4    || vau.sub.f16 v29 v2 v13   || cmu.cpvv v15 v29 //frac x   
LSU0.LD.128.u8.f16   v14 i21     || lsu1.cp  i14 v14.0   || iau.add i23 i23 i5                                || cmu.cpvv v18 v16 //frac y  
LSU0.LD.128.u8.f16   v14 i23  	 || lsu1.cp  i20 v14.0   || iau.add i25 i25 i6    || cmu.cpvv.i16.i32 v22 v1   
LSU0.LD.128.u8.f16   v14 i25     || lsu1.cp  i21 v14.0   || iau.add i27 i27 i7    || cmu.SHRIV.x32  v1 v1 i10    
LSU0.LD.128.u8.f16   v14 i27     || lsu1.cp  i22 v14.0   || iau.add i29 i29 i8    || cmu.SHRIV.x32  v1 v1 i10    
LSU0.LD.128.u8.f16   v14 i29     || lsu1.cp  i23 v14.0   || iau.add i31 i31 i9    || cmu.cpvv.i16.i32 v3 v1      
LSU0.LD.128.u8.f16   v14 i31     || lsu1.cp  i24 v14.0                            || cmu.cpvv.i16.i32 v17 v10    
                                    lsu1.cp  i25 v14.0                            || cmu.SHRIV.x32  v10 v10 i10   || LSU0.LDo.64.l v0  i16 0   
                                    lsu1.cp  i26 v14.0                            || cmu.SHRIV.x32  v10 v10 i10  || vau.add.i32 v12 v5 v22   || LSU0.LDo.64.h v0  i16 8
                                    lsu1.cp  i27 v14.0                            || cmu.cpvv.i16.i32 v9 v10     || vau.add.i32 v6 v5 v3    
                                    lsu1.cp  i28 v14.0                            || cmu.cpvid  i2 v12.0   || vau.sub.f16 v21 v26  v16 || LSU0.LDo.64.l v2  i15 0 || iau.add i16 i16 16 //xcoord            //1-frac x      
                                    lsu1.cp  i29 v14.0                            || cmu.cpvid  i4 v12.2   || vau.sub.f16  v7 v26  v29 || LSU0.LDo.64.h v2  i15 8 || iau.add i15 i15 16 //ycoord           //1-frac y      
                                    lsu1.cp  i30 v14.0                            || cmu.cpvid  i6 v6.0     || LSU0.ldo.32  i11  i2 0                           
                                    lsu1.cp  i31 v14.0                            || cmu.cpvid  i8 v6.2     || LSU0.ldo.32  i12  i2 4           

                                                   
LSU0.ld.64  i13  i3     || lsu1.cp i2 v17.0                                         ||  cmu.cpivd v30.0 i11    || bru.rpl i0 i1          
LSU0.ld.64  i20  i4     || lsu1.cp i3 v17.1                                         ||  cmu.cpivd v30.2 i13
LSU0.ld.64  i22  i5     || lsu1.cp i4 v17.2                                         ||  cmu.cpivd v27.0 i20   
LSU0.ld.64  i24  i6     || lsu1.cp i5 v17.3                                         ||  cmu.cpivd v27.2 i22   
LSU0.ld.64  i26  i7     || lsu1.cp i6  v9.0                                         ||  CMU.VDILV.x16 v19 v23 v30 v27        
LSU0.ld.64  i28  i8     || lsu1.cp i7  v9.1                                         ||  cmu.cpivd v28.0 i24 
LSU0.ld.64  i30  i9                                      || iau.add i11 i11  i2   ||  cmu.cpivd v28.2 i26  ||  lsu1.cp v31.3 i31                                                 
LSU0.LD.128.u8.f16   v14 i11   	                         || iau.add i13 i13  i3   ||  cmu.cpivd v31.0 i28  ||  lsu1.cp v31.2 i30                                     
LSU0.LD.128.u8.f16   v14 i13 	                         || iau.add i20 i20  i4   || CMU.VDILV.x16 v20 v25 v28 v31
LSU0.LD.128.u8.f16   v14 i20                             || iau.add i22 i22  i5   || vau.mul.f16 v23 v23 v4        || cmu.cpvid i8  v9.2         //x  
LSU0.LD.128.u8.f16   v14 i22                             || iau.add i24 i24  i6   || vau.mul.f16 v19 v19 v18       || cmu.cpvv.f16.i16s  v10 v0  //y       
LSU0.LD.128.u8.f16   v14 i24                             || iau.add i26 i26  i7   || vau.mul.f16 v25 v25 v4        || cmu.cpvv.f16.i16s  v11 v2     
LSU0.LD.128.u8.f16   v14 i26                             || iau.add i28 i28  i8   || vau.mul.f16 v20 v20 v18       || cmu.cpvv.i16.f16 v12 v10 
LSU0.LD.128.u8.f16   v14 i28                             || iau.add i30 i30  i9   || vau.mul.i16 v1 v24 v11        || cmu.cpvv.i16.f16 v13 v11 ||   lsu1.cp     v4 v21                            
LSU0.LD.128.u8.f16   v14 i30     || lsu1.cp  i11 v14.0   || iau.add i12 i12 i2                                     
LSU0.LD.128.u8.f16   v14 i12     || lsu1.cp  i12 v14.0   || iau.add i14 i14 i3    || cmu.cpvv v18 v16           || vau.add.f16 v23 v19 v23//frac y                                                              
LSU0.LD.128.u8.f16   v14 i14     || lsu1.cp  i13 v14.0   || iau.add i21 i21 i4    || cmu.cpvv.i16.i32 v22 v1    || vau.add.f16 v25 v20 v25
LSU0.LD.128.u8.f16   v14 i21     || lsu1.cp  i14 v14.0   || iau.add i23 i23 i5    || cmu.SHRIV.x32  v1 v1 i10     
LSU0.LD.128.u8.f16   v14 i23  	 || lsu1.cp  i20 v14.0   || iau.add i25 i25 i6    || cmu.SHRIV.x32  v1 v1 i10    || vau.mul.f16 v23 v23 v8
LSU0.LD.128.u8.f16   v14 i25     || lsu1.cp  i21 v14.0   || iau.add i27 i27 i7    || cmu.cpvv.i16.i32 v3 v1      || vau.mul.f16 v25 v25 v15   
LSU0.LD.128.u8.f16   v14 i27     || lsu1.cp  i22 v14.0   || iau.add i29 i29 i8    || cmu.cpvv.i16.i32 v17 v10       
LSU0.LD.128.u8.f16   v14 i29     || lsu1.cp  i23 v14.0   || iau.add i31 i31 i9    || cmu.SHRIV.x32  v10 v10 i10      || vau.add.i32 v12 v5 v22
LSU0.LD.128.u8.f16   v14 i31     || lsu1.cp  i24 v14.0                            || cmu.SHRIV.x32  v10 v10 i10     || vau.sub.f16 v16 v0 v12
mvcvWarpMeshSample8bit_loop:
.nowarn 10
                                    lsu1.cp  i25 v14.0                            || cmu.cpvv v15 v29                || LSU0.LDo.64.l v0  i16 0 || vau.sub.f16 v29 v2 v13                   //frac x  
.nowarnend
                                    lsu1.cp  i26 v14.0                            || cmu.cpvv  v8 v7                 || LSU0.LDo.64.h v0  i16 8        
                                    lsu1.cp  i27 v14.0                            || cmu.cpvv.i16.i32 v9 v10         || LSU0.LDo.64.l v2  i15 0      || vau.add.i32 v6 v5 v3    || iau.add i16 i16 16 //xcoord  //1-frac x   
                                    lsu1.cp  i28 v14.0                            || cmu.cpvid  i2 v12.0             || LSU0.LDo.64.h v2  i15 8      || vau.add.f16 v25 v25 v23  || iau.add i15 i15 16 //ycoord //1-frac y       
                                    lsu1.cp  i29 v14.0                            || cmu.cpvid  i4 v12.2                                            || LSU0.ldo.32  i11  i2 0          
                                    lsu1.cp  i30 v14.0                            || cmu.cpvid  i6 v6.0              || vau.sub.f16 v21 v26  v16    || LSU0.ldo.32  i12  i2 4                                                                     
                                    lsu1.cp  i31 v14.0                            || lsu0.sti.128.f16.u8 v25 i17 || cmu.cpvid  i8 v6.2 || vau.sub.f16  v7 v26  v29  







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

.size mvcvWarpMeshSample8bit_asm,.-mvcvWarpMeshSample8bit_asm
.end
