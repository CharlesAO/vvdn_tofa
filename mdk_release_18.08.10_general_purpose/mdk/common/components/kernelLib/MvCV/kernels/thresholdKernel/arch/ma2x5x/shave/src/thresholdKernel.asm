///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvThresholdKernel_asm,@function
.version 00.51.04

.data .data.thresholdKernel
.align 4

.code .text.thresholdKernel
//void threshKernel(u8** in(i18), u8** out(i17), u32 width(i16), u32 height(i15), u8 thresh(i14), u32 thresh_type(i13))

mvcvThresholdKernel_asm:
.nowarn
	LSU0.LD.32 i18 i18     //input
	LSU0.LD.32 i17 i17     //output
	nop
	LSU1.LDIL i7 0xFF   || IAU.SUB i6 i6 i6
	LSU1.LDIL i3 1        //Thresh_type = Threshold_To_Zero_Inv
	LSU1.LDIL i4 2        //Thresh_type = Threshold_To_Binary
	LSU1.LDIL i5 3        //Thresh_type = Threshold_To_Binary_Inv
	LSU1.LDIL i10 4       //Thresh_type = Threshold_Trunc
	LSU0.LDIL i2 0x000f   || IAU.SHR.u32 i8 i16 4  || CMU.CPIVR.x8 v5 i14   //copies i14=thresh into v5(transmitted by usr)
	CMU.CPIVR.x8 v7 i7    || IAU.AND i11 i16 i2    //last 4 remaining pixels
	IAU.SUB i9 i15 0

CMU.CMII.u8 i13 i6 || LSU0.LDIL i1 thresholdKernel_thresholdToZero_loop   || LSU1.LDIH i1 thresholdKernel_thresholdToZero_loop
PEU.PC1C EQ || BRU.BRA thresholdKernel_thresholdToZero_loop_head
nop 6

CMU.CMII.u8 i13 i3 || LSU0.LDIL i1 thresholdKernel_thresholdToZeroInv_loop   || LSU1.LDIH i1 thresholdKernel_thresholdToZeroInv_loop
PEU.PC1C EQ   || BRU.BRA thresholdKernel_thresholdToZeroInv_loop_head
nop 6

CMU.CMII.u8 i13 i4 || LSU0.LDIL i1 thresholdKernel_thresholdToBinary_loop    || LSU1.LDIH i1 thresholdKernel_thresholdToBinary_loop
PEU.PC1C EQ  || BRU.BRA thresholdKernel_thresholdToBinary_loop_head
nop 6

CMU.CMII.u8 i13 i5 || LSU0.LDIL i1 thresholdKernel_thresholdToBinaryInv_loop || LSU1.LDIH i1 thresholdKernel_thresholdToBinaryInv_loop
PEU.PC1C EQ  || BRU.BRA thresholdKernel_thresholdToBinaryInv_loop_head
nop 6

CMU.CMII.u8 i13 i10 || LSU0.LDIL i1 thresholdKernel_thresholdTrunc_loop      || LSU1.LDIH i1 thresholdKernel_thresholdTrunc_loop
PEU.PC1C GTE || BRU.BRA thresholdKernel_thresholdTrunc_loop_head
nop 6


 thresholdKernel_thresholdToZero_loop_head:
.lalign
CMU.CMZ.i8 i8
PEU.PC1C EQ || BRU.BRA thresholdKernel_thresholdToZero_compensation
nop 6

 thresholdKernel_thresholdToZeroHeight_loop:
// (Myriad2 COMPATIBILITY ISSUE): BRU.RPL: changed order of parameters at line number 53, in thresKernel.asm
LSU0.LDO.64.L v1 i18 0 || LSU1.LDO.64.H v1 i18 8 || IAU.ADD i18 i18 16 || BRU.RPL i1 i8
nop 5

 thresholdKernel_thresholdToZero_loop:
