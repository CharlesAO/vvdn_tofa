#include "convolution1x5Fp16ToFp16.h"
#include <stdio.h>

void mvcvConvolution1x5Fp16ToFp16(half **in, half **out, half *conv, u32 inWidth)
{
    s32 y;
	unsigned int i;
    half sum = 0.0;

    half *input  = *in;
    half *output = *out;

    for (i = 0; i < inWidth; i++)
    {
        sum = 0.0;
        for (y = 0; y < 5; y++)
        {
			 // convert value from u8 to u8f and after to fp16
            half val = input[y - 2];
			sum += val * conv[y];
        }
        input++;

        output[i] = sum;
    }
    return;
}
