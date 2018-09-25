 ///
 /// @file
 /// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
 ///            For License Warranty see: common/license.txt
 ///
 /// @brief
 /// This file contains the assembly optimised Two dimensional 8x8 DCT implementation
 /// The following SIMD implementation uses different computations for 1D transforms for rows followed by columns
 /// The functionality is achieved with 147 instructions.
 /// In order to understand the assembly code's design philosophy, reader must be aware of the butterfly implementation
.type mvcvDCT16bit_asm,@function
.version 00.51.05

.include svuCommonDefinitions.incl
// DCT Multiplier coefficients
.set  C1  11363
.set  C2  10703
.set  C3  9633
.set  C4  1
.set  C5  6436
.set  C6  4433
.set  C7  2260
.set kRound11B i8
.set kRound15B i9

// Following is the 8x8 matrix applied in order to perform row and column transforms
//   C4  C1  C2  C3  C4  C5  C6  C7
//   C4  C3  C6 -C7 -C4 -C1 -C2 -C5
//   C4  C5 -C6 -C1 -C4  C7  C2  C3
//  C4  C7 -C2 -C5  C4  C3 -C6 -C1
//  C4 -C7 -C2  C5  C4 -C3 -C6  C1
//  C4 -C5 -C6  C1 -C4 -C7  C2 -C3
//  C4 -C3  C6  C7 -C4  C1 -C2  C5
//  C4 -C1  C2 -C3  C4 -C5  C6 -C7

.data .data.DCT16bit

.align 16
 DCT_const_multiply:  // The ordering is in such a way that it will supplement the particular instructions used
//           rows[i][0]     rows[i][1]       rows[i][2]      rows[i][3]    rows[i][4]     rows[i][5]     rows[i][6]      rows[i][7]
  .short   C4,C4,C4,C4,  -C1,-C3,-C5,-C7,  C2,C6,-C6,-C2, -C3,C7,C1,C5, C4,-C4,-C4,C4, -C5,C1,-C7,-C3, C6,-C2,C2,-C6, -C7,C5,-C3,C1

