#include "convolution1x15.h"

void mvcvConvolution1x15(u8 **in, u8 **out, half conv[15], u32 inWidth)
{
    u32 i;
	s8 y;
    float sum = 0.0;
    u8    *input  = *in;
    u8    *output = *out;
	

    for (i = 0; i < inWidth; i++)
    {
        sum = 0.0;

        for (y = 0; y < 15; y++)
        {
            sum += ((float)input[y - 7]) * conv[y];
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
