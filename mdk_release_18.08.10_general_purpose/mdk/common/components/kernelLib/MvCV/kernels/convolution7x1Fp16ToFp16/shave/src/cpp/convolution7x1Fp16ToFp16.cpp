#include "convolution7x1Fp16ToFp16.h"

void mvcvConvolution7x1Fp16ToFp16(half **in, half *out, half *conv, u32 inWidth)
{
    u32   i, y;
    half sum;
	half *lines[7];
	
	//Initialize lines pointers
	lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];
    lines[3] = in[3];
    lines[4] = in[4];
    lines[5] = in[5];
    lines[6] = in[6];
	
    for (i = 0; i < inWidth; i++)
    {
        sum = 0.0;

        for (y = 0; y < 7; y++)
        {
			half val = (half)((float)lines[y][i]);
			sum += val * conv[y];
        }
		
        out[i] = sum;
    }
    return;
}
