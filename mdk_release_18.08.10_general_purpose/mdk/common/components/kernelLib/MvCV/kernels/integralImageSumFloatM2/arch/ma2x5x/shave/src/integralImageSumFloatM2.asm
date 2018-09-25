///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvIntegralimage_sum_f32_M2_asm,@function
.version 00.51.04

.data .data.integralimage_sum_f32_M2
.align 4
.nowarn
.code .text.integralimage_sum_f32_M2

mvcvIntegralimage_sum_f32_M2_asm:


cmu.cmz.i32 i16  || LSU0.LDIL i16 IntegralimageFM2_second || LSU1.LDIH i16 IntegralimageFM2_second
peu.pc1c neq || bru.jmp i16
nop 6


IAU.SUB i19 i19 4
LSU0.ST.32  i20  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i21  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i22  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i23  i19 || IAU.SUB i19 i19 4 
LSU0.ST.32  i24  i19

lsu0.ld.128.u8.u16 v0 i17 || iau.add i17 i17 4 
lsu0.ld.128.u8.u16 v1 i17 || iau.add i17 i17 4 
nop 5
cmu.cpvv.i16.i32 v0 v0  || iau.sub i0 i0 i0  || lsu1.st.32 i18 i15 
cmu.cpvv.i16.i32 v1 v1  || lsu0.ldil i23 IntegralimageFM2_first_loop || lsu1.ldih i23 IntegralimageFM2_first_loop || iau.shr.i32 i14 i14 3

lsu0.cp i1 v0.0  || lsu1.ld.128.u8.u16 v0 i17 || iau.add i17 i17 4 
lsu0.cp i2 v0.1  || lsu1.ld.128.u8.u16 v1 i17 || iau.add i17 i17 4
lsu0.cp i3 v0.2
lsu0.cp i4 v0.3
lsu0.cp i5 v1.0 
lsu0.cp i6 v1.1  
lsu0.cp i7 v1.2     
lsu0.cp i8 v1.3     || cmu.cpvv.i16.i32 v0 v0

				   iau.add i9 i1 i0    || cmu.cpvv.i16.i32 v1 v1   || bru.rpl i23 i14
lsu0.cp i1 v0.0 || iau.add i10 i9 i2   || cmu.cpiv v3.0 i9         || lsu1.ld.128.u8.u16 v0 i17 || sau.add.i32 i17 i17 4 
lsu0.cp i2 v0.1 || iau.add i11 i10 i3  || cmu.cpiv v3.1 i10    
lsu0.cp i3 v0.2 || iau.add i12 i11 i4  || cmu.cpiv v3.2 i11	        || lsu1.ld.128.u8.u16 v1 i17 || sau.add.i32 i17 i17 4
lsu0.cp i4 v0.3 || iau.add i13 i12 i5  || cmu.cpiv v3.3 i12
lsu0.cp i5 v1.0 || iau.add i20 i13 i6  || cmu.cpiv v4.0 i13
IntegralimageFM2_first_loop:
lsu0.cp i6 v1.1 || iau.add i21 i20 i7  || cmu.cpiv v4.1 i20
lsu0.cp i7 v1.2 || iau.add i22 i21 i8  || cmu.cpiv v4.2 i21 
lsu0.cp i8 v1.3 || iau.sub i0 i22 0    || cmu.cpiv v4.3 i22
										  cmu.cpvv.i32.f32 v3 v3    
										  cmu.cpvv.i32.f32 v4 v4    
lsu0.sto.64.l v3 i18 0    || lsu1.sto.64.h v3 i18 8   
lsu0.sto.64.l v4 i18 16   || lsu1.sto.64.h v4 i18 24  || iau.add i18 i18 32  || cmu.cpvv.i16.i32 v0 v0

LSU0.LD.32  i24  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i23  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i22  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i21  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i20  i19 || IAU.ADD i19 i19 4

BRU.jmp i30
nop 6

IntegralimageFM2_second:
IAU.SUB i19 i19 4
LSU0.ST.32  i20  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i21  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i22  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i23  i19 || IAU.SUB i19 i19 4 
LSU0.ST.32  i24  i19