.code .text.mvcvDCT16bit_asm
//void mvcvDCT16bit(short pixels[8][8], short data[8][8])
//                          i18               i17
mvcvDCT16bit_asm:
.nowarn 37
// ************************************************************************************************************************
// The Row transform begins here.
// Ideally in order to facilitate SIMD operations, we need to transpose the data matrix two times (beginning and end).
// But I have utilized the swizzle and horizontal-sum instructions so that the matrix transpose operation at the start and end are avoided.
// Multiplication is handled by VAU, additions by SAU, shift by IAU and packing by CMU and LSU
// Everything in pipeline so that one pixel output is produced at the end of each instruction.
// Note that for the odd numbered elements, the DCT multiplier co-efficients are stored with
// a negative sign since the swizzle works on input argument A of VAU
lsu0.ldil i0 DCT_const_multiply || lsu1.ldih i0 DCT_const_multiply
// ************************************************************************************************************************
// While loading the input data, each row pair is separated into two registers.
// 0,1,2,3 elements in one register and 4,5,6,7 elements in other register
lsu0.ldo.64.l v0 i18 0   || lsu1.ldo.64.l v1 i18 8   // Packing the 0,1,2,3 elements in v0 and v1 low halfs// 0,1,2,3 in v0
lsu0.ldo.64.h v0 i18 16  || lsu1.ldo.64.h v1 i18 24  // Packing the 4,5,6,7 elements in v0 and v1 high halfs //4,5,6,7 in v1
lsu0.ldo.64.l v2 i18 32  || lsu1.ldo.64.l v3 i18 40 || iau.xor kRound11B kRound11B kRound11B
lsu0.ldo.64.h v2 i18 48  || lsu1.ldo.64.h v3 i18 56 || iau.xor kRound15B kRound15B kRound15B
lsu0.ldo.64.l v4 i18 64  || lsu1.ldo.64.l v5 i18 72 || iau.xor i10 i10 i10 // vector with value of 1
lsu0.ldo.64.h v4 i18 80  || lsu1.ldo.64.h v5 i18 88
lsu0.ldo.64.l v6 i18 96  || lsu1.ldo.64.l v7 i18 104
lsu0.ldo.64.h v6 i18 112 || lsu1.ldo.64.h v7 i18 120
// ************************************************************************************************************************
// The Multipliers will be loaded in V16-V23 in signed 32-bit form
// The butterfly additions and summations for each row are stored in pairs (V8,V9), (V10,V11), (V12,V13), (V14,V15)
vau.add.i16 v8 v1 v0    || lsu1.swzv8 [45670123] || lsu0.ldo.64.l v16 i0 0  // v8 :s34 s25 s16 s07  s34 s25 s16 s07
vau.sub.i16 v9 v1 v0    || lsu1.swzv8 [45670123] || lsu0.ldo.64.l v17 i0 8  // v9 :d34 d25 d16 d07  d34 d25 d16 d07  all with negative sign
vau.add.i16 v10 v3 v2   || lsu1.swzv8 [45670123] || lsu0.ldo.64.l v18 i0 16 || cmu.cpvv.i16.i32 v0 v8
vau.sub.i16 v11 v3 v2   || lsu1.swzv8 [45670123] || lsu0.ldo.64.l v19 i0 24 || cmu.cpvv.i16.i32 v1 v9
vau.add.i16 v12 v5 v4   || lsu1.swzv8 [45670123] || lsu0.ldo.64.l v20 i0 32 || cmu.cpvv.i16.i32 v2 v10
vau.sub.i16 v13 v5 v4   || lsu1.swzv8 [45670123] || lsu0.ldo.64.l v21 i0 40 || cmu.cpvv.i16.i32 v3 v11
vau.add.i16 v14 v7 v6   || lsu1.swzv8 [45670123] || lsu0.ldo.64.l v22 i0 48
vau.sub.i16 v15 v7 v6   || lsu1.swzv8 [45670123] || lsu0.ldo.64.l v23 i0 56 || cmu.cpvv.i16.i32 v16 v16
// ************************************************************************************************************************
// The instruction pipeline starts here, whose throughput will be one pixel at a time.
// The pixels 0 to 7 of each row is computed and stored in V0 to V7. V0 contains first row, V1 contains second row,..etc
// Note that all the arithmetic computations happen in 32-bit precision and
// the 16 to 32-bit and 32 to 16-bit conversions are handled by CMU with no additional cycles.
vau.mul.i32 v6 v0 v16   || cmu.cpvv.i16.i32 v17 v17 || lsu0.ldil kRound11B 1024
vau.mul.i32 v7 v1 v17   || cmu.cpvv.i16.i32 v18 v18 || lsu0.ldil kRound15B 0x4000
vau.mul.i32 v6 v0 v18   || cmu.cpvv.i16.i32 v19 v19 || lsu0.ldil i10 1
vau.mul.i32 v7 v1 v19   || sau.sumx.i32 i0 v6  || cmu.cpvv.i16.i32 v20 v20
vau.mul.i32 v6 v0 v20   || sau.sumx.i32 i1 v7  || cmu.cpvv.i16.i32 v21 v21
vau.mul.i32 v7 v1 v21   || sau.sumx.i32 i2 v6  || iau.shl     i0 i0 2  || cmu.cpvv.i16.i32 v22 v22
iau.add i1 i1 kRound11B
vau.mul.i32 v6 v0 v22   || sau.sumx.i32 i3 v7  || iau.shr.i32 i1 i1 11 || cmu.cpvv.i16.i32 v23 v23
iau.add i2 i2 kRound11B
vau.mul.i32 v7 v1 v23   || sau.sumx.i32 i4 v6  || iau.shr.i32 i2 i2 11 || cmu.vszm.byte i0 i1 [10DD]
iau.add i3 i3 kRound11B
vau.mul.i32 v6 v2 v16   || sau.sumx.i32 i5 v7  || iau.shr.i32 i3 i3 11 || lsu1.cp v0.0 i0 || cmu.cpvv.i16.i32 v8 v8 || lsu0.swzc8 [76547654]
vau.mul.i32 v7 v3 v17   || sau.sumx.i32 i6 v6  || iau.shl     i4 i4 2  || cmu.vszm.byte i2 i3 [10DD]
iau.add i5 i5 kRound11B
vau.mul.i32 v6 v2 v18   || sau.sumx.i32 i7 v7  || iau.shr.i32 i5 i5 11 || lsu1.cp v0.1 i2 || cmu.cpvv.i16.i32 v9 v9 || lsu0.swzc8 [76547654]
iau.add i6 i6 kRound11B
vau.mul.i32 v7 v3 v19   || sau.sumx.i32 i0 v6  || iau.shr.i32 i6 i6 11 || cmu.vszm.byte i4 i5 [10DD]
iau.add i7 i7 kRound11B
vau.mul.i32 v6 v2 v20   || sau.sumx.i32 i1 v7  || iau.shr.i32 i7 i7 11 || lsu1.cp v0.2 i4 || cmu.cpvv.i16.i32 v10 v10 || lsu0.swzc8 [76547654]
vau.mul.i32 v7 v3 v21   || sau.sumx.i32 i2 v6  || iau.shl     i0 i0 2  || cmu.vszm.byte i6 i7 [10DD]
iau.add i1 i1 kRound11B
vau.mul.i32 v6 v2 v22   || sau.sumx.i32 i3 v7  || iau.shr.i32 i1 i1 11 || lsu1.cp v0.3 i6 || cmu.cpvv.i16.i32 v11 v11 || lsu0.swzc8 [76547654]
iau.add i2 i2 kRound11B
vau.mul.i32 v7 v3 v23   || sau.sumx.i32 i4 v6  || iau.shr.i32 i2 i2 11 || cmu.vszm.byte i0 i1 [10DD]
iau.add i3 i3 kRound11B
vau.mul.i32 v6 v8 v16   || sau.sumx.i32 i5 v7  || iau.shr.i32 i3 i3 11 || lsu1.cp v2.0 i0
vau.mul.i32 v7 v9 v17   || sau.sumx.i32 i6 v6  || iau.shl     i4 i4 2  || cmu.vszm.byte i2 i3 [10DD]
iau.add i5 i5 kRound11B
vau.mul.i32 v6 v8 v18   || sau.sumx.i32 i7 v7  || iau.shr.i32 i5 i5 11 || lsu1.cp v2.1 i2
iau.add i6 i6 kRound11B
vau.mul.i32 v7 v9 v19   || sau.sumx.i32 i0 v6  || iau.shr.i32 i6 i6 11 || cmu.vszm.byte i4 i5 [10DD]
iau.add i7 i7 kRound11B
vau.mul.i32 v6 v8 v20   || sau.sumx.i32 i1 v7  || iau.shr.i32 i7 i7 11 || lsu1.cp v2.2 i4
vau.mul.i32 v7 v9 v21   || sau.sumx.i32 i2 v6  || iau.shl     i0 i0 2  || cmu.vszm.byte i6 i7 [10DD]
iau.add i1 i1 kRound11B
vau.mul.i32 v6 v8 v22   || sau.sumx.i32 i3 v7  || iau.shr.i32 i1 i1 11 || lsu1.cp v2.3 i6
iau.add i2 i2 kRound11B
vau.mul.i32 v7 v9 v23   || sau.sumx.i32 i4 v6  || iau.shr.i32 i2 i2 11 || cmu.vszm.byte i0 i1 [10DD]
iau.add i3 i3 kRound11B
vau.mul.i32 v6 v10 v16  || sau.sumx.i32 i5 v7  || iau.shr.i32 i3 i3 11 || lsu1.cp v1.0 i0 || cmu.cpvv.i16.i32 v8 v12
vau.mul.i32 v7 v11 v17  || sau.sumx.i32 i6 v6  || iau.shl     i4 i4 2  || cmu.vszm.byte i2 i3 [10DD]
iau.add i5 i5 kRound11B
vau.mul.i32 v6 v10 v18  || sau.sumx.i32 i7 v7  || iau.shr.i32 i5 i5 11 || lsu1.cp v1.1 i2 || cmu.cpvv.i16.i32 v9 v13
iau.add i6 i6 kRound11B
vau.mul.i32 v7 v11 v19  || sau.sumx.i32 i0 v6  || iau.shr.i32 i6 i6 11 || cmu.vszm.byte i4 i5 [10DD]
iau.add i7 i7 kRound11B
vau.mul.i32 v6 v10 v20  || sau.sumx.i32 i1 v7  || iau.shr.i32 i7 i7 11 || lsu1.cp v1.2 i4
vau.mul.i32 v7 v11 v21  || sau.sumx.i32 i2 v6  || iau.shl     i0 i0 2  || cmu.vszm.byte i6 i7 [10DD]
iau.add i1 i1 kRound11B
vau.mul.i32 v6 v10 v22  || sau.sumx.i32 i3 v7  || iau.shr.i32 i1 i1 11 || lsu1.cp v1.3 i6
iau.add i2 i2 kRound11B
vau.mul.i32 v7 v11 v23  || sau.sumx.i32 i4 v6  || iau.shr.i32 i2 i2 11 || cmu.vszm.byte i0 i1 [10DD]
iau.add i3 i3 kRound11B
vau.mul.i32 v6 v8 v16   || sau.sumx.i32 i5 v7  || iau.shr.i32 i3 i3 11 || lsu1.cp v3.0 i0
vau.mul.i32 v7 v9 v17   || sau.sumx.i32 i6 v6  || iau.shl     i4 i4 2  || cmu.vszm.byte i2 i3 [10DD]
iau.add i5 i5 kRound11B
vau.mul.i32 v6 v8 v18   || sau.sumx.i32 i7 v7  || iau.shr.i32 i5 i5 11 || lsu1.cp v3.1 i2
iau.add i6 i6 kRound11B
vau.mul.i32 v7 v9 v19   || sau.sumx.i32 i0 v6  || iau.shr.i32 i6 i6 11 || cmu.vszm.byte i4 i5 [10DD]
iau.add i7 i7 kRound11B
vau.mul.i32 v6 v8 v20   || sau.sumx.i32 i1 v7  || iau.shr.i32 i7 i7 11 || lsu1.cp v3.2 i4 || cmu.cpvv.i16.i32 v12 v12 || lsu0.swzc8 [76547654]
vau.mul.i32 v7 v9 v21   || sau.sumx.i32 i2 v6  || iau.shl     i0 i0 2 || cmu.vszm.byte i6 i7 [10DD]
iau.add i1 i1 kRound11B
vau.mul.i32 v6 v8 v22   || sau.sumx.i32 i3 v7  || iau.shr.i32 i1 i1 11 || lsu1.cp v3.3 i6 || cmu.cpvv.i16.i32 v13 v13 || lsu0.swzc8 [76547654]
iau.add i2 i2 kRound11B
vau.mul.i32 v7 v9 v23   || sau.sumx.i32 i4 v6  || iau.shr.i32 i2 i2 11 || cmu.vszm.byte i0 i1 [10DD]
iau.add i3 i3 kRound11B
vau.mul.i32 v8 v12 v16  || sau.sumx.i32 i5 v7  || iau.shr.i32 i3 i3 11 || lsu1.cp v4.0 i0
vau.mul.i32 v9 v13 v17  || sau.sumx.i32 i6 v6  || iau.shl     i4 i4 2  || cmu.vszm.byte i2 i3 [10DD]
iau.add i5 i5 kRound11B
vau.mul.i32 v8 v12 v18  || sau.sumx.i32 i7 v7  || iau.shr.i32 i5 i5 11 || lsu1.cp v4.1 i2
iau.add i6 i6 kRound11B
vau.mul.i32 v9 v13 v19  || sau.sumx.i32 i0 v8  || iau.shr.i32 i6 i6 11 || cmu.vszm.byte i4 i5 [10DD]
iau.add i7 i7 kRound11B
vau.mul.i32 v8 v12 v20  || sau.sumx.i32 i1 v9  || iau.shr.i32 i7 i7 11 || lsu1.cp v4.2 i4 || cmu.cpvv.i16.i32 v10 v14
vau.mul.i32 v9 v13 v21  || sau.sumx.i32 i2 v8  || iau.shl     i0 i0 2  || cmu.vszm.byte i6 i7 [10DD]
iau.add i1 i1 kRound11B
vau.mul.i32 v8 v12 v22  || sau.sumx.i32 i3 v9  || iau.shr.i32 i1 i1 11 || lsu1.cp v4.3 i6 || cmu.cpvv.i16.i32 v11 v15
iau.add i2 i2 kRound11B
vau.mul.i32 v9 v13 v23  || sau.sumx.i32 i4 v8  || iau.shr.i32 i2 i2 11 || cmu.vszm.byte i0 i1 [10DD]
iau.add i3 i3 kRound11B
vau.mul.i32 v12 v10 v16 || sau.sumx.i32 i5 v9  || iau.shr.i32 i3 i3 11 || lsu1.cp v5.0 i0
vau.mul.i32 v13 v11 v17 || sau.sumx.i32 i6 v8  || iau.shl     i4 i4 2  || cmu.vszm.byte i2 i3 [10DD]
iau.add i5 i5 kRound11B
vau.mul.i32 v12 v10 v18 || sau.sumx.i32 i7 v9     || iau.shr.i32 i5 i5 11 || lsu1.cp v5.1 i2 || cmu.cpvv.i16.i32 v14 v14 || lsu0.swzc8 [76547654]
iau.add i6 i6 kRound11B || cmu.cpvv.i16.i32 v8 v0 || lsu0.swzc8 [76547654]
vau.mul.i32 v13 v11 v19 || sau.sumx.i32 i0 v12    || iau.shr.i32 i6 i6 11 || cmu.vszm.byte i4 i5 [10DD]
iau.add i7 i7 kRound11B || cmu.cpvv.i16.i32 v0 v0
vau.mul.i32 v12 v10 v20 || sau.sumx.i32 i1 v13    || iau.shr.i32 i7 i7 11 || lsu1.cp v5.2 i4 || cmu.cpvv.i16.i32 v15 v15 || lsu0.swzc8 [76547654]
vau.mul.i32 v13 v11 v21 || sau.sumx.i32 i2 v12    || iau.shl     i0 i0 2  || cmu.vszm.byte i6 i7 [10DD]
iau.add i1 i1 kRound11B || cmu.cpvv.i16.i32 v9 v1 || lsu0.swzc8 [76547654]
vau.mul.i32 v12 v10 v22 || sau.sumx.i32 i3 v13    || iau.shr.i32 i1 i1 11 || lsu1.cp v5.3 i6
iau.add i2 i2 kRound11B || cmu.cpvv.i16.i32 v1 v1
vau.mul.i32 v13 v11 v23 || sau.sumx.i32 i4 v12     || iau.shr.i32 i2 i2 11 || cmu.vszm.byte i0 i1 [10DD]
iau.add i3 i3 kRound11B || cmu.cpvv.i16.i32 v10 v2 || lsu0.swzc8 [76547654]
vau.mul.i32 v12 v14 v16 || sau.sumx.i32 i5 v13     || iau.shr.i32 i3 i3 11 || lsu1.cp v6.0 i0
vau.mul.i32 v13 v15 v17 || sau.sumx.i32 i6 v12     || iau.shl     i4 i4 2  || cmu.vszm.byte i2 i3 [10DD]
iau.add i5 i5 kRound11B || cmu.cpvv.i16.i32 v2 v2
vau.mul.i32 v12 v14 v18 || sau.sumx.i32 i7 v13     || iau.shr.i32 i5 i5 11 || lsu1.cp v6.1 i2
iau.add i6 i6 kRound11B || cmu.cpvv.i16.i32 v11 v3 || lsu0.swzc8 [76547654]
vau.mul.i32 v13 v15 v19 || sau.sumx.i32 i0 v12     || iau.shr.i32 i6 i6 11 || cmu.vszm.byte i4 i5 [10DD]
iau.add i7 i7 kRound11B || cmu.cpvv.i16.i32 v3 v3
vau.mul.i32 v12 v14 v20 || sau.sumx.i32 i1 v13     || iau.shr.i32 i7 i7 11 || lsu1.cp v6.2 i4
vau.mul.i32 v13 v15 v21 || sau.sumx.i32 i2 v12     || iau.shl     i0 i0 2  || cmu.vszm.byte i6 i7 [10DD]
iau.add i1 i1 kRound11B
vau.mul.i32 v12 v14 v22 || sau.sumx.i32 i3 v13     || iau.shr.i32 i1 i1 11 || lsu1.cp v6.3 i6
iau.add i2 i2 kRound11B || cmu.cpvv.i16.i32 v14 v6 || lsu0.swzc8 [76547654]
vau.mul.i32 v13 v15 v23 || sau.sumx.i32 i4 v12     || iau.shr.i32 i2 i2 11 || cmu.vszm.byte i0 i1 [10DD]
iau.add i3 i3 kRound11B || cmu.cpivr.x32 v22 i10
cmu.cpivr.x32 v23 kRound15B || sau.sumx.i32 i5 v13 || iau.shr.i32 i3 i3 11 || lsu1.cp v7.0 i0

