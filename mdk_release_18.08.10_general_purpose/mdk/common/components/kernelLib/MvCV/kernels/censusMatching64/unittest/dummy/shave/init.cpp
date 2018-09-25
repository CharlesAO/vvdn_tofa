///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
///

#include "censusMatching64.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"

#define PADDING 64
#define TEST_FRAME_WIDTH (1920 + PADDING)


unsigned int __attribute__ ((aligned (16))) input1[TEST_FRAME_WIDTH] ;
unsigned int __attribute__ ((aligned (16))) input2[TEST_FRAME_WIDTH] ;

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
unsigned char __attribute__((section(".kept.data")))  output[TEST_FRAME_WIDTH*64] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));



unsigned int *in1;
unsigned int *in2;
unsigned char *out;
unsigned int width;
unsigned int flag;

int main( void )
{
    if (flag == 0)
    {
        in1 = (unsigned int *)input1 + PADDING;
        in2 = (unsigned int *)input2 + PADDING;
    }
    else
    {
        in1 = (unsigned int *)input1;
        in2 = (unsigned int *)input2;
    }
    out = (unsigned char *)output +  PADDING/2;

    
    //printf("%x %x %x \n", in1,in2,out);
    #ifdef UNIT_TEST_USE_C_VERSION
        mvcvCensusMatching64(in1,in2,out,width,flag);
    #else
        mvcvCensusMatching64_asm(in1,in2,out,width,flag);
    #endif
    
    SHAVE_HALT; 
    return 0;
}
