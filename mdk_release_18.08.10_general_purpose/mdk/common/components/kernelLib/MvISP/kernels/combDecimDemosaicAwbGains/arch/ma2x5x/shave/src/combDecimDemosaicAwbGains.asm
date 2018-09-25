/////
///// @file      combDecimDemosaic.c
///// @copyright All code copyright Movidius Ltd 2012, all rights reserved
/////            For License Warranty see: common/license.txt
/////
///// @brief     combine decimation and demosaic - Obtain a RGB image, downscaled with 0.5 from 
/////            a bayer GRBG input image, and apply white-balance gains
/////



.type mvispCombDecimDemosaicAwbGains_asm,@function
.version 00.51.04

.data .data.combDecimDemosaicAwbGains_asm
.align 4

        
.code .text.combDecimDemosaicAwbGains_asm       
///// combine decimation and demosaic - Obtain a RGB image, downscaled with 0.5 from an bayer GRBG input image, and apply WB gains
///// @param[out] output   - 3 pointers to RGB lines channel
///// @param[in]  iline    - array of pointers to input line of the image 4 lines
///// @param[in] gains     - list 0f 3 white balance gains, in fixed point 8.8
///// @param[in] width     - width of the output lines
///// @return    Nothing
//#define CLAMPZ255(X) ((X)>255 ? 255 : ((X)<0 ? 0 : (X)))
//
//void mvispCombDecimDemosaicAwbGains(unsigned char **output, unsigned short **iline, unsigned short gains[3], unsigned int width)
//{
//    int i//
//    UInt16 pixR//
//    UInt16 pixG//
//    UInt16 pixB//
//    iline[1][-1] = iline[1][1]//
//    iline[3][-1] = iline[3][1]//
//    for(i=0// i<(int)width// i++)
//    {
//        pixR =   ((iline[1][i*2-1] * 3 +  // R
//                 iline[1][i*2+1] * 9 + 
//                 iline[3][i*2-1] * 1 + 
//                 iline[3][i*2+1] * 3) >> 4 )//
//                             
//        pixG =   ((iline[1][i*2  ] +  // G
//                 iline[2][i*2+1] + 1) >> 1)//
//
//        pixB =   ((iline[0][i*2  ] * 3 +  // B
//                 iline[0][i*2+2] * 1 + 
//                 iline[2][i*2  ] * 9 + 
//                 iline[2][i*2+2] * 3) >> 4)// 
//        // applay gains
//        output[0][i] = CLAMPZ255((((unsigned int)pixR * gains[0]) >> 8)>>2)// // 2 instructions / 4
//        output[1][i] = CLAMPZ255((((unsigned int)pixG * gains[1]) >> 8)>>2)// // 2 instructions / 4
//        output[2][i] = CLAMPZ255((((unsigned int)pixB * gains[2]) >> 8)>>2)// // 2 instructions / 4
//    }
//    output[2][width-1] = output[2][width-2]//
//}
//
//
//                                            i18                      i17                 i16                       i15
//void mvispCombDecimDemosaicAwbGains(unsigned char **output, unsigned short **iline, unsigned short gains[3], unsigned int width)
//{
.lalign
mvispCombDecimDemosaicAwbGains_asm:
// stack job
.nowarn 10
IAU.SUB i19 i19 128 ||lsu1.ldil i0, 0
lsu0.ldo.64 i4, i17, 0x00 || lsu1.sto.64.h v30 i19, 0x68
lsu0.ldo.64 i1, i18, 0x00   ||lsu1.sto.64.l v30 i19, 0x60 
lsu0.ldo.64 i6, i17, 0x08   ||lsu1.sto.64.h v31 i19, 0x78
lsu0.ldo.32 i3, i18, 0x08   ||lsu1.sto.64.l v31 i19, 0x70
lsu0.sto.64.l v24 i19, 0x00 || lsu1.sto.64.h v24 i19, 0x08 
lsu0.sto.64.l v25 i19, 0x10 || lsu1.sto.64.h v25 i19, 0x18
lsu0.sto.64.l v26 i19, 0x20 || lsu1.sto.64.h v26 i19, 0x28
lsu0.sto.64.l v27 i19, 0x30 || lsu1.sto.64.h v27 i19, 0x38
lsu0.sto.64.l v28 i19, 0x40 || lsu1.sto.64.h v28 i19, 0x48
lsu0.sto.64.l v29 i19, 0x50 || lsu1.sto.64.h v29 i19, 0x58
lsu0.ld.64.l  v30 i16 ||cmu.cpivr.x16 v31, i0 ||lsu1.ldil i0, 1
lsu0.ldo.16 i10, i5, 2  ||lsu1.ldo.16 i11, i7, 2 ||cmu.cpivr.x16 v22, i0 // R
lsu0.ldo.16 i8, i4, 0x20 ||lsu1.ldo.16 i9, i6, 0x20 
LSU0.LDXV v1, i5 || iau.incs i5, 0x10
LSU1.LDXV v5, i5 || iau.incs i5, 0x10
LSU0.LDXV v2, i6 || iau.incs i6, 0x10
LSU1.LDXV v6, i6 || iau.incs i6, 0x10
LSU0.LDXV v0, i4 || iau.incs i4, 0x10
LSU1.LDXV v4, i4 || iau.incs i4, 0x10
LSU0.LDXV v3, i7 || iau.incs i7, 0x10
LSU1.LDXV v7, i7 || iau.incs i7, 0x10
lsu0.ldil i0, 9  ||cmu.cpvv.i16.i32 v30, v30 // gains part
cmu.cpivr.x16 v20, i0 ||lsu1.ldil i0, 3 ||vau.shl.x32 v30, v30, 14
cmu.cpivr.x16 v21, i0
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
iau.shr.u32 i15, i15, 3
lsu1.ldil i12, end_loop_combDecimDemosaicAwbGains        ||lsu0.ldih i12, end_loop_combDecimDemosaicAwbGains
// start loop
.lalign
vau.shr.U16 v17, v14, 4         ||cmu.vdilv.x16 v10, v9 , v1, v5        ||bru.rpl i12, i15, delay_slot_combDecimDemosaicAwbGains
vau.shr.U16 v18, v15, 4         ||cmu.vdilv.x16 v12, v11, v2, v6   
vau.avg.x16 v16, v9, v12        ||cmu.vilv.x16 v24, v25, v31, v16  
vau.imuls.u32 v24, v30, v24     ||cmu.vilv.x16 v26, v27, v31, v17       ||lsu1.swzmv4.word [1111] [uuuu]   
vau.imuls.u32 v25, v30, v25     ||cmu.vilv.x16 v28, v29, v31, v18       ||lsu1.swzmv4.word [1111] [uuuu]   
vau.imuls.u32 v26, v30, v26     ||cmu.vdilv.x16 v23, v8 , v0, v4        ||lsu1.swzmv4.word [2222] [uuuu] 
vau.imuls.u32 v27, v30, v27     ||cmu.vdilv.x16 v13, v23, v3, v7        ||lsu1.swzmv4.word [2222] [uuuu] 
vau.imuls.u32 v28, v30, v28     ||cmu.vdilv.x16 v24, v25, v24, v25      ||lsu1.swzmv4.word [0000] [uuuu]   ||lsu0.ldo.16 i8, i4, 0x20  
vau.imuls.u32 v29, v30, v29     ||cmu.vdilv.x8 v24, v25, v24, v24       ||lsu1.swzmv4.word [0000] [uuuu]   ||lsu0.ldo.16 i9, i6, 0x20  
vau.macpz.u16 v8, v21           ||cmu.shriv.x16 v8, v8, i8              ||LSU0.LDXV v1, i5      ||iau.incs i5, 0x10
vau.macp.u16 v8, v22            ||cmu.vdilv.x16 v26, v27, v26, v27      ||LSU1.LDXV v5, i5      ||iau.incs i5, 0x10 
vau.macp.u16 v11, v20           ||cmu.shriv.x16 v11, v11, i9            ||LSU0.LDXV v2, i6      ||iau.incs i6, 0x10
vau.macpw.u16 v14, v11, v21     ||cmu.vdilv.x16 v28, v29, v28, v29      ||LSU1.LDXV v6, i6      ||iau.incs i6, 0x10
end_loop_combDecimDemosaicAwbGains:
vau.macpz.u16 v10, v20          ||cmu.shliv.x16 v19, v10, i10           ||LSU0.LDXV v0, i4      ||iau.incs i4, 0x10
vau.macp.u16  v19, v21          ||cmu.cpvi.x16 i10.l, v10.7             ||LSU1.LDXV v4, i4      ||iau.incs i4, 0x10
vau.macp.u16  v13, v21          ||cmu.shliv.x16 v19, v13, i11           ||LSU0.LDXV v3, i7      ||iau.incs i7, 0x10
vau.macpw.u16  v15, v19, v22    ||cmu.cpvi.x16 i11.l, v13.7             ||LSU1.LDXV v7, i7      ||iau.incs i7, 0x10
                                  cmu.vdilv.x8 v26, v27, v26, v26       ||LSU0.ST.64.l v24,i2   ||iau.incs i2, 0x8
                                  cmu.vdilv.x8 v28, v29, v28, v28       ||LSU0.ST.64.l v26,i3   ||iau.incs i3, 0x8  
                                                                          LSU1.ST.64.l v28,i1   ||iau.incs i1, 0x8 
