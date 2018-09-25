///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvIntegralimage_sqsum_f32_M2_asm,@function
.version 00.51.04

.data .data.integralimage_sqsum_f32_M2
.align 4

.code .text.integralimage_sqsum_f32_M2
//void integralimage_sqsum_f32(float* out, u8* in, u32 runNr, float** prev, u32 width)//
//                                  i18      i17       i16         i15           i14
mvcvIntegralimage_sqsum_f32_M2_asm:
.nowarn

cmu.cmz.i32 i16 || lsu0.ldil i16 IntegralimageSqF_M2_second_line || lsu1.ldih i16 IntegralimageSqF_M2_second_line 
peu.pc1c gt || BRU.jmp i16 
nop 6

//------------------------------------------------------------------------------------------------------
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
LSU0.ST.32  i29  i19 || lsu1.st.32 i18 i15 || iau.shr.i32 i14 i14 3
nop 6
 
iau.xor i0 i0 i0 
lsu0.ld.128.u8.u16 v0 i17 || iau.add i17 i17 4 
lsu0.ld.128.u8.u16 v1 i17 || iau.add i17 i17 4 
nop 5
cmu.cpvv.i16.f32 v0 v0
cmu.cpvv.i16.f32 v1 v1 

lsu0.cp i1 v0.0 || lsu1.ld.128.u8.u16 v0 i17 || iau.add i17 i17 4  
lsu0.cp i2 v0.1 || lsu1.ld.128.u8.u16 v1 i17 || iau.add i17 i17 4 
lsu0.cp i3 v0.2  
lsu0.cp i4 v0.3  
lsu0.cp i5 v1.0 
lsu0.cp i6 v1.1  
lsu0.cp i7 v1.2
lsu0.cp i8 v1.3	|| cmu.cpvv.i16.f32 v0 v0 
				   cmu.cpvv.i16.f32 v1 v1 
		           sau.mul.f32 i9 i1 i1   
lsu0.cp i1 v0.0 || sau.mul.f32 i10 i2 i2  
lsu0.cp i2 v0.1 || sau.mul.f32 i11 i3 i3  || lsu1.ld.128.u8.u16 v0 i17 || iau.add i17 i17 4
lsu0.cp i3 v0.2 || sau.mul.f32 i12 i4 i4 
lsu0.cp i4 v0.3 || sau.mul.f32 i13 i5 i5
lsu0.cp i5 v1.0 || sau.mul.f32 i20 i6 i6   || lsu1.ld.128.u8.u16 v1 i17 || iau.add i17 i17 4 
lsu0.cp i6 v1.1 || sau.mul.f32 i21 i7 i7   
lsu0.cp i7 v1.2 || sau.mul.f32 i22 i8 i8   
lsu0.cp i8 v1.3	
sau.add.f32 i23 i9 i0     || cmu.cpvv.i16.f32 v0 v0    
nop 2
sau.add.f32 i24 i10 i23   || cmu.cpiv v3.0 i23  || lsu1.ldil i0 IntegralimageSqF_M2_first_loop				   
nop 2
sau.add.f32 i25 i11 i24  || cmu.cpiv v3.1 i24  || lsu1.ldih i0 IntegralimageSqF_M2_first_loop				    
nop 2 
sau.add.f32 i26 i12 i25  || cmu.cpiv v3.2 i25
nop 2
sau.add.f32 i27 i13 i26  || lsu1.cp v3.3 i26 


		           sau.mul.f32 i9 i1 i1   
lsu0.cp i1 v0.0 || sau.mul.f32 i10 i2 i2 
sau.add.f32 i20 i20 i27  || lsu1.cp v4.0 i27 ||	cmu.cpvv.i16.f32 v1 v1 
lsu0.cp i2 v0.1 || sau.mul.f32 i11 i3 i3  || lsu1.ld.128.u8.u16 v0 i17 || iau.add i17 i17 4
lsu0.cp i3 v0.2 || sau.mul.f32 i12 i4 i4 
sau.add.f32 i21 i21 i20  || lsu1.cp v4.1 i20 
lsu0.cp i4 v0.3 || sau.mul.f32 i13 i5 i5
lsu0.cp i5 v1.0 || sau.mul.f32 i20 i6 i6   || lsu1.ld.128.u8.u16 v1 i17 || iau.add i17 i17 4 
sau.add.f32 i22 i22 i21  || cmu.cpiv v4.2 i21 
lsu0.cp i6 v1.1 || sau.mul.f32 i21 i7 i7   
lsu0.cp i7 v1.2 || sau.mul.f32 i22 i8 i8 || lsu1.cp i8 v1.3
lsu0.cp v4.3 i22  || sau.add.f32 i23 i9 i22     || cmu.cpvv.i16.f32 v0 v0  //|| lsu0.cp i0 i22

