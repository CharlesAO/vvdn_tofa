//-------------------------------------------------------------------------------
.version 00.51.05
// atb, 05 sep 2013
// note: yet another asm module
// C header
// void genDnsRef(UInt8 *inY, UInt32 yc, UInt32 x0,  YDnsRefParam* param, UInt8* outRef, UInt32 width)
//                       i18         i17        i16                i15           i14            i13
//-------------------------------------------------------------------------------

.code .text.genDnsRefOpt
.salign	 16

.set P_O_LUT_GAMMA             0
.set P_O_LUT_DIST              4
.set P_O_SHIFT                 8

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
genDnsRefOpt:
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
LSU1.LD.128.u8.u16 v0, i13
nop
                                  VAU.MUL.i32   v2,  v1,  v1
                                  IAU.ADD       i13, i13, 0x04
nop
nop
                                  VAU.IADDS.u32 v4,  v3,  v2
CMU.CPVV.i16.i32 v5,  v0
                                  VAU.SHR.u32   v4,  v4,  v6
                                  VAU.ADD.i32   v1,  v1,  0x04
                                  VAU.IADDS.u32 v4,  v4,  v7
                                  VAU.IADDS.u32 v5,  v5,  v8

CMU.CPVI.x32 i0,  v4.0                                            || LSU0.LD.128.u8.u16 v0,  i13
CMU.CPVI.x32 i1,  v4.1                                            || LSU1.LD.32.u8.u32  i0,  i0
CMU.CPVI.x32 i2,  v4.2                                            || LSU0.LD.32.u8.u32  i1,  i1
CMU.CPVI.x32 i3,  v4.3                                            || LSU1.LD.32.u8.u32  i2,  i2
CMU.CPVI.x32 i4,  v5.0         || VAU.MUL.i32   v2,  v1,  v1      || LSU0.LD.32.u8.u32  i3,  i3
CMU.CPVI.x32 i5,  v5.1         || IAU.ADD       i13, i13, 0x04    || LSU1.LD.32.u8.u32  i4,  i4
CMU.CPVI.x32 i6,  v5.2         || IAU.SUBSU     i14, i14, 0x04    || LSU0.LD.32.u8.u32  i5,  i5
CMU.CPVI.x32 i7,  v5.3         || BRU.BRA svuGenDnsRef_Loop       || LSU1.LD.32.u8.u32  i6,  i6
CMU.CPVV.i16.i32  v5,  v0      || VAU.IADDS.u32 v4,  v3,  v2      || LSU0.LD.32.u8.u32  i7,  i7
CMU.VSZM.BYTE i8,  i8,  [ZZZ1]
CMU.VSZM.BYTE i8,  i9,  [ZZ1D] || VAU.SHR.u32   v4,  v4,  v6
CMU.VSZM.BYTE i8,  i10, [Z1DD] || VAU.ADD.i32   v1,  v1,  0x04
CMU.VSZM.BYTE i8,  i11, [1DDD] || VAU.IADDS.u32 v4,  v4,  v7
                                  VAU.IADDS.u32 v5,  v5,  v8

//##############################################################################################
// warp zone
//##############################################################################################
.lalign
svuGenDnsRef_Loop:
CMU.CPVI.x32 i0,  v4.0         || IAU.MUL       i8,  i4,  i0      || LSU1.LD.128.u8.u16 v0,  i13
CMU.CPVI.x32 i1,  v4.1         || IAU.MUL       i9,  i5,  i1      || LSU0.LD.32.u8.u32  i0,  i0
CMU.CPVI.x32 i2,  v4.2         || IAU.MUL       i10, i6,  i2      || LSU1.LD.32.u8.u32  i1,  i1
CMU.CPVI.x32 i3,  v4.3         || IAU.MUL       i11, i7,  i3      || LSU0.LD.32.u8.u32  i2,  i2
CMU.CPVI.x32 i4,  v5.0         || VAU.MUL.i32   v2,  v1,  v1      || LSU1.LD.32.u8.u32  i3,  i3
CMU.CPVI.x32 i5,  v5.1         || IAU.ADD       i13, i13, 0x04    || LSU0.LD.32.u8.u32  i4,  i4
CMU.CPVI.x32 i6,  v5.2         || IAU.SUBSU     i14, i14, 0x04    || LSU1.LD.32.u8.u32  i5,  i5
CMU.CPVI.x32 i7,  v5.3         || BRU.BRA svuGenDnsRef_Loop       || LSU0.LD.32.u8.u32  i6,  i6   || PEU.PCIX.NEQ 0
CMU.CPVV.i16.i32  v5,  v0      || VAU.IADDS.u32 v4,  v3,  v2      || LSU1.LD.32.u8.u32  i7,  i7
CMU.VSZM.BYTE i8,  i8,  [ZZZ1]
CMU.VSZM.BYTE i8,  i9,  [ZZ1D] || VAU.SHR.u32   v4,  v4,  v6
CMU.VSZM.BYTE i8,  i10, [Z1DD] || VAU.ADD.i32   v1,  v1,  0x04
CMU.VSZM.BYTE i8,  i11, [1DDD] || VAU.IADDS.u32 v4,  v4,  v7
                                  VAU.IADDS.u32 v5,  v5,  v8      || LSU0.STI.32   i8,  i12

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

.end
