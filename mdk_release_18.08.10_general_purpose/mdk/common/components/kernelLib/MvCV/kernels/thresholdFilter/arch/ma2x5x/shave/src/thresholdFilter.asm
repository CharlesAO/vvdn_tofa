///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvThresholdFilter_asm,@function
.version 0.50.00
.nowarn
//void mvcvThresholdFilter(fp32* cornerScores, fp32 threshold, u32 width,
//                            u32 posOffset, u16* candPos, u32* candCount)
//{
//    u16 candidate//
//    for (candidate = posOffset// candidate < posOffset + width// candidate++)
//    {
//        if (cornerScores[candidate] > threshold)
//        {
//            candPos[*candCount] = candidate//
//            *candCount = *candCount + 1//
//        }
//    }
//}


// i18 - *cornerScores
// i17 - threshold
// i16 - width
// i15 - posOffset
// i14 - *candPos
// i13 - *candCount


// v0 - threshold value
// i0 - candidate0
// i1 - candidate1
// i2 - candidate2
// i3 - candidate3
// i4 - candCount value

// i5  - max(thrash si cornerScore[candidate0])
// i6  - max(thrash si cornerScore[candidate1])
// i7  - max(thrash si cornerScore[candidate2])
// i8  - max(thrash si cornerScore[candidate3])
// i9  - dummy temp
// i10 - lable
// i11 - 0x10


.code .text.mvcvThresholdFilter
.lalign
mvcvThresholdFilter_asm:


iau.shl i2 i15 2
iau.add i18 i18 i2		|| lsu0.ldil i11 0x10
lsu0.ldxvi v1 i18 i11	|| cmu.cpivr.x32 v0 i17
iau.xor i4 i4 i4
cmu.cpii i0 i15			|| iau.add i1 i15 1
iau.add i2 i15 2		|| sau.add.i32 i3 i15 3
lsu0.ldil i10 ___thresholdFilterLoop
lsu0.ldih i10 ___thresholdFilterLoop
sau.sub.i32 i16 i16 4
nop
cmu.max.f32 v2 v1 v0	|| lsu1.ldxvi v1 i18 i11
nop 
cmu.cpvi i5 v2.0		
cmu.cpvi i6 v2.1		|| iau.xor i9 i5 i17
peu.pc1i neq 			|| lsu0.sti.16 i0 i14 	|| iau.incs i4 1 

peu.pc1s eq 			|| bru.jmp i30
cmu.cpvi i7 v2.2		|| iau.xor i9 i6 i17	|| sau.add.i32 i0 i0 4	
peu.pc1i neq 			|| lsu0.sti.16 i1 i14 	|| iau.incs i4 1 
cmu.cpvi i8 v2.3		|| iau.xor i9 i7 i17	|| sau.add.i32 i1 i1 4	
peu.pc1i neq 			|| lsu0.sti.16 i2 i14 	|| iau.incs i4 1 
iau.xor i9 i8 i17		|| cmu.max.f32 v2 v1 v0	|| sau.add.i32 i2 i2 4	
peu.pc1i neq 			|| lsu0.sti.16 i3 i14 	|| iau.incs i4 1 

iau.shr.u32 i12 i16 2


lsu1.ld.64.l v1 i18 	|| lsu0.ldo.64.h v1 i18	0x8	|| bru.rpl i10 i12 ___thresholdFilterEndLoop
cmu.cpvi i5 v2.0		|| sau.add.i32 i3 i3 4	 	|| iau.incs i18 0x10
cmu.cpvi i6 v2.1		|| iau.xor i9 i5 i17
___thresholdFilterLoop:

peu.pc1i neq 			|| lsu0.sti.16 i0 i14 		|| iau.incs i4 1 
cmu.cpvi i7 v2.2		|| iau.xor i9 i6 i17		|| sau.add.i32 i0 i0 4	
peu.pc1i neq 			|| lsu0.sti.16 i1 i14 		|| iau.incs i4 1 
cmu.cpvi i8 v2.3		|| iau.xor i9 i7 i17		|| sau.add.i32 i1 i1 4	
peu.pc1i neq 			|| lsu0.sti.16 i2 i14 		|| iau.incs i4 1 
iau.xor i9 i8 i17		|| cmu.max.f32 v2 v1 v0		|| sau.add.i32 i2 i2 4	
peu.pc1i neq 			|| lsu0.sti.16 i3 i14 		|| iau.incs i4 1 
___thresholdFilterEndLoop:

lsu0.st.32 i4 i13

bru.jmp i30
nop 6

.size mvcvThresholdFilter_asm,.-mvcvThresholdFilter_asm
.nowarnend
.end


