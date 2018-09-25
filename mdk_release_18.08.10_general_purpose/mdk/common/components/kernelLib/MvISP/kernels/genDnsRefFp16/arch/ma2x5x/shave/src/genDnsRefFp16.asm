/////
///// @file      genDnsRefFp16.asm
///// @copyright All code copyright Movidius Ltd 2012, all rights reserved
/////            For License Warranty see: common/license.txt
/////
///// @brief     Generate Luma denoise reference plane as input for LumaDenoise kernel. 
/////            The reference plane is used for calculating weights to be assigned to 
/////            pixels in the neighbourhood
/////
//-------------------------------------------------------------------------------
.type mvispGenDnsRefFp16_asm,@function
.version 00.51.05
//                             i18             i17            i16        i15             i14                    i13
//void genDnsRefFp16(half *inY, unsigned int yc, int x0,  YDnsRefFp16Param* param, unsigned char* outRef, unsigned int width)
//{
//    int x, xc//
//    unsigned char idxLuma//
//    for(x=0// x<(int)width// x++)
//    {
//        xc = (x0 + x)//
//        xc = xc * xc//
//        // convert luma to u8 in order to be use as index in lut
//        idxLuma = (unsigned char)(inY[x] * 255.0f)//
//        unsigned short value = (unsigned short)param->lutGamma[idxLuma] * 
//            (unsigned short)param->lutDist[clampU8L(((xc+yc)>>param->shift))]//
//        value = value >> 8//
//        outRef[x] = (unsigned char)value//
//    }
//}
//------------------------------------------------------------------------------- 


.data .data.genDnsRefFp16_asm
.align 4

.code .text.genDnsRefFp16_asm   
//########################################

.set P_O_LUT_GAMMA             0
.set P_O_LUT_DIST              4
.set P_O_SHIFT                 8

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.lalign 
mvispGenDnsRefFp16_asm:
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
cmu.cpii       i12, i14
cmu.cpii       i14, i13 || LSU0.LDO.32 i0,  i15,  P_O_SHIFT
cmu.cpii       i13, i18 || LSU1.LDO.32 i0,  i15,  P_O_LUT_DIST
CMU.CPIVR.x32   v3, i17 || LSU0.LDO.32 i0,  i15,  P_O_LUT_GAMMA
CMU.CPIV.x32  v1.0, i16 || IAU.ADD i16,  i16,  0x01
CMU.CPIV.x32  v1.1, i16 || IAU.ADD i16,  i16,  0x01
CMU.CPIV.x32  v1.2, i16 || IAU.ADD i16,  i16,  0x01
CMU.CPIV.x32  v1.3, i16	
CMU.CPIVR.x32   v6,  i0	
CMU.CPIVR.x32   v7,  i0              	
CMU.CPIVR.x32   v8,  i0              	
//##############################################################################################
// take off
//##############################################################################################
LSU1.LD.64.l v0, i13
nop
                                  VAU.MUL.i32   v2,  v1,  v1
                                  IAU.ADD       i13, i13, 0x08
nop        
nop
                                  VAU.IADDS.u32 v4,  v3,  v2
CMU.CPVV.f16.u8fs v5,  v0
                                  VAU.SHR.u32   v4,  v4,  v6
                                  VAU.ADD.i32   v1,  v1,  0x04      ||CMU.CPVV.u8.u16 v5,  v5
                                  VAU.IADDS.u32 v4,  v4,  v7        ||CMU.CPVV.i16.i32 v5,  v5
                                  VAU.IADDS.u32 v5,  v5,  v8                             

