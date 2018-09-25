///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Up scale by two in each direction with bilinear (sw) kernel.
///            Fp16 in and out
///

//--------------------------------------------------------------------------------------------------------------
.type mvispScale2xBilinHV_Fp16ToFp16_odd_asm,@function
.version 00.50.0
.data .rodata.mvispScale2xBilinHV_Fp16ToFp16_odd_asm 
.align 16

//void mvispScale2xBilinHV_Fp16ToFp16_odd_asm(half** in, half** out, u32 inWidth)//
//                                               i18        i17        i16       
    
.code .text.Scale2xBilinHV_Fp16ToFp16_odd_asm 
mvispScale2xBilinHV_Fp16ToFp16_odd_asm:
.nowarn 10
lsu0.ld.64 i0 i18 || lsu1.ldil i3 .Lloop_scale  || iau.shr.i32 i16 i16 3
lsu0.ld.32 i17 i17 || lsu1.ldih i3 .Lloop_scale 
nop 6
lsu0.ldo.64.l v0 i0 0 || lsu1.ldo.64.h v0 i0 8 || iau.add i0 i0 8
lsu0.ldo.64.l v1 i1 0 || lsu1.ldo.64.h v1 i1 8 || iau.add i1 i1 8
nop 6
cmu.cpvv v4 v0 || LSU0.SWZC8 [07654321]
cmu.cpvv v5 v1 || LSU0.SWZC8 [07654321] 
vau.add.f16 v3 v0 v1 
nop 2
vau.add.f16 v6 v4 v5  || bru.rpl i3 i16  
lsu0.ldo.64.l v0 i0 0 || lsu1.ldo.64.h v0 i0 8 || iau.add i0 i0 8
lsu0.ldo.64.l v1 i1 0 || lsu1.ldo.64.h v1 i1 8 || iau.add i1 i1 8
vau.add.f16 v7 v6 v3
nop 2
vau.mul.f16 v8 v3 0.5
nop 2
vau.mul.f16 v9 v7 0.5 || cmu.cpvv v4 v0 || LSU0.SWZC8 [07654321]
.Lloop_scale:
cmu.cpvv v5 v1 || LSU0.SWZC8 [07654321] 
vau.add.f16 v3 v0 v1 
vau.mul.f16 v9 v9 0.5
nop 2
cmu.vilv.x16 v10 v11 v9 v8 
lsu0.sto.64.l v10 i17 0 || lsu1.sto.64.h v10 i17 8 || iau.add i17 i17 16
 
bru.jmp i30
nop 6
.nowarnend
.size mvispScale2xBilinHV_Fp16ToFp16_odd_asm,.-mvispScale2xBilinHV_Fp16ToFp16_odd_asm
.end
    
