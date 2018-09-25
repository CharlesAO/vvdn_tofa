///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Down scale by half in each direction wil bilinear (sw) kernel.
///            Fp16 in and out
///

.type mvispScale05BilinHVFp16_asm,@function
.version 00.51.00

.data .data.scale05BilinHVFp16_asm
.align 16

 scale05BilinHVFp16_val:
		.float16	0.25F16

//void scale05BilinHVFp16(half **in, half **out, u32 width)//
//								i18		  i17		i16

.code .text.scale05BilinHVFp16_asm
.align 16

.lalign
mvispScale05BilinHVFp16_asm:
.nowarn 10,11
LSU0.LDIL i4 scale05BilinHVFp16_loop	|| LSU1.LDIH i4 scale05BilinHVFp16_loop
LSU0.LDIL i3 scale05BilinHVFp16_val		|| LSU1.LDIH i3 scale05BilinHVFp16_val
LSU0.LDIL i5 scale05BilinHVFp16_compensate	|| LSU1.LDIH i5 scale05BilinHVFp16_compensate
LSU0.LDIL i7 0x0 || LSU1.LDIH i7 0x0
LSU0.LDIL i6 0x1f || LSU1.LDIH i6 0x0
LSU0.LDI.32 i0 i18
LSU0.LD.32 i17 i17
LSU0.LDI.32 i1 i18
LSU0.LDI.16R v11 i3

IAU.AND	i9 i16 i6
IAU.SHR.u32 i16 i16 4
nop

LSU0.LDO.64.L v0 i0	0	|| LSU1.LDO.64.H v0 i0 8
LSU0.LDO.64.L v1 i0	16  || LSU1.LDO.64.H v1 i0 24   || IAU.INCS i0 0x20
LSU0.LDO.64.L v2 i1	0	|| LSU1.LDO.64.H v2 i1 8
LSU0.LDO.64.L v3 i1	16  || LSU1.LDO.64.H v3 i1 24	|| IAU.INCS i1 0x20
LSU0.LDO.64.L v12 i0 0	|| LSU1.LDO.64.H v12 i0 8
LSU0.LDO.64.L v13 i0 16	|| LSU1.LDO.64.H v13 i0 24	|| IAU.INCS i0 0x20
LSU0.LDO.64.L v14 i1 0	|| LSU1.LDO.64.H v14 i1 8
LSU0.LDO.64.L v15 i1 16	|| LSU1.LDO.64.H v15 i1 24	|| IAU.INCS i1 0x20
nop 3

LSU0.LDO.64.L v0 i0	0	|| LSU1.LDO.64.H v0 i0 8						||VAU.ADD.f16 v5 v0 v2
LSU0.LDO.64.L v1 i0	16  || LSU1.LDO.64.H v1 i0 24   || IAU.INCS i0 0x20	||VAU.ADD.f16 v6 v1 v3
LSU0.LDO.64.L v2 i1	0	|| LSU1.LDO.64.H v2 i1 8						||VAU.ADD.f16 v16 v12 v14
LSU0.LDO.64.L v3 i1	16  || LSU1.LDO.64.H v3 i1 24	|| IAU.INCS i1 0x20	||VAU.ADD.f16 v17 v13 v15
LSU0.LDO.64.L v12 i0 0	|| LSU1.LDO.64.H v12 i0 8
LSU0.LDO.64.L v13 i0 16	|| LSU1.LDO.64.H v13 i0 24	|| IAU.INCS i0 0x20	||CMU.VDILV.x16 v7 v8 v5 v6
LSU0.LDO.64.L v14 i1 0	|| LSU1.LDO.64.H v14 i1 8						||CMU.VDILV.x16 v18 v19 v16 v17
LSU0.LDO.64.L v15 i1 16	|| LSU1.LDO.64.H v15 i1 24	|| IAU.INCS i1 0x20
nop 3

LSU0.LDO.64.L v0 i0	0	|| LSU1.LDO.64.H v0 i0 8						||VAU.ADD.f16 v5 v0 v2
LSU0.LDO.64.L v1 i0	16  || LSU1.LDO.64.H v1 i0 24   || IAU.INCS i0 0x20	||VAU.ADD.f16 v6 v1 v3
LSU0.LDO.64.L v2 i1	0	|| LSU1.LDO.64.H v2 i1 8						||VAU.ADD.f16 v16 v12 v14
LSU0.LDO.64.L v3 i1	16  || LSU1.LDO.64.H v3 i1 24	|| IAU.INCS i1 0x20	||VAU.ADD.f16 v17 v13 v15
LSU0.LDO.64.L v12 i0 0	|| LSU1.LDO.64.H v12 i0 8													||VAU.ADD.f16 v9 v7 v8
LSU0.LDO.64.L v13 i0 16	|| LSU1.LDO.64.H v13 i0 24	|| IAU.INCS i0 0x20	||CMU.VDILV.x16 v7 v8 v5 v6	||VAU.ADD.f16 v20 v18 v19
LSU0.LDO.64.L v14 i1 0	|| LSU1.LDO.64.H v14 i1 8						||CMU.VDILV.x16 v18 v19 v16 v17
LSU0.LDO.64.L v15 i1 16	|| LSU1.LDO.64.H v15 i1 24	|| IAU.INCS i1 0x20
nop 3

