#include "convolution5x1Fp16ToFp16.h"

void mvcvConvolution5x1Fp16ToFp16(half **in, half **out, half *conv, u32 inWidth)
{
    u32   i, y;
    half sum;
    half *output = *out;
	half *lines[5];
	
	//Initialize lines pointers
	lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];
    lines[3] = in[3];
    lines[4] = in[4];
	
    for (i = 0; i < inWidth; i++)
    {
        sum = 0.0;

        for (y = 0; y < 5; y++)
        {
			 // convert value from u8 to u8f and after to fp16
			half val = (half)((float)lines[y][i]);
			sum += val * conv[y];
        }
	
        output[i] = sum;
    }
    return;
}
