///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.version 00.70.00

.data .data.Convolution1x7
.align 16
 .Lconvolution1x7_clampVal:
        .float16 255.0

.code .text.Convolution1x7_asm
.align 16

//void Convolution1x7_asm(u8 **in(i18), u8 **out(i17), half* filter(i16), u32 inWidth(i15))
//                  
    .type Convolution1x7_step1,@function
Convolution1x7_step1:
	LSU0.LDi.32 i0 	i18 // load the address of "in" pointer as it is a double pointer but it is treated in the .c code as a simple line
	nop 6
	LSU0.LDi.32 i17 i17	//  load the address of "out" pointer"
	nop 6
	LSU0.LDIL i7 .Lconvolution1x7_clampVal //load in the irf7 register the address of the clamp, that will be used for final results (u8 domain 0 - 255)
	LSU0.LDIH i7 .Lconvolution1x7_clampVal //load in the irf7 register the address of the clamp, that will be used for final results (u8 domain 0 - 255)
	LSU0.LDIL i4 .Lconvolution1x7_loop // load the label address that will used, for the main loop
	LSU0.LDIH i4 .Lconvolution1x7_loop // load the label address that will used, for the main loop
	iau.shr.u32 i15 i15 3 //dividing the actual width parameter to 8 as we will process 8 elements at a time
	LSU0.LDO.64.L v3 i16 0x00 //// load the values that resemble the coeficients used for the convolution - lower part
	nop 6
	LSU1.LDO.64.H v3 i16 0x08 //// load the values that resemble the coeficients used for the convolution - higher part
	nop 6
	LSU0.LD.16R v23 i7 // load with repet the clamp value used at the end
	nop 6
	IAU.SUB i0 i0 8	 // go back 8 bytes from the input address, as we want to make aligned loads
	LSU1.LDi.128.u8.f16 v0 i0	 // load the -8 elements
	nop 6
	LSU1.LDi.128.u8.f16 v1 i0	// load the elements from offset 0
	nop 6

bru.rpl i4 i15
	LSU1.LDi.128.u8.f16 v2 i0 // load the next 8 elements, offset 8, for multiple operations reasons
	nop 6
	CMU.ALIGNVEC v22 v0 v1 10 // prepare elements in vrf registers to be able to compute 8 elements at a time
	CMU.ALIGNVEC v21 v0 v1 12 // cmu alignvec is used to align elements from two vectors if we want to end up having in one vector
	CMU.ALIGNVEC v20 v0 v1 14 // some elements from the first vrf register and some of the elements of the second one
	CMU.ALIGNVEC v19 v1 v2 2  // the number that is present in the instruction indicates the offset that will be considered from the 
	CMU.ALIGNVEC v18 v1 v2 4  // first vrf register
	CMU.ALIGNVEC v17 v1 v2 6	

	VAU.MACPZ.f16 v3 v22	|| LSU0.SWZV8 [00000000] //VAU.MAC instruction is used when we want to multiply and accumulate in the same
	VAU.MACP.f16  v3 v21 	|| LSU0.SWZV8 [11111111] // instruction// usualy when we use VAU.MAC, we first multiply and accumulate at 0 	 
	VAU.MACP.f16  v3 v20 	|| LSU0.SWZV8 [22222222] // and continue our desired operations untill 	we want to write out the result
	VAU.MACP.f16  v3 v1 	|| LSU0.SWZV8 [33333333] //VAU.MACPW - is used for writing out in a vrf register the result of the	
	VAU.MACP.f16  v3 v19 	|| LSU0.SWZV8 [44444444] // VAU.MAC operations	
	VAU.MACP.f16  v3 v18 	|| LSU0.SWZV8 [55555555] 
	VAU.MACPW.f16 v4 v3 v17 || LSU0.SWZV8 [66666666]
	nop 9
	CMU.CPVV v0 v1 // a simple copy from vrf to vrf register, this is done for performance reasons
	CMU.CPVV v1 v2 // in order to not have 3 load operations at the beggining of the loop, we will have two copy operations
    CMU.CLAMP0.f16 v4 v4 v23// because we will have the output in the u8 domain we want to clamp the results to 0.255 range
    nop
    LSU0.STi.128.f16.u8 v4 i17 // store the actual values at the output address
    nop
.Lconvolution1x7_loop: // label used for bru.rpl instruction
nop 7                  // it has a delay slot of 7  
	
    BRU.JMP i30 // mandatory instruction that will return after finishing up the asm code
    nop 6
    .size Convolution1x7_step1, .-Convolution1x7_step1
.end
