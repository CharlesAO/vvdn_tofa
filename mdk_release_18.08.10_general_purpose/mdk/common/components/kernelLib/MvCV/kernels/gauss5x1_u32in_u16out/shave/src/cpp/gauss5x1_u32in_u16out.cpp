#include "gauss5x1_u32in_u16out.h"
#include "stdio.h"

void mvcvGauss5x1_u32in_u16out(u32 **in, u16 **out, u32 inWidth)
{
    u32 i;
    u32 sum = 0;
    u16 *output = *out;
    u32 **input = in;

    for (i = 0; i < inWidth; i++)
    {
        sum = input[0][i] + input[4][i];
        sum +=((u32)input[1][i] + (u32)input[3][i])<< 2;
        sum += (input[2][i] << 2) + (input[2][i] << 1) + 128;
        output[i] = (u16)(sum >> 8);
    }
    return;
}
