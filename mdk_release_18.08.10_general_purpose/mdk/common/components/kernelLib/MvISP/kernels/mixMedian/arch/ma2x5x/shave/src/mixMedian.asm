///
/// @file mixMedian.asm
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief: mixMedian -  Mix two 8-bit planes according to a third 8-bit reference plane 
/// 
// 

.type mvispMixMedian_asm,@function
.version 00.51.04

.data .data.mixMedian_asm
.align 4

        
.code .text.mixMedian_asm       
//mvispMixMedian(u8 *out[3], u8 *in0[3], u8 *in1[3], u8 *ref, float offset, float slope, unsigned int width)
//{
//    unsigned int p = 0//
//    unsigned int i = 0//
//    for (p = 0// p < 3// p++) 
//    {
//        for(i = 0// i < width// i++) 
//        {
//            float alpha//
//            alpha = ((float)ref[i] / 255.0f + offset) * slope////
//            alpha = alpha > 1.0f ? 1.0f : alpha//
//            alpha = alpha < 0.0f ? 0.0f : alpha//
//            out[0][i] = alpha * (in0[0][i] - in1[0][i]) + in1[0][i]//
//            out[1][i] = alpha * (in0[1][i] - in1[1][i]) + in1[1][i]//
//            out[2][i] = alpha * (in0[2][i] - in1[2][i]) + in1[2][i]//
//        }
//    }
//}
//               i18       i17        i16         i15       i14           i13             i12
//mvispMixMedian(u8 *out[3], u8 *in0[3], u8 *in1[3], u8 *ref, float offset, float slope, unsigned int width)
//{
mvispMixMedian_asm:
.nowarn
   lsu0.ldo.64 i0, i17, 0x00  ||lsu1.ldi.128.u8f.f16 v9, i15
   lsu0.ldo.64 i3, i16, 0x00  ||LSU1.LDIL i9, __end_mixMedian_loop 
   lsu0.ldo.32 i2, i17, 0x08  ||LSU1.LDIH i9, __end_mixMedian_loop ||cmu.cpii.f32.f16 i14, i14
   lsu0.ldo.32 i5, i16, 0x08  ||iau.add i12, i12, 7      ||cmu.cpii.f32.f16 i13, i13 ||LSU1.LDIL i11, 0xFF
   lsu0.ldo.64 i6, i18, 0x00  ||iau.shr.u32 i12, i12, 3 
   lsu0.ldo.32 i8, i18, 0x08     ||cmu.cpivr.x16 v10, i14
   lsu0.ldi.128.u8f.f16 v9, i15 ||cmu.cpivr.x16 v11, i13
   vau.add.f16 v12, v9, v10 ||lsu0.ldi.128.u8f.f16 v0, i0 ||lsu1.ldi.128.u8f.f16 v1, i1
   lsu0.ldi.128.u8f.f16 v4, i4 ||lsu1.ldi.128.u8f.f16 v3, i3 || cmu.cpivr.u8f.f16 v23, i11
   lsu0.ldi.128.u8f.f16 v2, i2
   vau.mul.f16 v12, v12, v11 ||lsu1.ldi.128.u8f.f16 v5, i5
   nop 
   nop
   cmu.clamp0.f16  v19, v12, v23
   vau.add.f16 v12, v9, v10
   vau.sub.f16 v13, v0, v3    ||lsu0.ldi.128.u8f.f16 v9, i15
   vau.sub.f16 v14, v1, v4    ||lsu0.ldi.128.u8f.f16 v0, i0
   vau.sub.f16 v15, v2, v5    ||lsu0.ldi.128.u8f.f16 v1, i1
   vau.mul.f16 v12, v12, v11  ||lsu0.ldi.128.u8f.f16 v2, i2   
   vau.mul.f16 v13, v13, v19  ||lsu0.ldi.128.u8f.f16 v3, i3
   vau.mul.f16 v14, v14, v19  ||lsu0.ldi.128.u8f.f16 v4, i4
   vau.mul.f16 v15, v15, v19  ||lsu0.ldi.128.u8f.f16 v5, i5
   vau.add.f16 v16, v13, v3
   vau.add.f16 v17, v14, v4 ||cmu.clamp0.f16  v19, v12, v23
   vau.add.f16 v18, v15, v5
   //start loop
   vau.add.f16 v12, v9, v10   ||lsu1.sti.128.f16.u8f v16, i6  ||bru.rpl i9, i12, __end_mixMedian_delay_slot
   vau.sub.f16 v13, v0, v3    ||lsu0.ldi.128.u8f.f16 v9, i15
   vau.sub.f16 v14, v1, v4    ||lsu0.ldi.128.u8f.f16 v0, i0
   vau.sub.f16 v15, v2, v5    ||lsu0.ldi.128.u8f.f16 v1, i1
   __end_mixMedian_loop:
   vau.mul.f16 v12, v12, v11  ||lsu0.ldi.128.u8f.f16 v2, i2   
   vau.mul.f16 v13, v13, v19  ||lsu0.ldi.128.u8f.f16 v3, i3
   vau.mul.f16 v14, v14, v19  ||lsu0.ldi.128.u8f.f16 v4, i4
   vau.mul.f16 v15, v15, v19  ||lsu0.ldi.128.u8f.f16 v5, i5
   vau.add.f16 v16, v13, v3   ||lsu0.sti.128.f16.u8f v17, i7
   vau.add.f16 v17, v14, v4   ||cmu.clamp0.f16  v19, v12, v23
   vau.add.f16 v18, v15, v5   ||lsu1.sti.128.f16.u8f v18, i8
   __end_mixMedian_delay_slot:
   //end loop
   bru.jmp i30
   nop 6
//}
.nowarnend
.size mvispMixMedian_asm,.-mvispMixMedian_asm
.end
