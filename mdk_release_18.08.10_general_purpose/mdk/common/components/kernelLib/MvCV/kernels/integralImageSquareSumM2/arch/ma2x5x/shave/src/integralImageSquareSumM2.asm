///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///



.type mvcvIntegralimage_sqsum_u32_M2_asm,@function
.version 00.51.04


.code .text.mvcvIntegralimage_sqsum_u32_M2
mvcvIntegralimage_sqsum_u32_M2_asm:
//void mvcvIntegralimage_sqsum_u32(u32* out, u8* in, u32 runNr, u32** prevLnPointer, u32 width)
//                                   i18       i17      i16           i15               i14  
.nowarn
cmu.cmz.i32 i16 || lsu0.ldil i16 IntegralimageSq_M2_second_line || lsu1.ldih i16 IntegralimageSq_M2_second_line 
peu.pc1c gt || BRU.jmp i16 
nop 6

//------------------------------------------------------------------------------------------------------
IAU.SUB i19 i19 4
LSU0.ST.32  i20  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i21  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i22  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i23  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i24  i19 

lsu0.ld.128.u8.u16 v0 i17 || iau.add i17 i17 4 
lsu0.ld.128.u8.u16 v1 i17 || iau.add i17 i17 4 || lsu1.st.32 i18 i15 
nop 5
cmu.cpvv.i16.i32 v0 v0 ||  iau.sub i0 i0 i0   
cmu.cpvv.i16.i32 v1 v1 

lsu0.cp i1 v0.0  || lsu1.ld.128.u8.u16 v0 i17 || iau.add i17 i17 4  
lsu0.cp i2 v0.1  || lsu1.ld.128.u8.u16 v1 i17 || iau.add i17 i17 4 
lsu0.cp i3 v0.2  || lsu1.ldil i23 IntegralimageSq_M2_first_loop 
lsu0.cp i4 v0.3  || lsu1.ldih i23 IntegralimageSq_M2_first_loop || iau.shr.i32 i14 i14 3
lsu0.cp i5 v1.0 
lsu0.cp i6 v1.1  
lsu0.cp i7 v1.2
lsu0.cp i8 v1.3	  || cmu.cpvv.i16.i32 v0 v0 	 

lsu0.cp i1 v0.0 || iau.mul i9 i1 i1   || cmu.cpvv.i16.i32 v1 v1    
lsu0.cp i2 v0.1 || iau.mul i10 i2 i2  || lsu1.ld.128.u8.u16 v0 i17 || sau.add.i32 i17 i17 4
.lalign
lsu0.cp i3 v0.2 || iau.mul i11 i3 i3  || bru.rpl i23 i14
lsu0.cp i4 v0.3 || iau.mul i12 i4 i4  || lsu1.ld.128.u8.u16 v1 i17 || sau.add.i32 i17 i17 4
lsu0.cp i5 v1.0 || iau.mul i13 i5 i5 
lsu0.cp i6 v1.1 || iau.mul i20 i6 i6 
lsu0.cp i7 v1.2 || iau.mul i21 i7 i7 
lsu0.cp i8 v1.3	|| iau.mul i22 i8 i8 
nop
iau.add i9 i9 i0     || cmu.cpvv.i16.i32 v0 v0 
iau.add i10 i10 i9   || cmu.cpiv v3.0 i9 	
iau.add i11 i11 i10  || cmu.cpiv v3.1 i10 
iau.add i12 i12 i11  || cmu.cpiv v3.2 i11 
IntegralimageSq_M2_first_loop:
iau.add i13 i13 i12  || cmu.cpiv v3.3 i12 
iau.add i20 i20 i13  || cmu.cpiv v4.0 i13 || lsu0.sto.64.l v3 i18 0  || lsu1.sto.64.h v3 i18 8 || sau.add.i32 i18 i18 16
iau.add i21 i21 i20  || cmu.cpiv v4.1 i20 
iau.add i22 i22 i21  || cmu.cpiv v4.2 i21 || lsu1.ld.128.u8.u16 v0 i17 || sau.add.i32 i17 i17 4
iau.sub i0  i22 0    || cmu.cpiv v4.3 i22 
lsu0.cp i1 v0.0      || iau.mul i9 i1 i1    || lsu1.sto.64.l v4 i18 0 || cmu.cpvv.i16.i32 v1 v1
lsu0.cp i2 v0.1      || iau.mul i10 i2 i2   || lsu1.sto.64.h v4 i18 8 || sau.add.i32 i18 i18 16
nop
LSU0.LD.32  i24  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i23  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i22  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i21  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i20  i19 || IAU.ADD i19 i19 4