// ************************************************************************************************************************
// The column transform begins here.
// For the column transform, the data is already arranged in SIMD friendly manner. We compute the values in short and 
// expand into integer afterwards.
// The butterfly additions and summations between the rows are stored in following manner
// s07 = V0(V8), d07 = V7(V15), s16 = V1(V9), d16 = V6(V14), s25 = V2(V10), d25 = V5(V13), s34 = V3(V11), d34 = V4(v12),
// s0734 = V0(V8), d0734 = V3(V11), s1625 = V1(V9), d1625 = V2(V10)

vau.add.i32 v9 v9 v14       || sau.sumx.i32 i6 v12     || cmu.cpvv.i16.i32 v6 v6
vau.sub.i32 v14 v9 v14      || iau.shl     i4 i4 2     || cmu.vszm.byte i2 i3 [10DD]
vau.add.i32 v1 v1 v6        || sau.sumx.i32 i7 v13     || lsu1.cp v7.1 i2
vau.sub.i32 v6 v1 v6        || iau.add i5 i5 kRound11B || cmu.cpvv.i16.i32 v12 v4 || lsu0.swzc8 [76547654]

sau.add.i32 i6 i6 kRound11B || iau.shr.i32 i5 i5 11    || cmu.cpvv.i16.i32 v13 v5 || lsu0.swzc8 [76547654]
iau.add i7 i7 kRound11B     || cmu.cpvv.i16.i32 v4 v4

