//-------------------------------------------------------------------------------
.version 00.51.05
// atb, 05 sep 2013
// note: yet another asm module
// C header:
// void genChroma(UInt8* inRGB[3], UInt8 *inY, UInt8* outC[3], int width, int eps)
//                       i18              i17         i16          i15        i14
//-------------------------------------------------------------------------------

.code .text.genChromaOpt
.salign	 16

//@@@@@@@@@@@@@@@@@@@@@@@
genChromaOpt:
//@@@@@@@@@@@@@@@@@@@@@@@

//Load RGB-in pointers
 lsu0.ldo.32  i9, i18, 0x00 || cmu.cpivr.x16 v7, i14
 lsu0.ldo.32 i10, i18, 0x04 || cmu.cpii      i8, i17
 lsu0.ldo.32 i11, i18, 0x08 || lsu1.ldil  i0, (255/3)
//Load Chroma-out pointes
 lsu0.ldo.32 i12, i16, 0x00 || cmu.cpivr.x16 v8,  i0
 lsu0.ldo.32 i13, i16, 0x04
 lsu0.ldo.32 i14, i16, 0x08



//################################################################################################################
// takeoff 1
//################################################################################################################
LSU0.LDI.128.u8.u16 v0,  i8  //Load Luma
LSU0.LDI.128.u8.u16 v1,  i9  //Load R
LSU0.LDI.128.u8.u16 v2, i10  //Load G
LSU0.LDI.128.u8.u16 v3, i11  //Load B
nop
nop
nop
VAU.IADDS.u16 v0,  v0,  v7
nop

//################################################################################################################
// takeoff 2
//################################################################################################################
                                                              VAU.IMULS.u16  v1,  v1,  v8    || LSU1.CP i4,  v0.0
                                                                                                LSU1.CP i5,  v0.1
                                                                                                LSU1.CP i6,  v0.2
                                                              LSU0.LDI.128.u8.u16 v0,  i8    || LSU1.CP i7,  v0.3
                                                              VAU.IMULS.u16  v2,  v2,  v8    || LSU1.CP i0,  v1.0
                                 SAU.DIV.u16 i0,  i0,  i4                                    || LSU1.CP i1,  v1.1
                                 SAU.DIV.u16 i1,  i1,  i5  || IAU.SUBSU    i15, i15, 0x08    || LSU1.CP i2,  v1.2
                                 SAU.DIV.u16 i2,  i2,  i6  || LSU0.LDI.128.u8.u16  v1,  i9   || LSU1.CP i3,  v1.3
                                 SAU.DIV.u16 i3,  i3,  i7  || VAU.IMULS.u16  v3,  v3,  v8    || LSU1.CP i0,  v2.0
                                 SAU.DIV.u16 i0,  i0,  i4  || BRU.BRA  svuGenChroma_Loop     || LSU1.CP i1,  v2.1
                                 SAU.DIV.u16 i1,  i1,  i5                                    || LSU1.CP i2,  v2.2
                                 SAU.DIV.u16 i2,  i2,  i6  || LSU0.LDI.128.u8.u16  v2,  i10  || LSU1.CP i3,  v2.3
                                 SAU.DIV.u16 i3,  i3,  i7                                    || LSU1.CP i0,  v3.0
VAU.IADDS.u16  v0,  v0,  v7   || SAU.DIV.u16 i0,  i0,  i4                                    || LSU1.CP i1,  v3.1
CMU.CPIV.x32 v4.0,  i0        || SAU.DIV.u16 i1,  i1,  i5                                    || LSU1.CP i2,  v3.2
CMU.CPIV.x32 v4.1,  i1        || SAU.DIV.u16 i2,  i2,  i6  || LSU0.LDI.128.u8.u16  v3,  i11  || LSU1.CP i3,  v3.3