nop 6


  
lsu0.ldil i23 IntegralimageFM2_second_loop || lsu1.ldih i23 IntegralimageFM2_second_loop || iau.shr.i32 i14 i14 3
nop 6
lsu0.ld.128.u8.u16 v0 i17 || iau.add i17 i17 4 || lsu1.ld.32  i24 i15 
lsu0.ld.128.u8.u16 v1 i17 || iau.add i17 i17 4 || lsu1.st.32  i18 i15
nop 5
cmu.cpvv.i16.i32 v0 v0    || iau.sub i0 i0 i0
cmu.cpvv.i16.i32 v1 v1 

lsu0.cp i1 v0.0  || lsu1.ld.128.u8.u16 v0 i17 || iau.add i17 i17 4  
lsu0.cp i2 v0.1  || lsu1.ld.128.u8.u16 v1 i17 || iau.add i17 i17 4
lsu0.cp i3 v0.2  
lsu0.cp i4 v0.3  
lsu0.cp i5 v1.0  || lsu1.ldo.64.l v5 i24 0	 
lsu0.cp i6 v1.1  || lsu1.ldo.64.h v5 i24 8   
lsu0.cp i7 v1.2  || lsu1.ldo.64.l v6 i24 16
lsu0.cp i8 v1.3  || lsu1.ldo.64.h v6 i24 24    || iau.add i24 i24 32  || cmu.cpvv.i16.i32 v0 v0  
					iau.add i9 i1 i0     || cmu.cpvv.i16.i32 v1 v1 || lsu1.ld.128.u8.u16 v0 i17 || sau.add.i32 i17 i17 4 
lsu0.cp i1 v0.0  || iau.add i10 i9 i2    || cmu.cpiv v3.0 i9 
     
lsu0.cp i2 v0.1  || iau.add i11 i10 i3   || cmu.cpiv v3.1 i10   || bru.rpl i23 i14 
lsu0.cp i3 v0.2  || iau.add i12 i11 i4   || cmu.cpiv v3.2 i11   
lsu0.cp i4 v0.3  || iau.add i13 i12 i5   || cmu.cpiv v3.3 i12  || lsu1.ld.128.u8.u16 v1 i17 || sau.add.i32 i17 i17 4      
lsu0.cp i5 v1.0  || iau.add i20 i13 i6   || cmu.cpiv v4.0 i13  || lsu1.ldo.64.l v5 i24 0	      
lsu0.cp i6 v1.1  || iau.add i21 i20 i7   || cmu.cpiv v4.1 i20  || lsu1.ldo.64.h v5 i24 8        
lsu0.cp i7 v1.2  || iau.add i22 i21 i8   || cmu.cpiv v4.2 i21  || lsu1.ldo.64.l v6 i24 16      
lsu0.cp i8 v1.3  || iau.sub i0 i22 0     || cmu.cpiv v4.3 i22  || lsu1.ldo.64.h v6 i24 24     
IntegralimageFM2_second_loop:
cmu.cpvv.i32.f32 v3 v3    || iau.add i24 i24 32   
cmu.cpvv.i32.f32 v4 v4    || iau.add i9 i1 i0
							 iau.add i10 i9 i2  || vau.add.f32 v7 v3 v5   
vau.add.f32 v8 v4 v6      || iau.add i11 i10 i3 || cmu.cpvv.i16.i32 v0 v0   || lsu1.ld.128.u8.u16 v0 i17
lsu0.cp i1 v0.0			  || lsu1.cp i2 v0.1    || cmu.cpvv.i16.i32 v1 v1   || iau.add i17 i17 4 
lsu0.sto.64.l v7 i18 0    || lsu1.sto.64.h v7 i18 8   || cmu.cpiv v3.0 i9
lsu0.sto.64.l v8 i18 16   || lsu1.sto.64.h v8 i18 24  || iau.add i18 i18 32  || cmu.cpiv v3.1 i10  


LSU0.LD.32  i24  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i23  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i22  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i21  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i20  i19 || IAU.ADD i19 i19 4

BRU.jmp i30
nop 6

.size mvcvIntegralimage_sum_f32_M2_asm,.-mvcvIntegralimage_sum_f32_M2_asm
.nowarnend
.end