vau.add.i32 v10 v10 v13     || cmu.cpvv.i16.i32 v5 v5
vau.sub.i32 v13 v10 v13     || iau.shr.i32 i6 i6 11    || cmu.vszm.byte i4 i5 [10DD]
vau.add.i32 v2 v2 v5        || iau.shr.i32 i7 i7 11    || lsu1.cp v7.2 i4
vau.sub.i32 v5 v2 v5        || cmu.vszm.byte i6 i7 [10DD]

vau.add.i32 v3 v3 v4        || lsu1.cp v7.3 i6
vau.sub.i32 v4 v3 v4        || cmu.cpvv.i16.i32 v15 v7 || lsu0.swzc8 [76547654]
vau.add.i32 v11 v11 v12     || cmu.cpvv.i16.i32 v7  v7
vau.sub.i32 v12 v11 v12

vau.add.i32 v0 v0 v7        || cmu.vszm.word v16 v18 [10DD]
vau.sub.i32 v7 v0 v7
vau.add.i32 v8 v8 v15
vau.sub.i32 v15 v8 v15

vau.add.i32 v0 v0 v3  
vau.sub.i32 v3 v0 v3   
vau.add.i32 v8 v8 v11
vau.sub.i32 v11 v8 v11
vau.add.i32 v1 v1 v2 
vau.sub.i32 v2 v1 v2  
vau.add.i32 v9 v9 v10
vau.sub.i32 v10 v9 v10
// ************************************************************************************************************************
// The multiply and accumulate (VAU.MAC) operations are issued back to back so that at the end of
// each MAC write operation, we get 4 pixels. Note that each MAC write has a latency of 3 cycles.
// The shift operations are avoided with CMU interleave instructions since the shift is by 16 bits
// The DCT multipliers will be present in V8/17 =(-C7 -C5 -C3 -C1) and v9/16 =(C6 C2 C4 C4) - big endian
// The swizzle instruction is used to select the applicable co-efficient out of V8 and V9 along with the MAC operation.
// The odd numbered rows require more computations and they are calculated first.
// computations for Row-1
vau.macnz.i32 v17 v7     || lsu1.swzv8 [10101010]
vau.macn.i32  v17 v6     || lsu1.swzv8 [32323232]                        // C1 * d07 + C3 * d16
vau.macn.i32  v17 v5     || lsu1.swzv8 [54545454]                        // C1 * d07 + C3 * d16 + C5 * d25
vau.macp.i32  v23 v22                                                    // C1 * d07 + C3 * d16 + C5 * d25 + kRound11Bit
vau.macnw.i32 v18 v17 v4  || lsu1.swzv8 [76767676]                       // C1 * d07 + C3 * d16 + C5 * d25 + C7 * d34 + kRound11Bit
vau.macnz.i32 v17 v15     || lsu1.swzv8 [10101010]
vau.macn.i32  v17 v14     || lsu1.swzv8 [32323232]
vau.macn.i32  v17 v13     || lsu1.swzv8 [54545454]
vau.macp.i32  v23 v22
vau.macnw.i32 v19 v17 v12 || lsu1.swzv8 [76767676]

