///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///
.type mvcvBilinearInterpolationAligned16u_asm,@function
.version 0.50.1

// i30 : return pointer
// i19 : stack pointer
// i18 : pointer to source
// i17 : step (stride) for source
// i16 : pointer to destination
// i15 : step (stride) for destination
// v23 : window size
// v22 : center coordinates

.code .text.bilinearInterpolationAligned_u16
.lalign
mvcvBilinearInterpolationAligned16u_asm:
.nowarn
iau.incs i19 -8
iau.incs i19 -8	|| lsu1.st.64 i28 i19
iau.incs i19 -8	|| lsu1.st.64 i26 i19
iau.incs i19 -8	|| lsu1.st.64 i24 i19
iau.incs i19 -8	|| lsu1.st.64 i22 i19
                   lsu1.st.64 i20 i19




CMU.CPVI.x32 i3 v23.0 		|| VAU.SHL.X32 v0 v23 0x1F 		|| LSU1.LDIL i0 biInterAligned8
CMU.CPVI.x32 i11 v23.1 	|| sau.sub.i16 i10 i3 0x08 		|| LSU1.LDIH i0 biInterAligned8
LSU0.LDIH i20 0xffff      || LSU1.LDIL i20 0xfff8
LSU0.LDIH i21 0x0         || LSU1.LDIL i21 0x7           || IAU.ADD i9 i18 i17
CMU.CMZ.i32 v0 				    || sau.sub.i16 i10 i10 0      || LSU1.LDIL i1 0x01 || IAU.AND i8 i18 i20

PEU.PVV32 0x01 				    || VAU.SUB.f32 v22, v22, 0.5
PEU.PC1s gte 				 	    || LSU1.LDIL i0 biInterAligned16
PEU.PC1s gte			 		      || LSU1.LDIH i0 biInterAligned16
cmu.cpvi.x32 i4 v22.0		|| IAU.SUB i22 i18 i8 
cmu.cpvi.x32 i5 v22.1		|| sau.frac.f32 i4 i4
sau.frac.f32 i5 i5
cmu.cpiv.x32 v0.1 i4		  || lsu0.cp v0.0 i4 || iau.and i24 i9 i21
cmu.cpiv.x32 v0.3 i5   		|| lsu0.cp v0.2 i5 || iau.and i9 i9 i20
CMU.CPIVR.i32.f32 v1 i1
IAU.ADD i10 i16 0 //
VAU.SUB.f32 v0 v1 v0 // v0 == 1-a // 1-b
IAU.FEXTU i3 i3 0x00 0x03 	|| LSU1.LDIL i2 0x100
IAU.SHL i3 i1 i3
lsu0.ldil i6 256
lsu0.cp i5 v0.3				|| cmu.cpvi.x32 i4 v0.0 // i4 = 1-a//   i5 = 1-b
cmu.cpii.i32.f32 i6 i6 || vau.xor v0 v0 v0
IAU.SUB i3 i3 0x01
sau.mul.f32 i7 i5 i6 //i7 = firstSum
CMU.CMASK v3 i3 0x0  || lsu0.ldil i25 0x04
CMU.CMZ.i8 v3        || lsu0.ldil i26 0x02
cmu.cpii.f32.i16s i14 i7  || lsu0.ldil i27 0x01
CMU.CPIVR.x16 v1 i1
cmu.cpii.i16.f32 i7 i14
nop
sau.sub.f32 i5 i6 i7 //i5 = secondSum
nop
sau.mul.f32 i12 i4 i7	|| LSU1.LDIL i7 0x80
sau.mul.f32 i13 i4 i5	|| cmu.cpii.f32.i16s i5 i5 //i5 secondSum u16
CMU.CPIVR.x16 v12 i7	 	//v12=128
sau.add.f32 i12 i12 0.5 	//i12 = c00
sau.add.f32 i13 i13 0.5 	//i13 = c10
iau.xor i6 i6 i6
cmu.cpii.f32.i16s i12 i12					//i4 = c01
cmu.cpii.f32.i16s i13 i13	|| BRU.JMP i0 	//i5 = c11
iau.sub i4 i14 i12
iau.sub i5 i5 i13 			|| CMU.CPVI.x32 i2 v23.1
cmu.cpivr.x16 v9 i12
cmu.cpivr.x16 v11 i13
cmu.cpivr.x16 v8 i4
cmu.cpivr.x16 v10 i5


// handle line widths up to 8 pixels
.lalign
biInterAligned8:
// take off
iau.sub i13 i17 0 

