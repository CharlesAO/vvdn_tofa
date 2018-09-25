#include "cvtColorRGBfp16ToLumaU8.h"

void mvcvCvtColorRGBfp16ToLumaU8(half* inRGB[3], u8* yOut, u32 width)
{
    u32 i;

    half *r   = inRGB[0];
    half *g   = inRGB[1];
    half *b   = inRGB[2];

    half y;

    for (i = 0; i < width; i++)
    {
        y = 0.299f * r[i] + 0.587f * g[i] + 0.114f * b[i];

        if (y >= (half)1.0f)
            y = (half)1.0f;
        if (y <= (half)0.0f)
            y = (half)0.0f;
    yOut[i] = (u8)((float)y * 255.0f);
    }
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvCvtColorRGBfp16ToLumaU8_opt(short float * __restrict inRGB[3], unsigned char * __restrict yOut, unsigned int width)
{
  half8 * __restrict r = (half8 *) inRGB[0];
  half8 * __restrict g = (half8 *) inRGB[1];
  half8 * __restrict b = (half8 *) inRGB[2];
  uchar8 * __restrict out = (uchar8 *) yOut;

  for (unsigned int i = 0; i < (width/8); i+=4) {
    half8 r0 = r[i], r1 = r[i+1], r2 = r[i+2], r3 = r[i+3];
    half8 g0 = g[i], g1 = g[i+1], g2 = g[i+2], g3 = g[i+3];
    half8 b0 = b[i], b1 = b[i+1], b2 = b[i+2], b3 = b[i+3];

    half8 y0 = 0.299h * r0 + 0.587h * g0 + 0.114h * b0;
    half8 y1 = 0.299h * r1 + 0.587h * g1 + 0.114h * b1;
    half8 y2 = 0.299h * r2 + 0.587h * g2 + 0.114h * b2;
    half8 y3 = 0.299h * r3 + 0.587h * g3 + 0.114h * b3;

    __builtin_shave_lsu_st128_f16_u8f_rr(y0, &out[i]);
    __builtin_shave_lsu_st128_f16_u8f_rr(y1, &out[i+1]);
    __builtin_shave_lsu_st128_f16_u8f_rr(y2, &out[i+2]);
    __builtin_shave_lsu_st128_f16_u8f_rr(y3, &out[i+3]);
  }
}
#endif