//computations for Row-3
vau.macpz.i32 v17 v5      || lsu1.swzv8 [10101010]                         // -C1 * d25
vau.macn.i32  v17 v7      || lsu1.swzv8 [32323232]                        // C3 * d07 - C1 * d25
vau.macp.i32  v17 v4      || lsu1.swzv8 [54545454]                        // C3 * d07 - C1 * d25 - C5 * d34
vau.macp.i32  v23 v22                                                     // C3 * d07 - C1 * d25 - C5 * d34 + kRound11Bit
vau.macpw.i32 v20 v17 v6 || lsu1.swzv8 [76767676] || cmu.vdilv.x16 v18 v19 v18 v19  // C3 * d07 - C7 * d16 - C1 * d25 - C5 * d34 + kRound11Bit
vau.shr.i16 v18 v18 2
vau.macpz.i32 v17 v13     || lsu1.swzv8 [10101010]
vau.macn.i32  v17 v15     || lsu1.swzv8 [32323232] || lsu0.sto.64.l v18 i17 16
vau.macp.i32  v17 v12     || lsu1.swzv8 [54545454] || lsu0.sto.64.h v18 i17 24
vau.macp.i32  v23 v22
vau.macpw.i32 v21 v17 v14 || lsu1.swzv8 [76767676]

