#include "convolution1x5.h"

void mvcvConvolution1x5(u8 **in, u8 **out, half conv[5], u32 inWidth)
{
    u32   i;
	s8 y;
    float sum = 0.0;
    u8    *input  = *in;
    u8    *output = *out;
	
    for (i = 0; i < inWidth; i++)
    {
        sum = 0.0;

        for (y = 0; y < 5; y++)
        {
            sum += ((float)input[y - 2]) * conv[y];
        }
        input++;

        if (sum >= 255.0f)
            sum = 255.0f;
        if (sum <= 0.0f)
            sum = 0.0f;
        output[i] = (u8)(sum);
    }
    return;
}
