#include "accumulateFp16.h"
#include <stdlib.h>

void mvcvAccumulateFp16(half** dst, half** input, u32 width)

{
    u32 i;
    for (i = 0; i < width; i++)
    {
        dst[0][i] += input[0][i];
    }
    return;
}
