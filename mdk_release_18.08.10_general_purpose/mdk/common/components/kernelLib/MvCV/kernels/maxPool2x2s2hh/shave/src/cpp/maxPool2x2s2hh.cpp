#include "maxPool2x2s2hh.h"

void mvcvMaxPool2x2s2hh(half** src, half** dst, u32 outputWidth)
{
    half max;
    u32 tl = 0;
    u32 i;

    for (i = 0; i < outputWidth; i++)
    {
        tl = i * 2;
        //[X][ ]
        //[ ][ ]
        max = src[0][tl];

        //[ ][X]
        //[ ][ ]
        max = (max < src[0][tl + 1]) ? src[0][tl + 1] : max;

        //[ ][ ]
        //[X][ ]
        max = (max < src[1][tl]) ? src[1][tl] : max;

        //[ ][ ]
        //[ ][X]
        max = (max < src[1][tl +1]) ? src[1][tl + 1] : max;
        dst[0][i] = max;
    }
}
