///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvtile_sampleru10_asm,@function
.version 00.51.04

.data .data.tile_sampleru10
.align 4
 tile_sampler_indx:
		.float16    16, 15, 14, 13, 12, 11, 10, 9, 8,  7,  6,  5,  4,  3,  2,  1
		.float16     0,  1,  2,  3,  4,  5,  6, 7, 8,  9, 10, 11, 12, 13, 14, 15, 0.0625

.code .text.tile_sampleru10
//void mvcvtile_sampleru10(unsigned short *tile, unsigned short * out_tile, float *xcoord, float *ycoord, unsigned int tile_width, unsigned int tile_height)
//                                  i18                      i17               i16           i15                 i14                     i13

mvcvtile_sampleru10_asm:
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
iau.shl i14 i14 1
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
vau.add.f16 v20 v16 v18 || cmu.cpivr.x32  v5 i18
vau.add.f16 v21 v17 v19
vau.mul.f16 v25 v12 v23 || lsu0.ldil i0 0x10 || lsu1.ldih i0 0x10   //ycoord[0] * (mesh_gran_x - j) + ycoord[1] * j
vau.mul.f16 v26 v13 v23 || lsu0.ldil i6 0x0  || lsu1.ldih i6 0x0
vau.mul.f16 v27 v20 v23 || CMU.CPII.i16.f16 i0 i0 //(ycoord[2] * (mesh_gran_x - j) + ycoord[3] * j
vau.mul.f16 v28 v21 v23 || CMU.CPII.i16.f16 i6 i6 || lsu0.ldil i7 tile_sampler_loop  || lsu1.ldih i7 tile_sampler_loop


cmu.cpivr.x32 v2 i0
cmu.cpivr.x32 v3 i6
vau.mul.f16 v8 v2 v14 
vau.mul.f16 v9 v2 v15 
vau.mul.f16 v0 v3 v22 
vau.mul.f16 v1 v3 v24 
sau.sub.f16 i0 i0 i1
vau.add.f16 v8 v8 v0
vau.add.f16 v9 v9 v1  || cmu.cpiv.x32 v23.1 i1
sau.add.f16 i6 i6 i1  || cmu.cpiv.x32 v23.2 i31
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
vau.shl.x16 v10,v10,1
nop
vau.add.i16 v10 v11 v10   //first row of pixels
nop 
vau.add.i16 v11 v23 v10  || lsu1.swzv8 [44444444]    //second row of pixels
nop 


cmu.cpvv.i16.i32 v12 v10 || bru.rps i7 16
cmu.alignvec v10 v10 v10 8
cmu.cpvv.i16.i32 v13 v11
cmu.alignvec v11 v11 v11 8
cmu.cpvv.i16.i32 v10 v10
cmu.cpvv.i16.i32 v11 v11
vau.add.i32  v12 v12 v5  || cmu.cpvv.f16.i16s  v4 v1 //x
vau.add.i32  v10 v10 v5  || cmu.cpvv.f16.i16s  v6 v3 //y //first
vau.add.i32  v13 v13 v5  || cmu.cpvv.i16.f16 v30 v4 
vau.add.i32 v11 v11 v5   || cmu.cpvv.i16.f16 v31 v6//second


