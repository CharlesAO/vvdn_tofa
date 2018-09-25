/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvMinKernel_asm,@function
.version 00.51.04

.data .data.MinKernel
.align 4

.code .text.MinKernel

.lalign
//void mvcvMinKernel, u8* in, u8* minVal, u32 width//
//                     i18       i17          i16
mvcvMinKernel_asm:


IAU.SHR.u32 i16 i16 3
LSU0.ldil i0, 0xffff || LSU1.ldih i0, 0xffff
LSU0.ldil i1, mvcvMinKernel_loop || LSU1.ldih i1, mvcvMinKernel_loop
cmu.cpivr.x32 v0 i0

LSU0.LDi.128.U8.U16 v1 i18
mvcvMinKernel_loop:
bru.rpl i1 i16
LSU0.LDi.128.U8.U16 v1 i18
nop 4
cmu.min.u16 v0 v0 v1 


nop
cmu.alignvec v2 v0 v0 8
cmu.min.u16 v3 v0 v2 
nop
cmu.alignvec v4 v3 v3 4
cmu.min.u16 v5 v3 v4 
BRU.jmp i30
cmu.alignvec v6 v5 v5 2
cmu.min.u16 v7 v5 v6 
nop
lsu0.cp i0 v7.0
lsu0.st.8 i0 i17
nop

.size mvcvMinKernel_asm,.-mvcvMinKernel_asm
.end
