///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    
///

.version 00.51.05
//-------------------------------------------------------------------------------

.code .text.svuGet16BitVals16BitLUT
//ushort8 svuGets16BitVals16BitLUT(short8 in_values(v23), s16* lut_memory(i18))//
//ushort8 svuGetu16BitVals16BitLUT(ushort8 in_values(v23), u16* lut_memory(i18))//
svuGet16BitVals16BitLUT:
svuGets16BitVals16BitLUT:
svuGetu16BitVals16BitLUT:
    //Convert the values to 32 bit, makes it quicker to add the address to all
    cmu.cpivr.x32 v2, i18
	vau.xor v3 v3 v3
	cmu.vilv.x16 v0 v1 v3 v23
	
    // multiply by 2 since we're dealing with 16 bit values
    // the LUT component size is 2 bytes (16 bits)
	VAU.SHL.X32 v0, v0, 1
    VAU.SHL.X32 v1, v1, 1
    vau.add.i32 v0, v0, v2
    vau.add.i32 v1, v1, v2
    cmu.cpvi i0, v0.0
    LSU0.LD.16 i0, i0  || cmu.cpvi i1, v0.1 
    LSU0.LD.16 i1, i1  || cmu.cpvi i2, v0.2 || iau.xor i0, i0, i0
    LSU0.LD.16 i2, i2  || cmu.cpvi i3, v0.3 || iau.xor i1, i1, i1
    LSU0.LD.16 i3, i3  || cmu.cpvi i4, v1.0 || iau.xor i2, i2, i2
    LSU0.LD.16 i4, i4  || cmu.cpvi i5, v1.1 || iau.xor i3, i3, i3
    LSU0.LD.16 i5, i5  || cmu.cpvi i6, v1.2 || iau.xor i4, i4, i4
    LSU0.LD.16 i6, i6  || cmu.cpvi i7, v1.3 || vau.xor v23, v23, v23 || iau.xor i5, i5, i5
    LSU0.LD.16 i7, i7  || cmu.cpiv v23.0, i0 || iau.xor i6, i6, i6
    cmu.cpiv v23.1, i1 || iau.xor i7, i7, i7
    cmu.cpiv v23.2, i2
    cmu.cpiv v23.3, i3 || vau.xor v1, v1, v1
    cmu.cpiv v1.0, i4
    cmu.cpiv v1.1, i5 
    cmu.cpiv v1.2, i6 || bru.jmp i30
    cmu.cpiv v1.3, i7
    cmu.cpvv.u32.u16s v23, v23
    cmu.cpvv.u32.u16s v1, v1
	cmu.vszm.word v23 v1 [10DD] //Myriad2
	nop 2  

.code .text.svuGet8BitVals16BitLUT
//uchar8 svuGet8BitVals16BitLUT(ushort8 in_values(v23), u8* lut_memory(i18))//
//uchar8 svuGet8BitVals16BitLUT(ushort8 in_values(v23), u16* lut_memory(i18))//
svuGet8BitVals16BitLUT:
    //Convert the values to 32 bit, makes it quicker to add the address to all
    cmu.cpivr.x32 v2, i18
	vau.xor v3 v3 v3
	cmu.vilv.x16 v0 v1 v3 v23
    // multiply by 2 since we're dealing with 16 bit values
    // the LUT component size is 2 bytes (16 bits)
	VAU.SHL.X32 v0, v0, 1
    VAU.SHL.X32 v1, v1, 1	
    vau.add.i32 v0, v0, v2
    vau.add.i32 v1, v1, v2
    cmu.cpvi i0, v0.0
    LSU0.LD.8 i0, i0  || cmu.cpvi i1, v0.1 
    LSU0.LD.8 i1, i1  || cmu.cpvi i2, v0.2  || iau.xor i0, i0, i0
    LSU0.LD.8 i2, i2  || cmu.cpvi i3, v0.3  || iau.xor i1, i1, i1
    LSU0.LD.8 i3, i3  || cmu.cpvi i4, v1.0  || iau.xor i2, i2, i2
    LSU0.LD.8 i4, i4  || cmu.cpvi i5, v1.1  || iau.xor i3, i3, i3
    LSU0.LD.8 i5, i5  || cmu.cpvi i6, v1.2  || iau.xor i4, i4, i4
    LSU0.LD.8 i6, i6  || cmu.cpvi i7, v1.3  || vau.xor v0, v0, v0 || iau.xor i5, i5, i5
    LSU0.LD.8 i7, i7  || cmu.cpiv v0.0, i0 || iau.xor i6, i6, i6
    cmu.cpiv v0.1, i1 || iau.xor i7, i7, i7
    cmu.cpiv v0.2, i2
    cmu.cpiv v0.3, i3 || vau.xor v1, v1, v1
    cmu.cpiv v1.0, i4
    cmu.cpiv v1.1, i5 
    cmu.cpiv v1.2, i6
    cmu.cpiv v1.3, i7
    cmu.cpvv.u32.u8s v0, v0 || bru.jmp i30
    cmu.cpvv.u32.u8s v1, v1
    cmu.vszm.word v1 v1 [3201] //Myriad2
	cmu.vszm.word v0, v1, [DD1D] //Myriad2
    CMU.VSZM.WORD v23, v0, [ZZ10]
    //Space the result
    cmu.cpvv.u8.u16 v23, v23
	nop //Myriad2
    
