///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvIntegralImageSumU16U32_asm,@function
.version 00.51.04

.data .data.integralImageSumU16U32_asm
.align 4

.code .text.integralImageSumU16U32_asm
.nowarn
//void mvcvIntegralImageSumU16U32_asm(unsigned int* out, unsigned short* in, unsigned int runNr, unsigned int **previsionOutLnPointer, unsigned int width)//
//                                   i18                   i17                 i16                         i15                           i14
mvcvIntegralImageSumU16U32_asm:

cmu.cmz.i32 i16  || LSU0.LDIL i16 integralImageSumU16U32_second || LSU1.LDIH i16 integralImageSumU16U32_second
peu.pc1c neq || bru.jmp i16
nop 6
IAU.SUB i19 i19 4
LSU0.ST.32  i20  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i21  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i22  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i23  i19


lsu0.ldi.128.u16.u32 v1 i17 
lsu0.ldi.128.u16.u32 v2 i17 
IAU.SUB i0 i0 i0
lsu1.st.32 i18 i15 
IAU.SHR.u32 i14 i14 3
LSU0.LDIL i5 integralImageSumU16U32_mainloop || LSU1.LDIH i5 integralImageSumU16U32_mainloop //loop 
nop
LSU0.CP i1 v1.0  
LSU0.CP i2 v1.1  || IAU.ADD i10  i0 i1 
LSU0.CP i3 v1.2  || IAU.ADD i11 i10 i2 || cmu.cpiv v3.0 i10 || lsu1.ldi.128.u16.u32 v1 i17 || bru.rpl i5 i14 
LSU0.CP i4 v1.3  || IAU.ADD i12 i11 i3 || cmu.cpiv v3.1 i11 || lsu1.ldi.128.u16.u32 v2 i17
 integralImageSumU16U32_mainloop:                              
LSU0.CP i6 v2.0  || IAU.ADD i13 i12 i4 || cmu.cpiv v3.2 i12 || LSU1.STO.64.L v3 i18 0 
LSU0.CP i7 v2.1  || IAU.ADD i20 i13 i6 || cmu.cpiv v3.3 i13
LSU0.CP i8 v2.2  || IAU.ADD i21 i20 i7 || cmu.cpiv v4.0 i20 || LSU1.STO.64.H v3 i18 8 || SAU.ADD.i32 i18 i18 16
LSU0.CP i9 v2.3  || IAU.ADD i22 i21 i8 || cmu.cpiv v4.1 i21
                    IAU.ADD i23 i22 i9 || cmu.cpiv v4.2 i22 || LSU1.STO.64.L v4 i18 0
                                          cmu.cpiv v4.3 i23 || LSU0.CP i1 v1.0   || IAU.SUB i0 i23 0
LSU0.STO.64.H v4 i18 8 || SAU.ADD.i32 i18 i18 16 || LSU1.CP i2 v1.1  || IAU.ADD i10  i0 i1 

LSU0.LD.32  i23  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i22  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i21  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i20  i19 || IAU.ADD i19 i19 4

BRU.jmp i30
nop 6

 integralImageSumU16U32_second:
IAU.SUB i19 i19 4
LSU0.ST.32  i20  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i21  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i22  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i23  i19
LSU0.LDIL i5 integralImageSumU16U32_loop || LSU1.LDIH i5 integralImageSumU16U32_loop //loop 
IAU.SHR.u32 i14 i14 3
IAU.SUB i0 i0 i0
lsu0.ld.32  i16 i15
lsu0.st.32  i18 i15
nop 6


lsu0.ldi.128.u16.u32 v1 i17 
lsu0.ldi.128.u16.u32 v2 i17 
LSU0.LDO.64.L v3 i16 0  || LSU1.LDO.64.H v3 i16 8 
LSU0.LDO.64.L v4 i16 16 || LSU1.LDO.64.H v4 i16 24 || IAU.ADD i16 i16 32 
nop 3
cmu.cpvi i1 v1.0 
cmu.cpvi  i2 v1.1 || IAU.ADD i10  i0 i1
LSU0.CP  i3 v1.2  || IAU.ADD i11 i10 i2 || cmu.cpiv v5.0 i10 || lsu1.ldi.128.u16.u32 v1 i17 
LSU0.CP  i4 v1.3  || IAU.ADD i12 i11 i3 || cmu.cpiv v5.1 i11 || bru.rpl i5 i14
LSU0.CP  i6 v2.0  || IAU.ADD i13 i12 i4 || cmu.cpiv v5.2 i12 || lsu1.ldi.128.u16.u32 v2 i17                    
LSU0.CP  i7 v2.1  || IAU.ADD i20 i13 i6 || cmu.cpiv v5.3 i13 || LSU1.LDO.64.L v3 i16 0 
 integralImageSumU16U32_loop:

LSU0.CP  i8 v2.2  || IAU.ADD i21 i20 i7 || cmu.cpiv v6.0 i20 || VAU.ADD.i32 v7 v3 v5 || LSU1.LDO.64.H v3 i16 8 

LSU0.CP i9 v2.3 || IAU.ADD i22 i21 i8 || cmu.cpiv v6.1 i21 || LSU1.LDO.64.L v4 i16 16 || SAU.ADD.i32 i16 i16 24
                   IAU.ADD i23 i22 i9 || cmu.cpiv v6.2 i22 || LSU0.STO.64.L v7 i18 0  || LSU1.STO.64.H v7 i18 8 
                                         cmu.cpiv v6.3 i23 || IAU.SUB i0 i23 0 || LSU0.CP i1 v1.0 || LSU1.LDO.64.H v4 i16 0
										 cmu.cpvi  i2 v1.1 || IAU.ADD i10  i0 i1 || vau.ADD.i32 v8 v4 v6 

LSU0.CP  i3 v1.2  || IAU.ADD i11 i10 i2 || cmu.cpiv v5.0 i10 || lsu1.ldi.128.u16.u32 v1 i17  || SAU.ADD.i32 i16 i16 8
LSU0.STO.64.L v8 i18 16 || LSU1.STO.64.H v8 i18 24 || IAU.ADD i18 i18 32


LSU0.LD.32  i23  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i22  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i21  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i20  i19 || IAU.ADD i19 i19 4

BRU.jmp i30
nop 6

.size mvcvIntegralImageSumU16U32_asm,.-mvcvIntegralImageSumU16U32_asm
.nowarnend
.end
