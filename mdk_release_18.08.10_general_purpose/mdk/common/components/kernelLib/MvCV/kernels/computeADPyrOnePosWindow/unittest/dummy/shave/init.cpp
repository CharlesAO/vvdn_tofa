///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
///

#include "computeADPyrOnePosWindow.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define TEST_LINE_WIDTH 1920
#define PADDING 16 
#define DISPARITIES 5 
#define OFFSET PADDING/2 

u8 __attribute__ ((aligned (16))) input1[(TEST_LINE_WIDTH*DISPARITIES) + PADDING] ;
u8 __attribute__ ((aligned (16))) input2[(TEST_LINE_WIDTH*DISPARITIES) + PADDING] ;

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[(TEST_LINE_WIDTH*DISPARITIES) + PADDING] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));


u8 __attribute__ ((aligned (16))) *out;
u32 width;

int main( void )
{
    out = (u8 *)output + OFFSET;
    
    #ifdef UNIT_TEST_USE_C_VERSION
        mvcvComputeADPyrOnePosWindow((unsigned char *)(&input1[OFFSET]), (unsigned char *)(&input2[OFFSET]), out, width);
    #else
        mvcvComputeADPyrOnePosWindow_asm((unsigned char *)(&input1[OFFSET]), (unsigned char *)(&input2[OFFSET]), out, width);
    #endif
    
    SHAVE_HALT; 
    return 0;
}
