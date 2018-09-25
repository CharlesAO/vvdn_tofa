///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief Calculate saturated pixels in a paxel, accumulated for a paxel region
///

//--------------------------------------------------------------------------------------------------------------
.type mvispStatsAWBSatPixels_asm,@function
.version 00.50.37.8
.data .rodata.statsAWBSatPixels_asm 
.align 4
//.align 16
.nowarn
.code .text.statsAWBSatPixels_asm //text
.lalign
mvispStatsAWBSatPixels_asm:
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
	LSU0.LDIL i10 awbStatsSaturated_awbSatloop1   ||  LSU1.LDIH i10 awbStatsSaturated_awbSatloop1  ||iau.shr.u32 i14, i14, 1 ||cmu.cpii i9, i17
    LSU0.LDIL i3 0x0101   ||  LSU1.LDIH i3 0 ||cmu.cpii i8, i14     || iau.shl i4, i13, 16    // compared thereshold i2, i6 // repetition number i8, i12 paxel width// 3, 7 increment value with 1                       
    LSU0.LDIL i7 0x0101   ||  LSU1.LDIH i7 0 ||cmu.cpii i12, i14    ||iau.or  i2, i13, i4  
    iau.add i13, i9, 4      
    cmu.cpii i6, i2 ||iau.shl  i15, i15, 1
    
    .lalign
    bru.rpl i10, i16, awbStatsSaturated_awbSatloop1_delay ||lsu0.ld.16 i11, i18
    //========================================================
    bru.rfb8.z 0 ||lsu0.ld.32 i0, i9 ||iau.incs i9, 0x8
    lsu0.ld.32 i4, i13||iau.incs i13, 0x8  ||LSU1.LDIH i1 0 // number of pixels saturated
    lsu0.ld.32 i0, i9 ||iau.incs i9, 0x8 ||LSU1.LDIL i1 0   
    lsu0.ld.32 i4, i13||iau.incs i13, 0x8 ||LSU1.LDIH i5 0 // number of pixels saturated
    lsu0.ld.32 i0, i9 ||iau.incs i9, 0x8 ||LSU1.LDIL i5 0   
    lsu0.ld.32 i4, i13||iau.incs i13, 0x8
    lsu0.ld.32 i0, i9 ||iau.incs i9, 0x8
    lsu0.ld.32 i4, i13||iau.incs i13, 0x8 ||cmu.cmii.u16 i0, i2
    lsu0.ld.32 i0, i9 ||iau.incs i9, 0x4 ||cmu.cmii.u16 i4, i6 || peu.pvs8 GT ||sau.iadds.u8 i1, i1, i3 ||bru.rpi i8, 4 // loop
 awbStatsSaturated_awbSatloop1:
    nop
    bru.rfb32.z 0 
    //========================================================
    sau.iadds.u8 i5, i1, i5 ||iau.add i17, i17, i15
    cmu.cpii i9, i17
    sau.iadds.u8 i11, i11, i5
    iau.add i13, i9, 4
    lsu0.sti.16 i11, i18
 awbStatsSaturated_awbSatloop1_delay:
	bru.jmp i30
	nop 6

.size mvispStatsAWBSatPixels_asm,.-mvispStatsAWBSatPixels_asm
.nowarnend
.end
	
	 
		

	
	
