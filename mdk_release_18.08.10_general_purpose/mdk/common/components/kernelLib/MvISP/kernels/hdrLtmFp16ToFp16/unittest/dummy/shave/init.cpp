#include <hdrLtmFp16ToFp16.h>
#include <stdio.h>
#include <mv_types.h>
#include <moviVectorTypes.h>
#include <svuCommonShave.h>
    
#define TEST_FRAME_WIDTH 1920

half inLumaFusion[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
half inGaussUp[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
half output[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
u32 width;
HdrLtmParam* params;

float parameters[3];

int main( void )
{
    
    #ifdef UNIT_TEST_USE_C_VERSION
    mvispHdrLtmFp16ToFp16(inLumaFusion, inGaussUp, output, width, (HdrLtmParam*)parameters);
    #else                                                         
    mvispHdrLtmFp16ToFp16_asm(inLumaFusion, inGaussUp, output, width, (HdrLtmParam*)parameters);
    #endif
    
    SHAVE_HALT;
    return 0;
}
