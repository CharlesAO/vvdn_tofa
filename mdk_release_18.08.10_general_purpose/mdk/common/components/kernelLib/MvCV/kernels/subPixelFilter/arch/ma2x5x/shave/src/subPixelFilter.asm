.type mvcvSubPixelFilter_asm,@function
.version 00.50.00

//////
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
/// 
/// mvcvSubPixelFilter 
//     @param[in] in1            - pointer to input lines of the left image
//     @param[in] in2            - pointer to input lines of the right image
//     @param[out] out           - array of disparity cost
//     @param[in] width          - width of the input lines
//     @return    Nothing




.data .rodata.mvcvSubPixelFilter_asm
.align 16

.code .text.mvcvSubPixelFilter_asm
.align 16

.lalign
mvcvSubPixelFilter_asm:
        IAU.SUB i19 i19 16
	LSU0.STo.64.l v24  i19 0  || LSU1.STo.64.h v24  i19 8 || IAU.SUB i19 i19 16
	LSU0.STo.64.l v25  i19 0  || LSU1.STo.64.h v25  i19 8 || IAU.SUB i19 i19 16
	LSU0.STo.64.l v26  i19 0  || LSU1.STo.64.h v26  i19 8 || IAU.SUB i19 i19 16
	LSU0.STo.64.l v27  i19 0  || LSU1.STo.64.h v27  i19 8 || IAU.SUB i19 i19 16
	LSU0.STo.64.l v28  i19 0  || LSU1.STo.64.h v28  i19 8 || IAU.SUB i19 i19 16
	LSU0.STo.64.l v29  i19 0  || LSU1.STo.64.h v29  i19 8 || IAU.SUB i19 i19 16
	LSU0.STo.64.l v30  i19 0  || LSU1.STo.64.h v30  i19 8 || IAU.SUB i19 i19 16
	LSU0.STo.64.l v31  i19 0  || LSU1.STo.64.h v31  i19 8
//mvcvSubPixelFilter(u8* inputDisparityMap, u8* inputCostVolume, u16* outputDisparityMap, u32 width, u8 maxDisp, u8 numFractionalBit, u8 * lutToUse)
//                             i18                   i17                    i16               i15         i14            i13                 i12    
lsu0.ldil i0 0xff || lsu1.ldil i9 0xff
lsu0.ldil i10 0x3f || lsu1.ldil i11 0x7f //0x7f //0x3f

cmu.cpivr.x16 v27 i10 || iau.and i14 i14 i0 // maxDisp
cmu.cpivr.x16 v28 i11 || iau.and i13 i13 i0 // numFractionalBit
cmu.cpivr.x16 v29 i9  || iau.xor i1 i1 i1  || vau.xor v3 v3 v3 
cmu.cpivr.x32 v30 i12 || iau.add i2 i1 i14 || lsu0.cp v2.0 i1 
cmu.cpivr.x32 v11 i17 || iau.add i3 i2 i14 || lsu0.cp v2.1 i2// inputCostVolume
cmu.cpivr.x16 v24 i13 || iau.add i4 i3 i14 || lsu0.cp v2.2 i3// numFractionalBit repet
iau.add i5 i4 i14 || lsu0.cp v2.3 i4 || cmu.cpivr.x32 v1 i14 //maxdisp
iau.add i6 i5 i14 || lsu0.cp v6.0 i5 || cmu.cpivr.x16 v31 i14 //maxdisp
cmu.cmz.i32 i12
peu.pc1c eq || bru.bra .LmvcvSubPixelFilter_nolut
iau.add i7 i6 i14 || lsu0.cp v6.1 i6 || vau.sub.i16 v31 v31 1 //maxdisp -1
iau.add i8 i7 i14 || lsu0.cp v6.2 i7 
		     lsu0.cp v6.3 i8 || vau.add.i32 v7 v2 v1 || iau.shl i9 i14 3//upper1  //maxdisp * 8//lower(v2) & lower(v6) second part	
vau.add.i32 v8 v6 v1 || cmu.cpivr.x32 v1 i9 //step of maxdisp //upper2
////////////////////////////////////////
lsu0.ldil i11 .LmvcvSubPixelFilter_loop || lsu1.ldih i11 .LmvcvSubPixelFilter_loop
iau.shr.u32 i15 i15 3

lsu0.ldi.128.u8.u16 v0 i18 // currentDisp = inputDisparityMap[j];
nop 6
cmu.vilv.x16 v9 v10 v3 v0 //currentDisp v9 first, v10 second
vau.add.i32  v9 v9 v2  //minCostIndex1
vau.add.i32 v10 v10 v6  //minCostIndex2
vau.add.i32 v14  v9 v11
vau.add.i32 v15 v10 v11
vau.add.i32 v22  v9 1
vau.add.i32 v23 v10 1


