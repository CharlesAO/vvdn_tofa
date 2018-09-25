///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief: convertYUV400ToYUV422 - This kernel converts from yuv400 to yuv422 
///         

//--------------------------------------------------------------------------------------------------------------
.type mvcvConvertYUV400ToYUV422_asm,@function
.version 00.50.05
.data .rodata.vect_mask 
.align 16
.nowarn
.code .text.convertYUV400ToYUV422
.align 16
//void mvcvConvertYUV400ToYUV422(u8* in, u16* out, u32 width)
//                                i18       i17        i16
mvcvConvertYUV400ToYUV422_asm:

LSU0.ldil i15, 0x8000 || LSU1.ldih i15, 0x8000
LSU0.ldil i14, mvcvConvertYUV400ToYUV422_loop || LSU1.ldih i14, mvcvConvertYUV400ToYUV422_loop
LSU0.ldil i10, mvcvConvertYUV400ToYUV422_skip || LSU1.ldih i10, mvcvConvertYUV400ToYUV422_skip
LSU0.ldil i13, 0x3f || LSU1.ldih i13, 0x0
LSU0.ldil i11, 0x40 || LSU1.ldih i11, 0x0 || IAU.AND i12 i16 i13

cmu.cmii.i32 i16 i11
peu.pc1c lt || bru.jmp i10


cmu.cpzv v7 3
cmu.cpzv v11 3
cmu.cpzv v15 3
cmu.cpzv v19 3
cmu.cpzv v23 0
cmu.cpivr.x32 v23 i15 || IAU.SHR.u32 i16 i16 6



LSU0.LDi.128.U8.U16 v22 i18 
LSU0.LDi.128.U8.U16 v21 i18
LSU0.LDi.128.U8.U16 v20 i18
LSU0.LDi.128.U8.U16 v19 i18
LSU0.LDi.128.U8.U16 v18 i18
LSU0.LDi.128.U8.U16 v17 i18
LSU0.LDi.128.U8.U16 v16 i18
LSU0.LDi.128.U8.U16 v15 i18
vau.or v14 v23 v22 || LSU0.LDi.128.U8.U16 v22 i18|| bru.rpl i14 i16
vau.or v13 v23 v21 || LSU0.LDi.128.U8.U16 v21 i18
vau.or v12 v23 v20 || LSU0.LDi.128.U8.U16 v20 i18
vau.or v11 v23 v19 || LSU0.LDi.128.U8.U16 v19 i18
vau.or v10 v23 v18 || LSU0.LDi.128.U8.U16 v18 i18
vau.or  v9 v23 v17 || LSU0.LDi.128.U8.U16 v17 i18
vau.or  v8 v23 v16 || LSU0.LDi.128.U8.U16 v16 i18
vau.or  v7 v23 v15 || LSU0.LDi.128.U8.U16 v15 i18
lsu0.STO.64.L v14 i17  0 || lsu1.STO.64.H v14 i17  8 || IAU.ADD i17 i17 16 
mvcvConvertYUV400ToYUV422_loop:

lsu0.STO.64.L v13 i17  0 || lsu1.STO.64.H v13 i17  8 || IAU.ADD i17 i17 16 
lsu0.STO.64.L v12 i17  0 || lsu1.STO.64.H v12 i17  8 || IAU.ADD i17 i17 16 
lsu0.STO.64.L v11 i17  0 || lsu1.STO.64.H v11 i17  8 || IAU.ADD i17 i17 16 
lsu0.STO.64.L v10 i17  0 || lsu1.STO.64.H v10 i17  8 || IAU.ADD i17 i17 16 
lsu0.STO.64.L  v9 i17  0 || lsu1.STO.64.H  v9 i17  8 || IAU.ADD i17 i17 16 
lsu0.STO.64.L  v8 i17  0 || lsu1.STO.64.H  v8 i17  8 || IAU.ADD i17 i17 16 
lsu0.STO.64.L  v7 i17  0 || lsu1.STO.64.H  v7 i17  8 || IAU.ADD i17 i17 16 



iau.sub i18 i18 i11
CMU.CMZ.i32  i12
peu.pc1c eq || bru.jmp i30


mvcvConvertYUV400ToYUV422_skip:
LSU0.LDi.128.U8.U16 v22 i18

LSU0.LDi.128.U8.U16 v21 i18
LSU0.LDi.128.U8.U16 v20 i18
LSU0.LDi.128.U8.U16 v19 i18
LSU0.LDi.128.U8.U16 v18 i18
LSU0.LDi.128.U8.U16 v17 i18
LSU0.LDi.128.U8.U16 v16 i18
LSU0.LDi.128.U8.U16 v15 i18
vau.or v14 v23 v22
vau.or v13 v23 v21
vau.or v12 v23 v20
vau.or v11 v23 v19
vau.or v10 v23 v18
vau.or  v9 v23 v17
vau.or  v8 v23 v16
vau.or  v7 v23 v15

lsu0.STO.64.L v14 i17  0 || lsu1.STO.64.H v14 i17  8 || IAU.ADD i17 i17 16 
IAU.incs i12 -8
peu.pc1i gt || lsu0.STO.64.L v13 i17  0 || lsu1.STO.64.H v13 i17  8 || IAU.ADD i17 i17 16 
IAU.incs i12 -8
peu.pc1i gt || lsu0.STO.64.L v12 i17  0 || lsu1.STO.64.H v12 i17  8 || IAU.ADD i17 i17 16 
IAU.incs i12 -8
peu.pc1i gt || lsu0.STO.64.L v11 i17  0 || lsu1.STO.64.H v11 i17  8 || IAU.ADD i17 i17 16 
IAU.incs i12 -8
peu.pc1i gt || lsu0.STO.64.L v10 i17  0 || lsu1.STO.64.H v10 i17  8 || IAU.ADD i17 i17 16 
IAU.incs i12 -8
peu.pc1i gt || lsu0.STO.64.L  v9 i17  0 || lsu1.STO.64.H  v9 i17  8 || IAU.ADD i17 i17 16 
IAU.incs i12 -8
peu.pc1i gt || lsu0.STO.64.L  v8 i17  0 || lsu1.STO.64.H  v8 i17  8 || IAU.ADD i17 i17 16 
IAU.incs i12 -8
peu.pc1i gt || lsu0.STO.64.L  v7 i17  0 || lsu1.STO.64.H  v7 i17  8 || IAU.ADD i17 i17 16 

bru.jmp i30 
NOP 6    
.nowarnend
.size mvcvConvertYUV400ToYUV422_asm,.-mvcvConvertYUV400ToYUV422_asm
.end
