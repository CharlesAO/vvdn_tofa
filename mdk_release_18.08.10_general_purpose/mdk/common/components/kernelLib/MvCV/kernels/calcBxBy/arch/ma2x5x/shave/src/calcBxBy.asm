///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvCalcBxBy_asm,@function
.version 0.50.00
.nowarn
// i30 : return pointer
// i19 : stack pointer
// i18 : pointer to patchI
// i17 : pointer to Ix
// i16 : pointer to Iy
// i15 : pointer to patchJ
// i14 : pointer to error
// v23 : isz
// v22 : minI
// v21 : jsz
// v20 : minJ
.code .text.mvcvCalcBxBy
.lalign
mvcvCalcBxBy_asm:
iau.incs i19 -8
iau.incs i19 -8	|| lsu1.st.64 i28 i19
iau.incs i19 -8	|| lsu1.st.64 i26 i19
iau.incs i19 -8	|| lsu1.st.64 i24 i19
iau.incs i19 -8	|| lsu1.st.64 i22 i19
                   lsu1.st.64 i20 i19

// use jsz.x as increment, calculate { minJ.x - minI.x, minJ.y - minI.y }
CMU.CPVI.x32 i6,  v21.0	|| VAU.SUB.i32 v0,  v20, v22 	|| LSU0.LDIL i29, CalcBxBy_generic
//  isz.x used as increment
CMU.CPVI.x32 i5,  v23.0 || IAU.SUBSU i7, i6, 0x0C 		|| LSU0.LDIH i29, CalcBxBy_generic
CMU.CPVI.x32 i9,  v0.1
LSU0.LDIL i29, CalcBxBy12 || LSU1.LDIH i29, CalcBxBy12  || PEU.PC1I LT
// pj = patchJ// calculate (minJ.y - minI.y + 1)
CMU.CPII i2, i15        || IAU.ADD i9, i9, 0x01

 IAU.SUBSU i7, i6, 0x08
 LSU0.LDIL i29, CalcBxBy8  || LSU1.LDIH i29, CalcBxBy8  || PEU.PCIX.EQ 0x30
// calculate (minJ.y - minI.y + 1)*isz.x
IAU.MUL i9, i9, i5		|| lsu0.ldil i25 ___endCalcBxBy
// (isz.x-2) used as increment
CMU.CPVI.x32 i10, v0.0  || SAU.SUB.i32 i7,  i5,  0x02
// calculate (minJ.x - minI.x + 1)
IAU.ADD i10, i10, 0x01	|| lsu0.ldih i25 ___endCalcBxBy
// calculate ((minJ.y - minI.y + 1)*isz.x+(minJ.x - minI.x + 1))// shift (isz.x-2) as data type is float
CMU.CPVI.x32 i10, v0.0  || IAU.ADD i1,  i9,  i10 || SAU.SHL.X32 i7, i7, 0x02
// pi = patchI + ( minJ.y - minI.y + 1) * isz.x + minJ.x - minI.x + 1// shift (minJ.x - minI.x) as data types is float
CMU.CPVI.x32 i9,  v0.1  || IAU.ADD i1, i1, i18 || SAU.SHL.X32 i10, i10, 0x02 || BRU.JMP i29
// calculate (minJ.y - minI.y) * (isz.x - 2)
IAU.MUL i9 i9 i7
// jsz.y used as counter
CMU.CPVI.x32 i0 v21.1 || SAU.XOR i21 i21 i21 || LSU0.CP i24, i1
// calculate (( minJ.y - minI.y) * (isz.x - 2) + minJ.x - minI.x)
IAU.ADD i3 i9 i10 || SAU.XOR i20 i20 i20 || LSU0.CP i8, i6 || LSU1.LD.128.U8.U16 v4 i1 // load pi[x]
// ix = Ix + (minJ.y - minI.y) * (isz.x - 2) + minJ.x - minI.x
IAU.ADD i3 i3 i17 || SAU.XOR i12 i12 i12 || LSU1.LD.128.U8.U16 v6 i2 // load pj[x]
// iy = Iy + (ix - Ix)//
IAU.SUB i4 i3 i17 || SAU.XOR i11 i11 i11 || LSU0.CP i9, i3
IAU.ADD i4 i4 i16 || SAU.XOR i13 i13 i13