BRU.jmp i30
nop 6
//------------------------------------------------------------------------------------------------------
IntegralimageSq_M2_second_line:
IAU.SUB i19 i19 4
LSU0.ST.32  i20  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i21  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i22  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i23  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i24  i19 


lsu0.ld.128.u8.u16 v0 i17 || iau.add i17 i17 4 || lsu1.ld.32 i24 i15 
lsu0.ld.128.u8.u16 v1 i17 || iau.add i17 i17 4 || lsu1.st.32 i18 i15 
nop 5
cmu.cpvv.i16.i32 v0 v0    || iau.sub i0 i0 i0  || lsu0.ldil i23 IntegralimageSq_M2_second_loop 
cmu.cpvv.i16.i32 v1 v1    || lsu1.ldih i23 IntegralimageSq_M2_second_loop || iau.shr.i32 i14 i14 3
lsu0.cp i1 v0.0  || lsu1.ld.128.u8.u16 v0 i17 || iau.add i17 i17 4   
lsu0.cp i2 v0.1  || lsu1.ld.128.u8.u16 v1 i17 || iau.add i17 i17 4 
lsu0.cp i3 v0.2  || lsu1.ldo.64.l v5 i24 0
lsu0.cp i4 v0.3  || lsu1.ldo.64.h v5 i24 8
lsu0.cp i5 v1.0  || lsu1.ldo.64.l v6 i24 16
lsu0.cp i6 v1.1  || lsu1.ldo.64.h v6 i24 24 || iau.add i24 i24 32
lsu0.cp i7 v1.2
lsu0.cp i8 v1.3	|| iau.mul i9 i1 i1   || cmu.cpvv.i16.i32 v0 v0 		 
lsu0.cp i1 v0.0 || iau.mul i10 i2 i2  || cmu.cpvv.i16.i32 v1 v1  
lsu0.cp i2 v0.1 || iau.mul i11 i3 i3 
lsu0.cp i3 v0.2 || iau.mul i12 i4 i4 
.lalign
lsu0.cp i4 v0.3 || iau.mul i13 i5 i5  || bru.rpl i23 i14 
lsu0.cp i5 v1.0 || iau.mul i20 i6 i6 
lsu0.cp i6 v1.1 || iau.mul i21 i7 i7 
lsu0.cp i7 v1.2 || iau.mul i22 i8 i8  
nop
lsu0.cp i8 v1.3 || iau.add i9 i9 i0   
iau.add i10 i10 i9   || cmu.cpiv v3.0 i9 	|| lsu1.ld.128.u8.u16 v0 i17 || sau.add.i32 i17 i17 4  
iau.add i11 i11 i10  || cmu.cpiv v3.1 i10   || lsu1.ldo.64.l v5 i24 0
iau.add i12 i12 i11  || cmu.cpiv v3.2 i11   || lsu1.ld.128.u8.u16 v1 i17 || sau.add.i32 i17 i17 4 
iau.add i13 i13 i12  || cmu.cpiv v3.3 i12   || lsu1.ldo.64.h v5 i24 8    || sau.add.i32 i24 i24 16 
iau.add i20 i20 i13  || cmu.cpiv v4.0 i13 || vau.add.i32 v7 v3 v5
IntegralimageSq_M2_second_loop:
iau.add i21 i21 i20  || cmu.cpiv v4.1 i20 || lsu1.ldo.64.l v6 i24 0
iau.add i22 i22 i21  || cmu.cpiv v4.2 i21 || lsu1.ldo.64.h v6 i24 8 || sau.add.i32 i24 i24 16 
iau.sub i0  i22 0    || cmu.cpiv v4.3 i22 || lsu0.sto.64.l v7 i18 0 || lsu1.sto.64.h v7 i18 8 || sau.add.i32 i18 i18 16
vau.add.i32 v8 v4 v6 || cmu.cpvv.i16.i32 v0 v0 || iau.mul i9 i1 i1  //
lsu0.cp i1 v0.0      || cmu.cpvv.i16.i32 v1 v1 || iau.mul i10 i2 i2 
lsu0.cp i2 v0.1      || iau.mul i11 i3 i3      || lsu1.sto.64.l v8 i18 0 
lsu0.cp i3 v0.2      || iau.mul i12 i4 i4      || lsu1.sto.64.h v8 i18 8 || sau.add.i32 i18 i18 16
nop
LSU0.LD.32  i24  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i23  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i22  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i21  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i20  i19 || IAU.ADD i19 i19 4//
//------------------------------------------------------------------------------------------------------
BRU.jmp i30
nop 6
.nowarnend
.size mvcvIntegralimage_sqsum_u32_M2_asm,.-mvcvIntegralimage_sqsum_u32_M2_asm
.end
