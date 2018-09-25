//-------------------------------------------------------------------------------
.version 00.51.05

// C header:
//   void lumaBlur(UInt8 *inY0, UInt8 *inY1, UInt8 *inY2, UInt8 *outY, int width)

// Params:
//   i18 :inY0
//   i17 :inY1
//   i16 :inY2
//   i15 :outY
//   i14 :width
//-------------------------------------------------------------------------------

.code .text.lumaBlurOpt
.salign	 16

//##########################################
lumaBlurOpt:
//##########################################

//Initial loads
 lsu0.ld.128.u8.u16 v0, i18 || iau.incs i18, -8 //Load Current
 lsu0.ld.128.u8.u16 v1, i17 || iau.incs i17, -8
 lsu0.ld.128.u8.u16 v2, i16 || iau.incs i16, -8

 lsu0.ld.128.u8.u16 v3, i18 || iau.incs i18, 16 //Load Prev
 lsu0.ld.128.u8.u16 v4, i17 || iau.incs i17, 16
 lsu0.ld.128.u8.u16 v5, i16 || iau.incs i16, 16

 lsu0.ld.128.u8.u16 v6, i18 || iau.incs i18,  8
 lsu0.ld.128.u8.u16 v7, i17 || iau.incs i17,  8
 lsu0.ld.128.u8.u16 v8, i16 || iau.incs i16,  8
 nop 3

 cmu.alignvec v9,  v3, v0, 14 || lsu1.ldil  i6,  0
 cmu.alignvec v10, v0, v6,  2
 cmu.alignvec v11, v4, v1, 14
 cmu.alignvec v12, v1, v7,  2
 cmu.alignvec v13, v5, v2, 14
 cmu.alignvec v14, v2, v8,  2

//##########################################
.lalign
__lumaBlurLoop:
//##########################################

  vau.shl.x16 v19,  v1,   2  || cmu.cpvv v3, v0              || lsu0.ld.128.u8.u16  v6, i18
  vau.add.i16 v17,  v0,  v2  || cmu.cpvv v4, v1              || lsu0.ld.128.u8.u16  v7, i17
  vau.add.i16 v18, v11, v12  || cmu.cpvv v5, v2              || lsu0.ld.128.u8.u16  v8, i16
  vau.add.i16 v15,  v9, v10  || cmu.cpvv v0, v6              || lsu0.st.128.u16.u8 v20, i15 || iau.add i15, i15, i6
  vau.add.i16 v16, v13, v14  || cmu.cpvv v1, v7              || lsu1.ldil  i6,  8
  vau.add.i16 v17, v17, v18  || cmu.cpvv v2, v8              || iau.incs  i14, -8 //decrement number of left pixels
  vau.add.i16 v15, v15, v16  || cmu.alignvec v9,  v3, v0, 14 || peu.pcix.neq 0x00          || bru.bra __lumaBlurLoop
  vau.shl.x16 v17, v17,   1  || cmu.alignvec v10, v0, v6,  2
  vau.add.i16 v20, v19, v15  || cmu.alignvec v11, v4, v1, 14
                                cmu.alignvec v12, v1, v7,  2 || iau.incs i18, 8
  vau.add.i16 v20, v20, v17  || cmu.alignvec v13, v5, v2, 14 || iau.incs i17, 8
                                cmu.alignvec v14, v2, v8,  2 || iau.incs i16, 8
  vau.shr.u16 v20, v20, 4


//########################################
//The return:
//########################################
 bru.jmp i30
  nop
  lsu0.st.128.u16.u8 v20, i15 //final store
  nop 4
