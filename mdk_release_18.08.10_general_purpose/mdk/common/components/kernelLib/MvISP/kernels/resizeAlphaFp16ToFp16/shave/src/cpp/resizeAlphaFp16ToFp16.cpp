#include "resizeAlphaFp16ToFp16.h"

void mvispResizeAlphaFp16ToFp16(half** alpha, half* output, u32 width)
{
    u32 i;

    for (i=0; i < width; i++)
    {
        output[i] = ( (alpha[0][i*2] + alpha[0][i*2+1]) +
                      (alpha[1][i*2] + alpha[1][i*2+1])) * .25;
    }
}