lsu0.cp i1 v14.0
lsu0.cp i1 v14.1 || lsu1.ldo.32 i2 i1 -1
lsu0.cp i1 v14.2 || lsu1.ldo.32 i2 i1 -1
lsu0.cp i1 v14.3 || lsu1.ldo.32 i2 i1 -1
lsu0.cp i1 v15.0 || lsu1.ldo.32 i2 i1 -1
lsu0.cp i1 v15.1 || lsu1.ldo.32 i2 i1 -1
lsu0.cp i1 v15.2 || lsu1.ldo.32 i2 i1 -1
lsu0.cp i1 v15.3 || lsu1.ldo.32 i2 i1 -1
                    lsu1.ldo.32 i2 i1 -1 || lsu0.cp v14.0 i2
                                            lsu0.cp v14.1 i2
                                            lsu0.cp v14.2 i2
                                            lsu0.cp v14.3 i2
                                            lsu0.cp v15.0 i2
                                            lsu0.cp v15.1 i2
                                            lsu0.cp v15.2 i2
                                            lsu0.cp v15.3 i2 //pre & mincost & postcost


  // currentDisp = inputDisparityMap[j];
CMU.VSZM.byte v16 v14 [zzz0] || lsu0.cp v4 v0  || lsu1.ldi.128.u8.u16 v0 i18  //pre 1
CMU.VSZM.byte v17 v14 [zzz1]  //min
CMU.VSZM.byte v18 v14 [zzz2]  //post
CMU.VSZM.byte v19 v15 [zzz0]  //pre 2
CMU.VSZM.byte v20 v15 [zzz1]  //min
CMU.VSZM.byte v21 v15 [zzz2]  //post
cmu.cmz.i32 v9
peu.pvl032 eq  || lsu0.cp v16 v17 || cmu.cmvv.i32 v9 v2  || vau.add.i32 v2 v2 v1////////// increment disparity after usage of v2 v6 v7 v8 
peu.pvl032 lte || lsu0.cp v16 v17 || cmu.cmz.i32 v10     
peu.pvl032 eq  || lsu0.cp v19 v20 || cmu.cmvv.i32 v10 v6 || vau.add.i32 v6 v6 v1
cmu.vilv.x16 v9 v10 v3 v0 //currentDisp v9 first, v10 second
peu.pvl032 lte || lsu0.cp v19 v20 || cmu.cmvv.i32 v22 v7 || vau.add.i32 v7 v7 v1 //convert to u16//////////////////////////////////
peu.pvl032 gte || lsu0.cp v18 v17 || cmu.cmvv.i32 v23 v8 || vau.add.i32 v8 v8 v1
peu.pvl032 gte || lsu0.cp v21 v20 || cmu.vdilv.x16 v19 v16 v16 v19 || vau.add.i32  v9 v9 v2  //minCostIndex1 //precost                       
cmu.vdilv.x16 v20 v17 v17 v20 || vau.add.i32 v10 v10 v6  //minCostIndex2//mincost
cmu.vdilv.x16 v21 v18 v18 v21 || vau.add.i32 v14  v9 v11//postcost
vau.sub.i16 v19 v16 v17 //pre
vau.sub.i16 v20 v18 v17                                    || lsu0.cp i1 v14.0 //post
vau.add.i32 v15 v10 v11 || CMU.VSZM.byte v19 v19 [z2z0]    || lsu0.cp i4 v14.1  || lsu1.ldo.32 i1 i1 -1
vau.add.i32 v22  v9 1   || CMU.VSZM.byte v20 v20 [z2z0]    || lsu0.cp i5 v14.2  || lsu1.ldo.32 i4 i4 -1
vau.add.i32 v23 v10 1  || cmu.cpvv v12 v19                 || lsu0.cp i6 v14.3  || lsu1.ldo.32 i5 i5 -1
vau.shl.x16 v26 v4 v24 || cmu.cpvv v13 v20                 || lsu0.cp i7 v15.0  || lsu1.ldo.32 i6 i6 -1//result = minDisparity << numFractionalBit;
                                                              lsu0.cp i8 v15.1  || lsu1.ldo.32 i7 i7 -1
