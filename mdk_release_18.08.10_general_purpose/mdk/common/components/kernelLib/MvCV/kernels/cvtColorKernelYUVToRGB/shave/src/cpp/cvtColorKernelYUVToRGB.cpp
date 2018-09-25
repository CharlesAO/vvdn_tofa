#include "cvtColorKernelYUVToRGB.h"

void mvcvCvtColorKernelYUVToRGB(u8* yIn, u8* uIn, u8* vIn, u8* out, u32 width)
{
    u32 i, j;
    u32 uv_idx = 0;
    u32 out_idx_1 = 0;
    int y[4], u, v, r, g, b;

    i = 0;
    while(i < width)
    {
        y[0] = yIn[i];
        y[1] = yIn[i + 1];

        u = uIn[uv_idx] - 128;
        v = vIn[uv_idx] - 128;
        uv_idx++;

        for(j = 0; j < 2; j++)
        {
            r = y[j] + (int)(1.402f*v);
            g = y[j] - (int)(0.344f*u +0.714f*v);
            b = y[j] + (int)(1.772f*u);

            r = r>255? 255 : r<0 ? 0 : r;
            g = g>255? 255 : g<0 ? 0 : g;
            b = b>255? 255 : b<0 ? 0 : b;

                out[out_idx_1++] = (u8)r;
                out[out_idx_1++] = (u8)g;
                out[out_idx_1++] = (u8)b;

        }
        i += 2;
    }
}
