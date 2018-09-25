///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvConvolution7x7s3hhhh_asm,@function
.version 00.50.01

.data .data.convolution7x7s3hhhh
.align 16

.code .text.convolution7x7s3hhhh

//void Convolution7x7s3hhhh_asm(u8** in(i18), u8** out(i17), half conv[49](i16), u32 inWidth(i15))
mvcvConvolution7x7s3hhhh_asm:
.nowarn 37
    LSU0.LDIL i10 convolution7x7s3hhhh_loop  || LSU1.LDIH i10 convolution7x7s3hhhh_loop
    LSU1.LDi.64 i0 i18
    LSU1.LDi.64 i2 i18
    LSU1.LDi.64 i4 i18
    LSU1.LDi.64 i6 i18

    LSU1.LDo.64.l v14 i16 0x00
    LSU1.LDo.64.h v14 i16 0x08
    LSU1.LDo.64.l v15 i16 0x10       || SAU.XOR i13 i13 i13
    LSU1.LDo.64.h v15 i16 0x18       || SAU.XOR i12 i12 i12
    LSU1.LDo.64.l v16 i16 0x20       || SAU.XOR i11 i11 i11
    LSU1.LDo.64.h v16 i16 0x28
    LSU1.LDo.64.l v17 i16 0x30       || SAU.SUB.i32 i0 i0 16
    LSU1.LDo.64.h v17 i16 0x38       || IAU.SUB i1 i1 16
    LSU0.LDo.64.l v18 i16 0x40                          || LSU1.LDo.64.h v18 i16 0x48       || IAU.SUB i2 i2 16 || SAU.ADD.i32 i12 i12 24
    LSU0.LDo.64.l v19 i16 0x50                          || LSU1.LDo.64.h v19 i16 0x58       || IAU.SUB i3 i3 16
    LSU0.LDo.64.l v20 i16 0x60                          || LSU1.LDo.64.h v20 i16 0x68       || IAU.SUB i4 i4 16
    IAU.SUB i5 i5 16
    IAU.SUB i6 i6 16
    LSU1.LD.32 i17 i17                                  || SAU.DIV.u32 i12 i15 i12

    IAU.XOR i7 i7 i7
    IAU.XOR i8 i8 i8
    IAU.XOR i9 i9 i9
    IAU.ADD i9 i9 i15
    IAU.ADD i8 i8 24
    IAU.SUB i7 i9 16
    SAU.DIV.i32 i7 i7 i8
    
    

    LSU0.LD.64.L  v0  i0    || LSU1.LDO.64.H v0 i0 0x08    || IAU.ADD i0 i0 0x10
    LSU0.LD.64.L  v1  i1    || LSU1.LDO.64.H v1 i1 0x08    || IAU.ADD i1 i1 0x10
    LSU0.LD.64.L  v2  i2    || LSU1.LDO.64.H v2 i2 0x08    || IAU.ADD i2 i2 0x10
    LSU0.LD.64.L  v3  i3    || LSU1.LDO.64.H v3 i3 0x08    || IAU.ADD i3 i3 0x10
    LSU0.LD.64.L  v4  i4    || LSU1.LDO.64.H v4 i4 0x08    || IAU.ADD i4 i4 0x10
    LSU0.LD.64.L  v5  i5    || LSU1.LDO.64.H v5 i5 0x08    || IAU.ADD i5 i5 0x10
    LSU0.LD.64.L  v6  i6    || LSU1.LDO.64.H v6 i6 0x08    || IAU.ADD i6 i6 0x10
    
    LSU0.LD.64.L  v7 i0     || LSU1.LDO.64.H v7 i0 0x08    || IAU.ADD i0 i0 0x10
    LSU0.LD.64.L  v8 i1     || LSU1.LDO.64.H v8 i1 0x08    || IAU.ADD i1 i1 0x10
    LSU0.LD.64.L  v9 i2     || LSU1.LDO.64.H v9 i2 0x08    || IAU.ADD i2 i2 0x10
    LSU0.LD.64.L  v10 i3    || LSU1.LDO.64.H v10 i3 0x08   || IAU.ADD i3 i3 0x10
    LSU0.LD.64.L  v11 i4    || LSU1.LDO.64.H v11 i4 0x08   || IAU.ADD i4 i4 0x10
    LSU0.LD.64.L  v12 i5    || LSU1.LDO.64.H v12 i5 0x08   || IAU.ADD i5 i5 0x10
    LSU0.LD.64.L  v13 i6    || LSU1.LDO.64.H v13 i6 0x08   || IAU.ADD i6 i6 0x10
    LSU0.LD.64.L  v7 i0     || LSU1.LDO.64.H v7 i0 0x08    || IAU.ADD i0 i0 0x10
    nop 2
	
	
	cmu.alignvec v22, v0 , v7, 10 
