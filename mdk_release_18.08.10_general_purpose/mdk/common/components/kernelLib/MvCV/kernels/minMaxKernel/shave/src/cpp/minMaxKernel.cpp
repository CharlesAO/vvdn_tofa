#include "minMaxKernel.h"

void mvcvMinMaxKernel(u8** in, u32 width, u32 height, u8* minVal, u8* maxVal, u8* maskAddr)
{
    u8* in_1;
    in_1 = in[0];
    u8 minV = 0xFF;
    u8 maxV = 0x00;
    u32 j;
    height+=1;
    for(j = 0; j < width; j++)
    {
        if((maskAddr[j]) != 0)
        {
            if (in_1[j] < minV)
            {
                minV = in_1[j];
            }

            if (in_1[j] > maxV)
            {
                maxV = in_1[j];
            }
        }
    }

    if (minVal != NULL)
    {
        *minVal = minV;
    }
    if (maxVal != NULL)
    {
        *maxVal = maxV;
    }
    return;
}