delay_slot_combDecimDemosaicAwbGains:
// end loop

// this is not a sipp kernel, so need last pixel to be duplicated
lsu0.ldo.8 i10, i3, -2
lsu0.ldo.64.l v24 i19, 0x00 || lsu1.ldo.64.h v24 i19, 0x08
lsu0.ldo.64.l v25 i19, 0x10 || lsu1.ldo.64.h v25 i19, 0x18
lsu0.ldo.64.l v26 i19, 0x20 || lsu1.ldo.64.h v26 i19, 0x28
                                 
lsu0.ldo.64.l v27 i19, 0x30 || lsu1.ldo.64.h v27 i19, 0x38
lsu0.ldo.64.l v28 i19, 0x40 || lsu1.ldo.64.h v28 i19, 0x48
lsu0.ldo.64.l v29 i19, 0x50 || lsu1.ldo.64.h v29 i19, 0x58
lsu0.ldo.64.l v30 i19, 0x60 || lsu1.ldo.64.h v30 i19, 0x68
lsu0.ldo.64.l v31 i19, 0x70 || lsu1.ldo.64.h v31 i19, 0x78
IAU.add i19 i19 128 ||lsu0.sto.8 i10, i3, -1 || bru.jmp i30    
nop 6
//}
.nowarnend
.size mvispCombDecimDemosaicAwbGains_asm,.-mvispCombDecimDemosaicAwbGains_asm
.end