// start loop													   
_loopStartconvolution7x7s3hhhh_loop:	
.nowarn 10														   
	 VAU.MACPZ.f16 v14,  v22       ||lsu1.swzv8 [00000000] ||cmu.alignvec v22, v0 , v7, 12 || BRU.RPL i10 i12
.nowarnend                        
	VAU.MACP.f16  v14,  v22       ||lsu1.swzv8 [11111111] ||cmu.alignvec v22, v0 , v7, 14
	VAU.MACP.f16  v14,  v22       ||lsu1.swzv8 [22222222] ||cmu.cpvv v0, v7 
	VAU.MACP.f16  v14,  v0        ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v0 , v7 , 2   ||LSU0.LDXV v8  i1 || iau.add i1, i1, 0x10
	VAU.MACP.f16  v14,  v22       ||lsu1.swzv8 [44444444] ||cmu.alignvec v22, v0 , v7 , 4
	VAU.MACP.f16  v14,  v22       ||lsu1.swzv8 [55555555] ||cmu.alignvec v22, v0 , v7 , 6
	VAU.MACP.f16  v14,  v22       ||lsu1.swzv8 [66666666] ||cmu.alignvec v22, v1 , v8 , 10
	VAU.MACP.f16  v14,  v22       ||lsu1.swzv8 [77777777] ||cmu.alignvec v22, v1 , v8 , 12
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [00000000] ||cmu.alignvec v22, v1 , v8 , 14
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [11111111] ||cmu.cpvv v1, v8
	VAU.MACP.f16  v15,  v1        ||lsu1.swzv8 [22222222] ||cmu.alignvec v22, v1 , v8 , 2   ||LSU0.LDXV v9  i2 || iau.add i2, i2, 0x10
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v1 , v8 , 4
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [44444444] ||cmu.alignvec v22, v1 , v8 , 6
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [55555555] ||cmu.alignvec v22, v2 , v9 , 10 
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [66666666] ||cmu.alignvec v22, v2 , v9 , 12
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [77777777] ||cmu.alignvec v22, v2 , v9 , 14
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [00000000] ||cmu.cpvv v2 , v9 
	VAU.MACP.f16  v16,  v2        ||lsu1.swzv8 [11111111] ||cmu.alignvec v22, v2 , v9 , 2   ||LSU0.LDXV v10  i3 || iau.add i3, i3, 0x10
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [22222222] ||cmu.alignvec v22, v2 , v9 , 4
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v2 , v9 , 6
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [44444444] ||cmu.alignvec v22, v3 , v10, 10 
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [55555555] ||cmu.alignvec v22, v3 , v10, 12
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [66666666] ||cmu.alignvec v22, v3 , v10, 14
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [77777777] ||cmu.cpvv v3 , v10
	VAU.MACP.f16  v17,  v3        ||lsu1.swzv8 [00000000] ||cmu.alignvec v22, v3 , v10, 2   ||LSU0.LDXV v11  i4 || iau.add i4, i4, 0x10
	VAU.MACP.f16  v17,  v22       ||lsu1.swzv8 [11111111] ||cmu.alignvec v22, v3 , v10, 4
	VAU.MACP.f16  v17,  v22       ||lsu1.swzv8 [22222222] ||cmu.alignvec v22, v3 , v10, 6
	VAU.MACP.f16  v17,  v22       ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v4 , v11, 10 
	VAU.MACP.f16  v17,  v22       ||lsu1.swzv8 [44444444] ||cmu.alignvec v22, v4 , v11, 12
	VAU.MACP.f16  v17,  v22       ||lsu1.swzv8 [55555555] ||cmu.alignvec v22, v4 , v11, 14
	VAU.MACP.f16  v17,  v22       ||lsu1.swzv8 [66666666] ||cmu.cpvv v4 , v11
	VAU.MACP.f16  v17,  v4        ||lsu1.swzv8 [77777777] ||cmu.alignvec v22, v4 , v11, 2   ||LSU0.LDXV v12  i5 || iau.add i5, i5, 0x10
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [00000000] ||cmu.alignvec v22, v4 , v11, 4
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [11111111] ||cmu.alignvec v22, v4 , v11, 6
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [22222222] ||cmu.alignvec v22, v5 , v12, 10 
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v5 , v12, 12
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [44444444] ||cmu.alignvec v22, v5 , v12, 14
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [55555555] ||cmu.cpvv v5 , v12
	VAU.MACP.f16  v18,  v5        ||lsu1.swzv8 [66666666] ||cmu.alignvec v22, v5 , v12, 2   ||LSU0.LDXV v13  i6 || iau.add i6, i6, 0x10
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [77777777] ||cmu.alignvec v22, v5 , v12, 4
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [00000000] ||cmu.alignvec v22, v5 , v12, 6
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [11111111] ||cmu.alignvec v22, v6 , v13, 10 
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [22222222] ||cmu.alignvec v22, v6 , v13, 12
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v6 , v13, 14
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [44444444] ||cmu.cpvv v6 , v13
	VAU.MACP.f16  v19,  v6        ||lsu1.swzv8 [55555555] ||cmu.alignvec v22, v6 , v13, 2 
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [66666666] ||cmu.alignvec v22, v6 , v13, 4
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [77777777] ||cmu.alignvec v22, v6 , v13, 6
	VAU.MACPW.f16 v23, v20,  v22  ||lsu1.swzv8 [00000000] ||IAU.INCS i15 -8
	
    CMU.CMZ.i32 i13
    CMU.CPVI.x16 i11.l v23.2
    LSU0.STi.16 i11 i17    || PEU.PC1C gt
    CMU.CPVI.x16 i11.l v23.5
    LSU0.STi.16 i11 i17    || PEU.PC1C gt
    nop
    LSU0.LD.64.L v7 i0       || LSU1.LDO.64.H v7 i0 0x08   ||  IAU.ADD i0 i0 0x10
    CMU.ALIGNVEC v22 v0 v7 10 || IAU.INCS i13 1
    nop	
	nop
	
	
	VAU.MACPZ.f16 v14,  v22       ||lsu1.swzv8 [00000000] ||cmu.alignvec v22, v0 , v7, 12                  
	VAU.MACP.f16  v14,  v22       ||lsu1.swzv8 [11111111] ||cmu.alignvec v22, v0 , v7, 14
	VAU.MACP.f16  v14,  v22       ||lsu1.swzv8 [22222222] ||cmu.cpvv v0, v7 
	VAU.MACP.f16  v14,  v0        ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v0 , v7 , 2   ||LSU0.LDXV v8  i1 || iau.add i1, i1, 0x10
	VAU.MACP.f16  v14,  v22       ||lsu1.swzv8 [44444444] ||cmu.alignvec v22, v0 , v7 , 4
	VAU.MACP.f16  v14,  v22       ||lsu1.swzv8 [55555555] ||cmu.alignvec v22, v0 , v7 , 6
	VAU.MACP.f16  v14,  v22       ||lsu1.swzv8 [66666666] ||cmu.alignvec v22, v1 , v8 , 10
	VAU.MACP.f16  v14,  v22       ||lsu1.swzv8 [77777777] ||cmu.alignvec v22, v1 , v8 , 12
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [00000000] ||cmu.alignvec v22, v1 , v8 , 14
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [11111111] ||cmu.cpvv v1, v8
	VAU.MACP.f16  v15,  v1        ||lsu1.swzv8 [22222222] ||cmu.alignvec v22, v1 , v8 , 2   ||LSU0.LDXV v9  i2 || iau.add i2, i2, 0x10
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v1 , v8 , 4
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [44444444] ||cmu.alignvec v22, v1 , v8 , 6
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [55555555] ||cmu.alignvec v22, v2 , v9 , 10 
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [66666666] ||cmu.alignvec v22, v2 , v9 , 12
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [77777777] ||cmu.alignvec v22, v2 , v9 , 14
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [00000000] ||cmu.cpvv v2 , v9 
	VAU.MACP.f16  v16,  v2        ||lsu1.swzv8 [11111111] ||cmu.alignvec v22, v2 , v9 , 2   ||LSU0.LDXV v10  i3 || iau.add i3, i3, 0x10
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [22222222] ||cmu.alignvec v22, v2 , v9 , 4
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v2 , v9 , 6
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [44444444] ||cmu.alignvec v22, v3 , v10, 10 
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [55555555] ||cmu.alignvec v22, v3 , v10, 12
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [66666666] ||cmu.alignvec v22, v3 , v10, 14
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [77777777] ||cmu.cpvv v3 , v10
	VAU.MACP.f16  v17,  v3        ||lsu1.swzv8 [00000000] ||cmu.alignvec v22, v3 , v10, 2   ||LSU0.LDXV v11  i4 || iau.add i4, i4, 0x10
	VAU.MACP.f16  v17,  v22       ||lsu1.swzv8 [11111111] ||cmu.alignvec v22, v3 , v10, 4
	VAU.MACP.f16  v17,  v22       ||lsu1.swzv8 [22222222] ||cmu.alignvec v22, v3 , v10, 6
	VAU.MACP.f16  v17,  v22       ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v4 , v11, 10 
	VAU.MACP.f16  v17,  v22       ||lsu1.swzv8 [44444444] ||cmu.alignvec v22, v4 , v11, 12
	VAU.MACP.f16  v17,  v22       ||lsu1.swzv8 [55555555] ||cmu.alignvec v22, v4 , v11, 14
	VAU.MACP.f16  v17,  v22       ||lsu1.swzv8 [66666666] ||cmu.cpvv v4 , v11
	VAU.MACP.f16  v17,  v4        ||lsu1.swzv8 [77777777] ||cmu.alignvec v22, v4 , v11, 2   ||LSU0.LDXV v12  i5 || iau.add i5, i5, 0x10
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [00000000] ||cmu.alignvec v22, v4 , v11, 4
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [11111111] ||cmu.alignvec v22, v4 , v11, 6
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [22222222] ||cmu.alignvec v22, v5 , v12, 10 
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v5 , v12, 12
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [44444444] ||cmu.alignvec v22, v5 , v12, 14
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [55555555] ||cmu.cpvv v5 , v12
	VAU.MACP.f16  v18,  v5        ||lsu1.swzv8 [66666666] ||cmu.alignvec v22, v5 , v12, 2   ||LSU0.LDXV v13  i6 || iau.add i6, i6, 0x10
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [77777777] ||cmu.alignvec v22, v5 , v12, 4
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [00000000] ||cmu.alignvec v22, v5 , v12, 6
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [11111111] ||cmu.alignvec v22, v6 , v13, 10 
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [22222222] ||cmu.alignvec v22, v6 , v13, 12
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v6 , v13, 14
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [44444444] ||cmu.cpvv v6 , v13
	VAU.MACP.f16  v19,  v6        ||lsu1.swzv8 [55555555] ||cmu.alignvec v22, v6 , v13, 2 
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [66666666] ||cmu.alignvec v22, v6 , v13, 4
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [77777777] ||cmu.alignvec v22, v6 , v13, 6
	VAU.MACPW.f16 v23, v20,  v22  ||lsu1.swzv8 [00000000] || IAU.INCS i15 -8
	
	CMU.CPVI.x16 i11.l v23.0
    LSU0.STi.16 i11 i17
    CMU.CPVI.x16 i11.l v23.3
    LSU0.STi.16 i11 i17
    CMU.CPVI.x16 i11.l v23.6
    LSU0.STi.16 i11 i17

    LSU0.LD.64.L v7 i0        || LSU1.LDO.64.H v7 i0 0x08   ||  IAU.ADD i0 i0 0x10
    CMU.ALIGNVEC v22 v0 v7 10 
    nop	
	nop
	
	VAU.MACPZ.f16 v14,  v22       ||lsu1.swzv8 [00000000] ||cmu.alignvec v22, v0 , v7, 12                  
	VAU.MACP.f16  v14,  v22       ||lsu1.swzv8 [11111111] ||cmu.alignvec v22, v0 , v7, 14
	VAU.MACP.f16  v14,  v22       ||lsu1.swzv8 [22222222] ||cmu.cpvv v0, v7 
	VAU.MACP.f16  v14,  v0        ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v0 , v7 , 2   ||LSU0.LDXV v8  i1 || iau.add i1, i1, 0x10
	VAU.MACP.f16  v14,  v22       ||lsu1.swzv8 [44444444] ||cmu.alignvec v22, v0 , v7 , 4
	VAU.MACP.f16  v14,  v22       ||lsu1.swzv8 [55555555] ||cmu.alignvec v22, v0 , v7 , 6
	VAU.MACP.f16  v14,  v22       ||lsu1.swzv8 [66666666] ||cmu.alignvec v22, v1 , v8 , 10
	VAU.MACP.f16  v14,  v22       ||lsu1.swzv8 [77777777] ||cmu.alignvec v22, v1 , v8 , 12
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [00000000] ||cmu.alignvec v22, v1 , v8 , 14
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [11111111] ||cmu.cpvv v1, v8
	VAU.MACP.f16  v15,  v1        ||lsu1.swzv8 [22222222] ||cmu.alignvec v22, v1 , v8 , 2   ||LSU0.LDXV v9  i2 || iau.add i2, i2, 0x10
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v1 , v8 , 4
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [44444444] ||cmu.alignvec v22, v1 , v8 , 6
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [55555555] ||cmu.alignvec v22, v2 , v9 , 10 
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [66666666] ||cmu.alignvec v22, v2 , v9 , 12
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [77777777] ||cmu.alignvec v22, v2 , v9 , 14
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [00000000] ||cmu.cpvv v2 , v9 
	VAU.MACP.f16  v16,  v2        ||lsu1.swzv8 [11111111] ||cmu.alignvec v22, v2 , v9 , 2   ||LSU0.LDXV v10  i3 || iau.add i3, i3, 0x10
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [22222222] ||cmu.alignvec v22, v2 , v9 , 4
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v2 , v9 , 6
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [44444444] ||cmu.alignvec v22, v3 , v10, 10 
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [55555555] ||cmu.alignvec v22, v3 , v10, 12
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [66666666] ||cmu.alignvec v22, v3 , v10, 14
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [77777777] ||cmu.cpvv v3 , v10
	VAU.MACP.f16  v17,  v3        ||lsu1.swzv8 [00000000] ||cmu.alignvec v22, v3 , v10, 2   ||LSU0.LDXV v11  i4 || iau.add i4, i4, 0x10
	VAU.MACP.f16  v17,  v22       ||lsu1.swzv8 [11111111] ||cmu.alignvec v22, v3 , v10, 4
	VAU.MACP.f16  v17,  v22       ||lsu1.swzv8 [22222222] ||cmu.alignvec v22, v3 , v10, 6
	VAU.MACP.f16  v17,  v22       ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v4 , v11, 10 
	VAU.MACP.f16  v17,  v22       ||lsu1.swzv8 [44444444] ||cmu.alignvec v22, v4 , v11, 12
	VAU.MACP.f16  v17,  v22       ||lsu1.swzv8 [55555555] ||cmu.alignvec v22, v4 , v11, 14
	VAU.MACP.f16  v17,  v22       ||lsu1.swzv8 [66666666] ||cmu.cpvv v4 , v11
	VAU.MACP.f16  v17,  v4        ||lsu1.swzv8 [77777777] ||cmu.alignvec v22, v4 , v11, 2   ||LSU0.LDXV v12  i5 || iau.add i5, i5, 0x10
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [00000000] ||cmu.alignvec v22, v4 , v11, 4
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [11111111] ||cmu.alignvec v22, v4 , v11, 6
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [22222222] ||cmu.alignvec v22, v5 , v12, 10 
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v5 , v12, 12
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [44444444] ||cmu.alignvec v22, v5 , v12, 14
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [55555555] ||cmu.cpvv v5 , v12
	VAU.MACP.f16  v18,  v5        ||lsu1.swzv8 [66666666] ||cmu.alignvec v22, v5 , v12, 2   ||LSU0.LDXV v13  i6 || iau.add i6, i6, 0x10
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [77777777] ||cmu.alignvec v22, v5 , v12, 4
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [00000000] ||cmu.alignvec v22, v5 , v12, 6
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [11111111] ||cmu.alignvec v22, v6 , v13, 10 
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [22222222] ||cmu.alignvec v22, v6 , v13, 12
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v6 , v13, 14
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [44444444] ||cmu.cpvv v6 , v13
	VAU.MACP.f16  v19,  v6        ||lsu1.swzv8 [55555555] ||cmu.alignvec v22, v6 , v13, 2 
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [66666666] ||cmu.alignvec v22, v6 , v13, 4
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [77777777] ||cmu.alignvec v22, v6 , v13, 6
	VAU.MACPW.f16 v23, v20,  v22  ||lsu1.swzv8 [00000000] || IAU.INCS i15 -8
	
    CMU.CPVI.x16 i11.l v23.1
    LSU0.STi.16 i11 i17
    CMU.CPVI.x16 i11.l v23.4
    
