#include "convolution1x9.h"

void mvcvConvolution1x9(u8 **in, u8 **out, half conv[9], u32 inWidth)
{
    u32 i;
	s8 y;
    half sum = 0.0;
    u8    *input  = *in;
    u8    *output = *out;
	

    for (i = 0; i < inWidth; i++)
    {
        sum = 0.0;

        for (y = 0; y < 9; y++)
        {
            sum += ((half)input[y - 4]) * conv[y];
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
