///
/// @file
/// @copyright All code copyright Movidius Ltd 2013  all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///
//-------------------------------------------------------------------------------
.type mvcvMonoImbalance_asm,@function
.version 00.51.05
// C header
//
//			    i18  		i17		 i16           i15            i14               i13		
// void (UInt16** in, half** out,int thrDark, int thrBright, int inputBits, UInt32 inWidth)//
//----------------------------mvcvMonoImbalance_asm--------------------------------------------------- 
.data .data.MonoImbalance
.align 4


	
.code .text.MonoImbalance


mvcvMonoImbalance_asm:
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


//Load the address for all the 3 lines: i0, i1, i2
//i3 - rshift
//
    
	LSU0.LDI.64 i0 i18       || IAU.ADD i3,i14,1   ||  LSU1.LDIL i4,1            
	LSU0.LD.32 i2 i18        || IAU.SHL i14,i4,i14 ||  LSU1.LDIL i5,2        || CMU.CPII.i32.f32 i6,i4
    LSU0.LD.32 i17 i17       || IAU.INCS i14,-1    ||  CMU.CPII.i32.f32 i5,i5
	CMU.CPII.i32.f32 i14,i14 || IAU.SHR.u32 i13 i13 3 
	SAU.DIV.f32 i5,i6,i5     || LSU0.LDIL i11 MonoImbalance_u16_forLoop    	 || LSU1.LDIH i11 MonoImbalance_u16_forLoop
    SAU.DIV.f32 i4,i6,i14    
	nop
	IAU.INCS i1,-16 //normVal
    LSU0.LDi.64.l v0 i0  
	LSU0.LDi.64.h v0 i0  
	LSU0.LDi.64.l  v3 i1
	CMU.CPIVR.x16 v20,i15    || LSU0.LDi.64.h  v3 i1
	CMU.CPIVR.x16 v21,i16    || LSU0.LDi.64.l  v1 i1
	VAU.SUB.i16 v20,v20,v21  || LSU0.LDi.64.h  v1 i1
	nop 4
	LSU0.LDi.64.l  v4 i1
	LSU0.LDi.64.h  v4 i1	
	CMU.CPII.f32.f16 i5 i5   || LSU0.LDi.64.l  v2 i2
	CMU.CPII.f32.f16 i4 i4   || LSU0.LDi.64.h  v2 i2
	nop
	CMU.CPIVR.x16 v22,i3
	CMU.CPIVR.x16 v23,i5   
	CMU.CPIVR.x16 v24,i4    
	CMU.ALIGNVEC v5 v3 v1  14  
	CMU.ALIGNVEC v6 v1 v4  2  
	VAU.AVG.x16 v7,v0,v2     || CMU.CPVV v17,v1 
	VAU.AVG.x16 v8,v5,v6     || CMU.CPVV v25,v17 
	nop
	VAU.AVG.x16 v9,v7,v8
	nop	
	VAU.ADD.i16 v10,v9,v25
	////dif 
	VAU.SUB.i16 v11,v9,v25 
	////(thrBright - thrDark) * luma)
	VAU.MUL.i16 v12,v20,v10  || LSU0.LDI.64.l v0 i0          || CMU.CPVV v3,v1            ////v20- v_thr = thrBright - thrDark
	LSU0.LDI.64.h v0 i0      || LSU1.LDI.64.l  v4 i1         || CMU.CPVV v1,v4
	LSU0.LDI.64.l  v2 i2     || LSU1.LDI.64.h  v4 i1         || CMU.ALIGNVEC v5 v3 v1  14  
	VAU.SHD.i16 v12,v12,v22  || LSU0.LDI.64.h  v2 i2         || CMU.CPVV v15 v11   
	nop
	VAU.ADD.i16 v12,v12,v21 
	nop
	VAU.ADD.i16 v13,v12,v12  || CMU.VSZM.WORD v16 v16 [ZZZZ]
	VAU.SUB.i16 v16,v16,v12
	CMU.ALIGNVEC v6 v1 v4  2
    VAU.AVG.x16 v7,v0,v2     || CMU.CPVV v17,v1 
    nop
	VAU.XOR v19,v19,v19      || CMU.VSZM.WORD v18,v18,[zzzz]
	