// computations for Row-5
vau.macpz.i32 v17 v6      || lsu1.swzv8 [10101010]                        // -C1 * d16
vau.macn.i32  v17 v4      || lsu1.swzv8 [32323232]                        // -C1 * d16 + C3 * d34
vau.macn.i32  v17 v7      || lsu1.swzv8 [54545454]                        //  C5 * d07 - C1 * d16  + C3 * d34
vau.macp.i32  v23 v22                                                     //  C5 * d07 - C1 * d16  + C3 * d34 + kRound11Bit
vau.macnw.i32 v18 v17 v5  || lsu1.swzv8 [76767676] || cmu.vdilv.x16 v20 v21 v20 v21 // C5 * d07 - C1 * d16 + C7 * d25 + C3 * d34 + kRound11Bit
vau.shr.i16 v20 v20 2
vau.macpz.i32 v17 v14     || lsu1.swzv8 [10101010]
vau.macn.i32  v17 v12     || lsu1.swzv8 [32323232] || lsu0.sto.64.l v20 i17 48
vau.macn.i32  v17 v15     || lsu1.swzv8 [54545454] || lsu0.sto.64.h v20 i17 56
vau.macp.i32  v23 v22
vau.macnw.i32 v19 v17 v13 || lsu1.swzv8 [76767676]

