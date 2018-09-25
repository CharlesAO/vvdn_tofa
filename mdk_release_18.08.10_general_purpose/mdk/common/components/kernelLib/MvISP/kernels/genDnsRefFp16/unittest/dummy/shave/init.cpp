#include <genDnsRefFp16.h>
#include <stdio.h>
#include <mv_types.h>
#include <moviVectorTypes.h>
#include <svuCommonShave.h>

// 8 padding values added to the maximum width
#define TEST_FRAME_WIDTH 1920 + 16
#define TEST_FRAME_HEIGHT 1

half            input[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t outputprePad;
/*output data marker*/
unsigned char   output[TEST_FRAME_WIDTH] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t outputpostPad;

unsigned char   lutGamma[256] __attribute__ ((aligned (16)));
unsigned char   lutDist[256] __attribute__ ((aligned (16)));
int     __attribute__ ((aligned (16)))        shift = 8 ;
unsigned int  __attribute__ ((aligned (16)))  yc = 0 ; 
int       __attribute__ ((aligned (16)))      x0 = 0 ;
u32      __attribute__ ((aligned (16)))       width = 40 ;
YDnsRefFp16Param specificParams;
YDnsRefFp16Param* param;



int main( void )
{
    specificParams.lutGamma = &lutGamma[0];
    specificParams.lutDist = &lutDist[0];
    specificParams.shift = shift;
    param = &specificParams;
    
	#ifdef UNIT_TEST_USE_C_VERSION
	mvispGgenDnsRefFp16(input, yc, x0, param, output, width);
	#else
	mvispGenDnsRefFp16_asm(input, yc, x0, param, output, width);
	#endif
	
	SHAVE_HALT;
	return 0;
}
