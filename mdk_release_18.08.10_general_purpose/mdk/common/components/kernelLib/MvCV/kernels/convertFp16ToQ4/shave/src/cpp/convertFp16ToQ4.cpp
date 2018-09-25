#include "convertFp16ToQ4.h"
#include <stdlib.h>
#include <math.h>

void mvcvConvertFp16ToQ4(half* in, u16* out, u32 width)
{
    // Convert fp16 values to Q4 (fixed point with 4 fractional bits)
    for (u32 i = 0; i < width; i++)
    {
        out[i] = (unsigned short)((half)in[i] * (half)pow(2, 4) + (half)0.5);
    }
}