//Disable PEU.PVV warning since we are using it on purpose
.nowarn 22
		 
	CMU.CMVV.i16 v11 v12     || VAU.ADD.i16 v14,v16,v16      || BRU.RPL i11 i13  
	PEU.PVV16 GT             || VAU.SUB.i16 v19,v13,v11      || CMU.CPVV.i16.f16 v25 v25
	nop
	PEU.PVV16 LTE            || VAU.AND v18 v15 v15
	VAU.OR v15 v19 v18
	VAU.AVG.x16 v8,v5,v6     || CMU.VSZM.WORD v18,v18,[zzzz]
	VAU.ADD.f16 v26,v25,0.5  || CMU.VSZM.WORD v19,v19,[zzzz]
    VAU.MUL.i16 v12,v20,v10  || LSU0.LDI.64.l v0 i0          || CMU.CPVV v3,v1 
	CMU.CMZ.i16 v15          || LSU0.LDI.64.h v0 i0     
	PEU.PVV16 LT             || VAU.SUB.i16 v19 v11 v11      || LSU0.LDI.64.l  v2 i2  
	nop
	PEU.PVV16 GTE            || VAU.AND v18 v15 v15
	VAU.OR v15 v19 v18       || CMU.CPVV v25,v17
	VAU.AVG.x16 v9,v7,v8     || LSU0.LDI.64.h  v2 i2         || CMU.VSZM.WORD v19,v19,[zzzz] 
	CMU.CMVV.i16 v11 v16     || VAU.SHD.i16 v12,v12,v22       
	PEU.PVV16 LT             || VAU.SUB.i16 v19,v14,v11  
	CMU.VSZM.WORD v18,v18,[zzzz]
	PEU.PVV16 GTE            || VAU.AND v18 v15 v15
	VAU.OR v15 v19 v18       || CMU.CPVV v1,v4   
	VAU.ADD.i16 v10,v9,v25   || CMU.VSZM.WORD v19,v19,[zzzz] || LSU1.LDI.64.l  v4 i1            
	CMU.CMZ.i16 v15          || VAU.SUB.i16 v11,v9,v25       || LSU1.LDI.64.h  v4 i1
	PEU.PVV16 GT             || VAU.SUB.i16 v19 v11 v11
	CMU.VSZM.WORD v18,v18,[zzzz]
    PEU.PVV16 LTE            || VAU.AND v18 v15 v15  
	VAU.OR v15 v19 v18
	VAU.ADD.i16 v12,v12,v21  || CMU.ALIGNVEC v5 v3 v1  14  
	CMU.CPVV.i16.f16 v15 v15
	nop
.nowarnend

	VAU.MUL.f16 v15,v15,v23  || CMU.VSZM.WORD v18,v18,[zzzz]  // v23 = 1/2 precalc
	CMU.ALIGNVEC v6 v1 v4  2
	VAU.ADD.i16 v13,v12,v12  || CMU.VSZM.WORD v16 v16 [ZZZZ]
	VAU.ADD.f16 v26,v26,v15  || CMU.CPVV v15 v11  
	MonoImbalance_u16_forLoop:	
	nop
	VAU.SUB.i16 v16,v16,v12  || CMU.CPVV v17,v1 
	VAU.MUL.f16 v26,v26,v24  || CMU.VSZM.WORD v19,v19,[zzzz]// v24<- i4 normVal
	nop
	VAU.AVG.x16 v7,v0,v2    
	LSU0.STI.64.l v26,i17
	LSU0.STI.64.h v26,i17

	LSU0.LD.64.h  v31  i19   || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v31  i19   || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v30  i19   || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v30  i19   || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v29  i19   || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v29  i19   || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v28  i19   || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v28  i19   || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v27  i19   || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v27  i19   || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v26  i19   || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v26  i19   || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v25  i19   || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v25  i19   || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v24  i19   || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v24  i19   || IAU.ADD i19 i19 8	

	LSU0.LD.32  i31  i19     || IAU.ADD i19 i19 4
	LSU0.LD.32  i29  i19     || IAU.ADD i19 i19 4
	LSU0.LD.32  i28  i19     || IAU.ADD i19 i19 4	
	LSU0.LD.32  i27  i19     || IAU.ADD i19 i19 4
	LSU0.LD.32  i26  i19     || IAU.ADD i19 i19 4
	LSU0.LD.32  i25  i19     || IAU.ADD i19 i19 4
	LSU0.LD.32  i24  i19     || IAU.ADD i19 i19 4
	LSU0.LD.32  i23  i19     || IAU.ADD i19 i19 4
	LSU0.LD.32  i22  i19     || IAU.ADD i19 i19 4
	LSU0.LD.32  i21  i19     || IAU.ADD i19 i19 4
	LSU0.LD.32  i20  i19     || IAU.ADD i19 i19 4
	nop 6
	
	
    BRU.JMP i30
    nop 6

.size mvcvMonoImbalance_asm,.-mvcvMonoImbalance_asm
.end