lsu0.sto.64.l v3 i18 0 || lsu1.sto.64.h v3 i18 8 || iau.add i18 i18 16 || bru.rpl i0 i14
lsu0.sto.64.l v4 i18 0 || lsu1.sto.64.h v4 i18 8 || iau.add i18 i18 16  
sau.add.f32 i24 i10 i23   || cmu.cpiv v3.0 i23 
nop 2
sau.add.f32 i25 i11 i24  || cmu.cpiv v3.1 i24  
nop 2 
sau.add.f32 i26 i12 i25  || cmu.cpiv v3.2 i25
nop 2
sau.add.f32 i27 i13 i26  || lsu1.cp v3.3 i26 
sau.mul.f32 i9 i1 i1  
lsu0.cp i1 v0.0 || sau.mul.f32 i10 i2 i2 
sau.add.f32 i20 i20 i27  || lsu1.cp v4.0 i27 ||	cmu.cpvv.i16.f32 v1 v1 
lsu0.cp i2 v0.1 || sau.mul.f32 i11 i3 i3  || lsu1.ld.128.u8.u16 v0 i17 || iau.add i17 i17 4
lsu0.cp i3 v0.2 || sau.mul.f32 i12 i4 i4 
IntegralimageSqF_M2_first_loop:
sau.add.f32 i21 i21 i20  || lsu1.cp v4.1 i20
lsu0.cp i4 v0.3 || sau.mul.f32 i13 i5 i5
lsu0.cp i5 v1.0 || sau.mul.f32 i20 i6 i6   || lsu1.ld.128.u8.u16 v1 i17 || iau.add i17 i17 4 
sau.add.f32 i22 i22 i21  || cmu.cpiv v4.2 i21 
lsu0.cp i6 v1.1 || sau.mul.f32 i21 i7 i7   
lsu0.cp i7 v1.2 || sau.mul.f32 i22 i8 i8 || lsu1.cp i8 v1.3
lsu0.cp v4.3 i22  || sau.add.f32 i23 i9 i22     || cmu.cpvv.i16.f32 v0 v0  

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

BRU.jmp i30
nop 6
//------------------------------------------------------------------------------------------------------
IntegralimageSqF_M2_second_line:
IAU.SUB i19 i19 4
LSU0.ST.32  i20  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i21  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i22  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i23  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i24  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i25  i19


lsu1.ld.32 i24 i15
lsu1.st.32 i18 i15

lsu1.ldil i23 IntegralimageSqF_M2_second_loop || iau.shr.i32 i14 i14 3
lsu1.ldih i23 IntegralimageSqF_M2_second_loop || sau.sub.f32 i0 i0 i0   

lsu0.ld.128.u8.u16 v0 i17 || iau.add i17 i17 4 
lsu0.ld.128.u8.u16 v1 i17 || iau.add i17 i17 4 
nop 5
cmu.cpvv.i16.f32 v0 v0
cmu.cpvv.i16.f32 v1 v1 

lsu0.cp i1 v0.0  || lsu1.ld.128.u8.u16 v0 i17 || iau.add i17 i17 4 
lsu0.cp i2 v0.1  || lsu1.ld.128.u8.u16 v1 i17 || iau.add i17 i17 4 
lsu0.cp i3 v0.2  
lsu0.cp i4 v0.3  
lsu0.cp i5 v1.0  || lsu1.ldo.64.l v5 i24 0
lsu0.cp i6 v1.1  || lsu1.ldo.64.h v5 i24 8 || iau.add i24 i24 16 
lsu0.cp i7 v1.2  || lsu1.ldo.64.l v6 i24 0  
lsu0.cp i8 v1.3	 || lsu1.ldo.64.h v6 i24 8 || iau.add i24 i24 16  || cmu.cpvv.i16.f32 v0 v0
 
				    sau.mul.f32 i9 i1 i1   || cmu.cpvv.i16.f32 v1 v1  
lsu0.cp i1 v0.0  || sau.mul.f32 i10 i2 i2  || lsu1.ld.128.u8.u16 v0 i17 || iau.add i17 i17 4 
lsu0.cp i2 v0.1  || sau.mul.f32 i11 i3 i3  
lsu0.cp i3 v0.2  || sau.mul.f32 i12 i4 i4  || lsu1.ld.128.u8.u16 v1 i17 || iau.add i17 i17 4 
lsu0.cp i4 v0.3  || sau.mul.f32 i13 i5 i5  
lsu0.cp i5 v1.0  || sau.mul.f32 i20 i6 i6  
lsu0.cp i6 v1.1  || sau.mul.f32 i21 i7 i7  
lsu0.cp i7 v1.2  || sau.mul.f32 i25 i8 i8  
lsu0.cp i8 v1.3	 || sau.add.f32 i9 i9 i0   || cmu.cpvv.i16.f32 v0 v0  
nop 2
sau.add.f32 i10 i10 i9  || cmu.cpiv v3.0 i9 	
nop 2            
sau.add.f32 i11 i11 i10 || cmu.cpiv v3.1 i10
nop 2                    
sau.add.f32 i12 i12 i11 || cmu.cpiv v3.2 i11 
nop 2
sau.add.f32 i13 i13 i12 || cmu.cpiv v3.3 i12 
sau.mul.f32 i9 i1 i1   || cmu.cpvv.i16.f32 v1 v1  
lsu0.cp i1 v0.0 || sau.mul.f32 i10 i2 i2

