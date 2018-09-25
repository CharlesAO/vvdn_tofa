/////
///// @file      
///// @copyright All code copyright Movidius Ltd 2012, all rights reserved
/////            For License Warranty see: common/license.txt
/////
///// @brief     Extract Chroma differences CrCgCb from (debayerd) RGB image
/////
//-------------------------------------------------------------------------------
.type mvispGenChromaFp16_asm,@function
.version 00.51.05
//                           i18,                   i17,                  i16,       i15,        i14,          i13 
//void genChromaFp16(unsigned char* outC[3], unsigned short* inRGB[3], half *inY, float eps, float normVal, unsigned int width)
//{
//  unsigned short *inR   = inRGB[0]//
//  unsigned short *inG   = inRGB[1]//
//  unsigned short *inB   = inRGB[2]//
//  
//  unsigned char  *outCR = outC[0]//
//  unsigned char  *outCG = outC[1]//
//  unsigned char  *outCB = outC[2]//
//  
//  unsigned int i//
//  half ooLuma//
//  
//  half epsHalf = (half)eps//
//  half normValHalf = (half)normVal//
//
//  for(i=0// i<width// i++)
//  {  //div oo_luma by extra amount, to scale from input #bits to 8bit output
//      half inRGB[3]//
//      half ooLumaDiv//
//
//      ooLuma   =  (inY[i] + epsHalf)//
//      inRGB[0] = inR[i]//
//      inRGB[1] = inG[i]//
//      inRGB[2] = inB[i]//
//      ooLumaDiv = normValHalf/ooLuma//
//
//      outCR[i] =  clampU8(inRGB[0] * ooLumaDiv)//
//      outCG[i] =  clampU8(inRGB[1] * ooLumaDiv)//
//      outCB[i] =  clampU8(inRGB[2] * ooLumaDiv)//
//  }
//}
//------------------------------------------------------------------------------- 


.data .data.genLumaFp16_asm
.align 4

