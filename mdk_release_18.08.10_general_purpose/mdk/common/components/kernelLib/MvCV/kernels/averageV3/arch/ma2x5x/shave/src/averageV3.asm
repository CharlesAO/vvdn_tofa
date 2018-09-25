// ///
// /// @file
// /// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
// ///            For License Warranty see: common/license.txt
// ///
///// This kernel performs 1 average line from 3 input lines
///// @param[in] in        - 3 Input lines
///// @param[out] out      - 1 Output line
///// @param[in] width     - Width of input line (width%8 = 0)

//--------------------------------------------------------------------------------------------------------------
.type mvcvAverageV3_asm,@function
.version 00.50.05
.data .rodata.vect_mask 
.salign 16

.code .text.AverageV3 //text
.salign 16

.align 16  
//void mvcvAverageV3(u8** in, u8** out, u32 width)
//                         i18      i17       i16
mvcvAverageV3_asm:
.nowarn
.set RECIP_3  0x3555

lsu0.ldi.32 i0 i18 || iau.shr.u32 i3 i16 4
lsu0.ldi.32 i1 i18 || IAU.SHL i4 i3 4	
lsu0.ldi.32 i2 i18 || IAU.SUB i5 i16 i4	
lsu0.ldil i6, RECIP_3
cmu.cpivr.x16 v15, i6
lsu0.ldil i7, 16
nop 3
lsu0.ldi.32 i12 i17 || lsu1.ldil i11 AverageV3loop
                       lsu1.ldih i11 AverageV3loop

lsu0.ldi.64.l v0  i0  i7 || lsu1.ldo.64.h v0  i0  8
lsu0.ldi.64.l v3  i0  i7 || lsu1.ldo.64.h v3  i0 24
lsu0.ldi.64.l v1  i1  i7 || lsu1.ldo.64.h v1  i1  8
lsu0.ldi.64.l v4  i1  i7 || lsu1.ldo.64.h v4  i1 24
lsu0.ldi.64.l v2  i2  i7 || lsu1.ldo.64.h v2  i2  8
lsu0.ldi.64.l v5  i2  i7 || lsu1.ldo.64.h v5  i2 24
nop 2
CMU.CMZ.i32 i3
vau.add.f16 v12 v0 v1  || lsu0.ldi.64.l v0  i0  i7 || lsu1.ldo.64.h v0  i0  8 
vau.add.f16 v6 v3 v4   || lsu0.ldi.64.l v3  i0  i7 || lsu1.ldo.64.h v3  i0  24 
                          lsu0.ldi.64.l v1  i1  i7 || lsu1.ldo.64.h v1  i1  8  || PEU.PCCX.eq 0x0 || BRU.BRA AverageV3loop_compensate
vau.add.f16 v14 v12 v2 || lsu0.ldi.64.l v4  i1  i7 || lsu1.ldo.64.h v4  i1  24
vau.add.f16 v7 v6 v5   || lsu0.ldi.64.l v2  i2  i7 || lsu1.ldo.64.h v2  i2  8 
                          lsu0.ldi.64.l v5  i2  i7 || lsu1.ldo.64.h v5  i2  24 
vau.mul.f16 v16, v14, v15
vau.mul.f16 v17, v7, v15
nop

.lalign
vau.add.f16 v12 v0 v1  || lsu0.ldi.64.l v0  i0  i7 || lsu1.ldo.64.h v0  i0  8  || bru.rpl i11 i3 
vau.add.f16 v6 v3 v4   || lsu0.ldi.64.l v3  i0  i7 || lsu1.ldo.64.h v3  i0  24 
AverageV3loop:
                          lsu0.ldi.64.l v1  i1  i7 || lsu1.ldo.64.h v1  i1  8 
vau.add.f16 v14 v12 v2 || lsu0.ldi.64.l v4  i1  i7 || lsu1.ldo.64.h v4  i1  24 
vau.add.f16 v7 v6 v5   || lsu0.ldi.64.l v2  i2  i7 || lsu1.ldo.64.h v2  i2  8 
                          lsu0.ldi.64.l v5  i2  i7 || lsu1.ldo.64.h v5  i2  24 
vau.mul.f16 v16, v14, v15 || lsu0.sti.64.l v16 i12 i7 || lsu1.sto.64.h v16 i12 8
vau.mul.f16 v17, v7, v15  || lsu0.sti.64.l v17 i12 i7 || lsu1.sto.64.h v17 i12 24
nop

AverageV3loop_compensate:
IAU.SHR.u32 i5 i5 3	
PEU.PC1I gt || lsu0.sto.64.l v16 i12 0 || lsu1.sto.64.h v16 i12 8

bru.jmp i30 
nop 6  
.nowarnend  
.size mvcvAverageV3_asm,.-mvcvAverageV3_asm
.end