convolution7x7s3hhhh_loop:
    LSU0.STi.16 i11 i17
    CMU.CPVI.x16 i11.l v23.7
    LSU0.STi.16 i11 i17
	LSU0.LD.64.L v7 i0      || LSU1.LDO.64.H v7 i0 0x08   ||  IAU.ADD i0 i0 16
    CMU.ALIGNVEC v22 v0 v7 10 
    nop 2

	

////////////////////////Compensate for width = N * 24 + 8 or width = N *24 + 16

    ////////////////////////Store v23.0 v23.3 v23.6
    VAU.MACPZ.f16 v14,  v22       ||lsu1.swzv8 [00000000] ||cmu.alignvec v22, v0 , v7, 12                  
	VAU.MACP.f16  v14,  v22       ||lsu1.swzv8 [11111111] ||cmu.alignvec v22, v0 , v7, 14
	VAU.MACP.f16  v14,  v22       ||lsu1.swzv8 [22222222] ||cmu.cpvv v0, v7 
	VAU.MACP.f16  v14,  v0        ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v0 , v7 , 2   ||LSU0.LDXV v8  i1 || iau.add i1, i1, 0x10
	VAU.MACP.f16  v14,  v22       ||lsu1.swzv8 [44444444] ||cmu.alignvec v22, v0 , v7 , 4
	VAU.MACP.f16  v14,  v22       ||lsu1.swzv8 [55555555] ||cmu.alignvec v22, v0 , v7 , 6
	VAU.MACP.f16  v14,  v22       ||lsu1.swzv8 [66666666] ||cmu.alignvec v22, v1 , v8 , 10
	VAU.MACP.f16  v14,  v22       ||lsu1.swzv8 [77777777] ||cmu.alignvec v22, v1 , v8 , 12
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [00000000] ||cmu.alignvec v22, v1 , v8 , 14
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [11111111] ||cmu.cpvv v1, v8
	VAU.MACP.f16  v15,  v1        ||lsu1.swzv8 [22222222] ||cmu.alignvec v22, v1 , v8 , 2   ||LSU0.LDXV v9  i2 || iau.add i2, i2, 0x10
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v1 , v8 , 4
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [44444444] ||cmu.alignvec v22, v1 , v8 , 6
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [55555555] ||cmu.alignvec v22, v2 , v9 , 10 
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [66666666] ||cmu.alignvec v22, v2 , v9 , 12
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [77777777] ||cmu.alignvec v22, v2 , v9 , 14
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [00000000] ||cmu.cpvv v2 , v9 
	VAU.MACP.f16  v16,  v2        ||lsu1.swzv8 [11111111] ||cmu.alignvec v22, v2 , v9 , 2   ||LSU0.LDXV v10  i3 || iau.add i3, i3, 0x10
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [22222222] ||cmu.alignvec v22, v2 , v9 , 4
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v2 , v9 , 6
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [44444444] ||cmu.alignvec v22, v3 , v10, 10 
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [55555555] ||cmu.alignvec v22, v3 , v10, 12
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [66666666] ||cmu.alignvec v22, v3 , v10, 14
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [77777777] ||cmu.cpvv v3 , v10
	VAU.MACP.f16  v17,  v3        ||lsu1.swzv8 [00000000] ||cmu.alignvec v22, v3 , v10, 2   ||LSU0.LDXV v11  i4 || iau.add i4, i4, 0x10
	VAU.MACP.f16  v17,  v22       ||lsu1.swzv8 [11111111] ||cmu.alignvec v22, v3 , v10, 4
	VAU.MACP.f16  v17,  v22       ||lsu1.swzv8 [22222222] ||cmu.alignvec v22, v3 , v10, 6
	VAU.MACP.f16  v17,  v22       ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v4 , v11, 10 
	VAU.MACP.f16  v17,  v22       ||lsu1.swzv8 [44444444] ||cmu.alignvec v22, v4 , v11, 12
	VAU.MACP.f16  v17,  v22       ||lsu1.swzv8 [55555555] ||cmu.alignvec v22, v4 , v11, 14
	VAU.MACP.f16  v17,  v22       ||lsu1.swzv8 [66666666] ||cmu.cpvv v4 , v11
	VAU.MACP.f16  v17,  v4        ||lsu1.swzv8 [77777777] ||cmu.alignvec v22, v4 , v11, 2   ||LSU0.LDXV v12  i5 || iau.add i5, i5, 0x10
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [00000000] ||cmu.alignvec v22, v4 , v11, 4
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [11111111] ||cmu.alignvec v22, v4 , v11, 6
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [22222222] ||cmu.alignvec v22, v5 , v12, 10 
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v5 , v12, 12
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [44444444] ||cmu.alignvec v22, v5 , v12, 14
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [55555555] ||cmu.cpvv v5 , v12
	VAU.MACP.f16  v18,  v5        ||lsu1.swzv8 [66666666] ||cmu.alignvec v22, v5 , v12, 2   ||LSU0.LDXV v13  i6 || iau.add i6, i6, 0x10
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [77777777] ||cmu.alignvec v22, v5 , v12, 4
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [00000000] ||cmu.alignvec v22, v5 , v12, 6
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [11111111] ||cmu.alignvec v22, v6 , v13, 10 
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [22222222] ||cmu.alignvec v22, v6 , v13, 12
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v6 , v13, 14
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [44444444] ||cmu.cpvv v6 , v13
	VAU.MACP.f16  v19,  v6        ||lsu1.swzv8 [55555555] ||cmu.alignvec v22, v6 , v13, 2 
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [66666666] ||cmu.alignvec v22, v6 , v13, 4
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [77777777] ||cmu.alignvec v22, v6 , v13, 6
	VAU.MACPW.f16 v23, v20,  v22  ||lsu1.swzv8 [00000000] || IAU.INCS i15 -8

    CMU.CPVI.x16 i11.l v23.2
    LSU0.STi.16 i11 i17
    CMU.CPVI.x16 i11.l v23.5
    LSU0.STi.16 i11 i17
    nop 4
    CMU.CMZ.i32 i15
    CMU.CPVI.x16 i11.l v23.0
    LSU0.STi.16 i11 i17   || PEU.PC1C gte
    CMU.CPVI.x16 i11.l v23.3
    LSU0.STi.16 i11 i17   || PEU.PC1C gte
    CMU.CPVI.x16 i11.l v23.6
   
    
	IAU.MUL i7 i7 i8
	nop
    IAU.SUB i9 i9 16
    CMU.CMII.i32 i7 i9
    PEU.PCXC EQ,0 ||BRU.BRA .Lcompensate
    PEU.PCXC LT,0 ||BRU.BRA .Lcompensate
    PEU.PCXC GT,0 ||BRU.BRA .Lcompensate
    nop 6

   BRU.JMP i30
   nop 6
   .nowarnend
