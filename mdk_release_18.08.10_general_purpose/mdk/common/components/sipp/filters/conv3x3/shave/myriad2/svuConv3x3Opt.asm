//-------------------------------------------------------------------------------
.version 00.51.05
// C header
// void conv3x3opt(UInt8 *ln0(i18), UInt8 *ln1(i17), UInt8 *ln2(i16), UInt8 *output(i15), half *mat, UInt32 width)//
//-------------------------------------------------------------------------------

.code .text.conv3x3opt

.lalign
conv3x3opt:

//Load the convolution matrix in memory
//Do initial loads

 lsu0.ldo.16  i4,  i14, 0x00 || lsu1.ldil  i6,  0
 lsu0.ldo.16  i4,  i14, 0x02
 lsu0.ldo.16  i4,  i14, 0x04
 lsu0.ldo.16  i4,  i14, 0x06
 lsu0.ldo.16  i4,  i14, 0x08
 lsu0.ldo.16  i4,  i14, 0x0A
 lsu0.ldo.16  i4,  i14, 0x0C
 lsu0.ldo.16  i4,  i14, 0x0E  || cmu.cpivr.x16 v15, i4
 lsu0.ldo.16  i4,  i14, 0x10  || cmu.cpivr.x16 v16, i4
 lsu0.ld.128.U8.f16 v0, i18   || cmu.cpivr.x16 v17, i4 || iau.incs i18, -8 //Load Current
 lsu0.ld.128.U8.f16 v1, i17   || cmu.cpivr.x16 v18, i4 || iau.incs i17, -8
 lsu0.ld.128.U8.f16 v2, i16   || cmu.cpivr.x16 v19, i4 || iau.incs i16, -8
 lsu0.ld.128.U8.f16 v3, i18   || cmu.cpivr.x16 v20, i4 || iau.incs i18, 16 //Load Prev
 lsu0.ld.128.U8.f16 v4, i17   || cmu.cpivr.x16 v21, i4 || iau.incs i17, 16
 lsu0.ld.128.U8.f16 v5, i16   || cmu.cpivr.x16 v22, i4 || iau.incs i16, 16
 lsu0.ld.128.U8.f16 v6, i18   || cmu.cpivr.x16 v23, i4 || iau.incs i18,  8
 lsu0.ld.128.U8.f16 v7, i17   ||                          iau.incs i17,  8
 lsu0.ld.128.U8.f16 v8, i16   ||                          iau.incs i16,  8
 nop 3

//##########################################
.lalign
__svuConvLoop:
//##########################################

 //-----------------------------
 // |v9  v0 v10|   |v15 v16 v17|
 // |v11 v1 v12| * |v18 v19 v20|
 // |v13 v2 v14|   |v21 v22 v23|
 //-----------------------------

 vau.mul.f16 v25,  v0, v16 || cmu.alignvec v9,  v3, v0, 14
 vau.mul.f16 v24,  v9, v15 || cmu.alignvec v10, v0, v6,  2
 vau.mul.f16 v26, v10, v17 || cmu.alignvec v11, v4, v1, 14
 vau.mul.f16 v27, v11, v18 || cmu.alignvec v12, v1, v7,  2
 vau.mul.f16 v28,  v1, v19 || cmu.alignvec v13, v5, v2, 14
 vau.mul.f16 v29, v12, v20 || cmu.alignvec v14, v2, v8,  2

//At first iter, i6 =0, thus store is useless, then i6 = 8
 vau.mul.f16 v30, v13, v21 || lsu0.st.128.f16.u8 v31, i15 || iau.add i15, i15, i6
 vau.mul.f16 v31,  v2, v22 || lsu1.ldil i6, 8
 vau.mul.f16 v14, v14, v23

//Then slide the regs to reduce LOADS, load next data in advance
//Store back final result
 vau.add.f16 v24, v24, v25 || cmu.cpvv v3, v0    || lsu0.ld.128.U8.f16  v6, i18
 vau.add.f16 v26, v26, v27 || cmu.cpvv v4, v1    || lsu0.ld.128.U8.f16  v7, i17
 vau.add.f16 v28, v28, v29 || cmu.cpvv v5, v2    || lsu0.ld.128.U8.f16  v8, i16
 vau.add.f16 v24, v24, v14 || cmu.cpvv v0, v6    || iau.incs i13, -8
 vau.add.f16 v30, v30, v31 || cmu.cpvv v1, v7    || peu.pcix.neq 0x00 || bru.bra __svuConvLoop
                              cmu.cpvv v2, v8
 vau.add.f16 v24, v24, v26
 vau.add.f16 v28, v28, v30 ||  iau.incs i18, 8
                               iau.incs i17, 8
                               iau.incs i16, 8
 vau.add.f16 v31, v24, v28


//########################################
//The return:
//########################################
 bru.jmp i30
 nop
 lsu0.st.128.f16.u8 v31, i15 //final store
 nop 4