.code .text.genLumaFp16_asm   
//########################################
.lalign 
mvispGenChromaFp16_asm:
.nowarn 9 12 11 40 10
lsu0.ldo.64.l v0, i16, 0x00 ||lsu1.ldo.64.h v0, i16, 0x08   ||iau.incs i16, 0x10
lsu0.ldo.32 i5, i17, 0x00   ||cmu.cpii.f32.f16 i15, i15     ||LSU1.LDIL i12, __end_genChromaFp16_loop
lsu0.ldo.32 i6, i17, 0x04   ||cmu.cpii.f32.f16 i14, i14     ||LSU1.LDIH i12, __end_genChromaFp16_loop 
lsu0.ldo.32 i7, i17, 0x08   ||cmu.cpivr.x16 v23, i15        ||iau.add i13, i13, 7 
lsu0.ldo.32  i9, i18, 0x00  ||cmu.cpivr.x16 v22, i14        ||iau.shr.u32 i13, i13, 3
lsu0.ldo.32 i10, i18, 0x04  ||lsu1.cp i14, v22.0
lsu0.ldo.32 i11, i18, 0x08 	
vau.add.f16 v1,v0,v23       ||lsu0.ldo.64.l v0, i16, 0x00   ||lsu1.ldo.64.h v0, i16, 0x08 
iau.incs i16, 0x10
nop 
cmu.cpvi.x32 i0, v1.0
sau.div.f16 i1, i14, i0     ||cmu.cpvi.x32 i0, v1.1
sau.div.f16 i2, i14, i0     ||cmu.cpvi.x32 i0, v1.2
sau.div.f16 i3, i14, i0     ||cmu.cpvi.x32 i0, v1.3
sau.div.f16 i4, i14, i0     ||vau.add.f16 v1,v0,v23   
lsu0.ldo.64.l v3, i5, 0x00  ||lsu1.ldo.64.h v3, i5, 0x08    ||iau.incs i5, 0x10
lsu0.ldo.64.l v0, i16, 0x00 ||lsu1.ldo.64.h v0, i16, 0x08   ||iau.incs i16, 0x10
lsu0.ldo.64.l v4, i6, 0x00  ||lsu1.ldo.64.h v4, i6, 0x08    ||iau.incs i6, 0x10
lsu0.ldo.64.l v5, i7, 0x00  ||lsu1.ldo.64.h v5, i7, 0x08    ||iau.incs i7, 0x10
lsu0.cp i0, v1.0            ||cmu.cpiv v2.0, i1             ||lsu1.ldo.64.l v0, i16, 0x00   ||IAU.SUBSU i13, i13, 1
sau.div.f16 i1, i14, i0     ||lsu0.cp i0, v1.1              ||cmu.cpiv v2.1, i2             ||PEU.PCIX.EQ 0x00              || bru.bra __end_genChromaFp16_delay_slot
sau.div.f16 i2, i14, i0     ||lsu0.cp i0, v1.2              ||cmu.cpiv v2.2, i3             ||lsu1.ldo.64.h v0, i16, 0x08
sau.div.f16 i3, i14, i0     ||lsu0.cp i0, v1.3              ||cmu.cpiv v2.3, i4             ||iau.incs i16, 0x10 
sau.div.f16 i4, i14, i0     ||vau.add.f16 v1,v0,v23         ||cmu.cpvv.i16.f16 v6, v3
cmu.cpvv.i16.f16 v7, v4     ||lsu0.ldo.64.l v3, i5, 0x00    ||lsu1.ldo.64.h v3, i5, 0x08    ||iau.incs i5, 0x10
cmu.cpvv.i16.f16 v8, v5     ||lsu0.ldo.64.l v5, i7, 0x00    ||lsu1.ldo.64.h v5, i7, 0x08    ||iau.incs i7, 0x10
lsu0.ldo.64.l v4, i6, 0x00  ||lsu1.ldo.64.h v4, i6, 0x08    ||iau.incs i6, 0x10
// start loop
.lalign 
cmu.cpvv v18, v2            ||lsu0.ldo.64.l v3, i5, 0x00    ||lsu1.ldo.64.h v3, i5, 0x08    ||bru.rpl i12, i13, __end_genChromaFp16_delay_slot 
vau.mul.f16 v9 , v6, v18    ||lsu0.cp i0, v1.0              ||cmu.cpiv v2.0, i1             || lsu1.ldo.64.l v0, i16, 0x00    
__end_genChromaFp16_loop:
vau.mul.f16 v10, v7, v18    ||sau.div.f16 i1, i14, i0       ||lsu0.cp i0, v1.1              ||cmu.cpiv v2.1, i2   ||iau.incs i5, 0x10  
vau.mul.f16 v11, v8, v18    ||sau.div.f16 i2, i14, i0       ||lsu0.cp i0, v1.2              ||cmu.cpiv v2.2, i3
sau.div.f16 i3, i14, i0     ||lsu0.cp i0, v1.3              ||cmu.cpiv v2.3, i4             ||LSU1.STI.128.f16.u8  v9 ,  i9
sau.div.f16 i4, i14, i0     ||vau.add.f16 v1,v0,v23         ||cmu.cpvv.i16.f16 v6, v3       ||LSU0.STI.128.f16.u8  v10,  i10
cmu.cpvv.i16.f16 v7, v4     ||lsu0.ldo.64.l v4, i6, 0x00    ||lsu1.ldo.64.h v0, i16, 0x08   ||iau.incs i16, 0x10
cmu.cpvv.i16.f16 v8, v5     ||lsu0.ldo.64.l v5, i7, 0x00    ||lsu1.ldo.64.h v5, i7, 0x08    ||iau.incs i7, 0x10
lsu1.ldo.64.h v4, i6, 0x08  ||iau.incs i6, 0x10             ||LSU0.STI.128.f16.u8  v11,  i11  
// end loop
__end_genChromaFp16_delay_slot:
bru.jmp i30 ||cmu.cpvv v18, v2
vau.mul.f16 v9 , v6, v18 
vau.mul.f16 v10, v7, v18
vau.mul.f16 v11, v8, v18
LSU1.STI.128.f16.u8  v9 ,  i9
LSU0.STI.128.f16.u8  v10,  i10
LSU0.STI.128.f16.u8  v11,  i11  
.nowarnend
.size mvispGenChromaFp16_asm,.-mvispGenChromaFp16_asm
.end

