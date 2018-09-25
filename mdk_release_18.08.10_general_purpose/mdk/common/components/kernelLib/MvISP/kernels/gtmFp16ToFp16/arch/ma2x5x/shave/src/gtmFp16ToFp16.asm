///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief 
///

//--------------------------------------------------------------------------------------------------------------
.type mvispGtmFp16ToFp16_asm,@function
.version 00.50.37.8
.data .rodata.gtmFp16ToFp16_asm 
.align 4
//.align 16

.code .text.gtmFp16ToFp16_asm //text
.lalign
mvispGtmFp16ToFp16_asm:
// //                             i18          i17         i16          i15         i14
// void mvispGtmFp16ToFp16(half* iline, half* output, u32 width, half* scurve, int nPoints)
// {
   // int index//
   // u32 i//

   // for (i = 0// i < width// i++)
   // {
       // index = (int)((float)iline[i] * (nPoints - 1))//
       // output[i] = scurve[index]//
   // }
// }
lsu0.ldil i8 mvispGtmFp16ToFp16_asm_loop || lsu1.ldih i8 mvispGtmFp16ToFp16_asm_loop 
iau.shr.i32 i16 i16 3
iau.sub i14 i14 1
cmu.cpivr.x32 v2 i14
cmu.cpivr.x32 v0 i15 //address of curve
CMU.CPVV.i32.f32 v3 v2 //npoints -1
nop
LSU0.LDi.128.f16.f32 v1 i18 
LSU0.LDi.128.f16.f32 v7 i18 
nop 5
vau.mul.f32 v4 v1 v3 || LSU0.LDi.128.f16.f32 v1 i18
vau.mul.f32 v8 v7 v3 || LSU0.LDi.128.f16.f32 v7 i18
nop 
cmu.cpvv.f32.i32s v5 v4 || LSU0.LDi.128.f16.f32 v1 i18	
cmu.cpvv.f32.i32s v9 v8 || LSU0.LDi.128.f16.f32 v7 i18	
nop
vau.add.i32 v6 v5 v0
vau.add.i32 v10 v9 v0
vau.mul.f32 v4 v1 v3 											 
vau.mul.f32 v8 v7 v3
nop
vau.add.i32 v6 v5 v6
vau.add.i32 v10 v9 v10
nop 	
lsu0.cp      i0 v6.0    || bru.rpl i8 i16                                                 	
cmu.cpvi.x32 i1 v6.1    || lsu0.ld.16 i4 i0                              	
cmu.cpvi.x32 i2 v6.2    || lsu0.ld.16 i5 i1                              
lsu1.cp      i3 v6.3    || lsu0.ld.16 i6 i2                              || cmu.cpvv.f32.i32s v5 v4 
lsu1.cp      i8 v10.0   || lsu0.ld.16 i7 i3                              || cmu.cpvv.f32.i32s v9 v8 
lsu1.cp      i9 v10.1   || lsu0.ld.16 i12 i8                             									
cmu.cpvi.x32 i10 v10.2  || lsu0.ld.16 i13 i9                             
cmu.cpvi.x32 i11 v10.3  || lsu0.ld.16 i14 i10                            || vau.add.i32 v6 v5 v0
                           lsu0.ld.16 i15 i11 || lsu1.sti.16 i4 i17      || vau.add.i32 v10 v9 v0
						   mvispGtmFp16ToFp16_asm_loop:
                                                 lsu1.sti.16 i5 i17      || vau.mul.f32 v4 v1 v3 	|| LSU0.LDi.128.f16.f32 v1 i18
                                                 lsu1.sti.16 i6 i17      || vau.mul.f32 v8 v7 v3    || LSU0.LDi.128.f16.f32 v7 i18
                                                 lsu1.sti.16 i7 i17      
                                                 lsu1.sti.16 i12 i17     || vau.add.i32 v6 v5 v6
                                                 lsu1.sti.16 i13 i17     || vau.add.i32 v10 v9 v10
                                                 lsu1.sti.16 i14 i17
                                                 lsu1.sti.16 i15 i17
	
bru.jmp i30
nop 6

.size mvispGtmFp16ToFp16_asm,.-mvispGtmFp16ToFp16_asm
.end
	
	 
		

	
	
