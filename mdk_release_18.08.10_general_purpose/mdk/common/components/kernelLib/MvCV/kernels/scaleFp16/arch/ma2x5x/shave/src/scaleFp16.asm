///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// This kernel performs the multiplication of one input array with a constant scaling factor in fp16
/// @param[in] in        - Input line
/// @param[out] out      - Output line
/// @param[in] scale     - scaling factor
/// @param[in] width     - Width of input line
///
/// Constraints: width>4 && width%4 == 0


//--------------------------------------------------------------------------------------------------------------
.type mvcvScaleFp16_asm,@function
.version 00.50.05
.salign 16

.code .text.scaleFp16 //text
.salign 16

.align 16  
//void mvcvScaleFp16(u8** in, u8** out, half scale, u32 width)
//                      i18     i17       i16       i15
mvcvScaleFp16_asm:
.nowarn 10,11
lsu0.ldi.32 i0 i18
lsu0.ldi.32 i1 i17
iau.shr.u32 i3 i15 2
cmu.cpivr.x16 v16, i16
nop 3

lsu0.ldi.64.l v0  i0  
lsu0.ldi.64.l v0  i0  
lsu0.ldi.64.l v0  i0  
lsu0.ldi.64.l v0  i0  
lsu0.ldi.64.l v0  i0  
lsu0.ldi.64.l v0  i0  
lsu0.ldi.64.l v0  i0  
vau.mul.f16 v1 v0 v16 || lsu0.ldi.64.l v0 i0  
vau.mul.f16 v1 v0 v16 || lsu0.ldi.64.l v0 i0  
vau.mul.f16 v1 v0 v16 || lsu0.ldi.64.l v0 i0  
vau.mul.f16 v1 v0 v16 || lsu0.ldi.64.l v0 i0 || lsu1.sti.64.l v1, i1 || bru.rpi i3

bru.jmp i30 
nop 6  

.nowarnend  
.size mvcvScaleFp16_asm,.-mvcvScaleFp16_asm
.end
