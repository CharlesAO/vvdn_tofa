///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvtile_samplerRGB888_asm,@function
.version 00.51.04

.data .data.tile_samplerRGB888
.align 4
 tile_sampler_indx:
		.float16    16, 15, 14, 13, 12, 11, 10, 9, 8,  7,  6,  5,  4,  3,  2,  1
		.float16     0,  1,  2,  3,  4,  5,  6, 7, 8,  9, 10, 11, 12, 13, 14, 15, 0.0625

.code .text.tile_samplerRGB888
//void mvcvtile_samplerRGB888(unsigned char *tile, unsigned char * out_tile, float *xcoord, float *ycoord, unsigned int tile_width, unsigned int tile_height)
//                                  i18                      i17               i16           i15                 i14                     i13

mvcvtile_samplerRGB888_asm:
lsu0.ldil i10 0x0003 || lsu1.ldih i10 0x0000
iau.mul i14 i14 i10
nop

	IAU.SUB i19 i19 4
	LSU0.ST.32  i20  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i21  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i22  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i23  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i24  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i25  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i26  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i27  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i28  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i29  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i30  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i31  i19 


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

	
	
LSU0.LDo.64.l v0  i16 0  || LSU1.LDo.64.h v0 i16 8 //xcoord	
LSU0.LDo.64.l v1  i15 0  || LSU1.LDo.64.h v1 i15 8 //ycoord
LSU0.ldil i1, 0x3c00 || LSU1.ldih i1, 0x3c00 || IAU.SHL i31 i14 16   || vau.xor v2 v2 v2
LSU0.ldil i30, 0xffff || LSU1.ldih i30, 0x0000 || IAU.OR i31 i31 i14 || vau.xor v3 v3 v3
LSU0.ldil i0, tile_sampler_indx || LSU1.ldih i0, tile_sampler_indx
LSU0.LDo.64.l v4  i0 0   || LSU1.LDo.64.h v4  i0 8                           //16....9 indx
LSU0.LDo.64.l v5  i0 16  || LSU1.LDo.64.h v5  i0 24                          //8....1 indx
LSU0.LDo.64.l v6  i0 32  || LSU1.LDo.64.h v6  i0 40                      || cmu.cpvv.f32.f16 v2 v0 //xcoord     //0....7 indx
LSU0.LDo.64.l v7  i0 48  || LSU1.LDo.64.h v7  i0 56 || iau.add i0 i0 64  || cmu.cpvv.f32.f16 v3 v1 //ycoord    //8....15 indx
lsu0.ld.16r v23 i0 //1/16
nop 2




