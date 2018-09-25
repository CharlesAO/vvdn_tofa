///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Combine decimation demosaic kernel function call for unitary test
///

#include "combDecimDemosaicAwbGains.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_FRAME_WIDTH (4208)
#define KERNEL_SIZE 4
#define NR_PLANES 3

unsigned short  input[KERNEL_SIZE][TEST_FRAME_WIDTH] __attribute__ ((aligned (16))) = {
    {0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100},
    {0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100},
    {0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100},
    {0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100},
};

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
unsigned char __attribute__((section(".kept.data"))) output[NR_PLANES][TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

unsigned int width __attribute__ ((aligned (16)));
unsigned short  __attribute__ ((aligned (16))) gains[NR_PLANES]  = {
    0x0100, 0x0100, 0x0100
};

unsigned short* inLine[KERNEL_SIZE];
unsigned short** inLines;

unsigned char* outLine[NR_PLANES];
unsigned char** outLines;

int main( void )
{
    inLine[0] = (unsigned short*)(&input[0][0]);
    inLine[1] = (unsigned short*)(&input[1][0]);
    inLine[2] = (unsigned short*)(&input[2][0]);
    inLine[3] = (unsigned short*)(&input[3][0]);
    inLines = (unsigned short**)inLine;
    
    outLine[0] = (unsigned char*)(&output[0][0]);
    outLine[1] = (unsigned char*)(&output[1][0]);
    outLine[2] = (unsigned char*)(&output[2][0]);
	outLines = (unsigned char**)outLine;
    
	#ifdef UNIT_TEST_USE_C_VERSION
	mvispCombDecimDemosaicAwbGains(outLines, inLines, gains, width);
	#else
	mvispCombDecimDemosaicAwbGains_asm(outLines, inLines, gains, width);
	#endif
	
	SHAVE_HALT; 
    return 0;
}
