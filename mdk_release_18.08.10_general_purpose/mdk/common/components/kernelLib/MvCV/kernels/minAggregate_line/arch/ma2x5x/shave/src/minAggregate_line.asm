/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvMinAggregate_line_asm,@function
.version 00.51.04

.data .data.minAggregate_line
.align 4

.code .text.minAggregate_line

.lalign
//mvcvMinAggregate_line(u8* input1, u8* input2, u8* output, u32* penaltyP1, u32* penaltyP2, u32 width, u8 activateClamp, u8 direction, u32 disparities)//
//                          i18         i17        i16         i15             i14           i13		i12	    i11	           i10
mvcvMinAggregate_line_asm:
.nowarn 10
    lsu0.ldo.32 I10 i19 0
    nop 2

    lsu0.sto.32 I20,i19,-4    || iau.incs i19,-4 //32
    lsu0.sto.32 I21,i19,-4    || iau.incs i19,-4 //64
    lsu0.sto.32 I22,i19,-4    || iau.incs i19,-4 //directionIdx
    lsu0.sto.32 I23,i19,-4    || iau.incs i19,-4 //column
    lsu0.sto.32 I24,i19,-4    || iau.incs i19,-4 //cc

    IAU.SUB i19 i19 16
    LSU0.STo.64.l v28  i19 0  || LSU1.STo.64.h v28  i19 8 || IAU.SUB i19 i19 16
    LSU0.STo.64.l v29  i19 0  || LSU1.STo.64.h v29  i19 8 || IAU.SUB i19 i19 16
    LSU0.STo.64.l v30  i19 0  || LSU1.STo.64.h v30  i19 8 || IAU.SUB i19 i19 16
    LSU0.STo.64.l v31  i19 0  || LSU1.STo.64.h v31  i19 8

    lsu0.ldil i0 0  //directionIDx
    lsu0.ldil i1 0  //column
    lsu0.ldil i2 32 //disparities compare 32
    lsu0.ldil i3 64 //disparities compare 64

    cmu.cmii.i32 i10 i2
    peu.pc1c EQ || bru.bra .LmvcvMinAggregate_line32

    cmu.cmii.i32 i10 i3
    peu.pc1c EQ || bru.bra .LmvcvMinAggregate_line64
    nop 6

  .LmvcvMinAggregate_line32:
  .LmvcvMinAggregate_loop32:

    cmu.cmz.i8 i11
    peu.pc1c eq  || cmu.cpii i0 i1 //directionIdx  = column//
    iau.add i4 i1 1
    peu.pc1c neq || iau.sub i0 i13 i4

    iau.mul i5 i0 i2 		 // cc = directionIdx * disparities//
    nop
    iau.add i6 i18 i5 		 // u8* matchingCostPathPtr	= &input1[cc]//
    iau.add i7 i17 i5		 // u8* matchingCostCurrentPtr	= &input2[cc]//
    iau.add i8 i16 i5 		 // u8* aggregatedCostPtr	= &output[cc]//

    //minKernel32

    lsu0.ldo.64.l v0 i6  0 || lsu1.ldo.64.h v0 i6  8
    lsu0.ldo.64.l v1 i6 16 || lsu1.ldo.64.h v1 i6 24
    nop 6

    cmu.min.u8 v4 v0 v1
    nop
    cmu.alignvec v7 v4 v4 8
    cmu.min.u8 v8 v4 v7
    nop
    cmu.alignvec v9 v8 v8 4
    cmu.min.u8 v10 v9 v8
    nop
    cmu.alignvec v11 v10 v10 2
    cmu.min.u8 v12 v11 v10
    nop
    cmu.alignvec v13 v12 v12 1
    cmu.min.u8 v14 v12 v13
    nop
    lsu0.cp i9 v14.0    //min2

    iau.shl i20 i0 2    //idx for penalty computing
    iau.add i21 i20 i15 //penaltyP1[idx] addr
    iau.add i22 i20 i14 //penaltyP2[idx] addr
    lsu0.ld.32 i21 i21
    lsu0.ld.32 i22 i22
    nop 6

    //aggregateCostSGBM32_clamp

     lsu0.ldil i23, 0x00ff      || lsu1.ldih i23, 0x0
     lsu0.ldi.128.u8.u16 v0 i6  || iau.and i9 i9 i23      || cmu.cpiv.x16 v23.0 i21.l         // v23 = penaltyP1
     lsu0.ldi.128.u8.u16 v1 i6  || iau.add i24 i22 i9     || cmu.cpivr.x16 v22 i9 	    // v22 = min2
     lsu0.ldi.128.u8.u16 v2 i6  || cmu.cpivr.x16 v21 i24  || vau.xor v12,v12,v12	    // v21 = penaltyP2 + min2
     lsu0.ld.128.u8.u16 v3 i6   || cmu.vszm.byte i23,i12,[000d]
     nop 3

     vau.add.i16 v12,v0,v12      || LSU1.SWZV8  [65432101]  || cmu.cpivr.x16 v20 i23          // v20 -mask used for (u8)result part
     cmu.alignvec v5 v0 v1 2
     cmu.alignvec v7 v1 v2 2     || vau.xor v11,v11,v11
     cmu.alignvec v13 v0 v1 14   || vau.add.i16 v11,v3,v11  || LSU0.SWZV8  [67654321]
     cmu.alignvec v9 v2 v3 2
     cmu.alignvec v14 v1 v2 14
     cmu.alignvec v15 v2 v3 14
     cmu.min.u16 v12  v12  v5
     cmu.min.u16 v13  v13  v7
     cmu.min.u16 v14  v14  v9   || vau.add.i16 v12 v23 v12  || LSU0.SWZV8  [00000000]
     cmu.min.u16 v15  v15  v11  || vau.add.i16 v13 v23 v13  || LSU0.SWZV8  [00000000]
     cmu.min.u16 v4 v0 v21      || vau.add.i16 v14 v23 v14  || LSU0.SWZV8  [00000000]       //min_a
     cmu.min.u16 v6 v1 v21      || vau.add.i16 v15 v23 v15  || LSU0.SWZV8  [00000000]       //min_a
     cmu.min.u16 v8 v2 v21                                                                  //min_a
     cmu.min.u16 v10 v3 v21     || lsu1.ldi.128.u8.u16 v4 i7                                //min_a
     cmu.min.u16 v12  v12  v4   || lsu1.ldi.128.u8.u16 v5 i7
     cmu.min.u16 v13  v13  v6   || lsu1.ldi.128.u8.u16 v6 i7
     cmu.min.u16 v14  v14  v8   || vau.sub.i16 v12 v12 v22  || lsu1.ldi.128.u8.u16 v7 i7
     cmu.min.u16 v15  v15  v10  || vau.sub.i16 v13 v13 v22
     vau.sub.i16 v14 v14 v22
     vau.sub.i16 v15 v15 v22
     vau.add.i16 v4 v4 v12
     vau.add.i16 v5 v5 v13
     vau.add.i16 v6 v6 v14
     vau.add.i16 v7 v7 v15
     nop
     vau.and v4,v4,v20
     vau.and v5,v5,v20     || lsu0.sti.128.u16.u8 v4 i8
     vau.and v6,v6,v20     || lsu0.sti.128.u16.u8 v5 i8
     vau.and v7,v7,v20     || lsu0.sti.128.u16.u8 v6 i8
     lsu0.st.128.u16.u8 v7 i8
     nop 2

     iau.add i1 i1 1
     cmu.cmii.i32 i1 i13
     peu.pc1c neq || bru.bra .LmvcvMinAggregate_loop32
     nop 6

     bru.bra .Lexit
     nop 6

