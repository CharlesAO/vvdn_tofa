///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvispResizeAlphaFp16ToFp16_asm,@function
.version 00.51.04

//.data .data.ResizeAlphaFp16ToFp16
//.align 4

//.align 16
 //ResizeAlphaFp16ToFp16_clampVal:
  //      .float16 255.0


.code .text.ResizeAlphaFp16ToFp16

//MVCV_FUNC(void, mvispResizeAlphaFp16ToFp16, half** alpha, half* output, u32 width)
//                                                  i18          i17            i16
mvispResizeAlphaFp16ToFp16_asm:
.nowarn 10
lsu0.ld.64 i0 i18// line 1, line 2
nop 6
lsu0.ldil i5 0x3400// 0.25fp16 
lsu0.ldil i3 mvispResizeAlphaFp16ToFp16_asm_loop || lsu1.ldih i3 mvispResizeAlphaFp16ToFp16_asm_loop 
cmu.cpivr.x16 v6 i5
iau.shr.u32 i16 i16 3
lsu0.ldo.64.l v0 i0 0  || lsu1.ldo.64.h v0 i0 8  || iau.add i0 i0 16
lsu0.ldo.64.l v11 i0 0 || lsu1.ldo.64.h v11 i0 8 || iau.add i0 i0 16
lsu0.ldo.64.l v1 i1 0  || lsu1.ldo.64.h v1 i1 8  || iau.add i1 i1 16
lsu0.ldo.64.l v12 i1 0 || lsu1.ldo.64.h v12 i1 8 || iau.add i1 i1 16
nop 6
cmu.vdilv.x16 v2 v3 v0 v11  || lsu0.ldo.64.l v0 i0 0  || lsu1.ldo.64.h v0 i0 8  || iau.add i0 i0 16 
cmu.vdilv.x16 v4 v5 v1 v12  || lsu0.ldo.64.l v11 i0 0 || lsu1.ldo.64.h v11 i0 8 || iau.add i0 i0 16
vau.add.f16 v7 v2 v3        || lsu0.ldo.64.l v1 i1 0  || lsu1.ldo.64.h v1 i1 8  || iau.add i1 i1 16
vau.add.f16 v8 v4 v5        || lsu0.ldo.64.l v12 i1 0 || lsu1.ldo.64.h v12 i1 8 || iau.add i1 i1 16
nop 6

cmu.vdilv.x16 v2 v3 v0 v11  || lsu0.ldo.64.l v0 i0 0  || lsu1.ldo.64.h v0 i0 8  || iau.add i0 i0 16 || bru.rpl i3 i16 
cmu.vdilv.x16 v4 v5 v1 v12  || lsu0.ldo.64.l v11 i0 0 || lsu1.ldo.64.h v11 i0 8 || iau.add i0 i0 16
mvispResizeAlphaFp16ToFp16_asm_loop:
vau.add.f16 v9 v7 v8        || lsu0.ldo.64.l v1 i1 0  || lsu1.ldo.64.h v1 i1 8  || iau.add i1 i1 16 
vau.add.f16 v7 v2 v3        || lsu0.ldo.64.l v12 i1 0 || lsu1.ldo.64.h v12 i1 8 || iau.add i1 i1 16
vau.add.f16 v8 v4 v5        
vau.mul.f16 v10 v9 v6
nop 2
lsu0.sto.64.l v10 i17 0 || lsu1.sto.64.h v10 i17 8 || iau.add i17 i17 16

	BRU.JMP i30
	
	nop 6
.nowarnend
.size mvispResizeAlphaFp16ToFp16_asm,.-mvispResizeAlphaFp16ToFp16_asm
.end
