.type mvcvCensusMatching32_asm,@function
.version 00.50.00
    
//////
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
/// 
/// mvcvCensusMatching32 
//     @param[in] in1            - pointer to input lines of the left image
//     @param[in] in2            - pointer to input lines of the right image
//     @param[out] out           - array of disparity cost
//     @param[in] width          - width of the input lines
//     @return    Nothing




.data .rodata.mvcvCensusMatching32_asm
.align 16

.code .text.mvcvCensusMatching32_asm
.align 16

.lalign
mvcvCensusMatching32_asm:
.nowarn 9,10,11,12
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
//mvcvCensusMatching32(u32 *in1, u32 *in2, u8 *out, u32 width, u32 flag)
//                     i18       i17       i16        i15       i14 
cmu.cmz.i32 i14
peu.pc1c neq || bru.bra CensusMatching32_flag
nop 6

LSU0.ldil i12, CensusMatching32_loop || LSU1.ldih i12, CensusMatching32_loop
iau.shr.u32 i15 i15 2


IAU.SUB i13 i17 16 
LSU0.LDO.64.L  v0 i18 0 || LSU1.LDO.64.H  v0 i18 8 || IAU.ADD i18 i18 16
LSU0.LDO.64.L  v1 i17 0 || LSU1.LDO.64.H  v1 i17 8 || IAU.ADD i17 i17 16 
LSU0.LDO.64.L  v2 i13 0 || LSU1.LDO.64.H  v2 i13 8 || IAU.SUB i13 i13 16 
LSU0.LDO.64.L  v3 i13 0 || LSU1.LDO.64.H  v3 i13 8 || IAU.SUB i13 i13 16
LSU0.LDO.64.L  v4 i13 0 || LSU1.LDO.64.H  v4 i13 8 || IAU.SUB i13 i13 16 
LSU0.LDO.64.L  v5 i13 0 || LSU1.LDO.64.H  v5 i13 8 || IAU.SUB i13 i13 16
LSU0.LDO.64.L  v6 i13 0 || LSU1.LDO.64.H  v6 i13 8 || IAU.SUB i13 i13 16
LSU0.LDO.64.L  v7 i13 0 || LSU1.LDO.64.H  v7 i13 8 || IAU.SUB i13 i13 16
LSU0.LDO.64.L  v8 i13 0 || LSU1.LDO.64.H  v8 i13 8 || IAU.SUB i13 i13 16
LSU0.LDO.64.L  v9 i13 0 || LSU1.LDO.64.H  v9 i13 8 || IAU.SUB i13 i13 16  
cmu.alignvec v10 v2 v1 12  || vau.xor v17 v0 v1 
cmu.alignvec v11 v2 v1 8   || vau.xor v18 v0 v10
cmu.alignvec v12 v2 v1 4   || vau.xor v19 v0 v11  
cmu.alignvec v13 v3 v2 12  || vau.xor v20 v0 v12
cmu.alignvec v14 v3 v2 8   || vau.xor v21 v0 v2
cmu.alignvec v15 v3 v2 4   || vau.xor v22 v0 v13
cmu.alignvec v16 v4 v3 12  || vau.xor v23 v0 v14
sau.onesx.x32 i0 v17
sau.onesx.x32 i1 v18 || bru.rpl i12 i15
sau.onesx.x32 i2 v19 || lsu0.cp v24.0 i0
sau.onesx.x32 i3 v20 || lsu0.cp v24.1 i1
sau.onesx.x32 i4 v21 || lsu0.cp v24.2 i2
sau.onesx.x32 i5 v22 || lsu0.cp v24.3 i3
sau.onesx.x32 i6 v23 || lsu0.cp v25.0 i4
cmu.alignvec v10 v4 v3 8   || vau.xor v17 v0 v15   || lsu0.cp v25.1 i5
cmu.alignvec v11 v4 v3 4   || vau.xor v18 v0 v3    || lsu0.cp v25.2 i6
cmu.alignvec v12 v5 v4 12  || vau.xor v19 v0 v16                          
cmu.alignvec v13 v5 v4 8   || vau.xor v20 v0 v10                          
cmu.alignvec v14 v5 v4 4   || vau.xor v21 v0 v11
cmu.alignvec v15 v6 v5 12  || vau.xor v22 v0 v4
cmu.alignvec v16 v6 v5 8   || vau.xor v23 v0 v12    
sau.onesx.x32 i0 v17
sau.onesx.x32 i1 v18
sau.onesx.x32 i2 v19 || lsu0.cp v25.3 i0
sau.onesx.x32 i3 v20 || lsu0.cp v26.0 i1
sau.onesx.x32 i4 v21 || lsu0.cp v26.1 i2
sau.onesx.x32 i5 v22 || lsu0.cp v26.2 i3
sau.onesx.x32 i6 v23 || lsu0.cp v26.3 i4
cmu.alignvec v10 v6 v5 4   || vau.xor v17 v0 v13   || lsu0.cp v27.0 i5
cmu.alignvec v11 v7 v6 12  || vau.xor v18 v0 v14   || lsu0.cp v27.1 i6
cmu.alignvec v12 v7 v6 8   || vau.xor v19 v0 v5                           
cmu.alignvec v13 v7 v6 4   || vau.xor v20 v0 v15
cmu.alignvec v14 v8 v7 12  || vau.xor v21 v0 v16
cmu.alignvec v15 v8 v7 8   || vau.xor v22 v0 v10                            
cmu.alignvec v16 v8 v7 4   || vau.xor v23 v0 v6
sau.onesx.x32 i0 v17
sau.onesx.x32 i1 v18
sau.onesx.x32 i2 v19 || lsu0.cp v27.2 i0
sau.onesx.x32 i3 v20 || lsu0.cp v27.3 i1
sau.onesx.x32 i4 v21 || lsu0.cp v28.0 i2
sau.onesx.x32 i5 v22 || lsu0.cp v28.1 i3
sau.onesx.x32 i6 v23 || lsu0.cp v28.2 i4
cmu.alignvec v10 v9 v8 12  || vau.xor v17 v0 v11   || lsu0.cp v28.3 i5
cmu.alignvec v11 v9 v8 8   || vau.xor v18 v0 v12   || lsu0.cp v29.0 i6
cmu.alignvec v12 v9 v8 4   || vau.xor v19 v0 v13
vau.xor v20 v0 v7    || sau.onesx.x32 i0 v17                        
vau.xor v21 v0 v14   || sau.onesx.x32 i1 v18
vau.xor v22 v0 v15    || sau.onesx.x32 i2 v19 || lsu0.cp v29.1 i0                        
vau.xor v23 v0 v16   || sau.onesx.x32 i3 v20 || lsu0.cp v29.2 i1
vau.xor v17 v0 v8    || sau.onesx.x32 i4 v21 || lsu0.cp v29.3 i2 || IAU.SUB i13 i17 16 
vau.xor v18 v0 v10   || sau.onesx.x32 i5 v22 || lsu0.cp v30.0 i3 || LSU1.LDO.64.L  v0 i18 0  
vau.xor v19 v0 v11   || sau.onesx.x32 i6 v23 || lsu0.cp v30.1 i4 || LSU1.LDO.64.H  v0 i18 8 || IAU.ADD i18 i18 16                                                                                                    
vau.xor v20 v0 v12   || sau.onesx.x32 i0 v17 || lsu0.cp v30.2 i5 || LSU1.LDO.64.L  v1 i17 0 
sau.onesx.x32 i1 v18 || lsu0.cp v30.3 i6 || LSU1.LDO.64.H  v1 i17 8 || IAU.ADD i17 i17 16 
sau.onesx.x32 i2 v19 || lsu0.cp v31.0 i0 || LSU1.LDO.64.L  v2 i13 0
sau.onesx.x32 i3 v20 || lsu0.cp v31.1 i1 || LSU1.LDO.64.H  v2 i13 8 || IAU.SUB i13 i13 16 
CMU.VDILV.x8 v25 v24 v24 v25 ||   lsu0.cp v31.2 i2 || LSU1.LDO.64.L  v3 i13 0 
CMU.VDILV.x8 v27 v26 v26 v27 ||   lsu0.cp v31.3 i3 || LSU1.LDO.64.H  v3 i13 8 || IAU.SUB i13 i13 16                                                                                                                                                        
CMU.VDILV.x8 v29 v28 v28 v29 || LSU0.LDO.64.L  v4 i13 0 || LSU1.LDO.64.H  v4 i13 8 || IAU.SUB i13 i13 16                                                                  
CMU.VDILV.x8 v31 v30 v30 v31 || LSU0.LDO.64.L  v5 i13 0 || LSU1.LDO.64.H  v5 i13 8 || IAU.SUB i13 i13 16                                                                                                                
CMU.VDILV.x8 v25 v27 v25 v27 || LSU0.LDO.64.L  v6 i13 0 || LSU1.LDO.64.H  v6 i13 8 || IAU.SUB i13 i13 16    
CMU.VDILV.x8 v24 v26 v24 v26 || LSU0.LDO.64.L  v7 i13 0 || LSU1.LDO.64.H  v7 i13 8 || IAU.SUB i13 i13 16    
CMU.VDILV.x8 v29 v31 v29 v31 || LSU0.LDO.64.L  v8 i13 0 || LSU1.LDO.64.H  v8 i13 8 || IAU.SUB i13 i13 16    
CMU.VDILV.x8 v28 v30 v28 v30 || LSU0.LDO.64.L  v9 i13 0 || LSU1.LDO.64.H  v9 i13 8 || IAU.SUB i13 i13 16    
LSU0.STO.64.L  v26 i16 0 || LSU1.STO.64.H  v26 i16 8 || IAU.ADD i16 i16 16    || cmu.alignvec v10 v2 v1 12  || vau.xor v17 v0 v1 
CensusMatching32_loop:                                                                                                             
LSU0.STO.64.L  v30 i16 0 || LSU1.STO.64.H  v30 i16 8 || IAU.ADD i16 i16 16    || cmu.alignvec v11 v2 v1 8   || vau.xor v18 v0 v10                                                                                                            
LSU0.STO.64.L  v27 i16 0 || LSU1.STO.64.H  v27 i16 8 || IAU.ADD i16 i16 16    || cmu.alignvec v12 v2 v1 4   || vau.xor v19 v0 v11                                                                                                              
LSU0.STO.64.L  v31 i16 0 || LSU1.STO.64.H  v31 i16 8 || IAU.ADD i16 i16 16    || cmu.alignvec v13 v3 v2 12  || vau.xor v20 v0 v12                                                                                                            
LSU0.STO.64.L  v24 i16 0 || LSU1.STO.64.H  v24 i16 8 || IAU.ADD i16 i16 16    || cmu.alignvec v14 v3 v2 8   || vau.xor v21 v0 v2                                                                                                            
LSU0.STO.64.L  v28 i16 0 || LSU1.STO.64.H  v28 i16 8 || IAU.ADD i16 i16 16    || cmu.alignvec v15 v3 v2 4   || vau.xor v22 v0 v13                                                                                                            
LSU0.STO.64.L  v25 i16 0 || LSU1.STO.64.H  v25 i16 8 || IAU.ADD i16 i16 16    || cmu.alignvec v16 v4 v3 12  || vau.xor v23 v0 v14                                                                                                            
LSU0.STO.64.L  v29 i16 0 || LSU1.STO.64.H  v29 i16 8 || IAU.ADD i16 i16 16    || sau.onesx.x32 i0 v17                                                                                                            
    

    
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
        
    
bru.jmp i30
nop 6

