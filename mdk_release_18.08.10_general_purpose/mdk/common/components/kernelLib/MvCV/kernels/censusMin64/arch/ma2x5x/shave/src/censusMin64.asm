.type mvcvCensusMin64_asm,@function
.version 00.50.00

//////
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
/// 
/// mvcvCensusMatching64 
//     @param[in] in1            - pointer to input lines of the left image
//     @param[in] in2            - pointer to input lines of the right image
//     @param[out] out           - array of disparity cost
//     @param[in] width          - width of the input lines
//     @return    Nothing




.data .rodata.mvcvCensusMin64_asm
.align 16
	census_data:
		.byte    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
		.byte   16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
		.byte   32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47
		.byte   48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63

	
.code .text.mvcvCensusMin64_asm
.align 16

.lalign
//mvcvCensusMin64(u8 *in, u8 *out, u8* outm, u32 width)
//                  i18      i17        i16    i15
mvcvCensusMin64_asm:

cmu.cpzv v9 0x3  || LSU0.ldil i11, census64_loop || LSU1.ldih i11, census64_loop
cmu.cpzv v13 0x3 || LSU0.ldil i14, census_data || LSU1.ldih i14, census_data
cmu.cpzv v17 0x3 || lsu0.ldo.64.L v4 i14 0   || lsu1.ldo.64.H v4 i14 8  
cmu.cpzv v21 0x2 || lsu0.ldo.64.L v5 i14 16  || lsu1.ldo.64.H v5 i14 24 
lsu0.ldo.64.L v6 i14 32  || lsu1.ldo.64.H v6 i14 40 
lsu0.ldo.64.L v7 i14 48  || lsu1.ldo.64.H v7 i14 56 
nop 6




lsu0.ldo.64.L v0 i18  0 || lsu1.ldo.64.H v0 i18 8  
lsu0.ldo.64.L v1 i18 16 || lsu1.ldo.64.H v1 i18 24 
lsu0.ldo.64.L v2 i18 32 || lsu1.ldo.64.H v2 i18 40 
lsu0.ldo.64.L v3 i18 48 || lsu1.ldo.64.H v3 i18 56 || IAU.ADD i18 i18 64
nop 6




cmu.cmvv.u8 v0 v1 || bru.rpl i11 i15
PEU.PVL08 LTE || lsu0.cp v9 v4 || cmu.cmvv.u8 v1 v0
PEU.PVL08 LT || lsu0.cp v10 v5
cmu.min.u8 v0 v0 v1 || VAU.OR v11 v9 v10
cmu.cmvv.u8 v2 v3
PEU.PVL08 LTE || lsu0.cp v12 v6 || cmu.cmvv.u8 v3 v2
PEU.PVL08 LT || lsu0.cp v13 v7
cmu.min.u8 v2 v2 v3 || VAU.OR v14 v12 v13
cmu.cmvv.u8 v0 v2
PEU.PVL08 LTE || lsu0.cp v15 v11 || cmu.cmvv.u8 v2 v0
PEU.PVL08 LT || lsu0.cp v16 v14
cmu.min.u8 v0 v0 v2
VAU.OR v17 v15 v16
//***********




cmu.alignvec v18 v0 v0 8    || vau.xor v20 v20 v20
cmu.alignvec v19 v17 v17 8  || vau.xor v21 v21 v21
cmu.cmvv.u8 v0 v18 || vau.xor v23 v23 v23
PEU.PVL08 LTE || lsu0.cp v20 v17 || cmu.cmvv.u8 v18 v0
PEU.PVL08 LT || lsu0.cp v21 v19  || cmu.cpzv v13 0x3
cmu.min.u8 v0 v0 v18
VAU.OR v23 v21 v20
cmu.alignvec v18 v0 v0 4    || vau.xor v20 v20 v20
cmu.alignvec v19 v23 v23 4  || vau.xor v21 v21 v21
cmu.cmvv.u8 v0 v18 || vau.xor v22 v22 v22
PEU.PVL08 LTE || lsu0.cp v20 v23 || cmu.cmvv.u8 v18 v0
PEU.PVL08 LT || lsu0.cp v21 v19
cmu.min.u8 v8 v0 v18
VAU.OR v22 v21 v20
cmu.alignvec v18 v8 v8 2    || vau.xor v20 v20 v20
cmu.alignvec v19 v22 v22 2  || vau.xor v21 v21 v21
cmu.cmvv.u8 v8 v18 || vau.xor v23 v23 v23
PEU.PVL08 LTE || lsu0.cp v20 v22 || cmu.cmvv.u8 v18 v8
PEU.PVL08 LT || lsu0.cp v21 v19  || cmu.cpzv v9 0x3
cmu.min.u8 v8 v8 v18 || lsu0.ldo.64.L v0 i18  0 || lsu1.ldo.64.H v0 i18 8 
VAU.OR v23 v21 v20   || lsu0.ldo.64.L v1 i18 16 || lsu1.ldo.64.H v1 i18 24
cmu.cpzv v17 0x3 
census64_loop:
cmu.cpvi.x16 i0.l  v8.0 || lsu0.ldo.64.L v3 i18 48 || lsu1.ldo.64.H v3 i18 56
cmu.cpvi.x16 i1.l v23.0 || IAU.SHR.u32 i2 i0 8 || lsu0.ldo.64.L v2 i18 32 || lsu1.ldo.64.H v2 i18 40 
cmu.cpzv v21 0x2  || IAU.ADD i18 i18 64
IAU.SHR.u32 i3 i1 8 || cmu.cmii.u8 i2 i0       
peu.pc1c lt || lsu0.cp i1 i3 || lsu1.cp i0 i2 
lsu0.st.8 i1 i17 || IAU.ADD i17 i17 1 || lsu1.st.8 i0 i16 || sau.add.i32 i16 i16 1
nop //avoiding port clash


 

//debug purpose
//LSU0.ldil i28, 0x0 || LSU1.ldih i28, 0x0
//LSU0.ldil i29, 0x1 || LSU1.ldih i29, 0x0
//iau.add i28 i28 1	
//cmu.cmii.i32 i28 i29
//peu.pc1c eq || bru.rpim 0
bru.jmp i30
nop 6
.size mvcvCensusMin64_asm,.-mvcvCensusMin64_asm
.end
