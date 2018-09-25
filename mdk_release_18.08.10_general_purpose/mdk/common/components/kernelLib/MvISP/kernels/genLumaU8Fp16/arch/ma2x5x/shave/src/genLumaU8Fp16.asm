/////
///// @file genLumaU8Fp16.h
///// @copyright All code copyright Movidius Ltd 2012, all rights reserved
/////            For License Warranty see: common/license.txt
/////
///// @brief     Generate Luma from RGB short tint (U8) input data in Fp16 output data
/////
//-------------------------------------------------------------------------------
.type mvispGenLumaU8Fp16_asm,@function
.version 00.51.05
// void genLumaU8Fp16(u8* inR, u8 *inG, u8 *inB, half *outY, u32 width)
// {
    // int i//

    // for(i=0// i<width// i++) 
    // {
      // outY[i] = (inR[i]*0.25f + inG[i]*0.50f + inB[i]*0.25f) * (half)(1.0f/255.0f)//
    // }
// }
.data .rodata.genLumaU8Fp16_asm
.align 16
 genLumaU8Fp16_coef1:
	.float16 1.0 255.0 0 0
 genLumaU8Fp16_coef2:
	.float16 0.25 0.5 0 0

.code .text.genLumaU8Fp16_asm   
//########################################
mvispGenLumaU8Fp16_asm:
//void GenLumaU8Fp16(u8* inR, u8 *inG, u8 *inB, half *outY, half* coefs, u32 width)
//                       i18      i17      i16        i15          i14        i13        
//void genLumaU8Fp16(u8* inR, u8 *inG, u8 *inB, half *outY, u32 width)
//					   i18      i17      i16        i15      i14
.nowarn
    lsu0.ldi.16r v21 i14
    lsu0.ldi.16r v20 i14
    lsu0.ldi.16r v19 i14
    LSU0.LDIL i10 0x001F          ||  LSU1.LDIH i10  0x0
    LSU0.LDIL i12 genLumaU8Fp16_loop         ||  LSU1.LDIH i12 genLumaU8Fp16_loop
    LSU0.LDIL i9 genLumaU8Fp16_loop2        ||  LSU1.LDIH i9 genLumaU8Fp16_loop2
    LSU0.LDIL i11 genLumaU8Fp16_compensate   ||  LSU1.LDIH i11 genLumaU8Fp16_compensate
    LSU0.LDIL i2  0x3c00 		  ||  LSU1.LDIH i2   0          || iau.shr.u32 i8 i13 4
    LSU0.LDIL i3  0x5bf8		  ||  LSU1.LDIH i3   0          || iau.and i7 i13 i10
    sau.div.f16 i4 i2 i3

    cmu.cmz.i32 i8
    peu.pc1c eq || bru.jmp i11
    nop 6

    lsu0.ldi.128.u8.f16 v0 i18 
    lsu0.ldi.128.u8.f16 v1 i17
    lsu0.ldi.128.u8.f16 v2 i16
    lsu0.ldi.128.u8.f16 v3 i18
    lsu0.ldi.128.u8.f16 v4 i17
    lsu0.ldi.128.u8.f16 v5 i16
    nop 
    cmu.cpivr.x16 v22 i4
    vau.mul.f16 v6 v0 v21 
    vau.mul.f16 v7 v1 v20      || lsu0.ldi.128.u8.f16 v0 i18 
    vau.mul.f16 v8 v2 v19      || lsu0.ldi.128.u8.f16 v1 i17 
    vau.mul.f16 v9  v3 v21     || lsu0.ldi.128.u8.f16 v2 i16 
    vau.add.f16 v12 v6 v7     
    vau.mul.f16 v10 v4 v20    
    vau.mul.f16 v11 v5 v19     || lsu0.ldi.128.u8.f16 v3 i18    || bru.rpl i12 i8 _end_delay_loop_genLuma
    vau.add.f16 v13 v12 v8     || lsu0.ldi.128.u8.f16 v4 i17
    vau.add.f16 v15 v9 v10     || lsu0.ldi.128.u8.f16 v5 i16
    vau.mul.f16 v6 v0 v21      || lsu0.ldi.128.u8.f16 v0 i18  
    vau.mul.f16 v14 v13 v22    || lsu0.ldi.128.u8.f16 v1 i17 

.lalign
genLumaU8Fp16_loop:                  
    vau.add.f16 v16 v15 v11    || lsu0.ldi.128.u8.f16 v2 i16 
    vau.mul.f16 v7 v1 v20     
    vau.mul.f16 v8 v2 v19     
    vau.mul.f16 v17 v16 v22   
    vau.mul.f16 v9  v3 v21    
    vau.add.f16 v12 v6 v7      || lsu0.sto.64.l v14 i15 0       || lsu1.sto.64.h v14 i15 8  
    vau.mul.f16 v10 v4 v20     || lsu0.sto.64.l v17 i15 16      || lsu1.sto.64.h v17 i15 24  || iau.add i15 i15 32
    .lalign
_end_delay_loop_genLuma: 
 
    cmu.cmz.i32 i7
    peu.pc1c eq || bru.jmp i30
    nop 6

    iau.sub i18 i18 24 
    iau.sub i17 i17 24
    iau.sub i16 i16 24
  
genLumaU8Fp16_compensate: 
    //exception when width=8
    IAU.XOR i1 i1 i1
    IAU.ADD i1 i1 8
    nop 2
    CMU.CMII.u32 i13 i1
    PEU.PCXC EQ,0 || iau.shr.u32 i7 i7 3
    PEU.PCXC LT,0 || iau.shr.u32 i7 i7 4  
    PEU.PCXC GT,0 || iau.shr.u32 i7 i7 4  

    //exception when width=16
    IAU.ADD i1 i1 8
    nop 2
    CMU.CMII.u32 i13 i1
    PEU.PCXC EQ,0 || BRU.JMP i30 
    nop 6
    
    
    cmu.cpivr.x16 v22 i4
    lsu0.ldi.128.u8.f16 v0 i18 || bru.rpl i9 i7
    lsu0.ldi.128.u8.f16 v1 i17
    lsu0.ldi.128.u8.f16 v2 i16
    nop 4
    vau.mul.f16 v6 v0 v21
    vau.mul.f16 v7 v1 v20
    vau.mul.f16 v8 v2 v19
    nop 
    vau.add.f16 v12 v6 v7
    nop 2
    .lalign
genLumaU8Fp16_loop2:
    vau.add.f16 v13 v12 v8    || bru.jmp i30 
    nop 2
    vau.mul.f16 v14 v13 v22 
    nop 2
    lsu0.sto.64.l v14 i15 0   || lsu1.sto.64.h v14 i15 8  || iau.add i15 i15 16

.size mvispGenLumaU8Fp16_asm,.-mvispGenLumaU8Fp16_asm
.nowarnend
.end

