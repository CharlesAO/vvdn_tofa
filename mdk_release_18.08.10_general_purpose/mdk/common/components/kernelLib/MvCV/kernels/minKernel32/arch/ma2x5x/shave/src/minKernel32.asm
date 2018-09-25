/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///
    
.type mvcvMinKernel32_asm,@function
.version 00.51.04

.data .data.MinKernel32
.align 4

.code .text.MinKernel32

.lalign
//void mvcvMinKernel32(u8* in, u8* minVal)
//                       i18      i17
mvcvMinKernel32_asm:

lsu0.ldo.64.l v0 i18  0 || lsu1.ldo.64.h v0 i18  8 
lsu0.ldo.64.l v1 i18 16 || lsu1.ldo.64.h v1 i18 24 
nop 6
cmu.min.u8 v4 v0 v1 
nop
cmu.alignvec v7 v4 v4 8
cmu.min.u8 v8 v4 v7 
nop
cmu.alignvec v9 v8 v8 4
cmu.min.u8 v10 v9 v8 
nop
cmu.alignvec v11 v10 v10 2
cmu.min.u8 v12 v11 v10 
BRU.jmp i30
cmu.alignvec v13 v12 v12 1
cmu.min.u8 v14 v12 v13 
nop
lsu0.cp i0 v14.0
lsu0.st.8 i0 i17
nop
.size mvcvMinKernel32_asm,.-mvcvMinKernel32_asm
.end
