.type mvcvGaussHx2_asm,@function
.version 00.50.00

//////
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief   : Containd a function that applay downscale 2x horizontal with a gaussian filters with kernel 5x5. 
//                 Have to be use in combination with GaussVx2 for obtain correct output. 
//                 Gaussian 5x5 filter was decomposed in liniar 1x5, applayed horizontal and vertical
//                 Function not make replicate at margin, assumed the you have 3 more pixels on left and right from input buffer.


//FUNCTIONS DECLARATION
// ---------------------------------------------------------------------------
//void GaussHx2(unsigned char *inLine,unsigned char *outLine, int width)l
//  ***************************************************************************


//==================================================================================================================
//=================================== GAUSSIAN DOWNSCALE 2X HORIZONTAL ================START========================
//==================================================================================================================
//                                         i18                      i17            i16  
//void GaussHx2(unsigned char *inLine,unsigned char *outLine, int width)
//{
//    int i//
//    for (i = 0// i<(width<<1)//i+=2)
//    {
//        outLine[i>>1] = (((inLine[i-2] + inLine[i+2]) * gaus_matrix[0]) + ((inLine[i-1] + inLine[i+1]) * gaus_matrix[1]) + (inLine[i]  * gaus_matrix[2]))>>8//
//    }
//}
// gauss horizontal filter
// i18         : unsigned char *inLine //input line
// i17         : unsigned char *outLine //output buffer
// i16        : int width //number of pixels
.code .text.MvCV_GaussHx2
.align 16

.lalign
mvcvGaussHx2_asm:
IAU.SUB i12 i18 0
IAU.ADD i12 i12 16

LSU0.LDI.128.U8.U16 v0, i18      ||LSU1.LDO.16 i3, i18, -2    ||iau.sub i6, i6, i6
LSU0.LDI.128.U8.U16 v1, i18      ||LSU1.LD.128.U8.U16 v20, i12    || IAU.ADD i12 i12 16
LSU0.LDIL i0, 16                ||LSU1.LDIL i1, 64          ||iau.sub i3, i3, i3
LSU1.LDIL i2, 96                ||CMU.CPIVR.x16 v18, i0    
CMU.CPIVR.x16 v17, i1           ||iau.sub i4, i4, i4        ||lsu0.ldil i8, gaussHx2__GaussDownH_Loop ||lsu1.ldih i8, gaussHx2__GaussDownH_Loop
CMU.CPIVR.x16 v16, i2 
iau.sub i5, i5, i5
NOP //myriad2
cmu.vdilv.x16 v5,v4, v0, v1     ||LSU0.LDI.128.U8.U16 v0, i18||LSU1.LDO.16 i3, i18, -2
VAU.MACPZ.u16 v4, v16           ||CMU.cpvv v6 v4        ||IAU.FEXTU i4, i3, 0, 8     ||LSU0.LDI.128.U8.U16 v1, i18     ||LSU1.LD.128.U8.U16 v20, i12
VAU.MACP.u16  v5, v17           ||CMU.SHLIV.x16 V4, v4, i4      ||IAU.FEXTU i5, i3, 8, 8
VAU.MACP.u16  v4, v18           ||CMU.SHLIV.x16 V5, v5, i5 || IAU.ADD i12 i12 16
VAU.MACP.u16  v5, v17           ||cmu.alignvec v6 v6 v20 2
VAU.MACPW.u16 v7, v18, v6 
iau.incs i16, -9
NOP //myriad2
cmu.vdilv.x16 v5,v4, v0, v1     ||LSU0.LDI.128.U8.U16 v0, i18||LSU1.LDO.16 i3, i18, -2
VAU.MACPZ.u16 v4, v16           ||CMU.cpvv v6 v4        ||IAU.FEXTU i4, i3, 0, 8     ||LSU0.LDI.128.U8.U16 v1, i18     ||LSU1.LD.128.U8.U16 v20, i12
VAU.MACP.u16  v5, v17           ||CMU.SHLIV.x16 V4, v4, i4      ||IAU.FEXTU i5, i3, 8, 8
VAU.MACP.u16  v4, v18           ||CMU.SHLIV.x16 V5, v5, i5 || IAU.ADD i12 i12 16
VAU.MACP.u16  v5, v17           ||cmu.alignvec v6 v6 v20 2
VAU.MACPW.u16 v7, v18, v6       ||CMU.VSZM.BYTE v8, v7, [Z3Z1]
iau.shr.u32 i7, i16, 3
//// loop
nop 3
.lalign 
bru.rpl i8, i7                  ||cmu.vdilv.x16 v5,v4, v0, v1   || LSU0.LDI.128.U8.U16 v0, i18||LSU1.LDO.16 i3, i18, -2
 gaussHx2__GaussDownH_Loop:
 .nowarn 9
VAU.MACPZ.u16 v4, v16           || CMU.cpvv v6 v4           ||IAU.FEXTU i4, i3, 0, 8 ||LSU0.LDI.128.U8.U16 v1, i18     ||LSU1.LD.128.U8.U16 v20, i12
.nowarnend
VAU.MACP.u16  v5, v17           || CMU.SHLIV.x16 V4, v4, i4         ||IAU.FEXTU i5, i3, 8, 8
VAU.MACP.u16  v4, v18           || CMU.SHLIV.x16 V5, v5, i5         ||LSU0.STI.128.U16.U8 v8, i17 || IAU.ADD i12 i12 16
VAU.MACP.u16  v5, v17           || cmu.alignvec v6 v6 v20 2
VAU.MACPW.u16 v7, v18, v6       || CMU.VSZM.BYTE v8, v7, [Z3Z1]
nop
NOP //myriad2
////end loop
BRU.JMP i30                     ||LSU0.STI.128.U16.U8 v8, i17
nop 2
CMU.VSZM.BYTE v8, v7, [Z3Z1]
LSU0.STI.128.U16.U8 v8, i17
NOP 2//myriad2

.size mvcvGaussHx2_asm,.-mvcvGaussHx2_asm
.end
