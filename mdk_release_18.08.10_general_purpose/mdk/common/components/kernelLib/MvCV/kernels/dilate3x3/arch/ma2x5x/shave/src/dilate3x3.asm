///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvDilate3x3_asm,@function
.version 00.51.04

// Local defines
// Function arguments
.set srcImage1 i18
.set destImage i17
.set kernel i16
.set widthImage i15
.set heightImage i14
.set k i13

// Others registers

.set zero i11




.data .data.dilate3x3
.align 4
.code .text.dilate3x3

//  void dilate3x3(u8** src, u8** dst, u8** kernel, u32 width)
//                   i18       i17        i16        i15     
mvcvDilate3x3_asm:
.nowarn 10




LSU0.LDIL i14 16

// Set the src address lines
LSU0.LDO.32 i4 i16 0 || LSU1.LDO.32 i1 i18 0
LSU0.LDO.32 i5 i16 4 || LSU1.LDO.32 i2 i18 4
LSU0.LDO.32 i6 i16 8 || LSU1.LDO.32 i3 i18 8
LSU0.LD.32  i17 i17
NOP 4

// Load first kernel line || // Load the second kernel line
LSU0.LDO.8 i7 i4 0   || LSU1.LDO.8 i10 i5 0 || IAU.SUB i1 i1 1 // sipp
LSU0.LDO.8 i8 i4 1   || LSU1.LDO.8 i11 i5 1 || IAU.SUB i2 i2 1 // sipp
LSU0.LDO.8 i9 i4 2   || LSU1.LDO.8 i12 i5 2 || IAU.SUB i3 i3 1 // sipp


// Load source values
LSU0.LDO.64.L v1  i1 0  || LSU1.LDO.64.H v1 i1 8 || IAU.ADD i1 i1 i14
LSU0.LDO.64.L v23 i1 0  

LSU0.LDO.64.L v2  i2 0  || LSU1.LDO.64.H v2 i2 8 || IAU.ADD i2 i2 i14
LSU0.LDO.64.L v22 i2 0  

LSU0.LDO.64.L v3  i3 0  || LSU1.LDO.64.H v3 i3 8 || IAU.ADD i3 i3 i14
LSU0.LDO.64.L v9 i3 0  

// Create vectors using the first kernel line
CMU.CPIVR.x8 v4 i7
CMU.CPIVR.x8 v5 i8
CMU.CPIVR.x8 v6 i9   
CMU.ALIGNVEC v14 v1 v23  1

//Create vectors using the second kernel line
CMU.CPIVR.x8 v8 i10 
CMU.ALIGNVEC v15 v1 v23  2
CMU.CPIVR.x8 v7 i11 
CMU.ALIGNVEC v17 v2 v22  1
CMU.CPIVR.x8 v0 i12 
CMU.ALIGNVEC v18 v2 v22  2

LSU0.LDO.8 i7 i6 0   
LSU0.LDO.8 i8 i6 1   
LSU0.LDO.8 i9 i6 2

NOP 4

// Create vectors using the third kernel line
CMU.CPIVR.x8 v10 i7 
CMU.ALIGNVEC v18 v2 v22  2
CMU.CPIVR.x8 v11 i8 
CMU.ALIGNVEC v20 v3 v9  1
CMU.CPIVR.x8 v12 i9
CMU.ALIGNVEC v21 v3 v9  2 || LSU0.LDIL i16 0

//If width < 16 then
CMU.CMII.u32 i15 i14
PEU.PC1C LT || CMU.CPII i14 i15 || BRU.BRA dilate3x3_Compensate_Last_8_Bits
NOP 6

SAU.SHR.u32 i0 i15 4
LSU0.LDIL i4 dilate3x3_Process  || LSU1.LDIH i4 dilate3x3_Process
NOP 2

// Main Loop
BRU.RPL i4 i0

 dilate3x3_Start_Computing:

//

// FIRST LINE
// Multiply the first source input values with the kernel mask
VAU.MUL.i8 v13 v1  v4 
VAU.MUL.i8 v14 v14 v5
VAU.MUL.i8 v15 v15 v6