vau.mul.f16  v8 v2 v4 || lsu1.swzv8 [00000000] 
vau.mul.f16  v9 v2 v5 || lsu1.swzv8 [00000000] 
vau.mul.f16 v10 v2 v6 || lsu1.swzv8 [11111111] 
vau.mul.f16 v11 v2 v7 || lsu1.swzv8 [11111111] 
vau.mul.f16 v16 v2 v4 || lsu1.swzv8 [22222222] 
vau.mul.f16 v17 v2 v5 || lsu1.swzv8 [22222222] 
vau.mul.f16 v18 v2 v6 || lsu1.swzv8 [33333333] 
vau.mul.f16 v19 v2 v7 || lsu1.swzv8 [33333333] 
vau.add.f16 v12 v8 v10
vau.add.f16 v13 v9 v11
vau.add.f16 v20 v16 v18
vau.add.f16 v21 v17 v19
vau.mul.f16 v14 v12 v23    //xcoord[0] * (mesh_gran_x - j) + xcoord[1] * j
vau.mul.f16 v15 v13 v23
vau.mul.f16 v22 v20 v23  //xcoord[2] * (mesh_gran_x - j) + xcoord[3] * j
vau.mul.f16 v24 v21 v23
vau.mul.f16  v8 v3 v4 || lsu1.swzv8 [00000000] 
vau.mul.f16  v9 v3 v5 || lsu1.swzv8 [00000000] 
vau.mul.f16 v10 v3 v6 || lsu1.swzv8 [11111111] 
vau.mul.f16 v11 v3 v7 || lsu1.swzv8 [11111111] 
vau.mul.f16 v16 v3 v4 || lsu1.swzv8 [22222222] 
vau.mul.f16 v17 v3 v5 || lsu1.swzv8 [22222222] 
vau.mul.f16 v18 v3 v6 || lsu1.swzv8 [33333333] 
vau.mul.f16 v19 v3 v7 || lsu1.swzv8 [33333333] 
vau.add.f16 v12 v8 v10
vau.add.f16 v13 v9 v11
vau.add.f16 v20 v16 v18
vau.add.f16 v21 v17 v19
vau.mul.f16 v25 v12 v23 || lsu0.ldil i0 0x10 || lsu1.ldih i0 0x10   //ycoord[0] * (mesh_gran_x - j) + ycoord[1] * j
vau.mul.f16 v26 v13 v23 || lsu0.ldil i6 0x0  || lsu1.ldih i6 0x0
vau.mul.f16 v27 v20 v23 || CMU.CPII.i16.f16 i0 i0 //(ycoord[2] * (mesh_gran_x - j) + ycoord[3] * j
vau.mul.f16 v28 v21 v23 || CMU.CPII.i16.f16 i6 i6 || lsu0.ldil i7 tile_sampler_loop  || lsu1.ldih i7 tile_sampler_loop


lsu0.ldil i10 0x0003 || lsu1.ldih i10 0x0003
cmu.cpivr.x32  v5 i18

cmu.cpiv.x32 v23.1 i1
cmu.cpiv.x32 v23.2 i31
cmu.cpiv.x32 v23.3 i10

bru.rps i7 16

cmu.cpivr.x32 v2 i0
cmu.cpivr.x32 v3 i6
vau.mul.f16 v8 v2 v14 
vau.mul.f16 v9 v2 v15 


vau.mul.f16 v0 v3 v22 
vau.mul.f16 v1 v3 v24 
sau.sub.f16 i0 i0 i1
vau.add.f16 v8 v8 v0
vau.add.f16 v9 v9 v1
sau.add.f16 i6 i6 i1
vau.mul.f16 v0 v23 v8 || lsu1.swzv8 [00000000] 
vau.mul.f16 v1 v23 v9 || lsu1.swzv8 [00000000] 
vau.mul.f16 v8 v2 v25 
vau.mul.f16 v9 v2 v26 
vau.mul.f16 v2 v3 v27 
vau.mul.f16 v3 v3 v28 
nop 
vau.add.f16 v8 v8 v2
vau.add.f16 v9 v9 v3
nop 
vau.mul.f16 v2 v23 v8 || lsu1.swzv8 [00000000] 
vau.mul.f16 v3 v23 v9 || lsu1.swzv8 [00000000] 




cmu.cpvv.f16.i16s  v10 v0 //x
cmu.cpvv.f16.i16s  v11 v2 //y
cmu.cpvv.i16.f16 v12 v10 
cmu.cpvv.i16.f16 v13 v11

vau.sub.f16 v16 v0 v12  //frac x
vau.sub.f16 v29 v2 v13  //frac y 
nop 
vau.sub.f16 v21 v23  v16 || lsu1.swzv8 [22222222]  //1-frac x
vau.sub.f16  v7 v23  v29 || lsu1.swzv8 [22222222] //1-frac y





vau.mul.i16 v11 v23 v11  || lsu1.swzv8 [44444444] 
nop 2
vau.mul.i16 v10 v23 v10  || lsu1.swzv8 [66666666] 
nop 2
vau.add.i16 v10 v11 v10   //first row of pixels
nop 
vau.add.i16 v11 v23 v10  || lsu1.swzv8 [44444444]    //second row of pixels
nop 