// computations for Row-7
vau.macpz.i32 v17 v4      || lsu1.swzv8 [10101010]                        // -C1 * d34
vau.macn.i32  v17 v5      || lsu1.swzv8 [32323232]                        //  C3 * d25 - C1 * d34
vau.macp.i32  v17 v6      || lsu1.swzv8 [54545454]                        // -C5 * d16 + C3 * d25 - C1 * d34
vau.macp.i32  v23 v22                                                     // -C5 * d16 + C3 * d25 - C1 * d34 + kRound11Bit
vau.macnw.i32 v20 v17 v7  || lsu1.swzv8 [76767676] || cmu.vdilv.x16 v18 v19 v18 v19 // C7 * d07 - C5 * d16 + C3 * d25 - C1 * d34 + kRound11Bit
vau.shr.i16 v18 v18 2
vau.macpz.i32 v17 v12     || lsu1.swzv8 [10101010]
vau.macn.i32  v17 v13     || lsu1.swzv8 [32323232] || lsu0.sto.64.l v18 i17 80
vau.macp.i32  v17 v14     || lsu1.swzv8 [54545454] || lsu0.sto.64.h v18 i17 88
vau.macp.i32  v23 v22
vau.macnw.i32 v21 v17 v15 || lsu1.swzv8 [76767676]

