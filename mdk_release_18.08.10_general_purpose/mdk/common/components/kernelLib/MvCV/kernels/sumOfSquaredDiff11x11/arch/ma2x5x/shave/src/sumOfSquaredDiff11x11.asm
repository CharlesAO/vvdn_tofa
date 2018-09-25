///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief   
///

.type mvcvSumOfSquaredDiff11x11_asm,@function
.version 00.51.06
//-------------------------------------------------------------------------------
.data .rodata.sumOfSquaredDiff11x11
.align 16

 sumOfSquaredDiff11x11_clampVal:
		.short 255
	
.code .text.sumOfSquaredDiff11x11
//void sumOfSquaredDiff11x11(u8** in1(i18), u8** in2(i17), u8** out(i16), u32 width(i15))
mvcvSumOfSquaredDiff11x11_asm:
.nowarn 10
	LSU0.LDIL i0, sumOfSquaredDiff11x11_clampVal 		|| LSU1.LDIH i0, sumOfSquaredDiff11x11_clampVal
	
	IAU.SUB i19 i19 4		  || LSU1.LD.16R v20, i0 
	LSU0.ST.32 	i20  i19	|| IAU.SUB i19 i19 4
	LSU0.ST.32 	i21  i19	|| IAU.SUB i19 i19 4
	LSU0.ST.32 	i22  i19	|| IAU.SUB i19 i19 4
	LSU0.ST.32 	i23  i19	|| IAU.SUB i19 i19 4
	LSU0.ST.32 	i24  i19	|| IAU.SUB i19 i19 4
	LSU0.ST.32 	i25  i19	|| IAU.SUB i19 i19 4
	LSU0.ST.32 	i26  i19	|| IAU.SUB i19 i19 4
	LSU0.ST.32 	i27  i19	|| IAU.SUB i19 i19 8		
	LSU0.ST.64.L v24 i19	|| IAU.SUB i19 i19 8
	LSU0.ST.64.H v24 i19	|| IAU.SUB i19 i19 8
	LSU0.ST.64.L v25 i19	|| IAU.SUB i19 i19 8
	LSU0.ST.64.H v25 i19	|| IAU.SUB i19 i19 8
	LSU0.ST.64.L v26 i19	|| IAU.SUB i19 i19 8
	LSU0.ST.64.H v26 i19	|| IAU.SUB i19 i19 8
	LSU0.ST.64.L v27 i19	|| IAU.SUB i19 i19 8
	LSU0.ST.64.H v27 i19	|| IAU.SUB i19 i19 8
	LSU0.ST.64.L v28 i19	|| IAU.SUB i19 i19 8
	LSU0.ST.64.H v28 i19	|| IAU.SUB i19 i19 8
  LSU0.ST.64.L v29 i19	|| IAU.SUB i19 i19 8
	LSU0.ST.64.H v29 i19	
	 
	
	LSU0.LDI.32 i0  i18  || IAU.SHR.u32 i15 i15 3                
	LSU0.LDI.32 i1  i18  || IAU.SUB i15 i15 1            
	LSU0.LDI.32 i2  i18  || LSU1.LD.32 i16 i16 
	LSU0.LDI.32 i3  i18   
	LSU0.LDI.32 i4  i18   
	LSU0.LDI.32 i5  i18  || VAU.XOR v14 v14 v14
	LSU0.LDI.32 i6  i18	
	LSU0.LDI.32 i7  i18	 || IAU.SUB i0  i0  8
	LSU0.LDI.32 i8  i18	 || IAU.SUB i1  i1  8
	LSU0.LDI.32 i9  i18	 || IAU.SUB i2  i2  8
	LSU0.LDI.32 i10 i18	 || IAU.SUB i3  i3  8
                        
	LSU0.LDI.32 i11 i17 || IAU.SUB i4  i4  8				
	LSU0.LDI.32 i12 i17 || IAU.SUB i5  i5  8				
	LSU0.LDI.32 i13 i17 || IAU.SUB i6  i6  8
	LSU0.LDI.32 i14 i17 || IAU.SUB i7  i7  8
	LSU0.LDI.32 i20 i17	|| IAU.SUB i8  i8  8
	LSU0.LDI.32 i21 i17 || IAU.SUB i9  i9  8
	LSU0.LDI.32 i22 i17 || IAU.SUB i10 i10 8
	LSU0.LDI.32 i23 i17 || IAU.SUB i11 i11 8
  LSU0.LDI.32 i24 i17 || IAU.SUB i12 i12 8
	LSU0.LDI.32 i25 i17 || IAU.SUB i13 i13 8
	LSU0.LDI.32 i26 i17 || IAU.SUB i14 i14 8
						   IAU.SUB i20 i20 8	||  LSU0.LDI.128.U8.U16 v0 i0	|| LSU1.LDI.128.U8.U16 v3 i11
													LSU0.LDI.128.U8.U16 v1 i0	|| LSU1.LDI.128.U8.U16 v4 i11
													LSU0.LDI.128.U8.U16 v2 i0	|| LSU1.LDI.128.U8.U16 v5 i11
	
	IAU.SUB i21 i21 8		|| LSU0.LDI.128.U8.U16 v0 i1	|| LSU1.LDI.128.U8.U16 v3 i12
	IAU.SUB i22 i22 8		|| LSU0.LDI.128.U8.U16 v1 i1	|| LSU1.LDI.128.U8.U16 v4 i12
	IAU.SUB i23 i23 8		|| LSU0.LDI.128.U8.U16 v2 i1	|| LSU1.LDI.128.U8.U16 v5 i12
	IAU.SUB i24 i24 8		 
	IAU.SUB i25 i25 8		|| VAU.SUB.i16 v23 v0 v3	
	IAU.SUB i26 i26 8		|| VAU.SUB.i16 v22 v1 v4	
							             VAU.SUB.i16 v21 v2 v5

	VAU.SUB.i16 v0 v0 v3	|| LSU0.LDI.128.U8.U16 v0 i2 	|| LSU1.LDI.128.U8.U16 v3 i13 	
	VAU.SUB.i16 v1 v1 v4	|| LSU0.LDI.128.U8.U16 v1 i2	|| LSU1.LDI.128.U8.U16 v4 i13		
	VAU.SUB.i16 v2 v2 v5	|| LSU0.LDI.128.U8.U16 v2 i2	|| LSU1.LDI.128.U8.U16 v5 i13
	
	VAU.IMULS.i16 v6 v0 v0
	VAU.IMULS.i16 v7 v1 v1
	VAU.IMULS.i16 v8 v2 v2
	VAU.IMULS.i16 v23 v23 v23
	VAU.IMULS.i16 v22 v22 v22
	VAU.IMULS.i16 v21 v21 v21		
	NOP
	VAU.IADDS.i16 v23 v23 v6
	VAU.IADDS.i16 v22 v22 v7
	VAU.IADDS.i16 v21 v21 v8	
	
	VAU.SUB.i16 v0 v0 v3	|| LSU0.LDI.128.U8.U16 v0 i3 	|| LSU1.LDI.128.U8.U16 v3 i14
	VAU.SUB.i16 v1 v1 v4	|| LSU0.LDI.128.U8.U16 v1 i3	|| LSU1.LDI.128.U8.U16 v4 i14
	VAU.SUB.i16 v2 v2 v5	|| LSU0.LDI.128.U8.U16 v2 i3	|| LSU1.LDI.128.U8.U16 v5 i14
	
	VAU.IMULS.i16 v6 v0 v0
	VAU.IMULS.i16 v7 v1 v1
	VAU.IMULS.i16 v8 v2 v2
	NOP
	VAU.IADDS.i16 v23 v23 v6
	VAU.IADDS.i16 v22 v22 v7
	VAU.IADDS.i16 v21 v21 v8

	VAU.SUB.i16 v0 v0 v3	|| LSU0.LDI.128.U8.U16 v0 i4 	|| LSU1.LDI.128.U8.U16 v3 i20
	VAU.SUB.i16 v1 v1 v4	|| LSU0.LDI.128.U8.U16 v1 i4	|| LSU1.LDI.128.U8.U16 v4 i20
	VAU.SUB.i16 v2 v2 v5	|| LSU0.LDI.128.U8.U16 v2 i4	|| LSU1.LDI.128.U8.U16 v5 i20
	
	VAU.IMULS.i16 v6 v0 v0
	VAU.IMULS.i16 v7 v1 v1
	VAU.IMULS.i16 v8 v2 v2
	NOP
	VAU.IADDS.i16 v23 v23 v6
	VAU.IADDS.i16 v22 v22 v7
	VAU.IADDS.i16 v21 v21 v8
	
	VAU.SUB.i16 v0 v0 v3	|| LSU0.LDI.128.U8.U16 v0 i5 	|| LSU1.LDI.128.U8.U16 v3 i21
	VAU.SUB.i16 v1 v1 v4	|| LSU0.LDI.128.U8.U16 v1 i5	|| LSU1.LDI.128.U8.U16 v4 i21
	VAU.SUB.i16 v2 v2 v5	|| LSU0.LDI.128.U8.U16 v2 i5	|| LSU1.LDI.128.U8.U16 v5 i21

	VAU.IMULS.i16 v6 v0 v0
	VAU.IMULS.i16 v7 v1 v1
	VAU.IMULS.i16 v8 v2 v2
	NOP
	VAU.IADDS.i16 v23 v23 v6
	VAU.IADDS.i16 v22 v22 v7
	VAU.IADDS.i16 v21 v21 v8
	
	VAU.SUB.i16 v0 v0 v3	|| LSU0.LDI.128.U8.U16 v0 i6 	|| LSU1.LDI.128.U8.U16 v3 i22
	VAU.SUB.i16 v1 v1 v4	|| LSU0.LDI.128.U8.U16 v1 i6	|| LSU1.LDI.128.U8.U16 v4 i22
	VAU.SUB.i16 v2 v2 v5	|| LSU0.LDI.128.U8.U16 v2 i6	|| LSU1.LDI.128.U8.U16 v5 i22

	VAU.IMULS.i16 v6 v0 v0
	VAU.IMULS.i16 v7 v1 v1
	VAU.IMULS.i16 v8 v2 v2
	NOP
	VAU.IADDS.i16 v23 v23 v6
	VAU.IADDS.i16 v22 v22 v7
	VAU.IADDS.i16 v21 v21 v8

	VAU.SUB.i16 v0 v0 v3	|| LSU0.LDI.128.U8.U16 v0 i7 	|| LSU1.LDI.128.U8.U16 v3 i23
	VAU.SUB.i16 v1 v1 v4	|| LSU0.LDI.128.U8.U16 v1 i7	|| LSU1.LDI.128.U8.U16 v4 i23
	VAU.SUB.i16 v2 v2 v5	|| LSU0.LDI.128.U8.U16 v2 i7	|| LSU1.LDI.128.U8.U16 v5 i23
	
	VAU.IMULS.i16 v6 v0 v0
	VAU.IMULS.i16 v7 v1 v1
	VAU.IMULS.i16 v8 v2 v2
	NOP
	VAU.IADDS.i16 v23 v23 v6
	VAU.IADDS.i16 v22 v22 v7
	VAU.IADDS.i16 v21 v21 v8	
	
	VAU.SUB.i16 v0 v0 v3	|| LSU0.LDI.128.U8.U16 v0 i8 	|| LSU1.LDI.128.U8.U16 v3 i24
	VAU.SUB.i16 v1 v1 v4	|| LSU0.LDI.128.U8.U16 v1 i8	|| LSU1.LDI.128.U8.U16 v4 i24
	VAU.SUB.i16 v2 v2 v5	|| LSU0.LDI.128.U8.U16 v2 i8	|| LSU1.LDI.128.U8.U16 v5 i24

	VAU.IMULS.i16 v6 v0 v0
	VAU.IMULS.i16 v7 v1 v1
	VAU.IMULS.i16 v8 v2 v2
	NOP
	VAU.IADDS.i16 v23 v23 v6
	VAU.IADDS.i16 v22 v22 v7
	VAU.IADDS.i16 v21 v21 v8	

	VAU.SUB.i16 v0 v0 v3	|| LSU0.LDI.128.U8.U16 v0 i9 	|| LSU1.LDI.128.U8.U16 v3 i25
	VAU.SUB.i16 v1 v1 v4	|| LSU0.LDI.128.U8.U16 v1 i9	|| LSU1.LDI.128.U8.U16 v4 i25
	VAU.SUB.i16 v2 v2 v5	|| LSU0.LDI.128.U8.U16 v2 i9	|| LSU1.LDI.128.U8.U16 v5 i25

	VAU.IMULS.i16 v6 v0 v0
	VAU.IMULS.i16 v7 v1 v1
	VAU.IMULS.i16 v8 v2 v2
	NOP
	VAU.IADDS.i16 v23 v23 v6
	VAU.IADDS.i16 v22 v22 v7
	VAU.IADDS.i16 v21 v21 v8	
	
	VAU.SUB.i16 v0 v0 v3	|| LSU0.LDI.128.U8.U16 v0 i10 || LSU1.LDI.128.U8.U16 v3 i26
	VAU.SUB.i16 v1 v1 v4	|| LSU0.LDI.128.U8.U16 v1 i10 || LSU1.LDI.128.U8.U16 v4 i26
	VAU.SUB.i16 v2 v2 v5	|| LSU0.LDI.128.U8.U16 v2 i10 || LSU1.LDI.128.U8.U16 v5 i26

	VAU.IMULS.i16 v6 v0 v0
	VAU.IMULS.i16 v7 v1 v1
	VAU.IMULS.i16 v8 v2 v2
	NOP
	VAU.IADDS.i16 v23 v23 v6
	VAU.IADDS.i16 v22 v22 v7
	VAU.IADDS.i16 v21 v21 v8		

	VAU.SUB.i16 v0 v0 v3
	VAU.SUB.i16 v1 v1 v4
	VAU.SUB.i16 v2 v2 v5

	VAU.IMULS.i16 v6 v0 v0
	VAU.IMULS.i16 v7 v1 v1
	VAU.IMULS.i16 v8 v2 v2
	NOP
	VAU.IADDS.i16 v23 v23 v6
	VAU.IADDS.i16 v22 v22 v7
	VAU.IADDS.i16 v21 v21 v8
	
  LSU0.LDIL i27, sumOfSquaredDiff11x11_loop 				                  || LSU1.LDIH i27, sumOfSquaredDiff11x11_loop
	CMU.ALIGNVEC v0 v23 v22 6     || LSU0.LDI.128.U8.U16 v0  i0 		|| LSU1.LDI.128.U8.U16 v11 i11		
	CMU.ALIGNVEC v1 v23 v22 8     || LSU0.LDI.128.U8.U16 v1  i1		|| LSU1.LDI.128.U8.U16 v12 i12  || vau.iadds.u16 v28 v22 v0
	CMU.ALIGNVEC v2 v23 v22 10    || LSU0.LDI.128.U8.U16 v2  i2		|| LSU1.LDI.128.U8.U16 v13 i13  
	CMU.ALIGNVEC v3 v23 v22 12    || LSU0.LDI.128.U8.U16 v3  i3 		|| LSU1.LDI.128.U8.U16 v15 i14  || vau.iadds.u16 v29 v2 v1
	CMU.ALIGNVEC v4 v23 v22 14    || LSU0.LDI.128.U8.U16 v4  i4		|| LSU1.LDI.128.U8.U16 v16 i20  
	CMU.ALIGNVEC v5 v22 v21 2     || LSU0.LDI.128.U8.U16 v5  i5		|| LSU1.LDI.128.U8.U16 v17 i21  || vau.iadds.u16 v28 v28 v3
	CMU.ALIGNVEC v6 v22 v21 4     || LSU0.LDI.128.U8.U16 v6  i6 		|| LSU1.LDI.128.U8.U16 v18 i22  || vau.iadds.u16 v29 v29 v5
	CMU.ALIGNVEC v7 v22 v21 6     || LSU0.LDI.128.U8.U16 v7  i7		|| LSU1.LDI.128.U8.U16 v19 i23  || vau.iadds.u16 v28 v28 v4
	CMU.ALIGNVEC v8 v22 v21 8     || LSU0.LDI.128.U8.U16 v8  i8		|| LSU1.LDI.128.U8.U16 v24 i24  || vau.iadds.u16 v29 v29 v6
	CMU.ALIGNVEC v9 v22 v21 10    || LSU0.LDI.128.U8.U16 v9  i9 		|| LSU1.LDI.128.U8.U16 v25 i25  || vau.iadds.u16 v28 v28 v7
	LSU0.LDI.128.U8.U16 v10 i10   || LSU1.LDI.128.U8.U16 v26 i26  || vau.iadds.u16 v29 v29 v8
  vau.iadds.u16 v28 v28 v9
  nop
  vau.iadds.u16 v23 v28 v29

	VAU.SUB.i16 v0 v0 v11	 
	CMU.CLAMPAB.u16 v23 v14 v20	|| VAU.SUB.i16 v1 v1 v12
	VAU.SUB.i16 v2 v2 v13
	LSU0.STI.128.U16.U8 v23 i16	|| VAU.SUB.i16 v3 v3 v15
	
	CMU.CMZ.i32 i15
	PEU.PC1C eq || BRU.BRA sumOfSquaredDiff11x11_end
	NOP 6

	VAU.SUB.i16 v4  v4  v16 	|| BRU.RPL i27 i15
	VAU.SUB.i16 v5  v5  v17
	VAU.SUB.i16 v6  v6  v18
	VAU.SUB.i16 v7  v7  v19
	VAU.SUB.i16 v8  v8  v24
	VAU.SUB.i16 v9  v9  v25
	VAU.SUB.i16 v10 v10 v26
	
	
	VAU.IMULS.i16 v0  v0  v0
	VAU.IMULS.i16 v1  v1  v1
	VAU.IMULS.i16 v2  v2  v2
	VAU.IMULS.i16 v3  v3  v3
	VAU.IMULS.i16 v4  v4  v4
	VAU.IMULS.i16 v5  v5  v5
	VAU.IMULS.i16 v6  v6  v6
	VAU.IMULS.i16 v7  v7  v7
	VAU.IMULS.i16 v8  v8  v8
	VAU.IMULS.i16 v9  v9  v9
	VAU.IMULS.i16 v10 v10 v10
	NOP

	vau.iadds.u16 v28 v0 v1
	vau.iadds.u16 v29 v2 v3
	vau.iadds.u16 v28 v28 v4
	vau.iadds.u16 v29 v29 v5
	vau.iadds.u16 v28 v28 v6
	vau.iadds.u16 v29 v29 v7
	vau.iadds.u16 v28 v28 v8
	vau.iadds.u16 v29 v29 v9
  vau.iadds.u16 v28 v28 v10
  nop
  vau.iadds.u16 v23 v28 v29

	CMU.ALIGNVEC v0 v22 v21 6
  vau.iadds.u16 v28 v0 v21
	CMU.ALIGNVEC v1 v22 v21 8      		  || LSU0.LDI.128.U8.U16 v0  i0 	|| LSU1.LDI.128.U8.U16 v11 i11	
	CMU.ALIGNVEC v2 v22 v21 10      		|| LSU0.LDI.128.U8.U16 v1  i1	|| LSU1.LDI.128.U8.U16 v12 i12 || vau.iadds.u16 v28 v28 v1
	CMU.ALIGNVEC v3 v22 v21 12		 	    || LSU0.LDI.128.U8.U16 v2  i2	|| LSU1.LDI.128.U8.U16 v13 i13 
	CMU.ALIGNVEC v4 v22 v21 14      		|| LSU0.LDI.128.U8.U16 v3  i3 	|| LSU1.LDI.128.U8.U16 v15 i14 || vau.iadds.u16 v28 v28 v2
	CMU.ALIGNVEC v5 v21 v23 2      		  || LSU0.LDI.128.U8.U16 v4  i4	|| LSU1.LDI.128.U8.U16 v16 i20
	CMU.ALIGNVEC v6 v21 v23 4		 	      || LSU0.LDI.128.U8.U16 v5  i5	|| LSU1.LDI.128.U8.U16 v17 i21 || vau.iadds.u16 v28 v28 v3
	CMU.ALIGNVEC v7 v21 v23 6      		  || LSU0.LDI.128.U8.U16 v6  i6 	|| LSU1.LDI.128.U8.U16 v18 i22 || vau.iadds.u16 v29 v4 v5
	CMU.ALIGNVEC v8 v21 v23 8      		  || LSU0.LDI.128.U8.U16 v7  i7	|| LSU1.LDI.128.U8.U16 v19 i23 || vau.iadds.u16 v28 v28 v6
	CMU.ALIGNVEC v9 v21 v23 10		 	    || LSU0.LDI.128.U8.U16 v8  i8	|| LSU1.LDI.128.U8.U16 v24 i24	|| vau.iadds.u16 v29 v29 v7
	vau.iadds.u16 v28 v28 v8
  vau.iadds.u16 v29 v29 v9
  nop
  
  sumOfSquaredDiff11x11_loop:	
	//VAU.ACCPW.u16 v27 v9 									|| LSU0.LDI.128.U8.U16 v9  i9 	|| LSU1.LDI.128.U8.U16 v25 i25	
	LSU0.LDI.128.U8.U16 v9  i9 	|| LSU1.LDI.128.U8.U16 v25 i25	|| vau.iadds.u16 v27 v28 v29
	VAU.SUB.i16 v0 v0 v11				  || LSU0.LDI.128.U8.U16 v10 i10	|| LSU1.LDI.128.U8.U16 v26 i26 
	CMU.CLAMPAB.u16 v27 v14 v20	|| VAU.SUB.i16 v1 v1 v12
	VAU.SUB.i16 v2 v2 v13
	LSU0.STI.128.U16.U8 v27 i16	|| VAU.SUB.i16 v3 v3 v15
	VAU.SUB.i16 v22 v21 v14
	VAU.SUB.i16 v21 v23 v14	
	
	
 sumOfSquaredDiff11x11_end:
  LSU0.LD.64.H  v29 i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v29 i19 || IAU.ADD i19 i19 8
 	LSU0.LD.64.H  v28 i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v28 i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v27 i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v27 i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v26 i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v26 i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v25 i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v25 i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.H  v24 i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.L  v24 i19 || IAU.ADD i19 i19 8 	
	LSU0.LD.32 	 i27  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32 	 i26  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32 	 i25  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32 	 i24  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32 	 i23  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32 	 i22  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32 	 i21  i19 || IAU.ADD i19 i19 4
	LSU0.LD.32 	 i20  i19 || IAU.ADD i19 i19 4
	
	BRU.jmp i30
	NOP 6
	.nowarnend
.size mvcvSumOfSquaredDiff11x11_asm,.-mvcvSumOfSquaredDiff11x11_asm
.end
