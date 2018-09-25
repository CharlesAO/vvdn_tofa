///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     hammingDistance kernel function call for unitary test
///

#include "hammingDistance.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"


#define MAX_DESCRIPTOR_SIZE    128
#define MAX_ARRAY_SIZE         512

unsigned char __attribute__ ((aligned (16))) d1[MAX_DESCRIPTOR_SIZE];
unsigned char __attribute__ ((aligned (16))) d2[MAX_DESCRIPTOR_SIZE * MAX_ARRAY_SIZE];

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) distances_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
unsigned short __attribute__((section(".kept.data"))) distances[MAX_ARRAY_SIZE] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) distances_u32postPad[4] __attribute__ ((aligned (16)));

uint32_t distancespostPad;
int array_size;
int descriptor_size;

int main( void )
{
	#ifdef UNIT_TEST_USE_C_VERSION
		mvcvHammingDistance(d1, d2, array_size, descriptor_size, distances);
	#else
	#ifdef MOVICOMPILE_OPTIMIZED
		mvcvHammingDistance_opt(d1, d2, array_size, descriptor_size, distances);
		#else
		mvcvHammingDistance_asm(d1, d2, array_size, descriptor_size, distances);
		#endif
	#endif
	
	SHAVE_HALT; 
    return 0;
}
