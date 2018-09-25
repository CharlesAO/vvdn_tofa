///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief 
///

//--------------------------------------------------------------------------------------------------------------
.type mvispLumaFusionFp16ToFp16_asm,@function
.version 00.50.37.8
.data .rodata.lumaFusionFp16ToFp16_asm 
.align 4
//.align 16

.code .text.lumaFusionFp16ToFp16_asm //text
.lalign
//void mvispLumaFusionFp16ToFp16(half* inLumaShort, half* inLumaLong, half* outputAlpha, half* outputLumaFusion, u32 width, LumaFusionParam *params)
//                                     i18                i17                  i16                i15               i14               i13
mvispLumaFusionFp16ToFp16_asm:
lsu0.ldo.32 i0 i13 0   //float expRat//
lsu0.ldo.32 i1 i13 4   //float eps//
lsu0.ldo.32 i2 i13 8   //float lslope1//
lsu0.ldo.32 i3 i13 12  //float lslope2//
nop 3
cmu.cpii.f32.f16 i0 i0 //float expRat//
cmu.cpii.f32.f16 i1 i1 //float eps//
cmu.cpii.f32.f16 i2 i2 //float lslope2//
cmu.cpii.f32.f16 i3 i3 //float lslope2//
nop
cmu.cpivr.x16 v0 i0 || lsu0.ldil i5 0x3c00
cmu.cpivr.x16 v20 i1 || lsu0.ldil i4 .LLumaFusionFp16ToFp16 || SAU.RCP i0 i0.l//eps
cmu.cpivr.x16 v3 i2  || lsu1.ldih i4 .LLumaFusionFp16ToFp16//lslope1
cmu.cpivr.x16 v4 i3  || lsu0.ldil i6 0x398C//lslope2
cmu.cpivr.x16 v5 i5  || vau.sub.f16 v6 v4 v3 // (lslope2-lslope1)//1fp16
cmu.cpivr.x16 v22 i6 || iau.sub i7 i18 0
cmu.cpivr.x16 v13 i0 || iau.shr.u32 i14 i14 3//1/erat

SAU.RCP i0 v6.0
SAU.RCP i0 v6.1
SAU.RCP i0 v6.2
SAU.RCP i0 v6.3
SAU.RCP i0 v6.4 
SAU.RCP i0 v6.5 || cmu.cpiv.x16 v7.0 i0.l
SAU.RCP i0 v6.6 || cmu.cpiv.x16 v7.1 i0.l
SAU.RCP i0 v6.7 || cmu.cpiv.x16 v7.2 i0.l
                   cmu.cpiv.x16 v7.3 i0.l
                   cmu.cpiv.x16 v7.4 i0.l
                   cmu.cpiv.x16 v7.5 i0.l
                   cmu.cpiv.x16 v7.6 i0.l
                   cmu.cpiv.x16 v7.7 i0.l //slope

vau.xor v10 v10 v10
lsu0.ldo.64.l v1 i7 0 || lsu1.ldo.64.h v1 i7 8  || iau.add i7 i7 16
lsu0.ldo.64.l v14 i17 0 || lsu1.ldo.64.h v14 i17 8  || iau.add i17 i17 16
nop 6
vau.mul.f16 v2 v1 v0 //inLumaShort
nop 2
vau.sub.f16 v8 v2 v3 // (lumaShort - lslope1)
nop 2
vau.mul.f16 v11 v7 v8 //* slope =>alpha 
nop 2
vau.sub.f16 v12 v5 v11 //1.0f-alphaHalf
nop 2

bru.rpl i4 i14 
cmu.cmvv.f16 v2 v4 || vau.xor v10 v10 v10 || lsu0.ldo.64.l v1 i7 0 || lsu1.ldo.64.h v1 i7 8  || iau.add i7 i7 16
PEU.PVL016 lt || lsu0.cp v10 v12 || cmu.cmvv.f16 v2 v3 || lsu1.ldo.64.l v14 i17 0
PEU.PVL016 lt || lsu0.cp v10 v5  || cmu.cpvv v19 v2    || lsu1.ldo.64.h v14 i17 8  || iau.add i17 i17 16
vau.sub.f16 v15 v5 v10 //1-alpha
vau.mul.f16 v16 v10 v14 //inLumaLong[i] * alpha
nop 
vau.mul.f16 v17 v15 v19  //inLumaShort[i]*(1-alpha)
vau.mul.f16 v2 v1 v0 //inLumaShort
nop 
vau.add.f16 v18 v16 v17
vau.sub.f16 v8 v2 v3 // (lumaShort - lslope1)
nop 
vau.add.f16 v21 v18 v20 //temp
vau.mul.f16 v11 v7 v8 //* slope =>alpha 
nop 

SAU.LOG2 i0 v21.0
SAU.LOG2 i0 v21.1 || vau.sub.f16 v12 v5 v11 //1.0f-alphaHalf
SAU.LOG2 i0 v21.2
SAU.LOG2 i0 v21.3
SAU.LOG2 i0 v21.4 
SAU.LOG2 i0 v21.5 || cmu.cpiv.x16 v21.0 i0.l
SAU.LOG2 i0 v21.6 || cmu.cpiv.x16 v21.1 i0.l
SAU.LOG2 i0 v21.7 || cmu.cpiv.x16 v21.2 i0.l
                     cmu.cpiv.x16 v21.3 i0.l
                     cmu.cpiv.x16 v21.4 i0.l
                     cmu.cpiv.x16 v21.5 i0.l
                     cmu.cpiv.x16 v21.6 i0.l
					 .LLumaFusionFp16ToFp16:
					 cmu.cpiv.x16 v21.7 i0.l 
					 
vau.mul.f16 v23 v21 v22                                     
nop 2
lsu0.sto.64.l v23 i15 0 || lsu1.sto.64.h v23 i15 8 || iau.add i15 i15 16 
lsu0.sto.64.l v10 i16 0 || lsu1.sto.64.h v10 i16 8 || iau.add i16 i16 16 
lsu0.sto.64.l v19 i18 0 || lsu1.sto.64.h v19 i18 8 || iau.add i18 i18 16 

	
bru.jmp i30
nop 6

.size mvispLumaFusionFp16ToFp16_asm,.-mvispLumaFusionFp16ToFp16_asm
.end
	
	 
		

	
	
