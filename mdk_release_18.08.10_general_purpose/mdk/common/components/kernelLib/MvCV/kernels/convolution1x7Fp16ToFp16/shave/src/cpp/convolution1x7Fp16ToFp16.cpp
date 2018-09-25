#include "convolution1x7Fp16ToFp16.h"
#include <stdio.h>

void mvcvConvolution1x7Fp16ToFp16(half *in, half *out, half *conv, u32 inWidth)
{
    u32 i;
	s8 y;
    half sum = 0.0;

    for (i = 0; i < inWidth; i++)
    {
        sum = 0.0;
        for (y = 0; y < 7; y++)
        {
            half val = (half)((float)in[y - 3]);
			sum += val * conv[y];
        }
        in++;

        out[i] = sum;
    }
    return;
}