CalcBxBy_generic:
IAU.ADD i10 i4 0
SAU.XOR i16 i16 i16 || LSU0.LDI.64.L v12 i3 || LSU1.LDI.64.L v16 i4
SAU.XOR i17 i17 i17 || LSU0.LDI.64.H v12 i3 || LSU1.LDI.64.H v16 i4
SAU.XOR i18 i18 i18 || LSU0.LDI.64.L v13 i3 || LSU1.LDI.64.L v17 i4 || IAU.XOR i23 i23  i23
SAU.XOR i15 i15 i15 || LSU0.LDI.64.H v13 i3 || LSU1.LDI.64.H v17 i4 || IAU.XOR i22 i22  i22

.lalign
calcBxBy_gloop:
LSU0.LDIL i23 0x8
CMU.MIN.i32 i23 i6 i23  || VAU.SUB.I16 v8 v4 v6
SAU.ADD.f32 i18 i18 i11 || LSU0.LDIL i22 0x1
SAU.ADD.f32 i17 i17 i12 || IAU.SHL i22 i22 i23
SAU.ADD.f32 i16 i16 i13 || IAU.SUB i22 i22 0x1
SAU.ADD.f32 i18 i18 i15 || CMU.CMASK v0 i22 0x1 || IAU.SUB i6 i6 i23

CMU.CMASK v1 i22 0x2 || VAU.AND v8 v8 v0   || IAU.ADD i1 i1 i23
IAU.SHR.i32 i22 i22 4  || VAU.AND v12 v12 v1 || CMU.CMZ.I32 i6
PEU.PC1C NEQ || BRU.BRA calcBxBy_continue_loop

SAU.ADD.f32 i17 i17 i20 || CMU.CMASK v2 i22 0x2 || IAU.ADD  i2 i2 i23
VAU.AND v13 v13 v2 || CMU.CPVV.i16.f32 v8 v8
VAU.AND v16 v16 v1 || CMU.CPVV.i16.f32 v9 v8 	|| LSU0.SWZMC4.WORD [1032] [UUUU]
VAU.MUL.f32 v12 v12 v8
VAU.AND v17 v17 v2
VAU.MUL.f32 v13 v13 v9

//jump to next row
IAU.ADD i3 i9 i7 || SAU.ADD.i32 i4 i10 i7  || LSU0.CP i6 i8
IAU.ADD i1 i24 i5
IAU.SUB i0 i0 1  || LSU0.CP i9, i3 || LSU1.CP i10 i4 || CMU.CPII i24 i1

.lalign
calcBxBy_continue_loop:
VAU.MUL.f32 v17 v17 v9  || CMU.CMZ.I32 i0
PEU.PC1C GT || SAU.ADD.f32 i16 i16, i21 || BRU.BRA calcBxBy_gloop
VAU.MUL.f32 v16 v16 v8 || SAU.SUMX.F32 i11 v12   || LSU0.LD.128.U8.U16 v4 i1 || LSU1.LD.128.U8.U16 v6 i2
VAU.MUL.f32 v8 v8 v8     || SAU.SUMX.F32 i15 v13   || LSU0.LDI.64.L v12 i3     || LSU1.LDI.64.L v16 i4
VAU.MUL.f32 v9 v9 v9	 || SAU.SUMX.F32 i20 v17   || LSU0.LDI.64.H v12 i3     || LSU1.LDI.64.H v16 i4
SAU.SUMX.F32 i12 v16
SAU.SUMX.F32 i13 v8      || LSU0.LDI.64.L v13 i3 	|| LSU1.LDI.64.L v17 i4
SAU.SUMX.F32 i21 v9      || LSU0.LDI.64.H v13 i3    || LSU1.LDI.64.H v17 i4

