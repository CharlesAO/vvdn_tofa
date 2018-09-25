///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
///

#include "computeCombinedCost3.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define DISPARITIES 3
#define TEST_FRAME_WIDTH 1920 

unsigned char __attribute__ ((aligned (16))) adCost[TEST_FRAME_WIDTH * DISPARITIES];

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) disparityCost_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
unsigned char  __attribute__((section(".kept.data"))) disparityCost[TEST_FRAME_WIDTH * DISPARITIES] __attribute__ ((aligned (16))) ;
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) disparityCost_u32postPad[4] __attribute__ ((aligned (16)));


unsigned int alpha;
unsigned int beta;
unsigned int width;
unsigned int normFactor;


int main( void )
{
   
#ifdef UNIT_TEST_USE_C_VERSION
	mvcvComputeCombinedCost3(disparityCost, adCost, alpha, beta, width, normFactor);
#else
	mvcvComputeCombinedCost3_asm(disparityCost, adCost, alpha, beta, width, normFactor);
#endif
	
	SHAVE_HALT; 
    return 0;
}