CensusMatching32_flag:
LSU0.ldil i12, CensusMatching32_loop2 || LSU1.ldih i12, CensusMatching32_loop2
iau.shr.u32 i15 i15 2


IAU.add i13 i17 16 
LSU0.LDO.64.L  v0 i18 0 || LSU1.LDO.64.H  v0 i18 8 || IAU.ADD i18 i18 16
LSU0.LDO.64.L  v1 i17 0 || LSU1.LDO.64.H  v1 i17 8 || IAU.ADD i17 i17 16 
LSU0.LDO.64.L  v2 i13 0 || LSU1.LDO.64.H  v2 i13 8 || IAU.add i13 i13 16 
LSU0.LDO.64.L  v3 i13 0 || LSU1.LDO.64.H  v3 i13 8 || IAU.add i13 i13 16
LSU0.LDO.64.L  v4 i13 0 || LSU1.LDO.64.H  v4 i13 8 || IAU.add i13 i13 16 
LSU0.LDO.64.L  v5 i13 0 || LSU1.LDO.64.H  v5 i13 8 || IAU.add i13 i13 16
LSU0.LDO.64.L  v6 i13 0 || LSU1.LDO.64.H  v6 i13 8 || IAU.add i13 i13 16
LSU0.LDO.64.L  v7 i13 0 || LSU1.LDO.64.H  v7 i13 8 || IAU.add i13 i13 16
LSU0.LDO.64.L  v8 i13 0 || LSU1.LDO.64.H  v8 i13 8 || IAU.add i13 i13 16
LSU0.LDO.64.L  v9 i13 0 || LSU1.LDO.64.H  v9 i13 8 || IAU.add i13 i13 16  
cmu.alignvec v10 v1 v2 4  || vau.xor v17 v0 v1 
cmu.alignvec v11 v1 v2 8   || vau.xor v18 v0 v10
cmu.alignvec v12 v1 v2 12   || vau.xor v19 v0 v11  
cmu.alignvec v13 v2 v3 4  || vau.xor v20 v0 v12
cmu.alignvec v14 v2 v3 8   || vau.xor v21 v0 v2
cmu.alignvec v15 v2 v3 12   || vau.xor v22 v0 v13
cmu.alignvec v16 v3 v4 4  || vau.xor v23 v0 v14
sau.onesx.x32 i0 v17
sau.onesx.x32 i1 v18 || bru.rpl i12 i15
sau.onesx.x32 i2 v19 || lsu0.cp v24.0 i0
sau.onesx.x32 i3 v20 || lsu0.cp v24.1 i1
sau.onesx.x32 i4 v21 || lsu0.cp v24.2 i2
sau.onesx.x32 i5 v22 || lsu0.cp v24.3 i3
sau.onesx.x32 i6 v23 || lsu0.cp v25.0 i4
cmu.alignvec v10 v3 v4 8   || vau.xor v17 v0 v15   || lsu0.cp v25.1 i5
cmu.alignvec v11 v3 v4 12   || vau.xor v18 v0 v3    || lsu0.cp v25.2 i6
cmu.alignvec v12 v4 v5 4  || vau.xor v19 v0 v16                          
cmu.alignvec v13 v4 v5 8   || vau.xor v20 v0 v10                          
cmu.alignvec v14 v4 v5 12   || vau.xor v21 v0 v11
cmu.alignvec v15 v5 v6 4  || vau.xor v22 v0 v4
cmu.alignvec v16 v5 v6 8   || vau.xor v23 v0 v12    
sau.onesx.x32 i0 v17
sau.onesx.x32 i1 v18
sau.onesx.x32 i2 v19 || lsu0.cp v25.3 i0
sau.onesx.x32 i3 v20 || lsu0.cp v26.0 i1
sau.onesx.x32 i4 v21 || lsu0.cp v26.1 i2
sau.onesx.x32 i5 v22 || lsu0.cp v26.2 i3
sau.onesx.x32 i6 v23 || lsu0.cp v26.3 i4
cmu.alignvec v10 v5 v6 12   || vau.xor v17 v0 v13   || lsu0.cp v27.0 i5
cmu.alignvec v11 v6 v7 4  || vau.xor v18 v0 v14   || lsu0.cp v27.1 i6
cmu.alignvec v12 v6 v7 8   || vau.xor v19 v0 v5                           
cmu.alignvec v13 v6 v7 12   || vau.xor v20 v0 v15
cmu.alignvec v14 v7 v8 4  || vau.xor v21 v0 v16
cmu.alignvec v15 v7 v8 8   || vau.xor v22 v0 v10                            
cmu.alignvec v16 v7 v8 12   || vau.xor v23 v0 v6
sau.onesx.x32 i0 v17
sau.onesx.x32 i1 v18
sau.onesx.x32 i2 v19 || lsu0.cp v27.2 i0
sau.onesx.x32 i3 v20 || lsu0.cp v27.3 i1
sau.onesx.x32 i4 v21 || lsu0.cp v28.0 i2
sau.onesx.x32 i5 v22 || lsu0.cp v28.1 i3
sau.onesx.x32 i6 v23 || lsu0.cp v28.2 i4
cmu.alignvec v10 v8 v9 4  || vau.xor v17 v0 v11   || lsu0.cp v28.3 i5
cmu.alignvec v11 v8 v9 8   || vau.xor v18 v0 v12   || lsu0.cp v29.0 i6
cmu.alignvec v12 v8 v9 12   || vau.xor v19 v0 v13
vau.xor v20 v0 v7    || sau.onesx.x32 i0 v17                        
vau.xor v21 v0 v14   || sau.onesx.x32 i1 v18
vau.xor v22 v0 v15    || sau.onesx.x32 i2 v19 || lsu0.cp v29.1 i0                        
vau.xor v23 v0 v16   || sau.onesx.x32 i3 v20 || lsu0.cp v29.2 i1

