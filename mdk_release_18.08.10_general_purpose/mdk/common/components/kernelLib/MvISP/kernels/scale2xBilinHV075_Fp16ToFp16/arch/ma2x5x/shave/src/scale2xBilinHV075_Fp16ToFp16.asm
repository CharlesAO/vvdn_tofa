///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Up scale by two in each direction with bilinear (sw) kernel.
///            Fp16 in and out
///

//--------------------------------------------------------------------------------------------------------------
.type mvispScale2xBilinHV075_Fp16ToFp16_asm,@function
.version 00.50.0
.data .rodata.mvispScale2xBilinHV075_Fp16ToFp16_asm 
.align 16
scale2xBilinHV075_vect:                            //wt[1][3] = {
        .float16 0.0625, 0.1875, 0.5625    //{1.0/16, 3.0/16, 9.0/16},

//void mvispScale2xBilinHV075_Fp16ToFp16_asm(half** in, half** out, u32 inWidth)//
//                          		             i18        i17        i16       
	
.code .text.scale2xBilinHV075_Fp16ToFp16_asm 
mvispScale2xBilinHV075_Fp16ToFp16_asm:
	.nowarn
  
  lsu0.ldil i0 scale2xBilinHV075_vect || lsu1.ldih i0 scale2xBilinHV075_vect
	lsu1.ldi.32 i1 i18    || lsu0.ld.64.l v0 i0
	lsu1.ld.32 i2 i18     
  lsu0.ldil i15 0x10    || iau.shr.u32 i4 i16 4
  lsu0.ldil i14 0x20
  lsu0.ld.32 i17 i17
  lsu0.ldil i3 ___loopScl2xBilinHV_Fp16_075 || lsu1.ldih i3 ___loopScl2xBilinHV_Fp16_075
	nop 
	
	iau.sub i1 i1 0x10    
	iau.sub i2 i2 0x10    || lsu1.ldxvi v1 i1 i15    
  lsu0.ldxvi v3 i2 i15   
  lsu1.ldxvi v2 i1 i15
  lsu0.ldxvi v4 i2 i15
  iau.add i5 i17 0x10 
  nop 3  
  lsu1.ldxvi v2 i1 i15
  lsu0.ldxvi v4 i2 i15
  nop 
  
  vau.mul.f16 v7 v0 v4 || lsu0.swzv8 [11111111]  || cmu.alignvec v6 v1 v2 14 //v6 vector ce incepe de la poz [0][-1] 
  //v7 = 3/16 *   
  //in[1][0] //in[1][1] //in[1][2] //in[1][3] //in[1][4] //in[1][5] //in[1][6] //in[1][7]
  vau.mul.f16 v9 v0 v2 || lsu0.swzv8 [22222222]  || cmu.alignvec v8 v3 v4 14 //v8 vector ce incepe de la poz [1][-1] 
  //v9 = 9/16 *   
  //in[0][0] //in[0][1] //in[0][2] //in[0][3] //in[0][4] //in[0][5] //in[0][6] //in[0][7]
  
  vau.mul.f16 v5 v0 v6 || lsu0.swzv8 [11111111]  || cmu.cpvv v1 v2
  //v5 = 3/16 *   
  //in[0][-1]// in[0][0] //in[0][1] //in[0][2] //in[0][3] //in[0][4] //in[0][5] //in[0][6]
  
  vau.mul.f16 v10 v0 v8 || lsu0.swzv8 [00000000] || cmu.cpvv v3 v4
  //v10 = 1/16 *   
  //in[1][-1]// in[1][0] //in[1][1] //in[1][2] //in[1][3] //in[1][4] //in[1][5] //in[1][6]
  
  cmu.alignvec v6 v1 v2 2
 
  vau.mul.f16 v11 v0 v6 || lsu0.swzv8 [11111111] || cmu.alignvec v8 v3 v4 2
  //v11 = 3/16 *   
  //in[0][1]// in[0][2] //in[0][3] //in[0][4] //in[0][5] //in[0][6] //in[0][7] //in[0][8]
  vau.mul.f16 v12 v0 v8 || lsu0.swzv8 [00000000]
	//v12 = 1/16 *   
  //in[1][1]// in[1][2] //in[1][3] //in[1][4] //in[1][5] //in[1][6] //in[1][7] //in[1][8]	
  
  vau.add.f16 v13 v7 v9
  vau.add.f16 v14 v5 v10
  lsu1.ldxvi v2 i1 i15
  vau.add.f16 v15 v11 v12  || lsu0.ldxvi v4 i2 i15
  vau.add.f16 v16 v13 v14  

 
  vau.mul.f16 v7 v0 v4 || lsu0.swzv8 [11111111]  || cmu.alignvec v6 v1 v2 14  || bru.rpl i3 i4 ___endScl2xBilinHV_Fp16_075//v6 vector ce incepe de la poz [0][-1] 
  //v7 = 3/16 *   
  //in[1][0] //in[1][1] //in[1][2] //in[1][3] //in[1][4] //in[1][5] //in[1][6] //in[1][7]
  vau.mul.f16 v9 v0 v2 || lsu0.swzv8 [22222222]  || cmu.alignvec v8 v3 v4 14 //v8 vector ce incepe de la poz [1][-1] 
  //v9 = 9/16 *   
  //in[0][0] //in[0][1] //in[0][2] //in[0][3] //in[0][4] //in[0][5] //in[0][6] //in[0][7]
  
  vau.mul.f16 v5 v0 v6 || lsu0.swzv8 [11111111]  || cmu.cpvv v1 v2
  //v5 = 3/16 *   
  //in[0][-1]// in[0][0] //in[0][1] //in[0][2] //in[0][3] //in[0][4] //in[0][5] //in[0][6]
  
  vau.mul.f16 v10 v0 v8 || lsu0.swzv8 [00000000] || cmu.cpvv v3 v4
  //v10 = 1/16 *   
  //in[1][-1]// in[1][0] //in[1][1] //in[1][2] //in[1][3] //in[1][4] //in[1][5] //in[1][6]
   
  cmu.alignvec v6 v1 v2 2  || vau.add.f16 v17 v13 v15
  ___loopScl2xBilinHV_Fp16_075:
  vau.mul.f16 v11 v0 v6 || lsu0.swzv8 [11111111] || cmu.alignvec v8 v3 v4 2
  //v11 = 3/16 *   
  //in[0][1]// in[0][2] //in[0][3] //in[0][4] //in[0][5] //in[0][6] //in[0][7] //in[0][8]
  vau.mul.f16 v12 v0 v8 || lsu0.swzv8 [00000000] 
	//v12 = 1/16 *   
  //in[1][1]// in[1][2] //in[1][3] //in[1][4] //in[1][5] //in[1][6] //in[1][7] //in[1][8]	
  vau.add.f16 v13 v7 v9   || cmu.vilv.x16 v23 v21 v17 v16 
  vau.add.f16 v14 v5 v10  || lsu0.stxvi v23 i17 i14
  lsu1.ldxvi v2 i1 i15
  vau.add.f16 v15 v11 v12  || lsu0.ldxvi v4 i2 i15
  vau.add.f16 v16 v13 v14  || lsu1.stxvi v21 i5 i14  
  ___endScl2xBilinHV_Fp16_075:
	

  
  bru.jmp i30
	nop 6
.nowarnend
.size mvispScale2xBilinHV075_Fp16ToFp16_asm,.-mvispScale2xBilinHV075_Fp16ToFp16_asm
.end
	
