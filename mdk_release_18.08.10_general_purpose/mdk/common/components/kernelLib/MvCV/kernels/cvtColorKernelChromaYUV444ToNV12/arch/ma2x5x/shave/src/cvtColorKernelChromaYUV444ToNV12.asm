///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief   Format conversion chroma YUV444 to NV21
///

.type mvcvCvtColorKernelChromaYUV444ToNV12_asm,@function
.version 00.51.05
//-------------------------------------------------------------------------------

.code .text.cvtColorKernelChromaYUV420ToNV12

//void mvcvCvtColorKernelChromaYUV444ToNV12(u8* inU[2](i18), u8* inV[2](i17), u8* outUV(i16), u32 width(i15))
mvcvCvtColorKernelChromaYUV444ToNV12_asm:
.nowarn
	//load the pointers to the U and V lines
	lsu0.ld.64.l v0 i18 || lsu1.ld.64.h v0 i17
	//load the main loop label and compute the number of iterations (each iteration processes 32 pixels)
	lsu0.ldil i4 .LmvcvCvtColorKernelChromaYUV444ToNV12_loop || lsu1.ldih i4 .LmvcvCvtColorKernelChromaYUV444ToNV12_loop || iau.shr.u32 i5 i15 5
	//prepare compensation in this latency wait
	lsu0.ldil i6, 31
	iau.and i6, i15, i6 //get the remainder of integer division of the line width to 32
	iau.shr.u32 i6, i6, 3 //the compensation takes place in steps of 8 pixels, therefore the compensation counter is divided by 8
	//check whether the main loop needs to be executed at least once
	cmu.cmz.i32 i5
	nop //wait until v0 becomes available after loading from memory

	//copy the line pointers to IRFs
	lsu0.cp i0 v0.0 || lsu1.cp i1 v0.1
	lsu0.cp i2 v0.2 || lsu1.cp i3 v0.3

	//load the first 16 U, resp. V values
	lsu0.ldi.64.l v0 i0 || lsu1.ldi.64.l v1 i1
	lsu0.ldi.64.h v0 i0 || lsu1.ldi.64.h v1 i1
	peu.pc1c EQ || bru.bra .LmvcvCvtColorKernelChromaYUV444ToNV12_compensate //in case main loop needn't be executed, go directly to the compensation processing
	lsu0.ldi.64.l v2 i2 || lsu1.ldi.64.l v3 i3
	lsu0.ldi.64.h v2 i2 || lsu1.ldi.64.h v3 i3

	//load the next 16 U, resp. V values
	lsu0.ldi.64.l v8 i0 || lsu1.ldi.64.l v9 i1
	lsu0.ldi.64.h v8 i0 || lsu1.ldi.64.h v9 i1
	lsu0.ldi.64.l v10 i2 || lsu1.ldi.64.l v11 i3
	lsu0.ldi.64.h v10 i2 || lsu1.ldi.64.h v11 i3

	//main loop starts here
	//start averaging the U, resp. V values for groups of 4 pixels for the already loaded values and trigger the loading of the next U/V values
	//the averaging goes on like this:
	//the U values in the first line are averaged with those in the second line
	//then the resulting values are averaged once again in pairs
	//therefore for each 4 original U values we get 1 averaged new U value
	//same for the V values
	//the resulting U and V values are then interlaced using an OR operation
	bru.rpl i4 i5 || lsu0.ldi.64.l v0 i0 || lsu1.ldi.64.l v1 i1 || vau.avg.x8 v4 v0 v1
		lsu0.ldi.64.h v0 i0 || lsu1.ldi.64.h v1 i1
		lsu0.ldi.64.l v2 i2 || lsu1.ldi.64.l v3 i3 || vau.avg.x8 v6 v2 v3 || cmu.vszm.byte v5 v4 [Z3Z1]
		lsu0.ldi.64.h v2 i2 || lsu1.ldi.64.h v3 i3
		vau.avg.x8 v4 v4 v5 || lsu0.swzmv4.byte [3210] [ZUZU] || cmu.vszm.byte v7 v6 [2Z0Z]
		vau.avg.x8 v6 v6 v7 || lsu0.swzmv4.byte [3210] [UZUZ]
		lsu0.ldi.64.l v8 i0 || lsu1.ldi.64.l v9 i1  || vau.avg.x8 v12 v8 v9
		lsu0.ldi.64.h v8 i0 || lsu1.ldi.64.h v9 i1 || vau.avg.x8 v13 v10 v11
	.LmvcvCvtColorKernelChromaYUV444ToNV12_loop:
		lsu0.ldi.64.l v10 i2 || lsu1.ldi.64.l v11 i3 || cmu.vszm.byte v5 v12 [Z3Z1]
		lsu0.ldi.64.h v10 i2 || lsu1.ldi.64.h v11 i3 || vau.or v4 v4 v6
		vau.avg.x8 v5 v12 v5 || lsu0.swzmv4.byte [3210] [ZUZU] || cmu.vszm.byte v7 v13 [2Z0Z]
		vau.avg.x8 v7 v13 v7 || lsu0.swzmv4.byte [3210] [UZUZ]
		lsu0.sto.64.l v4 i16 0 || lsu1.sto.64.h v4 i16 8 || iau.incs i16 16
		vau.or v5 v5 v7
		lsu0.sto.64.l v5 i16 0 || lsu1.sto.64.h v5 i16 8 || iau.incs i16 16

	.LmvcvCvtColorKernelChromaYUV444ToNV12_compensate:
	//check whether the compensation needs being performed
	iau.incs i6, 0
	peu.pc1i EQ || bru.bra .LmvcvCvtColorKernelChromaYUV444ToNV12_exit
	//perform averaging and interleaving either on the U/V values loaded in the main loop last iteration OR
	//on the first set of values in case the main loop needn't actually be executed OR
	//on dummy data if no compensation at all is necessary (in this case the following 6 instructions are executed redundantly due to the bru.bra above)
	vau.avg.x8 v4 v0 v1
	vau.avg.x8 v6 v2 v3
	vau.avg.x8 v12 v8 v9 || cmu.vszm.byte v5 v4 [Z3Z1]
	vau.avg.x8 v13 v10 v11 || cmu.vszm.byte v7 v6 [2Z0Z]
	vau.avg.x8 v4 v4 v5 || lsu0.swzmv4.byte [3210] [ZUZU] || cmu.vszm.byte v5 v12 [Z3Z1]
	vau.avg.x8 v6 v6 v7 || lsu0.swzmv4.byte [3210] [UZUZ] || cmu.vszm.byte v7 v13 [2Z0Z]
	vau.avg.x8 v5 v12 v5 || lsu0.swzmv4.byte [3210] [ZUZU]
	vau.avg.x8 v7 v13 v7 || lsu0.swzmv4.byte [3210] [UZUZ]
	nop
	vau.or v4 v4 v6
	vau.or v5 v5 v7

	peu.pc1i GT || lsu0.sti.64.l v4 i16 || iau.incs i6, -1
	peu.pc1i GT || lsu0.sti.64.h v4 i16 || iau.incs i6, -1
	peu.pc1i GT || lsu0.sti.64.l v5 i16

	.LmvcvCvtColorKernelChromaYUV444ToNV12_exit:
	bru.jmp i30
	nop 6
.nowarnend
.size mvcvCvtColorKernelChromaYUV444ToNV12_asm,.-mvcvCvtColorKernelChromaYUV444ToNV12_asm
.end