LSU0.LD.128.U8.U16 v4 i8    || sau.xor i4 i4 i4 || iau.add i12 i8 8
LSU0.LD.128.U8.U16 v14 i12  || iau.add i8 i8 i13 
iau.add i8 i8 i22
IAU.AND i23 i8 i21 
IAU.AND i12 i8 i20
IAU.SUB i11 i11 0x01         	|| LSU0.LDi.128.U8.U16 v5 i12
nop
IAU.SHR.u32 i11 i11 0x01     	|| LSU0.LD.128.U8.U16 v13 i12
iau.add i8 i8 i13 
IAU.AND i24 i8 i21 
IAU.AND i12 i8 i20
LSU0.LD.128.U8.U16 v6 i12    || sau.xor i5 i5 i5 || iau.add i12 i12 8
LSU0.LD.128.U8.U16 v15 i12   || sau.xor i6 i6 i6 
iau.add i8 i8 i13

iau.and i29 i22 i25 
cmu.alignvec v4 v4 v14 8 || vau.or v14 v14 v0 || lsu0.swzv8 [32107654] || peu.pcix.neq 0x1C || iau.and i29 i22 i26 
cmu.alignvec v4 v4 v14 4 || vau.or v14 v14 v0 || lsu0.swzv8 [10765432] || peu.pcix.neq 0x1C || iau.and i29 i22 i27 
cmu.alignvec v4 v4 v14 2 || vau.or v14 v14 v0 || lsu0.swzv8 [07654321] || peu.pcix.neq 0x1C 
CMU.CPVI.X16 i4.l v4.0
CMU.ALIGNVEC v4 v4 v14 2 || iau.and i29 i23 i25 
cmu.alignvec v5 v5 v13 8 || vau.or v13 v13 v0 || lsu0.swzv8 [32107654] || peu.pcix.neq 0x1C || iau.and i29 i23 i26 
cmu.alignvec v5 v5 v13 4 || vau.or v13 v13 v0 || lsu0.swzv8 [10765432] || peu.pcix.neq 0x1C || iau.and i29 i23 i27 
cmu.alignvec v5 v5 v13 2 || vau.or v13 v13 v0 || lsu0.swzv8 [07654321] || peu.pcix.neq 0x1C 
CMU.CPVI.X16 i5.l v5.0
CMU.ALIGNVEC v5 v5 v13 2 || iau.and i29 i24 i25
cmu.alignvec v6 v6 v15 8 || vau.or v15 v15 v0 || lsu0.swzv8 [32107654] || peu.pcix.neq 0x1C || iau.and i29 i24 i26 
cmu.alignvec v6 v6 v15 4 || vau.or v15 v15 v0 || lsu0.swzv8 [10765432] || peu.pcix.neq 0x1C || iau.and i29 i24 i27 
cmu.alignvec v6 v6 v15 2 || vau.or v15 v15 v0 || lsu0.swzv8 [07654321] || peu.pcix.neq 0x1C 
CMU.CPVI.X16 i6.l v6.0   || IAU.AND i22 i8 i21  
CMU.ALIGNVEC v6 v6 v15 2 || IAU.AND i12 i8 i20 

LSU1.LDIL i0 bilInterAligned8_LoopEnd
LSU0.LDIH i0 bilInterAligned8_LoopEnd

nop
CMU.SHLIV.x16 V4 v4 i4    		|| VAU.MACPZ.u16 v4 v8     
CMU.CPVV v4 v5          		  || IAU.ADD i4 i5 0     				|| VAU.MACP.u16 v4 v9
CMU.SHLIV.x16 V5 v5 i5       || VAU.MACP.u16 v5 v10       	|| LSU1.LD.128.U8.U16 v5 i12 || iau.add i12 i12 8
VAU.MACP.u16 v5 v11       		|| LSU1.LD.128.U8.U16 v13 i12 || iau.add i8 i8 i13
IAU.AND i23 i8 i21
CMU.CPVV v5 v4    				    || VAU.MACPW.u16 v16 v12 v1  || IAU.AND i12 i8 i20 
CMU.SHLIV.x16 V5 v5 i5       || VAU.MACPZ.u16 v5 v8        || LSU1.LD.128.U8.U16 v6 i12 || iau.add i12 i12 8
CMU.CPVV v4 v6          		  || IAU.ADD i4 i6 0     				|| VAU.MACP.u16 v5 v9 				|| LSU1.LD.128.U8.U16 v15 i12
CMU.SHLIV.x16 V6 v6  i6      || VAU.MACP.u16 v6 v10        || iau.add i8 i8 i13
VAU.MACP.u16 v6 v11          || iau.add i28 i8 i13 
VAU.MACPW.u16 v17 v12 v1     || iau.add i13 i13 i13

