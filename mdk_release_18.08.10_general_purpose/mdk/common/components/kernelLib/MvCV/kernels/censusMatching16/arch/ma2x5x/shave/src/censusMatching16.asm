.type mvcvCensusMatching16_asm,@function
.version 00.50.00

//////
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
/// 
/// mvcvCensusMatching16 
//     @param[in] in1            - pointer to input lines of the left image
//     @param[in] in2            - pointer to input lines of the right image
//     @param[out] out           - array of disparity cost
//     @param[in] width          - width of the input lines
//     @return    Nothing




.data .rodata.mvcvCensusMatching16_asm
.align 16

.code .text.mvcvCensusMatching16_asm
.align 16

.lalign
mvcvCensusMatching16_asm:

//mvcvCensusMatching16(u32 *in1, u32 *in2, u8 *out, u32 width, u32 disparities)
//                     i18       i17       i16        i15         i14 
LSU0.ldil i12, CensusMatching16_loop || LSU1.ldih i12, CensusMatching16_loop
iau.shr.u32 i15 i15 2


IAU.SUB i13 i17 16  
LSU0.LDO.64.L  v0 i18 0 || LSU1.LDO.64.H  v0 i18 8 || IAU.ADD i18 i18 16
LSU0.LDO.64.L  v1 i17 0 || LSU1.LDO.64.H  v1 i17 8 || IAU.ADD i17 i17 16 
LSU0.LDO.64.L  v2 i13 0 || LSU1.LDO.64.H  v2 i13 8 || IAU.SUB i13 i13 16 
LSU0.LDO.64.L  v3 i13 0 || LSU1.LDO.64.H  v3 i13 8 || IAU.SUB i13 i13 16
LSU0.LDO.64.L  v4 i13 0 || LSU1.LDO.64.H  v4 i13 8 || IAU.SUB i13 i13 16 
LSU0.LDO.64.L  v5 i13 0 || LSU1.LDO.64.H  v5 i13 8 || IAU.SUB i13 i13 16
nop 6

cmu.alignvec  v6  v2  v1 12  || VAU.xor v12 v0 v1 
cmu.alignvec  v7  v2  v1 8   || VAU.xor v13 v0 v6
cmu.alignvec  v8  v2  v1 4   || VAU.xor v14 v0 v7
cmu.alignvec  v9  v3  v2 12  || VAU.xor v15 v0 v8
cmu.alignvec v10  v3  v2 8   || VAU.xor v16 v0 v2 || bru.rpl i12 i15
cmu.alignvec v11  v3  v2 4   || VAU.xor v17 v0 v9
                                VAU.xor v18 v0 v10 || sau.onesx.x32 i0 v12 
                                VAU.xor v19 v0 v11 || sau.onesx.x32 i1 v13
                                                      sau.onesx.x32 i2 v14  || lsu0.cp v20.0 i0 
                                                      sau.onesx.x32 i3 v15  || lsu0.cp v20.1 i1 
                                                      sau.onesx.x32 i4 v16  || lsu0.cp v20.2 i2 
                                                      sau.onesx.x32 i5 v17  || lsu0.cp v20.3 i3 
                                                      sau.onesx.x32 i6 v18  || lsu0.cp v21.0 i4
                                                      sau.onesx.x32 i7 v19  || lsu0.cp v21.1 i5
cmu.alignvec   v6  v4  v3 12 || VAU.xor v12 v0 v3                           || lsu0.cp v21.2 i6
cmu.alignvec   v7  v4  v3 8  || VAU.xor v13 v0 v6                           || lsu0.cp v21.3 i7
cmu.alignvec   v8  v4  v3 4  || VAU.xor v14 v0 v7  
cmu.alignvec   v9  v5  v4 12 || VAU.xor v15 v0 v8  
cmu.alignvec  v10  v5  v4 8  || VAU.xor v16 v0 v4  
cmu.alignvec  v11  v5  v4 4  || VAU.xor v17 v0 v9  || IAU.SUB i13 i17 16
                                VAU.xor v18 v0 v10 || sau.onesx.x32 i0 v12                    || LSU0.LDO.64.L  v0 i18 0
                                VAU.xor v19 v0 v11 || sau.onesx.x32 i1 v13                    || LSU0.LDO.64.H  v0 i18 8 || IAU.ADD i18 i18 16
                                                      sau.onesx.x32 i2 v14|| lsu1.cp v22.0 i0 || LSU0.LDO.64.L  v1 i17 0
								                      sau.onesx.x32 i3 v15|| lsu1.cp v22.1 i1 || LSU0.LDO.64.H  v1 i17 8 || IAU.ADD i17 i17 16 
								                      sau.onesx.x32 i4 v16|| lsu1.cp v22.2 i2 || LSU0.LDO.64.L  v2 i13 0 
								                      sau.onesx.x32 i5 v17|| lsu1.cp v22.3 i3 || LSU0.LDO.64.H  v2 i13 8 || IAU.SUB i13 i13 16 
								                      sau.onesx.x32 i6 v18|| lsu1.cp v23.0 i4 || LSU0.LDO.64.L  v3 i13 0
								                      sau.onesx.x32 i7 v19|| lsu1.cp v23.1 i5 || LSU0.LDO.64.H  v3 i13 8 || IAU.SUB i13 i13 16 
								                                             lsu1.cp v23.2 i6 || LSU0.LDO.64.L  v4 i13 0
                                                                             

CMU.VDILV.x8 v20 v21 v20 v21 || lsu1.cp v23.3 i7 || LSU0.LDO.64.H  v4 i13 8 || IAU.SUB i13 i13 16 	
CensusMatching16_loop: 		
.nowarn 9
CMU.VDILV.x8 v22 v23 v22 v23 || LSU0.LDO.64.L  v5 i13 0 || LSU1.LDO.64.H  v5 i13 8 || IAU.SUB i13 i13 16	 									
.nowarnend
////////
CMU.VDILV.x8 v22 v20 v20 v22 
CMU.VDILV.x8 v23 v21 v21 v23																			 
																			 
LSU0.STO.64.L  v21 i16 0 || LSU1.STO.64.H  v21 i16 8 || IAU.ADD i16 i16 16	|| cmu.alignvec  v6  v2  v1 12  || VAU.xor v12 v0 v1 																											
LSU0.STO.64.L  v20 i16 0 || LSU1.STO.64.H  v20 i16 8 || IAU.ADD i16 i16 16	|| cmu.alignvec  v7  v2  v1 8   || VAU.xor v13 v0 v6																											
LSU0.STO.64.L  v23 i16 0 || LSU1.STO.64.H  v23 i16 8 || IAU.ADD i16 i16 16	|| cmu.alignvec  v8  v2  v1 4   || VAU.xor v14 v0 v7																											
LSU0.STO.64.L  v22 i16 0 || LSU1.STO.64.H  v22 i16 8 || IAU.ADD i16 i16 16	|| cmu.alignvec  v9  v3  v2 12  || VAU.xor v15 v0 v8

																				
	

	
bru.jmp i30
nop 6
.size mvcvCensusMatching16_asm,.-mvcvCensusMatching16_asm
.end
