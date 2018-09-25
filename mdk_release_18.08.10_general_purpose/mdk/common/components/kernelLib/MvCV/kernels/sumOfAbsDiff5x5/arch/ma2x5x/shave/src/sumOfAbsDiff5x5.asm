///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief   
///

.type mvcvSumOfAbsDiff5x5_asm,@function
.version 00.51.05
//-------------------------------------------------------------------------------
.data .rodata.sumOfAbsDiff5x5
.align 16


 sumOfAbsDiff5x5_clampVal:
    .short 255
	
.code .text.sumOfAbsDiff5x5
//void sumOfAbsDiff5x5(u8** in1(i18), u8** in2(i17), u8** out(i16), u32 width(i15))
mvcvSumOfAbsDiff5x5_asm:
.nowarn
	LSU0.LDI.32 i0 i18  || LSU1.LDIL i11, sumOfAbsDiff5x5_loop 
	LSU0.LDI.32 i1 i17 	|| LSU1.LDIH i11, sumOfAbsDiff5x5_loop 
	LSU0.LDI.32 i2 i18 	|| LSU1.LDIL i10, sumOfAbsDiff5x5_clampVal
	LSU0.LDI.32 i3 i17 	|| LSU1.LDIH i10, sumOfAbsDiff5x5_clampVal 	
	LSU0.LDI.32 i4 i18 	|| LSU1.LD.32 i16 i16		|| IAU.SHR.u32 i12 i15 3
	LSU0.LDI.32 i5 i17 	|| LSU1.LD.16R v22, i10		|| IAU.SUB i12 i12 1
	LSU0.LDI.32 i6 i18 				
	LSU0.LDI.32 i7 i17 	|| IAU.SUB i0 i0 8			
	LSU0.LDI.32 i8 i18 	|| IAU.SUB i1 i1 8
	LSU1.LDI.32 i9 i17 	|| IAU.SUB i2 i2 8													   
	LSU0.LDI.128.U8.U16 v0 i0	|| LSU1.LDI.128.U8.U16 v1 i1	|| IAU.SUB i3 i3 8					
	LSU0.LDI.128.U8.U16 v2 i0	|| LSU1.LDI.128.U8.U16 v3 i1	|| IAU.SUB i4 i4 8				
	LSU0.LDI.128.U8.U16 v4 i0	|| LSU1.LDI.128.U8.U16 v5 i1	|| IAU.SUB i5 i5 8			
	LSU0.LDI.128.U8.U16 v0 i2 	|| LSU1.LDI.128.U8.U16 v1 i3    || IAU.SUB i6 i6 8  
	LSU0.LDI.128.U8.U16 v2 i2 	|| LSU1.LDI.128.U8.U16 v3 i3	|| IAU.SUB i7 i7 8
	LSU0.LDI.128.U8.U16 v4 i2	|| LSU1.LDI.128.U8.U16 v5 i3    || IAU.SUB i8 i8 8
	VAU.XOR v21 v21 v21 		                                || IAU.SUB i9 i9 8
	VAU.ADIFF.u16 v6 v0 v1  
	VAU.ADIFF.u16 v7 v2 v3
	VAU.ADIFF.u16 v8 v4 v5		|| LSU0.LDI.128.U8.U16 v0 i4 	|| LSU1.LDI.128.U8.U16 v1 i5 
	
	VAU.ADIFF.u16 v9 v0 v1		|| LSU0.LDI.128.U8.U16 v2 i4 	|| LSU1.LDI.128.U8.U16 v3 i5
	VAU.ADIFF.u16 v10 v2 v3	    || LSU0.LDI.128.U8.U16 v4 i4    || LSU1.LDI.128.U8.U16 v5 i5
	VAU.ADIFF.u16 v11 v4 v5 	                        
	VAU.ADD.i16 v6 v6 v9	
	VAU.ADD.i16 v7 v7 v10   
	VAU.ADD.i16 v8 v8 v11		|| LSU0.LDI.128.U8.U16 v0 i6	|| LSU1.LDI.128.U8.U16 v1 i7
	
	VAU.ADIFF.u16 v9 v0 v1 		|| LSU0.LDI.128.U8.U16 v2 i6	|| LSU1.LDI.128.U8.U16 v3 i7
	VAU.ADIFF.u16 v10 v2 v3	    || LSU0.LDI.128.U8.U16 v4 i6	|| LSU1.LDI.128.U8.U16 v5 i7
	VAU.ADIFF.u16 v11 v4 v5		
	VAU.ADD.i16 v6 v6 v9	
	VAU.ADD.i16 v7 v7 v10	
	VAU.ADD.i16 v8 v8 v11		|| LSU0.LDI.128.U8.U16 v0 i8	|| LSU1.LDI.128.U8.U16 v1 i9
	 
	VAU.ADIFF.u16 v9 v0 v1		|| LSU0.LDI.128.U8.U16 v2 i8	|| LSU1.LDI.128.U8.U16 v3 i9
	VAU.ADIFF.u16 v10 v2 v3		|| LSU0.LDI.128.U8.U16 v4 i8 	|| LSU1.LDI.128.U8.U16 v5 i9	
	VAU.ADIFF.u16 v11 v4 v5	
	VAU.ADD.i16 v16 v6 v9	
	VAU.ADD.i16 v17 v7 v10   
	VAU.ADD.i16 v18 v8 v11		|| LSU0.LDI.128.U8.U16 v0 i0	|| LSU1.LDI.128.U8.U16 v1 i1
	 
	 
	VAU.ADIFF.u16 v9 v0 v1		|| LSU0.LDI.128.U8.U16 v2 i2	|| LSU1.LDI.128.U8.U16 v3 i3	
	VAU.ADIFF.u16 v10 v2 v3	    || LSU0.LDI.128.U8.U16 v4 i4	|| LSU1.LDI.128.U8.U16 v5 i5
	VAU.ADIFF.u16 v11 v4 v5		|| LSU0.LDI.128.U8.U16 v6 i6	|| LSU1.LDI.128.U8.U16 v7 i7	                            
	VAU.ADD.i16 v16 v16 v9	    || LSU0.LDI.128.U8.U16 v8 i8	|| LSU1.LDI.128.U8.U16 v9 i9
	VAU.ADD.i16 v17 v17 v10	
	VAU.ADD.i16 v18 v18 v11
	 
	VAU.ACCPZ.u16 v17	|| CMU.ALIGNVEC v13 v16 v17 12	
	VAU.ACCP.u16 v13	|| CMU.ALIGNVEC v14 v16 v17 14 	
	VAU.ACCP.u16 v14	|| CMU.ALIGNVEC v15 v17 v18 2     
	VAU.ACCP.u16 v15	|| CMU.ALIGNVEC v16 v17 v18 4	
	VAU.ACCPW.u16 v12 v16	
	
	VAU.ADIFF.u16 v0 v0 v1
 	CMU.CLAMPAB.u16 v12 v21 v22	|| VAU.ADIFF.u16 v2 v2 v3
	VAU.ADIFF.u16 v4 v4 v5	
 	LSU0.STI.128.U16.U8 v12 i16 || VAU.ADIFF.u16 v6 v6 v7  
	VAU.ADIFF.u16 v8 v8 v9
	
	CMU.CMZ.i32 i12
	PEU.PC1C eq 	|| BRU.BRA sumOfAbsDiff5x5_end
	NOP 6
	
	VAU.ADD.i16 v7 v0 v2		|| BRU.RPL i11 i12
	VAU.ADD.i16 v6 v4 v6        || LSU0.LDI.128.U8.U16 v0 i0	|| LSU1.LDI.128.U8.U16 v1 i1
	VAU.ADD.i16 v16 v7 v8       || LSU0.LDI.128.U8.U16 v2 i2	|| LSU1.LDI.128.U8.U16 v3 i3
	VAU.ACCPZ.u16 v18			|| LSU0.LDI.128.U8.U16 v4 i4	|| LSU1.LDI.128.U8.U16 v5 i5
	VAU.ADD.i16 v16 v16 v6		|| CMU.ALIGNVEC v10 v17 v18 12 	|| LSU0.LDI.128.U8.U16 v6 i6 	|| LSU1.LDI.128.U8.U16 v7 i7
	VAU.ACCP.u16 v10			|| CMU.ALIGNVEC v11 v17 v18 14 	|| LSU0.LDI.128.U8.U16 v8 i8	|| LSU1.LDI.128.U8.U16 v9 i9
	VAU.ACCP.u16 v11			|| CMU.ALIGNVEC v12 v18 v16 2
	VAU.ACCP.u16 v12			|| CMU.ALIGNVEC v13 v18 v16 4
	VAU.ACCPW.u16 v14 v13
 sumOfAbsDiff5x5_loop:
	VAU.ADIFF.u16 v0 v0 v1
	CMU.CLAMPAB.u16 v14 v21 v22	|| VAU.ADIFF.u16 v2 v2 v3
	VAU.ADIFF.u16 v4 v4 v5
	LSU0.STI.128.U16.U8 v14 i16	|| VAU.ADIFF.u16 v6 v6 v7
	VAU.ADIFF.u16 v8 v8 v9
	VAU.SUB.i16 v17 v18 v21
	VAU.SUB.i16 v18 v16 v21

 sumOfAbsDiff5x5_end:
	BRU.jmp i30
	nop 6
.size mvcvSumOfAbsDiff5x5_asm,.-mvcvSumOfAbsDiff5x5_asm
.nowarnend
.end