CMU.CPIVR.x8 v6 i6
CMU.CMVV.u8 v1 v5
PEU.PVV8 GT || VAU.ADD.i8 v6 v6 v1
nop 3
LSU0.STO.64.L v6 i17 0 || LSU1.STO.64.H v6 i17 8 || IAU.ADD i17 i17 16

//LAST UP TO 15 PIXELS!
 thresholdKernel_thresholdToZero_compensation:
LSU0.LDO.64.L v1 i18 0 || LSU1.LDO.64.H v1 i18 8 || IAU.ADD i18 i18 16
nop 5
CMU.CPIVR.x8 v6 i6
CMU.CMVV.u8 v1 v5
PEU.PVV8 GT || VAU.ADD.i8 v6 v6 v1

LSU0.LDIL i2, 0xFFFF  || LSU1.LDIH i2, 0xFFFF  || IAU.SUB i0, i0, i0
IAU.SUB i12, i12, i12
IAU.FINSJ i12, i2, i11
CMU.CMASK v2, i12, 0
// (Myriad2 COMPATIBILITY ISSUE): VAU.SWZWORD: no longer available at line number 74, in thresKernel.asm
CMU.CMZ.i8 v2 
CMU.VSZM.WORD v2, v2, [3232]
PEU.PVL08  NEQ || LSU0.STO.64.L v6 i17 0 || CMU.CMZ.i8 v2
PEU.PVL18  NEQ || LSU1.STO.64.H v6 i17 8
// Fix for using Height, too
IAU.SUB i9 i9 1
CMU.CMZ.i32 i9
PEU.PC1C GT || BRU.BRA thresholdKernel_thresholdToZeroHeight_loop
nop 6

BRU.jmp i30
nop 6

 thresholdKernel_thresholdToZeroInv_loop_head:
.lalign
CMU.CMZ.i8 i8
PEU.PC1C EQ || BRU.BRA thresholdKernel_thresholdToZeroInv_compensation
nop 6

 thresholdKernel_thresholdToZeroInvHeight_loop:
// (Myriad2 COMPATIBILITY ISSUE): BRU.RPL: changed order of parameters at line number 86, in thresKernel.asm
LSU0.LDO.64.L v1 i18 0 || LSU1.LDO.64.H v1 i18 8 || IAU.ADD i18 i18 16 || BRU.RPL i1 i8
nop 5

 thresholdKernel_thresholdToZeroInv_loop:
CMU.CPIVR.x8 v6 i6
CMU.CMVV.u8 v1 v5
PEU.PVV8 LTE || VAU.ADD.i8 v6 v6 v1
nop 3
LSU0.STO.64.L v6 i17 0 || LSU1.STO.64.H v6 i17 8 || IAU.ADD i17 i17 16

//LAST UP TO 15 PIXELS!
 thresholdKernel_thresholdToZeroInv_compensation:
LSU0.LDO.64.L v1 i18 0 || LSU1.LDO.64.H v1 i18 8 || IAU.ADD i18 i18 16
nop 5
CMU.CPIVR.x8 v6 i6
CMU.CMVV.u8 v1 v5
PEU.PVV8 LTE || VAU.ADD.i8 v6 v6 v1

LSU0.LDIL i2, 0xFFFF || LSU1.LDIH i2, 0xFFFF || IAU.SUB i0, i0, i0
IAU.SUB i12, i12, i12
IAU.FINSJ i12, i2, i11
CMU.CMASK v2, i12, 0
// (Myriad2 COMPATIBILITY ISSUE): VAU.SWZWORD: no longer available at line number 107, in thresKernel.asm
CMU.CMZ.i8 v2  
CMU.VSZM.WORD v2, v2, [3232] //swizzle for placing on lower half the sup half//
PEU.PVL08  NEQ || LSU0.STO.64.L v6 i17 0 || CMU.CMZ.i8 v2
PEU.PVL18  NEQ || LSU1.STO.64.H v6 i17 8
// Fix for using Height, too
IAU.SUB i9 i9 1
CMU.CMZ.i32 i9
PEU.PC1C GT || BRU.BRA thresholdKernel_thresholdToZeroInvHeight_loop
nop 6