sau.add.f32 i20 i20 i13 || cmu.cpiv v4.0 i13  ||  lsu1.ld.128.u8.u16 v0 i17 || iau.add i17 i17 4 
lsu0.cp i2 v0.1  || sau.mul.f32 i11 i3 i3  
lsu0.cp i3 v0.2  || sau.mul.f32 i12 i4 i4 
sau.add.f32 i21 i21 i20 || cmu.cpiv v4.1 i20 
lsu0.cp i4 v0.3  || sau.mul.f32 i13 i5 i5 || lsu1.ldo.64.l v5 i24 0
lsu0.cp i5 v1.0  || sau.mul.f32 i20 i6 i6 || lsu1.ldo.64.h v5 i24 8 || iau.add i24 i24 16 
sau.add.f32 i22 i25 i21 || cmu.cpiv v4.2 i21 || lsu1.ld.128.u8.u16 v1 i17 || iau.add i17 i17 4
lsu0.cp i6 v1.1  || sau.mul.f32 i21 i7 i7  || lsu1.ldo.64.l v6 i24 0  
lsu0.cp i7 v1.2  || sau.mul.f32 i25 i8 i8  || lsu1.ldo.64.h v6 i24 8 || iau.add i24 i24 16                   
lsu0.cp i0 i22   || lsu1.cp v4.3 i22 	 || sau.add.f32 i9 i9 i22   || cmu.cpvv.i16.f32 v0 v0  

vau.add.f32 v7 v3 v5 || lsu0.cp i8 v1.3 || bru.rpl i23 i14
vau.add.f32 v8 v4 v6  
sau.add.f32 i10 i10 i9  || cmu.cpiv v3.0 i9 	
lsu0.sto.64.l v7 i18 0 || lsu1.sto.64.h v7 i18 8 || iau.add i18 i18 16 || sau.mul.f32 i9 i1 i1   || cmu.cpvv.i16.f32 v1 v1 
lsu0.sto.64.l v8 i18 0 || lsu1.sto.64.h v8 i18 8 || iau.add i18 i18 16 || sau.mul.f32 i10 i2 i2  
sau.add.f32 i11 i11 i10 || cmu.cpiv v3.1 i10 || lsu0.cp i1 v0.0 
nop 2                    
sau.add.f32 i12 i12 i11 || cmu.cpiv v3.2 i11 
nop 2
sau.add.f32 i13 i13 i12 || cmu.cpiv v3.3 i12 
nop 2
sau.add.f32 i20 i20 i13 || cmu.cpiv v4.0 i13  ||  lsu1.ld.128.u8.u16 v0 i17 || iau.add i17 i17 4 
lsu0.cp i2 v0.1  || sau.mul.f32 i11 i3 i3  
lsu0.cp i3 v0.2  || sau.mul.f32 i12 i4 i4 
IntegralimageSqF_M2_second_loop:
sau.add.f32 i21 i21 i20 || cmu.cpiv v4.1 i20 
lsu0.cp i4 v0.3  || sau.mul.f32 i13 i5 i5 || lsu1.ldo.64.l v5 i24 0
lsu0.cp i5 v1.0  || sau.mul.f32 i20 i6 i6 || lsu1.ldo.64.h v5 i24 8 || iau.add i24 i24 16 
sau.add.f32 i22 i25 i21 || cmu.cpiv v4.2 i21 || lsu1.ld.128.u8.u16 v1 i17 || iau.add i17 i17 4
lsu0.cp i6 v1.1  || sau.mul.f32 i21 i7 i7  || lsu1.ldo.64.l v6 i24 0  
lsu0.cp i7 v1.2  || sau.mul.f32 i25 i8 i8  || lsu1.ldo.64.h v6 i24 8 || iau.add i24 i24 16                   
lsu0.cp i0 i22   || lsu1.cp v4.3 i22 	 || sau.add.f32 i9 i9 i22   || cmu.cpvv.i16.f32 v0 v0  

LSU0.LD.32  i25  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i24  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i23  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i22  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i21  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i20  i19 || IAU.ADD i19 i19 4//
//------------------------------------------------------------------------------------------------------

BRU.jmp i30
nop 6
.nowarnend
.size mvcvIntegralimage_sqsum_f32_M2_asm,.-mvcvIntegralimage_sqsum_f32_M2_asm
.end

