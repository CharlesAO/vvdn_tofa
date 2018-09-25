#include "addV2Fp16.h"
#include <stdlib.h>

void mvcvAddV2Fp16(half** dst, half** input, u32 width)

{
    u32 i;
    for (i = 0; i < width; i++)
    {
        dst[0][i] = input[0][i] + input[1][i];
    }
    return;
}