BRU.jmp i30
nop 6


 thresholdKernel_thresholdToBinary_loop_head:
.lalign
CMU.CMZ.i8 i8
PEU.PC1C EQ || BRU.BRA thresholdKernel_thresholdToBinary_compensation
nop 6

 thresholdKernel_thresholdToBinaryHeight_loop:
// (Myriad2 COMPATIBILITY ISSUE): BRU.RPL: changed order of parameters at line number 120, in thresKernel.asm
LSU0.LDO.64.L v1 i18 0 || LSU1.LDO.64.H v1 i18 8 || IAU.ADD i18 i18 16 || BRU.RPL i1 i8
nop 5

 thresholdKernel_thresholdToBinary_loop:
CMU.CPIVR.x8 v6 i6
CMU.CMVV.u8 v1 v5
PEU.PVV8 GT || VAU.ADD.i8 v6 v6 v7
nop 3
LSU0.STO.64.L v6 i17 0 || LSU1.STO.64.H v6 i17 8 || IAU.ADD i17 i17 16

//LAST UP TO 15 PIXELS!
 thresholdKernel_thresholdToBinary_compensation:
LSU0.LDO.64.L v1 i18 0 || LSU1.LDO.64.H v1 i18 8 || IAU.ADD i18 i18 16
nop 5
CMU.CPIVR.x8 v6 i6
CMU.CMVV.u8 v1 v5
PEU.PVV8 GT || VAU.ADD.i8 v6 v6 v7

LSU0.LDIL i2, 0xFFFF || LSU1.LDIH i2, 0xFFFF || IAU.SUB i0, i0, i0
IAU.SUB i12, i12, i12
IAU.FINSJ i12, i2, i11
CMU.CMASK v2, i12, 0
// (Myriad2 COMPATIBILITY ISSUE): VAU.SWZWORD: no longer available at line number 141, in thresKernel.asm
CMU.CMZ.i8 v2
CMU.VSZM.WORD v2, v2, [3232]
PEU.PVL08  NEQ || LSU0.STO.64.L v6 i17 0 || CMU.CMZ.i8 v2
PEU.PVL18  NEQ || LSU1.STO.64.H v6 i17 8
// Fix for using Height, too
IAU.SUB i9 i9 1
CMU.CMZ.i32 i9
PEU.PC1C GT || BRU.BRA thresholdKernel_thresholdToBinaryHeight_loop
nop 6

BRU.jmp i30
nop 6


 thresholdKernel_thresholdToBinaryInv_loop_head:
.lalign
CMU.CMZ.i8 i8
PEU.PC1C EQ || BRU.BRA thresholdKernel_thresholdToBinaryInv_compensation
nop 6

 thresholdKernel_thresholdToBinaryInvHeight_loop:
// (Myriad2 COMPATIBILITY ISSUE): BRU.RPL: changed order of parameters at line number 154, in thresKernel.asm
LSU0.LDO.64.L v1 i18 0 || LSU1.LDO.64.H v1 i18 8 || IAU.ADD i18 i18 16 || BRU.RPL i1 i8
nop 5

 thresholdKernel_thresholdToBinaryInv_loop:
CMU.CPIVR.x8 v6 i6
CMU.CMVV.u8 v1 v5
PEU.PVV8 LTE || VAU.ADD.i8 v6 v6 v7
nop 3
LSU0.STO.64.L v6 i17 0 || LSU1.STO.64.H v6 i17 8 || IAU.ADD i17 i17 16

//LAST UP TO 15 PIXELS!
 thresholdKernel_thresholdToBinaryInv_compensation:
