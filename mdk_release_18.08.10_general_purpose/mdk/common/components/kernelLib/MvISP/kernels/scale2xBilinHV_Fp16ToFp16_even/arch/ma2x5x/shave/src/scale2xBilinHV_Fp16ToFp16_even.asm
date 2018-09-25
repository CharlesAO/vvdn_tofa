///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Up scale by two in each direction with bilinear (sw) kernel.
///            Fp16 in and out
///

//--------------------------------------------------------------------------------------------------------------
.type mvispScale2xBilinHV_Fp16ToFp16_even_asm,@function
.version 00.50.0
.data .rodata.mvispScale2xBilinHV_Fp16ToFp16_even_asm 
.align 16

//void mvispScale2xBilinHV_Fp16ToFp16_even_asm(half** in, half** out, u32 outWidth)//
//                                               i18        i17        i16       
    
.code .text.Scale2xBilinHV_Fp16ToFp16_even_asm 
mvispScale2xBilinHV_Fp16ToFp16_even_asm:
lsu0.ld.32 i18 i18 || iau.shr.u32 i16 i16 3 || lsu1.ldil i0 .Lloop_scale 
lsu0.ld.32 i17 i17 || lsu1.ldih i0 .Lloop_scale  
nop 6
lsu0.ldo.64.l v0 i18 0 || lsu1.ldo.64.h v0 i18 8 || iau.add i18 i18 8
nop 6  
cmu.cpvv v1 v0 || LSU0.SWZC8 [07654321] || bru.rpl i0 i16  
vau.add.f16 v2  v1 v0 
.Lloop_scale:
lsu0.ldo.64.l v0 i18 0 || lsu1.ldo.64.h v0 i18 8 || iau.add i18 i18 8
nop 
vau.mul.f16 v3 v2 0.5
nop 2
cmu.vilv.x16 v6 v7 v3 v0 
lsu0.sto.64.l v6 i17 0 || lsu1.sto.64.h v6 i17 8 || iau.add i17 i17 16
bru.jmp i30
nop 6
.nowarnend
.size mvispScale2xBilinHV_Fp16ToFp16_even_asm,.-mvispScale2xBilinHV_Fp16ToFp16_even_asm
.end
    