cmu.cpvv.i16.i32 v12 v10
cmu.alignvec v10 v10 v10 8
cmu.cpvv.i16.i32 v13 v11
cmu.alignvec v11 v11 v11 8
cmu.cpvv.i16.i32 v10 v10
cmu.cpvv.i16.i32 v11 v11
vau.add.i32  v12 v12 v5
vau.add.i32  v10 v10 v5 //first
vau.add.i32  v13 v13 v5
vau.add.i32 v11 v11 v5 //second


lsu0.cp  i2  v12.0 || lsu1.cp i15  v10.0  
lsu0.cp i21  v12.1 || lsu1.cp  i5  v10.1  
lsu0.cp  i8  v12.2 || lsu1.cp i12  v10.2  
lsu0.cp  i9  v12.3 || lsu1.cp i20  v10.3  

lsu1.cp  i3  v13.0 || cmu.cpvi  i16  v11.0	
lsu1.cp i22  v13.1 || cmu.cpvi  i27  v11.1	
lsu1.cp i28  v13.2 || cmu.cpvi  i29  v11.2 
lsu1.cp i13  v13.3 || cmu.cpvi  i26  v11.3  


                        
LSU0.LD.128.u8.f16   v12  i2   	 
LSU0.LD.128.u8.f16   v10 i21 	 
LSU0.LD.128.u8.f16   v13  i8     
LSU0.LD.128.u8.f16   v11  i9     
nop 6

cmu.cpvi  i2 v12.0 
cmu.cpvi i21 v12.1 || lsu1.cp v4.0   i2
cmu.cpvi  i8 v12.2 || lsu1.cp v6.0  i21
cmu.cpvi  i2 v10.0 || lsu1.cp v30.0  i8
cmu.cpvi i21 v10.1 || lsu1.cp v4.1   i2
cmu.cpvi  i8 v10.2 || lsu1.cp v6.1  i21
cmu.cpvi  i2 v13.0 || lsu1.cp v30.1  i8
cmu.cpvi i21 v13.1 || lsu1.cp v4.2   i2
cmu.cpvi  i8 v13.2 || lsu1.cp v6.2  i21
cmu.cpvi  i2 v11.0 || lsu1.cp v30.2  i8
cmu.cpvi i21 v11.1 || lsu1.cp v4.3   i2
cmu.cpvi  i8 v11.2 || lsu1.cp v6.3  i21
                      lsu1.cp v30.3  i8


					  
LSU0.LD.128.u8.f16   v12 i15   	 
LSU0.LD.128.u8.f16   v10  i5 	 
LSU0.LD.128.u8.f16   v13 i12     
LSU0.LD.128.u8.f16   v11 i20     
nop 6

cmu.cpvi i15 v12.0 
cmu.cpvi  i5 v12.1 || lsu1.cp v31.0  i15
cmu.cpvi i12 v12.2 || lsu1.cp v17.0   i5
cmu.cpvi i15 v10.0 || lsu1.cp v18.0  i12
cmu.cpvi  i5 v10.1 || lsu1.cp v31.1  i15
cmu.cpvi i12 v10.2 || lsu1.cp v17.1   i5
cmu.cpvi i15 v13.0 || lsu1.cp v18.1  i12
cmu.cpvi  i5 v13.1 || lsu1.cp v31.2  i15
cmu.cpvi i12 v13.2 || lsu1.cp v17.2   i5
cmu.cpvi i15 v11.0 || lsu1.cp v18.2  i12
cmu.cpvi  i5 v11.1 || lsu1.cp v31.3  i15
cmu.cpvi i12 v11.2 || lsu1.cp v17.3   i5
                      lsu1.cp v18.3  i12


LSU0.LD.128.u8.f16   v12  i3   	 
LSU0.LD.128.u8.f16   v10 i22 	 
LSU0.LD.128.u8.f16   v13 i28     
LSU0.LD.128.u8.f16   v11 i13     
nop 6