LSU0.LDO.64.L v0 i0	0	|| LSU1.LDO.64.H v0 i0 8						||VAU.ADD.f16 v5 v0 v2
LSU0.LDO.64.L v1 i0	16  || LSU1.LDO.64.H v1 i0 24   || IAU.INCS i0 0x20	||VAU.ADD.f16 v6 v1 v3
LSU0.LDO.64.L v2 i1	0	|| LSU1.LDO.64.H v2 i1 8						||VAU.ADD.f16 v16 v12 v14
LSU0.LDO.64.L v3 i1	16  || LSU1.LDO.64.H v3 i1 24	|| IAU.INCS i1 0x20	||VAU.ADD.f16 v17 v13 v15
LSU0.LDO.64.L v12 i0 0	|| LSU1.LDO.64.H v12 i0 8													||VAU.ADD.f16 v9 v7 v8
LSU0.LDO.64.L v13 i0 16	|| LSU1.LDO.64.H v13 i0 24	|| IAU.INCS i0 0x20	||CMU.VDILV.x16 v7 v8 v5 v6	||VAU.ADD.f16 v20 v18 v19
LSU0.LDO.64.L v14 i1 0	|| LSU1.LDO.64.H v14 i1 8						||CMU.VDILV.x16 v18 v19 v16 v17		||VAU.MUL.f16 v10 v9 v11
LSU0.LDO.64.L v15 i1 16	|| LSU1.LDO.64.H v15 i1 24	|| IAU.INCS i1 0x20										||VAU.MUL.f16 v21 v20 v11

CMU.CMII.i32 i16 i7
PEU.PC1C lte || bru.jmp i5
nop 6

bru.rpl i4 i16 			||LSU0.LDO.64.L v0 i0	0	|| LSU1.LDO.64.H v0 i0 8
LSU0.LDO.64.L v1 i0	16  || LSU1.LDO.64.H v1 i0 24   || IAU.INCS i0 0x20	||VAU.ADD.f16 v5 v0 v2
LSU0.LDO.64.L v2 i1	0	|| LSU1.LDO.64.H v2 i1 8						||VAU.ADD.f16 v6 v1 v3
 scale05BilinHVFp16_loop:
LSU0.LDO.64.L v3 i1	16  || LSU1.LDO.64.H v3 i1 24	|| IAU.INCS i1 0x20	||VAU.ADD.f16 v16 v12 v14
LSU0.LDO.64.L v12 i0 0	|| LSU1.LDO.64.H v12 i0 8						||VAU.ADD.f16 v17 v13 v15
LSU0.LDO.64.L v13 i0 16	|| LSU1.LDO.64.H v13 i0 24	|| IAU.INCS i0 0x20								||VAU.ADD.f16 v9 v7 v8
LSU0.LDO.64.L v14 i1 0	|| LSU1.LDO.64.H v14 i1 8						||CMU.VDILV.x16 v7 v8 v5 v6	||VAU.ADD.f16 v20 v18 v19
LSU0.LDO.64.L v15 i1 16	|| LSU1.LDO.64.H v15 i1 24	|| IAU.INCS i1 0x20	||CMU.VDILV.x16 v18 v19 v16 v17		||VAU.MUL.f16 v10 v9 v11
LSU0.STO.64.L v10 i17 0	|| LSU1.STO.64.H v10 i17 8	                      									||VAU.MUL.f16 v21 v20 v11
LSU0.STO.64.L v21 i17 16|| LSU1.STO.64.H v21 i17 24	|| IAU.ADD i17 i17 32


 scale05BilinHVFp16_compensate:
nop 4
iau.sub i9, i9, 0x16
peu.pc1i GTE ||LSU0.STO.64.L v10 i17 0	||LSU1.STO.64.H v10 i17 8 ||iau.sub i9, i9, 0x16
peu.pc1i GTE ||LSU0.STO.64.L v21 i17 16 ||LSU1.STO.64.H v21 i17 24 


bru.jmp i30
nop 6
.nowarnend
.size mvispScale05BilinHVFp16_asm,.-mvispScale05BilinHVFp16_asm
.end
