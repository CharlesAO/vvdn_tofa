.type mvcvCensusMatching65_asm,@function
.version 00.50.00

//////
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
/// 
/// mvcvCensusMatching65 
//     @param[in] in1            - pointer to input lines of the left image
//     @param[in] in2            - pointer to input lines of the right image
//     @param[out] out           - array of disparity cost
//     @param[in] width          - width of the input lines
//     @return    Nothing




.data .rodata.mvcvCensusMatching65_asm
.align 16

.code .text.mvcvCensusMatching65_asm
.align 16

.lalign
mvcvCensusMatching65_asm:

	IAU.SUB i19 i19 4
	LSU0.ST.32  i20  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i21  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i22  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i23  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i24  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i25  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i26  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i27  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i28  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i29  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i30  i19 || IAU.SUB i19 i19 4
	LSU0.ST.32  i31  i19 

	IAU.SUB i19 i19 8
	LSU0.ST.64.l v24  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v24  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v25  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v25  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v26  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v26  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v27  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v27  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v28  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v28  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v29  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v29  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v30  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v30  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v31  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v31  i19  
//mvcvCensusMatching65(u32 *in1, u32 *in2, u8 *out, u32 width, u32 disparities)
//                     i18       i17       i16        i15         i14 
LSU0.ldil i12, CensusMatching65_loop || LSU1.ldih i12, CensusMatching65_loop
iau.shr.u32 i15 i15 2


IAU.SUB i13 i17 16 
LSU0.LDO.64.L   v0 i18 0 || LSU1.LDO.64.H   v0 i18 8 || IAU.ADD i18 i18 16
LSU0.LDO.64.L   v1 i17 0 || LSU1.LDO.64.H   v1 i17 8 || IAU.ADD i17 i17 16
LSU0.LDO.64.L   v2 i13 0 || LSU1.LDO.64.H   v2 i13 8 || IAU.SUB i13 i13 16
LSU0.LDO.64.L   v3 i13 0 || LSU1.LDO.64.H   v3 i13 8 || IAU.SUB i13 i13 16

LSU0.LDO.64.L   v4 i13 0 || LSU1.LDO.64.H   v4 i13 8 || IAU.SUB i13 i13 16 
LSU0.LDO.64.L   v5 i13 0 || LSU1.LDO.64.H   v5 i13 8 || IAU.SUB i13 i13 16
LSU0.LDO.64.L   v6 i13 0 || LSU1.LDO.64.H   v6 i13 8 || IAU.SUB i13 i13 16
LSU0.LDO.64.L   v7 i13 0 || LSU1.LDO.64.H   v7 i13 8 || IAU.SUB i13 i13 16
LSU0.LDO.64.L   v8 i13 0 || LSU1.LDO.64.H   v8 i13 8 || IAU.SUB i13 i13 16
LSU0.LDO.64.L   v9 i13 0 || LSU1.LDO.64.H   v9 i13 8 || IAU.SUB i13 i13 16  
LSU0.LDO.64.L  v10 i13 0 || LSU1.LDO.64.H  v10 i13 8 || IAU.SUB i13 i13 16  
LSU0.LDO.64.L  v11 i13 0 || LSU1.LDO.64.H  v11 i13 8 || IAU.SUB i13 i13 16  
LSU0.LDO.64.L  v12 i13 0 || LSU1.LDO.64.H  v12 i13 8 || IAU.SUB i13 i13 16  
LSU0.LDO.64.L  v13 i13 0 || LSU1.LDO.64.H  v13 i13 8 || IAU.SUB i13 i13 16  
LSU0.LDO.64.L  v14 i13 0 || LSU1.LDO.64.H  v14 i13 8 || IAU.SUB i13 i13 16  
LSU0.LDO.64.L  v15 i13 0 || LSU1.LDO.64.H  v15 i13 8 || IAU.SUB i13 i13 16  
LSU0.LDO.64.L  v16 i13 0 || LSU1.LDO.64.H  v16 i13 8 || IAU.SUB i13 i13 16  
LSU0.LDO.64.L  v17 i13 0 || LSU1.LDO.64.H  v17 i13 8 || IAU.SUB i13 i13 16  

cmu.alignvec  v19  v2  v1 12   || vau.xor v18 v0 v1 
cmu.alignvec  v20  v2  v1 8    || vau.xor v19 v0 v19
cmu.alignvec  v21  v2  v1 4    || vau.xor v20 v0 v20 
                                  vau.xor v21 v0 v21 || sau.onesx.x32 i0 v18
								                        sau.onesx.x32 i1 v19  
								                        sau.onesx.x32 i2 v20   
								                        sau.onesx.x32 i3 v21  																						                     
