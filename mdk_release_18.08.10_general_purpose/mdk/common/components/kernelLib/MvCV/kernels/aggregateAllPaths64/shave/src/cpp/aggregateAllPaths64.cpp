#include "aggregateAllPaths64.h"

void mvcvAggregateAllPaths64(u8* path0, u8* path1, u8* path2, u8* path3, u8* out, u32 width)
{
    u32 disparities = 64;

    for (u32 k = 0; k < width; k++)
    {
        for (u32 disp = 0; disp < disparities; disp++)
        {
            out[k * disparities + disp] = (path0[k * disparities + disp] + path1[k * disparities + disp] + path2[k * disparities + disp] + path3[k * disparities + disp]) / 4;
        }
    }
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvAggregateAllPaths64_opt(unsigned char * __restrict path0, unsigned char * __restrict path1,
                                             unsigned char * __restrict path2, unsigned char * __restrict path3,
                                             unsigned char * out, unsigned int width) {
  uchar8 * path0_c = (uchar8 *) path0;
  uchar8 * path1_c = (uchar8 *) path1;
  uchar8 * path2_c = (uchar8 *) path2;
  uchar8 * path3_c = (uchar8 *) path3;
  uchar8 * out_c   = (uchar8 *) out;

  #pragma unroll 2
  do {
    #pragma unroll (DISPARITIES/8)
    for (unsigned int disp = 0; disp < DISPARITIES/8; ++disp){
      *out_c++ = mvuConvert_uchar8((mvuConvert_ushort8(*path0_c++) + mvuConvert_ushort8(*path1_c++)+
                                    mvuConvert_ushort8(*path2_c++)  + mvuConvert_ushort8(*path3_c++)) >> 2);
    }
    --width;
  } while (width > 0);
}
#endif
