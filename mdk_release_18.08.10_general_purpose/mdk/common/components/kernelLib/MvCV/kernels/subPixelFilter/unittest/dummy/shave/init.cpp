///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
///

#include "subPixelFilter.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"
    
#define MAX_DISP 96
#define PADDING 16
#define TEST_FRAME_WIDTH (1920 + PADDING)
#define TEST_FRAME_WIDTH_DISP (TEST_FRAME_WIDTH * MAX_DISP)

unsigned char inputDisparityMap[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
unsigned char inputCostVolume[TEST_FRAME_WIDTH_DISP] __attribute__ ((aligned (16)));

unsigned int width __attribute__ ((aligned (16)));
unsigned char maxDisp __attribute__ ((aligned (16)));
unsigned char numFractionalBit __attribute__ ((aligned (16)));
unsigned char lutToUse[4096] __attribute__ ((aligned (16)));


/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
unsigned short __attribute__((section(".kept.data"))) outputDisparityMap[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

int main( void )
{

        //width = 16;
    //maxDisp = 96;
    //numFractionalBit = 5;
    #ifdef UNIT_TEST_USE_C_VERSION
        mvcvSubPixelFilter(inputDisparityMap, inputCostVolume, outputDisparityMap, width, maxDisp, numFractionalBit, lutToUse);
    #else
        mvcvSubPixelFilter_asm(inputDisparityMap, inputCostVolume, outputDisparityMap, width, maxDisp, numFractionalBit, lutToUse);
    #endif
    
    SHAVE_HALT; 
    return 0;
}