cmu.cpvi  i3 v12.0 
cmu.cpvi i22 v12.1 || lsu1.cp v19.0  i3
cmu.cpvi i28 v12.2 || lsu1.cp v20.0 i22
cmu.cpvi  i3 v10.0 || lsu1.cp v8.0  i28
cmu.cpvi i22 v10.1 || lsu1.cp v19.1  i3
cmu.cpvi i28 v10.2 || lsu1.cp v20.1 i22
cmu.cpvi  i3 v13.0 || lsu1.cp v8.1  i28
cmu.cpvi i22 v13.1 || lsu1.cp v19.2  i3
cmu.cpvi i28 v13.2 || lsu1.cp v20.2 i22
cmu.cpvi  i3 v11.0 || lsu1.cp v8.2  i28
cmu.cpvi i22 v11.1 || lsu1.cp v19.3  i3
cmu.cpvi i28 v11.2 || lsu1.cp v20.3 i22
                      lsu1.cp v8.3  i28
   


LSU0.LD.128.u8.f16   v12 i16   	 
LSU0.LD.128.u8.f16   v10 i27 	 
LSU0.LD.128.u8.f16   v13 i29     
LSU0.LD.128.u8.f16   v11 i26     
nop 6

cmu.cpvi  i2 v12.0 
cmu.cpvi i21 v12.1
cmu.cpvi  i8 v12.2
cmu.cpvi  i9 v10.0
cmu.cpvi i15 v10.1
cmu.cpvi  i5 v10.2
cmu.cpvi i12 v13.0
cmu.cpvi i20 v13.1
cmu.cpvi  i3 v13.2
cmu.cpvi i22 v11.0
cmu.cpvi i28 v11.1
cmu.cpvi i13 v11.2
lsu1.cp v9.0  i2                      
lsu1.cp v12.0 i21                                                   
lsu1.cp v13.0   i8         
lsu1.cp v9.1  i9         
lsu1.cp v12.1 i15
lsu1.cp v13.1   i5
lsu1.cp v9.2 i12
lsu1.cp v12.2 i20
lsu1.cp v13.2   i3
lsu1.cp v9.3 i22
lsu1.cp v12.3 i28
lsu1.cp v13.3  i13



CMU.VDILV.x16 v31 v4 v4 v31
CMU.VDILV.x16 v17 v6 v6 v17
CMU.VDILV.x16 v18 v30 v30 v18
CMU.VDILV.x16 v9 v19 v19 v9
CMU.VDILV.x16 v12 v20 v20 v12
CMU.VDILV.x16 v13 v8 v8 v13 



vau.mul.f16 v4 v4 v21
vau.mul.f16 v17 v17 v16
vau.mul.f16 v19 v19 v21
vau.mul.f16 v12 v12 v16
nop 				 
vau.add.f16 v4 v17 v4
vau.add.f16 v19 v12 v19
nop 
vau.mul.f16 v4 v4 v7
vau.mul.f16 v19 v19 v29
nop 2
vau.add.f16 v19 v19 v4 //R
nop 2



vau.mul.f16 v31 v31 v21
vau.mul.f16 v30 v30 v16
vau.mul.f16 v9 v9 v21
vau.mul.f16 v8 v8 v16
nop 				 
vau.add.f16 v31 v30 v31
vau.add.f16 v9 v8 v9
nop 
vau.mul.f16 v31 v31 v7
vau.mul.f16 v9 v9 v29
nop 2
vau.add.f16 v9 v9 v31 //G
nop 2



