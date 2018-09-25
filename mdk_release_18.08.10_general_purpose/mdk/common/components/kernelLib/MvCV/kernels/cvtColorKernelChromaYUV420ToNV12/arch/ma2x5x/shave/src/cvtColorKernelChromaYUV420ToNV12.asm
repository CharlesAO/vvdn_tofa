///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief   Format conversion chroma YUV420 to NV21
///

.type mvcvCvtColorKernelChromaYUV420ToNV12_asm,@function
.version 00.51.05
//-------------------------------------------------------------------------------

.code .text.cvtColorKernelChromaYUV420ToNV12
.nowarn
//void mvcvCvtColorKernelChromaYUV420ToNV12_asm(u8* inU(i18), u8* inV(i17), u8* outUV(i16), u32 width(i15))
mvcvCvtColorKernelChromaYUV420ToNV12_asm:
	//load the main loop label and compute the number of iterations (each iteration processes 64 pixels)
	lsu0.ldil i1 .LmvcvCvtColorKernelChromaYUV420ToNV12_loop || lsu1.ldih i1 .LmvcvCvtColorKernelChromaYUV420ToNV12_loop || iau.shr.u32 i0, i15, 6

	//load the first 32 U, resp. V values
	lsu0.ldi.64.l v0, i18 || lsu1.ldi.64.l v1, i17 || cmu.cmz.i32 i0 //check whether the main loop needs to be executed at least once
	lsu0.ldi.64.h v0, i18 || lsu1.ldi.64.h v1, i17
	peu.pc1c EQ || bru.bra .LmvcvCvtColorKernelChromaYUV420ToNV12_compensate //otherwise go directly to the compensation processing
	lsu0.ldi.64.l v4, i18 || lsu1.ldi.64.l v5, i17
	lsu0.ldi.64.h v4, i18 || lsu1.ldi.64.h v5, i17

	//prepare compensation in this latency wait
	lsu0.ldil i2, 63
	iau.and i2, i15, i2 //get the remainder of integer division of the line width to 64
	iau.shr.u32 i2, i2, 3 //the compensation takes place in steps of 8 pixels, therefore the compensation counter is divided by 8
	nop //wait until 6 latency cycles are passed after bru.bra

	//main loop starts here
	//first interleave byte by byte the U and V values already loaded and trigger the loading of the next U/V values
	cmu.vilv.x8 v2, v3, v1, v0 || lsu0.ldi.64.l v0, i18 || lsu1.ldi.64.l v1, i17 || bru.rpl i1, i0
	.LmvcvCvtColorKernelChromaYUV420ToNV12_loop:
		lsu0.ldi.64.h v0, i18 || lsu1.ldi.64.h v1, i17
		lsu0.sto.64.l v2, i16 0 || lsu1.sto.64.h v2, i16 8
		lsu0.sto.64.l v3, i16 16 || lsu1.sto.64.h v3 i16 24 || iau.add i16 i16 32
		lsu0.ldi.64.l v4, i18 || lsu1.ldi.64.l v5, i17 || cmu.vilv.x8 v6, v7, v5, v4
		lsu0.ldi.64.h v4, i18 || lsu1.ldi.64.h v5, i17
		lsu0.sto.64.l v6, i16 0 || lsu1.sto.64.h v6, i16 8
		lsu0.sto.64.l v7, i16 16 || lsu1.sto.64.h v7 i16 24 || iau.add i16 i16 32

	.LmvcvCvtColorKernelChromaYUV420ToNV12_compensate:
	//check whether the compensation needs being performed
	iau.incs i2, 0
	peu.pc1i EQ || bru.bra .LmvcvCvtColorKernelChromaYUV420ToNV12_exit
	 //perform interleaving either on the U/V values loaded in the main loop last iteration OR
	 //on the first set of values in case the main loop needn't actually be executed OR
	 //on dummy data if no compensation at all is necessary
	cmu.vilv.x8 v2, v3, v1, v0
	nop //needed for v4 and v5 to become available for the following instruction when they are loaded in the main loop
	cmu.vilv.x8 v6, v7, v5, v4

	peu.pc1i GT || lsu0.sti.64.l v2, i16 || iau.incs i2, -1 //if needed store 8 UV bytes to output memory and decrement the compensation counter
	peu.pc1i GT || lsu0.sti.64.h v2, i16 || iau.incs i2, -1 //if still needed continue with storing the sequent 8 UV bytes
	peu.pc1i GT || lsu0.sti.64.l v3, i16 || iau.incs i2, -1 //until up to 56 bytes (another 8 would have caused an additional iteration in the main loop)
	peu.pc1i GT || lsu0.sti.64.h v3, i16 || iau.incs i2, -1
	peu.pc1i GT || lsu0.sti.64.l v6, i16 || iau.incs i2, -1
	peu.pc1i GT || lsu0.sti.64.h v6, i16 || iau.incs i2, -1
	peu.pc1i GT || lsu0.st.64.l v7, i16

	.LmvcvCvtColorKernelChromaYUV420ToNV12_exit:
	bru.jmp i30
	nop 6
.size mvcvCvtColorKernelChromaYUV420ToNV12_asm,.-mvcvCvtColorKernelChromaYUV420ToNV12_asm
.nowarnend
.end