CMU.CPVI.x32 i0,  v4.0                                              || LSU1.LD.64.l v0, i13
CMU.CPVI.x32 i1,  v4.1                                              || LSU1.LD.32.u8.u32  i0,  i0
CMU.CPVI.x32 i2,  v4.2                                              || LSU0.LD.32.u8.u32  i1,  i1
CMU.CPVI.x32 i3,  v4.3                                              || LSU1.LD.32.u8.u32  i2,  i2
CMU.CPVI.x32 i4,  v5.0          || VAU.MUL.i32   v2,  v1,  v1       || LSU0.LD.32.u8.u32  i3,  i3
CMU.CPVI.x32 i5,  v5.1          || IAU.ADD       i13, i13, 0x08     || LSU1.LD.32.u8.u32  i4,  i4
CMU.CPVI.x32 i6,  v5.2          || IAU.SUBSU     i14, i14, 0x04     || LSU0.LD.32.u8.u32  i5,  i5
CMU.CPVI.x32 i7,  v5.3          || BRU.BRA svuGenDnsRef_Loop        || LSU1.LD.32.u8.u32  i6,  i6
CMU.CPVV.f16.u8fs v5,  v0       || VAU.IADDS.u32 v4,  v3,  v2       || LSU0.LD.32.u8.u32  i7,  i7 
nop
VAU.SHR.u32   v4,  v4,  v6
VAU.ADD.i32   v1,  v1,  0x04    ||CMU.CPVV.u8.u16 v5,  v5
VAU.IADDS.u32 v4,  v4,  v7      ||CMU.CPVV.i16.i32 v5,  v5
                                  VAU.IADDS.u32 v5,  v5,  v8

//##############################################################################################
// warp zone
//##############################################################################################
.lalign
svuGenDnsRef_Loop:
CMU.CPVI.x32 i0,  v4.0          || IAU.MUL       i8,  i4,  i0    || LSU1.LD.64.l v0, i13
CMU.CPVI.x32 i1,  v4.1          || IAU.MUL       i9,  i5,  i1    || LSU0.LD.32.u8.u32  i0,  i0
CMU.CPVI.x32 i2,  v4.2          || IAU.MUL       i10, i6,  i2    || LSU1.LD.32.u8.u32  i1,  i1
CMU.VSZM.BYTE i8,  i8,  [ZZZ1]  ||lsu1.cp i3,  v4.3              || IAU.MUL       i11, i7,  i3       || LSU0.LD.32.u8.u32  i2,  i2
CMU.VSZM.BYTE i8,  i9,  [ZZ1D]  ||lsu0.cp i4,  v5.0              || VAU.MUL.i32   v2,  v1,  v1       || LSU1.LD.32.u8.u32  i3,  i3
CMU.VSZM.BYTE i8,  i10, [Z1DD]  ||lsu1.cp i5,  v5.1              || IAU.ADD       i13, i13, 0x08     || LSU0.LD.32.u8.u32  i4,  i4
CMU.VSZM.BYTE i8,  i11, [1DDD]  ||lsu0.cp i6,  v5.2              || IAU.SUBSU     i14, i14, 0x04     || LSU1.LD.32.u8.u32  i5,  i5
CMU.CPVI.x32 i7,  v5.3          || BRU.BRA svuGenDnsRef_Loop     || LSU0.LD.32.u8.u32  i6,  i6       || PEU.PCIX.NEQ 0
CMU.CPVV.f16.u8fs v5,  v0       || VAU.IADDS.u32 v4,  v3,  v2    || LSU1.LD.32.u8.u32  i7,  i7 
nop
VAU.SHR.u32   v4,  v4,  v6
VAU.ADD.i32   v1,  v1,  0x04    ||CMU.CPVV.u8.u16 v5,  v5
VAU.IADDS.u32 v4,  v4,  v7      ||CMU.CPVV.i16.i32 v5,  v5
                                VAU.IADDS.u32 v5,  v5,  v8       || LSU0.STI.32   i8,  i12
//##############################################################################################
// landing
//##############################################################################################
                                  IAU.MUL       i8,  i4,  i0
                                  IAU.MUL       i9,  i5,  i1
BRU.JMP i30                    || IAU.MUL       i10, i6,  i2
                                  IAU.MUL       i11, i7,  i3
CMU.VSZM.BYTE i8,  i8,  [ZZZ1]
CMU.VSZM.BYTE i8,  i9,  [ZZ1D]
CMU.VSZM.BYTE i8,  i10, [Z1DD]
CMU.VSZM.BYTE i8,  i11, [1DDD]
LSU0.ST.32    i8,  i12

.size mvispGenDnsRefFp16_asm,.-mvispGenDnsRefFp16_asm
.end