.lalign
biInterAligned8_Loop:
// warp zone
iau.and i29 i22 i25      || BRU.RPL i0 i11 bilInterAligned8_End
nop 2
cmu.alignvec v5 v5 v13 8 || vau.or v13 v13 v0 || lsu0.swzv8 [32107654] || peu.pcix.neq 0x1C || iau.and i29 i22 i26 
cmu.alignvec v5 v5 v13 4 || vau.or v13 v13 v0 || lsu0.swzv8 [10765432] || peu.pcix.neq 0x1C || iau.and i29 i22 i27 
cmu.alignvec v5 v5 v13 2 || vau.or v13 v13 v0 || lsu0.swzv8 [07654321] || peu.pcix.neq 0x1C 
CMU.CPVI.X16 i5.l v5.0    
CMU.ALIGNVEC v5 v5 v13 2 || iau.and i29 i23 i25 
cmu.alignvec v6 v6 v15 8 || vau.or v15 v15 v0 || lsu0.swzv8 [32107654] || peu.pcix.neq 0x1C || iau.and i29 i23 i26 
cmu.alignvec v6 v6 v15 4 || vau.or v15 v15 v0 || lsu0.swzv8 [10765432] || peu.pcix.neq 0x1C || iau.and i29 i23 i27 
cmu.alignvec v6 v6 v15 2 || vau.or v15 v15 v0 || lsu0.swzv8 [07654321] || peu.pcix.neq 0x1C 
CMU.CPVI.X16 i6.l v6.0   || IAU.AND i22 i8 i21 
CMU.ALIGNVEC v6 v6 v15 2 || IAU.AND i12 i8 i20
CMU.SHLIV.x16 V4 v4 i4   || VAU.MACPZ.u16 v4 v8 	|| IAU.AND i23 i28 i21
CMU.CPVV v4 v5          	|| VAU.MACP.u16 v4 v9   || IAU.ADD i4 i5 0 
CMU.SHLIV.x16 V5 v5 i5   || VAU.MACP.u16 v5 v10  || LSU1.LD.128.U8.U16 v5 i12 || iau.add i12 i12 8

bilInterAligned8_LoopEnd:
CMU.VSZM.BYTE  v16 v16 [Z3Z1] 	|| VAU.MACP.u16 v5 v11       		|| LSU1.LD.128.U8.U16 v13 i12 || IAU.AND i12 i28 i20
CMU.CPVV v5 v4          		  || PEU.PVL08 0x6              		  || VAU.MACPW.u16 v16 v12 v1  	 	|| LSU0.STI.128.U16.U8 v16 i10 i15
CMU.SHLIV.x16 V5 v5 i5      	|| VAU.MACPZ.u16 v5 v8        		|| LSU1.LD.128.U8.U16 v6 i12     || iau.add i12 i12 8
CMU.CPVV v4 v6          		  || IAU.ADD i4 i6 0     				    || VAU.MACP.u16 v5 v9        		 
CMU.SHLIV.x16 V6 v6 i6       || VAU.MACP.u16 v6 v10            || iau.add i28 i28 i13  || LSU0.LD.128.U8.U16 v15 i12
CMU.VSZM.BYTE v17 v17 [Z3Z1] || VAU.MACP.u16 v6 v11            || iau.add i8 i8 i13
IAU.FEXTU i3 i2 0 1    			  || PEU.PVL08 0x6              		  || VAU.MACPW.u16 v17 v12 v1   		|| LSU0.STI.128.U16.U8 v17 i10 i15
bilInterAligned8_End:

lsu1.ldi.64 i20 i19
lsu1.ldi.64 i22 i19
lsu1.ldi.64 i24 i19
lsu1.ldi.64 i26 i19
lsu1.ldi.64 i28 i19
iau.and i3 i3 i27 

// landing
PEU.PC1I gt 	|| BRU.JMP i30
PEU.PC1I eq 	|| BRU.JMP i30
nop 2
CMU.VSZM.BYTE v16 v16 [Z3Z1]
PEU.PVL08 0x6 	|| LSU0.STI.128.U16.U8 v16 i10 i15
CMU.VSZM.BYTE v17 v17 [Z3Z1]
PEU.PVL08 0x6 	|| LSU0.ST.128.U16.U8 v17 i10
nop 3