cmu.alignvec  v19  v3   v2 12  || vau.xor v18 v0 v2                           
cmu.alignvec  v20  v3   v2 8   || vau.xor v19 v0 v19                          
cmu.alignvec  v21  v3   v2 4   || vau.xor v20 v0 v20 
                                  vau.xor v21 v0 v21 || sau.onesx.x32 i4 v18
								                        sau.onesx.x32 i5 v19
								                        sau.onesx.x32 i6 v20
								                        sau.onesx.x32 i7 v21
cmu.alignvec  v19  v4   v3 12  || vau.xor v18 v0 v3                           
cmu.alignvec  v20  v4   v3 8   || vau.xor v19 v0 v19                          
cmu.alignvec  v21  v4   v3 4   || vau.xor v20 v0 v20                          
                                  vau.xor v21 v0 v21 || sau.onesx.x32 i20 v18 
								                        sau.onesx.x32 i21 v19 
														
								                        sau.onesx.x32 i22 v20  || bru.rpl i12 i15
								                        sau.onesx.x32 i23 v21 
cmu.alignvec  v19  v5   v4 12  || vau.xor v18 v0 v4                           
cmu.alignvec  v20  v5   v4 8   || vau.xor v19 v0 v19                          
cmu.alignvec  v21  v5   v4 4   || vau.xor v20 v0 v20                          
                                  vau.xor v21 v0 v21 || sau.onesx.x32 i24 v18 
								                        sau.onesx.x32 i25 v19 || lsu0.cp v22.0 i0 
								                        sau.onesx.x32 i26 v20 || lsu0.cp v22.1 i1  
								                        sau.onesx.x32 i27 v21 || lsu0.cp v22.2 i2 
cmu.alignvec  v19  v6   v5 12  || vau.xor v18 v0 v5                           || lsu0.cp v22.3 i3
cmu.alignvec  v20  v6   v5 8   || vau.xor v19 v0 v19                          || lsu0.cp v23.0 i4
cmu.alignvec  v21  v6   v5 4   || vau.xor v20 v0 v20                          || lsu0.cp v23.1 i5
                                  vau.xor v21 v0 v21 || sau.onesx.x32 i0 v18  || lsu0.cp v23.2 i6
								                        sau.onesx.x32 i1 v19  || lsu0.cp v23.3 i7
								                        sau.onesx.x32 i2 v20  || lsu0.cp v24.0 i20
								                        sau.onesx.x32 i3 v21  || lsu0.cp v24.1 i21
cmu.alignvec  v19  v7   v6 12  || vau.xor v18 v0 v6                           || lsu0.cp v24.2 i22
cmu.alignvec  v20  v7   v6 8   || vau.xor v19 v0 v19                          || lsu0.cp v24.3 i23
cmu.alignvec  v21  v7   v6 4   || vau.xor v20 v0 v20                          || lsu0.cp v25.0 i24
                                  vau.xor v21 v0 v21 || sau.onesx.x32 i4 v18  || lsu0.cp v25.1 i25
								                        sau.onesx.x32 i5 v19  || lsu0.cp v25.2 i26
								                        sau.onesx.x32 i6 v20  || lsu0.cp v25.3 i27
								                        sau.onesx.x32 i7 v21  || lsu0.cp v26.0 i0
cmu.alignvec  v19  v8   v7 12  || vau.xor v18 v0 v7                           || lsu0.cp v26.1 i1
cmu.alignvec  v20  v8   v7 8   || vau.xor v19 v0 v19                          || lsu0.cp v26.2 i2
cmu.alignvec  v21  v8   v7 4   || vau.xor v20 v0 v20                          || lsu0.cp v26.3 i3
                                  vau.xor v21 v0 v21 || sau.onesx.x32 i20 v18 || lsu0.cp v27.0 i4
								                        sau.onesx.x32 i21 v19 || lsu0.cp v27.1 i5
								                        sau.onesx.x32 i22 v20 || lsu0.cp v27.2 i6
								                        sau.onesx.x32 i23 v21 || lsu0.cp v27.3 i7
cmu.alignvec  v19  v9   v8 12  || vau.xor v18 v0 v8                          
cmu.alignvec  v20  v9   v8 8   || vau.xor v19 v0 v19                          || lsu0.cp v28.0 i20
cmu.alignvec  v21  v9   v8 4   || vau.xor v20 v0 v20                          || lsu0.cp v28.1 i21
                                  vau.xor v21 v0 v21 || sau.onesx.x32 i24 v18 || lsu0.cp v28.2 i22
								                        sau.onesx.x32 i25 v19 || lsu0.cp v28.3 i23
								                        sau.onesx.x32 i26 v20|| lsu0.cp v29.0 i24
								                        sau.onesx.x32 i27 v21|| lsu0.cp v29.1 i25