lsu0.cp i9 v15.2  || lsu1.ldo.32 i8 i8 -1 || vau.or v25 v19 v20 //onion
lsu0.cp i10 v15.3 || lsu1.ldo.32 i9 i9 -1 || cmu.cmvv.i16 v25 v27
                     lsu1.ldo.32 i10 i10 -1 || lsu0.cp v14.0 i1  ||  peu.pvv16 gt || vau.shr.u16 v19 v19 1 //pre
                                              lsu0.cp v14.1 i4   ||  peu.pvv16 gt || vau.shr.u16 v20 v20 1  || cmu.cmvv.i16 v25 v28//post
                                              lsu0.cp v14.2 i5   ||  peu.pvv16 gt || vau.shr.u16 v19 v19 1 //pre
                                              lsu0.cp v14.3 i6   ||  peu.pvv16 gt || vau.shr.u16 v20 v20 1 //post
                                              lsu0.cp v15.0 i7   ||  vau.shl.x32 v19 v19 6
                                              lsu0.cp v15.1 i8   
                                              lsu0.cp v15.2 i9   ||  vau.or v19 v19 v20 // return index
                                              lsu0.cp v15.3 i10  ||  cmu.vilv.x16 v25 v5 v3 v19                    //pre & mincost & postcost
											
vau.add.i32 v25 v25 v30                        || CMU.VSZM.byte v16 v14 [zzz0] || lsu0.cp v29 v4 || bru.rpl i11 i15
vau.add.i32 v5 v5 v30                          || CMU.VSZM.byte v17 v14 [zzz1] || lsu0.cp v4 v0  || lsu1.ldi.128.u8.u16 v0 i18  //pre 1 //min //pre 2 //min
lsu0.cp i1 v25.0                               || CMU.VSZM.byte v18 v14 [zzz2]  //post
lsu0.cp i4 v25.1  || lsu1.ld.32.i8.i32 i1 i1   || CMU.VSZM.byte v19 v15 [zzz0] 
lsu0.cp i5 v25.2  || lsu1.ld.32.i8.i32 i4 i4   || CMU.VSZM.byte v20 v15 [zzz1] 
lsu0.cp i6 v25.3  || lsu1.ld.32.i8.i32 i5 i5   || CMU.VSZM.byte v21 v15 [zzz2]  //post
lsu0.cp i7 v5.0  || lsu1.ld.32.i8.i32 i6 i6    || cmu.cmz.i32 v9
peu.pvl032 eq  || lsu0.cp v16 v17 || cmu.cmvv.i32 v9 v2  || vau.add.i32 v2 v2 v1////////// increment disparity after usage of v2 v6 v7 v8 
peu.pvl032 lte || lsu0.cp v16 v17 || cmu.cmz.i32 v10     
peu.pvl032 eq  || lsu0.cp v19 v20 || cmu.cmvv.i32 v10 v6 || vau.add.i32 v6 v6 v1
lsu0.cp i8 v5.1  || lsu1.ld.32.i8.i32 i7 i7 || cmu.vilv.x16 v9 v10 v3 v0 //currentDisp v9 first, v10 second
peu.pvl032 lte || lsu0.cp v19 v20 || cmu.cmvv.i32 v22 v7 || vau.add.i32 v7 v7 v1 //convert to u16//////////////////////////////////
peu.pvl032 gte || lsu0.cp v18 v17 || cmu.cmvv.i32 v23 v8 || vau.add.i32 v8 v8 v1
peu.pvl032 gte || lsu0.cp v21 v20 || cmu.vdilv.x16 v19 v16 v16 v19 || vau.add.i32  v9 v9 v2  //minCostIndex1 //precost  
lsu0.cp i9 v5.2  || lsu1.ld.32.i8.i32 i8 i8  || cmu.vdilv.x16 v20 v17 v17 v20 || vau.add.i32 v10 v10 v6  //minCostIndex2//mincost
lsu0.cp i10 v5.3 || lsu1.ld.32.i8.i32 i9 i9  || cmu.vdilv.x16 v21 v18 v18 v21 || vau.add.i32 v14  v9 v11//postcost
                     lsu1.ld.32.i8.i32 i10 i10 || cmu.cpiv.x16 v16.0 i1.l || vau.sub.i16 v19 v16 v17 //pre
                                                  cmu.cpiv.x16 v16.1 i4.l || vau.sub.i16 v20 v18 v17                                    || lsu0.cp i1 v14.0 //post
												  cmu.cpivr.x16 v18 i0    || lsu0.cp i4 v14.1  || lsu1.ldo.32 i1 i1 -1
                                                  cmu.cpiv.x16 v16.2 i5.l  || vau.add.i32 v15 v10 v11 || lsu0.cp i5 v14.2  || lsu1.ldo.32 i4 i4 -1
                                                  cmu.cpiv.x16 v16.3 i6.l  || vau.add.i32 v22  v9 1   || lsu0.cp i6 v14.3  || lsu1.ldo.32 i5 i5 -1 
                                                  cmu.cpiv.x16 v16.4 i7.l || vau.add.i32 v23 v10 1  || lsu0.cp i7 v15.0  || lsu1.ldo.32 i6 i6 -1
                                                  cmu.cpiv.x16 v16.5 i8.l ||  lsu0.cp i8 v15.1  || lsu1.ldo.32 i7 i7 -1//result = minDisparity << numFractionalBit;
                                                  cmu.cpiv.x16 v16.6 i9.l || lsu0.cp i9 v15.2  || lsu1.ldo.32 i8 i8 -1 
                                                  cmu.cpiv.x16 v16.7 i10.l || lsu0.cp i10 v15.3 || lsu1.ldo.32 i9 i9 -1  || vau.and v20 v20 v18 // fractionalOffset
											      cmu.cmz.i16 v29 ||  lsu1.ldo.32 i10 i10 -1 || lsu0.cp v14.0 i1 || vau.and v19 v19 v18
