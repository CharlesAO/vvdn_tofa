#include "maxPool3x3s2hh.h"

void mvcvMaxPool3x3s2hh(half** src, half** dst, u32 outputWidth)
{
    half max;
    u32 tl = 0;
    u32 i;

    for (i = 0; i < outputWidth; i++)
    {
        tl = i*2;

        //[X][x][x]
        //[ ][ ][ ]
        //[ ][ ][ ]
        max = src[0][tl];
        max = (max < src[0][tl + 1]) ? src[0][tl + 1] : max;
        max = (max < src[0][tl + 2]) ? src[0][tl + 2] : max;

        //[ ][ ][ ]
        //[X][x][x]
        //[ ][ ][ ]
        max = (max < src[1][tl]) ? src[1][tl] : max;
        max = (max < src[1][tl + 1]) ? src[1][tl + 1] : max;
        max = (max < src[1][tl + 2]) ? src[1][tl  + 2] : max;

        //[ ][ ][ ]
        //[ ][ ][ ]
        //[X][x][x]
        max = (max < src[2][tl]) ? src[2][tl] : max;
        max = (max < src[2][tl + 1]) ? src[2][tl + 1] : max;
        max = (max < src[2][tl + 2]) ? src[2][tl  + 2] : max;

        dst[0][i] = max;
    }
}