.LmvcvMinAggregate_line64:
.LmvcvMinAggregate_loop64:

    cmu.cmz.i8 i11
    peu.pc1c eq  || cmu.cpii i0 i1 //directionIdx  = column//
    iau.add i4 i1 1
    peu.pc1c neq || iau.sub i0 i13 i4

     iau.mul i5 i0 i3   // cc  = directionIdx * disparities//
     nop
     iau.add i6 i18 i5  // u8* matchingCostPathPtr	= &input1[cc]//
     iau.add i7 i17 i5  // u8* matchingCostCurrentPtr	= &input2[cc]//
     iau.add i8 i16 i5  // u8* aggregatedCostPtr	= &output[cc]//

     //aggregateCostSGBM64_clamp

     lsu0.ldo.64.l v0 i6  0 || lsu1.ldo.64.h v0 i6  8
     lsu0.ldo.64.l v1 i6 16 || lsu1.ldo.64.h v1 i6 24
     lsu0.ldo.64.l v2 i6 32 || lsu1.ldo.64.h v2 i6 40
     lsu0.ldo.64.l v3 i6 48 || lsu1.ldo.64.h v3 i6 56
     nop 5

     cmu.min.u8 v4 v0 v1
     cmu.min.u8 v5 v2 v3
     nop
     cmu.min.u8 v6 v4 v5
     nop
     cmu.alignvec v7 v6 v6 8
     cmu.min.u8 v8 v6 v7
     nop
     cmu.alignvec v9 v8 v8 4
     cmu.min.u8 v10 v9 v8
     nop
     cmu.alignvec v11 v10 v10 2
     cmu.min.u8 v12 v11 v10
     nop
     cmu.alignvec v13 v12 v12 1
     cmu.min.u8 v14 v12 v13
     nop
     lsu0.cp i9 v14.0    //min2

     iau.shl i20 i0 2    //idx for penalty computing
     iau.add i21 i20 i15 //penaltyP1[idx] addr
     iau.add i22 i20 i14 //penaltyP2[idx] addr
     lsu0.ld.32 i21 i21
     lsu0.ld.32 i22 i22
     nop 6

     //aggregate64

    lsu0.ldil i23, 0x00ff      || lsu1.ldih i23, 0x0
    lsu0.ldi.128.u8.u16 v0 i6  || iau.and i9 i9 i23   	|| cmu.cpiv.x16 v31.0 i21.l  			// v31 = penaltyP1
    lsu0.ldi.128.u8.u16 v1 i6  || iau.add i24 i22 i9   	|| cmu.cpivr.x16 v30 i9 			// v30 = min2
    lsu0.ldi.128.u8.u16 v2 i6  || cmu.cpivr.x16 v29 i24 						// v29 = penaltyP2 + min
    lsu0.ldi.128.u8.u16 v3 i6  || cmu.vszm.byte i23,i12,[000d]
    lsu0.ldi.128.u8.u16 v4 i6  || cmu.cpivr.x16 v28 i23                    				//v28 -mask used for (u8)result part
    lsu0.ldi.128.u8.u16 v5 i6  || vau.xor v16,v16,v16
    lsu0.ldi.128.u8.u16 v6 i6
    lsu0.ldi.128.u8.u16 v7 i6  || vau.add.i16 v16,v0,v16 || LSU1.SWZV8  [65432101]
    nop

    cmu.alignvec v9 v0 v1 2
    cmu.alignvec v11 v1 v2 2
    cmu.alignvec v17 v0 v1 14
    cmu.alignvec v13 v2 v3 2
    cmu.alignvec v18 v1 v2 14
    cmu.alignvec v15 v3 v4 2
    cmu.alignvec v19 v2 v3 14
    cmu.min.u16 v8 v0 v29   //min_a
    cmu.min.u16 v10 v1 v29  //min_a
    cmu.min.u16 v12 v2 v29  //min_a
    cmu.min.u16 v14 v3 v29  //min_a
    cmu.min.u16 v16  v16  v9
    cmu.min.u16 v17  v17  v11
    cmu.min.u16 v18  v18  v13
    cmu.min.u16 v19  v19  v15

    vau.add.i16 v16 v31 v16  || LSU0.SWZV8  [00000000] // + penaltyP1
    vau.add.i16 v17 v31 v17  || LSU0.SWZV8  [00000000] // + penaltyP1
    vau.add.i16 v18 v31 v18  || LSU0.SWZV8  [00000000]  || cmu.min.u16 v16  v16  v8  // + penaltyP1
    vau.add.i16 v19 v31 v19  || LSU0.SWZV8  [00000000]  || cmu.min.u16 v17  v17  v10 // + penaltyP1
    cmu.min.u16 v18  v18  v12
    cmu.min.u16 v19  v19  v14
    nop

    cmu.alignvec v9 v4 v5 2  || vau.sub.i16 v16 v16 v30
    cmu.alignvec v20 v3 v4 14|| vau.sub.i16 v17 v17 v30
    cmu.alignvec v11 v5 v6 2 || vau.sub.i16 v18 v18 v30
    cmu.alignvec v21 v4 v5 14|| vau.sub.i16 v19 v19 v30
    cmu.alignvec v13 v6 v7 2
    cmu.alignvec v22 v5 v6 14
    cmu.alignvec v23 v6 v7 14

    cmu.min.u16 v8 v4 v29     || vau.xor v15,v15,v15                                  //min_a
    cmu.min.u16 v10 v5 v29    || vau.add.i16 v15,v7,v15   || LSU0.SWZV8  [67654321]   //min_a
    cmu.min.u16 v12 v6 v29    || lsu1.ldi.128.u8.u16 v8 i7 	                          //min_a
    cmu.min.u16 v14 v7 v29    || lsu1.ldi.128.u8.u16 v9 i7  	                //min_a
    cmu.min.u16 v20  v20  v9  || lsu1.ldi.128.u8.u16 v10 i7
    cmu.min.u16 v21  v21  v11 || lsu1.ldi.128.u8.u16 v11 i7
    cmu.min.u16 v22  v22  v13 || vau.add.i16 v20 v31 v20  || LSU0.SWZV8  [00000000] || lsu1.ldi.128.u8.u16 v12 i7  // + penaltyP1
    cmu.min.u16 v23  v23  v15 || vau.add.i16 v21 v31 v21  || LSU0.SWZV8  [00000000] || lsu1.ldi.128.u8.u16 v13 i7  // + penaltyP1
    cmu.min.u16 v20  v20  v8  || vau.add.i16 v22 v31 v22  || LSU0.SWZV8  [00000000] || lsu1.ldi.128.u8.u16 v14 i7  // + penaltyP1
    cmu.min.u16 v21  v21  v10 || vau.add.i16 v23 v31 v23  || LSU0.SWZV8  [00000000] || lsu1.ldi.128.u8.u16 v15 i7  // + penaltyP1
    cmu.min.u16 v22  v22  v12 || vau.sub.i16 v20 v20 v30
    cmu.min.u16 v23  v23  v14 || vau.sub.i16 v21 v21 v30

    vau.sub.i16 v22 v22 v30
    vau.sub.i16 v23 v23 v30
    vau.add.i16 v8 v8 v16
    vau.add.i16 v9 v9 v17
    vau.add.i16 v10 v10 v18
    vau.add.i16 v11 v11 v19
    vau.add.i16 v12 v12 v20
    vau.add.i16 v13 v13 v21
    vau.add.i16 v14 v14 v22
    vau.add.i16 v15 v15 v23
    nop

    vau.and v8,v8,v28
    vau.and v9,v9,v28       ||  lsu0.sti.128.u16.u8 v8 i8
    vau.and v10,v10,v28     ||  lsu0.sti.128.u16.u8 v9 i8
    vau.and v11,v11,v28     ||  lsu0.sti.128.u16.u8 v10 i8
    vau.and v12,v12,v28     ||  lsu0.sti.128.u16.u8 v11 i8
    vau.and v13,v13,v28     ||  lsu0.sti.128.u16.u8 v12 i8
    vau.and v14,v14,v28     ||  lsu0.sti.128.u16.u8 v13 i8
    vau.and v15,v15,v28     ||  lsu0.sti.128.u16.u8 v14 i8
    lsu0.st.128.u16.u8 v15 i8
    nop 2

    iau.add i1 i1 1
    cmu.cmii.i32 i1 i13
    peu.pc1c neq || bru.bra .LmvcvMinAggregate_loop64
    nop 6

.Lexit:

    LSU0.LDo.64.l  v31  i19 0 || LSU1.LDo.64.h  v31  i19 8 ||  IAU.ADD i19 i19 16
    LSU0.LDo.64.l  v30  i19 0 || LSU1.LDo.64.h  v30  i19 8 ||  IAU.ADD i19 i19 16
    LSU0.LDo.64.l  v29  i19 0 || LSU1.LDo.64.h  v29  i19 8 ||  IAU.ADD i19 i19 16
    LSU0.LDo.64.l  v28  i19 0 || LSU1.LDo.64.h  v28  i19 8 ||  IAU.ADD i19 i19 16

    lsu0.ldo.32 I24,i19,0 || iau.incs i19,4
    lsu0.ldo.32 I23,i19,0 || iau.incs i19,4
    lsu0.ldo.32 I22,i19,0 || iau.incs i19,4
    lsu0.ldo.32 I21,i19,0 || iau.incs i19,4
    lsu0.ldo.32 I20,i19,0 || iau.incs i19,4
.nowarnend
    bru.jmp i30
    nop 6
    .size mvcvMinAggregate_line_asm,.-mvcvMinAggregate_line_asm
    .end
