/////
///// @file      combDecimDemosaic.c
///// @copyright All code copyright Movidius Ltd 2012, all rights reserved
/////            For License Warranty see: common/license.txt
/////
///// @brief     combine decimation and demosaic - Obtain a RGB image, downscaled with 0.5 from an bayer GRBG input image
/////



.type mvispCombDecimDemosaic_asm,@function
.version 00.51.04

.data .data.combDecimDemosaic_asm
.align 4

.nowarn        
.code .text.combDecimDemosaic_asm       
///// combine decimation Demosaic - Obtain a RGB image, downscaled with 0.5 from an bayer GRBG input image
///// @param[out] output   - 3 pointers to RGB channels (lines)
///// @param[in]  iline    - array of pointers to input line of the image 4 lines
///// @param[in]  width    - width of the output lines
///// @return    Nothing
//void combDecimDemosaic(unsigned short **output, unsigned short **iline, unsigned int width)
//{
//    int i//
//    for(i=0// i<(int)width// i++)
//    {
//        output[0][i] =   ((iline[1][i*2-1] * 3 +  // R
//                           iline[1][i*2+1] * 9 + 
//                           iline[3][i*2-1] * 1 + 
//                           iline[3][i*2+1] * 3) >> 4 )//
//                             
//        output[1][i] =   ((iline[1][i*2  ] +  // G
//                           iline[2][i*2+1]) >> 1)//
//
//        output[2][i] =   ((iline[0][i*2  ] * 3 +  // B
//                           iline[0][i*2+2] * 1 + 
//                           iline[2][i*2  ] * 9 + 
//                           iline[2][i*2+2] * 3) >> 4)// 
//    }
//}
//                                       i18                      i17                i16   
//void combDecimDemosaic(unsigned short **output, unsigned short **iline, unsigned int width)
//{
mvispCombDecimDemosaic_asm:

lsu0.ldo.64 i4, i17, 0x00
lsu0.ldo.64 i6, i17, 0x08
lsu0.ldo.64 i1, i18, 0x00
lsu0.ldo.32 i3, i18, 0x08
lsu1.ldil i0, 1
lsu1.ldil i0, 3 || cmu.cpivr.x16 v22, i0
cmu.cpivr.x16 v21, i0
nop 
lsu0.ldo.16 i10, i5, -2  ||lsu1.ldo.16 i11, i7, -2 
lsu0.ldo.16 i8, i4, 0x20 ||lsu1.ldo.16 i9, i6, 0x20 
LSU0.LDXV v1, i5 || iau.incs i5, 0x10
LSU1.LDXV v5, i5 || iau.incs i5, 0x10
LSU0.LDXV v2, i6 || iau.incs i6, 0x10
LSU1.LDXV v6, i6 || iau.incs i6, 0x10
LSU0.LDXV v0, i4 || iau.incs i4, 0x10
LSU1.LDXV v4, i4 || iau.incs i4, 0x10
LSU0.LDXV v3, i7 || iau.incs i7, 0x10
LSU1.LDXV v7, i7 || iau.incs i7, 0x10
lsu0.ldil i0, 9 
cmu.cpivr.x16 v20, i0
nop
                                  cmu.vdilv.x16 v10, v9 , v1, v5    ||lsu0.ldo.16 i8, i4, 0x20 ||lsu1.ldo.16 i9, i6, 0x20 // extra B1// extra B0
                                  cmu.vdilv.x16 v12, v11, v2, v6 
vau.avg.x16 v16, v9, v12        ||cmu.vdilv.x16 v23, v8 , v0, v4 
vau.macpz.u16 v8, v21           ||cmu.shriv.x16 v8, v8, i8          ||LSU0.LDXV v1, i5 || iau.incs i5, 0x10
vau.macp.u16 v8, v22                                                ||LSU1.LDXV v5, i5 || iau.incs i5, 0x10
vau.macp.u16 v11, v20           ||cmu.shriv.x16 v11, v11, i9        ||LSU0.LDXV v2, i6 || iau.incs i6, 0x10
vau.macpw.u16 v14, v11, v21     ||cmu.vdilv.x16 v13, v23, v3, v7    ||LSU1.LDXV v6, i6 || iau.incs i6, 0x10
vau.macpz.u16 v10, v20          ||cmu.shliv.x16 v19, v10, i10       ||LSU0.LDXV v0, i4 || iau.incs i4, 0x10
vau.macp.u16  v19, v21          ||cmu.cpvi.x16 i10.l, v10.7         ||LSU1.LDXV v4, i4 || iau.incs i4, 0x10
vau.macp.u16  v13, v21          ||cmu.shliv.x16 v19, v13, i11       ||LSU0.LDXV v3, i7 || iau.incs i7, 0x10
vau.macpw.u16  v15, v19, v22    ||cmu.cpvi.x16 i11.l, v13.7         ||LSU1.LDXV v7, i7 || iau.incs i7, 0x10
nop 
iau.shr.u32 i16, i16, 3
lsu1.ldil i12, end_loop_combDecimDemosaic        ||lsu0.ldih i12, end_loop_combDecimDemosaic
// start loop
vau.shr.U16 v17, v14, 4         ||cmu.vdilv.x16 v10, v9 , v1, v5    ||lsu0.ldo.16 i8, i4, 0x20      ||lsu1.ldo.16 i9, i6, 0x20      ||bru.rpl i12, i16, delay_slot_combDecimDemosaic
vau.shr.U16 v18, v15, 4         ||cmu.vdilv.x16 v12, v11, v2, v6    ||LSU0.STXV v16,i2 || iau.incs i2, 0x10  
vau.avg.x16 v16, v9, v12        ||cmu.vdilv.x16 v23, v8 , v0, v4    ||LSU1.STXV v17,i3 || iau.incs i3, 0x10 
vau.macpz.u16 v8, v21           ||cmu.shriv.x16 v8, v8, i8          ||LSU0.LDXV v1, i5 || iau.incs i5, 0x10
vau.macp.u16 v8, v22                                                ||LSU1.LDXV v5, i5 || iau.incs i5, 0x10
vau.macp.u16 v11, v20           ||cmu.shriv.x16 v11, v11, i9        ||LSU0.LDXV v2, i6 || iau.incs i6, 0x10
vau.macpw.u16 v14, v11, v21     ||cmu.vdilv.x16 v13, v23, v3, v7    ||LSU1.LDXV v6, i6 || iau.incs i6, 0x10
end_loop_combDecimDemosaic:
vau.macpz.u16 v10, v20          ||cmu.shliv.x16 v19, v10, i10       ||LSU0.LDXV v0, i4 || iau.incs i4, 0x10
vau.macp.u16  v19, v21          ||cmu.cpvi.x16 i10.l, v10.7         ||LSU1.LDXV v4, i4 || iau.incs i4, 0x10
vau.macp.u16  v13, v21          ||cmu.shliv.x16 v19, v13, i11       ||LSU0.LDXV v3, i7 || iau.incs i7, 0x10
vau.macpw.u16  v15, v19, v22    ||cmu.cpvi.x16 i11.l, v13.7         ||LSU1.LDXV v7, i7 || iau.incs i7, 0x10
                                                                      LSU0.STXV v18,i1 || iau.incs i1, 0x10
nop 
nop
delay_slot_combDecimDemosaic:
// end loop

bru.jmp i30
nop 6
.nowarnend
//}
.size mvispCombDecimDemosaic_asm,.-mvispCombDecimDemosaic_asm
.end
