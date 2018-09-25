///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
///

#include "computeCombinedCost64.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_LINE_WIDTH 320
#define DISPARITIES 64 

u8 __attribute__ ((aligned (16))) adCost[TEST_LINE_WIDTH * DISPARITIES] ;

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) disparityCost_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) disparityCost[TEST_LINE_WIDTH * DISPARITIES] __attribute__ ((aligned (16))) ;
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) disparityCost_u32postPad[4] __attribute__ ((aligned (16)));



u32 __attribute__ ((aligned (16))) width;
u32 __attribute__ ((aligned (16))) alpha;
u32 __attribute__ ((aligned (16))) beta;
u32 __attribute__ ((aligned (16))) normFactor;

int main( void )
{
	//printf("%x %x \n", in1,out);
	#ifdef UNIT_TEST_USE_C_VERSION
		mvcvComputeCombinedCost64(disparityCost, adCost, alpha, beta, width, normFactor);
	#else
	#ifdef MOVICOMPILE_OPTIMIZED
		mvcvComputeCombinedCost64_opt(disparityCost, adCost, alpha, beta, width, normFactor);
		#else
		mvcvComputeCombinedCost64_asm(disparityCost, adCost, alpha, beta, width, normFactor);
		#endif
	#endif
	SHAVE_HALT; 
    return 0;
}
