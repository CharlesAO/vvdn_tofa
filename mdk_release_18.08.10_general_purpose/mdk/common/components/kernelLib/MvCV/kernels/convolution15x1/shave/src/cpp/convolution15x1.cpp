#include "convolution15x1.h"

void mvcvConvolution15x1(u8 **in, u8 **out, half conv[15], u32 inWidth)
{
    u32   i, y;
    float sum = 0.0;
    u8    *output = *out;
	
    for (i = 0; i < inWidth; i++)
    {
        sum = 0.0;

        for (y = 0; y < 15; y++)
        {
            sum += ((float)in[y][i]) * conv[y];
        }

        if (sum >= 255.0f)
            sum = 255.0f;
        if (sum <= 0.0f)
            sum = 0.0f;
        output[i] = (u8)(sum);
    }
    return;
}
