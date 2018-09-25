///
/// @file      genLumaFp16.asm
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Extract Luma channel from RGB input data
///
//-------------------------------------------------------------------------------
.type mvispGenLumaFp16_asm,@function
.version 00.51.05
//-------------------------------------------------------------------------------
//C reference:
//                     i18,            i17,        i16,        i15,       i14,         i13              
//void genLumaFp16(half *outY, UInt16* inR, UInt16 *inG, UInt16 *inB, float normVal, int width)
//{
//    int i//
//
//    for(i=0// i<width// i++) 
//    {
//        //outY[i] = (inR[i]*0.25f + inG[i]*0.50f + inB[i]*0.25f)//
//        outY[i] = (half)( (float)(inR[i] + inG[i] + inG[i] + inB[i])*(normVal*0.25f))//
//    }
//}
//------------------------------------------------------------------------------- 
// Parameters
// i18 : *outY
// i17 : *inR
// i16 : *inG
// i15 : *inB
// i14 : normVal
// i13 : width
//-------------------------------------------------------------------------------

.data .data.genLumaFp16_asm
.align 4

.code .text.genLumaFp16_asm   
//########################################
.lalign 
mvispGenLumaFp16_asm:
//########################################
.nowarn
//########################################
// pre processing
    IAU.XOR i12 i12 i12
    IAU.ADD i12 i12 i13
    lsu0.ldil i3, 0x0000    ||lsu1.ldih i3, 0x3e80 //0.25 in fp 32 = 0x3e800000
    lsu0.ldo.64.l v0, i17, 0x00 ||lsu1.ldo.64.h v0, i17, 0x08 ||iau.incs i17, 0x10
    lsu0.ldo.64.l v1, i16, 0x00 ||lsu1.ldo.64.h v1, i16, 0x08 ||iau.incs i16, 0x10
    lsu0.ldo.64.l v2, i15, 0x00 ||lsu1.ldo.64.h v2, i15, 0x08 ||iau.incs i15, 0x10
    lsu0.ldo.64.l v0, i17, 0x00 ||lsu1.ldo.64.h v0, i17, 0x08 ||iau.incs i17, 0x10
    lsu0.ldo.64.l v1, i16, 0x00 ||lsu1.ldo.64.h v1, i16, 0x08 ||iau.incs i16, 0x10
    lsu0.ldo.64.l v2, i15, 0x00 ||lsu1.ldo.64.h v2, i15, 0x08 ||iau.incs i15, 0x10
    sau.mul.f32 i3, i14, i3
    lsu0.ldo.64.l v0, i17, 0x00 ||lsu1.ldo.64.h v0, i17, 0x08 ||iau.incs i17, 0x10
    VAU.IADDS.u16 v3, v1, v1    ||lsu0.ldo.64.l v1, i16, 0x00 ||lsu1.ldo.64.h v1, i16, 0x08 ||iau.incs i16, 0x10
    VAU.IADDS.u16 v4, v0, v2    || lsu0.ldo.64.l v2, i15, 0x00 ||lsu1.ldo.64.h v2, i15, 0x08 ||iau.incs i15, 0x10
    lsu0.ldil i1, 0x00                     
    VAU.IADDS.u16 v5, v3, v4    ||cmu.cpivr.x32 v22, i3
    VAU.IADDS.u16 v3, v1, v1 ||lsu0.ldo.64.l v0, i17, 0x00 ||lsu1.ldo.64.h v0, i17, 0x08 ||iau.incs i17, 0x10 
    VAU.IADDS.u16 v4, v0, v2   ||lsu0.ldo.64.l v1, i16, 0x00 ||lsu1.ldo.64.h v1, i16, 0x08 ||iau.incs i16, 0x10
    lsu0.ldo.64.l v2, i15, 0x00 ||lsu1.ldo.64.h v2, i15, 0x08 ||iau.incs i15, 0x10  ||cmu.cpivr.x16 v23, i1
    cmu.vilv.x16 v6, v7, v23, v5 ||VAU.IADDS.u16 v5, v3, v4 
    cmu.cpvv.i32.f32 v8, v6      ||VAU.IADDS.u16 v3, v1, v1 
    cmu.cpvv.i32.f32 v9, v7      ||VAU.IADDS.u16 v4, v0, v2 ||iau.incs i17, -16
    vau.mul.f32 v10, v8, v22    ||iau.add i13, i13, 7     
    vau.mul.f32 v11, v9, v22    ||iau.shr.u32 i13, i13, 3 
    IAU.SUBSU i13, i13, 2    

//start loop
.lalign 
start_loop_genLumaFp16_asm:
    PEU.PCIX.NEQ 0x00    || bru.bra start_loop_genLumaFp16_asm  ||iau.incs i17, 0x10
    cmu.vilv.x16 v6, v7, v23, v5 ||VAU.IADDS.u16 v5, v3, v4 ||lsu0.ldo.64.l v0, i17, 0x00 ||lsu1.ldo.64.h v0, i17, 0x08 
    cmu.cpvv.i32.f32 v8, v6      ||VAU.IADDS.u16 v3, v1, v1 ||lsu0.ldo.64.l v1, i16, 0x00 ||lsu1.ldo.64.h v1, i16, 0x08 
    cmu.cpvv.i32.f32 v9, v7      ||VAU.IADDS.u16 v4, v0, v2 ||lsu0.ldo.64.l v2, i15, 0x00 ||lsu1.ldo.64.h v2, i15, 0x08 
    vau.mul.f32 v10, v8, v22     ||lsu1.sti.128.f32.f16 v10, i18    ||iau.incs i16, 0x10
    vau.mul.f32 v11, v9, v22     ||lsu1.sti.128.f32.f16 v11, i18    ||iau.incs i15, 0x10
    IAU.SUBSU i13, i13, 1

    
mvispGenLumaFp16_compensate:
    //exception when width=8
    IAU.XOR i11 i11 i11
    IAU.ADD i11 i11 8
    CMU.CMII.u32 i11 i12
    PEU.PCXC EQ,0 || BRU.JMP i30
    nop 6

    lsu1.sti.128.f32.f16 v10, i18
    lsu1.sti.128.f32.f16 v11, i18

    BRU.JMP i30
    nop 6
    nop 6
   
.size mvispGenLumaFp16_asm,.-mvispGenLumaFp16_asm
.nowarnend
   .end
