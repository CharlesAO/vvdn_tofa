///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Up scale by two in each direction with bilinear (sw) kernel.
///            U8 in and out
///

//--------------------------------------------------------------------------------------------------------------
.type mvispScale2xBilinHV075_Fp16U8_asm,@function
.version 00.50.0
.data .rodata.mvispScale2xBilinHV075_Fp16U8_asm 
.align 16
scale2xBilinHV075_vect:                            //wt[2][4] = {
        .float16 0.1875, 0.5625, 0.0625, 0.1875    //{3.0/16, 9.0/16, 1.0/16, 3.0/16},
        .float16 0.5625, 0.1875, 0.1875, 0.0625    //{9.0/16, 3.0/16, 3.0/16, 1.0/16},}

//void mvispScale2xBilinHV075_Fp16U8_asm(half** in, u8** out, u32 inWidth)//
//                          		             i18        i17        i16       
	
.code .text.scale2xBilinHV075_Fp16U8_asm 
mvispScale2xBilinHV075_Fp16U8_asm:
	.nowarn 10
	lsu0.ldil i2 scale2xBilinHV075_vect || lsu1.ldih i2 scale2xBilinHV075_vect
	lsu0.ldil i3 scale2xBilinHV075_loop || lsu1.ldih i3 scale2xBilinHV075_loop
	lsu0.ld.32 i0 i18  || iau.add i18 i18 4
	lsu0.ld.32 i1 i18
	lsu0.ld.32 i17 i17
	lsu0.ldo.64.l v20 i2 0 || lsu1.ldo.64.h v20 i2 8 
	iau.shr.u32 i11 i16 4
	nop 2
	
	iau.sub i0 i0 0x02
	iau.sub i1 i1 0x02
	lsu0.ldo.64.l v0 i0 0  || lsu1.ldo.64.h v0 i0 8  || iau.add i0 i0 16
	lsu0.ldo.64.l v1 i1 0  || lsu1.ldo.64.h v1 i1 8  || iau.add i1 i1 16
	lsu0.ldo.64.l v18 i0 0 || lsu1.ldo.64.h v18 i0 8 || iau.add i0 i0 16
	lsu0.ldo.64.l v19 i1 0 || lsu1.ldo.64.h v19 i1 8 || iau.add i1 i1 16
	nop 4	
	
    CMU.CMZ.i32 i11
	PEU.PCXC EQ,0|| BRU.BRA .Lcompensate
	nop 6
	
	vau.mul.f16 v2 v20 v0  || lsu0.swzv8 [00000000]  || bru.rpl i3 i11
	vau.mul.f16 v3 v20 v1  || lsu0.swzv8 [22222222]  || cmu.alignvec v21 v0 v18 2
    vau.mul.f16 v9 v20 v21 || lsu0.swzv8 [44444444]  || cmu.alignvec v22 v1 v19 2 	
	vau.mul.f16 v4 v20 v21 || lsu0.swzv8 [11111111] 
	vau.mul.f16 v5 v20 v22 || lsu0.swzv8 [33333333] 
	vau.mul.f16 v10 v20 v22|| lsu0.swzv8 [66666666] 
	vau.add.f16 v6 v2 v3   || cmu.alignvec v0 v0 v18 4
	vau.add.f16 v7 v4 v5   || cmu.alignvec v1 v1 v19 4
    vau.mul.f16 v11 v20 v0 || lsu0.swzv8 [55555555] 
	vau.mul.f16 v12 v20 v1 || lsu0.swzv8 [77777777] 
	vau.add.f16 v8 v6 v7
	vau.add.f16 v13 v9 v10
	vau.add.f16 v14 v11 v12
	nop 
	scale2xBilinHV075_loop:
	lsu0.ldo.64.l v18 i0 0  || lsu1.ldo.64.h v18 i0 8 || iau.add i0 i0 16
	vau.add.f16 v15 v13 v14 || lsu0.ldo.64.l v19 i1 0 || lsu1.ldo.64.h v19 i1 8 || iau.add i1 i1 16
	nop 2
	cmu.vilv.x16 v16 v17 v15 v8 
	lsu0.st.128.f16.u8 v16 i17 || iau.add i17 i17 8 || cmu.cpvv v0 v18 
	lsu0.st.128.f16.u8 v17 i17 || iau.add i17 i17 8 || cmu.cpvv v1 v19

    IAU.XOR i12 i12 i12
	IAU.SHR.u32 i12 i16 4
	IAU.MUL i13 i12 16
	nop 2
	CMU.CMII.u32 i13 i16
	PEU.PCXC NEQ,0 || BRU.BRA .Lcompensate
	nop 6
	
	BRU.JMP i30
    nop 6

    
    
.Lcompensate:

    vau.mul.f16 v2 v20 v0  || lsu0.swzv8 [00000000] 
	vau.mul.f16 v3 v20 v1  || lsu0.swzv8 [22222222]  || cmu.alignvec v21 v0 v18 2
    vau.mul.f16 v9 v20 v21 || lsu0.swzv8 [44444444]  || cmu.alignvec v22 v1 v19 2 	
	vau.mul.f16 v4 v20 v21 || lsu0.swzv8 [11111111] 
	vau.mul.f16 v5 v20 v22 || lsu0.swzv8 [33333333] 
	vau.mul.f16 v10 v20 v22|| lsu0.swzv8 [66666666] 
	vau.add.f16 v6 v2 v3   || cmu.alignvec v0 v0 v18 4
	vau.add.f16 v7 v4 v5   || cmu.alignvec v1 v1 v19 4
    vau.mul.f16 v11 v20 v0 || lsu0.swzv8 [55555555] 
	vau.mul.f16 v12 v20 v1 || lsu0.swzv8 [77777777] 
	vau.add.f16 v8 v6 v7
	vau.add.f16 v13 v9 v10
	vau.add.f16 v14 v11 v12
	nop 
	lsu0.ldo.64.l v18 i0 0  || lsu1.ldo.64.h v18 i0 8 || iau.add i0 i0 16
	vau.add.f16 v15 v13 v14 || lsu0.ldo.64.l v19 i1 0 || lsu1.ldo.64.h v19 i1 8 || iau.add i1 i1 16
	nop 2
	cmu.vilv.x16 v16 v17 v15 v8 
	lsu0.st.128.f16.u8 v16 i17 || iau.add i17 i17 8 || cmu.cpvv v0 v18 
	
	BRU.JMP i30
	nop 6
	
	.nowarnend
.size mvispScale2xBilinHV075_Fp16U8_asm,.-mvispScale2xBilinHV075_Fp16U8_asm
.end
	
