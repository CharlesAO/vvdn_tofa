//-------------------------------------------------------------------------------
.version 00.51.05
//-------------------------------------------------------------------------------
//C reference:
//void genLuma(UInt8* inR, UInt8 *inG, UInt8 *inB, UInt8 *out, int width)
//{
//   int i//
//    for(i=0// i<width// i++)
//    {
//      out[i] = (inR[i]*0.25f + inG[i]*0.50f + inB[i]*0.25f)//
//    }
//}
//-------------------------------------------------------------------------------
// Parameters
// i18 : *inR
// i17 : *inG
// i16 : *inB
// i15 : *outY
// i14 : width
//-------------------------------------------------------------------------------

.code .text.genLumaOpt
.salign	 16

//########################################
genLumaOpt:
//########################################

//Initial loads
 lsu0.ld.128.u8.u16 v0, i18 //load R
 lsu0.ld.128.u8.u16 v2, i16 //load B
 lsu1.ld.128.u8.u16 v1, i17 //load G

 iau.incs i18, 8
 iau.incs i17, 8
 iau.incs i16, 8
 nop
 nop

//########################################
.lalign
 genLumaOpt_genLumaOptLoop:
//########################################

vau.add.i16 v3, v0, v2     || lsu0.ld.128.u8.u16 v0, i18 || iau.incs i14, -8
vau.add.i16 v4, v1, v1     || lsu0.ld.128.u8.u16 v2, i16 || peu.pcix.neq 0x00 || bru.bra genLumaOpt_genLumaOptLoop
                              lsu0.ld.128.u8.u16 v1, i17
vau.add.i16 v3, v3, v4     || iau.incs i18, 8
                              iau.incs i16, 8
vau.shr.u16 v3, v3,  2     || iau.incs i17, 8
nop
lsu0.st.128.u16.u8 v3, i15 || iau.incs i15, 8 //store Luma


//#################
//The return:
//#################
 bru.jmp i30
 nop 6