LSU0.LDO.64.L v1 i18 0 || LSU1.LDO.64.H v1 i18 8 || IAU.ADD i18 i18 16
nop 5
CMU.CPIVR.x8 v6 i6
CMU.CMVV.u8 v1 v5
PEU.PVV8 LTE || VAU.ADD.i8 v6 v6 v7

LSU0.LDIL i2, 0xFFFF || LSU1.LDIH i2, 0xFFFF || IAU.SUB i0, i0, i0
IAU.SUB i12, i12, i12
IAU.FINSJ i12, i2, i11
CMU.CMASK v2, i12, 0
// (Myriad2 COMPATIBILITY ISSUE): VAU.SWZWORD: no longer available at line number 175, in thresKernel.asm
CMU.CMZ.i8 v2
CMU.VSZM.WORD v2, v2, [3232]
PEU.PVL08  NEQ || LSU0.STO.64.L v6 i17 0 || CMU.CMZ.i8 v2
PEU.PVL18  NEQ || LSU1.STO.64.H v6 i17 8
// Fix for using Height, too
IAU.SUB i9 i9 1
CMU.CMZ.i32 i9
PEU.PC1C GT || BRU.BRA thresholdKernel_thresholdToBinaryInvHeight_loop
nop 6

BRU.jmp i30
nop 6


 thresholdKernel_thresholdTrunc_loop_head:
.lalign
CMU.CMZ.i8 i8
PEU.PC1C EQ || BRU.BRA thresholdKernel_thresholdTrunc_compensation
nop 6

 thresholdKernel_thresholdTruncHeight_loop:
// (Myriad2 COMPATIBILITY ISSUE): BRU.RPL: changed order of parameters at line number 188, in thresKernel.asm
LSU0.LDO.64.L v1 i18 0 || LSU1.LDO.64.H v1 i18 8 || IAU.ADD i18 i18 16 || BRU.RPL i1 i8
nop 6
 thresholdKernel_thresholdTrunc_loop:
CMU.CPIVR.x8 v6 i6
CMU.CMVV.u8 v1 v5
PEU.PVV8 GT  || VAU.ADD.i8 v6 v6 v5
PEU.PVV8 LTE || VAU.ADD.i8 v6 v6 v1
nop 2
LSU0.STO.64.L v6 i17 0 || LSU1.STO.64.H v6 i17 8 || IAU.ADD i17 i17 16

//LAST UP TO 15 PIXELS!
 thresholdKernel_thresholdTrunc_compensation:
LSU0.LDO.64.L v1 i18 0 || LSU1.LDO.64.H v1 i18 8 || IAU.ADD i18 i18 16
nop 5
CMU.CPIVR.x8 v6 i6
CMU.CMVV.u8 v1 v5
PEU.PVV8 GT  || VAU.ADD.i8 v6 v6 v5
PEU.PVV8 LTE || VAU.ADD.i8 v6 v6 v1

LSU0.LDIL i2, 0xFFFF ||LSU1.LDIH i2, 0xFFFF || IAU.SUB i0, i0, i0
IAU.SUB i12, i12, i12
IAU.FINSJ i12, i2, i11
CMU.CMASK v2, i12, 0
// (Myriad2 COMPATIBILITY ISSUE): VAU.SWZWORD: no longer available at line number 210, in thresKernel.asm
CMU.CMZ.i8 v2
CMU.VSZM.WORD v2, v2, [3232]
PEU.PVL08  NEQ || LSU0.STO.64.L v6 i17 0 || CMU.CMZ.i8 v2
PEU.PVL18  NEQ || LSU1.STO.64.H v6 i17 8
// Fix for using Height, too
IAU.SUB i9 i9 1
CMU.CMZ.i32 i9
PEU.PC1C GT || BRU.BRA thresholdKernel_thresholdTruncHeight_loop
nop 6

BRU.jmp i30
nop 6
.nowarnend
.size mvcvThresholdKernel_asm,.-mvcvThresholdKernel_asm
.end