.code .text.svuGet8BitVals8BitLUT
//uchar8 svuGet8BitVals8BitLUT(uchar8 in_values(v23), u8* lut_memory(i18))//
svuGet8BitVals8BitLUT:
    //Convert the values to 32 bit, makes it quicker to add the address to all
    cmu.cpiv v2.0, i18

    // Convert the in_values to unsigned short
	lsu0.ldil i0, 0x00FF || lsu1.ldih i0, 0x00FF
	cmu.cpivr.x32 v0 i0
	vau.and v23 v23 v0

	cmu.cpvv.i16.i32 v0, v23 
	cmu.vszm.word v1 v23 [1032] //Myriad2
    cmu.cpvv.i16.i32 v1, v1 
	cmu.vszm.word v2 v2 [0000] //Myriad2
    vau.add.i32 v0, v0, v2
    vau.add.i32 v1, v1, v2
    cmu.cpvi i0, v0.0
    LSU0.LD.8 i0, i0  || cmu.cpvi i1, v0.1 
    LSU0.LD.8 i1, i1  || cmu.cpvi i2, v0.2  || iau.xor i0, i0, i0
    LSU0.LD.8 i2, i2  || cmu.cpvi i3, v0.3  || iau.xor i1, i1, i1
    LSU0.LD.8 i3, i3  || cmu.cpvi i4, v1.0  || iau.xor i2, i2, i2
    LSU0.LD.8 i4, i4  || cmu.cpvi i5, v1.1  || iau.xor i3, i3, i3
    LSU0.LD.8 i5, i5  || cmu.cpvi i6, v1.2  || iau.xor i4, i4, i4
    LSU0.LD.8 i6, i6  || cmu.cpvi i7, v1.3  || vau.xor v0, v0, v0 || iau.xor i5, i5, i5
    LSU0.LD.8 i7, i7  || cmu.cpiv v0.0, i0 || iau.xor i6, i6, i6
    cmu.cpiv v0.1, i1 || iau.xor i7, i7, i7
    cmu.cpiv v0.2, i2
    cmu.cpiv v0.3, i3 || vau.xor v1, v1, v1
    cmu.cpiv v1.0, i4
    cmu.cpiv v1.1, i5 
    cmu.cpiv v1.2, i6
    cmu.cpiv v1.3, i7
    cmu.cpvv.u32.u8s v0, v0 || bru.jmp i30
    cmu.cpvv.u32.u8s v1, v1
    cmu.vszm.word v1 v1 [3201] //Myriad2
    cmu.vszm.word v0, v1, [DD1D] //Myriad2
    CMU.VSZM.WORD v23, v0, [ZZ10]
    //Space the result
    cmu.cpvv.u8.u16 v23, v23
    nop 2//Myriad2
	