// handle line widths up to 16 pixels
.lalign
biInterAligned16:
//take off

IAU.SUB i13 i17 16          	|| LSU1.LDI.128.U8.U16 v4,  i8
IAU.SUB i12 i15 0x08          || LSU1.LD.128.U8.U16 v14,  i8
IAU.SUB i11 i11 0x01         	|| LSU1.LDI.128.U8.U16 v6,  i9  || LSU0.LDI.128.U8.U16 v5 i8
LSU1.LD.128.U8.U16 v15 i9   	|| iau.xor i4 i4 i4               || LSU0.LDIL i0 bilInterAligned16_LoopEnd
LSU0.LDIH i0 bilInterAligned16_LoopEnd                            || LSU1.LDI.128.U8.U16 v13 i8 i13
nop
nop
LSU1.LDI.128.U8.U16 v7 i9
LSU1.LDI.128.U8.U16 v18 i9 i13
iau.and i29 i22 i25

peu.pc1i gt       || CMU.ALIGNVEC v4 v4 v14 8 || VAU.OR v14 v14 v0 || LSU0.SWZV8 [32107654] 
peu.pcix.neq 0x1C || CMU.ALIGNVEC v5 v5 v13 8 || VAU.OR v13 v13 v0 || LSU0.SWZV8 [32107654] || iau.and i29 i22 i26
peu.pc1i gt       || CMU.ALIGNVEC v4 v4 v14 4 || VAU.OR v14 v14 v0 || LSU0.SWZV8 [10765432] 
peu.pcix.neq 0x1C || CMU.ALIGNVEC v5 v5 v13 4 || VAU.OR v13 v13 v0 || LSU0.SWZV8 [10765432] || iau.and i29 i22 i27
peu.pc1i gt       || CMU.ALIGNVEC v5 v5 v13 2 || VAU.OR v13 v13 v0 || LSU0.SWZV8 [32107654]
peu.pcix.neq 0x1C || CMU.ALIGNVEC v4 v4 v14 2 || VAU.OR v14 v14 v0 || LSU0.SWZV8 [07654321]
CMU.CPVI.X16 i4.l v4.0   || sau.and i29, i24 i26    
CMU.ALIGNVEC v4 v4 v14 2 || iau.sub i22 i22 0

CMU.ALIGNVEC v5 v5 v13 2 || iau.and i29, i24 i25 
cmu.alignvec   v6,  v6 v15 8  || vau.or       v15, v15 v0  || lsu0.swzv8 [32107654] || peu.pc1i neq
cmu.alignvec   v7,  v7 v18 8  || vau.or       v18, v18 v0  || lsu0.swzv8 [32107654] || peu.pcix.neq 0x1C || iau.and i29 i24 i27   
cmu.alignvec   v6,  v6 v15 4  || vau.or       v15, v15 v0  || lsu0.swzv8 [10765432] || peu.pc1s neq
cmu.alignvec   v7,  v7 v18 4  || vau.or       v18, v18 v0  || lsu0.swzv8 [10765432] || peu.pc1s neq  
cmu.alignvec   v6,  v6 v15 2  || vau.or       v15, v15 v0  || lsu0.swzv8 [07654321] || peu.pc1i neq
cmu.alignvec   v7,  v7 v18 2  || vau.or       v18, v18 v0  || lsu0.swzv8 [07654321] || peu.pc1i neq
CMU.CPVI.X16 i6.l v6.0
CMU.ALIGNVEC v6 v6 v15 2 || iau.add i9 i9 i24
CMU.ALIGNVEC v7 v7 v18 2 || IAU.AND i24 i9 i21

cmu.cpvi.x32 i5 v4.3     || IAU.AND i9 i9 i20
CMU.SHLIV.x16 V4 v4 i4   || VAU.MACPZ.u16 v4 v8
CMU.CPVV v4 v6            || VAU.MACP.u16 v4 v9
cmu.cpvi.x32 i7 v4.3
CMU.SHLIV.x16 V4 v4 i6 || VAU.MACP.u16 v4 v10         ||  LSU0.LDI.128.U8.U16 v6,  i9
VAU.MACP.u16 v4 v11    || LSU0.LD.128.U8.U16 v15 i9
CMU.CPVV v4 v6          || IAU.SHR.u32 i5 i5 0x10  			|| VAU.MACPW.u16 v16 v12 v1 || LSU0.LDI.128.U8.U16 v7,  i9
CMU.SHLIV.x16 V5 v5 i5 || IAU.ADD i4 i6 0     				  || VAU.MACPZ.u16 v5 v8       || LSU0.LDI.128.U8.U16 v18,  i9, i13
CMU.CPVV v5 v7          || IAU.SHR.u32 i7 i7 0x10  			|| VAU.MACP.u16 v5 v9
CMU.SHLIV.x16 V5 v5 i7 || VAU.MACP.u16 v5 v10
VAU.MACP.u16 v5 v11
CMU.CPVV v5 v7           || VAU.MACPW.u16 v17 v12 v1 