cmu.alignvec v19  v10   v9 12  || vau.xor v18 v0 v9                          || lsu0.cp v29.2 i26
cmu.alignvec v20  v10   v9 8   || vau.xor v19 v0 v19                         || lsu0.cp v29.3 i27
cmu.alignvec v21  v10   v9 4   || vau.xor v20 v0 v20 
                                  vau.xor v21 v0 v21 || sau.onesx.x32 i0 v18
								                        sau.onesx.x32 i1 v19
								                        sau.onesx.x32 i2 v20|| lsu0.cp v30.0 i0
								                        sau.onesx.x32 i3 v21|| lsu0.cp v30.1 i1
cmu.alignvec v19  v11  v10 12  || vau.xor v18 v0 v10                        || lsu0.cp v30.2 i2
cmu.alignvec v20  v11  v10 8   || vau.xor v19 v0 v19                        || lsu0.cp v30.3 i3
cmu.alignvec v21  v11  v10 4   || vau.xor v20 v0 v20 
                                  vau.xor v21 v0 v21 || sau.onesx.x32 i4 v18
								                        sau.onesx.x32 i5 v19
								                        sau.onesx.x32 i6 v20|| lsu0.cp v31.0 i4
								                        sau.onesx.x32 i7 v21|| lsu0.cp v31.1 i5
cmu.alignvec v19  v12  v11 12  || vau.xor v18 v0 v11                        || lsu0.cp v31.2 i6
cmu.alignvec v20  v12  v11 8   || vau.xor v19 v0 v19                        || lsu0.cp v31.3 i7
cmu.alignvec v21  v12  v11 4   || vau.xor v20 v0 v20 
                                  vau.xor v21 v0 v21 || sau.onesx.x32 i20 v18
								                        sau.onesx.x32 i21 v19
								                        sau.onesx.x32 i22 v20|| lsu0.cp v11.0 i20
								                        sau.onesx.x32 i23 v21|| lsu0.cp v11.1 i21
cmu.alignvec v19  v13  v12 12  || vau.xor v18 v0 v12                         || lsu0.cp v11.2 i22
cmu.alignvec v20  v13  v12 8   || vau.xor v19 v0 v19                         || lsu0.cp v11.3 i23
cmu.alignvec v21  v13  v12 4   || vau.xor v20 v0 v20                   
                                  vau.xor v21 v0 v21 || sau.onesx.x32 i24 v18
								                        sau.onesx.x32 i25 v19
								                        sau.onesx.x32 i26 v20|| lsu0.cp v12.0 i24
								                        sau.onesx.x32 i27 v21|| lsu0.cp v12.1 i25
cmu.alignvec v19  v14  v13 12  || vau.xor v18 v0 v13                         || lsu0.cp v12.2 i26
cmu.alignvec v20  v14  v13 8   || vau.xor v19 v0 v19                         || lsu0.cp v12.3 i27
cmu.alignvec v21  v14  v13 4   || vau.xor v20 v0 v20 
                                  vau.xor v21 v0 v21 || sau.onesx.x32 i0 v18
								                        sau.onesx.x32 i1 v19
								                        sau.onesx.x32 i2 v20|| lsu0.cp v13.0 i0
								                        sau.onesx.x32 i3 v21|| lsu0.cp v13.1 i1
cmu.alignvec v19  v15  v14 12  || vau.xor v18 v0 v14                        || lsu0.cp v13.2 i2
cmu.alignvec v20  v15  v14 8   || vau.xor v19 v0 v19                        || lsu0.cp v13.3 i3
cmu.alignvec v21  v15  v14 4   || vau.xor v20 v0 v20 
                                  vau.xor v21 v0 v21 || sau.onesx.x32 i4 v18 
								                        sau.onesx.x32 i5 v19
								                        sau.onesx.x32 i6 v20|| lsu0.cp v14.0 i4
								                        sau.onesx.x32 i7 v21|| lsu0.cp v14.1 i5
cmu.alignvec v19  v16  v15 12  || vau.xor v18 v0 v15                        || lsu0.cp v14.2 i6
cmu.alignvec v20  v16  v15 8   || vau.xor v19 v0 v19                        || lsu0.cp v14.3 i7
cmu.alignvec v21  v16  v15 4   || vau.xor v20 v0 v20 
                                  vau.xor v21 v0 v21 || sau.onesx.x32 i20 v18
								                        sau.onesx.x32 i21 v19
								                        sau.onesx.x32 i22 v20|| lsu0.cp v15.0 i20
								                        sau.onesx.x32 i23 v21|| lsu0.cp v15.1 i21