//################################################################################################################
// warp zone
//################################################################################################################
.lalign
svuGenChroma_Loop:
CMU.CPIV.x32 v4.2,  i2        || SAU.DIV.u16 i3,  i3,  i7  || VAU.IMULS.u16  v1,  v1,  v8    || LSU1.CP i4,  v0.0
CMU.CPIV.x32 v4.3,  i3                                                                       || LSU1.CP i5,  v0.1
CMU.CPIV.x32 v5.0,  i0                                                                       || LSU1.CP i6,  v0.2
CMU.CPIV.x32 v5.1,  i1                                     || LSU0.LDI.128.u8.u16 v0,  i8    || LSU1.CP i7,  v0.3
CMU.CPIV.x32 v5.2,  i2                                     || VAU.IMULS.u16  v2,  v2,  v8    || LSU1.CP i0,  v1.0
CMU.CPIV.x32 v5.3,  i3        || SAU.DIV.u16 i0,  i0,  i4                                    || LSU1.CP i1,  v1.1
CMU.CPIV.x32 v6.0,  i0        || SAU.DIV.u16 i1,  i1,  i5  || IAU.SUBSU    i15, i15, 0x08    || LSU1.CP i2,  v1.2
CMU.CPIV.x32 v6.1,  i1        || SAU.DIV.u16 i2,  i2,  i6  || LSU0.LDI.128.u8.u16  v1,  i9   || LSU1.CP i3,  v1.3
CMU.CPIV.x32 v6.2,  i2        || SAU.DIV.u16 i3,  i3,  i7  || VAU.IMULS.u16  v3,  v3,  v8    || LSU1.CP i0,  v2.0
CMU.CPIV.x32 v6.3,  i3        || SAU.DIV.u16 i0,  i0,  i4  || BRU.BRA svuGenChroma_Loop      || LSU1.CP i1,  v2.1  || PEU.PCIX.NEQ 0
CMU.VSZM.BYTE v4,  v4, [Z2Z0] || SAU.DIV.u16 i1,  i1,  i5                                    || LSU1.CP i2,  v2.2
CMU.VSZM.BYTE v5,  v5, [Z2Z0] || SAU.DIV.u16 i2,  i2,  i6  || LSU0.LDI.128.u8.u16  v2,  i10  || LSU1.CP i3,  v2.3
CMU.VSZM.BYTE v6,  v6, [Z2Z0] || SAU.DIV.u16 i3,  i3,  i7  || LSU0.STI.128.u16.u8  v4,  i12  || LSU1.CP i0,  v3.0
VAU.IADDS.u16  v0,  v0,  v7   || SAU.DIV.u16 i0,  i0,  i4  || LSU0.STI.128.u16.u8  v5,  i13  || LSU1.CP i1,  v3.1
CMU.CPIV.x32 v4.0,  i0        || SAU.DIV.u16 i1,  i1,  i5  || LSU0.STI.128.u16.u8  v6,  i14  || LSU1.CP i2,  v3.2
CMU.CPIV.x32 v4.1,  i1        || SAU.DIV.u16 i2,  i2,  i6  || LSU0.LDI.128.u8.u16  v3,  i11  || LSU1.CP i3,  v3.3

//################################################################################################################
// landing
//################################################################################################################
CMU.CPIV.x32 v4.2,  i2        || SAU.DIV.u16 i3,  i3,  i7
CMU.CPIV.x32 v4.3,  i3
CMU.CPIV.x32 v5.0,  i0
CMU.CPIV.x32 v5.1,  i1
CMU.CPIV.x32 v5.2,  i2
CMU.CPIV.x32 v5.3,  i3
CMU.CPIV.x32 v6.0,  i0
CMU.CPIV.x32 v6.1,  i1        || BRU.JMP i30
CMU.CPIV.x32 v6.2,  i2
CMU.CPIV.x32 v6.3,  i3
CMU.VSZM.BYTE v4,  v4, [Z2Z0]
CMU.VSZM.BYTE v5,  v5, [Z2Z0]                              || LSU0.ST.128.u16.u8  v4,  i12
CMU.VSZM.BYTE v6,  v6, [Z2Z0]                              || LSU0.ST.128.u16.u8  v5,  i13
                                                              LSU0.ST.128.u16.u8  v6,  i14