.code .text.svuGet16_8BitVals8BitLUT
//uchar16 svuGet16_8BitVals8BitLUT(uchar16 in_values(v23), u8* lut_memory(i18))//
svuGet16_8BitVals8BitLUT:
    //Convert the values to 32 bit, makes it quicker to add the address to all
    cmu.cpiv v4.0, i18
    cmu.vszm.word v4, v4, [0000] //Myriad2

    cmu.cpvv.u8.u16 v1, v23 
	cmu.vszm.word v3 v23 [1032] //Myriad2
    cmu.cpvv.u8.u16 v3, v3
    cmu.cpvv.i16.i32 v0, v1 
	cmu.vszm.word v1 v1 [1032] //Myriad2
    cmu.cpvv.i16.i32 v1, v1
    cmu.cpvv.i16.i32 v2, v3 
	cmu.vszm.word v3 v3 [1032] //Myriad2
    cmu.cpvv.i16.i32 v3, v3 || vau.add.i32 v0, v4, v0
    vau.add.i32 v1, v4, v1
    vau.add.i32 v2, v4, v2
    vau.add.i32 v3, v4, v3 || cmu.cpvi i0, v0.0
    LSU0.LD.8 i0, i0    || cmu.cpvi i1,  v0.1
    LSU0.LD.8 i1, i1    || cmu.cpvi i2,  v0.2 || iau.xor i0,   i0, i0
    LSU0.LD.8 i2, i2    || cmu.cpvi i3,  v0.3 || iau.xor i1,   i1, i1
    LSU0.LD.8 i3, i3    || cmu.cpvi i4,  v1.0 || iau.xor i2,   i2, i2
    LSU0.LD.8 i4, i4    || cmu.cpvi i5,  v1.1 || iau.xor i3,   i3, i3
    LSU0.LD.8 i5, i5    || cmu.cpvi i6,  v1.2 || iau.xor i4,   i4, i4
    LSU0.LD.8 i6, i6    || cmu.cpvi i7,  v1.3 || iau.xor i5,   i5, i5
    LSU0.LD.8 i7, i7    || cmu.cpvi i8,  v2.0 || iau.xor i6,   i6, i6
    LSU0.LD.8 i8, i8    || cmu.cpvi i9,  v2.1 || iau.xor i7,   i7, i7
    LSU0.LD.8 i9, i9    || cmu.cpvi i10, v2.2 || iau.xor i8,   i8, i8
    LSU0.LD.8 i10, i10  || cmu.cpvi i11, v2.3 || iau.xor i9,   i9, i9
    LSU0.LD.8 i11, i11  || cmu.cpvi i12, v3.0 || iau.xor i10, i10, i10
    LSU0.LD.8 i12, i12  || cmu.cpvi i13, v3.1 || iau.xor i11, i11, i11
    LSU0.LD.8 i13, i13  || cmu.cpvi i14, v3.2 || iau.xor i12, i12, i12
    LSU0.LD.8 i14, i14  || cmu.cpvi i15, v3.3 || iau.xor i13, i13, i13 || vau.xor v23, v23, v23
    LSU0.LD.8 i15, i15  || iau.xor i14, i14, i14 || cmu.cpiv v23.0, i0
    iau.xor i15, i15, i15 || cmu.cpiv v23.1, i1
    cmu.cpiv v23.2, i2
    cmu.cpiv v23.3, i3 || vau.xor v1, v1, v1
    cmu.cpiv v1.0, i4
    cmu.cpiv v1.1, i5
    cmu.cpiv v1.2, i6
    cmu.cpiv v1.3, i7 || vau.xor v2, v2, v2
    cmu.cpiv v2.0, i8
    cmu.cpiv v2.1, i9
    cmu.cpiv v2.2, i10
    cmu.cpiv v2.3, i11 || vau.xor v3, v3, v3
    cmu.cpiv v3.0, i12
    cmu.cpiv v3.1, i13
    cmu.cpiv v3.2, i14
    cmu.cpiv v3.3, i15
    //Compress result
    cmu.cpvv.u32.u8s v23, v23
    cmu.cpvv.u32.u8s v1, v1 || bru.jmp i30
    cmu.cpvv.u32.u8s v2, v2
    cmu.cpvv.u32.u8s v3, v3
    cmu.vszm.word v23, v1, [DD0D] //Myriad2
	cmu.vszm.word v23, v2, [D0DD] //Myriad2
	cmu.vszm.word v23, v3, [0DDD] //Myriad2
	nop //Myriad2
   
.end