nop 2
SAU.ADD.f32 i16 i16 i21
SAU.ADD.f32 i18 i18 i11
SAU.ADD.f32 i17 i17 i12
SAU.ADD.f32 i16 i16 i13 || BRU.BRA ___endCalcBxBy
SAU.ADD.f32 i18 i18 i15
SAU.ADD.f32 i17 i17 i20
SAU.ADD.f32 i16 i16 i21
// i0 will be used as counter
CMU.CPiV.x32 v23.0 i18
CMU.CPiV.x32 v23.1 i17
LSU1.ST.32 i16 i14




// handle line widths up to 8 pixels
.lalign
CalcBxBy8:
// take off
// generate mask for end of row, read pi[x] pj[x]
iau.sub i10 i6 4
IAU.FEXTU i10  i10  0x00, 0x03
sau.xor i18 i18 i18 || IAU.FEXTU i9  i6  0x00, 0x04 	|| LSU1.LDI.128.U8.U16 v4 i1
sau.xor i17 i17 i17 	|| IAU.XOR i8 i8 i8       			|| LSU1.LDI.128.U8.U16 v6 i2
// read ix[x]
sau.xor i16 i16 i16 	|| IAU.ADD i8 i8 0x01      || LSU1.LDI.64.L v12 i3
sau.xor i15 i15 i15 	|| IAU.SHL i22 i8 i9        || LSU1.LDI.64.H v12 i3
sau.xor i13 i13 i13  	|| IAU.SUB i22 i22 0x01      || LSU1.LDI.64.L v13 i3
sau.xor i12 i12 i12 	|| IAU.SUB i5 i5 0x08      || LSU1.LDI.64.H v13 i3
// read iy[x]
sau.xor i11 i11 i11 	|| IAU.SUB i6 i6 0x08      || LSU1.LDI.64.L v16 i4
sau.xor i20 i20 i20 	|| IAU.SUB i7 i7 0x20      || LSU1.LDI.64.H v16 i4
lsu0.cp i21 i20 	|| IAU.ADD i1 i1 i5        || LSU1.LDI.64.L v17 i4 || VAU.SUB.i16 v8 v4 v6
// lock mask for end of row in condition codes of CMU
iau.ADD i2 i2 i6    || LSU1.LDI.64.H v17, i4 || CMU.CMASK v0, i22 0x1
IAU.SHL i22 i8 i10
IAU.SUB i22 i22 0x01
CMU.CMASK v1, i22 0x2
.lalign
CalcBxBy8_Loop:
// warp zone
// calculate t0 = pi[x] - pj[x], read pi[x] pj[x]
VAU.AND V8 V8 V0			|| LSU1.LDI.128.U8.U16 v4, i1
VAU.AND V13 V13 V1 ||			   LSU1.LDI.128.U8.U16 v6, i2
SAU.ADD.f32 i18 i18 i12     || CMU.CPVV.i16.f32 v8,  v8
SAU.ADD.f32 i17 i17 i20  	|| CMU.CPVV.i16.f32 v9,  v8 	|| LSU0.SWZMC4.WORD [1032] [UUUU]
SAU.ADD.f32 i16 i16 i15   	|| IAU.ADD i1 i1 i5

// calculate t0 * ix[x]
VAU.MUL.f32 v12, v12, v8  	|| IAU.ADD i2 i2 i6
VAU.AND V17 V17 V1
SAU.ADD.f32 i17 i17 i21  	|| IAU.ADD i3 i3 i7
VAU.MUL.f32 v13, v13, v9  	|| IAU.ADD i4 i4 i7           	|| LSU1.LDI.64.l v12 i3
// calculate t0 * iy[x]
SAU.ADD.f32 i16 i16 i13  	|| IAU.INCS i0, -1 				|| LSU1.LDI.64.h v12 i3
VAU.MUL.f32 v16, v16, v8  	|| SAU.SUMX.F32 i12 v12 		|| LSU1.LDI.64.L v13 i3
SAU.ADD.f32 i18 i18 i11
VAU.MUL.f32 v17 v17 v9  	|| BRU.BRA CalcBxBy8_Loop 		|| PEU.PCIX.NEQ 0
// column add to get partial results, calculate t0 * t0
VAU.MUL.f32 v8 v8 v8  		|| SAU.SUMX.F32 i11 v13   		|| LSU1.LDI.64.H v13 i3
VAU.MUL.f32 v9 v9 v9  		|| SAU.SUMX.F32 i20 v16   		|| LSU1.LDI.64.l v16 i4
							   SAU.SUMX.F32 i21 v17   		|| LSU1.LDI.64.h v16 i4
