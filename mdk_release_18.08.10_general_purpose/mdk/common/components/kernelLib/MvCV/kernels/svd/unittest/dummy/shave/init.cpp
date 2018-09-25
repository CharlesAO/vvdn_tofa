///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
///

#include "svd.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"


#define TEST_RANK 3

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) inputA_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
fp32 __attribute__((section(".kept.data"))) input_A[TEST_RANK][TEST_RANK] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) inputA_u32postPad[4] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) inputV_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
fp32 __attribute__((section(".kept.data"))) input_V[TEST_RANK][TEST_RANK] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) inputV_u32postPad[4] __attribute__ ((aligned (16)));

fp32 *inA[TEST_RANK];
fp32 *inV[TEST_RANK];

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) inS_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
fp32 __attribute__((section(".kept.data"))) inS[TEST_RANK] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) inS_u32postPad[4] __attribute__ ((aligned (16)));

fp32 **A;
fp32 **V;
fp32 *S;

int rank;

int main( void )
{  
    int i;
	for(i = 0; i < TEST_RANK; i++)
	{
    inA[i] = (fp32*) input_A[i];
	inV[i] = (fp32*) input_V[i];
	}
 
	#ifdef UNIT_TEST_USE_C_VERSION
		 mvcvSVD((fp32**) inA,(fp32*) inS,(fp32**)  inV, rank);
	 #else
		 mvcvSVD_asm((fp32**) inA,(fp32*) inS,(fp32**)  inV, rank);
	#endif

	
	SHAVE_HALT; 
    return 0;
}