// computations for Row-2
vau.macpz.i32 v16 v3      || lsu1.swzv8 [54545454] //|| cmu.cpvv.i16.i32 v8 v1                         // C2 * d0734
vau.macp.i32  v23 v22                                                                                  // C2 * d0734 + kRound15Bit
vau.macpw.i32 v18 v16 v2  || lsu1.swzv8 [76767676] //|| cmu.cpvv.i16.i32 v1 v1 || lsu0.swzc8 [76547654] // C2 * d0734 + C6 * d1625 + kRound15Bit
vau.macpz.i32 v16 v11     || lsu1.swzv8 [54545454]
vau.macp.i32  v23 v22
vau.macpw.i32 v19 v16 v10  || lsu1.swzv8 [76767676] || cmu.vdilv.x16 v20 v21 v20 v21
vau.shr.i16 v20 v20 2
// computations for Row-6
vau.macpz.i32 v16 v3      || lsu1.swzv8 [76767676]                                  // C6 * d0734
vau.macp.i32  v23 v22                                                               // C6 * d0734 + kRound15Bit
vau.macnw.i32 v20 v16 v2  || lsu1.swzv8 [54545454] || lsu0.sto.64.l v20 i17 112     // C6 * d0734 - C2 * d1625 + kRound15Bit
vau.macpz.i32 v16 v11     || lsu1.swzv8 [76767676] || lsu0.sto.64.h v20 i17 120
vau.macp.i32  v23 v22
vau.macnw.i32 v21 v16 v10 || lsu1.swzv8 [54545454] || cmu.vdilv.x16 v18 v19 v18 v19
vau.shr.i16 v18 v18 2
// computations for Row-0
vau.macpz.i32 v16 v0      || lsu1.swzv8 [10101010]
vau.macpw.i32 v18 v16 v1  || lsu1.swzv8 [10101010] || lsu0.sto.64.l v18 i17 32
vau.macpz.i32 v16 v8      || lsu1.swzv8 [10101010] || lsu0.sto.64.h v18 i17 40
vau.macpw.i32 v19 v16 v9  || lsu1.swzv8 [10101010] || cmu.vdilv.x16 v20 v21 v20 v21
vau.shr.i16 v20 v20 2

// computations for Row-4
vau.macpz.i32 v16 v0      || lsu1.swzv8 [10101010]
vau.shr.i32 v18 v18 5     || lsu0.sto.64.l v20 i17 96
vau.shr.i32 v19 v19 5     || lsu0.sto.64.h v20 i17 104
vau.macnw.i32 v20 v16 v1  || lsu1.swzv8 [10101010]
vau.macpz.i32 v16 v8      || lsu1.swzv8 [10101010]
vau.macnw.i32 v21 v16 v9  || lsu1.swzv8 [10101010] || cmu.vdilv.x16 v18 v19 v18 v19
lsu0.sto.64.l v19 i17 0  || bru.jmp i30
lsu0.sto.64.h v19 i17 8
vau.shr.i32 v20 v20 5
vau.shr.i32 v21 v21 5
nop
cmu.vdilv.x16 v20 v21 v20 v21
lsu0.sto.64.l v21 i17 64 || lsu1.sto.64.h v21 i17 72
.nowarnend
.size mvcvDCT16bit_asm,.-mvcvDCT16bit_asm
.end