cmu.cpvi  i3  v13.0 || lsu0.cp  i2  v12.0 || lsu1.cp i15  v10.0   || vau.sub.f16 v17 v1 v30  //frac x
cmu.cpvi i22  v13.1 || lsu0.cp i21  v12.1 || lsu1.cp  i5  v10.1   || vau.sub.f16 v18 v3 v31  //frac y 
cmu.cpvi i28  v13.2 || lsu0.cp  i8  v12.2 || lsu1.cp i12  v10.2  
cmu.cpvi i13  v13.3 || lsu0.cp  i9  v12.3 || lsu1.cp i20  v10.3   || vau.shl.x16 v4,v4,1           
LSU0.LD.64.l   v12  i2   	|| cmu.cpvi  i16  v11.0	          || vau.sub.f16 v19 v23  v17 || lsu1.swzv8 [22222222] //1-frac x                
LSU0.LD.64.l    v12 i21 	|| cmu.cpvi  i27  v11.1	          || vau.sub.f16 v20 v23  v18 || lsu1.swzv8 [22222222] //1-frac y
LSU0.LD.64.l    v12  i8     || cmu.cpvi  i29  v11.2           || vau.mul.i16 v6 v23 v6    || lsu1.swzv8 [44444444]
LSU0.LD.64.l    v12  i9     || cmu.cpvi  i26  v11.3          
LSU0.LD.64.l    v10 i15                                       || cmu.cpivr.x32 v2 i0
LSU0.LD.64.l    v10  i5                                       || vau.add.i16 v4 v6 v4   //first row of pixels
LSU0.LD.64.l    v10 i12                                       || cmu.cpivr.x32 v3 i6
LSU0.LD.64.l    v10 i20     || cmu.cpvi   i2  v12.0           || vau.add.i16 v6 v23 v4  || lsu1.swzv8 [44444444]  //second row of pixels
LSU0.LD.64.l    v13  i3     || lsu1.cp  i21 v12.0                           || cmu.cpvv.i16.i32 v30 v4 || vau.mul.f16 v8 v2 v14 
LSU0.LD.64.l    v13 i22     || lsu1.cp   i8 v12.0                           || cmu.alignvec v4 v4 v4 8
LSU0.LD.64.l    v13 i28     || lsu1.cp   i9 v12.0                           || cmu.cpvv.i16.i32 v31 v6 || vau.add.i32  v30 v30 v5
LSU0.LD.64.l    v13 i13  	||                        lsu1.cp  i15 v10.0    || cmu.alignvec v6 v6 v6 8
LSU0.LD.64.l    v10 i16     ||                        lsu1.cp   i5 v10.0    || cmu.cpvv.i16.i32 v4 v4 || vau.add.i32  v31 v31 v5
LSU0.LD.64.l    v10 i27     ||                        lsu1.cp  i12 v10.0    || cmu.cpvv.i16.i32 v6 v6 || vau.add.i32  v4 v4 v5 //first
LSU0.LD.64.l    v10 i29     ||                        lsu1.cp  i23 v10.0    || cmu.cpvi  i3  v31.0 
LSU0.LD.64.l    v10 i26     ||                        cmu.cpvi i24 v13.0    || lsu1.cp v12.0  i2      || vau.add.i32 v6 v6 v5 //second                    
                                                      lsu0.cp  i25 v13.0    || lsu1.cp v12.1 i21      || cmu.cpvi  i2  v30.0 || vau.mul.f16 v9 v2 v15 
                                                      lsu0.cp  i30 v13.0    || lsu1.cp v12.2  i8      || cmu.cpvi i22  v31.1 
                                                      lsu0.cp  i31 v13.0    || lsu1.cp v12.3  i9      || cmu.cpvi i21  v30.1	
cmu.cpvi i16 v10.0 || lsu1.cp i28  v31.2                                                        || vau.mul.f16 v0 v3 v22 
cmu.cpvi i27 v10.0 || lsu1.cp i13  v31.3                                                        || vau.mul.f16 v1 v3 v24 
cmu.cpvi i29 v10.0 || lsu1.cp v13.0 i15                                                         || sau.sub.f16 i0 i0 i1
cmu.cpvi i26 v10.0 || lsu1.cp i15  v4.0      || lsu0.cp v13.1  i5	                            || vau.add.f16 v8 v8 v0
lsu0.cp v10.0 i24  || cmu.cpiv.x32 v11.0 i16 || lsu1.cp  i5  v4.1                               || vau.add.f16 v9 v9 v1
lsu0.cp v10.1 i25  || cmu.cpiv.x32 v11.1 i27 || lsu1.cp v13.2 i12		                        || sau.add.f16 i6 i6 i1
lsu0.cp v10.2 i30  || cmu.cpiv.x32 v11.2 i29 || lsu1.cp  i8  v30.2 		  
lsu0.cp v10.3 i31  || cmu.cpiv.x32 v11.3 i26 || lsu1.cp  i9  v30.3 
lsu0.cp v13.3 i23  || lsu1.cp i12  v4.2      || cmu.cpvi i20  v4.3   

cmu.cpvv.i16.f16 v13 v13	 
cmu.cpvv.i16.f16 v12 v12	 
cmu.cpvv.i16.f16 v10 v10	 
cmu.cpvv.i16.f16 v11 v11
nop

