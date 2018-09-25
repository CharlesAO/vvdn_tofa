///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvGaussVx2_fp16_asm,@function
.version 00.50.05



.data .rodata.GaussVx2_fp16_asm
.align 16
 gaussVx2_fp16_vect:
		.float16    0.0625F16, 0.25F16,0.375F16


.code .text.GaussVx2_fp16_asm //text
.align 16

mvcvGaussVx2_fp16_asm:
//void GaussVx2_fp16_asm(inLines, outLines, width)//
//                        i18      i17        i16      
LSU0.LDI.64 i0 i18  
LSU0.ldil i10, gaussVx2_fp16_loop || LSU1.ldih i10, gaussVx2_fp16_loop 
LSU0.ldil i11, gaussVx2_fp16_vect || LSU1.ldih i11, gaussVx2_fp16_vect       
LSU0.LDI.64 i2 i18     || lsu1.ldi.16r v12 i11 
LSU0.LD.32 i4 i18            
LSU0.LD.32 i17 i17     || lsu1.ldi.16r v13 i11 
nop

IAU.SHR.u32  i16 i16 3 || lsu1.ldi.16r v14 i11

LSU0.LDO.64.L v0 i0 0 || LSU1.LDO.64.H v0 i0 8 || IAU.ADD i0 i0 16 
LSU0.LDO.64.L v1 i1 0 || LSU1.LDO.64.H v1 i1 8 || IAU.ADD i1 i1 16 
LSU0.LDO.64.L v3 i3 0 || LSU1.LDO.64.H v3 i3 8 || IAU.ADD i3 i3 16
LSU0.LDO.64.L v4 i4 0 || LSU1.LDO.64.H v4 i4 8 || IAU.ADD i4 i4 16
LSU0.LDO.64.L v2 i2 0 || LSU1.LDO.64.H v2 i2 8 || IAU.ADD i2 i2 16
LSU0.LDO.64.L v0 i0 0 || LSU1.LDO.64.H v0 i0 8 || IAU.ADD i0 i0 16  
LSU0.LDO.64.L v4 i4 0 || LSU1.LDO.64.H v4 i4 8 || IAU.ADD i4 i4 16
LSU0.LDO.64.L v1 i1 0 || LSU1.LDO.64.H v1 i1 8 || IAU.ADD i1 i1 16
LSU0.LDO.64.L v3 i3 0 || LSU1.LDO.64.H v3 i3 8 || IAU.ADD i3 i3 16
LSU0.LDO.64.L v2 i2 0 || LSU1.LDO.64.H v2 i2 8 || IAU.ADD i2 i2 16

vau.add.f16 v5 v0 v4  
vau.add.f16 v6 v1 v3  
vau.mul.f16 v7 v14 v2 
vau.mul.f16 v8 v12 v5   
vau.mul.f16 v9 v13 v6  

LSU1.LDO.64.L v2 i2 0 || LSU0.LDO.64.H v2 i2 8 || IAU.ADD i2 i2 16
vau.add.f16 v5 v0 v4  || LSU0.LDO.64.L v0 i0 0 || LSU1.LDO.64.H v0 i0 8 || IAU.ADD i0 i0 16 || bru.rpl i10 i16
 gaussVx2_fp16_loop:
 .nowarn 10
vau.add.f16 v10 v7 v8 || LSU0.LDO.64.L v4 i4 0 || LSU1.LDO.64.H v4 i4 8 || IAU.ADD i4 i4 16
.nowarnend
vau.mul.f16 v7 v14 v2 || LSU0.LDO.64.L v1 i1 0 || LSU1.LDO.64.H v1 i1 8 || IAU.ADD i1 i1 16
vau.add.f16 v6 v1 v3  || LSU0.LDO.64.L v3 i3 0 
vau.add.f16 v11 v9 v10|| LSU0.LDO.64.H v3 i3 8 ||  IAU.ADD i3 i3 16 
vau.mul.f16 v8 v12 v5 
vau.mul.f16 v9 v13 v6 || LSU1.LDO.64.L v2 i2 0 || LSU0.LDO.64.H v2 i2 8 || IAU.ADD i2 i2 16
LSU0.STO.64.L v11 i17 0 || LSU1.STO.64.H v11 i17 8 || IAU.ADD i17 i17 16


bru.jmp i30
nop 6

	
.size mvcvGaussVx2_fp16_asm,.-mvcvGaussVx2_fp16_asm
.end
