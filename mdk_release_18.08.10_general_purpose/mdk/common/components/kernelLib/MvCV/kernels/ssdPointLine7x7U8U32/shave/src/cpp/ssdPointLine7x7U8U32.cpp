#include "ssdPointLine7x7U8U32.h"
#include <stdio.h>

void mvcvSsdPointLine7x7U8U32(u8** in1, u8** in2, u32** out, u32 width)
{
    int x, y;
    unsigned int i;
    u8 *lines1[7], *lines2[7];
    int sum, diff;

	for(i = 0; i < 7; i++)
	{
		lines1[i] = in1[i];
		lines2[i] = in2[i];
	}
    for (i = 0; i < width; i++){
        sum = 0;
        for (x = 0; x < 7; x++)
        {
            for (y = 0; y < 7; y++)
            {
                diff = lines1[x][y - 3] - lines2[x][y - 3];
				diff = diff * diff;
                sum += diff;
            }         
            lines2[x]++;
        }
        out[0][i] = sum;
    }
    return;
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvSsdPointLine7x7U8U32_opt(unsigned char ** __restrict in1, unsigned char ** __restrict in2, unsigned int ** __restrict out, unsigned int width)
{
  short8 in1_8[7];
  short8 in1_8_offset[7];

  // Make a local copy of the in2 kernel
  for(unsigned int i = 0; i < 7; ++i) {
    uchar8 copy = *((uchar8 *) &in1[i][-3]);
    in1_8[i] = mvuConvert_short8(copy);
  }

  // Create a version of the kernel which is offset by 1 (for the i+1 iterations)
  for(unsigned int i = 0; i < 7; ++i)
    in1_8_offset[i] = in1_8[i].s70123456;

  for (unsigned int i = 0; i < width; i+=2){
    int sum1 = 0;
    int sum2 = 0;
    #pragma unroll 7
    for(unsigned int j = 0; j < 7; ++j) {
      uchar8 in2_8x8 = *((uchar8 *) &in2[j][i-3]);
      
      short8 in2_8x16 = mvuConvert_short8(in2_8x8);

      // Calculate for offset i
      short8 diff = in1_8[j] - in2_8x16;
      diff[7] = 0;

      int4 diff1 = mvuConvert_int4(diff.s0123);
      int4 diff2 = mvuConvert_int4(diff.s4567);

      diff1 *= diff1;
      diff2 *= diff2;

      sum1 += __builtin_shave_sau_sumx_i32_r(diff1);
      sum1 += __builtin_shave_sau_sumx_i32_r(diff2);

      // Calculate for offset i+1
      short8 diff_offset = in1_8_offset[j] - in2_8x16;
      diff_offset[0] = 0;

      int4 diff1_offset = mvuConvert_int4(diff_offset.s0123);
      int4 diff2_offset = mvuConvert_int4(diff_offset.s4567);

      diff1_offset *= diff1_offset;
      diff2_offset *= diff2_offset;

      sum2 += __builtin_shave_sau_sumx_i32_r(diff1_offset);
      sum2 += __builtin_shave_sau_sumx_i32_r(diff2_offset);
    }
    out[0][i] = sum1;
    out[0][i+1] = sum2;
  }
}
#endif //MOVICOMPILE_OPTIMIZED
