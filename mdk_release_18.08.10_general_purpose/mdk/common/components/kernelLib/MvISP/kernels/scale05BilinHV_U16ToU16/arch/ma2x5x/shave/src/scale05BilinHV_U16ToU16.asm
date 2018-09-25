///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Downscale to half resolution in each direction with bilinear (sw) kernel// 
///            U16 in / U16 out
///

//--------------------------------------------------------------------------------------------------------------
.type mvispScale05BilinHV_U16ToU16_asm,@function
.version 00.50.0
		
.code .text.scale05BilinHV_U16ToU16_asm 
mvispScale05BilinHV_U16ToU16_asm:
//void scale05BilinHV_Fp16U8(half** in, u8* out, u32 inWidth)//
//                          		i18        i17        i16       
	
	
	lsu0.ldi.32 i0 i18					|| lsu1.ldil i15 scale05BilinHV_U16ToU16_loopSc05BiHVu16u16
	nop
	lsu0.ldih i15 scale05BilinHV_U16ToU16_loopSc05BiHVu16u16	|| lsu1.ld.32 i1 i18
	iau.shr.u32 i13 i16 5				
	iau.xor i2 i2 i2
	iau.incs i2 0x10
	iau.shl i3 i13 5
	iau.sub i4 i16 i3
	
	lsu0.ldxvi v0 i0 i2		
	lsu1.ldxvi v1 i1 i2
	lsu0.ldxvi v3 i0 i2
	lsu1.ldxvi v4 i1 i2

	lsu0.ldxvi v6 i0 i2		
	lsu1.ldxvi v7 i1 i2
	lsu0.ldxvi v8 i0 i2
	lsu1.ldxvi v9 i1 i2
	
	iau.incs i13 0x00
	peu.pc1i lte || bru.bra scale05BilinHV_U16ToU16_compensateSc05BilinHVu16u16	
	
	lsu0.ldxvi v12 i0 i2		
	lsu1.ldxvi v13 i1 i2
	lsu0.ldxvi v14 i0 i2
	lsu1.ldxvi v15 i1 i2	
	
	lsu0.ldxvi v18 i0 i2		
	lsu1.ldxvi v19 i1 i2
	lsu0.ldxvi v20 i0 i2
	lsu1.ldxvi v21 i1 i2

	
	vau.avg.x16 v2 v0 v1						|| lsu0.ldxvi v0 i0 i2				|| bru.rpl i15 i13 scale05BilinHV_U16ToU16_endLoopSc05BiHVu16u16
	vau.avg.x16 v5 v3 v4						|| lsu1.ldxvi v1 i1 i2
	vau.avg.x16 v10 v6 v7 						|| lsu0.ldxvi v3 i0 i2
	cmu.vdilv.x16 v2 v5 v2 v5					|| vau.avg.x16 v11 v8 v9 			|| lsu1.ldxvi v4 i1 i2	
	vau.avg.x16 v5 v2 v5						|| lsu0.ldxvi v6 i0 i2		
	cmu.vdilv.x16 v10 v11 v10 v11				|| lsu1.ldxvi v7 i1 i2				|| vau.avg.x16 v16 v12 v13
	vau.avg.x16 v17 v14 v15						|| lsu0.ldxvi v8 i0 i2
	lsu1.ldxvi v9 i1 i2							|| vau.avg.x16 v10 v10 v11
	cmu.vdilv.x16 v16 v17 v16 v17				|| lsu0.ldxvi v12 i0 i2		
	vau.avg.x16 v16 v16 v17						|| lsu1.ldxvi v13 i1 i2
	lsu0.ldxvi v14 i0 i2						|| vau.avg.x16 v22 v18 v19
	lsu1.ldxvi v15 i1 i2						|| vau.avg.x16 v23 v20 v21	
	lsu0.ldxvi v18 i0 i2
 scale05BilinHV_U16ToU16_loopSc05BiHVu16u16:
	lsu1.ldxvi v19 i1 i2						|| cmu.vdilv.x16 v22 v23 v22 v23
	lsu0.ldxvi v20 i0 i2						|| vau.avg.x16 v22 v22 v23
	lsu1.ldxvi v21 i1 i2	
	
	lsu0.stxv v5 i17 	|| iau.incs i17 0x10
	lsu1.stxv v10 i17 	|| iau.incs i17 0x10	
	lsu0.stxv v16 i17 	|| iau.incs i17 0x10	
	lsu1.stxvi v22 i17 i2
 scale05BilinHV_U16ToU16_endLoopSc05BiHVu16u16:

 scale05BilinHV_U16ToU16_compensateSc05BilinHVu16u16:	
	vau.avg.x16 v2 v0 v1								
	vau.avg.x16 v5 v3 v4						
	vau.avg.x16 v10 v6 v7 			
	cmu.vdilv.x16 v2 v5 v2 v5						|| vau.avg.x16 v11 v8 v9 			
	vau.avg.x16 v5 v2 v5								
	cmu.vdilv.x16 v10 v11 v10 v11					|| vau.avg.x16 v16 v12 v13
	vau.avg.x16 v17 v14 v15							|| iau.incs i4 -8
	peu.pc1i gte	|| lsu0.st.64.l v5 i17 			|| lsu1.sto.64.h v5 i17 0x08
	vau.avg.x16 v10 v10 v11
	cmu.vdilv.x16 v16 v17 v16 v17					|| iau.incs i4 -8
	peu.pc1i gte	|| lsu0.sto.64.l v10 i17 0x10 	|| lsu1.sto.64.h v10 i17 0x18
	vau.avg.x16 v16 v16 v17					
	iau.incs i4 -8	
	peu.pc1i gte	|| lsu0.sto.64.l v16 i17 0x20	|| lsu1.sto.64.h v16 i17 0x28
			
	
	bru.jmp i30	
	nop 6
	
.size mvispScale05BilinHV_U16ToU16_asm,.-mvispScale05BilinHV_U16ToU16_asm
.end
	
