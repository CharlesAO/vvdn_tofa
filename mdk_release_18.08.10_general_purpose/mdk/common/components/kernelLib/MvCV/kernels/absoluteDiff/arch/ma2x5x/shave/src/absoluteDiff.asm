/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvAbsoluteDiff_asm,@function
.version 00.51.04
.nowarn
.data .data.absoluteDiff
.align 4

.code .text.absoluteDiff
//void AbsDiff(u8** in1(i18), u8** in2(i17), u8** out(i16),u32 width(i15))//
.lalign
mvcvAbsoluteDiff_asm:
LSU0.LDIL i2 0x001F || LSU1.LDIH i2 0x0 
LSU0.LD.32 i18 i18 || LSU1.LD.32 i17 i17
LSU0.LD.32 i16 i16
IAU.AND i3 i15 i2 //width mod 32
LSU0.ldil i5, absoluteDiff_loop || LSU1.ldih i5, absoluteDiff_loop || IAU.SHR.u32  i1 i15 5

CMU.CMZ.i32  i1 || LSU0.ldil i7, absoluteDiff_compensate || LSU1.ldih i7, absoluteDiff_compensate
peu.pc1c eq || bru.jmp i7
nop 6

LSU0.LDO.64.L        v1  i18 0  || LSU1.LDO.64.L        v2  i17 0 
LSU0.LDO.64.H        v1  i18 8  || LSU1.LDO.64.H        v2  i17 8 
LSU0.LDO.64.L        v3  i18 16 || LSU1.LDO.64.L        v4  i17 16
LSU0.LDO.64.H        v3  i18 24 || LSU1.LDO.64.H        v4  i17 24 
// (Myriad2 COMPATIBILITY ISSUE): BRU.RPL: changed order of parameters at line number 29, in absDiff.asm
.lalign
IAU.SUB i10 i10 i10 || bru.rpl i5 i1
nop 2
IAU.ADD i18 i18 32 
IAU.ADD i17 i17 32
 absoluteDiff_loop:

VAU.ADIFF.u8  v5 v1 v2 || LSU0.LDO.64.L        v1  i18 0  || LSU1.LDO.64.L        v2  i17 0 
VAU.ADIFF.u8  v6 v3 v4 || LSU0.LDO.64.H        v1  i18 8  || LSU1.LDO.64.H        v2  i17 8 

LSU0.ST.64.L        v5  i16  || IAU.ADD i16 i16 8 || LSU1.LDO.64.L        v3  i18 16
LSU0.ST.64.H        v5  i16  || IAU.ADD i16 i16 8 || LSU1.LDO.64.L        v4  i17 16
LSU0.ST.64.L        v6  i16  || IAU.ADD i16 i16 8 || LSU1.LDO.64.H        v3  i18 24
LSU0.ST.64.H        v6  i16  || IAU.ADD i16 i16 8 || LSU1.LDO.64.H        v4  i17 24 
nop


CMU.CMZ.i32  i3
peu.pc1c eq || bru.jmp i30
nop 6


 absoluteDiff_compensate:

LSU0.LDO.64.L        v1  i18 0  || LSU1.LDO.64.L        v2  i17 0  || IAU.SUB i19 i19 0x20
LSU0.LDO.64.H        v1  i18 8  || LSU1.LDO.64.H        v2  i17 8  || IAU.SUB i12  i19 0
LSU0.LDO.64.L        v3  i18 16 || LSU1.LDO.64.L        v4  i17 16 || IAU.SUB i13  i19 0

LSU0.LDO.64.H        v3  i18 24 || LSU1.LDO.64.H        v4  i17 24 
nop 3
IAU.ADD i18 i18 32 
IAU.ADD i17 i17 32
VAU.ADIFF.u8  v5 v1 v2
VAU.ADIFF.u8  v6 v3 v4

LSU0.ST.64.L        v5  i12  || IAU.ADD i12 i12 8
LSU0.ST.64.H        v5  i12  || IAU.ADD i12 i12 8
LSU0.ST.64.L        v6  i12  || IAU.ADD i12 i12 8
LSU0.ST.64.H        v6  i12  || IAU.ADD i12 i12 8
LSU0.ldil i8, absoluteDiff_compensate_loop || LSU1.ldih i8, absoluteDiff_compensate_loop

// (Myriad2 COMPATIBILITY ISSUE): BRU.RPL: changed order of parameters at line number 65, in absDiff.asm
LSU0.LDI.8 i7, i13 || bru.rpl i8 i3
 absoluteDiff_compensate_loop:
nop 6
LSU0.ST.8 i7, i16 || IAU.ADD i16 i16 1

IAU.ADD i19 i19 0x20



BRU.jmp i30
nop 6

.size mvcvAbsoluteDiff_asm,.-mvcvAbsoluteDiff_asm
.nowarnend
.end
