#include "integralImageSumFloatM2.h"

void mvcvIntegralimage_sum_f32_M2(float* out, u8* in, u32 runNr, float** prevPointer, u32 width)
{
    if (0 == runNr) 
    {
        unsigned int x;
        float onLineSum = 0;;
        for(x = 0; x < width; x++)
        {
            onLineSum += in[x];
            out[x] = onLineSum;
        }
        *prevPointer = out;
    }
    else
    {
        unsigned int x;
        float onLineSum = 0;;
        for(x = 0; x < width; x++)
        {
            onLineSum += in[x];
            out[x] = (*prevPointer)[x] + onLineSum;
        }
        *prevPointer = out;
    }
	return;
}