VAU.SUB.i16 v8 v4 v6  		|| SAU.SUMX.F32 i15 v8    		|| LSU1.LDI.64.L v17 i4
							   SAU.SUMX.F32 i13 v9  		|| LSU1.LDI.64.H v17 i4
NOP


// landing
NOP // not req'd
SAU.ADD.f32 i18 i18 i12
SAU.ADD.f32 i17 i17 i20
SAU.ADD.f32 i16 i16 i15 || BRU.BRA ___endCalcBxBy
SAU.ADD.f32 i18 i18 i11
SAU.ADD.f32 i17 i17 i21
SAU.ADD.f32 i16 i16 i13
CMU.CPiV.x32 v23.0 i18
CMU.CPiV.x32 v23.1 i17
LSU1.ST.32 i16 i14




//##################################################################################



// handle line widths up to 12 pixels
.lalign
CalcBxBy12:
// take off
// generate mask for end of row, read pi[x]

sau.xor i18 i18 i18 || IAU.FEXTU i9 i6 0x00 0x02 	|| LSU1.LDI.128.U8.U16 v4 i1

sau.xor i17 i17 i17                          		|| LSU1.LDI.128.U8.U16 v5 i1
// read pj[x]
sau.xor i16 i16 i16 || IAU.XOR i8 i8 i8        		|| LSU1.LDI.128.U8.U16 v6 i2
sau.xor i15 i15 i15 || IAU.ADD i8 i8 0x01      		|| LSU1.LDI.128.U8.U16 v7 i2
// read ix[x]
sau.xor i13 i13 i13 || IAU.SHL i8 i8 i9        		|| LSU1.LDI.64.L v12 i3
sau.xor i22 i22 i22 || IAU.SUB i8 i8 0x01      		|| LSU1.LDI.64.H v12 i3
sau.xor i12 i12 i12 || IAU.SUB i5 i5 0x10     		|| LSU1.LDI.64.L v13 i3
sau.xor i11 i11 i11 || IAU.SUB i6 i6 0x10      		|| LSU1.LDI.64.H v13 i3
sau.xor i23 i23 i23 || IAU.SUB i7 i7 0x30      		|| LSU1.LDI.64.L v14 i3
// lock mask for end of row in condition codes of CMU
sau.xor i20 i20 i20 || CMU.CMASK v0 i8 0x1          || LSU1.LDI.64.H v14 i3
// read iy[x],
sau.xor i21 i21 i21 || LSU1.LDI.64.L v16 i4 || CMU.CMASK v1 i8 0x2
sau.xor i24 i24 i24 || LSU1.LDI.64.H v16 i4 // || BRU.BRA CalcBxBy12_Loop
LSU1.LDI.64.L v17 i4
LSU1.LDI.64.H v17 i4
IAU.ADD i1 i1 i5          || LSU1.LDI.64.L v18 i4
IAU.ADD i2 i2 i6          || LSU1.LDI.64.H v18 i4