peu.pvl016 eq || lsu0.cp v16 v3 || cmu.cmvv.i16 v29 v31 || vau.or v25 v19 v20 //onion
peu.pvl016 eq || lsu0.cp v16 v3 || cmu.cmvv.i16 v12 v13 || vau.add.i16 v12 v19 0
peu.pvv16 lt  || vau.sub.i16 v18 v26 v16 || lsu0.cp v13 v20
peu.pvv16 gte || vau.add.i16 v18 v26 v16 || cmu.cmvv.i16 v25 v27
peu.pvv16 gt  || vau.shr.u16 v19 v19 1 
.LmvcvSubPixelFilter_loop:
peu.pvv16 gt  || vau.shr.u16 v20 v20 1  || cmu.cmvv.i16 v25 v28    || lsu1.sto.64.l v18 i16 0                       || lsu0.cp v14.1 i4   //post
peu.pvv16 gt  || vau.shr.u16 v19 v19 1                             || lsu1.sto.64.h v18 i16 8 || iau.add i16 i16 16 || lsu0.cp v14.2 i5   
peu.pvv16 gt  || vau.shr.u16 v20 v20 1  || lsu0.cp v14.3 i6   
                                              lsu0.cp v15.0 i7   ||  vau.shl.x32 v19 v19 6
                                              lsu0.cp v15.1 i8   
                                              lsu0.cp v15.2 i9   ||  vau.or v19 v19 v20 // return index
                                              lsu0.cp v15.3 i10  ||  cmu.vilv.x16 v25 v5 v3 v19         || vau.shl.x16 v26 v4 v24            //pre & mincost & postcost


bru.bra .LmvcvSubPixelFilter_exit
nop 6

//loop needed only if we do not have lut
.LmvcvSubPixelFilter_nolut:
lsu0.ldil i11 .LmvcvSubPixelFilter_loop2 || lsu1.ldih i11 .LmvcvSubPixelFilter_loop2
bru.rpl i11 i15
lsu0.ldi.128.u8.u16 v0 i18 // currentDisp = inputDisparityMap[j];
nop 6
vau.shl.x16 v26 v0 v24  //result = minDisparity << numFractionalBit;
nop 2
lsu0.sto.64.l v26 i16 0 || lsu1.sto.64.h v26 i16 8 || iau.add i16 i16 16

.LmvcvSubPixelFilter_loop2:
nop 7
.LmvcvSubPixelFilter_exit:


	
	LSU0.LDo.64.h  v31  i19 8 || LSU1.LDo.64.l  v31  i19 0 ||  IAU.ADD i19 i19 16
	LSU0.LDo.64.h  v30  i19 8 || LSU1.LDo.64.l  v30  i19 0 ||  IAU.ADD i19 i19 16
	LSU0.LDo.64.h  v29  i19 8 || LSU1.LDo.64.l  v29  i19 0 ||  IAU.ADD i19 i19 16
	LSU0.LDo.64.h  v28  i19 8 || LSU1.LDo.64.l  v28  i19 0 ||  IAU.ADD i19 i19 16
	LSU0.LDo.64.h  v27  i19 8 || LSU1.LDo.64.l  v27  i19 0 ||  IAU.ADD i19 i19 16
	LSU0.LDo.64.h  v26  i19 8 || LSU1.LDo.64.l  v26  i19 0 ||  IAU.ADD i19 i19 16
	LSU0.LDo.64.h  v25  i19 8 || LSU1.LDo.64.l  v25  i19 0 ||  IAU.ADD i19 i19 16
	LSU0.LDo.64.h  v24  i19 8 || LSU1.LDo.64.l  v24  i19 0 ||  IAU.ADD i19 i19 16 || BRU.jmp i30
    nop 6
.size mvcvSubPixelFilter_asm,.-mvcvSubPixelFilter_asm
.end
