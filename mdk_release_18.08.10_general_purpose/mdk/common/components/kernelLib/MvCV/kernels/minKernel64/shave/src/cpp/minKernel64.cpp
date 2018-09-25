#include "minKernel64.h"

void mvcvMinKernel64(u8* in, u8* minVal)
{
    u32 width = 64;
	u8 minV = 0xFF;
    u32 j;

    for(j = 0; j < width; j++)
    {
        if (in[j] < minV)
        {
            minV = in[j];
        }
    }

    if (minVal != NULL)
    {
        *minVal = minV;
    }
    return;
}
