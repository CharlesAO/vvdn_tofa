#include "cvtColorKernelChromaYUV444ToNV12.h"

void mvcvCvtColorKernelChromaYUV444ToNV12(u8* inU[2], u8* inV[2], u8* outUV, u32 width)
{
	u32 i;

	for(i=0; i < width/2*2; i+=2)
	{
		*outUV++ = (u8)((((inU[0][i] 	+ inU[1][i] 	+ 1) >> 1) + \
				         ((inU[0][i+1] 	+ inU[1][i+1] 	+ 1) >> 1) + \
						 1) >> 1);
		*outUV++ = (u8)((((inV[0][i] 	+ inV[1][i] 	+ 1) >> 1) + \
				         ((inV[0][i+1] 	+ inV[1][i+1] 	+ 1) >> 1) + \
						 1) >> 1);
	}
}