.Lcompensate:   
    
    LSU0.STi.16 i11 i17   || PEU.PC1C gte
    LSU0.LD.64.L v7 i0      || LSU1.LDO.64.H v7 i0 0x08   ||  IAU.ADD i0 i0 16
    CMU.ALIGNVEC v22 v0 v7 10 
    nop
    nop	

    //////////////Store v23.1 v23.4 v23.7
    VAU.MACPZ.f16 v14,  v22       ||lsu1.swzv8 [00000000] ||cmu.alignvec v22, v0 , v7, 12                  
	VAU.MACP.f16  v14,  v22       ||lsu1.swzv8 [11111111] ||cmu.alignvec v22, v0 , v7, 14
	VAU.MACP.f16  v14,  v22       ||lsu1.swzv8 [22222222] ||cmu.cpvv v0, v7 
	VAU.MACP.f16  v14,  v0        ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v0 , v7 , 2   ||LSU0.LDXV v8  i1 || iau.add i1, i1, 0x10
	VAU.MACP.f16  v14,  v22       ||lsu1.swzv8 [44444444] ||cmu.alignvec v22, v0 , v7 , 4
	VAU.MACP.f16  v14,  v22       ||lsu1.swzv8 [55555555] ||cmu.alignvec v22, v0 , v7 , 6
	VAU.MACP.f16  v14,  v22       ||lsu1.swzv8 [66666666] ||cmu.alignvec v22, v1 , v8 , 10
	VAU.MACP.f16  v14,  v22       ||lsu1.swzv8 [77777777] ||cmu.alignvec v22, v1 , v8 , 12
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [00000000] ||cmu.alignvec v22, v1 , v8 , 14
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [11111111] ||cmu.cpvv v1, v8
	VAU.MACP.f16  v15,  v1        ||lsu1.swzv8 [22222222] ||cmu.alignvec v22, v1 , v8 , 2   ||LSU0.LDXV v9  i2 || iau.add i2, i2, 0x10
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v1 , v8 , 4
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [44444444] ||cmu.alignvec v22, v1 , v8 , 6
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [55555555] ||cmu.alignvec v22, v2 , v9 , 10 
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [66666666] ||cmu.alignvec v22, v2 , v9 , 12
	VAU.MACP.f16  v15,  v22       ||lsu1.swzv8 [77777777] ||cmu.alignvec v22, v2 , v9 , 14
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [00000000] ||cmu.cpvv v2 , v9 
	VAU.MACP.f16  v16,  v2        ||lsu1.swzv8 [11111111] ||cmu.alignvec v22, v2 , v9 , 2   ||LSU0.LDXV v10  i3 || iau.add i3, i3, 0x10
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [22222222] ||cmu.alignvec v22, v2 , v9 , 4
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v2 , v9 , 6
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [44444444] ||cmu.alignvec v22, v3 , v10, 10 
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [55555555] ||cmu.alignvec v22, v3 , v10, 12
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [66666666] ||cmu.alignvec v22, v3 , v10, 14
	VAU.MACP.f16  v16,  v22       ||lsu1.swzv8 [77777777] ||cmu.cpvv v3 , v10
	VAU.MACP.f16  v17,  v3        ||lsu1.swzv8 [00000000] ||cmu.alignvec v22, v3 , v10, 2   ||LSU0.LDXV v11  i4 || iau.add i4, i4, 0x10
	VAU.MACP.f16  v17,  v22       ||lsu1.swzv8 [11111111] ||cmu.alignvec v22, v3 , v10, 4
	VAU.MACP.f16  v17,  v22       ||lsu1.swzv8 [22222222] ||cmu.alignvec v22, v3 , v10, 6
	VAU.MACP.f16  v17,  v22       ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v4 , v11, 10 
	VAU.MACP.f16  v17,  v22       ||lsu1.swzv8 [44444444] ||cmu.alignvec v22, v4 , v11, 12
	VAU.MACP.f16  v17,  v22       ||lsu1.swzv8 [55555555] ||cmu.alignvec v22, v4 , v11, 14
	VAU.MACP.f16  v17,  v22       ||lsu1.swzv8 [66666666] ||cmu.cpvv v4 , v11
	VAU.MACP.f16  v17,  v4        ||lsu1.swzv8 [77777777] ||cmu.alignvec v22, v4 , v11, 2   ||LSU0.LDXV v12  i5 || iau.add i5, i5, 0x10
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [00000000] ||cmu.alignvec v22, v4 , v11, 4
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [11111111] ||cmu.alignvec v22, v4 , v11, 6
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [22222222] ||cmu.alignvec v22, v5 , v12, 10 
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v5 , v12, 12
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [44444444] ||cmu.alignvec v22, v5 , v12, 14
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [55555555] ||cmu.cpvv v5 , v12
	VAU.MACP.f16  v18,  v5        ||lsu1.swzv8 [66666666] ||cmu.alignvec v22, v5 , v12, 2   ||LSU0.LDXV v13  i6 || iau.add i6, i6, 0x10
	VAU.MACP.f16  v18,  v22       ||lsu1.swzv8 [77777777] ||cmu.alignvec v22, v5 , v12, 4
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [00000000] ||cmu.alignvec v22, v5 , v12, 6
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [11111111] ||cmu.alignvec v22, v6 , v13, 10 
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [22222222] ||cmu.alignvec v22, v6 , v13, 12
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [33333333] ||cmu.alignvec v22, v6 , v13, 14
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [44444444] ||cmu.cpvv v6 , v13
	VAU.MACP.f16  v19,  v6        ||lsu1.swzv8 [55555555] ||cmu.alignvec v22, v6 , v13, 2 
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [66666666] ||cmu.alignvec v22, v6 , v13, 4
	VAU.MACP.f16  v19,  v22       ||lsu1.swzv8 [77777777] ||cmu.alignvec v22, v6 , v13, 6
	VAU.MACPW.f16 v23, v20,  v22  ||lsu1.swzv8 [00000000] || IAU.INCS i15 -8
    nop 8
    CMU.CMZ.i32 i15
    CMU.CPVI.x16 i11.l v23.1
    LSU0.STi.16 i11 i17   || PEU.PC1C gte
    CMU.CPVI.x16 i11.l v23.4
    LSU0.STi.16 i11 i17   || PEU.PC1C gte
    CMU.CPVI.x16 i11.l v23.7
    //LSU0.STi.16 i11 i17   || PEU.PC1C gte

    BRU.JMP i30
nop 6
    NOP 6
.nowarnend
.size mvcvConvolution7x7s3hhhh_asm,.-mvcvConvolution7x7s3hhhh_asm
.end