.lalign
CalcBxBy12_Loop:
// warp zone
// accumulate partial results, calculate t0 = pi[x] - pj[x], read pi[x]
VAU.SUB.i16 v10 v5 v7   	|| LSU1.LDI.128.U8.U16 v4 i1
VAU.SUB.i16 v8 v4 v6   		|| LSU1.LDI.128.U8.U16 v5 i1
SAU.ADD.f32 i18 i18 i12  	|| IAU.ADD i3 i3 i7
SAU.ADD.f32 i17 i17 i20  	|| IAU.ADD i4 i4 i7
SAU.ADD.f32 i16 i16 i15   	|| IAU.ADD i1 i1 i5
VAU.AND V10 V10 V0
VAU.AND v14 V14 V1
VAU.AND V18 V18 V1
// convert to float, read pj[x]
SAU.ADD.f32 i18 i18 i11  	|| CMU.CPVV.i16.f32 v9 v8   	|| LSU0.SWZMC4.WORD [1032] [UUUU]
SAU.ADD.f32 i17 i17 i21  	|| CMU.CPVV.i16.f32 v8 v8   	|| LSU1.LDI.128.U8.U16 v6 i2
SAU.ADD.f32 i16 i16 i13   	|| CMU.CPVV.i16.f32 v10 v10  	|| LSU1.LDI.128.U8.U16 v7 i2
// // calculate t0 * ix[x], read ix[x]
SAU.ADD.f32 i18 i18 i23  									|| LSU1.LDI.64.L v12 i3
VAU.MUL.f32 v12 v12 v8   	|| IAU.ADD i2 i2 i6 			|| LSU1.LDI.64.H v12 i3
SAU.ADD.f32 i17 i17 i24  	|| IAU.SUB i0 i0 0x01           || LSU1.LDI.64.L v13 i3
VAU.MUL.f32 v13 v13 v9   									|| LSU1.LDI.64.H v13 i3
SAU.ADD.f32 i16 i16 i22  									|| LSU1.LDI.64.L v14 i3
VAU.MUL.f32 v14 v14 v10  									|| LSU1.LDI.64.H v14 i3
// column add to get partial results, calculate bx += t0 * ix[x], calculate t0 * iy[x]
VAU.MUL.f32 v16 v16 v8   	|| SAU.SUMX.F32 i12 v12   		|| LSU1.LDI.64.L v16 i4
VAU.MUL.f32 v17 v17 v9   	|| SAU.SUMX.F32 i11 v13   		|| LSU1.LDI.64.H v16 i4
VAU.MUL.f32 v18 v18 v10  	|| SAU.SUMX.F32 i23 v14   		|| LSU1.LDI.64.l v17 i4
							   SAU.SUMX.F32 i20 v16   		|| LSU1.LDI.64.h v17 i4
// calculate by += t0 * iy[x], calculate t0 * t0
VAU.MUL.f32 v8 v8 v8   		|| SAU.SUMX.F32 i21 v17   		|| LSU1.LDI.64.L v18 i4
VAU.MUL.f32 v9 v9 v9   		|| BRU.BRA CalcBxBy12_Loop 		|| PEU.PCIX.NEQ 0
// calculate error += t0 * t0, read iy[x]
VAU.MUL.f32 v10, v10, v10  	|| SAU.SUMX.F32 i24 v18   		|| LSU1.LDI.64.H v18 i4
							   SAU.SUMX.F32 i15 v8
							   SAU.SUMX.F32 i13 v9
							   SAU.SUMX.F32 i22 v10
nop 2

// landing
SAU.ADD.f32 i18 i18 i12
SAU.ADD.f32 i17 i17 i20
SAU.ADD.f32 i16 i16 i15
SAU.ADD.f32 i18 i18 i11
SAU.ADD.f32 i17 i17 i21
SAU.ADD.f32 i16 i16 i13
SAU.ADD.f32 i18 i18 i23
SAU.ADD.f32 i17 i17 i24
SAU.ADD.f32 i16 i16 i22
CMU.CPiV.x32 v23.0 i18
CMU.CPiV.x32 v23.1 i17
LSU1.ST.32 i16 i14



___endCalcBxBy:
lsu1.ldi.64 i20 i19
lsu1.ldi.64 i22 i19
lsu1.ldi.64 i24 i19
lsu1.ldi.64 i26 i19
lsu1.ldi.64 i28 i19 || bru.jmp i30
nop 6

.size mvcvCalcBxBy_asm,.-mvcvCalcBxBy_asm
.nowarnend
.end