vau.xor v17 v0 v8    || sau.onesx.x32 i4 v21 || lsu0.cp v29.3 i2 || IAU.add i13 i17 16 
vau.xor v18 v0 v10   || sau.onesx.x32 i5 v22 || lsu0.cp v30.0 i3 || LSU1.LDO.64.L  v0 i18 0  
vau.xor v19 v0 v11   || sau.onesx.x32 i6 v23 || lsu0.cp v30.1 i4 || LSU1.LDO.64.H  v0 i18 8 || IAU.ADD i18 i18 16                                                                                                    
vau.xor v20 v0 v12   || sau.onesx.x32 i0 v17 || lsu0.cp v30.2 i5 || LSU1.LDO.64.L  v1 i17 0 
sau.onesx.x32 i1 v18 || lsu0.cp v30.3 i6 || LSU1.LDO.64.H  v1 i17 8 || IAU.ADD i17 i17 16 
sau.onesx.x32 i2 v19 || lsu0.cp v31.0 i0 || LSU1.LDO.64.L  v2 i13 0
sau.onesx.x32 i3 v20 || lsu0.cp v31.1 i1 || LSU1.LDO.64.H  v2 i13 8 || IAU.add i13 i13 16 

CMU.VDILV.x8 v25 v24 v24 v25 ||   lsu0.cp v31.2 i2 || LSU1.LDO.64.L  v3 i13 0 
CMU.VDILV.x8 v27 v26 v26 v27 ||   lsu0.cp v31.3 i3 || LSU1.LDO.64.H  v3 i13 8 || IAU.add i13 i13 16                                                                                                                                                        
CMU.VDILV.x8 v29 v28 v28 v29 || LSU0.LDO.64.L  v4 i13 0 || LSU1.LDO.64.H  v4 i13 8 || IAU.add i13 i13 16                                                                  
CMU.VDILV.x8 v31 v30 v30 v31 || LSU0.LDO.64.L  v5 i13 0 || LSU1.LDO.64.H  v5 i13 8 || IAU.add i13 i13 16                                                                                                                
CMU.VDILV.x8 v25 v27 v25 v27 || LSU0.LDO.64.L  v6 i13 0 || LSU1.LDO.64.H  v6 i13 8 || IAU.add i13 i13 16    
CMU.VDILV.x8 v24 v26 v24 v26 || LSU0.LDO.64.L  v7 i13 0 || LSU1.LDO.64.H  v7 i13 8 || IAU.add i13 i13 16    
CMU.VDILV.x8 v29 v31 v29 v31 || LSU0.LDO.64.L  v8 i13 0 || LSU1.LDO.64.H  v8 i13 8 || IAU.add i13 i13 16    
CMU.VDILV.x8 v28 v30 v28 v30 || LSU0.LDO.64.L  v9 i13 0 || LSU1.LDO.64.H  v9 i13 8 || IAU.add i13 i13 16    
LSU0.STO.64.L  v26 i16 0 || LSU1.STO.64.H  v26 i16 8 || IAU.ADD i16 i16 16    || cmu.alignvec v10 v1 v2 4  || vau.xor v17 v0 v1 
CensusMatching32_loop2:                                                                                                             
LSU0.STO.64.L  v30 i16 0 || LSU1.STO.64.H  v30 i16 8 || IAU.ADD i16 i16 16    || cmu.alignvec v11 v1 v2 8   || vau.xor v18 v0 v10                                                                                                            
LSU0.STO.64.L  v27 i16 0 || LSU1.STO.64.H  v27 i16 8 || IAU.ADD i16 i16 16    || cmu.alignvec v12 v1 v2 12   || vau.xor v19 v0 v11                                                                                                              
LSU0.STO.64.L  v31 i16 0 || LSU1.STO.64.H  v31 i16 8 || IAU.ADD i16 i16 16    || cmu.alignvec v13 v2 v3 4  || vau.xor v20 v0 v12                                                                                                            
LSU0.STO.64.L  v24 i16 0 || LSU1.STO.64.H  v24 i16 8 || IAU.ADD i16 i16 16    || cmu.alignvec v14 v2 v3 8   || vau.xor v21 v0 v2                                                                                                            
LSU0.STO.64.L  v28 i16 0 || LSU1.STO.64.H  v28 i16 8 || IAU.ADD i16 i16 16    || cmu.alignvec v15 v2 v3 12   || vau.xor v22 v0 v13                                                                                                            
LSU0.STO.64.L  v25 i16 0 || LSU1.STO.64.H  v25 i16 8 || IAU.ADD i16 i16 16    || cmu.alignvec v16 v3 v4 4  || vau.xor v23 v0 v14                                                                                                            
LSU0.STO.64.L  v29 i16 0 || LSU1.STO.64.H  v29 i16 8 || IAU.ADD i16 i16 16    || sau.onesx.x32 i0 v17                                                                                                            
    

    
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
        
    
bru.jmp i30
nop 6

.nowarnend
.size mvcvCensusMatching32_asm,.-mvcvCensusMatching32_asm
.end
