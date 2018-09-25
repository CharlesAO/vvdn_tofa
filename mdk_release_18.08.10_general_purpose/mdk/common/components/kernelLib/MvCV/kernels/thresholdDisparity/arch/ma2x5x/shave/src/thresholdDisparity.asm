/// @file
/// @copyright Three code copyright Movidius Ltd 2015, Three rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///
    
.type mvcvThresholdDisparity_asm,@function
.version 00.51.04

.data .data.thresholdDisparity
.align 4

.code .text.thresholdDisparity

.lalign
//void, mvcvThresholdDisparity, u8* input, u8* output, u32* countThresPixels, u32 width, u32 threshold, u32 setValue
//                                  i18        i17               i16              i15          i14           i13
mvcvThresholdDisparity_asm:
lsu1.ldi.128.u8.u16 v1 i18
lsu1.ld.32 i4 i16 || vau.xor v6 v6 v6 || lsu0.ldil i0 0x1
.nowarn 10, 37
iau.shr.u32 i15 i15 4 || vau.xor v10 v10 v10 || cmu.cpivr.x16 v7 i0 || lsu0.ldil i3 .LThresholdDisparity_loop
cmu.cpivr.x16 v0 i13  || lsu0.ldih i3 .LThresholdDisparity_loop //setvalue
cmu.cpivr.x16 v8 i14  || iau.sub i1 i17 0//thresh
nop 2

.LThresholdDisparity_loop:
cmu.vilv.x16 v4 v5 v1 v1 || lsu1.ldi.128.u8.u16 v1 i18 || bru.rpl i3 i15// v4 first
cmu.cmvv.u16 v4 v8
.nowarnend
peu.pvv16  gt || vau.add.i16 v6 v6 v7 
PEU.PVL08  gt || lsu0.st.128.u16.u8 v0 i1 || cmu.cmvv.u16 v5 v8 || iau.add i1 i1 8
peu.pvv16  gt || vau.add.i16 v10 v10 v7 
PEU.PVL08  gt || lsu0.st.128.u16.u8 v0 i1 || iau.add i1 i1 8
nop

sau.sumx.u16 i0 v6 || bru.jmp i30   
sau.sumx.u16 i1 v10 
nop 
iau.add i0 i1 i0
iau.add i4 i0 i4
lsu0.st.32 i4 i16
nop
.size mvcvThresholdDisparity_asm,.-mvcvThresholdDisparity_asm
.end
