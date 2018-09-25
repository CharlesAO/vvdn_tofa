/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvMinKernel64_asm,@function
.version 00.51.04

.data .data.MinKernel64
.align 4

.code .text.MinKernel64

.lalign
//void mvcvMinKernel64_asm_test(unsigned char *input1, unsigned char *input2, unsigned char *output, unsigned char min2, unsigned char penaltyP2)//
//                                                  i18                  i17                     i16                   i15                    i14
mvcvMinKernel64_asm:

lsu0.ldo.64.l v0 i18  0 || lsu1.ldo.64.h v0 i18  8 || iau.xor i0 i0 i0
lsu0.ldo.64.l v1 i18 16 || lsu1.ldo.64.h v1 i18 24 || iau.xor i1 i1 i1
lsu0.ldo.64.l v2 i18 32 || lsu1.ldo.64.h v2 i18 40 || iau.xor i2 i2 i2
lsu0.ldo.64.l v3 i18 48 || lsu1.ldo.64.h v3 i18 56 || iau.xor i3 i3 i3
lsu0.ldil i8 0xff
nop 4

cmu.min.u8 v4 v0 v1 
cmu.min.u8 v5 v2 v3 
nop
cmu.min.u8 v6 v4 v5 
nop
cmu.alignvec v7 v6 v6 8
cmu.min.u8 v8 v6 v7 
nop
cmu.alignvec v9 v8 v8 4
cmu.min.u8 v10 v9 v8 
nop
cmu.cpvv.u8.u16 v11 v10 || lsu0.cp i7 v10.0 || BRU.jmp i30
cmu.cpvi.x16 i1.l v11.1 || iau.and i0 i7 i8
cmu.cpvi.x16 i2.l v11.2 || iau.minu i4 i0 i1
cmu.cpvi.x16 i3.l v11.3 || iau.minu i5 i4 i2
                           iau.minu i6 i5 i3 
lsu0.st.8 i6 i17
nop 
.size mvcvMinKernel64_asm,.-mvcvMinKernel64_asm
.end