.lalign
biInterAligned16_Loop:
  // warp zone
cmu.cpvi.x32   i5, v4.3       || bru.rpl i0 i11 bilInterAligned16_End
iau.and i29, i24 i25    // writeback for the macpw above, when assembler introduces a esc_pad because of .lalign
sau.and i29, i24 i26    // writeback for the macpw in the last instruction of the loop
cmu.alignvec   v6,  v6 v15 8  || vau.or       v15, v15 v0  || lsu0.swzv8 [32107654] || peu.pc1i neq
cmu.alignvec   v7,  v7 v18 8  || vau.or       v18, v18 v0  || lsu0.swzv8 [32107654] || peu.pcix.neq 0x1C || iau.and i29 i24 i27   
cmu.alignvec   v6,  v6 v15 4  || vau.or       v15, v15 v0  || lsu0.swzv8 [10765432] || peu.pc1s neq
cmu.alignvec   v7,  v7 v18 4  || vau.or       v18, v18 v0  || lsu0.swzv8 [10765432] || peu.pc1s neq  
cmu.alignvec   v6,  v6 v15 2  || vau.or       v15, v15 v0  || lsu0.swzv8 [07654321] || peu.pc1i neq
cmu.alignvec   v7,  v7 v18 2  || vau.or       v18, v18 v0  || lsu0.swzv8 [07654321] || peu.pc1i neq  
cmu.cpvi.x16 i6.l, v6.0       || iau.add       i9,  i9 i24
cmu.alignvec   v6,  v6 v15 2  
cmu.alignvec   v7,  v7 v18 2  || iau.and      i24,  i9 i21
cmu.shliv.x16  v4,  v4 i4     || vau.macpz.u16      v4 v8  
cmu.cpvv       v4,  v6         || vau.macp.u16       v4 v9
cmu.cpvi.x32   i7, v4.3       || iau.and       i9,  i9 i20
cmu.shliv.x16  v4,  v4 i6     || vau.macp.u16       v4 v10  || lsu1.ldi.128.u8.u16  v6,  i9                          

bilInterAligned16_LoopEnd:                                
cmu.vszm.byte v16, v16 [z3z1] || vau.macp.u16        v4 v11  || lsu1.ld.128.u8.u16  v15,  i9                       
cmu.cpvv       v4,  v6         || vau.macpw.u16 v16, v12 v1  || lsu0.sti.128.u16.u8 v16, i10     || iau.shr.u32 i5, i5 0x10 
cmu.shliv.x16  v5,  v5 i5     || vau.macpz.u16       v5 v8   || lsu1.ldi.128.u8.u16  v7,  i9     || iau.add     i4, i6 0
cmu.cpvv       v5,  v7         || vau.macp.u16        v5 v9   || lsu1.ldi.128.u8.u16 v18,  i9 i13 || iau.shr.u32 i7, i7 0x10 
cmu.shliv.x16  v5,  v5 i7     || vau.macp.u16        v5 v10       
cmu.vszm.byte v17, v17 [z3z1] || vau.macp.u16        v5 v11      
cmu.cpvv       v5,  v7         || vau.macpw.u16 v17, v12 v1  || lsu0.sti.128.u16.u8 v17, i10 i12 || peu.pvl08 0x6
bilInterAligned16_End: 


// landing
lsu1.ldi.64 i20 i19
lsu1.ldi.64 i22 i19
lsu1.ldi.64 i24 i19
lsu1.ldi.64 i26 i19
lsu1.ldi.64 i28 i19
BRU.JMP i30
CMU.VSZM.BYTE  v16, v16, [Z3Z1]
LSU0.STI.128.U16.U8 v16, i10
CMU.VSZM.BYTE  v17, v17, [Z3Z1]
PEU.PVL08 0x6 || LSU0.ST.128.U16.U8 v17, i10
nop 2
.nowarnend
.size mvcvBilinearInterpolationAligned16u_asm,.-mvcvBilinearInterpolationAligned16u_asm
.end
