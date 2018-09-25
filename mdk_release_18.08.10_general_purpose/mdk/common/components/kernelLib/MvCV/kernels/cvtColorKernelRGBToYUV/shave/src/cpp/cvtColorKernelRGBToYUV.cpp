#include <math.h>
#include "cvtColorKernelRGBToYUV.h"

void mvcvCvtColorKernelRGBToYUV(u8* in, u8* yOut, u8* uOut, u8* vOut, u32 width)
{
    u32 i;
    u32 r, g, b;
    int y, u, v;
    u32 uv_idx = 0;

    for(i = 0; i < width * 2; i+= 2)
    {
        r = in[i * 3];
        g = in[i * 3 + 1];
        b = in[i * 3 + 2];

        y = ceil(0.299f * r + 0.587f * g + 0.114f * b);
        yOut[i] = y > 255 ? 255 : (u8)y;

        if(i >= width)
        {
        	u = (int)(((float)b - y) * 0.564f) + 128;
        	v = (int)(((float)r - y) * 0.713f) + 128;
        	uOut[uv_idx] = (u8)u;
        	vOut[uv_idx] = (u8)v;
        	uv_idx++;
        }

        r = in[(i + 1) * 3];
        g = in[(i + 1) * 3 + 1];
        b = in[(i + 1)* 3 + 2];

        y = ceil(0.299f * r + 0.587f * g + 0.114f * b);
        yOut[i + 1] = y > 255 ? 255 : (u8)y;
    }
}
