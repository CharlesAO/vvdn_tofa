///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
///

#include "computeAD64.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define PADDING 64 
#define DISPARITIES 64 
#define TEST_LINE_WIDTH (1920 + PADDING)

u8 __attribute__ ((aligned (16))) input1[(TEST_LINE_WIDTH*DISPARITIES) + PADDING]; 
u8 __attribute__ ((aligned (16))) input2[(TEST_LINE_WIDTH*DISPARITIES) + PADDING]; 

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data"))) output[(TEST_LINE_WIDTH*DISPARITIES) + PADDING] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));



u8 *in1;
u8 *in2;
u8 *out;
u32 width;
u32 flag;

int main( void )
{    
    in1 = (u8*)input1;
    if(flag == 0)    in2 = (u8*)input2 + PADDING;
    else             in2 = (u8*)input2;
    out = (u8*)output;

#ifdef UNIT_TEST_USE_C_VERSION
    mvcvComputeAD64(in1, in2, out, width, flag);
#else
    mvcvComputeAD64_asm(in1, in2, out, width, flag);
#endif
    
    SHAVE_HALT; 
    return 0;
}