LSU0.LD.64.l    v30  i2   	|| cmu.cpvi  i16  v6.0	|| vau.mul.f16 v0 v23 v8 || lsu1.swzv8 [00000000] 
LSU0.LD.64.l    v30 i21 	|| cmu.cpvi  i27  v6.1	|| vau.mul.f16 v1 v23 v9 || lsu1.swzv8 [00000000] 
LSU0.LD.64.l    v30  i8     || cmu.cpvi  i29  v6.2                           || vau.mul.f16 v8 v2 v25 
LSU0.LD.64.l    v30  i9     || cmu.cpvi  i26  v6.3                           || vau.mul.f16 v9 v2 v26 
LSU0.LD.64.l    v4 i15      || CMU.VDILV.x16 v13 v12 v12 v13                 || vau.mul.f16 v2 v3 v27 
LSU0.LD.64.l    v4  i5      || CMU.VDILV.x16 v11 v10 v10 v11 || vau.mul.f16 v12 v12 v21    
LSU0.LD.64.l    v4 i12                                       || vau.mul.f16 v13 v13 v16    
LSU0.LD.64.l    v4 i20      || cmu.cpvi   i2  v30.0          || vau.mul.f16 v10 v10 v21
LSU0.LD.64.l    v31  i3     || cmu.cpvi  i21 v30.0           || vau.mul.f16 v11 v11 v16
LSU0.LD.64.l    v31 i22     || cmu.cpvi   i8 v30.0                           || vau.mul.f16 v3 v3 v28 
LSU0.LD.64.l    v31 i28     || cmu.cpvi   i9 v30.0                           || vau.add.f16 v12 v13 v12
LSU0.LD.64.l    v31 i13  	||                        lsu1.cp  i15 v4.0      || vau.add.f16 v10 v11 v10
LSU0.LD.64.l    v4 i16      ||                        lsu1.cp   i5 v4.0      || vau.add.f16 v8 v8 v2
LSU0.LD.64.l    v4 i27      ||                        lsu1.cp  i12 v4.0      || vau.mul.f16 v12 v12 v7    
LSU0.LD.64.l    v4 i29      ||                        lsu1.cp  i20 v4.0      || vau.mul.f16 v10 v10 v29
LSU0.LD.64.l    v4 i26      ||  cmu.cpvi  i3 v31.0  ||lsu1.cp v30.0  i2      || vau.add.f16 v9 v9 v3
                                    cmu.cpvi i22 v31.0  || lsu1.cp v30.1 i21
                                    cmu.cpvi i28 v31.0  || lsu1.cp v30.2  i8 || vau.add.f16 v10 v10 v12
                                    cmu.cpvi i13 v31.0  || lsu1.cp v30.3  i9	
lsu1.cp v31.0 i15   || lsu0.cp i16 v4.0  
lsu1.cp v31.1  i5	|| lsu0.cp i27 v4.0  || cmu.cpvv.i16.f16 v30 v30
lsu1.cp v31.2 i12	|| lsu0.cp i29 v4.0  || cmu.cpvv.f16.i16s v10 v10
lsu1.cp v31.3 i20	|| lsu0.cp i26 v4.0  


lsu0.cp v4.0  i3 || cmu.cpiv.x32 v6.0 i16       || vau.mul.f16 v2 v23 v8 || lsu1.swzv8 [00000000]              
lsu0.cp v4.1 i22 || cmu.cpiv.x32 v6.1 i27		|| vau.mul.f16 v3 v23 v9 || lsu1.swzv8 [00000000]   
lsu0.cp v4.2 i28 || cmu.cpiv.x32  v6.2 i29      || lsu1.sti.64.l v10,i17 		    
lsu0.cp v4.3 i13 || cmu.cpiv.x32  v6.3 i26      || lsu1.sti.64.h v10,i17 

	 
cmu.cpvv.i16.f16 v31 v31	 
cmu.cpvv.i16.f16 v6 v6	 
cmu.cpvv.i16.f16 v4 v4
nop 	    
CMU.VDILV.x16 v31 v30 v30 v31
CMU.VDILV.x16 v6 v4 v4 v6


vau.mul.f16 v30 v30 v19 || cmu.cpvv.f16.i16s  v10 v0 //x 
vau.mul.f16 v31 v31 v17 || cmu.cpvv.f16.i16s  v11 v2 //y
vau.mul.f16 v4 v4 v19   ||  cmu.cpvv.i16.f16 v12 v10  
vau.mul.f16 v6 v6 v17   || cmu.cpvv.i16.f16 v13 v11
vau.sub.f16 v16 v0 v12  //frac x
vau.sub.f16 v29 v2 v13  //frac y 
vau.add.f16 v30 v31 v30
vau.add.f16 v4 v6 v4
vau.sub.f16 v21 v23  v16 || lsu1.swzv8 [22222222]  //1-frac x
vau.sub.f16  v7 v23  v29 || lsu1.swzv8 [22222222] //1-frac y
vau.mul.f16 v30 v30 v20
vau.mul.f16 v4 v4 v18
vau.mul.i16 v11 v23 v11  || lsu1.swzv8 [44444444] 
nop
tile_sampler_loop:
.nowarn 10
vau.add.f16 v4 v4 v30
.nowarnend
nop
vau.shl.x16 v10,v10,1   
cmu.cpvv.f16.i16s v4 v4
vau.add.i16 v10 v11 v10    //first row of pixels
lsu0.sti.64.l v4,i17
vau.add.i16 v11 v23 v10 || lsu1.swzv8 [44444444]  || lsu0.sti.64.h v4,i17  //second row of pixels



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

.size mvcvtile_sampleru10_asm,.-mvcvtile_sampleru10_asm
.end
