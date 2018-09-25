///
/// @file
/// @coPyrOnePosight All code coPyrOnePosight Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
///

#include "censusMatchingPyrOnePosWindow.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define PADDING 32
#define TEST_FRAME_WIDTH (1920 + PADDING)


unsigned int __attribute__ ((aligned (16))) input1[TEST_FRAME_WIDTH];
unsigned int __attribute__ ((aligned (16))) input2[TEST_FRAME_WIDTH];
/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
unsigned char  __attribute__((section(".kept.data"))) output[TEST_FRAME_WIDTH*5]__attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

unsigned char __attribute__ ((aligned (16))) predicted[TEST_FRAME_WIDTH];
unsigned int *in1;
unsigned int *in2;
unsigned char *out;
unsigned int width;// = 16;

int main( void )
{
    in1 = (unsigned int *)input1 + PADDING;
    in2 = (unsigned int *)input2 + PADDING;
    out = (unsigned char *)output +  PADDING/2;

    #ifdef UNIT_TEST_USE_C_VERSION
        mvcvCensusMatchingPyrOnePosWindow(in1,in2,predicted,out,width);
    #else
        mvcvCensusMatchingPyrOnePosWindow_asm(in1,in2,predicted,out,width);
    #endif
    
    SHAVE_HALT; 
    return 0;
}
