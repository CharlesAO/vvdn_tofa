.type mvcvComputeCombinedCost3Window_asm,@function
.version 00.50.00

//////
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
/// 



.data .rodata.mvcvComputeCombinedCost3Window_asm
.align 16

.code .text.mvcvComputeCombinedCost3Window_asm
.align 16

.lalign
//mvcvComputeCombinedCost3Window(u8* disparityCost, u8* adCost, u32 alpha, u32 beta, u32 width, u32 normFactor)
//                                i18               i17        i16        i15       i14       i13
mvcvComputeCombinedCost3Window_asm:

lsu0.ldil i0 0x3c00 || lsu1.ldih i0 0x3c00
lsu0.ldil i1 ___computeCombinedCost3WindowLoop || lsu1.ldih i1 ___computeCombinedCost3WindowLoop 
cmu.cpii.i32.f16 i13 i13 
cmu.cpii.i32.f16 i15 i15  
cmu.cpii.i32.f16 i16 i16  || sau.div.f16 i0 i0 i13
nop 7
sau.mul.f16 i16 i16 i0
sau.mul.f16 i15 i15 i0
cmu.cpii i12 i18
cmu.cpivr.x16 v16 i16
cmu.cpivr.x16 v17 i15  || iau.shr.u32 i14 i14 3


lsu0.ldi.128.u8.f16 v0 i12 || lsu1.ldi.128.u8.f16 v1 i17
lsu0.ldi.128.u8.f16 v2 i12 || lsu1.ldi.128.u8.f16 v3 i17
lsu0.ldi.128.u8.f16 v4 i12 || lsu1.ldi.128.u8.f16 v5 i17
lsu0.ldi.128.u8.f16 v6 i12 || lsu1.ldi.128.u8.f16 v7 i17
lsu0.ldi.128.u8.f16 v8 i12 || lsu1.ldi.128.u8.f16 v9 i17
nop 4
vau.mul.f16 v0 v0 v16
vau.mul.f16 v1 v1 v17      
vau.mul.f16 v2 v2 v16     
vau.mul.f16 v3 v3 v17 || bru.rpl i1 i14   
vau.mul.f16 v4 v4 v16
vau.mul.f16 v5 v5 v17
vau.mul.f16 v6 v6 v16 || lsu0.ldi.128.u8.f16 v0 i12 || lsu1.ldi.128.u8.f16 v1 i17
vau.mul.f16 v7 v7 v17 || lsu0.ldi.128.u8.f16 v2 i12 || lsu1.ldi.128.u8.f16 v3 i17
vau.mul.f16 v8 v8 v16 || lsu0.ldi.128.u8.f16 v4 i12 || lsu1.ldi.128.u8.f16 v5 i17
vau.mul.f16 v9 v9 v17 || lsu0.ldi.128.u8.f16 v6 i12 || lsu1.ldi.128.u8.f16 v7 i17
vau.add.f16 v11 v0 v1 || lsu0.ldi.128.u8.f16 v8 i12 || lsu1.ldi.128.u8.f16 v9 i17
___computeCombinedCost3WindowLoop:
vau.add.f16 v12 v2 v3
vau.add.f16 v13 v4 v5
vau.add.f16 v14 v6 v7 || lsu1.st.128.f16.u8 v11 i18   || iau.add i18 i18 0x8
vau.add.f16 v15 v8 v9 || lsu1.st.128.f16.u8 v12 i18   || iau.add i18 i18 0x8
lsu1.st.128.f16.u8 v13 i18   || iau.add i18 i18 0x8 || vau.mul.f16 v0 v0 v16
lsu1.st.128.f16.u8 v14 i18   || iau.add i18 i18 0x8 || vau.mul.f16 v1 v1 v17  
lsu1.st.128.f16.u8 v15 i18   || iau.add i18 i18 0x8 || vau.mul.f16 v2 v2 v16 



bru.jmp i30
nop 6
.size mvcvComputeCombinedCost3Window_asm,.-mvcvComputeCombinedCost3Window_asm
.end
