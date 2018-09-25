.type mvcvComputeCombinedCost3_asm,@function
.version 00.50.00

//////
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
/// 



.data .rodata.mvcvComputeCombinedCost3_asm
.align 16

.code .text.mvcvComputeCombinedCost3_asm
.align 16

.lalign
//mvcvComputeCombinedCost3(u8* disparityCost, u8* adCost, u32 alpha, u32 beta, u32 width, u32 normFactor)
//                                i18               i17        i16        i15       i14       i13
mvcvComputeCombinedCost3_asm:

lsu0.ldil i0 0x3c00 || lsu1.ldih i0 0x3c00
lsu0.ldil i1 ___computeCombinedCost3Loop || lsu1.ldih i1 ___computeCombinedCost3Loop 
cmu.cpii.i32.f16 i13 i13 
cmu.cpii.i32.f16 i15 i15  
cmu.cpii.i32.f16 i16 i16  || sau.div.f16 i0 i0 i13
nop 7
sau.mul.f16 i16 i16 i0
sau.mul.f16 i15 i15 i0
cmu.cpii i12 i18
cmu.cpivr.x16 v9 i16
cmu.cpivr.x16 v10 i15  || iau.shr.u32 i14 i14 3


lsu0.ldi.128.u8.f16 v0 i12 || lsu1.ldi.128.u8.f16 v1 i17
lsu0.ldi.128.u8.f16 v2 i12 || lsu1.ldi.128.u8.f16 v3 i17
lsu0.ldi.128.u8.f16 v4 i12 || lsu1.ldi.128.u8.f16 v5 i17
nop 4
vau.mul.f16 v0 v0 v9
vau.mul.f16 v1 v1 v10    || lsu0.ldi.128.u8.f16 v0 i12   || lsu1.ldi.128.u8.f16 v1 i17  
vau.mul.f16 v2 v2 v9     || lsu0.ldi.128.u8.f16 v2 i12   || lsu1.ldi.128.u8.f16 v3 i17
vau.mul.f16 v3 v3 v10    || lsu0.ldi.128.u8.f16 v4 i12   || lsu1.ldi.128.u8.f16 v5 i17
vau.mul.f16 v4 v4 v9
vau.mul.f16 v5 v5 v10
vau.add.f16 v6 v0 v1
vau.add.f16 v7 v2 v3
vau.add.f16 v8 v4 v5


vau.mul.f16 v0 v0 v9     || lsu1.st.128.f16.u8 v6 i18   || iau.add i18 i18 0x8    || bru.rpl i1 i14
vau.mul.f16 v1 v1 v10    || lsu1.st.128.f16.u8 v7 i18   || iau.add i18 i18 0x8
___computeCombinedCost3Loop:
.nowarn 10
vau.mul.f16 v2 v2 v9     || lsu1.ldi.128.u8.f16 v0 i12  || lsu0.ldi.128.u8.f16 v1 i17  
.nowarnend
vau.mul.f16 v3 v3 v10    || lsu1.st.128.f16.u8 v8 i18   || iau.add i18 i18 0x8
vau.mul.f16 v4 v4 v9     || lsu1.ldi.128.u8.f16 v2 i12  || lsu0.ldi.128.u8.f16 v3 i17
vau.mul.f16 v5 v5 v10
vau.add.f16 v6 v0 v1     || lsu1.ldi.128.u8.f16 v4 i12  || lsu0.ldi.128.u8.f16 v5 i17  
vau.add.f16 v7 v2 v3
vau.add.f16 v8 v4 v5


bru.jmp i30
nop 6
.size mvcvComputeCombinedCost3_asm,.-mvcvComputeCombinedCost3_asm
.end
