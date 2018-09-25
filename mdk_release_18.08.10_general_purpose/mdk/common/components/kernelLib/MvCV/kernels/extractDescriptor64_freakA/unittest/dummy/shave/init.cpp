///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     boxfilter3x3 kernel function call for unitary test
///

#include <extractDescriptor64_freakA.h>
#include <mv_types.h>
#include <svuCommonShave.h>

u16 input[64] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[64] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));



int main( void )
{
	
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvExtractDescriptor64_freakA(input, output);
	#else
	mvcvExtractDescriptor64_freakA_asm(input, output);
	#endif

    SHAVE_HALT; 
	return 0;
}