vau.mul.f16 v6 v6 v21
vau.mul.f16 v18 v18 v16
vau.mul.f16 v20 v20 v21
vau.mul.f16 v13 v13 v16
nop 				 
vau.add.f16 v6 v18 v6
vau.add.f16 v20 v13 v20
nop 
vau.mul.f16 v6 v6 v7
vau.mul.f16 v20 v20 v29
nop 2
vau.add.f16 v20 v20 v6 //B
nop 2

 vau.xor v6 v6 v6
 CMU.VILV.x16 v19 v9 v9  v19
 CMU.VILV.x16 v20 v6  v6 v20
 CMU.VILV.x32  v19 v20   v20 v19
 CMU.VILV.x32 v9  v6  v6 v9



 cmu.cpvv  v4 v19  || LSU0.SWZC8 [65421033] 
 cmu.cpvv  v31 v20 || LSU0.SWZC8 [65423333] 
 cmu.cpvv  v17 v9  || LSU0.SWZC8 [65334210] 
 cmu.cpvv  v18 v6  || LSU0.SWZC8 [33654210] 


 
 cmu.alignvec v19 v4 v20 4                 
 cmu.alignvec v9 v31 v17 8                 
 cmu.alignvec v20 v17 v18 12                

// bru.rpim 0
//nop 10



LSU1.STI.128.f16.u8 v19 i17
LSU1.STI.128.f16.u8 v9  i17
LSU1.STI.128.f16.u8 v20 i17


//************************************************
cmu.cpvv.f16.i16s  v10 v1 //x
cmu.cpvv.f16.i16s  v11 v3 //y
cmu.cpvv.i16.f16 v12 v10 
cmu.cpvv.i16.f16 v13 v11

vau.sub.f16 v16 v1 v12  //frac x
vau.sub.f16 v29 v3 v13  //frac y 
nop 
vau.sub.f16 v21 v23  v16 || lsu1.swzv8 [22222222]  //1-frac x
vau.sub.f16  v7 v23  v29 || lsu1.swzv8 [22222222] //1-frac y





vau.mul.i16 v11 v23 v11  || lsu1.swzv8 [44444444] 
nop 2
vau.mul.i16 v10 v23 v10  || lsu1.swzv8 [66666666] 
nop 2
vau.add.i16 v10 v11 v10   //first row of pixels
nop 
vau.add.i16 v11 v23 v10  || lsu1.swzv8 [44444444]    //second row of pixels
nop 

cmu.cpvv.i16.i32 v12 v10
cmu.alignvec v10 v10 v10 8
cmu.cpvv.i16.i32 v13 v11
cmu.alignvec v11 v11 v11 8
cmu.cpvv.i16.i32 v10 v10
cmu.cpvv.i16.i32 v11 v11
vau.add.i32  v12 v12 v5
vau.add.i32  v10 v10 v5 //first
vau.add.i32  v13 v13 v5
vau.add.i32 v11 v11 v5 //second


lsu0.cp  i2  v12.0 || lsu1.cp i15  v10.0  
lsu0.cp i21  v12.1 || lsu1.cp  i5  v10.1  
lsu0.cp  i8  v12.2 || lsu1.cp i12  v10.2  
lsu0.cp  i9  v12.3 || lsu1.cp i20  v10.3  

lsu1.cp  i3  v13.0 || cmu.cpvi  i16  v11.0	
lsu1.cp i22  v13.1 || cmu.cpvi  i27  v11.1	
lsu1.cp i28  v13.2 || cmu.cpvi  i29  v11.2 
lsu1.cp i13  v13.3 || cmu.cpvi  i26  v11.3  


                        
LSU0.LD.128.u8.f16   v12  i2   	 
LSU0.LD.128.u8.f16   v10 i21 	 
LSU0.LD.128.u8.f16   v13  i8     
LSU0.LD.128.u8.f16   v11  i9     
nop 6

