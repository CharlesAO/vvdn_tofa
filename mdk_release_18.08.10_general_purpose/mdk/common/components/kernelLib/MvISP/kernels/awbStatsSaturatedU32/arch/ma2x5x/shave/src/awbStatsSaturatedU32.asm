///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief Calculate saturated pixels in a paxel, accumulated for a paxel region
///

//--------------------------------------------------------------------------------------------------------------
.type mvispStatsAWBSatPixelsU32_asm,@function
.version 00.50.37.8
.data .rodata.statsAWBSatPixelsU32_asm 
.align 4
//.align 16
.nowarn
.code .text.statsAWBSatPixelsU32_asm //text
.lalign
mvispStatsAWBSatPixelsU32_asm:
////                              i18         i17         i16                 i15                 i14             i13                
//void statsAWBSatPixels(UInt8 *output, UInt16 *iline, UInt32 nPaxelX, UInt32 gapPaxelX, UInt32 widthPaxelX, UInt32 satPaxelX)
//{
//    Int32 x//
//    UInt32 crtP//
//
//    for(crtP = 0// crtP < nPaxelX// crtP++){
//        UInt16 *startPosition = (UInt16*)((crtP * gapPaxelX)*2 + (UInt32)iline)//
//        for(x=0// x<(int)(widthPaxelX>>1)// x++){
//            if (startPosition[2*x  ]>satPaxelX) {
//                output[crtP*2  ] = CLAMPZ255(output[crtP*2  ]+1)//
//            }
//            if (startPosition[2*x+1]>satPaxelX) {
//                output[crtP*2+1] = CLAMPZ255(output[crtP*2+1]+1)//
//            }
//        }
//    }
//}

	LSU0.LDIL i3 0x01   ||  LSU1.LDIH i3 0 
	cmu.cpivr.x32 v3 i3
	cmu.cpivr.x32 v7 i3
	LSU0.LDIL i10 awbStatsSaturated_awbSatloop1   ||  LSU1.LDIH i10 awbStatsSaturated_awbSatloop1  ||iau.shr.u32 i14, i14, 1 ||cmu.cpii i9, i17
    cmu.cpii i8, i14     || iau.shl i4, i13, 16    // compared thereshold i2, i6 // repetition number i8, i12 paxel width// 3, 7 increment value with 1                       
    cmu.cpii i12, i14    ||iau.or  i2, i13, i4  
    iau.add i13, i9, 4      
    cmu.cpii i6, i2 ||iau.shl  i15, i15, 1
    
    .lalign
    bru.rpl i10, i16, awbStatsSaturated_awbSatloop1_delay ||lsu0.ld.64.l v11, i18
    //========================================================
    bru.rfb8.z 0 ||lsu0.ld.32 i0, i9 ||iau.incs i9, 0x8
    lsu0.ld.32 i4, i13||iau.incs i13, 0x8  || vau.xor v1 v1 v1
    lsu0.ld.32 i0, i9 ||iau.incs i9, 0x8 
    lsu0.ld.32 i4, i13||iau.incs i13, 0x8 || vau.xor v5 v5 v5
    lsu0.ld.32 i0, i9 ||iau.incs i9, 0x8    
    lsu0.ld.32 i4, i13||iau.incs i13, 0x8
    lsu0.ld.32 i0, i9 ||iau.incs i9, 0x8
    lsu0.ld.32 i4, i13||iau.incs i13, 0x8 ||cmu.cmii.u16 i0, i2
    lsu0.ld.32 i0, i9 ||iau.incs i9, 0x4 ||cmu.cmii.u16 i4, i6 || peu.pvv32 GT || vau.add.i32 v1, v1, v3 ||bru.rpi i8, 4 // loop
 awbStatsSaturated_awbSatloop1:
    nop
    bru.rfb32.z 0 
    //========================================================
    vau.add.i32 v5, v1, v5 ||iau.add i17, i17, i15
    cmu.cpii i9, i17
    vau.add.i32 v11, v11, v5
    iau.add i13, i9, 4
    lsu0.sti.64.l v11, i18
 awbStatsSaturated_awbSatloop1_delay:
	bru.jmp i30
	nop 6
.nowarnend
.size mvispStatsAWBSatPixelsU32_asm,.-mvispStatsAWBSatPixelsU32_asm
.end
	
	 
		

	
	