// SECOND LINE
// Multiply the second source input values with the kernel mask || Add the necessary shift(line1)
VAU.MUL.i8 v16  v2 v8  || LSU1.LD.64.L  v1  i1
VAU.MUL.i8 v17 v17 v7  || LSU1.LDO.64.H v1  i1 8 || IAU.ADD i1 i1 i14
VAU.MUL.i8 v18 v18 v0  || LSU1.LDO.64.L v23 i1 0


// THIRD LINE
// Multiply the third source input values with the kernel mask || Add the necessary shift(line2)
VAU.MUL.i8 v19 v3  v10 || LSU1.LD.64.L  v2   i2
VAU.MUL.i8 v20 v20 v11 || LSU1.LDO.64.H v2  i2 8 || IAU.ADD i2 i2 i14
VAU.MUL.i8 v21 v21 v12 || LSU1.LDO.64.L v22 i2 0

// Compute the maximum value between the first two values on each line
CMU.MAX.u8 v13 v13 v14  || LSU0.LD.64.L v3 i3 || LSU1.LDO.64.H v3 i3 8 || IAU.ADD i3 i3 i14
CMU.MAX.u8 v16 v16 v17  || LSU0.LDO.64.L v9 i3 0
CMU.MAX.u8 v19 v19 v20

// Compute the maximum on each line
CMU.MAX.u8 v13 v13 v15 
nop
CMU.ALIGNVEC v14 v1 v23  1
//Compute the maximum value on a kernel
CMU.MAX.u8 v16 v16 v18 
nop
CMU.ALIGNVEC v15 v1 v23  2
CMU.MAX.u8 v19 v19 v21


CMU.MAX.u8 v13 v13 v16 

nop
 dilate3x3_Process:
CMU.ALIGNVEC v17  v2    v22  1 
CMU.ALIGNVEC v20 v3 v9  1
CMU.MAX.u8 v13 v13 v19 
nop
CMU.ALIGNVEC v18  v2     v22  2
CMU.ALIGNVEC v21 v3 v9  2
// Place the result in the destination file
LSU0.ST.64.L v13 i17    || LSU1.STO.64.H v13  i17 8         || IAU.ADD i17 i17 i14

SAU.SHL.X32 i0  i0 4
NOP 2
IAU.SUB i15 i15 i0
PEU.PC1I EQ || BRU.JMP i30
NOP 6


 dilate3x3_Compensate_Last_8_Bits:

// FIRST LINE
// Multiply the first source input values with the kernel mask
VAU.MUL.i8 v13 v1  v4 
VAU.MUL.i8 v14 v14 v5
VAU.MUL.i8 v15 v15 v6

// SECOND LINE
// Multiply the second source input values with the kernel mask || Add the necessary shift(line1)
VAU.MUL.i8 v16  v2 v8 
VAU.MUL.i8 v17 v17 v7  
VAU.MUL.i8 v18 v18 v0 

// THIRD LINE
// Multiply the third source input values with the kernel mask || Add the necessary shift(line2)
VAU.MUL.i8 v19 v3  v10 
VAU.MUL.i8 v20 v20 v11 
VAU.MUL.i8 v21 v21 v12 

// Compute the maximum value between the first two values on each line
CMU.MAX.u8 v13 v13 v14  
CMU.MAX.u8 v16 v16 v17  
CMU.MAX.u8 v19 v19 v20

// Compute the maximum on each line
CMU.MAX.u8 v13 v13 v15 


 dilate3x3_Process_Last_8_:
//Compute the maximum value on a kernel
CMU.MAX.u8 v16 v16 v18 
CMU.MAX.u8 v19 v19 v21
CMU.MAX.u8 v13 v13 v16 
NOP
CMU.MAX.u8 v13 v13 v19 
NOP
// Place the result in the destination file
LSU0.ST.64.L v13 i17 
   


BRU.JMP i30
NOP 6
.nowarnend
.size mvcvDilate3x3_asm,.-mvcvDilate3x3_asm
.end
