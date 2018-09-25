///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvLUT8to10_asm,@function
.version 00.50.34

//void LookupTable(const u8** src, u16** dest, const u16* lut, u32 width)
//						i18			i17			i16				i15

.data .data.lut8to10
.align 4
.code .text.lut8to10

mvcvLUT8to10_asm:
.lalign
LSU1.LD.32 i18, i18
LSU1.LD.32 i17, i17
cmu.cpiv.x32 v1.2 i26 || lsu0.cp v0.0 i20 || iau.shr.u32 i15 i15 3
cmu.cpiv.x32 v1.3 i27 || lsu0.cp v0.1 i21
cmu.cpiv.x32 v2.0 i28 || lsu0.cp v0.2 i22
cmu.cpiv.x32 v2.1 i29 || lsu0.cp v0.3 i23
cmu.cpiv.x32 v2.2 i30 || lsu0.cp v1.0 i24
cmu.cpiv.x32 v2.3 i31 || lsu0.cp v1.1 i25


lsu1.ldil i30, lookupTable8to10_loop
lsu1.ldih i30, lookupTable8to10_loop
lsu1.ldil i21, 0x3ff
lsu1.ldih i21, 0x0

LSU0.LDi.32.u8.u32 i0, i18
LSU0.LDi.32.u8.u32 i1, i18
LSU0.LDi.32.u8.u32 i2, i18
LSU0.LDi.32.u8.u32 i3, i18
LSU0.LDi.32.u8.u32 i4, i18
LSU0.LDi.32.u8.u32 i5, i18
LSU0.LDi.32.u8.u32 i6, i18
LSU0.LDi.32.u8.u32 i7, i18 || iau.add i8 i0 i16
                              iau.add i8 i0 i8
					          iau.add i9 i1 i16
                              iau.add i9 i1 i9
						      iau.add i10 i2 i16
                              iau.add i10 i2 i10
						      iau.add i11 i3 i16
                              iau.add i11 i3 i11
						      iau.add i12 i4 i16
						      iau.add i12 i4 i12 || lsu0.ld.16 i8 i8                          || LSU1.LDi.32.u8.u32 i0, i18  || bru.rpl i30 i15
						      iau.add i13 i5 i16 || lsu0.ld.16 i9 i9                          || LSU1.LDi.32.u8.u32 i1, i18
						      iau.add i13 i5 i13 || lsu0.ld.16 i10 i10                        || LSU1.LDi.32.u8.u32 i2, i18
						      iau.add i14 i6 i16 || lsu0.ld.16 i11 i11                        || LSU1.LDi.32.u8.u32 i3, i18
						      iau.add i14 i6 i14 || lsu0.ld.16 i12 i12                        || LSU1.LDi.32.u8.u32 i4, i18
						      iau.add i20 i7 i16 || lsu0.ld.16 i13 i13                        || LSU1.LDi.32.u8.u32 i5, i18
						      iau.add i20 i7 i20 || lsu0.ld.16 i14 i14                        || LSU1.LDi.32.u8.u32 i6, i18
						                            lsu0.ld.16 i20 i20 || sau.and i22 i8  i21 || LSU1.LDi.32.u8.u32 i7, i18 || iau.add i8 i0 i16
											                              sau.and i23 i9  i21 || lsu0.sti.16 i22 i17  || iau.add i8 i0 i8
																		  lookupTable8to10_loop:
											                              sau.and i24 i10 i21 || lsu0.sti.16 i23 i17  || iau.add i9 i1 i16
											                              sau.and i25 i11 i21 || lsu0.sti.16 i24 i17  || iau.add i9 i1 i9
											                              sau.and i26 i12 i21 || lsu0.sti.16 i25 i17  || iau.add i10 i2 i16
											                              sau.and i27 i13 i21 || lsu0.sti.16 i26 i17  || iau.add i10 i2 i10
											                              sau.and i28 i14 i21 || lsu0.sti.16 i27 i17  || iau.add i11 i3 i16
											                              sau.and i29 i20 i21 || lsu0.sti.16 i28 i17  || iau.add i11 i3 i11
						                                                                         lsu0.sti.16 i29 i17  || iau.add i12 i4 i16

lsu1.cp  i30 v2.2
BRU.jmp i30
cmu.cpvi.x32 i26 v1.2 || lsu0.cp i20 v0.0
cmu.cpvi.x32 i27 v1.3 || lsu0.cp i21 v0.1
cmu.cpvi.x32 i28 v2.0 || lsu0.cp i22 v0.2
cmu.cpvi.x32 i29 v2.1 || lsu0.cp i23 v0.3
 lsu0.cp i24 v1.0
cmu.cpvi.x32 i31 v2.3 || lsu0.cp i25 v1.1

.size mvcvLUT8to10_asm,.-mvcvLUT8to10_asm
.end
