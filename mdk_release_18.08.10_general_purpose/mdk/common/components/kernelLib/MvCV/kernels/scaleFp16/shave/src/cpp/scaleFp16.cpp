
#include "scaleFp16.h"

void mvcvScaleFp16(half** in, half** out, half scale, u32 width)
{
    u32 i;
    for (i = 0; i < width; i++)
    {
        out[0][i] = in[0][i] * scale;
    }
    return;
}