cmu.alignvec v19  v17  v16 12  || vau.xor v18 v0 v16                         || lsu0.cp v15.2 i22
cmu.alignvec v20  v17  v16 8   || vau.xor v19 v0 v19                         || lsu0.cp v15.3 i23 || IAU.SUB i13 i17 16  || LSU1.LDO.64.L   v0 i18 0
cmu.alignvec v21  v17  v16 4   || vau.xor v20 v0 v20  || LSU1.LDO.64.H   v0 i18 8 || IAU.ADD i18 i18 16                   
                                  vau.xor v21 v0 v21 || sau.onesx.x32 i24 v18 || LSU0.LDO.64.L   v1 i17 0 
                                                        sau.onesx.x32 i25 v19|| vau.xor v17 v0 v17 || LSU1.LDO.64.H   v1 i17 8 || IAU.ADD i17 i17 16 
                                                        sau.onesx.x32 i26 v20|| lsu0.cp v16.0 i24 || LSU1.LDO.64.L   v2 i13 0
                                                        sau.onesx.x32 i27 v21|| lsu0.cp v16.1 i25 || LSU1.LDO.64.H   v2 i13 8 || IAU.SUB i13 i13 16
                                                        sau.onesx.x32 i28 v17 || lsu0.cp v16.2 i26  || LSU1.LDO.64.L   v3 i13 0
                                                                                 lsu0.cp v16.3 i27 || LSU1.LDO.64.H   v3 i13 8 || IAU.SUB i13 i13 16
                                
CMU.VDILV.x8 v23 v22 v22 v23 || IAU.align i29 i28 i28 1 || LSU0.ldil i22, 0x0104 || LSU1.ldih i22, 0x0000
CMU.VDILV.x8 v25 v24 v24 v25 || IAU.align i30 i29 i29 1
CMU.VDILV.x8 v27 v26 v26 v27 || IAU.align i31 i30 i30 1
CMU.VDILV.x8 v29 v28 v28 v29 || LSU0.LDO.64.L   v4 i13 0 || LSU1.LDO.64.H   v4 i13 8 || IAU.SUB i13 i13 16 
CMU.VDILV.x8 v31 v30 v30 v31 || LSU0.LDO.64.L   v5 i13 0 || LSU1.LDO.64.H   v5 i13 8 || IAU.SUB i13 i13 16
CMU.VDILV.x8 v12 v11 v11 v12 || LSU0.LDO.64.L   v6 i13 0 || LSU1.LDO.64.H   v6 i13 8 || IAU.SUB i13 i13 16
CMU.VDILV.x8 v14 v13 v13 v14 || LSU0.LDO.64.L   v7 i13 0 || LSU1.LDO.64.H   v7 i13 8 || IAU.SUB i13 i13 16
CMU.VDILV.x8 v16 v15 v15 v16 || LSU0.LDO.64.L   v8 i13 0 || LSU1.LDO.64.H   v8 i13 8 || IAU.SUB i13 i13 16
.nowarn 10,11,12
CMU.VDILV.x8 v23 v25 v23 v25 || LSU0.LDO.64.L   v9 i13 0 || LSU1.LDO.64.H   v9 i13 8 || IAU.SUB i13 i13 16  
CMU.VDILV.x8 v22 v24 v22 v24 || LSU0.LDO.64.L  v10 i13 0 || LSU1.LDO.64.H  v10 i13 8 || IAU.SUB i13 i13 16  
CMU.VDILV.x8 v27 v29 v27 v29 || LSU0.LDO.64.L  v11 i13 0 || LSU1.LDO.64.H  v11 i13 8 || IAU.SUB i13 i13 16  
CMU.VDILV.x8 v26 v28 v26 v28 || LSU0.LDO.64.L  v12 i13 0 || LSU1.LDO.64.H  v12 i13 8 || IAU.SUB i13 i13 16  
CMU.VDILV.x8 v31 v12 v31 v12 || LSU0.LDO.64.L  v13 i13 0 || LSU1.LDO.64.H  v13 i13 8 || IAU.SUB i13 i13 16  
CMU.VDILV.x8 v30 v11 v30 v11 || LSU0.LDO.64.L  v14 i13 0 || LSU1.LDO.64.H  v14 i13 8 || IAU.SUB i13 i13 16  
CMU.VDILV.x8 v14 v16 v14 v16 || LSU0.LDO.64.L  v15 i13 0 || LSU1.LDO.64.H  v15 i13 8 || IAU.SUB i13 i13 16  
CMU.VDILV.x8 v13 v15 v13 v15 || LSU0.LDO.64.L  v16 i13 0 || LSU1.LDO.64.H  v16 i13 8 || IAU.SUB i13 i13 16  