cmu.cpvi  i2 v12.0 
cmu.cpvi i21 v12.1 || lsu1.cp v4.0   i2
cmu.cpvi  i8 v12.2 || lsu1.cp v6.0  i21
cmu.cpvi  i2 v10.0 || lsu1.cp v30.0  i8
cmu.cpvi i21 v10.1 || lsu1.cp v4.1   i2
cmu.cpvi  i8 v10.2 || lsu1.cp v6.1  i21
cmu.cpvi  i2 v13.0 || lsu1.cp v30.1  i8
cmu.cpvi i21 v13.1 || lsu1.cp v4.2   i2
cmu.cpvi  i8 v13.2 || lsu1.cp v6.2  i21
cmu.cpvi  i2 v11.0 || lsu1.cp v30.2  i8
cmu.cpvi i21 v11.1 || lsu1.cp v4.3   i2
cmu.cpvi  i8 v11.2 || lsu1.cp v6.3  i21
                      lsu1.cp v30.3  i8


					  
LSU0.LD.128.u8.f16   v12 i15   	 
LSU0.LD.128.u8.f16   v10  i5 	 
LSU0.LD.128.u8.f16   v13 i12     
LSU0.LD.128.u8.f16   v11 i20     
nop 6

cmu.cpvi i15 v12.0 
cmu.cpvi  i5 v12.1 || lsu1.cp v31.0  i15
cmu.cpvi i12 v12.2 || lsu1.cp v17.0   i5
cmu.cpvi i15 v10.0 || lsu1.cp v18.0  i12
cmu.cpvi  i5 v10.1 || lsu1.cp v31.1  i15
cmu.cpvi i12 v10.2 || lsu1.cp v17.1   i5
cmu.cpvi i15 v13.0 || lsu1.cp v18.1  i12
cmu.cpvi  i5 v13.1 || lsu1.cp v31.2  i15
cmu.cpvi i12 v13.2 || lsu1.cp v17.2   i5
cmu.cpvi i15 v11.0 || lsu1.cp v18.2  i12
cmu.cpvi  i5 v11.1 || lsu1.cp v31.3  i15
cmu.cpvi i12 v11.2 || lsu1.cp v17.3   i5
                      lsu1.cp v18.3  i12


LSU0.LD.128.u8.f16   v12  i3   	 
LSU0.LD.128.u8.f16   v10 i22 	 
LSU0.LD.128.u8.f16   v13 i28     
LSU0.LD.128.u8.f16   v11 i13     
nop 6

cmu.cpvi  i3 v12.0 
cmu.cpvi i22 v12.1 || lsu1.cp v19.0  i3
cmu.cpvi i28 v12.2 || lsu1.cp v20.0 i22
cmu.cpvi  i3 v10.0 || lsu1.cp v8.0  i28
cmu.cpvi i22 v10.1 || lsu1.cp v19.1  i3
cmu.cpvi i28 v10.2 || lsu1.cp v20.1 i22
cmu.cpvi  i3 v13.0 || lsu1.cp v8.1  i28
cmu.cpvi i22 v13.1 || lsu1.cp v19.2  i3
cmu.cpvi i28 v13.2 || lsu1.cp v20.2 i22
cmu.cpvi  i3 v11.0 || lsu1.cp v8.2  i28
cmu.cpvi i22 v11.1 || lsu1.cp v19.3  i3
cmu.cpvi i28 v11.2 || lsu1.cp v20.3 i22
                      lsu1.cp v8.3  i28
   


LSU0.LD.128.u8.f16   v12 i16   	 
LSU0.LD.128.u8.f16   v10 i27 	 
LSU0.LD.128.u8.f16   v13 i29     
LSU0.LD.128.u8.f16   v11 i26     
nop 6

cmu.cpvi  i2 v12.0 
cmu.cpvi i21 v12.1
cmu.cpvi  i8 v12.2
cmu.cpvi  i9 v10.0
cmu.cpvi i15 v10.1
cmu.cpvi  i5 v10.2
cmu.cpvi i12 v13.0
cmu.cpvi i20 v13.1
cmu.cpvi  i3 v13.2
cmu.cpvi i22 v11.0
cmu.cpvi i28 v11.1
cmu.cpvi i13 v11.2
lsu1.cp v9.0  i2                      
lsu1.cp v12.0 i21                                                   
lsu1.cp v13.0   i8         
lsu1.cp v9.1  i9         
lsu1.cp v12.1 i15
lsu1.cp v13.1   i5
lsu1.cp v9.2 i12
lsu1.cp v12.2 i20
lsu1.cp v13.2   i3
lsu1.cp v9.3 i22
lsu1.cp v12.3 i28
lsu1.cp v13.3  i13



