///   
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved. 
///            For License Warranty see: common/license.txt   
///
/// @brief     Shave common. 
/// 
 
.type mvcvFastAtan2Positive_asm,@function
.version 00.50.05
.code .text.mvcvFastAtan2Positive_asm
.align 16
.nowarn
mvcvFastAtan2Positive_asm:
LSU0.LDIL i1, 0xbc00     || SAU.DIV.F16 i16, i18, i17 || IAU.XOR i2 i2 i2 //-1F16
LSU0.LDIL i9, 0x3a48   || LSU1.LDIL i10, 0xba48 || CMU.CMII.F16 i18, i2
// if x==0 and y == 0 we quit. the result is 0
PEU.PCCX.EQ 0x4 || CMU.CMII.F16 i17, i2   || LSU0.LDIL i1, 0x3c00      || LSU1.cp i14, i1 //1F16
PEU.PCCX.EQ 0x0 || BRU.JMP i30            || LSU0.LDIL i4, 0x3e48   || LSU1.LDIL i6, 0xbe48  
LSU0.LDIL i11, fastAtan2Positive_asm_fastAtan2Positive_asm_map_to_0_2pi  || LSU1.LDIH I11, fastAtan2Positive_asm_fastAtan2Positive_asm_map_to_0_2pi ||  cmu.cpii i7, i1
LSU0.LDIL i12, 0x4248 || LSU1.LDIL i13, 0x4648 || CMU.CPII i5 i18
LSU0.LDIL i8, 0       || LSU1.LDIH i8, 0
nop
//division complete
sau.rcp i15, i16.l 
cmu.CMII.f16 i16, i7
peu.pcxc GT 0 || cmu.vsignf.f16 i1 i1 [PN] || iau.add i8, i8, i4 || sau.rcp i16, i16.l
peu.pcxc EQ 0 || cmu.cpii i18,  i9 || bru.jmp i11
cmu.CMII.f16 i16, i14
peu.pcxc LT 0 || cmu.cpii i16 i15 || iau.add i8, i8, i6 || sau.mul.f16 i1 i1 i14
peu.pcxc EQ 0 || cmu.cpii i18,  i10 || bru.jmp i11
sau.atn i16, i16.l
nop 3
cmu.CMII.f16 i1 i2
peu.pcxc GT 0 || sau.add.f16 i18, i8, i16
peu.pcxc LT 0 || sau.sub.f16 i18, i8, i16
nop 1
 fastAtan2Positive_asm_fastAtan2Positive_asm_map_to_0_2pi:
cmu.CMII.f16 i17, i2
peu.pcxc LT 0 || sau.add.f16 i18, i18, i12 || iau.xor i13 i13 i13
BRU.JMP i30
cmu.CMII.f16 i5, i2
peu.pcxc LT 0 || sau.add.f16 i18, i18, i13
nop 4
.size mvcvFastAtan2Positive_asm,.-mvcvFastAtan2Positive_asm
.nowarnend
.end