LSU0.STO.64.L  v11 i16 32  || LSU1.STO.64.H  v11 i16 40                             || cmu.alignvec  v19  v2  v1 12   || vau.xor v18 v0 v1 
LSU0.STO.64.L  v12 i16 97  || LSU1.STO.64.H  v12 i16 105	                        || cmu.alignvec  v20  v2  v1 8    || vau.xor v19 v0 v19
LSU0.STO.64.L  v13 i16 178 || LSU1.STO.64.H  v13 i16 186                            || cmu.alignvec  v21  v2  v1 4    || vau.xor v20 v0 v20 
LSU0.STO.64.L  v14 i16 243 || LSU1.STO.64.H  v14 i16 251 		                    ||                                   vau.xor v21 v0 v21 || sau.onesx.x32 i0 v18
LSU0.STO.64.L  v15 i16 48  || LSU1.STO.64.H  v15 i16 56 	                        || 								                        sau.onesx.x32 i1 v19  
LSU0.STO.64.L  v16 i16 113 || LSU1.STO.64.H  v16 i16 121	                        || 								                        sau.onesx.x32 i2 v20   
LSU0.STO.64.L  v24 i16 0   || LSU1.STO.64.H  v24 i16 8 								||									                        sau.onesx.x32 i3 v21  																			
LSU0.STO.64.L  v28 i16 16  || LSU1.STO.64.H  v28 i16 24 							||	cmu.alignvec  v19  v3   v2 12  || vau.xor v18 v0 v2                           																																										
lsu0.sto.8 i28 i16 64  || lsu1.sto.8 i29 i16 129									||	cmu.alignvec  v20  v3   v2 8   || vau.xor v19 v0 v19                          																
LSU0.STO.64.L  v25 i16 65  || LSU1.STO.64.H  v25 i16 73 							||	cmu.alignvec  v21  v3   v2 4   || vau.xor v20 v0 v20 																			
LSU0.STO.64.L  v29 i16 81  || LSU1.STO.64.H  v29 i16 89 							||	                                  vau.xor v21 v0 v21 || sau.onesx.x32 i4 v18																																												
LSU0.STO.64.L  v22 i16 130 || LSU1.STO.64.H  v22 i16 138 							||									                        sau.onesx.x32 i5 v19																		
CensusMatching65_loop: 
LSU0.STO.64.L  v26 i16 146 || LSU1.STO.64.H  v26 i16 154 							||									                        sau.onesx.x32 i6 v20																		
LSU0.STO.64.L  v30 i16 162 || LSU1.STO.64.H  v30 i16 170 							||									                        sau.onesx.x32 i7 v21																																											
lsu0.sto.8 i30 i16 194 || lsu1.sto.8 i31 i16 259                                    || cmu.alignvec  v19  v4   v3 12  || vau.xor v18 v0 v3                           
LSU0.STO.64.L  v23 i16 195 || LSU1.STO.64.H  v23 i16 203 							||	cmu.alignvec  v20  v4   v3 8   || vau.xor v19 v0 v19                          																		
LSU0.STO.64.L  v27 i16 211 || LSU1.STO.64.H  v27 i16 219 							||	cmu.alignvec  v21  v4   v3 4   || vau.xor v20 v0 v20                          																		
LSU0.STO.64.L  v31 i16 227 || LSU1.STO.64.H  v31 i16 235 ||	iau.add i16 i16 i22		||	                                  vau.xor v21 v0 v21 || sau.onesx.x32 i20 v18 																							
LSU0.LDO.64.L  v17 i13 0 || LSU1.LDO.64.H  v17 i13 8 || IAU.SUB i13 i13 16  		||									                        sau.onesx.x32 i21 v19 																					
.nowarnend																									
		
																							
																												
																												

	
	
	
	

	




	LSU0.LD.64.h  v31  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v31  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v30  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v30  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v29  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v29  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v28  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v28  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v27  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v27  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v26  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v26  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v25  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v25  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v24  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v24  i19 || IAU.ADD i19 i19 8		
	
	LSU0.LD.32  i31  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i30  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i29  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i28  i19 || IAU.ADD i19 i19 4	
	LSU0.LD.32  i27  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i26  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i25  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i24  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i23  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i22  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i21  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32  i20  i19 || IAU.ADD i19 i19 4
		nop 6
	
	

bru.jmp i30
nop 6
.size mvcvCensusMatching65_asm,.-mvcvCensusMatching65_asm
.end