CMU.VDILV.x16 v31 v4 v4 v31
CMU.VDILV.x16 v17 v6 v6 v17
CMU.VDILV.x16 v18 v30 v30 v18
CMU.VDILV.x16 v9 v19 v19 v9
CMU.VDILV.x16 v12 v20 v20 v12
CMU.VDILV.x16 v13 v8 v8 v13 



vau.mul.f16 v4 v4 v21
vau.mul.f16 v17 v17 v16
vau.mul.f16 v19 v19 v21
vau.mul.f16 v12 v12 v16
nop 				 
vau.add.f16 v4 v17 v4
vau.add.f16 v19 v12 v19
nop 
vau.mul.f16 v4 v4 v7
vau.mul.f16 v19 v19 v29
nop 2
vau.add.f16 v19 v19 v4 //R
nop 2



vau.mul.f16 v31 v31 v21
vau.mul.f16 v30 v30 v16
vau.mul.f16 v9 v9 v21
vau.mul.f16 v8 v8 v16
nop 				 
vau.add.f16 v31 v30 v31
vau.add.f16 v9 v8 v9
nop 
vau.mul.f16 v31 v31 v7
vau.mul.f16 v9 v9 v29
nop 2
vau.add.f16 v9 v9 v31 //G
nop 2



vau.mul.f16 v6 v6 v21
vau.mul.f16 v18 v18 v16
vau.mul.f16 v20 v20 v21
vau.mul.f16 v13 v13 v16
nop 				 
vau.add.f16 v6 v18 v6
vau.add.f16 v20 v13 v20
nop 
vau.mul.f16 v6 v6 v7
vau.mul.f16 v20 v20 v29
nop 2
vau.add.f16 v20 v20 v6 //B
nop 2

 vau.xor v6 v6 v6
 CMU.VILV.x16 v19 v9 v9  v19
 CMU.VILV.x16 v20 v6  v6 v20
 CMU.VILV.x32  v19 v20   v20 v19
 CMU.VILV.x32 v9  v6  v6 v9



 cmu.cpvv  v4 v19  || LSU0.SWZC8 [65421033] 
 cmu.cpvv  v31 v20 || LSU0.SWZC8 [65423333] 
 cmu.cpvv  v17 v9  || LSU0.SWZC8 [65334210] 
 cmu.cpvv  v18 v6  || LSU0.SWZC8 [33654210] 


 
 cmu.alignvec v19 v4 v20 4                 
 cmu.alignvec v9 v31 v17 8                 
 cmu.alignvec v20 v17 v18 12                

// bru.rpim 0
//nop 10



LSU1.STI.128.f16.u8 v19 i17
LSU1.STI.128.f16.u8 v9  i17
LSU1.STI.128.f16.u8 v20 i17



//*******************************************
tile_sampler_loop:
nop 7

//
//IAU.ADD i26 i18 i23 //y
//IAU.ADD i27 i18 i25 //y
//IAU.ADD i28 i26 i14 //y+1
//IAU.ADD i29 i27 i14 //y+1
//


//bru.rpim 0
//nop 10


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

	LSU0.LD.32  i31  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i30  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i29  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i28  i19 || IAU.ADD i19 i19 4	
	LSU0.LD.32  i27  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i26  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i25  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i24  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i23  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i22  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i21  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i20  i19 || IAU.ADD i19 i19 4
	nop 6

BRU.jmp i30
nop 6

.size mvcvtile_samplerRGB888_asm,.-mvcvtile_samplerRGB888_asm
.end
